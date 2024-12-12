import os


path1='output'
path2='output_chs'
for f in os.listdir(path1):
    print(f)
    fs=open(os.path.join(path1,f),'rb')
    ls=fs.read().decode("sjis").split('\r\n')
    fs.close()
    fs=open(os.path.join(path2,f.replace('.txt','.txt')),'wb')
    fs.write("\r\n".join(ls).encode('u16'))
    fs.close()
