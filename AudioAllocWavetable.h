//
//  AudioAllocWavetable.h
//  
//
//  Created by Connor Delaplane on 1/19/17.
//
//

#ifndef AudioAllocWavetable_h
#define AudioAllocWavetable_h

#include "AudioSynthWavetable.h"

class AudioAllocWavetable
{
public:
	/**
	 * Class constructor.
	 */
	AudioAllocWavetable(void)
		: voices(NULL)
		, num_voices(0)
		, amplitude(1.0)
	{}
	/**
	 * @brief Initialize the allocator.
	 * Each voice is initialized to the same sample data set and amplitude.
	 * The allocator is meant to be used as an interface to an array of "homogenic" voices.
	 * Therefore, when using allocator, voice settings should not be altered on an individual basis.
	 * 
	 * @param voices an array of AudioSynthWavetable objects to manage.
	 * @param num_voices the length of 'voices'.
	 * @param samples an array of samples from a decoded SoundFont file.
	 * @param num_samples the length of 'samples'.
	 * @param amp the amplitude for each voice (between 0.0 and 1.0).
	 */
	void init(AudioSynthWavetable* voices, uint8_t num_voices, sample_data* wavetable, int num_samples, float amp);
	
	/**
	 * @brief Set the sample data for each voice.
	 *
	 * @param samples an array of samples from a decoded SoundFont file.
	 * @param num_samples the length of 'samples'.
	 */
	void setSamples(sample_data* samples, int num_samples);
	
	/**
	 * @brief Set the amplitude for each voice.
	 *
	 * @param amp A value between 0.0 and 1.0.
	 */
	void setAmplitude(float amp);
	
	/**
	 * @brief Find an available voice and instruct it to play at the specified frequency.
	 *
	 * If there are no available voices or this frequency is already being
	 * played by another voice, nothing happens.
	 *
	 * @param freq the desired frequency for playback (between 0 and the board-specific sample rate).
	 */
	void playFreq(float freq);
	
	/**
	 * @brief Find an available voice and instruct it to play the specified note.
	 *
	 * If there are no available voices or this note is already being
	 * played by another voice, nothing happens.
	 *
	 * @param note the desired note to be played (between 0 and 127).
	 */
	void playNote(int note);
	
	/**
	 * @brief If a voice is playing at the specified frequency, stop it.
	 * If no such voice exists, do nothing.
	 *
	 * @param freq the desired frequency to stop (between 0 and the board-specific sample rate).
	 */
	void stopFreq(float freq);
	
	/**
	 * @brief If a voice is playing the specified note, stop it.
	 * If no such voice exists, do nothing.
	 *
	 * @param note the desired note to stop (between 0 and 127).
	 */
	void stopNote(int note);
	
	/**
	 * @return the number of voices currently playing (not idle).
	 */
	uint8_t voicesPlaying(void);
	
	/**
	 * @return the current amplitude settings.
	 */
	float getAmplitude() { return amplitude; }
	
	/**
	 * @param index the index of a voice (between 0 and the number of voices allocated minus 1).
	 * @return the frequency that voice is playing (or -1 if voice is idle).
	 */
	float getFrequency(uint8_t index) { return freqTrack[index].freq; }
    
private:
	struct freqTrack_t
	{
		float freq;
		unsigned int count;
	};
	
	float noteToFreq(int note);
	
	AudioSynthWavetable* voices;
	uint8_t num_voices;
	freqTrack_t freqTrack[256];
	float amplitude;
};
#endif
