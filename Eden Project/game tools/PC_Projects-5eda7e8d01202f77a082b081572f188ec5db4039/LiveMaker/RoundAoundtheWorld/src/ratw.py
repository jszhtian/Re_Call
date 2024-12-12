#encoding=gbk
#Round Aound the World Tools
#By KiD


import mmap
import os
import sys
import struct
import codecs
import zlib
import copy
import parse_lsb

class HeadIndex:
	def __init__(self, buf, len):
		self.name = buf
		self.len = len

class Head:
	def __init__(self):
		self.num = 0
		self.head1 = ""
		self.head2 = ""
		self.head3 = ""
		self.head4 = ""
		self.head5 = ""
		self.head6 = ""
		
	def Add(self, element):
		self.num += 1
		self.head1 += struct.pack("<I", element.len) + element.name
		self.head2 += struct.pack("<II", element.data1, element.data2)
		self.head3 += struct.pack("<c", element.uncompressed)
		self.head4 += struct.pack("<I", element.unknown1)
		self.head5 += struct.pack("<I", element.unknown2)
		self.head6 += struct.pack("<B", element.unknown3)
		
	def Write(self, name, end):
		self.buf = struct.pack("<6sI", "vff\x00\x00\x00", self.num)
		open(name,"wb+").write(self.buf+self.head1+self.head2+end+self.head3+self.head4+self.head5+self.head6)
		print "Write head index %s, %d files." % (name, self.num)


def compress(buf, output):
	open(output, "wb+").write(zlib.compress(buf, 9))

def comment_name(name, key1, key2):
	key2 = (key1 + key2 + (key2 << 2)) & 0xFFFFFFFF
	name_buf = chr(0x2f ^ (key2&0xff)) + name[1:]
	return name_buf

