[loadplugin module=wuvorbis.dll]
[current page=fore layer=message0]

[playbgm storage="bgm01.ogg"]
[cm]
������BGM�J�n[p]

[playbgm storage="bgm01.ogg" start=���x��0 loop=false]
[setbgmstop target=*bgmend]
[setbgmlabel name=���x��1 exp='dm("BGM���x��1�ʉ�")']
[setbgmlabel name=���x��2 target=*bgmlabel2 exp='dm("BGM���x��2�ʉ�")']
[cm]
�r������BGM�J�n[p]
[cm]
�҂���
[s]

*bgmlabel2|
BGM�̃��x��2��ʉ߂����̂ŃW�����v���Ă���
[s]

*bgmend|
BGM���I�������̂ŃW�����v���Ă���[p]

*start|
[cm]
�V�K�W�����v�����̃e�X�g[r]
[link target=*link]�ʏ�̃����N[endlink]
[timeout time=5000 target=*timeout se=musicbox.ogg]
[wheel   target=*wheel   se=musicbox.ogg]
[click    target=*click   se=musicbox.ogg]
[s]

*link|
[cm]
�����N����΂�܂���[p]
[jump target=*start]

*timeout|
[cm]
�^�C���A�E�g����΂�܂���[p]
[jump target=*start]

*wheel|
[cm]
�z�C�[������΂�܂���[p]
[jump target=*start]

*click|
[cm]
�N���b�N����΂�܂���[p]
[jump target=*start]
