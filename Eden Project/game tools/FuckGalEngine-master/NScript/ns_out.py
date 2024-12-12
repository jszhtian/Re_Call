import os,sys

def fmt(string, count):
    res = "○%05d○%s\n●%05d●%s\n\n"%(count, string,count, string)
    return res

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
                l=l[:l.find(';')]
                if IsJis(l) == True:
                    ret.append(fmt(l,i))
                    i+=1
            else:
                ret.append(fmt(l,i))
                i+=1
    return ret

fs=open('nscript.out','rb')
ls=fs.read().decode('932').split('\n')
fs.close()
ret = proc(ls)
fs=open('nscript.txt','wb')
fs.write('\n'.join(ret).encode('u16'))
fs.close()