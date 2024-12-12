import os

Name=[]

def IsEx(s):
    for n in Name:
        if n == s:
            return True
    return False

path1='raw_jp'
for f in os.listdir(path1):
    fs=open(os.path.join(path1,f),'rb')
    ls=fs.read().decode("utf-8-sig").split('\r\n')
    fs.close()
    
    for l in ls:
        if l.startswith('【'):
            name = l[:l.find('】')+1]
            if(IsEx(name) == False):
                Name.append(name)
for n in Name:
    #print(n + "->" + n.encode('sjis').decode('gbk') + "->")
    string = 'else if(!strcmp(Name, "%s"))\n\tstrcpy(Name, "%s");' %\
             (n.encode('sjis').decode('gbk'),n)
    print(string)
