@echo off
rem �����ֶ�������ϷĿ¼
set GAMEPATH="D:\wa2"
rem ����ɹ����ʱ�Զ�������Ϸ
set RUNAFTERSUCCESS=no

set SELFPATH=%~dp0
set GAMEEXE=%GAMEPATH%\WA2_chs.EXE
set pypath="%~dp0python\python.exe"
rem set pypath="D:\Tools\Program\python-2.6.5\python.exe"
if not exist %pypath% ( echo I can't find "%~dp0python\python.exe" !
pause
goto :EOF
)

if "%1"=="" goto :ASM_TEXT_DIR

:LOOP
%pypath% "%SELFPATH%join.py" "%1" "%SELFPATH%script\%~n1.txt"
shift
if %1=="" goto :PACK
goto :LOOP

:ASM_TEXT_DIR
for %%a in ("%SELFPATH%text\*.inf") do %pypath% "%SELFPATH%join.py" "%%a" "%SELFPATH%output\%%~na.txt"

:PACK
%pypath% "%SELFPATH%KCAPPAK.py" "%SELFPATH%output" "%GAMEPATH%\WA2_chs.pak"

:END
echo �������
pause
if %RUNAFTERSUCCESS%==yes (
cd /d %GAMEPATH%
start %GAMEEXE%
)
