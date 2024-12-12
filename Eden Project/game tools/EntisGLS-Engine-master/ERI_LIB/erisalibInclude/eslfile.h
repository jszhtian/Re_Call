
/*****************************************************************************
                   Entis Standard Library declarations
 ----------------------------------------------------------------------------

	In this file, the file object classes definitions.

	Copyright (C) 1998-2002 Leshade Entis.  All rights reserved.

 ****************************************************************************/


#if	!defined(__ESLFILE_H__)
#define	__ESLFILE_H__	1

/*****************************************************************************
                           �t�@�C�����ۃN���X
 ****************************************************************************/

class	ESLFileObject : public	ESLObject
{
public:
	// �\�z�֐�
	ESLFileObject( void ) ;
	// ���Ŋ֐�
	virtual ~ESLFileObject( void ) ;
	// �N���X���
	DECLARE_CLASS_INFO( ESLFileObject, ESLObject )

protected:
	CRITICAL_SECTION	m_cs ;
	int					m_nAttribute ;

public:
	// �X���b�h�r���A�N�Z�X�p�֐�
	void Lock( void ) const ;
	void Unlock( void ) const ;

public:
	// ����
	enum	OpenFlag
	{
		modeCreateFlag	= 0x0001 ,
		modeCreate		= 0x0005 ,
		modeRead		= 0x0002 ,
		modeWrite		= 0x0004 ,
		modeReadWrite	= 0x0006 ,
		shareRead		= 0x0010 ,
		shareWrite		= 0x0020
	} ;
protected:
	void SetAttribute( int nAttribute )
		{
			m_nAttribute = nAttribute ;
		}
public:
	int GetAttribute( void ) const
		{
			return	m_nAttribute ;
		}

public:
	// �t�@�C���I�u�W�F�N�g�𕡐�����
	virtual ESLFileObject * Duplicate( void ) const = 0 ;

public:
	// �t�@�C������ǂݍ���
	virtual unsigned long int Read
		( void * ptrBuffer, unsigned long int nBytes ) = 0 ;
	// �t�@�C���֏����o��
	virtual unsigned long int Write
		( const void * ptrBuffer, unsigned long int nBytes ) = 0 ;

public:
	// �V�[�N���@
	enum	SeekOrigin
	{
		FromBegin	= FILE_BEGIN,
		FromCurrent	= FILE_CURRENT,
		FromEnd		= FILE_END
	} ;
	// �t�@�C���̒������擾
	virtual unsigned long int GetLength( void ) const = 0 ;
	// �t�@�C���|�C���^���ړ�
	virtual unsigned long int Seek
		( long int nOffsetPos, SeekOrigin fSeekFrom ) = 0 ;
	// �t�@�C���|�C���^���擾
	virtual unsigned long int GetPosition( void ) const = 0 ;
	// �t�@�C���̏I�[�����݂̈ʒu�ɐݒ肷��
	virtual ESLError SetEndOfFile( void ) ;

public:	// 64 �r�b�g�t�@�C����
	// �t�@�C���̒������擾
	virtual UINT64 GetLargeLength( void ) const ;
	// �t�@�C���|�C���^���ړ�
	virtual UINT64 SeekLarge
		( INT64 nOffsetPos, SeekOrigin fSeekFrom ) ;
	// �t�@�C���|�C���^���擾
	virtual UINT64 GetLargePosition( void ) const ;

} ;


/*****************************************************************************
                           ���t�@�C���N���X
 ****************************************************************************/

