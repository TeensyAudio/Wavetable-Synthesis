##
# Audio Library for Teensy 3.X
# Copyright (c) 2017, TeensyAudio PSU Team
#
# Development of this audio library was sponsored by PJRC.COM, LLC.
# Please support PJRC's efforts to develop open source 
# software by purchasing Teensy or other PJRC products.
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice, development funding notice, and this permission
# notice shall be included in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
##

from sf2utils.sf2parse import Sf2File
import logging
import sys
import getopt
import inspect
import re
import math

DEBUG_FLAG = False
logging.disable(logging.WARNING)

## Prints out debug information
# @param flag the current debug flag being used
# @param message the debug message to print
# @param function the function being debugged
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

## Prints the menu
# @param choices a list of different input choices for the menu
#
# @return returns the selected choice
def menu(choices):
    print('')
    print_menu(choices)
    choice = safe_input('Select [1-{}]: '.format(len(choices)), int, 1, len(choices))
    return choice

## Prints out the menu options when running from command line
# @param menu_items a list of menu items
def print_menu(menu_items):
    if len(menu_items) < 1:
        return
    for i, j in enumerate(menu_items, 1):
        print('{}. {}'.format(i, j))
    print('')

## Gets user input from the command line and verifies that it's valid and
#  within the menu option bounds
# @param prompt the prompt for user input
# @param type the type of user input expected (int, char, etc...)
# @param min_ the minimum allowable entered value
# @param max_ the maximum allowable entered value
# @param range_ an allowable range for user input to be within
#
# @return returns the user's choice
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

## Clears the screen
def clear_screen():
    print(150*'\n')

## Prints usage string
def print_usage():
    print("USAGE: python3 decoder.py -i/--ifile (.sf2 file) <options>")
    print("-o (output file) : output to selected file")
    print("-d : debug mode")

## Get the options from command line and open file for menu
# @param argv the list of command line arguments given to the program
# @return returns the input file path and the output file path
def read_args(argv):
    global DEBUG_FLAG

    path = None
    outFile = None
    try:
        opts, args = getopt.getopt(argv, 'di:o:')
        if '-i' not in [opt[0] for opt in opts]: raise getopt.GetoptError("Missing -i option")
    except getopt.GetoptError as err:
        print("ERROR: " + err.args[0])
        print_usage()
        sys.exit(2)

    # read all options and open .sf2 file
    try:
        for opt, arg in opts:
            if opt == '-d':
                DEBUG_FLAG = True
            elif opt in ('-i'):
                if(arg == None or arg[-4:].lower() != '.sf2'):
                    print(arg[-4:])
                    raise TypeError("Invalid .sf2 file given: " + arg)
                path = arg
    except TypeError as err:
        print("ERROR: " + str(err.args[0]))
        print_usage()
        sys.exit(2)

    return path, outFile

## Main loop of the decoder when running from the command line
def main(argv):
    global DEBUG_FLAG
    # Disable warning logging to prevent sf2utils from logging any un-needed messages
    logging.disable(logging.WARNING)

    # Read args from the command line and open file
    try:
        path, outFile = read_args(argv)
        with open(path, 'rb') as sf2_file:
            sf2 = Sf2File(sf2_file)
    except FileNotFoundError as err:
        print("ERROR: " + str(err.args[1]) + ": " + path)
        print_usage()
        sys.exit(2)
    except:
        print("ERROR: An unexpected error occured while opening the file")
        sys.exit(2)

    clear_screen()
    print('       WELCOME  ')
    # print out some info:
    # version, date, etc...

    options = ('Select by Instrument', 'Quit')
    options2 = ('Select Again', 'Save and Quit')
    while True:
        choice = menu(options)
        if choice == 1: # select instrument
            clear_screen()
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
            if method == 1: # decode all samples for instrument
                decode_all(path, instrument, global_bag_index)
                print('All samples for instrument decoded successfully. Exiting Program.')
                return
            else: # decode selected samples for instrument
                selected_bags = []
                while True: # select which samples to decode
                    clear_screen()
                    print('Select Samples to Export\n')
                    print_menu(sample_names)
                    sample = safe_input('Select Sample [1-{}]: '.format(len(sample_names)), int, 1, len(sample_names))
                    print_debug(DEBUG_FLAG, 'Selected Sample is {}'.format(samples[sample-1].name))
                    if(bag_to_sample[sample-1][0] not in selected_bags):
                        selected_bags.append(bag_to_sample[sample-1][0])
                    i_result = menu(options2)
                    if i_result == 1: # select another sample
                        continue
                    elif i_result == 2: # decode list of selected samples
                        decode_selected(path, instrument, selected_bags, global_bag_index)
                        print('Selected samples for instrument decoded successfully. Exiting Program.')
                        return
        elif choice == 2: # exit
            print('Program Terminated by User')
            return
        else: # shouldn't be reached
            input("Wrong option selection. Enter any key to try again..")


