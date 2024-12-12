#!/usr/bin/env python

import os
import sys, argparse
from struct import unpack
import re

## *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

CHOICEPREFIX = {
    0x31: b'CHOICE-\x81\x9F',
    0x32: b'SWITCH-\x81\x9F',
    0x54: b'CHOICENG-\x81\x9F',
}
SPECIALPREFIX = {
    0x46: b'\x81\x9F:SP:\x81\x9F',
    0x55: b'\x81\x9F:TITLE:\x81\x9F',
    #0x85: b'x85-\x81\x9F',
    0x86: b'x86-\x81\x9F',
    0x79: b'\x81\x9F:LOCATION:\x81\x9F',
    0x7B: b'\x81\x9F:CHAPTER:\x81\x9F',
}
RECORDSEP = b"\n\n"

def get_data_sjis(data, idx, end):
    result = b''
    while idx < end:
        tmp = data[idx:idx+1]
        if tmp == b'\x00':
            break
        result += tmp
        idx += 1
    return result

def escape_text(org):
    subs = org.split(b"\x0A")
    return b"{{newline}}".join(subs)

parser = argparse.ArgumentParser(description='Scan file to find shift-jis string')
parser.add_argument('-v', dest='verbose', help='show verbose output', action='store_true')
parser.add_argument('input', help='script file')
args = parser.parse_args()

outputpath = args.input + '-extr.txt'

## *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
# Generic string extraction routines

def getlen_opcode_X_plus_sz(data, start, extralen):
    str = get_data_sjis(data, start+1+extralen, start+500)
    return (1+extralen+len(str)+1, str)

def getlen_opcode_2_plus_sz(data, start):
    return getlen_opcode_X_plus_sz(data, start, 2)

def getlen_opcode_4_plus_sz(data, start):
    return getlen_opcode_X_plus_sz(data, start, 4)

def getlen_opcode_7_plus_sz(data, start):
    return getlen_opcode_X_plus_sz(data, start, 7)

## *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
# Specialized opcode handlers

def getlen_opcode05(data, start):
    first_byte = ord(data[start+1:start+2])
    if first_byte in opcodeslen: # opcodeslen is a global
        return (1, None)
    return (5, None)

def getlen_opcode0D(data, start):
    absofs, = unpack("<L", data[start+3:start+7])
    if absofs > start + 7 and absofs <= start + 0x10:
        return (absofs - start, data[start+7:absofs])
    return (7, None)

def getlen_opcodes_0E_56(data, start):
    count, = unpack("<H", data[start+3:start+5])
    return (5+6*count, None)

def getlen_opcodes_31_32(data, start):
    choices = []
    offsetstr = 7
    for i in range(ord(data[start+3:start+4])):
        choice = get_data_sjis(data, start+offsetstr+10, start+offsetstr+500)
        offsetstr += 10 + len(choice) + 1
        choices.append(choice)
    return (offsetstr, choices)

def getlen_opcode44(data, start):
    val, = unpack("<H", data[start+3:start+5])
    return (5 if val != 0xFFFF else 6, None)

def getlen_opcode54(data, start):
    choices = []
    offsetstr = 11
    for i in range(unpack("<H", data[start+7:start+9])[0]):
        choice = get_data_sjis(data, start+offsetstr+2, start+offsetstr+500)
        offsetstr += 2 + len(choice) + 1
        choices.append(choice)
    return (offsetstr, choices)

def getlen_opcodes_55_79(data, start):
    startofs, = unpack("<H", data[start+1:start+3])
    endofs,   = unpack("<H", data[start+6:start+8])
    str = get_data_sjis(data, start+10, start+500)
    # assert (endofs-startofs) == len(str)+1
    return (10+len(str)+1, str)

def getlen_opcode7B_root2(data, start):
    if unpack("<L", data[start+1:start+5])[0] != 0:
        return (5, None)
    if ord(data[start+5:start+6]) == 0x55:
        startofs, = unpack("<H", data[start+6:start+8])
        endofs,   = unpack("<H", data[start+11:start+13])
        str = get_data_sjis(data, start+15, start+500)
        # assert (endofs-startofs) == len(str)+1
        return (15+len(str)+1, str)
    return (5, None)

def getlen_opcode87_root2(data, start):
    curoffset = 3
    while curoffset < 500 and unpack("<H", data[start+curoffset:start+curoffset+2])[0] != 0xFFFF:
        curoffset += 18
    if curoffset >= 500:
        raise Exception('opcode 87 len>500 probably incorrect')
    return (curoffset+2, None)

## *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

