import os,sys,re,pdb

fs=open('01 不運と幸運と恋占いのタロット.txt','rb')
ls=fs.read().decode('u16').split('\n')
i=0
nl=[]
for l in ls:
    if l.find('\0'):
        l=l.replace('\0','\r')
    nl.append(l+'\r')
        

fs=open('01 不運と幸運と恋占いのタロット.txt.txt','wb')
fs.write('\n'.join(nl).encode('u16'))
fs.close()
