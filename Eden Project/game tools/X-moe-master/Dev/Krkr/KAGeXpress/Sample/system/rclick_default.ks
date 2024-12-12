@if exp="typeof(global.rclickconfig_object) == 'undefined'"
@iscript
// �ҥ���å��Ǥ��O������� TJS2 �ǌg�F���륵��ץ�
// 2002/6/14 �� ���� :
//   �ȥ�󥸥���󌝏�Щ`�����?��

// �ݤ� 28 �����ÿ���

// Config.tjs ���O�������¤Τ褦�ˤ����Ҫ����
//  freeSaveDataMode = false
//  saveThumbnail = true
//  numBookMarks = 28

class RButtonLayer extends ButtonLayer
	// parent �� onClick ���٥�Ȥ��ͤ�褦�ˤ����ܥ���쥤��
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
		// �ȥ�󥸥�����Фʤ饹���å�
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
	// ��һ��һ���ˌ��ꤹ��쥤��
	var num; // �ݷ���
	var bgColor = 0xa0ffffff; // ����ɫ ( 0xAARRGGBB )
	var focusedColor = 0xffffffff;

	var commentEdit; // �����ȤΥ��ǥ��å�
	var protectCheckBox; // ���ǩ`�����o�������å��ܥå���

	function SaveDataItemLayer(window, parent, num)
	{
		super.KAGLayer(window, parent);

		this.num = num;

		setImageSize(500, 112); // ������
		face = dfBoth;
		fillRect(0, 0, imageWidth, imageHeight, bgColor);
		setSizeToImageSize();

		hitType = htMask;
		hitThreshold = 0; // ȫ��͸�^

		cursor = kag.cursorPointed;

		focusable = true; // �ե��`�������ܤ�ȡ���

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

		drawText(420, 35, "�ǩ`�����o", 0x000000);

		// ���Ť��ʾ
		var str = string (num + 1);
		font.height = 20;
		var ty = font.getTextHeight(str);
		drawText(6, ( imageHeight - ty ) \ 2, str, 0);

		// ����ͥ��뻭����i���z��
		var tmplayer = new global.Layer(window, parent);

 		var tnname = kag.getBookMarkFileNameAtNum(num);

		if(Storages.isExistentStorage(tnname))
		{
			tmplayer.loadImages(tnname);
		}
		else
		{
			tmplayer.setImageSize(133, 100);
			var str = "�ǩ`���ʤ�";
			var tx = tmplayer.font.getTextWidth(str);
			tmplayer.drawText((tmplayer.imageWidth - tx) \ 2, 40,
				str, 0xffffff);
		}

		copyRect(32, 6, tmplayer, 0, 0, tmplayer.imageWidth, tmplayer.imageHeight);

		invalidate tmplayer;

		// �ݤα��������ʾ
		font.height = 14;

		var str = kag.bookMarkNames[num];
		if(str == '') str = '�ǩ`���ʤ�';
		drawText(180, 15, str, 0x000000);

		// �ո����ʾ
		if(kag.bookMarkDates[num] == '')
			str = "�ո� : ----/--/-- --:--", commentEdit.enabled = false;
		else
			str = "�ո� : " + kag.bookMarkDates[num];

		drawText(180, 35, str, 0x000000);

		// ������ : 
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
		// ״�B�򥷥��ƥ�������ӛ�h����
		if(kag.scflags.bookMarkComments === void)
			kag.scflags.bookMarkComments = [];
		kag.scflags.bookMarkComments[num] = commentEdit.text;
		kag.bookMarkProtectedStates[num] = protectCheckBox.checked;
	}

	function onPaint()
	{
		super.onPaint(...);

		// update() �����Ф줿�ᡢ�軭��ֱǰ�˺��Ф��
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
		// �ե��`������ä�
		super.onFocus(...);
		update();
	}

	function onBlur()
	{
		// �ե��`������ʧ�ä�
		super.onBlur(...);
		update();
	}

	function onHitTest(x, y, process)
	{
		if(process)
		{
			// �ҥܥ���Ѻ����Ƥ����Ȥ��˥��٥�Ȥ�͸�^
			if(System.getKeyState(VK_RBUTTON))
				super.onHitTest(x, y, false);
			else
				super.onHitTest(x, y, true);
		}
	}

	function onKeyDown(key, shift, process)
	{
		// �ȥ�󥸥�����Фʤ饹���å�
		if(kag.transCount != 0)
		{
			kag.stopAllTransitions();
			return;
		}

		// ���`��Ѻ���줿
		if(process && key == VK_RETURN || key == VK_SPACE)
		{
			// ���ک`�����`�ޤ��ϥ��󥿩`���`
			super.onKeyDown(key, shift, false);
			saveToSystemVariable();
			parent.onLoadOrSave(num);
		}
		else
		{
			// process �� false �Έ��ϤτI����Ф�ʤ�
			super.onKeyDown(...);
		}
	}

	function onMouseDown(x, y, button, shift)
	{
		// �ȥ�󥸥�����Фʤ饹���å�
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

class RClickConfigLayer extends Layer // �O������쥤��
{
	var closeButton; // �]���� �ܥ���
	var saveButton; // ���`�� �ܥ���
	var loadButton; // ��`�� �ܥ���
	var hideMessageButton; // ��å��`�������� �ܥ���
	var saveDataItems;
	var state = -1; // 0 = �ᥤ���˥�`  1 = ��`�ɻ���  2 = ���`�ֻ���
	var owner; // RClickConfigPlugin ���֥������Ȥؤβ���
	var currentPage = 0; // ���`�֥ǩ`���x�k�Ф˱�ʾ�ФΥک`��
	var returnButton; // ��ɤ� �ܥ���
	var pageButtons; // ���`�֥ǩ`���Υک`���ܥ���

	function RClickConfigLayer(win, par, owner)
	{
		super.Layer(win, par);
		this.owner = owner;

		// �쥤���״�B����ڻ�
		setImageSize(640, 480);
		clearBase();
		setSizeToImageSize();
		setPos(0, 0);
		hitType = htMask;
		hitThreshold = 0; // ȫ��͸�^
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
		// �µؤ�T��ֱ��
		fillRect(0, 0, imageWidth, imageHeight, 0xc0000000);
	}

	function makeMainMenu()
	{
		// �ᥤ���˥�`�α�ʾ
		if(state != 0)
		{
			clear();
			state = 0;

			/*
				�ܥ�������ɤ��Ƥ��ޤ�
				�����Ǥ����֥ܥ�������ɤ��Ƥޤ�����
				����ܥ�������ꤿ���Ȥ���
			if(closeButton === void)
			{
				closeButton = new RButtonLayer(window, this);
				closeButton.showFocusImage = true;
				closeButton.loadImages("�ܥ�����ե�������");
				closeButton.left = 270;
				closeButton.top = 350;
				closeButton.focusable = true;
			}
				�ߤ����ʸФ���ӛ�����ޤ�

				������ָ������ܥ������ͨ�Υܥ�����
				( ���Ĥ�״�B����ˁK��Ǥ��뻭�� ) �ǤϤʤ��ơ�
				���Ĥ�״�B ( ͨ����Ѻ���줿�r���ޥ������Ϥˤ���Ȥ���
				�ե��`����������r ) ����ˁK�������Ǥ����Ҫ��
				����ޤ����Ȥ�������Ρ��ե��`����������Ȥ�����
				״�B�ϡ����`�ܩ`�ɤǲ����򤹤��ˤΤ���˱�Ҫ�Ǥ�
				( showFocusImage=true �ˤ���ȣ��Ĥ��ˤʤ�٤�
				  �����ʹ�ä���褦�ˤʤ�ޤ�;�ƊX ) 
			 */

			// ���]���롹�ܥ��������
			if(closeButton === void)
			{
				closeButton = new RButtonLayer(window, this);
				closeButton.width = 100;
				closeButton.height = 25;
				closeButton.caption = "�]����";
				closeButton.captionColor = 0xffffff;
				closeButton.left = 270;
				closeButton.top = 350;
				closeButton.focusable = true;
			}
			closeButton.visible = true;

			// �����`�֡��ܥ��������
			if(saveButton === void)
			{
				saveButton = new RButtonLayer(window, this);
				saveButton.width = 100;
				saveButton.height = 25;
				saveButton.caption = "���`��";
				saveButton.captionColor = 0xffffff;
				saveButton.left = 270;
				saveButton.top = 100;
				saveButton.focusable = true;
			}
			saveButton.enabled = kag.canStore();
			saveButton.visible = true;

			// ����`�ɡ��ܥ��������
			if(loadButton === void)
			{
				loadButton = new RButtonLayer(window, this);
				loadButton.width = 100;
				loadButton.height = 25;
				loadButton.caption = "��`��";
				loadButton.captionColor = 0xffffff;
				loadButton.left = 270;
				loadButton.top = 150;
				loadButton.focusable = true;
			}
			loadButton.enabled = kag.canRestore();
			loadButton.visible = true;

			// ����å��`�����������ܥ��������
			if(hideMessageButton === void)
			{
				hideMessageButton = new RButtonLayer(window, this);
				hideMessageButton.width = 100;
				hideMessageButton.height = 25;
				hideMessageButton.caption = "��å��`��������";
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
		// ���`�֥ǩ`����ʾ�Υ��ꥢ
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
		// ���`�֥ǩ`���α�ʾ
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
		// �ک`���ܥ���Υ��ꥢ
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
		// �ک`���ܥ�������ɤ���
		clearPageButtons();
		pageButtons = [];
		for(var i = 0; i < 7; i++)
		{
			// �ک`���ܥ���ϻ���Ǥ�褤���⤷��ޤ���
			// ���Έ��ϤϬF�ڤΥک`������ܥ����������
			// �䤨��Ȥ褤����
			var obj = new RButtonLayer(window, this);
			pageButtons[i] = obj;
			obj.width = 100;
			obj.height = 25;
			obj.color = currentPage == i ? 0xff0000 : 0x000000;
			obj.caption = "�ǩ`�� " + (i*4+1) + "��" + ((i+1)*4);
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
		// �ک`����������Ȥ�
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
		// ���� �ܥ��������
		if(returnButton === void)
		{
			returnButton = new RButtonLayer(window, this);
			returnButton.width = 100;
			returnButton.height = 25;
			returnButton.caption = "����";
			returnButton.captionColor = 0xffffff;
			returnButton.left = 10;
			returnButton.top = 440;
			returnButton.focusable = true;
		}
		returnButton.visible = true;
	}

	function makeLoadMenu()
	{
		// ����`�ɡ���˥�`
		if(state != 1)
		{
			clear();
			state = 1;
			makeSaveDataItems(currentPage);
			makeReturnButton();
			makePageButtons();
			font.height = 24;
			drawText(30, 30, "��`��", 0xffffff);
		}
	}

	function makeSaveMenu()
	{
		// �����`�֡���˥�`
		if(state != 2)
		{
			clear();
			state = 2;
			makeSaveDataItems(currentPage);
			makeReturnButton();
			makePageButtons();
			font.height = 24;
			drawText(30, 30, "���`��", 0xffffff);
		}
	}

	function clear()
	{
		// �����ϤΥܥ���򤹤٤ƷǱ�ʾ�ˤ��뤫���o��������
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
		// �����ƥ������˥ǩ`���򱣴椹���Ҫ������Ȥ�
		if(saveDataItems !== void)
		{
			for(var i = 0; i < saveDataItems.count; i++)
				saveDataItems[i].saveToSystemVariable();
			kag.setBookMarkMenuCaptions();
		}
	}

	function onButtonClick(sender)
	{
		// �ܥ���Ѻ���줿�Ȥ�
		switch(sender)
		{
		case closeButton: // �]���� �ܥ���
			owner.onConfigClose();
			break;
		case saveButton: // ���`�� �ܥ���
			owner.backCopyConfig();
			owner.backConfig.makeSaveMenu();
			kag.process('', '*rclick_2');
			break;
		case loadButton: // ��`�� �ܥ���
			owner.backCopyConfig();
			owner.backConfig.makeLoadMenu();
			kag.process('', '*rclick_2');
			break;
		case hideMessageButton: // ��å��`�������� �ܥ���
			owner.closeConfig();
			kag.process('', '*hidemessage');
			break;
		case returnButton: // ���� �ܥ���
			returnMenu();
			break;
		default:
			if(sender.tag == 'page')
			{
				// page �ܥ���
				changePage(sender.page);
			}
		}
	}

	function onLoadOrSave(num)
	{
		// ���� num �򥻩`�֤ޤ��ϥ�`��
		if(state == 1)
		{
			// ��`��
			kag.loadBookMarkWithAsk(num);
		}
		else
		{
			// ���`��
			if(kag.saveBookMarkWithAsk(num))
			{
				clearSaveDataItems();
				if(kag.scflags.bookMarkComments !== void)
					kag.scflags.bookMarkComments[num] = ''; // �����Ȥ�һ�ꥯ�ꥢ
				makeSaveDataItems(); // ��ʾ�����

				// �Y����ˉ��������򥳥ԩ`
				owner.backCopyConfig();
			}
		}
	}

	function show()
	{
		// �쥤����ʾ����
		visible = true;
		setMode();
		focus();
	}

	function hide(opt)
	{
		// �쥤����L��
		if (opt) removeMode(); // ����Υ�`�������
		visible = false;
		clear();
		state = -1;
	}

	function returnMenu()
	{
		// �ҥ���å�, ESC ���`,�����롹�ܥ���ǤΑ������ж�
		owner.backCopyConfig();
		if(!state || f.rclickmode)
		{
			// �ᥤ���˥�`����ʾ����Ƥ���Ȥ�����
			// ���`��?��`�ɻ����ֱ�Ӻ��ӳ��������Ϥ��]����
			owner.onConfigClose();
		}
		else
		{
			// ֱ�Ӻ��ӳ�����Ƥ��餺�����`��?��`�ɻ��椬
			// ��ʾ����Ƥ�����Ϥϥᥤ���˥�`�ؑ���
			owner.backConfig.makeMainMenu();
			kag.process('', '*rclick_2');
		}
	}

	function onKeyDown(key)
	{
		// �ȥ�󥸥������
		if(kag.transCount != 0)
		{
			// Enter ���`�ޤ��ϥ��ک`�����`��Ѻ���줿�饹���å�
			if(key == VK_RETURN || key == VK_SPACE)
				kag.stopAllTransitions();
			return;
		}

		super.onKeyDown(...);

		// ESC ���`��Ѻ���줿��������ж�
		if(key == VK_ESCAPE) returnMenu();
	}

	function onMouseDown(x, y, button, shift)
	{
		// �ȥ�󥸥������
		if(kag.transCount != 0)
		{
			// �󥯥�å����줿�饹���å�
			if(button == mbLeft) kag.stopAllTransitions();
			return;
		}

		// �ҥ���å����줿��������ж�
		if(button == mbRight) returnMenu();
	}
}

class RClickConfigPlugin extends KAGPlugin // ���ҥ���å��O�����ץ饰���󥯥饹
{
	var window; // ������ɥ��ؤβ���

	var foreConfig; // �O���쥤�䣨���棩
	var backConfig; // �O���쥤�䣨�Y���棩

	function RClickConfigPlugin(window)
	{
		// RClickPlugin ���󥹥ȥ饯��
		super.KAGPlugin(); // ���`�ѩ`���饹�Υ��󥹥ȥ饯�������
		this.window = window; // window �ؤβ��դ򱣴椹��
	}

	function finalize()
	{
		invalidate foreConfig if foreConfig !== void;
		invalidate backConfig if backConfig !== void;
		super.finalize(...);
	}

	function show()
	{
		// ������Y������O���쥤�������
		if(foreConfig === void)
			foreConfig = new RClickConfigLayer(window, kag.fore.base, this);
		if(backConfig === void)
			backConfig = new RClickConfigLayer(window, kag.back.base, this);

		// �ؤͺϤ碌���ϥ�å��`���Ěs�����
		// ��ѩ�ץ饰����Ȥ�ʹ�äƤ�����Ϥ�
		// ���ؤʤ�ʤ��褦��ָ���������{�������ۤ�����������
		foreConfig.absolute = 2000000-1;
		backConfig.absolute = foreConfig.absolute;

		// �ޤ��Ǳ�ʾ�ˤ��Ƥ���
		foreConfig.visible = false;
		backConfig.visible = false;

		if(f.rclickmode == 1)
		{
			// ֱ�ӡ���`�ɡ���˥�`�����
			foreConfig.makeLoadMenu();
			backConfig.makeLoadMenu();
		}
		else if(f.rclickmode == 2)
		{
			// ֱ�ӡ����`�֡���˥�`�����
			foreConfig.makeSaveMenu();
			backConfig.makeSaveMenu();
		}
		else
		{
			// ��������ϥᥤ���˥�`
			foreConfig.makeMainMenu();
			backConfig.makeMainMenu();
		}
	}

	function setConfigTrans(opt)
	{
		// ������ʾ���뤫�Ǳ�ʾ�ˤ��뤫���O��
		backConfig.visible = opt;

		foreConfig.removeMode(); // ����Υ�`�������
	}

	function backCopyConfig()
	{
		// ����Υ��`�֥ǩ`����״�B�򱣴�
		foreConfig.saveToSystemVariable();

		// ��Ҫ�ʱ���������Y����˥��ԩ`����
		backConfig.visible = foreConfig.visible;
		backConfig.state = foreConfig.state;
		backConfig.currentPage = foreConfig.currentPage;

		var fc = foreConfig.saveDataItems;
		var bc = backConfig.saveDataItems;

		// �Y�������¤���
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
		// ��Ҫ���Y������������˥��ԩ`����
		foreConfig.visible = backConfig.visible;
		foreConfig.state = backConfig.state;
		foreConfig.currentPage = backConfig.currentPage;

		var fc = foreConfig.saveDataItems;
		var bc = backConfig.saveDataItems;

		// �������¤���
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
		// �O���쥤����]����ʂ�
		f.rclickmode = 0; // ��Τ�����ӳ����ե饰����ڻ�
		setConfigTrans(false); // �Y�����Ǳ�ʾ
		window.trigger('config'); // 'config'�ȥꥬ��k�Ӥ���
	}

	function closeConfig()
	{
		// �O���쥤�����ȫ���]����
		foreConfig.hide(true)  if foreConfig !== void;
		backConfig.hide(false) if backConfig !== void;
	}

	// ���¡�KAGPlugin �Υ᥽�åɤΥ��`�Щ`�饤��

	function onStore(f, elm)
	{
	}

	function onRestore(f, clear, elm)
	{
		// �ݤ��i�߳����Ȥ�
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
		// �쥤��α�����Y�����Υ��ԩ`
		// backlay ������ȥ�󥸥����νK�˕r�˺��Ф��
		if(toback)
			if(foreConfig !== void) backCopyConfig(); // ����Y
		else
			if(backConfig !== void) foreCopyConfig(); // �Y����
	}

	function onExchangeForeBack()
	{
		// �Y�ȱ�ι������򽻓Q
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
		// ���`�֥ǩ`���Υ����ȤϤ��Υ����ߥ󥰤Ǥ� scflags ��
		// ���椷�ʤ���Фʤ�ʤ�
		if(foreConfig !== void) foreConfig.saveToSystemVariable();
	}
}

kag.addPlugin(global.rclickconfig_object = new RClickConfigPlugin(kag));

@endscript
@endif
@return


*rclick
; �ҥ���å��Ǻ��Ф�륵�֥�`����
; ֱ�ӡ���`�ɡ��䡸���`�֡��λ������ӳ������Ϥ⤳�������
@locklink
@locksnapshot
;---
; �O���쥤�������
@eval exp="rclickconfig_object.show()"
@backlay
;---
*rclick_2
;---
; �ȥ�󥸥����Ǳ�ʾ������
@eval exp="rclickconfig_object.setConfigTrans(true)"
@trans method=crossfade time=2000
@wt
;---
*rclick_3
;---
; ����α�ʾ�O��
@eval exp="rclickconfig_object.foreConfig.show()"
;---
; 'config'�ȥꥬ���k�Ӥ����ޤǴ��C
@waittrig name="config"
;---
; �ȥ�󥸥������]����
@trans method=crossfade time=2000
@wt
;---
; �O���쥤����������ȥ
@eval exp="rclickconfig_object.closeConfig()"
;---
@unlocksnapshot
@unlocklink
@return

*hidemessage
; ��å��`���������Ȥ��˺��Ф��
@hidemessage
;---
; �O���쥤����ٱ�ʾ
@eval exp="rclickconfig_object.show()"
;---
@jump target=*rclick_3


;------------------
; 2005/12/10 Ranka 
;------------------

