#! /usr/bin/env python
# -*- coding: utf-8 -*-

from codecs import open
import os
import struct

from lib.orm import DBManager

CHARACTER_HEADER = '010000000032000000'.decode('hex')
LINE_DELIMITER = '060c00000001'.decode('hex') + 'MESOFF'
DELIMITER = '\x0d\x0a'
COMMAND_CODE = '\x06'
FUNCTION_CODE = '\x0a'
FOOTER_CODE = '\x02'

IGNORED_COMMANDS = ('CREATECG', '\x01CREATECG', '\x01DELETECG', '\x01CHANGECG',
                    '\x01PLAYSND', '\x01STOPSND',
                    '\x01NAMELABEL',
                    '\x01QUAKE', '\x01', 'MESEND')
SCRIPT_BLOCK_SIZE = 6


class ScriptBlock(list):
    """Script line abstraction. Has a handy dandy to_hex() function."""

    def __init__(self, *args, **kwargs):
        location = kwargs.pop('location')
        super(ScriptBlock, self).__init__(*args, **kwargs)
        self.location = location

    def __unicode__(self):
        return u''.join(unicode(c) for c in self)

    def render(self):
        return u'<{0}>\n{1}'.format(self.location, u''.join(c.render() for c in self))

    def to_hex(self, msg_header=False, msg_footer=False):
        """Converts script characters to bytes"""

        out = bytearray()
        if msg_header:
            pass

        # iterate over characters
        for char in self:
            out += char.to_hex()

        if msg_footer:
            pass

        return out


class LineElement(object):
    """Abstract interfacey thing"""

    def __unicode__(self):
        raise NotImplementedError()

    def render(self):
        return unicode(self)

    def to_hex(self):
        raise NotImplementedError()


class LineNameLabel(LineElement):
    """Line speaker."""

    def __init__(self, name):
        if isinstance(name, str):
            name = name.decode('cp932')

        self.name = name

    def __unicode__(self):
        return self.name

    def render(self):
        return u'\n【{0}】 '.format(unicode(self))

    def to_hex(self):
        buf = bytearray()
        buf += COMMAND_CODE
        buf += struct.pack('L', (len(self.name),))
        return buf


class ScriptCharacter(LineElement):
    """Script character abstraction. """

    def __init__(self, character):
        # convert cp932 bytes to unicode
        if isinstance(character, str):
            try:
                character = character.decode('cp932')
            except UnicodeDecodeError:
                raise Exception('Error decoding character ' + repr(character))
        self.character = character

    def __unicode__(self):
        return self.character

    def to_hex(self):
        """Converts script line to bytes"""

        # initialize with header
        out = bytearray()
        out += CHARACTER_HEADER

        # plop character into place
        char = self.character.encode('cp932')
        out += char

        # pad the character block with null
        pads = SCRIPT_BLOCK_SIZE - len(char)
        if pads > 0:
            out += '\x00' * pads
        else:
            raise IndexError('Character too large')

        return out


class ScriptLinebreak(ScriptCharacter):
    """ Script linebreak abstraction """

    def to_hex(self):
        return b'\x03\x01'


def decode_block(byte_block):
    # newline
    if byte_block.startswith(b'\x03\x01'):
        return ScriptLinebreak('\n'), 2

    # sjis character - why is this MSB?
    elif byte_block.startswith(CHARACTER_HEADER):
        character = byte_block[9:14][::-1].strip('\x00')
        return ScriptCharacter(character), 15

    # namelabel, cg/voice, and other inlines
    elif byte_block.startswith(COMMAND_CODE):
        length = struct.unpack('L', byte_block[1:5])[0]
        relevant = byte_block[5:5 + length]
        end = 5 + length

        # namelabel
        if relevant.startswith('NAMELABEL'):
            name = relevant.split(DELIMITER)[1]
            return LineNameLabel(name), end

        elif relevant.startswith(IGNORED_COMMANDS):
            return None, end

        raise ValueError('Unknown command:' + repr(relevant[:10]) + '...')

    # skippable? function code
    elif byte_block.startswith(FUNCTION_CODE):
        length = struct.unpack('LLLL', byte_block[1:1 + 4 * 4])[3]
        return None, 17 + length

    # skippable footer code
    elif byte_block.startswith(FOOTER_CODE):
        return None, len(byte_block)

    # unknown padding
    if byte_block.startswith(b'\x03\x00'):
        return None, 2

    raise Exception('Unparsable block:' + repr(byte_block[:10]))


