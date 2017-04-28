#include "trombone_samples.h"
const sample_data trombone_samples[4] = {
	{
		(int16_t*)sample_0_trombone_tromb2, // sample
		true, // LOOP
		11, // LENGTH_BITS
		(1 << (32 - 11)) * CENTS_SHIFT(-7) * 44100.0 / NOTE(70) / AUDIO_SAMPLE_RATE_EXACT + 0.5, // PER_HERTZ_PHASE_INCREMENT
		((uint32_t)1330 - 1) << (32 - 11), // MAX_PHASE
		((uint32_t)1326 - 1) << (32 - 11), // LOOP_PHASE_END
		(((uint32_t)1326 - 1) << (32 - 11)) - (((uint32_t)1232 - 1) << (32 - 11)), // LOOP_PHASE_LENGTH
		uint16_t(UINT16_MAX * DECIBEL_SHIFT(-0.0)), // INITIAL_ATTENUATION_SCALAR
		RATE_NORMALIZED_ENV_COUNT(0.00), // DELAY_COUNT
		RATE_NORMALIZED_ENV_COUNT(6.00), // ATTACK_COUNT
		RATE_NORMALIZED_ENV_COUNT(20.01), // HOLD_COUNT
		RATE_NORMALIZED_ENV_COUNT(11993.38), // DECAY_COUNT
		RATE_NORMALIZED_ENV_COUNT(536.20), // RELEASE_COUNT
		NEG_INV_COUNT(536.20), // RELEASE_COUNT
		SUSTAIN_DROP(2.3, 11993.38), // SUSTAIN_MULT
		LFO_NORMALIZED_DELAY_COUNT(0.00), // VIBRATO_DELAY
		uint32_t(0.1 * LFO_PERIOD * (UINT32_MAX / AUDIO_SAMPLE_RATE_EXACT)), // VIBRATO_INCREMENT
		(CENTS_SHIFT(0) - 1.0) * 4, // VIBRATO_PITCH_COEFFICIENT_INITIAL
		(1.0 - CENTS_SHIFT(0)) * 4, // VIBRATO_COEFFICIENT_SECONDARY
		LFO_NORMALIZED_DELAY_COUNT(260.01), // MODULATION_DELAY
		uint32_t(5.0 * LFO_PERIOD * (UINT32_MAX / AUDIO_SAMPLE_RATE_EXACT)), // MODULATION_INCREMENT
		(CENTS_SHIFT(9) - 1.0) * 4, // MODULATION_PITCH_COEFFICIENT_INITIAL
		(1.0 - CENTS_SHIFT(-9)) * 4, // MODULATION_PITCH_COEFFICIENT_SECOND
		int32_t(UINT16_MAX * (DECIBEL_SHIFT(0) - 1.0)) * 4, // MODULATION_AMPLITUDE_INITIAL_GAIN
		int32_t(UINT16_MAX * (1.0 - DECIBEL_SHIFT(0))) * 4, // MODULATION_AMPLITUDE_FINAL_GAIN
	},
	{
		(int16_t*)sample_1_trombone_troma3, // sample
		true, // LOOP
		11, // LENGTH_BITS
		(1 << (32 - 11)) * CENTS_SHIFT(33) * 44100.0 / NOTE(80) / AUDIO_SAMPLE_RATE_EXACT + 0.5, // PER_HERTZ_PHASE_INCREMENT
		((uint32_t)1333 - 1) << (32 - 11), // MAX_PHASE
		((uint32_t)1329 - 1) << (32 - 11), // LOOP_PHASE_END
		(((uint32_t)1329 - 1) << (32 - 11)) - (((uint32_t)1275 - 1) << (32 - 11)), // LOOP_PHASE_LENGTH
		uint16_t(UINT16_MAX * DECIBEL_SHIFT(-0.0)), // INITIAL_ATTENUATION_SCALAR
		RATE_NORMALIZED_ENV_COUNT(0.00), // DELAY_COUNT
		RATE_NORMALIZED_ENV_COUNT(6.00), // ATTACK_COUNT
		RATE_NORMALIZED_ENV_COUNT(20.01), // HOLD_COUNT
		RATE_NORMALIZED_ENV_COUNT(11993.38), // DECAY_COUNT
		RATE_NORMALIZED_ENV_COUNT(536.20), // RELEASE_COUNT
		NEG_INV_COUNT(536.20), // RELEASE_COUNT
		SUSTAIN_DROP(2.3, 11993.38), // SUSTAIN_MULT
		LFO_NORMALIZED_DELAY_COUNT(0.00), // VIBRATO_DELAY
		uint32_t(0.1 * LFO_PERIOD * (UINT32_MAX / AUDIO_SAMPLE_RATE_EXACT)), // VIBRATO_INCREMENT
		(CENTS_SHIFT(0) - 1.0) * 4, // VIBRATO_PITCH_COEFFICIENT_INITIAL
		(1.0 - CENTS_SHIFT(0)) * 4, // VIBRATO_COEFFICIENT_SECONDARY
		LFO_NORMALIZED_DELAY_COUNT(260.01), // MODULATION_DELAY
		uint32_t(5.0 * LFO_PERIOD * (UINT32_MAX / AUDIO_SAMPLE_RATE_EXACT)), // MODULATION_INCREMENT
		(CENTS_SHIFT(9) - 1.0) * 4, // MODULATION_PITCH_COEFFICIENT_INITIAL
		(1.0 - CENTS_SHIFT(-9)) * 4, // MODULATION_PITCH_COEFFICIENT_SECOND
		int32_t(UINT16_MAX * (DECIBEL_SHIFT(0) - 1.0)) * 4, // MODULATION_AMPLITUDE_INITIAL_GAIN
		int32_t(UINT16_MAX * (1.0 - DECIBEL_SHIFT(0))) * 4, // MODULATION_AMPLITUDE_FINAL_GAIN
	},
	{
		(int16_t*)sample_2_trombone_tromd4, // sample
		true, // LOOP
		11, // LENGTH_BITS
		(1 << (32 - 11)) * CENTS_SHIFT(-3) * 44100.0 / NOTE(84) / AUDIO_SAMPLE_RATE_EXACT + 0.5, // PER_HERTZ_PHASE_INCREMENT
		((uint32_t)1120 - 1) << (32 - 11), // MAX_PHASE
		((uint32_t)1116 - 1) << (32 - 11), // LOOP_PHASE_END
		(((uint32_t)1116 - 1) << (32 - 11)) - (((uint32_t)1074 - 1) << (32 - 11)), // LOOP_PHASE_LENGTH
		uint16_t(UINT16_MAX * DECIBEL_SHIFT(-0.0)), // INITIAL_ATTENUATION_SCALAR
		RATE_NORMALIZED_ENV_COUNT(0.00), // DELAY_COUNT
		RATE_NORMALIZED_ENV_COUNT(6.00), // ATTACK_COUNT
		RATE_NORMALIZED_ENV_COUNT(20.01), // HOLD_COUNT
		RATE_NORMALIZED_ENV_COUNT(11993.38), // DECAY_COUNT
		RATE_NORMALIZED_ENV_COUNT(536.20), // RELEASE_COUNT
		NEG_INV_COUNT(536.20), // RELEASE_COUNT
		SUSTAIN_DROP(2.3, 11993.38), // SUSTAIN_MULT
		LFO_NORMALIZED_DELAY_COUNT(0.00), // VIBRATO_DELAY
		uint32_t(0.1 * LFO_PERIOD * (UINT32_MAX / AUDIO_SAMPLE_RATE_EXACT)), // VIBRATO_INCREMENT
		(CENTS_SHIFT(0) - 1.0) * 4, // VIBRATO_PITCH_COEFFICIENT_INITIAL
		(1.0 - CENTS_SHIFT(0)) * 4, // VIBRATO_COEFFICIENT_SECONDARY
		LFO_NORMALIZED_DELAY_COUNT(260.01), // MODULATION_DELAY
		uint32_t(5.0 * LFO_PERIOD * (UINT32_MAX / AUDIO_SAMPLE_RATE_EXACT)), // MODULATION_INCREMENT
		(CENTS_SHIFT(9) - 1.0) * 4, // MODULATION_PITCH_COEFFICIENT_INITIAL
		(1.0 - CENTS_SHIFT(-9)) * 4, // MODULATION_PITCH_COEFFICIENT_SECOND
		int32_t(UINT16_MAX * (DECIBEL_SHIFT(0) - 1.0)) * 4, // MODULATION_AMPLITUDE_INITIAL_GAIN
		int32_t(UINT16_MAX * (1.0 - DECIBEL_SHIFT(0))) * 4, // MODULATION_AMPLITUDE_FINAL_GAIN
	},
	{
		(int16_t*)sample_3_trombone_tromg4, // sample
		true, // LOOP
		11, // LENGTH_BITS
		(1 << (32 - 11)) * CENTS_SHIFT(16) * 44100.0 / NOTE(90) / AUDIO_SAMPLE_RATE_EXACT + 0.5, // PER_HERTZ_PHASE_INCREMENT
		((uint32_t)1568 - 1) << (32 - 11), // MAX_PHASE
		((uint32_t)1564 - 1) << (32 - 11), // LOOP_PHASE_END
		(((uint32_t)1564 - 1) << (32 - 11)) - (((uint32_t)1504 - 1) << (32 - 11)), // LOOP_PHASE_LENGTH
		uint16_t(UINT16_MAX * DECIBEL_SHIFT(-0.0)), // INITIAL_ATTENUATION_SCALAR
		RATE_NORMALIZED_ENV_COUNT(0.00), // DELAY_COUNT
		RATE_NORMALIZED_ENV_COUNT(6.00), // ATTACK_COUNT
		RATE_NORMALIZED_ENV_COUNT(20.01), // HOLD_COUNT
		RATE_NORMALIZED_ENV_COUNT(11993.38), // DECAY_COUNT
		RATE_NORMALIZED_ENV_COUNT(536.20), // RELEASE_COUNT
		NEG_INV_COUNT(536.20), // RELEASE_COUNT
		SUSTAIN_DROP(2.3, 11993.38), // SUSTAIN_MULT
		LFO_NORMALIZED_DELAY_COUNT(0.00), // VIBRATO_DELAY
		uint32_t(0.1 * LFO_PERIOD * (UINT32_MAX / AUDIO_SAMPLE_RATE_EXACT)), // VIBRATO_INCREMENT
		(CENTS_SHIFT(0) - 1.0) * 4, // VIBRATO_PITCH_COEFFICIENT_INITIAL
		(1.0 - CENTS_SHIFT(0)) * 4, // VIBRATO_COEFFICIENT_SECONDARY
		LFO_NORMALIZED_DELAY_COUNT(260.01), // MODULATION_DELAY
		uint32_t(5.0 * LFO_PERIOD * (UINT32_MAX / AUDIO_SAMPLE_RATE_EXACT)), // MODULATION_INCREMENT
		(CENTS_SHIFT(9) - 1.0) * 4, // MODULATION_PITCH_COEFFICIENT_INITIAL
		(1.0 - CENTS_SHIFT(-9)) * 4, // MODULATION_PITCH_COEFFICIENT_SECOND
		int32_t(UINT16_MAX * (DECIBEL_SHIFT(0) - 1.0)) * 4, // MODULATION_AMPLITUDE_INITIAL_GAIN
		int32_t(UINT16_MAX * (1.0 - DECIBEL_SHIFT(0))) * 4, // MODULATION_AMPLITUDE_FINAL_GAIN
	},
};

