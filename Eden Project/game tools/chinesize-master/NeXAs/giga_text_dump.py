# -*- coding:utf-8 -*-
import struct,os,fnmatch,re

def walk(adr):
	mylist=[]
	for root,dirs,files in os.walk(adr):
		for name in files:
			if name[-5:] != '.bin':
				continue
			adrlist=os.path.join(root, name)
			mylist.append(adrlist)
	return mylist

def byte2int(byte):
	long_tuple=struct.unpack('L',byte)
	long = long_tuple[0]
	return long

def int2byte(num):
	return struct.pack('L',num)

def FormatString(string, count):
	'''
	res = "★%08d★\n%s\n"%(count, string+'\n')
	
	res = "☆%08d☆\n%s★%08d★\n%s\n"%(count, string+'\n', count, string+'\n')
	'''
	
	#res = "○%08d○%s○\n%s●%08d●%s●\n%s\n"%(count, name, string, count, name, string)
	
	
	#res = "○%08d○\n%s\n●%08d●\n%s\n\n"%(count, string, count, string)
	
	return string

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
	f_lst = walk('Script')
	print(f_lst)
	for fn in f_lst:
		print(fn)
		src = open(fn, 'rb')
		dstname = fn[:-5] + '.txt'
		dst = open(dstname, 'w', encoding='utf16')

		src.seek(4)
		entry_count = byte2int(src.read(4))
		str_offset = (entry_count << 1) * 4 + 8
		src.seek(str_offset)
		str_count = byte2int(src.read(4))

		str_list = dumptxt(src, src.tell()+1, str_count-1) #最后一句不是文本
		i = 0
		for string in str_list:
			dst.write(FormatString(string, i))
			i += 1

		src.close()
		dst.close()
		#print(fn[7:]+'--> done!')
main()
