
/*****************************************************************************
                         E R I S A - L i b r a r y
													last update 2002/12/12
 -----------------------------------------------------------------------------
      Copyright (C) 2002 Leshade Entis, Entis-soft. All rights reserved.
 *****************************************************************************/


#if	!defined(__ERISA_CONTEXT_H__)
#define	__ERISA_CONTEXT_H__


/*****************************************************************************
                         �n�t�}���E�c���[�E�\����
 *****************************************************************************/

#define	ERINA_CODE_FLAG			0x80000000U
#define	ERINA_HUFFMAN_ESCAPE	0x7FFFFFFF
#define	ERINA_HUFFMAN_NULL		0x8000U
#define	ERINA_HUFFMAN_MAX		0x4000
#define	ERINA_HUFFMAN_ROOT		0x200

struct	ERINA_HUFFMAN_NODE
{
	WORD	m_weight ;
	WORD	m_parent ;
	DWORD	m_child_code ;
} ;

struct	ERINA_HUFFMAN_TREE
{
	ERINA_HUFFMAN_NODE	m_hnTree[0x201] ;
	int					m_iSymLookup[0x100] ;
	int					m_iEscape ;
	int					m_iTreePointer ;

	// �c���[�̏�����
	void Initialize( void ) ;
	// �����p�x���C���N�������g
	void IncreaseOccuedCount( int iEntry ) ;
	// �e�̏d�݂��Čv�Z����
	void RecountOccuredCount( int iParent ) ;
	// �c���[�̐��K��
	void Normalize( int iEntry ) ;
	// �V�����t���[�G���g�����쐬���Ēǉ�
	void AddNewEntry( int nNewCode ) ;
	// �e�o������2����1�ɂ��Ė؂��č\��
	void HalfAndRebuild( void ) ;

} ;


/*****************************************************************************
                             �Z�p�������v���f��
 *****************************************************************************/

#define	ERISA_TOTAL_LIMIT	0x2000		// �ꐔ�̌��E�l
#define	ERISA_SYMBOL_SORTS	0x101		// �V���{���̎��
#define	ERISA_SUB_SORT_MAX	0x80
#define	ERISA_PROB_SLOT_MAX	0x800		// ���v���f���̍ő�X���b�g��
#define	ERISA_ESC_CODE		(-1)		// �G�X�P�[�v�L��

struct	ERISA_CODE_SYMBOL
{
	WORD	wOccured ;					// �V���{���̏o����
	SWORD	wSymbol ;					// �V���{���i����8�r�b�g�̂݁j
} ;

struct	ERISA_PROB_MODEL
{
	DWORD				dwTotalCount ;			// �ꐔ < 2000H
	DWORD				dwSymbolSorts ;			// �V���{���̎�ސ�
	DWORD				dwReserved[2] ;
	ERISA_CODE_SYMBOL	acsSymTable[ERISA_SYMBOL_SORTS] ;
												// ���v���f��
	DWORD				dwReserved2[3] ;
	ERISA_CODE_SYMBOL	acsSubModel[ERISA_SUB_SORT_MAX] ;
												// �T�u���v���f���i�w�W�j

	// ���v���̏�����
	void Initialize( void ) ;
	// ���v�r�b�g�����v�Z����
	int AccumulateProb( SWORD wSymbol ) ;
	// �V���{���̐��N���� 1/2 �ɂ���
	void HalfOccuredCount( void ) ;
	// �w��̃V���{���̐��N�����C���N�������g����
	int IncreaseSymbol( int index ) ;
	// �w��̃V���{���̎w�W���擾����
	int FindSymbol( SWORD wSymbol ) ;
	// �w��̃V���{����ǉ�����
	int AddSymbol( SWORD wSymbol ) ;
} ;

struct	ERISA_PROB_BASE
{
	ERISA_PROB_MODEL *	ptrProbWork ;			// ���v�f�[�^�p���[�N
	DWORD				dwWorkUsed ;			// �g�p����Ă���X���b�g��
	DWORD				dwReserved[2] ;			// 16 �o�C�g�A���C��
	ERISA_PROB_MODEL	epmBaseModel ;			// �x�[�X���v���f��
	ERISA_PROB_MODEL *	ptrProbIndex[ERISA_PROB_SLOT_MAX] ;

	static const int	m_nShiftCount[4] ;
	static const int	m_nNewProbLimit[4] ;
} ;