const uint32_t sample_0_trombone_tromb2[768] = {
0x00000000,0x0001fff5,0xfff8fff8,0xffe7ffe7,0xffeaffe4,0x0003fff6,0x00230012,0x00230022,
0x0016001b,0x0000000f,0xfff5fffb,0xffeffff0,0xffeeffeb,0xfffbfff3,0x00220008,0x0049003c,
0x0066005b,0x007d007b,0x007f0083,0x00700078,0x00780071,0x00880083,0x0086008a,0x007d0083,
0x00800080,0x0092008d,0x0084008b,0x00690080,0x003c004e,0x000a0022,0xffdcfff9,0xffbfffd1,
0xff49ffa8,0xfbd0fdc1,0xf760f95a,0xf6f9f64e,0xfc4ef922,0x02a2ffc3,0x066504de,0x07570749,
0x059b06e7,0x02b4042e,0x016f01ac,0x01e20171,0x01c001f0,0x00810140,0xff89ffd9,0xfffbff93,
0x01390092,0x01f2019d,0x01e201d2,0x0151018c,0x01010123,0x009a00e7,0xff9c0028,0xfeabff07,
0xfe2dfe6b,0xfd10fdb4,0xfbb3fc55,0xfb8cfb5f,0xfd76fc5b,0x0005fec6,0x018e00ea,0x022901c3,
0x02e10268,0x04460391,0x0604053a,0x07c20713,0x073d07e9,0x034f05c3,0xfc3e0019,0xf2bcf7b2,
0xea1bedf3,0xe7d5e7e3,0xedcae9ab,0xf9d3f354,0x057b0034,0x0b64096e,0x0a5a0b9a,0x06a2085d,
0x0468053c,0x034003f2,0x012a0269,0xfe31ff8a,0xfcb2fcec,0xff02fd84,0x032c011b,0x05eb04c3,
0x06ba0675,0x06f306fb,0x07460722,0x07220751,0x056c068b,0x02390414,0xfe7a005d,0xfb4bfcb2,
0xf974fa26,0xf922f920,0xfa40f981,0xfc44fb24,0xfe87fd71,0x0020ff7a,0x00cf008b,0x010e00f1,
0x00c700f6,0x00d100a8,0x01c8013d,0x0239023c,0x018401f9,0x0102011f,0x011600d9,0x015a0135,
0x016e0140,0x01cf017d,0x020d01f0,0x01f20205,0x01d901e8,0x0109019e,0xfe67fff6,0xf9fffc72,
0xf4aaf752,0xf156f298,0xf249f146,0xf71cf431,0xfdf9fa85,0x0407013e,0x07680630,0x06de07a7,
0x03f10585,0x00aa0270,0xfd30fecb,0xfc60fc4c,0xfeb8fd57,0x01290006,0x037e0236,0x05fc04d6,
0x08160711,0x09a508fc,0x098a09d0,0x06f40887,0x03ad053d,0x01b70290,0x0033010b,0xfd78fef7,
0xfb04fc05,0xf99cfa37,0xf923f925,0xfab4f9ae,0xfdcffc3a,0x0090ff40,0x029201a1,0x0376032d,
0x031b0367,0x022f02aa,0x013a0193,0x010e010e,0x018a0135,0x029a0216,0x03ea0347,0x04b30458,
0x041c0482,0x022f0332,0x00950179,0xfec0ffdd,0xfbb6fd6f,0xf668f95a,0xefdef344,0xe857ec45,
0xe327e51b,0xe822e36d,0xf9bef00c,0x0a020309,0x10c20e51,0x11b711a1,0x0fd11137,0x0a010d58,
0x03b5068a,0x008801ac,0x000c002c,0xffd7000f,0x0027ffdc,0x01ec00e2,0x04c40332,0x07f40660,
0x0a4e0946,0x0a080a74,0x07cc0902,0x04c6065b,0x01160301,0xfce8ff12,0xf8b4fad7,0xf43df686,
0xf091f235,0xf0f5f025,0xf611f329,0xfc1bf93c,0x00e4feab,0x049702d1,0x06f805f5,0x07cc0778,
0x081607ee,0x08340821,0x07dc082a,0x06520721,0x056e05c0,0x05a7056b,0x064205e9,0x0681066a,
0x05f3064a,0x04330550,0x01940315,0xfd65ffcf,0xf4e5f9ee,0xe5ffee16,0xd6bcddcb,0xcb6ad142,
0xc69ac6bc,0xe59dd16f,0x15b2feb8,0x2c8a2554,0x282c2cb2,0x1b5621bc,0x0e2f14cd,0x01fa07b2,
0xfa6dfdb4,0xf75af865,0xf846f739,0xfcc5fa28,0x02eeffbe,0x0a770677,0x11820e3c,0x147513a6,
0x12df140c,0x0f5f113c,0x09c80cd3,0x0291065a,0xfab5fe99,0xf3b9f70f,0xedd0f08c,0xe9f7eb7d,
0xea46e91d,0xf2d1ed74,0xff08f90f,0x061b0388,0x075c072c,0x06ad0735,0x05ff0629,0x074b0659,
0x09090840,0x0a2e0995,0x0a750a5f,0x099d0a66,0x06bf0836,0x047a0596,0x029e0396,0x00270174,
0xfd67fed6,0xfa4ffc34,0xf1a0f6ae,0xe4abeb22,0xdcf1df59,0xe40ade83,0xf552ec62,0xf805fa11,
0xdf97edf2,0xe5e1db89,0x0fc6fa28,0x23f81e6c,0x1e1722b4,0x161c1940,0x108613b4,0x04ef0b76,
0xfa0dfec2,0xf584f721,0xf700f54f,0xfff6fade,0x0bd205da,0x150a10f4,0x190f17ce,0x15d21857,
0x0e14121f,0x05e20a1e,0xfd3a01a6,0xf4b9f8b8,0xf013f1b2,0xf125eff3,0xf49cf2d7,0xf4f3f54c,
0xf1d9f358,0xf51ef23d,0x004cfa3c,0x0a2d05fc,0x0dbd0cb3,0x0d4b0de0,0x09e30be1,0x058b0791,
0x04320465,0x0551049d,0x06cf05ec,0x08a0079a,0x09f6097f,0x08c309c4,0x057f0759,0x01bf03bb,
0xfc61ff6e,0xf068f71c,0xdfe1e841,0xd22cd839,0xd131cf8a,0xdca0d709,0xd35ddd57,0xd1f5c6c1,
0x1214ec7a,0x419531b7,0x37dc4252,0x1bad284a,0x0f141409,0x023d0953,0xf8b0fc55,0xf834f827,
0xfa4bf915,0xff92fcdd,0x061e02f1,0x0c0b095e,0x12660f01,0x16eb1588,0x13761624,0x0cae1036,
0x068409ac,0xfd6302a7,0xf261f79e,0xedefef19,0xf0c1eed6,0xf29bf29a,0xebb1f050,0xea10e8f4,
0xfa02f03c,0x0b780402,0x0ed60ea5,0x0c7e0daa,0x0a7c0b9a,0x07520907,0x06b2067d,0x085c0782,
0x09750917,0x0a4a0a11,0x09f60a2d,0x09fe09d0,0x09ac0a06,0x0586082a,0xfe9b0224,0xf8e6fb4f,
0xf1dff5ec,0xe546ec3f,0xd66edda6,0xcfe6d169,0xd844d2b3,0xcabbd93b,0xb4c9ac48,0x0579d3ef,
0x4cd73340,0x4adf53e8,0x288738af,0x17d01f2d,0x042e0f3b,0xf70ffc08,0xf5d2f688,0xf3d6f4f4,
0xfaa8f649,0x04a70000,0x0d450949,0x178c1207,0x1fcd1d05,0x1a5d1eef,0x0f4a14af,0x07b50b3b,
0xfbf902f3,0xec81f3c8,0xe731e860,0xeb33e8a4,0xf0e6eec2,0xebd8f09d,0xe9eee87b,0xfb35f0d0,
0x0c1d04cb,0x0e8d0ebf,0x09f10cac,0x063607a5,0x05d305da,0x061f05f7,0x074c069a,0x09d40881,
0x0c220b43,0x0bf50c63,0x0a4b0af4,0x086b096b,0x0436067c,0x00ab022c,0xfc1bfeaf,0xf3b0f8c1,
0xe6acedc8,0xd805df24,0xcf68d252,0xd6a7d03c,0xe287defd,0xbed1d2c2,0xebb8cde4,0x39df18e0,
0x45bd489c,0x27ee38b7,0x18b41df3,0x0c121435,0xfbdc02dc,0xf74af826,0xf383f57b,0xf60ef32b,
0x01a9fb3f,0x0e2f07c5,0x18ec13aa,0x20991d4e,0x1ce82034,0x10331677,0x07030ab5,0xff16035a,
0xf41ef996,0xecd1ef62,0xed1aec4e,0xf024ee8d,0xec88efc5,0xe352e60d,0xedb0e556,0x0344f928,
0x0be109ef,0x0be50b9e,0x0ecd0d31,0x0c710eb5,0x04460838,0x01d001e2,0x047a02bf,0x08510630,
0x0b860a29,0x0bf30be3,0x0d770cad,0x0c030d73,0x069a0960,0xffad0357,0xf7f8fc17,0xebc5f284,
0xdaeae3b1,0xca09d24e,0xc5e2c586,0xb996c733,0xb91fa494,0x150dde5f,0x5a3341d4,0x4fc05d7d,
0x2633390b,0x13651ad3,0xffea0aea,0xf21cf74c,0xf0cff10e,0xf15af051,0xf98df510,0x045dff72,
0x0db9096e,0x186912cc,0x205a1e23,0x1c0d1f9d,0x11c91704,0x09370d69,0xfe29046d,0xf2e0f835,
0xf01bf095,0xf10ef0b8,0xf00cf14c,0xe314ebae,0xdf6cde45,0xf2cfe71e,0x097dffb5,0x0aaa0c7d,
0x05a70705,0x07a806a1,0x06320794,0x03f204c1,0x066c0496,0x0b2c091a,0x0e150cff,0x0ced0e10,
0x09620b1a,0x09db08ea,0x09ed0a71,0x050d0801,0xfe9701e1,0xf809fc13,0xec29f2bc,0xdbc6e48a,
0xcd79d3fe,0xcb90cac4,0xace4cc0d,0xb0c590d7,0x1ec3dbec,0x61ab4a4c,0x52de623b,0x2b643b43,
0x1a6821d3,0x00b20e59,0xf09df615,0xed82ef8a,0xe843ea4c,0xf3daed1a,0x039cfc7d,0x10b60ac9,
0x1ee517ba,0x27982527,0x209525f8,0x136c1a68,0x07b50e08,0xf6870012,0xec23ef3b,0xedc1ec97,
0xedbaee68,0xedbeedda,0xe4fdebcb,0xe47be296,0xf7c1edcb,0x09680235,0x0a9e0b1b,0x066e0840,
0x06b00630,0x05a30685,0x02970463,0x01b601a8,0x065103df,0x0bac099d,0x0cc00d40,0x0c050c1c,
0x0b3c0b82,0x08ec0a10,0x08ab08d5,0x02f7073f,0xf5ebfd2c,0xe801eecc,0xda6ce11c,0xcefad41f,
0xce99cd58,0xb599d178,0xb5939764,0x231de039,0x62004e5d,0x4ac45da5,0x221d31ce,0x15e21acd,
0xfefe0bb9,0xf13af590,0xf09af1ae,0xec3ded69,0xf5a9f05a,0x04fefd9a,0x15770d9c,0x241d1d39,
0x297b294f,0x1e1425ce,0x0dc6158b,0x037c0867,0xf377fcf6,0xeb0cebf4,0xf129eda6,0xf2fdf370,
0xef8df24d,0xe2eeebcd,0xdfe5df5d,0xf2abe775,0x08c3fff3,0x09d40b67,0x05af06b0,0x082d073c,
0x0444073f,0xff7a0145,0x020afffa,0x070a04be,0x0a690976,0x0c080bcd,0x0d860c85,0x0ff00ed7,
0x0e800f2e,0x0c660db7,0x02990864,0xf0d3f9be,0xe0d5e84d,0xd44ad9e9,0xcfe2d130,0xd5ffd2cb,
0x91f7c937,0xc6139e83,0x43bc066c,0x65cd5f87,0x3e4e578b,0x20202945,0x127019f1,0xf81f035c,
0xed87eeaf,0xe923eb6d,0xeaf9e62c,0xfc1bf251,0x0c6c050a,0x18cd128d,0x27d31fbf,0x2a1a2ba1,
0x1c8323c1,0x0d2a1411,0xff88062f,0xecdef61f,0xe3f9e540,0xee7fe854,0xf77cf3a9,0xf730f91e,
0xe24aebf5,0xe54adfdc,0xfbd8f05c,0x0b8b0801,0x04eb0a15,0x04570235,0x083807a7,0x015b0579,
0xfc29fd3e,0x00fffd9c,0x07aa0423,0x0c5c0a5c,0x0b5f0c6f,0x0ab90b20,0x0d8b0cb0,0x0cea0daa,
0x07370a77,0xff2d0321,0xf0c4f8f2,0xe121e851,0xd699db13,0xd4afd424,0xda20d511,0x8571ae9d,
0xdcedb146,0x57832e7a,0x65b56bd8,0x322b505a,0x183b21a9,0x06d61295,0xf063f9d5,0xeef7ece8,
0xe9f8edb8,0xeb8fe635,0xfda9f33e,0x0de80638,0x1b4113e9,0x297122a0,0x29172b7b,0x1c082286,
0x0d0413d4,0xfdae0537,0xea8df3a0,0xe423e415,0xeedce8cb,0xf781f3c7,0xf730f914,0xe279ebd3,
0x0000e0b8,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
0x00000000,0x00000000,0x00000000,
};

