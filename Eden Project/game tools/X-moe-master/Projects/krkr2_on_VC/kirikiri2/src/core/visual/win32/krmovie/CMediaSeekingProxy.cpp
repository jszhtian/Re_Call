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

#include "CMediaSeekingProxy.h"

//#define ENABLE_SET

//----------------------------------------------------------------------------
//! @brief	  	CMediaSeekingProxy constructor
//! @param		delegate : �������ϔC���邽�߂̃N���X
//----------------------------------------------------------------------------
CMediaSeekingProxy::CMediaSeekingProxy(IMediaSeeking* delegate)
: m_Delegate(delegate)
{}
//----------------------------------------------------------------------------
//! @brief	  	CMediaSeekingProxy destructor
//----------------------------------------------------------------------------
CMediaSeekingProxy::~CMediaSeekingProxy()
{}
//----------------------------------------------------------------------------
//! @brief	  	�������ϔC����V�[�N�C���^�[�t�F�C�X��ݒ肷��
//! @param		seeker : �������ϔC���邽�߂̃N���X
//----------------------------------------------------------------------------
void CMediaSeekingProxy::SetSeeker( IMediaSeeking *seeker )
{
//	if( m_Delegate.p != NULL )
//		Release();
	m_Delegate = seeker;
}
//----------------------------------------------------------------------------
//! @brief	  	�v�����ꂽ�C���^�[�t�F�C�X��Ԃ�
//! @param		riid : �C���^�[�t�F�C�X��IID
//! @param		ppvObj : �C���^�[�t�F�C�X��Ԃ��|�C���^�[�ւ̃|�C���^
//! @return		�G���[�R�[�h
//----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE CMediaSeekingProxy::QueryInterface( REFIID riid, LPVOID *ppvObj )
{
	if( IID_IMediaSeeking == riid )
		return GetInterface( static_cast<IMediaSeeking*>(this), ppvObj );
	return E_NOINTERFACE;
}
//----------------------------------------------------------------------------
//! @brief	  	�C���^�[�t�F�C�X�̎Q�ƃJ�E���g�� 1 �����₷
//! @return		�V�����Q�ƃJ�E���g�l��Ԃ�
//----------------------------------------------------------------------------
ULONG STDMETHODCALLTYPE CMediaSeekingProxy::AddRef()
{
	return MediaSeeking()->AddRef();
}
//----------------------------------------------------------------------------
//! @brief	  	�C���^�[�t�F�C�X�̎Q�ƃJ�E���g�� 1 ������������
//! @return		�V�����Q�ƃJ�E���g�l��Ԃ�
//----------------------------------------------------------------------------
ULONG STDMETHODCALLTYPE CMediaSeekingProxy::Release()
{
	return MediaSeeking()->Release();
}
//----------------------------------------------------------------------------
//! @brief	  	�V�[�N�\�͂��擾����
//! @param		pCapabilities : �V�[�N�\��
//! @return		�G���[�R�[�h
//----------------------------------------------------------------------------
STDMETHODIMP CMediaSeekingProxy::GetCapabilities(DWORD *pCapabilities)
{
	return MediaSeeking()->GetCapabilities(pCapabilities);
}
//----------------------------------------------------------------------------
//! @brief	  	�w�肵���V�[�N�\�͂��X�g���[���������Ă��邩�ǂ�����₢���킹��
//! @param		pCapabilities : �V�[�N�\��
//! @return		�G���[�R�[�h
//----------------------------------------------------------------------------
STDMETHODIMP CMediaSeekingProxy::CheckCapabilities(DWORD *pCapabilities)
{
	return MediaSeeking()->CheckCapabilities(pCapabilities);
}
//----------------------------------------------------------------------------
//! @brief	  	�^�C���t�H�[�}�b�g���T�|�[�g����Ă��邩�ǂ����m�F����
//! @param		pFormat : �^�C���t�H�[�}�b�g
//! @return		�G���[�R�[�h
//----------------------------------------------------------------------------
STDMETHODIMP CMediaSeekingProxy::IsFormatSupported(const GUID *pFormat)
{
	return MediaSeeking()->IsFormatSupported(pFormat);
}
//----------------------------------------------------------------------------
//! @brief	  	�X�g���[���̗D��^�C�� �t�H�[�}�b�g���擾����
//! @param		pFormat : �^�C���t�H�[�}�b�g
//! @return		�G���[�R�[�h
//----------------------------------------------------------------------------
STDMETHODIMP CMediaSeekingProxy::QueryPreferredFormat(GUID *pFormat)
{
	return MediaSeeking()->QueryPreferredFormat(pFormat);
}
//----------------------------------------------------------------------------
//! @brief	  	�^�C���t�H�[�}�b�g��ݒ肷��
//! @param		pFormat : �^�C���t�H�[�}�b�g
//! @return		�G���[�R�[�h
//----------------------------------------------------------------------------
STDMETHODIMP CMediaSeekingProxy::SetTimeFormat(const GUID *pFormat)
{
#ifdef ENABLE_SET
	return MediaSeeking()->SetTimeFormat(pFormat);
#else
	return S_OK;
#endif
}
//----------------------------------------------------------------------------
//! @brief	  	���݂̃^�C�� �t�H�[�}�b�g���擾����
//! @param		pFormat : �^�C���t�H�[�}�b�g
//! @return		�G���[�R�[�h
//----------------------------------------------------------------------------
STDMETHODIMP CMediaSeekingProxy::GetTimeFormat( GUID *pFormat)
{
	return MediaSeeking()->GetTimeFormat(pFormat);
}
//----------------------------------------------------------------------------
//! @brief	  	�X�g���[���̎��ԕ����擾����
//! @param		pDuration : ����
//! @return		�G���[�R�[�h
//----------------------------------------------------------------------------
STDMETHODIMP CMediaSeekingProxy::GetDuration(LONGLONG *pDuration)
{
	return MediaSeeking()->GetDuration(pDuration);
}
//----------------------------------------------------------------------------
//! @brief	  	�X�g���[���̒�~���Ԃ��擾����
//! @param		pStop : ��~����
//! @return		�G���[�R�[�h
//----------------------------------------------------------------------------
STDMETHODIMP CMediaSeekingProxy::GetStopPosition(LONGLONG *pStop)
{
	return MediaSeeking()->GetStopPosition(pStop);
}
//----------------------------------------------------------------------------
//! @brief	  	�X�g���[���̌��ݎ��Ԃ��擾����
//! @param		pCurrent : ���ݎ���
//! @return		���T�|�[�g
//----------------------------------------------------------------------------
STDMETHODIMP CMediaSeekingProxy::GetCurrentPosition(LONGLONG *pCurrent)
{
	return MediaSeeking()->GetCurrentPosition(pCurrent);
}
//----------------------------------------------------------------------------
//! @brief	  	1 �̃^�C�� �t�H�[�}�b�g����ʂ̃^�C�� �t�H�[�}�b�g�ɕϊ�����
//! @param		pTarget : �ϊ����ꂽ�^�C�����󂯎��ϐ��ւ̃|�C���^
//! @param		pTargetFormat : �^�[�Q�b�g �t�H�[�}�b�g�̃^�C�� �t�H�[�}�b�g GUID �ւ̃|�C���^�BNULL �̏ꍇ�́A���݂̃t�H�[�}�b�g���g����
//! @param		Source : �ϊ�����^�C���l
//! @param		pSourceFormat : �ϊ�����t�H�[�}�b�g�̃^�C�� �t�H�[�}�b�g GUID �ւ̃|�C���^�BNULL �̏ꍇ�́A���݂̃t�H�[�}�b�g���g����
//! @return		�G���[�R�[�h
//----------------------------------------------------------------------------
STDMETHODIMP CMediaSeekingProxy::ConvertTimeFormat(LONGLONG *pTarget, const GUID *pTargetFormat, LONGLONG Source, const GUID *pSourceFormat)
{
	return MediaSeeking()->ConvertTimeFormat(pTarget,pTargetFormat,Source,pSourceFormat);
}
//----------------------------------------------------------------------------
//! @brief	  	���݈ʒu�ƒ�~�ʒu��ݒ肷��
//! @param		pCurrent : ���݈ʒu���w�肷��ϐ��ւ̃|�C���^�A���݂̃^�C�� �t�H�[�}�b�g�̒P��
//! @param		CurrentFlags : �ʒu���w�肷�邽�߂̃t���O�̃r�b�g���Ƃ̑g�ݍ��킹
//! @param		pStop : �I���^�C�����w�肷��ϐ��ւ̃|�C���^�A���݂̃^�C�� �t�H�[�}�b�g�̒P��
//! @param		StopFlags : �ʒu���w�肷�邽�߂̃t���O�̃r�b�g���Ƃ̑g�ݍ��킹
//! @return		�G���[�R�[�h
//----------------------------------------------------------------------------
STDMETHODIMP CMediaSeekingProxy::SetPositions(LONGLONG *pCurrent,DWORD dwCurrentFlags,LONGLONG *pStop,DWORD dwStopFlags)
{
#ifdef ENABLE_SET
	return MediaSeeking()->SetPositions(pCurrent,dwCurrentFlags,pStop,dwStopFlags);
#else
	return S_OK;
#endif
}
//----------------------------------------------------------------------------
//! @brief	  	���݂̈ʒu�ƒ�~�ʒu���擾����
//! @param		pCurrent : �J�n�ʒu���󂯎��ϐ��ւ̃|�C���^
//! @param		pStop : ��~�ʒu���󂯎��ϐ��ւ̃|�C���^
//! @return		�G���[�R�[�h
//----------------------------------------------------------------------------
STDMETHODIMP CMediaSeekingProxy::GetPositions(LONGLONG *pCurrent, LONGLONG *pStop)
{
	return MediaSeeking()->GetPositions(pCurrent,pStop);
}
//----------------------------------------------------------------------------
//! @brief	  	�V�[�N���L���ȃ^�C���͈̔͂��擾����
//! @param		pEarliest : �V�[�N���L���ȍł������^�C�����󂯎��ϐ��ւ̃|�C���^
//! @param		pLatest : �V�[�N���L���ȍł��x���^�C�����󂯎��ϐ��ւ̃|�C���^
//! @return		�G���[�R�[�h
//----------------------------------------------------------------------------
STDMETHODIMP CMediaSeekingProxy::GetAvailable(LONGLONG *pEarliest, LONGLONG *pLatest)
{
	return MediaSeeking()->GetAvailable(pEarliest,pLatest);
}
//----------------------------------------------------------------------------
//! @brief	  	�Đ����[�g��ݒ肷��
//! @param		dRate : �Đ����[�g
//! @return		�G���[�R�[�h
//----------------------------------------------------------------------------
STDMETHODIMP CMediaSeekingProxy::SetRate(double dRate)
{
#ifdef ENABLE_SET
	return MediaSeeking()->SetRate(dRate);
#else
	return S_OK;
#endif
}
//----------------------------------------------------------------------------
//! @brief	  	�Đ����[�g���擾����
//! @param		dRate : �Đ����[�g
//! @return		�G���[�R�[�h
//----------------------------------------------------------------------------
STDMETHODIMP CMediaSeekingProxy::GetRate(double *dRate)
{
	return MediaSeeking()->GetRate(dRate);
}
//----------------------------------------------------------------------------
//! @brief	  	�J�n�ʒu�̑O�ɃL���[�ɓ���f�[�^�̗ʂ��擾����
//! @param		pPreroll : �v�����[�� �^�C�����󂯎��ϐ��ւ̃|�C���^
//! @return		�G���[�R�[�h
//----------------------------------------------------------------------------
STDMETHODIMP CMediaSeekingProxy::GetPreroll(LONGLONG *pllPreroll)
{
	return MediaSeeking()->GetPreroll(pllPreroll);
}
//----------------------------------------------------------------------------
//! @brief	  	�w�肵���^�C�� �t�H�[�}�b�g�����ݎg���Ă���t�H�[�}�b�g���ǂ������m�F����
//! @param		pFormat : �^�C���t�H�[�}�b�g
//! @return		�G���[�R�[�h
//----------------------------------------------------------------------------
STDMETHODIMP CMediaSeekingProxy::IsUsingTimeFormat(const GUID *pFormat)
{
	return MediaSeeking()->IsUsingTimeFormat(pFormat);
}

