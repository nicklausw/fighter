#!/usr/bin/env python3
# symbol file to bsnes
# by nicklausw

import sys
import argparse

parser = argparse.ArgumentParser()
parser.add_argument("in_file", help="the input symbol file")
parser.add_argument("out_file", help="the output")
args = parser.parse_args()

f_in = open(args.in_file, 'r')
f_out = open(args.out_file, 'w')

f_out.write("#SNES65816\n[SYMBOL]\n")

f_in.seek(0)

line = f_in.readline()
while line:
  f_out.write(line[:-1] + " ANY 1\n")
  line = f_in.readline()

# the end mark
f_in.close()
f_out.close()
