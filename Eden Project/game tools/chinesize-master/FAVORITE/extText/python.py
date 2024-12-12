import os,sys

unpack=True
path1='txt1'
path2='txt2'
path3='out'

cnter = 0
def proc(lines):
    ret=[]
    fmt = lambda i,s:'☆%08d☆%s\r\n★%08d★%s\r\n'%(i,s,i,s)
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
            ret.append(l[10:])
    return ret
if (unpack==True):
    for f in os.listdir(path1):
        print(f)
        fs=open(os.path.join(path1,f),'rb')
        ls=fs.read().decode("u16").split('\r\n')
        newls=proc(ls)
        fs=open(os.path.join(path2,f),'wb')
        fs.write("\r\n".join(newls).encode('u16'))
        fs.close()
else:
    for f in os.listdir(path2):
        print(f)
        fs=open(os.path.join(path2,f),'rb')
        ls=fs.read().decode("u16").split('\r\n')
        newls=proc2(ls)
        fs=open(os.path.join(path3,f),'wb')
        fs.write("\r\n".join(newls).encode('u16'))
        fs.close()

#print(cnter)
