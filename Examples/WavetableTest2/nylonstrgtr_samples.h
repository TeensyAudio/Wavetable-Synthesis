#pragma once
#include <AudioStream.h>
#include <AudioSynthWavetable.h>

extern const sample_data nylonstrgtr_samples[3];
extern const uint32_t nylonstrgtr_sample_0_nguitrf2[1920];
extern const uint32_t nylonstrgtr_sample_1_nguitb2[2688];
extern const uint32_t nylonstrgtr_sample_2_acgtrb3[3200];

const int nylonstrgtr_ranges[] = { 56, 77, 128 };

const instrument_data nylonstrgtr = { 3, nylonstrgtr_ranges, nylonstrgtr_samples };

//to implement in decoder:
// - output of range array
// - output of instrument_data
// - removal of note range from sample_data