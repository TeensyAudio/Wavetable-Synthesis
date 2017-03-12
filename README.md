# Wavetable-Synthesis
General repository for Wavetable Synthesis Capstone project at Portland State, Fall 2016 - Winter 2017

The purpose of this project is to provide a C++ library and accompanying Python utility scripts allowing realistic instrumentation audio to be synthesized on the Teensy 3.2/3.6 Arduino Digital Analog Converter (DAC). 

The project consists of two main components:

1. SoundFont Decoder
2. Audio Synth Wavetable class

The project documentation can be found at: https://teensyaudio.github.io/Wavetable-Synthesis/html/index.html.

The project's source is available on GitHub: https://github.com/TeensyAudio/Wavetable-Synthesis

## SoundFont Decoder

The SoundFont decoder is a python utility to decode a SoundFont file into native C++ datatypes. The decoder can be run via a GUI or via the command line. 

### How To Run
To run the pre-built version of the SoundfontDecoder, simply execute the SoundfontDecoder.exe executable

### To Run Soundfont Decoder Via Python
To run the GUI via Python, you must use Python3, specifically 3.6. The Sf2Utils Python package must be installed in order to run/build the decoder.
Sf2Utils can be installed via `pip install sf2utils`

After dependancies have been installed, invoke with `$ python3 controller.py`

### Command Line Python Execution
How to invoke the script:  
The `-d` flag is for debug mode  
The `-i` flag precedes the input file  
`$ python decoder.py -i soundfont`  
`$ python decoder.py -d -i soundfont`  

### How To Build SoundfontDecoder.exe
1. Pull the latest version of PyInstaller from https://github.com/pyinstaller/pyinstaller (at the time of this writing, PyInstaller in PIP doesn't support Python 3.6).
2. Run the setup.py script from the PyInstaller repo.
3. After PyInstaller is installed on the system, the pyinstaller.exe will be within the \Scripts folder of the system's Python directory. Navigate to this directory, and run the following command:
`pyinstaller --onefile --noconsole --name SoundfontDecoder <path-to-wavetable-code>\controller.py`
4. This will produce an executable file named controller.exe which contains the Soundfont Decoder.

## Audio Synth Wavetable

C++ class implementing wavetable synthesis. For in-depth class member and field descriptions, see https://teensyaudio.github.io/Wavetable-Synthesis/html/index.html

