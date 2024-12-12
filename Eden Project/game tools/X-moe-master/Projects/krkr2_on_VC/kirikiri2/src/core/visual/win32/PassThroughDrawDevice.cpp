//---------------------------------------------------------------------------
/*
	TVP2 ( T Visual Presenter 2 )  A script authoring tool
	Copyright (C) 2000-2007 W.Dee <dee@kikyou.info> and contributors

	See details of license at "license.txt"
*/
//---------------------------------------------------------------------------
//!@file "PassThrough" �`��f�o�C�X�Ǘ�
//---------------------------------------------------------------------------

#include "tjsCommHead.h"
#include <vfw.h>
#include "DrawDevice.h"
#include "PassThroughDrawDevice.h"
#include "LayerIntf.h"
#include "MsgIntf.h"
#include "SysInitIntf.h"
#include "WindowIntf.h"
#include "DebugIntf.h"
#define DIRECTDRAW_VERSION 0x0300
#include <ddraw.h>



//---------------------------------------------------------------------------
// �I�v�V����
//---------------------------------------------------------------------------
static bool TVPPassThroughOptionsInit = false;
static bool TVPWaitVSync = false;
static bool TVPZoomInterpolation = true;
static bool TVPForceDoublebuffer = false;
static bool TVPDBGDIPrefered = false;
//---------------------------------------------------------------------------
static void TVPInitPassThroughOptions()
{
	if(TVPPassThroughOptionsInit) return;

	bool initddraw = false;
	tTJSVariant val;

	if(TVPGetCommandLine(TJS_W("-waitvsync"), &val))
	{
		ttstr str(val);
		if(str == TJS_W("yes"))
		{
			TVPWaitVSync = true;
			initddraw = true;
		}
	}

	if(TVPGetCommandLine(TJS_W("-dbstyle"), &val) )
	{
		ttstr str(val);
		if(str == TJS_W("none") || str == TJS_W("no"))
			TVPForceDoublebuffer = false;
		if(str == TJS_W("auto") || str == TJS_W("gdi") || str == TJS_W("ddraw") || str == TJS_W("yes"))
			TVPForceDoublebuffer = true,
			initddraw = true;
		if(str == TJS_W("gdi")) TVPDBGDIPrefered = true;
	}

	if(TVPGetCommandLine(TJS_W("-smoothzoom"), &val))
	{
		ttstr str(val);
		if(str == TJS_W("no"))
			TVPZoomInterpolation = false;
		else
			TVPZoomInterpolation = true;
	}

	if(initddraw) TVPEnsureDirectDrawObject();

	TVPPassThroughOptionsInit = true;
}
//---------------------------------------------------------------------------





//---------------------------------------------------------------------------
//! @brief	PassThrough �ŗp����`����@�p�C���^�[�t�F�[�X
//---------------------------------------------------------------------------
class tTVPDrawer
{
protected:
	tTVPPassThroughDrawDevice * Device;
	tTVPRect DestRect;
	tjs_int SrcWidth;
	tjs_int SrcHeight;
	HWND TargetWindow;
	HDRAWDIB DrawDibHandle;
	bool DrawUpdateRectangle;
public:
	tTVPDrawer(tTVPPassThroughDrawDevice * device)
	{
		Device = device;
		SrcWidth = 0;
		SrcHeight = 0;
		DestRect.clear();
		TargetWindow = NULL;
		DrawDibHandle = NULL;
		DrawUpdateRectangle = NULL;
	} 
	virtual ~tTVPDrawer()
	{
		if(DrawDibHandle) DrawDibClose(DrawDibHandle), DrawDibHandle = NULL;
	}

