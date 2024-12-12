import numpy as np
from PIL import Image
 
# 24位转8位灰度
image1 = Image.open('c_mata.bmp')
image2 = Image.fromarray(np.uint8(image1))
print(image2.mode)
t = image2.convert("L")
print(t.mode)
image3 = Image.fromarray(np.uint8(t)*255)
print(image3.mode)
image3.save('c_mata.bmp.bmp')
image3.close()
