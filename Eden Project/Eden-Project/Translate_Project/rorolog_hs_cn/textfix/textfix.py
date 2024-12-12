import os
import sys
import re
import pdb

def proc(lines):
	newl=[]
	i=1
	for l in lines:
		l=l.strip()
		if len(l)==0:
			newl.append(l)
		elif l.startswith('●'):
			#tstr=l[l.find('●')+10:]
			sig1=0
			sig2=0
			sig3=0
			for i in range(len(l)):
				if l.find('芦宮')!=-1:
					l=l.replace('芦宮','芦之宮')
					#print(l)
					if l.find('芦宮')==-1:sig1=1
				elif l.find('学園')!=-1:
					l=l.replace('学園','学园')
					#print(l)
					if l.find('学園')==-1:sig3=1
				elif (l.find('<') and l.find('>')):
					tstr=l[l.find('<r'):l.find('>')+1]
					if len(tstr)!=0:log.write(tstr+'\n')
					#tstr=l[l.find('<r')+2:l.find('>')]
					l=l.replace(tstr,'')
					l=l.replace('</r>','')
					if (l.find('<')==-1 and l.find('>')==-1):sig2=1
				else:
					sig1=1
					sig2=1
					sig3=1

				if (sig1==1 and sig2==1 and sig3==1):break
			newl.append(l)
		elif l.startswith('○'):
			newl.append(l)
		else:
			log.write('Find error words:'+l+'\n')
			newl.append(l)
			
	#print (fs)
	#print (i-1)
	return newl

path1='RAW'
path2='OUT'
log=open('log.txt','w',encoding='utf8')
for f in os.listdir(path1):
    print(f)
    log.write('File:'+f+'\n')
    fs=open(os.path.join(path1,f),'rb')
    ls=fs.read().decode("u16").split('\n')
    newls=proc(ls)
    fs=open(os.path.join(path2,f.replace('.txt','.txt')),'wb')
    fs.write("\n".join(newls).encode('u16'))
    fs.close()
    log.write('-------------------------------------------------------------\n')
log.close()
A=input('按回车键退出')
