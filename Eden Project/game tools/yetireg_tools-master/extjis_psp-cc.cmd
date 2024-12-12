SET PYTHON="D:\Program Files (x86)\Microsoft Visual Studio\Shared\Python37_64\python"

for %%f in (*.ops) DO %PYTHON% %~dp0extjis_psp-cc.py %%f

pause
