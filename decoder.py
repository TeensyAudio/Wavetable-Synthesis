# Python script for decoding a .SF2 file for use with Wavetable library.
from sf2utils.sf2parse import Sf2File
import struct
import sys
import logging
import sys, getopt
import time
import getopt
import inspect

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
    global BCOUNT, DCOUNT, DEBUG_FLAG
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

    options = ('Select by Instrument', 'Quit')
    options2 = ('Select Again', 'Save and Quit')
    while True:
        choice = menu(options)

        if choice == 1:
            # Returns a List of sf2Instrument.name
            instruments = map(lambda x: x.name, sf2.instruments)
            print''
            instruments.remove('EOI')
            print_menu(instruments)
            instrument = safe_input('Select Instrument [1-{}]: '.format(len(instruments)), int, 1, len(instruments))
            print''
			#for the selected instrument, go through all bags and
			#retrieve sample(s)
            instrument = instrument - 1
            bagToSample = [] #lists (bag index, sample) tuples
            bagIndex = 0
            globalBagIndex = None

			# Create a list of tuples that hold bagIndex to sample pairs
            for bag in sf2.instruments[instrument].bags:
                if bag.sample == None:
                    globalBagIndex = bagIndex
                elif bag.sample != None and bag.sample not in bagToSample:
                    bagToSample.append((bagIndex, bag.sample))
                bagIndex += 1
				
            samples = [x[1] for x in bagToSample]
            sampleNames = map(lambda x: x.name, samples)
            print '{} contains {} samples.'.format(sf2.instruments[instrument].name, len(sampleNames))
            method = menu(('Export All Samples', 'Select Samples to Export'))
            if method == 1:
                decodeAll(path, instrument, globalBagIndex)
                sys.exit('All samples for instrument decoded successfully. Exiting Program.')
            else:
                selectedBags = []
                while True:
                    print_menu(sampleNames)
                    sample = safe_input('Select Sample [1-{}]: '.format(len(sampleNames)), int, 1, len(sampleNames))
                    print_debug(DEBUG_FLAG, 'Selected Sample is {}'.format(samples[sample-1].name))
                    selectedBags.append(bagToSample[sample-1][0])
                    i_result = menu(options2)
                    if i_result == 1:
                        continue
                    elif i_result == 2:
                        decodeSelected(path, instrument, selectedBags, globalBagIndex) 
                        sys.exit('Program Terminated by User')
>>>>>>> 00032b54b636fc9e805fe31cc07a4055bc7c2d32
        elif choice == 2:
            sys.exit('Program Terminated by User')
        else:   #shouldn't be reached
            raw_input("Wrong option selection. Enter any key to try again..")

def decodeSelected(path, instIndex, selectedBags, globalBagIndex):
	global DCOUNT, BCOUNT, WCOUNT, BUF32
	with open(path, 'rb') as sf2_file:
		sf2 = Sf2File(sf2_file)
		for bag in selectedBags:
			aBag = sf2.instruments[instIndex].bags[bag]
			DCOUNT = DCOUNT + 1
			valid = is_sample_valid(aBag.sample)

			if valid[0] == False:
				error(valid[1])
				#return
			
			if(globalBagIndex != None):
				globalBag = sf2.instruments[instIndex].bags[globalBagIndex]
			else:
				#set globalBag to aBag to avoid needing to check for None when printing
				globalBag = aBag

			#Ignore extra 8 bits in the 24 bit specification
			aBag.sample.sm24_offset = None
			print_debug(DEBUG_FLAG, 'Selected Sample is {}'.format(aBag.sample.name))

			#If a sample has already been exported, set mode to
			#append rather than overwrite and reset write buffer variables
			mode = 'w'
			if DCOUNT > 1:
			  mode = 'a'
			  BCOUNT = 0
			  WCOUNT = 2
			  BUF32 = 0

			export_sample(aBag, globalBag, mode, len(selectedBags))

