#pragma comment(lib, "Irrlicht.lib")
#include "ncbind/ncbind.hpp"
#include "IrrlichtDrawDevice.h"
#include "SWFMovie.hpp"

/**
 * ���O�o�͗p
 */
void
message_log(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	char msg[1024];
	_vsnprintf(msg, 1024, format, args);
	TVPAddLog(ttstr(msg));
	va_end(args);
}

/**
 * �G���[���O�o�͗p
 */
void
error_log(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	char msg[1024];
	_vsnprintf(msg, 1024, format, args);
	TVPAddImportantLog(ttstr(msg));
	va_end(args);
}

/**
 * Irrlicht �x�[�X DrawDevice �̃N���X
 */
class IrrlichtDrawDevice {
public:
	// �f�o�C�X���
	tTVPIrrlichtDrawDevice *device;
public:

	/**
	 * �R���X�g���N�^
	 */
	IrrlichtDrawDevice() {
		device = new tTVPIrrlichtDrawDevice();
	}

	~IrrlichtDrawDevice() {
		if (device) {
			device->Destruct();
			device = NULL;
		}
	}

	/**
	 * @return �f�o�C�X���
	 */
	tjs_int64 GetDevice() {
		return reinterpret_cast<tjs_int64>((tTVPDrawDevice*)device);
	}

	// ---------------------------------------------
	// �ȉ� Irrlicht �𐧌䂷�邽�߂̋@�\�������ǉ��\��
	// ---------------------------------------------

	// SWF �t�@�C�������H
	void loadSWF(const char *name) {
		if (device) {
			device->loadSWF(name);
		}
	}

};


NCB_REGISTER_CLASS(IrrlichtDrawDevice) {
	NCB_CONSTRUCTOR(());
	NCB_PROPERTY_RO(interface, GetDevice);
	NCB_METHOD(loadSWF);
}

/**
 * �t�@�C�����ϊ��p proxy
 */
void swfload(SWFMovie *swf, const char *name)
{
	ttstr path(name);
	TVPGetLocalName(path);
	int len = path.GetNarrowStrLen() + 1;
	char *filename = new char[len];
	path.ToNarrowStr(filename, len);
	swf->load(filename);
	delete filename;
}

NCB_REGISTER_CLASS(SWFMovie) {
	NCB_CONSTRUCTOR(());
	NCB_METHOD_PROXY(load, swfload);
	NCB_METHOD(update);
	NCB_METHOD(notifyMouse);
	NCB_METHOD(play);
	NCB_METHOD(stop);
	NCB_METHOD(restart);
	NCB_METHOD(back);
	NCB_METHOD(next);
	NCB_METHOD(gotoFrame);
}

extern void initSWFMovie();
extern void destroySWFMovie();

NCB_POST_REGIST_CALLBACK(initSWFMovie);
NCB_PRE_UNREGIST_CALLBACK(destroySWFMovie);
