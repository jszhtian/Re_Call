#!/usr/bin/env python3

"""
1,解包mp4为帧图片
2,单帧处理为png
3,加背景颜色打底
4,合成目标mp4
"""

import os, shutil, sys
from PIL import Image

def cleardir(dir):
    if os.path.exists(dir):
        shutil.rmtree(dir)
    shutil.os.makedirs(dir)    

def out_mp4_frames(mp4, dest):
    cleardir(dest)
    os.system('ffmpeg -i ' + mp4 + ' -r 24 -q:v 2 -f image2 ' + dest + '/%d.jpeg')

def transparents(src, dest):
    cleardir(dest)
    for each in os.listdir(src):
        transparent(src + '/' + each, dest + '/' + each.replace('jpeg', 'png'))

def transparent(jpeg, dest):
    simg = Image.open(jpeg)
    ssz = simg.size
    nw = int(ssz[0]/2)    
    # 拆成两张图
    imgl = simg.crop((0, 0, nw, ssz[1])).convert('L')
    imgr = simg.crop((nw, 0, ssz[0], ssz[1]))
    r,g,b = imgr.split()
    oimg = Image.merge('RGBA', [r, g, b, imgl])
    oimg.save(dest)

def overlays(src, dest, color):
    cleardir(dest)
    for each in os.listdir(src):
        overlay(src + '/' + each, dest + '/' + each.replace('png', 'jpeg'), color)

def overlay(src, dest, color):    
    simg = Image.open(src)
    cimg = Image.new('RGBA', simg.size, color)
    cimg.paste(simg, mask=simg)
    cimg.convert('RGB').save(dest)

def compose(src, mp4):
    if os.path.exists(mp4):
        shutil.os.unlink(mp4)
    os.system('ffmpeg -f image2 -r 24 -i '  + src + '/%d.jpeg ' + mp4)

if __name__ == "__main__":
    inf = sys.argv[1]
    outf = sys.argv[2]
    shutil.os.makedirs('tmp', exist_ok=True)
    out_mp4_frames(inf, 'tmp/frames')
    transparents('tmp/frames', 'tmp/pngs')
    overlays('tmp/pngs', 'tmp/overlays', 0x00ff00ff)
    compose('tmp/overlays', outf)
    shutil.rmtree('tmp')