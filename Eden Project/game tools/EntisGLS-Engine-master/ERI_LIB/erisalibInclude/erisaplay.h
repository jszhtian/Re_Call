
/*****************************************************************************
                         E R I S A - L i b r a r y
 -----------------------------------------------------------------------------
    Copyright (C) 2002-2003 Leshade Entis, Entis-soft. All rights reserved.
 *****************************************************************************/


#if	!defined(__ERISA_PLAY_H__)
#define	__ERISA_PLAY_H__	1

#if	!defined(COMPACT_NOA_DECODER)


/*****************************************************************************
                    MIO�t�@�C���X�g���[���Đ��I�u�W�F�N�g
 *****************************************************************************/

class	MIODynamicPlayer	: public	ESLObject
{
public:
	// �\�z�֐�
	MIODynamicPlayer( void ) ;
	// ���Ŋ֐�
	virtual ~MIODynamicPlayer( void ) ;
	// �N���X���
	DECLARE_CLASS_INFO( MIODynamicPlayer, ESLObject )

protected:
	//
	// ���R�[�h��ǂ݃I�u�W�F�N�g
	class	EPreloadBuffer	: public	EMemoryFile
	{
	public:
		BYTE *			m_ptrBuffer ;
		ULONG			m_nKeySample ;
		MIO_DATA_HEADER	m_miodh ;
	public:
		// �\�z�֐�
		EPreloadBuffer( DWORD dwLength ) ;
		// ���Ŋ֐�
		virtual ~EPreloadBuffer( void ) ;
	} ;
	//
	// �L�[�t���[���|�C���g�I�u�W�F�N�g
	class	EKeyPoint	: public	ESLObject
	{
	public:
		ULONG	m_nKeySample ;
		DWORD	m_dwRecOffset ;
	public:
		// �\�z�֐�
		EKeyPoint( void ) { }
		EKeyPoint( const EKeyPoint & key )
			: m_nKeySample(key.m_nKeySample),
				m_dwRecOffset(key.m_dwRecOffset ) { }
	} ;
	//
	// �X���b�h�p���b�Z�[�W
	enum	ThreadMessage
	{
		tmSeekSound	= WM_USER,
		tmQuit
	} ;

	// ERI�A�j���[�V�����t�@�C��
	ERIFile					m_erif ;
	// �W�J�I�u�W�F�N�g
	ERISADecodeContext *	m_pmioc ;
	MIODecoder *			m_pmiod ;
	// ��ǂ݃X���b�h
	HANDLE					m_hThread ;
	DWORD					m_idThread ;
	// ��ǂ݃L���[
	EObjArray<EPreloadBuffer>	m_queueSound ;
	HANDLE					m_hQueueFull ;		// ��ǂ݃L���[����t
	HANDLE					m_hQueueNotEmpty ;	// ��ǂ݃L���[����łȂ�
	HANDLE					m_hQueueSpace ;		// ��ǂ݃L���[�ɓǂݍ��݉\
	unsigned int			m_nCurrentSample ;	// ���ݓǂݍ���ł���T���v����
	// �����V�[�N�p�L�[�|�C���g�z��
	EObjArray<EKeyPoint>	m_listKeySample ;
	// �r���I����
	CRITICAL_SECTION		m_cs ;

public:
	// MIO �t�@�C�����J��
	ESLError Open( ESLFileObject * pFile, unsigned int nPreloadSize = 0 ) ;
	// MIO �t�@�C�������
	void Close( void ) ;

