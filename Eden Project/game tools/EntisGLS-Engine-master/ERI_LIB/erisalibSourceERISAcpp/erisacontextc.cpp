
/*****************************************************************************
                         E R I S A - L i b r a r y
 -----------------------------------------------------------------------------
    Copyright (C) 2002-2003 Leshade Entis, Entis-soft. All rights reserved.
 *****************************************************************************/


#define	STRICT	1
#include <windows.h>
#include <xerisa.h>


/*****************************************************************************
                         �n�t�}���E�c���[�E�\����
 *****************************************************************************/

//
// �����p�x���C���N�������g
//////////////////////////////////////////////////////////////////////////////
void ERINA_HUFFMAN_TREE::IncreaseOccuedCount( int iEntry )
{
	m_hnTree[iEntry].m_weight ++ ;
	Normalize( iEntry ) ;
	//
	if ( m_hnTree[ERINA_HUFFMAN_ROOT].m_weight >= ERINA_HUFFMAN_MAX )
	{
		HalfAndRebuild( ) ;
	}
}

//
// �c���[�̐��K��
//////////////////////////////////////////////////////////////////////////////
void ERINA_HUFFMAN_TREE::Normalize( int iEntry )
{
	while ( iEntry < ERINA_HUFFMAN_ROOT )
	{
		//
		// ����ւ���G���g��������
		int		iSwap = iEntry + 1 ;
		WORD	weight = m_hnTree[iEntry].m_weight ;
		while ( iSwap < ERINA_HUFFMAN_ROOT )
		{
			if ( m_hnTree[iSwap].m_weight >= weight )
				break ;
			++ iSwap ;
		}
		if ( iEntry == -- iSwap )
		{
			iEntry = m_hnTree[iEntry].m_parent ;
			RecountOccuredCount( iEntry ) ;
			continue ;
		}
		//
		// ����ւ�
		int		iChild, nCode ;
		if ( !(m_hnTree[iEntry].m_child_code & ERINA_CODE_FLAG) )
		{
			iChild = m_hnTree[iEntry].m_child_code ;
			m_hnTree[iChild].m_parent = iSwap ;
			m_hnTree[iChild + 1].m_parent = iSwap ;
		}
		else
		{
			nCode = m_hnTree[iEntry].m_child_code & ~ERINA_CODE_FLAG ;
			if ( nCode != ERINA_HUFFMAN_ESCAPE )
				m_iSymLookup[nCode & 0xFF] = iSwap ;
			else
				m_iEscape = iSwap ;
		}
		if ( !(m_hnTree[iSwap].m_child_code & ERINA_CODE_FLAG) )
		{
			int	iChild = m_hnTree[iSwap].m_child_code ;
			m_hnTree[iChild].m_parent = iEntry ;
			m_hnTree[iChild+1].m_parent = iEntry ;
		}
		else
		{
			int	nCode = m_hnTree[iSwap].m_child_code & ~ERINA_CODE_FLAG ;
			if ( nCode != ERINA_HUFFMAN_ESCAPE )
				m_iSymLookup[nCode & 0xFF] = iEntry ;
			else
				m_iEscape = iEntry ;
		}
		ERINA_HUFFMAN_NODE	node ;
		WORD	iEntryParent = m_hnTree[iEntry].m_parent ;
		WORD	iSwapParent = m_hnTree[iSwap].m_parent ;
		node = m_hnTree[iSwap] ;
		m_hnTree[iSwap] = m_hnTree[iEntry] ;
		m_hnTree[iEntry] = node ;
		m_hnTree[iSwap].m_parent = iSwapParent ;
		m_hnTree[iEntry].m_parent = iEntryParent ;
		//
		// �e�̏d�݂��Čv�Z����
		RecountOccuredCount( iSwapParent ) ;
		iEntry = iSwapParent ;
	}
}


/*****************************************************************************
                             �Z�p�������v���f��
 *****************************************************************************/

