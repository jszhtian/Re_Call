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


#include "dslayerd.h"
#include "CIStream.h"

#include "DShowException.h"
#include "BufferRenderer.h"
#include "OptionInfo.h"

//----------------------------------------------------------------------------
//! @brief	  	m_BmpBits��NULL��ݒ肷��
//----------------------------------------------------------------------------
tTVPDSLayerVideo::tTVPDSLayerVideo()
{
	m_BmpBits[0] = NULL;
	m_BmpBits[1] = NULL;
}
//----------------------------------------------------------------------------
//! @brief	  	m_BmpBits��NULL��ݒ肷��
//----------------------------------------------------------------------------
tTVPDSLayerVideo::~tTVPDSLayerVideo()
{
	Shutdown = true;

	m_BmpBits[0] = NULL;
	m_BmpBits[1] = NULL;
	ReleaseAll();
}
//----------------------------------------------------------------------------
//! @brief	  	�t�B���^�O���t�̍\�z
//! @param 		callbackwin : ���b�Z�[�W�𑗐M����E�B���h�E
//! @param 		stream : �ǂݍ��݌��X�g���[��
//! @param 		streamname : �X�g���[���̖��O
//! @param 		type : ���f�B�A�^�C�v(�g���q)
//! @param 		size : ���f�B�A�T�C�Y
//----------------------------------------------------------------------------
void __stdcall tTVPDSLayerVideo::BuildGraph( HWND callbackwin, IStream *stream,
	const wchar_t * streamname, const wchar_t *type, unsigned __int64 size )
{
	HRESULT			hr;

//	CoInitialize(NULL);

	// detect CMediaType from stream's extension ('type')
	try {
		// create IFilterGraph instance
		if( FAILED(hr = m_GraphBuilder.CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC)) )
			ThrowDShowException(L"Failed to create FilterGraph.", hr);

// ���O�������o�����ɗL���ɂ���B�ł��A����܂���ɗ����Ȃ��悤�ȁB�B�B
#if	0
		{
			HANDLE	hFile = CreateFile( "C:\\krdslog.txt", GENERIC_WRITE|GENERIC_READ, FILE_SHARE_WRITE|FILE_SHARE_READ, NULL, CREATE_ALWAYS,
				FILE_ATTRIBUTE_NORMAL | FILE_FLAG_WRITE_THROUGH, NULL );
			if( hFile != INVALID_HANDLE_VALUE )
			{
				GraphBuilder()->SetLogFile( (DWORD_PTR)hFile );
			}
		}
#endif

		// Register to ROT
		if(GetShouldRegisterToROT())
		{
			AddToROT(m_dwROTReg);
		}

		// Create the Buffer Renderer object
		CComPtr<IBaseFilter>	pBRender;	// for buffer renderer filter
		TBufferRenderer			*pCBR;
		pCBR = new TBufferRenderer( NAME("Buffer Renderer"), NULL, &hr );
		if( FAILED(hr) )
			ThrowDShowException(L"Failed to create buffer renderer object.", hr);
		pBRender = pCBR;

		if( IsWindowsMediaFile(type) )
		{
			if( FAILED(hr = GraphBuilder()->AddFilter( pBRender, L"Buffer Renderer")) )
				ThrowDShowException(L"Failed to call GraphBuilder()->AddFilter( pBRender, L\"Buffer Renderer\").", hr);

			BuildWMVGraph( pBRender, stream );
		}
		else
		{
			CMediaType mt;
			mt.majortype = MEDIATYPE_Stream;
			ParseVideoType( mt, type ); // may throw an exception
	
			// create proxy filter
			m_Proxy = new CIStreamProxy( stream, size );
			hr = S_OK;
			m_Reader = new CIStreamReader( m_Proxy, &mt, &hr );
			if( FAILED(hr) || m_Reader == NULL )
				ThrowDShowException(L"Failed to create proxy filter object.", hr);
			m_Reader->AddRef();

			// add fliter
			if( FAILED(hr = GraphBuilder()->AddFilter( m_Reader, L"Stream Reader")) )
				ThrowDShowException(L"Failed to call GraphBuilder()->AddFilter( m_Reader, L\"Stream Reader\").", hr);

			// AddFilter �����̂Ń����[�X����B
			m_Reader->Release();
	
			if( mt.subtype == MEDIASUBTYPE_Avi || mt.subtype == MEDIASUBTYPE_QTMovie )
			{
// GraphBuilder�Ɏ����I�ɃO���t���\�z��������AVideo Renderer�������ւ���
// ����O���t���\�z���Ă��������AAVI�t�@�C���ւ̑Ή��󋵂��ǂ��Ȃ�͂�
#if 1
				if( FAILED(hr = GraphBuilder()->Render(m_Reader->GetPin(0))) )
					ThrowDShowException(L"Failed to call IGraphBuilder::Render.", hr);
	
				CComPtr<IBaseFilter>	pRender;
				if( FAILED(hr = FindVideoRenderer( &pRender ) ) )
					ThrowDShowException(L"Failed to call FindVideoRenderer( &pRender ).", hr);
	
				CComPtr<IPin>	pRenderPin;
				pRenderPin = GetInPin(pRender, 0);
	
				// get decoder output pin
				CComPtr<IPin>			pDecoderPinOut;
				if( FAILED(hr = pRenderPin->ConnectedTo( &pDecoderPinOut )) )
					ThrowDShowException(L"Failed to call pRenderPin->ConnectedTo( &pDecoderPinOut ).", hr);
	
				// dissconnect pins
				if( FAILED(hr = pDecoderPinOut->Disconnect()) )
					ThrowDShowException(L"Failed to call pDecoderPinOut->Disconnect().", hr);
				if( FAILED(hr = pRenderPin->Disconnect()) )
					ThrowDShowException(L"Failed to call pRenderPin->Disconnect().", hr);
	
				// remove default render
				if( FAILED(hr = GraphBuilder()->RemoveFilter( pRender ) ) )
					ThrowDShowException(L"Failed to call GraphBuilder->RemoveFilter(pRenderPin).", hr);
	
				if( FAILED(hr = GraphBuilder()->AddFilter( pBRender, L"Buffer Renderer")) )
					ThrowDShowException(L"Failed to call GraphBuilder()->AddFilter( pBRender, L\"Buffer Renderer\").", hr);
	
				CComPtr<IPin>	pRdrPinIn;
				pRdrPinIn = GetInPin(pBRender, 0);
	
				if( FAILED(hr = GraphBuilder()->ConnectDirect( pDecoderPinOut, pRdrPinIn, NULL )) )
					ThrowDShowException(L"Failed to call GraphBuilder()->ConnectDirect( pDecoderPinOut, pRdrPinIn, NULL ).", hr);
#else
				CComPtr<IPin>			pRdrPinIn;
				CComPtr<IPin>			pSrcPinOut;
				if( FAILED(hr = pBRender->FindPin( L"In", &pRdrPinIn )) )
					ThrowDShowException(L"Failed to call pBRender->FindPin( L\"In\", &pRdrPinIn ).", hr);
				pSrcPinOut = m_Reader->GetPin(0);
				if( FAILED(hr = GraphBuilder()->Connect( pSrcPinOut, pRdrPinIn )) )
					ThrowDShowException(L"Failed to call GraphBuilder()->Connect( pSrcPinOut, pRdrPinIn ).", hr);
		
				CComPtr<IPin>			pSpliterPinIn;
				if( FAILED(hr = pSrcPinOut->ConnectedTo( &pSpliterPinIn )) )
					ThrowDShowException(L"Failed to call pSrcPinOut->ConnectedTo( &pSpliterPinIn ).", hr);
		
				{	// Connect to DDS render filter
					CComPtr<IBaseFilter>	pDDSRenderer;	// for sound renderer filter
					if( FAILED(hr = pDDSRenderer.CoCreateInstance(CLSID_DSoundRender, NULL, CLSCTX_INPROC_SERVER)) )
						ThrowDShowException(L"Failed to create sound render filter object.", hr);
					if( FAILED(hr = GraphBuilder()->AddFilter(pDDSRenderer, L"Sound Renderer")) )
						ThrowDShowException(L"Failed to call GraphBuilder()->AddFilter(pDDSRenderer, L\"Sound Renderer\").", hr);
		
					CComPtr<IBaseFilter>	pSpliter;
					PIN_INFO	pinInfo;
					if( FAILED(hr = pSpliterPinIn->QueryPinInfo( &pinInfo )) )
						ThrowDShowException(L"Failed to call pSpliterPinIn->QueryPinInfo( &pinInfo ).", hr);
					pSpliter = pinInfo.pFilter;
					pinInfo.pFilter->Release();
					if( FAILED(hr = ConnectFilters( pSpliter, pDDSRenderer ) ) )
					{
						if( FAILED(hr = GraphBuilder()->RemoveFilter( pDDSRenderer)) )	// �������Ƃ݂Ȃ��āA�t�B���^���폜����
							ThrowDShowException(L"Failed to call GraphBuilder()->RemoveFilter( pDDSRenderer).", hr);
					}
				}
#endif
			}
			else
			{
				if( FAILED(hr = GraphBuilder()->AddFilter( pBRender, L"Buffer Renderer")) )
					ThrowDShowException(L"Failed to call GraphBuilder()->AddFilter( pBRender, L\"Buffer Renderer\").", hr);
				BuildMPEGGraph( pBRender, m_Reader); // may throw an exception
			}
		}

#if 0	// �g���g����Bitmap�͏㉺�t�̌`���炵���̂ŁA�㉺���]�̂��߂̍Đڑ��͕K�v�Ȃ�
		{	// Reconnect buffer render filter
			// get decoder output pin
			CComPtr<IPin>			pDecoderPinOut;
			if( FAILED(hr = pRdrPinIn->ConnectedTo( &pDecoderPinOut )) )
				ThrowDShowException(L"Failed to call pRdrPinIn->ConnectedTo( &pDecoderPinOut ).", hr);

			// get connection media type
			CMediaType	mt;
			if( FAILED(hr = pRdrPinIn->ConnectionMediaType( &mt )) )
				ThrowDShowException(L"Failed to call pRdrPinIn->ConnectionMediaType( &mt ).", hr);

			// dissconnect pins
			if( FAILED(hr = pDecoderPinOut->Disconnect()) )
				ThrowDShowException(L"Failed to call pDecoderPinOut->Disconnect().", hr);
			if( FAILED(hr = pRdrPinIn->Disconnect()) )
				ThrowDShowException(L"Failed to call pRdrPinIn->Disconnect().", hr);

			if( IsEqualGUID( mt.FormatType(), FORMAT_VideoInfo) )
			{	// reverse vertical line
				VIDEOINFOHEADER	*pVideoInfo;
				pVideoInfo = reinterpret_cast<VIDEOINFOHEADER*>(mt.Format());
				if( pVideoInfo->bmiHeader.biHeight > 0 )
					pVideoInfo->bmiHeader.biHeight *= -1;
			}

			if( FAILED(hr = GraphBuilder()->ConnectDirect( pDecoderPinOut, pRdrPinIn, &mt )) )
				ThrowDShowException(L"Failed to call GraphBuilder()->ConnectDirect( pDecoderPinOut, pRdrPinIn, &mt ).", hr);
		}
#endif

		// query each interfaces
		if( FAILED(hr = m_GraphBuilder.QueryInterface( &m_MediaControl )) )
			ThrowDShowException(L"Failed to query IMediaControl", hr);
		if( FAILED(hr = m_GraphBuilder.QueryInterface( &m_MediaPosition )) )
			ThrowDShowException(L"Failed to query IMediaPosition", hr);
		if( FAILED(hr = m_GraphBuilder.QueryInterface( &m_MediaSeeking )) )
			ThrowDShowException(L"Failed to query IMediaSeeking", hr);
		if( FAILED(hr = m_GraphBuilder.QueryInterface( &m_MediaEventEx )) )
			ThrowDShowException(L"Failed to query IMediaEventEx", hr);

		if( FAILED(hr = m_GraphBuilder.QueryInterface( &m_BasicVideo )) )
			ThrowDShowException(L"Failed to query IBasicVideo", hr);
//		m_GraphBuilder.QueryInterface( &m_BasicAudio );
		if( FAILED(hr = m_GraphBuilder.QueryInterface( &m_BasicAudio )) )
			ThrowDShowException(L"Failed to query IBasicAudio", hr);

		if( FAILED(hr = pBRender->QueryInterface( &m_BuffAccess )) )
			ThrowDShowException(L"Failed to query IRendererBufferAccess.", hr);
		if( FAILED(hr = pBRender->QueryInterface( &m_BuffVideo )) )
			ThrowDShowException(L"Failed to query IRendererBufferVideo.", hr);

//		if( FAILED(hr = MediaSeeking()->SetTimeFormat( &TIME_FORMAT_FRAME )) )
//			ThrowDShowException(L"Failed to call IMediaSeeking::SetTimeFormat.", hr);

		// set notify event
		if(callbackwin)
		{
			if(FAILED(Event()->SetNotifyWindow((OAHWND)callbackwin, WM_GRAPHNOTIFY, (long)(this))))
				ThrowDShowException(L"Failed to set IMediaEventEx::SetNotifyWindow.", hr);
		}
	}
	catch(const wchar_t *msg)
	{
		MakeAPause(true);
		ReleaseAll();
//		CoUninitialize();
		TVPThrowExceptionMessage(msg);
	}
	catch(...)
	{
		MakeAPause(true);
		ReleaseAll();
//		CoUninitialize();
		throw;
	}

	MakeAPause(false);
