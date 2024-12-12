call Config.cmd
@echo off

python ..\src\ratw.py -e "%PC_RESOURCE%\game.ext" "%PC_RESOURCE%\game.dat" ..\resource
pause
