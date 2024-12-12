#encoding=utf-8
#py2.7
import os
#from pdb import set_trace as int3

path1='S\\'
path2='ST\\'
path3='SF\\'

for f in os.listdir(path1):
    fs=open(path1+f,'rb')
    lines=fs.read().decode('932').replace('<BR>','\\n').split('\r\n')
    fs.close()
    ptext=[]
    tags=[]
    print(f)
    for li in lines:
        text=''
        cur=0
        print(len(li))
        while cur<len(li):
            print(cur)
            if li[cur]=='<':
                p=cur
                while li[p]=='<':
                    p=li.find('>',p)+1
                    if p==-1: print("err")
                    if p==len(li): break
                tags.append(li[cur:p])
                text+='@'
                cur=p
            else:
                p=li.find('<',cur)
                if p==-1:
                    p=len(li)
                text+=li[cur:p]
                cur=p
        ptext.append(text)

    textf=open(path2+f,'wb')
    textf.write('\r\n'.join(ptext).encode('utf-16'))
    textf.close()

    tagf=open(path3+f,'wb')
    tagf.write('\r\n'.join(tags).encode('utf-16'))
    tagf.close()

print ('Success!')
input()
