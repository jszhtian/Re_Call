;------------------------------------------------------------
;800x600文字框设定文件    
;------------------------------------------------------------
;全屏幕透明文字框（菜单、特典等用）
@macro name=menul
[position layer=%layer|%layer|message0 page=fore visible=true opacity=0 top=0 height=600 left=0 width=800 marginl=0 margint=0 marginr=0 marginb=0 frame=%frame| ]
@endmacro

;电子小说文字框
@macro name=val
[position layer=%layer|%layer|message0 page=fore visible=true opacity=128 top=15 height=570 left=20 width=760 marginl=60 margint=45 marginr=24 marginb=30 frame=%frame| ]
@endmacro

;对话文字框
@macro name=advl
[position layer=%layer|%layer|message0 page=fore visible=true opacity=128 top=360 height=150 left=20 width=760 marginl=16 margint=8 marginr=10 marginb=8 frame=%frame| ]
@endmacro

;文字点击换行
@macro name=lre
@l
@r
@er
@endmacro

@return
