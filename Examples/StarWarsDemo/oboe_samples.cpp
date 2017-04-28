#include "oboe_samples.h"
const sample_data oboe_samples[3] = {
	{
		(int16_t*)sample_0_oboe_oboecx3, // sample
		true, // LOOP
		10, // LENGTH_BITS
		(1 << (32 - 10)) * CENTS_SHIFT(33) * 44100.0 / NOTE(80) / AUDIO_SAMPLE_RATE_EXACT + 0.5, // PER_HERTZ_PHASE_INCREMENT
		((uint32_t)891 - 1) << (32 - 10), // MAX_PHASE
		((uint32_t)886 - 1) << (32 - 10), // LOOP_PHASE_END
		(((uint32_t)886 - 1) << (32 - 10)) - (((uint32_t)832 - 1) << (32 - 10)), // LOOP_PHASE_LENGTH
		uint16_t(UINT16_MAX * DECIBEL_SHIFT(-4.5)), // INITIAL_ATTENUATION_SCALAR
		RATE_NORMALIZED_ENV_COUNT(0.00), // DELAY_COUNT
		RATE_NORMALIZED_ENV_COUNT(6.20), // ATTACK_COUNT
		RATE_NORMALIZED_ENV_COUNT(20.01), // HOLD_COUNT
		RATE_NORMALIZED_ENV_COUNT(20.01), // DECAY_COUNT
		RATE_NORMALIZED_ENV_COUNT(100.02), // RELEASE_COUNT
		NEG_INV_COUNT(100.02), // RELEASE_COUNT
		SUSTAIN_DROP(5.5, 20.01), // SUSTAIN_MULT
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
		(int16_t*)sample_1_oboe_oboefx3, // sample
		true, // LOOP
		11, // LENGTH_BITS
		(1 << (32 - 11)) * CENTS_SHIFT(38) * 44100.0 / NOTE(84) / AUDIO_SAMPLE_RATE_EXACT + 0.5, // PER_HERTZ_PHASE_INCREMENT
		((uint32_t)1225 - 1) << (32 - 11), // MAX_PHASE
		((uint32_t)1220 - 1) << (32 - 11), // LOOP_PHASE_END
		(((uint32_t)1220 - 1) << (32 - 11)) - (((uint32_t)1177 - 1) << (32 - 11)), // LOOP_PHASE_LENGTH
		uint16_t(UINT16_MAX * DECIBEL_SHIFT(-4.5)), // INITIAL_ATTENUATION_SCALAR
		RATE_NORMALIZED_ENV_COUNT(0.00), // DELAY_COUNT
		RATE_NORMALIZED_ENV_COUNT(6.20), // ATTACK_COUNT
		RATE_NORMALIZED_ENV_COUNT(20.01), // HOLD_COUNT
		RATE_NORMALIZED_ENV_COUNT(20.01), // DECAY_COUNT
		RATE_NORMALIZED_ENV_COUNT(100.02), // RELEASE_COUNT
		NEG_INV_COUNT(100.02), // RELEASE_COUNT
		SUSTAIN_DROP(5.5, 20.01), // SUSTAIN_MULT
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
		(int16_t*)sample_2_oboe_oboeax3, // sample
		true, // LOOP
		10, // LENGTH_BITS
		(1 << (32 - 10)) * CENTS_SHIFT(32) * 44100.0 / NOTE(88) / AUDIO_SAMPLE_RATE_EXACT + 0.5, // PER_HERTZ_PHASE_INCREMENT
		((uint32_t)997 - 1) << (32 - 10), // MAX_PHASE
		((uint32_t)992 - 1) << (32 - 10), // LOOP_PHASE_END
		(((uint32_t)992 - 1) << (32 - 10)) - (((uint32_t)958 - 1) << (32 - 10)), // LOOP_PHASE_LENGTH
		uint16_t(UINT16_MAX * DECIBEL_SHIFT(-4.5)), // INITIAL_ATTENUATION_SCALAR
		RATE_NORMALIZED_ENV_COUNT(0.00), // DELAY_COUNT
		RATE_NORMALIZED_ENV_COUNT(6.20), // ATTACK_COUNT
		RATE_NORMALIZED_ENV_COUNT(20.01), // HOLD_COUNT
		RATE_NORMALIZED_ENV_COUNT(20.01), // DECAY_COUNT
		RATE_NORMALIZED_ENV_COUNT(100.02), // RELEASE_COUNT
		NEG_INV_COUNT(100.02), // RELEASE_COUNT
		SUSTAIN_DROP(5.5, 20.01), // SUSTAIN_MULT
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

const uint32_t sample_0_oboe_oboecx3[512] = {
0x00000000,0x018200e3,0x014b0187,0x00ca00d6,0x02e501b2,0x030f048a,0xffaf0100,0xfc53fe9c,
0xfca5fafa,0xfac7fb91,0xfc3afb95,0xfedefca4,0x02f9037a,0x015f0118,0x04b30318,0x019e01de,
0x069d03a8,0x01bd0752,0x05100110,0x05680869,0xfda5fe11,0xfa5ffd1b,0xf58df60f,0xfe3ffc05,
0xf3eef99f,0xfb2ff6ac,0xf6adf808,0x0266fc32,0x021b02bf,0x05420427,0x03a00544,0x00e30148,
0x00c50111,0x00350066,0xfdb0ff75,0xfaf1fc3a,0xfab5fab9,0xfcc2fb60,0x027bff27,0x0556035e,
0x0a4a08c2,0x0a8f0aa2,0x0b6f0b70,0x0c090bea,0x0a630b1d,0x08080900,0x012a0552,0xfdcafefc,
0xf5b3fa33,0xf7def579,0xf9f2f9d4,0xf947f8f8,0xf900f9ab,0xf870f7c3,0xfc2afa5c,0xfe7cfdfc,
0x025fff52,0x05a004f4,0x043f051d,0xfe0601b6,0xfc62fc69,0xfc15fc6e,0xfbb0fbd2,0xfb56fb8e,
0xfd7dfbce,0x019cffa6,0x05a303a2,0x088e0701,0x0bf70a8a,0x0f370d6b,0x0e9c0fdc,0x0bbe0d2a,
0x04d508d2,0xfe07010a,0xfad5fc07,0xf90ff9f7,0xfa56f97d,0xfc6afb1d,0xfce3fcc4,0xfc61fcc4,
0xfb6cfba4,0xfcf8fc3b,0xfe14fdb6,0x00dfff72,0x02090198,0x01a801f9,0xfddf0022,0xfb06fbea,
0xf95bfa84,0xf903f8e1,0xfa3ff9c8,0xfd27fb50,0xffc3fe4f,0x02120139,0x059e033a,0x0c0808c3,
0x12f80fb0,0x159d14f4,0x0dc212e3,0x03740894,0xf9cbfeb6,0xf3ebf606,0xf5a9f39e,0xfb6ff8b5,
0xffadfdd9,0xfdbfff8a,0xfa25fb74,0xf9c3f9ff,0xfc60fa11,0x0179ffbd,0x02a401fc,0x017f0311,
0xfe24ff11,0xfdf4fe37,0xfc37fc5a,0xfba9fcf3,0xf9aafa2c,0xfa82f9fc,0xfbc9fae1,0xfe8bfd4f,
0xfdf8fe15,0x03370004,0x0c4b06d7,0x1aa8136f,0x20b41fc5,0x17c61d7e,0x058c0fc7,0xf54afc26,
0xebebeffd,0xeebaeb51,0xfbf2f482,0x05880269,0x014004d8,0xf764fc43,0xf4bbf46a,0xfcb0f835,
0x04c8011a,0x07c906fe,0x037f067e,0xfa9bff05,0xf823f861,0xfa23f8a9,0xfe29fc70,0xfc4ffdf0,
0xf9c8fa6e,0xf9a0f9b0,0xfaa5fa20,0xf8b6fa2f,0xf9adf7d7,0x0ab3fff0,0x24491742,0x32612ebb,
0x24662e66,0x0a5817c4,0xf298fe05,0xe2d5e898,0xe3b0e13e,0xf8d5eb5c,0x0ef506d0,0x09320fd4,
0xf429fe79,0xea53ec52,0xfac9f038,0x0d7f0548,0x13f512fc,0x06dc0f86,0xf367fc90,0xe9b2ec7d,
0xf3a3ec59,0x01bafb9f,0x02320415,0xf966fe01,0xf5aef677,0xf938f758,0xf49af871,0xeec0efd4,
0x0409f54d,0x2ee61847,0x479040f1,0x347341ec,0x135a2462,0xf02b01de,0xd869e12e,0xd2a1d364,
0xf57fdd3a,0x1ba00ddf,0x17db1f2c,0xee3a054f,0xdba9de82,0xf231e344,0x177a0561,0x26112346,
0x11ee1fdc,0xea5bfecc,0xd173d927,0xe21ed585,0x036ef304,0x0fbf0e81,0xfe1b08e5,0xede4f380,
0xf2beee68,0xf218f569,0xe676ea66,0xfff8edd9,0x36551a09,0x54254cc8,0x3cb34c44,0x27dc2eba,
0xf9631941,0xc659d609,0xc645c5b5,0xe401cc24,0x2d3a0d31,0x22ab3270,0xf59f0cd2,0xca15db21,
0xe08ecc85,0x1732fbe0,0x38422cdd,0x27be37a5,0xe7650a5e,0xb6a3c8e0,0xc57eb60b,0xfc08dea5,
0x1e221434,0x09961906,0xe9d5f828,0xe62ae3b7,0xedceec35,0xe4c6e8a3,0x0201ed64,0x3f8a1f6e,
0x5ba75655,0x397e4d06,0x373e31c4,0x0f7735ca,0xbedada93,0xbff4be4d,0xd7c2c0d6,0x31cb05d4,
0x30143ffd,0x011a17ec,0xc799e5a0,0xca07bad9,0x08d6e8d4,0x430827f5,0x41e84e33,0xf1b01f79,
0xa83ac6d0,0xa6669cd8,0xe7a2c1cf,0x26850cf3,0x1efe2c8e,0xf2f107db,0xddfee4ce,0xe10ade25,
0xe308e167,0x08cef094,0x492927c5,0x645c60f3,0x33c04fc2,0x3d3d2c46,0x1fbf4697,0xc040e084,
0xc35cc209,0xc442b8c6,0x3980fb98,0x314549ac,0x14241dbc,0xc7f8f575,0xbe57b149,0xf8c2db6c,
0x3cb91944,0x512954c2,0xfeac2f94,0xa994cf1c,0x93ca94b0,0xcd35a763,0x1fb3fa7c,0x2af83030,
0x026718ff,0xdbaaebd1,0xd5ccd61b,0xdae6d585,0x0962ed2c,0x51562c0e,0x6cab6a4d,0x3144540a,
0x3eef2816,0x26774dd9,0xc2f2e38c,0xca19c69c,0xba9cb959,0x36dcf1e2,0x2e7c49c1,0x1f731ecd,
0xd0a20496,0xb8cdafcf,0xed81d4d2,0x32b20af3,0x566753ae,0x055a3595,0xaf77d741,0x8a0393f6,
0xbaa896ed,0x1601ea42,0x2e3b2d43,0x0e4a21d0,0xe07df69c,0xd03dd3aa,0xd42dcf7d,0x05d0e6b6,
0x56ef2da1,0x752c7202,0x34545a98,0x433829fc,0x273951f8,0xc3d6e251,0xce81c9ad,0xba83bd80,
0x3235ee44,0x2b0d464e,0x21e11d16,0xd7a40a24,0xb79eb3ce,0xe8d4d164,0x2bfe04a9,0x54024ea6,
0x05f234b8,0xb3d7da8a,0x895b9660,0xb0d091ae,0x0bf5deed,0x2ba62669,0x1353236e,0xe71ffd33,
0xd177d8a6,0xd1d6cd10,0x0351e591,0x55342a62,0x77457311,0x38415cfd,0x49042e4e,0x2b105816,
0xc373e2ba,0xd0c8cafa,0xbbeec0cf,0x30e3ed4e,0x29364485,0x2250199c,0xdc0d0fa2,0xb724b4a4,
0xe535d193,0x25fcfd59,0x518b4b03,0x03533247,0xb671da3e,0x891d98a3,0xac478f18,0x05c3d8b1,
0x28fb2168,0x14a222d2,0xec9601ab,0xd49cdcb1,0xd498d05e,0x028fe647,0x53922895,0x78527222,
0x3bbe5f5b,0x4bc8321d,0x2f285c40,0xc68fe6ff,0xd0f2ccd1,0xbacfbe70,0x314eecba,0x24c342e2,
0x225b1702,0xdeeb122d,0xb8a7b71f,0xe139d09b,0x1bfff458,0x50d045e9,0x053a33c6,0xbb1edd51,
0x8b5a9c85,0xaafa8f19,0x016ad684,0x24e01be0,0x15e121df,0xf0ef045f,0xd7e4e1f1,0xd305d089,
0xffc7e3d7,0x50082501,0x797970fb,0x40cc633d,0x4f8f366f,0x2f435e02,0xc551e650,0xcfb6cbb1,
0xbe2abf57,0x2a19ecfd,0x23de3bb7,0x210917ef,0xe06110bb,0xb7d6b93c,0xdb29ccc6,0x1339ecac,
0x4c943e87,0x068932fe,0xbf40df71,0x9038a1bd,0xab839213,0xfc73d4fb,0x204b1597,0x17a62028,
0xf6c10879,0xdc31e78e,0xd157d2c6,0xfb30dfb8,0x4b521fed,0x7b1d6ec8,0x457866fa,0x52a23a99,
0x31976139,0xc262e660,0xcf5ec8c4,0xc04ec174,0x299ded6d,0x23463b13,0x200a16c8,0xe12e1115,
0xb784b999,0xda0acc15,0x10b4eac3,0x4b303c2e,0x064b325b,0xc026dfba,0x916fa2fb,0xabd992f3,
0xfc1fd4d7,0x20301552,0x17ad2028,0xf7840879,0xde15e965,0x00000000,0x00000000,0x00000000,
0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
};

const uint32_t sample_1_oboe_oboefx3[640] = {
0x00000000,0xffa0feaa,0x0079ffcc,0x002c00cc,0x01780124,0x009f00f6,0x007bff85,0xffd2ffd5,
0xff6eff91,0x0076007f,0xffcd00a9,0xfeffff44,0xfdbefeb7,0xff8dfea0,0x00c1ff84,0x0221013c,
0x01930347,0x01160185,0xfef6feaf,0x0126001b,0x018e01f6,0x006d0336,0x0484016b,0x07f806a6,
0x00fa0401,0x00e7034d,0xfd87fe90,0xfa80fd90,0xfa20f8d5,0xfb28f9f2,0x0064fbed,0xfc430133,
0x01f3fc50,0xfee70298,0xfd2afa5c,0xfe33fe86,0xfe5ffd7d,0xfe640048,0xfe19fd8a,0xfc4ffdf9,
0xff1cfcf3,0xfe07ff3e,0x04e0ffe3,0x0a63090b,0x0af90af1,0x084f0979,0x04f10776,0x00e4022c,
0xfd6cffc8,0xfbbdfbf4,0xfbfffc43,0xfccdfbaa,0xfe32fd8c,0x00b8ffa6,0x00210130,0xfebcfe98,
0xfeb2ff13,0xfddcfddb,0xfd1afdba,0xfc9cfc43,0xfddafd81,0xfd17fe3f,0xfc87fc9e,0xfff1fdb8,
0x055e0200,0x0ab20829,0x0c130c51,0x0b290bfc,0x0419088d,0xfbefff3f,0xf769f8ae,0xf5faf696,
0xfa6af760,0xff7afda8,0x027600a8,0x03510380,0x00ab0229,0xfec0ff19,0xfe08ff35,0xfcf4fce5,
0xfeb8fe11,0xfdc1feb5,0xfb57fc7e,0xfa06fb06,0xfaeff97e,0x0152fdc1,0x09ba04e9,0x10f40d6e,
0x112b11fb,0x089c0dcf,0xfea7036f,0xf5fffa2b,0xf369f379,0xf7b3f5c0,0xfda2fa3b,0x02e300e5,
0x04960437,0x02de0434,0x004b017d,0xfdf5fedf,0xfdb0fd75,0xfe26fded,0xfe51fea0,0xfbdbfd39,
0xf7abf9c7,0xf6bdf6bb,0xfbf7f83d,0x0726013e,0x11370cb7,0x169d154d,0x0ff5145f,0x04b90af3,
0xf6f4fd5a,0xef47f266,0xefd9ee43,0xf7c9f35d,0x0075fbdf,0x07240510,0x071a06fd,0x04e606f7,
0xff9301f1,0xfdb8fea3,0xfc5cfcba,0xfcd6fcc5,0xfcc0fd47,0xf8b1fb90,0xf3b2f61e,0xf33df249,
0xff4cf77a,0x10dd0826,0x1de61859,0x1cb31f0c,0x0eaa16cf,0xfac1048d,0xed8af324,0xe8f2e9b1,
0xf00dec5f,0xfbfff570,0x084b02b4,0x0b150a61,0x09590af7,0x029a05f8,0xfe0fffd7,0xfc4dfd1e,
0xfca0fb95,0xfd22fd59,0xf956fb83,0xf139f588,0xecb2ee61,0xf38bee49,0x07f1fc84,0x1f631462,
0x290426da,0x1f102637,0x0711143d,0xeec6f9ec,0xe2abe6c1,0xe51ae22c,0xf291eb21,0x00b6f991,
0x0b170705,0x0d7c0ce4,0x0ac10cd9,0x03f4076d,0xfe420145,0xfa77fbb0,0xfa4bfa38,0xf950fa3c,
0xf489f806,0xeacbef30,0xe815e7ef,0xf7d2ed16,0x14de05ec,0x2fa923ee,0x33ce354f,0x1d892a8e,
0xfc130d9e,0xe26aecf1,0xd81bda8e,0xe28fdb51,0xf630ecd5,0x0931ffaf,0x10fb0eba,0x11a211b6,
0x0afd0fd1,0x01e406b3,0xfb27fe62,0xf8c4f900,0xf8c7f8ba,0xf81ff8a8,0xecf5f3ff,0xdfffe5d7,
0xe1ffde2b,0xff84ed1b,0x296c14a4,0x435d39c6,0x3c9244ef,0x16f82bb1,0xeae200b4,0xd024da2f,
0xd11acd48,0xe674d9e6,0xfee9f398,0x1159093a,0x14d814dc,0x129013a1,0x0a0b0f5a,0xffce048a,
0xf917fc21,0xf672f6b5,0xf7d7f762,0xf386f6cd,0xe324eca1,0xd398d98d,0xe205d559,0x10dff706,
0x40372b6f,0x52f34da0,0x3e1d4e7e,0x09202521,0xd7d0eea9,0xc235c839,0xcfabc5eb,0xeee0df58,
0x08b4fcc7,0x1a4f12bb,0x163b19a2,0x11de1351,0x06810daf,0xfb17ff99,0xf4e9f791,0xf451f36d,
0xf772f65f,0xefa2f5ae,0xd6dbe42f,0xcc57cd7e,0xeb88d5bd,0x28fb09ae,0x5498425d,0x5e095ea2,
0x36164fdf,0xf4dc16c9,0xc2d7d765,0xbc16ba7a,0xd85cc723,0xfa58ebab,0x14dd06cc,0x20df1ee2,
0x15161a58,0x0f571396,0xfd2506b5,0xf171f5d7,0xf067ef60,0xf5d3f3b4,0xfa02f9a9,0xe809f57b,
0xc975d6a8,0xccacc4ce,0x01c4e22a,0x3ee5222f,0x63cd546e,0x5bec6661,0x25c34589,0xd996ff5c,
0xb38fbf54,0xc00db53e,0xe7a8d3b7,0x061ff704,0x23b3173c,0x1cd922e6,0x157317e5,0x03ed0f27,
0xf4dbfc08,0xeb1cee73,0xf287ebed,0xfb76f776,0xf868fd64,0xd89deb56,0xc1c1ca98,0xdb52c65c,
0x1dc5fc3b,0x4ff93999,0x6cc6617c,0x51eb697b,0x01b52b0d,0xc695de43,0xb111b63e,0xcf10bb51,
0xf117e131,0x12bcffe9,0x2bba268f,0x1a8a25b6,0x13c5132e,0xfb790de2,0xded7e7a9,0xeb05e42c,
0xf51cf0fa,0x0328fb08,0xf24000c5,0xc9c8dd74,0xc33fbf10,0xf541d84d,0x362515b3,0x65dd5255,
0x6a0e6d77,0x37f3590a,0xe1460b61,0xb3eec332,0xbad6b20e,0xe01bccd3,0xfd75eed5,0x25b81319,
0x250c2be9,0x15511a9e,0x0b2a1254,0xe8f1fb82,0xdd08dc9f,0xebe2e3d1,0xf9d4f191,0x020c020f,
0xe215f642,0xc1b1ce68,0xd6d6c2fd,0x1310f41d,0x4e2e3292,0x6db863b8,0x5aaf6c55,0x0d9238dd,
0xc890e462,0xb383b7ed,0xcebfbc7b,0xea8fdf3d,0x0feaf9cd,0x2c06251b,0x1dc524eb,0x161a1838,
0xfda0112a,0xd41ae504,0xe066d664,0xebbde686,0x00f0f49f,0xf86403cb,0xd344e60a,0xc65cc5d7,
0xf244d78d,0x2f0f0f51,0x633b4cb1,0x69106c96,0x3f675b04,0xe7cd139c,0xb6a7c7c0,0xbe49b5af,
0xe0d8cf76,0xf899ec1c,0x24200f1a,0x240e2900,0x1c5e1d34,0x0dd918ad,0xe74dfce9,0xd328d69b,
0xe373da68,0xf164e74d,0x0222fe46,0xe803f914,0xc925d6e1,0xd88ac80f,0x11d1f49f,0x48e72e99,
0x6b175f80,0x5b38699c,0x0e4a3a84,0xcf98eadf,0xb79bbd98,0xd35fbf16,0xe8cce369,0x0d19f50f,
0x2730208a,0x1ab1223b,0x1a3b1a37,0x021514e3,0xd193e3b6,0xdec3d2f0,0xe379e46d,0xfb0eec26,
0xf80f00f8,0xd6b0e66f,0xca64ca5c,0xf332da36,0x30350f42,0x60d74dd4,0x65c66838,0x43985b73,
0xe8e916cf,0xb934c95b,0xc3f0bac6,0xe38cd34b,0xf3c0eb84,0x1af3076e,0x243e2498,0x20b42131,
0x15501e81,0xea100322,0xcc1cd3fb,0xdbe5d1c7,0xebafe20c,0x02f9fa6e,0xee86fe15,0xcb57dbe2,
0xd44bc66d,0x0de2ef9f,0x490b2c70,0x6d325fac,0x5f6b6d24,0x181644e0,0xca2cec1c,0xb4d8b54a,
0xd1bdc0da,0xec19e3f5,0x0758f19b,0x28861f11,0x1f6f2653,0x1dc81f0e,0x012d126d,0xd54de964,
0xd57dcf0a,0xe1c7dec9,0xf733e91f,0xfbbbfecd,0xdba9ee06,0xc7eacc51,0xefacd56b,0x2ccb0d21,
0x60a84a49,0x6e916e39,0x457f6152,0xe90f17d1,0xb790c8fa,0xc10bb5a8,0xe50dd326,0xefb6ebeb,
0x1af30358,0x2711278e,0x1e012007,0x146a1c3d,0xeaf30424,0xcb98d3ac,0xdef1d32a,0xe83ce28d,
0x006cf5fd,0xef3bfd4b,0xcc33dcbd,0xd39cc5e3,0x0ae9ee9b,0x492a2945,0x700c6287,0x62516ff7,
0x19ca47e8,0xcb08ec7a,0xb397b685,0xd447c03a,0xeae4e569,0x03eeef61,0x26bd1a89,0x205e2686,
0x1b741c90,0x061315c3,0xd2eceb5c,0xd39ccbda,0xe1fdde6c,0xf667e886,0xfdea006c,0xdf05f206,
0xc4edcbe2,0xea84d0c6,0x27fa0757,0x631d4a1d,0x70d6706d,0x4a4b63f2,0xf1161e86,0xb485cd3f,
0xc0bdb1b6,0xe4dbd4d5,0xee4feb4f,0x1a38018e,0x26bd2641,0x1a8c1db0,0x153e1cc2,0xec1b03ca,
0xd00bd6e1,0xdc74d3a4,0xe889df65,0x00d7f838,0xf13efd63,0xcceddf6b,0xd099c4f2,0x05c7e8e5,
0x47cb2683,0x6eb7616c,0x664a7252,0x1d894c57,0xce49ef8c,0xb62dbaa8,0xd74ac1c4,0xeb47e7f0,
0xff84ed1c,0x25be17cc,0x1f5125ec,0x17d21803,0x077a14f7,0xd6deef34,0xd59fcf09,0xe2f6dfa2,
0xf6aae81f,0xfdd3010f,0xe02bf1c7,0xc44acd35,0xe770cea8,0x23cd03c2,0x5f7c451c,0x721c703f,
0x4be8650a,0xf57a2279,0xba4cd32c,0xc09ab466,0xe5fcd39a,0xebc9eca9,0x162dfc9e,0x25fa2490,
0x191b1d12,0x17bb1b79,0xf064087e,0xd24fda9d,0xdbc2d562,0xe458de0a,0x0000f35f,0xf69100f4,
0xd093e384,0xcf6dc818,0xffc3e49d,0x41981f10,0x6d5c5c8a,0x670a715d,0x280f5084,0xd4d0f9c2,
0xb3ecbc06,0xd22fbeeb,0xeb9de455,0xfadaea5d,0x26121543,0x1e63291b,0x1a7f19a8,0x064a13f1,
0xda10ef33,0xd5edd1d1,0xe0f2de99,0xf567e763,0x0053008e,0xe37bf59c,0xc7bfd04e,0xe4ebcf87,
0x1ed5003c,0x59a53f10,0x70826be0,0x4f9465c7,0xfb73282a,0xbe17d79e,0xc042b4f8,0xe411d37d,
0xe9fdeaa1,0x132af8bf,0x288524ea,0x17671e09,0x16541a2a,0xef470734,0xd3c2dbbb,0xde08d68a,
0xe5cae02e,0x01daf46d,0xf8a9034f,0xd048e432,0xcfe6c96e,0xfd15e285,0x3d7f1bdd,0x6a6858bb,
0x668d6f42,0x29335140,0xd8b3fc4f,0xb889c122,0xd276c0ee,0xebb4e4b1,0xf8cceb15,0x22ce10c6,
0x1e782846,0x179916f0,0x0a7e1586,0xdf53f455,0xd67dd50d,0xdfcfdd0a,0xf0c2e4a1,0x0337fe8f,
0xe959fb88,0xcacad503,0xdeb6ce4a,0x15a7f7b0,0x53c436fc,0x6e726773,0x53ed67a9,0x00af2db5,
0xc28fdb8a,0xc0c1b8e3,0xe28fd276,0xeb7cea00,0x00000000,0x00000000,0x00000000,0x00000000,
0x00000000,0x00000000,0x00000000,0x00000000,
};

const uint32_t sample_2_oboe_oboeax3[512] = {
0x00000000,0xffbeffbe,0xffbeffbf,0xffacffb5,0xffbeffb5,0xffc3ffbe,0xffc3ffc2,0xffd4ffc9,
0xffc9ffe0,0xff7fffb5,0xffdbff9f,0xff64ffbe,0xfff8ff88,0xff9cffd8,0xffabffa3,0x0007fff9,
0xffde0005,0xff67ffe7,0xffeeff5f,0xffc2000c,0x002c0017,0xffd20044,0xfff3ff7f,0x001f0099,
0x0005fff4,0xfe9dfee9,0xff29ff88,0xffbefeef,0x005d0069,0xfda2ff7f,0xfefdfd87,0xfeacfec3,
0x009dff2f,0x004b00ab,0xff2dfdbd,0x02b8036c,0x005900fb,0xfd08fea9,0xff08fc1e,0xff5dffa2,
0xfdd0ff76,0x0334016a,0x02b803b8,0x00610236,0xfd7efe06,0xfff8fd4d,0xfc8d0208,0xfd00f861,
0x007d0176,0x006e00b9,0xff67fe48,0x018cff73,0x011101ec,0xff2effe5,0xffb0fee2,0x01a301e0,
0xff8ffff8,0x0186ffb6,0xfd120083,0x0203fdc4,0xff7301c3,0xfe720011,0xff80ff3b,0xfef0fed8,
0xfc7cfdd2,0xfc5cfc65,0xfd22fc34,0x0147ffc7,0x009a00aa,0x00fbff2e,0x036e0379,0xfdcf018f,
0xff25fcd5,0xffa3ff73,0x00cd0022,0x01dd00d3,0x023602bc,0x00c5029d,0xfbe7feae,0xfcc6fb22,
0xfffd0008,0xfd74fc26,0xfd1ffedf,0xfc8afd0a,0xfcdefc9f,0xffb2fe2b,0x028a009e,0x030f02ce,
0x019b0268,0x01e601da,0xffe900e0,0xff53ff7a,0xff75002c,0x02c40028,0x014a0250,0x00a20125,
0xfd7bff8f,0xfb2ffc0d,0xfc01fbec,0xfc4dfaf1,0xfd35fce2,0xfefffdc6,0xff92ffe4,0x0031ff4f,
0x033d01ea,0x03430418,0x02b9024e,0x00a60218,0x00f3009c,0xfee40017,0x0000ff27,0x018e0089,
0xff680106,0xfe9dfe97,0xfd9dfe89,0xfb86fcad,0xf922faac,0xf90bf880,0xfb6afa4a,0xfe91fc9a,
0x041c0101,0x07f1062b,0x071a08ad,0x020003a5,0x00ac01c9,0xff6bff62,0xffe600a5,0xfedbfdb3,
0xffec00d6,0x0075ffde,0xfdf7fff3,0xfd1afdae,0xfb22fb5a,0xf93bfa99,0xf790f7b3,0xfc2ef9ce,
0xfd7dfce3,0x017bfe7f,0x0798040f,0x0b850b45,0x0652083d,0x03ca0588,0xfdb0006d,0xfbd5fc50,
0xfbd8fbb6,0x01f6fec2,0x02ed02ca,0x000f02f1,0xfd9bfe74,0xf83cfbdd,0xf46ef5ca,0xf556f2ed,
0xfb1cf814,0xfef3fcf8,0x03b40102,0x0a0606ff,0x0ab10a68,0x07910a53,0x004002dd,0xfd88ff7f,
0xfc4cfd1e,0x0101ff75,0x0022ffa3,0x02910304,0xfcf60047,0xfaa4faa7,0xf874fb3e,0xf6d1f5f8,
0xf660f67c,0xf700f714,0xff5efa59,0x0ab402e7,0x06760bd3,0x0cd7064b,0x098a0db1,0x01f70805,
0xfc34fdbd,0x0097fd8d,0xfd4dff2d,0x00a5ff1b,0x02d8021f,0x01a80314,0xf93bfecc,0xf5bcf3c7,
0xf5dcf6c0,0xf61bf516,0xf5fdf717,0xfc9ef433,0x06f504ac,0x0a79097f,0x10c80d8a,0x0f0911a3,
0x05d80abf,0xfd58003c,0xfb1dfc70,0xfe0efd4a,0x012bfeff,0x028a0294,0x010d020c,0xfcb1ff33,
0xf3fcf7fb,0xf130f2cd,0xf16ef139,0xf469f0d0,0x010dfb99,0x092b0313,0x108d1012,0x118210e4,
0x0f001182,0x044d0a54,0xfb8cffa7,0xf805f90a,0xfaf9f800,0xffc7fec2,0x05ccffbe,0x037f0878,
0xfb25ff4a,0xf1f1f4e1,0xed1fef32,0xed65ecda,0xf36cef72,0xfed6f7ed,0x0cdd06f0,0x15d2121b,
0x18cd1877,0x11c61703,0x04ae0b33,0xf718fceb,0xf60df4cf,0xfac4f7ed,0x04b7fe7e,0x09540912,
0x04310756,0xfad70113,0xecbbf32d,0xe66fe8a0,0xe60ce55a,0xeeafe98e,0xfd3cf5cf,0x11fa079e,
0x1f8a1979,0x227a234d,0x15111d8f,0x009709ba,0xf2f6fa09,0xefc9eeba,0xfa2af301,0x07b1033f,
0x0db60a81,0x064d0cc9,0xf698000c,0xe752ecd3,0xe052e352,0xdba9dea9,0xe9abde89,0x009af59d,
0x17d40b7a,0x2ecd2463,0x2e003159,0x16c8248f,0xfbb106d8,0xeaccf2ec,0xec4be651,0x0059f885,
0x0d3e05b9,0x157313d6,0x07f511d0,0xf2c2fe0b,0xdeeae77e,0xd2a0d82c,0xd6a9d2e3,0xef89e0c4,
0xf949f7c5,0x21eb089f,0x3ad731d9,0x32143a2a,0x194a276d,0xfa54094b,0xe476edb4,0xe55ee1a3,
0xff33ef0b,0x183f0d6d,0x1c351e15,0x082a13b5,0xef90fb32,0xdb25e6f2,0xc7f7cd6a,0xce29c91f,
0xe0d2d680,0xffcceef8,0x27081295,0x49883bec,0x3d7c4906,0x1b602c73,0xf5dd0998,0xdde9e66f,
0xe0f1dbd5,0xfee6edb3,0x2299120f,0x27d02931,0x08d51d68,0xe289f117,0xd6dddcd5,0xc2fcccb9,
0xc59ec0fa,0xdf20cca8,0x0399f520,0x2c1814d7,0x4ef44130,0x49f35386,0x1bf03435,0xef38045d,
0xd7a8e0d8,0xdd59d6a4,0x00e5ed05,0x2ce3169a,0x37b83a89,0x017b2117,0xddc7e840,0xd22bd96d,
0xb714c4c5,0xc142b36b,0xde33d14d,0x024beefc,0x30e117c6,0x5aca4afe,0x4e465b99,0x1c6c36ea,
0xeac90297,0xd468dc7e,0xd889d360,0x0482e9cc,0x38c12179,0x388441bc,0xfe531f2e,0xdac8e479,
0xccbbd4f1,0xb441c166,0xba45af71,0xdc21ca17,0x00f8f112,0x2f4b1282,0x64a64fb5,0x55f56695,
0x1bfe3a59,0xe888fff4,0xd0e3db18,0xd5eccf2a,0x062fe8db,0x3ffd261c,0x439b4db4,0xf33c1c90,
0xd76bdd08,0xccf2d50e,0xb104bc88,0xb959b09f,0xdcdec7bd,0xff11f0bc,0x2b381059,0x69dc4e09,
0x5d616fdc,0x1d143ce1,0xe5de0016,0xccacd650,0xd3eacaf4,0x0a00eb21,0x47de2bde,0x42bf51b6,
0xf18c1cd5,0xd237d69a,0xcc68d3ae,0xad6cba95,0xbd02b017,0xd9bbc970,0xfa32ec45,0x2e450e91,
0x6c595068,0x620872f3,0x1e6c4244,0xe033fc88,0xc6afd0c3,0xd313c7b8,0x0f29ec44,0x4e7b32b9,
0x47695a07,0xeaf6183b,0xcce8d18f,0xcbf6cfac,0xaf1bbb6a,0xbc52b045,0xe0d8ccd5,0xfa5ef0a7,
0x27960bb1,0x6ce84d31,0x612574c9,0x1c9b3e48,0xe2c8ff63,0xc9d7d1d4,0xd290c8f2,0x0e4aec34,
0x51733404,0x45075984,0xe97718ea,0xce05ce77,0xcab0d05f,0xae4eba85,0xc220b27a,0xdc22ce11,
0xf726ecae,0x2b650ad7,0x6ea54f8c,0x65c0790b,0x1add3fea,0xe11dfb56,0xc197cec6,0xd4a3c3c2,
0x15dff2c5,0x575639ef,0x474e5fe0,0xe38c13fc,0xc7d1ca39,0xc946cc3e,0xae3bba7a,0xc245b02f,
0xe9c0d7e5,0xf643f1bc,0x283b076b,0x6dec4f6b,0x5f4473e7,0x193a3ba1,0xe3d1fc05,0xc4a6d1a7,
0xd2b3c45a,0x15d3f0c3,0x580a3b8d,0x434f5e62,0xe1a21033,0xc8cfc90c,0xca11cec2,0xaec9b9e7,
0xc647b471,0xe638d836,0xf2f4edae,0x2b3e0729,0x724354e9,0x60c476de,0x16653ad5,0xe0f5f891,
0xc05cce7f,0xd4d7c135,0x1c04f691,0x5ae93f98,0x41905efc,0xdde40b8d,0xc618c7a1,0xc4efc94b,
0xaf47b858,0xc976b512,0xeca0def4,0xf665f15b,0x2b480952,0x6e535366,0x5b1a7093,0x15a737bb,
0xe047f81e,0xc278ce77,0xd62fc2f8,0x1e4df7ff,0x5bff42d8,0x3b125b97,0xda4b06ea,0xc5e5c560,
0xc52bc969,0xafaab832,0xcc58b765,0xefe0e23d,0xf4e9f27d,0x2d9b08bb,0x6e4654d8,0x58926fa1,
0x145335fa,0xe038f5fe,0xc152cd34,0xd70ec2d6,0x20b4f972,0x5cde44b4,0x37b35c66,0xd60d001e,
0xc485c563,0xc38ec7fb,0xb05fb713,0xce23b8ec,0xf06de3bd,0xf585f2f2,0x2dac092d,0x6e3e54ea,
0x581d6fa6,0x138434f4,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
};