//
// ���v���̏�����
//////////////////////////////////////////////////////////////////////////////
void ERISA_PROB_MODEL::Initialize( void )
{
	dwTotalCount = ERISA_SYMBOL_SORTS ;
	dwSymbolSorts = ERISA_SYMBOL_SORTS ;
	//
	int	i ;
	for ( i = 0; i < 0x100; i ++ )
	{
		acsSymTable[i].wOccured = 1 ;
		acsSymTable[i].wSymbol = (SWORD) (BYTE) i ;
	}
	acsSymTable[0x100].wOccured = 1 ;
	acsSymTable[0x100].wSymbol = (SWORD) ERISA_ESC_CODE ;
	//
	for ( i = 0; i < ERISA_SUB_SORT_MAX; i ++ )
	{
		acsSubModel[i].wOccured = 0 ;
		acsSubModel[i].wSymbol = (SWORD) -1 ;
	}
}

//
// �w��̃V���{���̐��N�����C���N�������g����
//////////////////////////////////////////////////////////////////////////////
int ERISA_PROB_MODEL::IncreaseSymbol( int index )
{
	WORD	wOccured = ++ acsSymTable[index].wOccured ;
	SWORD	wSymbol = acsSymTable[index].wSymbol ;
	//
	while ( -- index >= 0 )
	{
		if ( acsSymTable[index].wOccured >= wOccured )
			break ;
		acsSymTable[index + 1] = acsSymTable[index] ;
	}
	acsSymTable[++ index].wOccured = wOccured ;
	acsSymTable[index].wSymbol = wSymbol ;
	//
	if ( ++ dwTotalCount >= ERISA_TOTAL_LIMIT )
	{
		HalfOccuredCount( ) ;
	}
	//
	return	index ;
}


/*****************************************************************************
                        ERISA �����W�J�R���e�L�X�g
 *****************************************************************************/

static const BYTE nGammaCodeLookup[0x200] =
{
   2,  2,   2,  2,   2,  2,   2,  2,   2,  2,   2,  2,   2,  2,   2,  2,
   2,  2,   2,  2,   2,  2,   2,  2,   2,  2,   2,  2,   2,  2,   2,  2,
   2,  2,   2,  2,   2,  2,   2,  2,   2,  2,   2,  2,   2,  2,   2,  2,
   2,  2,   2,  2,   2,  2,   2,  2,   2,  2,   2,  2,   2,  2,   2,  2,
   2,  2,   2,  2,   2,  2,   2,  2,   2,  2,   2,  2,   2,  2,   2,  2,
   2,  2,   2,  2,   2,  2,   2,  2,   2,  2,   2,  2,   2,  2,   2,  2,
   2,  2,   2,  2,   2,  2,   2,  2,   2,  2,   2,  2,   2,  2,   2,  2,
   2,  2,   2,  2,   2,  2,   2,  2,   2,  2,   2,  2,   2,  2,   2,  2,
   4,  4,   4,  4,   4,  4,   4,  4,   4,  4,   4,  4,   4,  4,   4,  4,
   4,  4,   4,  4,   4,  4,   4,  4,   4,  4,   4,  4,   4,  4,   4,  4,
   8,  6,   8,  6,   8,  6,   8,  6,  16,  8,  -1, -1,  17,  8,  -1, -1,
   9,  6,   9,  6,   9,  6,   9,  6,  18,  8,  -1, -1,  19,  8,  -1, -1,
   5,  4,   5,  4,   5,  4,   5,  4,   5,  4,   5,  4,   5,  4,   5,  4,
   5,  4,   5,  4,   5,  4,   5,  4,   5,  4,   5,  4,   5,  4,   5,  4,
  10,  6,  10,  6,  10,  6,  10,  6,  20,  8,  -1, -1,  21,  8,  -1, -1,
  11,  6,  11,  6,  11,  6,  11,  6,  22,  8,  -1, -1,  23,  8,  -1, -1,
   3,  2,   3,  2,   3,  2,   3,  2,   3,  2,   3,  2,   3,  2,   3,  2,
   3,  2,   3,  2,   3,  2,   3,  2,   3,  2,   3,  2,   3,  2,   3,  2,
   3,  2,   3,  2,   3,  2,   3,  2,   3,  2,   3,  2,   3,  2,   3,  2,
   3,  2,   3,  2,   3,  2,   3,  2,   3,  2,   3,  2,   3,  2,   3,  2,
   3,  2,   3,  2,   3,  2,   3,  2,   3,  2,   3,  2,   3,  2,   3,  2,
   3,  2,   3,  2,   3,  2,   3,  2,   3,  2,   3,  2,   3,  2,   3,  2,
   3,  2,   3,  2,   3,  2,   3,  2,   3,  2,   3,  2,   3,  2,   3,  2,
   3,  2,   3,  2,   3,  2,   3,  2,   3,  2,   3,  2,   3,  2,   3,  2,
   6,  4,   6,  4,   6,  4,   6,  4,   6,  4,   6,  4,   6,  4,   6,  4,
   6,  4,   6,  4,   6,  4,   6,  4,   6,  4,   6,  4,   6,  4,   6,  4,
  12,  6,  12,  6,  12,  6,  12,  6,  24,  8,  -1, -1,  25,  8,  -1, -1,
  13,  6,  13,  6,  13,  6,  13,  6,  26,  8,  -1, -1,  27,  8,  -1, -1,
   7,  4,   7,  4,   7,  4,   7,  4,   7,  4,   7,  4,   7,  4,   7,  4,
   7,  4,   7,  4,   7,  4,   7,  4,   7,  4,   7,  4,   7,  4,   7,  4,
  14,  6,  14,  6,  14,  6,  14,  6,  28,  8,  -1, -1,  29,  8,  -1, -1,
  15,  6,  15,  6,  15,  6,  15,  6,  30,  8,  -1, -1,  31,  8,  -1, -1
} ;

