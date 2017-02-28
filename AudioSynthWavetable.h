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
	/**
	 * Class constructor.
	 */
	AudioSynthWavetable(void) : AudioStream(0, NULL) {}

	/**
	 * @brief Set the wavetable samples.
	 *
	 * A wavetable uses a set of samples to generate sound.
	 * This function is used to set the samples.
	 * @param samples an array of samples from a decoded SoundFont file
	 * @param num_samples number of samples in the array
	 */
	void setSamples(const sample_data * samples, int num_samples) {
		this->samples = samples;
		this->num_samples = num_samples;
	}
	
	/**
	 * @brief Set the loop start to and end points.
	 *
	 * @param start index within the sample array
	 * representing the start of the loop
	 * @param end index within the sample array
	 * representing the end of the loop
	 */
	void setLoop(int start, int end) {
		loop_start = start;
		loop_end = end;
		loop_length = loop_end - loop_start;
		
		// TODO: remove, if dead code (which I think it is)
		length_bits = 1;
		for (int len = loop_length; len >>= 1; ++length_bits);
		loop_phase = (loop_length - 1) << (32 - length_bits);
	}
	
	/**
	 * @brief Set the frequency and amplitude of the
	 * Wavetable
	 *
	 * You can use this to set the frequency and amplitude
	 * before calling play().
	 * @param freq frequency of the generated output (range?)
	 * @param amp amplitude of generated output
	 */
	void setFreqAmp(float freq, float amp) {
		frequency(freq);
		amplitude(amp);
	}
	
	/**
	 * @brief Define the note of the sample
	 * @param note the midi note number a value between 0 and 127
	 */
	void setSampleNote(int note) {
		sample_freq = noteToFreq(note);
	}

	/**
	 * @brief Changes the amplitude to 'v'
	 *
	 * A value of 0 will set the synth output to minimum amplitude
	 * (i.e., no output). A value of 1 will set the output to the
	 * maximum amplitude. Amplitude is set linearly with intermediate
	 * values.
	 * @param v a value between 0.0 and 1.0
	 */
	void amplitude(float v) {
		v = (v < 0.0) ? 0.0 : (v > 1.0) ? 1.0 : v;
		tone_amp = (uint16_t)(32767.0*v);
	}

	/**
	 * @brief Scale midi_amp to a value between 0.0 and 1.0
	 * using a logarithmic tranformation.
	 *
	 * @param midi_amp a value between 0 and 127
	 * @return a value between 0.0 to 1.0
	 */
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
	
	/**
	 * @brief Convert a MIDI note value to
	 * its corresponding frequency.
	 *
	 * @param note a value between 0 and 127
	 * @return a frequency
	 */
	static float noteToFreq(int note) {
		//return 440.0 * pow(2.0, (note - 69) / 12.0);
		//float exp = (note + 36.37631656) / 12.0;
		float exp = note * 0.083333333 + 3.0313597;
		float freq = pow(2, exp);
		return freq;
	}

	/**
	 * @brief Set the delay time.
	 *
	 * @param milliseconds length of delay (in milliseconds)
	 */
	void env_delay(float milliseconds) {
		delay_count = milliseconds2count(milliseconds);
	}

	/**
	 * @brief Set the attack time.
	 *
	 * @param milliseconds length of the attack time
	 */
	void env_attack(float milliseconds) {
		attack_count = milliseconds2count(milliseconds);
	}

	/**
	 * @brief Set the hold time.
	 *
	 * @param milliseconds the length of the hold time
	 */
	void env_hold(float milliseconds) {
		hold_count = milliseconds2count(milliseconds);
	}

	/**
	 * Set the decay time.
	 *
	 * @param milliseconds the length of the decay time
	 */
	void env_decay(float milliseconds) {
		decay_count = milliseconds2count(milliseconds);
	}

	/**
	 * @brief Set the sustain level.
	 *
	 * @param level a value between 0.0 and 1.0
	 */
	void env_sustain(float level) {
		if (level < 0.0) level = 0;
		else if (level > 1.0) level = 1.0;
		sustain_mult = level * UNITY_GAIN;
	}

	/**
	 * @brief Set the release time.
	 *
	 * @param milliseconds the length of the release time
	 */
	void env_release(float milliseconds) {
		release_count = milliseconds2count(milliseconds);
	}
	
	// Defined in AudioSynthWavetable.cpp
	void play(void);
	void stop(void);
	void parseSample(int sample_num, bool custom_env);
	void playFrequency(float freq, bool custom_env=0);
	void playNote(int note, int amp=AMP_DEF, bool custom_env=0);

	/**
	 * @return true (non-zero) if playing, otherwise false (zero) if not playing.
	 */
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
		if (milliseconds < 1.0) milliseconds = 1.0;
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
	int32_t  sustain_mult = 0;
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