## Decodes selected samples and outputs them to a file
# @param path the path of the file that contains the samples 
# @param inst_index the index of the instrument in the sf2 file whose samples are being decoded
# @param selected_bags a list of bag indexes for the samples being decoded
# @param global_bag_index the index of the insrument global bag if it exists otherwise value is None
# @param user_title a user selected filename for the samples output file
# @param user_dir the output directory for the decoded samples
def decode_selected(path, inst_index, selected_bags, global_bag_index, user_dir,
                    user_title=None):
    with open(path, 'rb') as file:
        sf2 = Sf2File(file)

        bags_to_decode = \
            [sf2.instruments[inst_index].bags[n] for n in selected_bags] if selected_bags \
                else sf2.instruments[inst_index].bags[1:] if global_bag_index != None \
                else sf2.instruments[inst_index].bags

        for bag in bags_to_decode:
            bag.sample.sm24_offset = None
            is_valid, error_msg = check_is_valid_sample(bag.sample)
            if not is_valid:
                error(error_msg)
                return False
            if selected_bags:
                print_debug(DEBUG_FLAG, 'Selected Sample is {}'.format(bag.sample.name))

        global_bag = sf2.instruments[inst_index].bags[global_bag_index] if global_bag_index != None else None
        file_title = user_title if user_title else sf2.instruments[inst_index].name
        file_title = re.sub(r'[\W]+', '', file_title)
        
        export_samples(bags_to_decode, global_bag, len(bags_to_decode), user_dir,
                       file_title=file_title)

        return True

## Decodes all the samples for an instrument
# @param path the path of the file that contains the samples 
# @param inst_index the index of the instrument in the sf2 file whose samples are being decoded
# @param global_bag_index the index of the insrument global bag if it exists otherwise value is None
def decode_all(path, inst_index, global_bag_index):
    decode_selected(path, inst_index, selected_bags=None, global_bag_index=global_bag_index)

