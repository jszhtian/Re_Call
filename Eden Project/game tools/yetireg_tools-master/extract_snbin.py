#!/usr/bin/env python

import os
import sys, argparse
from struct import unpack

from splz77.splz77_decompress import *

## *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
"""
parser = argparse.ArgumentParser(description='Uncompress sn.bin script archive to sn.ccpak')
parser.add_argument('--output', default=None, help='output file path (default: based on input path)')
parser.add_argument('input', help='sn.bin (compressed archive)')
args = parser.parse_args()
"""
DATAPATH = "sn.bin"#args.input
OUTPATH = "sn_out" if "sn_out" is not None else "{0}.ccpak".format(os.path.splitext(DATAPATH)[0])

# Debug only: map unpacked (.ccpak) to memory address
#MAPBASE = 0x08DD1700

## *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

with open(DATAPATH, 'rb') as data:
    size, = unpack('<L', data.read(0x4))
    print(size)
    uncompbin = splz77_decompress(data, size)

    with open(OUTPATH, 'wb') as pakfile:
        pakfile.write(uncompbin)
    
#    map = []

    offset = 0
    entid = 0
    maxoffset, = unpack('<L', uncompbin[0:4])
    while offset < maxoffset:
        entoffset, entsize, unk03, unk04 = unpack('<LLLL', uncompbin[offset:offset+16])
        fn = "z__{0}-{1:03d}.opcodescript".format(os.path.splitext(OUTPATH)[0], entid)
        with open(fn, 'wb') as filebin:
            filebin.write(uncompbin[entoffset:entoffset+entsize])
#        map.append("0x{0:08X}".format(MAPBASE+entoffset))

        entid += 1
        offset += 16

#    with open("{0}.map".format(os.path.splitext(DATAPATH)[0]), 'w') as mapfile:
#        mapfile.write(",\n".join(map))


    sys.exit(0)
