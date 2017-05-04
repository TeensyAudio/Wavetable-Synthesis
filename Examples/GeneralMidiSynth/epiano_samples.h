#pragma once
#include <AudioStream.h>
#include <AudioSynthWavetable.h>

extern const sample_data epiano_samples[1];
const uint8_t epiano_ranges[] = {127, };

const instrument_data epiano = {1, epiano_ranges, epiano_samples };


extern const uint32_t sample_0_epiano_epiano2ms[640];
