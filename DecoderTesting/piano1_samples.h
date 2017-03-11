#pragma once
#include <AudioStream.h>
#include <AudioSynthWavetable.h>

extern const sample_data piano1_samples[1];
const uint8_t piano1_ranges[] = {127, };

const instrument_data piano1 = {1, piano1_ranges, piano1_samples };


extern const uint32_t sample_0_piano1_kpianob1[8704];