opcodeslen = {
    #0x00: 1, # likely sign of an error
    0x01: 5,
    0x02: 5,
    0x03: 5,
    0x04: 5,
    0x05: getlen_opcode05,
    0x06: 9,
    0x07: 9, # for some reason sometimes followed by a 4-byte number 0x00012C33
    0x08: 9,
    0x09: 9,
    0x0A: 9,
    0x0B: 9,
    0x0C: 7,
    0x0D: getlen_opcode0D,
    0x0E: getlen_opcodes_0E_56,
    
    0x10: 5,
    0x11: 5,
    0x12: 5,
    0x13: 5,
    0x14: 5,
    0x15: 5,
    0x16: 5,
    0x17: 5,
    0x18: 5,
    0x19: 9,
    0x1A: 5,
    0x1B: 1,
    0x1C: 1,
    0x1D: 7,
    0x1E: 11,
    0x1F: 13,

    0x20: 7,
    0x21: 7,
    0x22: 5,
    0x23: 7,
    0x24: 7,
    0x25: 5,
    0x27: 1,
    0x28: 3,
    0x2B: 1,
    0x2C: 3,
    0x2D: 5,
    0x2E: 1,
    0x2F: 3,

    0x30: 11,
    0x31: getlen_opcodes_31_32, # choice
    0x32: getlen_opcodes_31_32,
    0x33: 1,
    0x34: 11,
    0x35: 5,
    0x36: 3,
    0x37: 1,
    0x38: 3,
    0x39: 5,
    0x3A: 5,
    0x3B: 3,
    0x3C: 3,
    0x3D: 1,
    0x3F: 1,

    0x42: 9,
    0x43: 3,
    0x44: getlen_opcode44,
    0x45: getlen_opcode_4_plus_sz, # text
    0x46: getlen_opcode_4_plus_sz, # special text?
    0x47: getlen_opcode_2_plus_sz, # charname
    0x48: 3,
    0x49: 5,
    0x4A: 3,
    0x4B: 5,
    0x4C: 7,
    0x4E: 5,
    0x4F: 5,

    0x51: 7,
    0x54: getlen_opcode54, #choice
    0x55: getlen_opcodes_55_79,
    0x56: getlen_opcodes_0E_56,
    0x59: 1,
    0x5A: 1,
    0x5E: 3,
    0x5F: 1,

    0x60: 7,
    0x61: 7,
    0x62: 5,
    0x63: 11,
    0x64: 7,
    0x65: 5,
    0x66: 3,
    0x68: 11,
    0x69: 3,
    0x6A: 5,
    0x6B: 3,
    0x6C: 17,
    0x6D: 3,
    0x6E: 5,
    0x6F: 7,

    0x70: 1,
    0x71: 7,
    0x72: 5,
    0x74: 7,
    0x75: 5,
    0x79: getlen_opcodes_55_79,
    0x7A: 5,
    0x7B: 5,
    0x7C: 5,
    0x7D: 3,
    0x7E: 3,
    0x7F: 5,

    0x80: 5,
    0x81: 3, # disabled for error detection
    0x82: 3,
    0x83: 5,
    0x85: getlen_opcode_4_plus_sz, # ? Debug string ?
    0x86: 5, #psp
    0x87: 5, #psp
    0x88: 1,
}

## *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
# Fix quirks (psp)

if re.search('cc.*psp', args.input) is not None:
    if args.verbose:
        print("cross channel psp input detected based on filename", file=sys.stderr)
    opcodeslen.update({0x05: 5})  # 0x05 at end of script
if re.search('sgpsp', args.input) is not None:
    if args.verbose:
        print("secret game psp input detected based on filename", file=sys.stderr)
    opcodeslen.update({
        0x0F: 1,
        0x84: 3,
    })
    if re.search('000', args.input) is not None:
        opcodeslen.update({0x00: 1})  # bad def 0x0F,0x5F?
if re.search('kon.*psp.*(000|001)', args.input) is not None:
    opcodeslen.update({0x00: 1})
if re.search('tama.*psp.*(000|022|044|053|083)', args.input) is not None:
    opcodeslen.update({0x00: 1})
if re.search('sg2.*psp', args.input) is not None:
    if args.verbose:
        print("secret game 2 psp input detected based on filename", file=sys.stderr)
    del opcodeslen[0x6A]
    opcodeslen.update({
        0x43: 5,  # and possibly all newer psp titles (2013+)
        0x47: getlen_opcode_4_plus_sz,
        0x49: 1,
        0x4A: 1,  # sg2-specific so far
        0x56: 5,
        0x6A: 1,
        0x7A: 7,  # and newer
        0x81: 7,  # sg2-specific so far
    })

## *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
# Fix quirks (x360)

