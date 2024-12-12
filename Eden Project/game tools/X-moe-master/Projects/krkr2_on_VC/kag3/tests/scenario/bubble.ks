@if exp="typeof(global.bubble_object) == 'undefined'"
@iscript

/*
	�A���ӂ点��v���O�C��
*/

class BubbleGrain
{
	// �A���̃N���X

	var fore; // �\��ʂ̖A���I�u�W�F�N�g
	var back; // ����ʂ̖A���I�u�W�F�N�g
	var xvelo; // �����x
	var yvelo; // �c���x
	var xaccel; // ������
	var l, t; // ���ʒu�Əc�ʒu
	var ownwer; // ���̃I�u�W�F�N�g�����L���� BubblePlugin �I�u�W�F�N�g
	var spawned = false; // �A�����o�����Ă��邩
	var window; // �E�B���h�E�I�u�W�F�N�g�ւ̎Q��

	function BubbleGrain(window, n, owner)
	{
		// BubbleGrain �R���X�g���N�^
		this.owner = owner;
		this.window = window;

		fore = new Layer(window, window.fore.base);
		back = new Layer(window, window.back.base);

		fore.absolute = 2000000-1; // �d�ˍ��킹�����̓��b�Z�[�W����������
		back.absolute = fore.absolute;

		fore.hitType = htMask;
		fore.hitThreshold = 256; // �}�E�X���b�Z�[�W�͑S�擧��
		back.hitType = htMask;
		back.hitThreshold = 256;

		fore.loadImages("bubble_" + n); // �摜��ǂݍ���
		back.assignImages(fore);
		fore.setSizeToImageSize(); // ���C���̃T�C�Y���摜�̃T�C�Y�Ɠ�����
		back.setSizeToImageSize();
		xvelo = 0; // ���������x
		yvelo = -(n*0.6 + 1.9 + Math.random() * 0.2); // �c�������x
		xaccel = Math.random(); // ���������x
	}

	function finalize()
	{
		invalidate fore;
		invalidate back;
	}

	function spawn()
	{
		// �o��
		l = Math.random() * window.primaryLayer.width; // �������ʒu
		t = window.primaryLayer.height; // �c�����ʒu
		spawned = true;
		fore.setPos(l, t);
		back.setPos(l, t); // ����ʂ̈ʒu��������
		fore.visible = owner.foreVisible;
		back.visible = owner.backVisible;
	}

	function resetVisibleState()
	{
		// �\���E��\���̏�Ԃ��Đݒ肷��
		if(spawned)
		{
			fore.visible = owner.foreVisible;
			back.visible = owner.backVisible;
		}
		else
		{
			fore.visible = false;
			back.visible = false;
		}
	}

	function move()
	{
		// �A���𓮂���
		if(!spawned)
		{
			// �o�����Ă��Ȃ��̂ŏo������@�����������
			if(Math.random() < 0.002) spawn();
		}
		else
		{
			l += xvelo;
			t += yvelo;
			xvelo += xaccel;
			xaccel += (Math.random() - 0.5) * 0.3;
			if(xvelo>=1.5) xvelo=1.5;
			if(xvelo<=-1.5) xvelo=-1.5;
			if(xaccel>=0.2) xaccel=0.2;
			if(xaccel<=-0.2) xaccel=-0.2;
			if(t <= -fore.height)
			{
				t = window.primaryLayer.height;
				l = Math.random() * window.primaryLayer.width;
			}
			fore.setPos(l, t);
			back.setPos(l, t); // ����ʂ̈ʒu��������
		}
	}

	function exchangeForeBack()
	{
		// �\�Ɨ��̊Ǘ�������������
		var tmp = fore;
		fore = back;
		back = tmp;
	}
}

class BubblePlugin extends KAGPlugin
{
	// �A��U�炷�v���O�C���N���X

	var bubbles = []; // �A��
	var timer; // �^�C�}
	var window; // �E�B���h�E�ւ̎Q��
	var foreVisible = true; // �\��ʂ��\����Ԃ��ǂ���
	var backVisible = true; // ����ʂ��\����Ԃ��ǂ���

	function BubblePlugin(window)
	{
		super.KAGPlugin();
		this.window = window;
	}

