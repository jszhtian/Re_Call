# -*- coding: utf-8 -*-
# For Windows OS Only....

import sys,os,time,struct,threading,subprocess,signal
from tkinter import *
from tkinter.filedialog import *
from tkinter import messagebox
from tkinter.ttk import Combobox
import SceneUnpacker,ScenePacker,GameexeUnpacker,GameexePacker
import ssDumper,ssPacker,dbsDecrypt,dbsEncrypt,pckUnpacker,pckPacker,omvCuter


tool=["Unpack Scene","Pack Scene","Decrypt Gameexe","Encrypt Gameexe",
      "Dump ss","Pack ss","Dump dbs","Pack dbs","Unpack pck","Pack pck",
      "Cut OMV header"]
command=["SceneUnpacker","ScenePacker","GameexeUnpacker","GameexePacker",
         "ssDumper","ssPacker","dbsDecrypt","dbsEncrypt",
         "pckUnpacker","pckPacker","omvCuter"]

ENTRY_WIDTH=58
BUTTON_WIDTH=6
PAD=4
lastDir=os.getcwd()
typedKey=True
hasList=False
hasSkf=False
singleProcess=True
DECRYPT_KEY=[0xC0, 0xA1, 0x8A, 0x52, 0xE9, 0x8D, 0x16, 0x22, 
             0x8B, 0xF7, 0xCE, 0xCC, 0xF8, 0x7A, 0x56, 0xAF]
KEY_FILE="SiglusKey.txt"
KEY_LIST="KeyList.txt"
cmdOutput=sys.stdout


def stringKey(key):
    keyHex=[]
    for byte in key:
        keyHex.append(hex(byte))
    keyStr=str(keyHex).replace("[","").replace("]","").replace("'","")
    return keyStr

def setKey(keyStr):
    keyHex=keyStr.split(',')
    key=[]
    for byte in keyHex:
        key.append(int(byte,16))
    return key
    
def loadKey():
    try:
        file=open(KEY_FILE,'r')
        keyStr=file.readline()
        file.close()
        key=setKey(keyStr)
    except:
        return []
    else:
        return key

def saveKey():
    keyStr=stringKey(DECRYPT_KEY)
    file=open(KEY_FILE,'w')
    file.write(keyStr)
    file.close()
    return True

def selectKey(value):
    global typedKey
    typedKey=False
    keyVar.set(keyList[keySelect.current()])

def unlock(value):
    global typedKey
    typedKey=True

def start():
    global lastSelect,option,DECRYPT_KEY
    optionList.selection_set(lastSelect)
    try:
        if lastSelect<4:
            tempKey=setKey(keyVar.get())
            if tempKey:
                DECRYPT_KEY=tempKey
        check=option.run()
    except:
        messagebox.showerror("Error!","Error!\nKey is wrong?")
    '''
    else:
        if check:
            messagebox.showinfo("Notice","Finished!")
            if lastSelect<4 and typedKey:
                saveKey()
        else:
            messagebox.showwarning("Warning","Input error!")
    '''

class findKey(threading.Thread):
    def __init__(self):
        global DECRYPT_KEY,typedKey,kfButton
        super(findKey,self).__init__()
        self.setDaemon(True)
        self.signal=threading.Event()
        self.signal.set()
    def stop(self):
        self.signal.clear()
    def run(self):
        try:
            f=subprocess.Popen(os.getcwd()+"/skf.exe",
                               creationflags=subprocess.CREATE_NO_WINDOW,
                               stdin=subprocess.PIPE,
                               stdout=subprocess.PIPE,
                               stderr=subprocess.STDOUT)
        except Exception as e:
            messagebox.showerror("Error!","Can't start skf.exe!\n"+e)
        cmdText['state']='normal'
        print("Please start the game and wait for a moment...\n"+
              "Click the button again to stop.")
        cmdText['state']='disabled'
        n=1
        while f.poll()==None:
            kfButton['text']="Finding"+'.'*n
            n=n%3+1
            time.sleep(1)
            if not self.signal.isSet():
                f.kill()
                kfButton['text']="Find Key"
                return
        try:
            output=f.stdout.readlines()
            #print(output)
            keyIndex=output.index(b'Keys found:\n')+1
            keyStr=output[keyIndex].decode()
            newKey=setKey(keyStr)
        except:
            if output[0][:6]==b'Please':
                messagebox.showwarning("Warning",
                                   "Can't find key!\nPlease try again.")
            else:
                messagebox.showerror("Error!","Can't start skf.exe!")
            kfButton['text']="Find Key"
            return
        else:
            messagebox.showinfo("Notice","Key Found!")
            DECRYPT_KEY=newKey
            typedKey=True
            keyVar.set(keyStr)
            if hasList:
                keySelect.current(0)
                keyList[0]=stringKey(DECRYPT_KEY)
            kfButton['text']="Find Key"

