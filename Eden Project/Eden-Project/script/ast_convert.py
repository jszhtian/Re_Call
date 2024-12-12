#encoding=utf-8
import os
import re
import pdb
#for ラズベリーキューブ
log=''
def convert(text,count):
        converted="☆%08d☆%s\n★%08d★%s\n"%(count,text,count,text)
        return converted

def bold(text,sig1,sig2):
        bolded=text[text.find(sig1)+len(sig1):text.find(sig2)]
        return bolded

def proc(lines):
	newl=[]
	i=0
	for l in lines:
		l=l.strip()
		if len(l)==0:
			continue
		elif l.startswith('"'):
			#print(l)
			tmpstr=l[l.find('"')+1:l.find('",')]
			newl.append(convert(tmpstr,i))
			i+=1
		elif (l.startswith('name') or l.startswith('"')):
			if (l.find('name=')!=-1 and l.find('text=')!=-1):
				#tmpstr=l[l.find(',text="')+7:l.find('",en')]
				newl.append(convert(bold(l,'text="','",en="'),i))
				i+=1
			elif (l.find('name=')!=-1 and l.find('text=')==-1):
				#tmpstr=l[l.find(',name="')+7:l.find('"},')]
				newl.append(convert(bold(l,'name="','"},'),i))
				i+=1
			elif (l.find('name=')!=-1 and l.find('",en')!=-1):
				#tmpstr=l[l.find(',name="')+7:l.find('"},')]
				newl.append(convert(bold(l,'name="','",en'),i))
				i+=1
		elif l.startswith('{"select",label='):
			if l.find('",exp')==-1:
				newl.append(convert(bold(l,',text="','"},'),i))
				i+=1
                                #tmpstr=l[l.find(',text="')+7:l.find('"},')]
			else:
				newl.append(convert(bold(l,',text="','",exp'),i))
				i+=1
		else:
			continue
	print ('有'+str(i)+'行。')
	return newl

path1='script'
path2='text_cn'
file_count=0
for f in os.listdir(path1):
    print(f)
    fs=open(os.path.join(path1,f),'rb')
    ls=fs.read().decode("utf8").split('\n')
    newls=proc(ls)
    fs=open(os.path.join(path2,f.replace('.ast','.txt')),'wb')
    fs.write("\n".join(newls).encode('utf8'))
    fs.close()
    file_count+=1
print('总文件数：'+str(file_count))
