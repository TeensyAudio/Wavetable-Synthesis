#!/usr/bin/env python3

import sys
import decoder
from sf2utils.sf2parse import Sf2File
from struct import *
from collections import namedtuple

def display_contents(arg1):
	with open (arg1, 'rb') as sf2_file:
		sf2 = Sf2File(sf2_file)
		for i, j in enumerate(sf2.samples, 1):
			print("{}. {}".format(i, j.name))

if len(sys.argv) < 2:
	path = raw_input("Please provide filepath to an SF2: ")
#	path = message.prompt("Please provide filepath to an SF2: ")
else:
	path = sys.argv[1]

display_contents(path)
count = 0
while True:
	selection = raw_input('Enter a number to select a sample or enter (c) to continue: ')
	str_selection = str(selection)
	if str_selection == "c":
		break
	int_selection = int(selection)
	int_selection = int_selection - 1
	decoder.main(path, int_selection, count)
	count = count + 1


	
	

#def make_selection(message):
#	selection = int(input(message))

#	decoder.main(path,selection)

#def check_input():

#def error():
