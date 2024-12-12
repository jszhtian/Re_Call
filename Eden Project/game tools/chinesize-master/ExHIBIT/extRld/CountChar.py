import os

AllCount = 0

def CountChar(ls):
    for l in ls:
        for c in l:
            global AllCount
            if ord(c) > 0x80:
                AllCount+=1

path1 = "txt"
for f in os.listdir(path1):
    fs=open(os.path.join(path1,f),"rb")
    ls=fs.read().decode('u16').split('\r\n')
    CountChar(ls)
    fs.close()
print(AllCount)
    
