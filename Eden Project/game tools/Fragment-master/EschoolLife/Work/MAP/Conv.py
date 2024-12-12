#encoding=utf-8
import os
import re
import codecs
import pdb

def proc(lines,lines2):
	newl=[]
	idx=0
	offset=0
	while idx<len(lines):
		try:
			#print(idx)
			#print(idx+offset+1)
			#print(lines[idx+offset])
			if len(lines[idx])==0:
				newl.append(lines[idx])
				break
			if lines[idx][14]=="<" and lines[idx]!=lines2[idx+offset]:
				newl.append(lines[idx])
				newl.append(lines[idx+1])
				newl.append(lines[idx+2])
				idx+=3
				offset-=3
				continue
			if len(lines2[idx+offset+1])==12:
				newl.append(lines[idx])
				newl.append(lines[idx+1]+lines[idx][14:])
				newl.append(lines[idx+2])
				idx+=3
				continue
			elif lines2[idx+offset+1][12]=='/' and lines2[idx+offset+1][13]=='/':
				newl.append(lines[idx])
				newl.append(lines[idx+1][0:13]+lines2[idx+offset+1][14:])
				newl.append(lines[idx+2])
				idx+=3
				continue
			newl.append(lines[idx])
			newl.append(lines[idx+1][0:13]+lines2[idx+offset+1][12:])
			newl.append(lines[idx+2])
			idx+=3
		except Exception as er:
			print(er)
			print(idx)
			print(lines[idx+offset+1])
			print(lines2[idx+offset+1])
			exit(-1)
	return newl

def	estarray(lines):
	array=[]
	for l in lines:
		l=l.strip('\n')
		array.append(l)
	return array


path1='raw'
path2='chs'
path3='out'
for f in os.listdir(path2):
	print("Process:"+f)
	fs1=open(os.path.join(path1,f),'rb')
	ls=fs1.read().decode('utf-8').split('\r\n')
	fs2=open(os.path.join(path2,f),'rb')
	rs=fs2.read().decode('utf-8').split('\r\n')
	trans=estarray(ls)
	trans2=estarray(rs)
	newls=proc(trans,trans2)
	fs3=open(os.path.join(path3,f),'wb')	
	fs3.write('\r\n'.join(newls).encode('utf-8'))
	fs1.close()
	fs2.close()
	fs3.close()
