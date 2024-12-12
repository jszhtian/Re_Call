/****************************************************************************/
/*! @file
@brief Windows Media Reader

Windows Media�t�@�C����ǂݍ���
-----------------------------------------------------------------------------
	Copyright (C) 2005 T.Imoto <http://www.kaede-software.com>
-----------------------------------------------------------------------------
@author		T.Imoto
@date		2005/10/22
@note
*****************************************************************************/

#include "CWMReader.h"
#include "CWMAllocator.h"
#include "CWMBuffer.h"

//## CWMOutput
//----------------------------------------------------------------------------
//! @brief	  	CWMOutput destructor
//----------------------------------------------------------------------------
CWMOutput::~CWMOutput()
{}
//----------------------------------------------------------------------------
//! @brief	  	�v�����ꂽ�C���^�[�t�F�C�X��Ԃ�
//! @param		riid : �C���^�[�t�F�C�X��IID
//! @param		ppv : �C���^�[�t�F�C�X��Ԃ��|�C���^�[�ւ̃|�C���^
//! @return		�G���[�R�[�h
//----------------------------------------------------------------------------
STDMETHODIMP CWMOutput::NonDelegatingQueryInterface( REFIID riid, void ** ppv )
{
	return CUnknown::NonDelegatingQueryInterface(riid, ppv);
}
//----------------------------------------------------------------------------
//! @brief	  	IWMSyncReader���擾����
//! @return		IWMSyncReader�ւ̃|�C���^
//----------------------------------------------------------------------------
IWMSyncReader *CWMOutput::WMReader()
{
	return Reader()->Reader();
}
//----------------------------------------------------------------------------
//! @brief	  	�X�g���[���̐ݒ���s��
//! @return		�G���[�R�[�h
//----------------------------------------------------------------------------
HRESULT CWMOutput::SetStream(void)
{
    WMT_STREAM_SELECTION	wmtSS = WMT_ON;
	HRESULT hr = S_OK;
	if( IsEnable() )
	{
		if( FAILED( hr = WMReader()->SetStreamsSelected( 1, &m_StreamNum, &wmtSS ) ) )
			return( hr );

		if( FAILED(hr = WMReader()->SetReadStreamSamples( m_StreamNum, TRUE ) ) )
			return( hr );
	}
	return hr;
}
//----------------------------------------------------------------------------
//! @brief	  	�T�|�[�g���Ă��郁�f�B�A�^�C�v���擾����
//!
//! ���k�t�H�[�}�b�g�̂ݏo�͂���悤�ɂȂ��Ă���B
//! @param		iPosition : ���f�B�A�^�C�v�̏���
//! @param		pmt : ���f�B�A�^�C�v
//! @return		�G���[�R�[�h
//----------------------------------------------------------------------------
HRESULT CWMOutput::GetMediaType( int iPosition, CMediaType *pmt )
{
	if( iPosition != 0 ) return VFW_S_NO_MORE_ITEMS;

	HRESULT	hr;
	CComPtr<IWMProfile>	readerProfile;
	if( FAILED( hr = WMReader()->QueryInterface( IID_IWMProfile, (void **)&readerProfile ) ) )
        return( hr );

	CComPtr<IWMStreamConfig>	streamConfig;
	if( FAILED(hr = readerProfile->GetStreamByNumber( m_StreamNum, &streamConfig )) )
		return hr;

	CComPtr<IWMMediaProps> pMediaProps;
	if( FAILED(hr = streamConfig.QueryInterface( &pMediaProps ) ) )
		return hr;

	DWORD	mediaTypeSize;
	if( FAILED( hr = pMediaProps->GetMediaType( NULL, &mediaTypeSize ) ) )
		return hr;

	std::vector<BYTE>	buff(mediaTypeSize,0);
	WM_MEDIA_TYPE	*pMediaType = reinterpret_cast<WM_MEDIA_TYPE*>( &( buff.at(0) ) );
	if( FAILED( hr = pMediaProps->GetMediaType( pMediaType, &mediaTypeSize ) ) )
		return hr;

	GUID	type = pMediaType->subtype;
	if( type == WMMEDIASUBTYPE_MP43 || type == WMMEDIASUBTYPE_MP4S || type == WMMEDIASUBTYPE_MPEG2_VIDEO ||
		type == WMMEDIASUBTYPE_MSS1 || type == WMMEDIASUBTYPE_MSS2 || type == WMMEDIASUBTYPE_WMVP ||
		type == WMMEDIASUBTYPE_WMAudio_Lossless || type == WMMEDIASUBTYPE_WMAudioV2 || type == WMMEDIASUBTYPE_WMAudioV7 ||
		type == WMMEDIASUBTYPE_WMAudioV8 || type == WMMEDIASUBTYPE_WMAudioV9 || type == WMMEDIASUBTYPE_WMSP1 ||
		type == WMMEDIASUBTYPE_WMV1 || type == WMMEDIASUBTYPE_WMV2 || type == WMMEDIASUBTYPE_WMV3 )
	{
		pmt->Set( *(reinterpret_cast<AM_MEDIA_TYPE*>(pMediaType)) );
		return hr;
	}
	return VFW_S_NO_MORE_ITEMS;
}
//----------------------------------------------------------------------------
//! @brief	  	���f�B�A�^�C�v��ݒ肷��
//! @param		pmt : ���f�B�A�^�C�v
//! @return		�G���[�R�[�h
//----------------------------------------------------------------------------
HRESULT CWMOutput::SetMediaType( const CMediaType *pmt )
{
	CMediaType	mt;
	if( GetMediaType(0, &mt ) == S_OK )
	{
		if( mt.majortype == pmt->majortype &&
			mt.subtype == pmt->subtype &&
			mt.bFixedSizeSamples == pmt->bFixedSizeSamples &&
			mt.bTemporalCompression == pmt->bTemporalCompression &&
//			mt.lSampleSize == pmt->lSampleSize &&
			mt.formattype == pmt->formattype )
		{
			return S_OK;
		}
	}
	return VFW_E_INVALIDMEDIATYPE;
}
//----------------------------------------------------------------------------
//! @brief	  	�K�v�ȃo�b�t�@�T�C�Y���擾����
//! @param		buffers : �o�b�t�@�̐�
//! @param		bufsize : �o�b�t�@�̃T�C�Y
//! @return		�G���[�R�[�h
//----------------------------------------------------------------------------
HRESULT CWMOutput::GetNeedBufferSize( long &buffers, long &bufsize )
{
	HRESULT		hr;
	DWORD max;
	if( FAILED(hr = WMReader()->GetMaxStreamSampleSize( m_StreamNum, &max ) ) )
		return hr;

//	buffers = 2;
	buffers = 7;
	bufsize = max;
	return S_OK;
}
//----------------------------------------------------------------------------
//! @brief	  	�o�b�t�@�T�C�Y�̌����ʒm����
//! @param		buffers : �o�b�t�@�̐�
//! @param		bufsize : �o�b�t�@�̃T�C�Y
//! @param		aling : �A���C�����g
//! @param		prefix : �v���t�B�b�N�X
//! @return		�G���[�R�[�h
//----------------------------------------------------------------------------
HRESULT CWMOutput::SetDecidedBufferSize( long buffers, long bufsize, long aling, long prefix )
{
	// ���ɉ������Ȃ�
	return S_OK;
}
//----------------------------------------------------------------------------
//! @brief	  	���̃T���v���𓾂�
//! @param		pSample : �T���v����Ԃ��|�C���^�̃|�C���^
//! @return		�G���[�R�[�h
//----------------------------------------------------------------------------
HRESULT CWMOutput::GetNextSample( IMediaSample **pSample )
{
	HRESULT hr;
	if( m_StreamNum == 0 || pSample == NULL )
		return S_FALSE;	// ���̃X�g���[���͂Ȃ�

	INSSBuffer	*pWMSample = NULL;
	QWORD	cnsSampleTime;
	QWORD	cnsDuration;
	DWORD	dwFlags;

	if( FAILED(hr = WMReader()->GetNextSample( m_StreamNum, &pWMSample, &cnsSampleTime, &cnsDuration, &dwFlags, NULL, NULL )) )
	{
		if( hr == NS_E_NO_MORE_SAMPLES ) return S_FALSE;
		return hr;
	}

	REFERENCE_TIME	startTime = (REFERENCE_TIME)cnsSampleTime;
	REFERENCE_TIME	endTime = (REFERENCE_TIME)(cnsSampleTime + cnsDuration);
	IMediaSample *pOutSample = reinterpret_cast<CWMBuffer*>(pWMSample)->GetSample();
	pOutSample->AddRef();
	pWMSample->Release();
	pOutSample->SetMediaTime(&startTime, &endTime);
#if 0
	if( startTime < Reader()->m_StartTime )
		pOutSample->SetPreroll(TRUE);
	else
		pOutSample->SetPreroll(FALSE);
#endif
	startTime -= Reader()->m_StartTime;
	endTime -= Reader()->m_StartTime;
	pOutSample->SetTime(&startTime, &endTime);
	pOutSample->SetSyncPoint(dwFlags & WM_SF_CLEANPOINT);
	*pSample = pOutSample;

	return hr;
}
//----------------------------------------------------------------------------
//! @brief	  	�A���P�[�^�[��ݒ肷��
//! @param		alloc : �A���P�[�^�[
//! @return		�G���[�R�[�h
//----------------------------------------------------------------------------
HRESULT CWMOutput::SetAllocator( IMemAllocator *alloc )
{
	HRESULT		hr;

	if( alloc == NULL )
		return E_INVALIDARG;

	CComPtr<IWMSyncReader2>	reader2;
	if( FAILED(hr = WMReader()->QueryInterface( &reader2 ) ) )
		return hr;

	CWMAllocator *wmAlloc = new CWMAllocator( alloc );
	CComPtr<IWMReaderAllocatorEx>	pWMRAE;
	pWMRAE = wmAlloc;

	hr = reader2->SetAllocateForStream( m_StreamNum, pWMRAE );
	return hr;
}
//----------------------------------------------------------------------------
//! @brief	  	���f�B�A�^�C�v��DXVA���ǂ����m�F����
//! 
//! ������@��WMF SDK Help�� "Enabling DirectX Video Acceleration" �ɋL�q����Ă������
//! @param		pmt : ���f�B�A�^�C�v
//! @return		DXVA���ǂ���
//----------------------------------------------------------------------------
bool CWMOutput::IsDXVASubtype( const AM_MEDIA_TYPE *pmt )
{
	GUID guidDXVA = { 0x00000000, 0xa0c7, 0x11d3, { 0xb9,0x84,0x00,0xc0,0x4f,0x2e,0x73,0xc5 } };
	unsigned long const *plguid;
	unsigned long const *plguidDXVA;
	plguid = (unsigned long const *)&pmt->subtype;
	plguidDXVA = (unsigned long *)&guidDXVA;

	if( ( plguid[1] == plguidDXVA[1] ) && ( plguid[2] == plguidDXVA[2] ) && ( plguid[3] == plguidDXVA[3] ) )
		return true;
	return false;
}


