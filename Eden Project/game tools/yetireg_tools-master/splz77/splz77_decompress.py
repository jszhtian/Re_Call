#!/usr/bin/env python
# Original LZSS decompression code written by Treeki - http://jul.rustedlogic.net/thread.php?pid=413390#413390
# Adapted for use in PS2 Strawberry Panic

from struct import unpack

def splz77_decompress(f, exlen, inlen=500000000):
  dout = bytearray(b'\0' * exlen)
  offset = 0
  max = f.tell() + inlen

  while offset < exlen:
    if f.tell() >= max:
      break
    flags = ord(f.read(1))
    #print("+ %X %X" % (flags, f.tell()))

    for i in range(8):
      if f.tell() >= max:
        break
      #print("bit %d" % (flags & 0x80))
      if flags & 0x01:
        dout[offset] = ord(f.read(1))
        #print("copy @ %X %X" % (f.tell(),dout[offset]))
        offset += 1
      else:
        info = unpack("<H", f.read(2))[0]
        num = 3 + ((info&0x0F00)>>8)
        disp = ((info&0xF000)>>4) | (info&0xFF)
        ptr = offset - ((offset - 18 - disp) & 0xFFF)
        #print("LZ @ %X ctl=%04X num=%d disp=%04X wroffs=%d" % (f.tell(),info,num,disp,offset))
        for j in range(num):
          dout[offset] = dout[ptr] if ptr >=0 else 0
          offset += 1
          ptr += 1
          if offset >= exlen:
            break
        #debugging return dout

      flags >>= 1
      if offset >= exlen:
        break

  return dout
