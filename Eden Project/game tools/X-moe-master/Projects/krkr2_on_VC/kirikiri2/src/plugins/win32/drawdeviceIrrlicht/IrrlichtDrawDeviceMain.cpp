#include <windows.h>
#include "IrrlichtDrawDevice.h"
#include "LayerManagerInfo.h"
#include "SWFMovie.hpp"

using namespace irr;
using namespace core;
using namespace video;
using namespace scene;
using namespace io;
using namespace gui;

extern void message_log(const char* format, ...);

/**
 * Irrlicht �Ăяo�������J�n
 */
void
tTVPIrrlichtDrawDevice::start()
{
	stop();
	TVPAddContinuousEventHook(this);
}

/**
 * Irrlicht �Ăяo��������~
 */
void
tTVPIrrlichtDrawDevice::stop()
{
	TVPRemoveContinuousEventHook(this);
}

/**
 * Continuous �R�[���o�b�N
 * �g���g�����ɂȂƂ��ɏ�ɌĂ΂��
 * ���ꂪ������̃��C�����[�v�ɂȂ�
 */
void TJS_INTF_METHOD
tTVPIrrlichtDrawDevice::OnContinuousCallback(tjs_uint64 tick)
{
	if (device) {
		device->getTimer()->tick();

		/// �h���C�o
		video::IVideoDriver* driver = device->getVideoDriver();

		dimension2d<s32> screenSize = driver->getScreenSize();
		
		// �`��J�n
		driver->beginScene(true, true, irr::video::SColor(255,0,0,0));

		/// �V�[���}�l�[�W���̕`��
		ISceneManager* smgr = device->getSceneManager();
		smgr->drawAll();

		// �ʃ��C���}�l�[�W���̕`��
		for (std::vector<iTVPLayerManager *>::iterator i = Managers.begin(); i != Managers.end(); i++) {
			LayerManagerInfo *info = (LayerManagerInfo*)(*i)->GetDrawDeviceData();
			if (info && info->texture) {
				driver->draw2DImage(info->texture, core::position2d<s32>(0,0),
									core::rect<s32>(0,0,screenSize.Width,screenSize.Height), 0, 
									video::SColor(255,255,255,255), true);
			}
		}

		// GUI�̕`��
		device->getGUIEnvironment()->drawAll();

		// SWF UI �̕`��
		drawSWF(tick, 0, 0, screenSize.Width, screenSize.Height);
		
		// �`�抮��
		driver->endScene();
	}
};

/**
 * �C�x���g��
 * HWND ���w�肵�Đ������Ă���֌W�� Irrlicht ���g�̓E�C���h�E����
 * �C�x���g���擾���邱�Ƃ͂Ȃ��B�̂� GUI Environment ����̃C�x���g
 * �����������ɂ��邱�ƂɂȂ�H
 * @param event �C�x���g���
 * @return ���������� true
 */
bool
tTVPIrrlichtDrawDevice::OnEvent(irr::SEvent event)
{
	switch (event.EventType) {
	case EET_GUI_EVENT:
		message_log("GUI�C�x���g:%d", event.GUIEvent.EventType);
		switch(event.GUIEvent.EventType) {
		case EGET_BUTTON_CLICKED:
			message_log("�{�^�������ꂽ");
			break;
		}
		break;
	case EET_MOUSE_INPUT_EVENT:
		message_log("�}�E�X�C�x���g:%d x:%d y:%d wheel:%f",
			event.MouseInput.Event,
			event.MouseInput.X,
			event.MouseInput.Y,
			event.MouseInput.Wheel);
		break;
	case EET_KEY_INPUT_EVENT:
		message_log("�L�[�C�x���g:%x", event.KeyInput.Key);
		{
			int shift = 0;
			if (event.KeyInput.Shift) {
				shift |= 0x01;
			}
			if (event.KeyInput.Control) {
				shift |= 0x04;
			}
		}
		break;
	case EET_LOG_TEXT_EVENT:
		message_log("���O���x��:%d ���O:%s",
			event.LogEvent.Level,
			event.LogEvent.Text);
		break;
	case EET_USER_EVENT:
		message_log("���[�U�C�x���g");
		break;
	}
	return false;
}