def parse_lsb(script):
    lines = []

    # find lines
    line_start = script.find(CHARACTER_HEADER)
    abs_offset = line_start
    while line_start > 0:

        try:
            line_end = script[line_start:].index(LINE_DELIMITER) + line_start
        # no end in sight, end of file
        except ValueError:
            line_end = len(script)

        block = script[line_start:line_end]
        line = ScriptBlock(location=line_start)

        # find characters
        char_count = 0
        while block:
            # decode the character
            element, offset = decode_block(block)
            abs_offset += offset
            # append if there's anything
            if element:
                line.append(element)

            # advance
            block = block[offset:]
            char_count += 1

        lines.append(line)

        # advance script and find next starting point
        script = script[line_end:]
        line_start = script.find(CHARACTER_HEADER)

    return lines


def write_to_tl_file(lines, outfile):
    """ Exercises more delicate control over rendering. """
    for line in lines:

        # keep a line buffer so that we can write the line twice -- the first commented
        line_buffer = u''
        outfile.write(u'<{0}>\n'.format(line.location))
        for element in line:
            if isinstance(element, ScriptLinebreak):

                # write once commented, and then again uncommented
                if line_buffer:
                    outfile.write('// ')
                    outfile.write(line_buffer)
                    outfile.write(element.render())
                    outfile.write(line_buffer)
                    outfile.write(element.render() * 2)
                    line_buffer = u''

                # script-bound \n == \n\n
                else:
                    outfile.write(element.render() * 2)
            elif isinstance(element, LineNameLabel):
                # remove newline
                line_buffer += element.render().lstrip('\n')
            else:
                line_buffer += element.render()

        if line_buffer:
            outfile.write(line_buffer)
        # write the rather large buffer
        outfile.flush()


def store_lines(lines, filename, orm):
    """ Commits lines and speakers. """
    assert isinstance(orm, DBManager)
    for line in lines:
        # keep a line buffer so that we can write the line twice -- the first commented
        line_buffer = u''
        name_id = None
        for element in line:
            if isinstance(element, ScriptLinebreak):
                # commit to sql
                orm.add_jp_line(line.location, filename, line_buffer, speaker_id=name_id, commit=False)
                line_buffer = u''
                name_id = None

            elif isinstance(element, LineNameLabel):
                # remove newline
                name = element.render().strip()
                if orm.speakers.get(name, None):
                    name_id = orm.speakers[name]
                else:
                    name_id = orm.get_or_add_speaker('jp', name)
            else:
                line_buffer += element.render()

        # write anything trailing
        if line_buffer:
            orm.add_jp_line(line.location, filename, line_buffer, name_id, commit=False)

    orm.commit()


def test():
    script = open('out\\00000623.lsb', 'rb').read()
    lines = parse_lsb(script)
    out = open('test.txt', 'w', encoding='utf8')
    out.write(u'\n'.join(line.render() for line in lines))


def main():
    orm = DBManager()
    for fn in os.listdir('out'):
        # all text LSB scripts
        if fn.startswith('00') and fn.endswith('.lsb'):
            print 'Processing', fn
            script = open(os.path.join('out', fn), 'rb').read()
            lines = parse_lsb(script)

            # out_fn = fn.replace('.lsb', '.txt')
            # out = open(os.path.join('scripts', out_fn), 'w', encoding='utf8')
            # write_to_tl_file(lines, out)
            store_lines(lines, fn, orm)


if __name__ == '__main__':
    main()