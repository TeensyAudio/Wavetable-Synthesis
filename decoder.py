# Python script for decoding a .SF2 file for use with Wavetable library.
from sf2utils.sf2parse import Sf2File
import logging
import sys
import getopt
import inspect

BCOUNT = 0
WCOUNT = 2
BUF32 = 0
DCOUNT = 0
DEBUG_FLAG = False


def print_debug(flag, message, function=None):
    if flag:
        debug = '-----DEBUG-----'
        if function is None:
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
    print('')


def safe_input(prompt, type_=None, min_=None, max_=None, range_=None):
    if min_ is not None and max_ is not None and max_ < min_:
        raise ValueError('min_ must be less than or equal to max_.')
    while True:
        ui = input(prompt)
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
    print('')
    print_menu(choices)
    choice = safe_input('Select [1-{}]: '.format(len(choices)), int,
                        1, len(choices))
    return choice


def main(argv):
    global BCOUNT, DCOUNT, DEBUG_FLAG
    # Disable warning logging to prevent sf2utils from logging any un-needed messages
    logging.disable(logging.WARNING)

    path = None
    try:
        opts, args = getopt.getopt(argv, 'di:o:', ['ifile=', 'ofile='])
    except getopt.GetoptError:
        print('INVALID ARGUMENTS')
        sys.exit(2)
    for opt, arg in opts:
        if opt == '-d':
            DEBUG_FLAG = True
        elif opt in ('-i', '--ifile'):
            path = arg
        elif opt in ('-o', '--ofile'):
            outFile = arg

    print(150*'\n')
    print('       WELCOME  ')
    # print out some info:
    # version, date, etc...

    with open(path, 'rb') as sf2_file:
        sf2 = Sf2File(sf2_file)

    options = ('Select by Instrument', 'Quit')
    options2 = ('Select Again', 'Save and Quit')
    while True:
        choice = menu(options)

        if choice == 1:
            # Returns a List of sf2Instrument.name
            instruments = [x.name for x in sf2.instruments]
            print('')
            instruments.remove('EOI')
            print_menu(instruments)
            instrument = safe_input('Select Instrument [1-{}]: '.format(len(instruments)), int, 1, len(instruments))
            print('')
            # for the selected instrument, go through all bags and
            # retrieve sample(s)
            instrument -= 1
            bag_to_sample = []  # lists (bag index, sample) tuples
            bag_index = 0
            global_bag_index = None

            # Create a list of tuples that hold bag_index to sample pairs
            for bag in sf2.instruments[instrument].bags:
                if bag.sample is None:
                    global_bag_index = bag_index
                elif bag.sample is not None and bag.sample not in bag_to_sample:
                    bag_to_sample.append((bag_index, bag.sample))
                bag_index += 1

            samples = [x[1] for x in bag_to_sample]
            sample_names = [x.name for x in samples]
            print('{} contains {} samples.'.format(sf2.instruments[instrument].name, len(sample_names)))
            method = menu(('Export All Samples', 'Select Samples to Export'))
            if method == 1:
                decode_all(path, instrument, global_bag_index)
                sys.exit('All samples for instrument decoded successfully. Exiting Program.')
            else:
                selected_bags = []
                while True:
                    print_menu(sample_names)
                    sample = safe_input('Select Sample [1-{}]: '.format(len(sample_names)), int, 1, len(sample_names))
                    print_debug(DEBUG_FLAG, 'Selected Sample is {}'.format(samples[sample-1].name))
                    selected_bags.append(bag_to_sample[sample-1][0])
                    i_result = menu(options2)
                    if i_result == 1:
                        continue
                    elif i_result == 2:
                        decode_selected(path, instrument, selected_bags, global_bag_index)
                        sys.exit('Program Terminated by User')
        elif choice == 2:
            sys.exit('Program Terminated by User')
        else:   # shouldn't be reached
            input("Wrong option selection. Enter any key to try again..")


