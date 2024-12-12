set EXE=Noli_me_tangere.exe
set ORGDIR="E:\神学番長完全版\Data"

copy /Y %ORGDIR%\ban_text\*. ban_text\
%EXE% -it ban_text

pause
