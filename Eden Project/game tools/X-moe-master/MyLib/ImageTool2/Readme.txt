ImageTool(ver 0.2a):
输入文件支持jpg bmp png三种格式
输出支持bmp png两种格式

Usage:
--bmp       指定输出:--bmp 或者 --png
back.png    背景图（位于最底层）
2           前景图的个数(越早放入的前景图，位于越底层，以此类推)
fore1.png   
20          x坐标
20          y坐标
fore2.jpg   （fore1.png的部分会被fore2挡住）
40
40
output.bmp  格式和指定的一样


TODO:
修复UTF-8输入障碍（拖

xmoe.project@gmail.com
