rem krkr.exe �����𑜓x�Ή��ς݃}�j�t�F�X�g��������Ԃɂ���o�b�`�t�@�C��
copy krkr.exe tvpwin32.exe
mt.exe -manifest highdpi.manifest -outputresource:tvpwin32.exe;#1
copy /b tvpwin32.exe+"..\..\src\core\environ\win32\optionarea.bin" krkr.exe /y
