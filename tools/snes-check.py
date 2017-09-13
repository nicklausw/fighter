#!/usr/bin/env python3
# snes lorom checksum fixer
# by nicklausw

import sys
import argparse

parser = argparse.ArgumentParser()
parser.add_argument("sfc_file")
args = parser.parse_args()

f_use = open(args.sfc_file, 'rb+')


all_bytes = 0

for c in f_use.read():
  all_bytes += c

# add extra checksum bytes
all_bytes += 0x1fe

# make it 16-bit
all_bytes &= 0xffff

# and make the inverse
inverse = all_bytes ^ 0xffff

f_use.seek(0x7fdc)

f_use.write(bytes([inverse & 0xff]))
f_use.write(bytes([inverse >> 8]))

f_use.write(bytes([all_bytes & 0xff]))
f_use.write(bytes([all_bytes >> 8]))

f_use.close()
