#py3.2

import zlib
import bytefile
import os

path1='Script_Re\\'
path2='Txt_Re_CN\\'
path3='out\\'

def packTxtBlock(lines):
    block=bytearray()
    for l in lines:
        block+=l+b'\x00'
    return block

def packTxt(stm,lines):
    stm.seek(4)
    inst_count=stm.readu32()
    stm.seek(inst_count*8+8)
    count=stm.readu32()
    pos1=stm.tell()
    for i in range(count):
        stm.readstr()
    pos2=stm.tell()
    newstm=bytearray()
    newstm+=stm[0:pos1]
    newstm+=packTxtBlock(lines)
    newstm+=stm[pos2:]
    return newstm

for f in os.listdir(path1):
    print(f)
    fs=open(path1+f,'rb')
    stm=bytefile.ByteIO(fs.read())
    fs.close()
    if os.path.exists(path2+f.replace('.bin','.txt')):
        fs=open(path2+f.replace('.bin','.txt'),'rb')
        lines=fs.read().decode('U16').encode('gb18030','replace').split(b'\r\n')
        fs.close()
        stm=packTxt(stm,lines)
    fs=open(path3+f.replace('.bin','.bin'),'wb')
    fs.write(stm[0:])
    fs.close()