/*****************************************************************************
                        �X���C�h�����Q�ƃe�[�u��
 *****************************************************************************/

#define	NEMESIS_BUF_SIZE	0x10000
#define	NEMESIS_BUF_MASK	0xFFFF
#define	NEMESIS_INDEX_LIMIT	0x100
#define	NEMESIS_INDEX_MASK	0xFF

struct	ERISAN_PHRASE_LOOKUP
{
	DWORD	first ;
	DWORD	index[NEMESIS_INDEX_LIMIT] ;
} ;


/*****************************************************************************
                             BSHF �����o�b�t�@
 *****************************************************************************/

class	ERIBshfBuffer	: public	ESLObject
{
public:
	EString	m_strPassword ;				// �p�X���[�h
	DWORD	m_dwPassOffset ;			// �p�X���[�h�I�t�Z�b�g
	BYTE	m_bufBSHF[32] ;				// BSHF �����p�o�b�t�@
	BYTE	m_srcBSHF[32] ;
	BYTE	m_maskBSHF[32] ;

public:
	// �\�z�֐�
	ERIBshfBuffer( void ) ;
	// ���Ŋ֐�
	virtual ~ERIBshfBuffer( void ) ;
	// �N���X���
	DECLARE_CLASS_INFO( ERIBshfBuffer, ESLObject )

public:
	// ������
	void EncodeBuffer( void ) ;
	// ����
	void DecodeBuffer( void ) ;

} ;


/*****************************************************************************
                        ERISA �����W�J�R���e�L�X�g
 *****************************************************************************/

class	ERISADecodeContext	: public	ESLObject
{
protected:
	// �r�b�g�X�g���[���o�b�t�@
	int		m_nIntBufCount ;	// ���ԓ��̓o�b�t�@�ɒ~�ς���Ă���r�b�g��
	DWORD	m_dwIntBuffer ;		// ���ԓ��̓o�b�t�@
	ULONG	m_nBufferingSize ;	// �o�b�t�@�����O����o�C�g��
	ULONG	m_nBufCount ;		// �o�b�t�@�̎c��o�C�g��
	PBYTE	m_ptrBuffer ;		// ���̓o�b�t�@�̐擪�ւ̃|�C���^
	PBYTE	m_ptrNextBuf ;		// ���ɓǂݍ��ނׂ����̓o�b�t�@�ւ̃|�C���^

	ESLFileObject *	m_pFile ;	// ���̓I�u�W�F�N�g
	ERISADecodeContext *	m_pContext ;

	// ���ݎg�p����Ă��镄���̓W�J�֐�
	ULONG (ERISADecodeContext::*m_pfnDecodeSymbolBytes)
						( SBYTE * ptrDst, ULONG nCount ) ;

	// ���������O�X�K���}�����R���e�L�X�g
	int		m_flgZero ;			// �[���t���O
	ULONG	m_nLength ;			// ���������O�X

	// ERINA�i�n�t�}���j�����R���e�L�X�g
	DWORD					m_dwERINAFlags ;
	ERINA_HUFFMAN_TREE *	m_pLastHuffmanTree ;
	ERINA_HUFFMAN_TREE **	m_ppHuffmanTree ;

	// ERISA�i�Z�p�j�����R���e�L�X�g
	DWORD	m_dwCodeRegister ;			// �R�[�h���W�X�^�i16 bit�j
	DWORD	m_dwAugendRegister ;		// �I�[�W�F���h���W�X�^�i16 bit�j
	int		m_nPostBitCount ;			// �I�[�r�b�g�o�b�t�@�J�E���^
	BYTE	m_bytLastSymbol[4] ;		// �ŋ߂̐��N�V���{��
	int		m_iLastSymbol ;
	ERISA_PROB_BASE *	m_pProbERISA ;
	// ERISA-N �����R���e�L�X�g
	int		m_nNemesisLeft ;			// �X���C�h���������J�E���^
	int		m_nNemesisNext ;
	BYTE *	m_pNemesisBuf ;				// �X���C�h�����p�o�b�t�@
	int		m_nNemesisIndex ;
	ERISAN_PHRASE_LOOKUP *	m_pNemesisLookup ;
	ERISA_PROB_MODEL *	m_pPhraseLenProb ;
	ERISA_PROB_MODEL *	m_pPhraseIndexProb ;
	ERISA_PROB_MODEL *	m_pRunLenProb ;
	ERISA_PROB_MODEL *	m_pLastERISAProb ;
	ERISA_PROB_MODEL **	m_ppTableERISA ;
	int		m_nFlagEOF ;

