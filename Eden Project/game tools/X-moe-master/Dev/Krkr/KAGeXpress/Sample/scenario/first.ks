*start
;--------------------------------------
;           Logo��ʾ
;--------------------------------------
*logo
@menul
@bg storage=logo time=1500
@wait time=1000

;--------------------------------------
;           �˵�����
;--------------------------------------
*menu
@clickskip enabled=false
@startanchor
@menul
@bg storage=menu time=2000
@wait time=100
@font size=32
@nowait
@locate x=120 y=180
@link storage=script.ks target=*scenario
����Ϸ
@endlink
@locate x=120 y=250
@link exp="kag.close()"
������Ϸ
@endlink
@resetfont
@endnowait
@clickskip enabled=true
@s
