@if exp="typeof(global.rclickconfig_object) == 'undefined'"
@iscript
// 嘔クリックでの�O協鮫中を TJS2 で�g�Fするサンプル
// 2002/6/14 井 個夛 :
//   トランジション��鬋乂`ジョン?個

// �櫃� 28 ��旋喘辛嬬

// Config.tjs の�O協を參和のようにする駅勣あり
//  freeSaveDataMode = false
//  saveThumbnail = true
//  numBookMarks = 28

class RButtonLayer extends ButtonLayer
	// parent に onClick イベントを僕るようにしたボタンレイヤ
{
	var tag;

	function RButtonLayer(window, parent)
	{
		super.ButtonLayer(window, parent);
		focusable = false;
	}

	function finalize()
	{
		super.finalize(...);
	}

	function onClick()
	{
		// トランジション嶄ならスキップ
		if(kag.transCount != 0)
		{
			kag.stopAllTransitions();
			return;
		}

		super.onClick(...);
		if(enabled)
			parent.onButtonClick(this);
	}
}

class SaveDataItemLayer extends KAGLayer
{
	// �殯���匯��に��鬉垢襯譽ぅ�
	var num; // �欸�催
	var bgColor = 0xa0ffffff; // 嘘尚弼 ( 0xAARRGGBB )
	var focusedColor = 0xffffffff;

	var commentEdit; // コメントのエディット
	var protectCheckBox; // 仝デ�`タ隠�o々チェックボックス

	function SaveDataItemLayer(window, parent, num)
	{
		super.KAGLayer(window, parent);

		this.num = num;

		setImageSize(500, 112); // サイズ
		face = dfBoth;
		fillRect(0, 0, imageWidth, imageHeight, bgColor);
		setSizeToImageSize();

		hitType = htMask;
		hitThreshold = 0; // 畠囃音邑�^

		cursor = kag.cursorPointed;

		focusable = true; // フォ�`カスは鞭け函れる

		protectCheckBox = new CheckBoxLayer(window, this);
		protectCheckBox.width = 16;
		protectCheckBox.height = 16;
		protectCheckBox.color = 0xffffff;
		protectCheckBox.opacity = 64;
		protectCheckBox.textColor = 0x000000;
		protectCheckBox.setPos(398, 34);
		protectCheckBox.visible = true;

		protectCheckBox.checked = kag.bookMarkProtectedStates[num];

		commentEdit = new EditLayer(window, this);

		commentEdit.setPos(180, 79);
		commentEdit.width = 310;
		commentEdit.height = 18;
		commentEdit.color = 0xffffff;
		commentEdit.opacity = 64;
		commentEdit.textColor = 0x000000;
		commentEdit.visible = true;

		if(kag.scflags.bookMarkComments !== void)
			commentEdit.text = kag.scflags.bookMarkComments[num];

		font.height = 14;
		face = dfBoth;

		drawText(420, 35, "デ�`タ隠�o", 0x000000);

		// 桑催を燕幣
		var str = string (num + 1);
		font.height = 20;
		var ty = font.getTextHeight(str);
		drawText(6, ( imageHeight - ty ) \ 2, str, 0);

		// サムネイル鮫�颪鰌iみ�zむ
		var tmplayer = new global.Layer(window, parent);

 		var tnname = kag.getBookMarkFileNameAtNum(num);

		if(Storages.isExistentStorage(tnname))
		{
			tmplayer.loadImages(tnname);
		}
		else
		{
			tmplayer.setImageSize(133, 100);
			var str = "デ�`タなし";
			var tx = tmplayer.font.getTextWidth(str);
			tmplayer.drawText((tmplayer.imageWidth - tx) \ 2, 40,
				str, 0xffffff);
		}

		copyRect(32, 6, tmplayer, 0, 0, tmplayer.imageWidth, tmplayer.imageHeight);

		invalidate tmplayer;

		// �櫃留４翆�を燕幣
		font.height = 14;

		var str = kag.bookMarkNames[num];
		if(str == '') str = 'デ�`タなし';
		drawText(180, 15, str, 0x000000);

		// 晩原を燕幣
		if(kag.bookMarkDates[num] == '')
			str = "晩原 : ----/--/-- --:--", commentEdit.enabled = false;
		else
			str = "晩原 : " + kag.bookMarkDates[num];

		drawText(180, 35, str, 0x000000);

		// コメント : 
		drawText(180, 63, "Comments :", 0x000000);
	}

