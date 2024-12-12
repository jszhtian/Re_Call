/****************************************************************************/
/*! @file
@brief Demux source filter

�f�}���`�v���N�T�\�[�X�t�B���^����������
-----------------------------------------------------------------------------
	Copyright (C) 2005 T.Imoto <http://www.kaede-software.com>
-----------------------------------------------------------------------------
@author		T.Imoto
@date		2005/10/22
@note
*****************************************************************************/

#include <assert.h>
#include <streams.h>
#include "CDemuxSource.h"
#include "CDemuxOutputPin.h"

// {68D40D07-25DB-4c49-AAAA-327DF25387AA}
static const GUID CLSID_DemuxSource = { 0x68d40d07, 0x25db, 0x4c49, { 0xaa, 0xaa, 0x32, 0x7d, 0xf2, 0x53, 0x87, 0xaa } };


//----------------------------------------------------------------------------
//! @brief	  	CDemuxSource constructor
//! @param		lpunk : �W�����ꂽ���L�҃I�u�W�F�N�g�ւ̃|�C���^�B
//! @param		phr : HRESULT �l�ւ̃|�C���^�B
//! @param		reader : ���̃t�B���^�̃��[�_�[
//! @param		clsid : ���̃t�B���^�̃N���XID
//----------------------------------------------------------------------------
CDemuxSource::CDemuxSource( LPUNKNOWN lpunk, HRESULT *phr, IDemuxReader *reader, CLSID clsid )
: CSource( NAME("Demux Source"), lpunk, clsid, phr ), m_DemuxReader(reader)//, m_cRef(0)
{
	m_rtStop = _I64_MAX / 2;
	m_rtDuration = m_rtStop;
	m_dRateSeeking = 1.0;
	m_dwSeekingCaps = AM_SEEKING_CanSeekForwards | AM_SEEKING_CanSeekBackwards | AM_SEEKING_CanSeekAbsolute
		| AM_SEEKING_CanGetStopPos | AM_SEEKING_CanGetDuration;
}
//----------------------------------------------------------------------------
//! @brief	  	CDemuxSource destructor
//----------------------------------------------------------------------------
CDemuxSource::~CDemuxSource()
{
	delete m_DemuxReader;
}
//----------------------------------------------------------------------------
//! @brief	  	�v�����ꂽ�C���^�[�t�F�C�X��Ԃ�
//! @param		riid : �C���^�[�t�F�C�X��IID
//! @param		ppv : �C���^�[�t�F�C�X��Ԃ��|�C���^�[�ւ̃|�C���^
//! @return		�G���[�R�[�h
//----------------------------------------------------------------------------
STDMETHODIMP CDemuxSource::NonDelegatingQueryInterface( REFIID riid, void ** ppv )
{
	if( riid == IID_IMediaSeeking ) {
		return GetInterface( static_cast<IMediaSeeking*>( this ), ppv );
	} else if( riid == IID_IAMFilterMiscFlags ) {
		return GetInterface( static_cast<IAMFilterMiscFlags*>( this ), ppv );
	} else {
		return CSource::NonDelegatingQueryInterface(riid, ppv);
	}
}
//----------------------------------------------------------------------------
//! @brief	  	�Đ��������X�g���[�����J��
//! @param		stream : ���[�r�[�ւ̃X�g���[��
//! @return		�G���[�R�[�h
//----------------------------------------------------------------------------
HRESULT CDemuxSource::OpenStream( IStream *stream )
{
	CAutoLock lock(&m_crtFilterLock);

	HRESULT hr;
	if( (hr = Reader()->OpenStream(stream)) != S_OK )
		return hr;

	m_rtDuration = Reader()->GetDuration();
	m_rtStop = m_rtDuration;

	ClearPins();
	for( int i = 0; i < Reader()->GetNumberOfOutput(); ++i )
	{
		IOutputStream	*outStream = Reader()->GetOutputStream(i);
		assert( outStream );
		CDemuxOutputPin *pin = new CDemuxOutputPin( NAME("Demux Source"), this, &hr, L"Demux output pin", this, outStream, &m_crtFilterLock );
//		pin->AddRef();
		// AddPin��new�����i�K�ŃR�[�������̂ŁA�����I�ɒǉ�����K�v�͂Ȃ�
	}
	return hr;
}
//----------------------------------------------------------------------------
//! @brief	  	�Đ����[�g���ύX���ꂽ���ɃR�[�������
//! @return		�G���[�R�[�h
//----------------------------------------------------------------------------
HRESULT CDemuxSource::ChangeRate(void)
{
	{	// �N���e�B�J�� �Z�N�V���� ���b�N�̃X�R�[�v�B
		CAutoLock cAutoLock(&m_crtFilterLock);
		if( m_dRateSeeking <= 0 ) {
			m_dRateSeeking = 1.0;  // �K�؂Ȓl�Ƀ��Z�b�g����B
			return E_FAIL;
		}
	}
	UpdateFromSeek();
	return Reader()->SetRate( m_dRateSeeking );
}
//----------------------------------------------------------------------------
//! @brief	  	�Đ����J�n���ꂽ���ɃR�[�������
//! @return		�G���[�R�[�h
//----------------------------------------------------------------------------
HRESULT CDemuxSource::ChangeStart(void)
{
	UpdateFromSeek();
	if( m_rtStart > m_rtDuration  )
		m_rtStart = m_rtDuration;	// �ۂ�

	return Reader()->SetStartTime( m_rtStart );
}
//----------------------------------------------------------------------------
//! @brief	  	�Đ�����~���ꂽ���ɃR�[�������
//! @return		�G���[�R�[�h
//----------------------------------------------------------------------------
HRESULT CDemuxSource::ChangeStop(void)
{
	UpdateFromSeek();
	return Reader()->SetEndTime(m_rtStop);
}
//----------------------------------------------------------------------------
//! @brief	  	�ێ����Ă���s�������ׂč폜����
//! @return		�G���[�R�[�h
//----------------------------------------------------------------------------
void CDemuxSource::ClearPins()
{
	while( GetPinCount() > 0 )
		delete GetPin(GetPinCount()-1);
}
//----------------------------------------------------------------------------
//! @brief	  	�t�B���^���|�[�Y����
//! @return		�G���[�R�[�h
//----------------------------------------------------------------------------
STDMETHODIMP CDemuxSource::Pause()
{
	{
		CAutoLock lock(&m_crtFilterLock);
		if( m_State == State_Stopped )
		{
			HRESULT	hr = Reader()->OnStart();
			if( FAILED(hr) )
				return hr;
			NewSegment();
		}
	}
	return CSource::Pause();
}
//----------------------------------------------------------------------------
//! @brief	  	�V�[�N����̕ύX�𔽉f����
//! @return		�G���[�R�[�h
//----------------------------------------------------------------------------
void CDemuxSource::UpdateFromSeek()
{
	DeliverBeginFlush();
	Stop();
	DeliverEndFlush();
}
//----------------------------------------------------------------------------
//! @brief	  	�V���ɍĐ����J�n���ꂽ���Ƃ�ʒm����
//! @return		�G���[�R�[�h
//----------------------------------------------------------------------------
HRESULT CDemuxSource::NewSegment()
{
	return DeliverNewSegment(m_rtStart, m_rtStop, m_dRateSeeking);
}
//----------------------------------------------------------------------------
//! @brief	  	�V���ɍĐ����J�n���ꂽ���Ƃ��_�E���X�g���[���֒ʒm����
//! @param		tStart : �J�n����
//! @param		tStop : ��~����
//! @param		dRate : �Đ����[�g
//! @return		�G���[�R�[�h
//----------------------------------------------------------------------------
HRESULT CDemuxSource::DeliverNewSegment(REFERENCE_TIME tStart, REFERENCE_TIME tStop, double dRate)
{
	int cPins = GetPinCount();
	for( int c = 0; c < cPins; c++ )
	{
		CBaseOutputPin *pPin = reinterpret_cast<CBaseOutputPin*>(GetPin(c));
		if( pPin->IsConnected() ) {
			HRESULT hr = pPin->DeliverNewSegment(tStart,tStop,dRate);
			if (FAILED(hr)) {
				return hr;
			}
		}
	}
	return S_OK;
}
//----------------------------------------------------------------------------
//! @brief	  	�X�g���[�����I�[�ɒB�������Ƃ��_�E���X�g���[���֒ʒm����
//! @return		�G���[�R�[�h
//----------------------------------------------------------------------------
HRESULT CDemuxSource::DeliverEndOfStream(void)
{
	int cPins = GetPinCount();
	for( int c = 0; c < cPins; c++ )
	{
		CBaseOutputPin *pPin = reinterpret_cast<CBaseOutputPin*>(GetPin(c));
		if( pPin->IsConnected() ) {
			HRESULT hr = pPin->DeliverEndOfStream();
			if (FAILED(hr)) {
				return hr;
			}
		}
	}
	return S_OK;
}
//----------------------------------------------------------------------------
//! @brief	  	�t���b�V�������̏I�����_�E���X�g���[���֗v������
//! @return		�G���[�R�[�h
//----------------------------------------------------------------------------
HRESULT CDemuxSource::DeliverEndFlush(void)
{
	int cPins = GetPinCount();
	for( int c = 0; c < cPins; c++ )
	{
		CBaseOutputPin *pPin = reinterpret_cast<CBaseOutputPin*>(GetPin(c));
		if( pPin->IsConnected() ) {
			HRESULT hr = pPin->DeliverEndFlush();
			if (FAILED(hr)) {
				return hr;
			}
		}
	}
	return S_OK;
}
//----------------------------------------------------------------------------
//! @brief	  	�t���b�V�������̊J�n���_�E���X�g���[���֗v������
//! @return		�G���[�R�[�h
//----------------------------------------------------------------------------
HRESULT CDemuxSource::DeliverBeginFlush(void)
{
	int cPins = GetPinCount();
	for( int c = 0; c < cPins; c++ )
	{
		CBaseOutputPin *pPin = reinterpret_cast<CBaseOutputPin*>(GetPin(c));
		if( pPin->IsConnected() ) {
			HRESULT hr = pPin->DeliverBeginFlush();
			if (FAILED(hr)) {
				return hr;
			}
		}
	}
	return S_OK;
}
//----------------------------------------------------------------------------
//! @brief	  	�t�B���^�^�C�v���擾����
//! @return		�t�B���^�^�C�v
//----------------------------------------------------------------------------
ULONG STDMETHODCALLTYPE CDemuxSource::GetMiscFlags(void)
{
	return AM_FILTER_MISC_FLAGS_IS_SOURCE;
}
//----------------------------------------------------------------------------
//! @brief	  	���ݐڑ�����Ă���s���̐����擾����
//! @return		�ڑ���
//----------------------------------------------------------------------------
ULONG CDemuxSource::GetNumberOfConnection()
{
	ULONG	result = 0;
	int cPins = GetPinCount();
	for( int c = 0; c < cPins; c++ )
	{
		CBaseOutputPin *pPin = reinterpret_cast<CBaseOutputPin*>(GetPin(c));
		if( pPin->IsConnected() ) {
			++result;
		}
	}
	return result;
}
//----------------------------------------------------------------------------
//! @brief	  	�^�C���t�H�[�}�b�g���T�|�[�g����Ă��邩�ǂ����m�F����
//! @param		pFormat : �^�C���t�H�[�}�b�g
//! @return		�G���[�R�[�h
//----------------------------------------------------------------------------
HRESULT CDemuxSource::IsFormatSupported(const GUID * pFormat)
{
	CheckPointer(pFormat, E_POINTER);
	// only seeking in time (REFERENCE_TIME units) is supported
	return *pFormat == TIME_FORMAT_MEDIA_TIME ? S_OK : S_FALSE;
}
//----------------------------------------------------------------------------
//! @brief	  	�X�g���[���̗D��^�C�� �t�H�[�}�b�g���擾����
//! @param		pFormat : �^�C���t�H�[�}�b�g
//! @return		�G���[�R�[�h
//----------------------------------------------------------------------------
HRESULT CDemuxSource::QueryPreferredFormat(GUID *pFormat)
{
	CheckPointer(pFormat, E_POINTER);
	*pFormat = TIME_FORMAT_MEDIA_TIME;
	return S_OK;
}
//----------------------------------------------------------------------------
//! @brief	  	�^�C���t�H�[�}�b�g��ݒ肷��
//! @param		pFormat : �^�C���t�H�[�}�b�g
//! @return		�G���[�R�[�h
//----------------------------------------------------------------------------
HRESULT CDemuxSource::SetTimeFormat(const GUID * pFormat)
{
	CheckPointer(pFormat, E_POINTER);
	// nothing to set; just check that it's TIME_FORMAT_TIME
	return *pFormat == TIME_FORMAT_MEDIA_TIME ? S_OK : E_INVALIDARG;
}
//----------------------------------------------------------------------------
//! @brief	  	�w�肵���^�C�� �t�H�[�}�b�g�����ݎg���Ă���t�H�[�}�b�g���ǂ������m�F����
//! @param		pFormat : �^�C���t�H�[�}�b�g
//! @return		�G���[�R�[�h
//----------------------------------------------------------------------------
HRESULT CDemuxSource::IsUsingTimeFormat(const GUID * pFormat)
{
	CheckPointer(pFormat, E_POINTER);
	return *pFormat == TIME_FORMAT_MEDIA_TIME ? S_OK : S_FALSE;
}
//----------------------------------------------------------------------------
//! @brief	  	���݂̃^�C�� �t�H�[�}�b�g���擾����
//! @param		pFormat : �^�C���t�H�[�}�b�g
//! @return		�G���[�R�[�h
//----------------------------------------------------------------------------
HRESULT CDemuxSource::GetTimeFormat(GUID *pFormat)
{
	CheckPointer(pFormat, E_POINTER);
	*pFormat = TIME_FORMAT_MEDIA_TIME;
	return S_OK;
}
//----------------------------------------------------------------------------
//! @brief	  	�X�g���[���̎��ԕ����擾����
//! @param		pDuration : ����
//! @return		�G���[�R�[�h
//----------------------------------------------------------------------------
HRESULT CDemuxSource::GetDuration(LONGLONG *pDuration)
{
	CheckPointer(pDuration, E_POINTER);
	CAutoLock lock(&m_crtFilterLock);
	*pDuration = m_rtDuration;
	return S_OK;
}
//----------------------------------------------------------------------------
//! @brief	  	�X�g���[���̒�~���Ԃ��擾����
//! @param		pStop : ��~����
//! @return		�G���[�R�[�h
//----------------------------------------------------------------------------
HRESULT CDemuxSource::GetStopPosition(LONGLONG *pStop)
{
	CheckPointer(pStop, E_POINTER);
	CAutoLock lock(&m_crtFilterLock);
	*pStop = m_rtStop;
	return S_OK;
}
//----------------------------------------------------------------------------
//! @brief	  	�X�g���[���̌��ݎ��Ԃ��擾����
//! @param		pCurrent : ���ݎ���
//! @return		���T�|�[�g
//----------------------------------------------------------------------------
HRESULT CDemuxSource::GetCurrentPosition(LONGLONG *pCurrent)
{
	// GetCurrentPosition is typically supported only in renderers and
	// not in source filters.
	return E_NOTIMPL;
}
//----------------------------------------------------------------------------
//! @brief	  	�V�[�N�\�͂��擾����
//! @param		pCapabilities : �V�[�N�\��
//! @return		�G���[�R�[�h
//----------------------------------------------------------------------------
HRESULT CDemuxSource::GetCapabilities( DWORD * pCapabilities )
{
	CheckPointer(pCapabilities, E_POINTER);
	*pCapabilities = m_dwSeekingCaps;
	return S_OK;
}
//----------------------------------------------------------------------------
//! @brief	  	�w�肵���V�[�N�\�͂��X�g���[���������Ă��邩�ǂ�����₢���킹��
//! @param		pCapabilities : �V�[�N�\��
//! @return		�G���[�R�[�h
//----------------------------------------------------------------------------
HRESULT CDemuxSource::CheckCapabilities( DWORD * pCapabilities )
{
	CheckPointer(pCapabilities, E_POINTER);
	// make sure all requested capabilities are in our mask
	return (~m_dwSeekingCaps & *pCapabilities) ? S_FALSE : S_OK;
}
//----------------------------------------------------------------------------
//! @brief	  	1 �̃^�C�� �t�H�[�}�b�g����ʂ̃^�C�� �t�H�[�}�b�g�ɕϊ�����
//! @param		pTarget : �ϊ����ꂽ�^�C�����󂯎��ϐ��ւ̃|�C���^
//! @param		pTargetFormat : �^�[�Q�b�g �t�H�[�}�b�g�̃^�C�� �t�H�[�}�b�g GUID �ւ̃|�C���^�BNULL �̏ꍇ�́A���݂̃t�H�[�}�b�g���g����
//! @param		Source : �ϊ�����^�C���l
//! @param		pSourceFormat : �ϊ�����t�H�[�}�b�g�̃^�C�� �t�H�[�}�b�g GUID �ւ̃|�C���^�BNULL �̏ꍇ�́A���݂̃t�H�[�}�b�g���g����
//! @return		�G���[�R�[�h
//! @note	TIME_FORMAT_MEDIA_TIME�ȊO�̓T�|�[�g���Ă��Ȃ�
//----------------------------------------------------------------------------
HRESULT CDemuxSource::ConvertTimeFormat( LONGLONG * pTarget, const GUID * pTargetFormat, LONGLONG Source, const GUID * pSourceFormat )
{
	CheckPointer(pTarget, E_POINTER);
	// format guids can be null to indicate current format

	// since we only support TIME_FORMAT_MEDIA_TIME, we don't really
	// offer any conversions.
	if(pTargetFormat == 0 || *pTargetFormat == TIME_FORMAT_MEDIA_TIME)
	{
		if(pSourceFormat == 0 || *pSourceFormat == TIME_FORMAT_MEDIA_TIME)
		{
			*pTarget = Source;
			return S_OK;
		}
	}
	return E_INVALIDARG;
}
//----------------------------------------------------------------------------
//! @brief	  	���݈ʒu�ƒ�~�ʒu��ݒ肷��
//! @param		pCurrent : ���݈ʒu���w�肷��ϐ��ւ̃|�C���^�A���݂̃^�C�� �t�H�[�}�b�g�̒P��
//! @param		CurrentFlags : �ʒu���w�肷�邽�߂̃t���O�̃r�b�g���Ƃ̑g�ݍ��킹
//! @param		pStop : �I���^�C�����w�肷��ϐ��ւ̃|�C���^�A���݂̃^�C�� �t�H�[�}�b�g�̒P��
//! @param		StopFlags : �ʒu���w�肷�邽�߂̃t���O�̃r�b�g���Ƃ̑g�ݍ��킹
//! @return		�G���[�R�[�h
//----------------------------------------------------------------------------
HRESULT CDemuxSource::SetPositions( LONGLONG * pCurrent, DWORD CurrentFlags, LONGLONG * pStop,  DWORD StopFlags )
{
	DWORD StopPosBits = StopFlags & AM_SEEKING_PositioningBitsMask;
	DWORD StartPosBits = CurrentFlags & AM_SEEKING_PositioningBitsMask;

	if(StopFlags) {
		CheckPointer(pStop, E_POINTER);
		// accept only relative, incremental, or absolute positioning
		if(StopPosBits != StopFlags) {
			return E_INVALIDARG;
		}
	}

	if(CurrentFlags) {
		CheckPointer(pCurrent, E_POINTER);
		if(StartPosBits != AM_SEEKING_AbsolutePositioning &&
			StartPosBits != AM_SEEKING_RelativePositioning) {
			return E_INVALIDARG;
		}
	}

	{	// scope for autolock
		CAutoLock lock(&m_crtFilterLock);

		// set start position
		if(StartPosBits == AM_SEEKING_AbsolutePositioning) {
			m_rtStart = *pCurrent;
		} else if(StartPosBits == AM_SEEKING_RelativePositioning) {
			m_rtStart += *pCurrent;
		}

		// set stop position
		if(StopPosBits == AM_SEEKING_AbsolutePositioning) {
			m_rtStop = *pStop;
		} else if(StopPosBits == AM_SEEKING_IncrementalPositioning) {
			m_rtStop = m_rtStart + *pStop;
		} else if(StopPosBits == AM_SEEKING_RelativePositioning) {
			m_rtStop = m_rtStop + *pStop;
		}
	}

	HRESULT hr = S_OK;
	if(SUCCEEDED(hr) && StopPosBits) {
		hr = ChangeStop();
	}
	if(StartPosBits) {
		hr = ChangeStart();
	}
	return hr;
}
//----------------------------------------------------------------------------
//! @brief	  	���݂̈ʒu�ƒ�~�ʒu���擾����
//! @param		pCurrent : �J�n�ʒu���󂯎��ϐ��ւ̃|�C���^
//! @param		pStop : ��~�ʒu���󂯎��ϐ��ւ̃|�C���^
//! @return		�G���[�R�[�h
//----------------------------------------------------------------------------
HRESULT CDemuxSource::GetPositions( LONGLONG * pCurrent, LONGLONG * pStop )
{
	if(pCurrent) {
		*pCurrent = m_rtStart;
	}
	if(pStop) {
		*pStop = m_rtStop;
	}
	return S_OK;
}
//----------------------------------------------------------------------------
//! @brief	  	�V�[�N���L���ȃ^�C���͈̔͂��擾����
//! @param		pEarliest : �V�[�N���L���ȍł������^�C�����󂯎��ϐ��ւ̃|�C���^
//! @param		pLatest : �V�[�N���L���ȍł��x���^�C�����󂯎��ϐ��ւ̃|�C���^
//! @return		�G���[�R�[�h
//----------------------------------------------------------------------------
HRESULT CDemuxSource::GetAvailable( LONGLONG * pEarliest, LONGLONG * pLatest )
{
	if(pEarliest) {
		*pEarliest = 0;
	}
	if(pLatest) {
		CAutoLock lock(&m_crtFilterLock);
		*pLatest = m_rtDuration;
	}
	return S_OK;
}
//----------------------------------------------------------------------------
//! @brief	  	�Đ����[�g��ݒ肷��
//! @param		dRate : �Đ����[�g
//! @return		�G���[�R�[�h
//----------------------------------------------------------------------------
HRESULT CDemuxSource::SetRate( double dRate)
{
	{
		CAutoLock lock(&m_crtFilterLock);
		m_dRateSeeking = dRate;
	}
	return ChangeRate();
}
//----------------------------------------------------------------------------
//! @brief	  	�Đ����[�g���擾����
//! @param		dRate : �Đ����[�g
//! @return		�G���[�R�[�h
//----------------------------------------------------------------------------
HRESULT CDemuxSource::GetRate( double * pdRate)
{
	CheckPointer(pdRate, E_POINTER);
	CAutoLock lock(&m_crtFilterLock);
	*pdRate = m_dRateSeeking;
	return S_OK;
}
//----------------------------------------------------------------------------
//! @brief	  	�J�n�ʒu�̑O�ɃL���[�ɓ���f�[�^�̗ʂ��擾����
//! @param		pPreroll : �v�����[�� �^�C�����󂯎��ϐ��ւ̃|�C���^
//! @return		�G���[�R�[�h
//----------------------------------------------------------------------------
HRESULT CDemuxSource::GetPreroll(LONGLONG *pPreroll)
{
	CheckPointer(pPreroll, E_POINTER);
	*pPreroll = 0;
	return S_OK;
}
