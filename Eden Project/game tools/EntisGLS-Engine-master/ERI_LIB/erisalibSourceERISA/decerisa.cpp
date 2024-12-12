
/*****************************************************************************
                         E R I S A - L i b r a r y
 -----------------------------------------------------------------------------
   Copyright (C) 2002-2003 Leshade Entis, Entis-soft. All rights reserved.
 *****************************************************************************/


#define	STRICT	1
#include <windows.h>
#include <xerisa.h>


/*****************************************************************************
                        ERISA �����W�J�R���e�L�X�g
 *****************************************************************************/

// �N���X���
//////////////////////////////////////////////////////////////////////////////
IMPLEMENT_CLASS_INFO( ERISADecodeContext, ESLObject )

// �\�z�֐�
//////////////////////////////////////////////////////////////////////////////
ERISADecodeContext::ERISADecodeContext( ULONG nBufferingSize )
{
	m_nIntBufCount = 0 ;
	m_nBufferingSize = (nBufferingSize + 0x03) & ~0x03 ;
	m_nBufCount = 0 ;
	m_ptrBuffer = (PBYTE) ::eslHeapAllocate( NULL, nBufferingSize, 0 ) ;
	//
	m_pFile = NULL ;
	m_pContext = NULL ;
	//
	m_pfnDecodeSymbolBytes = NULL ;
	//
	m_ppHuffmanTree = NULL ;
	m_pProbERISA = NULL ;
	m_pNemesisBuf = NULL ;
	m_pNemesisLookup = NULL ;
	m_pPhraseLenProb = NULL ;
	m_pPhraseIndexProb = NULL ;
	m_pRunLenProb = NULL ;
	m_ppTableERISA = NULL ;
	m_nFlagEOF = false ;
	m_pBshfBuf = NULL ;
}

// ���Ŋ֐�
//////////////////////////////////////////////////////////////////////////////
ERISADecodeContext::~ERISADecodeContext( void )
{
	::eslHeapFree( NULL, m_ptrBuffer ) ;
	if ( m_ppHuffmanTree != NULL )
		::eslHeapFree( NULL, m_ppHuffmanTree ) ;
	if ( m_pProbERISA != NULL )
		::eslHeapFree( NULL, m_pProbERISA ) ;
	if ( m_pNemesisBuf != NULL )
		::eslHeapFree( NULL, m_pNemesisBuf ) ;
	if ( m_pNemesisLookup != NULL )
		::eslHeapFree( NULL, m_pNemesisLookup ) ;
	if ( m_pPhraseLenProb != NULL )
		::eslHeapFree( NULL, m_pPhraseLenProb ) ;
	if ( m_pPhraseIndexProb != NULL )
		::eslHeapFree( NULL, m_pPhraseIndexProb ) ;
	if ( m_pRunLenProb != NULL )
		::eslHeapFree( NULL, m_pRunLenProb ) ;
	if ( m_ppTableERISA != NULL )
		::eslHeapFree( NULL, m_ppTableERISA ) ;
	delete	m_pBshfBuf ;
}

// ���̓t�@�C���I�u�W�F�N�g���֘A�t����
//////////////////////////////////////////////////////////////////////////////
void ERISADecodeContext::AttachInputFile( ESLFileObject * pfile )
{
	m_pFile = pfile ;
	m_pContext = NULL ;
}

// ���̓R���e�L�X�g�I�u�W�F�N�g���֘A�t����
//////////////////////////////////////////////////////////////////////////////
void ERISADecodeContext::AttachInputContext( ERISADecodeContext * pcontext )
{
	m_pFile = NULL ;
	m_pContext = pcontext ;
}

// ���̃f�[�^��ǂݍ���
//////////////////////////////////////////////////////////////////////////////
ULONG ERISADecodeContext::ReadNextData( PBYTE ptrBuffer, ULONG nBytes )
{
	if ( m_pFile != NULL )
	{
		return	m_pFile->Read( ptrBuffer, nBytes ) ;
	}
	else if ( m_pContext != NULL )
	{
		return	m_pContext->DecodeSymbolBytes( (SBYTE*) ptrBuffer, nBytes ) ;
	}
	else
	{
		ESLTrace( "�x���FReadNextData �֐����I�[�o�[���C�h����Ă��܂���B\n" ) ;
		return	0 ;
	}
}

