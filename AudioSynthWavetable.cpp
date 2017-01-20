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

void AudioSynthWavetable::play(const unsigned int *data)
{
    uint32_t length_temp;
	uint32_t format;
	tone_phase = 0;
	playing = 0;
	prior = 0;
    format = *data++;
	next = data;
	beginning = data;
	length_temp = length = format & 0xFFFFFF;
	uint8_t length_bits = 1;
	while (length_temp >>= 1) ++length_bits;
	playing = format >> 24;
	
	//Can update this value to produce a different note.
	//This value just plays back as normal
	//tone_incr = 0x00010000;
	
	
	//tone_amp = (uint16_t)(32767.0*.5);
}
 
void AudioSynthWavetable::stop(void)
{
	playing = 0;
}

void AudioSynthWavetable::update(void)
{
	audio_block_t *block;
	const unsigned int *in;
	int16_t *out;
	uint32_t tmp32, consumed;
	uint32_t index, scale;
	int16_t s0, s1, s2, s3;
	uint32_t v1, v2, v3;
	int16_t* waveform = (int16_t*)beginning;
	int i;

	if (!playing) return;
	block = allocate();
	if (block == NULL) return;

	out = block->data;
	in = next;
	s0 = prior;

	switch (playing) {
	  case 0x81: // 16 bit PCM, 44100 Hz
		for (i=0; i < AUDIO_BLOCK_SAMPLES; i++) {
			index = tone_phase >> 16;
			s1 = waveform[index];
			s2 = waveform[++index];
			scale = tone_phase & 0xFFFF;
			v2 = s2 * scale;
			v1 = s1 * (0xFFFF - scale);
			v3 = (v1 + v2) >> 16;
			*out++ = (int16_t)(v3);
			tone_phase += tone_incr;
		}
		//consumed = 128 * (tone_incr >> 16);
		break;
	}
	prior = s0;
	next = in;
	transmit(block);
	release(block);
}