## Write a sample out to C++ style data files.
# @param bags a list of bag indexes
# @param global_bag a global bag index. value is None if there isn't one
# @param num_samples number of samples to decode
# @param file_title the title of the ouptut file
# @param file_dir the output directory for the decoded samples
def export_samples(bags, global_bag, num_samples, file_dir, file_title="samples"):
    instrument_name = file_title
    instrument_name = ''.join([i for i in instrument_name if not i.isdigit()])
    h_file_name = "{}_samples.h".format(instrument_name)
    cpp_file_name = "{}_samples.cpp".format(instrument_name)
    with open(file_dir + "/" + cpp_file_name, "w") as cpp_file, open(file_dir + "/" + h_file_name, "w") as h_file:
        # Decode data to sample_data array in header file
        h_file.write("#pragma once\n#include <AudioStream.h>\n#include <AudioSynthWavetable.h>\n\n")
        h_file.write("extern const sample_data {0}_samples[{1}];\n".format(instrument_name, num_samples))

        #Sort bags by key range and expand ranges to fill all key values
        keyRanges = []
        getKeyRanges(bags, keyRanges)

        h_file.write("const uint8_t {0}_ranges[] = {{".format(instrument_name))
        for keyRange in keyRanges:
            h_file.write("{0}, ".format(keyRange[1]))
        h_file.write("};\n\n")
        h_file.write("const instrument_data {0} = {{{1}, {0}_ranges, {0}_samples }};\n\n".format(instrument_name, num_samples))

        cpp_file.write("#include \"{}\"\n".format(h_file_name))
        cpp_file.write("const sample_data {0}_samples[{1}] = {{\n".format(instrument_name, num_samples))
        for i in range(len(bags)):
            out_str = gen_sample_meta_data_string(bags[i], global_bag if global_bag else bags[i], i, instrument_name, keyRanges[i])
            cpp_file.write(out_str)
        cpp_file.write("};\n")

        # For each sample print out sample array to .cpp file and init to .h file
        for i in range(len(bags)):
            raw_wav_data = bags[i].sample.raw_sample_data
            length_16 = bags[i].sample.duration
            length_8 = length_16 * 2
            length_32 = math.ceil(length_16 / 2)
            pad_length = 0 if length_32 % 128 == 0 else 128 - length_32 % 128

            ary_length = int(length_32 + pad_length)

            smpl_identifier = "sample_{0}_{1}_{2}[{3}]" \
                .format(i, instrument_name, re.sub(r'[\W]+', '', bags[i].sample.name), ary_length)

            # Write array init to header file.
            h_file.write("\nextern const uint32_t {0};\n".format(smpl_identifier))

            # Write array contents to .cpp
            cpp_file.write("\nconst uint32_t {0} = {{\n".format(smpl_identifier))

            # Output 32-bit hex literals
            line_width = 0
            for j in range(0, length_8, 4):
                out_bytes = raw_wav_data[j:j+4]
                if len(out_bytes) != 4:
                    out_bytes += bytes(4 - len(out_bytes))
                hex_str = ''.join(['{:02x}'.format(out_bytes[k]) for k in range(3, -1, -1)])
                cpp_file.write('0x{},'.format(hex_str))
                line_width += 1
                if line_width == 8:
                    line_width = 0
                    cpp_file.write('\n')
            while pad_length > 0:
                cpp_file.write('0x00000000,')
                line_width += 1
                if line_width == 8:
                    line_width = 0
                    cpp_file.write('\n')
                pad_length -= 4
            cpp_file.write("};\n" if line_width == 8 else "\n};\n")

