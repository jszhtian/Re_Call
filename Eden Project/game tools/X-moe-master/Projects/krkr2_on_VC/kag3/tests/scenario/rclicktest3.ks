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
@locksnapshot
; ���X�i�b�v�V���b�g�����b�N����
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
[link exp="kag.loadBookMarkFromFileWithAsk()"]�x�����ǂ�[endlink][r]
[link exp="kag.saveBookMarkToFileWithAsk()"]�x���͂���[endlink][r]
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
@unlocksnapshot
@return

*hide
; ���b�Z�[�W������
@hidemessage
@jump target=*menu

*history
; ���b�Z�[�W����������
@showhistory
@jump target=*menu

*gotostart
; �u�ŏ��ɖ߂�v
@gotostart ask=true
@jump target=*menu
