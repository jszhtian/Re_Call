//---------------------------------------------------------------------------
/*
	TVP2 ( T Visual Presenter 2 )  A script authoring tool
	Copyright (C) 2000-2007 W.Dee <dee@kikyou.info> and contributors

	See details of license at "license.txt"
*/
//---------------------------------------------------------------------------
// "Window" TJS Class implementation
//---------------------------------------------------------------------------


#ifndef WindowImplH
#define WindowImplH

#include <Forms.hpp>
#include "WindowIntf.h"


/*[*/
//---------------------------------------------------------------------------
// window message receivers
//---------------------------------------------------------------------------
enum tTVPWMRRegMode { wrmRegister=0, wrmUnregister=1 };
#pragma pack(push, 4)
struct tTVPWindowMessage
{
	unsigned int Msg; // window message
	int WParam;  // WPARAM
	int LParam;  // LPARAM
	int Result;  // result
};
#pragma pack(pop)
typedef bool (__stdcall * tTVPWindowMessageReceiver)
	(void *userdata, tTVPWindowMessage *Message);

#define TVP_WM_DETACH (WM_USER+106)  // before re-generating the window
#define TVP_WM_ATTACH (WM_USER+107)  // after re-generating the window


/*]*/
//---------------------------------------------------------------------------




//---------------------------------------------------------------------------
// Mouse Cursor Management
//---------------------------------------------------------------------------
extern tjs_int TVPGetCursor(const ttstr & name);
//---------------------------------------------------------------------------



//---------------------------------------------------------------------------
// Utility functions
//---------------------------------------------------------------------------
TJS_EXP_FUNC_DEF(tjs_uint32, TVPGetCurrentShiftKeyState, ());
HWND TVPGetModalWindowOwnerHandle();
//---------------------------------------------------------------------------




//---------------------------------------------------------------------------
// Color Format Detection
//---------------------------------------------------------------------------
extern tjs_int TVPDisplayColorFormat;
//---------------------------------------------------------------------------




//---------------------------------------------------------------------------
// Screen Mode management
//---------------------------------------------------------------------------

//! @brief		Structure for monitor screen mode
struct tTVPScreenMode
{
	tjs_int Width; //!< width of screen in pixel
	tjs_int Height; //!< height of screen in pixel
	tjs_int BitsPerPixel; //!< bits per pixel (0 = unspecified)

	ttstr Dump() const
	{
		return
			DumpHeightAndWidth() +
			TJS_W(", BitsPerPixel=") + (BitsPerPixel?ttstr(BitsPerPixel):ttstr(TJS_W("unspecified")));
	}

	ttstr DumpHeightAndWidth() const
	{
		return
			TJS_W("Width=") + ttstr(Width) +
			TJS_W(", Height=") + ttstr(Height);
	}

	bool operator < (const tTVPScreenMode & rhs) const {
		tjs_int area_this = Width * Height;
		tjs_int area_rhs  = rhs.Width * rhs.Height;
		if(area_this < area_rhs) return true;
		if(area_this > area_rhs) return false;
		if(BitsPerPixel < rhs.BitsPerPixel) return true;
		if(BitsPerPixel > rhs.BitsPerPixel) return false;
		return false;
	}
};

//! @brief		Structure for monitor screen mode candidate
struct tTVPScreenModeCandidate : tTVPScreenMode
{
	tjs_int ZoomNumer; //!< zoom ratio numer
	tjs_int ZoomDenom; //!< zoom ratio denom
	tjs_int RankZoomIn;
	tjs_int RankBPP;
	tjs_int RankZoomBeauty;
	tjs_int RankSize; //!< candidate preference priority (lower value is higher preference)

