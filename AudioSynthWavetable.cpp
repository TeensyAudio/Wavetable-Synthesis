/* Audio Library for Teensy 3.X
 * Copyright (c) 2017, TeensyAudio PSU Team
 *
 * Development of this audio library was sponsored by PJRC.COM, LLC.
 * Please support PJRC's efforts to develop open source
 * software by purchasing Teensy or other PJRC products.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice, development funding notice, and this permission
 * notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "AudioSynthWavetable.h"
#include <dspinst.h>
#include <SerialFlash.h>

#define TIME_TEST_ON
//#define ENVELOPE_DEBUG
//#define STATE_DEBUG
//#define LFO_DEBUG
//#define LFO_OUTPUT_DEBUG

// Performance testing macro generally unrelated to the wavetable object, but was used to
// fine tune the performance of specific blocks of code in update(); usage is to specify a
// display interval in ms, then place the code block to be tracked *IN* the macro parens as
// the second argument
#ifdef TIME_TEST_ON
#define TIME_TEST(INTERVAL, CODE_BLOCK_TO_TEST) \
static float MICROS_AVG = 0.0; \
static int TEST_CUR_CNT = 0; \
static int TEST_LST_CNT = 0; \
static int NEXT_DISPLAY = 0; \
static int TEST_TIME_ACC = 0; \
int micros_start = micros(); \
{ CODE_BLOCK_TO_TEST } \
int micros_end = micros(); \
TEST_TIME_ACC += micros_end - micros_start; \
++TEST_CUR_CNT; \
if (NEXT_DISPLAY < micros_end) { \
	MICROS_AVG += (TEST_TIME_ACC - TEST_CUR_CNT * MICROS_AVG) / (TEST_LST_CNT + TEST_CUR_CNT); \
	NEXT_DISPLAY = micros_end + INTERVAL*1000; \
	TEST_LST_CNT += TEST_CUR_CNT; \
	TEST_TIME_ACC = TEST_CUR_CNT = 0; \
	Serial.printf("avg: %f, n: %i\n", MICROS_AVG, TEST_LST_CNT); \
}
#else
#define TIME_TEST(INTERVAL, CODE_BLOCK_TO_TEST) do { } while(0); \
CODE_BLOCK_TO_TEST
#endif

// Debug code to track state variables for the volume envelope
#ifdef ENVELOPE_DEBUG
#define DEBUG_PRINT_ENV(NAME) cli(); Serial.printf("%14s-- env_mult:%7.3f%% of UNITY_GAIN env_incr:%7.3f%% of UNITY_GAIN env_count:%i\n", #NAME, float(env_mult)*100/float(UNITY_GAIN), float(env_incr)*100/float(UNITY_GAIN), env_count); Serial.flush(); sei();
#else
#define DEBUG_PRINT_ENV(NAME) do { } while(0);
#endif


/**
 * @brief Stop playing waveform.
 *
 * Waveform does not immediately stop,
 * but fades out based on release time.
 *
 */
void AudioSynthWavetable::stop(void) {
	cli();
	switch(env_state) {
	case STATE_IDLE:
		break;
	case STATE_PLAY:
		env_state = STATE_IDLE;
		state_change = true;
		break;
	default:
		env_state = STATE_STOP;
		state_change = true;
		DEBUG_PRINT_ENV(STATE_STOP);
	}
	sei();
}

/**
 * @brief Play waveform at defined frequency, amplitude.
 *
 * @param freq Frequency of note to playback, value between 1.0 and half of AUDIO_SAMPLE_RATE_EXACT
 * @param amp Amplitude scaling of note, value between 0-127, with 127 being base volume
 */
void AudioSynthWavetable::playFrequency(float freq, int amp) {
	setState(freqToNote(freq), amp, freq);
}

/**
 * @brief Play sample at specified note, amplitude.
 *
 * @param note Midi note number to playback, value between 0-127
 * @param amp Amplitude scaling of playback, value between 0-127, with 127 being base volume
 */
void AudioSynthWavetable::playNote(int note, int amp) {
	setState(note, amp, noteToFreq(note));
}

/**
 * @brief Initializes object state variables, sets freq/amp, and chooses appropriate sample
 *
 * @param note Midi note number to play, value between 0-127
 * @param amp the amplitude level at which playback should occur
 * @param freq exact frequency of the note to be played played
 */
