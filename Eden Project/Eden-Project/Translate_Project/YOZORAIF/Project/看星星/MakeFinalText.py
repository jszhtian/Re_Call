import os,sys,re,pdb

def rep(lines):
    ret=[]
    for l in lines:
        if(len(l)==0):
            continue
        else:
            ret.append(l)
    return ret

mix=[]
path1='utf16'
for f in os.listdir(path1):
    print(f)
    fs1=open(os.path.join(path1,f),'rb')
    ls=rep(fs1.read().decode('u16').split('\n'))
    fs1.close()
    for l in ls:
        mix.append(l)

fs=open('Script.txt',"wb")
i=0
for i in range(len(mix)):
    feff=False
    if mix[i].find('\ufeff')!=-1:
        mix[i]=mix[i].replace('\ufeff','')
    if mix[i].find('//')!=-1:
        fs.write((mix[i]+'\n').encode('u8'))
    else:
        fs.write((mix[i]+'\n').encode('u8'))
fs.close()