	// BSHF �����R���e�L�X�g
	ERIBshfBuffer *	m_pBshfBuf ;
	DWORD			m_dwBufPos ;

public:
	// �\�z�֐�
	ERISADecodeContext( ULONG nBufferingSize ) ;
	// ���Ŋ֐�
	virtual ~ERISADecodeContext( void ) ;
	// �N���X���
	DECLARE_CLASS_INFO( ERISADecodeContext, ESLObject )

public:
	// ���̓t�@�C���I�u�W�F�N�g���֘A�t����
	void AttachInputFile( ESLFileObject * pfile ) ;
	// ���̓R���e�L�X�g�I�u�W�F�N�g���֘A�t����
	void AttachInputContext( ERISADecodeContext * pcontext ) ;
	// ���̃f�[�^��ǂݍ���
	virtual ULONG ReadNextData( PBYTE ptrBuffer, ULONG nBytes ) ;

public:
	// �o�b�t�@����̎��A���̃f�[�^��ǂݍ���
	ESLError PrefetchBuffer( void ) ;
	// 1�r�b�g�擾����i 0 ���́|1��Ԃ� �j
	int GetABit( void ) ;
	// n�r�b�g�擾����
	UINT GetNBits( int n ) ;
	// �o�b�t�@���t���b�V������
	void FlushBuffer( void ) ;
	// �f�[�^�𕜍�����
	ULONG DecodeSymbolBytes( SBYTE * ptrDst, ULONG nCount )
		{
			ESLAssert( m_pfnDecodeSymbolBytes != NULL ) ;
			return	(this->*m_pfnDecodeSymbolBytes)( ptrDst, nCount ) ;
		}

#if	!defined(COMPACT_NOA_DECODER)
public:
	// ���������O�X�K���}�����R���e�L�X�g������������
	void PrepareGammaCode( void ) ;
	// ���������O�X�K���}�����̃[���t���O��ǂݍ���
	void InitGammaContext( void ) ;
	// �K���}�R�[�h���擾����
	int GetGammaCode( void ) ;
	// ���������O�X�K���}�����𕜍�����
	ULONG DecodeGammaCodeBytes( SBYTE * ptrDst, ULONG nCount ) ;

public:
	// ���k����
	enum	ERINAEncodingFlag
	{
		efERINAOrder0	= 0x0000,
		efERINAOrder1	= 0x0001
	} ;
	// ERINA �����̕����̏���������
	void PrepareToDecodeERINACode( DWORD dwFlags = efERINAOrder1 ) ;
	// �n�t�}���������擾����
	int GetHuffmanCode( ERINA_HUFFMAN_TREE * tree ) ;
	// �����̃n�t�}���������擾����
	int GetLengthHuffman( ERINA_HUFFMAN_TREE * tree ) ;
	// ERINA �����𕜍�����
	ULONG DecodeERINACodeBytes( SBYTE * ptrDst, ULONG nCount ) ;

#endif	//	!defined(COMPACT_NOA_DECODER)

public:
	// ERISA �����̕����̏���������
	void PrepareToDecodeERISACode( void ) ;
	// ERISA-N �����̕����̏���������
	void PrepareToDecodeERISANCode( void ) ;
	// �Z�p�����̕����̏��������s��
	void InitializeERISACode( void ) ;
	// �w��̓��v���f�����g����1�̎Z�p�����𕜍�
	int DecodeERISACode( ERISA_PROB_MODEL * pModel ) ;
	int DecodeERISACodeIndex( ERISA_PROB_MODEL * pModel ) ;
	// ERISA �����𕜍�����
	ULONG DecodeERISACodeBytes( SBYTE * ptrDst, ULONG nCount ) ;
	ULONG DecodeERISACodeWords( SWORD * ptrDst, ULONG nCount ) ;
	// ERISA �����𕜍�����
	ULONG DecodeERISANCodeBytes( SBYTE * ptrDst, ULONG nCount ) ;
	// EOF �t���O���擾����
	int GetEOFFlag( void ) const
		{
			return	m_nFlagEOF ;
		}

public:
	// BSHF �����̕����̏���������
	void PrepareToDecodeBSHFCode( const char * pszPassword ) ;
	// BSHF �����𕜍�����
	ULONG DecodeBSHFCodeBytes( SBYTE * ptrDst, ULONG nCount ) ;

} ;


