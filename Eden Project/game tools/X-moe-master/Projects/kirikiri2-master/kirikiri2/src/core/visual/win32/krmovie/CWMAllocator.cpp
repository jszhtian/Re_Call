/****************************************************************************/
/*! @file
@brief DirectShow allocator wrapper for Windows media format

DirectShow�̃A���P�[�^�[�����b�v����Windows Media Format SDK�p�̃A���P�[�^�[
-----------------------------------------------------------------------------
	Copyright (C) 2005 T.Imoto <http://www.kaede-software.com>
-----------------------------------------------------------------------------
@author		T.Imoto
@date		2005/10/28
@note
*****************************************************************************/

#include "CWMAllocator.h"
#include "CWMBuffer.h"
#include <assert.h>


//----------------------------------------------------------------------------
//! @brief	  	�f�X�g���N�^
//----------------------------------------------------------------------------
CWMAllocator::~CWMAllocator()
{
}

//----------------------------------------------------------------------------
//! @brief	  	�v�����ꂽ�C���^�[�t�F�C�X��Ԃ�
//! @param		riid : �C���^�[�t�F�C�X��IID
//! @param		ppv : �C���^�[�t�F�C�X��Ԃ��|�C���^�[�ւ̃|�C���^
//! @return		�G���[�R�[�h
//----------------------------------------------------------------------------
STDMETHODIMP CWMAllocator::NonDelegatingQueryInterface( REFIID riid, void ** ppv )
{
	if( IID_IWMReaderAllocatorEx == riid ) {
		return GetInterface( static_cast<IWMReaderAllocatorEx*>(this), ppv );
	} else if( IID_IMemAllocator == riid ) {
		return GetInterface( static_cast<IMemAllocator*>(m_MemAlloc), ppv );
	} else {
		return CUnknown::NonDelegatingQueryInterface(riid, ppv);
	}
}
//----------------------------------------------------------------------------
//! @brief	  	�o�b�t�@���m�ۂ��� ( �X�g���[������v�����ꂽ�ꍇ�ɂ��̃��\�b�h���Ă΂�� )
//! @param		wStreamNum : �X�g���[���ԍ�
//! @param		cbBuffer : �v�����ꂽ�o�b�t�@�T�C�Y
//! @param		ppBuffer : �o�b�t�@��Ԃ��|�C���^�̃|�C���^
//! @param		dwFlags : �p�r�t���O
//! @param		cnsSampleTime : �T���v���^�C��
//! @param		cnsSampleDuration : �T���v���\������
//! @param		pvContext : IWMReader::Start�R�[�����ɓn���ꂽ�|�C���^
//! @return		�G���[�R�[�h
//----------------------------------------------------------------------------
STDMETHODIMP CWMAllocator::AllocateForStreamEx( WORD wStreamNum, DWORD cbBuffer, INSSBuffer **ppBuffer,
	DWORD dwFlags, QWORD cnsSampleTime, QWORD cnsSampleDuration, void *pvContext )
{
	HRESULT	hr;

	if( ppBuffer == NULL )
		return E_INVALIDARG;

	DWORD	flag = 0;
	if( dwFlags & WM_SFEX_NOTASYNCPOINT )	// �� Key frame
		flag |= AM_GBF_NOTASYNCPOINT;

	REFERENCE_TIME	StartTime = cnsSampleTime;
	REFERENCE_TIME	EndTime = cnsSampleTime + cnsSampleDuration;

	IMediaSample	*pSample = NULL;
	hr = Allocator()->GetBuffer( &pSample, &StartTime, &EndTime, flag );
	if( hr == S_OK )
	{
		*ppBuffer = new CWMBuffer(pSample);
		(*ppBuffer)->AddRef();
		(*ppBuffer)->SetLength(cbBuffer);
		pSample->Release();	// CWMBuffer�֓n�������_�ŎQ�ƃJ�E���g��������
	}
	return hr;
}
//----------------------------------------------------------------------------
//! @brief	  	�o�b�t�@���m�ۂ��� ( �A�E�g�v�b�g����v�����ꂽ�ꍇ�ɂ��̃��\�b�h���Ă΂�� )
//! @param		wStreamNum : �X�g���[���ԍ�
//! @param		cbBuffer : �v�����ꂽ�o�b�t�@�T�C�Y
//! @param		ppBuffer : �o�b�t�@��Ԃ��|�C���^�̃|�C���^
//! @param		dwFlags : �p�r�t���O
//! @param		cnsSampleTime : �T���v���^�C��
//! @param		cnsSampleDuration : �T���v���\������
//! @param		pvContext : IWMReader::Start�R�[�����ɓn���ꂽ�|�C���^
//! @return		�G���[�R�[�h
//! @note		���݂��̃��\�b�h�͎g���Ă��Ȃ��B���Ԃ񐳏�ɋ@�\���Ȃ��B
//----------------------------------------------------------------------------
STDMETHODIMP CWMAllocator::AllocateForOutputEx( DWORD dwOutputNum, DWORD cbBuffer, INSSBuffer **ppBuffer,
	DWORD dwFlags, QWORD cnsSampleTime, QWORD cnsSampleDuration, void *pvContext )
{
	HRESULT	hr;

	if( ppBuffer == NULL )
		return E_INVALIDARG;

	DWORD	flag = 0;
	if( dwFlags & WM_SFEX_NOTASYNCPOINT )	// �� Key frame
		flag |= AM_GBF_NOTASYNCPOINT;

	REFERENCE_TIME	StartTime = cnsSampleTime;
	REFERENCE_TIME	EndTime = cnsSampleTime + cnsSampleDuration;

	IMediaSample	*pSample;
	hr = Allocator()->GetBuffer( &pSample, &StartTime, &EndTime, flag );
	if( hr == S_OK )
	{
		*ppBuffer = new CWMBuffer(pSample);
		(*ppBuffer)->AddRef();
		pSample->Release();	// CWMBuffer�֓n�������_�ŎQ�ƃJ�E���g��������
	}
	return hr;
}