	ttstr Dump() const
	{
		return tTVPScreenMode::Dump() +
			TJS_W(", ZoomNumer=") + ttstr(ZoomNumer) +
			TJS_W(", ZoomDenom=") + ttstr(ZoomDenom) +
			TJS_W(", RankZoomIn=") + ttstr(RankZoomIn) +
			TJS_W(", RankBPP=") + ttstr(RankBPP) +
			TJS_W(", RankZoomBeauty=") + ttstr(RankZoomBeauty) +
			TJS_W(", RankSize=") + ttstr(RankSize);
	}

	bool operator < (const tTVPScreenModeCandidate & rhs) const{
		if(RankZoomIn < rhs.RankZoomIn) return true;
		if(RankZoomIn > rhs.RankZoomIn) return false;
		if(RankBPP < rhs.RankBPP) return true;
		if(RankBPP > rhs.RankBPP) return false;
		if(RankZoomBeauty < rhs.RankZoomBeauty) return true;
		if(RankZoomBeauty > rhs.RankZoomBeauty) return false;
		if(RankSize < rhs.RankSize) return true;
		if(RankSize > rhs.RankSize) return false;
		return false;
	}
};

interface IDirectDraw2;
interface IDirectDrawSurface;
interface IDirectDrawClipper;
extern void TVPTestDisplayMode(tjs_int w, tjs_int h, tjs_int & bpp);
extern void TVPSwitchToFullScreen(HWND window, tjs_int w, tjs_int h);
extern void TVPRevertFromFullScreen(HWND window);
TJS_EXP_FUNC_DEF(void, TVPEnsureDirectDrawObject, ());
extern tTVPScreenModeCandidate TVPFullScreenMode;
/*[*/
//---------------------------------------------------------------------------
// DirectDraw former declaration
//---------------------------------------------------------------------------
#ifndef __DDRAW_INCLUDED__
struct IDirectDraw2;
struct IDirectDrawSurface;
struct IDirectDrawClipper;
#endif

/*]*/
TJS_EXP_FUNC_DEF(IDirectDraw2 *,  TVPGetDirectDrawObjectNoAddRef, ());
TJS_EXP_FUNC_DEF(IDirectDrawSurface *, TVPGetDDPrimarySurfaceNoAddRef, ());
TJS_EXP_FUNC_DEF(void, TVPSetDDPrimaryClipper, (IDirectDrawClipper * clipper));
extern void TVPMinimizeFullScreenWindowAtInactivation();
extern void TVPRestoreFullScreenWindowAtActivation();
//---------------------------------------------------------------------------










//---------------------------------------------------------------------------
// tTJSNI_Window : Window Native Instance
//---------------------------------------------------------------------------
class TTVPWindowForm;
class iTVPDrawDevice;
class tTJSNI_BaseLayer;
class tTJSNI_Window : public tTJSNI_BaseWindow
{
	TTVPWindowForm *Form;

public:
	tTJSNI_Window();
	tjs_error TJS_INTF_METHOD Construct(tjs_int numparams, tTJSVariant **param,
		iTJSDispatch2 *tjs_obj);
	void TJS_INTF_METHOD Invalidate();
	bool CloseFlag;

public:
	bool CanDeliverEvents() const; // tTJSNI_BaseWindow::CanDeliverEvents override

public:
	TTVPWindowForm * GetForm() const { return Form; }
	void NotifyWindowClose();

	void SendCloseMessage();

	void TickBeat();

private:
	bool GetWindowActive();

public:
//-- draw device
	virtual void ResetDrawDevice();

//-- event control
	virtual void PostInputEvent(const ttstr &name, iTJSDispatch2 * params);  // override

//-- interface to layer manager
	void TJS_INTF_METHOD NotifySrcResize(); // is called from primary layer

