#!/usr/bin/env python

import sys
import decoder
from sf2utils.sf2parse import Sf2File
from struct import *
import time



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
        sys.stdout.write("\r{}{}{}{}{}".format(pad*i,
            smbl[i%len(smbl)], smbl[(i+1)%len(smbl)],
            smbl[(i+2)%len(smbl)], smbl[(i+3)%len(smbl)]))
        sys.stdout.flush()
        time.sleep(time_)
    sys.stdout.write("\r{}{}".format(pad*(num+1), ' '))
    sys.stdout.flush()
    print''

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

count = 0
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
            #for the selected instrument, go through all bags and
            #retrieve sample(s)
#TODO
            print''
            print('This functionality is not complete...')
            print('No samples decoded.')
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
            print 'Exporting Sample...'
            decoder.main(path, sample-1, count=count+1)
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
                animate(num=15, time_=0.16, smbl='  Exiting ')
                sys.exit('Program Terminated by User')

    elif choice == 3:
        animate(num=15, time_=0.16, smbl='  Exiting ')
        sys.exit('Program Terminated by User')
    else:   #shouldn't be reached
        raw_input("Wrong option selection. Enter any key to try again..")