	virtual bool SetDestRectangle(const tTVPRect & rect) { DestRect = rect; return true; }
	virtual bool NotifyLayerResize(tjs_int w, tjs_int h) { SrcWidth = w; SrcHeight = h; return true; }
	virtual void SetTargetWindow(HWND wnd)
	{
		if(DrawDibHandle) DrawDibClose(DrawDibHandle), DrawDibHandle = NULL;
		TargetWindow = wnd;
		DrawDibHandle = DrawDibOpen();
	}
	virtual void StartBitmapCompletion() = 0;
	virtual void NotifyBitmapCompleted(tjs_int x, tjs_int y,
		const void * bits, const BITMAPINFO * bitmapinfo,
		const tTVPRect &cliprect) = 0;
	virtual void EndBitmapCompletion() = 0;
	virtual void SetShowUpdateRect(bool b)  { DrawUpdateRectangle = b; }
};
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
//! @brief	GDI�ɂ��`���K�v�Ƃ����{�N���X
//---------------------------------------------------------------------------
class tTVPDrawer_GDI : public tTVPDrawer
{
	typedef tTVPDrawer inherited;
protected:
	HDC TargetDC;

public:
	//! @brief	�R���X�g���N�^
	tTVPDrawer_GDI(tTVPPassThroughDrawDevice * device) : tTVPDrawer(device)
	{
		TargetDC = NULL;
	}

	//! @brief	�f�X�g���N�^
	~tTVPDrawer_GDI()
	{
		if(TargetDC && TargetWindow) ReleaseDC(TargetWindow, TargetDC);
	}

	void SetTargetWindow(HWND wnd)
	{
		if(wnd)
		{
			// �`��p DC ���擾����
			TargetDC = GetDC(wnd);
		}
		else
		{
			// �`��p DC ���J������
			if(TargetDC) ReleaseDC(TargetWindow, TargetDC), TargetDC = NULL;
		}

		inherited::SetTargetWindow(wnd);

	}
};
//---------------------------------------------------------------------------



//---------------------------------------------------------------------------
//! @brief	DrawDib�ɂ��o�b�t�@�����`����s����{�N���X
//---------------------------------------------------------------------------
class tTVPDrawer_DrawDibNoBuffering : public tTVPDrawer_GDI
{
	typedef tTVPDrawer_GDI inherited;

	HPEN BluePen;
	HPEN YellowPen;

public:
	//! @brief	�R���X�g���N�^
	tTVPDrawer_DrawDibNoBuffering(tTVPPassThroughDrawDevice * device) : tTVPDrawer_GDI(device)
	{
		BluePen = NULL;
		YellowPen = NULL;
	}

	//! @brief	�f�X�g���N�^
	~tTVPDrawer_DrawDibNoBuffering()
	{
		if(BluePen)   DeleteObject(BluePen);
		if(YellowPen) DeleteObject(YellowPen);
	}

	bool SetDestRectangle(const tTVPRect & rect)
	{
		// ���̃f�o�C�X�ł͊g��k���͂ł��Ȃ��̂�
		// �g��k�����K�v�ȏꍇ�� false ��Ԃ�
		tjs_int w, h;
		Device->GetSrcSize(w, h);
		if(rect.get_width() != w || rect.get_height() != h)
			return false;

		return inherited::SetDestRectangle(rect);
	}

	bool NotifyLayerResize(tjs_int w, tjs_int h)
	{
		return inherited::NotifyLayerResize(w, h);
	}

	void SetTargetWindow(HWND wnd)
	{
		inherited::SetTargetWindow(wnd);
	}

	void StartBitmapCompletion()
	{
		// ��邱�ƂȂ�
	}

	void NotifyBitmapCompleted(tjs_int x, tjs_int y, const void * bits, const BITMAPINFO * bitmapinfo,
		const tTVPRect &cliprect)
	{
		// DrawDibDraw �ɂ� TargetDC �ɕ`����s��
		if(DrawDibHandle && TargetDC)
			DrawDibDraw(DrawDibHandle,
				TargetDC,
				x + DestRect.left,
				y + DestRect.top,
				cliprect.get_width(),
				cliprect.get_height(),
				const_cast<BITMAPINFOHEADER*>(reinterpret_cast<const BITMAPINFOHEADER*>(bitmapinfo)),
				const_cast<void*>(bits),
				cliprect.left,
				cliprect.top,
				cliprect.get_width(),
				cliprect.get_height(),
				0);

		// �X�V��`�̕\��
		if(DrawUpdateRectangle)
		{
			if(!BluePen) BluePen = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
			if(!YellowPen) YellowPen = CreatePen(PS_SOLID, 1, RGB(255, 255, 0));

			HPEN oldpen;

			int ROP_save = GetROP2(TargetDC);

			tjs_int rleft   = x + DestRect.left;
			tjs_int rtop    = y + DestRect.top;
			tjs_int rright  = rleft + cliprect.get_width();
			tjs_int rbottom = rtop  + cliprect.get_height();

			POINT points[5];
			points[0].x = rleft;
			points[0].y = rtop;
			points[1].x = rright -1;
			points[1].y = rtop;
			points[2].x = rright -1;
			points[2].y = rbottom -1;
			points[3].x = rleft;
			points[3].y = rbottom -1;
			points[4] = points[0];

			oldpen = (HPEN)SelectObject(TargetDC, BluePen);
			SetROP2(TargetDC, R2_NOTMASKPEN);
			Polyline(TargetDC, points, 4);

			SelectObject(TargetDC, YellowPen);
			SetROP2(TargetDC, R2_MERGEPEN);
			Polyline(TargetDC, points, 5);

			SelectObject(TargetDC, oldpen);
		}
	}