class	ERawFile : public	ESLFileObject
{
protected:
	HANDLE			m_hFile ;
	EString			m_strFilePath ;
	const char *	m_pszFileTitle ;

public:
	// �\�z�֐�
	ERawFile( void ) ;
	// ���Ŋ֐�
	virtual ~ERawFile( void ) ;
	// �N���X���
	DECLARE_CLASS_INFO( ERawFile, ESLFileObject )

public:
	// �t�@�C�����J��
	ESLError Open( const char * pszFileName, int nOpenFlags ) ;
	// �t�@�C���n���h���𕡐����ăt�@�C���I�u�W�F�N�g�Ɋ֘A�t����
	ESLError Create( HANDLE hFile, int nOpenFlags ) ;
	// �t�@�C�������
	void Close( void ) ;
	// �t�@�C���I�u�W�F�N�g�𕡐�����
	virtual ESLFileObject * Duplicate( void ) const ;

public:
	// �t�@�C������f�[�^��ǂݍ���
	virtual unsigned long int Read
		( void * ptrBuffer, unsigned long int nBytes ) ;
	// �t�@�C���փf�[�^�������o��
	virtual unsigned long int Write
		( const void * ptrBuffer, unsigned long int nBytes ) ;

public:
	// �t�@�C���̒������擾����
	virtual unsigned long int GetLength( void ) const ;
	// �t�@�C���|�C���^���ړ�����
	virtual unsigned long int Seek
		( long int nOffsetPos, SeekOrigin fSeekFrom ) ;
	// �t�@�C���|�C���^���擾����
	virtual unsigned long int GetPosition( void ) const ;
	// �t�@�C���̏I�[�����݂̈ʒu�ɐݒ肷��
	virtual ESLError SetEndOfFile( void ) ;

public:	// 64 �r�b�g�t�@�C����
	// �t�@�C���̒������擾
	virtual UINT64 GetLargeLength( void ) const ;
	// �t�@�C���|�C���^���ړ�
	virtual UINT64 SeekLarge
		( INT64 nOffsetPos, SeekOrigin fSeekFrom ) ;
	// �t�@�C���|�C���^���擾
	virtual UINT64 GetLargePosition( void ) const ;

public:
	// �t�@�C���n���h�����擾����
	HANDLE GetFileHandle( void ) const
		{
			return	m_hFile ;
		}
	operator HANDLE ( void ) const
		{
			return	m_hFile ;
		}
	// �t�@�C���̃^�C���X�^���v���擾����
	ESLError GetFileTime
		( LPSYSTEMTIME lpCreationTime,
			LPSYSTEMTIME lpLastAccessTime, LPSYSTEMTIME lpLastWriteTime ) ;
	// �t�@�C���̃^�C���X�^���v��ݒ肷��
	ESLError SetFileTime
		( const SYSTEMTIME * lpCreationTime,
			const SYSTEMTIME * lpLastAccessTime,
				const SYSTEMTIME * lpLastWriteTime ) ;

public:
	// �t�@�C���t���p�X���擾����
	const char * GetFilePath( void ) const
		{
			return	(const char *) m_strFilePath ;
		}
	// �t�@�C�������擾����
	const char * GetFileTitle( void ) const
		{
			return	 m_pszFileTitle ;
		}

} ;


/*****************************************************************************
                         �������t�@�C���N���X
 ****************************************************************************/

class	EMemoryFile : public	ESLFileObject
{
protected:
	void *				m_ptrMemory ;
	unsigned long int	m_nLength ;
	unsigned long int	m_nPosition ;
	unsigned long int	m_nBufferSize ;

public:
	// �\�z�֐�
	EMemoryFile( void ) ;
	// ���Ŋ֐�
	virtual ~EMemoryFile( void ) ;
	// �N���X���
	DECLARE_CLASS_INFO( EMemoryFile, ESLFileObject )

public:
	// �ǂݏ����\�ȃ������t�@�C�����쐬����
	ESLError Create( unsigned long int nLength ) ;
	// �ǂݍ��ݐ�p�̃������t�@�C�����쐬����
	ESLError Open( const void * ptrMemory, unsigned long int nLength ) ;
	// �������\�[�X���������
	void Delete( void ) ;
	// �����������ւ̃|�C���^���擾����
	void * GetBuffer( void ) const
		{
			return	m_ptrMemory ;
		}

	// �������t�@�C���𕡐�����
	virtual ESLFileObject * Duplicate( void ) const ;

public:
	// �������t�@�C������f�[�^��]������
	virtual unsigned long int Read
		( void * ptrBuffer, unsigned long int nBytes ) ;
	// �������t�@�C���փf�[�^��]������
	virtual unsigned long int Write
		( const void * ptrBuffer, unsigned long int nBytes ) ;

public:
	// �������t�@�C���̒������擾����
	virtual unsigned long int GetLength( void ) const ;
	// �t�@�C���|�C���^���ړ�����
	virtual unsigned long int Seek
		( long int nOffsetPos, SeekOrigin fSeekFrom ) ;
	// �t�@�C���|�C���^���擾����
	virtual unsigned long int GetPosition( void ) const ;
	// �t�@�C���̏I�[�����݂̈ʒu�ɐݒ肷��
	virtual ESLError SetEndOfFile( void ) ;

} ;


