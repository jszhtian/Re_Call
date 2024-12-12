[wait time=200]
*start|スタート
[cm]
[loadplugin module="gfxEffect.dll"]
[iscript]
System.setArgument("-contfreq", 240);
kag.fire = new gfxFire();
kag.fire.randomSeed = 0; // 毎回同じになるように乱数のシード設定
class EffectHandler
{
	var		moving = false; // タイマーが始動しているかどうか
	var		targetLayer;
	var		seedLayer;
	var		lastTick;
	var		move_pos;

	function EffectHandler(targetLayer,seedLayer) {
		this.targetLayer = targetLayer;
		this.seedLayer = seedLayer;
		this.moving = false;
		this.move_pos = 0;

		kag.fire.init( targetLayer, 1 );
		kag.fire.setFireSeed( seedLayer, 0 );
		kag.fire.setFireSeedPos( seedLayer.left, seedLayer.top, 0 );
		targetLayer.face = dfAddAlpha;
		targetLayer.type = ltAddAlpha;
	}

	function setColorTable(colorLayer) {
		kag.fire.setCustomColorTable( colorLayer );
	}

	function finalize() { stop(); }

	function start() {
		if(moving==false) {
			lastTick = System.getTickCount();
			System.addContinuousHandler(handler); // ContinuousHandlers に追加
			moving = true;
		}
	}

	function stop() {
		if(moving) {
			System.removeContinuousHandler(handler);
			moving = false;
		}
	}

	function handler(tick) {
		var		interval = tick - lastTick;
		if( interval > 16 ) {
			kag.fire.cycle();
			//targetLayer.update();
			lastTick = tick;
			if( this.move_pos ) {
				kag.fire.seedX += move_pos;
				if( kag.fire.seedX < 50 )
					move_pos = 1;
				if( kag.fire.seedX > 175 )
					move_pos = -1;
			}
		}
	}

	function DoMoving() {
		move_pos = 1;
	}
}
[endscript]

[macro name=show_framed_message_area]
[position layer="message1" page="fore" visible="false"]
[position layer="message1" page="back" visible="false"]
[position layer="message0" page="fore" left=10 top=490 width=780 height=100 visible="true" frame="" opacity=128]
[current layer="message0" page="fore"]
[endmacro]

[image storage=black480 page=fore layer=1 visible=false left=160 top=0]
;[image storage=black512 page=fore layer=1 visible=false left=160 top=0]
[image storage=honoo page=fore layer=0 visible=false left=111 top=224]
[image storage=table page=fore layer=2 visible=true]

[show_framed_message_area]
[iscript]
kag.effect = new EffectHandler(kag.fore.layers[1],kag.fore.layers[0]);
kag.add( kag.effect );
[endscript]

[cm]
開始します[p]
[iscript]
kag.fire.textureFilterType = 0;
kag.fire.coolRange = 1;
kag.fire.coolStrength = 160;
kag.fire.updateCoolMap();
kag.effect.start();
[endscript]
[layopt page=fore layer=1 visible=true]

[cm]
青系に変えます。[p]
[iscript]
kag.fire.applyBlue();
[endscript]

[cm]
白系に変えます。[p]
[iscript]
kag.fire.applyWhite();
[endscript]

[cm]
独自色テーブル指定に変えます。[p]
[iscript]
kag.effect.setColorTable( kag.fore.layers[2] );
[endscript]

[cm]
赤系に戻します。[p]
[iscript]
kag.fire.applyRed();
[endscript]

[cm]
バイリニアフィルタにします。[p]
[iscript]
kag.fire.textureFilterType = 1;
[endscript]

[cm]
左から風を与えます。[p]
[iscript]
kag.fire.forceH = -3;
kag.fire.updateWarpMap();
[endscript]

[cm]
右から風を与えます。[p]
[iscript]
kag.fire.forceH = 1;
kag.fire.updateWarpMap();
[endscript]

[cm]
上から風を与えます。[p]
[iscript]
kag.fire.forceH = -1;
kag.fire.forceV = -1;
kag.fire.updateWarpMap();
[endscript]

[cm]
元に戻します。[p]
[iscript]
kag.fire.forceH = -1;
kag.fire.forceV = 0;
kag.fire.updateWarpMap();
[endscript]

[cm]
消火。[p]
[iscript]
kag.fire.clearFireSeed();
[endscript]

[cm]
終了[p]
[iscript]
kag.effect.stop();
[endscript]
[s]