def decode_selected(path, inst_index, selected_bags, global_bag_index):
    global DCOUNT, BCOUNT, WCOUNT, BUF32
    with open(path, 'rb') as sf2_file:
        sf2 = Sf2File(sf2_file)
        bags_to_decode = []
        a_bag = None

        for bag in selected_bags:
            a_bag = sf2.instruments[inst_index].bags[bag]
            a_bag.sample.sm24_offset = None
            bags_to_decode.append(bag)
            DCOUNT += 1
            valid = is_sample_valid(a_bag.sample)

            if not valid[0]:
                error(valid[1])
                # return

        global_bag = None
        if global_bag_index is not None:
            global_bag = sf2.instruments[inst_index].bags[global_bag_index]

        print_debug(DEBUG_FLAG, 'Selected Sample is {}'.format(a_bag.sample.name))

        export_samples(bags_to_decode, global_bag, len(selected_bags))


def decode_all(path, inst_index, global_bag_index):
    global DCOUNT, BCOUNT, WCOUNT, BUF32
    with open(path, 'rb') as sf2_file:
        sf2 = Sf2File(sf2_file)
        # check for global instrument bag
        if global_bag_index is not None:
            bags = sf2.instruments[inst_index].bags[1:]
        else:
            bags = sf2.instruments[inst_index].bags
        for aBag in bags:
            DCOUNT += 1
            valid = is_sample_valid(aBag.sample)
            aBag.sample.sm24_offset = None

            if not valid[0]:
                error(valid[1])
            # return

        global_bag = None
        if global_bag_index is not None:
            global_bag = sf2.instruments[inst_index].bags[global_bag_index]

        export_samples(bags, global_bag, len(bags))


# Write a sample out to C++ style data files.
def export_samples(bags, global_bag, num_samples):
    global DCOUNT, BUF32, BCOUNT, WCOUNT
    global_bag_exists = True if global_bag is not None else False
    with open("SF2_Decoded_Samples.cpp", "w") as cpp_file:
        with open("SF2_Decoded_Samples.h", "w") as header_file:
            cpp_file.write("#include \"SF2_Decoded_Samples.h\"\n")
            header_file.write("#include \"AudioSynthWavetable.h\"\n")

            # Decode data to sample_data array in header file
            header_file.write("extern sample_data samples[" + str(num_samples) + "];\n")
            cpp_file.write("sample_data samples[" + str(num_samples) + "] = {\n")
            sample_num = 0
            for aBag in bags:
                if global_bag_exists is False:
                    global_bag = aBag

                print_metadata_to_header(cpp_file, aBag, global_bag, sample_num)
                sample_num += 1

            cpp_file.write("};\n")

            sample_num = 0
            for aBag in bags:
                raw_wav_data = aBag.sample.raw_sample_data

                BCOUNT = 0
                WCOUNT = 0
                BUF32 = 0

                length_16 = aBag.sample.duration
                length_8 = length_16 * 2
                length_32 = length_16/2
                padlength = padding(length_32, 128)

                array_length = length_32 + padlength

                # Write array init to header file.
                header_file.write("extern const unsigned int sample_" + str(sample_num) + "[" + str(int(array_length)) + "];\n")

                # Write array contents to .cpp
                cpp_file.write("const unsigned int sample_" + str(sample_num) + "[" + str(int(array_length)) + "] = {\n")

                i = 0
                while i < length_8:
                    audio = cc_to_int16(raw_wav_data[i], raw_wav_data[i+1])
                    # Use PCM Encoding
                    print_bytes(cpp_file, audio)
                    print_bytes(cpp_file, audio >> 8)
                    # consuming 2 chars at a time, so add another increment
                    i += 2

                while padlength > 0:
                    print_bytes(cpp_file, 0)
                    padlength -= 1

                cpp_file.write("};\n")
                sample_num += 1


