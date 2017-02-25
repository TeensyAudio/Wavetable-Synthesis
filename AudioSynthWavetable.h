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

	void setInstrument(const instrument_data& instrument) {
		this->instrument = &instrument;
	}

	void setFreqAmp(float freq, float amp) {
		setFrequency(freq);
		amplitude(amp);
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
		//440.0 * pow(2.0, (note - 69) / 12.0);
		float exp = note * (1.0 / 12.0) + 3.0313597;
		return powf(2.0, exp);
	}

	static int freqToNote(float freq) {
		return (12.0 / 440.0) * log2f(freq) + 69.5;
	}

	// Defined in AudioSynthWavetable.cpp
	void stop(void);
	void parseSample(int sample_num);
	void playFrequency(float freq);
	void playNote(int note, int amp = AMP_DEF);
	bool isPlaying(void) { return envelopeState != STATE_IDLE; }
	void setFrequency(float freq);
	virtual void update(void);

private:

	enum envelopeStateEnum { STATE_IDLE, STATE_DELAY, STATE_ATTACK, STATE_HOLD, STATE_DECAY, STATE_SUSTAIN, STATE_RELEASE };

	uint32_t milliseconds2count(float milliseconds) {
		if (milliseconds < 0.0) milliseconds = 0.0;
		// # of 8-sample units to process
		// Add 7 to round up
		return ((uint32_t)(milliseconds*SAMPLES_PER_MSEC) + 7) >> 3;
	}

	const sample_data* current_sample = NULL;
	const instrument_data* instrument = NULL;
	uint8_t num_samples = 0;
	uint32_t tone_phase = 0;
	uint32_t tone_incr = 0;
	uint16_t tone_amp = 0;

	// state
	envelopeStateEnum  envelopeState = STATE_IDLE;  // idle, delay, attack, hold, decay, sustain, release
	uint32_t count = 0;  // how much time remains in this state, in 8 sample units
	float    mult = 0;   // attenuation, 0=off, 0x10000=unity gain
	float    inc = 0;    // amount to change mult on each sample
	// settings
};

