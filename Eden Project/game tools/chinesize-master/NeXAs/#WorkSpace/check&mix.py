import os
import sys

PATH_OLD_JP = "OLD_JP\\"
PATH_OLD_CN = "OLD_CN\\"
PATH_RE_JP = "RE_JP\\"
PATH_RE_CN = "RE_CN\\"

def CleanNopLine(lines):
    ret = []
    for l in lines:
        if len(l) == 0:
            continue
        ret.append(l)
    return ret

for f in os.listdir(PATH_OLD_JP):
    print(f)
    
    fs=open(PATH_OLD_JP + f,"rb")
    OLD_JP = CleanNopLine(fs.read().decode('u16').split("\r\n"))
    fs.close()

    fs=open(PATH_OLD_CN + f,"rb")
    OLD_CN = CleanNopLine(fs.read().decode('u16').split("\r\n"))
    fs.close()

    szOfOLD_JP = len(OLD_JP)
    szOfOLD_CN = len(OLD_CN)

    if(szOfOLD_JP != szOfOLD_CN):
        print("OLD_JP:"+str(szOfOLD_JP))
        print("OLD_CN:"+str(szOfOLD_CN))
        continue

    try:fs=open(PATH_RE_JP + f,"rb")
    except:
        print("\t找不到该文件")
        continue
    RE_JP = fs.read().decode('u16').split("\r\n")
    fs.close()

    szOfRE_JP = len(RE_JP)

    i=0
    j=0
    for i in range(szOfRE_JP):
        for j in range(szOfOLD_JP):
            if RE_JP[i] == OLD_JP[j]:
                RE_JP[i] = OLD_CN[j]
    
    fs=open(PATH_RE_CN + f,"wb")
    fs.write("\r\n".join(RE_JP).encode('u16'))
    fs.close()
    #break
