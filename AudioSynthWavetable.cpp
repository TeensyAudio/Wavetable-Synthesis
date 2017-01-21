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

void AudioSynthWavetable::play(const unsigned int *data) {
	int32_t length_temp;
	uint32_t format;

	this->tone_phase = 0;
	this->playing = 0;
	format = *data++;
	//switch (format >>= 24) {
	//	case 0x81: // 16 bit PCM, 44100 Hz
	length_temp = this->length = (format & 0x00FFFFFF) * 2;
	//		break;
	//}
	this->length_bits = 1;
	while (length_temp >>= 1)
		++length_bits;
	this->waveform = (uint32_t*)data;
	Serial.printf("length=%i, length_bits=%i, tone_phase=%u", length, length_bits, this->tone_phase);
	this->playing = format;
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

	switch (playing) {
	case 0x81: // 16 bit PCM, 44100 Hz
		int16_t* waveform = (int16_t*)this->waveform;
		for (int i = 0; i < AUDIO_BLOCK_SAMPLES; i++) {
			index = tone_phase >> length_bits;
			scale = (tone_phase << (32-17)) >> 16;//length_bits) >> 16;
			switch (length - index) {
			case 0:
				tone_phase -= index;
				s1 = waveform[0]; s2 = waveform[1];
				break;
			case 1:
				s1 = waveform[index]; s2 = waveform[0];
				break;
			default:
				s1 = waveform[index]; s2 = waveform[index+1];
				break;
			}
			v2 = s2 * scale;
			v1 = s1 * (0xFFFF - scale);
			v3 = (v1 + v2) >> 16;
			*out++ = (int16_t)v3;
			//*out++ = (int16_t)((v3 * tone_amp) >> 16);
			tone_phase += tone_incr;
		}
		break;
	}

	transmit(block);
	release(block);
}

void AudioSynthWavetable::frequency(float freq) {
	if (freq < 0.0)
		freq = 0.0;
	else if (freq > AUDIO_SAMPLE_RATE_EXACT / 2)
		freq = AUDIO_SAMPLE_RATE_EXACT / 2;

	tone_incr = freq * (0x80000000LL / AUDIO_SAMPLE_RATE_EXACT) + 0.5;
}