def clickFindKey():
    global finding
    if kfButton['text']=="Find Key":
        finding=findKey()
        finding.start()
    else:
        finding.stop()

def select(event):
    global lastSelect,option,title
    selected=optionList.curselection()[0]
    if selected==lastSelect:
        return
    else:
        lastSelect=selected
    option=eval("set"+command[selected]+"()")
    title.pack_forget()
    title=Label(selectedFrame,text=tool[selected],font=('Fixdsys 14 bold'))
    title.pack()

def clear():
    global inputFrame
    inputFrame.pack_forget()
    inputFrame=Frame(rightFrame)
    inputFrame.pack(side='top')

def selectFile(v):
    global lastDir
    temp=askopenfilename(initialdir=lastDir)
    if temp:
        v.set(temp)
        lastDir=temp[:temp.rfind('/')]
def setFile(v):
    global lastDir
    temp=asksaveasfilename(initialdir=lastDir,initialfile=v.get())
    if temp:
        v.set(temp)
        lastDir=temp[:temp.rfind('/')]
def selectFolder(v):
    global lastDir
    temp=askdirectory(initialdir=lastDir)
    if temp:
        v.set(temp)
        lastDir=temp

def running(cmd,key):
    if key==None:
        code=cmd[0]+".main(cmd)"
    else:
        code=cmd[0]+".main(cmd,key)"
    cmdText['state']='normal'
    print('Running "'+cmd[0]+'"...')
    if singleProcess:
        startButton['state']='disabled'
    check=eval(code)
    if check:
        messagebox.showinfo("Notice","Finished!")
        if lastSelect<4 and typedKey:
            saveKey()
    else:
        messagebox.showwarning("Warning","Input error!")
    cmdText['state']='disabled'
    startButton['state']='normal'


def runningExe(cmd):
    cmdText['state']='normal'
    if singleProcess:
        startButton['state']='disabled'
    print('Running "'+cmd[0:cmd.find('.exe')+4]+'"...')
    try:
        exe=subprocess.Popen(os.getcwd()+"/"+cmd,bufsize=1,
                             creationflags=subprocess.CREATE_NO_WINDOW,
                             stdin=subprocess.PIPE,stdout=subprocess.PIPE,
                             stderr=subprocess.PIPE,universal_newlines=True)
        for line in iter(exe.stdout.readline,''):
            print(line.replace('\n',''))
    except Exception as e:
        messagebox.showerror("Error!","Can't create process!\n"+e)
    else:
        if exe.stderr.readlines() or line==[]:
            messagebox.showerror("Error!","Failed!")
        else:
            if line[:5]=="error":
                messagebox.showwarning("Warning",line)
            else:
                messagebox.showinfo("Notice",line)
    cmdText['state']='disabled'
    startButton['state']='normal'

class setSceneUnpacker:
    def __init__(self):
        clear()
        name1=Label(inputFrame,text="Scene file:")
        name2=Label(inputFrame,text="Output folder:")
        name1.grid(row=0,padx=2,sticky='w')
        name2.grid(row=2,padx=2,sticky='w')
        value1.set("Scene.pck")
        value2.set("Scene")
        entry1=Entry(inputFrame,width=ENTRY_WIDTH,textvariable=value1)
        entry2=Entry(inputFrame,width=ENTRY_WIDTH,textvariable=value2)
        entry1.grid(row=1,column=0,padx=2)
        entry2.grid(row=3,column=0,padx=2)
        button1=Button(inputFrame,text="Select",width=BUTTON_WIDTH,command=lambda:selectFile(value1))
        button2=Button(inputFrame,text="Select",width=BUTTON_WIDTH,command=lambda:selectFolder(value2))
        button1.grid(row=1,column=1,padx=2)
        button2.grid(row=3,column=1,padx=2)
    def run(self):
        cmd=["SceneUnpacker",value1.get(),value2.get()]
        runPy=threading.Thread(target=running,args=(cmd,DECRYPT_KEY))
        runPy.setDaemon(True)
        runPy.start()
        return 

