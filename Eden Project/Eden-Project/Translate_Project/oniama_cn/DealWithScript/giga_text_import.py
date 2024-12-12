# -*- coding:utf-8 -*-

import struct
import os
import fnmatch
import pdb

def walk(adr):
	mylist=[]
	for root,dirs,files in os.walk(adr):
		for name in files:
			if name[-4:] != '.bin':
				continue
			adrlist=os.path.join(root, name)
			mylist.append(adrlist)
	return mylist

def byte2int(byte):
	long_tuple=struct.unpack('L',byte)
	long = long_tuple[0]
	return long

def dumpstr(src):
	bstr = b''
	c = src.read(1)
	while c != b'\x00':
		bstr += c
		c = src.read(1)
	return bstr.decode('932')

def dumptxt(src, offset, count):
	src.seek(offset)
	str_list = []
	for i in range(0, count):
		str_list.append(dumpstr(src))
	return str_list

def main():
	f_lst = walk('txt')
	tbl = open('oniama.tbl','r',encoding='utf16')
	dicts = {}
	for rows in tbl:
		row = rows.rstrip('\r\n').split('=')
		if len(row) == 3:
			row[1] = '='
		dicts[row[1]]=int(row[0],16)
	for fn in f_lst:
		print(fn)
		src = open(fn, 'rb')
		dstname = fn[:-4] + '.txt'
		txt = open(dstname, 'r', encoding='utf16')
		filesize=os.path.getsize(fn)
		src.seek(4)
		entry_count = byte2int(src.read(4))
		str_offset = (entry_count << 1) * 4 + 8
		src.seek(0)
		data=src.read(str_offset+5)
		dst = open(fn[:-4]+'.bin.out','wb')
		dst.write(data)
		for rows in txt:
			if rows[0] != '★':
				continue
			row = rows[10:]
			row=row.strip('\r\n')
			for ch in row:
				try:
					if dicts[ch] > 0xFF:
						dst.write(struct.pack('>H',dicts[ch]))
					else:
						dst.write(struct.pack('B',dicts[ch]))
				except Exception as inst:
					print(fn)
					print(row)
					inte()
			dst.write(struct.pack('B',0))
		src.seek(str_offset)
		str_count = byte2int(src.read(4))
		dumptxt(src, src.tell()+1, str_count-1)
		data=src.read(filesize-src.tell())
		dst.write(data)
		src.close()
		dst.close()
		txt.close()

main()