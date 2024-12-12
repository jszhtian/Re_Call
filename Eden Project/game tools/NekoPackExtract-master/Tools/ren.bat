@echo on
for /f "delims=" %%i in ('*.png') do ren "%%i" "00%%i"
pause