// �o�b�t�@���t���b�V������
//////////////////////////////////////////////////////////////////////////////
void ERISADecodeContext::FlushBuffer( void )
{
	m_nIntBufCount = 0 ;
	m_nBufCount = 0 ;
}

#if	!defined(COMPACT_NOA_DECODER)

// ���������O�X�K���}�����R���e�L�X�g������������
//////////////////////////////////////////////////////////////////////////////
void ERISADecodeContext::PrepareGammaCode( void )
{
	m_pfnDecodeSymbolBytes = &ERISADecodeContext::DecodeGammaCodeBytes ;
}

// ���������O�X�K���}�����̃[���t���O��ǂݍ���
//////////////////////////////////////////////////////////////////////////////
void ERISADecodeContext::InitGammaContext( void )
{
	m_flgZero = GetABit( ) ;
	m_nLength = 0 ;
}

// ERINA �����̕����̏���������
//////////////////////////////////////////////////////////////////////////////
void ERISADecodeContext::PrepareToDecodeERINACode( DWORD dwFlags )
{
	//
	// ���������m��
	//
	int		i ;
	if ( m_ppHuffmanTree == NULL )
	{
		DWORD	dwSize =
			(sizeof(ERINA_HUFFMAN_TREE*)
				+ sizeof(ERINA_HUFFMAN_TREE)) * 0x101 ;
		dwSize = (dwSize + 0x0F) & ~0x0F ;
		m_ppHuffmanTree =
			(ERINA_HUFFMAN_TREE **) ::eslHeapAllocate( NULL, dwSize, 0 ) ;
		//
		PBYTE	ptrBuf = (PBYTE)(m_ppHuffmanTree + 0x101) ;
		for ( i = 0; i < 0x101; i ++ )
		{
			m_ppHuffmanTree[i] = (ERINA_HUFFMAN_TREE*) ptrBuf ;
			ptrBuf += sizeof(ERINA_HUFFMAN_TREE) ;
		}
	}
	//
	// �n�t�}���e�[�u����������
	//
	m_dwERINAFlags = dwFlags ;
	m_nLength = 0 ;
	if ( dwFlags == efERINAOrder0 )
	{
		m_ppHuffmanTree[0]->Initialize( ) ;
		m_ppHuffmanTree[0x100]->Initialize( ) ;
		for ( i = 1; i < 0x100; i ++ )
		{
			m_ppHuffmanTree[i] = m_ppHuffmanTree[0] ;
		}
	}
	else
	{
		for ( i = 0; i < 0x101; i ++ )
		{
			m_ppHuffmanTree[i]->Initialize( ) ;
		}
	}
	m_pLastHuffmanTree = m_ppHuffmanTree[0] ;
	//
	m_pfnDecodeSymbolBytes = &ERISADecodeContext::DecodeERINACodeBytes ;
}

#endif

// ERISA �����̕����̏���������
//////////////////////////////////////////////////////////////////////////////
void ERISADecodeContext::PrepareToDecodeERISACode( void )
{
	//
	// ���������m��
	//
	int		i ;
	if ( m_ppTableERISA == NULL )
	{
		DWORD	dwBytes =
			sizeof(ERISA_PROB_MODEL*) * 0x104
					+ sizeof(ERISA_PROB_MODEL) * 0x101 ;
		dwBytes = (dwBytes + 0x100F) & (~0xFFF) ;
		m_ppTableERISA =
			(ERISA_PROB_MODEL**) ::eslHeapAllocate( NULL, dwBytes, 0 ) ;
	}
	if ( m_pPhraseLenProb == NULL )
	{
		m_pPhraseLenProb = (ERISA_PROB_MODEL*)
			::eslHeapAllocate( NULL, sizeof(ERISA_PROB_MODEL), 0 ) ;
	}
	if ( m_pPhraseIndexProb == NULL )
	{
		m_pPhraseIndexProb = (ERISA_PROB_MODEL*)
			::eslHeapAllocate( NULL, sizeof(ERISA_PROB_MODEL), 0 ) ;
	}
	if ( m_pRunLenProb == NULL )
	{
		m_pRunLenProb = (ERISA_PROB_MODEL*)
			::eslHeapAllocate( NULL, sizeof(ERISA_PROB_MODEL), 0 ) ;
	}
	//
	// ���v���f����������
	//
	ERISA_PROB_MODEL *	pNextProb =
		(ERISA_PROB_MODEL*) (m_ppTableERISA + 0x104) ;
	m_pLastERISAProb = pNextProb ;
	for ( i = 0; i < 0x101; i ++ )
	{
		pNextProb->Initialize( ) ;
		m_ppTableERISA[i] = pNextProb ;
		pNextProb ++ ;
	}
	m_pPhraseLenProb->Initialize( ) ;
	m_pPhraseIndexProb->Initialize( ) ;
	m_pRunLenProb->Initialize( ) ;
	//
	// ���W�X�^��������
	//
	m_nLength = 0 ;
	m_dwCodeRegister = GetNBits( 32 ) ;
	m_dwAugendRegister = 0xFFFF ;
	m_nPostBitCount = 0 ;
	//
	m_pfnDecodeSymbolBytes = &ERISADecodeContext::DecodeERISACodeBytes ;
}

