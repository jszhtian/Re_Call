call Config.cmd
@echo off

for %%i in ("..\resource\*.lsb") do (
echo Extract %%i...
python ..\src\ratw.py -et %%i
)
pause