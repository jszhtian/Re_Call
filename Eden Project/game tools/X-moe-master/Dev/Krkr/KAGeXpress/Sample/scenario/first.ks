*start
;--------------------------------------
;           Logo显示
;--------------------------------------
*logo
@menul
@bg storage=logo time=1500
@wait time=1000

;--------------------------------------
;           菜单画面
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
新游戏
@endlink
@locate x=120 y=250
@link exp="kag.close()"
结束游戏
@endlink
@resetfont
@endnowait
@clickskip enabled=true
@s