# prints out the sample metadata into the first portion of the sample array
def print_metadata_to_header(file, a_bag, global_bag, sample_num):
    file.write("{\n")
    file.write(str(a_bag.base_note if a_bag.base_note else a_bag.sample.original_pitch) + ",\n")  # original pitch
    file.write(str(a_bag.sample.duration) + ",\n")  # length
    file.write(str(a_bag.sample.sample_rate) + ",\n")  # sample rate
    file.write(str(a_bag.cooked_loop_start) + ",\n")  # loop start
    file.write(str(a_bag.cooked_loop_end) + ",\n")  # loop end

    if a_bag.key_range:
        # write out key_range lower and upper bounds
        file.write(str(a_bag.key_range[0]) + ",\n")  # key range lower
        file.write(str(a_bag.key_range[1]) + ",\n")  # key range upper
    else:
        file.write("0,\n")
        file.write("0,\n")

    if a_bag.velocity_range:
        # write out velocity_range lower and upper bounds
        file.write(str(a_bag.velocity_range[0]) + ",\n")  # velocity_range lower
        file.write(str(a_bag.velocity_range[1]) + ",\n")  # velocity_range upper
    else:
        file.write("0,\n")
        file.write("0,\n")

    # delay_env & hold_env
    delay_temp = volume_envelope_delay(a_bag)
    hold_temp = a_bag.volume_envelope_hold
    if delay_temp is None:
        delay_temp = volume_envelope_delay(global_bag)
    if hold_temp is None:
        hold_temp = global_bag.volume_envelope_hold

    file.write(str(check_gen_value(delay_temp)) + ",\n")  # delay_env
    file.write(str(check_gen_value(hold_temp)) + ",\n")  # hold_env

    # attack_env
    if a_bag.volume_envelope_attack is None:
        file.write(str(check_gen_value(global_bag.volume_envelope_attack)) + ",\n")
    else:
        file.write(str(check_gen_value(a_bag.volume_envelope_attack)) + ",\n")

    # decay_env
    if a_bag.volume_envelope_decay is None:
        file.write(str(check_gen_value(global_bag.volume_envelope_decay)) + ",\n")
    else:
        file.write(str(check_gen_value(a_bag.volume_envelope_decay)) + ",\n")

    # sustain_env
    if a_bag.volume_envelope_sustain is None:
        file.write(str(check_gen_value(global_bag.volume_envelope_sustain)) + ",\n")
    else:
        file.write(str(check_gen_value(a_bag.volume_envelope_sustain)) + ",\n")

    # release_env
    if a_bag.volume_envelope_release is None:
        file.write(str(check_gen_value(global_bag.volume_envelope_release)) + ",\n")
    else:
        file.write(str(check_gen_value(a_bag.volume_envelope_release)) + ",\n")

    file.write("sample_" + str(sample_num) + "\n},")


# A function that returns the value of a generator (a float value) multiplied 
# by 1000. This is done to save the decimal of the float to the thousandths 
# place before being coverted to an int.
# If the generator was set to None 0 is returned instead.
def check_gen_value(gen_value):
    if gen_value is None:
        return 0
    return int(gen_value * 1000)


# This function retrives the volume_envelope_delay generator value.
# There is no bag property in Sf2Utils to get the volume_envelope_delay value
# so it's necessary to grab that generator from the bag manually. 
# Each bag has a dictionary that maps a specified # from the SF2 manual to 
# each of its generators. The # for the delay envelope generator is 33.
def volume_envelope_delay(a_bag):
    try:
        a_gen = a_bag.gens[33]
        return a_gen.cents
    except KeyError:
        return None


# Checks if the selected sample is valid. Input is a sample object, and output is
# a tuple with (boolean, error_message - if any)
def is_sample_valid(sample):
    if sample.loop_duration >= sample.duration:
        return False, 'Loop length >= sample length'
    if sample.end_loop > sample.duration:
        return False, 'End loop index is larger than sample end index'
    return True, None


def error(message):
    print("ERROR: " + message)


# Copying functionality from wav2sketch.c
def print_bytes(file, b):
    global BCOUNT, WCOUNT, BUF32

    BUF32 |= b << (8 * BCOUNT)
    BCOUNT += 1
    if BCOUNT >= 4:
        file.write("0x%0.8X" % BUF32)
        file.write(",")
        BUF32 = 0
        BCOUNT = 0
        WCOUNT += 1
        if WCOUNT >= 8:
            file.write("\n")
            WCOUNT = 0


def padding(length, block):
    extra = length % block
    if extra == 0:
        return 0
    return block - extra


# Copying functionality from wav2sketch.c
def cc_to_int16(c1, c2):
    # i1 = int(ord(c1))
    # i2 = int(ord(c2))
    i1 = c1
    i2 = c2
    i1 &= 255
    i2 &= 255
    return (i2 << 8) | i1


# Copying functionality from wav2sketch.c
def ulaw_encode(audio):
    if audio >= 0:
        mag = audio
        neg = 0
    else:
        mag = audio * -1
        neg = 0x80

    mag += 128
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
