# Python script for decoding a .SF2 file for use with Wavetable library.
from sf2utils.sf2parse import Sf2File
import logging
import sys
import getopt
import inspect
import re
import math

DEBUG_FLAG = False
logging.disable(logging.WARNING)

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
    global DEBUG_FLAG
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
                    if(bag_to_sample[sample-1][0] not in selected_bags):
                        selected_bags.append(bag_to_sample[sample-1][0])
                    i_result = menu(options2)
                    if i_result == 1:
                        continue
                    elif i_result == 2:
                        decode_selected(path, instrument, selected_bags, global_bag_index)
                        sys.exit('Selected samples for instrument decoded successfully. Exiting Program.')
        elif choice == 2:
            sys.exit('Program Terminated by User')
        else:   # shouldn't be reached
            input("Wrong option selection. Enter any key to try again..")

def decode_selected(path, inst_index, selected_bags, global_bag_index, user_title=None, user_dir=None):
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
        export_samples(bags_to_decode, global_bag, len(bags_to_decode), file_title=file_title, file_dir=user_dir)
        return True


def decode_all(path, inst_index, global_bag_index):
    decode_selected(path, inst_index, selected_bags=None, global_bag_index=global_bag_index)


# Write a sample out to C++ style data files.
def export_samples(bags, global_bag, num_samples, file_title="samples", file_dir=""):
    instrument_name = file_title
    h_file_name = file_dir + "/{}_samples.h".format(instrument_name)
    cpp_file_name = file_dir + "/{}_samples.cpp".format(instrument_name)
    with open(cpp_file_name, "w") as cpp_file, open(h_file_name, "w") as h_file:
        h_file.write("#pragma once\n#include <AudioStream.h>\n#include <AudioSynthWavetable.h>\n\n")
        # Decode data to sample_data array in header file
        h_file.write("extern const sample_data {0}_samples[{1}];\n".format(instrument_name, num_samples))

        #Sort bags by key range and expand ranges to fill all key values
        keyRanges = []
        getKeyRanges(bags, keyRanges)
		
        h_file.write("const int {0}_ranges[] = {{".format(instrument_name))
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

        for i in range(len(bags)):
            raw_wav_data = bags[i].sample.raw_sample_data
            length_16 = bags[i].sample.duration
            length_8 = length_16 * 2
            length_32 = math.ceil(length_16 / 2)
            pad_length = 0 if length_32 % 128 == 0 else 128 - length_32 % 128

            ary_length = int(length_32 + pad_length)

            smpl_identifier = "sample_{0}_{1}_{2}[{3}]"\
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


