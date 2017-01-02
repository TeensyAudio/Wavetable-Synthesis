# Python script for decoding a .SF2 file for use with Wavetable library.
from sf2utils.sf2parse import Sf2File
import struct

bcount = 0

def main():
	#Test code: Opens a specified .sf2 and prints out some info using sf2utils
	with open('piano.sf2', 'rb') as sf2_file:
		sf2 = Sf2File(sf2_file)
		sf2.samples[0].sm24_offset = None
		#Can also export a selected sample array to a .wav file
		#sf2.instruments[20].bags[4].sample.export('piano.wav')
		raw_wav_data = sf2.instruments[20].bags[4].sample.raw_sample_data
		print(raw_wav_data[0:2])
		print(cc_to_int16(raw_wav_data[0:2]))
		
#Copying functionality from wav2sketch.c
def print_byte(file, b):
	buf32 = buf32 | (b << (8 * bcount))
	bcount = bcount + 1
	file.write(ulaw_encode(buf32))

#Copying functionality from wav2sketch.c
def cc_to_int16(cc):
	c1 = float(cc[0])
	c2 = float(cc[1])
	c1 = c1 & 255
	c2 = c2 & 255
	return (c2 << 8) | c1
	
#Copying functionality from wav2sketch.c
def ulaw_encode(audio):
	mag = 0
	neg = 0

	if audio >= 0:
		mag = audio
		neg = 0
	else:
		mag = audio * -1
		neg = 0x80
		
	mag = mag + 128
	if mag > 0x7FFF: mag = 0x7FFF
	if mag >= 0x4000: return neg | 0x70 | ((mag >> 10) & 0x0F)  # 01wx yz00 0000 0000
	if mag >= 0x2000: return neg | 0x60 | ((mag >> 9) & 0x0F)   # 001w xyz0 0000 0000
	if mag >= 0x1000: return neg | 0x50 | ((mag >> 8) & 0x0F)   # 0001 wxyz 0000 0000
	if mag >= 0x0800: return neg | 0x40 | ((mag >> 7) & 0x0F)   # 0000 1wxy z000 0000
	if mag >= 0x0400: return neg | 0x30 | ((mag >> 6) & 0x0F)   # 0000 01wx yz00 0000
	if mag >= 0x0200: return neg | 0x20 | ((mag >> 5) & 0x0F)   # 0000 001w xyz0 0000
	if mag >= 0x0100: return neg | 0x10 | ((mag >> 4) & 0x0F)   # 0000 0001 wxyz 0000
	return neg | 0x00 | ((mag >> 3) & 0x0F)   # 0000 0000 1wxy z000

if __name__ == "__main__":
    main()