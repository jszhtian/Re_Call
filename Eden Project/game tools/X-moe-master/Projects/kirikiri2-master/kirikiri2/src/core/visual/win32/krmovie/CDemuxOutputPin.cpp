/****************************************************************************/
/*! @file
@brief Demux output pin

�f�}���`�v���N�T�̏o�̓s������������
-----------------------------------------------------------------------------
	Copyright (C) 2005 T.Imoto <http://www.kaede-software.com>
-----------------------------------------------------------------------------
@author		T.Imoto
@date		2005/10/22
@note
*****************************************************************************/

#include "CDemuxOutputPin.h"
#include "CDemuxSource.h"
//----------------------------------------------------------------------------
//! @brief	  	CDemuxOutputPin constructor
//! @param		szName : �f�o�b�O�̂��߂Ɏg�p�����L�q�ւ̃|�C���^�B
//! @param		pFilter : ���̃s�����쐬�����t�B���^
//! @param		pHr : HRESULT �l�ւ̃|�C���^�B
//! @param		pszName : ���̃s���̖��O
//! @param		pSeek : �\�[�X�t�B���^��IMediaSeeking
//! @param		outstream : ���̃s���̏o�̓X�g���[��
//! @param		lock : ���b�N�I�u�W�F�N�g
//----------------------------------------------------------------------------
CDemuxOutputPin::CDemuxOutputPin( TCHAR *szName, CSource *pFilter, HRESULT *pHr, LPCWSTR pszName, IMediaSeeking *pSeek, IOutputStream *outstream, CCritSec *lock )
 : CSourceStream( szName, pHr, pFilter, pszName ), m_SeekProxy(pSeek), m_Stream(outstream), m_Lock(lock)
{}
//----------------------------------------------------------------------------
//! @brief	  	CDemuxOutputPin destructor
//----------------------------------------------------------------------------
CDemuxOutputPin::~CDemuxOutputPin()
{}
//----------------------------------------------------------------------------
//! @brief	  	�v�����ꂽ�C���^�[�t�F�C�X��Ԃ�
//! @param		riid : �C���^�[�t�F�C�X��IID
//! @param		ppv : �C���^�[�t�F�C�X��Ԃ��|�C���^�[�ւ̃|�C���^
//! @return		�G���[�R�[�h
//----------------------------------------------------------------------------
STDMETHODIMP CDemuxOutputPin::NonDelegatingQueryInterface( REFIID riid, void ** ppv )
{
	if( riid == IID_IMediaSeeking ) {
		return GetInterface( &m_SeekProxy, ppv );
	} else {
		return CUnknown::NonDelegatingQueryInterface(riid, ppv);
	}
}
//----------------------------------------------------------------------------
//! @brief	  	�o�͉\�ȃ��f�B�A�^�C�v��Ԃ�
//! @param		iPosition : ���f�B�A�^�C�v�̏���
//! @param		pmt : ���f�B�A�^�C�v���i�[����ϐ�
//! @return		�G���[�R�[�h
//----------------------------------------------------------------------------
HRESULT CDemuxOutputPin::GetMediaType( int iPosition, CMediaType *pmt )
{
	CAutoLock cAutoLock(m_Lock);
	return m_Stream->GetMediaType( iPosition, pmt );
}
//----------------------------------------------------------------------------
//! @brief	  	�w�肳�ꂽ���f�B�A�^�C�v�����p�ł��邩�ǂ����m�F����
//! @param		pmt : �o�͏o���邩�ǂ����m�F�����郁�f�B�A�^�C�v
//! @return		�G���[�R�[�h
//! @note		���݁ADirectX VA����ɂ͂����悤�ɂ��Ă���
//----------------------------------------------------------------------------
HRESULT CDemuxOutputPin::CheckMediaType( const CMediaType *pmt )
{
	CAutoLock cAutoLock(m_Lock);
	if( m_Stream->IsDXVASubtype( pmt ) )
		return VFW_E_TYPE_NOT_ACCEPTED;

	CMediaType	mt;
	int i = 0;
	while( GetMediaType( i, &mt ) == S_OK )
	{
		if( mt.majortype == pmt->majortype && mt.subtype == pmt->subtype && mt.formattype == pmt->formattype )
			return S_OK;
		++i;
	}
	return VFW_E_TYPE_NOT_ACCEPTED;
}
//----------------------------------------------------------------------------
//! @brief	  	�w�肳�ꂽ���f�B�A�^�C�v�����p�ł��邩�ǂ����m�F����
//! @param		pmt : �o�͏o���邩�ǂ����m�F�����郁�f�B�A�^�C�v
//! @return		�G���[�R�[�h
//! @note		���݁ADirectX VA����ɂ͂����悤�ɂ��Ă���
//----------------------------------------------------------------------------
HRESULT CDemuxOutputPin::SetMediaType( const CMediaType *pmt )
{
	CAutoLock cAutoLock(m_Lock);
	HRESULT hr = CSourceStream::SetMediaType(pmt);
	if( hr == S_OK ) {
		hr = m_Stream->SetMediaType(pmt);
	}
	return hr;
}
//----------------------------------------------------------------------------
//! @brief	  	�o�b�t�@�̃T�C�Y�����肷��
//! @param		pIMemAllocator : �A���P�[�^�[�ւ̃|�C���^
//! @param		pProp : �v���T�C�Y
//! @return		�G���[�R�[�h
//----------------------------------------------------------------------------
HRESULT CDemuxOutputPin::DecideBufferSize( IMemAllocator *pIMemAllocator, ALLOCATOR_PROPERTIES *pProp )
{
	CAutoLock cAutoLock(m_Lock);
	HRESULT	hr = S_OK;
	ALLOCATOR_PROPERTIES	reqAlloc;
	ALLOCATOR_PROPERTIES	actualAlloc;

	long	buffers = 0;
	long	bufsize = 0;
	m_Stream->GetNeedBufferSize( buffers, bufsize );

	//Allignment of data
	if( pProp->cbAlign <= 0 )
		reqAlloc.cbAlign = 1;
	else
		reqAlloc.cbAlign = pProp->cbAlign;

	//Size of each buffer
	if( pProp->cbBuffer < bufsize )
		reqAlloc.cbBuffer = bufsize;
	else
		reqAlloc.cbBuffer = pProp->cbBuffer;

	//How many prefeixed bytes
	if( pProp->cbPrefix < 0 )
		reqAlloc.cbPrefix = 0;
	else
		reqAlloc.cbPrefix = pProp->cbPrefix;

	//Number of buffers in the allcoator
	if( pProp->cBuffers < buffers )
		reqAlloc.cBuffers = buffers;
	else
		reqAlloc.cBuffers = pProp->cBuffers;

	//Set the properties in the allocator
	hr = pIMemAllocator->SetProperties(&reqAlloc, &actualAlloc);

	//Check the response
	if( hr != S_OK ) {
		switch( hr ) {
			case E_POINTER:
				DbgLog(( LOG_ERROR, 0, "DecideBufferSize : SetProperties - NULL POINTER" ));
				return hr;
			case VFW_E_ALREADY_COMMITTED:
				DbgLog(( LOG_ERROR, 0, "DecideBufferSize : SetProperties - Already COMMITED" ));
				return hr;
			case VFW_E_BADALIGN:
				DbgLog(( LOG_ERROR, 0, "DecideBufferSize : SetProperties - Bad ALIGN" ));
				return hr;
			case VFW_E_BUFFERS_OUTSTANDING:
				DbgLog(( LOG_ERROR, 0, "DecideBufferSize : SetProperties - BUFFS OUTSTANDING" ));
				return hr;
			default:
				DbgLog(( LOG_ERROR, 0, "DecideBufferSize : SetProperties - UNKNOWN ERROR" ));
				break;
		}
	}
	// ���������ꍇ���A���ۂ̌��ʂ��m�F����B
	if( pProp->cbBuffer > actualAlloc.cbBuffer )
	{
		return E_FAIL;
	}
	hr = m_Stream->SetDecidedBufferSize( actualAlloc.cBuffers, actualAlloc.cbBuffer, actualAlloc.cbAlign, actualAlloc.cbPrefix );
	if( FAILED(hr) )
		return hr;

//	hr = pIMemAllocator->Commit();
// �o�̓s�����A�N�e�B�u�ɂȂ�܂�Commit�͒x�点��

#ifdef _DEBUG
	switch( hr ) {
		case E_FAIL:
			DbgLog(( LOG_ERROR, 0, "DecideBufferSize : Commit - FAILED " ));
			return hr;
		case E_POINTER:
			DbgLog(( LOG_ERROR, 0, "DecideBufferSize : Commit - NULL POINTER " ));
			return hr;
		case E_INVALIDARG:
			DbgLog(( LOG_ERROR, 0, "DecideBufferSize : Commit - INVALID ARG " ));
			return hr;
		case E_NOTIMPL:
			DbgLog(( LOG_ERROR, 0, "DecideBufferSize : Commit - NOT IMPL" ));
			return hr;
		case S_OK:
			DbgLog(( LOG_ERROR, 0, "DecideBufferSize : Commit - ** SUCCESS **" ));
			break;
		default:
			DbgLog(( LOG_ERROR, 0, "DecideBufferSize : Commit - UNKNOWN ERROR " ));
			return hr;
	}
#endif

	return hr;
}
//----------------------------------------------------------------------------
//! @brief	  	�T���v�����_�E���X�g���[���֑��葱���鏈��
//! @return		�G���[�R�[�h
//----------------------------------------------------------------------------
HRESULT CDemuxOutputPin::DoBufferProcessingLoop(void)
{
	Command com;
	HRESULT	hr;
	OnThreadStartPlay();
	do {
		while( !CheckRequest(&com) ) {
			// Virtual function user will override.
			IMediaSample *pSample;
			hr = RetrieveBuffer(&pSample);
			if( hr == S_OK ) {
				hr = Deliver(pSample);
				pSample->Release();
				if(hr != S_OK)
				{
					DbgLog((LOG_TRACE, 2, TEXT("Deliver() returned %08x; stopping"), hr));
					return S_OK;
				}
			} else if (hr == S_FALSE) {
				// derived class wants us to stop pushing data
				DeliverEndOfStream();
				return S_OK;
			} else {
				// derived class encountered an error
				DbgLog((LOG_ERROR, 1, TEXT("Error %08lX from FillBuffer!!!"), hr));
				DeliverEndOfStream();
				m_pFilter->NotifyEvent(EC_ERRORABORT, hr, 0);
				return hr;
			}
		}
		if( com == CMD_RUN || com == CMD_PAUSE ) {
			Reply(NOERROR);
		} else if( com != CMD_STOP ) {
			Reply((DWORD) E_UNEXPECTED);
			DbgLog((LOG_ERROR, 1, TEXT("Unexpected command!!!")));
		}
	} while( com != CMD_STOP );

	return S_FALSE;
}
//----------------------------------------------------------------------------
//! @brief	  	�T���v�����X�g���[������擾����
//! @param		pSample : �T���v���ւ̃|�C���^�̃|�C���^
//! @return		�G���[�R�[�h
//----------------------------------------------------------------------------
HRESULT CDemuxOutputPin::RetrieveBuffer( IMediaSample **pSample )
{
	CAutoLock cAutoLock(m_Lock);
	HRESULT hr = m_Stream->GetNextSample( pSample );
	return hr;
}
//----------------------------------------------------------------------------
//! @brief	  	�ڑ�����
//! @param		pReceivePin : �ڑ���v������Ă���s��
//! @param		pmt : �ڑ��Ŏg�p���邱�Ƃ�v������Ă��郁�f�B�A�^�C�v
//! @return		�G���[�R�[�h
//----------------------------------------------------------------------------
STDMETHODIMP CDemuxOutputPin::Connect( IPin *pReceivePin, const AM_MEDIA_TYPE *pmt )
{
	return CSourceStream::Connect( pReceivePin, pmt );
}
//----------------------------------------------------------------------------
//! @brief	  	�ڑ���������
//! @param		pReceivePin : �ڑ��Ɏg��ꂽ�s��
//! @return		�G���[�R�[�h
//----------------------------------------------------------------------------
HRESULT CDemuxOutputPin::CompleteConnect( IPin *pReceivePin )
{
	HRESULT	hr = CSourceStream::CompleteConnect( pReceivePin );
	return hr;
}
//----------------------------------------------------------------------------
//! @brief	  	�A���P�[�^�[�����肷�鏈��
//! @param		pPin : ���̓s��
//! @param		pAlloc : �A���P�[�^�[
//! @return		�G���[�R�[�h
//----------------------------------------------------------------------------
HRESULT CDemuxOutputPin::DecideAllocator( IMemInputPin *pPin, IMemAllocator **pAlloc )
{
	HRESULT	hr = CSourceStream::DecideAllocator( pPin, pAlloc );
	if( SUCCEEDED(hr) )
	{
		hr = m_Stream->SetAllocator( *pAlloc );
	}
	return hr;
}
