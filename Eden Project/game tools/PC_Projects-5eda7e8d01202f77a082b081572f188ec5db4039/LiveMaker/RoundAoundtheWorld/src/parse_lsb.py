#LiveMaker lsb file parser
#By KiD


import struct
import json


#D: unsigned dword
#d: signed dword
#B: unsigned byte
#b: byte
#L: LString (dword len + string)

def getByte(f):
    return struct.unpack("<B",f.read(1))[0]

def getByteS(s,i):
    return struct.unpack("<B",s[i])[0]

def getSByteS(s,i):
    return struct.unpack("<b",s[i])[0]

def getWord(f):
    return struct.unpack("<H",f.read(2))[0]

def getWordS(s,i):
    return struct.unpack("<H",s[i:i+2])[0]

def getWordSBigEndian(s,i):
    return struct.unpack(">H",s[i:i+2])[0]

def getSWordS(s,i):
    return struct.unpack("<h",s[i:i+2])[0]

def getDWord(f):
    return struct.unpack("<I",f.read(4))[0]

def getDWordS(s,i):
    return struct.unpack("<I",s[i:i+4])[0]

def getSDWordS(s,i):
    return struct.unpack("<i",s[i:i+4])[0]

def getFloat(f):
    return struct.unpack("<f",f.read(10))[0]

def getDWordBE(f):
    return struct.unpack(">I",f.read(4))[0]

def putDWordBE(f, d):
    f.write(struct.pack(">I",d))

def getQWord(f):
    return struct.unpack("<Q",f.read(8))[0]

def getLStr(f):
    return f.read(getDWord(f))

def getLStrS(s,i):
    l = getDWordS(s,i)
    return i+4+l, s[i+4:i+l+4]

class LSBException(Exception):
    def __init__(self,msg):
        self.msg=msg
    def __str__(self):
        return repr(self.msg)

def myUnpack(format, src, pos):
    res = []
    for i in xrange(len(format)):
        fmt = format[i]
        if fmt=='d':
            res.append(getSDWordS(src,pos))
            pos += 4
        elif fmt=='D':
            res.append(getDWordS(src,pos))
            pos += 4
        elif fmt=='b':
            res.append(getSByteS(src,pos))
            pos += 1
        elif fmt=='B':
            res.append(getByteS(src,pos))
            pos += 1
        elif fmt=='w':
            res.append(getSWordS(src,pos))
            pos += 2
        elif fmt=='W':
            res.append(getWordS(src,pos))
            pos += 2
        elif fmt=='-':
            res.append(getWordSBigEndian(src,pos))
            pos += 2
        elif fmt=='L':
            pos, s = getLStrS(src, pos)
            res.append(s)
        else:
            raise LSBException("Unknown format type %s!" % fmt)
    return (pos, res)


# Decorate:
#  dword x 4
#  byte x 2
#  if ver>=100 dword else byte
#  string x 2
#  if ver>=100 dword x 2

def parseDecorate(tpWord, off, ver):
    if ver>=100:
        format = 'ddddbbdLLdd'
    else:
        format = 'ddddbbbLL'
    off, d = myUnpack(format, tpWord, off)
    #print d
    return off

def parseChar(tpWord, off, ver):
    off, d = myUnpack('Ld-d', tpWord, off)
    #print "WdChar:", d
    return (off, d)

def parseReturn(tpWord, off, ver):
    off, d = myUnpack('b', tpWord, off)
    #print "Return:", d
    return (off, d)

def parseEvent(tpWord, off, ver):
    off, d = myUnpack('L', tpWord, off)
    #print "Event:", d
    return (off, d)

glyphTypes = {1: parseChar, 3: parseReturn, 6: parseEvent}

class TpWord:
	def parse(self):
	    buf = self.tpWord
	    if buf[:6] != 'TpWord':
	        raise LSBException("Not a TpWord block!")
	    self.version = int(buf[6:9])
	    self.nDecs = getDWordS(buf,9)
	    #print "Version: %d, decorates: %d" % (self.version, self.nDecs)
	    off = 13
	    for i in xrange(self.nDecs):
	        off = parseDecorate(buf, off, self.version)
	        #print "off: %x" % off
	    self.nGlyphs = getDWordS(buf,off)
	    #print "Glyphs: %d" % self.nGlyphs
	    off += 4
	    text = ""
	    text_begin = 0
	    text_pos = 0
	    text_d1 =0 
	    text_d3 =0 
	    for i in xrange(self.nGlyphs):
	        t = getByteS(buf, off)
	        pos = off
	        off += 1
	        if t in glyphTypes:
	            off, d = glyphTypes[t](buf, off, self.version)
	            if t==1:
	                charval = d[2]
	                if text == '' :
	                    text_begin = i
	                    text_pos = pos + 4
	                    text_d1 = d[1]
	                    text_d3 = d[3]
	                if d[0] != '' or d[1] != text_d1 or d[3] != text_d3 :
		                if text != '' :
		                    self.scriptTexts.append((text_pos, text_begin, i-1, unicode(text,'cp932'), text_d1, text_d3))
		                    text = ""
		                    text_begin = 0
		                text_begin = i
		                text_pos = pos + 4
		                text_d1 = d[1]
		                text_d3 = d[3]
		                print "Change text property.  %x,%d" % (self.pos+pos, i) , d
	                text += struct.pack("<H", charval)
	                self.events.append((1,charval))
	            elif t==3:
	                if text_begin:
		                if d[0] == 0x1:
		            	    text += '<BR>'
		            	elif d[0] == 0x0:
		            	    text += '<BR2>'
		            	else:
		            	    print "Unknown Return.  %x,%x" % (self.pos + pos, d[0])
	                self.events.append((3,d[0]))
	            elif t==6:
	                if text != '' :
	                    self.scriptTexts.append((text_pos, text_begin, i-1, unicode(text,'cp932'), text_d1, text_d3))
	                text = ""
	                text_begin = 0
	                #ev = d[0].replace('\x01','').replace('\r\n','\\r\\n')
	                self.events.append((6,d[0]))
	        else:
	            raise LSBException("Unknown glyph type: %02X!" % t)
	    if off!=len(buf):
	        print "Warning: didn't parse whole block (%d bytes left)" % (len(buf)-off)
	    
	def __init__(self, buf, pos): 
		self.scriptTexts = []
		self.events = []
		self.pos = pos
		pos, self.tpWord = getLStrS(buf,pos)
		self.parse()
	def size(self): 
		return len(self.tpWord)
	def dump_txt(self): 
		data = u''
		for item in (self.scriptTexts):
			headkey = ['Block','pos','index','len','d1','d3']
			headvalue = [self.pos,self.pos+item[0],"%x-%x" % (item[1], item[2]),len(item[3]),item[4],item[5]]
			data += u"# %s\r\n" % json.dumps(zip(headkey,headvalue))
			#data += u"# Block:%x pos:%x index:%x-%x len:%d d1:%d d3:%d\r\n" % (self.pos, self.pos+item[0], item[1], item[2], len(item[3]), item[4], item[5])
			data += item[3] + u"\r\n"
			data += item[3] + u"\r\n\r\n"
			#x = json.loads(jstr)
			#print zip(*x)
		return data

def parse_lsb(buf):
    i=0
    j=0
    outdata = u''
    while i<len(buf):
        i = buf.find('TpWord',i)
        if i==-1: break
        i -= 4
        block = TpWord(buf,i)
        #print "Block %d: %x,%x  %d,%d" % (j, i, block.size(), block.nGlyphs, len(block.events))
        outdata += block.dump_txt()
        i += 4 + block.size()
        j+=1
    return outdata