def import_dat(buf, datafile, inputdir):
	if buf[0:3] != "vff" :
		print "Unknown file format."
		return
	num = struct.unpack("<I", buf[6:10])[0]
	file_indexs = []
	pos = 0xA
	for i in range(0, num):
		name_len = struct.unpack("<I", buf[pos:pos+4])[0]
		file_indexs.append(HeadIndex(buf[pos+4:pos+4+name_len], name_len))
		pos += 4 + name_len
	print "Total %d files." % len(file_indexs)
	file_indexs.append(HeadIndex("", 0))	# for calc last file size
	#key1,key2 = struct.unpack("<II", buf[pos:pos+8])
	key1 = 0x75D6EE39
	key2 = 0
	#t1,t2 = struct.unpack("<II", buf[pos:pos+8])
	t1 = 0x75D6EE39
	t2 = 0
	txt = u''
	print "Decrypting index ..."
	cnt = 0
	base_ptr = pos+8+num*8
	base_ptr1 = base_ptr+num
	base_ptr2 = base_ptr1+num*4
	base_ptr3 = base_ptr2+num*4
	for element in file_indexs:
		name_buf = ""
		element.name_key1 = key1
		element.name_key2 = key2
		for i in element.name:
			key2 = (key1 + key2 + (key2 << 2)) & 0xFFFFFFFF
			name_buf += chr(ord(i) ^ (key2&0xff))
		#element.name = name_buf.decode('cp932').encode('cp936','ignore')
		element.plain_name = name_buf.decode('cp932').replace(u'\u30fb', u"·")
		element.plain_name = element.plain_name.replace(u':', u"")
		#print "t %x  %x" %(t1,t2)
		t2 = (t1 + t2 + (t2 << 2)) & 0xFFFFFFFF
		if t2 & 0x80000000 :
			x2 = 0xFFFFFFFF
		else :

			x2 = 0
		data1, data2 = struct.unpack("<II", buf[pos:pos+8])
		#print "data %x  %x" %(data1,data2)
		element.data1 = data1
		element.data2 = data2
		data1 = t2 ^ data1
		data2 = x2 ^ data2
		element.key = t2
		element.d_data1 = data1
		element.d_data2 = data2
		element.pos = data1
		if element.len == 0:
			pos += 8
			break;
		element.uncompressed = buf[base_ptr:base_ptr+1]
		element.unknown1 = struct.unpack("<I", buf[base_ptr1:base_ptr1+4])[0]
		element.unknown2 = struct.unpack("<I", buf[base_ptr2:base_ptr2+4])[0]
		element.unknown3 = struct.unpack("<B", buf[base_ptr3:base_ptr3+1])[0]
		pos += 8
		base_ptr += 1
		base_ptr1 += 4
		base_ptr2 += 4
		base_ptr3 += 1	
		cnt += 1
	if len(buf) - pos != num * 2 + num * 4 * 2:
		print "Bad ext size. %x %x %x" % (pos, len(buf) - pos, num * 2 + num * 4 * 2)
		return
	cnt = 1
	add_file_indexs = []
	pos_end = file_indexs[num].pos
	t2 = file_indexs[num-1].key
	new_head = Head()
	fd = open(datafile, "rb+")
	fd.seek(pos_end,0)
	for element in file_indexs:
		if element.len == 0:
			print "end %x  %x" %(element.d_data1, element.d_data2)
			break;
		element.size = file_indexs[cnt].pos - element.pos
		file_name = u"%s\\%s" % (inputdir, element.plain_name)
		if os.path.isfile(file_name) :
			src = open(file_name, "rb").read()
			if (element.uncompressed == '\x00') :
				dst = zlib.compress(src)
			else :
				dst = src
			element.name = comment_name(element.name, element.name_key1, element.name_key2)
			new_element = copy.deepcopy(element)
			name_buf = ""
			for i in element.plain_name:
				key2 = (key1 + key2 + (key2 << 2)) & 0xFFFFFFFF
				name_buf += chr(ord(i) ^ (key2&0xff))
			new_element.name = name_buf
			new_element.pos = pos_end
			new_element.size = len(dst)
			pos_end += new_element.size
			t2 = (t1 + t2 + (t2 << 2)) & 0xFFFFFFFF
			if t2 & 0x80000000 :
				x2 = 0xFFFFFFFF
			else :
				x2 = 0
			new_element.data1 = t2 ^ new_element.pos
			new_element.data2 = x2 ^ new_element.d_data2
			new_element.key = t2
			add_file_indexs.append(new_element)
			if fd.tell() != new_element.pos :
				print "Bad file size. %x %x" % (fd.tell(), new_element.pos)
			fd.write(dst)
			print "import %s  %x:%x,%x  (%x %x)" % (new_element.plain_name, new_element.pos, len(src), new_element.size, new_element.data1, new_element.data2)
		new_head.Add(element)
		cnt += 1
		txt += u"%s (%x,%x)  %x,%x  (%x-%x,%x) %x,%x,%x\r\n" % (element.plain_name, element.name_key1, element.name_key2, element.pos, element.size, 
			element.data1, element.data2, element.key, element.unknown1, element.unknown2, element.unknown3)
	fd.close()
	for element in add_file_indexs:
		new_head.Add(element)

	t2 = (t1 + t2 + (t2 << 2)) & 0xFFFFFFFF
	if t2 & 0x80000000 :
		x2 = 0xFFFFFFFF
	else :
		x2 = 0
	file_indexs[num].d_data1 = pos_end
	file_indexs[num].data1 = t2 ^ file_indexs[num].d_data1
	file_indexs[num].data2 = x2 ^ file_indexs[num].d_data2
	print "new end %x (%x,%x)" % (pos_end, file_indexs[num].data1, file_indexs[num].data2)
	new_head.Write("head.bin", struct.pack("<II", file_indexs[num].data1,file_indexs[num].data2))
	codecs.open("index.txt","w+",encoding="utf-16").write(txt)  
		
