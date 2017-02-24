#include "AudioSynthWavetable.h"
#include <dspinst.h>
#include <SerialFlash.h>

void AudioSynthWavetable::stop(void) {
	envelopeState = STATE_RELEASE;
	count = release_count;
	inc = (-(float)mult / ((int32_t)count << 3));
}

void AudioSynthWavetable::parseSample(int sample_num, bool custom_env) {
	const sample_data* s = &instrument->samples[sample_num];
	current_sample = s;
	
	tone_phase = 0;
	 
	length = s->SAMPLE_LENGTH;
	waveform = (uint32_t*)s->sample;
	setSampleNote(s->ORIGINAL_PITCH);

	cents_offset = s->CENTS_OFFSET;
	
	//setting start and end loop
	setLoop(s->LOOP_START, s->LOOP_END);

	if (!custom_env) {
		env_delay(s->DELAY_ENV);
		env_hold(s->HOLD_ENV);
		env_attack(s->ATTACK_ENV);
		env_decay(s->DECAY_ENV);
		if (s->SUSTAIN_ENV > 0)
			env_sustain((float)s->SUSTAIN_ENV / UNITY_GAIN);
		else
			env_sustain(1);
		env_release(s->RELEASE_ENV);
	}

	//length_bits = length & 0xFFFFF000 ? 13 : loop_length & 0xFFFFFF00 ? 9 : loop_length & 0xFFFFFFF0 ? 5 : 1;
	//for (int len = length >> length_bits; len; len >>= 1) ++length_bits;
	//int temp = length_bits
	
	length_bits = 1;
	for (int len = length; len >>= 1; ++length_bits);

	max_phase = (length - 1) << (32 - length_bits);
	if (loop_start >= 0)
		loop_start_phase = (loop_start - 1) << (32 - length_bits);
	if (loop_end > 0)
		loop_end_phase = (loop_end - 1) << (32 - length_bits);
	else
		loop_end_phase = max_phase;

}

void AudioSynthWavetable::playFrequency(float freq, bool custom_env) {
	int i, note;
	for (i = 0, note = freqToNote(freq); note > instrument->sample_note_ranges[i]; i++);
	parseSample(i, custom_env);
	if (waveform == NULL) {
		return;
	}
	setFrequency(freq);
	mult = 0;
	count = delay_count;
	if (count > 0) {
		envelopeState = STATE_DELAY;
		inc = 0;
	} else {
		envelopeState = STATE_ATTACK;
		count = attack_count;
		// 2^16 divided by the number of samples
		inc = (UNITY_GAIN / (count << 3));
	}
	tone_phase = 0;
}

void AudioSynthWavetable::playNote(int note, int amp, bool custom_env) {
	int i;
	for(i = 0; note > instrument->sample_note_ranges[i]; i++);
	parseSample(i, custom_env);
	if (waveform == NULL) return;
	setFrequency(noteToFreq(note));
	tone_phase = inc = mult = 0;
	count = delay_count;
	envelopeState = STATE_DELAY;
	amplitude(midi_volume_transform(amp));
}

void AudioSynthWavetable::setFrequency(float freq) {
	float rate_coef = current_sample->SAMPLE_RATE_COEFFICIENT;
	//(0x80000000 >> (length_bits - 1) by itself results in a tone_incr that
	//steps through the wavetable sample one element at a time; from there we
	//only need to scale based a ratio of freq/sample_freq for the desired increment
	tone_incr = cents_offset * ((rate_coef * freq) / sample_freq) * (0x80000000 >> (length_bits - 1)) + 0.5;
}

