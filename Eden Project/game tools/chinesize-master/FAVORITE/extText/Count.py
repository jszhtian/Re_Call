import os, sys



path1 = "World_txt"
cnt = 0
def proc(lines):
    for ls in lines:
        for c in ls:
            if c != "【" or c != "】" or c != "、" or c != "。"or c != "[" or c != "]":
                global cnt
                cnt += 1

for f in os.listdir(path1):
    fs=open(os.path.join(path1,f),'rb')
    ls=fs.read().decode("u16").split('\r\n')
    fs.close()
    proc(ls)

print(cnt)
