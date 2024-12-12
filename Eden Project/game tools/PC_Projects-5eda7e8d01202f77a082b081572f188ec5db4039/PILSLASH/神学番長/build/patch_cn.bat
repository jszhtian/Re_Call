@echo off
title 神学番長 汉化补丁

rem 2014.2.7    by kid	忘了1个月==补坑。。。
rem 2013.12.30  by kid	测试版～元旦是啥，好吃咩


if not exist BOOTUP.DAT (
	echo 未找到文件!请放到exe所在的同目录下!
	goto END
)

echo 备份原文件
if not exist BOOTUP.DAT.org (
	ren BOOTUP.DAT BOOTUP.DAT.org
)
move /Y patch_cn\BOOTUP.DAT BOOTUP.DAT

echo 应用补丁,须一段时间,请耐心等候!
for /f "delims= " %%i in (list.txt) do (
if not exist DATA\%%i.dat.org (
	ren DATA\%%i.dat %%i.dat.org
)
echo Patch %%i.dat...
xdelta3.exe -f -d -s DATA\%%i.dat.org patch_cn\%%i.diff DATA\%%i.dat
IF %errorlevel% NEQ 0 (goto FAILED)
)
IF %errorlevel% NEQ 0 (goto FAILED)

echo. 
echo 补丁应用成功！
echo 请使用 "神学番長完全版_cn.exe" 运行汉化版~
goto END

:FAILED
echo. 
echo 补丁应用失败！请检查目录！

:END
del xdelta3.exe
del list.txt
rd /S/Q patch_cn
pause
del patch_cn.bat
