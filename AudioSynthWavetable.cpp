#include "AudioSynthWavetable.h"
#include <dspinst.h>
#include <SerialFlash.h>

#define TIME_TEST_ON
//#define ENVELOPE_DEBUG

#ifdef TIME_TEST_ON
#define TIME_TEST(INTERVAL, CODE_BLOCK_TO_TEST) \
static float MICROS_AVG = 0.0; \
static int TEST_CUR_CNT = 0; \
static int TEST_LST_CNT = 0; \
static int NEXT_DISPLAY = 0; \
static int TEST_TIME_ACC = 0; \
int micros_start = micros(); \
CODE_BLOCK_TO_TEST \
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

#ifdef ENVELOPE_DEBUG
#define PRINT_ENV(NAME) Serial.printf("%14s-- env_mult:%06.4f%% of UNITY_GAIN env_incr:%06.4f%% of UNITY_GAIN env_count:%i\n", #NAME, float(env_mult)/float(UNITY_GAIN), float(env_incr)/float(UNITY_GAIN), env_count);
#else
#define PRINT_ENV(NAME) do { } while(0);
#endif


void AudioSynthWavetable::stop(void) {
	cli();
	env_state = STATE_RELEASE;
	env_count = current_sample->RELEASE_COUNT;
	if (env_count == 0) env_count = 1;
	env_incr = -(env_mult) / (env_count * 8);
	PRINT_ENV(STATE_RELEASE)
	sei();
}

void AudioSynthWavetable::playFrequency(float freq, int amp) {
	setState(freqToNote(freq), amp, freq);
}

void AudioSynthWavetable::playNote(int note, int amp) {
	setState(note, amp, noteToFreq(note));
}

void AudioSynthWavetable::setState(int note, int amp, float freq) {
	cli();
	int i;
	env_state = STATE_IDLE;
	for (i = 0; note > instrument->sample_note_ranges[i]; i++);
	current_sample = &instrument->samples[i];
	if (current_sample == NULL) return;
	setFrequency(freq);
	vib_count = vib_phase = tone_phase = env_incr = env_mult = 0;
	env_count = current_sample->DELAY_COUNT;
	tone_amp = amp * (UINT16_MAX / 127);
	tone_amp = current_sample->INITIAL_ATTENUATION_SCALAR * tone_amp >> 16;
	env_state = STATE_DELAY;
	PRINT_ENV(STATE_DELAY);
	state_change = true;
	sei();
}

void AudioSynthWavetable::setFrequency(float freq) {
	float tone_incr_temp = freq * current_sample->PER_HERTZ_PHASE_INCREMENT;
	tone_incr = tone_incr_temp;
	vib_pitch_offset_init = tone_incr_temp * current_sample->VIBRATO_PITCH_COEFFICIENT_INITIAL;
	vib_pitch_offset_scnd = tone_incr_temp * current_sample->VIBRATO_PITCH_COEFFICIENT_SECOND;
	mod_pitch_offset_init = tone_incr_temp * current_sample->MODULATION_PITCH_COEFFICIENT_INITIAL;
	mod_pitch_offset_scnd = tone_incr_temp * current_sample->MODULATION_PITCH_COEFFICIENT_SECOND;
}

