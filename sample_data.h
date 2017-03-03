#pragma once
#include <stdint.h>


struct sample_data {
	//SAMPLE
	const bool LOOP;
	const int INDEX_BITS;
	const float PER_HERTZ_PHASE_INCREMENT;
	const uint32_t MAX_PHASE;
	const uint32_t LOOP_PHASE_END;
	const uint32_t LOOP_PHASE_LENGTH;
	const int16_t* sample;
	
	//VOLUME ENVELOPE
	const uint32_t DELAY_COUNT;
	const uint32_t ATTACK_COUNT;
	const uint32_t HOLD_COUNT;
	const uint32_t DECAY_COUNT;
	const uint32_t RELEASE_COUNT;
	const int32_t SUSTAIN_MULT;

	//VIRBRATO
	const uint32_t VIBRATO_DELAY;
	const uint32_t VIBRATO_INCREMENT;
	const float VIBRATO_COEFFICIENT_HIGH;
	const float VIBRATO_COEFFICIENT_LOW;
	const int VIBRATO_START_SIGN;
};

struct instrument_data {
	const int sample_count;
	const int* sample_note_ranges;
	const sample_data* samples;
};
