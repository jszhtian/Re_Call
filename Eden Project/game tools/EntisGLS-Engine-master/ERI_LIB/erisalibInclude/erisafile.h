
/*****************************************************************************
                         E R I S A - L i b r a r y
 -----------------------------------------------------------------------------
    Copyright (C) 2002-2003 Leshade Entis, Entis-soft. All rights reserved.
 *****************************************************************************/


#if	!defined(__ERISA_FILE_H__)
#define	__ERISA_FILE_H__	1


/*****************************************************************************
                  EMC (Entis Media Complex) �t�@�C���`��
 *****************************************************************************/

class	EMCFile	: public	ESLFileObject
{
public:
	// �\�z�֐�
	EMCFile( void ) ;
	// ���Ŋ֐�
	virtual ~EMCFile( void ) ;
	// �N���X���
	DECLARE_CLASS_INFO( EMCFile, ESLFileObject )

public:
	enum	FileIdentity
	{
		fidArchive			= 0x02000400,
		fidRasterizedImage	= 0x03000100,
		fidEGL3DSurface		= 0x03001100,
		fidEGL3DModel		= 0x03001200,
		fidUndefinedEMC		= -1
	} ;
	struct	FILE_HEADER
	{
		char	cHeader[8] ;			// �t�@�C���V�O�l�`��
		DWORD	dwFileID ;				// �t�@�C�����ʎq
		DWORD	dwReserved ;			// �\�񁁂O
		char	cFormatDesc[0x30] ;		// �t�H�[�}�b�g��
	} ;
protected:
	struct	RECORD_HEADER
	{
		UINT64			nRecordID ;		// ���R�[�h�̎��ʎq
		UINT64			nRecLength ;	// ���R�[�h�̒���
	} ;
	struct	RECORD_INFO
	{
		RECORD_INFO *	pParent ;		// �e���R�[�h
		DWORD			dwWriteFlag ;	// �������݃t���O
		UINT64			qwBasePos ;		// ���R�[�h�̊�ʒu
		RECORD_HEADER	rechdr ;		// ���R�[�h�w�b�_
	} ;

	ESLFileObject *	m_pFile ;			// �t�@�C���I�u�W�F�N�g
	RECORD_INFO *	m_pRecord ;			// ���݂̃��R�[�h
	FILE_HEADER		m_fhHeader ;		// �t�@�C���w�b�_

	static char	m_cDefSignature[8] ;

public:
	// �t�@�C�����J��
	ESLError Open( ESLFileObject * pfile,
				const FILE_HEADER * pfhHeader = NULL ) ;
	// �t�@�C�������
	void Close( void ) ;
	// EMC �t�@�C���𕡐�
	virtual ESLFileObject * Duplicate( void ) const ;

public:
	// �t�@�C���w�b�_�擾
	const FILE_HEADER & GetFileHeader( void ) const
		{
			return	m_fhHeader ;
		}
	// �t�@�C���w�b�_������
	static void SetFileHeader
		( FILE_HEADER & fhHeader,
			DWORD dwFileID, const char * pszDesc = NULL ) ;
	// �f�t�H���g�t�@�C���V�O�l�`�����擾
	static void GetFileSignature( char cHeader[8] ) ;
	// �f�t�H���g�t�@�C���V�O�l�`����ݒ�
	static void SetFileSignature( const char cHeader[8] ) ;

public:
	// ���R�[�h���J��
	virtual ESLError DescendRecord( const UINT64 * pRecID = NULL ) ;
	// ���R�[�h�����
	virtual ESLError AscendRecord( void ) ;
	// ���R�[�h���ʎq���擾
	UINT64 GetRecordID( void ) const
		{
			ESLAssert( m_pRecord != NULL ) ;
			return	m_pRecord->rechdr.nRecordID ;
		}
	// ���R�[�h�����擾
	UINT64 GetRecordLength( void ) const
		{
			ESLAssert( m_pRecord != NULL ) ;
			return	m_pRecord->rechdr.nRecLength ;
		}

public:
	// �f�[�^��ǂݍ���
	virtual unsigned long int Read
		( void * ptrBuffer, unsigned long int nBytes ) ;
	// �f�[�^�������o��
	virtual unsigned long int Write
		( const void * ptrBuffer, unsigned long int nBytes ) ;
	// ���R�[�h�̒������擾����
	virtual UINT64 GetLargeLength( void ) const ;
	virtual unsigned long int GetLength( void ) const ;
	// �t�@�C���|�C���^���ړ�����
	virtual UINT64 SeekLarge
		( INT64 nOffsetPos, SeekOrigin fSeekFrom ) ;
	virtual unsigned long int Seek
		( long int nOffsetPos, SeekOrigin fSeekFrom ) ;
	// �t�@�C���|�C���^���擾����
	virtual UINT64 GetLargePosition( void ) const ;
	virtual unsigned long int GetPosition( void ) const ;
	// �t�@�C���̏I�[�����݂̈ʒu�ɐݒ肷��
	virtual ESLError SetEndOfFile( void ) ;

} ;


