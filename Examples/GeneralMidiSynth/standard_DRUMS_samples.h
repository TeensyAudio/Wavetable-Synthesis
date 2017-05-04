#pragma once
#include <AudioStream.h>
#include <AudioSynthWavetable.h>

extern const sample_data standard_DRUMS_samples[46];
const uint8_t standard_DRUMS_ranges[] = {35, 36, 37, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 127, };

const instrument_data standard_DRUMS = {46, standard_DRUMS_ranges, standard_DRUMS_samples };


extern const uint32_t sample_0_standard_DRUMS_bd15[896];

extern const uint32_t sample_1_standard_DRUMS_bd15[896];

extern const uint32_t sample_2_standard_DRUMS_verbclickwave[640];

extern const uint32_t sample_3_standard_DRUMS_snare24[2048];

extern const uint32_t sample_4_standard_DRUMS_snare24[2048];

extern const uint32_t sample_5_standard_DRUMS_floortombrite[3712];

extern const uint32_t sample_6_standard_DRUMS_hatopenms[5888];

extern const uint32_t sample_7_standard_DRUMS_floortombrite[3712];

extern const uint32_t sample_8_standard_DRUMS_hatopenms[5888];

extern const uint32_t sample_9_standard_DRUMS_floortombrite[3712];

extern const uint32_t sample_10_standard_DRUMS_hatopenms[5888];

extern const uint32_t sample_11_standard_DRUMS_floortombrite[3712];

extern const uint32_t sample_12_standard_DRUMS_floortombrite[3712];

extern const uint32_t sample_13_standard_DRUMS_crash5[6784];

extern const uint32_t sample_14_standard_DRUMS_floortombrite[3712];

extern const uint32_t sample_15_standard_DRUMS_paisteping[6656];

extern const uint32_t sample_16_standard_DRUMS_chcrash[4864];

extern const uint32_t sample_17_standard_DRUMS_rideping[3072];

extern const uint32_t sample_18_standard_DRUMS_tamborine[1920];

extern const uint32_t sample_19_standard_DRUMS_crash5[6784];

extern const uint32_t sample_20_standard_DRUMS_cowbell[896];

extern const uint32_t sample_21_standard_DRUMS_crash5[6784];

extern const uint32_t sample_22_standard_DRUMS_vibraloop[512];

extern const uint32_t sample_23_standard_DRUMS_paisteping[6656];

extern const uint32_t sample_24_standard_DRUMS_ebongostone[1664];

extern const uint32_t sample_25_standard_DRUMS_mbongotone[1408];

extern const uint32_t sample_26_standard_DRUMS_quintoslap[1536];

extern const uint32_t sample_27_standard_DRUMS_quintotone[1664];

extern const uint32_t sample_28_standard_DRUMS_lowtumba[2048];

extern const uint32_t sample_29_standard_DRUMS_timpani[3968];

extern const uint32_t sample_30_standard_DRUMS_timpani[3968];

extern const uint32_t sample_31_standard_DRUMS_agogolotone[2304];

extern const uint32_t sample_32_standard_DRUMS_agogolotone[2304];

extern const uint32_t sample_33_standard_DRUMS_cabasastrk[1408];

extern const uint32_t sample_34_standard_DRUMS_maracas[1664];

extern const uint32_t sample_35_standard_DRUMS_sambawhistle[896];

extern const uint32_t sample_36_standard_DRUMS_sambawhistle[896];

extern const uint32_t sample_37_standard_DRUMS_guirodown[1408];

extern const uint32_t sample_38_standard_DRUMS_guiro2[1408];

extern const uint32_t sample_39_standard_DRUMS_bockclave[896];

extern const uint32_t sample_40_standard_DRUMS_woodblock[640];

extern const uint32_t sample_41_standard_DRUMS_woodblock[640];

extern const uint32_t sample_42_standard_DRUMS_quicadown[896];

extern const uint32_t sample_43_standard_DRUMS_quicadown[896];

extern const uint32_t sample_44_standard_DRUMS_triangle[1152];

extern const uint32_t sample_45_standard_DRUMS_triangle[1152];
