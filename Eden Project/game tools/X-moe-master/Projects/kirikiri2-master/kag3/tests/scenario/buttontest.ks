@call storage="rclick_tjs.ks"
@loadplugin module="extrans.dll"
@macro name=fimg
@image *
@eval exp="sf[mp.storage]=1"
@endmacro
; �O���t�B�J���{�^���̃e�X�g
@fimg storage=_24 page=fore layer=base
@wait time=200
*first|"buttontest.ks"�̍ŏ�<�d�v>
@nowait
@backlay
@fimg storage=_24_3 page=back layer=base
@current page=back layer=message0
@position left=0 top=0 width=310 height=480 draggable=true
[edit length=200 maxchars=6]
@locate x=100 y=100
@button graphic=YesButton target=*button1 recthit=false
@locate x=200 y=100
@button graphic=NoButton target=*button2 recthit=false
@locate x=10 y=200
@r
[edit length=200]
[link target=*link1]���ʂ̃����N1[endlink]
[edit length=200]
[checkbox opacity=20] �`�F�b�N�{�b�N�X
[link target=*link2]���ʂ̃����N2[endlink]
[edit length=200]
@current page=back layer=message1
@layopt page=back layer=message1 visible=true
@position left=320 top=0 width=310 height=480 draggable=true
[checkbox] �`�F�b�N�{�b�N�X
[edit length=200]
@locate x=100 y=100
@button graphic=YesButton target=*button1
@locate x=200 y=100
@button graphic=NoButton target=*button2
@locate x=10 y=200
@r
[link target=*link1]���ʂ̃����N1[endlink]
[link target=*link2]���ʂ̃����N2[endlink]
[edit length=200]
[checkbox] �`�F�b�N�{�b�N�X
@endnowait
@trans method=turn time=3000 maxsize=40
@wt
;@forelay
@current page=fore layer=message0
@s

*button1
@backlay
@layopt page=back layer=message1 visible=false
@fimg storage=_24 page=back layer=base
@current page=back
@position left=0 top=0 width=640 height=480
@er
@nowait
�{�^���P\
@endnowait
@trans method=wave time=1500
@wt
@current page=fore
@l
@jump target=*first

*button2
@backlay
@layopt page=back layer=message1 visible=false
@fimg storage=_24 page=back layer=base
@current page=back
@position left=0 top=0 width=640 height=480
@er
@nowait
�{�^���Q\
@endnowait
@trans method=wave time=1500
@wt
@current page=fore
@l
@jump target=*first

*link1
@backlay
@layopt page=back layer=message1 visible=false
@fimg storage=_24 page=back layer=base
@current page=back
@position left=0 top=0 width=640 height=480
@er
@nowait
�����N�P\
@endnowait
@trans method=wave time=1500
@wt
@current page=fore
@l
@jump target=*first

*link2
@backlay
@layopt page=back layer=message1 visible=false
@fimg storage=_24 page=back layer=base
@current page=back
@position left=0 top=0 width=640 height=480
@er
@nowait
�����N�Q\
@endnowait
@trans method=wave time=1500
@wt
@current page=fore
@l
@jump target=*first
