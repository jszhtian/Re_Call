*first|�ŏ�
@startanchor
[rclick name="�T�u���[�`���̃e�X�g" call=true target=*sub1]\
�E�N���b�N�̃e�X�g�Ȃ�[[l]
�Ȃ�[[l]

*a
[link target=*hogera]�I�����P[endlink]
[link]�I�����Q[endlink]
[s]

*hogera
��[l]
@jump target=*a

*sub1
@trace exp="kag.fore.layers[0].loadedProvinceActions"
@tempsave
; ���ꎞ�I�ɏ�Ԃ�ۑ�
@history output=false
; �����b�Z�[�W�����ւ̏o�͂𖳌���
@mapdisable layer=0 page=fore
; ���N���b�J�u���}�b�v�������g���Ă���ꍇ�͂��̂悤�ɂ��Ė���������
;@locklink
; �� �I�����I���̃��b�N
@backlay
@layopt layer=message1 page=back visible=true
; �����̃T�u���[�`�����ł̓��b�Z�[�W���C��1���g��
@layopt layer=message0 page=back visible=false
@current layer=message1 page=back
@position left=0 top=0 width=640 height=480
@eval exp="f.r_first=true"
; �����̃��[�`���ɓ������Ƃ��ɂ����g�����W�V�������s���悤��
;
*menu
@er
@nowait
[link target=*hide]���b�Z�[�W������[endlink][r]
[link target=*history]���b�Z�[�W����������[endlink][r]
[link target=*load]�x�����ǂ�[endlink][r]
[link target=*save]�x���͂���[endlink][r]
[link target=*gotostart]�ŏ��ɖ߂�[endlink][r]
[link target=*ret]�߂�[endlink][r]
@endnowait
@current layer=message1 page=fore
@if exp="f.r_first"
@trans time=500 rule=trans1 vague=128
@wt
@endif
@eval exp="f.r_first=false"
@s

*ret
; �T�u���[�`������߂�
@tempload bgm=false se=false backlay=true
@trace exp="kag.back.layers[0].loadedProvinceActions"
@trans time=500 rule=trans1 vague=128
@wt
@return

*hide
; ���b�Z�[�W������
@hidemessage
@jump target=*menu

*history
; ���b�Z�[�W����������
@showhistory
@jump target=*menu

*load
; �x�����ǂ�
; emb exp= .... �ɂ��Ă� TJS Tips ���Q��
@er
@nowait
[link target=*lt0][emb exp="kag.getBookMarkPageName(0)"][endlink][r]
[link target=*lt1][emb exp="kag.getBookMarkPageName(1)"][endlink][r]
[link target=*lt2][emb exp="kag.getBookMarkPageName(2)"][endlink][r]
[link target=*lt3][emb exp="kag.getBookMarkPageName(3)"][endlink][r]
[link target=*lt4][emb exp="kag.getBookMarkPageName(4)"][endlink][r]
[link target=*menu]�߂�[endlink][r]
@endnowait
@s

*lt0
@load place=0 ask=true
@jump target=*menu
*lt1
@load place=1 ask=true
@jump target=*menu
*lt2
@load place=2 ask=true
@jump target=*menu
*lt3
@load place=3 ask=true
@jump target=*menu
*lt4
@load place=4 ask=true
@jump target=*menu

*save
; �x���͂���
; emb exp= .... �ɂ��Ă� TJS Tips ���Q��
@er
@nowait
[link target=*st0][emb exp="kag.getBookMarkPageName(0)"][endlink][r]
[link target=*st1][emb exp="kag.getBookMarkPageName(1)"][endlink][r]
[link target=*st2][emb exp="kag.getBookMarkPageName(2)"][endlink][r]
[link target=*st3][emb exp="kag.getBookMarkPageName(3)"][endlink][r]
[link target=*st4][emb exp="kag.getBookMarkPageName(4)"][endlink][r]
[link target=*menu]�߂�[endlink][r]
@endnowait
@s

*st0
@save place=0 ask=true
@jump target=*menu
*st1
@save place=1 ask=true
@jump target=*menu
*st2
@save place=2 ask=true
@jump target=*menu
*st3
@save place=3 ask=true
@jump target=*menu
*st4
@save place=4 ask=true
@jump target=*menu


*gotostart
; �u�ŏ��ɖ߂�v
@gotostart ask=true
@jump target=*menu
