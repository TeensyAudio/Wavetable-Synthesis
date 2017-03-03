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
#define PRINT_ENV(NAME) Serial.printf("%14s-- mult:%06.4f%% of UNITY_GAIN inc:%06.4f%% of UNITY_GAIN count:%i\n", #NAME, float(mult)/float(UNITY_GAIN), float(inc)/float(UNITY_GAIN), count);
#else
#define PRINT_ENV(NAME) do { } while(0);
#endif


void AudioSynthWavetable::stop(void) {
	cli();
	envelopeState = STATE_RELEASE;
	count = current_sample->RELEASE_COUNT;
	if (count == 0) count = 1;
	inc = -(mult) / (count * 8);
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
	envelopeState = STATE_IDLE;
	for (i = 0; note > instrument->sample_note_ranges[i]; i++);
	current_sample = &instrument->samples[i];
	if (current_sample == NULL) return;
	setFrequency(freq);
	vcount = vphase = tone_phase = inc = mult = 0;
	count = current_sample->DELAY_COUNT;
	amplitude(midi_volume_transform(amp));
	tone_amp = 2855;
	envelopeState = STATE_DELAY;
	PRINT_ENV(STATE_DELAY)
		state_change = true;
	sei();
}

void AudioSynthWavetable::setFrequency(float freq) {
	//float per_hz_increment_rate = ((0x80000000 >> (index_bits - 1)) * cents_offset * rate_coef) / sample_freq + 0.5;
	tone_incr = freq * current_sample->PER_HERTZ_PHASE_INCREMENT;
	//(0x80000000 >> (index_bits - 1) by itself results in a tone_incr that
	//steps through the wavetable sample one element at a time; from there we
	//only need to scale based a ratio of freq/sample_freq for the desired increment
	//tone_incr = cents_offset * ((rate_coef * freq) / sample_freq) * (0x80000000 >> (index_bits - 1)) + 0.5;
}

