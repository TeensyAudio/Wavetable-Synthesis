#pragma once
#include <stdint.h>

struct sample_data{
	const int ORIGINAL_PITCH;
	const float CENTS_OFFSET;
	const int SAMPLE_LENGTH;
	const int SAMPLE_RATE;
	const int LOOP_START;
	const int LOOP_END;
	const int NOTE_RANGE_1;
	const int NOTE_RANGE_2;
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
