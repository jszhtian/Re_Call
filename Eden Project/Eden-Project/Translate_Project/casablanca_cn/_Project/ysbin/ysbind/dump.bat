md text
md json
for %%i in (*.ybn) do extYbn.exe -e -ybn %%i -json json\\%%i.json -txt text\\%%i.txt -op-msg 108 -op-call 43
pause