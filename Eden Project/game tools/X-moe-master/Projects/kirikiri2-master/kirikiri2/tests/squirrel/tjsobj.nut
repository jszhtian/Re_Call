print("�I�u�W�F�N�g�Q�Ƃ̃e�X�g");

// �g���g���N���X�̃o�C���h
Layer <- createTJSClass("Layer");
layer <- Layer(::krkr.win, ::krkr.win.base);
layer.setSize(100,100);
layer.setPos(150,150);
layer.fillRect(0,0,100,100,0xffff00ff);
print("layer visible:" + layer.visible);
layer.visible = true;
print("layer visible:" + layer.visible);

System <- createTJSClass("System");

print("type:" + typeof System);
print("tick�l:" + System.getTickCount());
print("���s�t�@�C���̃p�X:" + System.getExePath());
