import json
import codecs
import sys
dict1={}
def BuildTBL(lines):
    for line in lines:
        if len(line)==0:
            break
        cp932num=int(line[0:4],16)
        gbk=line[5:]
        #print(gbk)
        sjis=cp932num.to_bytes(2,byteorder='big').decode('932')
        #print (gbk+":"+sjis)
        dict1.update({gbk:sjis})

def getMapedChar(ch):
    newch=''
    try:
        newch=dict1[ch]
    except:
        newch=ch
    return newch

def Proc(lines):
    out=[]
    for line in lines:
        newline=''
        for ch in line:
            newline+=getMapedChar(ch)
        out.append(newline)
    return out

FileName="0_all_000.txt"
fs1=open(FileName, 'rb')
ls=fs1.read().decode('u16').split('\r\n')
fs2=open('RU.TBL', 'rb')
ls2=fs2.read().decode('u16').split('\r\n')
BuildTBL(ls2)
out=Proc(ls)
fs3=open(FileName+".out", 'wb')
fs3.write('\r\n'.join(out).encode('932'))
fs1.close()
fs2.close()
fs3.close()
