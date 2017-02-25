#pragma once
#include <stdint.h>


struct sample_data {
	const int INDEX_BITS;
	const float PER_HERTZ_PHASE_INCREMENT;
	const uint32_t MAX_PHASE;
	const uint32_t LOOP_PHASE_END;
	const uint32_t LOOP_PHASE_LENGTH;
	const uint32_t DELAY_COUNT;
	const uint32_t ATTACK_COUNT;
	const uint32_t HOLD_COUNT;
	const uint32_t DECAY_COUNT;
	const uint32_t RELEASE_COUNT;
	const uint32_t SUSTAIN_MULT;

	const int ORIGINAL_PITCH;
	const float CENTS_OFFSET;
	const int SAMPLE_LENGTH;
	const float SAMPLE_RATE_COEFFICIENT;
	const int LOOP_START;
	const int LOOP_END;
	const int VELOCITY_RANGE_1;
	const int VELOCITY_RANGE_2;
	const int DELAY_ENV;
	const int ATTACK_ENV;
	const int HOLD_ENV;
	const int DECAY_ENV;
	const int SUSTAIN_ENV;
	const int RELEASE_ENV;
	const int16_t* sample;
};

struct instrument_data {
	const int sample_count;
	const int* sample_note_ranges;
	const sample_data* samples;
};
