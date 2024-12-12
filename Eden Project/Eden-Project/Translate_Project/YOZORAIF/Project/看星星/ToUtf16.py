import os,sys,re,pdb
nt=[
    ['暁斗','晓斗'],
    ['声','谈话声'],
    ['沙夜','沙夜'],
    ['ひかり','光'],
    ['沙夜の声','沙夜的声音'],
    ['ひかりの声','光的声音'],
    ['店長','店长'],
    ['ころな','可乐娜'],
    ['陽南','阳南'],
    ['球紀','球纪'],
    ['吉岡','吉冈'],
    ['１年生女子部員','1年级女部员'],
    ['他校の１年女子Ｂ','他校的１年级女生Ｂ'],
    ['他校の１年女子Ａ','他校的１年级女生Ａ'],
    ['コタロウ','小太郎'],
    ['武一','武一'],
    ['アマコーの１年女子','AMAKO的一年级女生'],
    ['織姫','织姬'],
    ['知らない人','不知道的人'],
    ['店員さん','店员'],
    ['ヒソヒソ声','窃窃私语声'],
    ['ひかり・沙夜','光・沙夜'],
    ['明光１年女子','明光１年级女生'],
    ['サークルの子Ａ','社团成员Ａ'],
    ['サークルの子Ｂ','社团成员Ｂ'],
    ['加納','加纳'],
    ['鳴枝','鸣枝'],
    ['宙見先輩','宙見学长'],
    ['あなた','你'],
    ['謎の通信','谜之通信'],
    ['アルビレオのプリンセス','天鹅座β的公主'],
    ['サーヤ姫','沙夜公主'],
    ['リヒカ姫','丽希卡公主'],
    ['ぴなみー','日南'],
    ['コメット星人','彗星人'],
    ['サーヤ姫・リヒカ姫','沙夜公主・丽希卡公主'],
    ['銀河皇帝','银河皇帝'],
    ['？？？','？？？']
    ]
def rep(lines):
    ret=[]
    for l in lines:
        if(len(l)==0):
            continue
        else:
            ret.append(l)
    return ret

def test():
    i=0
    for i in range(len(nt)):
        print(nt[i][1])

def RebuildTable(lines):
    ret=[]
    for l in lines:
        sig=0
        if l.startswith('//'):
            l=l[2:]
            i=0
            for i in range(len(nt)):
                if (l==nt[i][0]):
                    ret.append("//%LF"+l)
                    sig=1
                    break
                else:
                    i+=1
            if(sig==0):
                ret.append('//'+l+"%K%P")
        else:
            i=0
            for i in range(len(nt)):
                if (l==nt[i][0]):
                    l=l.replace(nt[i][0],nt[i][1])
                    ret.append("%LF"+l)
                    sig=1
                    break
                elif (l==nt[i][1]):
                    ret.append("%LF"+l)
                    sig=1
                    break
                else:
                    i+=1
            if(sig==0):
                ret.append(l+"%K%P")
    return ret


path1='mix'
path2='utf16'
for f in os.listdir(path1):
    print(f)
    fs1=open(os.path.join(path1,f),'rb')
    ls=RebuildTable(rep(fs1.read().decode('u8').split('\n')))
    fs1.close()
    fs1=open(os.path.join(path2,f),'wb')
    fs1.write('\n'.join(ls).encode('u16'))
    fs1.close()
