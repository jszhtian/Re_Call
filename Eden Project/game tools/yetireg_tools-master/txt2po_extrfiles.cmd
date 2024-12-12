goto :main

:fun
C:\Sites\djangostack\python\python C:\Sites\djangostack\python\scripts\txt2po -o %~n1.po %~nf1
goto :eof

:main
for %%f in (*-extr.txt) do call :fun %%f
