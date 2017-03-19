#pragma once
#include <AudioStream.h>
#include <AudioSynthWavetable.h>

extern const sample_data standard_DRUMS_samples[6];
const uint8_t standard_DRUMS_ranges[] = {38, 40, 41, 45, 51, 127, };

const instrument_data standard_DRUMS = {6, standard_DRUMS_ranges, standard_DRUMS_samples };


extern const uint32_t sample_0_standard_DRUMS_bd15[896];

extern const uint32_t sample_1_standard_DRUMS_snare24[2048];

extern const uint32_t sample_2_standard_DRUMS_floortombrite[3712];

extern const uint32_t sample_3_standard_DRUMS_hatopenms[5888];

extern const uint32_t sample_4_standard_DRUMS_crash5[6784];

extern const uint32_t sample_5_standard_DRUMS_tamborine[1920];