/*****************************************************************************
                        ERI �t�@�C���C���^�[�t�F�[�X
 *****************************************************************************/

 #if	!defined(COMPACT_NOA_DECODER)

class	ERIFile	: public	EMCFile
{
public:
	//
	// �^�O���C���f�b�N�X
	enum	TagIndex
	{
		tagTitle,				// �Ȗ�
		tagVocalPlayer,			// �̎�E���t��
		tagComposer,			// ��Ȏ�
		tagArranger,			// �ҋȎ�
		tagSource,				// �o�W�E�A���o��
		tagTrack,				// �g���b�N
		tagReleaseDate,			// �����[�X�N����
		tagGenre,				// �W������
		tagRewindPoint,			// ���[�v�|�C���g
		tagHotSpot,				// �z�b�g�X�|�b�g
		tagResolution,			// �𑜓x
		tagComment,				// �R�����g
		tagWords,				// �̎�
		tagMax
	} ;
	//
	// �^�O��񕶎���
	static const wchar_t *	m_pwszTagName[tagMax] ;
	//
	// �^�O���I�u�W�F�N�g
	class	ETagObject
	{
	public:
		EWideString		m_tag ;
		EWideString		m_contents ;
	public:
		// �\�z�֐�
		ETagObject( void ) ;
		// ���Ŋ֐�
		~ETagObject( void ) ;
	} ;
	//
	// �^�O����̓I�u�W�F�N�g
	class	ETagInfo
	{
	public:
		EObjArray<ETagObject>	m_lstTags ;
	public:
		// �\�z�֐�
		ETagInfo( void ) ;
		// ���Ŋ֐�
		~ETagInfo( void ) ;
		// �^�O�����
		void CreateTagInfo( const wchar_t * pwszDesc ) ;
		// �^�O�����t�H�[�}�b�g
		void FormatDescription( EWideString & wstrDesc ) ;
		// �^�O��ǉ�����
		void AddTag( TagIndex tagIndex, const wchar_t * pwszContents ) ;
		// �^�O���̃N���A
		void DeleteContents( void ) ;
		// �^�O���擾
		const wchar_t * GetTagContents( const wchar_t * pwszTag ) const ;
		const wchar_t * GetTagContents( TagIndex tagIndex ) const ;
		// �g���b�N�ԍ����擾
		int GetTrackNumber( void ) const ;
		// �����[�X�N�������擾
		ESLError GetReleaseDate( int & year, int & month, int & day ) const ;
		// ���[�v�|�C���g���擾
		int GetRewindPoint( void ) const ;
		// �z�b�g�X�|�b�g���擾
		EGL_POINT GetHotSpot( void ) const ;
		// �𑜓x���擾
		long int GetResolution( void ) const ;
	} ;
	//
	// �V�[�P���X�\����
	struct	SEQUENCE_DELTA
	{
		DWORD	dwFrameIndex ;
		DWORD	dwDuration ;
	} ;