	void EndBitmapCompletion()
	{
		// ��邱�ƂȂ�
	}
};
//---------------------------------------------------------------------------





//---------------------------------------------------------------------------
//! @brief	GDI�ɂ��_�u���o�b�t�@�����O���s���N���X
//---------------------------------------------------------------------------
class tTVPDrawer_GDIDoubleBuffering : public tTVPDrawer_GDI
{
	typedef tTVPDrawer_GDI inherited;
	HBITMAP OffScreenBitmap; //!< �I�t�X�N���[���r�b�g�}�b�v
	HDC OffScreenDC; //!< �I�t�X�N���[�� DC
	HBITMAP OldOffScreenBitmap; //!< OffScreenDC �ɈȑO�I������Ă��� �r�b�g�}�b�v

public:
	//! @brief	�R���X�g���N�^
	tTVPDrawer_GDIDoubleBuffering(tTVPPassThroughDrawDevice * device) : tTVPDrawer_GDI(device)
	{
		OffScreenBitmap = NULL;
		OffScreenDC = NULL;
		OldOffScreenBitmap = NULL;
	}

	//! @brief	�f�X�g���N�^
	~tTVPDrawer_GDIDoubleBuffering()
	{
		DestroyBitmap();
	}

	void DestroyBitmap()
	{
		if(OffScreenBitmap && OffScreenDC)
		{
			SelectObject(OffScreenDC, OldOffScreenBitmap), OldOffScreenBitmap = NULL;
			if(OffScreenBitmap) DeleteObject(OffScreenBitmap), OffScreenBitmap = NULL;
			if(OffScreenDC)     DeleteDC(OffScreenDC), OffScreenDC = NULL;
		}
	}

	void CreateBitmap()
	{
		// �X�N���[���݊��� DDB ���쐬����B
		// ����͂����Ă��̏ꍇ�A�r�f�I��������ɍ쐬�����B
		DestroyBitmap();
		if(TargetWindow && SrcWidth > 0 && SrcHeight > 0)
		{
			try
			{
				HDC screendc = GetDC(TargetWindow);
				if(!screendc) TVPThrowExceptionMessage(TJS_W("Failed to create screen DC"));
				OffScreenBitmap = CreateCompatibleBitmap(screendc, SrcWidth, SrcHeight);
				if(!OffScreenBitmap) TVPThrowExceptionMessage(TJS_W("Failed to create offscreen bitmap"));
				OffScreenDC     = CreateCompatibleDC(screendc);
				if(!OffScreenDC) TVPThrowExceptionMessage(TJS_W("Failed to create offscreen DC"));
				ReleaseDC(TargetWindow, screendc);
				OldOffScreenBitmap = (HBITMAP)SelectObject(OffScreenDC, OffScreenBitmap);
			}
			catch(...)
			{
				DestroyBitmap();
				throw;
			}
		}
	}

	bool SetDestRectangle(const tTVPRect & rect)
	{
		return inherited::SetDestRectangle(rect);
	}

	bool NotifyLayerResize(tjs_int w, tjs_int h)
	{
		if(inherited::NotifyLayerResize(w, h))
		{
			CreateBitmap();
			return true;
		}
		return false;
	}

	void SetTargetWindow(HWND wnd)
	{
		inherited::SetTargetWindow(wnd);
		CreateBitmap();
	}

	void StartBitmapCompletion()
	{
		// ��邱�ƂȂ�
	}