void AudioSynthWavetable::setState(int note, int amp, float freq) {
	cli();
	int i;
	env_state = STATE_IDLE;
	// note ranges calculated by sound font decoder
	for (i = 0; note > instrument->sample_note_ranges[i]; i++);
	current_sample = &instrument->samples[i];
	if (current_sample == NULL) {
		sei();
		return;
	}
	setFrequency(freq);
	tone_amp = signed_multiply_32x16b((INT32_MAX / 127), amp);
	tone_amp = signed_multiply_32x16b(uint32_t(current_sample->INITIAL_ATTENUATION_SCALAR) * 2, tone_amp);
	env_state = STATE_PLAY;
	state_change = true;
	DEBUG_PRINT_ENV(STATE_PLAY);
	sei();
}

/**
 * @brief Set various integer offsets to values that will produce intended frequencies
 * @details the main integer offset, tone_incr, is used to step through the current sample's 16-bit PCM audio sample.
 * Specifically, the tone_incr is the rate at which the interpolation code in update() steps through uint32_t space.
 * The remaining offset variables represent a minimum and maximum offset allowed for tone_incr, which allows for low-frequency
 * variation in playback frequency (aka vibrato). Further details on implementation in update() and in sample_data.h.
 *
 * @param freq frequency of the generated output (between 0 and the board-specific sample rate)
 */
void AudioSynthWavetable::setFrequency(float freq) {
	vib_tone_incr = mod_tone_incr = freq * current_sample->PER_HERTZ_PHASE_INCREMENT;
	vib_phase = mod_phase = LFO_INITIAL_PHASE;
}

/**
 * @brief Called by the AudioStream library to fill the audio output buffer.
 * The major parts are the interpoalation stage, and the volume envelope stage.
 * Further details on implementation included inline.
 *
 */