	function finalize()
	{
		invalidate commentEdit;
		invalidate protectCheckBox;
		super.finalize(...);
	}

	function saveToSystemVariable()
	{
		// 彜�Bをシステム�篳�に���hする
		if(kag.scflags.bookMarkComments === void)
			kag.scflags.bookMarkComments = [];
		kag.scflags.bookMarkComments[num] = commentEdit.text;
		kag.bookMarkProtectedStates[num] = protectCheckBox.checked;
	}

	function onPaint()
	{
		super.onPaint(...);

		// update() が柵ばれた瘁、宙鮫の岷念に柵ばれる
		face = dfBoth;
		if(focused)
		{
			fillRect(0, 0, imageWidth, 2, focusedColor);
			fillRect(0, 2, 2, imageHeight - 2, focusedColor);
			fillRect(imageWidth - 2, 2, 2, imageHeight - 2, focusedColor);
			fillRect(2, imageHeight - 2, imageWidth - 4, 2, focusedColor);
		}
		else
		{
			fillRect(0, 0, imageWidth, 2, bgColor);
			fillRect(0, 2, 2, imageHeight - 2, bgColor);
			fillRect(imageWidth - 2, 2, 2, imageHeight - 2, bgColor);
			fillRect(2, imageHeight - 2, imageWidth - 4, 2, bgColor);
		}
	}

	function onFocus()
	{
		// フォ�`カスを誼た
		super.onFocus(...);
		update();
	}

	function onBlur()
	{
		// フォ�`カスを払った
		super.onBlur(...);
		update();
	}

	function onHitTest(x, y, process)
	{
		if(process)
		{
			// 嘔ボタンが兀されていたときにイベントを邑�^
			if(System.getKeyState(VK_RBUTTON))
				super.onHitTest(x, y, false);
			else
				super.onHitTest(x, y, true);
		}
	}

	function onKeyDown(key, shift, process)
	{
		// トランジション嶄ならスキップ
		if(kag.transCount != 0)
		{
			kag.stopAllTransitions();
			return;
		}

		// キ�`が兀された
		if(process && key == VK_RETURN || key == VK_SPACE)
		{
			// スペ�`スキ�`またはエンタ�`キ�`
			super.onKeyDown(key, shift, false);
			saveToSystemVariable();
			parent.onLoadOrSave(num);
		}
		else
		{
			// process が false の��栽は�I尖は佩わない
			super.onKeyDown(...);
		}
	}

	function onMouseDown(x, y, button, shift)
	{
		// トランジション嶄ならスキップ
		if(kag.transCount != 0)
		{
			kag.stopAllTransitions();
			return;
		}

		super.onMouseDown(...);
		if(button == mbLeft)
		{
			focus();
			saveToSystemVariable();
			parent.onLoadOrSave(num);
		}
	}
}

class RClickConfigLayer extends Layer // �O協鮫中レイヤ
{
	var closeButton; // �]じる ボタン
	var saveButton; // セ�`ブ ボタン
	var loadButton; // ロ�`ド ボタン
	var hideMessageButton; // メッセ�`ジを��す ボタン
	var saveDataItems;
	var state = -1; // 0 = メインメニュ�`  1 = ロ�`ド鮫中  2 = セ�`ブ鮫中
	var owner; // RClickConfigPlugin オブジェクトへの歌孚
	var currentPage = 0; // セ�`ブデ�`タ�x�k嶄に燕幣嶄のペ�`ジ
	var returnButton; // もどる ボタン
	var pageButtons; // セ�`ブデ�`タのペ�`ジボタン

