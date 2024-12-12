from __future__ import print_function
import struct
import argparse
import sys
import os

from bs4 import BeautifulSoup

from constants import *


FURIGANA_ENABLED = False  # Untested, use not recommended


def hexstr2bytearray(input):
    if ' ' in input:
        input = input.replace(' ', '')

    return bytearray(input.decode('hex'))


def printhex(input):
    for b in input:
        print("%02x" % b)
    print("")


class Assembler:
    soup = None
    output = None
    cur_id = 0
    filename = ""
    label_mapping = {}
    label_offset = []
    output_filename = ""

    def __init__(self, input_filename, output_filename):
        self.soup = None
        self.output = None
        self.cur_id = 0
        self.filename = ""
        self.label_mapping = {}
        self.label_offset = []
        self.output_filename = ""

        if not os.path.isfile(input_filename):
            print("Could not open '%s'" % input_filename)
            return

        xml = open(input_filename).read()

        self.soup = BeautifulSoup(xml)
        self.output_filename = output_filename

    def assemble(self):
        if self.soup == None:
            return

        try:
            output_path = os.path.dirname(self.output_filename)
            os.makedirs(output_path)
        except:
            pass

        print("Assembling %s..." % self.output_filename)

        self.output = open(self.output_filename, "wb")

        if self.soup.lmscript.has_attr("name"):
            self.filename = self.soup.lmscript['name']

        self.assemble_header()
        self.assemble_body()

    def assemble_header(self):
        if self.soup.lmscript.header is None:
            print("Could not find header")
            exit(-1)

        script_version = int(self.soup.lmscript.header['version'])
        unk1 = int(self.soup.lmscript.header['flag'])

        self.write_int(script_version)
        self.write_byte(unk1)

        # Parse header
        infos = self.soup.lmscript.header.properties.find_all('property')

        info_data_len = 0
        if len(infos) > 0:
            info_data_len = len(hexstr2bytearray(infos[0].text))

        self.write_int(len(infos))
        self.write_int(info_data_len)

        property_info = {}
        for info in infos:
            id = int(info['id'])
            data = info.text
            property_info[id] = hexstr2bytearray(data)

        for i in range(0, len(infos)):
            # Write all of the properties in order
            if property_info.has_key(i):
                self.write(property_info[i])
            else:
                self.write('\00' * info_data_len)

    def assemble_body(self):
        self.cur_id = 0

        if self.soup.lmscript.body is None:
            print("Could not find body")
            exit(-1)

        items = self.soup.lmscript.body.find_all('item')

        self.write_int(len(items))

        for item in items:
            if item['command'] is None:
                print("ERROR: Could not find command associated with item")
                exit(-2)

            if not item['command'] in opcode_names_inv:
                print("ERROR: Could not find command in opcode table: %s" % item['command'])
                exit(-2)

            # Write opcode header
            self.write_byte(opcode_names_inv[item['command']])

            if item.has_attr('diff'):
                self.cur_id += int(item['diff'])
            else:
                self.cur_id += 1

            indent = 0
            if item.has_attr('indent'):
                indent = int(item['indent'])
            self.write_int(indent)

            mute = 0
            if item.has_attr('mute'):
                mute = int(item['mute'])
            self.write_byte(mute)

            not_update = 0
            if item.has_attr('notupdate'):
                not_update = int(item['notupdate'])
            self.write_byte(not_update)

            if not self.label_mapping.has_key(item['id']):
                self.label_mapping[item['id']] = self.cur_id
            else:
                pass
                # print "Found colliding id with '%s' command..." % last
            self.write_int(self.cur_id)

            # Write opcode data
            for content in item.findAll(recursive=False):
                self.write_type(content)

            # if item['command'] not in ("WhileInit", "WhileLoop"):
            #    self.cur_id += 1


    @staticmethod
    def write_byte_data(data):
        return bytearray(struct.pack('<B', data))

    @staticmethod
    def write_word_data(data):
        return bytearray(struct.pack('<H', data))

    @staticmethod
    def write_int_data(data):
        return bytearray(struct.pack('<I', data))

    @staticmethod
    def write_string_data(data):
        if data == '\n':
            data = "\r\n"

        str_data = bytearray(map(ord, data.encode('shift-jis')))
        output = bytearray(struct.pack('<I', len(str_data)))
        output += str_data
        return output

    def write_bool_data(self, data):
        data = data.lower() == "true"
        return self.write_byte_data(data)

    def write(self, data):
        self.output.write(data)

    def write_byte(self, data):
        self.output.write(self.write_byte_data(data))

    def write_word(self, data):
        self.output.write(self.write_word_data(data))

    def write_int(self, data):
        self.output.write(self.write_int_data(data))

    def write_string(self, data):
        self.write(self.write_string_data(data))

    def write_bool(self, data):
        self.output.write(self.write_bool_data(data))

    charcount = 0

    def write_type(self, data, write_opcode=False):
        ignore_tag = ("param", "array", "event", "eventblock", "eventarg", "furigana", "label", "setlabel")
        extra_commands = 0

        if data.name not in type_table_inv_lower and data.name not in ignore_tag:
            print("Could not find type '%s'" % data.name)
            exit(-2)

        if write_opcode and data.name not in ignore_tag:
            self.write_byte(type_table_inv_lower[data.name])

        if data.name == "str":
            self.write_string(data.text)

        elif data.name == "flag":
            self.write_bool(data.text)

        elif data.name == "byte":
            self.write_byte(int(data.text))

        elif data.name == "var":
            self.write_string(data.text)

        elif data.name == "int":
            self.write_int(int(data.text))

        elif data.name == "furigana":
            version = int(data['version'])
            output = bytearray()

            output += self.write_int_data(int(data['textcommands']))
            output += self.write_int_data(int(data['unk1']))
            output += self.write_int_data(int(data['unk2']))
            output += self.write_int_data(int(data['unk3']))
            output += self.write_byte_data(int(data['unk4']))
            output += self.write_byte_data(int(data['unk5']))

            if version < 0x64:
                output += self.write_byte_data(int(data['unk6']))

            output += self.write_int_data(int(data['unk7']))
            output += self.write_string_data(data['unk8'])
            output += self.write_string_data(data['unk9'])

            if version >= 0x64:
                output += self.write_int_data(int(data['unk10']))
                output += self.write_int_data(int(data['unk11']))

            return output

        elif data.name == "float":
            self.write(hexstr2bytearray(data.text))

        elif data.name == "label":
            self.write_string(data['script'])

            self.label_offset.append({'script': data['script'], 'target': data['target'], 'offset': self.output.tell()})
            self.write_int(int(data['target']))

        elif data.name == "setlabel":
            self.label_mapping[data.text] = self.cur_id
            self.write_string(data.text)

        elif data.name == "eventarg":
            output = bytearray()

            if data['type'] == "Str":
                output += self.write_string_data(data.text)

            elif data['type'] == "Byte":
                output += self.write_byte_data(int(data.text))

            elif data['type'] == "Int":
                output += self.write_int_data(int(data.text))

            elif data['type'] == "CharString":
                # Write a OpeChar for each byte in the string
                
                extra_data = 0x32
                
                if data.has_attr("extradata"):
                    extra_data = int(data['extradata'])

                # Python is stupid (or maybe that's just me) so decode the text and then combine each shift-jis
                # character manually
                t = data.text.encode('shift-jis')
                i = 0
                while i < len(t):
                    c = ord(t[i])
                    i += 1

                    if c > 0x7f:
                        c = (c << 8) | ord(t[i])
                        i += 1

                    output += self.write_byte_data(event_opcode_names_inv['Char'])
                    output += self.write_string_data("")
                    output += self.write_int_data(extra_data)
                    output += self.write_word_data(c)
                    output += self.write_int_data(0)

                    extra_commands += 1
                extra_commands -= 1  # Because the initial CharString was counted before, offset by -1
            else:
                print("Unhandled event type: '%s'" % data['type'])
                exit(1)

            return output, extra_commands

        elif data.name == "event":
            output = bytearray()
            extra_commands = 0

            # if data['command'] == "Return" or data['command'] == "HistChar":
            if data['command'] != "Event" and data['command'] != "String":
                self.charcount += 1

            if data['command'] == "String":
                # Special case. I created  the string command to combine Char commands, so it must be handled separately
                event_contents = data.findAll("eventarg", type="CharString", recursive=False)

                for ec in event_contents:
                    _d, _e = self.write_type(ec)
                    output += _d
                    extra_commands += _e

            else:
                output += bytearray(struct.pack("<B", event_opcode_names_inv[data['command']]))

                event_contents = data.findAll(recursive=False)
                for ec in event_contents:
                    if data['command'] == "Event" and ec['type'] == "Str":
                        ec = BeautifulSoup(unicode(ec).replace('|', '\r\n').replace('[\\x01]', '\x01')).contents[0]

                    output += self.write_type(ec)[0]

            return output, extra_commands

        elif data.name == "eventblock":
            self.charcount = 0
            event_contents = data.findAll('event', recursive=False)
            variable_contents = data.findAll('furigana', recursive=False)

            has_furigana = int(data['hasfurigana'])

            output = bytearray()

            if FURIGANA_ENABLED or not has_furigana:
                output += bytearray(data['name'], encoding='shift-jis')

            furigana = []
            if FURIGANA_ENABLED == True and has_furigana:
                for vc in variable_contents:
                    furigana.append(self.write_type(vc))
            elif has_furigana:
                output += "TpWord102"
                variable_output = bytearray()
                variable_output += self.write_int_data(0xffffffff)
                variable_output += self.write_int_data(0xffffffff)
                variable_output += self.write_int_data(0)
                variable_output += self.write_byte_data(0)
                variable_output += self.write_byte_data(0)
                variable_output += self.write_int_data(0xffffffff)
                variable_output += self.write_string_data("")
                variable_output += self.write_string_data("")
                variable_output += self.write_int_data(0xffffffff)
                variable_output += self.write_int_data(0xffffffff)
                furigana.append(variable_output)

            event_content_data = bytearray()
            event_count = len(event_contents)

            e = 0
            for ec in event_contents:
                _d, _e = self.write_type(ec)
                event_content_data += _d
                event_count += _e
                e += _e

            self.charcount -= 1
            # print "%08x %02x %02x %02x %02x %02x" % (self.output.tell(), len(variable_contents), event_count, e, e + self.charcount, self.charcount)

            output += self.write_int_data(len(furigana))
            if len(furigana):
                # This number seems to be the number of times a certain furigana is used. The first entry is for when
                # there is no furigana.
                # TODO: Calculate this properly later.
                # variable_output = self.write_int_data(e) + variable_output

                for i in range(0, len(furigana)):
                    output += self.write_int_data(event_count) + furigana[i]

            output += self.write_int_data(event_count)
            output += event_content_data

            output = bytearray(struct.pack("<I", len(output))) + output
            self.write(output)

        elif data.name == "param":
            self.write_byte(int(data['type']))
            self.write_string(data['name'])

            param_contents = data.findAll(recursive=False)

            # print param_contents
            # print ""
            self.write_int(len(param_contents))

            if data['calc'] != "":
                #print "%s %02x" % (data['calc'],calc_funcs_inv[data['calc']])

                if data['calc'] in calc_funcs_inv:
                    self.write_byte(calc_funcs_inv[data['calc']])
                else:
                    print("Unknown calc function: '%s'" % data['calc'])
                    exit(-3)

            for param in param_contents:
                self.write_type(param, True)

        elif data.name == "array":
            array_contents = data.findAll(recursive=False)

            self.write_int(len(array_contents))

            for ac in array_contents:
                self.write_type(ac, True)

        else:
            print("Unhandled type: '%s'" % data.name)
            exit(-3)


def main():
    parser = argparse.ArgumentParser(description='Assemble disassembled LiveMaker 2/3 scripts into an LSB script file.')
    parser.add_argument('--input', required=True, dest='input_filename', help='Input XML file')
    parser.add_argument('--output', required=True, dest='output_filename', help='Output LSB file')
    args = parser.parse_args(sys.argv[1:])

    asm = Assembler(args.input_filename, args.output_filename)
    asm.assemble()
    # asm.write(args.output_filename)


if __name__ == "__main__":
    main()