def decodeAll(path, instIndex, globalBagIndex):
	global DCOUNT, BCOUNT, WCOUNT, BUF32
	with open(path, 'rb') as sf2_file:
		sf2 = Sf2File(sf2_file)
		#check for global instrument bag
		if(globalBagIndex != None):
			bags = sf2.instruments[instIndex].bags[1:]
		else:
			bags = sf2.instruments[instIndex].bags
		for aBag in bags:
			DCOUNT = DCOUNT + 1
			valid = is_sample_valid(aBag.sample)

			if valid[0] == False:
				error(valid[1])
				#return
			
			if(globalBagIndex != None):
				globalBag = sf2.instruments[instIndex].bags[globalBagIndex]
			else:
				#set globalBag to aBag to avoid needing to check for None when printing
				globalBag = aBag

			#Ignore extra 8 bits in the 24 bit specification
			aBag.sample.sm24_offset = None

			#If a sample has already been exported, set mode to
			#append rather than overwrite and reset write buffer variables
			mode = 'w'
			if DCOUNT > 1:
			  mode = 'a'
			  BCOUNT = 0
			  WCOUNT = 2
			  BUF32 = 0

			export_sample(aBag, globalBag, mode, len(bags))


# Write a sample out to C++ style data files.
def export_sample(aBag, globalBag, mode, num_samples):	
    global DCOUNT
    with open("SF2_Decoded_Samples.cpp", mode) as cpp_file:
        with open("SF2_Decoded_Samples.h", mode) as header_file:
            if(mode == 'w'):
				cpp_file.write("#include \"SF2_Decoded_Samples.h\"\n")
				cpp_file.write("const unsigned int * samples[" + str(num_samples) + "] = {\n")
				header_file.write("extern const unsigned int * samples[" + str(num_samples) + "];\n")
				bag_num = 1
				while bag_num < num_samples+1:
					cpp_file.write("\tsample_" + str(bag_num) + ",\n")
					bag_num = bag_num + 1					
				cpp_file.write("};\n")
            else:
                cpp_file.write("\n")

            raw_wav_data = aBag.sample.raw_sample_data

            B_COUNT = 0;
            length_16 = aBag.sample.duration
            length_8 = length_16 * 2
            length_32 = length_16/2
            padlength = padding(length_32, 128)

            array_length = length_32 + padlength + 11 # Adding 10 because we are adding 10 array entries for metadata

            #Write array init to header file.
            #header_file.write("extern const unsigned int sample_" + str(DCOUNT) + "[" + str(array_length) + "];\n")
            header_file.write("extern const unsigned int sample_" + str(DCOUNT) + "[" + str(array_length) + "];\n")
            print_metadata_to_header(header_file, aBag, globalBag)

            #Write array contents to .cpp
            #file.write("const unsigned int sample_" + str(DCOUNT) + "[" + str(array_length) + "] = {\n")
            cpp_file.write("const unsigned int sample_" + str(DCOUNT) + "[" + str(array_length) + "] = {\n")

            format = 0x81

            print_metadata_to_array(cpp_file, aBag, globalBag, length_16, format)
            i = 0
            while i < length_8:
                audio = cc_to_int16(raw_wav_data[i], raw_wav_data[i+1])
				# Use PCM Encoding
                print_bytes(cpp_file, audio)
                print_bytes(cpp_file, audio >> 8)
				#consuming 2 chars at a time, so add another increment
                i = i + 2

            while padlength > 0:
                    print_bytes(cpp_file, 0)
                    padlength = padlength - 1

            cpp_file.write("};\n")

