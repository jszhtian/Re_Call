call Config.cmd
@echo off

copy /y "%PC_RESOURCE%\game.ext.org" "%PC_RESOURCE%\game.ext"
python ..\src\ratw.py -i "%PC_RESOURCE%\game.ext" "%PC_RESOURCE%\game.dat" ..\chs
move /y head.bin ..\output\game.ext
pause