// �o�b�t�@����̎��A���̃f�[�^��ǂݍ���
//////////////////////////////////////////////////////////////////////////////
ESLError ERISADecodeContext::PrefetchBuffer( void )
{
	if ( m_nIntBufCount == 0 )
	{
		if ( m_nBufCount == 0 )
		{
			m_ptrNextBuf = m_ptrBuffer ;
			m_nBufCount = ReadNextData( m_ptrBuffer, m_nBufferingSize ) ;
			if ( m_nBufCount == 0 )
			{
				return	eslErrGeneral ;
			}
			if ( m_nBufCount & 0x03 )
			{
				unsigned int	i = m_nBufCount ;
				m_nBufCount += 4 - (m_nBufCount & 0x03) ;
				while ( i < m_nBufCount )
					m_ptrBuffer[i ++] = 0x00 ;
			}
		}
		m_nIntBufCount = 32 ;
#if	!defined(PROCESSOR_BIG_ENDIAN)
		m_dwIntBuffer =
			((DWORD)m_ptrNextBuf[0] << 24) | ((DWORD)m_ptrNextBuf[1] << 16)
				| ((DWORD)m_ptrNextBuf[2] << 8) | ((DWORD)m_ptrNextBuf[3]) ;
#else
		m_dwIntBuffer = *((DWORD*)m_ptrNextBuf) ;
#endif
		m_ptrNextBuf += 4 ;
		m_nBufCount -= 4 ;
	}
	return	eslErrSuccess ;
}

// 1�r�b�g�擾����i 0 ���́|1��Ԃ� �j
//	���G���[�����������ꍇ�� 1 ��Ԃ��B
//////////////////////////////////////////////////////////////////////////////
int ERISADecodeContext::GetABit( void )
{
	if ( PrefetchBuffer( ) )
	{
		return	1 ;
	}
	int	nValue = (int)(((SDWORD)m_dwIntBuffer) >> 31) ;
	-- m_nIntBufCount ;
	m_dwIntBuffer <<= 1 ;
	return	nValue ;
}

// n �r�b�g�擾����
//////////////////////////////////////////////////////////////////////////////
UINT ERISADecodeContext::GetNBits( int n )
{
	UINT	nCode = 0 ;
	while ( n != 0 )
	{
		if ( PrefetchBuffer( ) )
			break ;
		//
		int	nCopyBits = n ;
		if ( nCopyBits > m_nIntBufCount )
			nCopyBits = m_nIntBufCount ;
		//
		nCode = (nCode << nCopyBits) | (m_dwIntBuffer >> (32 - nCopyBits)) ;
		n -= nCopyBits ;
		m_nIntBufCount -= nCopyBits ;
		m_dwIntBuffer <<= nCopyBits ;
	}
	return	nCode ;
}

