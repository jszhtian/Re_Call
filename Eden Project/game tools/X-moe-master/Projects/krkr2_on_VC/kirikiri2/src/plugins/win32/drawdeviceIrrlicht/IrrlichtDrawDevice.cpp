#include <windows.h>
#include "IrrlichtDrawDevice.h"
#include "LayerManagerInfo.h"

extern void message_log(const char* format, ...);
extern void error_log(const char *format, ...);

using namespace irr;
using namespace core;
using namespace video;
using namespace scene;
using namespace io;
using namespace gui;

/**
 * コンストラクタ
 */
tTVPIrrlichtDrawDevice::tTVPIrrlichtDrawDevice()
{
	device = NULL;
	driver = NULL;
	initSWF();
}

/**
 * デストラクタ
 */
tTVPIrrlichtDrawDevice::~tTVPIrrlichtDrawDevice()
{
	deinitSWF();
	detach();
}

/**
 * テクスチャの割り当て
 * @param manager レイヤマネージャ
 */
void 
tTVPIrrlichtDrawDevice::allocInfo(iTVPLayerManager * manager)
{
	if (driver) {

		// テクスチャ割り当てXXX サイズ判定が必要…
		ITexture *texture = driver->addTexture(core::dimension2d<s32>(1024, 1024), "layer", ECF_A8R8G8B8);
		if (texture == NULL) {
			TVPThrowExceptionMessage(L"テクスチャの割り当てに失敗しました");
		}

		manager->SetDrawDeviceData((void*)new LayerManagerInfo(texture));

		// 更新要求
		tjs_int w, h;
		if (manager->GetPrimaryLayerSize(w, h)) {
			manager->RequestInvalidation(tTVPRect(0,0,w,h));
		}
	}
}

/**
 *  テクスチャの解放
 * @param manager レイヤマネージャ
 */
void
tTVPIrrlichtDrawDevice::freeInfo(iTVPLayerManager * manager)
{
	if (driver) {
		LayerManagerInfo *info = (LayerManagerInfo*)manager->GetDrawDeviceData();
		if (info != NULL) {
			driver->removeTexture(info->texture);
			manager->SetDrawDeviceData(NULL);
			delete info;
		}
	}
}

/**
 * デバイスの生成
 * @param hwnd ハンドル
 * @param ドライバの種類
 */
static IrrlichtDevice *
create(HWND hwnd, irr::video::E_DRIVER_TYPE type, irr::IEventReceiver *receiver)
{
	SIrrlichtCreationParameters params;
	params.WindowId     = reinterpret_cast<s32>(hwnd);
	params.DriverType    = type;
	params.Bits          = 32;
	params.Stencilbuffer = true;
	params.Vsync = true;
	params.EventReceiver = receiver;
	params.AntiAlias = true;
	return createDeviceEx(params);
}

/**
 * ウインドウの再設定
 * @param hwnd ハンドル
 */
void
tTVPIrrlichtDrawDevice::attach(HWND hwnd)
{
	// デバイス生成
	if ((device = create(hwnd, video::EDT_DIRECT3D9, this))) {
		TVPAddLog(L"DirectX9で初期化");
	} else {
		if ((device = create(hwnd, video::EDT_DIRECT3D8, this))) {
			TVPAddLog(L"DirectX8で初期化");
		} else {
			TVPThrowExceptionMessage(L"Irrlicht デバイスの初期化に失敗しました");
		}
	}
	driver = device->getVideoDriver();
	
	dimension2d<s32> size = driver->getScreenSize();
	message_log("デバイス生成後のスクリーンサイズ:%d, %d", size.Width, size.Height);

	size = driver->getCurrentRenderTargetSize();
	message_log("デバイス生成後のRenderTargetの:%d, %d", size.Width, size.Height);

	// マネージャに対するテクスチャの割り当て
	for (std::vector<iTVPLayerManager *>::iterator i = Managers.begin(); i != Managers.end(); i++) {
		allocInfo(*i);
	}

	// XXX テスト用
	init();
	
	// 駆動開始
	start();
}

/**
 * ウインドウの解除
 */
