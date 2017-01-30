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

void AudioSynthWavetable::setSample(const unsigned int *data) {

	/**********extracting header data 
	Index 0 = Format & Sample size. Same as before.
	Index 1 = Original Pitch
	Index 2 = Sample Rate
	Index 3 = Loop Start
	Index 4 = Loop End
	Index 5 = Top 16-bits Delay envelope | Bottom 16-bits Hold Envelope
	Index 6 = Attack envelope
	Index 7 = Decay envelope
	Index 8 = Sustain envelope
	Index 9 = Release envelope

	*****************************************/
	tone_phase = 0;
	playing = 0;

	//note: assuming 16-bit PCM at 44100 Hz for now
	length = (*data++ & 0x00FFFFFF);
	waveform = (uint32_t*)data;
	
	length = loop_end;

	length_bits = 1;
	for (int len = length; len >>= 1; ++length_bits);
	max_phase = (length - 1) << (32 - length_bits);

	//Serial.printf("length=%i, length_bits=%i, tone_phase=%u, max_phase=%u\n", length, length_bits, tone_phase, max_phase);
}

void AudioSynthWavetable::play(void) {
	if (waveform == NULL)
		return;
	tone_phase = 0;
	playing = 1;
}

void AudioSynthWavetable::playFrequency(float freq) {
	if (waveform == NULL)
		return;
	frequency(freq);
	tone_phase = 0;
	playing = 1;
}

void AudioSynthWavetable::playNote(int note) {
	float freq = noteToFreq(note);
	Serial.println(freq);
	playFrequency(freq);
}

void AudioSynthWavetable::stop(void) {
	playing = 0;
}

void AudioSynthWavetable::update(void) {
	audio_block_t* block;
	int16_t* out;
	uint32_t index, scale;
	int16_t s1, s2;
	uint32_t v1, v2, v3;

	if (!playing)
		return;

	block = allocate();
	if (block == NULL)
		return;

	out = block->data;

	//assuming 16 bit PCM, 44100 Hz
	int16_t* waveform = (int16_t*)this->waveform;
	//Serial.printf("length=%i, length_bits=%i, tone_phase=%u, max_phase=%u\n", length, length_bits, tone_phase, max_phase);
	//Serial.printf("tone_incr=%u, tone_amp=%u, sample_freq=%f\n", tone_incr, tone_amp, sample_freq);
	for (int i = 0; i < AUDIO_BLOCK_SAMPLES; i++) {
		tone_phase = tone_phase < max_phase ? tone_phase : tone_phase - loop_phase;
		index = tone_phase >> (32 - length_bits);
		scale = (tone_phase << length_bits) >> 16;
		s1 = waveform[index];
		s2 = waveform[index + 1];
		v2 = s2 * scale;
		v1 = s1 * (0xFFFF - scale);
		v3 = (v1 + v2) >> 16;
		*out++ = (int16_t)v3;
		//*out++ = (int16_t)((v3 * tone_amp) >> 16);
		tone_phase += tone_incr;
	}

	transmit(block);
	release(block);
}

void AudioSynthWavetable::frequency(float freq) {
	if (freq < 0.0)
		freq = 0.0;
	else if (freq > AUDIO_SAMPLE_RATE_EXACT / 2)
		freq = AUDIO_SAMPLE_RATE_EXACT / 2;

	//(0x80000000 >> (length_bits - 1) by itself results in a tone_incr that
	//steps trhough the wavetable sample one element at a time; from there we
	//only need to scale based a ratio of freq/sample_freq for the desired increment
	tone_incr = (freq / sample_freq) * (0x80000000 >> (length_bits - 1)) + 0.5;
}
