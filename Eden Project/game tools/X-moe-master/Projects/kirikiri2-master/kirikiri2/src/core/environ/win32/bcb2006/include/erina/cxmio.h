
/*****************************************************************************
                          E R I N A - L i b r a r y
                                                        �ŏI�X�V 2001/5/23
 ----------------------------------------------------------------------------
         Copyright (C) 2000-2001 Leshade Entis. All rights reserved.
 *****************************************************************************/


#if	!defined(CXMIO_H_INCLUDE)
#define	CXMIO_H_INCLUDE

#if	!defined(ERIMATRIX_H_INCLUDE)
#include "erimatrix.h"
#endif

#if	!defined(CMPERI_H_INCLUDE)
#include "cmperi.h"
#endif


/*****************************************************************************
                                �������
 *****************************************************************************/

struct	MIO_INFO_HEADER
{
	DWORD	dwVersion ;
	DWORD	fdwTransformation ;
	DWORD	dwArchitecture ;
	DWORD	dwChannelCount ;
	DWORD	dwSamplesPerSec ;
	DWORD	dwBlocksetCount ;
	DWORD	dwSubbandDegree ;
	DWORD	dwAllSampleCount ;
	DWORD	dwLappedDegree ;
	DWORD	dwBitsPerSample ;
} ;

struct	MIO_DATA_HEADER
{
	BYTE	bytVersion ;
	BYTE	bytFlags ;
	BYTE	bytReserved1 ;
	BYTE	bytReserved2 ;
	DWORD	dwSampleCount ;
} ;

#define	MIO_LEAD_BLOCK	0x01


/*****************************************************************************
                            �������k�I�u�W�F�N�g
 *****************************************************************************/

class	MIOEncoder
{
protected:
	MIO_INFO_HEADER		m_mioih ;				// �������w�b�_

	unsigned int		m_nBufLength ;			// �o�b�t�@���i�T���v�����j
	void *				m_ptrBuffer1 ;			// ���������o�b�t�@
	void *				m_ptrBuffer2 ;			// ���ёւ��o�b�t�@
	SBYTE *				m_ptrBuffer3 ;			// �C���^�[���[�u�p�o�b�t�@
	REAL32 *			m_ptrSamplingBuf ;		// �T���v�����O�p�o�b�t�@
	REAL32 *			m_ptrInternalBuf ;		// ���ԃo�b�t�@
	REAL32 *			m_ptrWorkBuf ;			// DCT ���Z�p���[�N�G���A
	REAL32 *			m_ptrWeightTable ;		// �e���g�������̏d�݃e�[�u��
	REAL32 *			m_ptrLastDCT ;			// ���O�u���b�N�� DCT �W��

	double				m_rLowWeight ;			// ����g�����̏d��
	double				m_rMiddleWeight ;		// �����g�����̏d��
	double				m_rPowerScale ;			// �ʎq���̊�r�b�g��
	int					m_nOddWeight ;			// �u���b�N�c�΍�W��

	SWORD *				m_ptrNextDstBuf ;		// �o�̓o�b�t�@�A�h���X
	REAL32 *			m_ptrLastDCTBuf ;		// �d�����Z�p�o�b�t�@
	unsigned int		m_nSubbandDegree ;		// �s��̃T�C�Y
	unsigned int		m_nDegreeNum ;
	int					m_nFrequencyWidth[7] ;	// �e���g���т̕�
	int					m_nFrequencyPoint[7] ;	// �e���g���т̒��S�ʒu
	SIN_COS *			m_pRevolveParam ;

public:
	// �\�z�֐�
	MIOEncoder( void ) ;
	// ���Ŋ֐�
	virtual ~MIOEncoder( void ) ;

	// �������i�p�����[�^�̐ݒ�j
	int Initialize( const MIO_INFO_HEADER & infhdr ) ;
	// �I���i�������̉���Ȃǁj
	void Delete( void ) ;
	// ���������k
	int EncodeSound
		( RLHEncodeContext & context,
			const MIO_DATA_HEADER & datahdr, const void * ptrWaveBuf ) ;
	// ���k�I�v�V������ݒ�
	void SetCompressionParameter
		( double rLowWeight = 16.0,
			double rMiddleWeight = 8.0,
			double rPowerScale = 0.5,
			int nOddWeight = 1 ) ;

protected:
	// 8�r�b�g��PCM�����k
	int EncodeSoundPCM8
		( RLHEncodeContext & context,
			const MIO_DATA_HEADER & datahdr, const void * ptrWaveBuf ) ;
	// 16�r�b�g��PCM�����k
	int EncodeSoundPCM16
		( RLHEncodeContext & context,
			const MIO_DATA_HEADER & datahdr, const void * ptrWaveBuf ) ;

protected:
	// �s��T�C�Y�̕ύX�ɔ����p�����[�^�̍Čv�Z
	void InitializeWithDegree( unsigned int nSubbandDegree ) ;
	// �w��T���v����̉��ʂ����߂�
	double EvaluateVolume( const REAL32 * ptrWave, int nCount ) ;
	// �����R�[�h���擾����
	int GetDivisionCode( void ) ;
	// 16�r�b�g�̔�t���k
	int EncodeSoundDCT
		( RLHEncodeContext & context,
			const MIO_DATA_HEADER & datahdr, const void * ptrWaveBuf ) ;
	// LOT �ϊ����{��
	void PerformLOT
		( RLHEncodeContext & context,
			REAL32 * ptrSamples, REAL32 rPowerScale ) ;
	// �ʏ�̃u���b�N�𕄍������ďo�͂���
	int EncodeInternalBlock
		( RLHEncodeContext & context,
			REAL32 * ptrSamples, REAL32 rPowerScale ) ;
	// ���[�h�u���b�N�𕄍������ďo�͂���
	int EncodeLeadBlock
		( RLHEncodeContext & context,
			REAL32 * ptrSamples, REAL32 rPowerScale ) ;
	// �|�X�g�u���b�N�𕄍������ďo�͂���
	int EncodePostBlock
		( RLHEncodeContext & context, REAL32 rPowerScale ) ;
	// �ʎq��
	void Quantumize
		( PINT ptrQuantumized, const REAL32 * ptrSource,
			int nDegreeNum, REAL32 rPowerScale,
			DWORD * ptrWeightCode, int * ptrCoefficient ) ;

} ;