	// �w��T���v���ֈړ����A���߂̃u���b�N�̃f�[�^���擾����
	virtual void * GetWaveBufferFrom
		( ULONG nSample, DWORD & dwBytes, DWORD & dwOffsetBytes ) ;
	// ���̉����f�[�^���X�g���[���̐擪�ł��邩�H
	virtual bool IsNextDataRewound( void ) ;
	// ���̉����f�[�^���擾
	virtual void * GetNextWaveBuffer( DWORD & dwBytes ) ;
	// �����o�b�t�@�m��
	virtual void * AllocateWaveBuffer( DWORD dwBytes ) ;
	// �����f�[�^�j������
	virtual void DeleteWaveBuffer( void * ptrWaveBuf ) ;
	// �����W�J�I�u�W�F�N�g�𐶐�
	virtual MIODecoder * CreateMIODecoder( void ) ;

public:
	// ERIFile �I�u�W�F�N�g���擾����
	const ERIFile & GetERIFile( void ) const ;
	// �`���l�������擾����
	DWORD GetChannelCount( void ) const ;
	// �T���v�����O���g�����擾����
	DWORD GetFrequency( void ) const ;
	// �T���v�����O�r�b�g����\���擾����
	DWORD GetBitsPerSample( void ) const ;
	// �S�̂̒����i�T���v�����j���擾����
	DWORD GetTotalSampleCount( void ) const ;

protected:
	// ��ǂ݃o�b�t�@���擾����
	EPreloadBuffer * GetPreloadBuffer( void ) ;
	// ��ǂ݃o�b�t�@�ɒǉ�����
	void AddPreloadBuffer( EPreloadBuffer * pBuffer ) ;

protected:
	// �X���b�h�֐�
	static DWORD WINAPI ThreadProc( LPVOID parameter ) ;
	DWORD LoadingThreadProc( void ) ;
	// �����f�[�^���R�[�h��ǂݍ���
	EPreloadBuffer * LoadSoundStream( unsigned int & nCurrentSample ) ;
	// �L�[�t���[���|�C���g��ǉ�����
	void AddKeySample( const EKeyPoint & key ) ;
	// �w��̃L�[�t���[������������
	EKeyPoint * SearchKeySample( unsigned int nKeySample ) ;
	// �w��̃T���v�����܂ރu���b�N��ǂݍ���
	void SeekKeySample( ULONG nSample, unsigned int & nCurrentSample ) ;

public:
	// �r�������i�N���e�B�J���Z�N�V�����j
	void Lock( void ) ;
	void Unlock( void ) ;

} ;


/*****************************************************************************
                    ERI�A�j���[�V�����t�@�C���I�u�W�F�N�g
 *****************************************************************************/

class	ERIAnimation	: public	ESLObject
{
public:
	// �\�z�֐�
	ERIAnimation( void ) ;
	// ���Ŋ֐�
	virtual ~ERIAnimation( void ) ;
	// �N���X���
	DECLARE_CLASS_INFO( ERIAnimation, ESLObject )

protected:
	//
	// ���R�[�h��ǂ݃I�u�W�F�N�g
	class	EPreloadBuffer	: public	EMemoryFile
	{
	public:
		BYTE *	m_ptrBuffer ;
		ULONG	m_iFrameIndex ;
		UINT64	m_ui64RecType ;
	public:
		// �\�z�֐�
		EPreloadBuffer( DWORD dwLength ) ;
		// ���Ŋ֐�
		virtual ~EPreloadBuffer( void ) ;
	} ;
	//
	// �L�[�t���[���|�C���g�\����
	class	EKeyPoint	: public	ESLObject
	{
	public:
		ULONG	m_iKeyFrame ;
		ULONG	m_dwSubSample ;
		DWORD	m_dwRecOffset ;
	public:
		// �\�z�֐�
		EKeyPoint( void ) { }
		EKeyPoint( const EKeyPoint & key )
			: m_iKeyFrame(key.m_iKeyFrame),
				m_dwSubSample(key.m_dwSubSample),
				m_dwRecOffset(key.m_dwRecOffset ) { }
	} ;
	//
	// �X���b�h�p���b�Z�[�W
	enum	ThreadMessage
	{
		tmSeekFrame	= WM_USER,
		tmSeekSound,
		tmQuit
	} ;
	//
	// �t���[���^�C�v
	enum	FrameType
	{
		ftOtherData		= -1,		// �摜�ȊO
		ftIntraFrame,				// �Ɨ��t���[���iI �s�N�`���j
		ftPredictionalFrame,		// �����t���[���iP �s�N�`���j
		ftBidirectionalFrame		// �o�����t���[���iB �s�N�`���j
	} ;

