
/*****************************************************************************
                         E R I S A - L i b r a r y
                                                      �ŏI�X�V 2003/01/28
 ----------------------------------------------------------------------------
    Copyright (C) 2000-2003 Leshade Entis, Entis-soft. All rights reserved.
 *****************************************************************************/


#if	!defined(__ERISA_MATRIX_H__)
#define	__ERISA_MATRIX_H__	1


/*****************************************************************************
                            DCT�s�񉉎Z�p�萔
 *****************************************************************************/

#define	MIN_DCT_DEGREE	2
#define	MAX_DCT_DEGREE	12


/*****************************************************************************
                              �����ۂߊ֐�
 *****************************************************************************/

extern	"C"
{
	// 32 �r�b�g���������_�� 32 �r�b�g�����ɕϊ�
	int eriRoundR32ToInt( REAL32 r ) ;
	// 64 �r�b�g���������_�� 64 �r�b�g�����ɕϊ�
	INT64 eriRoundR64ToLInt( REAL64 r ) ;
	// 32 �r�b�g���������_�z��� 16 �r�b�g�����z��ɕϊ�
	void eriRoundR32ToWordArray
		( SWORD * ptrDst, int nStep, const REAL32 * ptrSrc, int nCount ) ;
	// 8 �r�b�g�������������z��� 32 �r�b�g���������_�z��ɕϊ�
	void eriConvertArrayByteToR32
		( REAL32 * ptrDst, const BYTE * ptrSrc, int nCount ) ;
	// 8 �r�b�g�����L�萮���z��� 32 �r�b�g���������_�z��ɕϊ�
	void eriConvertArraySByteToR32
		( REAL32 * ptrDst, const SBYTE * ptrSrc, int nCount ) ;
	// 32 �r�b�g���������_�z��� 8 �r�b�g�����Ȃ������z��ɕϊ�
	void eriConvertArrayR32ToByte
		( BYTE * ptrDst, const REAL32 * ptrSrc, int nCount ) ;
	// 32 �r�b�g���������_�z��� 8 �r�b�g�������萮���z��ɕϊ�
	void eriConvertArrayR32ToSByte
		( SBYTE * ptrDst, const REAL32 * ptrSrc, int nCount ) ;
} ;


/*****************************************************************************
                             ��]�s��\����
 *****************************************************************************/

struct	ERI_SIN_COS
{
	REAL32	rSin ;
	REAL32	rCos ;
} ;


/*****************************************************************************
                             �s�񉉎Z�֐�
 *****************************************************************************/

extern	"C"
{
	// �s�񏉊���
	void eriInitializeMatrix( void ) ;

	// �X�J����Z
	void eriScalarMultiply
		( REAL32 * ptrDst, REAL32 rScalar, unsigned int nCount ) ;
	// �x�N�g����Z
	void eriVectorMultiply
		( REAL32 * ptrDst, const REAL32 * ptrSrc, unsigned int nCount ) ;

	// 2 �_��]�ϊ�
	void eriRevolve2x2
		( REAL32 * ptrBuf1, REAL32 * ptrBuf2,
			REAL32 rSin, REAL32 rCos,
			unsigned int nStep, unsigned int nCount ) ;

	// ���� DCT �ϊ�
	void eriFastDCT
		(
			REAL32 *		ptrDst,
			unsigned int	nDstInterval,
			REAL32 *		ptrSrc,
			REAL32 *		ptrWorkBuf,
			unsigned int	nDegreeDCT
		) ;
	// �����t DCT �ϊ�
	void eriFastIDCT
		(
			REAL32 *		ptrDst,
			REAL32 *		ptrSrc,
			unsigned int	nSrcInterval,
			REAL32 *		ptrWorkBuf,
			unsigned int	nDegreeDCT
		) ;

	// ��]�s��𐶐�����
	ERI_SIN_COS * eriCreateRevolveParameter( unsigned int nDegreeDCT ) ;

	// LOT �ϊ����O�s��
	void eriFastPLOT
		(
			REAL32 *		ptrSrc,
			unsigned int	nDegreeDCT
		) ;
	// LOT �ϊ�
	void eriFastLOT
		(
			REAL32 *		ptrDst,
			const REAL32 *	ptrSrc1,
			const REAL32 *	ptrSrc2,
			unsigned int	nDegreeDCT
		) ;

	// Givens ��]�s��
	void eriOddGivensMatrix
		(
			REAL32 *			ptrDst,
			const ERI_SIN_COS *	ptrRevolve,
			unsigned int		nDegreeDCT
		) ;
	// �t Givens ��]�s��
	void eriOddGivensInverseMatrix
		(
			REAL32 *			ptrSrc,
			const ERI_SIN_COS *	ptrRevolve,
			unsigned int		nDegreeDCT
		) ;

	// �t LOT �ϊ����O�s��
	void eriFastIPLOT
		(
			REAL32 *		ptrSrc,
			unsigned int	nDegreeDCT
		) ;
	// �t LOT �ϊ�
	void eriFastILOT
		(
			REAL32 *		ptrDst,
			const REAL32 *	ptrSrc1,
			const REAL32 *	ptrSrc2,
			unsigned int	nDegreeDCT
		) ;

	// ����2���� DCT �ϊ�
	void eriFastDCT8x8( REAL32 * ptrDst ) ;
	// ����2�����t DCT �ϊ�
	void eriFastIDCT8x8( REAL32 * ptrDst ) ;
	// ����2���� LOT �ϊ�
	void eriFastLOT8x8
		(
			REAL32 *		ptrDst,
			REAL32 *		ptrHorzCur,
			REAL32 *		ptrVertCur
		) ;
	// ����2�����t LOT �ϊ�
	void eriFastILOT8x8
		(
			REAL32 *		ptrDst,
			REAL32 *		ptrHorzCur,
			REAL32 *		ptrVertCur
		) ;

	// RGB-YUV �F��ԕϊ�
	void eriConvertRGBtoYUV
		( REAL32 * ptrBuf1, REAL32 * ptrBuf2,
			REAL32 * ptrBuf3, unsigned int nCount ) ;

} ;


#endif