const uint32_t sample_1_trombone_troma3[768] = {
0x00000000,0xffbfffbf,0xffbfffbf,0xffbfffbf,0xffbfffbf,0xffbfffbf,0xffbfffc1,0xffbfffbf,
0xffbfffbe,0xffbfffbe,0xffc4ffbd,0xfffdffdd,0x002e001a,0x003e003b,0x003f0043,0x00370038,
0x0050003e,0x007e0069,0x007d0081,0x00560068,0xfffd0029,0xffc3ffd5,0xffccffc1,0xfff6ffe3,
0xfff0fff1,0xffd6ffe3,0xffbeffc7,0xffa0ffc1,0xff13ff5f,0xfe8dfed3,0xfd07fe17,0xfa00fb6a,
0xfa28f976,0xfee3fc1f,0x04d601f0,0x0755068b,0x0695074e,0x03950545,0x00d801fb,0xffe60018,
0xffb3ffc7,0xffb3ffaa,0xffc2ffc9,0xfff3ffd5,0x00ef0076,0x027001bb,0x03df033c,0x04b40484,
0x066c0565,0x07660770,0x00990502,0xf557fb0a,0xecc3f055,0xecc8eb6e,0xf610f094,0x0023fbac,
0x01a90207,0xfda2ffbf,0xfceffc2d,0x026eff87,0x06dd04c5,0x0b710953,0x0a1e0bca,0x0186060c,
0xfaf6fd5b,0xfbf0fa49,0x0206fee8,0x047b040a,0x01110300,0xffe1ffd7,0x026200d9,0x068e049b,
0x090d0850,0x078f08e6,0x056f0642,0x050e0538,0x02bd0425,0xfee800f6,0xf9f8fcd8,0xf32cf6e7,
0xeca8efc2,0xe974ea6e,0xed0aea34,0xf4b8f0c4,0xff35f964,0x0b3105aa,0x0f750e8a,0x0ae10dba,
0x059707a3,0x02d303f1,0x015901c8,0xff8e0088,0xfdd6feae,0xfdacfd92,0xfec5fe59,0xfffdff6c,
0x01eb00fb,0x04870332,0x074c05cb,0x090c0853,0x0a2709bf,0x0a130a78,0x06f808e6,0x029b04d4,
0xfe7a009a,0xfb23fc93,0xf93afa39,0xf88cf8e5,0xf6e4f808,0xf2d0f506,0xedc4f054,0xe888ead8,
0xed10e8d4,0xff13f544,0x0ef50866,0x11b31214,0x0ae80eb9,0x03cc06c7,0x00170135,0x010ffff8,
0x038e0275,0x02940380,0xfff3012c,0xfef5ff59,0xffb1ff1e,0x02540099,0x065d0443,0x09820850,
0x0aa40a8d,0x0a5a0a11,0x0a7a0aa7,0x08510972,0x06d80794,0x017c048b,0xf84cfd34,0xf08df3fa,
0xe978ed48,0xded6e547,0xc7b4d38e,0xe1a5ca8a,0x1a600102,0x2c1a293c,0x1ef527cf,0x070f130e,
0xf879fd07,0xf89af6e1,0x0002fbe8,0x06a00423,0x047506a4,0xff2d0192,0xfbcffd8f,0xfb4ffaed,
0xffe6fd27,0x05a602bc,0x09cd0794,0x0ef90c59,0x125610ff,0x13d91389,0x0f0812ce,0x010b08c9,
0xf53dfab3,0xeceaf07c,0xe77fe9e3,0xdf3ce3c6,0xcd32d8cf,0xb928b65a,0x0d18e0f7,0x3f553030,
0x36b24119,0x16ae27d0,0xfa2206e7,0xef8ef252,0xf57bf0d7,0x00affb9f,0x04d303af,0x043204da,
0x00de0270,0xfb4afe0d,0xf9c3f9a5,0xfdaafb29,0x0547016f,0x0d45089c,0x175312c2,0x186f196a,
0x10f2157f,0x06a40c27,0xfab5007f,0xf11ef542,0xeb49edc9,0xe302e85b,0xd7d6dcbe,0xbe8fd25e,
0xc9a7a8ae,0x2b48fd3a,0x4d0145c6,0x35494627,0x0df22235,0xef10fc48,0xe811e910,0xf3dfed36,
0x018ffbbf,0x089f0626,0x0a8f0a49,0x06a909ab,0xfe180286,0xf974faad,0xfd22faf5,0x0298ff82,
0x0cb207b9,0x14fe119e,0x14271676,0x0baf0f8e,0x013506b9,0xf912fcef,0xf323f5ee,0xee93f0c5,
0xe3eaea91,0xe05de05c,0xe0c0e23c,0xbed9d5fb,0xf187c59f,0x3a771b6a,0x42ba4498,0x22863480,
0xff060ef4,0xeee8f423,0xefb1edd5,0xfc45f524,0x088f0348,0x0e1a0c8f,0x0af70dde,0x00a906e8,
0xf602fa2b,0xf50bf481,0xfc9ef84b,0x090a0261,0x16311000,0x19f019f8,0x13b017c0,0x08620e46,
0xff97042c,0xf312fa1f,0xeaa6ee5c,0xe4a0e834,0xdb98dfe8,0xdd95dcd8,0xbda1dbb7,0xddc4a791,
0x3d7c10b1,0x4c9d4b9c,0x29be3e70,0x02391516,0xeb3bf501,0xea46e8e5,0xfa1bf2f4,0x0a02035f,
0x134410cc,0x10051385,0x01c50a24,0xf20ef92f,0xed80ef01,0xf4c6efaf,0x0468fc36,0x15a80da9,
0x1fa51c3b,0x1caf1fbb,0x1127171d,0x04a60b84,0xf216fa64,0xea61ec96,0xe875e97c,0xdaf2e3bb,
0xd43bd8ff,0x94fdcf31,0xef009ca5,0x549f254c,0x5a2f598b,0x2d6443df,0xf93e1098,0xdf65e7d0,
0xe329dde5,0xf6a1ed25,0x08ee00dd,0x160f1236,0x134d1794,0x04870d98,0xf2d1fb1b,0xeae8edb8,
0xf1d9ed2e,0x0294f91f,0x18a70de3,0x22fe2033,0x1cc121fc,0x10d716b6,0x04500ae1,0xf255fb4c,
0xe80eeb06,0xe9dee903,0xdc92e48e,0xd9fcdd1f,0x9dbcd2ce,0xee61a01c,0x53dc26de,0x57d459e1,
0x2b39428d,0xf5e20ea1,0xdda8e58e,0xe226dd86,0xf641ec16,0x09270147,0x18161233,0x17031a24,
0x03700f46,0xec85f733,0xe684e78b,0xef9dea50,0xfed8f62d,0x17380ae6,0x22791fae,0x1f3221a0,
0x17ee1c88,0x07e51151,0xf43cfe19,0xeb28edb2,0xeb5ceb7e,0xdee8e595,0xdde5de38,0xc179db33,
0xbf158db8,0x40f80890,0x5c795c36,0x389751d4,0x02121ffe,0xe071eed3,0xdeeede85,0xf201e7db,
0x04dffd86,0x152e0e53,0x17cb18c1,0x066311cd,0xec85f92b,0xe6d7e72c,0xee04e974,0xfb4cf455,
0x0eba03f1,0x223319a7,0x24872531,0x1c0e2106,0x0e881688,0xf9b804da,0xec52f08f,0xeb91eaed,
0xdfe0e86c,0xda20d96d,0xdadcd9a9,0x9829ac4a,0x1624ddc3,0x59c24bb0,0x4bc75e08,0x1372322e,
0xe9dffa22,0xe1f0e18c,0xed13e4d5,0xfdb0f37d,0x115505dc,0x16281646,0x092410ff,0xf49aff48,
0xe926eb41,0xe7bfe6bc,0xf709ecd7,0x0b9400e3,0x222c1726,0x2b6a2968,0x22f128c2,0x14ac1bf5,
0xfc740a20,0xe863efe9,0xe845e680,0xe4b3e9cc,0xd86adb10,0xe2c0db2e,0x8db8c8c3,0x00e3c31c,
0x571c39ee,0x53e45d97,0x1b1638d8,0xec3ffc01,0xdf8fdfca,0xeae1e15b,0xfad5f251,0x0ed60265,
0x195416ca,0x120f17c7,0xf9130630,0xe740ec3e,0xe461e45b,0xf263e9b8,0x0745fb0b,0x1f2a13fe,
0x2a0a2692,0x27442941,0x1ae822ad,0x014f0fc6,0xeb2cf48f,0xe897e77a,0xe886eb5c,0xdad3e003,
0xdef7dde4,0x8db8cd23,0xf81cb306,0x56c13067,0x58325dea,0x20d33d8c,0xeb22ff36,0xdb78dd2b,
0xe881df3f,0xf935f074,0x0ae6ff5d,0x1b1d157d,0x16871bcb,0xfbc909f0,0xe609ee13,0xe4bae482,
0xee3de8ce,0x00c2f518,0x19510dcf,0x2a3e22f6,0x2cfe2d15,0x209629a2,0x04801304,0xedcdf72a,
0xe888e94a,0xe9bfeab7,0xdaefe346,0xda5cd943,0xab83dea9,0xe39995ca,0x4d071995,0x5b815802,
0x31b84a77,0xf2010ff6,0xd923e0d4,0xe29cddf2,0xf2f4ec42,0x01acfa32,0x186f0fdf,0x1a331be6,
0x048a1271,0xea54f6e1,0xe5dbe6fc,0xe9d9e6f8,0xf60beef7,0x1068028f,0x29051ebc,0x2ee92dbf,
0x27372dc3,0x0da01c02,0xf129fd4b,0xe93eeacb,0xe9eeea67,0xd9b3e51d,0xd744d542,0xcf14e257,
0xc2259028,0x39d402b6,0x5d7856ba,0x3b00561a,0xf8c91b59,0xdd79e731,0xe17bdf00,0xed6de8bb,
0xfa9ff491,0x161c09d3,0x1c7d1bd4,0x0c2e184d,0xed9cfd0c,0xe579e79b,0xe830e57d,0xf440ee2f,
0x0b9afe22,0x25ff1a60,0x2fdd2c7b,0x2a212e0f,0x176d2267,0xf49205d2,0xe3bae95b,0xe341e335,
0xd8f8e242,0xd75ad340,0xd38edea4,0xbb509651,0x3308ff1c,0x5c525427,0x40b758d1,0xfe5a2224,
0xe001ea9f,0xe261df08,0xeed6eb04,0xf836f4b4,0x12360409,0x1f5a1bbb,0x0ffe1c44,0xefc2ff90,
0xe53be6fd,0xe5a8e4bd,0xf002ea65,0x09befbd3,0x232616d0,0x31622c7b,0x2d6630a4,0x185c253a,
0xf52e0652,0xe028e72d,0xe12bde60,0xdb9ae156,0xdab9d5b6,0xdba3de2d,0xae4ca442,0x266cf4fd,
0x590c4f64,0x48045b95,0x09742d40,0xe258f06f,0xdfc9dd60,0xedafe805,0xf85bf3fa,0x0dfdffe3,
0x218719d9,0x15141fc4,0xf37003c1,0xe2cee701,0xe442e26b,0xef09e909,0x064bf8b8,0x1ebd1174,
0x2fdb2a10,0x2cca2f29,0x19782621,0xf76c07e6,0xe480eaec,0xe18ee000,0xddc3e170,0xd546d565,
0xdcf5d96e,0xa6beabbc,0x24a6f141,0x5b3a50fa,0x4dc95fb8,0x10d834e7,0xe00ff1cc,0xd9c1d8f8,
0xeab7e179,0xfa6cf392,0x0c92ffa4,0x225d18a8,0x1a9d22a0,0xf8b009f6,0xe1a9e9a7,0xe2efe068,
0xef03e78d,0x01d9f644,0x19180d04,0x299e2447,0x29e62b17,0x1ce92656,0xfbdf0d9c,0xe613ecf2,
0xe47ae31d,0xde22e2bb,0xd61ed7ab,0xe1cbd90a,0xa117c64d,0x13cedd44,0x558044e6,0x50015b25,
0x1b8239fc,0xe4bcf8d9,0xdc19db78,0xeb12e072,0xfd1ef29d,0x0cc30155,0x2157184f,0x1b4d20b3,
0xfdd50e3e,0xe408ed9f,0xe0dedfdb,0xeb2ee4a1,0xfbd8f1f9,0x12f207a5,0x25151d87,0x2a9e29a1,
0x21602882,0x02d515b3,0xe4c7efc6,0xe013e0b9,0xde0bdff0,0xd89fd9fd,0xe156d8c6,0xa3e7cc3d,
0x0fa0d950,0x521b4018,0x4f26565f,0x22863cb5,0xeaaafff8,0xe0aee0bd,0xedace30c,0xfccef3cd,
0x06ceff6a,0x1b1d1186,0x1bac1e48,0x030f1229,0xe689f26d,0xdeafe008,0xe6e4e0f8,0xf8f2ee43,
0x11380669,0x23421b21,0x2aa227e6,0x24272a1f,0x073c19c5,0xe419f288,0xdd46de9f,0xdd7adf18,
0xdb08dbd1,0xe19bdbde,0xaa0cd7ba,0x055fca1c,0x4e8e3492,0x50555243,0x2ae34050,0xf3160a49,
0xe319e605,0xea36e1af,0xf930f167,0x0294fe10,0x16590ccd,0x1e761ea9,0x071d15ca,0xe8bff694,
0xdcdddfce,0xe6d2df35,0xf7faee98,0x0e6a038d,0x21c41811,0x292526f0,0x24fe28de,0x09601b15,
0xe67df55d,0xdd80e08b,0xdebbdf8f,0xdceddd63,0xe081dc89,0xaf3ddb71,0xff83c484,0x4c2d2ebc,
0x507750e4,0x2cf4418c,0xf48e0ca1,0xe2d3e68c,0xe95ee13c,0xf8ccf102,0x02b3fe23,0x165a0cd8,
0x1e7e1ea9,0x078715e7,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
};

