#pragma once
#include <stdint.h>
#include <AudioSynthWavetable.h>

extern const uint32_t steelstrgtr_sample_0_acgtrg2[2560];
extern const uint32_t steelstrgtr_sample_1_acgtrb3[3200];
extern const uint32_t steelstrgtr_sample_2_kpianob5[2816];

sample_data steelstrgtr[3] = {
	{
		74,
		4881,
		44100,
		4800,
		4877,
		0,
		72,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		steelstrgtr_sample_0_acgtrg2
	}, {
		76,
		6240,
		44100,
		6168,
		6236,
		73,
		97,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		steelstrgtr_sample_1_acgtrb3
	}, {
		111,
		5398,
		44100,
		2963,
		5394,
		98,
		127,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		steelstrgtr_sample_2_kpianob5
	}
};
