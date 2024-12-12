
/*****************************************************************************
                   Entis Standard Library declarations
 ----------------------------------------------------------------------------

	In this file, the description object classes declarations.

	Copyright (C) 2002-2004 Leshade Entis, Entis-soft. All rights reserved.

 ****************************************************************************/


#if	!defined(__ESLDESCRIPTION_H__)
#define	__ESLDESCRIPTION_H__	1

#if	!defined(COMPACT_NOA_DECODER)


/*****************************************************************************
                        �\�����L�q�I�u�W�F�N�g
 *****************************************************************************/

class	EDescription	: public	ESLObject
{
public:
	// �\�z�֐�
	EDescription( void ) ;
	EDescription( const EDescription & desc ) ;
	// ���Ŋ֐�
	virtual ~EDescription( void ) ;
	// �N���X���
	DECLARE_CLASS_INFO( EDescription, ESLObject )

protected:
	EDescription *				m_pParent ;			// �e�I�u�W�F�N�g
	EWideString					m_wstrTag ;			// �^�O��
	EWideString					m_wstrContents ;	// ���e�i�^�O������̎��j
	EWStrTagArray<EWideString>	m_wstaAttr ;		// �^�O����
	EObjArray<EDescription>		m_listDesc ;		// ���e

public:
	//
	// �������̓N���X
	class	EStreamXMLString	: public	EStreamWideString
	{
	public:
		// �\�z�֐�
		EStreamXMLString( void ) { }
		EStreamXMLString( const EStreamWideString & swidestr )
			: EStreamWideString( swidestr ) { }
		EStreamXMLString( const EGenString<wchar_t,EWideString> & widestr )
			: EStreamWideString( widestr ) { }
		EStreamXMLString( const wchar_t * pwszString, unsigned int nLength )
			: EStreamWideString( pwszString, nLength ) { }
		EStreamXMLString( const wchar_t * pwszString )
			: EStreamWideString( pwszString ) { }
		EStreamXMLString( const char * pszString, unsigned int nLength = -1 )
			: EStreamWideString( pszString, nLength ) { }
		// �������
		const EStreamXMLString &
					operator = ( const EStreamWideString & swstr )
			{
				EStreamWideString::operator = ( swstr ) ;
				return	*this ;
			}
		const EStreamXMLString &
					operator = ( const wchar_t * pwszString )
			{
				EStreamWideString::operator = ( pwszString ) ;
				return	*this ;
			}
		const EStreamXMLString &
					operator = ( const char * pszString )
			{
				EStreamWideString::operator = ( pszString ) ;
				return	*this ;
			}
		// ���݂̕�����i��؂�L���͖����j��ʉ߂���
		virtual void PassEnclosedString
			( wchar_t wchClose, int flagCtrlCode = FALSE ) ;
		// ���݂̃g�[�N����ʉ߂���iXML�d�l�j
		virtual void PassAToken( int * pTokenType = NULL ) ;
	} ;

public:
	// �������
	enum	DataFormatType
	{
		dftXML,
		dftXMLandCEsc,
		dftAuto
	} ;
	// �����G���R�[�f�B���O���
	enum	CharacterEncoding
	{
		ceShiftJIS,
		ceUTF8,
		ceISO2022JP,
		ceEUCJP,
		ceUnknown	= -1
	} ;
	// �ǂ݂���
	ESLError ReadDescription
		( EStreamBuffer & buf,
			DataFormatType dftType = dftAuto,
			CharacterEncoding ceType = ceShiftJIS ) ;
	ESLError ReadDescription
		( EStreamWideString & swsDesc,
			DataFormatType dftType = dftXMLandCEsc ) ;
	// �����o��
	ESLError WriteDescription
		( ESLFileObject & file, int nTabIndent = 0,
			DataFormatType dftType = dftAuto,
			CharacterEncoding ceType = ceShiftJIS ) ;

public:
	// ���^�O�𐶐�
	virtual EDescription * CreateDescription( void ) ;
	// �G���[�o��
	virtual ESLError OutputError( const char * pszErrMsg ) ;
	// �x���o��
	virtual ESLError OutputWarning( const char * pszErrMsg ) ;
	// �������
	const EDescription & operator = ( const EDescription & desc ) ;
	// �}�[�W����
	const EDescription & operator += ( const EDescription & desc ) ;

public:	// �^�O
	// �e�擾
	EDescription * GetParent( void ) const ;
	// �^�O���擾
	const EWideString & Tag( void ) const ;
	// �^�O���ݒ�
	void SetTag( const wchar_t * pwszTag ) ;
	// �L���ȁi�Ӗ��̂���j�^�O���ǂ����𔻒肷��
	bool IsValid( void ) const
		{
			if ( this == NULL )
				return	false ;
			if ( m_wstrTag.IsEmpty() )
				return	false ;
			wchar_t	wch = m_wstrTag.GetAt(0) ;
			return	((wch >= L'A') && (wch <= L'Z'))
				|| ((wch >= L'a') && (wch <= L'z')) || (wch == L'_') ;
		}
	// �R�����g�^�O�ɂ���
	void SetCommentTag( const wchar_t * pwszComment = NULL ) ;
	// ���e�擾
	const EWideString & Contents( void ) const ;
	// ���e�ݒ�
	void SetContents( const wchar_t * pwszContents ) ;

public:	// �^�O����
	// �^�O�����擾�i�����j
	int GetAttrInteger( const wchar_t * pwszAttr, int nDefValue ) const ;
	// �^�O�����擾�i�����j
	double GetAttrReal( const wchar_t * pwszAttr, double rDefValue ) const ;
	// �^�O�����擾�i������j
	EWideString GetAttrString
		( const wchar_t * pwszAttr, const wchar_t * pwszDefValue ) const ;
	// �^�O�����ݒ�i�����j
	void SetAttrInteger( const wchar_t * pwszAttr, int nValue ) ;
	// �^�O�����ݒ�i�����j
	void SetAttrReal( const wchar_t * pwszAttr, double rValue ) ;
	// �^�O�����ݒ�i������j
	void SetAttrString( const wchar_t * pwszAttr, const wchar_t * pwszValue ) ;
	// �^�O�����̑������擾
	int GetAttributeCount( void ) const ;
	// �^�O�����̎w�W���擾
	int GetAttributeIndexAs( const wchar_t * pwszAttr ) ;
	// �^�O�����̖��O���擾
	EWideString GetAttributeNameAt( int nIndex ) const ;
	// �^�O�����̒l���擾
	EWideString GetAttributeValueAt( int nIndex ) const ;
	// �^�O�������폜
	void RemoveAttributeAt( int nIndex ) ;
	void RemoveAttributeAs( const wchar_t * pwszAttr ) ;
	// �S�Ẵ^�O�������폜
	void RemoveAllAttribute( void ) ;

public:	// ���e��
	// �܂܂�Ă���^�O�̐����擾
	int GetContentTagCount( void ) const ;
	// �܂܂�Ă���^�O���擾
	EDescription * GetContentTagAt( int nIndex ) const ;
	// �^�O���������w�肵�Č���
	EDescription * SearchTagAs
		( int nFirst, const wchar_t * pwszAttr = NULL,
			const wchar_t * pwszValue = NULL,
			const wchar_t * pwszTag = NULL, int * pNext = NULL ) const ;
	// �^�O���������Ď擾
	EDescription * GetContentTagAs
		( int nFirst, const wchar_t * pwszTag = NULL, int * pNext = NULL ) const ;
	// �^�O�𐶐�
	EDescription * CreateContentTagAs( int nFirst, const wchar_t * pwszTag ) ;
	// �^�O������
	int FindContentTag( int nFirst, const wchar_t * pwszTag = NULL ) const ;
	// �^�O��ǉ�
	void AddContentTag( EDescription * pDesc ) ;
	// �^�O��}��
	void InsertContentTag( int nIndex, EDescription * pDesc ) ;
	// �����񑮐��^�O���擾
	EWideString GetContentString
		( const wchar_t * pwszTag, const wchar_t * pwszDefValue ) const ;
	// �����񑮐��^�O��ݒ�
	ESLError SetContentString
		( const wchar_t * pwszTag, const wchar_t * pwszValue ) ;
	// �^�O���폜
	void RemoveContentTagAt( int nIndex ) ;
	// �S�Ẵ^�O���폜
	void RemoveAllContentTag( void ) ;

public:	// �����֐�
	// �^�O������
	EDescription * GetContentTag( const wchar_t * pwszTagPath ) const ;
	// �^�O�𐶐�
	EDescription * CreateContentTag( const wchar_t * pwszTagPath ) ;
	// ������f�[�^�擾
	EWideString GetStringAt
		( const wchar_t * pwszTagPath,
			const wchar_t * pwszAttr, const wchar_t * pwszDefValue ) const ;
	// �����f�[�^�擾
	int GetIntegerAt
		( const wchar_t * pwszTagPath,
			const wchar_t * pwszAttr, int nDefValue ) const ;
	// �����f�[�^�擾
	double GetRealAt
		( const wchar_t * pwszTagPath,
			const wchar_t * pwszAttr, double rDefValue ) const ;
	// �o�C�i���f�[�^�擾
	ESLError GetBinaryAt
		( EStreamBuffer & bufBinary,
			const wchar_t * pwszTagPath, const wchar_t * pwszAttr ) const ;
	// ������f�[�^�ݒ�
	void SetStringAt( const wchar_t * pwszTagPath,
			const wchar_t * pwszAttr, const wchar_t * pwszValue ) ;
	// �����f�[�^�ݒ�
	void SetIntegerAt
		( const wchar_t * pwszTagPath,
			const wchar_t * pwszAttr, int nValue ) ;
	// �����f�[�^�ݒ�
	void SetRealAt
		( const wchar_t * pwszTagPath,
			const wchar_t * pwszAttr, double rValue ) ;
	// �o�C�i���f�[�^�ݒ�
	void SetBinaryAt
		( const wchar_t * pwszTagPath, const wchar_t * pwszAttr,
			const void * ptrData, unsigned int nBytes ) ;

public:	// �����G���R�[�f�B���O
	// �����G���R�[�f�B���O��ނ𔻕�
	static CharacterEncoding GetCharaEncodingType( const char * pszType ) ;
	// �����G���R�[�f�B���O��ޕ�������擾
	static const char * GetCharaEncodingName( CharacterEncoding ceType ) ;
	// ������� Unicode �ɕ���
	static ESLError DecodeText
		( EWideString & wstrText,
			const EString & strSrc, CharacterEncoding ceEncoding ) ;
	// ������� Unicode ���畄����
	static ESLError EncodeText
		( EString & strText,
			const EWideString & wstrSrc, CharacterEncoding ceEncoding ) ;
	// �K�؂ȕ����R�[�h��I������
	static CharacterEncoding GetCharacterEncoding( const EString & strText ) ;

public:	// �f�[�^�G���R�[�f�B���O
	// C ����G�X�P�[�v�V�[�P���X�𕜌�
	static ESLError DecodeTextCEscSequence( EWideString & wstrText ) ;
	// C ����G�X�P�[�v�V�[�P���X�ɕϊ�
	static ESLError EncodeTextCEscSequence( EWideString & wstrText ) ;
	// ������R���e���c�̃f�R�[�h
	static ESLError DecodeTextContents( EWideString & wstrText ) ;
	// ������R���e���c�̃G���R�[�f�B���O
	static ESLError EncodeTextContents
		( EWideString & wstrText,
			const EWideString & wstrSrc, int nTabIndent = 0 ) ;
	// base64 ���畜��
	static ESLError DecodeBase64( EStreamBuffer & buf, const char * pszText ) ;
	// base64 �ɕ�����
	static ESLError EncodeBase64
		( EString & strText, const void * ptrBuf, unsigned int nBytes ) ;

} ;

#endif	//	!defined(COMPACT_NOA_DECODER)

#endif

