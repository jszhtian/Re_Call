@if exp="typeof(global.zoomrot_object) == 'undefined'"
@iscript

/*
	背景/前景画像の拡大縮小効果による表示を行うプラグイン
*/

class ZoomRotPlugin extends KAGPlugin
{
	var cx, cy;                        // 回転中心
	var sr, ss, sx, sy;                // 初期位置
	var dr, ds, dx, dy;                // 最終位置
	var m00, m01, m10, m11, mtx, mty;  // アフィン変換行列パラメータ
	var startTick;                     // 開始ティック
	var time;                          // ズームを行っている時間
	var accel;                         // 加速度的な動きを行うか ( 負 : 0 : 正 )
	var storage;                       // 画像名
	var key;                           // カラーキー
	var moving = false;                // 動作中フラグ
	var nextstop;                      // 次回停止フラグ
	var moveFunc;                      // 移動位置計算用関数
	var targetLayerName;               // 対象レイヤ名
	var targetLayer;                   // 対象レイヤ
	var targetPage;                    // 対象ページ（表／裏画面）
	var tempLayer;                     // テンポラリレイヤ

	function ZoomRotPlugin(window)
	{
		super.KAGPlugin();
		this.window = window;
	}

	function finalize()
	{
		// finalize メソッド
		// このクラスの管理するすべてのオブジェクトを明示的に破棄
		stop();

		invalidate tempLayer if tempLayer !== void;

		super.finalize(...);
	}

	function startZoomRot(
		storage, key, layer='0', page='fore',
		cx='center', cy='center',
		sr='0.0', ss='1.0', sx='center', sy='center',
		dr='0.0', ds='1.0', dx='center', dy='center',
		time='1000', accel='1.0' )
	{
		// storage : 表示画像
		// layer : 対象レイヤ
		// cx, cy : 回転中心位置（画像上）
		// sr ss sx sy : 初期位置
		// dr ds dy dy : 最終位置
		// time : ズームを行っている時間
		// accel : 加速をつけるかどうか

		var base = window.fore.base;

		// 既存の動作を停止
		stop();

		// テンポラリレイヤを確保
		{
			this.storage = storage;
			if( key == '' )
				this.key = clNone;
			else if( key == 'adapt' )
				this.key = clAdapt;
			else if( (key[0] == '0') && (key[0] == 'x') )
				this.key = +key;
			else
				this.key = (+key) + 0x3000000;
			
			if(tempLayer === void)
			{
				tempLayer = new Layer(window, base);
				tempLayer.loadImages(this.storage, clNone);
			}
		}

		// 対象レイヤを設定
		{
			this.targetLayerName = layer;
			this.targetPage = page;
			if( layer == 'base')
			{
				if( page == 'back' )
					this.targetLayer = window.back.base;
				else
					this.targetLayer = window.fore.base;
			}
			else
			{
				if( page == 'back' )
					this.targetLayer = window.back.layers[+layer];
				else
					this.targetLayer = window.fore.layers[+layer];
				
				targetLayer.face = dfBoth;
				targetLayer.type = ltTransparent;
				targetLayer.setImageSize( base.imageWidth, base.imageHeight );
				targetLayer.setPos( 0,0, base.imageWidth,base.imageHeight );
			}
		}

		// オブジェクトにパラメータを設定
		{
			// 回転中心
			if( (cx == '') || (cx == 'center') )
				this.cx = tempLayer.imageWidth / 2;
			else if( typeof +cx == 'Real' )
				this.cx = tempLayer.imageWidth * +cx;
			else
				this.cx = (real)cx;
			
			if( (cy == '') || (cy == 'center') )
				this.cy = tempLayer.imageHeight / 2;
			else if( typeof +cy == 'Real' )
				this.cy = tempLayer.imageHeight * +cy;
			else
				this.cy = (real)cy;
			
			// 初期位置
			if( sr == '' )
				this.sr = (real)0;
			else if( typeof +sr == 'Real' )
				this.sr = Math.PI * +sr;
			else
				this.sr = (Math.PI/180) * +sr;
			
			if( ss == '' )
				this.ss = (real)1;
			else
				this.ss = (real)ss;
			
			if( (sx == '') || (sx == 'center') )
				this.sx = targetLayer.imageWidth / 2;
			else if( typeof +sx == 'Real' )
				this.sx = targetLayer.imageWidth * +sx;
			else
				this.sx = (real)sx;
			
			if( (sy == '') || (sy == 'center') )
				this.sy = targetLayer.imageHeight / 2;
			else if( typeof +sy == 'Real' )
				this.sy = targetLayer.imageHeight * +sy;
			else
				this.sy = (real)sy;
			
			// 最終位置
			if( dr == '' )
				this.dr = (real)0;
			else if( typeof +dr == 'Real' )
				this.dr = Math.PI * +dr;
			else
				this.dr = (Math.PI/180) * +dr;
			
			if( ss == '' )
				this.ds = (real)1;
			else
				this.ds = (real)ds;
			
			if( (dx == '') || (dx == 'center') )
				this.dx = targetLayer.imageWidth / 2;
			else if( typeof +dx == 'Real' )
				this.dx = targetLayer.imageWidth * +dx;
			else
				this.dx = (real)dx;
			
			if( (dy == '') || (dy == 'center') )
				this.dy = targetLayer.imageHeight / 2;
			else if( typeof +dy == 'Real' )
				this.dy = targetLayer.imageHeight * +dy;
			else
				this.dy = (real)dy;
			
			// 時間設定
			if( time == '' )
				this.time = 1000;
			else
				this.time = (int)time;
			
			if( accel == '' )
				this.accel = (real)0;
			else
				this.accel = (real)accel;
		}
		
		// 移動位置計算関数の設定
		moveFunc = defaultMover;

		// 初期位置に表示
		moveFunc(moveAt, 0);
		targetLayer.visible = true;

		// 開始
		startTick = System.getTickCount();
		System.addContinuousHandler(continuousHandler);
		moving = true;
		nextstop = false;
	}

