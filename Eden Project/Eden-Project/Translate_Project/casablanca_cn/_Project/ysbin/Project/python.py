import os,sys

unpack=False
path1='raw'
path2='comp'
path3='out'

def proc(lines):
    ret=[]
    fmt = lambda i,s:'☆%08d☆%s\n★%08d★%s\n'%(i,s,i,s)
    i=1
    for l in lines:
        if len(l)!=0:
            ret.append(fmt(i,l))
            i+=1
    return ret

def proc2(lines):
    ret=[]
    for l in lines:
        l=l.strip()
        if l.startswith('★'):
            l=l.replace("【カナ】","【加奈】")
            l=l.replace("【イツキ】","【一木】")
            l=l.replace("【一木キ】","【一木】")
            l=l.replace("【为了】","【未来】")
            l=l.replace("【一木】啊","【一木】")
            l=l.replace("【加奈 】","【加奈】")
            l=l.replace("【ユリ】","【由莉】")

            """
            l=l.replace("【由莉】「","亂儐儕亃乽")
            l=l.replace("【加奈】「","亂僇僫亃乽")
            l=l.replace("【未来】「","亂儈僋亃乽")
            l=l.replace("【一木】「","亂僀僣僉亃乽")
            l=l.replace("【女学生Ａ】「","亂彈巕妛惗俙亃乽")
            l=l.replace("【？？？】「","亂丠丠丠亃乽")
            l=l.replace("【男学生Ａ】「","亂抝巕妛惗俙亃乽")
            l=l.replace("【一木＆由莉】「","亂僀僣僉仌儐儕亃乽")
            l=l.replace("【鹿田爷爷】「","亂幁揷栮亃乽")
            l=l.replace("【后辈女生Ｂ】「","亂屻攜彈巕俛亃乽")
            l=l.replace("【后辈女生Ｃ】「","亂屻攜彈巕俠亃乽")
            l=l.replace("【男学生Ｂ】「","亂抝巕妛惗俛亃乽")
            l=l.replace("【后辈女生Ａ】「","亂愭攜彈巕俙亃乽")
            l=l.replace("【杂用工】「","亂梡柋堳亃乽")
            l=l.replace("【女学生Ｄ】「","亂彈巕妛惗俢亃乽")
            l=l.replace("【老师】「","亂嫵巘亃乽")
            l=l.replace("【学生】「","亂妛惗亃乽")
            l=l.replace("【后辈女生Ｄ】「","亂屻攜彈巕俢亃乽")
            l=l.replace("【数学老师】「","亂悢妛嫵巘亃乽")
            l=l.replace("【体育老师】「","亂懱堢嫵巘亃乽")
            l=l.replace("【女学生Ｅ】「","亂彈巕妛惗俤亃乽")
            l=l.replace("【保健老师】「","亂梴岇嫵桜亃乽")
            l=l.replace("【女学生Ｃ】「","亂彈巕妛惗俠亃乽")
            l=l.replace("【男学生Ｃ】「","亂抝巕妛惗俠亃乽")
            l=l.replace("【老师Ａ】「","亂嫵巘俙亃乽")
            l=l.replace("【女学生Ｇ】「","亂彈巕妛惗俧亃乽")
            l=l.replace("【女学生Ｈ】「","亂彈巕妛惗俫亃乽")
            l=l.replace("【椎名母亲】「","亂捙柤曣亃乽")
            l=l.replace("【一木＆加奈】「","亂僀僣僉仌僇僫亃乽")
            l=l.replace("【男学生Ｇ】「","亂抝巕妛惗俧亃乽")
            l=l.replace("【后辈女生Ｇ】「","亂屻攜彈巕俧亃乽")
            """
            l=l.replace("由莉","百合")
            l=l.replace("♪","")

            ret.append(l[10:])
    return ret
if (unpack==True):
    for f in os.listdir(path1):
        print(f)
        fs=open(os.path.join(path1,f),'rb')
        ls=fs.read().decode("utf-8-sig").split('\r\n')
        newls=proc(ls)
        fs=open(os.path.join(path2,f),'wb')
        fs.write("\n".join(newls).encode('utf16'))
        fs.close()
else:
    for f in os.listdir(path2):
        print(f)
        fs=open(os.path.join(path2,f),'rb')
        ls=fs.read().decode("u16").split('\n')
        newls=proc2(ls)
        fs=open(os.path.join(path3,f),'wb')
        fs.write("\r\n".join(newls).encode('utf-8-sig'))
        fs.close()
