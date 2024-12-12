
/*****************************************************************************
                         E R I S A - L i b r a r y
													last update 2003/01/28
 -----------------------------------------------------------------------------
    Copyright (C) 2002-2003 Leshade Entis, Entis-soft. All rights reserved.
 *****************************************************************************/


#if	!defined(__ERISA_SOUND_H__)
#define	__ERISA_SOUND_H__	1


/*****************************************************************************
                            �������k�I�u�W�F�N�g
 *****************************************************************************/

class	MIOEncoder	: public	ESLObject
{
protected:
	MIO_INFO_HEADER		m_mioih ;				// �������w�b�_

	unsigned int		m_nBufLength ;			// �o�b�t�@���i�T���v�����j
	void *				m_ptrBuffer1 ;			// ���������o�b�t�@
	void *				m_ptrBuffer2 ;			// ���ёւ��o�b�t�@
	SBYTE *				m_ptrBuffer3 ;			// �C���^�[���[�u�p�o�b�t�@
	REAL32 *			m_ptrSamplingBuf ;		// �T���v�����O�p�o�b�t�@
	REAL32 *			m_ptrInternalBuf ;		// ���ԃo�b�t�@
	REAL32 *			m_ptrDstBuf ;			// �o�͗p�o�b�t�@
	REAL32 *			m_ptrWorkBuf ;			// DCT ���Z�p���[�N�G���A
	REAL32 *			m_ptrWeightTable ;		// �e���g�������̏d�݃e�[�u��
	REAL32 *			m_ptrLastDCT ;			// ���O�u���b�N�� DCT �W��

public:
	enum	PresetParameter
	{
		ppVBR235kbps, ppVBR176kbps, ppVBR156kbps,
		ppVBR141kbps, ppVBR128kbps, ppVBR117kbps,
		ppVBR94kbps, ppVBR78kbps, ppVBR70kbps,
		ppMax
	} ;
	struct	PARAMETER
	{
		double			rLowWeight ;			// ����g�����̏d��
		double			rMiddleWeight ;			// �����g�����̏d��
		double			rPowerScale ;			// �ʎq���̊�r�b�g��
		int				nOddWeight ;			// �u���b�N�c�΍�W��
		int				nPreEchoThreshold ;		// �v���G�R�[�΍�臒l

		PARAMETER( void ) ;
		void LoadPresetParam
			( PresetParameter ppIndex, MIO_INFO_HEADER & infhdr ) ;
	} ;

protected:
	PARAMETER			m_parameter ;

	SWORD *				m_ptrNextDstBuf ;		// �o�̓o�b�t�@�A�h���X
	REAL32 *			m_ptrLastDCTBuf ;		// �d�����Z�p�o�b�t�@
	unsigned int		m_nSubbandDegree ;		// �s��̃T�C�Y
	unsigned int		m_nDegreeNum ;
	int					m_nFrequencyWidth[7] ;	// �e���g���т̕�
	int					m_nFrequencyPoint[7] ;	// �e���g���т̒��S�ʒu
	ERI_SIN_COS *		m_pRevolveParam ;

public:
	// �\�z�֐�
	MIOEncoder( void ) ;
	// ���Ŋ֐�
	virtual ~MIOEncoder( void ) ;
	// �N���X���
	DECLARE_CLASS_INFO( MIOEncoder, ESLObject )

