# -*- coding: utf-8 -*-
from __future__ import print_function
from __future__ import print_function
import ctypes
import struct


class Deobfuscator(object):
    # you didn't see this one coming did you
    # thanks to asmodean for figuring this xor key out!

    """Tool for deobfuscating the file table."""

    def __init__(self):
        self.ulong_key = ctypes.c_ulong(0)
        self.filename_key = ctypes.c_ulong(0)
        self.filename_key_char_ptr = ctypes.cast(ctypes.pointer(self.filename_key), ctypes.POINTER(ctypes.c_ubyte))

    def deobfuscate_filename(self, byte_name):
        """
        Obfuscates the filename with a running XOR key.

        :param byte_name: Tuple of ordinals
        :return: unicode
        """
        deobfuscated = bytearray()
        # mean, obscure XOR scheme
        for ordinal in byte_name:
            ordinal = ctypes.c_ubyte(ordinal)
            self.filename_key.value += self.filename_key.value * 4
            self.filename_key.value += 0x75D6EE39

            deobfuscated += struct.pack('B', ordinal.value ^ self.filename_key_char_ptr.contents.value)

        # convert cp932 byte array to delicious unicode
        try:
            return deobfuscated.decode('cp932')
        except UnicodeEncodeError:
            print('CAUTION: could not encode', repr(deobfuscated))

    def deobfuscate_ulong(self, ulong_num):
        """Uses an incrementing XOR key."""
        # mean, obscure XOR scheme
        ulong_num = ctypes.c_ulong(ulong_num)
        self.ulong_key.value += self.ulong_key.value * 4
        self.ulong_key.value += 0x75D6EE39

        return ulong_num.value ^ self.ulong_key.value


class Obfuscator(Deobfuscator):
    """Enacting XOR magic."""

    def obfuscate_filename(self, unicode_name):
        """
        Obfuscates the filename with a running XOR key.

        :param unicode_name: Unicode filename
        :return:
        """

        byte_name = unicode_name.encode('cp932')
        obfuscated = bytearray()
        # mean, obscure XOR scheme
        for bite in byte_name:
            ordinal = ord(bite)
            ordinal = ctypes.c_ubyte(ordinal)
            self.filename_key.value += self.filename_key.value * 4
            self.filename_key.value += 0x75D6EE39

            obfuscated += struct.pack('B', ordinal.value ^ self.filename_key_char_ptr.contents.value)

        # convert cp932 byte array
        try:
            return obfuscated
        except UnicodeEncodeError:
            print('CAUTION: could not encode', repr(obfuscated))

    obfuscate_ulong = Deobfuscator.deobfuscate_ulong