#prints out the sample metadata into the first portion of the sample array
def print_metadata_to_array(file, aBag, globalBag, length_16, format):
    file.write("0x%0.8X," % (length_16 | (format << 24))) #length
    file.write("0x%0.8X," % (aBag.base_note if aBag.base_note else aBag.sample.original_pitch)) #original pitch
    file.write("0x%0.8X," % (aBag.sample.sample_rate)) #sample rate
    file.write("0x%0.8X," % (aBag.cooked_loop_start)) #loop start
    file.write("0x%0.8X," % (aBag.cooked_loop_end)) #loop end
	
    if aBag.key_range:
        print(aBag.key_range)
        file.write("0x%0.8X," % (aBag.key_range[1] | (aBag.key_range[0] << 16))) #key range
    else: 
	    file.write("0x00000000,")
	
    if aBag.velocity_range:
	    file.write("0x%0.8X," % (aBag.velocity_range[1] | (aBag.velocity_range[0] << 16))) #velocity range
    else: 
	    file.write("0x00000000,")

    #delay_env & hold_env
    delayTemp = volume_envelope_delay(aBag)
    holdTemp = aBag.volume_envelope_hold
    if(delayTemp == None):
        delayTemp = volume_envelope_delay(globalBag)
    if(holdTemp == None):
        holdTemp = globalBag.volume_envelope_hold
    file.write("0x%0.8X," % 
            (checkGenValue(delayTemp) << 16 | #delay_env
            checkGenValue(holdTemp))) #hold_env

    #attack_env
    if(aBag.volume_envelope_attack == None):
        file.write("0x%0.8X," % (checkGenValue(globalBag.volume_envelope_attack)))
    else:
        file.write("0x%0.8X," % (checkGenValue(aBag.volume_envelope_attack)))
    
    #decay_env
    if(aBag.volume_envelope_decay == None):
        file.write("0x%0.8X," % (checkGenValue(globalBag.volume_envelope_decay)))
    else:
        file.write("0x%0.8X," % (checkGenValue(aBag.volume_envelope_decay)))
    file.write("\n") # 8 entries so far new line

    #sustain_env
    if(aBag.volume_envelope_sustain == None):
        file.write("0x%0.8X," % (checkGenValue(globalBag.volume_envelope_sustain))) 
    else:
        file.write("0x%0.8X," % (checkGenValue(aBag.volume_envelope_sustain))) 
    
    #release_env
    if(aBag.volume_envelope_release == None):
        file.write("0x%0.8X," % (checkGenValue(globalBag.volume_envelope_release))) 
    else:
        file.write("0x%0.8X," % (checkGenValue(aBag.volume_envelope_release))) 

# pretty prints metadata for the .h file
def print_metadata_to_header(header_file, aBag, globalBag):
    header_file.write("struct sample_info_" + str(DCOUNT) + " {\n")
    header_file.write("\tconst int ORIGINAL_PITCH = " + str(aBag.base_note if aBag.base_note else aBag.sample.original_pitch) + ";\n")
    header_file.write("\tconst int SAMPLE_RATE = " + str(aBag.sample.sample_rate) + ";\n")
    header_file.write("\tconst int LOOP_START = " + str(aBag.cooked_loop_start) + ";\n")
    header_file.write("\tconst int LOOP_END = " + str(aBag.cooked_loop_end) + ";\n")

    #delay_env
    header_file.write("\tconst float DELAY_ENV = ") 
    if(volume_envelope_delay(aBag) == None):
        header_file.write(str(
            0.0 if volume_envelope_delay(globalBag) == None 
            else volume_envelope_delay(globalBag)) + ";\n")
    else:
        header_file.write(str(volume_envelope_delay(aBag)) + ";\n")

    #attack_env
    header_file.write("\tconst float ATTACK_ENV = ") 
    if(aBag.volume_envelope_attack == None):
        header_file.write(str(
            0.0 if globalBag.volume_envelope_attack == None 
            else globalBag.volume_envelope_attack) + ";\n")
    else:
        header_file.write(str(aBag.volume_envelope_attack) + ";\n")
    
    #hold_env
    header_file.write("\tconst float HOLD_ENV = ")
    if(aBag.volume_envelope_hold == None):
        header_file.write(str(
            0.0 if globalBag.volume_envelope_hold == None 
            else globalBag.volume_envelope_hold) + ";\n")
    else:
        header_file.write(str(aBag.volume_envelope_hold) + ";\n")
    
    #decay_env
    header_file.write("\tconst float DECAY_ENV = ")
    if(aBag.volume_envelope_decay == None):
        header_file.write(str(
            0.0 if globalBag.volume_envelope_decay == None 
            else globalBag.volume_envelope_decay) + ";\n")
    else:
        header_file.write(str(aBag.volume_envelope_decay) + ";\n")

    #sustain_env
    header_file.write("\tconst float SUSTAIN_ENV = ")
    if(aBag.volume_envelope_sustain == None):
        header_file.write(str(
            0.0 if globalBag.volume_envelope_sustain == None 
            else globalBag.volume_envelope_sustain) + ";\n")
    else:
        header_file.write(str(aBag.volume_envelope_sustain) + ";\n")
       
    #release_env
    header_file.write("\tconst float RELEASE_ENV = ")
    if(aBag.volume_envelope_release == None):
        header_file.write(str(
            0.0 if globalBag.volume_envelope_release == None 
            else globalBag.volume_envelope_release) + ";\n")
    else:
        header_file.write(str(aBag.volume_envelope_release) + ";\n")

    header_file.write("};\n")

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