void AudioSynthWavetable::update(void) {

	audio_block_t* block;
	int16_t* out;
	uint32_t index, scale;
	int32_t s1, s2, v1, v2, v3;
	//elapsedMillis timer = 0;

	if (envelopeState == STATE_IDLE)
		return;

	block = allocate();
	if (block == NULL) {
		return;
	}

	out = block->data;

	//assuming 16 bit PCM, 44100 Hz
	int16_t* waveform = (int16_t*)this->waveform;
	for (int i = 0; i < AUDIO_BLOCK_SAMPLES; i++) {
		//tone_phase = tone_phase < max_phase ? tone_phase : tone_phase - loop_phase;

		if (tone_phase >= loop_end_phase)
			tone_phase = tone_phase - loop_end_phase + loop_start_phase;
		//tone_phase = tone_phase < loop_end_phase ? tone_phase : tone_phase - loop_end_phase + loop_start_phase;
		index = tone_phase >> (32 - length_bits);
		scale = (tone_phase << length_bits) >> 16;
		s1 = waveform[index];
		s2 = waveform[index + 1];
		v1 = s1 * (0xFFFF - scale);
		v2 = s2 * scale;
		v3 = (v1 + v2) >> 16;
		*out++ = (int16_t)((v3 * tone_amp) >> 16);
		tone_phase += tone_incr;
	}


	//*********************************************************************
	//Envelope code
	//*********************************************************************

	uint32_t* p;
	uint32_t* end;
	uint32_t sample12, sample34, sample56, sample78, tmp1, tmp2;

	p = (uint32_t *)block->data;
	// p increments by 1 for every 2 samples processed.
	end = p + AUDIO_BLOCK_SAMPLES / 2;

	while (p < end) {
		// we only care about the state when completing a region
		if (count == 0) switch (envelopeState) {
		case STATE_DELAY:
			envelopeState = STATE_ATTACK;
			count = attack_count;
			inc = (UNITY_GAIN / (count << 3));
			continue;
		case STATE_ATTACK:
			envelopeState = STATE_HOLD;
			count = hold_count;
			mult = hold_count ? UNITY_GAIN : mult;
			inc = 0;
			continue;
		case STATE_HOLD:
			envelopeState = STATE_DECAY;
			count = decay_count;
			//inc = count > 0 ? (float)(-sustain_mult) / ((int32_t)count << 3) : 0;
			inc = (float)(-sustain_mult) / ((int32_t)count << 3);
			continue;
		case STATE_DECAY:
			envelopeState = STATE_SUSTAIN;
			count = 0xFFFF;
			mult = UNITY_GAIN - sustain_mult;
			inc = 0;
			break;
		case STATE_SUSTAIN:
			count = 0xFFFF;
			break;
		case STATE_RELEASE:
			envelopeState = STATE_IDLE;
			for (; p < end; p += 4) p[0] = p[1] = p[2] = p[3] = 0;
			continue;
		default:
			p = end;
			continue;
		}
		// process 8 samples, using only mult and inc
		sample12 = p[0];
		mult += inc;
		tmp1 = signed_multiply_32x16b((int32_t)mult, sample12);
		mult += inc;
		tmp2 = signed_multiply_32x16t((int32_t)mult, sample12);
		p[0] = pack_16b_16b(tmp2, tmp1);

		sample34 = p[1];
		mult += inc;
		tmp1 = signed_multiply_32x16b((int32_t)mult, sample34);
		mult += inc;
		tmp2 = signed_multiply_32x16t((int32_t)mult, sample34);
		p[1] = pack_16b_16b(tmp2, tmp1);

		sample56 = p[2];
		mult += inc;
		tmp1 = signed_multiply_32x16b((int32_t)mult, sample56);
		mult += inc;
		tmp2 = signed_multiply_32x16t((int32_t)mult, sample56);
		p[2] = pack_16b_16b(tmp2, tmp1);

		sample78 = p[3];
		mult += inc;
		tmp1 = signed_multiply_32x16b((int32_t)mult, sample78);
		mult += inc;
		tmp2 = signed_multiply_32x16t((int32_t)mult, sample78);
		p[3] = pack_16b_16b(tmp2, tmp1);

		p += 4;
		count--;
	}

	transmit(block);
	release(block);
}
