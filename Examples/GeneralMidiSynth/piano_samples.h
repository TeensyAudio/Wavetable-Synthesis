#pragma once
#include <AudioStream.h>
#include <AudioSynthWavetable.h>

extern const sample_data piano_samples[3];
const uint8_t piano_ranges[] = {62, 79, 127, };

const instrument_data piano = {3, piano_ranges, piano_samples };


extern const uint32_t sample_0_piano_kpianog2[11136];

extern const uint32_t sample_1_piano_kpianob4[12672];

extern const uint32_t sample_2_piano_kpianodx5[3200];
