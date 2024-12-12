import struct
import ctypes
import os
import zlib
import sys
import random
import argparse
    
def main():
    parser = argparse.ArgumentParser(description='Unpack LiveMaker 2/3 archives and EXEs')
    parser.add_argument('--input', required=True, dest='input_filename', help='Input filename')
    parser.add_argument('--output', required=True, dest='output_foldername', help='Output folder')
    args = parser.parse_args(sys.argv[1:])
    
    unpack_archive(args.input_filename, args.output_foldername)

def unpack_archive(input_filename, output_foldername):
    filename, ext = os.path.splitext(input_filename)
    file = open(input_filename, "rb")
    
    exe_archive = False
    if file.read(4) == "MZP\0":
        exe_archive = True
    
    file.seek(0, 0)

    if exe_archive:
        # Go to check bytes and verify that it's a VF EXE
        file.seek(-2, 2)
        check = file.read(2)

        if check != "lv":
            print "Not a valid VF EXE"
            exit(-1)

        # Go to archive offset position and read it
        file.seek(-6, 2)
        archive_offset = struct.unpack('I', file.read(4))[0]

        # Dump EXE without archive attached
        file.seek(0, 0)

        exe_data = file.read(archive_offset)
        exe = open(filename + "_unpacked" + ext, "wb")
        exe.write(exe_data)
        exe.close()

        #file.seek(archive_offset, 0)
    else:
        archive_offset = 0

    # Read VF archive
    check_2 = file.read(2)

    if check_2 != "vf":
        print "Not a valid VF archive at 0x%08d" % archive_offset
        exit(-2)

    unk = struct.unpack('I', file.read(4))[0]
    file_count = struct.unpack('I', file.read(4))[0]
    # file_count = 5

    key = 0x75d6ee39
    key_2 = 0
    files = []
    size_key = []

    # Build list of filenames
    for i in range(0, file_count):
        pos = file.tell()
        name_len = struct.unpack('I', file.read(4))[0]
        name = bytearray(file.read(name_len))

        # print "%08x: %d (%08x)" % (pos, name_len, name_len)
        i += 1

        for x in range(0, len(name)):
            key_3 = ctypes.c_uint32(key_2 * 4).value
            key_4 = ctypes.c_uint32(key_2 + key_3).value
            tkey = ctypes.c_uint32(key + key_4).value
            key_2 = tkey

            size_key.append(key_2)

            tkey ^= name[x]
            name[x] = tkey & 0xff

        files.append(name.decode('Shift_JIS'))
        # files.append(str(name))
    files.append("")  # One extra file because we need to calculate the size of the last file

    # Add file offsets to filename list
    for i in range(0, file_count + 1):
        rel_offset = struct.unpack('I', file.read(4))[0]
        b = struct.unpack('I', file.read(4))[0]

        rel_offset ^= size_key[i]

        files[i] = (files[i], archive_offset + rel_offset)

    # Add method flag to file list
    for i in range(0, file_count):
        method = struct.unpack('b', file.read(1))[0]

        files[i] = (files[i][0], files[i][1], method)

    files[-1] = (files[-1][0], files[-1][1], 0)  # Add last entry
    
    # Create output directory if needed
    if not os.path.exists(output_foldername):
        os.mkdir(output_foldername)
    # print [tmp[1] for tmp in files]
    # Dump data
    for i in range(0, len(files) - 1):
        filename = files[i][0]
        offset = files[i][1]
        size = files[i + 1][1] - offset
        method = files[i][2]

        output_filename = os.path.join(output_foldername, filename)
        output_path = os.path.dirname(output_filename)

        try:
            if not os.path.exists(output_path):
                os.makedirs(output_path)
        except:
            pass

        try:
            if os.path.exists(output_filename):
                continue
        except:
            pass

        file.seek(offset, 0)
        data = file.read(size)

        if method == 0:
            data = zlib.decompress(data)
        elif method == 1:
            # Raw data, don't process
            pass
        elif method == 2:
            data = scramble_data(data)
        elif method == 3:
            data = scramble_data(data)
            data = zlib.decompress(data)
                

        outfile = open(output_filename, "wb")
        outfile.write(data)
        outfile.close()

        print "%08x %08x %d" % (offset, size, method)


    
    
    
    
    
def mul(a, b):
    a *= b
    
    carry = 0
    if a > 0xffffffff:
        carry = 1
    
    over = (a >> 32) & 0xffffffff        
    a &= 0xffffffff

    return a, over, carry

def add(a, b):
    a += b
    
    carry = 0
    if a > 0xffffffff:
        carry = 1
        
    a &= 0xffffffff

    return a, carry

def adc(a, b, carry):
    a = a + b + carry
    
    if a > 0xffffffff:
        carry = 1
        
    a &= 0xffffffff
    
    return a, carry
    
def perform_round(l):
    m, edi, carry = mul(0x7dd4ffc7, l[3])
    ecx = m
    
    m, edx, carry = mul(l[2], 0x5d4)
    ecx, carry = add(ecx, m)
    l[3] = l[2]
    edi, carry = adc(edi, edx, carry)
    
    m, edx, carry = mul(l[1], 0x6f0)
    ecx, carry = add(ecx, m)
    l[2] = l[1]
    edi, carry = adc(edi, edx, carry)
    
    m, edx, carry = mul(l[0], 0x13fb)
    eax, carry = add(0, m)
    l[1] = l[0]
    
    eax, carry = add(eax, ecx)
    edx, carry = adc(edx, edi, carry)
    
    eax, carry = add(eax, l[4])
    edx, carry = adc(edx, 0, carry)
    
    l[0] = eax
    l[4] = edx
    
    return l

def random(seed, l):
    for i in range(0, 5):
        seed2 = ((seed << 0x0d) ^ seed) & 0xffffffff
        seed = seed2 ^ (seed2 >> 0x11)
        seed = seed ^ (seed << 0x05)
        seed &= 0xffffffff
        l.append(seed)
        
    for i in range(0, 0x13):
        l = perform_round(l)
            
    return l
    
def scramble_data(data):
    seed = struct.unpack("<I", data[4:8])[0]
    seed ^= 0xf8ea

    block_size = struct.unpack("<I", data[0:4])[0]
    block_count = (len(data) - 8) / block_size

    if (len(data) - 8) % block_size > 0:
        block_count += 1

    l = random(seed, [])

    blocks = range(block_count)
    new_blocks = [0] * block_count

    for i in range(block_count, 0, -1):
        perform_round(l)
        
        f = l[0] * 0.00000000023283064365386962890 * (i - 1)
        idx = int(f)
            
        new_blocks[blocks[idx]] = block_count - i
        blocks.pop(idx)
        blocks.append(block_count)

    arranged = ""
    for block in new_blocks:
        arranged += data[block * block_size + 8:(block + 1) * block_size + 8]
        
    return arranged

    
if __name__ == "__main__":
    main()