// ERISA �����̕����̏���������
//////////////////////////////////////////////////////////////////////////////
void ERISADecodeContext::PrepareToDecodeERISANCode( void )
{
	//
	// ���������m��
	//
	int		i ;
	if ( m_pProbERISA == NULL )
	{
		DWORD	dwBytes =
			sizeof(ERISA_PROB_BASE)
				+ sizeof(ERISA_PROB_MODEL) * ERISA_PROB_SLOT_MAX ;
		dwBytes = (dwBytes + 0x0F) & (~0x0F) ;
		m_pProbERISA =
			(ERISA_PROB_BASE*) ::eslHeapAllocate( NULL, dwBytes, 0 ) ;
	}
	//
	BYTE *	pbytNext = (BYTE*) m_pProbERISA ;
	pbytNext += sizeof(ERISA_PROB_BASE) ;
	//
	m_iLastSymbol = 0 ;
	for ( i = 0; i < 4; i ++ )
	{
		m_bytLastSymbol[i] = 0 ;
	}
	//
	// ���v���f����������
	//
	m_pProbERISA->ptrProbWork = (ERISA_PROB_MODEL*) pbytNext ;
	m_pProbERISA->dwWorkUsed = 0 ;
	m_pProbERISA->epmBaseModel.Initialize( ) ;
	//
	for ( i = 0; i < ERISA_PROB_SLOT_MAX; i ++ )
	{
		m_pProbERISA->ptrProbIndex[i] = m_pProbERISA->ptrProbWork + i ;
	}
	//
	// ���W�X�^��������
	//
	m_dwCodeRegister = GetNBits( 32 ) ;
	m_dwAugendRegister = 0xFFFF ;
	m_nPostBitCount = 0 ;
	//
	// �X���C�h�����Z�b�g�A�b�v
	//
	if ( m_pNemesisBuf == NULL )
	{
		m_pNemesisBuf =
			(BYTE*) ::eslHeapAllocate( NULL, NEMESIS_BUF_SIZE, 0 ) ;
	}
	if ( m_pNemesisLookup == NULL )
	{
		m_pNemesisLookup =
			(ERISAN_PHRASE_LOOKUP*) ::eslHeapAllocate
				( NULL, 0x100 * sizeof(ERISAN_PHRASE_LOOKUP), 0 ) ;
	}
	::eslFillMemory( m_pNemesisBuf, 0, NEMESIS_BUF_SIZE ) ;
	::eslFillMemory
		( m_pNemesisLookup, 0, 0x100 * sizeof(ERISAN_PHRASE_LOOKUP) ) ;
	m_nNemesisIndex = 0 ;
	//
	if ( m_pPhraseLenProb == NULL )
	{
		m_pPhraseLenProb = (ERISA_PROB_MODEL*)
			::eslHeapAllocate( NULL, sizeof(ERISA_PROB_MODEL), 0 ) ;
	}
	if ( m_pPhraseIndexProb == NULL )
	{
		m_pPhraseIndexProb = (ERISA_PROB_MODEL*)
			::eslHeapAllocate( NULL, sizeof(ERISA_PROB_MODEL), 0 ) ;
	}
	if ( m_pRunLenProb == NULL )
	{
		m_pRunLenProb = (ERISA_PROB_MODEL*)
			::eslHeapAllocate( NULL, sizeof(ERISA_PROB_MODEL), 0 ) ;
	}
	//
	m_pPhraseLenProb->Initialize( ) ;
	m_pPhraseIndexProb->Initialize( ) ;
	m_pRunLenProb->Initialize( ) ;
	//
	m_nNemesisLeft = 0 ;
	//
	m_nFlagEOF = false ;
	m_pfnDecodeSymbolBytes = &ERISADecodeContext::DecodeERISANCodeBytes ;
}