class setScenePacker:
    def __init__(self):
        clear()
        name1=Label(inputFrame,text="Scene file:")
        name2=Label(inputFrame,text="Scene folder:")
        name3=Label(inputFrame,text="Output file:")
        name1.grid(row=0,padx=2,sticky='w')
        name2.grid(row=2,padx=2,sticky='w')
        name3.grid(row=4,padx=2,sticky='w')
        value1.set("Scene.pck")
        value2.set("Scene")
        value3.set("Scene.pck2")
        entry1=Entry(inputFrame,width=ENTRY_WIDTH,textvariable=value1)
        entry2=Entry(inputFrame,width=ENTRY_WIDTH,textvariable=value2)
        entry3=Entry(inputFrame,width=ENTRY_WIDTH,textvariable=value3)
        entry1.grid(row=1,column=0,padx=2)
        entry2.grid(row=3,column=0,padx=2)
        entry3.grid(row=5,column=0,padx=2)
        button1=Button(inputFrame,text="Select",width=BUTTON_WIDTH,command=lambda:selectFile(value1))
        button2=Button(inputFrame,text="Select",width=BUTTON_WIDTH,command=lambda:selectFolder(value2))
        button3=Button(inputFrame,text="Select",width=BUTTON_WIDTH,command=lambda:setFile(value3))
        button1.grid(row=1,column=1,padx=2)
        button2.grid(row=3,column=1,padx=2)
        button3.grid(row=5,column=1,padx=2)
        valueC.set('17')
        nameC=Label(inputFrame,text="Compression Level(2-17, 0 for Fake Compression): ")
        nameC.grid(row=7,padx=2,pady=4,sticky='e')
        entryC=Entry(inputFrame,width=4,textvariable=valueC)
        entryC.grid(row=7,column=1,padx=2,pady=4)
    def run(self):
        cmd=["ScenePacker",value1.get(),value2.get(),value3.get()]
        try:
            comp=int(valueC.get())
        except:
            comp=0
        else:
            if comp<2:
                comp=0
            elif comp>17:
                comp=17
        if comp!=0:
            cmd.append("-c")
            cmd.append(str(comp))
        runPy=threading.Thread(target=running,args=(cmd,DECRYPT_KEY))
        runPy.setDaemon(True)
        runPy.start()
        return 
        
class setGameexeUnpacker:
    def __init__(self):
        clear()
        name1=Label(inputFrame,text="Gameexe:")
        name2=Label(inputFrame,text="Output file:")
        name1.grid(row=0,padx=2,sticky='w')
        name2.grid(row=2,padx=2,sticky='w')
        value1.set("Gameexe.dat")
        value2.set("Gameexe.ini")
        entry1=Entry(inputFrame,width=ENTRY_WIDTH,textvariable=value1)
        entry2=Entry(inputFrame,width=ENTRY_WIDTH,textvariable=value2)
        entry1.grid(row=1,column=0,padx=2)
        entry2.grid(row=3,column=0,padx=2)
        button1=Button(inputFrame,text="Select",width=BUTTON_WIDTH,command=lambda:selectFile(value1))
        button2=Button(inputFrame,text="Select",width=BUTTON_WIDTH,command=lambda:setFile(value2))
        button1.grid(row=1,column=1,padx=2)
        button2.grid(row=3,column=1,padx=2)
    def run(self):
        cmd=["GameexeUnpacker",value1.get(),value2.get()]
        runPy=threading.Thread(target=running,args=(cmd,DECRYPT_KEY))
        runPy.setDaemon(True)
        runPy.start()
        return 

