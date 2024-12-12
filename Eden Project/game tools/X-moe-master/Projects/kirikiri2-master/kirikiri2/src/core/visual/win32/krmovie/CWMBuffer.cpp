/****************************************************************************/
/*! @file
@brief DirectShow media sample wrapper for Windows media format

DirectShow��Media Sample�����b�v����Windows Media Format SDK�p��Buffer
-----------------------------------------------------------------------------
	Copyright (C) 2005 T.Imoto <http://www.kaede-software.com>
-----------------------------------------------------------------------------
@author		T.Imoto
@date		2005/10/28
@note
*****************************************************************************/

#include "CWMBuffer.h"

//----------------------------------------------------------------------------
//! @brief	  	�v�����ꂽ�C���^�[�t�F�C�X��Ԃ�
//! @param		riid : �C���^�[�t�F�C�X��IID
//! @param		ppv : �C���^�[�t�F�C�X��Ԃ��|�C���^�[�ւ̃|�C���^
//! @return		�G���[�R�[�h
//----------------------------------------------------------------------------
STDMETHODIMP CWMBuffer::NonDelegatingQueryInterface( REFIID riid, void ** ppv )
{
	if( IID_INSSBuffer == riid ) {
		return GetInterface(static_cast<INSSBuffer*>(this), ppv);
	} else if( riid == IID_IMediaSample ) {
		return GetInterface(static_cast<IMediaSample*>(m_Sample), ppv);
	} else {
		return CUnknown::NonDelegatingQueryInterface(riid, ppv);
	}
}
//----------------------------------------------------------------------------
//! @brief	  	�o�b�t�@���擾����
//! @param		ppdwBuffer : �|�C���^�̃|�C���^
//! @return		�G���[�R�[�h
//----------------------------------------------------------------------------
STDMETHODIMP CWMBuffer::GetBuffer( BYTE **ppdwBuffer )
{
	return Sample()->GetPointer( ppdwBuffer );
}
//----------------------------------------------------------------------------
//! @brief	  	�o�b�t�@�ƃT�C�Y���擾����
//! @param		ppdwBuffer : �|�C���^�̃|�C���^
//! @param		pdwLength : �T�C�Y
//! @return		�G���[�R�[�h
//----------------------------------------------------------------------------
STDMETHODIMP CWMBuffer::GetBufferAndLength( BYTE **ppdwBuffer, DWORD *pdwLength )
{
	*pdwLength = static_cast<DWORD>( Sample()->GetActualDataLength() );
	return Sample()->GetPointer( ppdwBuffer );
}
//----------------------------------------------------------------------------
//! @brief	  	�f�[�^���i�[����Ă�����T�C�Y���擾����
//! @param		pdwLength : �T�C�Y
//! @return		�G���[�R�[�h
//----------------------------------------------------------------------------
STDMETHODIMP CWMBuffer::GetLength( DWORD *pdwLength )
{
	*pdwLength = static_cast<DWORD>( Sample()->GetActualDataLength() );
	return S_OK;
}
//----------------------------------------------------------------------------
//! @brief	  	�o�b�t�@�̃T�C�Y���擾����
//! @param		pdwLength : �T�C�Y
//! @return		�G���[�R�[�h
//----------------------------------------------------------------------------
STDMETHODIMP CWMBuffer::GetMaxLength( DWORD *pdwLength )
{
	*pdwLength = static_cast<DWORD>( Sample()->GetSize() );
	return S_OK;
}
//----------------------------------------------------------------------------
//! @brief	  	�T�C�Y��ݒ肷��
//! @param		dwLength : �T�C�Y
//! @return		�G���[�R�[�h
//----------------------------------------------------------------------------
STDMETHODIMP CWMBuffer::SetLength( DWORD dwLength )
{
	return Sample()->SetActualDataLength( static_cast<long>( dwLength ) );
}

