/****************************************************************************/
/*! @file
@brief IMediaSeeking Proxy

IMediaSeeking�̌Ăяo����㗝����B
�s�����\�[�X�t�B���^��IMediaSeeking���g�p�������ꍇ�ȂǂɎg��
-----------------------------------------------------------------------------
	Copyright (C) 2005 T.Imoto <http://www.kaede-software.com>
-----------------------------------------------------------------------------
@author		T.Imoto
@date		2005/10/22
@note
*****************************************************************************/


#ifndef __MEDIA_SEEKING_PROXY_H__
#define __MEDIA_SEEKING_PROXY_H__

#include <streams.h>
#include <assert.h>
#include <atlcomcli.h>

//! IMediaSeeking�ւ̑����P�ɃA�b�v�X�g���[���֓n���ď�������ꍇ�́ACPosPassThru���g�p���邪�A
//! �\�[�X�t�B���^�̃s����IMediaSeeking����������K�v������ꍇ�A�\�[�X�t�B���^��IMediaSeeking
//! �C���^�[�t�F�C�X���Ăяo�����ɂȂ邱�Ƃ������Ǝv����B
//! CMediaSeekingProxy�͂��̂悤�ȗp�r�Ɏg�����߂̃N���X�B
class CMediaSeekingProxy : public IMediaSeeking
{
private:
//	CComPtr<IMediaSeeking>	m_Delegate;
	IMediaSeeking*			m_Delegate;	// CComPtr �Ŏ��Ǝ��g�������ƂɂȂ��ĊJ���ł��Ȃ��Ȃ邱�Ƃ�����
	//----------------------------------------------------------------------------
	//! @brief	  	IMediaSeeking���擾����
	//! @return		IMediaSeeking�C���^�[�t�F�C�X
	//----------------------------------------------------------------------------
	IMediaSeeking *MediaSeeking()
	{
		assert( m_Delegate );
		return m_Delegate;
	}

	// delegate��ݒ肵�Ȃ�������R�s�[���֎~����
	CMediaSeekingProxy();
	CMediaSeekingProxy(const CMediaSeekingProxy& copy);
	void operator =(const CMediaSeekingProxy& lhs);

public:
	CMediaSeekingProxy(IMediaSeeking* delegate);
	virtual ~CMediaSeekingProxy(void);
	void SetSeeker( IMediaSeeking *seeker );

	//Methods of IUnknown
	HRESULT STDMETHODCALLTYPE QueryInterface( REFIID riid, LPVOID *ppvObj );
	ULONG STDMETHODCALLTYPE AddRef();
	ULONG STDMETHODCALLTYPE Release();

	//IMediaSeeking Interface
	virtual STDMETHODIMP GetCapabilities(DWORD *pCapabilities);
	virtual STDMETHODIMP CheckCapabilities(DWORD *pCapabilities);
	virtual STDMETHODIMP IsFormatSupported(const GUID *pFormat);
	virtual STDMETHODIMP QueryPreferredFormat(GUID *pFormat);
	virtual STDMETHODIMP SetTimeFormat(const GUID *pFormat);
	virtual STDMETHODIMP GetTimeFormat( GUID *pFormat);
	virtual STDMETHODIMP GetDuration(LONGLONG *pDuration);
	virtual STDMETHODIMP GetStopPosition(LONGLONG *pStop);
	virtual STDMETHODIMP GetCurrentPosition(LONGLONG *pCurrent);
	virtual STDMETHODIMP ConvertTimeFormat(LONGLONG *pTarget, const GUID *pTargetFormat, LONGLONG Source, const GUID *pSourceFormat);
	virtual STDMETHODIMP SetPositions(LONGLONG *pCurrent,DWORD dwCurrentFlags,LONGLONG *pStop,DWORD dwStopFlags);
	virtual STDMETHODIMP GetPositions(LONGLONG *pCurrent, LONGLONG *pStop);
	virtual STDMETHODIMP GetAvailable(LONGLONG *pEarliest, LONGLONG *pLatest);
	virtual STDMETHODIMP SetRate(double dRate);
	virtual STDMETHODIMP GetRate(double *dRate);
	virtual STDMETHODIMP GetPreroll(LONGLONG *pllPreroll);
	virtual STDMETHODIMP IsUsingTimeFormat(const GUID *pFormat);
};

#endif	// __MEDIA_SEEKING_PROXY_H__
