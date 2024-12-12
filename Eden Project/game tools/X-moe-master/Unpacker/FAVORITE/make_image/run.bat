@echo off
for %%s in (Img\\*.bmp) do FVPCompress.exe "%%s" "graph.bin"
@echo Process Finished
@echo on
@echo OK!
pause