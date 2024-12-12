#py3.2

import os
import sys
import bytefile

path1=r'Script_old_cn\\'
path2=r'Txt_old_cn\\'

def exTxt(stm):
    """
    version = stm.readu32()
    if version != 0:
        print (hex(version))
        print("unsupport version.")
        sys.exit(0)
        #return ['\r\n']
    """
    inst_count=stm.readu32()
    stm.seek(inst_count*8+4)
    count=stm.readu32()
    txts=[]
    for i in range(count):
        txts.append(stm.readstr().replace(b'\n',b'\\n'))
    return txts

for f in os.listdir(path1):
    print(f)
    fs=open(path1+f,'rb')
    stm=bytefile.ByteIO(fs.read())
    fs.close()
    lines=exTxt(stm)
    fs=open(path2+f.replace('.bin','.txt'),'wb')
    fs.write(b'\r\n'.join(lines).decode('gb18030').encode('U16'))
    fs.close()
