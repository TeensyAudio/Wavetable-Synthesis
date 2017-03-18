/* Audio Library for Teensy 3.X
 * Copyright (c) 2017, TeensyAudio PSU Team
 *
 * Development of this audio library was sponsored by PJRC.COM, LLC.
 * Please support PJRC's efforts to develop open source 
 * software by purchasing Teensy or other PJRC products.
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
#include <stdint.h>

#define CENTS_SHIFT(C) (pow(2.0, C/1200.0))
#define NOTE(N) (440.0 * pow(2.0, (N - 69) / 12.0))
#define DECIBEL_SHIFT(dB) (pow(10.0, dB/20.0))

struct sample_data {
	// SAMPLE VALUES
	const int16_t* sample;
	const bool LOOP;
	const int INDEX_BITS;
	const float PER_HERTZ_PHASE_INCREMENT;
	const uint32_t MAX_PHASE;
	const uint32_t LOOP_PHASE_END;
	const uint32_t LOOP_PHASE_LENGTH;
	const uint16_t INITIAL_ATTENUATION_SCALAR;
	
	// VOLUME ENVELOPE VALUES
	const uint32_t DELAY_COUNT;
	const uint32_t ATTACK_COUNT;
	const uint32_t HOLD_COUNT;
	const uint32_t DECAY_COUNT;
	const uint32_t RELEASE_COUNT;
	const int32_t SUSTAIN_MULT;

	// VIRBRATO VALUES
	const uint32_t VIBRATO_DELAY;
	const uint32_t VIBRATO_INCREMENT;
	const float VIBRATO_PITCH_COEFFICIENT_INITIAL;
	const float VIBRATO_PITCH_COEFFICIENT_SECOND;

	// MODULATION VALUES
	const uint32_t MODULATION_DELAY;
	const uint32_t MODULATION_INCREMENT;
	const float MODULATION_PITCH_COEFFICIENT_INITIAL;
	const float MODULATION_PITCH_COEFFICIENT_SECOND;
	const int32_t MODULATION_AMPLITUDE_INITIAL_GAIN;
	const int32_t MODULATION_AMPLITUDE_SECOND_GAIN;
};

struct instrument_data {
	const uint8_t sample_count;
	const uint8_t* sample_note_ranges;
	const sample_data* samples;
};
