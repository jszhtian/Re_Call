#include "vcl.h"
#include "SysUtils.hpp"

#include <wx/dir.h>

//----------------------------------------------------------------------------
PACKAGE AnsiString __fastcall ExtractFileDir(const AnsiString & FileName)
{
	char drive[MAX_PATH], dir[MAX_PATH];
	_splitpath( FileName.c_str(), drive, dir, NULL, NULL );

	// ExtractFileDir�́A�I�[ �� \ ���܂܂Ȃ��Bsplitpath �͊܂�ł��܂��炵���̂ŁA��菜��
	int n = strlen(dir);
	if ( dir[n - 1] == '\\' ) dir[n - 1] = '\0';
	return AnsiString(drive) + AnsiString(dir);
}

//----------------------------------------------------------------------------
PACKAGE AnsiString __fastcall IncludeTrailingBackslash(const AnsiString & FileName)
{
	int n = FileName.Length();

	if ( n == 0 )
		return AnsiString("\\");
	if ( FileName.c_str()[n - 1] != '\\' )
		return FileName + '\\';
	else
		return FileName;
}

//----------------------------------------------------------------------------
// �P�c�́����폜����
PACKAGE AnsiString __fastcall ExcludeTrailingBackslash(const AnsiString & FileName)
{
	int n = FileName.Length();

	if ( n == 0 )
		return FileName;
	if ( FileName.c_str()[n - 1] != '\\' )
		return FileName;

	return FileName.SubString(0, n-1);
}

//----------------------------------------------------------------------------
// �t�@�C�����̊g���q��ύX�����t�@�C������Ԃ�
// �������͊g���q�i�s���I�h���܂ށj
PACKAGE AnsiString __fastcall ChangeFileExt(const AnsiString & FileName, const AnsiString & Extension)
{
	AnsiString str = FileName;

	char *p = strrchr(str.c_str(), '.');
	if ( p != NULL ) *p = '\0';

	return str + Extension;
}
//----------------------------------------------------------------------------
//�t���p�X����t�@�C����������Ԃ�
PACKAGE AnsiString __fastcall ExtractFileName(const AnsiString & FileName)
{
	AnsiString str = FileName;

	char *p = strrchr(str.c_str(), '\\');
	if ( p != NULL )
		return *(p + 1);
	return FileName;
}
//----------------------------------------------------------------------------
//�t���p�X����p�X����Ԃ��B�Ō���� "\" ���܂�
PACKAGE AnsiString __fastcall ExtractFilePath(const AnsiString & FileName)
{
	AnsiString str = FileName;

	char *p = strrchr(str.c_str(), '\\');
	if ( p != NULL )
		*(p + 1) = '\0';
	return str;
}

//----------------------------------------------------------------------------
// UNC�`�������΃p�X�ɕϊ�����
AnsiString __fastcall ExpandUNCFileName(const AnsiString & FileName)
{
	// ������
	return "\\\\?\\" + FileName;
}

//----------------------------------------------------------------------------
//�w��̃f�B���N�g�������݂��邩�ǂ�����Ԃ�
bool DirectoryExists(const AnsiString& Name )
{
	return wxDir::Exists( Name.c_str() );
}
//----------------------------------------------------------------------------
//�w��̃t�@�C�������݂��邩�ǂ����𒲂ׂ�
bool FileExists(const AnsiString& Name)
{
	return wxFile::Exists( Name.c_str() );
}
//----------------------------------------------------------------------------
//�f�B�X�N����t�@�C�����폜����
bool DeleteFile(const AnsiString& Name)
{
	if ( remove( Name.c_str() ) == 0 )
		return true;
	else
		return false;
}
//----------------------------------------------------------------------------
// ������̐擪����і��������̃X�y�[�X�Ɛ��䕶�����폜����B�S�p�X�y�[�X�͍폜�ł��Ȃ�
AnsiString Trim(const AnsiString& str)
{
	const char * p = str.c_str();
	while( *p > '\0' && *p < 0x20 )
		p++;

	AnsiString _str(p);
	char * p0 = _str.c_str();
	char * p1 = _str.c_str() + _str.Length() - 1;
	while( p0 < p1 && *p1 >= '\0' && *p1 < 0x20 )
		*p1-- = '\0';

	return _str;
}

