import os,sys,re,pdb

def rep(lines):
    ret=[]
    for l in lines:
        if(len(l)==0):
            continue
        else:
            ret.append(l)
    return ret

path1='txt_jp'
path2='txt_cn'
path3='mix'
for f in os.listdir(path2):
    mix=[]
    print(f)
    fs1=open(os.path.join(path1,f),'rb')
    ls=rep(fs1.read().decode('u8').split('\r\n'))
    fs2=open(os.path.join(path2,f),'rb')
    ls2=rep(fs2.read().decode('u8').split('\r\n'))
    fs1.close()
    fs2.close()
    ###cmp###
    if(len(ls)!=len(ls2)):
        print(len(ls))
        print(len(ls2))
    else:
        i=0
        for i in range(len(ls)):
            mix.append('//'+ls[i]+'\n'+ls2[i]+'\n')
        fs3=open(os.path.join(path3,f),'wb')
        fs3.write('\n'.join(mix).encode('u8'))
        fs3.close()