class setGameexePacker:
    def __init__(self):
        clear()
        name1=Label(inputFrame,text="Gameexe:")
        name2=Label(inputFrame,text="Output file:")
        name1.grid(row=0,padx=2,sticky='w')
        name2.grid(row=2,padx=2,sticky='w')
        value1.set("Gameexe.ini")
        value2.set("Gameexe.dat2")
        entry1=Entry(inputFrame,width=ENTRY_WIDTH,textvariable=value1)
        entry2=Entry(inputFrame,width=ENTRY_WIDTH,textvariable=value2)
        entry1.grid(row=1,column=0,padx=2)
        entry2.grid(row=3,column=0,padx=2)
        button1=Button(inputFrame,text="Select",width=BUTTON_WIDTH,command=lambda:selectFile(value1))
        button2=Button(inputFrame,text="Select",width=BUTTON_WIDTH,command=lambda:setFile(value2))
        button1.grid(row=1,column=1,padx=2)
        button2.grid(row=3,column=1,padx=2)
        valueC.set('0')
        nameC=Label(inputFrame,text="Compression Level(2-17, 0 for Fake Compression): ")
        nameC.grid(row=7,padx=2,pady=4,sticky='e')
        entryC=Entry(inputFrame,width=4,textvariable=valueC)
        entryC.grid(row=7,column=1,padx=2,pady=4)
        valueB.set(False)
        buttonB=Checkbutton(inputFrame,text="Double Encryption(Useless)",variable=valueB)
        buttonB.grid(row=8,padx=2,pady=4,sticky='e')
    def run(self):
        cmd=["GameexePacker",value1.get(),value2.get(),value3.get()]
        if valueB.get():
            cmd.append("-p")
        try:
            comp=int(valueC.get())
        except:
            comp=0
        else:
            if comp<2:
                comp=0
            elif comp>17:
                comp=17
        if comp!=0:
            cmd.append("-c")
            cmd.append(str(comp))
        runPy=threading.Thread(target=running,args=(cmd,DECRYPT_KEY))
        runPy.setDaemon(True)
        runPy.start()
        return 

class setssDumper:
    def __init__(self):
        
        def checkEnable():
            if valueB1.get():
                buttonB2['state']='normal'
            else:
                valueB2.set(False)
                checkName()
                buttonB2['state']='disabled'
        def checkName():
            if valueB2.get():
                name2['text']='Output file:'
            else:
                name2['text']="Output folder:"
        def outSelect():
            if valueB2.get():
                setFile(value2)
            else:
                selectFolder(value2)
                
        clear()
        name1=Label(inputFrame,text="Ss folder:")
        name2=Label(inputFrame,text="Output folder:")
        name1.grid(row=0,padx=2,sticky='w')
        name2.grid(row=2,padx=2,sticky='w')
        value1.set("Scene")
        value2.set("Text")
        entry1=Entry(inputFrame,width=ENTRY_WIDTH,textvariable=value1)
        entry2=Entry(inputFrame,width=ENTRY_WIDTH,textvariable=value2)
        entry1.grid(row=1,column=0,padx=2)
        entry2.grid(row=3,column=0,padx=2)
        button1=Button(inputFrame,text="Select",width=BUTTON_WIDTH,command=lambda:selectFolder(value1))
        button2=Button(inputFrame,text="Select",width=BUTTON_WIDTH,command=outSelect)
        button1.grid(row=1,column=1,padx=2)
        button2.grid(row=3,column=1,padx=2)
        valueB.set(False)
        valueB1.set(False)
        valueB2.set(False)
        valueB3.set(True)
        buttonB=Frame(inputFrame)
        buttonB.grid(row=6,column=0,padx=2,pady=4,sticky='w')
        buttonB3=Checkbutton(buttonB,text="Copy text",variable=valueB3)
        buttonB0=Checkbutton(buttonB,text="Export all data",variable=valueB)
        buttonB1=Checkbutton(buttonB,text="Export as xlsx",command=checkEnable,variable=valueB1)
        buttonB2=Checkbutton(buttonB,text="Use single xlsx",state='disabled',command=checkName,variable=valueB2)
        buttonB3.grid(row=0,column=0,sticky='w')
        buttonB0.grid(row=0,column=1,sticky='w')
        buttonB1.grid(row=0,column=2,sticky='e')
        buttonB2.grid(row=0,column=3,sticky='e')
    def run(self):
        cmd=["ssDumper",value1.get(),value2.get()]
        if valueB.get():
            cmd.append("-a")
        if valueB3.get():
            cmd.append("-d")
        if valueB1.get():
            cmd.append("-x")
            if valueB2.get():
                cmd.append("-s")
        runPy=threading.Thread(target=running,args=(cmd,None))
        runPy.setDaemon(True)
        runPy.start()
        return 

