@echo on
for  %%i in (*.ybn) do extYbn.exe -e -ybn %%i -op-call 0x2b -op-msg 0 -txt txt\\%%i.txt -key 00000000
pause