	void NotifyBitmapCompleted(tjs_int x, tjs_int y, const void * bits, const BITMAPINFO * bitmapinfo,
		const tTVPRect &cliprect)
	{
		// DrawDibDraw �ɂ� OffScreenDC �ɕ`����s��
		if(DrawDibHandle && OffScreenDC)
			DrawDibDraw(DrawDibHandle,
				OffScreenDC,
				x,
				y,
				cliprect.get_width(),
				cliprect.get_height(),
				const_cast<BITMAPINFOHEADER*>(reinterpret_cast<const BITMAPINFOHEADER*>(bitmapinfo)),
				const_cast<void*>(bits),
				cliprect.left,
				cliprect.top,
				cliprect.get_width(),
				cliprect.get_height(),
				0);
	}

	void EndBitmapCompletion()
	{
		if(TargetDC)
		{
			// �I�t�X�N���[���r�b�g�}�b�v�� TargetDC �ɓ]������
			if(TVPZoomInterpolation)
				SetStretchBltMode(TargetDC, HALFTONE);
			else
				SetStretchBltMode(TargetDC, COLORONCOLOR);
			SetBrushOrgEx(TargetDC, 0, 0, NULL);

			StretchBlt(TargetDC,
				DestRect.left,
				DestRect.top,
				DestRect.get_width(),
				DestRect.get_height(),
				OffScreenDC,
				0,
				0,
				SrcWidth,
				SrcHeight,
				SRCCOPY);
		}
	}
};
//---------------------------------------------------------------------------






//---------------------------------------------------------------------------
//! @brief	DirectDraw�ɂ��_�u���o�b�t�@�����O���s���N���X
//---------------------------------------------------------------------------
class tTVPDrawer_DDDoubleBuffering : public tTVPDrawer
{
	typedef tTVPDrawer inherited;

	HDC OffScreenDC;
	IDirectDrawSurface * Surface;
	IDirectDrawClipper * Clipper;

public:
	//! @brief	�R���X�g���N�^
	tTVPDrawer_DDDoubleBuffering(tTVPPassThroughDrawDevice * device) : tTVPDrawer(device)
	{
		TVPEnsureDirectDrawObject();
		OffScreenDC = NULL;
		Surface = NULL;
		Clipper = NULL;
	}

	//! @brief	�f�X�g���N�^
	~tTVPDrawer_DDDoubleBuffering()
	{
		DestroyOffScreenSurface();
	}

	void DestroyOffScreenSurface()
	{
		if(OffScreenDC && Surface) Surface->ReleaseDC(OffScreenDC);
		if(Surface) Surface->Release(), Surface = NULL;
		if(Clipper) Clipper->Release(), Clipper = NULL;
	}

	void InvalidateAll()
	{
		// ���C�����Z���ʂ����ׂă��N�G�X�g����
		// �T�[�t�F�[�X�� lost �����ۂɓ��e���č\�z����ړI�ŗp����
		Device->RequestInvalidation(tTVPRect(0, 0, DestRect.get_width(), DestRect.get_height()));
	}