class setssPacker:
    def __init__(self):
        clear()
        name1=Label(inputFrame,text="Ss folder:")
        name2=Label(inputFrame,text="Text folder:")
        name3=Label(inputFrame,text="Output folder:")
        name1.grid(row=0,padx=2,sticky='w')
        name2.grid(row=2,padx=2,sticky='w')
        name3.grid(row=4,padx=2,sticky='w')
        value1.set("Scene")
        value2.set("Text")
        value3.set("Output")
        entry1=Entry(inputFrame,width=ENTRY_WIDTH,textvariable=value1)
        entry2=Entry(inputFrame,width=ENTRY_WIDTH,textvariable=value2)
        entry3=Entry(inputFrame,width=ENTRY_WIDTH,textvariable=value3)
        entry1.grid(row=1,column=0,padx=2)
        entry2.grid(row=3,column=0,padx=2)
        entry3.grid(row=5,column=0,padx=2)
        button1=Button(inputFrame,text="Select",width=BUTTON_WIDTH,command=lambda:selectFolder(value1))
        button2=Button(inputFrame,text="Select",width=BUTTON_WIDTH,command=lambda:selectFolder(value2))
        button3=Button(inputFrame,text="Select",width=BUTTON_WIDTH,command=lambda:selectFolder(value3))
        button1.grid(row=1,column=1,padx=2)
        button2.grid(row=3,column=1,padx=2)
        button3.grid(row=5,column=1,padx=2)
        valueB.set(True)
        buttonB=Checkbutton(inputFrame,text="Have Excel text file",variable=valueB)
        buttonB.grid(row=8,padx=2,pady=4,sticky='e')
    def run(self):
        cmd=["ssPacker",value1.get(),value2.get(),value3.get()]
        if valueB.get():
            cmd.append("-x")
        runPy=threading.Thread(target=running,args=(cmd,None))
        runPy.setDaemon(True)
        runPy.start()
        return 

class setdbsDecrypt:
    def __init__(self):
        clear()
        name1=Label(inputFrame,text="Dbs file:")
        name1.grid(row=0,padx=2,sticky='w')
        value1.set("*.dbs")
        entry1=Entry(inputFrame,width=ENTRY_WIDTH,textvariable=value1)
        entry1.grid(row=1,column=0,padx=2)
        button1=Button(inputFrame,text="Select",width=BUTTON_WIDTH,command=lambda:selectFile(value1))
        button1.grid(row=1,column=1,padx=2)
        valueB.set(False)
        buttonB=Checkbutton(inputFrame,text="Export all data",variable=valueB)
        buttonB.grid(row=8,padx=2,pady=4,sticky='e')
    def run(self):
        cmd=["dbsDecrypt",value1.get()]
        if valueB.get():
            cmd.append("-a")
        runPy=threading.Thread(target=running,args=(cmd,None))
        runPy.setDaemon(True)
        runPy.start()
        return 

class setdbsEncrypt:
    def __init__(self):
        clear()
        name1=Label(inputFrame,text="Dbs.out file:")
        name2=Label(inputFrame,text="Dbs.txt file:")
        name1.grid(row=0,padx=2,sticky='w')
        name2.grid(row=2,padx=2,sticky='w')
        value1.set("*.dbs.out")
        value2.set("")
        entry1=Entry(inputFrame,width=ENTRY_WIDTH,textvariable=value1)
        entry2=Entry(inputFrame,width=ENTRY_WIDTH,textvariable=value2)
        entry1.grid(row=1,column=0,padx=2)
        entry2.grid(row=3,column=0,padx=2)
        button1=Button(inputFrame,text="Select",width=BUTTON_WIDTH,command=lambda:selectFile(value1))
        button2=Button(inputFrame,text="Select",width=BUTTON_WIDTH,command=lambda:selectFile(value2))
        button1.grid(row=1,column=1,padx=2)
        button2.grid(row=3,column=1,padx=2)
        valueC.set('17')
        nameC=Label(inputFrame,text="Compression Level(2-17, 0 for Fake Compression): ")
        nameC.grid(row=7,padx=2,pady=4,sticky='e')
        entryC=Entry(inputFrame,width=4,textvariable=valueC)
        entryC.grid(row=7,column=1,padx=2,pady=4)
    def run(self):
        cmd=["dbsEncrypt",value1.get(),value2.get()]
        try:
            comp=int(valueC.get())
        except:
            comp=0
        else:
            if comp<2:
                comp=0
            elif comp>17:
                comp=17
        if comp!=0:
            cmd.append("-c")
            cmd.append(str(comp))
        runPy=threading.Thread(target=running,args=(cmd,None))
        runPy.setDaemon(True)
        runPy.start()
        return 