	function RClickConfigLayer(win, par, owner)
	{
		super.Layer(win, par);
		this.owner = owner;

		// レイヤの彜�Bを兜豚晒
		setImageSize(640, 480);
		clearBase();
		setSizeToImageSize();
		setPos(0, 0);
		hitType = htMask;
		hitThreshold = 0; // 畠囃音邑�^
	}

	function finalize()
	{
		clear();

		invalidate closeButton if closeButton !== void;
		invalidate saveButton if saveButton !== void;
		invalidate loadButton if loadButton !== void;
		invalidate hideMessageButton if hideMessageButton !== void;

		invalidate returnButton if returnButton !== void;

		super.finalize(...);
	}

	function clearBase()
	{
		// 和仇を�Tり岷す
		fillRect(0, 0, imageWidth, imageHeight, 0xc0000000);
	}

	function makeMainMenu()
	{
		// メインメニュ�`の燕幣
		if(state != 0)
		{
			clear();
			state = 0;

			/*
				ボタンを恬撹しています
				ここでは猟忖ボタンを恬撹してますが、
				鮫�颯椒織鵑鰈�りたいときは
			if(closeButton === void)
			{
				closeButton = new RButtonLayer(window, this);
				closeButton.showFocusImage = true;
				closeButton.loadImages("ボタン鮫�颯侫．ぅ訝�");
				closeButton.left = 270;
				closeButton.top = 350;
				closeButton.focusable = true;
			}
				みたいな湖じで��峰します

				ここで峺協するボタンは噸宥のボタン鮫��
				( ３つの彜�Bが罪に�Kんでいる鮫�� ) ではなくて、
				４つの彜�B ( 宥械、兀された�r、マウスが貧にあるとき、
				フォ�`カスがある�r ) が罪に�Kんだ鮫�颪任△覬慴�が
				あります。とくに恷瘁の仝フォ�`カスがあるとき々の
				彜�Bは、キ�`ボ�`ドで荷恬をする繁のために駅勣です
				( showFocusImage=true にすると４つを罪にならべた
				  鮫�颪鯤荒辰垢襪茲Δ砲覆蠅泙�;容�X ) 
			 */

			// 仝�]じる々ボタンを恬撹
			if(closeButton === void)
			{
				closeButton = new RButtonLayer(window, this);
				closeButton.width = 100;
				closeButton.height = 25;
				closeButton.caption = "�]じる";
				closeButton.captionColor = 0xffffff;
				closeButton.left = 270;
				closeButton.top = 350;
				closeButton.focusable = true;
			}
			closeButton.visible = true;

			// 仝セ�`ブ々ボタンを恬撹
			if(saveButton === void)
			{
				saveButton = new RButtonLayer(window, this);
				saveButton.width = 100;
				saveButton.height = 25;
				saveButton.caption = "セ�`ブ";
				saveButton.captionColor = 0xffffff;
				saveButton.left = 270;
				saveButton.top = 100;
				saveButton.focusable = true;
			}
			saveButton.enabled = kag.canStore();
			saveButton.visible = true;

			// 仝ロ�`ド々ボタンを恬撹
			if(loadButton === void)
			{
				loadButton = new RButtonLayer(window, this);
				loadButton.width = 100;
				loadButton.height = 25;
				loadButton.caption = "ロ�`ド";
				loadButton.captionColor = 0xffffff;
				loadButton.left = 270;
				loadButton.top = 150;
				loadButton.focusable = true;
			}
			loadButton.enabled = kag.canRestore();
			loadButton.visible = true;

			// 仝メッセ�`ジを��す々ボタンを恬撹
			if(hideMessageButton === void)
			{
				hideMessageButton = new RButtonLayer(window, this);
				hideMessageButton.width = 100;
				hideMessageButton.height = 25;
				hideMessageButton.caption = "メッセ�`ジを��す";
				hideMessageButton.captionColor = 0xffffff;
				hideMessageButton.left = 270;
				hideMessageButton.top = 200;
				hideMessageButton.focusable = true;
			}
			hideMessageButton.visible = true;
		}
	}

