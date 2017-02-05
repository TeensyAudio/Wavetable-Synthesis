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

#pragma once

#include "Arduino.h"
#include "AudioStream.h"
#include <math.h>

#define MAX_MS 11000.0      // Max section length (milliseconds)
#define UNITY_GAIN 65536.0  // Max amplitude
#define SAMPLES_PER_MSEC (AUDIO_SAMPLE_RATE_EXACT/1000.0)

class AudioSynthWavetable : public AudioStream
{
public:
	AudioSynthWavetable(void)
		: AudioStream(0, NULL)
		, waveform(NULL)
		, num_samples(0)
		, length(0)
		, length_bits(0)
		, sample_freq(440.0)
		, playing(0)
		, tone_phase(0)
		, loop_phase(0)
		, max_phase(0)
		, tone_incr(0)
		, tone_amp(0)
		, loop_start(0)
		, loop_end(0)
		, loop_start_phase(0)
		, loop_end_phase(0)
	{}

	void setSamples(const unsigned int ** samples);
	void setLoop(int start, int end) {
		loop_start = start;
		loop_end = end;
		loop_length = loop_end - loop_start;
		
		length_bits = 1;
		for (int len = loop_length; len >>= 1; ++length_bits);
		loop_phase = (loop_length - 1) << (32 - length_bits);
	}
	
	void play(void);
	void playFrequency(float freq);
	void playNote(int note, int amp=63);
	void stop(void);
	bool isPlaying(void);
	void frequency(float freq);
	void parseSample(int sample_num);
	
	uint32_t getNoteRange(int sample_num);
	void setFreqAmp(float freq, float amp) {
		frequency(freq);
		amplitude(amp);
	}
	
	void setSampleNote(int note) {
		sample_freq = noteToFreq(note);
	}

	void amplitude(float v) {
		v = (v < 0.0) ? 0.0 : (v > 1.0) ? 1.0 : v;
		tone_amp = (uint16_t)(32767.0*v);
	}

	float midi_volume_transform(int midi_amp) {
		// 4 approximates a logarithmic taper for the volume
		// however, we might need to play with this value
		// if people think the volume is too quite at low
		// input amplitudes
		int logarithmicness = 4;

		// scale midi_amp which is 0 t0 127 to be between
		// 0 and 1 using a logarithmic transformation
		return (float)pow(midi_amp, logarithmicness) /
			(float)pow(127, logarithmicness);
	}
	
	static float noteToFreq(int note) {
		return 27.5 * pow(2, (float)(note - 21)/12);
	}
	
	void env_delay(float milliseconds) {
		delay_count = milliseconds2count(milliseconds);
	}
	void env_attack(float milliseconds) {
		if (milliseconds <= 0) {
			milliseconds = 1.5;
		}
		attack_count = milliseconds2count(milliseconds);
	}
	void env_hold(float milliseconds) {
		if (milliseconds <= 0) {
			milliseconds = 0.5;
		}
		hold_count = milliseconds2count(milliseconds);
	}
	void env_decay(float milliseconds) {
		decay_count = milliseconds2count(milliseconds);
	}
	void env_sustain(float level) {
		if (level < 0.0) level = 0;
		else if (level > 1.0) level = 1.0;
		sustain_mult = level * UNITY_GAIN;
	}
	void env_release(float milliseconds) {
		release_count = milliseconds2count(milliseconds);
	}
	
	virtual void update(void);

private:
	uint32_t* waveform;
	const unsigned int ** samples;
	int length, length_bits, loop_start, loop_end, loop_length;
	float sample_freq;
	uint8_t playing, num_samples;
	uint32_t tone_phase, loop_phase, loop_start_phase, loop_end_phase;
	uint32_t max_phase;
	uint32_t tone_incr;
	uint16_t tone_amp, sample_rate;
	
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
