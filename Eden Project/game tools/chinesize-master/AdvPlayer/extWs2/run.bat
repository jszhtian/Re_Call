for %%i in (ws2\*.ws2) do extWs2.exe -e "AdvHD.exe" -i %%i -o "txt\%%~ni.txt" -only-txt -name-prefix "%%LC" -index-file "idx\%%~ni.idx"

pause