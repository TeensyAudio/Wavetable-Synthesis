#pragma once
#include <AudioStream.h>
#include <AudioSynthWavetable.h>

extern const sample_data strings_samples[3];
const uint8_t strings_ranges[] = {66, 115, 127, };

const instrument_data strings = {3, strings_ranges, strings_samples };


extern const uint32_t sample_0_strings_stringsg2[4736];

extern const uint32_t sample_1_strings_stringsdx4[5376];

extern const uint32_t sample_2_strings_kpianob5[2816];