# prints out the sample metadata into the first portion of the sample array
def gen_sample_meta_data_string(bag, global_bag, sample_num, instrument_name, keyRange):
    out_fmt_str = \
        "\t{{\n" \
		"\t\t{LOOP},\t//Whether or not to loop this sample\n" \
        "\t\t{LENGTH_BITS},\t//Number of bits needed to hold length\n" \
        "\t\t({PHASE_MULT}*{CENTS_OFFSET}*({SAMPLE_RATE} / AUDIO_SAMPLE_RATE_EXACT)) / {SAMPLE_FREQ} + 0.5,\t//((0x80000000 >> (index_bits - 1)) * cents_offset * sampling_rate / AUDIO_SAME_RATE_EXACT) / sample_freq + 0.5\n" \
        "\t\t((uint32_t){LENGTH}-1) << (32 - {LENGTH_BITS}),\t//(sample_length-1) << (32 - sample_length_bits)\n" \
        "\t\t((uint32_t){LOOP_END}-1) << (32 - {LENGTH_BITS}),\t//(loop_end-1) << (32 - sample_length_bits) == LOOP_PHASE_END\n" \
        "\t\t(((uint32_t){LOOP_END}-1) << (32 - {LENGTH_BITS})) - (((uint32_t){LOOP_START}-1) << (32 - {LENGTH_BITS})),\t//LOOP_PHASE_END - (loop_start-1) << (32 - sample_length_bits) == LOOP_PHASE_END - LOOP_PHASE_START == LOOP_PHASE_LENGTH\n" \
        "\t\t(int16_t*){SAMPLE_ARRAY_NAME},\t//16-bit PCM encoded audio sample\n" \
		"\t\tuint32_t({DELAY_ENV}*SAMPLES_PER_MSEC/8.0+0.5),\t//DELAY_COUNT\n" \
        "\t\tuint32_t({ATTACK_ENV}*SAMPLES_PER_MSEC/8.0+0.5),\t//ATTACK_COUNT\n" \
        "\t\tuint32_t({HOLD_ENV}*SAMPLES_PER_MSEC/8.0+0.5),\t//HOLD_COUNT\n" \
        "\t\tuint32_t({DECAY_ENV}*SAMPLES_PER_MSEC/8.0+0.5),\t//DECAY_COUNT\n" \
        "\t\tuint32_t({RELEASE_ENV}*SAMPLES_PER_MSEC/8.0+0.5),\t//RELEASE_COUNT\n" \
        "\t\tint32_t({SUSTAIN_FRAC}*UNITY_GAIN),\t//SUSTAIN_MULT\n" \
        "\t\tuint32_t({VIB_DELAY_ENV} * SAMPLES_PER_MSEC / (2 * LFO_PERIOD)), \t// VIBRATO_DELAY\n" \
        "\t\tuint32_t({VIB_INC_ENV}/1000 * LFO_PERIOD * (UINT32_MAX / AUDIO_SAMPLE_RATE_EXACT)), // VIBRATO_INCREMENT\n" \
        "\t\t(CENTS_SHIFT(-{VIB_PITCH}) - 1.0)*4, // VIBRATO_PITCH_COEFFICIENT_INITIAL\n" \
        "\t\t(1.0 - CENTS_SHIFT({VIB_PITCH}))*4, // VIBRATO_COEFFICIENT_SECONDARY\n" \
        "\t\tuint32_t({MOD_DELAY_ENV} * SAMPLES_PER_MSEC / (2 * LFO_PERIOD)), // MODULATION_DELAY\n" \
        "\t\tuint32_t({MOD_INC_ENV}/1000 * LFO_PERIOD * (UINT32_MAX / AUDIO_SAMPLE_RATE_EXACT)), // MODULATION_INCREMENT\n" \
        "\t\t(CENTS_SHIFT(-{MOD_PITCH}) - 1.0)*4, // MODULATION_PITCH_COEFFICIENT_INITIAL\n" \
        "\t\t(1.0 - CENTS_SHIFT({MOD_PITCH}))*4, // MODULATION_PITCH_COEFFICIENT_SECOND\n" \
        "\t\tint32_t(UINT16_MAX * (DECIBEL_SHIFT(-0.1) - 1.0)), // MODULATION_AMPLITUDE_INITIAL_GAIN\n" \
        "\t\tint32_t(UINT16_MAX * (1.0 - DECIBEL_SHIFT(0.1))), // MODULATION_AMPLITUDE_FINAL_GAIN\n" \
        "\t}},\n"

    base_note = bag.base_note if bag.base_note else bag.sample.original_pitch
    cents_offset = (pow(2.0, float(bag.fine_tuning)/1200.0)) if bag.fine_tuning else 1.0
    length_bits = 0
    length = bag.sample.duration
    len = length
    shift_res = 1
    while len != 0:
        length_bits += 1
        len = len >> 1
    phase_mult = (0x80000000 >> (length_bits - 1))
    
    out_vals = {
        "LOOP": "true" if bag.sample_loop == 1 else "false",
        "ORIGINAL_PITCH": base_note,
        "CENTS_OFFSET": cents_offset,
        "PHASE_MULT": phase_mult,
        "LENGTH": length,
        "LENGTH_BITS": length_bits,
        "SAMPLE_RATE": float(bag.sample.sample_rate),
        "SAMPLE_FREQ": note_to_freq(base_note),
        "LOOP_START": bag.cooked_loop_start,
        "LOOP_END": bag.cooked_loop_end,
        "KEY_RANGE_LOWER": keyRange[0],
        "KEY_RANGE_UPPER": keyRange[1],
        "VELOCITY_RANGE_LOWER": bag.velocity_range[0] if bag.velocity_range else 0,
        "VELOCITY_RANGE_UPPER": bag.velocity_range[1] if bag.velocity_range else 0,
        "SAMPLE_ARRAY_NAME": "sample_{0}_{1}_{2}".format(sample_num, instrument_name, re.sub(r'[\W]+', '', bag.sample.name)),
    }

    sustain_env = bag.volume_envelope_sustain if bag.volume_envelope_sustain else global_bag.volume_envelope_sustain
    if sustain_env is not None:
        sustain_frac = float(sustain_env) / 96000.0
    else:
        sustain_frac = 0.0
	
    if sustain_frac > 1.0: 
        sustain_frac = 1.0
	
    env_vals = {
        # No property provided by SF2Utils; 33 is from form the SF2 spec
        "DELAY_ENV": bag.gens[33].cents if 33 in bag.gens else global_bag.gens[33].cents if 33 in global_bag.gens else None,
        "ATTACK_ENV": bag.volume_envelope_attack if bag.volume_envelope_attack else global_bag.volume_envelope_attack,
        "HOLD_ENV": bag.volume_envelope_hold if bag.volume_envelope_hold else global_bag.volume_envelope_hold,
        "DECAY_ENV": bag.volume_envelope_decay if bag.volume_envelope_decay else global_bag.volume_envelope_decay,
        "SUSTAIN_FRAC": sustain_frac,
        "RELEASE_ENV": bag.volume_envelope_release if bag.volume_envelope_release else global_bag.volume_envelope_release,
        "VIB_DELAY_ENV": bag.gens[23].cents if 23 in bag.gens else global_bag.gens[23].cents if 23 in global_bag.gens else None,
        "VIB_INC_ENV": bag.gens[24].absolute_cents if 24 in bag.gens else global_bag.gens[24].absolute_cents if 24 in global_bag.gens else None,
        "MOD_DELAY_ENV": bag.gens[21].cents if 21 in bag.gens else global_bag.gens[21].cents if 21 in global_bag.gens else None,
        "MOD_INC_ENV": bag.gens[22].absolute_cents if 22 in bag.gens else global_bag.gens[22].absolute_cents if 22 in global_bag.gens else None,
        "VIB_PITCH": bag.gens[6].absolute_cents/1000 if 6 in bag.gens else global_bag.gens[6].absolute_cents/1000 if 6 in global_bag.gens else None,
        "MOD_PITCH": bag.gens[5].absolute_cents/1000 if 5 in bag.gens else global_bag.gens[5].absolute_cents/1000 if 5 in global_bag.gens else None,
    }
    env_vals = {k: int(env_vals[k] * 1000) if env_vals[k] else 0 for k in env_vals}

    return out_fmt_str.format(**out_vals, **env_vals)


# Checks if the selected sample is valid. Input is a sample object, and output is
# a tuple with (boolean, error_message - if any)
def check_is_valid_sample(sample):
    if sample.loop_duration >= sample.duration:
        return False, 'Loop length >= sample length'
    if sample.end_loop > sample.duration:
        return False, 'End loop index is larger than sample end index'
    return True, None
	
def note_to_freq(note):
    exp = (float(note) - 69.0) / 12.0
    freq = float(pow(2, exp)) * 440.0
    return freq

# Retrieves all key ranges for the samples and expands them to fill empty
# space in the 0-127 range if needed.
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

def error(message):
    print("ERROR: " + message)


# Copying functionality from wav2sketch.c
# Currently unused
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