void
tTVPIrrlichtDrawDevice::detach()
{
	if (device) {
		stop();
		for (std::vector<iTVPLayerManager *>::iterator i = Managers.begin(); i != Managers.end(); i++) {
			freeInfo(*i);
		}
		device->drop();
		device = NULL;
		driver = NULL;
	}
}

/**
 * レイヤマネージャの登録
 * @param manager レイヤマネージャ
 */
void TJS_INTF_METHOD
tTVPIrrlichtDrawDevice::AddLayerManager(iTVPLayerManager * manager)
{
	allocInfo(manager);
	tTVPDrawDevice::AddLayerManager(manager);
}

/**
 * レイヤマネージャの削除
 * @param manager レイヤマネージャ
 */
void TJS_INTF_METHOD
tTVPIrrlichtDrawDevice::RemoveLayerManager(iTVPLayerManager * manager)
{
	freeInfo(manager);
	tTVPDrawDevice::RemoveLayerManager(manager);
}


/***
 * ウインドウの指定
 * @param wnd ウインドウハンドラ
 */
void TJS_INTF_METHOD
tTVPIrrlichtDrawDevice::SetTargetWindow(HWND wnd)
{
	detach();
	if (wnd != NULL) {
		attach(wnd);
	}
}


bool
tTVPIrrlichtDrawDevice::postEvent(SEvent &ev)
{
	if (device) {
		if (device->getGUIEnvironment()->postEventFromUser(ev) ||
			device->getSceneManager()->postEventFromUser(ev)) {
			return true;
		}
	}
	return false;
}

// -------------------------------------------------------------------------------------
// 入力イベント処理用
// -------------------------------------------------------------------------------------

void TJS_INTF_METHOD
tTVPIrrlichtDrawDevice::OnMouseDown(tjs_int x, tjs_int y, tTVPMouseButton mb, tjs_uint32 flags)
{
	if (uiSWF) {
		swfMouseX = x;
		swfMouseY = y;
		switch ((mb & 0xff)) {
		case mbLeft:
			swfMouseButton |= 0x01;
			break;
		case mbMiddle:
			swfMouseButton |= 0x02;
			break;
		case mbRight:
			swfMouseButton |= 0x04;
			break;
		}
	}
	if (driver) {
		SEvent ev;
		ev.EventType = EET_MOUSE_INPUT_EVENT;
		ev.MouseInput.X = x;
		ev.MouseInput.Y = y;
		ev.MouseInput.Wheel = 0;
		switch ((mb & 0xff)) {
		case mbLeft:
			ev.MouseInput.Event = EMIE_LMOUSE_PRESSED_DOWN;
			break;
		case mbMiddle:
			ev.MouseInput.Event = EMIE_MMOUSE_PRESSED_DOWN;
			break;
		case mbRight:
			ev.MouseInput.Event = EMIE_RMOUSE_PRESSED_DOWN;
			break;
		}
		if (postEvent(ev)) {
			return;
		}
	}
	// XXX 画面サイズに応じた補正が必要
	tTVPDrawDevice::OnMouseDown(x, y, mb, flags);
}

void TJS_INTF_METHOD
tTVPIrrlichtDrawDevice::OnMouseUp(tjs_int x, tjs_int y, tTVPMouseButton mb, tjs_uint32 flags)
{
	if (uiSWF) {
		swfMouseX = x;
		swfMouseY = y;
		switch ((mb & 0xff)) {
		case mbLeft:
			swfMouseButton &= ~0x01;
			break;
		case mbMiddle:
			swfMouseButton &= ~0x02;
			break;
		case mbRight:
			swfMouseButton &= ~0x04;
			break;
		}
	}
	if (driver) {
		SEvent ev;
		ev.EventType = EET_MOUSE_INPUT_EVENT;
		ev.MouseInput.X = x;
		ev.MouseInput.Y = y;
		ev.MouseInput.Wheel = 0;
		switch ((mb & 0xff)) {
		case mbLeft:
			ev.MouseInput.Event = EMIE_LMOUSE_LEFT_UP;
			break;
		case mbMiddle:
			ev.MouseInput.Event = EMIE_MMOUSE_LEFT_UP;
			break;
		case mbRight:
			ev.MouseInput.Event = EMIE_RMOUSE_LEFT_UP;
			break;
		}
		if (postEvent(ev)) {
			return;
		}
	}
	// XXX 画面サイズに応じた補正が必要
	tTVPDrawDevice::OnMouseUp(x, y, mb, flags);
}