void AudioSynthWavetable::update(void) {
	cli();
	if (env_state == STATE_IDLE) {
		sei();
		return;
	}
	this->state_change = false;

	const sample_data* s = (const sample_data*)current_sample;
	uint32_t tone_phase = this->tone_phase;
	uint32_t tone_incr = this->tone_incr;
	uint16_t tone_amp = this->tone_amp;

	envelopeStateEnum  env_state = this->env_state;
	int32_t env_count = this->env_count;
	int32_t env_mult = this->env_mult;
	int32_t env_incr = this->env_incr;

	uint32_t vib_count = this->vib_count;
	uint32_t vib_phase = this->vib_phase;
	int32_t vib_pitch_offset_init = this->vib_pitch_offset_init;
	int32_t vib_pitch_offset_scnd = this->vib_pitch_offset_scnd;
	sei();

	if (s->LOOP == false && tone_phase >= s->MAX_PHASE) return;

	audio_block_t* block;
	block = allocate();
	if (block == NULL) return;

	uint32_t* p, * end;
	uint32_t index, scale;
	int32_t s1, s2;
	uint32_t tmp1, tmp2;
	int32_t vib_tone_incr = 0;
	int32_t mod_tone_incr = 0;
	int32_t mod_amp;

	mod_amp = tone_amp;
	
	p = (uint32_t*)block->data;
	end = p + AUDIO_BLOCK_SAMPLES / 2;
	//static int modtest = 64;

	TIME_TEST(5000,

	while(p < end) {
		if (s->LOOP == false && tone_phase >= s->MAX_PHASE) break;
		if (vib_count++ > s->VIBRATO_DELAY) {
			vib_phase += s->VIBRATO_INCREMENT;
			int16_t vib_scale = (((vib_phase - 0x40000000) & 0x80000000) ? vib_phase : (0x7FFFFFFF - vib_phase)) >> 15;
			vib_tone_incr = (int32_t(vib_scale) * (vib_scale >= 0 ? vib_pitch_offset_init : vib_pitch_offset_scnd)) >> 15;
		}
		if (mod_count++ > s->MODULATION_DELAY) {
			mod_phase += s->MODULATION_INCREMENT;
			int16_t mod_scale = (((mod_phase - 0x40000000) & 0x80000000) ? mod_phase : (0x7FFFFFFF - mod_phase)) >> 15;
			mod_tone_incr = (int32_t(mod_scale) * (mod_scale >= 0 ? mod_pitch_offset_init : mod_pitch_offset_scnd)) >> 15;
			mod_amp = int32_t(mod_scale) * (mod_scale >= 0 ? s->MODULATION_AMPLITUDE_INITIAL_GAIN : s->MODULATION_AMPLITUDE_FINAL_GAIN) >> 15;
			mod_amp = tone_amp - (tone_amp * mod_amp >> 15);
			//if (!--modtest) {
			//	modtest += 64;
			//	Serial.printf("mod_amp:%u, tone_amp:%hu, ATTEN:%hu\n", mod_amp, tone_amp, INITIAL_ATTENUATION_SCALAR);
			//	Serial.printf("INIT_GAIN:%i, SCND_GAIN:%i, mod_scale:%hi\n", MODULATION_AMPLITUDE_INITIAL_GAIN, MODULATION_AMPLITUDE_FINAL_GAIN, mod_scale);
			//	Serial.printf("INTI_CALC:%f, SCND_CALC:%f\n", float(DECIBEL_SHIFT(-3.5) - 1.0), float(1.0 - DECIBEL_SHIFT(3.5)));
			//}
		}
		for (int i = LFO_PERIOD/2; i; --i) {
			index = tone_phase >> (32 - s->INDEX_BITS);
			tmp1 = *((uint32_t*)(s->sample + index));
			scale = (tone_phase << s->INDEX_BITS) >> 16;
			s1 = signed_multiply_32x16t(scale, tmp1);
			s1 = signed_multiply_accumulate_32x16b(s1, 0xFFFF - scale, tmp1);
			s1 = signed_multiply_32x16b(mod_amp, s1);

			tone_phase += tone_incr + vib_tone_incr + mod_tone_incr;
			if (s->LOOP == false && tone_phase >= s->MAX_PHASE) break;
			tone_phase = s->LOOP && tone_phase >= s->LOOP_PHASE_END ? tone_phase - s->LOOP_PHASE_LENGTH : tone_phase;

			index = tone_phase >> (32 - s->INDEX_BITS);
			tmp1 = *((uint32_t*)(s->sample + index));
			scale = (tone_phase << s->INDEX_BITS) >> 16;
			s2 = signed_multiply_32x16t(scale, tmp1);
			s2 = signed_multiply_accumulate_32x16b(s2, 0xFFFF - scale, tmp1);
			s2 = signed_multiply_32x16b(mod_amp, s2);

			*p++ = pack_16b_16b(s2, s1);

			tone_phase += tone_incr + vib_tone_incr + mod_tone_incr;
			if (s->LOOP == false && tone_phase >= s->MAX_PHASE) break;
			tone_phase = s->LOOP && tone_phase >= s->LOOP_PHASE_END ? tone_phase - s->LOOP_PHASE_LENGTH : tone_phase;
		}
	}
	); //end TIME_TEST

	p = (uint32_t *)block->data;
	end = p + AUDIO_BLOCK_SAMPLES / 2;

	while (p < end) {
		if (env_count <= 0) switch (env_state) {
		case STATE_DELAY:
			env_state = STATE_ATTACK;
			env_count = s->ATTACK_COUNT;
			env_incr = UNITY_GAIN / (env_count * 8);
			PRINT_ENV(STATE_ATTACK);
			continue;
		case STATE_ATTACK:
			env_mult = UNITY_GAIN;
			env_state = STATE_HOLD;
			env_count = s->HOLD_COUNT;
			env_incr = 0;
			PRINT_ENV(STATE_HOLD);
			continue;
		case STATE_HOLD:
			env_state = STATE_DECAY;
			env_count = s->DECAY_COUNT;
			env_incr = (-s->SUSTAIN_MULT) / (env_count * 8);
			PRINT_ENV(STATE_DECAY);
			continue;
		case STATE_DECAY:
			env_mult = UNITY_GAIN - s->SUSTAIN_MULT;
			env_state = env_mult < UNITY_GAIN / 10000 ? STATE_RELEASE : STATE_SUSTAIN;
			env_incr = 0;
			continue;
		case STATE_SUSTAIN:
			env_count = INT32_MAX;
			PRINT_ENV(STATE_SUSTAIN);
			continue;
		case STATE_RELEASE:
			env_state = STATE_IDLE;
			for (; p < end; ++p) *p = 0;
			PRINT_ENV(STATE_IDLE);
			continue;
		default:
			p = end;
			PRINT_ENV(DEFAULT);
			continue;
		}
		// process 8 samples, using only env_mult and env_incr
		env_mult += env_incr;
		tmp1 = signed_multiply_32x16b(env_mult >> 15, p[0]);
		env_mult += env_incr;
		tmp2 = signed_multiply_32x16t(env_mult >> 15, p[0]);
		p[0] = pack_16b_16b(tmp2, tmp1);
		env_mult += env_incr;
		tmp1 = signed_multiply_32x16b(env_mult >> 15, p[1]);
		env_mult += env_incr;
		tmp2 = signed_multiply_32x16t(env_mult >> 15, p[1]);
		p[1] = pack_16b_16b(tmp2, tmp1);
		env_mult += env_incr;
		tmp1 = signed_multiply_32x16b(env_mult >> 15, p[2]);
		env_mult += env_incr;
		tmp2 = signed_multiply_32x16t(env_mult >> 15, p[2]);
		p[2] = pack_16b_16b(tmp2, tmp1);
		env_mult += env_incr;
		tmp1 = signed_multiply_32x16b(env_mult >> 15, p[3]);
		env_mult += env_incr;
		tmp2 = signed_multiply_32x16t(env_mult >> 15, p[3]);
		p[3] = pack_16b_16b(tmp2, tmp1);

		p += 4;
		env_count--;
	}

	cli();
	if (this->state_change == false) {
		this->tone_phase = tone_phase;
		this->env_state = env_state;
		this->env_count = env_count;
		this->env_mult = env_mult;
		this->env_incr = env_incr;
		this->vib_count = vib_count;
		this->vib_phase = vib_phase;
	}
	sei();

	transmit(block);
	release(block);
}