// �Z�p�����̕����̏��������s��
//////////////////////////////////////////////////////////////////////////////
void ERISADecodeContext::InitializeERISACode( void )
{
	m_nLength = 0 ;
	m_dwCodeRegister = GetNBits( 32 ) ;
	m_dwAugendRegister = 0xFFFF ;
	m_nPostBitCount = 0 ;
}

// ERISA �����𕜍�����
//////////////////////////////////////////////////////////////////////////////
ULONG ERISADecodeContext::DecodeERISACodeBytes( SBYTE * ptrDst, ULONG nCount )
{
	ERISA_PROB_MODEL *	pProb = m_pLastERISAProb ;
	int		nSymbol, iSym ;
	int		i = 0 ;
	while ( (ULONG) i < nCount )
	{
		if ( m_nLength > 0 )
		{
			//
			// �냉�������O�X
			//
			ULONG	nCurrent = nCount - i ;
			if ( nCurrent > m_nLength )
				nCurrent = m_nLength ;
			//
			m_nLength -= nCurrent ;
			for ( ULONG j = 0; j < nCurrent; j ++ )
			{
				ptrDst[i ++] = 0 ;
			}
			//
			continue ;
		}
		//
		// ���̎Z�p�����𕜍�
		//
		iSym = DecodeERISACodeIndex( pProb ) ;
		if ( iSym < 0 )
			break ;
		nSymbol = pProb->acsSymTable[iSym].wSymbol ;
		pProb->IncreaseSymbol( iSym ) ;
		ptrDst[i ++] = (SBYTE) nSymbol ;
		//
		if ( nSymbol == 0 )
		{
			//
			// �냌���O�X���擾
			//
			iSym = DecodeERISACodeIndex( m_pRunLenProb ) ;
			if ( iSym < 0 )
				break ;
			m_nLength = m_pRunLenProb->acsSymTable[iSym].wSymbol ;
			m_pRunLenProb->IncreaseSymbol( iSym ) ;
		}
		//
		pProb = m_ppTableERISA[(nSymbol & 0xFF)] ;
	}
	m_pLastERISAProb = pProb ;
	//
	return	i ;
}

ULONG ERISADecodeContext::DecodeERISACodeWords( SWORD * ptrDst, ULONG nCount )
{
	ERISA_PROB_MODEL *	pProb = m_pLastERISAProb ;
	int		nSymbol, iSym ;
	int		i = 0 ;
	while ( (ULONG) i < nCount )
	{
		if ( m_nLength > 0 )
		{
			//
			// �냉�������O�X
			//
			ULONG	nCurrent = nCount - i ;
			if ( nCurrent > m_nLength )
				nCurrent = m_nLength ;
			//
			m_nLength -= nCurrent ;
			for ( ULONG j = 0; j < nCurrent; j ++ )
			{
				ptrDst[i ++] = 0 ;
			}
			//
			continue ;
		}
		//
		// ���̎Z�p�����𕜍�
		//
		iSym = DecodeERISACodeIndex( pProb ) ;
		if ( iSym < 0 )
			break ;
		nSymbol = pProb->acsSymTable[iSym].wSymbol ;
		pProb->IncreaseSymbol( iSym ) ;
		//
		if ( nSymbol == ERISA_ESC_CODE )
		{
			iSym = DecodeERISACodeIndex( m_pPhraseIndexProb ) ;
			if ( iSym < 0 )
				break ;
			nSymbol = m_pPhraseIndexProb->acsSymTable[iSym].wSymbol ;
			m_pPhraseIndexProb->IncreaseSymbol( iSym ) ;
			//
			iSym = DecodeERISACodeIndex( m_pPhraseLenProb ) ;
			if ( iSym < 0 )
				break ;
			nSymbol = (nSymbol << 8)
				| (m_pPhraseLenProb->acsSymTable[iSym].wSymbol & 0xFF) ;
			m_pPhraseLenProb->IncreaseSymbol( iSym ) ;
			//
			ptrDst[i ++] = (SWORD) nSymbol ;
			pProb = m_ppTableERISA[0x100] ;
		}
		else
		{
			ptrDst[i ++] = (SWORD) (SBYTE) nSymbol ;
			pProb = m_ppTableERISA[(nSymbol & 0xFF)] ;
			//
			if ( nSymbol == 0 )
			{
				//
				// �냌���O�X���擾
				//
				iSym = DecodeERISACodeIndex( m_pRunLenProb ) ;
				if ( iSym < 0 )
					break ;
				m_nLength = m_pRunLenProb->acsSymTable[iSym].wSymbol ;
				m_pRunLenProb->IncreaseSymbol( iSym ) ;
			}
		}
	}
	m_pLastERISAProb = pProb ;
	//
	return	i ;
}

