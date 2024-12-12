#! /usr/bin/env python
# -*- coding: utf-8 -*-

from __future__ import print_function
import os
import struct
import zlib
import argparse
from six.moves import range
from codecs import open
from collections import namedtuple

# struct tuples
from lib.obfuscation import Deobfuscator

VFFHeader = namedtuple('VFFHeader', 'signature, entry_count')
VFFHeader.struct = '=6sL'
VFFHeader.struct_size = struct.calcsize(VFFHeader.struct)

VFFEntry = namedtuple('VFFEntry', 'offset, unknown2')
VFFEntry.struct = 'LL'
VFFEntry.struct_size = struct.calcsize(VFFEntry.struct)


def parse_file_table(ext_file):
    ext = open(ext_file, 'rb')
    print('Reading game.ext...')

    # read header
    sig, entries = struct.unpack(VFFHeader.struct, ext.read(VFFHeader.struct_size))
    sig = sig.rstrip(b'\x00')
    header = VFFHeader(sig, entries)
    print(header.entry_count, 'entries found')

    deobf = Deobfuscator()

    # read filenames
    filenames = []
    for i in range(header.entry_count):
        fn_length = struct.unpack('L', ext.read(4))[0]
        byte_name = struct.unpack('%dB' % fn_length, ext.read(fn_length))
        filename = deobf.deobfuscate_filename(byte_name)
        filenames.append(filename)

    # read offsets
    offsets = []
    for i in range(header.entry_count):
        offset, idk = struct.unpack(VFFEntry.struct, ext.read(VFFEntry.struct_size))
        offset = deobf.deobfuscate_ulong(offset)
        offsets.append(offset)

    ext.close()
    return list(zip(filenames, offsets))


def write_csv(table, output_dir):
    # write results to unicode csv
    csvfile = open(os.path.join(output_dir, 'files.csv'), 'w', encoding='utf8')
    csvfile.write('Filename,Offset\n')
    for filename, offset in table:
        csvfile.write(u'{},{}\n'.format(filename, offset))
    csvfile.close()


def extract_files(table, dat_file, output_dir, extension=None, deflate=True):
    dat = open(dat_file, 'rb')

    # the file table doesn't include a file size...
    for i, x in enumerate(table):
        filename = x[0]
        offset = x[1]
        # skip files if extension provided
        if extension is not None and not filename.endswith(extension):
            continue

        if os.path.sep in filename:
            home = os.path.abspath('.')
            os.chdir('out')
            dirs = filename.split(os.path.sep)[:-1]

            # walk down and create directories
            for directory in dirs:
                try:
                    os.mkdir(directory)
                except OSError:
                    pass
                os.chdir(directory)

            # go back to the beginning
            os.chdir(home)

        # make output folder
        try:
            os.mkdir('out')
        except OSError:
            pass

        # open file for reading
        # but make sure we've got the
        output_path = os.path.join(output_dir, filename)
        try:
            os.makedirs(os.path.dirname(output_path), 0o755)
        except OSError:
            pass
        out = open(output_path, 'wb')

        # yoink the next offset
        try:
            next_offset = table[i + 1][1]
        # is the last item, so the offset would be the end
        except IndexError:
            next_offset = os.stat(dat_file).st_size

        # read and write
        dat.seek(offset)

        # compression detection
        data = dat.read(next_offset - offset)
        if deflate and data.startswith(b'x'):
            print('zlib detected on {}. Decompressing.'.format(repr(filename)))
            data = zlib.decompress(data)

        out.write(data)
        out.close()


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('ext_file', help='LM3 file table (e.g. game.ext)')
    parser.add_argument('dat_file', help='LM3 archive file (e.g. game.dat)')
    parser.add_argument('output', default='output', help='Output directory')
    args = parser.parse_args()

    # parse table
    table = parse_file_table(args.ext_file)

    # extract files
    extract_files(table, args.dat_file, args.output)


if __name__ == '__main__':
    main()