void TJS_INTF_METHOD
tTVPIrrlichtDrawDevice::OnMouseMove(tjs_int x, tjs_int y, tjs_uint32 flags)
{
	swfMouseX = x;
	swfMouseY = y;
	if (driver) {
		SEvent ev;
		ev.EventType = EET_MOUSE_INPUT_EVENT;
		ev.MouseInput.X = x;
		ev.MouseInput.Y = y;
		ev.MouseInput.Wheel = 0;
		ev.MouseInput.Event = EMIE_MOUSE_MOVED;
		if (postEvent(ev)) {
			return;
		}
	}
	// XXX 画面サイズに応じた補正が必要
	tTVPDrawDevice::OnMouseMove(x, y, flags);
}

void TJS_INTF_METHOD
tTVPIrrlichtDrawDevice::OnKeyDown(tjs_uint key, tjs_uint32 shift)
{
	if (uiSWF) {
		notifyKeySWF(key, true);
	}
	tTVPDrawDevice::OnKeyDown(key, shift);
}

void TJS_INTF_METHOD
tTVPIrrlichtDrawDevice::OnKeyUp(tjs_uint key, tjs_uint32 shift)
{
	if (uiSWF) {
		notifyKeySWF(key, false);
	}
	tTVPDrawDevice::OnKeyUp(key, shift);
}

void TJS_INTF_METHOD
tTVPIrrlichtDrawDevice::OnKeyPress(tjs_char key)
{
	tTVPDrawDevice::OnKeyPress(key);
}

void TJS_INTF_METHOD
tTVPIrrlichtDrawDevice::OnMouseWheel(tjs_uint32 shift, tjs_int delta, tjs_int x, tjs_int y)
{
	tTVPDrawDevice::OnMouseWheel(shift, delta, x, y);
}

// -------------------------------------------------------------------------------------
// 描画処理用
// -------------------------------------------------------------------------------------

/**
 * ビットマップコピー処理開始
 */
void TJS_INTF_METHOD
tTVPIrrlichtDrawDevice::StartBitmapCompletion(iTVPLayerManager * manager)
{
	LayerManagerInfo *info = (LayerManagerInfo*)manager->GetDrawDeviceData();
	if (info) {
		info->lock();
	}
}

/**
 * ビットマップコピー処理
 */
void TJS_INTF_METHOD
tTVPIrrlichtDrawDevice::NotifyBitmapCompleted(iTVPLayerManager * manager,
	tjs_int x, tjs_int y, const void * bits, const BITMAPINFO * bitmapinfo,
	const tTVPRect &cliprect, tTVPLayerType type, tjs_int opacity)
{
	LayerManagerInfo *info = (LayerManagerInfo*)manager->GetDrawDeviceData();
	if (info) {
		info->copy(x, y, bits, bitmapinfo, cliprect, type, opacity);
	}
}

/**
 * ビットマップコピー処理終了
 */
void TJS_INTF_METHOD
tTVPIrrlichtDrawDevice::EndBitmapCompletion(iTVPLayerManager * manager)
{
	LayerManagerInfo *info = (LayerManagerInfo*)manager->GetDrawDeviceData();
	if (info) {
		info->unlock();
	}
}

//---------------------------------------------------------------------------

/**
 * テスト用初期化処理
 */
void
tTVPIrrlichtDrawDevice::init()
{
	// GUI 環境のテスト
//	IGUIEnvironment* env = device->getGUIEnvironment();
//	env->addButton(rect<s32>(10,210,110,210 + 32), 0, 101, L"TEST BUTTON", L"Button Test");
//	env->addButton(rect<s32>(10,250,110,250 + 32), 0, 102, L"てすとぼたん", L"ボタンのテスト");
	
	/// シーンマネージャでの irr ファイルロードのテスト
	ISceneManager* smgr = device->getSceneManager();
	smgr->loadScene("data/sample/example.irr");
	smgr->addCameraSceneNode(0, vector3df(0,30,-40), vector3df(0,5,0));
}