	// �ǂݍ��݃}�X�N
	enum	ReadMask
	{
		rmFileHeader	= 0x00000001,
		rmPreviewInfo	= 0x00000002,
		rmImageInfo		= 0x00000004,
		rmSoundInfo		= 0x00000008,
		rmCopyright		= 0x00000010,
		rmDescription	= 0x00000020,
		rmPaletteTable	= 0x00000040,
		rmSequenceTable	= 0x00000080
	} ;
	DWORD			m_fdwReadMask ;
	// �t�@�C���w�b�_
	ERI_FILE_HEADER	m_FileHeader ;
	// �v���r���[�摜���w�b�_
	ERI_INFO_HEADER	m_PreviewInfo ;
	// �摜���w�b�_
	ERI_INFO_HEADER	m_InfoHeader ;
	// �������w�b�_
	MIO_INFO_HEADER	m_MIOInfHdr ;
	// �p���b�g�e�[�u��
	EGL_PALETTE		m_PaletteTable[0x100] ;
	// ���쌠���
	EWideString		m_wstrCopyright ;
	// �R�����g
	EWideString		m_wstrDescription ;

protected:
	DWORD				m_dwSeqLength ;
	SEQUENCE_DELTA *	m_pSequence ;

public:
	// �\�z�֐�
	ERIFile( void ) ;
	// ���Ŋ֐�
	virtual ~ERIFile( void ) ;
	// �N���X���
	DECLARE_CLASS_INFO( ERIFile, EMCFile )

public:
	// �t�@�C���̃I�[�v�����@
	enum	OpenType
	{
		otOpenRoot,			// ���[�g���R�[�h���J������
		otReadHeader,		// ���w�b�_���R�[�h��ǂݍ���Œl������
		otOpenStream,		// �w�b�_��ǂݍ��݃X�g���[�����R�[�h���J��
		otOpenImageData		// �摜�f�[�^���R�[�h���J��
	} ;
	// ERI �t�@�C�����J��
	ESLError Open( ESLFileObject * pFile, OpenType type = otOpenImageData ) ;

public:
	// �V�[�P���X�e�[�u�����擾����
	const SEQUENCE_DELTA * GetSequenceTable( DWORD * pdwLength ) const ;

} ;

#endif	//	!defined(COMPACT_NOA_DECODER)


/*****************************************************************************
                    ERISA �A�[�J�C�u�`�� (NOA �`��)
 *****************************************************************************/

class	ERISAArchive	: public	EMCFile
{
public:
	// �\�z�֐�
	ERISAArchive( void ) ;
	// ���Ŋ֐�
	virtual ~ERISAArchive( void ) ;
	// �N���X���
	DECLARE_CLASS_INFO( ERISAArchive, EMCFile )

public:
	struct	FILE_TIME
	{
		BYTE	nSecond ;
		BYTE	nMinute ;
		BYTE	nHour ;
		BYTE	nWeek ;
		BYTE	nDay ;
		BYTE	nMonth ;
		WORD	nYear ;
	} ;
	struct	FILE_INFO
	{
		UINT64		nBytes ;
		DWORD		dwAttribute ;
		DWORD		dwEncodeType ;
		UINT64		nOffsetPos ;
		FILE_TIME	ftFileTime ;
		DWORD		dwExtraInfoBytes ;
		void *		ptrExtraInfo ;
		DWORD		dwFileNameLen ;
		char *		ptrFileName ;

