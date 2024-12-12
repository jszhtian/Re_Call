import os,sys,re,pdb

mix=[]

def rep(lines):
    ret=[]
    for l in lines:
        if(len(l)==0):
            continue
        else:
            ret.append(l)
    return ret

def proc(lines):
    for l in lines:
        if l.startswith('<'):
            if l.find('//')!=-1:
                l=l[l.find('//')+2:]
                if l.startswith('#'):
                    name=l[l.find('#')+1:l.find('$&')]
                    text=l[l.find('$&')+2:]
                    if text.find('〜')!=-1:
                        text=text.replace('〜','～')
                    jp.append(name)
                    jp.append(text)
                else:
                    text=l
                    if text.find('〜')!=-1:
                        text=text.replace('〜','～')
                    jp.append(text)
            else:
                l=l[l.find('>')+1:]
                if l.startswith('#'):
                    name=l[l.find('#')+1:l.find('$&')]
                    text=l[l.find('$&')+2:]
                    if text.find('〜')!=-1:
                        text=text.replace('〜','～')
                    if text.find('≪')!=-1:
                        text=text.replace('≪','《')
                    if text.find('≫')!=-1:
                        text=text.replace('≫','》')
                    if text.find('・')!=-1:
                        text=text.replace('・','·')
                    cn.append(name)
                    cn.append(text)
                else:
                    text=l
                    if text.find('〜')!=-1:
                        text=text.replace('〜','～')
                    if text.find('≪')!=-1:
                        text=text.replace('≪','《')
                    if text.find('≫')!=-1:
                        text=text.replace('≫','》')
                    if text.find('・')!=-1:
                        text=text.replace('・','·')
                    cn.append(text)


path1='raw'
for f in os.listdir(path1):
    jp=[]
    cn=[]
    print(f)
    fs=open(os.path.join(path1,f),'rb')
    ls=rep(fs.read().decode('u8').split('\n'))
    fs.close()
    proc(ls)
    ###check###
    if(len(jp) == 0 or len(cn)==0):
        #sys.exit(0)
        print(len(jp))
        print(len(cn))
        print('zero text')
    if(len(jp)!=len(cn)):
        print(len(jp))
        print(len(cn))
        sys.exit(0)
    i=0
    for i in range(len(jp)):
        mix.append('//'+jp[i]+'\n'+cn[i])

fs=open('Script.txt','wb')
fs.write('\n'.join(mix).encode('u8'))
fs.close()
