// �S��O��\������
notifyAllExceptions(1);
printf("squirrel test start\n");

local cnt=0;
while (cnt < vargc) {
	print(format("arg%d:%s",cnt,vargv[cnt++]));
}

// �N���X���Q��
Window <- ::createTJSClass("TestWindow", "Window"); // �p���N���X�͗v��
Layer  <- ::createTJSClass("Layer");
System <- ::createTJSClass("System");

local WIDTH=800;
local HEIGHT=600;

/**
 * �E�C���h�E�N���X
 */
class MyWindow extends Window
{
	base = null;  //< �v���C�}�����C��
	layer = null; //< �\�����C��

	/**
	 * �R���X�g���N�^
	 */
	constructor(w, h) {
		::Window.constructor();
		setInnerSize(w, h);

		// primary layer
		base = Layer(this, ::tjsNull);
		base.setSize(w, h);
		add(base);

		// �\���p���C��
		layer = Layer(this, base);
		layer.setSize(100,100);
		layer.fillRect(0,0,100,100,0xffff0000);
		layer.setPos(400,100);
		layer.visible = true;

		// �C�x���g���󗝂ł���悤��TJS�C���X�^���X���㏑������

		//tjsOverride("onMouseDown", onMouseDown);   // ���ꂾ�ƃR���e�L�X�g�� global
		//tjsOverride("onMouseDown", onMouseDown.bindenv(this));   // ���ꂪ�Ó�
		tjsOverride("onMouseDown"); // ���̋L�q�͎����I�� bindenv(this) ���������� onMouseDown ���擾

		//�R�����g�A�E�g����ƃN���[�Y����
		//tjsOverride("onCloseQuery");

		// onKeyDown �ɂ��Ă� TJS���� callSQ �ɂ��Ăяo���u���b�W�L�q������
	}

	/**
	 * ���s����
	 */
	function main() {
		// ���C�����[�v���\�z
		while (tjsIsValid()) {
			if (System.getKeyState(13)) { // VK_RETURN
				break;
			}
			local tick = ::getCurrentTick();
			layer.left = 400 + 200*sin(2 * PI * tick / 1000);
			// ����ɂ��`�揈������������g���g���ɖ߂�
			::suspend();
		}
	};
	
	/**
	 * �}�E�X���������C�x���g
	 */
	function onMouseDown(x, y, button, shift) {
		printf("%s:onMouseDown:%d,%d,%d,%d\n", this, x, y, button, shift);
	}

	/**
	 * �L�[���������C�x���g
	 */
	function onKeyDown(key,shift) {
		printf("%s:onKeyDown:%02x,%02x\n", this, key, shift);
	}

	/**
	 * �I���m�F�C�x���g
	 */
	function onCloseQuery(onclose) {
		// �I�������Ȃ�
		::Window.onCloseQuery(false);
	}

};

// window
local win = MyWindow(WIDTH, HEIGHT);
win.visible = true;
win.main();
