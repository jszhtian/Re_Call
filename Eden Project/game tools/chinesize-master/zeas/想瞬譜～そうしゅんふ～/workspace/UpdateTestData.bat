@echo off
rem �����ֶ�������ϷĿ¼
set GAMEPATH=H:\TDDOWNLOAD\soxp

set SELFPATH=%~dp0
set GAMEEXE=%GAMEPATH%\sohsyunfu_chs.exe
set pypath="%~dp0python\python.exe"
if not exist %pypath% ( echo I can't find "%~dp0python\python.exe" !
pause
goto :EOF
)

if "%1"=="" goto :ASM_TEXT_DIR

:LOOP
%pypath% "%SELFPATH%asm.py" "%SELFPATH%code\%~n1.code" -resfile "%1" -o "%SELFPATH%Output\%~n1.mjo"
shift
if %1=="" goto :PACK
goto :LOOP

:ASM_TEXT_DIR
for %%a in ("%SELFPATH%text\*.txt") do %pypath% "%SELFPATH%asm.py" "%SELFPATH%code\%%~na.code" -resfile "%%a" -o "%SELFPATH%Output\%%~na.mjo"

:PACK
echo ���Ҫ�ֶ������ļ�����ֱ�ӹرմ���
pause
rem call "%SELFPATH%packqliefp1.bat" "%SELFPATH%output" "%GAMEPATH%\GameData\data8.pack"
copy /y "%SELFPATH%Output\*.mjo" "%GAMEPATH%"

echo �������
pause
rem start "%GAMEEXE%"