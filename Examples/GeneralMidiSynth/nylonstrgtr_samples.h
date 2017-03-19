#pragma once
#include <AudioStream.h>
#include <AudioSynthWavetable.h>

extern const sample_data nylonstrgtr_samples[2];
const uint8_t nylonstrgtr_ranges[] = {56, 127, };

const instrument_data nylonstrgtr = {2, nylonstrgtr_ranges, nylonstrgtr_samples };


extern const uint32_t sample_0_nylonstrgtr_nguitrf2[1920];

extern const uint32_t sample_1_nylonstrgtr_nguitb2[2688];
