#pragma once

#include "Arduino.h"
#include "AudioStream.h"
#include <math.h>
#include <sample_data.h>

#define UNITY_GAIN 65536.0  // Max amplitude
#define SAMPLES_PER_MSEC (AUDIO_SAMPLE_RATE_EXACT/1000.0)
#define AMP_DEF 69

class AudioSynthWavetable : public AudioStream
{
public:
	AudioSynthWavetable(void) : AudioStream(0, NULL) {}

	void setSamples(const sample_data * samples, int num_samples) {
		this->samples = samples;
		this->num_samples = num_samples;
	}
	
	void setLoop(int start, int end) {
		loop_start = start;
		loop_end = end;
		loop_length = loop_end - loop_start;
		
		length_bits = 1;
		for (int len = loop_length; len >>= 1; ++length_bits);
		loop_phase = (loop_length - 1) << (32 - length_bits);
	}
	
	void setFreqAmp(float freq, float amp) {
		frequency(freq);
		amplitude(amp);
	}
	
	void setSampleNote(int note) {
		sample_freq = noteToFreq(note);
	}

	void amplitude(float v) {
		v = (v < 0.0) ? 0.0 : (v > 1.0) ? 1.0 : v;
		tone_amp = (uint16_t)(32767.0*v);
	}

	static float midi_volume_transform(int midi_amp) {
		// 4 approximates a logarithmic taper for the volume
		// however, we might need to play with this value
		// if people think the volume is too quiet at low
		// input amplitudes
		int logarithmicness = 4;

		// scale midi_amp which is 0 t0 127 to be between
		// 0 and 1 using a logarithmic transformation
		return (float)pow(midi_amp, logarithmicness) / (float)pow(127, logarithmicness);
	}
	
	static float noteToFreq(int note) {
		//return 440.0 * pow(2.0, (note - 69) / 12.0);
		//float exp = (note + 36.37631656) / 12.0;
		float exp = note * 0.083333333 + 3.0313597;
		float freq = pow(2, exp);
		return freq;
	}
	
	void env_delay(float milliseconds) {
		delay_count = milliseconds2count(milliseconds);
	}
	void env_attack(float milliseconds) {
		if (milliseconds <= 0) {
			milliseconds = 1.5;
		}
		attack_count = milliseconds2count(milliseconds);
	}
	void env_hold(float milliseconds) {
		if (milliseconds <= 0) {
			milliseconds = 0.5;
		}
		hold_count = milliseconds2count(milliseconds);
	}
	void env_decay(float milliseconds) {
		if (milliseconds <= 0) {
			milliseconds = 100;
		}
		decay_count = milliseconds2count(milliseconds);
	}
	void env_sustain(float level) {
		if (level < 0.0) level = 0;
		else if (level > 1.0) level = 1.0;
		sustain_mult = level * UNITY_GAIN;
	}
	void env_release(float milliseconds) {
		release_count = milliseconds2count(milliseconds);
	}
	
	// Defined in AudioSynthWavetable.cpp
	void play(void);
	void stop(void);
	void parseSample(int sample_num, bool custom_env);
	void playFrequency(float freq, bool custom_env=0);
	void playNote(int note, int amp=AMP_DEF, bool custom_env=0);
	bool isPlaying(void) { return envelopeState != STATE_IDLE; }
	void frequency(float freq);
	virtual void update(void);
	static void print_performance(void);

private:

	enum envelopeStateEnum { 
		STATE_IDLE,
		STATE_DELAY,
		STATE_ATTACK,
		STATE_HOLD,
		STATE_DECAY,
		STATE_SUSTAIN,
		STATE_RELEASE
	};

	uint32_t milliseconds2count(float milliseconds) {
		if (milliseconds < 0.0) milliseconds = 0.0;
		// # of 8-sample units to process
		// Add 7 to round up
		return ((uint32_t)(milliseconds*SAMPLES_PER_MSEC)+7)>>3;
	}
	//int32_t signed_multiply_32x16b(int32_t a, uint32_t b) {
	//	return ((int64_t)a * (int16_t)(b & 0xFFFF)) >> 16;
	//}
	//int32_t signed_multiply_32x16t(int32_t a, uint32_t b) {
	//	return ((int64_t)a * (int16_t)(b >> 16)) >> 16;
	//}
	//uint32_t pack_16b_16b(int32_t a, int32_t b) {
	//	return (a << 16) | (b & 0x0000FFFF);
	//}

	uint32_t* waveform = NULL;
	const sample_data * samples = NULL;
	int length = 0, length_bits = 0;
	int loop_start = 0, loop_end = 0, loop_length = 0;
	float sample_freq = 440.0, cents_offset = 1.0;
	uint8_t playing = 0;
	uint8_t num_samples = 0;
	uint32_t loop_phase = 0, loop_start_phase = 0, loop_end_phase = 0;
	uint32_t tone_phase = 0;
	uint32_t max_phase = 0;
	uint32_t tone_incr = 0;
	uint16_t tone_amp = 0;
	uint16_t sample_rate = 0;
    
	// state
	envelopeStateEnum  envelopeState = STATE_IDLE;  // idle, delay, attack, hold, decay, sustain, release
	uint32_t count = 0;  // how much time remains in this state, in 8 sample units
	float    mult = 0;   // attenuation, 0=off, 0x10000=unity gain
	float    inc = 0;    // amount to change mult on each sample
	// settings
	uint32_t delay_count = 0;
	uint32_t attack_count = 0;
	uint32_t hold_count = 0;
	uint32_t decay_count = 0;
	int32_t  sustain_mult = 0; // Why not uint?..
	uint32_t release_count = 0;

	static uint32_t
		interpolation_update,
		envelope_update,
		total_update,
		total_parseSample,
		total_playFrequency,
		total_frequency,
		total_playNote,
		total_amplitude;
};