// �K���}�R�[�h���擾����
//	���G���[�����������ꍇ�� 0 ��Ԃ��B
//////////////////////////////////////////////////////////////////////////////
int ERISADecodeContext::GetGammaCode( void )
{
	//
	// �P�̔���
	//
	if ( PrefetchBuffer( ) )
	{
		return	0 ;
	}
	register DWORD	dwIntBuf ;
	m_nIntBufCount -- ;
	dwIntBuf = m_dwIntBuffer ;
	m_dwIntBuffer <<= 1 ;
	if ( !(dwIntBuf & 0x80000000) )
	{
		return	1 ;
	}
	//
	// �����I�[�̔���
	//
	if ( PrefetchBuffer( ) )
	{
		return	0 ;
	}
	if ( (~m_dwIntBuffer & 0x55000000) && (m_nIntBufCount >= 8) )
	{
		int		i = (m_dwIntBuffer >> 24) << 1 ;
		int		nCode = nGammaCodeLookup[i] ;
		int		nBitCount = nGammaCodeLookup[i + 1] ;
		ESLAssert( nBitCount <= m_nIntBufCount ) ;
		ESLAssert( nCode > 0 ) ;
		m_nIntBufCount -= nBitCount ;
		m_dwIntBuffer <<= nBitCount ;
		return	nCode ;
	}
	//
	// �ėp���[�`��
	//
	int		nCode = 0, nBase = 2 ;
	//
	for ( ; ; )
	{
		if ( m_nIntBufCount >= 2 )
		{
			//
			// 2 �r�b�g�ꊇ����
			//
			dwIntBuf = m_dwIntBuffer ;
			m_dwIntBuffer <<= 2 ;
			nCode = (nCode << 1) | (dwIntBuf >> 31) ;
			m_nIntBufCount -= 2 ;
			if ( !(dwIntBuf & 0x40000000) )
			{
				return	nCode + nBase ;
			}
			nBase <<= 1 ;
		}
		else
		{
			//
			// 1 �r�b�g���o��
			//
			if ( PrefetchBuffer( ) )
			{
				return	0 ;
			}
			nCode = (nCode << 1) | (m_dwIntBuffer >> 31) ;
			m_nIntBufCount -- ;
			m_dwIntBuffer <<= 1 ;
			//
			// �����I�[����
			//
			if ( PrefetchBuffer( ) )
			{
				return	0 ;
			}
			dwIntBuf = m_dwIntBuffer ;
			m_nIntBufCount -- ;
			m_dwIntBuffer <<= 1 ;
			if ( !(dwIntBuf & 0x80000000) )
			{
				return	nCode + nBase ;
			}
			nBase <<= 1 ;
		}
	}
}

// ���������O�X�K���}�����𕜍�����
//////////////////////////////////////////////////////////////////////////////
ULONG ERISADecodeContext::DecodeGammaCodeBytes( SBYTE * ptrDst, ULONG nCount )
{
	ULONG	nDecoded = 0, nRepeat ;
	SBYTE	nSign, nCode ;
	//
	if ( m_nLength == 0 )
	{
		m_nLength = (ULONG) GetGammaCode( ) ;
		if ( m_nLength == 0 )
		{
			return	nDecoded ;
		}
	}
	//
	for ( ; ; )
	{
		//
		// �o�̓V���{�������Z�o
		//
		nRepeat = m_nLength ;
		if ( nRepeat > nCount )
		{
			nRepeat = nCount ;
		}
		ESLAssert( nRepeat > 0 ) ;
		m_nLength -= nRepeat ;
		nCount -= nRepeat ;
		//
		// �V���{�����o��
		//
		if ( !m_flgZero )
		{
			nDecoded += nRepeat ;
			do
			{
				*(ptrDst ++) = 0 ;
			}
			while ( -- nRepeat ) ;
		}
		else
		{
			do
			{
				nSign = (SBYTE) GetABit( ) ;
				nCode = (SBYTE) GetGammaCode( ) ;
				if ( nCode == 0 )
				{
					return	nDecoded ;
				}
				nDecoded ++ ;
				*(ptrDst ++) = (nCode ^ nSign) - nSign ;
			}
			while ( -- nRepeat ) ;
		}
		//
		// �I�����H
		//
		if ( nCount == 0 )
		{
			if ( m_nLength == 0 )
			{
				m_flgZero = ~m_flgZero ;
			}
			return	nDecoded ;
		}
		//
		// �����O�X�R�[�h���擾
		//
		m_flgZero = ~m_flgZero ;
		m_nLength = (ULONG) GetGammaCode( ) ;
		if ( m_nLength == 0 )
		{
			return	nDecoded ;
		}
	}
}

