#ifndef _D3DFont_
#define _D3DFont_

#include "dxsdk\Include\d3dx9core.h"
#include "DebugInfo.h"

class XmoeD3DFont : public ID3DXFont
{
	ID3DXFont* FontInfo;

public:
	VOID STDMETHODCALLTYPE Init(ID3DXFont* RealFont)
	{
		FontInfo = RealFont;
	}

	// IUnknown
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, LPVOID *ppv)
	{
		return FontInfo->QueryInterface(iid, ppv);
	}

	virtual ULONG   STDMETHODCALLTYPE AddRef()
	{
		return FontInfo->AddRef();
	}

	virtual ULONG   STDMETHODCALLTYPE Release()
	{
		return FontInfo->Release();
	}

	// ID3DXFont
	virtual HRESULT STDMETHODCALLTYPE GetDevice(LPDIRECT3DDEVICE9 *ppDevice)
	{
		return FontInfo->GetDevice(ppDevice);
	}

	virtual HRESULT STDMETHODCALLTYPE GetDescA(D3DXFONT_DESCA *pDesc)
	{
		HRESULT Result = FontInfo->GetDescA(pDesc);
		pDesc->CharSet = GB2312_CHARSET;
		return Result;
	}

	HRESULT STDMETHODCALLTYPE GetDescW(D3DXFONT_DESCW *pDesc)
	{
		return FontInfo->GetDescW(pDesc);
	}

	virtual BOOL    STDMETHODCALLTYPE GetTextMetricsA(TEXTMETRICA *pTextMetrics)
	{
		HRESULT Result = FontInfo->GetTextMetricsA(pTextMetrics);
		pTextMetrics->tmCharSet = GB2312_CHARSET;
		return Result;
	}

	virtual BOOL    STDMETHODCALLTYPE GetTextMetricsW(TEXTMETRICW *pTextMetrics)
	{
		HRESULT Result = FontInfo->GetTextMetricsW(pTextMetrics);
		pTextMetrics->tmCharSet = GB2312_CHARSET;
		return Result;
	}

	virtual HDC     STDMETHODCALLTYPE GetDC()
	{
		return FontInfo->GetDC();
	}

	virtual HRESULT STDMETHODCALLTYPE GetGlyphData(UINT Glyph, LPDIRECT3DTEXTURE9 *ppTexture, RECT *pBlackBox, POINT *pCellInc)
	{
		return FontInfo->GetGlyphData(Glyph, ppTexture, pBlackBox, pCellInc);
	}

	virtual HRESULT STDMETHODCALLTYPE PreloadCharacters(UINT First, UINT Last)
	{
		return FontInfo->PreloadCharacters(First, Last);
	}

	virtual HRESULT STDMETHODCALLTYPE PreloadGlyphs(UINT First, UINT Last)
	{
		return FontInfo->PreloadGlyphs(First, Last);
	}

	virtual HRESULT STDMETHODCALLTYPE PreloadTextA(LPCSTR pString, INT Count)
	{
#if 1
		ULONG MsLength = lstrlenA(pString) * 4;
		WCHAR* wpString = (WCHAR*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, MsLength);
		MultiByteToWideChar(936, 0, pString, lstrlenA(pString), wpString, MsLength);
		HRESULT Result = PreloadTextW(wpString, lstrlenW(wpString));
		HeapFree(GetProcessHeap(), 0, wpString);
		return Result;
#else
		return FontInfo->PreloadTextA(pString, Count);
#endif
	}

	virtual HRESULT STDMETHODCALLTYPE PreloadTextW(LPCWSTR pString, INT Count)
	{
		WCHAR upString[2048] = {0};
		lstrcpyW(upString, pString);
		
		for (ULONG i = 0; i < lstrlenW(upString); i++)
		{
			if (upString[i] == 0x2468)
			{
				upString[i] = 0x266A;
			}
		}
		
		return FontInfo->PreloadTextW(upString, Count);
	}

	virtual INT STDMETHODCALLTYPE DrawTextA(LPD3DXSPRITE pSprite, LPCSTR pString, INT Count, LPRECT pRect, DWORD Format, D3DCOLOR Color)
	{
		ULONG MsLength = lstrlenA(pString) * 4;
		WCHAR* wpString = (WCHAR*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, MsLength);
		MultiByteToWideChar(936, 0, pString, lstrlenA(pString), wpString, MsLength);
		HRESULT Result = DrawTextW(pSprite, wpString, lstrlenW(wpString), pRect, Format, Color);

		HeapFree(GetProcessHeap(), 0, wpString);
		return Result;
	}
	
	virtual INT STDMETHODCALLTYPE DrawTextW(LPD3DXSPRITE pSprite, LPCWSTR pString, INT Count, LPRECT pRect, DWORD Format, D3DCOLOR Color)
	{
		WCHAR upString[2048] = { 0 };
		lstrcpyW(upString, pString);
		
		ULONG TextCount = lstrlenW(upString);

#if 0
		for (ULONG i = 0; i < TextCount; i++)
		{
			if (upString[i] == 0x2468)
			{
				upString[i] = 0x0020;
				lstrcpyW(&(upString[i + 1]), &(upString[i + 2]));
				upString[i + 1] = 0x266A;
				TextCount++;
			}
		}
#else
		for (ULONG i = 0; i < TextCount; i++)
		{
			if (upString[i] == 0x2468)
			{
				upString[i] = 0x3000;//0x266A;
			}
		}
#endif

		//OutputStringW(upString);
		return FontInfo->DrawTextW(pSprite, upString, lstrlenW(upString), pRect, Format, Color);
	}

	virtual HRESULT STDMETHODCALLTYPE OnLostDevice()
	{
		return FontInfo->OnLostDevice();
	}

	virtual HRESULT STDMETHODCALLTYPE OnResetDevice()
	{
		return FontInfo->OnResetDevice();
	}
};

#endif