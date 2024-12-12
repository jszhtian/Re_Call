#include "vcl.h"
#include "Classes.hpp"

//enum { soFromBeginning, soFromCurrent, soFromEnd };

//----------------------------------------------------------------------------
// ���͂Ȃ��Ƃ��f�X�g���N�^
TStream::~TStream()
{
}

//----------------------------------------------------------------------------
void TStream::ReadBuffer(void * Buffer, int Count )
{
	Read(Buffer, Count);
}

//----------------------------------------------------------------------------
// �w��̃o�C�g�����o�b�t�@����X�g���[���Ƀf�[�^����������
int TStream::WriteBuffer(const void* p, unsigned int len)
{
	return Write(p, len);
}



//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
TFileStream::TFileStream( const AnsiString& file, Word mode )
{
	// �ȉ��̃I�v�V�����Ƌ��ɂ̂ݗp�����Ă���̂ŁA�t���X�y�b�N�̎����͂��Ȃ�
	// enum { fmOpenRead=0, fmOpenWrite=1, fmOpenReadWrite=2, fmShareDenyWrite=32, fmCreate=0xFFFF };
	// enum { fmOpenRead=0, fmOpenReadWrite=2, fmShareDenyWrite=32, fmCreate=0xFFFF };
	// enum { fmOpenRead=0, fmShareDenyWrite=32, fmCreate=0xFFFF };
	if ( mode == fmOpenRead || mode == (fmOpenRead|fmShareDenyWrite) )
	{
		m_wxInputStream  = new wxFileInputStream( file.c_str() );
		m_wxOutputStream = NULL;
	}
	else if ( mode == (fmCreate)    || mode == (fmCreate|fmShareDenyWrite)   ||
	          mode == (fmOpenWrite) || mode == (fmOpenWrite|fmShareDenyWrite) )
	{
		m_wxInputStream  = NULL;
		m_wxOutputStream = new wxFileOutputStream( file.c_str() );
	}
	else if ( mode == (fmOpenReadWrite) || mode == (fmOpenReadWrite|fmShareDenyWrite) )
	{
		m_wxInputStream  = new wxFileInputStream( file.c_str() );
		m_wxOutputStream = new wxFileOutputStream( file.c_str() );
	}
	else
	{
		m_wxInputStream  = NULL;
		m_wxOutputStream = NULL;
	}
}

//----------------------------------------------------------------------------
TFileStream::~TFileStream()
{
	if (m_wxInputStream)  delete m_wxInputStream;
	if (m_wxOutputStream) delete m_wxOutputStream;
}

//----------------------------------------------------------------------------
int TFileStream::Read(void* buffer, unsigned int size)
{
	if (m_wxInputStream)
	{
		m_wxInputStream->Read(buffer, size);
		return m_wxInputStream->LastRead();
	}
	return -1;
}

//----------------------------------------------------------------------------
// �X�g���[���Ƀf�[�^�������o�����z���\�b�h
int TFileStream::Write(const void* buffer, unsigned int size)
{
	if (m_wxOutputStream)
	{
		m_wxOutputStream->Write(buffer, size);
		return m_wxOutputStream->LastWrite();
	}
	return -1;
}

//----------------------------------------------------------------------------
//	PROPERTY(int, getSize);
int TFileStream::getSize() const
{
	if (m_wxInputStream)
		return m_wxInputStream->GetSize();
	return -1;
}
void TFileStream::setSize(int x)
{
//	if (m_wxOutputStream)
//		m_wxOutputStream->SetLength(x);
}

//----------------------------------------------------------------------------
//	PROPERTY(int, getPosition);
int TFileStream::getPosition() const
{
//    virtual wxFileOffset SeekI(wxFileOffset pos, wxSeekMode mode = wxFromStart);
//    virtual wxFileOffset TellI() const;
	if (m_wxInputStream)
		return m_wxInputStream->TellI();
	return -1;
}

void TFileStream::setPosition(int pos)
{
	if (m_wxInputStream)
		m_wxInputStream->SeekI(pos);
}

//----------------------------------------------------------------------------
int TFileStream::CopyFrom(const TFileStream* file, unsigned int size)
{
	wxInputStream  * pis = file->m_wxInputStream;
	wxOutputStream * pos = m_wxOutputStream;
	if ( pis == NULL || pos == NULL )
		return -1;

	char buf[1024];
	int i = 0;
	while( i < size )
	{
		pis->Read( buf, std::min<int>(1024, size-i) );
		int lr = pis->LastRead();
		if ( lr > 0 )
		{
			pos->Write( buf, lr );
			i += lr;
		}
		else
			break;
	}
	return i;
}
