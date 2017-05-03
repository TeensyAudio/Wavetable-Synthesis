#pragma once
#include <AudioStream.h>
#include <AudioSynthWavetable.h>

extern const sample_data piano_samples[1];
const uint8_t piano_ranges[] = {127, };

const instrument_data piano = {1, piano_ranges, piano_samples };


extern const uint32_t sample_0_piano_kpianob4[12672];
