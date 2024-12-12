@echo off
set EXE=Noli_me_tangere.exe
set ORGDIR="E:\神学番長完全版\Data"

for /f "delims= " %%i in (list.txt) do (
echo %%i

del %%i.dat
copy /Y %ORGDIR%\%%i.dat.org %%i.dat
%EXE% -i %%i.dat
rem copy /Y %%i.dat %ORGDIR%\%%i.dat
)

del BOOTUP.DAT
copy /Y %ORGDIR%\..\BOOTUP.DAT.org BOOTUP.DAT
%EXE% -it3 BOOTUP
%EXE% -i BOOTUP.DAT

pause
