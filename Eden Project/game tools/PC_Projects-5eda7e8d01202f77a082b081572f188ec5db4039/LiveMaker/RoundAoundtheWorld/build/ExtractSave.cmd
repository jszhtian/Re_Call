call Config.cmd
@echo off

python ..\src\ratw.py -e "%PC_RESOURCE%\save.dat" "%PC_RESOURCE%\save.dat" ..\resource\save
pause
