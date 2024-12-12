SET PYTHON=C:\Python33\python

for %%f in (*-sn.bin) DO %PYTHON% %~dp0extract_snbin.py %%f

pause
