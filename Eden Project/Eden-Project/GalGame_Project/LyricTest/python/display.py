
from PIL import Image, ImageDraw, ImageFont
import imageio

fontName= r'.\simhei.ttf'

# 把每个文字与它的三个运动结合为一个基本单位
def newTextMotion(char, posFunc, sizeFunc, colorFunc):
    tm={}
    tm['char']=char
    tm['posFunc']= posFunc
    tm['sizeFunc']= sizeFunc
    tm['colorFunc']= colorFunc
    return tm

# 在指定的时间，计算文字的位置、大小、颜色等
def showText(img, textMotion, time):
    char= textMotion['char'] 
    pos= textMotion['posFunc'](time)
    size= textMotion['sizeFunc'](time)
    color= textMotion['colorFunc'](time)
    font= ImageFont.truetype(fontName, size)
    draw = ImageDraw.Draw(im=img)
    textSize= draw.textsize(text=char, font=font)
    tx= pos[0]- textSize[0]// 2
    ty= pos[1]- textSize[1]// 2
    draw.text(xy=(tx, ty), text=char, fill=color, font=font)

# 文字缩小
def makeTextShrink(char, toSize, toPos, toColor, offset, dur):
    def colorFunc(time):
        if time< offset:
            return (0,0,0,0)
        if time> offset+ dur:
            return toColor
        return toColor[:-1] + (50+ round((time-offset)/dur*200),)
    def sizeFunc(time):
        if time< offset:
            return toSize* 8
        if time> offset+ dur:
            return toSize
        return toSize*8 - round((time-offset)/dur* toSize*7.5)
    def posFunc(time):
        if time< offset:
            return (0,0)
        if time> offset+ dur:
            return toPos
        # return (toPos[0], round((time-offset)/dur*toPos[1]))
        return toPos
    return newTextMotion(char, posFunc, sizeFunc, colorFunc)

# 抛物线降落（有一个回弹效果）
def makeTextParaDrop(char, toSize, toPos, toColor, offset, dur):
    def colorFunc(time):
        if time< offset:
            return (0,0,0,0)
        if time> offset+ dur:
            return toColor
        return toColor[:-1] + (50+ round((time-offset)/dur*200),)
    def sizeFunc(time):
        if time< offset:
            return toSize
        if time> offset+ dur:
            return toSize
        return toSize
    def posFunc(time):
        if time< offset:
            return (toPos[0], 0)
        if time> offset+ dur:
            return toPos
        r= 0.75
        dur2= dur
        a= toPos[1]/(dur2* dur2* (1- 2* r))
        b= -2* a* dur2* r
        x= (time-offset)
        return (toPos[0], round(a* x* x+ b*x))
    # print(toPos)
    return newTextMotion(char, posFunc, sizeFunc, colorFunc)

def getTextFrame(tmList, time):
    textImg= Image.new('RGBA', (1280, 720))
    for tm in tmList:
        showText(textImg, tm, time) 
    return textImg

# 一行文字，给定所有参数，配置运动函数与延时
def getMotionList(text, fontSize, fontColor, startPos, fromTime, dur, func):
    tmList=[]
    inter= round(dur/ len(text))
    for i in range(len(text)):
        char= text[i]
        pos= (startPos[0]+ i* fontSize+ 10, startPos[1])
        color= fontColor
        # tm= makeTextDropMotion(char, fontSize, pos, color, 150*i)
        tm= func(char, fontSize, pos, color, fromTime+inter*i, dur)
        tmList.append(tm)
    return tmList

def showTextDrop(text, startPos, func):
    fontSize= 50
    color=(255, 255,  0, 255)
    tmList= getMotionList(text, fontSize, color, startPos, 0, 1000, func)
    frames=[]
    outfilename='temp.gif'
    for i in range(0, 2000, 50):
        print(i)
        img= Image.new('RGB', (640, 360))
        # img= Image.open('back.png').resize((640, 360), Image.ANTIALIAS)
        # img = img.convert("RGB")   
        textImg= getTextFrame(tmList, i)
        r, g, b, a= textImg.split()
        img.paste(textImg, (0,0), mask= a)
        str1= 'tempAA.png'
        img.save(str1)
        im = imageio.imread(str1)
        frames.append(im)
    # writeGif('temp.gif', frames, duration=0.1, subRectangles=False) #
    imageio.mimsave(outfilename, frames, 'GIF', duration=0.05) # 生成方式也差不

if __name__=='__main__':
    # showTextDrop('淡妆浓抹总相宜', (150,200), makeTextParaDrop)
    showTextDrop('淡妆浓抹总相宜', (150,200), makeTextParaDrop)


