#!/usr/bin/env python  
# coding:utf-8  
import os
import re
import codecs
import pdb
from fuzzywuzzy import fuzz

def proc(lines,replist):
	newls=[]
	idx=0
	while idx+2 <len(lines):
		if lines[idx].startswith('<'):
			newls.append(lines[idx])
			newtext=get_newtext(lines[idx][14:],replist)
			if newtext is not None:
				if len(newtext)!=0:
					newls.append(lines[idx+1]+newtext)
				else:
					newls.append(lines[idx+1]+lines[idx][14:])
			else:
				newls.append(lines[idx+1]+lines[idx][14:])
			newls.append("")
			idx+=3
		else:
			newls.append(lines[idx])
			idx+=1		

	return newls


def parse_texts_file(lines):
	newtexts = []
	ptr1=0
	while ptr1<len(lines):
		if ptr1+2<len(lines) and lines[ptr1].startswith('<'):
			pos=lines[ptr1].find(">")+1
			rawtext=lines[ptr1][pos+2:]
			newtext=lines[ptr1+1][pos:]
			newtexts.append(rawtext)
			newtexts.append(newtext)
			ptr1+=3
		else:
			ptr1+=1
			continue
	return newtexts

def get_newtext(txt,list):
	ptr1=0
	while ptr1<len(list):
		if list[ptr1]==txt:
			list[ptr1]="!"+txt
			return list[ptr1+1]
		else:
			ptr1+=2
	ptr1=0
	while ptr1<len(list):
		if list[ptr1]=="!"+txt:
			return list[ptr1+1]
		else:
			ptr1+=2
	ptr1=0
	while ptr1<len(list):
		if fuzz.ratio(list[ptr1],txt)>=75:
			return list[ptr1+1]
		else:
			ptr1+=2
	return None
	
	
path1='raw'
path2='old'
path3='out'
for f in os.listdir(path2):
	print(f)
	trans=[]
	fs1=open(os.path.join(path1,f),'rb')
	ls=fs1.read().decode('utf-8').split('\r\n')
	fs2=open(os.path.join(path2,f),'rb')
	rs=fs2.read().decode('utf-8-sig').split('\r\n')
	replist=parse_texts_file(rs)
	newls=proc(ls,replist)
	fs3=open(os.path.join(path3,f),'wb')	
	fs3.write('\r\n'.join(newls).encode('utf-8'))
	fs1.close()
	fs2.close()
	fs3.close()
