import os,sys,re,pdb

mix=[]
n=[]

def decodestr(mstr):
    try :
        mstr=mstr.encode('932').decode('gb18030')
    except:
        print(mstr)
        sys.exit(0)
    return mstr

def BuildBacklogDataJP(l):
    n.append(decodestr("//【あかね】@n"+l))
    n.append(decodestr("//【陽介】@n"+l))
    n.append(decodestr("//【母】@n"+l))

def BuildBacklogDataCN(l):
    mix.append(n[0])
    mix.append("【茜】@n"+l)
    mix.append(n[1])
    mix.append("【阳介】@n"+l)
    mix.append(n[2])
    mix.append("【母】@n"+l)
    n.clear()

def IsText(c):
    for i in range(len(c)):
        if c[i] > 0x7F:
            return True
    return False

def proc(lines):
    for l in lines:
        if len(l) == 0:
            continue
        elif l.startswith('☆'):
            l=l[10:]
            mix.append(decodestr('//'+l))
            if (l.startswith('@v') or l.find('「')!=-1 or l.find('」')!=-1):
                BuildBacklogDataJP(l)


        elif l.startswith('★'):
            l=l[10:]
            mix.append(l)
            if (l.startswith('@v') or l.find('「')!=-1 or l.find('」')!=-1):
                BuildBacklogDataCN(l)

path1='txt'
for f in os.listdir(path1):
    print(f)
    fs=open(os.path.join(path1,f),'rb')
    ls=fs.read().decode("u16").split('\r\n')
    proc(ls)
    fs.close()

fs=open("Script.txt","wb")
fs.write("\r\n".join(mix).encode('u8'))
fs.close()
