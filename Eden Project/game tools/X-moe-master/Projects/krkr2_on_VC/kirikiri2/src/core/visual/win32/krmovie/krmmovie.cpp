/****************************************************************************/
/*! @file
@brief part of KRLMOVIE.DLL

���s�ɂ�DirectX9�ȍ~���K�v
-----------------------------------------------------------------------------
	Copyright (C) 2005 T.Imoto <http://www.kaede-software.com>
-----------------------------------------------------------------------------
@author		T.Imoto
@date		2005/09/25
@note
			2005/09/25	T.Imoto		�쐬
*****************************************************************************/


#include "dsmixer.h"
#include "..\krmovie.h"

#include "asyncio.h"
#include "asyncrdr.h"

#include "tp_stub.h"

#include "OptionInfo.h"


//----------------------------------------------------------------------------
//! @brief	  	VideoOverlay Object (�~�L�V���O�Đ��p) ���擾����
//! @param		callbackwin : �R�[���o�b�N�E�B���h�E�̃n���h��
//! @param		stream : ���̓X�g���[��
//! @param		streamname : �X�g���[���̖��O
//! @param		type : �X�g���[���̊g���q
//! @param		size : �X�g���[���̃T�C�Y
//! @param		out : VideoOverlay Object
//! @return		�G���[������
//----------------------------------------------------------------------------
void __stdcall GetMixingVideoOverlayObject(
	HWND callbackwin, IStream *stream, const wchar_t * streamname,
	const wchar_t *type, unsigned __int64 size, iTVPVideoOverlay **out)
{
	*out = new tTVPDSMixerVideoOverlay;

	if( *out )
		static_cast<tTVPDSMixerVideoOverlay*>(*out)->BuildGraph( callbackwin, stream, streamname, type, size );
}