	function finalize()
	{
		// finalize ���\�b�h
		// ���̃N���X�̊Ǘ����邷�ׂẴI�u�W�F�N�g�𖾎��I�ɔj��
		for(var i = 0; i < bubbles.count; i++)
			invalidate bubbles[i];
		invalidate bubbles;

		invalidate timer if timer !== void;

		super.finalize(...);
	}

	function init(num, options)
	{
		// num �̖A�����o��������
		if(timer !== void) return; // ���łɖA���͂łĂ���

		// �A�����쐬
		for(var i = 0; i < num; i++)
		{
			var n = intrandom(0, 4); // �A���̑傫�� ( �����_�� )
			bubbles[i] = new BubbleGrain(window, n, this);
		}
		bubbles[0].spawn(); // �ŏ��̖A�������͍ŏ�����\��

		// �^�C�}�[���쐬
		timer = new Timer(onTimer, '');
		timer.interval = 80;
		timer.enabled = true;

		foreVisible = true;
		backVisible = true;
		setOptions(options); // �I�v�V������ݒ�
	}

	function uninit()
	{
		// �A��������
		if(timer === void) return; // �A���͂łĂ��Ȃ�

		for(var i = 0; i < bubbles.count; i++)
			invalidate bubbles[i];
		bubbles.count = 0;

		invalidate timer;
		timer = void;
	}

	function setOptions(elm)
	{
		// �I�v�V������ݒ肷��
		foreVisible = +elm.forevisible if elm.forevisible !== void;
		backVisible = +elm.backvisible if elm.backvisible !== void;
		resetVisibleState();
	}

	function onTimer()
	{
		// �^�C�}�[�̎������ƂɌĂ΂��
		var bubblecount = bubbles.count;
		for(var i = 0; i < bubblecount; i++)
			bubbles[i].move(); // move ���\�b�h���Ăяo��
	}

	function resetVisibleState()
	{
		// ���ׂĂ̖A���� �\���E��\���̏�Ԃ��Đݒ肷��
		var bubblecount = bubbles.count;
		for(var i = 0; i < bubblecount; i++)
			bubbles[i].resetVisibleState(); // resetVisibleState ���\�b�h���Ăяo��
	}

	function onStore(f, elm)
	{
		// �x��ۑ�����Ƃ�
		var dic = f.bubbles = %[];
		dic.init = timer !== void;
		dic.foreVisible = foreVisible;
		dic.backVisible = backVisible;
		dic.bubbleCount = bubbles.count;
	}

	function onRestore(f, clear, elm)
	{
		// �x��ǂݏo���Ƃ�
		var dic = f.bubbles;
		if(dic === void || !+dic.init)
		{
			// �A�͂łĂ��Ȃ�
			uninit();
		}
		else if(dic !== void && +dic.init)
		{
			// �A�͂łĂ���
			init(dic.bubbleCount, %[ forevisible : dic.foreVisible, backvisible : dic.backVisible ] );
		}
	}

	function onStableStateChanged(stable)
	{
	}

	function onMessageHiddenStateChanged(hidden)
	{
	}

	function onCopyLayer(toback)
	{
		// ���C���̕\���������̃R�s�[
		// ���̃v���O�C���ł̓R�s�[���ׂ����͕\���E��\���̏�񂾂�
		if(toback)
		{
			// �\����
			backVisible = foreVisible;
		}
		else
		{
			// �����\
			foreVisible = backVisible;
		}
		resetVisibleState();
	}

	function onExchangeForeBack()
	{
		// ���ƕ\�̊Ǘ���������
		var bubblecount = bubbles.count;
		for(var i = 0; i < bubblecount; i++)
			bubbles[i].exchangeForeBack(); // exchangeForeBack ���\�b�h���Ăяo��
	}
}

kag.addPlugin(global.bubble_object = new BubblePlugin(kag));
	// �v���O�C���I�u�W�F�N�g���쐬���A�o�^����

@endscript
@endif
; �}�N���o�^
@macro name="bubbleinit"
@eval exp="bubble_object.init(17, mp)"
@endmacro
@macro name="bubbleuninit"
@eval exp="bubble_object.uninit()"
@endmacro
@macro name="bubbleopt"
@eval exp="bubble_object.setOptions(mp)"
@endmacro
@return