		FILE_INFO( void ) ;
		FILE_INFO( const FILE_INFO & finfo ) ;
		~FILE_INFO( void ) ;
		void SetExtraInfo( const void * pInfo, DWORD dwBytes ) ;
		void SetFileName( const char * pszFileName ) ;
	} ;
	enum	FileAttribute
	{
		attrNormal			= 0x00000000,
		attrReadOnly		= 0x00000001,
		attrHidden			= 0x00000002,
		attrSystem			= 0x00000004,
		attrDirectory		= 0x00000010,
		attrEndOfDirectory	= 0x00000020,
		attrNextDirectory	= 0x00000040
	} ;
	enum	EncodeType
	{
		etRaw				= 0x00000000,
		etERISACode			= 0x80000010,
		etBSHFCrypt			= 0x40000000,
		etERISACrypt		= 0xC0000010
	} ;
	class	EDirectory	: public	EObjArray<FILE_INFO>
	{
	public:
		EDirectory *	m_pParent ;
		DWORD			m_dwWriteFlag ;
	public:
		EDirectory( void )
			: m_pParent( NULL ), m_dwWriteFlag( 0 ) { }
		~EDirectory( void ) { }
		void CopyDirectory( const EDirectory & dir ) ;
		FILE_INFO * GetFileInfo( const char * pszFileName ) ;
		void AddFileEntry
			( const char * pszFileName, DWORD dwAttribute = 0,
				DWORD dwEncodeType = etRaw, FILE_TIME * pTime = NULL ) ;
	} ;
	enum	OpenType
	{
		otNormal,
		otStream
	} ;

protected:
	EDirectory *			m_pCurDir ;			// ���݂̃f�B���N�g��
	FILE_INFO *				m_pCurFile ;		// ���݂̃t�@�C��

	ERISADecodeContext *	m_pDecERISA ;		// ERISA ����
	ERISADecodeContext *	m_pDecBSHF ;		// BSHF ����
	EMemoryFile *			m_pBufFile ;		// �W�J���ꂽ�t�@�C���f�[�^

#if	!defined(COMPACT_NOA_DECODER)
	ERISAEncodeContext *	m_pEncERISA ;		// ERISA ������
	ERISAEncodeContext *	m_pEncBSHF ;		// BSHF �Í���
#endif
	UINT64					m_qwWrittenBytes ;	// �����o���ꂽ�o�C�g��
	BYTE					m_bufCRC[4] ;		// �r�b�g�`�F�b�N�p
	int						m_iCRC ;

public:
	// �A�[�J�C�u�t�@�C�����J��
	ESLError Open
		( ESLFileObject * pfile,
			const EDirectory * pRootDir = NULL ) ;
	// �A�[�J�C�u�t�@�C�������
	void Close( void ) ;

protected:
	// ���R�[�h���J��
	virtual ESLError DescendRecord( const UINT64 * pRecID = NULL ) ;
	// ���R�[�h�����
	virtual ESLError AscendRecord( void ) ;

public:
	// �J�����g�f�B���N�g���̃t�@�C���G���g�����擾����
	ESLError GetFileEntries( EDirectory & dirFiles ) ;
	EDirectory & ReferFileEntries( void )
		{
			ESLAssert( m_pCurDir != NULL ) ;
			return	*m_pCurDir ;
		}
	// �w��̃f�B���N�g�����J��
	ESLError DescendDirectory
		( const char * pszDirName, const EDirectory * pDir = NULL ) ;
	// �f�B���N�g�������
	ESLError AscendDirectory( void ) ;
	// �t�@�C�����J��
	ESLError DescendFile
		( const char * pszFileName,
			const char * pszPassword = NULL, OpenType otType = otNormal ) ;
	// �t�@�C�������
	ESLError AscendFile( void ) ;
	// �f�B���N�g�����J���i�p�X�w��j
	ESLError OpenDirectory( const char * pszDirPath ) ;
	// �t�@�C�����J���i�p�X�w��j
	ESLError OpenFile
		( const char * pszFilePath,
			const char * pszPassword = NULL, OpenType otType = otNormal ) ;

protected:
	// ���݂̈ʒu����f�B���N�g���G���g�����R�[�h�������o��
	ESLError WriteinDirectoryEntry( void ) ;
	// ���݂̈ʒu����f�B���N�g���G���g�����R�[�h��ǂݍ���
	ESLError LoadDirectoryEntry( void ) ;

protected:
	// �W�J�E�����̐i�s�󋵂�ʒm����
	virtual ESLError OnProcessFileData( DWORD dwCurrent, DWORD dwTotal ) ;

public:
	// �t�@�C���I�u�W�F�N�g�𕡐�����
	virtual ESLFileObject * Duplicate( void ) const ;
	// �A�[�J�C�u����f�[�^��ǂݍ���
	virtual unsigned long int Read
		( void * ptrBuffer, unsigned long int nBytes ) ;
	// �A�[�J�C�u�Ƀf�[�^�������o��
	virtual unsigned long int Write
		( const void * ptrBuffer, unsigned long int nBytes ) ;
	// �t�@�C���̒������擾����
	virtual UINT64 GetLargeLength( void ) const ;
	virtual unsigned long int GetLength( void ) const ;
	// �t�@�C���|�C���^���ړ�����
	virtual UINT64 SeekLarge
		( INT64 nOffsetPos, SeekOrigin fSeekFrom ) ;
	virtual unsigned long int Seek
		( long int nOffsetPos, SeekOrigin fSeekFrom ) ;
	// �t�@�C���|�C���^���擾����
	virtual UINT64 GetLargePosition( void ) const ;
	virtual unsigned long int GetPosition( void ) const ;

} ;


