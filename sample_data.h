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
