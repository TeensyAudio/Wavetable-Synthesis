# Python script for decoding a .SF2 file for use with Wavetable library.
from sf2utils.sf2parse import Sf2File
import struct
import sys
import logging
import sys, getopt
import time
import getopt
import inspect

MAX_LENGTH = 52000

BCOUNT = 0
WCOUNT = 2
BUF32 = 0
DCOUNT = 0
DEBUG_FLAG = False

def print_debug(flag, message, function=None):
    if flag == True:
        debug = '-----DEBUG-----'
        if function == None:
            caller = inspect.stack()[1][3]
        else:
            caller = function
        print(debug)
        print('In function: {}'.format(caller))
        print(message)
        print(debug)

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

def main(argv):
    global BCOUNT
    global DCOUNT
    global DEBUG_FLAG
    #Disable warning logging to prevent sf2utils from logging any un-needed messages
    logging.disable(logging.WARNING)

    try:
        opts, args = getopt.getopt(argv,'di:o:',['ifile=','ofile='])
    except getopt.GetoptError:
        print 'INVALID ARGUMENTS'
        sys.exit(2)
    for opt, arg in opts:
        if opt == '-d':
            DEBUG_FLAG = True
        elif opt in ('-i', '--ifile'):
            path = arg
        elif opt in ('-o', '--ofile'):
            outFile = arg

    print 150*'\n'
    print('       WELCOME  ')
    #print out some info:
    #version, date, etc...

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
		bagToSample = [] #lists a bag's index with its sample as a pair
                bagIndex = 0

                # Create a list of tuples that hold bagIndex to sample pairs
                for bag in sf2.instruments[instrument].bags:
                    if bag.sample != None and bag.sample not in bagToSample:
                        bagToSample.append((bagIndex, bag.sample))
                    bagIndex += 1

                samples = [x[1] for x in bagToSample]
                sampleNames = map(lambda x: x.name, samples)
                print '{} contains {} samples.'.format(sf2.instruments[instrument].name, len(sampleNames))
                print_menu(sampleNames)
                sample = safe_input('Select Sample [1-{}]: '.format(len(sampleNames)), int, 1, len(sampleNames))
                print_debug(DEBUG_FLAG, 'Selected Sample is {}'.format(samples[sample-1].name))
                DCOUNT=DCOUNT+1
                bagIndex = bagToSample[sample-1][0]
                decodeIt(path, instrument, bagIndex, DCOUNT) 
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
                DCOUNT=DCOUNT+1
                decodeIt(path, sample-1, DCOUNT)
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
                    sys.exit('Program Terminated by User')

        elif choice == 3:
            sys.exit('Program Terminated by User')
        else:   #shouldn't be reached
            raw_input("Wrong option selection. Enter any key to try again..")

def decodeIt(path, instIndex, bagIndex, DCOUNT):
    with open(path, 'rb') as sf2_file:
        sf2 = Sf2File(sf2_file)
        aBag = sf2.instruments[instIndex].bags[bagIndex]
        sample = aBag.sample
        valid = is_sample_valid(sample)

        if valid[0] == False:
            error(valid[1])
            #return

        #Ignore extra 8 bits in the 24 bit specification
        sample.sm24_offset = None
        print_debug(DEBUG_FLAG, 'Selected Sample is {}'.format(sample.name))

        #If a sample has already been exported, set mode to
        #append rather than overwrite
        mode = 'w'
        if DCOUNT > 1:
          mode = 'a'

        print sample.name

        with open("SF2_Decoded_Samples.cpp", mode) as output_file:
            with open("SF2_Decoded_Samples.h", mode) as header_file:
                export_sample(output_file, header_file, sample, aBag, True)