	function moveAt( m00, m01, m10, m11, myx, mty )
	{
		// m00 m01 m10 m11 mtx mty 位置に移動

		targetLayer.visible = true;
		// レイヤを一度クリアする
		targetLayer.fillRect( 0,0, targetLayer.imageWidth,targetLayer.imageHeight, 0 );
		// アフィン変換転送
		targetLayer.affineCopy(
				tempLayer, 0, 0, tempLayer.imageWidth, tempLayer.imageHeight, 'true',
				m00, m01, m10, m11, mtx, mty, 'stNearest'
			);
	}

	/*static*/ function defaultMover(func, tm)
	{
		// 位置計算
		// tm は 0.0(開始点) ～ 1.0(終了点) の間で変化する変数なので、
		// これを元にして位置を計算する
		var r = (dr - sr) * tm + sr;
		var s = (ds - ss) * tm + ss;
		var x = (dx - sx) * tm + sx;
		var y = (dy - sy) * tm + sy;
		
		var rc = Math.cos(r);
		var rs = Math.cos((Math.PI/2.0) - r);
		
		m00 = s * rc;
		m01 = s * -rs;
		m10 = s * rs;
		m11 = s * rc;
		mtx = (m00*-cx) + (m10*-cy) + x;
		mty = (m01*-cx) + (m11*-cy) + y;
		
		// 移動
		func(m00, m01, m10, m11, mtx, mty);
	}

	function continuousHandler(tick)
	{
		// ハンドラ
		if(nextstop)
		{
			// 終了
			finish();
			return;
		}

		// 時間を得る
		var tm = tick - startTick;
		tm /= time;
		if(tm >= 1)
		{
			nextstop = true;
			tm = 1;
		}
		else
		{
			// 加速計算
			if(accel < 0)
			{
				// 上弦 ( 最初が動きが早く、徐々に遅くなる )
				tm = 1.0 - tm;
				tm = Math.pow(tm, -accel);
				tm = 1.0 - tm;
			}
			else if(accel > 0)
			{
				// 下弦 ( 最初は動きが遅く、徐々に早くなる )
				tm = Math.pow(tm, accel);
			}
		}

		// 移動
		moveFunc(moveAt, tm);
	}

	function finish()
	{
		// 最終位置に移動
		moveFunc(moveAt, 1.0);
		
		// 回転ズームの終了
		stop(); // 停止
	}

	function stop()
	{
		// 停止
		if(moving)
		{
			window.trigger('zoomrot');
			System.removeContinuousHandler(continuousHandler);
			moving = false;
		}
		invalidate tempLayer if tempLayer !== void;
		tempLayer = void;
		targetLayer = void;
	}

	function onStore(f, elm)
	{
		// 栞を保存するとき
	}

	function onRestore(f, clear, elm)
	{
		// 栞を読み出すとき
		stop(); // 動作を停止
	}

	function onStableStateChanged(stable)
	{
	}

	function onMessageHiddenStateChanged(hidden)
	{
	}

	function onCopyLayer(toback)
	{
	}

	function onExchangeForeBack()
	{
	}
}

kag.addPlugin(global.zoomrot_object = new ZoomRotPlugin(kag));
	// プラグインオブジェクトを作成し、登録する

@endscript
@endif

; マクロ登録
@macro name="zoomrot"
@eval exp="zoomrot_object.startZoomRot(mp.storage,mp.key, mp.layer,mp.page, mp.cx,mp.cy, mp.sr,mp.ss,mp.sx,mp.sy, mp.dr,mp.ds,mp.dx,mp.dy, mp.time,mp.accel)"
@endmacro

@macro name="wzoomrot"
@if exp="zoomrot_object.moving"
@waittrig * name="zoomrot" onskip="zoomrot_object.finish()"
@endif
@endmacro

@return