// ERISA-N �����𕜍�����
//////////////////////////////////////////////////////////////////////////////
ULONG ERISADecodeContext::DecodeERISANCodeBytes( SBYTE * ptrDst, ULONG nCount )
{
	//
	//	�w�肳�ꂽ���̃V���{���𕜍����I����܂ŌJ��Ԃ�
	//
	ULONG	nDecoded = 0 ;
	ERISA_PROB_BASE *	pBase = m_pProbERISA ;
	if ( m_nFlagEOF )
	{
		return	0 ;
	}
	//
	while ( nDecoded < nCount )
	{
		//
		// �X���C�h�������畜��
		//////////////////////////////////////////////////////////////////////
		if ( m_nNemesisLeft > 0 )
		{
			int		nNemesisCount = m_nNemesisLeft ;
			if ( (ULONG) nNemesisCount > nCount - nDecoded )
				nNemesisCount = nCount - nDecoded ;
			BYTE	bytLastSymbol =
				m_pNemesisBuf[(m_nNemesisIndex - 1) & NEMESIS_BUF_MASK] ;
			//
			for ( int i = 0; i < nNemesisCount; i ++ )
			{
				BYTE	bytSymbol = bytLastSymbol ;
				if ( m_nNemesisNext >= 0 )
				{
					bytSymbol = m_pNemesisBuf[m_nNemesisNext ++] ;
					m_nNemesisNext &= NEMESIS_BUF_MASK ;
				}
				//
				m_bytLastSymbol[m_iLastSymbol ++] = bytSymbol ;
				m_iLastSymbol &= 0x03 ;
				//
				ERISAN_PHRASE_LOOKUP *	ppl = &(m_pNemesisLookup[bytSymbol]) ;
				ppl->index[ppl->first] = m_nNemesisIndex ;
				ppl->first = (ppl->first + 1) & NEMESIS_INDEX_MASK ;
				bytLastSymbol = bytSymbol ;
				//
				m_pNemesisBuf[m_nNemesisIndex ++] = bytSymbol ;
				m_nNemesisIndex &= NEMESIS_BUF_MASK ;
				//
				*(ptrDst ++) = (SBYTE) bytSymbol ;
			}
			//
			m_nNemesisLeft -= nNemesisCount ;
			nDecoded += nNemesisCount ;
			continue ;
		}
		//
		//	�L���ȓ��v���f�����擾����
		//////////////////////////////////////////////////////////////////////
		int		iDeg ;
		ERISA_PROB_MODEL *	pModel ;
		pModel = &(pBase->epmBaseModel) ;
		for ( iDeg = 0; iDeg < 4; iDeg ++ )
		{
			int		iLast =
				m_bytLastSymbol[(m_iLastSymbol + 0x03 - iDeg) & 0x03]
							>> ERISA_PROB_BASE::m_nShiftCount[iDeg] ;
			if ( pModel->acsSubModel[iLast].wSymbol < 0 )
				break ;
			//
			ESLAssert( (DWORD) pModel->acsSubModel[iLast].wSymbol < pBase->dwWorkUsed ) ;
			pModel = pBase->ptrProbWork + pModel->acsSubModel[iLast].wSymbol ;
		}
		//
		//	�Z�p�����𕜍�
		//////////////////////////////////////////////////////////////////////
		//
		// �Z�p�����𕜍�
		//
		int		iSym = DecodeERISACodeIndex( pModel ) ;
		if ( iSym < 0 )
		{
			return	nDecoded ;
		}
		//
		// ���݂̓��v���f�����X�V
		//
		int		nSymbol = pModel->acsSymTable[iSym].wSymbol ;
		int		iSymIndex = pModel->IncreaseSymbol( iSym ) ;
		//
		bool	fNemesis = false ;
		if ( nSymbol == ERISA_ESC_CODE )
		{
			if ( pModel != &(pBase->epmBaseModel) )
			{
				iSym = DecodeERISACodeIndex( &(pBase->epmBaseModel) ) ;
				if ( iSym < 0 )
				{
					return	nDecoded ;
				}
				nSymbol = pBase->epmBaseModel.acsSymTable[iSym].wSymbol ;
				pBase->epmBaseModel.IncreaseSymbol( iSym ) ;
				if ( nSymbol != ERISA_ESC_CODE )
				{
					pModel->AddSymbol( (SWORD) nSymbol ) ;
					iSym = -1 ;
				}
				else
				{
					fNemesis = true ;
				}
			}
			else
			{
				fNemesis = true ;
			}
		}
		//
		if ( fNemesis )
		{
			//
			// �X���C�h�������g���ĕ���
			//
			int		nLength, nPhraseIndex ;
			nPhraseIndex = DecodeERISACode( m_pPhraseIndexProb ) ;
			if ( nPhraseIndex == ERISA_ESC_CODE )
			{
				m_nFlagEOF = true ;
				return	nDecoded ;
			}
			if ( nPhraseIndex == 0 )
			{
				nLength = DecodeERISACode( m_pRunLenProb ) ;
			}
			else
			{
				nLength = DecodeERISACode( m_pPhraseLenProb ) ;
			}
			if ( nLength == ERISA_ESC_CODE )
			{
				return	nDecoded ;
			}
			//
			BYTE	bytLastSymbol =
				m_pNemesisBuf[(m_nNemesisIndex - 1) & NEMESIS_BUF_MASK] ;
			ERISAN_PHRASE_LOOKUP *	ppl = &(m_pNemesisLookup[bytLastSymbol]) ;
			m_nNemesisLeft = nLength ;
			if ( nPhraseIndex == 0 )
			{
				m_nNemesisNext = -1 ;
			}
			else
			{
				m_nNemesisNext =
					ppl->index[(ppl->first - nPhraseIndex) & NEMESIS_INDEX_MASK] ;
				ESLAssert( m_pNemesisBuf[m_nNemesisNext] == bytLastSymbol ) ;
				m_nNemesisNext = (m_nNemesisNext + 1) & NEMESIS_BUF_MASK ;
			}
			//
			continue ;
		}
		//
		// �f�[�^���o��
		//////////////////////////////////////////////////////////////////////
		//
		// �������ꂽ�V���{�����o��
		//
		BYTE	bytSymbol = (BYTE) nSymbol ;
		m_bytLastSymbol[m_iLastSymbol ++] = bytSymbol ;
		m_iLastSymbol &= 0x03 ;
		//
		ERISAN_PHRASE_LOOKUP *	ppl = &(m_pNemesisLookup[bytSymbol]) ;
		ppl->index[ppl->first] = m_nNemesisIndex ;
		ppl->first = (ppl->first + 1) & NEMESIS_INDEX_MASK ;
		m_pNemesisBuf[m_nNemesisIndex ++] = bytSymbol ;
		m_nNemesisIndex &= NEMESIS_BUF_MASK ;
		//
		*(ptrDst ++) = (SBYTE) bytSymbol ;
		nDecoded ++ ;
		//
		// ���v���f���̃c���[���g��
		//
		if ( (pBase->dwWorkUsed < ERISA_PROB_SLOT_MAX) && (iDeg < 4) )
		{
			int		iSymbol =
				((BYTE) nSymbol) >> ERISA_PROB_BASE::m_nShiftCount[iDeg] ;
			ESLAssert( iSymbol < ERISA_SUB_SORT_MAX ) ;
			if ( ++ pModel->acsSubModel[iSymbol].wOccured
					>= ERISA_PROB_BASE::m_nNewProbLimit[iDeg] )
			{
				int		i ;
				ERISA_PROB_MODEL *	pParent = pModel ;
				pModel = &(pBase->epmBaseModel) ;
				for ( i = 0; i <= iDeg; i ++ )
				{
					iSymbol = m_bytLastSymbol
						[(m_iLastSymbol + 0x03 - i) & 0x03]
								>> ERISA_PROB_BASE::m_nShiftCount[i] ;
					if ( pModel->acsSubModel[iSymbol].wSymbol < 0 )
						break ;
					//
					ESLAssert
						( (DWORD) pModel->acsSubModel[iSymbol].
										wSymbol < pBase->dwWorkUsed ) ;
					pModel = pBase->ptrProbWork
								+ pModel->acsSubModel[iSymbol].wSymbol ;
				}
				if ( (i <= iDeg) &&
					(pModel->acsSubModel[iSymbol].wSymbol < 0) )
				{
					ERISA_PROB_MODEL *	pNew ;
					pNew = pBase->ptrProbWork + pBase->dwWorkUsed ;
					pModel->acsSubModel[iSymbol].
								wSymbol = (SWORD) (pBase->dwWorkUsed ++) ;
					//
					pNew->dwTotalCount = 0 ;
					int		j = 0 ;
					for ( i = 0; i < (int) pParent->dwSymbolSorts; i ++ )
					{
						WORD	wOccured =
							(pParent->acsSymTable[i].wOccured >> 4) ;
						if ( (wOccured > 0) &&
							(pParent->acsSymTable[i].wSymbol != ERISA_ESC_CODE) )
						{
							pNew->dwTotalCount += wOccured ;
							pNew->acsSymTable[j].wOccured = wOccured ;
							pNew->acsSymTable[j].wSymbol
								= pParent->acsSymTable[i].wSymbol ;
							j ++ ;
						}
					}
					pNew->dwTotalCount ++ ;
					pNew->acsSymTable[j].wOccured = 1 ;
					pNew->acsSymTable[j].wSymbol = (SWORD) ERISA_ESC_CODE ;
					pNew->dwSymbolSorts = ++ j ;
					//
					for ( i = 0; i < ERISA_SUB_SORT_MAX; i ++ )
					{
						pNew->acsSubModel[i].wOccured = 0 ;
						pNew->acsSubModel[i].wSymbol = (SWORD) -1 ;
					}
				}
			}
		}
	}
	return	nDecoded ;
}