if re.search('[xX]360', args.input) is not None:
    if args.verbose:
        print("x360 input detected based on filename", file=sys.stderr)
    del opcodeslen[0x46]
    opcodeslen.update({
        0x0F: 9,
        0x43: 5,
        #0x46: ... string?
        0x47: getlen_opcode_4_plus_sz,
        0x86: getlen_opcode_4_plus_sz, # ?
        0x8B: 5,
        0x8C: 5,
        0x8D: 1,
    })
    del opcodeslen[0x88]
    del opcodeslen[0x56]  # chk please

    if re.search('cc.*360.*001', args.input) is not None:
        opcodeslen.update({0x00: 1})
    if re.search('root.*360', args.input) is not None:
        if args.verbose:
            print("root double x360 input detected based on filename", file=sys.stderr)
        opcodeslen.update({
            #0x56: 5,
            0x7A: 11,
            0x7B: getlen_opcode7B_root2,
            0x87: getlen_opcode87_root2,
            0x8A: 3,
            0x8E: 11,
            0x8F: 7,
        })

matches = re.search('.+-([0-9]+)\\.opcodescript$', args.input)
detected_scriptnum = int(matches.group(1), 10) if matches is not None else None

## *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

data = open(args.input, 'rb').read()
with open(outputpath, 'w', encoding='utf8', newline="\r\n") as resfile:
    debug05 = ''
    entryidx = 1
    codeofs, = unpack('<L', data[0:0+4])

    length = len(data)
    is_near_end = 1 if length <= 0x20 else 0

    while codeofs < length:
        opcode = data[codeofs]
        if args.verbose:
            print("ofs {0:04X} {1:02X}".format(codeofs, opcode), file=sys.stderr)

#        if opcode == 2 and unpack("<H", data[codeofs+1:codeofs+3])[0] != 0:
#            print("j {0:02X}".format(unpack("<H", data[codeofs+1:codeofs+3])[0]), file=sys.stderr)

        # begin file footer processing -- Gosh it is kinda confusing...
        if length - codeofs - 5 < 0x30:
            opcodeslen.update({0x00: 1})
            if opcode == 0x05:
                is_near_end += 1

        if opcode == 0 and is_near_end >= 1:
            is_near_end += 1
        if is_near_end >= 2 and opcode != 0:
            if args.verbose:
                print("near end: ofs {0:04X} - [4W]={1} this={2} - skipping (ok)".format(
                  codeofs,
                  unpack("<l", data[codeofs:codeofs+4])[0],
                  detected_scriptnum if detected_scriptnum is not None else args.input
                ), file=sys.stderr)
            codeofs += 4
            continue
        # end file footer processing

        if opcode not in opcodeslen:
            print(debug05, end='', file=sys.stderr)
            print("!ofs {0:04X} error {1:02X}".format(codeofs, opcode), file=sys.stderr)
            break
        if hasattr(opcodeslen[opcode], '__call__'):
            try:
                oplen, opdata = opcodeslen[opcode](data, codeofs)
                if oplen <= 0 or oplen is None:
                    print("ofs {0:04X} error {1:02X} could not determine len={2}".format(codeofs, opcode, oplen), file=sys.stderr)
                    break

                # [0x45] [<unk>.4B] [<serifu>.B0]
                # [0x46] [<unk>.4B] [<serifu>.B0]
                # [0x47] [<unk>.2B] [<charname>.B0]
                # [0x86] [<unk>.4B] [<serifu>.B0]
                if opcode in [0x45, 0x46, 0x47, 0x79, 0x86] or (opcode == 0x55 and opdata is not None) or (opcode == 0x7B and opdata is not None):
                    rng = escape_text(opdata)
                    idxprefix = '<{0:03d}> '.format(entryidx).encode('utf-8')
                    rng = idxprefix + (SPECIALPREFIX[opcode] if opcode in SPECIALPREFIX else b'') + rng + RECORDSEP
                    sjistext = rng.decode('cp932')
                    entryidx = entryidx + 1
                    resfile.write(sjistext)
                # [0x31] [<unk>.2B] [<#choices>.4B?] #choices_times--([<>].10B [<text>.B0])--
                # [0x54] [<unk>.2B] [<unk>.4B] [<#choices>.2B] [<unk>.2B] #choices_times--([<>].2B [<text>.B0])--
                elif opcode in [0x31, 0x32, 0x54]:
                    for choicetext in opdata:
                        choicerng = escape_text(choicetext)
                        idxprefix = '<{0:03d}> '.format(entryidx).encode('utf-8')
                        bintext = idxprefix + CHOICEPREFIX[opcode] + choicerng + RECORDSEP
                        choicetext = bintext.decode('cp932')
                        entryidx = entryidx + 1
                        resfile.write(choicetext)
                    
            except Exception as e:
                print(debug05, end='', file=sys.stderr)
                print("Unexpected error @{0:04X} - {1}: {2}".format(codeofs,sys.exc_info()[0], e), file=sys.stderr)
                print(opcodeslen[opcode], file=sys.stderr)
                break
            
            codeofs += oplen
        else:
            codeofs += opcodeslen[opcode]

        if codeofs + 4 < length and opcode == 0x7 and unpack('<L', data[codeofs:codeofs+4])[0] == 0x00012C33:
            codeofs += 4

if os.path.getsize(outputpath) == 0:
    os.remove(outputpath)