	// ERI�A�j���[�V�����t�@�C��
	bool					m_fTopDown ;
	bool					m_fWaveOutput ;
	bool					m_fWaveStreaming ;
	ERIFile					m_erif ;
	// �W�J�I�u�W�F�N�g
	DWORD					m_fdwDecFlags ;
	ERISADecodeContext *	m_peric ;
	ERISADecoder *			m_perid ;
	ERISADecodeContext *	m_pmioc ;
	MIODecoder *			m_pmiod ;
	// �摜�o�b�t�@
	unsigned int			m_iCurrentFrame ;
	unsigned int			m_iDstBufIndex ;	// ����t���[���̎w�W
	unsigned int			m_nCacheBFrames ;	// ���݂̐�ǂ݃L���[��
												// �L���b�V�����ꂽ B �t���[����
												// -1 �̎��ɂ� B �t���[����
												// �Ή����Ă��Ȃ��t�H�[�}�b�g
	EGL_IMAGE_INFO *		m_pDstImage[5] ;	// 0,1 : I, P �t���[��
												// 2   : B �t���[���W�J�p
												// 3,4 : �t�B���^�����p
	unsigned int			m_iDstFrameIndex[5] ;
												// m_pDstImage �ɑΉ�����
												// �t���[���ԍ�
	// ��ǂ݃X���b�h
	HANDLE					m_hThread ;
	DWORD					m_idThread ;
	unsigned int			m_iPreloadFrame ;
	unsigned long int		m_nPreloadWaveSamples ;
	// �摜��ǂ݃L���[
	unsigned int			m_nPreloadLimit ;
	EObjArray<EPreloadBuffer>	m_queueImage ;
	HANDLE					m_hQueueNotEmpty ;	// ��ǂ݃L���[����łȂ�
	HANDLE					m_hQueueSpace ;		// ��ǂ݃L���[�ɓǂݍ��݉\
	// �t���[���V�[�N�p�L�[�t���[���|�C���g�z��
	typedef	EObjArray<EKeyPoint>	EKeyPointList ;
	EKeyPointList			m_listKeyFrame ;
	EKeyPointList			m_listKeyWave ;
	// �r���I����
	CRITICAL_SECTION		m_cs ;

protected:
	// �摜�W�J�o�̓o�b�t�@�v��
	virtual EGL_IMAGE_INFO * CreateImageBuffer
		( DWORD format, SDWORD width, SDWORD height, DWORD bpp ) ;
	// �摜�W�J�o�̓o�b�t�@����
	virtual void DeleteImageBuffer( EGL_IMAGE_INFO * peii ) ;
	// �摜�W�J�I�u�W�F�N�g����
	virtual ERISADecoder * CreateERIDecoder( void ) ;
	// �����W�J�I�u�W�F�N�g����
	virtual MIODecoder * CreateMIODecoder( void ) ;
	// �����o�͗v��
	virtual bool RequestWaveOut
		( DWORD channels, DWORD frequency, DWORD bps ) ;
	// �����o�͏I��
	virtual void CloseWaveOut( void ) ;
	// �����f�[�^�o��
	virtual void PushWaveBuffer( void * ptrWaveBuf, DWORD dwBytes ) ;
public:
	// �����o�b�t�@�m��
	virtual void * AllocateWaveBuffer( DWORD dwBytes ) ;
	// �����f�[�^�j������
	virtual void DeleteWaveBuffer( void * ptrWaveBuf ) ;
	// �����X�g���[�~���O�J�n
	virtual void BeginWaveStreaming( void ) ;
	// �����X�g���[�~���O�I��
	virtual void EndWaveStreaming( void ) ;

public:
	// �A�j���[�V�����t�@�C�����J��
	ESLError Open
		( ESLFileObject * pFile,
			unsigned int nPreloadSize = 0, DWORD fdwFlags = 0 ) ;
	// �A�j���[�V�����t�@�C�������
	void Close( void ) ;