/*****************************************************************************
                        ERISA �������k�R���e�L�X�g
 *****************************************************************************/

 #if	!defined(COMPACT_NOA_DECODER)

class	ERISAEncodeContext	: public	ESLObject
{
protected:
	// �r�b�g�X�g���[���o�b�t�@
	int		m_nIntBufCount ;	// ���ԓ��̓o�b�t�@�ɒ~�ς���Ă���r�b�g��
	DWORD	m_dwIntBuffer ;		// ���ԓ��̓o�b�t�@
	ULONG	m_nBufferingSize ;	// �o�b�t�@�����O����o�C�g��
	ULONG	m_nBufCount ;		// �o�b�t�@�ɒ~�ς���Ă���o�C�g��
	PBYTE	m_ptrBuffer ;		// �o�̓o�b�t�@�̐擪�ւ̃|�C���^

	ESLFileObject *	m_pFile ;			// �o�̓I�u�W�F�N�g
	ERISAEncodeContext *	m_pContext ;

	// ���ݎg�p����Ă��镄���̕������֐�
	ULONG (ERISAEncodeContext::*m_pfnEncodeSymbolBytes)
						( const SBYTE * ptrSrc, ULONG nCount ) ;
	ESLError (ERISAEncodeContext::*m_pfnFinishEncoding)( void ) ;

	// ERINA�i�n�t�}���j�����R���e�L�X�g
	DWORD					m_dwERINAFlags ;
	ERINA_HUFFMAN_TREE *	m_pLastHuffmanTree ;
	ERINA_HUFFMAN_TREE **	m_ppHuffmanTree ;

	// ERISA�i�Z�p�j�����R���e�L�X�g
	DWORD	m_dwCodeRegister ;			// �R�[�h���W�X�^�i16 bit�j
	DWORD	m_dwAugendRegister ;		// �I�[�W�F���h���W�X�^�i16 bit�j
	DWORD	m_dwCodeBuffer ;			// �R�[�h���W�X�^�o�b�t�@
	SDWORD	m_dwBitBufferCount ;		// �o�͂��ꂽ'1'�̐���~�ς���
			// '0' �́A���� '0' ���o�͂��ꂽ���_�ŁA�����ɑ��o�����B
			// '1' �́A'0' ���o�͂����܂Œ~�ς����B
			// -1 �́A��̃r�b�g�� '' ���A
			// 0 �́A�r�b�g�� '0' ���A
			// 1 �́A�r�b�g�� '01' ���A
			// 2 �́A�r�b�g�� '011' ��\�����܂��B

	BYTE	m_bytLastSymbol[4] ;		// �ŋ߂̐��N�V���{��
	int		m_iLastSymbol ;
	ERISA_PROB_BASE *	m_pProbERISA ;	// ���v���f���p���[�N������
	// ERISA-N �����R���e�L�X�g
	DWORD	m_dwERISAFlags ;			// ���k����
	BYTE *	m_pNemesisBuf ;				// �X���C�h�����p�o�b�t�@
	int		m_nNemesisIndex ;
	ERISAN_PHRASE_LOOKUP *	m_pNemesisLookup ;
	ERISA_PROB_MODEL *	m_pPhraseLenProb ;
	ERISA_PROB_MODEL *	m_pPhraseIndexProb ;
	ERISA_PROB_MODEL *	m_pRunLenProb ;
	ERISA_PROB_MODEL *	m_pLastERISAProb ;
	ERISA_PROB_MODEL **	m_ppTableERISA ;

