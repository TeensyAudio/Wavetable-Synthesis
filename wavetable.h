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

#ifndef wavetable_h_
#define wavetable_h_

#include "Arduino.h"
#include "AudioStream.h"

class AudioWavetable : public AudioStream
{
public:
	public:
	AudioWavetable(void) : AudioStream(0, NULL), playing(0) {
		tone_amp = 400;
	}
	void play(const unsigned int *data);
	void stop(void);
	bool isPlaying(void) { return playing; }
	virtual void update(void);
	void frequency(float t_freq) {
		if (t_freq < 0.0) t_freq = 0.0;
		else if (t_freq > AUDIO_SAMPLE_RATE_EXACT / 2) t_freq = AUDIO_SAMPLE_RATE_EXACT / 2;
		tone_incr = (t_freq * (0x80000000LL/AUDIO_SAMPLE_RATE_EXACT)) + 0.5;
	}
private:
	const unsigned int *next;
	const unsigned int *beginning;
	uint32_t length;
	int16_t prior;
	volatile uint8_t playing;
	
	uint32_t tone_phase;
	volatile uint32_t tone_incr;
	short    tone_amp;
	short    tone_freq;
};

#endif
