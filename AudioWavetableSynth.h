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

class AudioWavetableSynth : public AudioStream
{
public:
	public:
	AudioWavetableSynth(void) : AudioStream(0, NULL), playing(0) {
		tone_phase = 0;
        tone_incr = 0;
        tone_amp = 0;
	}
	void play(const unsigned int *data);
	void stop(void);
	bool isPlaying(void) { return playing; }
	virtual void update(void);
    void begin(float freq, float amp) {
        fundamental(freq);
        amplitude(amp);
    }

    void fundamental(float freq) {
        if (freq < 0.0) {
            freq = 0.0;
        } else if (freq > AUDIO_SAMPLE_RATE_EXACT / 2) {
            freq = AUDIO_SAMPLE_RATE_EXACT / 2;
        }
		
        tone_incr = freq * (0x80000000LL/AUDIO_SAMPLE_RATE_EXACT) + 0.5;
	}

    void amplitude(float v) {
        if (v < 0.0) {
            v = 0.0;
        } else if (v > 1.0) {
            v = 1.0;
        }

        if ((tone_amp == 0) && v) {
            tone_phase = 0;
        }

        tone_amp = (uint16_t)(32767.0*v);
    }
private:
	const unsigned int *next;
	const unsigned int *beginning;
	uint32_t length;
	int16_t prior;
	volatile uint8_t playing;
	
	uint32_t tone_phase;
	volatile uint32_t tone_incr;
	uint16_t tone_amp;
	short    tone_freq;
};

#endif