	function clearSaveDataItems()
	{
		// セ�`ブデ�`タ燕幣のクリア
		if(saveDataItems !== void)
		{
			for(var i = 0; i < saveDataItems.count; i++)
			{
				saveDataItems[i].saveToSystemVariable();
				invalidate saveDataItems[i];
			}
			saveDataItems = void;
			kag.setBookMarkMenuCaptions();
		}
	}

	function makeSaveDataItems()
	{
		// セ�`ブデ�`タの燕幣
		clearSaveDataItems();
		saveDataItems = [];
		for(var i = 0; i < 4; i++)
		{
			var obj = new SaveDataItemLayer(window, this, currentPage * 4 + i);
			saveDataItems[i] = obj;
			obj.setPos(140 - 4, i * 120 + 4);
			obj.visible = true;
		}
	}

	function clearPageButtons()
	{
		// ペ�`ジボタンのクリア
		if(pageButtons !== void)
		{
			for(var i = 0; i < pageButtons.count; i++)
			{
				invalidate pageButtons[i];
			}
			pageButtons = void;
		}
	}

	function makePageButtons()
	{
		// ペ�`ジボタンを恬撹する
		clearPageButtons();
		pageButtons = [];
		for(var i = 0; i < 7; i++)
		{
			// ペ�`ジボタンは鮫�颪任發茲い�もしれません
			// その��栽は�F壓のペ�`ジを燕すボタン鮫�颪鯔�と
			// �笋┐襪箸茲い�と
			var obj = new RButtonLayer(window, this);
			pageButtons[i] = obj;
			obj.width = 100;
			obj.height = 25;
			obj.color = currentPage == i ? 0xff0000 : 0x000000;
			obj.caption = "デ�`タ " + (i*4+1) + "゛" + ((i+1)*4);
			obj.captionColor = 0xffffff;
			obj.top = i * 30 + 100;
			obj.left = 10;
			obj.focusable = true;
			obj.enabled = currentPage != i;
			obj.visible = true;
			obj.tag = 'page';
			obj.page = i;
		}
	}

	function changePage(newpage)
	{
		// ペ�`ジを�筝�するとき
		if(pageButtons !== void)
		{
			pageButtons[currentPage].color = 0x000000;
			pageButtons[currentPage].enabled = true;
			pageButtons[newpage].color = 0xff0000;
			pageButtons[newpage].enabled = false;
			currentPage = newpage;
			makeSaveDataItems();
		}
	}

	function makeReturnButton()
	{
		// ��る ボタンを恬撹
		if(returnButton === void)
		{
			returnButton = new RButtonLayer(window, this);
			returnButton.width = 100;
			returnButton.height = 25;
			returnButton.caption = "��る";
			returnButton.captionColor = 0xffffff;
			returnButton.left = 10;
			returnButton.top = 440;
			returnButton.focusable = true;
		}
		returnButton.visible = true;
	}

	function makeLoadMenu()
	{
		// 仝ロ�`ド々メニュ�`
		if(state != 1)
		{
			clear();
			state = 1;
			makeSaveDataItems(currentPage);
			makeReturnButton();
			makePageButtons();
			font.height = 24;
			drawText(30, 30, "ロ�`ド", 0xffffff);
		}
	}

	function makeSaveMenu()
	{
		// 仝セ�`ブ々メニュ�`
		if(state != 2)
		{
			clear();
			state = 2;
			makeSaveDataItems(currentPage);
			makeReturnButton();
			makePageButtons();
			font.height = 24;
			drawText(30, 30, "セ�`ブ", 0xffffff);
		}
	}

