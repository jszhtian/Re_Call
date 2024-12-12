import struct,os

#定义映射
dict1={}
lstIn=["丂@乽","乿"]
lstOut=["「","」"]
def BuildTBL(lines):
    for line in lines:
        if len(line)==0:
            break
        cp932num=int(line[0:4],16)
        gbk=line[5:]
        sjis=cp932num.to_bytes(2,byteorder='big').decode('932')
        #print (gbk+":"+sjis)
        dict1.update({gbk:sjis})
#加载TBL文件，读取SJIS GBK映射关系
fs2=open('map.tbl', 'rb')
ls2=fs2.read().decode('u16').split('\r\n')
BuildTBL(ls2)
#映射GBK到SJIS
def getMapedChar(ch):
    newch=''
    try:
        newch=dict1[ch]
    except:
        #print("pass:"+ch)
        newch=ch
    return newch

def FilterText(line):
    newline=''
    if(len(lstIn)!=len(lstOut)):
        print("Not Equal List, check Filter Text Function!")
    tmpline=line
    for i in range(len(lstIn)):
        tmpline=tmpline.replace(lstIn[i],lstOut[i])
    for ch in tmpline:
        newline+=getMapedChar(ch)
    return newline

def proc(oldlist,newlist):
    newls=[]
    if len(oldlist)!=len(newlist):
        print("Input Lists are not equal!, check input text file!")
        return newls
    for i in range(len(oldlist)):
        oldLine=oldlist[i]
        newLine=newlist[i]
        newls.append(FilterText(newLine))
    return newls



path1='raw'
path2='old'
path3='out'
for f in os.listdir(path2):
	print(f)
	trans=[]
	fs1=open(os.path.join(path1,f),'rb')
	ls=fs1.read().decode('u16').split('\r\n')
	fs2=open(os.path.join(path2,f),'rb')
	rs=fs2.read().decode('u16').split('\r\n')
	newls=proc(ls,rs)
	fs3=open(os.path.join(path3,f),'wb')	
	fs3.write('\r\n'.join(newls).encode('u16'))
	fs1.close()
	fs2.close()
	fs3.close()