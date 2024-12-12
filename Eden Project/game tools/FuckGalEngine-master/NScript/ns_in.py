import os
import re
import pdb
import sys
from fuzzywuzzy import fuzz


def get_newtextV2(rawtext):
    ptr1 = 0
    pos = 0
    storageRatio = -1
    while ptr1 < len(OldList):
        rat = fuzz.ratio(OldList[ptr1], rawtext)
        if rat >= storageRatio and rat >= 75:
            storageRatio = rat
            pos = ptr1
            break
        ptr1 += 1
    if storageRatio != -1:
        # print(rawtext)
        return NewList[pos]
    else:
        return None

def getStr(oldStr):
    try:
        ptr1 = 0
        while ptr1 < len(OldList):
            #print(OldList[ptr1])
            #print(oldStr)
            #if ptr1==10:sys.exit(0)
            if (OldList[ptr1] == oldStr or re.search(oldStr,OldList[ptr1])!=None):
                OldList[ptr1] = "Used!"
                return NewList[ptr1]
            else:
                ptr1 += 1
        TryGetText = get_newtextV2(oldStr)
        if(TryGetText != None):
            return TryGetText
        print('Miss Match:'+oldStr)
        return oldStr
    except Exception as e:
        print(e)
        print(oldStr)
        print(OldList)
        print('----------------------------------------------')
        print(NewList)
        sys.exit(0)


def ReplaceText(OldStr):
    rebuildStr = OldStr
    newText = getStr(OldStr)
    rebuildStr = rebuildStr.replace(OldStr, newText, 1)
    return rebuildStr

def IsJis(l):
    i=0
    for i in range(len(l)):
        if ord(l[i]) > 0x80:
            return True
    return False

def proc(lines):
    ret=[]
    i=0
    for l in lines:
        if len(l) == 0:
            continue
        elif IsJis(l) == True:
            if l.find(';') != -1:
                tsl=l[:l.find(';')]
                if IsJis(tsl) == True:
                    NewText = ReplaceText(tsl)
                    ret.append(l.replace(tsl,NewText))
                else:
                    ret.append(l)
            else:
                ret.append(ReplaceText(l))
        else:
            ret.append(l)
        print(i)
        i+=1
    return ret

def estarray(lines):
    for l in lines:
        l = l.strip()
        if len(l) == 0:
            continue
        elif l.startswith('○'):
            if l[6] != '○':
                print('error at jp line!!!')
                print(l)
                sys.exit(-1)
            tstr = l[7:]
            OldList.append(tstr)
        elif l.startswith('●'):
            if l[6] != '●':
                print('error at cn line!!!')
                print(l)
                sys.exit(-1)
            tstr = l[7:]
            NewList.append(tstr)
        else:
            print(l)
            sys.exit(-1)

OldList=[]
NewList=[]

fs=open('nscript.out','rb')
ls=fs.read().decode('932').split('\n')
fs.close()

fs=open('nscript.txt','rb')
ls2=fs.read().decode('u16').split('\n')
fs.close()
estarray(ls2)

if(len(OldList) != len(NewList)):
    print(len(OldList))
    print(len(NewList))
    sys.exit(-1)

ret = proc(ls)

fs=open('nscript.dat','wb')
fs.write('\n'.join(ret).encode('gb18030'))
fs.close()
