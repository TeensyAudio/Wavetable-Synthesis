#pragma once

#include "Arduino.h"
#include "AudioStream.h"
#include <math.h>
#include <sample_data.h>
#include <stdint.h>

#define UNITY_GAIN INT32_MAX // Max amplitude
#define DEFAULT_AMPLITUDE 127
#define SAMPLES_PER_MSEC (AUDIO_SAMPLE_RATE_EXACT/1000.0)
#define TRIANGLE_INITIAL_PHASE (-0x40000000)

// int n in range 1..log2(AUDIO_BLOCK_SAMPLES/2)-1 (1..7 for AUDIO_BLOCK_SAMPLES == 128)
// where AUDIO_BLOCK_SAMPLES%n == 0, higher == more smooth and more CPU usage
#define LFO_SMOOTHNESS 3
#define LFO_PERIOD (AUDIO_BLOCK_SAMPLES/(1 << (LFO_SMOOTHNESS-1)))

#define ENVELOPE_PERIOD 8
#define ENVELOPE_ATTACK_UPDATE_RATE 2
#define MINIMUM_VOL_MULT UINT16_MAX

class AudioSynthWavetable : public AudioStream
{
public:
	AudioSynthWavetable(void) : AudioStream(0, NULL) {}

	void setInstrument(const instrument_data& instrument) {
		cli();
		this->instrument = &instrument;
		current_sample = NULL;
		env_state = STATE_IDLE;
		state_change = true;
		sei();
	}

	void amplitude(float v) {
		v = (v < 0.0) ? 0.0 : (v > 1.0) ? 1.0 : v;
		tone_amp = (uint16_t)(UINT16_MAX*v);
	}

	static float midi_volume_transform(int midi_amp) {
		// scale midi_amp which is 0 t0 127 to be between
		// 0 and 1 using a logarithmic transformation
		return powf(midi_amp / 127.0, 4);
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
	void playFrequency(float freq, int amp = DEFAULT_AMPLITUDE);
	void playNote(int note, int amp = DEFAULT_AMPLITUDE);
	bool isPlaying(void) { return env_state != STATE_IDLE; }
	virtual void update(void);

private:
	void setState(int note, int amp, float freq);
	void setFrequency(float freq);

	enum envelopeStateEnum { STATE_IDLE, STATE_PLAY, STATE_DELAY, STATE_ATTACK, STATE_HOLD, STATE_DECAY, STATE_SUSTAIN, STATE_RELEASE, STATE_STOP };

	volatile bool state_change = false;

	volatile const instrument_data* instrument = NULL;
	volatile const sample_data* current_sample = NULL;

	//sample output state
	volatile uint32_t tone_phase = 0;
	volatile uint32_t tone_incr = 0;
	volatile uint16_t tone_amp = 0;

	//volume environment state
	volatile envelopeStateEnum  env_state = STATE_IDLE;
	volatile int32_t env_count = 0;
	volatile int32_t env_mult = 0;
	volatile int32_t env_incr = 0;

	//vibrato LFO state
	volatile uint32_t vib_count = 0;
	volatile uint32_t vib_phase = 0;
	volatile int32_t vib_pitch_offset_init = 0;
	volatile int32_t vib_pitch_offset_scnd = 0;

	//modulation LFO state
	volatile uint32_t mod_count = 0;
	volatile uint32_t mod_phase = TRIANGLE_INITIAL_PHASE;
	volatile int32_t mod_pitch_offset_init = 0;
	volatile int32_t mod_pitch_offset_scnd = 0;
};

