@loadplugin module=wuvorbis.dll
@l
@call storage=gvolume.ks
@gvolmenu name="BGM(&B)" control=bgm
@gvolmenu name="���ʉ����̂P(&1)" control=0
@gvolmenu name="���ʉ����̂Q(&2)" control=1
*start|�͂���
@startanchor
@ct
[playbgm storage=c:\mss\musicbox.ogg loop=false]\
[wl canskip=true][l]\
*1|�P
@ct
[fadeinbgm time=6000 storage="c:\mss\trk.wav"]\
���y���t�F�[�h�C�����Ȃ���Đ�[l]
[wb canskip=true][l]\
@pausebgm
@l
@resumebgm
@l
*1b|�P��
@ct
���y���t�F�[�h�C�����Ȃ���Đ������B[l]
*2|�Q
@ct
[xchgbgm storage="c:\mss\trk.ogg" time=5000 overlap=5000]\
���y���N���X�t�F�[�h
[wb][l]\
;[xchgbgm storage="c:\mss\trk_e_03.ogg" outtime=5000 intime=0 overlap=3000]\
;[xchgbgm storage="c:\mss\trk_e_03.ogg" overlap=5000]\
[xchgbgm storage="c:\mss\trk_e_03.ogg" overlap=5000 volume=50]\
���y���N���X�t�F�[�h
[wb canskip=true][l]\
[l]\
*2b|�Q��
@ct
���y���N���X�t�F�[�h�����B[l]
[fadebgm volume=100 time=5000]\
���y��100%���ʂ܂Ńt�F�[�h
[wb][l]\
*3|�R
@ct
[fadebgm volume=50 time=5000]\
���y��50%���ʂ܂Ńt�F�[�h
[wb][l]\
*3b|�R��
@ct
���y��50%���ʂ܂Ńt�F�[�h�����B[l]
*4|�S
@ct
[bgmopt volume=100]\
���y��100%���ʂɂ����B[l]
*4b|�S��
@ct
���y��100%���ʂɂ������̂Q�B[l]
*5|�T
@ct
@stopbgm
���y���~�����B[l]
*5b|�T��
@ct
���y���~�������̂Q�B[l]
*6|�U
@ct
[playbgm storage="c:\mss\trk.wav"]\
���y���Đ������B[l]\
*6b|�U��
@ct
���y���Đ��������̂Q�B[l]
*7|�V
@ct
[fadeoutbgm time=4000]\
���y���t�F�[�h�A�E�g����[l]
[wb]\
*7b|�V��
@ct
���y���t�F�[�h�A�E�g����[l]
[s]