// BSHF �����̕����̏���������
//////////////////////////////////////////////////////////////////////////////
void ERISADecodeContext::PrepareToDecodeBSHFCode( const char * pszPassword )
{
	if ( m_pBshfBuf == NULL )
	{
		m_pBshfBuf = new ERIBshfBuffer ;
	}
	if ( (pszPassword != NULL) && (pszPassword[0] != '\0') )
	{
		m_pBshfBuf->m_strPassword = pszPassword ;
	}
	else
	{
		m_pBshfBuf->m_strPassword = " " ;
	}
	int	nLength = m_pBshfBuf->m_strPassword.GetLength( ) ;
	if ( nLength < 32 )
	{
		m_pBshfBuf->m_strPassword += '\x1b' ;
		nLength = m_pBshfBuf->m_strPassword.GetLength( ) ;
		//
		for ( int i = nLength; i < 32; i ++ )
		{
			ESLAssert( i > 0 ) ;
			m_pBshfBuf->m_strPassword +=
				m_pBshfBuf->m_strPassword[i % nLength]
					+ m_pBshfBuf->m_strPassword[i - 1] ;
		}
	}
	m_pBshfBuf->m_dwPassOffset = 0 ;
	m_dwBufPos = 32 ;
	//
	m_pfnDecodeSymbolBytes = &ERISADecodeContext::DecodeBSHFCodeBytes ;
}

