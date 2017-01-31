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

#ifndef effect_envelope_test_h_
#define effect_envelope_test_h_
#include "Arduino.h"
#include "AudioStream.h"
//#include "dspinst.h"

#define MAX_MS 11000.0
#define UNITY_GAIN 65536.0
#define SAMPLES_PER_MSEC (AUDIO_SAMPLE_RATE_EXACT/1000.0)

class AudioEffectEnvelopeTest : public AudioStream
{
public:
	AudioEffectEnvelopeTest() : AudioStream(1, inputQueueArray) {
		state = 0;
		delay(0.0);  // default values...
		attack(1.5);
		hold(0.5);
		decay(15.0);
		sustain(0.667);
		release(30.0);
	}
	void noteOn();
	void noteOff();
	void delay(float milliseconds) {
		delay_count = milliseconds2count(milliseconds);
	}
	void attack(float milliseconds) {
		attack_count = milliseconds2count(milliseconds);
	}
	void hold(float milliseconds) {
		hold_count = milliseconds2count(milliseconds);
	}
	void decay(float milliseconds) {
		decay_count = milliseconds2count(milliseconds);
	}
	void sustain(float level) {
		if (level < 0.0) level = 0;
		else if (level > 1.0) level = 1.0;
		sustain_mult = level * UNITY_GAIN;
	}
	void release(float milliseconds) {
		release_count = milliseconds2count(milliseconds);
	}
	using AudioStream::release;
	virtual void update(void);
    bool isIdle();
    
private:
	uint16_t milliseconds2count(float milliseconds) {
		if (milliseconds < 0.0) milliseconds = 0.0;
        if (milliseconds > MAX_MS) milliseconds = MAX_MS;
        // # of 8-sample units to process
        // Add 7 to round up
        return ((uint32_t)(milliseconds*SAMPLES_PER_MSEC)+7)>>3;
	}
    int32_t signed_multiply_32x16b(int32_t a, uint32_t b) {
        return ((int64_t)a * (int16_t)(b & 0xFFFF)) >> 16;
    }
    int32_t signed_multiply_32x16t(int32_t a, uint32_t b) {
        return ((int64_t)a * (int16_t)(b >> 16)) >> 16;
    }
    uint32_t pack_16b_16b(int32_t a, int32_t b) {
        return (a << 16) | (b & 0x0000FFFF);
    }
    
	audio_block_t *inputQueueArray[1];
	// state
	uint8_t  state;  // idle, delay, attack, hold, decay, sustain, release
	uint16_t count;  // how much time remains in this state, in 8 sample units
    float    mult;   // attenuation, 0=off, 0x10000=unity gain
    float    inc;    // amount to change mult on each sample
	// settings
	uint16_t delay_count;
	uint16_t attack_count;
	uint16_t hold_count;
	uint16_t decay_count;
	int32_t  sustain_mult;
	uint16_t release_count;
};

#undef SAMPLES_PER_MSEC
#endif