const uint32_t sample_2_trombone_tromd4[640] = {
0x00000000,0xfffefff3,0xffebffef,0xffdeffe1,0xffdcffdc,0xffd3ffd4,0xffc4ffc5,0xffb6ffbb,
0xffabffaf,0xff9effa7,0xff9fffa2,0xff9dff9e,0xffb7ffa3,0xfff6ffd7,0x00240011,0x004b0037,
0x005e005b,0x005f005e,0x006e0063,0x00700073,0x005b006c,0x00320049,0x0007001a,0xffdbfff3,
0xffcaffd1,0xff81ffac,0xff4cff62,0xff45ff47,0xff5aff4f,0xff78ff69,0xff86ff86,0xff7dff8a,
0xff67ff73,0xff6eff69,0xff9eff78,0xfe3bff33,0xfa93fc69,0xf952f95f,0xfdccfaa9,0x05b101cb,
0x0a150879,0x088409e9,0x03ab05f9,0x0130021c,0x003d00cc,0xff93fffb,0x0023ffc9,0x02f2018b,
0x071c048f,0x097509e8,0x003305ce,0xf3baf9da,0xeae3ee5f,0xebfeea09,0xf692f091,0x0050fbf7,
0x052d03bd,0x04e404d1,0x06750541,0x080307bc,0x057906c3,0x02ca0405,0x017c0212,0xffac00a2,
0xfed7ff05,0xff25fed3,0x01330023,0x047002d9,0x05df05ba,0x05c805a3,0x06d80612,0x087f07e2,
0x03f207b1,0xf8defef9,0xedf8f311,0xe8bfea14,0xeceeea2c,0xf3f4ef99,0xffa6f963,0x09da057e,
0x0cf00c55,0x09170b9c,0x03df066e,0x018f01ff,0x024c01ff,0x01cb0234,0x004e0168,0xffc0ffce,
0x01dd005d,0x050703a1,0x05e005a8,0x05c305ef,0x06e105ff,0x08d307bf,0x0602089f,0xfaea0147,
0xed8cf408,0xe5e2e88a,0xe6cae5b3,0xefddea09,0xfeb3f6b2,0x0d5b06fd,0x11da1125,0x0c660fa0,
0x05920913,0x015b0285,0x01160159,0xffc90084,0xffa8ff69,0x02ec012e,0x03cc03a9,0x05720480,
0x057905b0,0x04eb0516,0x04e004e6,0x046b0488,0x033a0419,0xfc130072,0xf0c4f6a9,0xe7d1ebd2,
0xe3cbe580,0xe94ee48a,0xfad7f0a8,0x117c0733,0x18d417a4,0x11431629,0x06ef0c42,0xff160255,
0xfd7efe36,0xfd4dfd15,0x006bfe54,0x0388022e,0x047f03de,0x07c40608,0x07c50818,0x059806d7,
0x03320445,0x0283031b,0x005501d3,0xf9f7fdb0,0xee42f4b6,0xdc06e691,0xcbf1d0ff,0xef45d5fb,
0x228a0bf5,0x2f192e24,0x1db2284e,0x066d117c,0xf89bfdea,0xf58ff607,0xf89df6c0,0xfcb3fb15,
0xfd7afd7e,0xff2ffdae,0x0664023f,0x0de70abb,0x0d250ebd,0x0a450bb2,0x07ca08fd,0x025c04f8,
0xfa42ff1b,0xea39f36b,0xceb3de80,0xbdecbf8a,0xf307d194,0x2c5c145b,0x3519367f,0x1c5f2af8,
0x017f0cd2,0xf9fefb79,0xfdb0fb63,0x013d002e,0xfedf00c4,0xf968fc2b,0xf80ef7e4,0xfe1ff9d0,
0x07d20300,0x0e6f0b07,0x13841234,0x114f1312,0x09bc0e17,0xfc7f0457,0xe6b8f297,0xca94d97a,
0xb3e3bac9,0xde72c00c,0x239703a3,0x39e835b9,0x245232a5,0x09b8154a,0x008902f1,0x023600c3,
0x02fa0361,0xff160178,0xf9c0fc67,0xf74df7de,0xfab0f807,0x0466ff44,0x0c03086a,0x128d0fbe,
0x12cc1387,0x0f20115a,0x03660a92,0xed90f9b5,0xd195e01f,0xb36dc190,0xc278b00d,0x1142e837,
0x3d222faf,0x304a3ba9,0x11642040,0x02510709,0x02270160,0x029e0317,0xff21011f,0xfb13fd05,
0xf806f92c,0xf968f7e7,0x0154fca0,0x0b560694,0x11020ea4,0x11641231,0x0f67104a,0x06950c2a,
0xf506fe5c,0xdff9ead4,0xc6edd480,0xabdeb5e5,0xe911bdb9,0x37b51705,0x405d443b,0x1fcc318f,
0x07c2111a,0x0195031d,0x011c0162,0xfe23fff3,0xfa60fc53,0xf51ff7e9,0xf3e6f31a,0xfd4ff79b,
0x0a020401,0x0f790e0d,0x107c0f7f,0x1075107c,0x0c390f58,0x00e9078d,0xeddcf8e1,0xccd4e018,
0x9f59b1d9,0xe2ccb38e,0x33c01249,0x3e5440ee,0x1f7a3132,0x04830f11,0x008f0044,0x03af0231,
0x03990423,0xff6e024d,0xf51cfabb,0xef5df0c8,0xf4d2f0e5,0x000ffa58,0x0b6d063b,0x139e100b,
0x168e1500,0x156e172b,0x096510cf,0xf09ffe72,0xc51ade6b,0x93dfa1c1,0xed15b786,0x39d01ceb,
0x3b4e41f7,0x18212b48,0x01430900,0x0303004b,0x07170588,0x067f0725,0x0248051d,0xf813fdd0,
0xf11cf38a,0xf1aff089,0xfa02f4b6,0x07c50138,0x120e0d5b,0x1693147e,0x16e317dd,0x0c5f1396,
0xf17300f0,0xc041dcb3,0x8ea59ba0,0xefb0b66f,0x3dd82169,0x3a88440b,0x119d26f4,0xfe700353,
0x05020071,0x0b7808ed,0x0a8c0bdd,0x044507f1,0xfaa8ff9d,0xf494f70b,0xf2f2f300,0xf9fff4ff,
0x05a60011,0x0fdb0bbf,0x125b11bb,0x140b142d,0x0bc31224,0xf2ca00dc,0xc9bbe116,0x889aa664,
0xe657a772,0x3fbf1e37,0x3cc246f8,0x0fb92700,0xfbba007d,0x0567feb4,0x0f060b81,0x0d860f45,
0x06390a7e,0xfb3f00eb,0xf4faf761,0xf2d2f324,0xfa8bf4d8,0x072b01c8,0x0cbb0aee,0x0da30d0d,
0x11e6102e,0x0c2f110c,0xf73102b7,0xdc3bea7f,0x9e46c80c,0xbf548aa8,0x30a8ffdc,0x43604580,
0x172e3163,0xfb7e0301,0x04eafd90,0x11460cb2,0x0ee8117c,0x08910bbb,0xfeb104c3,0xf353f7c8,
0xf0acf0c9,0xfa89f420,0x075e0255,0x078f084c,0x091907f1,0x10470c42,0x0e08107f,0xfd0506c0,
0xe4bdf226,0xa7ecd150,0xbe4d8fd2,0x2ccbfc4c,0x40d14207,0x1a0b31c9,0xf3f201fa,0xf8cdf1c1,
0x0d82048d,0x12d81248,0x0edf1185,0x03340ab1,0xf220f932,0xefb0eed5,0xfa32f3ed,0x05b0016e,
0x042b056e,0x080004bd,0x13960de3,0x103e143c,0xfd53081c,0xe343f1bd,0xa336ce04,0xc8d494b9,
0x321104ef,0x40e943f1,0x1904312b,0xf081ffb2,0xf46eed5e,0x09350037,0x0f700e37,0x102e0fd1,
0x082d0e96,0xf68cfe3b,0xf18df27a,0xf6d3f302,0x02f6fde5,0x05840448,0x07d505b0,0x11290c43,
0x0ffc1275,0xfe0708ed,0xdd98f0ca,0x97e3c45f,0xd3c39691,0x3b361055,0x438b4921,0x17e7314f,
0xeee1fd92,0xf306ebd3,0x05a6fdc6,0x0db10b1c,0x0f230ed5,0x09ee0e02,0xfd870367,0xf5a9f874,
0xf775f560,0x015ffd15,0x05730388,0x06f00629,0x0e900a6b,0x0bf90f14,0xfa7b04ea,0xdb7bed8e,
0x99e0c4ca,0xd1d994cc,0x3c3a1040,0x44a04a61,0x1913328d,0xf078fece,0xf447ed6e,0x05c9fe8f,
0x0c250a4a,0x0d0d0d05,0x08f70be0,0x02250568,0xf826fcf5,0xf93df69e,0x03fbff3d,0x07c20632,
0x047106bc,0x0a28061c,0x09d20c21,0xf87902a4,0xdf3eed57,0xac8dcfab,0xbbe48ced,0x3427005d,
0x488b4b39,0x1e0e386b,0xf1440225,0xf270ecf0,0x0739fe0f,0x0c110b71,0x0b9e0bea,0x0a4f0b6b,
0x02550648,0xf87ffdb2,0xfb9bf73b,0x073002a4,0x068107d3,0x00f6040a,0x07250203,0x09cc0a22,
0xfb81041c,0xe56cf218,0xb43fd61d,0xb6469038,0x2f03f9e9,0x471648a0,0x215f388d,0xf2530680,
0xeb98e9f3,0x033bf792,0x0d580a89,0x0bd90d5b,0x086709ee,0xfffd0474,0xf726fafe,0xff59f84f,
0x0c6f0975,0x06010b1a,0xfdc80041,0x06b9ffec,0x092c09aa,0xfb790305,0xe837f37b,0xb1c2d88e,
0xc71e9533,0x32bc0239,0x40f34583,0x1c223376,0xefd6041e,0xea7ae959,0x0339f8a6,0x0cbd0af3,
0x0b750c41,0x094e0a1a,0xfe9d03cb,0xf4d3f813,0xff76f784,0x0d610851,0x07160b38,0xfd79018f,
0x06e7005f,0x08fb0ad2,0xfa92028c,0xe4d4f30f,0xa4f5d057,0xd9819d92,0x3b1e0fe1,0x3d334435,
0x18b42c05,0xefb20248,0xe97ce871,0x0110f701,0x0d820a84,0x0c8f0e5e,0x0a760b63,0x029a06d9,
0xf733fb74,0xfcf3f744,0x089a03f7,0x06eb0833,0x002f034c,0x076b0167,0x07690ab8,0xf584ffc5,
0xe091ee1b,0xa0fbcd27,0xe0849fc8,0x40ca165d,0x3e49469c,0x157e2ab3,0xef45ffdc,0xe9ede857,
0xff81f617,0x0ad507ed,0x0c0e0cec,0x0b930bfd,0x08600ae0,0xf91b00d3,0xfb26f57c,0x076902bc,
0x090708b8,0x02ac06ce,0x0786026a,0x06bc0a64,0xf275fdfd,0xdf62ea9f,0xa709d14e,0xd9e197fa,
0x41321352,0x40c84a00,0x15892d21,0xeeb60029,0xe8c4e7ca,0xfea4f4f0,0x0a330727,0x0bf90c83,
0x0c5e0c42,0x08f00b80,0xf9660150,0xfae7f55f,0x077b02c9,0x090708c0,0x029f06c9,0x00000268,
0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
0x00000000,0x00000000,0x00000000,0x00000000,
};

