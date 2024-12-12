import os,sys,pdb,shutil

FileName=[]
def find_last(string,str):
    last_position=-1
    while True:
        position=string.find(str,last_position+1)
        if position==-1:
            return last_position
        last_position=position

def WalkFile(file):
    fl=[]
    for root,dirs,files in os.walk(file):
        for f in files:
            #print(os.path.join(root,f))
            tstr=os.path.join(root,f)
            fl.append(tstr)
        #for d in dirs:
            #print(os.path.join(root,d))
    return fl


filelist=WalkFile('data')
i=0
p=0
CopyList=WalkFile('patch')
for i in range(len(filelist)):
    for p in range(len(CopyList)):
        if (filelist[i][find_last(filelist[i],'\\')+1:])==(CopyList[p][CopyList[p].find('\\')+1:]):
            print(CopyList[p][CopyList[p].find('\\')+1:])
            print('-----------------------------------------')
            print(filelist[i][find_last(filelist[i],'\\')+1:])
            shutil.copyfile(CopyList[p], filelist[i])
        else:p+=1
    i+=1
    
