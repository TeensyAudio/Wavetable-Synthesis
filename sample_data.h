#pragma once
#include <stdint.h>


struct sample_data {
	const bool LOOP;
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
	const int32_t SUSTAIN_MULT;
	const int16_t* sample;
};

struct instrument_data {
	const int sample_count;
	const int* sample_note_ranges;
	const sample_data* samples;
};