// �n�t�}���������擾����
//////////////////////////////////////////////////////////////////////////////
int ERISADecodeContext::GetHuffmanCode( ERINA_HUFFMAN_TREE * tree )
{
	int		nCode ;
	if ( tree->m_iEscape != ERINA_HUFFMAN_NULL )
	{
		int		iEntry = ERINA_HUFFMAN_ROOT ;
		int		iChild = tree->m_hnTree[ERINA_HUFFMAN_ROOT].m_child_code ;
		//
		// �����𕜍�
		//
		do
		{
			if ( PrefetchBuffer() )
			{
				return	ERINA_HUFFMAN_ESCAPE ;
			}
			//
			// 1�r�b�g���o��
			//
			iEntry = iChild + (m_dwIntBuffer >> 31) ;
			-- m_nIntBufCount ;
			iChild = tree->m_hnTree[iEntry].m_child_code ;
			m_dwIntBuffer <<= 1 ;
		}
		while ( !(iChild & ERINA_CODE_FLAG) ) ;
		//
		// �����̏o���p�x�����Z
		//
		if ( (m_dwERINAFlags != efERINAOrder0) ||
			(tree->m_hnTree[ERINA_HUFFMAN_ROOT].
							m_weight < ERINA_HUFFMAN_MAX-1) )
		{
			tree->IncreaseOccuedCount( iEntry ) ;
		}
		//
		// �G�X�P�[�v�R�[�h������
		//
		nCode = iChild & ~ERINA_CODE_FLAG ;
		if ( nCode != ERINA_HUFFMAN_ESCAPE )
		{
			return	nCode ;
		}
	}
	//
	// �G�X�P�[�v�R�[�h�̂Ƃ���8�r�b�g�Œ蒷
	//
	nCode = GetNBits( 8 ) ;
	tree->AddNewEntry( nCode ) ;
	//
	return	nCode ;
}

// �����̃n�t�}���������擾����
//////////////////////////////////////////////////////////////////////////////
int ERISADecodeContext::GetLengthHuffman( ERINA_HUFFMAN_TREE * tree )
{
	int		nCode ;
	if ( tree->m_iEscape != ERINA_HUFFMAN_NULL )
	{
		int		iEntry = ERINA_HUFFMAN_ROOT ;
		int		iChild = tree->m_hnTree[ERINA_HUFFMAN_ROOT].m_child_code ;
		//
		// �����𕜍�
		//
		do
		{
			if ( PrefetchBuffer() )
			{
				return	ERINA_HUFFMAN_ESCAPE ;
			}
			//
			// 1�r�b�g���o��
			//
			iEntry = iChild + (m_dwIntBuffer >> 31) ;
			-- m_nIntBufCount ;
			iChild = tree->m_hnTree[iEntry].m_child_code ;
			m_dwIntBuffer <<= 1 ;
		}
		while ( !(iChild & ERINA_CODE_FLAG) ) ;
		//
		// �����̏o���p�x�����Z
		//
		if ( (m_dwERINAFlags != efERINAOrder0) ||
			(tree->m_hnTree[ERINA_HUFFMAN_ROOT].
							m_weight < ERINA_HUFFMAN_MAX-1) )
		{
			tree->IncreaseOccuedCount( iEntry ) ;
		}
		//
		// �G�X�P�[�v�R�[�h������
		//
		nCode = iChild & ~ERINA_CODE_FLAG ;
		if ( nCode != ERINA_HUFFMAN_ESCAPE )
		{
			return	nCode ;
		}
	}
	//
	// �G�X�P�[�v�R�[�h�̂Ƃ��̓K���}����
	nCode = GetGammaCode( ) ;
	if ( nCode == -1 )
	{
		return	ERINA_HUFFMAN_ESCAPE ;
	}
	tree->AddNewEntry( nCode ) ;
	//
	return	nCode ;
}

