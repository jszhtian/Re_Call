from __future__ import print_function
import sys
import os
import struct
import argparse

from lib.lmdis import Disassembler
from lib.lmasm import Assembler


label_mappings = {}
label_offsets = {}
filename_mappings = {}


def main():
    parser = argparse.ArgumentParser(description='Assemble and disassemble LiveMaker 2/3 scripts.')
    parser.add_argument('mode', choices=['x', 'c'], help='x (disassemble)/c (create)')
    parser.add_argument('input_folder', help='Input folder')
    parser.add_argument('output_folder', help='Output folder')
    args = parser.parse_args(sys.argv[1:])

    if args.mode == "x":
        disassemble(args.input_folder, args.output_folder)
    elif args.mode == "c":
        assemble(args.input_folder, args.output_folder)


def disassemble(folder, output_path):
    file_list = []

    for root, directories, files in os.walk(folder):
        for filename in files:
            if filename.endswith(".lsb"):
                path = os.path.join(root, filename)

                f = path
                if path.startswith(folder + "/") or path.startswith(folder + "\\"):
                    f = f[len(folder) + 1:]

                file_list.append((path, f))
    project_filename = os.path.join(output_path, "project.proj")
    try:
        os.makedirs(os.path.dirname(project_filename))
    except:
        pass

    project = open(project_filename, "w")

    for file in file_list:
        project.write(file[1].replace(".lsb", ".xml") + "\n")
        
        dis = Disassembler(file[0], file[1])
        dis.disassemble()
        dis.write(os.path.join(output_path, file[1].replace(".lsb", ".xml")))

    project.close()


def assemble(folder, output_path):
    proj_path = os.path.join(folder, "project.proj")

    if not os.path.isfile(proj_path):
        print("Could not find project file")
        return

    files = open(proj_path).readlines()
    filenames = []
    for file in files:
        file = file.strip()

        if file == "":
            continue

        input_filename = os.path.join(folder, file)
        output_filename = os.path.join(output_path, file.replace(".xml", ".lsb"))

        filenames.append((input_filename, output_filename))

    for filename in filenames:
        first_pass(filename)

    for filename in filenames:
        second_pass(filename)


def first_pass(filenames):
    filename = filenames[0]
    output_filename = filenames[1]

    asm = Assembler(filename, output_filename)
    asm.assemble()

    label_mappings[asm.filename] = asm.label_mapping
    label_offsets[asm.filename] = asm.label_offset
    filename_mappings[filename] = asm.filename


def second_pass(filenames):
    filename = filenames[0]
    output_filename = filenames[1]

    if not os.path.isfile(output_filename):
        print("Could not open '%s'" % output_filename)
        return

    file = open(output_filename, "rb")
    data = bytearray(file.read())
    file.close()

    for label in label_offsets[filename_mappings[filename]]:
        if label['script'] in label_mappings:
            if label['target'] in label_mappings[label['script']]:
                new_label = label_mappings[label['script']][label['target']]

                if int(label['target']) != new_label:
                    print("Updated label %s -> %d at offset %08x" % (label['target'], new_label, label['offset']))
                    offset = struct.pack("<I", new_label)
                    data[label['offset']: label['offset'] + 4] = offset
            else:
                if int(label['target']) != 0:
                    print("Could not find label '%s' in script '%s'" % (label['target'], label['script']))
        else:
            print("Could not find label mapping for script '%s'" % (label['script']))

    file = open(output_filename, "wb")
    file.write(data)
    file.close()


if __name__ == "__main__":
    main()