const uint32_t sample_3_trombone_tromg4[896] = {
0x00000000,0xffc7ffd4,0xffb9ffc2,0xffa1ffab,0xff9eff9d,0xffb9ffa8,0xfff4ffcf,0x00370014,
0x0072004f,0x00980080,0x0099009c,0x00750089,0x003b005d,0xfff6001c,0xffc4ffe1,0xff95ffaa,
0xff80ff8b,0xff83ff7f,0xff8eff84,0xff90ff88,0xffa8ff99,0xffecffd2,0x00440020,0x0083005d,
0x00bb009e,0x00d300cc,0x009e00ba,0x003b0070,0xfff00015,0xffb4ffd7,0xff94ffa5,0xff72ff81,
0xff68ff71,0xfeffff75,0xf9e9fc6f,0xf7d0f800,0xfe46fa39,0x059e0238,0x088807b8,0x072e07f0,
0x047e0598,0x03c00366,0x06a4047e,0x07da0804,0x01720585,0xf892fd80,0xef66f3d9,0xea1eec11,
0xedc7eaaf,0xfcfdf42d,0x0dd5062a,0x13bb1244,0x0e73120c,0x056609f4,0xff5e0186,0xfeeffec6,
0x0112ffd3,0x049202c0,0x06b1068f,0x01c3055f,0xf6b6fcbe,0xea9ff069,0xdfffe516,0xe7a9df4a,
0x0952f7b0,0x20ff1826,0x1dc322b3,0x0cd015d4,0xfe7a051a,0xf86cfacf,0xf9daf83c,0xfdf9fc00,
0x012fff45,0x05db0353,0x05cd06b6,0xfbac0168,0xee0ef4c8,0xe0d3e74c,0xe2fadd41,0x0718f333,
0x22ff1855,0x216d25a7,0x0f0418cd,0xff1205ba,0xf83ffab5,0xf828f740,0xfa46f9a2,0xfa0ffa03,
0x0283fda2,0x0a4507d7,0x052e09cc,0xf70bfece,0xe70bef12,0xdc6fdfec,0xf310e16f,0x195206f6,
0x27202440,0x18ad2217,0x04430dc7,0xf8cefceb,0xf745f738,0xfa0bf90f,0xfb83fb2b,0x00b7fd9d,
0x086604ea,0x083b0a2a,0xfcee0390,0xef5bf5ab,0xe51eea17,0xddc8e045,0xf916e5cb,0x22600f98,
0x2aab2bb4,0x15cd223b,0xfe4d08f3,0xf403f717,0xf78ef4d1,0xfc23fa1f,0xfef3fd2c,0x041d00b7,
0x09560729,0x03d1083d,0xf696fd9b,0xec40f07e,0xe4abe8cc,0xdb9fde1f,0xfe48e6dd,0x2c381910,
0x2e5632c9,0x132b220d,0xf9f004c3,0xf46bf48b,0xf8d7f6e6,0xfc06fb0c,0xfed5fc5f,0x055101b3,
0x097508bc,0x00e40764,0xf20ff975,0xe6bdec30,0xcd00de5f,0xd9cdc020,0x2b3305cd,0x434f411d,
0x25a538c6,0x019512d4,0xf2eff698,0xf71bf4df,0xfb6af8af,0xfdbafd7b,0xfd52fd12,0x00c0fe6d,
0x00d6039a,0xf5a2fc12,0xeaa8efca,0xd9ede39b,0xd186cbf6,0x15eaf009,0x41ac347a,0x3354406b,
0x0d8820de,0xf48ffe11,0xf35bf254,0xf700f48b,0xfe89fb29,0xfbfbfda2,0xfedafcb7,0x04af039b,
0xfbe80214,0xed37f434,0xe0b3e73d,0xc883d67e,0xf7abd402,0x3b011e71,0x40e64508,0x1d99313c,
0xfa3e094e,0xee3cf0e1,0xf0d0ee61,0xfc03f5a9,0x014200a0,0x024e0216,0x056d03c1,0xffd30419,
0xef9ff821,0xe006e76c,0xcc8ad84b,0xe51ec919,0x2ef70bb5,0x4562430e,0x28863a9d,0x017d1475,
0xeea9f415,0xef51ee1f,0xf9e1f372,0x02d1000c,0x05af03d9,0x054f060d,0xfe420334,0xf0f9f74c,
0xe188e891,0xcfa2d8ab,0xddc5ca41,0x229600f9,0x435d3b23,0x2eb83ef4,0x07871bbe,0xefddf870,
0xf134ef2a,0xfa14f4c3,0x0133fece,0x00c90001,0x037e0291,0xff4502b8,0xf490f994,0xe8deeee6,
0xd545e033,0xd278ca19,0x18e4f462,0x43ad36d2,0x35cd438d,0x0d6a226a,0xee96fb4d,0xeb15e96f,
0xf43deefc,0xffecfa26,0x04b9031e,0x087906b0,0x0567082d,0xf82eff64,0xe9a5f0e6,0xd32be0e6,
0xc43fc102,0x1477e96c,0x465f363c,0x38e9461b,0x10a724db,0xf150feb9,0xeaebeaa4,0xf2c3ed98,
0x006efa15,0x03d502c1,0x08b60611,0x06f309f9,0xf97f0099,0xebb3f223,0xd06ce1b8,0xbe74b81a,
0x14e0e67e,0x48bb385b,0x3b59492e,0x1317278f,0xf3c10168,0xeca1ed39,0xf0c9ed43,0xfd26f772,
0x0016feb0,0x08d603fa,0x083a0aa2,0xfa850230,0xeaecf230,0xd1d1e254,0xb781b24c,0x1844e7a5,
0x4f293f7e,0x3e214ede,0x10332713,0xf175fe4b,0xeb2fec3f,0xed47ea3c,0xfcb3f464,0x01b70005,
0x09db058a,0x08e30b99,0xf9f201fb,0xe9d0f0dc,0xd309e214,0xac5cad00,0x1a5de54b,0x56c94699,
0x43ab5604,0x107f2a20,0xed97fb42,0xe961e8d9,0xed7fe9c5,0xfdbaf4cc,0x011b0076,0x07760280,
0x09e50aa1,0xfc6d03b3,0xea1ff274,0xd503e10c,0xa708b359,0x14a4dcc9,0x56e342b6,0x4909584a,
0x15a23032,0xec97fd1d,0xe7aee63c,0xee79e986,0xfd6ff62b,0xfcc5fdda,0x058e0006,0x0a4d0a96,
0xfe1605c3,0xeb9af532,0xdad9e405,0xa029c2c5,0x0651c5bc,0x57fe3906,0x53e75c8e,0x20a23b7d,
0xefdc04b4,0xe2f1e3d0,0xeae0e47b,0xfe89f49e,0xfc3bffb0,0xff77fb86,0x0a5906ab,0x02e3099c,
0xf06efa2d,0xde79e836,0xa41bca99,0xf558b27d,0x53a02d49,0x573e5cd0,0x28d94247,0xf63f0dc2,
0xe12ee60a,0xe525e147,0xfbf2ef4f,0xffdd031c,0xfdfdfcd5,0x07e2033d,0x04d908a4,0xf540fd28,
0xe46ced75,0xaed6d4e9,0xdb3da0f2,0x48061812,0x5a8d5b2e,0x326b4aa7,0x0199194a,0xe6adf010,
0xe33be3cc,0xf683ea87,0x000200a7,0xfc75fcb4,0x0686017c,0x04d607bd,0xf6ddfdbd,0xe54cef80,
0xa89ad2c7,0xdb7d9e99,0x46f617cf,0x5a645982,0x33764b65,0x05931bc0,0xec6df5ae,0xe2eae7c3,
0xf206e73b,0x0106fde3,0xfbb5fccc,0x028cfe72,0x025504bd,0xf7b3fe81,0xe720f1f2,0xaa7fd374,
0xdc05a630,0x447916b7,0x58f55750,0x31c84a14,0x033f1852,0xeeeff55e,0xe74eebbb,0xf09ae802,
0xfed7fafe,0xfeebfd40,0x065b032b,0x00c404af,0xf565fbe7,0xe558ef1a,0xa16dcea4,0xe486a8a1,
0x49e51e4a,0x588d58ec,0x2f24477e,0xff56147f,0xf09ef37b,0xea3deeaf,0xf14cea78,0xfc36fa3b,
0xfe63fbf1,0x06d20439,0xfe720452,0xf3b6f9ac,0xe54dee16,0x9824cda1,0xeb07a8a4,0x518924fc,
0x5bf35d30,0x2d7d4734,0xf964101f,0xebbded1f,0xeb5bebbc,0xf6aeee96,0xfda5fdb2,0xfee9fbb0,
0x08950550,0xfd1f04ac,0xf16df7fb,0xe4fded4e,0x91e8cec0,0xefc8a5e3,0x589a2b4b,0x5ecc60bf,
0x2cac478f,0xf6630d9c,0xe7efe981,0xe93ce82b,0xf79eee36,0xfd1eff40,0xfc3ff9d2,0x093003c3,
0xff9005dc,0xf564fb2b,0xe78ef0d5,0x936bd38a,0xea5d9eb5,0x5730267f,0x607b6002,0x30934afc,
0xf6ed10e1,0xe461e6a2,0xe60de564,0xf773ecd0,0xfc72001f,0xfc3ff93d,0x090b0399,0x01bb06fe,
0xf867fd19,0xec6ff520,0xa12ede2b,0xd922872f,0x54331760,0x64136238,0x366a5203,0xfb5e184f,
0xe19fe8b3,0xe31de49d,0xf565eae7,0xff270209,0xfa77faad,0x05a20055,0x01cd05ff,0xf83afd32,
0xefbcf6bd,0xae06e314,0xcb71871c,0x4a910c54,0x644d6150,0x3c9456b7,0x027c20d8,0xe29ced6e,
0xdfe2e2b3,0xf1cbe66d,0xff47007a,0xf9fdf9bd,0x05f4ff4a,0x02f205ef,0xf8bffde3,0xf12bf679,
0xb2efe4af,0xc48f85fa,0x459e06b0,0x65a761bd,0x3e895a2c,0x02d0220f,0xe469eed5,0xe1a5e567,
0xefe5e5c8,0x0051feaa,0xfaf9fb38,0x060e0165,0xfe050480,0xf46ef98a,0xf001f431,0xaffbe4df,
0xcf378fec,0x453a0a1a,0x61ad5d31,0x3bbe5511,0x03bc20d8,0xe85ef1c4,0xe539e98b,0xeee5e5e6,
0xff24fd14,0xfa43fae6,0x04df0112,0xfb3401f6,0xf374f806,0xee42f1cc,0xa4f8df3e,0xdda49e59,
0x477d14eb,0x5d0c58f5,0x378a4ee8,0x02c11ce8,0xeb3af215,0xe9a2ebba,0xf087e88d,0x000afe3d,
0xfb4dfa7f,0x0664021c,0xf7ad0067,0xefd4f3d9,0xea84ed5e,0x9d09d5f6,0xef92b1fe,0x4b482502,
0x5753566c,0x2fbe466f,0xffca169e,0xe9efeee3,0xecacead8,0xf520ed2b,0xfebbff08,0xfb48f8b2,
0x086502de,0xf7b300a7,0xefb1f35d,0xe958ec5b,0x954ad037,0xf8bcbd3b,0x4d9e2f80,0x532853e4,
0x2ab44157,0xfbfe1064,0xe957ebd2,0xeebfea90,0xf859f030,0xfd76ff3c,0xfd5df88e,0x0b4205dd,
0xf851017c,0xedb4f1a4,0xe81dea75,0x93fcd152,0xfaebbb4c,0x53aa344d,0x54775562,0x29ba4195,
0xf7d30d1b,0xe43be68d,0xecd1e6d4,0xfc03f24d,0xffff03b8,0xfe0afaa8,0x0a740650,0xf676006b,
0xebb0ef54,0xe9d6eb61,0x9a1fdbc2,0xf858b0ec,0x56bb3015,0x565f5687,0x2b5842e7,0xf6b60e09,
0xe001e3d2,0xe9ffe393,0xfd22f257,0x011305e7,0xfda0fbfe,0x0a9e05c9,0xf73f00e0,0xebebef05,
0xeb18ec2b,0x9eede0eb,0xf3b8a860,0x58862bb1,0x58a85a19,0x2c69455a,0xf5d80ecc,0xdd37e2a6,
0xe674e15f,0xfb1aef94,0x03cf074d,0xfe70fd59,0x0bd706fd,0xf6eb01f2,0xebeceed5,0xec11ed54,
0xa4c5e367,0xeddba304,0x56b02677,0x5be55d60,0x2d8347fe,0xf55b0f85,0xdc5de248,0xe43fe0e5,
0xf9fded15,0x053d084a,0xfe81fd9d,0x0c0c06d4,0xf63f01e6,0xece9eec7,0xec70ee26,0xa7f8e333,
0xec65a587,0x52d0242e,0x5cac5d12,0x2e6f498c,0xf5b91033,0xdf14e492,0xe4b2e2e7,0xf61deaf0,
0x03f804f9,0xfdd0fcd5,0x0e6207ed,0xf6f503f2,0xeca1eef0,0xea09eca7,0xa40adac1,0xf281b195,
0x4fd5291e,0x5a385a18,0x2e01482d,0xf5460f58,0xe10ce479,0xe680e2fa,0xf665ea95,0x03da03df,
0xfebbfd24,0x0f7108a9,0xf63402bc,0xecd1edb2,0xe8f2eb27,0x9e93d1d1,0xfb1fbfc7,0x4e2a312a,
0x55675628,0x2afc4406,0xf4640c81,0xe350e489,0xe955e4cc,0xf677ec29,0x028501ff,0x00c8fdef,
0x10b50aa0,0xf49702bf,0xeac0ec30,0xe56fe851,0x9e0ac807,0x05b6cda0,0x50023b34,0x50c5533e,
0x278d3f4e,0xf20d0904,0xe2b9e304,0xead1e4a5,0xf8e9eeb5,0x013c0239,0x020afe61,0x10b50baf,
0xf2e00169,0xe963ea76,0xe4dae775,0x9d74c2ad,0x0d0ed61f,0x538942d3,0x4e21533e,0x24293baf,
0xefdb0664,0xe0bde18f,0xe906e1fb,0xfa64f06f,0xff1100f4,0x03f4fe8a,0x11e70dc6,0xf34602d9,
0xe859e903,0xe418e6aa,0x9b7bbf55,0x10ced8b5,0x57d74856,0x4f2855af,0x23793ba3,0xed750527,
0xdcb9de74,0xe5b7de81,0xfd06f08c,0xffab02bb,0x03b4fe8d,0x11c30db9,0xf4540230,0xe898e8f9,
0xe4a1e753,0x9b9dc3be,0x1011d56a,0x597846b5,0x51065777,0x23203bb5,0xecf2051d,0xdc01dd4c,
0xe3f8de2b,0xfcc7ef66,0xfcbd00e6,0x03e0fd4e,0x13bd0f89,0xf5ac04c1,0xe9a1ea3e,0xe440e883,
0x9b44c573,0x0d27d0fa,0x59454313,0x55125a27,0x25923ede,0xed69065b,0xdb96dc36,0xe261ddc8,
0xfceeedf6,0xfd0a02a4,0x0235fbe6,0x14360ed9,0xf5cb0522,0xe9e4ea82,0xe4b8e8b8,0x9ae7c683,
0x0b11cc94,0x59fe40cf,0x57785d5f,0x25714010,0xed6605ec,0xdcc9dcbb,0xe0cade37,0xfba0eb8e,
0xfd1d02c2,0x02f3fc40,0x15f71077,0xf55f0571,0xe950e961,0xe391e7c3,0x9a46c249,0x0b85cecc,
0x58443f8a,0x57255d9e,0x263340bb,0xeda6075a,0xe07cde99,0xe343e19d,0xfaadea40,0xfedd035f,
0x030dfd02,0x13e40f53,0xf24802ae,0xe7f6e7b1,0xe1fbe5ac,0x9c85ba7d,0x11f2d866,0x569d4444,
0x545e5c26,0x24d53f53,0xed3706cf,0xe0bdded2,0xe3d3e218,0xfad9ea7c,0xfedd0352,0x030dfd02,
0x13e40f53,0xf24802ae,0xe7f6e7b1,0xe1fbe5ac,0x9c85ba7d,0x11f2d866,0x569d4444,0x545e5c26,
0x24d53f53,0xed3706cf,0xe0bdded2,0xe3d3e218,0xfad9ea7c,0xfee90352,0x032ffce8,0x00000fba,
0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
0x00000000,0x00000000,0x00000000,0x00000000,
};