	function clear()
	{
		// 鮫中貧のボタン��をすべて掲燕幣にするか、�o�浸�する
		clearBase();

		closeButton.visible = false if closeButton !== void;
		saveButton.visible = false if saveButton !== void;
		loadButton.visible = false if loadButton !== void;
		hideMessageButton.visible = false if hideMessageButton !== void;

		returnButton.visible = false if returnButton !== void;

		clearSaveDataItems();
		clearPageButtons();
	}

	function saveToSystemVariable()
	{
		// システム�篳�にデ�`タを隠贋する駅勣があるとき
		if(saveDataItems !== void)
		{
			for(var i = 0; i < saveDataItems.count; i++)
				saveDataItems[i].saveToSystemVariable();
			kag.setBookMarkMenuCaptions();
		}
	}

	function onButtonClick(sender)
	{
		// ボタンが兀されたとき
		switch(sender)
		{
		case closeButton: // �]じる ボタン
			owner.onConfigClose();
			break;
		case saveButton: // セ�`ブ ボタン
			owner.backCopyConfig();
			owner.backConfig.makeSaveMenu();
			kag.process('', '*rclick_2');
			break;
		case loadButton: // ロ�`ド ボタン
			owner.backCopyConfig();
			owner.backConfig.makeLoadMenu();
			kag.process('', '*rclick_2');
			break;
		case hideMessageButton: // メッセ�`ジを��す ボタン
			owner.closeConfig();
			kag.process('', '*hidemessage');
			break;
		case returnButton: // ��る ボタン
			returnMenu();
			break;
		default:
			if(sender.tag == 'page')
			{
				// page ボタン
				changePage(sender.page);
			}
		}
	}

	function onLoadOrSave(num)
	{
		// 桑催 num をセ�`ブまたはロ�`ド
		if(state == 1)
		{
			// ロ�`ド
			kag.loadBookMarkWithAsk(num);
		}
		else
		{
			// セ�`ブ
			if(kag.saveBookMarkWithAsk(num))
			{
				clearSaveDataItems();
				if(kag.scflags.bookMarkComments !== void)
					kag.scflags.bookMarkComments[num] = ''; // コメントは匯鬋�リア
				makeSaveDataItems(); // 燕幣を厚仟

				// �Y鮫中に�筝�瘁の秤�鵑鬟灰圦`
				owner.backCopyConfig();
			}
		}
	}

	function show()
	{
		// レイヤを燕幣する
		visible = true;
		setMode();
		focus();
	}

	function hide(opt)
	{
		// レイヤを�Lす
		if (opt) removeMode(); // 燕鮫中のモ�`ダルを盾茅
		visible = false;
		clear();
		state = -1;
	}

	function returnMenu()
	{
		// 嘔クリック, ESC キ�`,仝��る々ボタンでの��り枠登協
		owner.backCopyConfig();
		if(!state || f.rclickmode)
		{
			// メインメニュ�`が燕幣されているときか、
			// セ�`ブ?ロ�`ド鮫中を岷俊柵び竃した��栽は�]じる
			owner.onConfigClose();
		}
		else
		{
			// 岷俊柵び竃されておらず、セ�`ブ?ロ�`ド鮫中が
			// 燕幣されている��栽はメインメニュ�`へ��る
			owner.backConfig.makeMainMenu();
			kag.process('', '*rclick_2');
		}
	}

	function onKeyDown(key)
	{
		// トランジション嶄
		if(kag.transCount != 0)
		{
			// Enter キ�`またはスペ�`スキ�`が兀されたらスキップ
			if(key == VK_RETURN || key == VK_SPACE)
				kag.stopAllTransitions();
			return;
		}

		super.onKeyDown(...);

		// ESC キ�`が兀されたら��り枠登協
		if(key == VK_ESCAPE) returnMenu();
	}

