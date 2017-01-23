# Python script for decoding a .SF2 file for use with Wavetable library.
from sf2utils.sf2parse import Sf2File
import struct
import sys
import logging
import sys, getopt
import time

BCOUNT = 0
WCOUNT = 1
BUF32 = 0

def print_menu(menu_items):
    if len(menu_items) < 1:
        return
    for i, j in enumerate(menu_items, 1):
        print('{}. {}'.format(i, j))
    print ''

def safe_input(prompt, type_=None, min_=None, max_=None, range_=None):
    if min_ is not None and max_ is not None and max_ < min_:
        raise ValueError('min_ must be less than or equal to max_.')
    while True:
        ui = raw_input(prompt)
        if type_ is not None:
            try:
                ui = type_(ui)
            except ValueError:
                print('Input type must be {0}.'.format(type_.__name__))
                continue
        if max_ is not None and ui > max_:
            print('Input must be less than or equal to {0}.'.
                    format(max_))
        elif min_ is not None and ui < min_:
            print('Input must be greater than or equal to {0}.'.format(min_))
        elif range_ is not None and ui not in range_:
            if isinstance(range_, range):
                template = 'Input must be between {0.start} and {0.stop}.'
                print(template.format(range_))
            else:
                template = 'Input must be {0}.'
                if len(range_) == 1:
                    print(template.format(*range_))
                else:
                    print(template.format(' or '.join((', '.join(map(str, range_[:-1])), str(range_[-1])))))
        else:
            return ui

def menu(choices):
    print''
    print_menu(choices)
    choice = safe_input('Select [1-{}]: '.format(len(choices)), int,
            1, len(choices))
    return choice

#['_','|','-','|']
#['|','/','--','\\']
def animate(num=5, time_=0.1, pad=' ', smbl=' '):
    for i in range(num):
        sys.stdout.flush()
        sys.stdout.write("\r{}{}{}{}{}{}".format(pad*i,
            smbl[i%len(smbl)], smbl[(i+1)%len(smbl)],
            smbl[(i+2)%len(smbl)], smbl[(i+3)%len(smbl)],
            smbl[(i+4)%len(smbl)]))
        sys.stdout.flush()
        time.sleep(time_)
    sys.stdout.write('\r{}{}'.format(pad*(num+1), ' '))
    sys.stdout.flush()
    print''

def main():
    global BCOUNT
    global DCOUNT
    DCOUNT = 0
    #Disable warning logging to prevent sf2utils from logging any un-needed messages
    logging.disable(logging.WARNING)
    print 150*'\n'
    print('       WELCOME  ')
    #print out some info:
    #version, date, etc...

    if len(sys.argv) < 2:
        sys.exit('ERROR: INVALID ARGUMENT LIST')
    else:
        path = sys.argv[1]

    with open (path, 'rb') as sf2_file:
        sf2 = Sf2File(sf2_file)

    options = ('Select by Instrument', 'Select by Sample', 'Quit')
    options2 = ('Select Again', 'Repeat List', 'Back', 'Quit')
    while True:
        choice = menu(options)

        if choice == 1:
                        # Returns a List of sf2Instrument.name
            instruments = map(lambda x: x.name, sf2.instruments)
            print''
            instruments.remove('EOI')
            print_menu(instruments)

            while True:
                instrument = safe_input('Select Instrument [1-{}]: '.format(len(instruments)), int, 1, len(instruments))
                print''
                #for the selected instrument, go through all bags and
                #retrieve sample(s)
                instrument = instrument - 1
		samples = []
		#notes = []

		for bag in sf2.instruments[instrument].bags:
			if bag.sample != None and bag.sample not in samples:
				samples.append(bag.sample)
				#notes.append(bag.sample.original_pitch)

                samples = map(lambda x: x.name, samples)
                print '{} contains {} samples.'.format(sf2.instruments[instrument].name, len(samples))
                print_menu(samples)
                sample = safe_input('Select Sample [1-{}]: '.format(len(samples)), int, 1, len(samples))
                animate(num=15, time_=0.17, smbl=' Decoding Sample  ')
                decodeIt(path, sample-1, DCOUNT=DCOUNT+1)
                i_result = menu(options2)
                if i_result == 1:
                    continue
                elif i_result == 2:
                    print''
                    print_menu(instruments)
                    continue
                elif i_result == 3:
                    break
                elif i_result == 4:
                    animate(num=15, time_=0.16, smbl=' Exiting ')
                    sys.exit('Program Terminated by User')
        elif choice == 2:
                    # Returns a List of sf2Sample.name
            samples = map(lambda x: x.name, sf2.samples)
            print''
            samples.remove('EOS')
            print_menu(samples)

            while True:
                sample = safe_input('Select Sample [1-{}]: '.format(
                    len(samples)), int, 1, len(samples))
                animate(num=15, time_=0.17, smbl=' Decoding Sample  ')
                decodeIt(path, sample-1, DCOUNT=DCOUNT+1)
                s_result = menu(options2)
                if s_result == 1:
                    continue
                elif s_result == 2:
                    print''
                    print_menu(samples)
                    continue
                elif s_result == 3:
                    break
                elif s_result == 4:
                    animate(num=15, time_=0.16, smbl=' Exiting ')
                    sys.exit('Program Terminated by User')

        elif choice == 3:
            animate(num=15, time_=0.16, smbl=' Exiting ')
            sys.exit('Program Terminated by User')
        else:   #shouldn't be reached
            raw_input("Wrong option selection. Enter any key to try again..")

	#if len(sys.argv) < 2:
		#path = raw_input("Please provide filepath to an SF2: ")
	#else:
		#path = sys.argv[1]

	#with open(path, 'rb') as sf2_file:
		#sf2 = Sf2File(sf2_file)
		#for i, j in enumerate(sf2.instruments, 1):
			#print("{}. {}".format(i, j.name))

		#print('Select an instrument:')
		#selection = int(input('Input the corresponding number: '))
		#selection = selection - 1

		#samples = []
		#notes = []

		#print('Select a note:')
		#for bag in sf2.instruments[selection].bags:
			#if bag.sample != None and bag.sample.original_pitch not in notes:
				#samples.append(bag.sample)
				#notes.append(bag.sample.original_pitch)

		#for i, j in enumerate(samples, 1):
			#print("{}. {}".format(i, j.original_pitch))

		#sample_selection = int(input('Input the corresponding number: '))
		#sample_selection = sample_selection - 1