	void CreateOffScreenSurface()
	{
		// �I�t�X�N���[���T�[�t�F�[�X��ݒ肷��
		DestroyOffScreenSurface();
		if(TargetWindow && SrcWidth > 0 && SrcHeight > 0)
		{
			IDirectDraw2 *object = TVPGetDirectDrawObjectNoAddRef();
			if(!object) TVPThrowExceptionMessage(TJS_W("DirectDraw not available"));

			// allocate secondary off-screen buffer
			DDSURFACEDESC ddsd;
			ZeroMemory(&ddsd, sizeof(ddsd));
			ddsd.dwSize = sizeof(ddsd);
			ddsd.dwFlags = DDSD_WIDTH | DDSD_HEIGHT | DDSD_CAPS;
			ddsd.dwWidth = SrcWidth;
			ddsd.dwHeight = SrcHeight;
			ddsd.ddsCaps.dwCaps =
				DDSCAPS_OFFSCREENPLAIN | DDSCAPS_VIDEOMEMORY | DDSCAPS_LOCALVIDMEM;
			HRESULT hr;

			hr = object->CreateSurface(&ddsd, &Surface, NULL);

			if(hr != DD_OK)
				TVPThrowExceptionMessage(TJS_W("Cannot allocate off-screen surface/HR=%1"),
					TJSInt32ToHex(hr, 8));

			// check whether the surface is on video memory
			ZeroMemory(&ddsd, sizeof(ddsd));
			ddsd.dwSize = sizeof(ddsd);

			hr = Surface->GetSurfaceDesc(&ddsd);

			if(hr != DD_OK)
			{
				Surface->Release(), Surface = NULL;
				TVPThrowExceptionMessage(TJS_W("Cannot get surface description/HR=%1"),
					TJSInt32ToHex(hr, 8));
			}

			if(ddsd.ddsCaps.dwCaps & DDSCAPS_VIDEOMEMORY &&
				ddsd.ddsCaps.dwCaps & DDSCAPS_LOCALVIDMEM)
			{
				// ok
			}
			else
			{
				Surface->Release(), Surface = NULL;
				TVPThrowExceptionMessage(TJS_W("Cannot allocate the surface on the local video memory"),
					TJSInt32ToHex(hr, 8));
			}


			// create clipper object
			hr = object->CreateClipper(0, &Clipper, NULL);
			if(hr != DD_OK)
			{
				Surface->Release(), Surface = NULL;
				TVPThrowExceptionMessage(TJS_W("Cannot create a clipper object/HR=%1"),
					TJSInt32ToHex(hr, 8));
			}
			hr = Clipper->SetHWnd(0, TargetWindow);
			if(hr != DD_OK)
			{
				Clipper->Release(), Clipper = NULL;
				Surface->Release(), Surface = NULL;
				TVPThrowExceptionMessage(TJS_W("Cannot set the window handle to the clipper object/HR=%1"),
					TJSInt32ToHex(hr, 8));
			}
		}
	}

	bool SetDestRectangle(const tTVPRect & rect)
	{
		return inherited::SetDestRectangle(rect);
	}

	bool NotifyLayerResize(tjs_int w, tjs_int h)
	{
		if(inherited::NotifyLayerResize(w, h))
		{
			try
			{
				CreateOffScreenSurface();
			}
			catch(const eTJS & e)
			{
				TVPAddLog(TJS_W("Failed to create DirectDraw off-screen buffer: ") + e.GetMessage());
				return false;
			}
			catch(...)
			{
				TVPAddLog(TJS_W("Failed to create DirectDraw off-screen buffer: unknown reason"));
				return false;
			}
			return true;
		}
		return false;
	}

	void SetTargetWindow(HWND wnd)
	{
		inherited::SetTargetWindow(wnd);
		CreateOffScreenSurface();
	}

	void StartBitmapCompletion()
	{
		// retrieve DC
		if(Surface && TargetWindow)
		{
			HDC dc;
			HRESULT hr = Surface->GetDC(&dc);
			if(hr == DDERR_SURFACELOST)
			{
				Surface->Restore();
				InvalidateAll();  // causes reconstruction of off-screen image
				hr = Surface->GetDC(&dc);
			}

			if(hr != DD_OK)
			{
				TVPThrowExceptionMessage(TJS_W("Off-screen surface, IDirectDrawSurface::GetDC failed/HR=%1"),
					TJSInt32ToHex(hr, 8));
			}

			OffScreenDC = dc;
		}
	}

	void NotifyBitmapCompleted(tjs_int x, tjs_int y, const void * bits, const BITMAPINFO * bitmapinfo,
		const tTVPRect &cliprect)
	{
		// DrawDibDraw �ɂ� OffScreenDC �ɕ`����s��
		if(DrawDibHandle && OffScreenDC && TargetWindow)
			DrawDibDraw(DrawDibHandle,
				OffScreenDC,
				x,
				y,
				cliprect.get_width(),
				cliprect.get_height(),
				const_cast<BITMAPINFOHEADER*>(reinterpret_cast<const BITMAPINFOHEADER*>(bitmapinfo)),
				const_cast<void*>(bits),
				cliprect.left,
				cliprect.top,
				cliprect.get_width(),
				cliprect.get_height(),
				0);
	}