	void TJS_INTF_METHOD SetDefaultMouseCursor(); // set window mouse cursor to default
	void TJS_INTF_METHOD SetMouseCursor(tjs_int cursor); // set window mouse cursor
	void TJS_INTF_METHOD GetCursorPos(tjs_int &x, tjs_int &y);
	void TJS_INTF_METHOD SetCursorPos(tjs_int x, tjs_int y);
	void TJS_INTF_METHOD SetHintText(const ttstr & text);
	void TJS_INTF_METHOD SetAttentionPoint(tTJSNI_BaseLayer *layer,
		tjs_int l, tjs_int t);
	void TJS_INTF_METHOD DisableAttentionPoint();
	void TJS_INTF_METHOD SetImeMode(tTVPImeMode mode);
	void SetDefaultImeMode(tTVPImeMode mode);
	tTVPImeMode GetDefaultImeMode() const;
	void TJS_INTF_METHOD ResetImeMode();

//-- update managment
	void BeginUpdate(const tTVPComplexRect &rects);
	void EndUpdate();

//-- interface to MenuItem object
	TMenuItem *  GetRootMenuItem();
	void SetMenuBarVisible(bool b);
	bool GetMenuBarVisible() const;

//-- interface to VideoOverlay object
public:
	HWND GetSurfaceWindowHandle();
	HWND GetWindowHandle(tjs_int &ofsx, tjs_int &ofsy);

	void ReadjustVideoRect();
	void WindowMoved();
	void DetachVideoOverlay();

//-- interface to plugin
	void ZoomRectangle(
		tjs_int & left, tjs_int & top,
		tjs_int & right, tjs_int & bottom);
	HWND GetWindowHandleForPlugin();
	void RegisterWindowMessageReceiver(tTVPWMRRegMode mode,
		void * proc, const void *userdata);

//-- methods
	void Close();
	void OnCloseQueryCalled(bool b);

	void BeginMove();
	void BringToFront();
	void Update(tTVPUpdateType);
	void ShowModal();

	void HideMouseCursor();

//-- properties
	bool GetVisible() const;
	void SetVisible(bool s);

	void GetCaption(ttstr & v) const;
	void SetCaption(const ttstr & v);

	void SetWidth(tjs_int w);
	tjs_int GetWidth() const;
	void SetHeight(tjs_int h);
	tjs_int GetHeight() const;
	void SetSize(tjs_int w, tjs_int h);

	void SetLeft(tjs_int l);
	tjs_int GetLeft() const;
	void SetTop(tjs_int t);
	tjs_int GetTop() const;
	void SetPosition(tjs_int l, tjs_int t);

	void SetLayerLeft(tjs_int l);
	tjs_int GetLayerLeft() const;
	void SetLayerTop(tjs_int t);
	tjs_int GetLayerTop() const;
	void SetLayerPosition(tjs_int l, tjs_int t);


	void SetInnerSunken(bool b);
	bool GetInnerSunken() const;

	void SetInnerWidth(tjs_int w);
	tjs_int GetInnerWidth() const;
	void SetInnerHeight(tjs_int h);
	tjs_int GetInnerHeight() const;

	void SetInnerSize(tjs_int w, tjs_int h);

	void SetBorderStyle(tTVPBorderStyle st);
	tTVPBorderStyle GetBorderStyle() const;

	void SetStayOnTop(bool b);
	bool GetStayOnTop() const;

	void SetShowScrollBars(bool b);
	bool GetShowScrollBars() const;

	void SetFullScreen(bool b);
	bool GetFullScreen() const;

	void SetUseMouseKey(bool b);
	bool GetUseMouseKey() const;

	void SetTrapKey(bool b);
	bool GetTrapKey() const;

	void SetMaskRegion(tjs_int threshold);
	void RemoveMaskRegion();

	void SetMouseCursorState(tTVPMouseCursorState mcs);
    tTVPMouseCursorState GetMouseCursorState() const;

	void SetFocusable(bool b);
	bool GetFocusable();

	void SetZoom(tjs_int numer, tjs_int denom);
	void SetZoomNumer(tjs_int n);
	tjs_int GetZoomNumer() const;
	void SetZoomDenom(tjs_int n);
	tjs_int GetZoomDenom() const;

protected:
};
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#endif