//## CWMReader
#pragma warning(disable: 4355)	// �R���X�g���N�^�̃x�[�X�����o����������this���g���ƃ��[�j���O���o��̂ł����}�~
//----------------------------------------------------------------------------
//! @brief	  	CWMReader constructor
//----------------------------------------------------------------------------
CWMReader::CWMReader()
: m_AudioOut(this), m_VideoOut(this), m_StartTime(0), m_EndTime(0), m_Rate(1.0)
{
	m_AudioOut.AddRef();
	m_VideoOut.AddRef();
}
#pragma warning(default: 4355)
//----------------------------------------------------------------------------
//! @brief	  	CWMReader destructor
//----------------------------------------------------------------------------
CWMReader::~CWMReader()
{
	if( m_HeaderInfo.p )
		m_HeaderInfo.Release();

	if( m_WMReader.p )
		m_WMReader.Release();
}
//----------------------------------------------------------------------------
//! @brief	  	�o�͐����擾���� ( �f���Ɖ����Ȃ�2�Ȃ� )
//! @return		�o�͐�
//----------------------------------------------------------------------------
int CWMReader::GetNumberOfOutput(void)
{
	return (m_AudioOut.IsEnable() != 0 ? 1 : 0) + (m_VideoOut.IsEnable() != 0 ? 1 : 0);
}
//----------------------------------------------------------------------------
//! @brief	  	�o�̓X�g���[���𓾂�
//! @param		num : �o�̓X�g���[���ԍ�
//! @return		�o�̓X�g���[��
//----------------------------------------------------------------------------
IOutputStream* CWMReader::GetOutputStream( int num )
{
	if( num >= 2 || num < 0 )
		return NULL;

	if( m_VideoOut.IsEnable() )
	{
		if( num == 0 )
			return &m_VideoOut;
		else if( num == 1 && m_AudioOut.IsEnable() )
			return &m_AudioOut;
		else
			return NULL;
	}
	else if( m_AudioOut.IsEnable() )
	{
		if( num == 0 )
			return &m_AudioOut;
		else
			return NULL;
	}
	else
		return NULL;
}
//----------------------------------------------------------------------------
//! @brief	  	�X�g���[�����J��
//! @param		stream : �X�g���[��
//! @return		�G���[�R�[�h
//----------------------------------------------------------------------------
HRESULT CWMReader::OpenStream( IStream *stream )
{
	HRESULT		hr = S_OK;
	if( stream == NULL )
		return E_INVALIDARG;

	if( m_WMReader.p == NULL )
	{	// DLL����WMCreateSyncReader��ǂݍ���Ŏg�p����
		if( m_WmvDll.IsLoaded() == false )
			m_WmvDll.Load("wmvcore.dll");
		if( m_WmvDll.IsLoaded() == false )
			return m_WmvDll.GetLastError();

		typedef HRESULT (*FuncWMCreateSyncReader)( IUnknown *pUnkCert, DWORD dwRights, IWMSyncReader **ppSyncReader );
		FuncWMCreateSyncReader pWMCreateSyncReader = (FuncWMCreateSyncReader)m_WmvDll.GetProcAddress("WMCreateSyncReader");
		if( pWMCreateSyncReader == NULL )
			return m_WmvDll.GetLastError();
		if( FAILED(hr = pWMCreateSyncReader(  NULL, 0, &m_WMReader ) ))
			return hr;
	}

	if( FAILED( hr = Reader()->OpenStream( stream ) ) )
		return hr;

	CComPtr<IWMProfile>	pProfile;
	if( FAILED(hr = Reader()->QueryInterface( &pProfile ) ) )
		return hr;

	if( FAILED(hr = GetStreamNumbers( pProfile ) ) )
		return hr;

	if( FAILED(hr = Reader()->QueryInterface( &m_HeaderInfo ) ) )
		return hr;

	if( FAILED(hr = SetStream() ) )
		return hr;

//	hr = SetRange();
	return hr;
}
//----------------------------------------------------------------------------
//! @brief	  	�Đ��J�n���ɃR�[�������
//! @return		�G���[�R�[�h
//----------------------------------------------------------------------------
HRESULT CWMReader::OnStart()
{
	HRESULT hr;
	if( FAILED(hr = SetStream() ) )
		return hr;
	if( FAILED(hr = SetRange() ) )
		return hr;

	return hr;
}
//----------------------------------------------------------------------------
//! @brief	  	�X�g���[���������鎞�ɃR�[�������
//! @return		�G���[�R�[�h
//----------------------------------------------------------------------------
HRESULT CWMReader::Close(void)
{
	Reader()->Close();
	m_AudioOut.SetStreamNumber( 0 );
	m_VideoOut.SetStreamNumber( 0 );
	m_StartTime = 0;
	m_EndTime = 0;
	m_Rate = 1.0;
	return S_OK;
}
//----------------------------------------------------------------------------
//! @brief	  	�J�n���Ԃ�ݒ肷��
//! @param		start : �J�n����
//! @return		�G���[�R�[�h
//----------------------------------------------------------------------------
HRESULT CWMReader::SetStartTime( QWORD start )
{
	m_StartTime = start;
	return SetRange();
}
//----------------------------------------------------------------------------
//! @brief	  	��~���Ԃ�ݒ肷��
//! @param		end : ��~����
//! @return		�G���[�R�[�h
//----------------------------------------------------------------------------
HRESULT CWMReader::SetEndTime( QWORD end )
{
	m_EndTime = end;
	return SetRange();
}
//----------------------------------------------------------------------------
//! @brief	  	�Đ����[�g��ݒ肷��
//! @param		rate : �Đ����[�g
//! @return		�G���[�R�[�h
//----------------------------------------------------------------------------
HRESULT CWMReader::SetRate( double rate )
{
	m_Rate = rate;
	// ���Ɏg��Ȃ�
	return S_OK;
}
//----------------------------------------------------------------------------
//! @brief	  	�Đ��͈͂̐ݒ�𔽉f����
//! @return		�G���[�R�[�h
//----------------------------------------------------------------------------
HRESULT CWMReader::SetRange(void)
{
	QWORD		duration;
	if( m_EndTime != 0 )
		duration = m_EndTime - m_StartTime;
	else
		duration = GetDuration() - m_StartTime;
	return Reader()->SetRange( m_StartTime, duration );
}
//----------------------------------------------------------------------------
//! @brief	  	�o�͂���X�g���[���̐ݒ���s��
//! @return		�G���[�R�[�h
//----------------------------------------------------------------------------
HRESULT CWMReader::SetStream(void)
{
	HRESULT hr;
	if( FAILED(hr = m_VideoOut.SetStream() ) )
		return( hr );
	hr = m_AudioOut.SetStream();
	return hr;
}
//----------------------------------------------------------------------------
//! @brief	  	���f�B�A�̊e��l���擾����
//! @param		name : �v��������̖̂��O
//! @return		�v�������l
//----------------------------------------------------------------------------
QWORD CWMReader::GetAttributeQWORD( LPCWSTR name )
{
	HRESULT	hr;
	WORD	stream = 0;
	QWORD	result = 0;
	WORD	length = sizeof(QWORD);
	WMT_ATTR_DATATYPE	attrType;
	hr = HeaderInf()->GetAttributeByName( &stream, name, &attrType, (BYTE*)&result, &length );
	if( FAILED( hr ) || attrType != WMT_TYPE_QWORD || length != sizeof(QWORD) )
	{
		result = 0;
	}
	return result;
}
//----------------------------------------------------------------------------
//! @brief	  	���f�B�A�̊e��l���擾����
//! @param		name : �v��������̖̂��O
//! @return		�v�������l
//----------------------------------------------------------------------------
DWORD CWMReader::GetAttributeDWORD( LPCWSTR name )
{
	HRESULT	hr;
	WORD	stream = 0;
	DWORD	result = 0;
	WORD	length = sizeof(DWORD);
	WMT_ATTR_DATATYPE	attrType;
	hr = HeaderInf()->GetAttributeByName( &stream, name, &attrType, (BYTE*)&result, &length );
	if( FAILED( hr ) || attrType != WMT_TYPE_DWORD || length != sizeof(DWORD) )
	{
		result = 0;
	}
	return result;
}
//----------------------------------------------------------------------------
//! @brief	  	���f�B�A�̎��ԕ����擾����
//! @return		���ԕ�
//----------------------------------------------------------------------------
QWORD CWMReader::GetDuration(void)
{
	return GetAttributeQWORD(g_wszWMDuration);
}
//----------------------------------------------------------------------------
//! @brief	  	���f�B�A�̃t���[�������擾����
//! @return		�t���[����
//----------------------------------------------------------------------------
QWORD CWMReader::GetNumberOfFrame(void)
{
	return GetAttributeQWORD(g_wszWMNumberOfFrames);
}
//----------------------------------------------------------------------------
//! @brief	  	�r�f�I�̕����擾����
//! @return		�r�f�I�̕�
//----------------------------------------------------------------------------
DWORD CWMReader::GetVideoWidth(void)
{
	return GetAttributeDWORD(g_wszWMVideoWidth);
}
//----------------------------------------------------------------------------
//! @brief	  	�r�f�I�̍������擾����
//! @return		�r�f�I�̍���
//----------------------------------------------------------------------------
DWORD CWMReader::GetVideoHeight(void)
{
	return GetAttributeDWORD(g_wszWMVideoHeight);
}
//----------------------------------------------------------------------------
//! @brief	  	�r�f�I�̃t���[�����[�g���擾����
//! @return		�r�f�I�̃t���[�����[�g
//----------------------------------------------------------------------------
DWORD CWMReader::GetVideoFrameRate(void)
{
	return GetAttributeDWORD(g_wszWMVideoFrameRate);
}
//----------------------------------------------------------------------------
//! @brief	  	�X�g���[���ԍ����擾����
//!
//! m_AudioOut.m_StreamNum��m_VideoOut.m_StreamNum�֒l��ݒ肷��;
//! �X�g���[���ԍ���1-63�̒l���Ƃ�
//! @param		pProfile : �v���t�@�C��
//! @return		�G���[�R�[�h
//----------------------------------------------------------------------------
HRESULT CWMReader::GetStreamNumbers( IWMProfile* pProfile )
{
	HRESULT		hr = S_OK;
	GUID		streamType;

	if ( NULL == pProfile )
		return( E_INVALIDARG );

	DWORD	numOfStreams = 0;
	if( FAILED(hr = pProfile->GetStreamCount( &numOfStreams ) ) )
		return( hr );

	m_AudioOut.SetStreamNumber( 0 );
	m_VideoOut.SetStreamNumber( 0 );

	for( DWORD i = 0; i < numOfStreams; i++ )
	{
		CComPtr<IWMStreamConfig>	pStream;
		if( FAILED(hr = pProfile->GetStream( i, &pStream ) ) )
			break;

		WORD streamNumber = 0;
		if( FAILED(hr = pStream->GetStreamNumber( &streamNumber ) ) )
			break;

		if( FAILED(hr = pStream->GetStreamType( &streamType ) ) )
			break;

		// ��ԏ��߂Ɍ��������X�g���[�����g��
		if( WMMEDIATYPE_Audio == streamType && m_AudioOut.GetStreamNumber() == 0 )
			m_AudioOut.SetStreamNumber( streamNumber );
		else if( WMMEDIATYPE_Video == streamType && m_VideoOut.GetStreamNumber() == 0 )
			m_VideoOut.SetStreamNumber( streamNumber );
	}
	return hr;
}