	// BSHF �����R���e�L�X�g
	ERIBshfBuffer *	m_pBshfBuf ;
	DWORD			m_dwBufPos ;

public:
	// �\�z�֐�
	ERISAEncodeContext( ULONG nBufferingSize ) ;
	// ���Ŋ֐�
	virtual ~ERISAEncodeContext( void ) ;
	// �N���X���
	DECLARE_CLASS_INFO( ERISAEncodeContext, ESLObject )

public:
	// �o�̓t�@�C���I�u�W�F�N�g���֘A�t����
	void AttachOutputFile( ESLFileObject * pfile ) ;
	// �o�̓R���e�L�X�g�I�u�W�F�N�g���֘A�t����
	void AttachOutputContext( ERISAEncodeContext * pcontext ) ;
	// �o�b�t�@�̓��e���o�͂���
	virtual ULONG WriteNextData( const BYTE * ptrBuffer, ULONG nBytes ) ;

public:
	// ���r�b�g�o�͂���
	ESLError OutNBits( DWORD dwData, int nBits ) ;
	// �o�b�t�@�̓��e���o�͂��ċ�ɂ���
	ESLError Flushout( void ) ;
	// �f�[�^�𕄍�������
	ULONG EncodeSymbolBytes( const SBYTE * ptrSrc, ULONG nCount )
		{
			ESLAssert( m_pfnEncodeSymbolBytes != NULL ) ;
			return	(this->*m_pfnEncodeSymbolBytes)( ptrSrc, nCount ) ;
		}
	// ���������I������
	ESLError FinishEncoding( void )
		{
			ESLAssert( m_pfnFinishEncoding != NULL ) ;
			return	(this->*m_pfnFinishEncoding)( ) ;
		}

public:
	// �K���}�����̕������̏���������
	void PrepareToEncodeGammaCode( void ) ;
	// �K���}�R�[�h�ɕ����������ۂ̃r�b�g�����v�Z
	static ULONG EstimateGammaCode( int num ) ;
	// ���������O�X�K���}�����ɕ����������ۂ̃T�C�Y�i�r�b�g���j���v�Z
	static ULONG EstimateGammaCodeBytes( const SBYTE * ptrSrc, ULONG nCount ) ;
	// �K���}�R�[�h���o�͂���
	ESLError OutGammaCode( int num ) ;
	// ���������O�X�K���}�����ɕ��������ďo�͂���
	ULONG EncodeGammaCodeBytes( const SBYTE * ptrSrc, ULONG nCount ) ;

public:
	// ���k����
	enum	ERINAEncodingFlag
	{
		efERINAOrder0	= 0x0000,
		efERINAOrder1	= 0x0001
	} ;
	// ERINA �����̕������̏���������
	void PrepareToEncodeERINACode( DWORD dwFlags = efERINAOrder1 ) ;
	// �n�t�}�������ŏo�͂���
	ESLError OutHuffmanCode( ERINA_HUFFMAN_TREE * tree, int num ) ;
	// �������n�t�}�������ŏo�͂���
	ESLError OutLengthHuffman( ERINA_HUFFMAN_TREE * tree, int length ) ;
	// ERINA �����ɕ��������ďo�͂���
	ULONG EncodeERINACodeBytes( const SBYTE * ptrSrc, ULONG nCount ) ;

public:
	// ���k����
	enum	ERISAEncodingFlag
	{
		efSimple		= 0x0000,
		efNemesis		= 0x0001,
		efRunLength		= 0x0002,
		efRLNemesis		= 0x0003
	} ;
	// ERISA �����̕������̏���������
	void PrepareToEncodeERISACode( void ) ;
	// ERISA-N �����̕������̏���������
	void PrepareToEncodeERISANCode( DWORD dwFlags = efNemesis ) ;
	// �w��̓��v���f�����g����1�̎Z�p�������o��
	int EncodeERISACodeSymbol
		( ERISA_PROB_MODEL * pModel, SWORD wSymbol ) ;
	int EncodeERISACodeIndex
		( ERISA_PROB_MODEL * pModel, int iSym, WORD wFs ) ;
	// ERISA �����ɕ��������ďo�͂���
	ULONG EncodeERISACodeBytes( const SBYTE * ptrSrc, ULONG nCount ) ;
	ULONG EncodeERISACodeWords( const SWORD * ptrSrc, ULONG nCount ) ;
	// ERISA-N �����ɕ��������ďo�͂���
	ULONG EncodeERISANCodeBytes( const SBYTE * ptrSrc, ULONG nCount ) ;
	// ERISA-N ������ EOF ���o�͂���
	void EncodeERISANCodeEOF( void ) ;
	// ERISA ��������������
	ESLError FinishERISACode( void ) ;

public:
	// BSHF �����̈Í����̏���������
	void PrepareToEncodeBSHFCode( const char * pszPassword ) ;
	// BSHF �����ɈÍ�������
	ULONG EncodeBSHFCodeBytes( const SBYTE * ptrSrc, ULONG nCount ) ;
	// BSHF ��������������
	ESLError FinishBSHFCode( void ) ;

} ;

#endif	// !defined(COMPACT_NOA_DECODER)

#endif
