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
		return i

def safe_input(prompt, type_=None, min_=None, max_=None, range_=None):
	if min_ is not None and max_ is not None and max_ < min_:
		raise ValueError("min_ must be less than or equal to max_.")
	while True:
		ui = raw_input(prompt)
		if type_ is not None:
			try:
				ui = type_(ui)
			except ValueError:
				print("Input type must be {0}.".format(type_.__name__))
				continue
		if max_ is not None and ui > max_:
			print("Input must be less than or equal to {0}.".format(max_))
		elif min_ is not None and ui < min_:
			print("Input must be greater than or equal to {0}.".format(min_))
		elif range_ is not None and ui not in range_:
			if isinstance(range_, range):
				template = "Input must be between {0.start} and {0.stop}."
				print(template.format(range_))
			else:
				template = "Input must be {0}."
				if len(range_) == 1:
					print(template.format(*range_))
				else:
					print(template.format(" or ".join((", ".join(map(str, range_[:-1])), str(range_[-1])))))
		else:
			return ui

if len(sys.argv) < 2:
	path = raw_input("Please provide filepath to an SF2: ")
#	path = message.prompt("Please provide filepath to an SF2: ")
else:
	path = sys.argv[1]

elements = int(display_contents(path))
count = 0
while True:
	selection = safe_input('Enter an item\'s corresponding number: ', int, 1, elements)
#	str_selection = str(selection)
#	if str_selection == "c":
#		break
	
#	int_selection = int(selection)
#	if int_selection < 1 or int_selection > elements:
#		print('Invalid input. Try again.')
#		continue
	selection = selection - 1
	decoder.main(path, selection, count)
	count = count + 1


	
	

#def make_selection(message):
#	selection = int(input(message))

#	decoder.main(path,selection)

#def check_input():

#def error():
