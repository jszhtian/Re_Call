@call storage=staffroll.ks
@image storage=_24 page=fore layer=base
*loop
@staffrollinit
@font size=50 color=0xff0000
@staffrolltext x=100 y=480 text="��������"
@resetfont
@staffrolltext x=100 y=80 text="�X�^�b�t�P"
@staffrolltext x=100 y=40 text="�X�^�b�t�Q"
@staffrolltext x=100 y=40 text="�X�^�b�t�R"
@staffrolltext x=100 y=40 text="�X�^�b�t�S"
@staffrolltext x=100 y=40 text="�X�^�b�t�T"
@staffrolltext x=100 y=40 text="�X�^�b�t�U"
@staffrolltext x=100 y=40 text="�X�^�b�t�V"
@staffrolltext x=100 y=40 text="�X�^�b�t�W"
@staffrolltext x=100 y=40 text="�X�^�b�t�X"
@staffrolltext x=100 y=40 text="�X�^�b�t�P�O"
@staffrolltext x=100 y=40 text="�X�^�b�t�P�P"
@staffrolltext x=100 y=40 text="�X�^�b�t�P�Q"
@staffrolltext x=100 y=40 text="�X�^�b�t�P�R"
@staffrolltext x=100 y=40 text="�X�^�b�t�P�S"
@staffrolltext x=100 y=40 text="�X�^�b�t�P�T"
@staffrolltext x=100 y=40 text="�X�^�b�t�P�U"
@staffrolltext x=100 y=40 text="�X�^�b�t�P�V"
@staffrolltext x=100 y=40 text="�X�^�b�t�P�W"
@staffrolltext x=100 y=40 text="�X�^�b�t�P�X"
@staffrolltext x=100 y=40 text="�X�^�b�t�Q�O"
@staffrollimage x=100 y=40 storage=YesButton
@l
@staffrollstart height=1500 time=20000
@wait time=20000
@staffrolluninit
@l
@jump target=*loop

*vertical
@position vertical=true
@call storage=staffroll.ks
@image storage=_24 page=fore layer=base
*loopv
@staffrollinit
@font size=50 color=0xff0000
@staffrolltext y=100 x=640 text="��������"
@resetfont
@staffrolltext y=100 x=80 text="�X�^�b�t�P"
@staffrolltext y=100 x=40 text="�X�^�b�t�Q"
@staffrolltext y=100 x=40 text="�X�^�b�t�R"
@staffrolltext y=100 x=40 text="�X�^�b�t�S"
@staffrolltext y=100 x=40 text="�X�^�b�t�T"
@staffrolltext y=100 x=40 text="�X�^�b�t�U"
@staffrolltext y=100 x=40 text="�X�^�b�t�V"
@staffrolltext y=100 x=40 text="�X�^�b�t�W"
@staffrolltext y=100 x=40 text="�X�^�b�t�X"
@staffrolltext y=100 x=40 text="�X�^�b�t�P�O"
@staffrolltext y=100 x=40 text="�X�^�b�t�P�P"
@staffrolltext y=100 x=40 text="�X�^�b�t�P�Q"
@staffrolltext y=100 x=40 text="�X�^�b�t�P�R"
@staffrolltext y=100 x=40 text="�X�^�b�t�P�S"
@staffrolltext y=100 x=40 text="�X�^�b�t�P�T"
@staffrolltext y=100 x=40 text="�X�^�b�t�P�U"
@staffrolltext y=100 x=40 text="�X�^�b�t�P�V"
@staffrolltext y=100 x=40 text="�X�^�b�t�P�W"
@staffrolltext y=100 x=40 text="�X�^�b�t�P�X"
@staffrolltext y=100 x=40 text="�X�^�b�t�Q�O"
@staffrollimage y=100 x=40 storage=YesButton
@l
@staffrollstart height=1800 time=20000
@wait time=20000
@staffrolluninit
@l
@jump target=*loopv
