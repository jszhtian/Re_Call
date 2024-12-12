#pragma once
//----------------------------------------------------------------------------
class TStream
{
public:
	enum { soFromBeginning, soFromCurrent, soFromEnd };

	virtual ~TStream();

	virtual int Read(void*, unsigned int) = 0;
	virtual int Write(const void*, unsigned int) = 0;
	virtual void ReadBuffer(void * Buffer, int Count );
	virtual int WriteBuffer(const void*, unsigned int);

	PROPERTY_abstract(int, Size); // �����z
	PROPERTY_abstract(int, Position); // �����z
};


//----------------------------------------------------------------------------
class TFileStream : public TStream
{
private:
	wxInputStream  * m_wxInputStream;
	wxOutputStream * m_wxOutputStream;
public:
	TFileStream( const AnsiString&, Word mode );
	virtual ~TFileStream();

	virtual int Read(void*, unsigned int);
	virtual int Write(const void*, unsigned int);
	int CopyFrom(const TFileStream*, unsigned int);

	PROPERTY(int, Size); // ���z
	PROPERTY(int, Position); // ���z
};

