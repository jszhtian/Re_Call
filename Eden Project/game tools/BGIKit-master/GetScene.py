import os,sys,re,pdb

def fmt(string,index):
    return "☆%06d☆%s\r\n★%06d★%s\r\n"%(index, string, index, string)

def IsJis(c:str) -> bool:
    if(ord(c) > 0x80):
        return True
    return False

def GetScene(lines):
    i=0
    ret=[]
    for l in lines:
        t_str=l[l.find('>')+1]
        if((IsJis[t_str[0]] == True) or (t_str[0] == '<')):
            ret.append(fmt(t_str,i))
            i+=1
        else:
            