#if	!defined(COMPACT_NOA_DECODER)

/*****************************************************************************
                 �X�g���[�~���O�o�b�t�@�t�@�C���N���X
 ****************************************************************************/

class	EStreamFileBuffer	: public ESLFileObject, public EStreamBuffer
{
public:
	// �\�z�֐�
	EStreamFileBuffer( void ) ;
	// ���Ŋ֐�
	virtual ~EStreamFileBuffer( void ) ;
	// �N���X���
	DECLARE_CLASS_INFO2( EStreamFileBuffer, ESLFileObject, EStreamBuffer )

public:
	// �t�@�C���I�u�W�F�N�g�𕡐�����
	virtual ESLFileObject * Duplicate( void ) const ;

public:
	// �t�@�C������ǂݍ���
	virtual unsigned long int Read
		( void * ptrBuffer, unsigned long int nBytes ) ;
	// �t�@�C���֏����o��
	virtual unsigned long int Write
		( const void * ptrBuffer, unsigned long int nBytes ) ;

public:
	// �t�@�C���̒������擾
	virtual unsigned long int GetLength( void ) const ;
	// �t�@�C���|�C���^���ړ�
	virtual unsigned long int Seek
		( long int nOffsetPos, SeekOrigin fSeekFrom ) ;
	// �t�@�C���|�C���^���擾
	virtual unsigned long int GetPosition( void ) const ;

} ;


/*****************************************************************************
                 �����X�g���[�~���O�o�b�t�@�t�@�C���N���X
 ****************************************************************************/

class	ESyncStreamFile	: public	ESLFileObject
{
public:
	// �\�z�֐�
	ESyncStreamFile( void ) ;
	// ���Ŋ֐�
	virtual ~ESyncStreamFile( void ) ;
	// �N���X���
	DECLARE_CLASS_INFO( ESyncStreamFile, ESLFileObject )

public:
	enum
	{
		BUFFER_SIZE = 0x4000,
		BUFFER_SCALE = 14,
		BUFFER_MASK = 0x3FFF
	} ;
protected:
	class	EBuffer
	{
	public:
		BYTE *	pbytBuf ;
		DWORD	dwBytes ;
	public:
		EBuffer( void )
			{
				pbytBuf = (BYTE*) ::eslHeapAllocate( NULL, BUFFER_SIZE, 0 ) ;
				dwBytes = 0 ;
			}
		~EBuffer( void )
			{
				::eslHeapFree( NULL, pbytBuf ) ;
			}
	} ;
	EObjArray<EBuffer>	m_tblBuffer ;
	HANDLE				m_hFinished ;
	HANDLE				m_hWritten ;
	unsigned long int	m_nLength ;
	unsigned long int	m_nPosition ;
	unsigned long int	m_nBufLength ;

public:
	// ���e��������
	void Initialize( unsigned long int nLength = -1 ) ;
	// �o�b�t�@�ւ̏������݂�����
	void FinishStream( void ) ;

public:
	// �t�@�C���I�u�W�F�N�g�𕡐�����
	virtual ESLFileObject * Duplicate( void ) const ;

public:
	// �t�@�C������ǂݍ���
	virtual unsigned long int Read
		( void * ptrBuffer, unsigned long int nBytes ) ;
	// �t�@�C���֏����o��
	virtual unsigned long int Write
		( const void * ptrBuffer, unsigned long int nBytes ) ;

public:
	// �t�@�C���̒������擾
	virtual unsigned long int GetLength( void ) const ;
	// �t�@�C���|�C���^���ړ�
	virtual unsigned long int Seek
		( long int nOffsetPos, SeekOrigin fSeekFrom ) ;
	// �t�@�C���|�C���^���擾
	virtual unsigned long int GetPosition( void ) const ;
	// �t�@�C���̏I�[�����݂̈ʒu�ɐݒ肷��
	virtual ESLError SetEndOfFile( void ) ;

} ;


#endif	//	!defined(COMPACT_NOA_DECODER)

#endif
