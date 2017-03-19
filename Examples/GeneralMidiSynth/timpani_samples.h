#pragma once
#include <AudioStream.h>
#include <AudioSynthWavetable.h>

extern const sample_data timpani_samples[4];
const uint8_t timpani_ranges[] = {48, 53, 65, 127, };

const instrument_data timpani = {4, timpani_ranges, timpani_samples };


extern const uint32_t sample_0_timpani_timpani[3968];

extern const uint32_t sample_1_timpani_timpani[3968];

extern const uint32_t sample_2_timpani_timpani[3968];

extern const uint32_t sample_3_timpani_timpani[3968];
