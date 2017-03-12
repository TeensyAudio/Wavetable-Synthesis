# Getting started

This section is currently under construction

This is a short guide on how to get started using the Audio Synth Wavetable library. 

## What is the Audio Synth Wavetable library?

The Audio Synth Wavetable is a C++ library uses sound data from a decoded SoundFont file and creates sound using wavetable synthesis. This library is for [Teensy 3.2/3.6](https://www.pjrc.com/teensy/).

## How do I use the Audio Synth Wavetable library?

To use the Audio Synth Wavetable library you must include the header file in your Arduino sketch (a `.ino` file): 

~~~~~~~~~~~~~~~~~~~~~
#include <AudioSynthWavetable.h>
~~~~~~~~~~~~~~~~~~~~~

The `AudioSynthWavetable.h` and `AudioSynthWavetable.cpp` files need to be in your Arduino library folder.

## Dependencies

To use the Audio Synth Wavetable library you must have decoded SoundFont files. The library retrieves its sound data from these files. The decoded SoundFont files must be in the same directory as your Arduino sketch. 

To learn how to decode a SoundFont file read [SoundFont Decoder User-Guide](https://teensyaudio.github.io/Wavetable-Synthesis/html/md_additional_pages_soundfont_decoder.html).

## Using decoded SoundFont file and the library
The name of the data structures containing the sound data are in the decoded SoundFont's `.h` file. 

For example, in this example `.h`

~~~~~~~~~~~~~~~{.c} 
#include <AudioStream.h>
#include <AudioSynthWavetable.h>

extern const sample_data nylonstrgtr_samples[3];
const uint8_t nylonstrgtr_ranges[] = {56, 77, 127, };
const instrument_data nylonstrgtr = {3, nylonstrgtr_ranges, nylonstrgtr_samples };
extern const uint32_t sample_0_nylonstrgtr_nguitrf2[1920];
extern const uint32_t sample_1_nylonstrgtr_nguitb2[2688];
extern const uint32_t sample_2_nylonstrgtr_acgtrb3[3200];
~~~~~~~~~~~~~~~

# A very simple first program

~~~~~~~~~~~~~~~~~~~~~
#include <AudioSynthWavetable.h>
~~~~~~~~~~~~~~~~~~~~~







