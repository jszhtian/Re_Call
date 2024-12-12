# encoding=utf-8
import os
import re
import pdb
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
            if OldList[ptr1] == oldStr:
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
        print(OldList)
        print('----------------------------------------------')
        print(NewList)


def ReplaceText(OldStr):
    rebuildStr = OldStr
    newText = getStr(OldStr)
    rebuildStr = rebuildStr.replace(OldStr, newText, 1)
    return rebuildStr


def proc(lines):
    newl = []
    i = 0
    for l in lines:
        l = l.strip()
        if len(l) == 0:
            newl.append(l)
            continue
        elif l.startswith("@"):
            newl.append(l)
            continue
        elif l.startswith(';'):
            newl.append(l)
            continue
        elif l.startswith(':'):
            newl.append(l)
            continue
        elif l.startswith('.'):
            newl.append(l)
            continue
        elif l.startswith(' '):
            newl.append(l)
            continue
        elif l.startswith('\t'):
            newl.append(l)
            continue
        elif l.startswith('\n'):
            newl.append(l)
            continue
        else:
            NewText = ReplaceText(l)
            newl.append(NewText)
    return newl


def estarray(lines):
    for l in lines:
        l = l.strip()
        if len(l) == 0:
            continue
        elif l.startswith('<'):
            if (l.find('//') != -1):
                #tstr = l[l.find('//')+2:]
                #OldList.append(tstr)
                continue
            else:
                tstr = l[l.find('>')+1:]
                NewList.append(tstr)

def estarray_V2(lines):
    for l in lines:
        l = l.strip()
        if len(l) == 0:
            continue
        elif l.startswith('<'):
            if (l.find('//') != -1):
                tstr = l[l.find('//')+2:]
                OldList.append(tstr)
            else:
                continue

path1 = 'RAW_JP'
path2 = 'CHS'
path3 = 'OUT'
path4 = 'JP'
for f in os.listdir(path2):
    OldList = []
    NewList = []
    print(f)
    fs = open(os.path.join(path1, f.replace('.txt', '.txt')), 'rb')
    ls = fs.read().decode('utf16').split('\n')
    fs2 = open(os.path.join(path2, f.replace('.txt', '.txt')), 'rb')
    ls2 = fs2.read().decode('utf-8').split('\n')
    fs4 = open(os.path.join(path4, f.replace('.txt', '.txt')), 'rb')
    ls4 = fs4.read().decode('utf-8').split('\n')
    estarray(ls2)
    estarray_V2(ls4)
    #print(OldList)
    #print(NewList)
    newls = proc(ls)
    fs3 = open(os.path.join(path3, f.replace('.txt', '.txt')), 'wb')
    fs3.write('\n'.join(newls).encode('utf8'))
    fs.close()
    fs2.close()
    fs3.close()
