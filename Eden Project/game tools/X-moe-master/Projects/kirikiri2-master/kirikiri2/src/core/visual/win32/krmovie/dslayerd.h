/****************************************************************************/
/*! @file
@brief DirectShow�𗘗p�������[�r�[�̃��C���[�`��Đ�

-----------------------------------------------------------------------------
	Copyright (C) 2004 T.Imoto <http://www.kaede-software.com>
-----------------------------------------------------------------------------
@author		T.Imoto
@date		2004/08/25
@note
			2004/08/25	T.Imoto		
*****************************************************************************/

#ifndef __DSLAYERD_H__
#define __DSLAYERD_H__

#include "dsmovie.h"
#include "IRendererBufferAccess.h"
#include "IRendererBufferVideo.h"

class tTVPBaseBitmap;
//----------------------------------------------------------------------------
//! @brief ���C���[�`��r�f�I�N���X
//----------------------------------------------------------------------------
class tTVPDSLayerVideo : public tTVPDSMovie
{
private:
	CComPtr<IRendererBufferAccess>	m_BuffAccess;
	CComPtr<IRendererBufferVideo>	m_BuffVideo;

	BYTE			*m_BmpBits[2];
	//----------------------------------------------------------------------------
	//! @brief	  	IRendererBufferAccess���擾����
	//! @return		IRendererBufferAccess�C���^�[�t�F�C�X
	//----------------------------------------------------------------------------
	IRendererBufferAccess *BufferAccess()
	{
		assert( m_BuffAccess.p );
		return m_BuffAccess;
	}
	//----------------------------------------------------------------------------
	//! @brief	  	IRendererBufferVideo���擾����
	//! @return		IRendererBufferVideo�C���^�[�t�F�C�X
	//----------------------------------------------------------------------------
	IRendererBufferVideo *BufferVideo()
	{
		assert( m_BuffVideo.p );
		return m_BuffVideo;
	}

public:
	tTVPDSLayerVideo();
	virtual ~tTVPDSLayerVideo();

	virtual void __stdcall BuildGraph( HWND callbackwin, IStream *stream,
		const wchar_t * streamname, const wchar_t *type, unsigned __int64 size );

	virtual void __stdcall ReleaseAll();

	virtual void __stdcall GetFrontBuffer( BYTE **buff );
	virtual void __stdcall SetVideoBuffer( BYTE *buff1, BYTE *buff2, long size );

	virtual void __stdcall GetVideoSize( long *width, long *height );
	virtual HRESULT __stdcall GetAvgTimePerFrame( REFTIME *pAvgTimePerFrame );
};

#endif	// __DSLAYERD_H__
