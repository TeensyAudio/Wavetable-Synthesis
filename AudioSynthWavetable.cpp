/* Audio Library for Teensy 3.X
 * Copyright (c) 2014, Paul Stoffregen, paul@pjrc.com
 *
 * Development of this audio library was funded by PJRC.COM, LLC by sales of
 * Teensy and Audio Adaptor boards.  Please support PJRC's efforts to develop
 * open source software by purchasing Teensy or other PJRC products.
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
#include <SerialFlash.h>
#include <dspinst.h>

//#define STATE_IDLE	0
//#define STATE_DELAY	1
//#define STATE_ATTACK	2
//#define STATE_HOLD	3
//#define STATE_DECAY	4
//#define STATE_SUSTAIN	5
//#define STATE_RELEASE	6

uint32_t
	AudioSynthWavetable::interpolation_update,
	AudioSynthWavetable::envelope_update,
	AudioSynthWavetable::total_update,
	AudioSynthWavetable::total_parseSample,
	AudioSynthWavetable::total_playFrequency,
	AudioSynthWavetable::total_frequency,
	AudioSynthWavetable::total_playNote,
	AudioSynthWavetable::total_amplitude;

void AudioSynthWavetable::setSamples(const sample_data * samples, int num_samples) {
	this->samples = samples;
	this->num_samples = num_samples;
}

bool AudioSynthWavetable::isPlaying() {
	return envelopeState != STATE_IDLE;
}

void AudioSynthWavetable::parseSample(int sample_num, bool custom_env) {
	total_parseSample -= micros();
	//int note1, note2, velocity1, velocity2;
	sample_data data = samples[sample_num];
	
	tone_phase = 0;
	playing = 0;

	/**********************extracting sample header data: *********************************/
	/*
	 Index 0 = Format & Sample size. Same as before.
	 Index 1 = Original Pitch
	 Index 2 = Sample Rate
	 Index 3 = Loop Start
	 Index 4 = Loop End
	 Index 5 = Lower Bound Note Range | Upper Bound Note Range
	 Index 6 = Lower Bound Velocity Range | Upper Bound Velocity Range
	 Index 7 = Top 16-bits Delay envelope | Bottom 16-bits Hold Envelope
	 Index 8 = Attack envelope
	 Index 9 = Decay envelope
	 Index 10 = Sustain envelope //not ms
	 Index 11 = Release envelope
	 */
	 
	//note: assuming 16-bit PCM at 44100 Hz for now
	length = data.SAMPLE_LENGTH;
	waveform = (uint32_t*)data.sample;
	setSampleNote(data.ORIGINAL_PITCH);
	sample_rate = data.SAMPLE_RATE;

	//setting start and end loop
	setLoop(data.LOOP_START, data.LOOP_END);

	//note1 = data.NOTE_RANGE_1;
	//note2 = data.NOTE_RANGE_2;
	
	//velocity1 = data.VELOCITY_RANGE_1;
	//velocity2 = data.VELOCITY_RANGE_2;
	
	if (!custom_env) {
		env_delay(data.DELAY_ENV);
		env_hold(data.HOLD_ENV);
		env_attack(data.ATTACK_ENV);
		env_decay(data.DECAY_ENV);
		if (data.SUSTAIN_ENV > 0)
			env_sustain(data.SUSTAIN_ENV/1000);
		else
			env_sustain(1);
		env_release(data.RELEASE_ENV);
	}

	length_bits = 1;

	for (int len = length; len >>= 1; ++length_bits);

	max_phase = (length - 1) << (32 - length_bits);
	if (loop_start >= 0)
		loop_start_phase = (loop_start - 1) << (32 - length_bits);
	if (loop_end > 0)
		loop_end_phase = (loop_end - 1) << (32 - length_bits);
	else
		loop_end_phase = max_phase;

	//Serial.printf("set sample: loop_start_phase=%u, ", loop_start_phase);
	//Serial.printf("loop_end_phase=%u, ", loop_end_phase);
	//Serial.printf("tone_phase=%u, ", tone_phase);
	//Serial.printf("max_phase=%u\n", max_phase);
	total_parseSample += micros();
}

void AudioSynthWavetable::play(void) {
	if (waveform == NULL)
		return;
	tone_phase = 0;
	playing = 1;
}

void AudioSynthWavetable::playFrequency(float freq, bool custom_env) {
	total_playFrequency -= micros();
	float freq1, freq2;
	//elapsedMillis timer = 0;
	for(int i = 0; i < num_samples; i++) {
		freq1 = noteToFreq(samples[i].NOTE_RANGE_1);
		freq2 = noteToFreq(samples[i].NOTE_RANGE_2);
		if (freq >= freq1 && freq <= freq2) {
			parseSample(i, custom_env);
			//Serial.println("Branch 1");
			break;
		} else if (i == 0 && freq < freq1) {
			parseSample(0, custom_env);
			//Serial.println("Branch 2");
			break;
		} else if (i == num_samples-1 && freq > freq2) {
			parseSample(num_samples-1, custom_env);
			//Serial.println("Branch 3");
			break;
		} else if (freq > freq2 && freq < noteToFreq(samples[i+1].NOTE_RANGE_1)) {
			if (freq - freq2 < 0.5 * (noteToFreq(samples[i+1].NOTE_RANGE_1) - freq2)) {
				parseSample(i, custom_env);
				//Serial.println("Branch 4");
			}
			else {
				parseSample(i+1, custom_env);
				//Serial.println("Branch 5");
			}
			break;
		}
	}
	if (waveform == NULL) {
		total_playFrequency += micros();
		return;
	}
	frequency(freq);
	mult = 0;
	count = delay_count;
	if (count > 0) {
		envelopeState = STATE_DELAY;
		inc = 0;
		//Serial.printf("DELAY: %f\n", inc);
	} else {
		envelopeState = STATE_ATTACK;
		count = attack_count;
		// 2^16 divided by the number of samples
		inc = (UNITY_GAIN / (count << 3));
		//Serial.printf("ATTACK: %f\n", inc);
	}
	tone_phase = 0;
	playing = 1;
	//Serial.printf("Latency: %dms\n", (int)timer);
	total_playFrequency += micros();
}

