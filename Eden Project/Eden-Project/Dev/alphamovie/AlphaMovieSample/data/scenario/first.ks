[wait time=200]
*start|�X�^�[�g
[cm]
[position layer="message1" page="fore" visible="false"]
[position layer="message1" page="back" visible="false"]
[position layer="message0" page="fore" left=10 top=490 width=780 height=100 visible="true" frame="" opacity=128]
[position layer="message0" page="back" visible="false"]

[iscript]
System.setArgument("-contfreq", 240);
[endscript]
[image storage=seara01 page=fore layer=0 visible=true left=160 top=0]
[image storage=bg layer=base page=fore visible=true]
[call storage="alphamovie.ks"]

*particle
[cm]
��������g�����v�������_�����O�̃p�[�e�B�N�����J�n���܂��B[p]
[playamov slot=0 storage="particle.amv" loop=false]
[amovopt slot=0 visible=true]
[wam]
[amovopt slot=0 visible=false]
[cm]

*water
�����΂��܂��B[p]
[playamov slot=0 storage="water.amv" loop=false]
[amovopt slot=0 visible=true]
[wam]
[amovopt slot=0 visible=false]
[cm]

*explosion
�������܂��B[p]
[playamov slot=0 storage="explosion.amv" loop=false]
[amovopt slot=0 visible=true]
[wam]
[amovopt slot=0 visible=false]
[cm]

*magic_circle
�����w��\�����܂��B[p]
[playamov slot=0 storage="maho2.amv" loop=false]
[amovopt slot=0 visible=true]
[wam]
[amovopt slot=0 visible=false]
[cm]
����1��[p]
[jump target="*particle"]

; �ȉ��A�T���v���X�N���v�g
;[amovopt slot=0 opacity=0]
;[playamov slot=0 storage="00.amv" loop=true]
;[amovmove slot=0 fps=30 path="(15,255)"]
;[wm]
;[p]
;[stopamov slot=0]
;[playamov slot=0 storage="01.amv" loop=false]
;[nextamov slot=0 storage="01.amv" loop=true]
;[amovopt slot=0 loop=false]
;[wam slot=0]
;
[s]