	// �擪�t���[���ֈړ�
	ESLError SeekToBegin( void ) ;
	// ���̃t���[���ֈړ�
	ESLError SeekToNextFrame( int nSkipFrame = 0 ) ;
	// �w��̃t���[���Ɉړ�
	ESLError SeekToFrame( unsigned int iFrameIndex ) ;
	// �w��̃t���[���̓L�[�t���[�����H
	bool IsKeyFrame( unsigned int iFrameIndex ) ;
	// �œK�ȃt���[���X�L�b�v�����擾����
	unsigned int GetBestSkipFrames( unsigned int nCurrentTime ) ;

protected:
	// �t���[����W�J����
	ESLError DecodeFrame( EPreloadBuffer * pFrame, DWORD fdwFlags = 0 ) ;
	// �p���b�g�e�[�u����K�p����
	void ApplyPaletteTable( EPreloadBuffer * pBuffer ) ;
	// ��ǂ݃o�b�t�@���擾����
	EPreloadBuffer * GetPreloadBuffer( void ) ;
	// ��ǂ݃o�b�t�@�ɒǉ�����
	void AddPreloadBuffer( EPreloadBuffer * pBuffer ) ;
	// �w��̃t���[���� I, P, B �s�N�`�������肷��
	int GetFrameBufferType( EPreloadBuffer * pBuffer ) ;

public:
	// ERIFile �I�u�W�F�N�g���擾����
	const ERIFile & GetERIFile( void ) const ;
	// �J�����g�t���[���̃C���f�b�N�X���擾����
	unsigned int CurrentIndex( void ) const ;
	// �J�����g�t���[���̉摜���擾
	const EGL_IMAGE_INFO * GetImageInfo( void ) const ;
	// �p���b�g�e�[�u���擾
	const EGL_PALETTE * GetPaletteEntries( void ) const ;
	// �L�[�t���[�����擾
	unsigned int GetKeyFrameCount( void ) const ;
	// �S�t���[�������擾
	unsigned int GetAllFrameCount( void ) const ;
	// �S�A�j���[�V�������Ԃ��擾
	unsigned int GetTotalTime( void ) const ;
	// �t���[���ԍ����玞�Ԃ֕ϊ�
	unsigned int FrameIndexToTime( unsigned int iFrameIndex ) const ;
	// ���Ԃ���t���[���ԍ��֕ϊ�
	unsigned int TimeToFrameIndex( unsigned int nMilliSec ) const ;

protected:
	// �X���b�h�֐�
	static DWORD WINAPI ThreadProc( LPVOID parameter ) ;
	DWORD LoadingThreadProc( void ) ;
	// ���摜�X�g���[����ǂݍ���
	EPreloadBuffer * LoadMovieStream( unsigned int & iCurrentFrame ) ;
	// �L�[�t���[���|�C���g��ǉ�����
	void AddKeyPoint( EKeyPointList & list, const EKeyPoint & key ) ;
	// �w��̃L�[�t���[������������
	EKeyPoint * SearchKeyPoint
		( EKeyPointList & list, unsigned int iKeyFrame ) ;
	// �w��̃t���[���ɃV�[�N����
	void SeekKeyPoint
		( EKeyPointList & list,
			unsigned int iFrame, unsigned int & iCurtrentFrame ) ;
	// �w��̉����f�[�^�܂ŃV�[�N���ăX�g���[�~���O�o�͂���
	void SeekKeyWave
		( EKeyPointList & list, unsigned int iFrame ) ;

public:
	// �r�������i�N���e�B�J���Z�N�V�����j
	void Lock( void ) ;
	void Unlock( void ) ;

} ;


