// �T���v���R�[�h

// TJS�̃N���X���v���g�^�C�v�̌`�Ŏ擾
var Window = createTJSClass("Window");
var Layer  = createTJSClass("Layer");

// �Ǝ����C���N���X
function MyLayer(win, parent, width, height, color)
{
	// �e�R���X�g���N�^�Ăяo��
	Layer.call(this, win, parent);
	this.setSize(width,height);
	this.fillRect(0,0,width,height,color);
}

// �Ǝ����C���̃v���g�^�C�v
MyLayer.prototype = {
  __proto__: Layer.prototype
};

// �Ǝ��E�C���h�E�N���X
function MyWindow(width, height)
{
	// �e�R���X�g���N�^�Ăяo��
	Window.call(this);
	this.setInnerSize(width, height);
	this.add(new MyLayer(this, null, this.innerWidth, this.innerHeight, 0xff00ff));
	// �C�x���g�o�^
	this.tjsOverride("onKeyDown");
}

// �Ǝ��E�C���h�E�̃v���g�^�C�v
MyWindow.prototype = {
  __proto__: Window.prototype,
  onKeyDown : function (key, shift) {
	  if (key == krkr.VK_ESCAPE) {
		  this.close();
	  }
  }
};

// ����
var win = new MyWindow(400,200);
win.visible = true;