class setpckUnpacker:
    def __init__(self):
        clear()
        name1=Label(inputFrame,text="Pck file:")
        name2=Label(inputFrame,text="Output folder:")
        name1.grid(row=0,padx=2,sticky='w')
        name2.grid(row=2,padx=2,sticky='w')
        value1.set("*.pck")
        value2.set("")
        entry1=Entry(inputFrame,width=ENTRY_WIDTH,textvariable=value1)
        entry2=Entry(inputFrame,width=ENTRY_WIDTH,textvariable=value2)
        entry1.grid(row=1,column=0,padx=2)
        entry2.grid(row=3,column=0,padx=2)
        button1=Button(inputFrame,text="Select",width=BUTTON_WIDTH,command=lambda:selectFile(value1))
        button2=Button(inputFrame,text="Select",width=BUTTON_WIDTH,command=lambda:selectFolder(value2))
        button1.grid(row=1,column=1,padx=2)
        button2.grid(row=3,column=1,padx=2)
    def run(self):
        cmd=["pckUnpacker",value1.get(),value2.get()]
        runPy=threading.Thread(target=running,args=(cmd,None))
        runPy.setDaemon(True)
        runPy.start()
        return 
        
class setpckPacker:
    def __init__(self):
        clear()
        name1=Label(inputFrame,text="Folder to pack:")
        name2=Label(inputFrame,text="Output file:")
        name1.grid(row=0,padx=2,sticky='w')
        name2.grid(row=2,padx=2,sticky='w')
        value1.set("")
        value2.set("")
        entry1=Entry(inputFrame,width=ENTRY_WIDTH,textvariable=value1)
        entry2=Entry(inputFrame,width=ENTRY_WIDTH,textvariable=value2)
        entry1.grid(row=1,column=0,padx=2)
        entry2.grid(row=3,column=0,padx=2)
        button1=Button(inputFrame,text="Select",width=BUTTON_WIDTH,command=lambda:selectFolder(value1))
        button2=Button(inputFrame,text="Select",width=BUTTON_WIDTH,command=lambda:setFile(value2))
        button1.grid(row=1,column=1,padx=2)
        button2.grid(row=3,column=1,padx=2)
    def run(self):
        cmd=["pckPacker",value1.get(),value2.get()]
        runPy=threading.Thread(target=running,args=(cmd,None))
        runPy.setDaemon(True)
        runPy.start()
        return 

class setomvCuter:
    def __init__(self):
        clear()
        name1=Label(inputFrame,text="OMV file:")
        name1.grid(row=0,padx=2,sticky='w')
        value1.set("*.omv")
        entry1=Entry(inputFrame,width=ENTRY_WIDTH,textvariable=value1)
        entry1.grid(row=1,column=0,padx=2)
        button1=Button(inputFrame,text="Select",width=BUTTON_WIDTH,command=lambda:selectFile(value1))
        button1.grid(row=1,column=1,padx=2)
    def run(self):
        cmd=["omvCuter",value1.get()]
        runPy=threading.Thread(target=running,args=(cmd,None))
        runPy.setDaemon(True)
        runPy.start()
        return 

class setsiglusOmv:
    def __init__(self):
        clear()
        name1=Label(inputFrame,text="ogv file(Must be YUV444p):")
        name2=Label(inputFrame,text="OMV file:")
        name1.grid(row=0,padx=2,sticky='w')
        name2.grid(row=2,padx=2,sticky='w')
        value1.set("*.ogv")
        value2.set("")
        entry1=Entry(inputFrame,width=ENTRY_WIDTH,textvariable=value1)
        entry2=Entry(inputFrame,width=ENTRY_WIDTH,textvariable=value2)
        entry1.grid(row=1,column=0,padx=2)
        entry2.grid(row=3,column=0,padx=2)
        button1=Button(inputFrame,text="Select",width=BUTTON_WIDTH,command=lambda:selectFile(value1))
        button2=Button(inputFrame,text="Select",width=BUTTON_WIDTH,command=lambda:selectFile(value2))
        button1.grid(row=1,column=1,padx=2)
        button2.grid(row=3,column=1,padx=2)
    def run(self):
        cmd='siglusomv.exe "'+value1.get()+'" "'
        if not value2.get():
            cmd+=value1.get().replace('.ogv','.omv')+'"'
        else:
            cmd+=value2.get()+'"'
        runExe=threading.Thread(target=runningExe,args=(cmd.replace("*",""),))
        runExe.setDaemon(True)
        runExe.start()
        return 