void AudioSynthWavetable::update(void) {
	uint32_t* p, *end;
	uint32_t index, phase_scale;
	int32_t s1, s2;
	uint32_t tmp1, tmp2;

	cli();
	if (env_state == STATE_IDLE) {
		sei();
		return;
	}

	state_change = false;
	const sample_data* s = (const sample_data*)current_sample;

	if (env_state == STATE_PLAY) {
		this->vib_delay_count = s->VIBRATO_DELAY;
		this->mod_delay_count = s->MODULATION_DELAY;

		this->tone_phase = 0;

		this->vib_freq_mult = 0;
		this->mod_freq_mult = 0;
		this->mod_amp_mult = 0;
	}

	uint32_t tone_phase = this->tone_phase;
	uint32_t tone_amp = this->tone_amp;

	envelopeStateEnum env_state = this->env_state;
	int32_t env_count = this->env_count;
	int32_t env_mult = this->env_mult;
	int32_t env_incr = this->env_incr;

	int32_t vib_delay_count = this->vib_delay_count;
	int32_t vib_phase = this->vib_phase;
	uint32_t vib_tone_incr = this->vib_tone_incr;
	int32_t vib_freq_mult = this->vib_freq_mult;

	int32_t mod_delay_count = this->mod_delay_count;
	int32_t mod_phase = this->mod_phase;
	uint32_t mod_tone_incr = this->mod_tone_incr;
	int32_t mod_freq_mult = this->mod_freq_mult;
	int32_t mod_amp_mult = this->mod_amp_mult;
	sei();

#ifdef STATE_DEBUG
	static envelopeStateEnum previous_state = STATE_IDLE;
	if (env_state != previous_state) {
		Serial.printf(
			"\n"
			"tone_phase = %u,"
			"tone_incr = %u,"
			"tone_amp = %u\n"
			"env_count = %i,"
			"env_mult = %i,"
			"env_incr = %i\n"
			"vib_delay_count = %u,"
			"vib_phase = %u\n"
			"mod_delay_count = %u,"
			"mod_phase = %u\n",
			tone_phase,
			(vib_tone_incr + mod_tone_incr) / 2,
			tone_amp,
			env_count,
			env_mult,
			env_incr,
			vib_delay_count,
			vib_phase,
			mod_delay_count,
			mod_phase
		);
		previous_state = env_state;
	}
#endif

	audio_block_t* block;
	block = allocate();
	if (block == NULL) return;

	p = (uint32_t*)block->data;
	end = p + AUDIO_BLOCK_SAMPLES / 2;

	TIME_TEST(5000, 
	uint32_t vib_incr = vib_delay_count > 0 ? 0 : s->VIBRATO_INCREMENT;
	uint32_t mod_incr = mod_delay_count > 0 ? 0 : s->MODULATION_INCREMENT;

	if (!vib_delay_count) {
		vib_freq_mult = s->VIBRATO_FREQ_MULT;
	}
	if (!mod_delay_count) {
		mod_freq_mult = s->MODULATION_FREQ_MULT;
		mod_amp_mult = s->MODULATION_AMP_MULT;
	}
	
	vib_delay_count--;
	mod_delay_count--;

	// lfo freq variance
	while (p < end) {
		vib_tone_incr = multiply_accumulate_32x32_rshift32_rounded(vib_tone_incr, vib_tone_incr, vib_freq_mult);
		mod_tone_incr = multiply_accumulate_32x32_rshift32_rounded(mod_tone_incr, mod_tone_incr, mod_freq_mult);
		//tone_amp = multiply_accumulate_32x32_rshift32_rounded(tone_amp, tone_amp, mod_amp_mult);
		*p++ = (vib_tone_incr + mod_tone_incr) / 2;
		*p++ = tone_amp;

		vib_phase += vib_incr;
		if (vib_phase < 0) {
			vib_phase += INT32_MAX;
			vib_freq_mult = -vib_freq_mult;
		}

		mod_phase += mod_incr;
		if (mod_phase < 0) {
			mod_phase += INT32_MAX;
			mod_freq_mult = -mod_freq_mult;
			mod_amp_mult = -mod_amp_mult;
		}
	}
	); //TIME_TEST end

#ifdef LFO_OUTPUT_DEBUG
	p = (uint32_t*)block->data;

	uint32_t max_incr = 0;
	uint32_t min_incr = UINT32_MAX;
	float avg_incr = 0.0;
	uint32_t max_amp = 0;
	uint32_t min_amp = UINT32_MAX;
	float avg_amp = 0.0;

	for (int i = 0; i < AUDIO_BLOCK_SAMPLES / 2; i++) {
		max_incr = p[i] > max_incr ? p[i] : max_incr;
		min_incr = p[i] < min_incr ? p[i] : min_incr;
		avg_incr += p[i];
		i++;
		max_amp = p[i] > max_amp ? p[i] : max_amp;
		min_amp = p[i] < min_amp ? p[i] : min_amp;
		avg_amp += p[i];
	}

	avg_incr /= 32.0;
	avg_amp /= 32.0;

	Serial.printf("incr: {%u, %u, %f}, amp: {%u, %u, %f}\n", max_incr, min_incr, avg_incr, max_amp, min_amp, avg_amp);
#endif

#ifdef LFO_DEBUG
	Serial.printf(
		"\n"
		"tone_phase = %u\n"
		"tone_amp = %hu\n"
		"\n"
		"vib_delay_count = %i\n"
		"mod_delay_count = %i\n"
		"vib_phase = %i\n"
		"mod_phase = %i\n"
		"vib_tone_incr = %u\n"
		"mod_tone_incr = %u\n"
		"vib_freq_mult = %i\n"
		"mod_freq_mult = %i\n"
		"mod_amp_mult = %i\n"
		"vib_incr = %u\n"
		"mod_incr = %u\n"
		"\n",
		tone_phase,
		tone_amp,
		vib_delay_count,
		mod_delay_count,
		vib_phase,
		mod_phase,
		vib_tone_incr,
		mod_tone_incr,
		vib_freq_mult,
		mod_freq_mult,
		mod_amp_mult,
		vib_incr,
		mod_incr
	);
#endif

	update_interpolation(block, s, tone_phase);

	p = (uint32_t *)block->data;
	end = p + AUDIO_BLOCK_SAMPLES / 2;
		
	while (p < end) {
		if (env_count <= 0) switch (env_state) {
		case STATE_PLAY:
			env_state = STATE_DELAY;
			env_count = s->DELAY_COUNT;
			env_mult = 0;
			env_incr = 0;
			DEBUG_PRINT_ENV(STATE_DELAY);
			continue;
		case STATE_DELAY:
			env_state = STATE_ATTACK;
			env_count = s->ATTACK_COUNT;
			env_mult = 0;
			env_incr = UNITY_GAIN / env_count;
			DEBUG_PRINT_ENV(STATE_ATTACK);
			continue;
		case STATE_ATTACK:
			env_state = STATE_HOLD;
			env_count = s->HOLD_COUNT;
			env_mult = UNITY_GAIN;
			env_incr = 0;
			DEBUG_PRINT_ENV(STATE_HOLD);
			continue;
		case STATE_HOLD:
			env_state = STATE_DECAY;
			env_count = s->DECAY_COUNT;
			env_mult = UNITY_GAIN;
			env_incr = s->SUSTAIN_MULT;
			DEBUG_PRINT_ENV(STATE_DECAY);
			continue;
		case STATE_DECAY:
			env_state = STATE_SUSTAIN;
		case STATE_SUSTAIN:
			env_count = INT32_MAX;
			env_incr = 0;
			DEBUG_PRINT_ENV(STATE_SUSTAIN);
			continue;
		case STATE_STOP:
			env_state = STATE_RELEASE;
			env_count = s->RELEASE_COUNT;
			//env_incr = -(env_mult / env_count);
			env_incr = pow(env_mult * (1.0/MINIMUM_VOL_MULT), s->RELEASE_COUNT_NEG_INV) * UNITY_GAIN;
			DEBUG_PRINT_ENV(STATE_RELEASE)
			continue;
		case STATE_RELEASE:
			env_state = STATE_IDLE;
		case STATE_IDLE:
			for (; p < end; ++p) *p = 0;
			DEBUG_PRINT_ENV(STATE_IDLE);
			continue;
			p = end;
			DEBUG_PRINT_ENV(STATE_IDLE);
			continue;
		}

		switch (env_state) {
		case STATE_PLAY:
		case STATE_STOP:
		case STATE_IDLE:
			env_count = 0;
			continue;

		case STATE_DELAY:
			p[0] = p[1] = p[2] = p[3] = 0; 
		case STATE_HOLD:
			p += ENVELOPE_PERIOD / 2;
			env_count--;
			continue;

		case STATE_ATTACK:
			env_mult += env_incr;
			break;
		case STATE_DECAY:
		case STATE_RELEASE:
			env_mult = multiply_32x32_rshift32_rounded(env_mult, env_incr) << 1;
			if (env_mult < MINIMUM_VOL_MULT) {
				env_state = STATE_IDLE;
				env_count = 0;
				continue;
			}
			break;
		case STATE_SUSTAIN:
			break;
		}

		tmp1 = signed_multiply_32x16b(env_mult, p[0]) << 1;
		tmp2 = signed_multiply_32x16t(env_mult, p[0]) << 1;
		p[0] = pack_16t_16t(tmp2, tmp1);
		tmp1 = signed_multiply_32x16b(env_mult, p[1]) << 1;
		tmp2 = signed_multiply_32x16t(env_mult, p[1]) << 1;
		p[1] = pack_16t_16t(tmp2, tmp1);
		tmp1 = signed_multiply_32x16b(env_mult, p[2]) << 1;
		tmp2 = signed_multiply_32x16t(env_mult, p[2]) << 1;
		p[2] = pack_16t_16t(tmp2, tmp1);
		tmp1 = signed_multiply_32x16b(env_mult, p[3]) << 1;
		tmp2 = signed_multiply_32x16t(env_mult, p[3]) << 1;
		p[3] = pack_16t_16t(tmp2, tmp1);
		p += ENVELOPE_PERIOD / 2;
		env_count--;
	}

	//for (int i = 0; i < AUDIO_BLOCK_SAMPLES; i+=8) {
	//	Serial.printf("%hi,%hi,%hi,%hi,%hi,%hi,%hi,%hi,", block->data[i],block->data[i+1],block->data[i+2],block->data[i+3],block->data[i+4],block->data[i+5],block->data[i+6],block->data[i+7]);
	//}

	transmit(block);
	release(block);

	cli();
	this->tone_phase = tone_phase;
	//this->tone_amp = tone_amp;

	this->env_state = state_change ? this->env_state : env_state;
	this->env_count = env_count;
	this->env_mult = env_mult;
	this->env_incr = env_incr;

	this->vib_delay_count = vib_delay_count;
	this->mod_delay_count = mod_delay_count;
	this->vib_phase = vib_phase;
	this->mod_phase = mod_phase;
	this->vib_tone_incr = vib_tone_incr;
	this->mod_tone_incr = mod_tone_incr;
	this->vib_freq_mult = vib_freq_mult;
	this->mod_freq_mult = mod_freq_mult;
	sei();
}

