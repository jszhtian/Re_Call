#encoding=utf-8
import os
import re
import pdb

def proc(lines):
	newl=[]
	i=0
	for l in lines:
		l=l.strip()
		if l.startswith('<EventArg Type="CharString"'):
			if(l.find('>') != -2):
				n=l[l.find('>')+1:]
				t=n.strip('</EventArg>')
			tmp='【'+str(i)+'】'+'//'+t
			newl.append(tmp)
			tmp2='【'+str(i)+'】'+t
			newl.append(tmp2)
			#newl.append(l)
			newl.append('')
			i+=1
		else:
			continue
	print (fs)
	print (i)
	return newl

path1='RAW'
path2='CHS'
for f in os.listdir(path1):
    fs=open(os.path.join(path1,f),'rb')
    ls=fs.read().decode("utf16").split('\n')
    newls=proc(ls)
    fs=open(os.path.join(path2,f.replace('.txt','.txt')),'wb')
    fs.write("\n".join(newls).encode('utf16'))
    fs.close()
