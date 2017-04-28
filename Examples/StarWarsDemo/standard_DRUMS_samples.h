#pragma once
#include <AudioStream.h>
#include <AudioSynthWavetable.h>

extern const sample_data standard_DRUMS_samples[5];
const uint8_t standard_DRUMS_ranges[] = {37, 44, 55, 70, 127, };

const instrument_data standard_DRUMS = {5, standard_DRUMS_ranges, standard_DRUMS_samples };


extern const uint32_t sample_0_standard_DRUMS_bd15[896];

extern const uint32_t sample_1_standard_DRUMS_snare24[2048];

extern const uint32_t sample_2_standard_DRUMS_paisteping[6656];

extern const uint32_t sample_3_standard_DRUMS_paisteping[6656];

extern const uint32_t sample_4_standard_DRUMS_triangle[1152];