// BSHF �����𕜍�����
//////////////////////////////////////////////////////////////////////////////
ULONG ERISADecodeContext::DecodeBSHFCodeBytes( SBYTE * ptrDst, ULONG nCount )
{
	ULONG	nDecoded = 0 ;
	while ( nDecoded < nCount )
	{
		if ( m_dwBufPos >= 32 )
		{
			//
			// ���̃f�[�^��ǂݍ���
			//
			int		i ;
			for ( i = 0; i < 32; i ++ )
			{
				if ( m_nBufCount == 0 )
				{
					m_ptrNextBuf = m_ptrBuffer ;
					m_nBufCount =
						ReadNextData( m_ptrBuffer, m_nBufferingSize ) ;
					if ( m_nBufCount == 0 )
					{
						return	nDecoded ;
					}
				}
				m_pBshfBuf->m_srcBSHF[i] = *(m_ptrNextBuf ++) ;
				m_nBufCount -- ;
			}
			//
			// �Í��𕜍�
			//
			m_pBshfBuf->DecodeBuffer( ) ;
			//
			m_dwBufPos = 0 ;
		}
		//
		ptrDst[nDecoded ++] = m_pBshfBuf->m_bufBSHF[m_dwBufPos ++] ;
	}
	return	nDecoded ;
}