# Write a sample out to C++ style data files. PCM is a bool which when True 
# encodes in PCM. Otherwise, encode in ulaw.
def export_sample(file, header_file, sample, aBag, PCM):
	file.write("#include \"SF2_Decoded_Samples.h\"\n")
	raw_wav_data = sample.raw_sample_data

	B_COUNT = 0;
	length_16 = sample.duration
	length_8 = length_16 * 2
	length_32 = length_16/2
	padlength = padding(length_32, 128)

	array_length = length_32 + padlength

	if array_length > MAX_LENGTH:
		length_32 = MAX_LENGTH - padlength
		array_length = length_32
		length_16 = array_length * 2
		length_8 = length_16 * 2
		sample.end = length_16
		end_loop = length_16

	#Write array init to header file.
	header_file.write("extern const unsigned int sample[" + str(array_length) + "];\n")

	#Write array contents to .cpp
	file.write("const unsigned int sample[" + str(array_length) + "] = {\n")

	if PCM == True:
		format = 0x81
	else:
		format = 0x01

        print_metadata(file, sample, aBag, length_16, format)

	i = 0
	while i < length_8:
		audio = cc_to_int16(raw_wav_data[i], raw_wav_data[i+1])
		if PCM == True:
			# Use PCM Encoding
			print_bytes(file, audio)
			print_bytes(file, audio >> 8)
			#consuming 2 chars at a time, so add another increment
			i = i + 2
		else:
			pass
			# Using ulaw encoding
			#TODO

	while padlength > 0:
		print_bytes(file, 0)
		padlength = padlength - 1

	file.write("};\n")

	#Write sample to header file
	header_file.write("struct sample_info {\n")
	header_file.write("\tconst int ORIGINAL_PITCH = " + str(sample.original_pitch) + ";\n")
	header_file.write("\tconst int SAMPLE_RATE = " + str(sample.sample_rate) + ";\n")
	header_file.write("\tconst int LOOP_START = " + str(aBag.cooked_loop_start) + ";\n")
	header_file.write("\tconst int LOOP_END = " + str(aBag.cooked_loop_end) + ";\n")
	header_file.write("\tconst int DELAY_ENV = " + str(volume_envelope_delay(aBag)) + ";\n")
	header_file.write("\tconst int ATTACK_ENV = " + str(aBag.volume_envelope_attack) + ";\n")
	header_file.write("\tconst int HOLD_ENV = " + str(aBag.volume_envelope_hold) + ";\n")
	header_file.write("\tconst int DECAY_ENV = " + str(aBag.volume_envelope_decay) + ";\n")
	header_file.write("\tconst int SUSTAIN_ENV = " + str(aBag.volume_envelope_sustain) + ";\n")
	header_file.write("\tconst int RELEASE_ENV = " + str(aBag.volume_envelope_release) + ";\n")
	header_file.write("};\n")

#prints out the sample metadata into the first portion of the sample array
def print_metadata(file, sample, aBag, length_16, format):
    file.write("0x%0.8X," % (length_16 | (format << 24))) #length
    file.write("0x%0.8X," % (sample.original_pitch)) #original pitch
    file.write("0x%0.8X," % (sample.sample_rate)) #sample rate
    file.write("0x%0.8X," % (aBag.cooked_loop_start)) #loop start
    file.write("0x%0.8X," % (aBag.cooked_loop_end)) #loop end
    file.write("0x%0.8X," % 
            ((checkGenValue(volume_envelope_delay(aBag))) << 16 | #delay_env
            (checkGenValue(aBag.volume_envelope_hold)))) #hold_env
    file.write("0x%0.8X," % (checkGenValue(aBag.volume_envelope_attack))) #attack_env
    file.write("0x%0.8X," % (checkGenValue(aBag.volume_envelope_decay))) #decay_env
    file.write("\n") # 8 entries so far new line
    file.write("0x%0.8X," % (checkGenValue(aBag.volume_envelope_sustain))) #sustain_env
    file.write("0x%0.8X," % (checkGenValue(aBag.volume_envelope_release))) #release_env

# A function that returns the value of a generator (a float value) multiplied 
# by 1000. This is done to save the decimal of the float to the thousandths 
# place before being coverted to an int.
# If the generator was set to None 0 is returned instead.
def checkGenValue(genValue):
    if(genValue == None):
        return 0
    return int(genValue*1000)

# This function retrives the volume_envelope_delay generator value.
# There is no bag property in Sf2Utils to get the volume_envelope_delay value
# so it's necessary to grab that generator from the bag manually. 
# Each bag has a dictionary that maps a specified # from the SF2 manual to 
# each of its generators. The # for the delay envelope generator is 33.
def volume_envelope_delay(aBag):
    try:
        aGen = aBag.gens[33]
        return aGen.cents
    except KeyError:
        return None

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
    main(sys.argv[1:])
