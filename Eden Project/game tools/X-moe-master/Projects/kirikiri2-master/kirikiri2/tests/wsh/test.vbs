'
' VBScript �ɂ��L�q�B
' VBScript �ɂ͈�������̃R���X�g���N�^�̊T�O���Ȃ�㩁i��)
' �d�����Ȃ��̂� tjs �ŋL�q�����������\�b�h���Ăяo���Ă܂��B
'
Dim win, base
Set win = createWindow()
Call win.setInnerSize(400,400)
Set base = createLayer(win, null)
Call base.setSize(400,400)
Call base.fillRect(0,0,400,400,&hff00ff)
base.visible = true
win.visible = true