	function onMouseDown(x, y, button, shift)
	{
		// トランジション嶄
		if(kag.transCount != 0)
		{
			// 恣クリックされたらスキップ
			if(button == mbLeft) kag.stopAllTransitions();
			return;
		}

		// 嘔クリックされたら��り枠登協
		if(button == mbRight) returnMenu();
	}
}

class RClickConfigPlugin extends KAGPlugin // 仝嘔クリック�O協々プラグインクラス
{
	var window; // ウィンドウへの歌孚

	var foreConfig; // �O協レイヤ�┗躬�中��
	var backConfig; // �O協レイヤ���Y鮫中��

	function RClickConfigPlugin(window)
	{
		// RClickPlugin コンストラクタ
		super.KAGPlugin(); // ス�`パ�`クラスのコンストラクタを柵ぶ
		this.window = window; // window への歌孚を隠贋する
	}

	function finalize()
	{
		invalidate foreConfig if foreConfig !== void;
		invalidate backConfig if backConfig !== void;
		super.finalize(...);
	}

	function show()
	{
		// 燕鮫中と�Y鮫中に�O協レイヤを恬撹
		if(foreConfig === void)
			foreConfig = new RClickConfigLayer(window, kag.fore.base, this);
		if(backConfig === void)
			backConfig = new RClickConfigLayer(window, kag.back.base, this);

		// 嶷ね栽わせ��會はメッセ�`ジ堕�sよりも安
		// ☆僣プラグインとか聞っている��栽は
		// 　嶷ならないように峺協方�､鰌{屁したほうが措いかも
		foreConfig.absolute = 2000000-1;
		backConfig.absolute = foreConfig.absolute;

		// まだ掲燕幣にしておく
		foreConfig.visible = false;
		backConfig.visible = false;

		if(f.rclickmode == 1)
		{
			// 岷俊仝ロ�`ド々メニュ�`を柵ぶ
			foreConfig.makeLoadMenu();
			backConfig.makeLoadMenu();
		}
		else if(f.rclickmode == 2)
		{
			// 岷俊仝セ�`ブ々メニュ�`を柵ぶ
			foreConfig.makeSaveMenu();
			backConfig.makeSaveMenu();
		}
		else
		{
			// それ參翌はメインメニュ�`
			foreConfig.makeMainMenu();
			backConfig.makeMainMenu();
		}
	}

	function setConfigTrans(opt)
	{
		// 鮫中を燕幣するか掲燕幣にするかの�O協
		backConfig.visible = opt;

		foreConfig.removeMode(); // 燕鮫中のモ�`ダルを盾茅
	}

	function backCopyConfig()
	{
		// 燕鮫中のセ�`ブデ�`タの彜�Bを隠贋
		foreConfig.saveToSystemVariable();

		// 駅勣な燕鮫中の秤�鵑鰕Y鮫中にコピ�`する
		backConfig.visible = foreConfig.visible;
		backConfig.state = foreConfig.state;
		backConfig.currentPage = foreConfig.currentPage;

		var fc = foreConfig.saveDataItems;
		var bc = backConfig.saveDataItems;

		// �Y鮫中を厚仟する
		if(fc !== void)
		{
			     if(backConfig.state == 1) backConfig.makeLoadMenu();
			else if(backConfig.state == 2) backConfig.makeSaveMenu();
		}
		if(fc !== void && bc !== void)
		{
			for(var i = 0; i < fc.count; i++)
			{
				bc[i].commentEdit.text = fc[i].commentEdit.text;
				bc[i].protectCheckBox.checked = fc[i].protectCheckBox.checked;
				bc[i].num = fc[i].num;
			}
		}
	}

