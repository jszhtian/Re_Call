@call storage="rain.ks"
@image layer=base page=fore storage=d:\temp\testbg02
*start1|�J�͔�\��
�J���ӂ点�܂�[l]
@backlay
@raininit forevisible=false backvisible=true
@trans method=crossfade time=2000
@wt
;
;@jump target=*test
;
*start2|�J�͕\����
�J��\�����ł��B[l]�ǂ�ǂ�B[l]�ǂ�ǂ�B[l]
�݂�݂�݂�݂�݂�݂�݂�݂�[l]
�w�i�����ւ��܂�[l]
@backlay
@image layer=base page=back storage=d:\temp\testbg01
@trans method=crossfade time=2000
@wt
@l
@backlay
@image layer=base page=back storage=d:\temp\testbg02
@trans children=false method=crossfade time=2000
@wt
@l
�J����܂��܂�[l]
@backlay
@rainopt backvisible=false
@trans method=crossfade time=2000
@wt
@rainuninit
@jump target=*start1



*test
@layopt layer=message0 page=fore visible=false
@backlay
@image layer=base page=back storage=_24
@trans children=false method=crossfade time=2000
@wt
@backlay
@image layer=base page=back storage=_24_3
@trans children=false method=crossfade time=2000
@wt
@jump target=*test

