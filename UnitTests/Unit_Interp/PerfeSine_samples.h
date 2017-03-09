#pragma once
#include <AudioStream.h>
#include <AudioSynthWavetable.h>

extern const sample_data PerfeSine_samples[7];
const uint8_t PerfeSine_ranges[] = {51, 58, 63, 66, 72, 82, 127, };

const instrument_data PerfeSine = {7, PerfeSine_ranges, PerfeSine_samples };


extern const uint32_t sample_0_PerfeSine_PerfeSine048L[22144];

extern const uint32_t sample_1_PerfeSine_PerfeSine054L[22144];

extern const uint32_t sample_2_PerfeSine_PerfeSine060L[22144];

extern const uint32_t sample_3_PerfeSine_PerfeSine066L[22144];

extern const uint32_t sample_4_PerfeSine_PerfeSine072L[22144];

extern const uint32_t sample_5_PerfeSine_PerfeSine078L[18304];

extern const uint32_t sample_6_PerfeSine_PerfeSine084L[13696];
