import os,sys,re,pdb

def IsJis(s):
    i=0
    for i in range(len(s)):
        if(ord(s[i])>0x80):
            return True
        if(s[i]==';'):
            return False
    return False

def proc(lines):
    ret = []
    fmt = lambda s,i:"○%08d○%s\n●%08d●%s\n"%(i,s,i,s)
    i=0
    for l in lines:
        l=l.strip('\n')
        if len(l)==0:
            continue
        elif (l.startswith(';') or
              IsJis(l)==False):
              continue
        #elif (l.find('/')!=-1 or l[-1]=='\\'):
        else:
            i+=1
            ret.append(fmt(l,i))
    return ret
fp = open('de_nscript.bin',"rb")
ls = proc(fp.read().decode('sjis').split('\n'))
fp = open('txt.txt','wb')
fp.write('\n'.join(ls).encode('u16'))
fp.close()
