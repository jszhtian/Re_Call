import os,sys

tbl_str=[]

def MapChar(line):
    for l in line:
        if (ord(l) >= 0x80):
            print(hex(ord(l)))

def proc(lines):
    for l in lines:
        l=l.strip()
        MapChar(l)


fs=open("0_all_000.txt","rb")
ls=fs.read().decode('SJIS').split('\r\n')
proc(ls)
fs.close()
