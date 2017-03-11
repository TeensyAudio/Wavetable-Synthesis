#pragma once
#include <AudioStream.h>
#include <AudioSynthWavetable.h>

extern const sample_data vibraphone_samples[1];
const uint8_t vibraphone_ranges[] = {127, };

const instrument_data vibraphone = {1, vibraphone_ranges, vibraphone_samples };


extern const uint32_t sample_0_vibraphone_xylo1[20480];