void AudioSynthWavetable::playNote(int note, int amp, bool custom_env) {
	total_playNote -= micros();
	float freq = noteToFreq(note);
	this->playing = 0;
	//Serial.printf("Amplitude: %i\n", amp);  
	//amplitude((float)amp/(float)127);
	total_amplitude -= micros();
	amplitude(midi_volume_transform(amp));
	total_amplitude += micros();
	//Serial.println(freq);
	playFrequency(freq, custom_env);
	total_playNote += micros();
}

void AudioSynthWavetable::stop(void) {
	envelopeState = STATE_RELEASE;
	count = release_count;
	inc = (-(float)mult / ((int32_t)count << 3));
	//Serial.printf("RELEASE: %f\n", inc);
}

void AudioSynthWavetable::update(void) {
	total_update -= micros();

	audio_block_t* block;
	int16_t* out;
	uint32_t index, scale;
	int32_t s1, s2, v1, v2, v3;
	//elapsedMillis timer = 0;

	if (!playing || envelopeState == STATE_IDLE) {
		total_update += micros();
		return;
	}

	block = allocate();
	if (block == NULL) {
		total_update += micros();
		return;
	}

	out = block->data;

	interpolation_update -= micros();
	//assuming 16 bit PCM, 44100 Hz
	int16_t* waveform = (int16_t*)this->waveform;
	//Serial.printf("update: length=%i, length_bits=%i, tone_phase=%u, max_phase=%u\n", length, length_bits, tone_phase, max_phase);
	//Serial.printf("update: loop_start_phase=%u, loop_end_phase=%u, tone_phase=%u, max_phase=%u\n", loop_start_phase, loop_end_phase, tone_phase, max_phase);
	//Serial.printf("tone_incr=%u, tone_amp=%u, sample_freq=%f\n", tone_incr, tone_amp, sample_freq);
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
	interpolation_update += micros();


	//*********************************************************************
	//Envelope code
	//*********************************************************************

	uint32_t* p;
	uint32_t* end;
	uint32_t sample12, sample34, sample56, sample78, tmp1, tmp2;

	envelope_update -= micros();
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
			//Serial.printf("ATTACK: %f\n", inc);
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
			inc = count > 0 ? ((sustain_mult - UNITY_GAIN) / ((int32_t)count << 3)) : 0;
			//Serial.printf("DECAY: %f\n", inc);
			continue;
		case STATE_DECAY:
			if (decay_count) {
				envelopeState = STATE_SUSTAIN;
				count = 0xFFFF;
				mult = sustain_mult;
				inc = 0;
				//Serial.printf("SUSTAIN: %f\n", inc);
			}
			break;
		case STATE_SUSTAIN:
			count = 0xFFFF;
			break;
		case STATE_RELEASE:
			envelopeState = STATE_IDLE;
			playing = 0;
			//Serial.println("IDLE");
			while (p < end) {
				*p++ = 0;
				*p++ = 0;
				*p++ = 0;
				*p++ = 0;
			}
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
	envelope_update += micros();

	transmit(block);
	release(block);
	//Serial.printf("Latency: %dms\n", (int)timer);
	total_update += micros();
}

void AudioSynthWavetable::frequency(float freq) {
	total_frequency -= micros();
	if (freq < 0.0)
		freq = 0.0;
	else if (freq > AUDIO_SAMPLE_RATE_EXACT / 2)
		freq = AUDIO_SAMPLE_RATE_EXACT / 2;

	float rate_coef = sample_rate / AUDIO_SAMPLE_RATE_EXACT;

	//(0x80000000 >> (length_bits - 1) by itself results in a tone_incr that
	//steps through the wavetable sample one element at a time; from there we
	//only need to scale based a ratio of freq/sample_freq for the desired increment
	tone_incr = ((rate_coef * freq) / sample_freq) * (0x80000000 >> (length_bits - 1)) + 0.5;
	total_frequency += micros();
}

void AudioSynthWavetable::print_performance() {
	Serial.printf(
		"interpolation_update=%i\n"
		"envelope_update=%i\n"
		"total_update=%i\n"
		"total_parseSample=%i\n"
		"total_playFrequency=%i\n"
		"total_frequency=%i\n"
		"total_playNote=%i\n"
		"total_amplitude=%i\n",
		AudioSynthWavetable::interpolation_update,
		AudioSynthWavetable::envelope_update,
		AudioSynthWavetable::total_update,
		AudioSynthWavetable::total_parseSample,
		AudioSynthWavetable::total_playFrequency,
		AudioSynthWavetable::total_frequency,
		AudioSynthWavetable::total_playNote,
		AudioSynthWavetable::total_amplitude);
}