/*****************************************************************************
                    ERISA �A�[�J�C�u���t�@�C�����X�g
 *****************************************************************************/

class	ERISAArchiveList
{
public:
	// �\�z�֐�
	ERISAArchiveList( void ) ;
	// ���Ŋ֐�
	~ERISAArchiveList( void ) ;

public:
	class	EDirectory ;
	class	EFileEntry
	{
	public:
		EString			m_strFileName ;
		DWORD			m_dwAttribute ;
		DWORD			m_dwEncodeType ;
		EString			m_strPassword ;
		EDirectory *	m_pSubDir ;
	public:
		EFileEntry( void )
			: m_dwAttribute( 0 ), m_dwEncodeType( 0 ), m_pSubDir( NULL ) { }
		~EFileEntry( void )
			{
				delete	m_pSubDir ;
			}
	} ;
	class	EDirectory	: public	EObjArray<EFileEntry>
	{
	public:
		EDirectory *	m_pParentDir ;
	public:
		EDirectory( void ) : m_pParentDir( NULL ) { }
	} ;

protected:
	EDirectory		m_dirRoot ;
	EDirectory *	m_pCurDir ;

public:
	// �t�@�C�����X�g��ǂݍ���
	ESLError LoadFileList( ESLFileObject & file ) ;
	// �t�@�C�����X�g�������o��
	ESLError SaveFileList( ESLFileObject & file ) ;
	// ���e��S�č폜
	void DeleteContents( void ) ;

protected:
	// �w��̃f�B���N�g����ǂݍ���
	static ESLError ReadListOnDirectory
		( EDirectory & flist, EDescription & desc ) ;
	// �w��̃f�B���N�g���������o��
	static ESLError WriteListOnDirectory
		( EDescription & desc, EDirectory & flist ) ;

public:
	// ���݂̃f�B���N�g���Ƀt�@�C����ǉ�
	void AddFileEntry
		( const char * pszFilePath,
			DWORD dwEncodeType = ERISAArchive::etRaw,
			const char * pszPassword = NULL ) ;
	// ���݂̃f�B���N�g���ɃT�u�f�B���N�g����ǉ�
	void AddSubDirectory( const char * pszDirName ) ;
	// ���[�g�f�B���N�g���̃t�@�C���G���g�����擾
	EDirectory * GetRootFileEntries( void ) ;
	// ���݂̃f�B���N�g���̃t�@�C���G���g�����擾
	EDirectory * GetCurrentFileEntries( void ) ;
	// �T�u�f�B���N�g���ֈړ�
	ESLError DescendDirectory( const char * pszDirName ) ;
	// �e�f�B���N�g���ֈړ�
	ESLError AscendDirectory( void ) ;
	// ���݂̐�΃f�B���N�g���p�X���擾
	EString GetCurrentDirectoryPath( void ) const ;
	// ���݂̃f�B���N�g�����΃p�X���w�肵�Ĉړ�
	ESLError MoveCurrentDirectory( const char * pszDirPath ) ;

} ;


#endif