/*****************************************************************************
                  ERI�A�j���[�V�����t�@�C���o�̓I�u�W�F�N�g
 *****************************************************************************/

 class	ERIAnimationWriter	: public	ESLObject
{
public:
	// �\�z�֐�
	ERIAnimationWriter( void ) ;
	// ���Ŋ֐�
	virtual ~ERIAnimationWriter( void ) ;
	// �N���X���
	DECLARE_CLASS_INFO( ERIAnimationWriter, ESLObject )

public:
	//
	// �t�@�C���^�C�v
	enum	FileIdentity
	{
		fidImage,
		fidSound,
		fidMovie
	} ;

protected:
	//
	// ���k�R���e�L�X�g
	class	EEncodeContext	: public	ERISAEncodeContext
	{
	public:
		EStreamBuffer	m_buf ;
	public:
		// �\�z�֐�
		EEncodeContext( void ) : ERISAEncodeContext( 0x10000 ) { }
		// ���Ŋ֐�
		virtual ~EEncodeContext( void ) ;
		// �f�[�^����
		void Delete( void ) { m_buf.Delete( ) ; }
		// ���̃f�[�^�������o��
		virtual ULONG WriteNextData( const BYTE * ptrBuffer, ULONG nBytes ) ;
	} ;
	//
	// �X���b�h���b�Z�[�W
	enum	ThreadMessage
	{
		tmEncodeImage	= WM_USER,
		tmQuit
	} ;

