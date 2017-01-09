# Python script for decoding a .SF2 file for use with Wavetable library.
from sf2utils.sf2parse import Sf2File
import struct

BCOUNT = 0
WCOUNT = 1
BUF32 = 0

def main():
	#Test code: Opens a specified .sf2 and prints out some info using sf2utils
	with open('piano.sf2', 'rb') as sf2_file:
		instr = 20
		bag = 4
		sf2 = Sf2File(sf2_file)
		
		#Ignore extra 8 bits in the 24 bit specification
		sf2.instruments[instr].bags[bag].sample.sm24_offset = None
		
		#Can also export a selected sample array to a .wav file
		#sf2.instruments[instr].bags[bag].sample.export('piano.wav')
		raw_wav_data = sf2.instruments[instr].bags[bag].sample.raw_sample_data
		
		#Printing raw_wav_data to a text file
		with open("raw_wav_data.txt", "w") as text_file:
			text_file.write(raw_wav_data)
		
		with open("piano.cpp", "w") as output_file:
			output_file.write("#include \"piano.h\"\n")
			start_loop = sf2.instruments[instr].bags[bag].sample.start_loop
			end_loop = sf2.instruments[instr].bags[bag].sample.end_loop
			duration = sf2.instruments[instr].bags[bag].sample.duration
			print(duration)
			print(end_loop)
			#print out attack
			i = 0
			output_file.write("const unsigned int AudioWaveform_Attack[" + str(start_loop/4) + "] = {\n")
			while i <= start_loop:
				audio = cc_to_int16(raw_wav_data[i], raw_wav_data[i+1])
				print_bytes(output_file, audio)
				#consuming 2 chars at a time, so add another increment
				i = i + 2
			output_file.write("};")
			
			#print out loop
			i = start_loop
			output_file.write("const unsigned int AudioWaveform_Loop[" + str((end_loop - start_loop)/4) + "] = {\n")
			while i < end_loop:
				audio = cc_to_int16(raw_wav_data[i], raw_wav_data[i+1])
				print_bytes(output_file, audio)
				#consuming 2 chars at a time, so add another increment
				i = i + 2
			output_file.write("};")
			
			#print out decay
			#i = end_loop
			#output_file.write("const unsigned int AudioWaveform_Decay[" + str((duration - end_loop)/2) + "] = {\n")
			#while i < duration-1:
			#	audio = cc_to_int16(raw_wav_data[i], raw_wav_data[i+1])
			#	print_bytes(output_file, audio)
			#	#consuming 2 chars at a time, so add another increment
			#	i = i + 2
			#output_file.write("};")
		
		with open("piano.h", "w") as header_file:
			header_file.write("extern const unsigned int AudioWaveform_Attack[" + str(start_loop/4) + "];\n")
			header_file.write("extern const unsigned int AudioWaveform_Loop[" + str((end_loop - start_loop)/4) + "];\n")
			#header_file.write("extern const unsigned int AudioWaveform_Decay[" + str((duration - end_loop)/4) + "];\n")
		
#Copying functionality from wav2sketch.c
def print_bytes(file, b):
	global BCOUNT, WCOUNT, BUF32
	
	BCOUNT = BCOUNT + 1
	BUF32 = BUF32 | (b << (8 * BCOUNT))
	if BCOUNT >= 2:
		file.write(str(BUF32))
		file.write(",")
		BUF32 = 0
		BCOUNT = 0
		WCOUNT = WCOUNT + 1
		if WCOUNT >= 8:
			file.write("\n")
			WCOUNT = 0
		
	

#Copying functionality from wav2sketch.c
def cc_to_int16(c1, c2):
	i1 = int(ord(c1))
	i2 = int(ord(c2))
	i1 = i1 & 255
	i2 = i2 & 255
	return (i2 << 8) | i1
	
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