/*****************************************************************************
                            �����W�J�I�u�W�F�N�g
 *****************************************************************************/

class	MIODecoder
{
protected:
	MIO_INFO_HEADER		m_mioih ;				// �������w�b�_

	unsigned int		m_nBufLength ;			// �o�b�t�@���i�T���v�����j
	void *				m_ptrBuffer1 ;			// ���������o�b�t�@
	void *				m_ptrBuffer2 ;			// ���ёւ��o�b�t�@
	SBYTE *				m_ptrBuffer3 ;			// �C���^�[���[�u�p�o�b�t�@
	PBYTE				m_ptrDivisionTable ;	// �����R�[�h�e�[�u��
	SDWORD *			m_ptrWeightCode ;		// �ʎq���W���e�[�u��
	PINT				m_ptrCoefficient ;		//
	REAL32 *			m_ptrMatrixBuf ;		// �s�񉉎Z�p�o�b�t�@
	REAL32 *			m_ptrInternalBuf ;		// ���ԃo�b�t�@
	REAL32 *			m_ptrWorkBuf ;			// DCT ���Z�p���[�N�G���A
	REAL32 *			m_ptrWorkBuf2 ;
	REAL32 *			m_ptrWeightTable ;		// �e���g�������̏d�݃e�[�u��
	REAL32 *			m_ptrLastDCT ;			// ���O�� DCT �W��

	PBYTE				m_ptrNextDivision ;		// ���̕����R�[�h
	SDWORD *			m_ptrNextWeight ;		// ���̗ʎq���W��
	PINT				m_ptrNextCoefficient ;	//
	PINT				m_ptrNextSource ;		// ���̓��͐M��
	REAL32 *			m_ptrLastDCTBuf ;		// �d�����Z�p�o�b�t�@
	unsigned int		m_nSubbandDegree ;		// �s��̃T�C�Y
	unsigned int		m_nDegreeNum ;
	SIN_COS *			m_pRevolveParam ;
	int					m_nFrequencyPoint[7] ;	// �e���g���т̒��S�ʒu

public:
	// �\�z�֐�
	MIODecoder( void ) ;
	// ���Ŋ֐�
	virtual ~MIODecoder( void ) ;

	// �������i�p�����[�^�̐ݒ�j
	int Initialize( const MIO_INFO_HEADER & infhdr ) ;
	// �I���i�������̉���Ȃǁj
	void Delete( void ) ;
	// ���������k
	int DecodeSound
		( RLHDecodeContext & context,
			const MIO_DATA_HEADER & datahdr, void * ptrWaveBuf ) ;

protected:
	// 8�r�b�g��PCM��W�J
	int DecodeSoundPCM8
		( RLHDecodeContext & context,
			const MIO_DATA_HEADER & datahdr, void * ptrWaveBuf ) ;
	// 16�r�b�g��PCM��W�J
	int DecodeSoundPCM16
		( RLHDecodeContext & context,
			const MIO_DATA_HEADER & datahdr, void * ptrWaveBuf ) ;

protected:
	// �s��T�C�Y�̕ύX�ɔ����p�����[�^�̍Čv�Z
	void InitializeWithDegree( unsigned int nSubbandDegree ) ;
	// 16�r�b�g�̔�t�W�J
	int DecodeSoundDCT
		( RLHDecodeContext & context,
			const MIO_DATA_HEADER & datahdr, void * ptrWaveBuf ) ;
	// �ʏ�̃u���b�N�𕜍�����
	int DecodeInternalBlock
		( RLHDecodeContext & context,
			SWORD * ptrDst, unsigned int nSamples ) ;
	// ���[�h�u���b�N�𕜍�����
	int DecodeLeadBlock
		( RLHDecodeContext & context ) ;
	// �|�X�g�u���b�N�𕜍�����
	int DecodePostBlock
		( RLHDecodeContext & context,
			SWORD * ptrDst, unsigned int nSamples ) ;
	// �ʎq��
	void IQuantumize
		( REAL32 * ptrDestination,
			const INT * ptrQuantumized, int nDegreeNum,
			SDWORD nWeightCode, int nCoefficient ) ;
	// ����
	int DecodeSamples
		( RLHDecodeContext & context,
			INT * ptrQuantumized, unsigned int nCount ) ;

} ;


#endif