## Prints out the sample metadata
# @param bag the bag of the current sample being dedoded
# @param global_bag the global bag of the instrument
# @param sample_num the number assigned to this sample in the output file
# @param instrument_name name of the instrument
# @param keyRange the key range value pair for this sample. Always between 0-127.
def gen_sample_meta_data_string(bag, global_bag, sample_num, instrument_name, keyRange):
    out_fmt_str = \
        "\t{{\n" \
        "\t\t(int16_t*){SAMPLE_ARRAY_NAME}, // sample\n" \
        "\t\t{LOOP}, // LOOP\n" \
        "\t\t{LENGTH_BITS}, // LENGTH_BITS\n" \
        "\t\t(1 << (32 - {LENGTH_BITS})) * CENTS_SHIFT({CENTS_OFFSET}) * {SAMPLE_RATE:.1f} / NOTE({SAMPLE_NOTE}) / AUDIO_SAMPLE_RATE_EXACT + 0.5, // PER_HERTZ_PHASE_INCREMENT\n" \
        "\t\t((uint32_t){LENGTH} - 1) << (32 - {LENGTH_BITS}), // MAX_PHASE\n" \
        "\t\t((uint32_t){LOOP_END} - 1) << (32 - {LENGTH_BITS}), // LOOP_PHASE_END\n" \
        "\t\t(((uint32_t){LOOP_END} - 1) << (32 - {LENGTH_BITS})) - (((uint32_t){LOOP_START} - 1) << (32 - {LENGTH_BITS})), // LOOP_PHASE_LENGTH\n" \
        "\t\tuint16_t(UINT16_MAX * DECIBEL_SHIFT({INIT_ATTENUATION})), // INITIAL_ATTENUATION_SCALAR\n" \
        "\t\tuint32_t({DELAY_ENV:.2f} * SAMPLES_PER_MSEC / ENVELOPE_PERIOD + 0.5), // DELAY_COUNT\n" \
        "\t\tuint32_t({ATTACK_ENV:.2f} * SAMPLES_PER_MSEC / ENVELOPE_PERIOD + 0.5), // ATTACK_COUNT\n" \
        "\t\tuint32_t({HOLD_ENV:.2f} * SAMPLES_PER_MSEC / ENVELOPE_PERIOD + 0.5), // HOLD_COUNT\n" \
        "\t\tuint32_t({DECAY_ENV:.2f} * SAMPLES_PER_MSEC / ENVELOPE_PERIOD + 0.5), // DECAY_COUNT\n" \
        "\t\tuint32_t({RELEASE_ENV:.2f} * SAMPLES_PER_MSEC / ENVELOPE_PERIOD + 0.5), // RELEASE_COUNT\n" \
        "\t\tint32_t((1.0 - DECIBEL_SHIFT({SUSTAIN_FRAC:.1f})) * UNITY_GAIN), // SUSTAIN_MULT\n" \
        "\t\tuint32_t({VIB_DELAY_ENV:.2f} * SAMPLES_PER_MSEC / (2 * LFO_PERIOD)), // VIBRATO_DELAY\n" \
        "\t\tuint32_t({VIB_INC_ENV:.1f} * LFO_PERIOD * (UINT32_MAX / AUDIO_SAMPLE_RATE_EXACT)), // VIBRATO_INCREMENT\n" \
        "\t\t(CENTS_SHIFT({VIB_PITCH_INIT}) - 1.0) * 4, // VIBRATO_PITCH_COEFFICIENT_INITIAL\n" \
        "\t\t(1.0 - CENTS_SHIFT({VIB_PITCH_SCND})) * 4, // VIBRATO_COEFFICIENT_SECONDARY\n" \
        "\t\tuint32_t({MOD_DELAY_ENV:.2f} * SAMPLES_PER_MSEC / (2 * LFO_PERIOD)), // MODULATION_DELAY\n" \
        "\t\tuint32_t({MOD_INC_ENV:.1f} * LFO_PERIOD * (UINT32_MAX / AUDIO_SAMPLE_RATE_EXACT)), // MODULATION_INCREMENT\n" \
        "\t\t(CENTS_SHIFT({MOD_PITCH_INIT}) - 1.0) * 4, // MODULATION_PITCH_COEFFICIENT_INITIAL\n" \
        "\t\t(1.0 - CENTS_SHIFT({MOD_PITCH_SCND})) * 4, // MODULATION_PITCH_COEFFICIENT_SECOND\n" \
        "\t\tint32_t(UINT16_MAX * (DECIBEL_SHIFT({MOD_AMP_INIT_GAIN}) - 1.0)) * 4, // MODULATION_AMPLITUDE_INITIAL_GAIN\n" \
        "\t\tint32_t(UINT16_MAX * (1.0 - DECIBEL_SHIFT({MOD_AMP_SCND_GAIN}))) * 4, // MODULATION_AMPLITUDE_FINAL_GAIN\n" \
        "\t}},\n"

    length_bits = 0
    length = bag.sample.duration
    len = length
    shift_res = 1
    while len != 0:
        length_bits += 1
        len = len >> 1
    phase_mult = (0x80000000 >> (length_bits - 1))

    out_vals = {
        "LOOP": "true" if bag.sample_loop == 1 or global_bag.sample_loop == 1 else "false",
        "SAMPLE_NOTE": bag.base_note if bag.base_note else bag.sample.original_pitch,
        "CENTS_OFFSET": bag.fine_tuning if bag.fine_tuning else 0,
        "PHASE_MULT": phase_mult,
        "LENGTH": length,
        "LENGTH_BITS": length_bits,
        "SAMPLE_RATE": float(bag.sample.sample_rate),
        "LOOP_START": bag.cooked_loop_start,
        "LOOP_END": bag.cooked_loop_end,
        "KEY_RANGE_LOWER": keyRange[0],
        "KEY_RANGE_UPPER": keyRange[1],
        "VELOCITY_RANGE_LOWER": bag.velocity_range[0] if bag.velocity_range else 0,
        "VELOCITY_RANGE_UPPER": bag.velocity_range[1] if bag.velocity_range else 0,
        "SAMPLE_ARRAY_NAME": "sample_{0}_{1}_{2}".format(sample_num, instrument_name, re.sub(r'[\W]+', '', bag.sample.name)),
    }

    def get_gen(bag_num):
        return bag.gens[bag_num] if bag_num in bag.gens \
            else global_bag.gens[bag_num] if bag_num in global_bag.gens \
            else None

    def get_decibel_value(bag_num, default, min, max):
        gen = get_gen(bag_num)
        val = gen.short / 10.0 if gen else default
        return max if val > max else min if val < min else val

    def get_timecents_value(bag_num, default, min):
        gen = get_gen(bag_num)
        val = gen.cents * 1000.0 if gen else default
        return val if val > min else min

    def get_hertz(bag_num, default, min, max):
        gen = get_gen(bag_num)
        val = gen.absolute_cents if gen else default
        return max if val > max else min if val < min else val

    def get_pitch_cents(bag_num, default, min, max):
        gen = get_gen(bag_num)
        val = gen.short if gen else default
        return max if val > max else min if val < min else val

    env_vals = {
        # bag numbers correspond to generator numbers in the SoundFont spec
        "DELAY_ENV": get_timecents_value(33, 0, 0),
        "ATTACK_ENV": get_timecents_value(34, 1, 1),
        "HOLD_ENV": get_timecents_value(35, 0, 0),
        "DECAY_ENV": get_timecents_value(36, 1, 1),
        "SUSTAIN_FRAC": -get_decibel_value(37, 0, 0, 144),
        "RELEASE_ENV": get_timecents_value(38, 1, 1),
        "VIB_DELAY_ENV": get_timecents_value(23, 0, 0),
        "VIB_INC_ENV": get_hertz(24, 8.176, 0.1, 100),
        "MOD_DELAY_ENV": get_timecents_value(21, 0, 0),
        "MOD_INC_ENV": get_hertz(22, 8.176, 0.1, 100),
        "VIB_PITCH_INIT": get_pitch_cents(6, 0, -12000, 12000),
        "VIB_PITCH_SCND": -get_pitch_cents(6, 0, -12000, 12000),
        "MOD_PITCH_INIT": get_pitch_cents(5, 0, -12000, 12000),
        "MOD_PITCH_SCND": -get_pitch_cents(5, 0, -12000, 12000),
        "INIT_ATTENUATION": -get_decibel_value(48, 0, 0, 144),
        "MOD_AMP_INIT_GAIN": get_decibel_value(13, 0, -96, 96),
        "MOD_AMP_SCND_GAIN": -get_decibel_value(13, 0, -96, 96)
    }

    # dictionary comprehesion to merge out_vals and env_vals
    fmt_vals = {k: v for d in [out_vals, env_vals] for k, v in d.items()}

    return out_fmt_str.format(**fmt_vals)