	// �����o���X�e�[�^�X
	enum	WriterStatus
	{
		wsNotOpened,
		wsOpened,
		wsWritingHeader,
		wsWritingStream
	} ;
	WriterStatus			m_wsStatus ;		// �X�e�[�^�X
	// �t�@�C���I�u�W�F�N�g
	EMCFile					m_eriwf ;			// �t�@�C���I�u�W�F�N�g
	// �w�b�_���
	DWORD					m_dwHeaderBytes ;	// �w�b�_���R�[�h�̃o�C�g��
	ERI_FILE_HEADER			m_efh ;				// �t�@�C���w�b�_
	ERI_INFO_HEADER			m_prvwih ;			// �v���r���[�摜���w�b�_
	ERI_INFO_HEADER			m_eih ;				// �摜���w�b�_
	MIO_INFO_HEADER			m_mih ;				// �������w�b�_
	// �t���[���ԍ�
	bool					m_fWithSeqTable ;	// �V�[�P���X�e�[�u���g�p
	DWORD					m_dwKeyFrame ;		// �L�[�t���[��
	DWORD					m_dwBidirectKey ;	// B �s�N�`���Ԋu
	DWORD					m_dwKeyWave ;		// �L�[�E�F�[�u
	DWORD					m_dwFrameCount ;	// �o�͍ς݃t���[������
	DWORD					m_dwWaveCount ;		// �o�͍ς݃E�F�[�u����
	DWORD					m_dwDiffFrames ;	// �����t���[���I�t�Z�b�g
	// �����o�͏��
	DWORD					m_dwMioHeaderPos ;		// �������w�b�_�̈ʒu
	DWORD					m_dwOutputWaveSamples ;	// �o�͍ς݃T���v����
	// ���k�I�u�W�F�N�g
	EEncodeContext			m_eric1 ;
	EEncodeContext			m_eric2 ;
	ERISAEncoder *			m_perie1 ;
	ERISAEncoder *			m_perie2 ;
	ERISAEncodeContext *	m_pmioc ;
	MIOEncoder *			m_pmioe ;
	// ���������p�o�b�t�@
	EGL_IMAGE_INFO *		m_peiiLast ;
	EGL_IMAGE_INFO *		m_peiiNext ;
	EGL_IMAGE_INFO *		m_peiiBuf ;
	ENumArray<DWORD>		m_lstEncFlags ;
	EPtrObjArray<EGL_IMAGE_INFO>
							m_lstFrameBuf ;		// B �s�N�`���p�摜�o�b�t�@
	// �����o�̓o�b�t�@
	bool					m_fKeyWaveBlock ;
	EStreamBuffer			m_bufWaveBuffer ;
	DWORD					m_dwWaveBufSamples ;
	// �摜���k�X���b�h
	bool					m_fDualEncoding ;
	bool					m_fCompressSuccessed ;
	HANDLE					m_hCompressed ;
	HANDLE					m_hThread ;
	DWORD					m_idThread ;
	// �摜�̈��k�p�����[�^
	ERISAEncoder::PARAMETER	m_eriep_i ;
	ERISAEncoder::PARAMETER	m_eriep_p ;
	ERISAEncoder::PARAMETER	m_eriep_b ;
	// �����̈��k�p�����[�^
	MIOEncoder::PARAMETER	m_mioep ;

public:
	// �t�@�C�����J��
	ESLError Open( ESLFileObject * pFile, FileIdentity fidType ) ;
	// �t�@�C�������
	void Close( void ) ;

public:
	// �t�@�C���w�b�_���J��
	ESLError BeginFileHeader
		( DWORD dwKeyFrame, DWORD dwKeyWave, DWORD dwBidirectKey = 3 ) ;
	// �v���r���[�摜���w�b�_�������o��
	ESLError WritePreviewInfo( const ERI_INFO_HEADER & eih ) ;
	// �摜���w�b�_�������o��
	ESLError WriteEriInfoHeader( const ERI_INFO_HEADER & eih ) ;
	// �������w�b�_�������o��
	ESLError WriteMioInfoHeader( const MIO_INFO_HEADER & mih ) ;
	// ���쌠���������o��
	ESLError WriteCopyright( const void * ptrCopyright, DWORD dwBytes ) ;
	// �R�����g�������o��
	ESLError WriteDescription( const void * ptrDescription, DWORD dwBytes ) ;
	// �V�[�P���X�e�[�u���������o��
	ESLError WriteSequenceTable
		( ERIFile::SEQUENCE_DELTA * pSequence, DWORD dwLength ) ;
	// �t�@�C���w�b�_�����
	void EndFileHeader( void ) ;

public:
	// �摜�̈��k�p�����[�^��ݒ肷��
	void SetImageCompressionParameter
			( const ERISAEncoder::PARAMETER & eriep ) ;
	// �����̈��k�p�����[�^��ݒ肷��
	void SetSoundCompressionParameter
			( const MIOEncoder::PARAMETER & mioep ) ;

public:
	// �X�g���[�����J�n����
	ESLError BeginStream( void ) ;
	// �p���b�g�e�[�u���������o��
	ESLError WritePaletteTable
		( const EGL_PALETTE * paltbl, unsigned int nLength ) ;
	// �v���r���[�摜���o�͂���
	ESLError WritePreviewData( const EGL_IMAGE_INFO & eii, DWORD fdwFlags ) ;
	// �����f�[�^���o�͂���
	ESLError WriteWaveData( const void * ptrWaveBuf, DWORD dwSampleCount ) ;
	// �摜�f�[�^���o�͂���
	ESLError WriteImageData( const EGL_IMAGE_INFO & eii, DWORD fdwFlags ) ;
	// �X�g���[�������
	ESLError EndStream( DWORD dwTotalTime ) ;

protected:
	// B �s�N�`�������k���ď����o��
	ESLError WriteBirectionalFrames( void ) ;
	// �����f�[�^�����k���ď����o��
	ESLError WriteWaveBuffer( void ) ;
	// �摜�o�b�t�@�𐶐�
	EGL_IMAGE_INFO * CreateImageBuffer( const ERI_INFO_HEADER & eih ) ;
	// �摜�o�b�t�@������
	void DeleteImageBuffer( EGL_IMAGE_INFO * peii ) ;
	// �摜���k�I�u�W�F�N�g�𐶐�
	virtual ERISAEncoder * CreateERIEncoder( void ) ;
	// �������k�I�u�W�F�N�g�𐶐�
	virtual MIOEncoder * CreateMIOEncoder( void ) ;

public:
	// �o�͂��ꂽ�摜�̖������擾����
	DWORD GetWrittenFrameCount( void ) const ;
	// �}���`�X���b�h���k��L����
	void EnableDualEncoding( bool fDualEncoding ) ;

protected:
	// �X���b�h�֐�
	static DWORD WINAPI ThreadProc( LPVOID parameter ) ;
	DWORD EncodingThreadProc( void ) ;

} ;


#endif

#endif
