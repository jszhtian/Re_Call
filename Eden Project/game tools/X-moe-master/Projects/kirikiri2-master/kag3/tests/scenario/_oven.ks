;@mappfont storage="D:\Documents and Settings\Administrator\�f�X�N�g�b�v\font.tft"
;[deffont size=24 antialiased=false bold=false]\
;[position vertical=true]\
@image storage="_24" page=fore layer=base
;@image storage="pagebreak" layer=0 left=100 top=100 visible=true fliplr=true
;@image storage="linebreak" layer=1 left=150 top=100 visible=true flipud=true
;@image storage="linebreak" layer=2 left=200 top=100 visible=true flipud=true fliplr=true
;@position vertical=true
@wait time=300
@eval exp="f.hoge = [1,2,3]"
*s0|&'�d�q�����W�̗��j'
@ct
[link hint="�d�q�����W�̗��j"]�d�q�����W�V���j[endlink][l]
��[edit length=420]
@eval exp="kag.fore.messages[0].links[1].object.focus()"
[checkbox] �`�F�b�N�{�b�N�X
;@eval exp="kag.conductor.callLabel('*sub1')"
@call target="&'*sub1'"
[nowait]�ނ��[endnowait]�΂��g��Ȃ������ɁA[l]�肩��[link hint="�C"]�u�C�v[endlink]�ƌĂ΂��[link hint="���m"]���m[endlink]�Ȃ���̂���o���ĕ������߂Ă���B[p]
@jump target=*s1 storage=_oven2.ks

*sub1
[DEFFONT FACE="�l�r �o�S�V�b�N"][resetfont]�����̉��n�ɖ���[deffont face="user"][resetfont]�΂��g��Ȃ�[link hint="����"]����[endlink]���Z��ł���B[l]
@return