void AudioSynthWavetable::update_interpolation(audio_block_t*& block, const sample_data* s, uint32_t& tone_phase_ext) {
	uint32_t* p, *end;
	uint32_t index;
	uint16_t phase_scale;
	int32_t s1, s2;
	uint32_t tmp1;
	uint32_t tone_phase = tone_phase_ext;

	p = (uint32_t*)block->data;
	end = p + AUDIO_BLOCK_SAMPLES / 2;

	if (s->LOOP) while (p < end) {
		int32_t tone_incr = p[0];
		int32_t mod_amp = p[1];

		for (int i = 0; i < 2; ++i, ++p) {
			index = tone_phase >> (32 - s->INDEX_BITS);
			tmp1 = *((uint32_t*)(s->sample + index));
			phase_scale = tone_phase >> (16 - s->INDEX_BITS);
			s1 = signed_multiply_32x16t(phase_scale, tmp1);
			phase_scale = ~phase_scale;
			s1 = signed_multiply_accumulate_32x16b(s1, phase_scale, tmp1);
			s1 = signed_multiply_32x16b(mod_amp, s1);

			tone_phase += tone_incr;
			tone_phase = tone_phase >= s->LOOP_PHASE_END ? tone_phase - s->LOOP_PHASE_LENGTH : tone_phase;

			index = tone_phase >> (32 - s->INDEX_BITS);
			tmp1 = *((uint32_t*)(s->sample + index));
			phase_scale = tone_phase >> (16 - s->INDEX_BITS);
			s2 = signed_multiply_32x16t(phase_scale, tmp1);
			phase_scale = ~phase_scale;
			s2 = signed_multiply_accumulate_32x16b(s2, phase_scale, tmp1);
			s2 = signed_multiply_32x16b(mod_amp, s2);

			*p = pack_16b_16b(s2, s1);

			tone_phase += tone_incr;
			tone_phase = tone_phase >= s->LOOP_PHASE_END ? tone_phase - s->LOOP_PHASE_LENGTH : tone_phase;
		}
	} else while (tone_phase < s->MAX_PHASE && p < end) {
		int32_t tone_incr = p[0];
		int32_t mod_amp = p[1];

		for (int i = 0; i < 2; ++i, ++p) {
			index = tone_phase >> (32 - s->INDEX_BITS);
			tmp1 = *((uint32_t*)(s->sample + index));
			phase_scale = tone_phase >> (16 - s->INDEX_BITS);
			s1 = signed_multiply_32x16t(phase_scale, tmp1);
			phase_scale = ~phase_scale;
			s1 = signed_multiply_accumulate_32x16b(s1, phase_scale, tmp1);
			s1 = signed_multiply_32x16b(mod_amp, s1);

			tone_phase += tone_incr;
			if (tone_phase >= s->MAX_PHASE) {
				*p = pack_16b_16b(0, s1);
				p++;
				break;
			}

			index = tone_phase >> (32 - s->INDEX_BITS);
			tmp1 = *((uint32_t*)(s->sample + index));
			phase_scale = tone_phase >> (16 - s->INDEX_BITS);
			s2 = signed_multiply_32x16t(phase_scale, tmp1);
			phase_scale = ~phase_scale;
			s2 = signed_multiply_accumulate_32x16b(s2, phase_scale, tmp1);
			s2 = signed_multiply_32x16b(mod_amp, s2);

			tone_phase += tone_incr;

			*p = pack_16b_16b(s2, s1);
		}
	}
	while (p < end) *p++ = 0;
	tone_phase_ext = tone_phase;
}