# Checks if the selected sample is valid
# @param sample a sample object
# @return a tuple with (boolean, error_message)
def check_is_valid_sample(sample):
    if sample.end_loop > sample.duration:
        return False, 'End loop index is larger than sample end index'
    return True, None

def note_to_freq(note):
    exp = (float(note) - 69.0) / 12.0
    freq = float(pow(2, exp)) * 440.0
    return freq

# Retrieves all key ranges for the samples and expands them to fill empty
# space in the 0-127 range if needed.
# @param bags the sample bags to expand key ranges for
# @param keyRanges a list object that will hold the expanded key ranges of the samples. Should be empty when passed to this function.
def getKeyRanges(bags, keyRanges):
    # remove any bags without key ranges before sorting bags by key range
    tempList = [bag for bag in bags if bag.key_range == None]
    bags = [bag for bag in bags if bag not in tempList]

    # sort bags with key ranges and fill any gaps in range
    if(len(bags) != 0):
        bags.sort(key=lambda x:x.key_range[0])
        for aBag in bags:
            keyRanges.append([aBag.key_range[0], aBag.key_range[1]])

        for i in range(len(keyRanges)):
            if i == 0:
                keyRanges[i][0] = 0

            if i == len(keyRanges)-1:
                keyRanges[i][1] = 127
            else:
                keyDiff = keyRanges[i+1][0]-keyRanges[i][1]-1
                if keyDiff < 0: keyDiff = 0
                keyRanges[i][1] += int(keyDiff/2) + keyDiff%2
                keyRanges[i+1][0] -= int(keyDiff/2)

    # append bags without key ranges to the end of the bags list
    for aBag in tempList:
        bags.append(aBag)
        keyRanges.append([0, 127])

## Prints out an error message
# @param message an error message to display
def error(message):
    print("ERROR: " + message)

if __name__ == "__main__":
    main(sys.argv[1:])
