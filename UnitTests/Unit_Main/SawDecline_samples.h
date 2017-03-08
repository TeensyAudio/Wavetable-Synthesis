#pragma once
#include <AudioStream.h>
#include <AudioSynthWavetable.h>

extern const sample_data SawDecline_samples[8];
const uint8_t SawDecline_ranges[] = {31, 43, 55, 67, 79, 91, 103, 127, };

const instrument_data SawDecline = {8, SawDecline_ranges, SawDecline_samples };


extern const uint32_t sample_0_SawDecline_SawDecline9375Hz[512];

extern const uint32_t sample_1_SawDecline_SawDecline1875Hz[256];

extern const uint32_t sample_2_SawDecline_SawDecline375Hz[128];

extern const uint32_t sample_3_SawDecline_SawDecline750Hz[128];

extern const uint32_t sample_4_SawDecline_SawDecline1500Hz[128];

extern const uint32_t sample_5_SawDecline_SawDecline3000Hz[128];

extern const uint32_t sample_6_SawDecline_SawDecline6000Hz[128];

extern const uint32_t sample_7_SawDecline_SawDecline12000Hz[128];