//	CoUninitialize();	// �����ł�����ĂԂƂ܂������ȋC�����邯�ǁA���v�Ȃ̂��Ȃ�
}

//----------------------------------------------------------------------------
//! @brief	  	�C���^�[�t�F�C�X���������
//----------------------------------------------------------------------------
void __stdcall tTVPDSLayerVideo::ReleaseAll()
{
	if( m_MediaControl.p != NULL )
	{
		m_MediaControl->Stop();
	}

	if( m_BuffAccess.p )
		m_BuffAccess.Release();

	if( m_BuffVideo.p )
		m_BuffVideo.Release();

	tTVPDSMovie::ReleaseAll();
}
//----------------------------------------------------------------------------
//! @brief	  	�`�悷��o�b�t�@��ݒ肷��
//! @param		buff1 : �o�b�t�@1
//! @param		buff2 : �o�b�t�@2
//! @param		size : �o�b�t�@�̃T�C�Y
//----------------------------------------------------------------------------
void __stdcall tTVPDSLayerVideo::SetVideoBuffer( BYTE *buff1, BYTE *buff2, long size )
{
	if( buff1 == NULL || buff2 == NULL )
		TVPThrowExceptionMessage(L"SetVideoBuffer Parameter Error");

	m_BmpBits[0] = buff1;
	m_BmpBits[1] = buff2;
	HRESULT hr;
	if( FAILED(hr = BufferAccess()->SetFrontBuffer( m_BmpBits[0], &size )) )
		ThrowDShowException(L"Failed to call IBufferAccess::SetFrontBuffer.", hr);
	if( FAILED(hr = BufferAccess()->SetBackBuffer( m_BmpBits[1], &size )) )
		ThrowDShowException(L"Failed to call IBufferAccess::SetBackBuffer.", hr);
}
//----------------------------------------------------------------------------
//! @brief	  	�t�����g�o�b�t�@���擾����
//! @param		buff : �o�b�t�@
//----------------------------------------------------------------------------
void __stdcall tTVPDSLayerVideo::GetFrontBuffer( BYTE **buff )
{
	long		size;
	HRESULT hr;
	if( FAILED(hr = BufferAccess()->GetFrontBuffer( buff, &size )) )
		ThrowDShowException(L"Failed to call IBufferAccess::GetFrontBuffer.", hr);
}
//----------------------------------------------------------------------------
//! @brief	  	�r�f�I�̉摜�T�C�Y���擾����
//! @param		width : ��
//! @param		height : ����
//----------------------------------------------------------------------------
void __stdcall tTVPDSLayerVideo::GetVideoSize( long *width, long *height )
{
	if( width != NULL )
		BufferVideo()->get_VideoWidth( width );

	if( height != NULL )
		BufferVideo()->get_VideoHeight( height );
}
//----------------------------------------------------------------------------
//! @brief	  	1�t���[���̕��ϕ\�����Ԃ��擾���܂�
//! @param		pAvgTimePerFrame : 1�t���[���̕��ϕ\������
//! @return		�G���[�R�[�h
//----------------------------------------------------------------------------
HRESULT __stdcall tTVPDSLayerVideo::GetAvgTimePerFrame( REFTIME *pAvgTimePerFrame )
{
	return BufferVideo()->get_AvgTimePerFrame( pAvgTimePerFrame );
}