	// �������i�p�����[�^�̐ݒ�j
	virtual ESLError Initialize( const MIO_INFO_HEADER & infhdr ) ;
	// �I���i�������̉���Ȃǁj
	virtual void Delete( void ) ;
	// ���������k
	virtual ESLError EncodeSound
		( ERISAEncodeContext & context,
			const MIO_DATA_HEADER & datahdr, const void * ptrWaveBuf ) ;
	// ���k�I�v�V������ݒ�
	void SetCompressionParameter( const PARAMETER & parameter ) ;

protected:		// �t���k
	// 8�r�b�g��PCM�����k
	ESLError EncodeSoundPCM8
		( ERISAEncodeContext & context,
			const MIO_DATA_HEADER & datahdr, const void * ptrWaveBuf ) ;
	// 16�r�b�g��PCM�����k
	ESLError EncodeSoundPCM16
		( ERISAEncodeContext & context,
			const MIO_DATA_HEADER & datahdr, const void * ptrWaveBuf ) ;

protected:		// ��t���k
	// �s��T�C�Y�̕ύX�ɔ����p�����[�^�̍Čv�Z
	void InitializeWithDegree( unsigned int nSubbandDegree ) ;
	// �w��T���v����̉��ʂ����߂�
	double EvaluateVolume( const REAL32 * ptrWave, int nCount ) ;
	// �����R�[�h���擾����
	int GetDivisionCode( const REAL32 * ptrSamples ) ;

protected:	// ���m�����E�X�e���I
	// 16�r�b�g�̔�t���k
	ESLError EncodeSoundDCT
		( ERISAEncodeContext & context,
			const MIO_DATA_HEADER & datahdr, const void * ptrWaveBuf ) ;
	// LOT �ϊ����{��
	void PerformLOT
		( ERISAEncodeContext & context,
			REAL32 * ptrSamples, REAL32 rPowerScale ) ;
	// �ʏ�̃u���b�N�𕄍������ďo�͂���
	ESLError EncodeInternalBlock
		( ERISAEncodeContext & context,
			REAL32 * ptrSamples, REAL32 rPowerScale ) ;
	// ���[�h�u���b�N�𕄍������ďo�͂���
	ESLError EncodeLeadBlock
		( ERISAEncodeContext & context,
			REAL32 * ptrSamples, REAL32 rPowerScale ) ;
	// �|�X�g�u���b�N�𕄍������ďo�͂���
	ESLError EncodePostBlock
		( ERISAEncodeContext & context, REAL32 rPowerScale ) ;
	// �ʎq��
	void Quantumize
		( PINT ptrQuantumized, const REAL32 * ptrSource,
			int nDegreeNum, REAL32 rPowerScale,
			DWORD * ptrWeightCode, int * ptrCoefficient ) ;

protected:		// �~�h���T�C�h�X�e���I
	// 16�r�b�g�̔�t���k
	ESLError EncodeSoundDCT_MSS
		( ERISAEncodeContext & context,
			const MIO_DATA_HEADER & datahdr, const void * ptrWaveBuf ) ;
	// ��]�p�����[�^���擾����
	int GetRevolveCode
		( const REAL32 * ptrBuf1, const REAL32 * ptrBuf2 ) ;
	// LOT �ϊ����{��
	void PerformLOT_MSS
		( REAL32 * ptrDst, REAL32 * ptrLapBuf, REAL32 * ptrSrc ) ;
	// �ʏ�̃u���b�N�𕄍������ďo�͂���
	ESLError EncodeInternalBlock_MSS
		( ERISAEncodeContext & context,
			REAL32 * ptrSrc1, REAL32 * ptrSrc2, REAL32 rPowerScale ) ;
	// ���[�h�u���b�N�𕄍������ďo�͂���
	ESLError EncodeLeadBlock_MSS
		( ERISAEncodeContext & context,
			REAL32 * ptrSrc1, REAL32 * ptrSrc2, REAL32 rPowerScale ) ;
	// �|�X�g�u���b�N�𕄍������ďo�͂���
	ESLError EncodePostBlock_MSS
		( ERISAEncodeContext & context, REAL32 rPowerScale ) ;
	// �ʎq��
	void Quantumize_MSS
		( PINT ptrQuantumized, const REAL32 * ptrSource,
			int nDegreeNum, REAL32 rPowerScale,
			DWORD * ptrWeightCode, int * ptrCoefficient ) ;

} ;


/*****************************************************************************
                            �����W�J�I�u�W�F�N�g
 *****************************************************************************/

 class	MIODecoder	: public	ESLObject
{
protected:
	MIO_INFO_HEADER		m_mioih ;				// �������w�b�_

	unsigned int		m_nBufLength ;			// �o�b�t�@���i�T���v�����j
	void *				m_ptrBuffer1 ;			// ���������o�b�t�@
	void *				m_ptrBuffer2 ;			// ���ёւ��o�b�t�@
	SBYTE *				m_ptrBuffer3 ;			// �C���^�[���[�u�p�o�b�t�@
	PBYTE				m_ptrDivisionTable ;	// �����R�[�h�e�[�u��
	PBYTE				m_ptrRevolveCode ;		// ��]�R�[�h�e�[�u��
	SDWORD *			m_ptrWeightCode ;		// �ʎq���W���e�[�u��
	PINT				m_ptrCoefficient ;		//
	REAL32 *			m_ptrMatrixBuf ;		// �s�񉉎Z�p�o�b�t�@
	REAL32 *			m_ptrInternalBuf ;		// ���ԃo�b�t�@
	REAL32 *			m_ptrWorkBuf ;			// DCT ���Z�p���[�N�G���A
	REAL32 *			m_ptrWorkBuf2 ;
	REAL32 *			m_ptrWeightTable ;		// �e���g�������̏d�݃e�[�u��
	REAL32 *			m_ptrLastDCT ;			// ���O�� DCT �W��

