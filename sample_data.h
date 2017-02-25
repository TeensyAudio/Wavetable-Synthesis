#pragma once
#include <stdint.h>

struct sample_data {
	const int INDEX_BITS;
	const float PER_HERTZ_PHASE_INCREMENT;
	const int MAX_PHASE;
	const int LOOP_PHASE_END;
	const int LOOP_PHASE_LENGTH;

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
	const uint32_t* sample;
};

struct instrument_data {
	const int sample_count;
	const int* sample_note_ranges;
	const sample_data* samples;
};
