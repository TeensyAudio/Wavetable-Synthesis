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
	int32_t length_temp;
	uint32_t format;
	tone_phase = 0;
	playing = 0;
	format = *data++;
	length_temp = length = format & 0xFFFFFF;
   length = 8000;
	uint8_t length_bits = 1;
	while (length_temp >>= 1) ++length_bits;
	this->waveform = (uint32_t*)data;
	this->playing = format >> 24;
   
   if(playing & 0x80) { //16-bit
      sample_count = length * 2;
      max_phase = sample_count << 16;
   } else { //8-bit
      sample_count = length * 4;
      max_phase = sample_count << 16;
   }
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
			//tone_phase = tone_phase > max_tone_phase ? tone_phase - max_tone_phase : tone_phase;
			//index = tone_phase >> 32 - sample_count_magnitude; //enough bits to hold all index values for array
			//val1 = arbdata[index];
			//++index;
			//index = index == sample_count ? 0 : index; //loops if at end of array
			//val2 = arbdata[index];
			//scale = (tone_phase >> 32 - sample_count_mag - 16) & 0xFFFF;
			//val2 *= scale;
			//val1 *= 0xFFFF - scale;
			//val3 = (val1 + val2) >> 16;
			//*bp++ = (short)((val3 * tone_amp) >> 15);

			index = tone_phase >> 16;
			s1 = waveform[index];
			index++;
         index = index == sample_count ? 0 : index; //loops if at end of array
			s2 = waveform[index];
			scale = tone_phase & 0xFFFF;
			v2 = s2 * scale;
			v1 = s1 * (0xFFFF - scale);
			v3 = (v1 + v2) >> 16;
			*out++ = (int16_t)(v3);

			tone_phase += tone_incr;
         if(tone_phase >= max_phase) {
            tone_phase -= max_phase;
         }
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