class StdoutRedirector(object):

    def __init__(self, text_area):
        self.text_area = text_area

    def write(self, str):
        self.text_area.insert(END, str)
        self.text_area.see(END)

root=Tk()
root.title("Siglus Tools")
root.geometry("640x480")
#root.resizable(False,False)
lastSelect=0

try:
    f=subprocess.Popen(os.getcwd()+"/siglusomv.exe")
    f.kill()
except:pass
else:
    tool.append("Pack OMV")
    command.append("siglusOmv")

cmdFrame=Frame(root)
cmdFrame.pack(side='bottom',fill='both',padx=PAD,pady=PAD)
cmdScroll=Scrollbar(cmdFrame,bd=2)
cmdScroll.pack(side='right',fill='y',padx=PAD,pady=PAD)
cmdText=Text(cmdFrame,wrap='word',state='disabled',
             yscrollcommand=cmdScroll.set,
             width=86,height=22-len(command),bd=2)
sys.stdout=StdoutRedirector(cmdText)
cmdText.pack(side='left',fill='x')
cmdScroll.config(command=cmdText.yview)

keyFrame=Frame(root)
keyFrame.pack(side='bottom',fill='x')
mainFrame=Frame(root)
mainFrame.pack(side='top',fill='x')
optionFrame=Frame(mainFrame,padx=PAD,pady=PAD)
optionFrame.pack(side='left',fill='both')
rightFrame=Frame(mainFrame,padx=PAD,pady=PAD)
rightFrame.pack(side='right',fill='both')
selectedFrame=Frame(rightFrame)
selectedFrame.pack(side='top')
inputFrame=Frame(rightFrame)
inputFrame.pack(side='top')

value1=StringVar()
value2=StringVar()
value3=StringVar()
valueB=BooleanVar()
valueB1=BooleanVar()
valueB2=BooleanVar()
valueB3=BooleanVar()
valueC=StringVar()

keyVar=StringVar()
tempKey=loadKey()

keyInfo=Frame(keyFrame)
keyInfo.pack(side='top',anchor='w',fill='x')
startButton=Button(keyFrame,text="Start",command=start,width=7)
startButton.pack(side='right',padx=PAD,pady=PAD,anchor='e')

if tempKey:
    DECRYPT_KEY=tempKey
keyVar.set(stringKey(DECRYPT_KEY))
keyEntry=Entry(keyFrame,width=80,textvariable=keyVar)
keyEntry.bind("<Control-Key-v>",unlock)
keyEntry.pack(side='left',padx=PAD,pady=PAD,anchor='w')

try:
    f=subprocess.Popen(os.getcwd()+"/skf.exe")
    f.kill()
except:
    hasSkf=False
else:
    hasSkf=True
    kfButton=Button(keyInfo,text="Find Key",command=clickFindKey,width=8)
    kfButton.pack(side='right',padx=PAD,pady=PAD,anchor='e')

keyLabel=Label(keyInfo,text='Decryption Key(Hex separate by ","):')
keyLabel.pack(side='left',anchor='w')

try:
    listFile=open(KEY_LIST,'r',1,'UTF-8')
except:
    hasList=False
else:
    hasList=True
    keyList=[stringKey(DECRYPT_KEY)]
    keyName=[""]
    for line in listFile.readlines():
        if line[-2:]=='：\n':
            keyName.append(line[:-2])
        elif line!='\n':
            keyList.append(line[:-1])
    listFile.close()
    if hasSkf:
        listWidth=46
    else:
        listWidth=56
    keySelect=Combobox(keyInfo,width=listWidth,state='readonly',value=keyName)
    keySelect.bind("<<ComboboxSelected>>",selectKey)
    keySelect.current(0)
    keySelect.pack(side='left',anchor='e')

optionLabel=Label(optionFrame,text="Select option:")
optionLabel.pack(side='top',anchor='w')
optionVar=StringVar()
optionVar.set(tool)
optionList=Listbox(optionFrame,listvariable=optionVar,height=len(command))
optionList.selection_set(0)
title=Label(selectedFrame,text=tool[0],font=('Fixdsys 14 bold'))
title.pack()
option=setSceneUnpacker()
optionList.bind('<ButtonRelease-1>',select)
optionList.pack(side='top',fill='y')

root.mainloop()
sys.stdout=cmdOutput