def decodeIt(path, sample_selection, DCOUNT):
    with open(path, 'rb') as sf2_file:
        sf2 = Sf2File(sf2_file)

        sample = sf2.samples[sample_selection]
        valid = is_sample_valid(sample)

        if valid[0] == False:
            error(valid[1])
            #return

        #Ignore extra 8 bits in the 24 bit specification
        sample.sm24_offset = None

        #If a sample has already been exported, set mode to
        #append rather than overwrite
        mode = 'w'
        if DCOUNT > 1:
            mode = 'a'

        with open("SF2_Decoded_Samples.cpp", mode) as output_file:
            with open("SF2_Decoded_Samples.h", mode) as header_file:
                    export_sample(output_file, header_file, sample, True)


#Write a sample out to C++ style data files. PCM is a bool which when True encodes in PCM. Otherwise, encode in ulaw.
def export_sample(file, header_file, sample, PCM):

	nameSplit = sample.name.split('-') #removing anything starting with a dash
	name = nameSplit[0];

	#replace spaces with underscores
	name.replace(" ", "_")

	file.write("#include \"SF2_Decoded_Samples.h\"\n")
	raw_wav_data = sample.raw_sample_data
	start_loop = sample.start_loop
	end_loop = sample.end_loop
	duration = sample.duration

	B_COUNT = 0;
	length = (sample.end - sample.start)/2
	padlength = padding(length, 128)

	#generating all three sections as one array
	file.write("const unsigned int " + name + "_sample[" + str(length + padlength) + "] = {\n")

	#print out attack
	if PCM == True:
		format = 0x81
	else:
		format = 0x01

	i = 0
	file.write("0x%0.8X," % (length | (format << 24)))
	while i < length:
		audio = cc_to_int16(raw_wav_data[i], raw_wav_data[i+1])
		if PCM == True:
			# Use PCM Encoding
			print_bytes(file, audio)
			print_bytes(file, audio >> 8)
			#consuming 2 chars at a time, so add another increment
			i = i + 2
		else:
			# Using ulaw encoding
			print_bytes(file, ulaw_encode(audio))

	while padlength > 0:
		print_bytes(file, 0)
		padlength = padlength - 1

	file.write("};\n")


	#Write sample to header file
	header_file.write("#include <string>\n\n\n")

	header_file.write("extern const unsigned int " + name + "_sample[" + str(length + padlength) + "];\n")

	header_file.write("struct " + name + "_info {\n")
	header_file.write("\tconst std::string SAMPLE_INFO = \"" + str(sample) + "\";\n")
	header_file.write("\tconst std::string SAMPLE_NAME = \"" + str(sample.name) + "\";\n")
	header_file.write("\tconst int ORIGINAL_PITCH = " + str(sample.original_pitch) + ";\n")
	header_file.write("\tconst int SAMPLE_RATE = " + str(sample.sample_rate) + ";\n")
	header_file.write("\tconst int SAMPLE_NAME = " + str(sample.sample_type) + ";\n")
	header_file.write("\tconst bool IS_MONO = " + str(sample.is_mono) + ";\n")
	header_file.write("\tconst int LOOP_START = " + str(start_loop) + ";\n")
	header_file.write("\tconst int LOOP_END = " + str(end_loop) + ";\n")
	header_file.write("};\n")


#Checks if the selected sample is valid. Input is a sample object, and output is
#a tuple with (boolean, error_message - if any)
def is_sample_valid(sample):
	if sample.loop_duration >= sample.duration: return (False, 'Loop length >= sample length')
	if sample.end_loop > sample.duration: return (False, 'End loop index is larger than sample end index')
	return (True, None)

def error(message):
	print("ERROR: " + message)

#Copying functionality from wav2sketch.c
def print_bytes(file, b):
	global BCOUNT, WCOUNT, BUF32

	BUF32 = BUF32 | (b << (8 * BCOUNT))
	BCOUNT = BCOUNT + 1
	if BCOUNT >= 4:
		file.write("0x%0.8X" % BUF32)
		file.write(",")
		BUF32 = 0
		BCOUNT = 0
		WCOUNT = WCOUNT + 1
		if WCOUNT >= 8:
			file.write("\n")
			WCOUNT = 0

def padding(length, block):
	extra = length % block
	if extra == 0: return 0
	return block - extra

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