def extract(buf, datafile, outputdir):
	if buf[0:3] != "vff" :
		print "Unknown file format."
		return
	num = struct.unpack("<I", buf[6:10])[0]
	file_indexs = []
	pos = 0xA
	for i in range(0, num):
		name_len = struct.unpack("<I", buf[pos:pos+4])[0]
		file_indexs.append(HeadIndex(buf[pos+4:pos+4+name_len], name_len))
		pos += 4 + name_len
	print "Total %d files." % len(file_indexs)
	file_indexs.append(HeadIndex("", 0))	# for calc last file size
	#key1,key2 = struct.unpack("<II", buf[pos:pos+8])
	key1 = 0x75D6EE39
	key2 = 0
	#t1,t2 = struct.unpack("<II", buf[pos:pos+8])
	t1 = 0x75D6EE39
	t2 = 0
	txt = u''
	print "Decrypting index ..."
	for element in file_indexs:
		name_buf = ""
		element.name_key1 = key1
		element.name_key2 = key2
		for i in element.name:
			key2 = (key1 + key2 + (key2 << 2)) & 0xFFFFFFFF
			name_buf += chr(ord(i) ^ (key2&0xff))
		#element.name = name_buf.decode('cp932').encode('cp936','ignore')
		element.name = name_buf.decode('cp932').replace(u'\u30fb', u"·")
		element.name = element.name.replace(u':', u"")
		#print element.name
		t2 = (t1 + t2 + (t2 << 2)) & 0xFFFFFFFF
		if t2 & 0x80000000 :
			x2 = 0xFFFFFFFF
		else :
			x2 = 0
		data1, data2 = struct.unpack("<II", buf[pos:pos+8])
		element.data1 = data1
		element.data2 = data2
		data1 = t2 ^ data1
		data2 = x2 ^ data2
		element.key = t2
		element.d_data1 = data1
		element.d_data2 = data2
		element.pos = data1
		pos += 8
	#if len(buf) - pos != num * 2 + num * 4 * 2:
	#	print "Bad ext size. %x %x %x" % (pos, len(buf) - pos, num * 2 + num * 4 * 2)
	#	return
	fd = open(datafile, "rb")
	cnt = 1
	for element in file_indexs:
		if element.len == 0:
			print "end %x  %x" %(element.d_data1, element.d_data2)
			break;
		fd.seek(element.pos, 0)
		file_name = u"%s\\%s" % (outputdir, element.name)
		path_name = os.path.split(file_name)[0]
		if not os.path.isdir(path_name) :
			os.makedirs(path_name)
		size = file_indexs[cnt].pos - element.pos
		if (buf[pos:pos+1] == '\x00') :
			dst = zlib.decompress(fd.read(size))
		else :
			dst = fd.read(size)
		print "Write %d  %s  %x  %x-%x" % (cnt, element.name, element.pos, size, len(dst))
		open(file_name, "wb+").write(dst)
		cnt += 1
		pos += 1
		txt += u"%s (%x,%x)  %x:%x,%x  (%x-%x,%x)\r\n" % (element.name, element.name_key1, element.name_key2, element.pos, size, len(dst), element.data1, element.d_data2, element.key)
	fd.close()
	codecs.open("name.txt","w+",encoding="utf-16").write(txt)  

def extract_script_texts(buf, filename):
	ret = parse_lsb.parse_lsb(buf)
	if ret != '' :
		codecs.open(filename+".txt","w+",encoding="utf-16").write(ret)  

if __name__ == "__main__":
    print "Round Aound the World Tools By Kid, 2014\n"

    if len(sys.argv) < 3:
        print('Bad argv.')
        sys.exit(0)

    fd = os.open(sys.argv[2], os.O_RDONLY)
    size = os.fstat(fd).st_size
    buf = mmap.mmap(fd, size, access=mmap.ACCESS_READ)
    if sys.argv[1] == '-e':
        extract(buf, sys.argv[3], sys.argv[4])
    elif sys.argv[1] == '-i':
        import_dat(buf, sys.argv[3], sys.argv[4])
    elif sys.argv[1] == '-c':
        compress(buf, sys.argv[3])
    elif sys.argv[1] == '-et':
        extract_script_texts(buf, sys.argv[2])
    else:
        print 'Bad argv.'
    os.close(fd)

    
