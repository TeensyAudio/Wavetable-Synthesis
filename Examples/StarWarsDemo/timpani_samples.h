#pragma once
#include <AudioStream.h>
#include <AudioSynthWavetable.h>

extern const sample_data timpani_samples[7];
const uint8_t timpani_ranges[] = {47, 48, 51, 55, 59, 71, 127, };

const instrument_data timpani = {7, timpani_ranges, timpani_samples };


extern const uint32_t sample_0_timpani_timpani[3968];

extern const uint32_t sample_1_timpani_timpani[3968];

extern const uint32_t sample_2_timpani_timpani[3968];

extern const uint32_t sample_3_timpani_timpani[3968];

extern const uint32_t sample_4_timpani_timpani[3968];

extern const uint32_t sample_5_timpani_timpani[3968];

extern const uint32_t sample_6_timpani_timpani[3968];
