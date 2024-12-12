import os,sys,re,pdb,cv2,tkinter
import matplotlib.pyplot as plt
import matplotlib.patheffects as path_effects

#解决中文乱码问题
plt.rcParams["font.sans-serif"] = ["SimHei"]
#解决负号显示问题
plt.rcParams["axes.unicode_minus"] = False

def plot_stoke():
    fig = plt.figure(figsize=(5,2))
    text = fig.text(0.5,0.5,"Python 大家好!",
                    color="white",ha="center",va="center",size=30)
    text.set_path_effects([path_effects.Stroke(linewidth=5,foreground="blue"),path_effects.Normal()])
    plt.show()
plot_stoke()