	function foreCopyConfig()
	{
		// 駅勣な�Y鮫中の秤�鵑魃躬�中にコピ�`する
		foreConfig.visible = backConfig.visible;
		foreConfig.state = backConfig.state;
		foreConfig.currentPage = backConfig.currentPage;

		var fc = foreConfig.saveDataItems;
		var bc = backConfig.saveDataItems;

		// 燕鮫中を厚仟する
		if(bc !== void)
		{
			     if(foreConfig.state == 1) foreConfig.makeLoadMenu();
			else if(foreConfig.state == 2) foreConfig.makeSaveMenu();
		}
		if(bc !== void && fc !== void)
		{
			for(var i = 0; i < bc.count; i++)
			{
				fc[i].commentEdit.text = bc[i].commentEdit.text;
				fc[i].protectCheckBox.checked = bc[i].protectCheckBox.checked;
				fc[i].num = bc[i].num;
			}
		}
	}

	function onConfigClose()
	{
		// �O協レイヤを�]じる����
		f.rclickmode = 0; // 廷のため柵び竃しフラグを兜豚晒
		setConfigTrans(false); // �Y鮫中を掲燕幣
		window.trigger('config'); // 'config'トリガを�k�咾垢�
	}

	function closeConfig()
	{
		// �O協レイヤを頼畠に�]じる
		foreConfig.hide(true)  if foreConfig !== void;
		backConfig.hide(false) if backConfig !== void;
	}

	// 參和、KAGPlugin のメソッドのオ�`バ�`ライド

	function onStore(f, elm)
	{
	}

	function onRestore(f, clear, elm)
	{
		// �櫃鰌iみ竃すとき
		closeConfig();
	}

	function onStableStateChanged(stable)
	{
	}

	function onMessageHiddenStateChanged(hidden)
	{
	}

	function onCopyLayer(toback)
	{
		// レイヤの燕○★�Yの秤�鵑離灰圦`
		// backlay タグやトランジションの�K阻�rに柵ばれる
		if(toback)
			if(foreConfig !== void) backCopyConfig(); // 燕★�Y
		else
			if(backConfig !== void) foreCopyConfig(); // �Y★燕
	}

	function onExchangeForeBack()
	{
		// �Yと燕の砿尖秤�鵑鮟��Q
		if (foreConfig !== void)
		{
			var tmp;
			tmp = backConfig;
			backConfig = foreConfig;
			foreConfig = tmp;
		}
	}

	function onSaveSystemVariables()
	{
		// セ�`ブデ�`タのコメントはこのタイミングでも scflags に
		// 隠贋しなければならない
		if(foreConfig !== void) foreConfig.saveToSystemVariable();
	}
}

kag.addPlugin(global.rclickconfig_object = new RClickConfigPlugin(kag));

@endscript
@endif
@return


*rclick
; 嘔クリックで柵ばれるサブル�`チン
; 岷俊仝ロ�`ド々や仝セ�`ブ々の鮫中を柵び竃す��栽もここを柵ぶ
@locklink
@locksnapshot
;---
; �O協レイヤを恬撹
@eval exp="rclickconfig_object.show()"
@backlay
;---
*rclick_2
;---
; トランジションで燕幣させる
@eval exp="rclickconfig_object.setConfigTrans(true)"
@trans method=crossfade time=2000
@wt
;---
*rclick_3
;---
; 燕鮫中の燕幣�O協
@eval exp="rclickconfig_object.foreConfig.show()"
;---
; 'config'トリガが�k�咾気譴襪泙粘��C
@waittrig name="config"
;---
; トランジションで�]じる
@trans method=crossfade time=2000
@wt
;---
; �O協レイヤの嶄附を��肇
@eval exp="rclickconfig_object.closeConfig()"
;---
@unlocksnapshot
@unlocklink
@return

*hidemessage
; メッセ�`ジを��すときに柵ばれる
@hidemessage
;---
; �O協レイヤを壅燕幣
@eval exp="rclickconfig_object.show()"
;---
@jump target=*rclick_3


;------------------
; 2005/12/10 Ranka 
;------------------