	void EndBitmapCompletion()
	{
		if(!TargetWindow) return;
		if(!Surface) return;
		if(!OffScreenDC) return;

		Surface->ReleaseDC(OffScreenDC), OffScreenDC = NULL;

		// Blt to the primary surface
		IDirectDrawSurface *pri = TVPGetDDPrimarySurfaceNoAddRef();
		if(!pri)
			TVPThrowExceptionMessage(TJS_W("Cannot retrieve primary surface object"));

		// set clipper
		TVPSetDDPrimaryClipper(Clipper);

		// get PaintBox's origin
		POINT origin; origin.x = DestRect.left, origin.y = DestRect.top;
		ClientToScreen(TargetWindow, &origin);

		// entire of the bitmap is to be transfered (this is not optimal. FIX ME!)
		RECT drect;
		drect.left   = origin.x;
		drect.top    = origin.y;
		drect.right  = origin.x + DestRect.get_width();
		drect.bottom = origin.y + DestRect.get_height();

		RECT srect;
		srect.left = 0;
		srect.top = 0;
		srect.right  = SrcWidth;
		srect.bottom = SrcHeight;

		HRESULT hr = pri->Blt(&drect, Surface, &srect, DDBLT_WAIT, NULL);
		if(hr == DDERR_SURFACELOST)
		{
			pri->Restore();
			Surface->Restore();
			InvalidateAll();  // causes reconstruction of off-screen image
		}
		else if(hr == DDERR_INVALIDRECT)
		{
			// ignore this error
		}
		else if(hr != DD_OK)
		{
			TVPThrowExceptionMessage(TJS_W("Primary surface, IDirectDrawSurface::Blt failed/HR=%1"),
				TJSInt32ToHex(hr, 8));
		}
	}
};
//---------------------------------------------------------------------------









