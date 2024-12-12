#encoding=utf-8
import hcbParser
import bytefile
import os

extOriginalScript = False

fnt={
    0x00000004:"真紅",
    0x00000124:"加奈",
    0x00000322:"",
    0x00000332:"澪",
    0x000004F5:"",
    0x00000505:"鏡",
    0x00000645:"つかさ",
    0x000007DB:"蓮",
    0x000008EE:"白",
    0x00000A01:"鈴",
    0x00000C00:"あゆむ",
    0x00000DC3:"時雨",
    0x00000E9B:"女１",
    0x00000EE1:"担任教師",
    0x00000F37:"女の子",
    0x00000FB0:"祖母",
    0x00001029:"とおる",
    0x00001125:"お母さん",
    0x0000119E:"藍",
    0x00001217:"祟り",
    0x000012C4:"蓮也",
    0x0000133D:"悠馬",
    }

fs=open(r'World.hcb','rb')
stm=bytefile.ByteFile(fs.read())
parser=hcbParser.HcbParser(stm)

if extOriginalScript:
    txt=parser.Parse()
    fs=open('World.txt','wb')
    fs.write(txt.encode('u16'))
    fs.close()
else:
    parser.SetFuncNameTable(fnt,'932')
    os.chdir('World')
    #first: Find PushStr x1.2
    #secend: FInd PushStr help_sys, the next func
    #thread:PushStr graph_bs的内容所在函数
    #4th:找加载staff图（PushStr Staffxxxx）所在的函数，往下找一个函数
    #5th:找文本开始的函数
    parser.ParseTxt([(2763, 2783), (2927, 3036)], 3355)