// ERINA �����𕜍�����
//////////////////////////////////////////////////////////////////////////////
ULONG ERISADecodeContext::DecodeERINACodeBytes( SBYTE * ptrDst, ULONG nCount )
{
	ERINA_HUFFMAN_TREE *	tree = m_pLastHuffmanTree ;
	int		symbol, length ;
	ULONG	i = 0 ;
	if ( m_nLength > 0 )
	{
		length = m_nLength ;
		if ( length > (int) nCount )
		{
			length = nCount ;
		}
		m_nLength -= length ;
		do
		{
			ptrDst[i ++] = 0 ;
		}
		while ( -- length ) ;
	}
	while ( i < nCount )
	{
		symbol = GetHuffmanCode( tree ) ;
		if ( symbol == ERINA_HUFFMAN_ESCAPE )
		{
			break ;
		}
		ptrDst[i ++] = (SBYTE) symbol ;
		//
		if ( symbol == 0 )
		{
			length = GetLengthHuffman( m_ppHuffmanTree[0x100] ) ;
			if ( length == ERINA_HUFFMAN_ESCAPE )
			{
				break ;
			}
			if ( -- length )
			{
				m_nLength = length ;
				if ( i + length > nCount )
				{
					length = nCount - i ;
				}
				m_nLength -= length ;
				if ( length > 0 )
				{
					do
					{
						ptrDst[i ++] = 0 ;
					}
					while ( -- length ) ;
				}
			}
		}
		tree = m_ppHuffmanTree[symbol & 0xFF] ;
	}
	m_pLastHuffmanTree = tree ;
	//
	return	i ;
}

// �w��̓��v���f�����g����1�̎Z�p�����𕜍�
//////////////////////////////////////////////////////////////////////////////
int ERISADecodeContext::DecodeERISACode( ERISA_PROB_MODEL * pModel )
{
	int		iSym = DecodeERISACodeIndex( pModel ) ;
	int		nSymbol = ERISA_ESC_CODE ;
	if ( iSym >= 0 )
	{
		nSymbol = pModel->acsSymTable[iSym].wSymbol ;
		pModel->IncreaseSymbol( iSym ) ;
	}
	return	nSymbol ;
}

int ERISADecodeContext::DecodeERISACodeIndex( ERISA_PROB_MODEL * pModel )
{
	//
	// �w�W�𕜍����Č���
	//
	DWORD	dwAcc =
		m_dwCodeRegister
			* pModel->dwTotalCount
			/ m_dwAugendRegister ;
	if ( dwAcc >= ERISA_TOTAL_LIMIT )
	{
		return	-1 ;			// �G���[
	}
	//
	int		iSym = 0 ;
	WORD	wAcc = (WORD) dwAcc ;
	WORD	wFs = 0 ;
	WORD	wOccured ;
	for ( ; ; )
	{
		wOccured = pModel->acsSymTable[iSym].wOccured ;
		if ( wAcc < wOccured )
		{
			break ;
		}
		wAcc -= wOccured ;
		wFs += wOccured ;
		if ( (DWORD) ++ iSym >= pModel->dwSymbolSorts )
		{
			return	-1 ;		// �G���[
		}
	}
	//
	// �R�[�h���W�X�^�ƃI�[�W�F���h���W�X�^���X�V
	//
	m_dwCodeRegister -=
		(m_dwAugendRegister * wFs
			+ pModel->dwTotalCount - 1) / pModel->dwTotalCount ;
	m_dwAugendRegister =
		m_dwAugendRegister * wOccured / pModel->dwTotalCount ;
	ESLAssert( m_dwAugendRegister != 0 ) ;
	//
	// �I�[�W�F���g���W�X�^�𐳋K�����A�R�[�h���W�X�^�ɕ�����ǂݍ���
	//
	while ( !(m_dwAugendRegister & 0x8000) )
	{
		//
		// �R�[�h���W�X�^�ɃV�t�g�C��
		//
		int	nNextBit = GetABit( ) ;
		if ( nNextBit == 1 )
		{
			if ( (++ m_nPostBitCount) >= 256 )
			{
				return	-1 ;		// �G���[
			}
			nNextBit = 0 ;
		}
		m_dwCodeRegister =
			(m_dwCodeRegister << 1) | (nNextBit & 0x01) ;
		//
		m_dwAugendRegister <<= 1 ;
	}
	ESLAssert( m_dwAugendRegister & 0x8000 ) ;
	m_dwCodeRegister &= 0xFFFF ;
	//
	return	iSym ;
}