void AudioSynthWavetable::update(void) {
	cli();
	if (envelopeState == STATE_IDLE) {
		sei();
		return;
	}
	this->state_change = false;
	const sample_data* s = (const sample_data*)current_sample;
	uint32_t tone_phase = this->tone_phase;
	uint32_t tone_incr = this->tone_incr;
	uint16_t tone_amp = this->tone_amp;
	envelopeStateEnum  envelopeState = this->envelopeState;
	int32_t count = this->count;
	int32_t mult = this->mult;
	int32_t inc = this->inc;
	sei();

	audio_block_t* block;
	int16_t* out;
	uint32_t index, scale;
	int32_t s1, s2;
	uint32_t* p;
	uint32_t* end;
	uint32_t tmp1, tmp2;

	int16_t vscale = 0;
	int32_t vtone_incr = 0;

	block = allocate();
	if (block == NULL) return;

	//out = block->data;

	p = (uint32_t*)block->data;
	end = p + AUDIO_BLOCK_SAMPLES / 2;

	TIME_TEST(5000,
	int32_t voffset_high = voffset_high_coef*tone_incr;
	int32_t voffset_low = voffset_low_coef*tone_incr;

	//static int display_vib = 64;  test vibrato
	//assuming 16 bit PCM, 44100 Hz
	while(p < end) {
		for (int i = VIBRATO_PERIOD/2; i; --i) {
			index = tone_phase >> (32 - s->INDEX_BITS);
			tmp1 = *((uint32_t*)(s->sample + index));
			scale = (tone_phase << s->INDEX_BITS) >> 16;
			s1 = signed_multiply_32x16t(scale, tmp1);
			s1 = signed_multiply_accumulate_32x16b(s1, 0xFFFF - scale, tmp1);

			tone_phase += tone_incr + vtone_incr;
			tone_phase = tone_phase >= s->LOOP_PHASE_END ? tone_phase - s->LOOP_PHASE_LENGTH : tone_phase;

			index = tone_phase >> (32 - s->INDEX_BITS);
			tmp1 = *((uint32_t*)(s->sample + index));
			scale = (tone_phase << s->INDEX_BITS) >> 16;
			s2 = signed_multiply_32x16t(scale, tmp1);
			s2 = signed_multiply_accumulate_32x16b(s2, 0xFFFF - scale, tmp1);

			*p++ = pack_16b_16b(s2, s1);

			tone_phase += tone_incr + vtone_incr;
			tone_phase = tone_phase >= s->LOOP_PHASE_END ? tone_phase - s->LOOP_PHASE_LENGTH : tone_phase;
		}
		
		if (++vcount > vdelay) {
			vphase += vincr;
			vscale = (((vphase - 0x40000000) & 0x80000000) ? vphase : (0x7FFFFFFF - vphase)) >> 15;
			vtone_incr = signed_multiply_32x16b((vscale < 0 ? voffset_low : voffset_high), int32_t(vscale));
			//vtone_incr = (int32_t(vscale) * (vscale < 0 ? voffset_low : voffset_high)) >> 15;
		}
	}
	); //end TIME_TEST


	//*********************************************************************
	//Envelope code
	//*********************************************************************

	p = (uint32_t *)block->data;
	// p increments by 1 for every 2 samples processed.
	end = p + AUDIO_BLOCK_SAMPLES / 2;

	while (p < end) {
		// we only care about the state when completing a region
		if (count <= 0) switch (envelopeState) {
		case STATE_DELAY:
			envelopeState = STATE_ATTACK;
			count = s->ATTACK_COUNT;
			inc = UNITY_GAIN / (count * 8);
			PRINT_ENV(STATE_ATTACK);
			continue;
		case STATE_ATTACK:
			mult = UNITY_GAIN;
			envelopeState = STATE_HOLD;
			count = s->HOLD_COUNT;
			inc = 0;
			PRINT_ENV(STATE_HOLD);
			continue;
		case STATE_HOLD:
			envelopeState = STATE_DECAY;
			count = s->DECAY_COUNT;
			inc = (-s->SUSTAIN_MULT) / (count * 8);
			PRINT_ENV(STATE_DECAY);
			continue;
		case STATE_DECAY:
			mult = UNITY_GAIN - s->SUSTAIN_MULT;
			envelopeState = mult < UNITY_GAIN / 10000 ? STATE_RELEASE : STATE_SUSTAIN;
			inc = 0;
			continue;
		case STATE_SUSTAIN:
			count = INT32_MAX;
			PRINT_ENV(STATE_SUSTAIN);
			continue;
		case STATE_RELEASE:
			envelopeState = STATE_IDLE;
			for (; p < end; ++p) *p = 0;
			PRINT_ENV(STATE_IDLE);
			continue;
		default:
			p = end;
			PRINT_ENV(DEFAULT);
			continue;
		}
		// process 8 samples, using only mult and inc
		mult += inc;
		tmp1 = signed_multiply_32x16b(mult >> 15, p[0]);
		mult += inc;
		tmp2 = signed_multiply_32x16t(mult >> 15, p[0]);
		p[0] = pack_16b_16b(tmp2, tmp1);
		mult += inc;
		tmp1 = signed_multiply_32x16b(mult >> 15, p[1]);
		mult += inc;
		tmp2 = signed_multiply_32x16t(mult >> 15, p[1]);
		p[1] = pack_16b_16b(tmp2, tmp1);
		mult += inc;
		tmp1 = signed_multiply_32x16b(mult >> 15, p[2]);
		mult += inc;
		tmp2 = signed_multiply_32x16t(mult >> 15, p[2]);
		p[2] = pack_16b_16b(tmp2, tmp1);
		mult += inc;
		tmp1 = signed_multiply_32x16b(mult >> 15, p[3]);
		mult += inc;
		tmp2 = signed_multiply_32x16t(mult >> 15, p[3]);
		p[3] = pack_16b_16b(tmp2, tmp1);

		p += 4;
		count--;
	}

	cli();
	if (this->state_change == false) {
		this->tone_phase = tone_phase;
		this->envelopeState = envelopeState;
		this->count = count;
		this->mult = mult;
		this->inc = inc;
	}
	sei();

	transmit(block);
	release(block);
}