//---------------------------------------------------------------------------
tTVPPassThroughDrawDevice::tTVPPassThroughDrawDevice()
{
	TVPInitPassThroughOptions(); // read and initialize options
	TargetWindow = NULL;
	Drawer = NULL;
	DrawerType = dtNone;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
tTVPPassThroughDrawDevice::~tTVPPassThroughDrawDevice()
{
	DestroyDrawer();
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
void tTVPPassThroughDrawDevice::DestroyDrawer()
{
	if(Drawer) delete Drawer, Drawer = NULL;
	DrawerType = dtNone;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
void tTVPPassThroughDrawDevice::EnsureDrawer(tDrawerType failedtype)
{
	// ���̃��\�b�h�ł́A�ȉ��̏����̍ۂ� drawer �����(��蒼��)�B
	// 1. Drawer �� NULL �̏ꍇ
	// 2. ���݂� Drawer �̃^�C�v���K�؂łȂ��Ȃ����Ƃ�

	if(TargetWindow)
	{
		// drawer �̎�ނ���肷��
		tDrawerType newtype = dtDrawDib;

		// �Y�[���͕K�v�H
		bool zoom_required = false;
		tjs_int srcw, srch;
		GetSrcSize(srcw, srch);
		if(DestRect.get_width() != srcw || DestRect.get_height() != srch)
			zoom_required = true;

		if(!zoom_required && !TVPForceDoublebuffer)
		{
			// �Y�[�����K�v�Ȃ��A���A�_�u���o�b�t�@���v������Ă��Ȃ��ꍇ
			newtype = dtDrawDib;
		}
		else
		{
			// �Y�[�����K�v���A�_�u���o�b�t�@���K�v
			if(!TVPZoomInterpolation || failedtype == dtDBDD || TVPDBGDIPrefered)
			{
				// 1) �Y�[���ŕ⊮���s��Ȃ��ꍇ(�⊮�Ȃ���GDI�ł����ł��Ȃ�)
				// 2) DirectDraw �Ń_�u���o�b�t�@�����O�Ɏ��s�����ꍇ
				// 3) GDI���I�v�V�����Ŏw�肳�ꂽ�ꍇ
				// �̂����ꂩ�̏ꍇ�� GDI ��p����
				newtype = dtDBGDI;
			}
			else
			{
				// �����łȂ���� DirectDraw
				newtype = dtDBDD;
			}
		}

		if(newtype != DrawerType)
		{
			// drawer ���Ȃ� (�܂�DrawerType �� dtNone�Ȃ̂�newtype�Ɉ�v���Ȃ�)
			// ���邢�� DrawerType �� newtype �ƈႤ�ꍇ
			if(Drawer) delete Drawer, Drawer = NULL;

			bool success;
			do
			{
				success = true;
				try
				{
					switch(newtype)
					{
					case dtNone:
						break;
					case dtDrawDib:
						Drawer = new tTVPDrawer_DrawDibNoBuffering(this);
						break;
					case dtDBGDI:
						Drawer = new tTVPDrawer_GDIDoubleBuffering(this);
						break;
					case dtDBDD:
						Drawer = new tTVPDrawer_DDDoubleBuffering(this);
						break;
					}
					Drawer->SetTargetWindow(TargetWindow);
					if(!Drawer->SetDestRectangle(DestRect))
						TVPThrowExceptionMessage(
							TJS_W("Failed to set destination rectangle to draw device drawer"));
					if(!Drawer->NotifyLayerResize(srcw, srch))
						TVPThrowExceptionMessage(
							TJS_W("Failed to set source layer size to draw device drawer"));
				}
				catch(...)
				{
					// ��O�����������B
					if(newtype == dtDBDD)
					{
						// DirectDraw �̏ꍇ�� GDI �� fall back �����݂�
						newtype = dtDBGDI;
						success = false;
					}
					else
					{
						// ����ȊO�� fall back �̂��悤���Ȃ�
						throw;
					}
				}
			} while(!success);

			DrawerType = newtype;
		} // if(newtype != DrawerType)
	} // if(TargetWindow)

}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
void TJS_INTF_METHOD tTVPPassThroughDrawDevice::AddLayerManager(iTVPLayerManager * manager)
{
	if(inherited::Managers.size() > 0)
	{
		// "Pass Through" �f�o�C�X�ł͂Q�ȏ��Layer Manager��o�^�ł��Ȃ�
		TVPThrowExceptionMessage(TJS_W("\"passthrough\" device does not support layer manager more than 1"));
			// TODO: i18n
	}
	inherited::AddLayerManager(manager);

	manager->SetDesiredLayerType(ltOpaque); // ltOpaque �ȏo�͂��󂯎�肽��
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
void TJS_INTF_METHOD tTVPPassThroughDrawDevice::SetTargetWindow(HWND wnd)
{
	DestroyDrawer();
	TargetWindow = wnd;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
void TJS_INTF_METHOD tTVPPassThroughDrawDevice::SetDestRectangle(const tTVPRect & rect)
{
	inherited::SetDestRectangle(rect);

	// �g���k��������ꍇ��drawer����蒼���Ȃ���΂Ȃ�Ȃ���������Ȃ�
	if(Drawer)
	{
		if(!Drawer->SetDestRectangle(rect))
		{
			tDrawerType failedtype = DrawerType; // ���s���� drawer
			DestroyDrawer(); // DestRect �̐ݒ�Ɏ��s�����ꍇ�� drawer ����蒼��
			EnsureDrawer(failedtype);
		}
	}
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
void TJS_INTF_METHOD tTVPPassThroughDrawDevice::NotifyLayerResize(iTVPLayerManager * manager)
{
	inherited::NotifyLayerResize(manager);

	// �g���k��������ꍇ��drawer����蒼���Ȃ���΂Ȃ�Ȃ���������Ȃ�
	if(Drawer)
	{
		tjs_int w, h;
		GetSrcSize(w, h);
		if(!Drawer->NotifyLayerResize(w, h))
		{
			tDrawerType failedtype = DrawerType; // ���s���� drawer
			DestroyDrawer(); // DestRect �̐ݒ�Ɏ��s�����ꍇ�� drawer ����蒼��
			EnsureDrawer(failedtype);
		}
	}
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
void TJS_INTF_METHOD tTVPPassThroughDrawDevice::StartBitmapCompletion(iTVPLayerManager * manager)
{
	EnsureDrawer(dtNone);

	if(Drawer) Drawer->StartBitmapCompletion();
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
void TJS_INTF_METHOD tTVPPassThroughDrawDevice::NotifyBitmapCompleted(iTVPLayerManager * manager,
	tjs_int x, tjs_int y, const void * bits, const BITMAPINFO * bitmapinfo,
	const tTVPRect &cliprect, tTVPLayerType type, tjs_int opacity)
{
	// bits, bitmapinfo �ŕ\�����r�b�g�}�b�v�� cliprect �̗̈���Ax, y �ɕ`��
	// ����B
	// opacity �� type �͖������邵���Ȃ��̂Ŗ�������
	if(Drawer)
	{
		if(TVPWaitVSync)
		{
			tjs_int w, h;
			GetSrcSize(w, h);
			if(cliprect.get_width() == w && cliprect.get_height() == h)
			{
				// �S��ʂ��X�V����Ƃ������Avsync ���܂B
				IDirectDraw2 *object = TVPGetDirectDrawObjectNoAddRef();
				if(object) object->WaitForVerticalBlank(DDWAITVB_BLOCKBEGIN, NULL);
			}
		}

		Drawer->NotifyBitmapCompleted(x, y, bits, bitmapinfo, cliprect);
	}
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
void TJS_INTF_METHOD tTVPPassThroughDrawDevice::EndBitmapCompletion(iTVPLayerManager * manager)
{
	if(Drawer) Drawer->EndBitmapCompletion();
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
void TJS_INTF_METHOD tTVPPassThroughDrawDevice::SetShowUpdateRect(bool b)
{
	if(Drawer) Drawer->SetShowUpdateRect(b);
}
//---------------------------------------------------------------------------








//---------------------------------------------------------------------------
// tTJSNC_PassThroughDrawDevice : PassThroughDrawDevice TJS native class
//---------------------------------------------------------------------------
tjs_uint32 tTJSNC_PassThroughDrawDevice::ClassID = (tjs_uint32)-1;
tTJSNC_PassThroughDrawDevice::tTJSNC_PassThroughDrawDevice() :
	tTJSNativeClass(TJS_W("PassThroughDrawDevice"))
{
	// register native methods/properties

	TJS_BEGIN_NATIVE_MEMBERS(PassThroughDrawDevice)
	TJS_DECL_EMPTY_FINALIZE_METHOD
//----------------------------------------------------------------------
// constructor/methods
//----------------------------------------------------------------------
TJS_BEGIN_NATIVE_CONSTRUCTOR_DECL(/*var.name*/_this, /*var.type*/tTJSNI_PassThroughDrawDevice,
	/*TJS class name*/PassThroughDrawDevice)
{
	return TJS_S_OK;
}
TJS_END_NATIVE_CONSTRUCTOR_DECL(/*TJS class name*/PassThroughDrawDevice)
//----------------------------------------------------------------------

//---------------------------------------------------------------------------
//----------------------------------------------------------------------
// properties
//----------------------------------------------------------------------
TJS_BEGIN_NATIVE_PROP_DECL(interface)
{
	TJS_BEGIN_NATIVE_PROP_GETTER
	{
		TJS_GET_NATIVE_INSTANCE(/*var. name*/_this, /*var. type*/tTJSNI_PassThroughDrawDevice);
		*result = reinterpret_cast<tjs_int64>(_this->GetDevice());
		return TJS_S_OK;
	}
	TJS_END_NATIVE_PROP_GETTER

	TJS_DENY_NATIVE_PROP_SETTER
}
TJS_END_NATIVE_PROP_DECL(interface)

//----------------------------------------------------------------------
	TJS_END_NATIVE_MEMBERS
}
//---------------------------------------------------------------------------
iTJSNativeInstance *tTJSNC_PassThroughDrawDevice::CreateNativeInstance()
{
	return new tTJSNI_PassThroughDrawDevice();
}
//---------------------------------------------------------------------------










//---------------------------------------------------------------------------
tTJSNI_PassThroughDrawDevice::tTJSNI_PassThroughDrawDevice()
{
	Device = new tTVPPassThroughDrawDevice();
}
//---------------------------------------------------------------------------
tTJSNI_PassThroughDrawDevice::~tTJSNI_PassThroughDrawDevice()
{
	if(Device) Device->Destruct(), Device = NULL;
}
//---------------------------------------------------------------------------
tjs_error TJS_INTF_METHOD
	tTJSNI_PassThroughDrawDevice::Construct(tjs_int numparams, tTJSVariant **param,
		iTJSDispatch2 *tjs_obj)
{
	return TJS_S_OK;
}
//---------------------------------------------------------------------------
void TJS_INTF_METHOD tTJSNI_PassThroughDrawDevice::Invalidate()
{
	if(Device) Device->Destruct(), Device = NULL;
}
//---------------------------------------------------------------------------

