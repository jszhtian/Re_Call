[if exp="typeof(global.EXLControl) == 'undefined'"]
[iscript]
/******************************************************************************
EXLayerControl.ks v0.1.1
前景レイヤをテキトーな方法で拡張操作するプラグイン

使い方等は付属のreadme.txtを参照してください。
*******************************************************************************/


class EXLayerControl extends KAGPlugin
{
	var window;	// ウィンドウ参照
	
	var targetLayer;	// 操作対象レイヤ
	var tmpLayer;	// 操作用一時レイヤ
	
	var layerNumber;	// 操作中のレイヤ番号
	var targetPage;	// 操作対象ページ
	
	function EXLayerControl(win)
	{
		// コンストラクタ
		
		super.KAGPlugin(win);
		
		window = win;
		
		tmpLayer = new Layer(window, window.fore.base);
		
		tmpLayer.visible = false;
		tmpLayer.enabled = true;
		tmpLayer.focusable = false;
	}
	
	function finalize()
	{
		// ファイナライザ
		
		invalidate tmpLayer if (tmpLayer !== void);
		
		super.finalize(...);
	}
	
	function setTargetLayer(lnum, page)
	{
		// 操作対象レイヤ/ページを確定する
		
		var base = (page == 'fore') ? window.fore : window.back;
		
		if (base.layers[lnum] === void) {
			throw new Exception("不明なレイヤーです。");
			return false;
		}
		targetLayer = base.layers[lnum];
		targetPage = page;
		layerNumber = lnum;
	}
	
	function expand(base, rate, nx, ny)
	{
		// レイヤを伸縮
		
		if (base == "") {
			tmpLayer.assignImages(targetLayer);
			tmpLayer.setSizeToImageSize();
		}
		else {
			tmpLayer.loadImages(base);
			tmpLayer.setSizeToImageSize();
		}
		
		var nw = tmpLayer.width * rate;
		var nh = tmpLayer.height * rate;
		
		targetLayer.setImageSize(nw, nh);
		targetLayer.setSizeToImageSize();
		targetLayer.setPos(nx, ny);
		targetLayer.stretchCopy(
			0,
			0,
			targetLayer.width,
			targetLayer.height,
			tmpLayer,
			0,
			0,
			tmpLayer.width,
			tmpLayer.height,
			stNearest
		);
	}
	
	function rotate(base, angle, ax=0, ay=0)
	{
		// レイヤを回す（ように見せる）
		// 正の角度で半時計回り、負の角度で時計回りになる。
		
		if (base == "") {
			tmpLayer.assignImages(targetLayer);
			tmpLayer.setSizeToImageSize();
		}
		else {
			tmpLayer.loadImages(base);
			tmpLayer.setSizeToImageSize();
		}
		
		var wd = tmpLayer.width;
		var hg = tmpLayer.height;
		
		targetLayer.setImageSize(wd, hg);
		targetLayer.setSizeToImageSize();
		targetLayer.fillRect(0, 0, wd, hg, 0x00000000);
		
		var angle = getRadian(angle);
		
		targetLayer.affineCopy(
			tmpLayer,
			0,
			0,
			wd,
			hg,
			true,
			Math.cos(angle), Math.sin(angle) * -1,
			Math.sin(angle), Math.cos(angle),
			(0 - Math.sin(angle) * hg) / 2 + ax,
			(hg - (Math.sin(angle) * -1 * wd + Math.cos(angle) * hg)) / 2 + ay
		);
	}
	
	function trimAndExpand(base, left=0, top=0, width=100, height)
	{
		// 画像の一部を拡大表示
		
		if (base == "") {
			tmpLayer.assignImages(targetLayer);
			tmpLayer.setSizeToImageSize();
		}
		else {
			tmpLayer.loadImages(base);
			tmpLayer.setSizeToImageSize();
		}
		
		
		if (width == 0) {
			width = height * (tmpLayer.width / tmpLayer.height);
		}
		else {
			height = width * (tmpLayer.height / tmpLayer.width);
		}
		
		dm(
			"scoped (" + left + ", " + top + ")(" + 
			Math.round(left * 1 + width * 1) + ", " + Math.round(top * 1 + height * 1) + ")"
		);
		targetLayer.stretchCopy(
			0, 0,
			targetLayer.width,
			targetLayer.height,
			tmpLayer,
			left, top,
			width, height
		);
	}
}

kag.addPlugin(global.EXLControl = new EXLayerControl(kag));
[endscript]
[endif]

[macro name="exl_expand"]
[eval exp="global.EXLControl.setTargetLayer(mp.layer, mp.page)"]
[eval exp="global.EXLControl.expand(mp.origin, mp.rate, mp.left, mp.top)"]
[endmacro]

[macro name="exl_rotate"]
[eval exp="global.EXLControl.setTargetLayer(mp.layer, mp.page)"]
[eval exp="global.EXLControl.rotate(mp.origin, mp.angle, mp.left * 1, mp.top * 1)"]
[endmacro]

[macro name="exl_scope"]
[eval exp="global.EXLControl.setTargetLayer(mp.layer, mp.page)"]
[eval exp="global.EXLControl.trimAndExpand(mp.origin, mp.cutx, mp.cuty, mp.cutw, mp.cuth)"]
[endmacro]

[return]