	PBYTE				m_ptrNextDivision ;		// ���̕����R�[�h
	PBYTE				m_ptrNextRevCode ;		// ���̉�]�R�[�h
	SDWORD *			m_ptrNextWeight ;		// ���̗ʎq���W��
	PINT				m_ptrNextCoefficient ;	//
	PINT				m_ptrNextSource ;		// ���̓��͐M��
	REAL32 *			m_ptrLastDCTBuf ;		// �d�����Z�p�o�b�t�@
	unsigned int		m_nSubbandDegree ;		// �s��̃T�C�Y
	unsigned int		m_nDegreeNum ;
	ERI_SIN_COS *		m_pRevolveParam ;
	int					m_nFrequencyPoint[7] ;	// �e���g���т̒��S�ʒu

public:
	// �\�z�֐�
	MIODecoder( void ) ;
	// ���Ŋ֐�
	virtual ~MIODecoder( void ) ;
	// �N���X���
	DECLARE_CLASS_INFO( MIODecoder, ESLObject )

	// �������i�p�����[�^�̐ݒ�j
	virtual ESLError Initialize( const MIO_INFO_HEADER & infhdr ) ;
	// �I���i�������̉���Ȃǁj
	virtual void Delete( void ) ;
	// ���������k
	virtual ESLError DecodeSound
		( ERISADecodeContext & context,
			const MIO_DATA_HEADER & datahdr, void * ptrWaveBuf ) ;

protected:		// �t���k
	// 8�r�b�g��PCM��W�J
	ESLError DecodeSoundPCM8
		( ERISADecodeContext & context,
			const MIO_DATA_HEADER & datahdr, void * ptrWaveBuf ) ;
	// 16�r�b�g��PCM��W�J
	ESLError DecodeSoundPCM16
		( ERISADecodeContext & context,
			const MIO_DATA_HEADER & datahdr, void * ptrWaveBuf ) ;

protected:		// ���m�����E�X�e���I
	// �s��T�C�Y�̕ύX�ɔ����p�����[�^�̍Čv�Z
	void InitializeWithDegree( unsigned int nSubbandDegree ) ;
	// 16�r�b�g�̔�t�W�J
	ESLError DecodeSoundDCT
		( ERISADecodeContext & context,
			const MIO_DATA_HEADER & datahdr, void * ptrWaveBuf ) ;
	// �ʏ�̃u���b�N�𕜍�����
	ESLError DecodeInternalBlock
		( SWORD * ptrDst, unsigned int nSamples ) ;
	// ���[�h�u���b�N�𕜍�����
	ESLError DecodeLeadBlock( void ) ;
	// �|�X�g�u���b�N�𕜍�����
	ESLError DecodePostBlock
		( SWORD * ptrDst, unsigned int nSamples ) ;
	// �t�ʎq��
	void IQuantumize
		( REAL32 * ptrDestination,
			const INT * ptrQuantumized, int nDegreeNum,
			SDWORD nWeightCode, int nCoefficient ) ;

protected:		// �~�h���T�C�h�X�e���I
	// 16�r�b�g�̔�t�W�J
	ESLError DecodeSoundDCT_MSS
		( ERISADecodeContext & context,
			const MIO_DATA_HEADER & datahdr, void * ptrWaveBuf ) ;
	// �ʏ�̃u���b�N�𕜍�����
	ESLError DecodeInternalBlock_MSS
		( SWORD * ptrDst, unsigned int nSamples ) ;
	// ���[�h�u���b�N�𕜍�����
	ESLError DecodeLeadBlock_MSS( void ) ;
	// �|�X�g�u���b�N�𕜍�����
	ESLError DecodePostBlock_MSS
		( SWORD * ptrDst, unsigned int nSamples ) ;

} ;


#endif
