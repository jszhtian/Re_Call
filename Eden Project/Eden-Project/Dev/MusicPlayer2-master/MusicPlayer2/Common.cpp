﻿#include "stdafx.h"
#include "Common.h"
#include "resource.h"
#include "FilePathHelper.h"

CCommon::CCommon()
{
}


CCommon::~CCommon()
{
}


//void CCommon::GetAllFormatFiles(wstring path, vector<wstring>& files, const vector<wstring>& format, size_t max_file)
//{
//	//文件句柄 
//	int hFile = 0;
//	//文件信息（用Unicode保存使用_wfinddata_t，多字节字符集使用_finddata_t）
//	_wfinddata_t fileinfo;
//	wstring file_path;
//	for (auto a_format : format)
//	{
//		if ((hFile = _wfindfirst(file_path.assign(path).append(L"\\*.").append(a_format).c_str(), &fileinfo)) != -1)
//		{
//			do
//			{
//				if (files.size() >= max_file) break;
//				files.push_back(file_path.assign(fileinfo.name));  //将文件名保存
//			} while (_wfindnext(hFile, &fileinfo) == 0);
//		}
//		_findclose(hFile);
//	}
//	std::sort(files.begin(), files.end());		//对容器里的文件按名称排序
//}

bool CCommon::FileExist(const wstring & file)
{
	_wfinddata_t fileinfo;
	return (_wfindfirst(file.c_str(), &fileinfo) != -1);
}

bool CCommon::FolderExist(const wstring & file)
{
	DWORD dwAttrib = GetFileAttributes(file.c_str());
	return INVALID_FILE_ATTRIBUTES != dwAttrib && 0 != (dwAttrib & FILE_ATTRIBUTE_DIRECTORY);
}

bool CCommon::IsFolder(const wstring& path)
{
	DWORD dwAttrib = GetFileAttributes(path.c_str());
	return (dwAttrib & FILE_ATTRIBUTE_DIRECTORY) != 0;
}

void CCommon::DeleteStringBom(string & str)
{
    //去掉utf8的BOM
    if (str.size() >= 3 && str.substr(0, 3) == string{ '\xef', '\xbb', '\xbf' })
        str = str.substr(3);
    //去掉utf16的BOM
    if (str.size() >= 2 && str.substr(0, 2) == string{ '\xff', '\xfe' })
        str = str.substr(2);
}

//bool CCommon::FileIsMidi(const wstring & file_name)
//{
//	wstring type;
//	type = file_name.substr(file_name.size() - 4, 4);			//获取扩展名
//	std::transform(type.begin(), type.end(), type.begin(), tolower);		//将扩展名转换成小写
//	return (type == L".mid");
//}

void CCommon::StringCopy(char * dest, int size, string source)
{
	int source_size = source.size();
	for (int i{}; i < size && i<source_size; i++)
	{
		dest[i] = source[i];
	}
}

//bool CCommon::StringCompareNoCase(const wstring & str1, const wstring & str2)
//{
//	wstring _str1{ str1 }, _str2{ str2 };
//	StringTransform(_str1, false);
//	StringTransform(_str2, false);
//	return (_str1 == _str2);
//}

//size_t CCommon::StringFindNoCase(const wstring & str, const wstring & find_str)
//{
//	wstring _str{ str }, _find_str{ find_str };
//	StringTransform(_str, false);
//	StringTransform(_find_str, false);
//	return _str.find(_find_str);
//}

bool CCommon::IsDivideChar(wchar_t ch)
{
	if ((ch >= L'0' && ch <= L'9') || (ch >= L'a' && ch <= L'z') || (ch >= L'A' && ch <= L'Z') || ch > 255)
		return false;
	else
		return true;
}

bool CCommon::StrIsNumber(const wstring & str)
{
	for (const auto& ch : str)
	{
		if (ch < L'0' || ch > L'9')
			return false;
	}
	return true;
}

bool CCommon::CharIsNumber(wchar_t ch)
{
	return (ch >= L'0' && ch <= L'9');
}

void CCommon::StringSplit(const wstring & str, wchar_t div_ch, vector<wstring>& results, bool skip_empty)
{
	results.clear();
	size_t split_index = -1 ;
	size_t last_split_index = -1;
	while (true)
	{
		split_index = str.find(div_ch, split_index + 1);
		wstring split_str = str.substr(last_split_index + 1, split_index - last_split_index - 1);
		StringNormalize(split_str);
		if(!split_str.empty() || !skip_empty)
			results.push_back(split_str);
		if (split_index == wstring::npos)
			break;
		last_split_index = split_index;
	}
}

void CCommon::StringSplit(const wstring& str, const wstring& div_str, vector<wstring>& results, bool skip_empty /*= true*/)
{
    results.clear();
    size_t split_index = 0 - div_str.size();
    size_t last_split_index = 0 - div_str.size();
    while (true)
    {
        split_index = str.find(div_str, split_index + div_str.size());
        wstring split_str = str.substr(last_split_index + div_str.size(), split_index - last_split_index - div_str.size());
        StringNormalize(split_str);
        if (!split_str.empty() || !skip_empty)
            results.push_back(split_str);
        if (split_index == wstring::npos)
            break;
        last_split_index = split_index;
    }
}

void CCommon::StringSplitWithMulitChars(const wstring& str, const wchar_t* div_ch, vector<wstring>& results, bool skip_empty /*= true*/)
{
    results.clear();
    size_t split_index = -1;
    size_t last_split_index = -1;
    while (true)
    {
        split_index = str.find_first_of(div_ch, split_index + 1);
        wstring split_str = str.substr(last_split_index + 1, split_index - last_split_index - 1);
        StringNormalize(split_str);
        if (!split_str.empty() || !skip_empty)
            results.push_back(split_str);
        if (split_index == wstring::npos)
            break;
        last_split_index = split_index;
    }
}

wstring CCommon::StringMerge(const vector<wstring>& strings, wchar_t div_ch)
{
	wstring result;
	for (const auto& str : strings)
	{
		result.append(str).push_back(div_ch);
	}
	result.pop_back();
	return result;
}

wstring CCommon::TranslateToSimplifiedChinese(const wstring & str)
{
	wstring result;
	size_t size{ str.size() };
	if (size == 0) return wstring();
	wchar_t* out_buff = new wchar_t[size + 1];
	WORD wLanguageID = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
	LCID Locale = MAKELCID(wLanguageID, SORT_CHINESE_PRCP);
	int rtn = LCMapString(Locale, LCMAP_SIMPLIFIED_CHINESE, str.c_str(), -1, out_buff, size * sizeof(wchar_t));
	result.assign(out_buff);
	delete[] out_buff;
	return result;
}

wstring CCommon::TranslateToTranditionalChinese(const wstring & str)
{
	wstring result;
	size_t size{ str.size() };
	if (size == 0) return wstring();
	wchar_t* out_buff = new wchar_t[size + 1];
	WORD wLanguageID = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
	LCID Locale = MAKELCID(wLanguageID, SORT_CHINESE_PRCP);
	int rtn = LCMapString(Locale, LCMAP_TRADITIONAL_CHINESE, str.c_str(), -1, out_buff, size * sizeof(wchar_t));
	result.assign(out_buff);
	delete[] out_buff;
	return result;
}

void CCommon::FileNameNormalize(wstring & file_name)
{
	//查找字符串中的无效字符，并将其替换成‘_’
	const wstring invalid_chars{ L"\\\"/:*?<>|\a\b\f\n\r\t\v" };
	int index{ -1 };
	while (true)
	{
		index = file_name.find_first_of(invalid_chars, index + 1);
		if (index == wstring::npos)
			return;
		else
			file_name[index] = L'_';
	}
}

bool CCommon::IsFileNameValid(const wstring& file_name)
{
    const wstring invalid_chars{ L"\\\"/:*?<>|\a\b\f\n\r\t\v" };
    return (file_name.find_first_of(invalid_chars) == wstring::npos);
}

size_t CCommon::GetFileSize(const wstring & file_name)
{
	std::streampos l, m;
	ifstream file(file_name, std::ios::in | std::ios::binary);
	l = file.tellg();
	file.seekg(0, std::ios::end);
	m = file.tellg();
	file.close();
	return m - l;
}

wstring CCommon::StrToUnicode(const string & str, CodeType code_type)
{
	if (str.empty()) return wstring();

	if (code_type == CodeType::AUTO)
	{
		//code_type为AUTO时自动判断编码类型
		//如果前面有UTF8的BOM，则编码类型为UTF8
		if (str.size() >= 3 && str[0] == -17 && str[1] == -69 && str[2] == -65)
			code_type = CodeType::UTF8;
		//如果前面有UTF16的BOM，则编码类型为UTF16
		else if (str.size() >= 2 && str[0] == -1 && str[1] == -2)
			code_type = CodeType::UTF16;
		//else if (IsUTF8Bytes(str.c_str()))		//如果没有找到UTF8和UTF16的BOM，则判断字符串是否有UTF8编码的特性
		//	code_type = CodeType::UTF8_NO_BOM;
		else
			code_type = CodeType::ANSI;
	}

	wstring result;
	int size;
	if (code_type == CodeType::ANSI)
	{
		size = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
		if (size <= 0) return wstring();
		wchar_t* str_unicode = new wchar_t[size + 1];
		MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, str_unicode, size);
		result.assign(str_unicode);
		delete[] str_unicode;
	}
	else if (code_type == CodeType::UTF8 || code_type == CodeType::UTF8_NO_BOM)
	{
		string temp;
		//如果前面有BOM，则去掉BOM
		if (str.size() >= 3 && str[0] == -17 && str[1] == -69 && str[2] == -65)
			temp = str.substr(3);
		else
			temp = str;
		size = MultiByteToWideChar(CP_UTF8, 0, temp.c_str(), -1, NULL, 0);
		if (size <= 0) return wstring();
		wchar_t* str_unicode = new wchar_t[size + 1];
		MultiByteToWideChar(CP_UTF8, 0, temp.c_str(), -1, str_unicode, size);
		result.assign(str_unicode);
		delete[] str_unicode;
	}
	else if (code_type == CodeType::UTF16)
	{
		string temp;
		//如果前面有BOM，则去掉BOM
		if (str.size() >= 2 && str[0] == -1 && str[1] == -2)
			temp = str.substr(2);
		else
			temp = str;
		if (temp.size() % 2 == 1)
			temp.pop_back();
		temp.push_back('\0');
		result = (const wchar_t*)temp.c_str();
	}
	return result;
}

string CCommon::UnicodeToStr(const wstring & wstr, CodeType code_type, bool* char_cannot_convert)
{
	if (wstr.empty()) return string();
	if (char_cannot_convert != nullptr)
		*char_cannot_convert = false;
	BOOL UsedDefaultChar{ FALSE };
	string result;
	int size{ 0 };
	if (code_type == CodeType::ANSI)
	{
		size = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
		if (size <= 0) return string();
		char* str = new char[size + 1];
		WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, str, size, NULL, &UsedDefaultChar);
		result.assign(str);
		delete[] str;
	}
	else if (code_type == CodeType::UTF8 || code_type == CodeType::UTF8_NO_BOM)
	{
		result.clear();
		if (code_type == CodeType::UTF8)
		{
			result.push_back(-17);
			result.push_back(-69);
			result.push_back(-65);
		}
		size = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
		if (size <= 0) return string();
		char* str = new char[size + 1];
		WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, str, size, NULL, NULL);
		result.append(str);
		delete[] str;
	}
	else if (code_type == CodeType::UTF16)
	{
		result.clear();
		result.push_back(-1);	//在前面加上UTF16的BOM
		result.push_back(-2);
		result.append((const char*)wstr.c_str(), (const char*)wstr.c_str() + wstr.size() * 2);
		result.push_back('\0');
	}
	if (char_cannot_convert != nullptr)
		*char_cannot_convert = (UsedDefaultChar != FALSE);
	return result;
}

wstring CCommon::ASCIIToUnicode(const string & ascii)
{
	wstring result;
	for (const char& ch: ascii)
	{
		result.push_back(ch);
	}
	return result;
}

bool CCommon::IsUTF8Bytes(const char * data)
{
	int charByteCounter = 1;  //计算当前正分析的字符应还有的字节数
	unsigned char curByte; //当前分析的字节.
	bool ascii = true;
	int length = strlen(data);
	for (int i = 0; i < length; i++)
	{
		curByte = static_cast<unsigned char>(data[i]);
		if (charByteCounter == 1)
		{
			if (curByte >= 0x80)
			{
				ascii = false;
				//判断当前
				while (((curByte <<= 1) & 0x80) != 0)
				{
					charByteCounter++;
				}
				//标记位首位若为非0 则至少以2个1开始 如:110XXXXX...........1111110X 
				if (charByteCounter == 1 || charByteCounter > 6)
				{
					return false;
				}
			}
		}
		else
		{
			//若是UTF-8 此时第一位必须为1
			if ((curByte & 0xC0) != 0x80)
			{
				return false;
			}
			charByteCounter--;
		}
	}
	if (ascii) return false;		//如果全是ASCII字符，返回false
	else return true;
}

CodeType CCommon::JudgeCodeType(const string & str, CodeType default_code)
{
	//如果前面有UTF8的BOM，则编码类型为UTF8
	if (str.size() >= 3 && str[0] == -17 && str[1] == -69 && str[2] == -65)
		return CodeType::UTF8;
	//如果前面有UTF16的BOM，则编码类型为UTF16
	else if (str.size() >= 2 && str[0] == -1 && str[1] == -2)
		return CodeType::UTF16;
	//else if (IsUTF8Bytes(str.c_str()))		//如果没有找到UTF8和UTF16的BOM，则判断字符串是否有UTF8编码的特性
	//	return CodeType::UTF8_NO_BOM;
	else
		return default_code;
}

bool CCommon::IsURL(const wstring& str)
{
    return (str.substr(0, 7) == L"http://" || str.substr(0, 8) == L"https://" || str.substr(0, 6) == L"ftp://" || str.substr(0, 6) == L"mms://");
}

bool CCommon::StringCharacterReplace(wstring & str, wchar_t ch, wchar_t ch_replaced)
{
    bool replaced = false;
    for (size_t i = 0; i < str.size(); i++)
    {
        if (str[i] == ch)
        {
            str[i] = ch_replaced;
            replaced = true;
        }
    }
    return replaced;
}

wstring CCommon::GetExePath()
{
	wchar_t path[MAX_PATH];
	GetModuleFileNameW(NULL, path, MAX_PATH);
	size_t index;
	wstring current_path{ path };
	index = current_path.find_last_of(L'\\');
	current_path = current_path.substr(0, index + 1);
	return current_path;
}

wstring CCommon::GetDesktopPath()
{
	LPITEMIDLIST ppidl;
	TCHAR pszDesktopPath[MAX_PATH];
	if (SHGetSpecialFolderLocation(NULL, CSIDL_DESKTOP, &ppidl) == S_OK)
	{
		SHGetPathFromIDList(ppidl, pszDesktopPath);
		CoTaskMemFree(ppidl);
	}
	return wstring(pszDesktopPath);
}

wstring CCommon::GetTemplatePath()
{
	wstring result;
	wchar_t buff[MAX_PATH];
	GetTempPath(MAX_PATH, buff);		//获取临时文件夹的路径
	result = buff;
	if (!result.empty() && result.back() != L'\\' && result.back() != L'/')		//确保路径后面有斜杠
		result.push_back(L'\\');
	return result;
}

wstring CCommon::GetSpecialDir(int csidl)
{
	LPITEMIDLIST ppidl;
	TCHAR folder_dir[MAX_PATH];
	if (SHGetSpecialFolderLocation(NULL, csidl, &ppidl) == S_OK)
	{
		SHGetPathFromIDList(ppidl, folder_dir);
		CoTaskMemFree(ppidl);
	}
	return wstring(folder_dir);
}

//int CCommon::GetListWidth(CListBox & list)
//{
//	CDC *pDC = list.GetDC();
//	if (NULL == pDC)
//	{
//		return 0;
//	}
//	int nCount = list.GetCount();
//	if (nCount < 1)
//		return 0;
//	int nMaxExtent = 0;
//	CString szText;
//	for (int i = 0; i < nCount; ++i)
//	{
//		list.GetText(i, szText);
//		CSize &cs = pDC->GetTextExtent(szText);
//		if (cs.cx > nMaxExtent)
//		{
//			nMaxExtent = cs.cx;
//		}
//	}
//	return nMaxExtent;
//}


int CCommon::DeleteAFile(HWND hwnd, _tstring file)
{
	file.push_back(_T('\0'));	//pFrom必须以两个\0结尾
	LPCTSTR strTitle = CCommon::LoadText(IDS_DELETE);	//文件删除进度对话框标题
	SHFILEOPSTRUCT FileOp{};	//定义SHFILEOPSTRUCT结构对象
	FileOp.hwnd = hwnd;
	FileOp.wFunc = FO_DELETE;	//执行文件删除操作;
	FileOp.pFrom = file.c_str();
	FileOp.fFlags = FOF_ALLOWUNDO;	//此标志使删除文件备份到Windows回收站
	FileOp.hNameMappings = NULL;
	FileOp.lpszProgressTitle = strTitle;
	return SHFileOperation(&FileOp);	//删除文件
}

int CCommon::DeleteFiles(HWND hwnd, const vector<_tstring>& files)
{
	_tstring file_list;
	for (const auto& file : files)
	{
		file_list += file;
		file_list.push_back(_T('\0'));
	}
	return DeleteAFile(hwnd, file_list);
}

int CCommon::CopyAFile(HWND hwnd, _tstring file_from, _tstring file_to)
{
	file_from.push_back(_T('\0'));	//pFrom必须以两个\0结尾
	file_to.push_back(_T('\0'));	//pTo必须以两个\0结尾
	SHFILEOPSTRUCT FileOp{};
	FileOp.hwnd = hwnd;
	FileOp.wFunc = FO_COPY;
	FileOp.pFrom = file_from.c_str();
	FileOp.pTo = file_to.c_str();
	FileOp.fFlags = FOF_ALLOWUNDO;
	FileOp.hNameMappings = NULL;
	FileOp.lpszProgressTitle = LoadText(IDS_COPY);
	return SHFileOperation(&FileOp);
}

int CCommon::CopyFiles(HWND hwnd, const vector<_tstring>& files, _tstring file_to)
{
	_tstring file_list;
	for (const auto& file : files)
	{
		file_list += file;
		file_list.push_back(_T('\0'));
	}
	return CopyAFile(hwnd, file_list, file_to);
}

int CCommon::MoveAFile(HWND hwnd, _tstring file_from, _tstring file_to)
{
	file_from.push_back(_T('\0'));	//pFrom必须以两个\0结尾
	file_to.push_back(_T('\0'));	//pTo必须以两个\0结尾
	SHFILEOPSTRUCT FileOp{};
	FileOp.hwnd = hwnd;
	FileOp.wFunc = FO_MOVE;
	FileOp.pFrom = file_from.c_str();
	FileOp.pTo = file_to.c_str();
	FileOp.fFlags = FOF_ALLOWUNDO;
	FileOp.hNameMappings = NULL;
	FileOp.lpszProgressTitle = LoadText(IDS_MOVE);
	return SHFileOperation(&FileOp);
}

int CCommon::MoveFiles(HWND hwnd, const vector<_tstring>& files, _tstring file_to)
{
	_tstring file_list;
	for (const auto& file : files)
	{
		file_list += file;
		file_list.push_back(_T('\0'));
	}
	return MoveAFile(hwnd, file_list, file_to);
}

bool CCommon::CreateDir(const _tstring & path)
{
    if (!FolderExist(path))
    {
        if (CreateDirectory(path.c_str(), NULL))
        {
            return true;
        }
    }
    return false;
}

_tstring CCommon::FileRename(const _tstring& file_path, const _tstring& new_file_name)
{
    if (!FileExist(file_path))
        return _tstring();

    _tstring dir;
    size_t index = file_path.find_last_of(_T("/\\"));
    if (index == _tstring::npos)
        return _tstring();
    dir = file_path.substr(0, index + 1);

    _tstring extension;
    index = file_path.rfind(_T('.'));
    if (index != _tstring::npos && index < file_path.size() - 1)
        extension = file_path.substr(index);

    _tstring new_file_path = dir + new_file_name + extension;
    try
    {
        CFile::Rename(file_path.c_str(), new_file_path.c_str());
    }
    catch (CFileException* pEx)
    {
        new_file_path.clear();
        pEx->Delete();
    }
    return new_file_path;
}

bool CCommon::CopyStringToClipboard(const wstring & str)
{
	if (OpenClipboard(NULL))
	{
		HGLOBAL clipbuffer;
		EmptyClipboard();
		size_t size = (str.size() + 1) * 2;
		clipbuffer = GlobalAlloc(GMEM_DDESHARE, size);
		memcpy_s(GlobalLock(clipbuffer), size, str.c_str(), size);
		GlobalUnlock(clipbuffer);
		if(SetClipboardData(CF_UNICODETEXT, clipbuffer)==NULL)
			return false;
		CloseClipboard();
		return true;
	}
	else return false;
}

wstring CCommon::GetStringFromClipboard()
{
	if (OpenClipboard(NULL))
	{
		if (IsClipboardFormatAvailable(CF_TEXT))
		{
			HANDLE hClip;
			wchar_t *pBuf;
			hClip = GetClipboardData(CF_UNICODETEXT); //获取剪贴板数据
            if (hClip == NULL)
                return wstring();
			pBuf = (wchar_t*)GlobalLock(hClip);
            if (pBuf == nullptr)
                return wstring();
			CloseClipboard();
            return wstring(pBuf);
		}
	}
	return wstring();
}

void CCommon::WriteLog(const wchar_t* path, const wstring & content)
{
	SYSTEMTIME cur_time;
	GetLocalTime(&cur_time);
	char buff[32];
	sprintf_s(buff, "%d/%.2d/%.2d %.2d:%.2d:%.2d.%.3d ", cur_time.wYear, cur_time.wMonth, cur_time.wDay,
		cur_time.wHour, cur_time.wMinute, cur_time.wSecond, cur_time.wMilliseconds);
	ofstream out_put{ path, std::ios::app };
	out_put << buff << CCommon::UnicodeToStr(content, CodeType::UTF8_NO_BOM) << std::endl;
}

wstring CCommon::DisposeCmdLineFiles(const wstring & cmd_line, vector<wstring>& files)
{
	files.clear();
	if (cmd_line.empty()) return wstring();
	wstring path;
	//先找出字符串中的文件夹路径，从命令行参数传递过来的文件肯定都是同一个文件夹下的
	if (cmd_line[0] == L'\"')		//如果第一个文件用双引号包含起来
	{
		int index1 = cmd_line.find(L'\"', 1);		//查找和第1个双引号匹配的双引号
		int index2 = cmd_line.rfind(L'\\', index1);		//往前查找反斜杠
		path = cmd_line.substr(1, index2);		//获取文件夹路径（包含最后一个反斜杠）
		files.push_back(cmd_line.substr(1, index1 - 1));
	}
	else		//如果第一个文件没有用双引号包含起来，则说明路径中不包含空格，
	{
		int index1 = cmd_line.find(L' ');		//查找和第1空格
		int index2 = cmd_line.rfind(L'\\', index1);		//往前查找反斜杠
		path = cmd_line.substr(0, index2 + 1);		//获取文件夹路径（包含最后一个反斜杠）
		files.push_back(cmd_line.substr(0, index1));
	}
	int path_size = path.size();
	if (path_size < 2) return wstring();
	if (IsFolder(files[0]))
	//if (files[0].size() > 4 && files[0][files[0].size() - 4] != L'.' && files[0][files[0].size() - 5] != L'.')
	{
		//如果第1个文件不是文件而是文件夹，则返直接回该文件夹的路径
		return files[0];
	}
	int index{};
	while (true)
	{
		index = cmd_line.find(path, index + path_size);		//从第2个开始查找路径出现的位置
		if (index == string::npos) break;
		if (index > 0 && cmd_line[index - 1] == L'\"')		//如果路径前面一个字符是双引号
		{
			int index1 = cmd_line.find(L'\"', index);
			files.push_back(cmd_line.substr(index, index1 - index));
		}
		else
		{
			int index1 = cmd_line.find(L' ', index);
			files.push_back(cmd_line.substr(index, index1 - index));
		}
	}
	return wstring();
	//CString out_info;
	//out_info += _T("命令行参数：");
	//out_info += cmd_line.c_str();
	//out_info += _T("\r\n");
	//out_info += _T("路径：");
	//out_info += path.c_str();
	//out_info += _T("\r\n");
	//CCommon::WriteLog(L".\\command.log", wstring{ out_info });
}

bool CCommon::GetCmdLineCommand(const wstring & cmd_line, int & command)
{
    if (CCommon::StringNatchWholeWord(cmd_line, wstring(L"-play_pause")) != string::npos || CCommon::StringNatchWholeWord(cmd_line, wstring(L"-p")) != string::npos)
        command |= ControlCmd::PLAY_PAUSE;
    if (CCommon::StringNatchWholeWord(cmd_line, wstring(L"-previous")) != string::npos || CCommon::StringNatchWholeWord(cmd_line, wstring(L"-pre")) != string::npos)
        command |= ControlCmd::_PREVIOUS;
    if (CCommon::StringNatchWholeWord(cmd_line, wstring(L"-next")) != string::npos || CCommon::StringNatchWholeWord(cmd_line, wstring(L"-n")) != string::npos)
        command |= ControlCmd::_NEXT;
    if (CCommon::StringNatchWholeWord(cmd_line, wstring(L"-stop")) != string::npos || CCommon::StringNatchWholeWord(cmd_line, wstring(L"-s")) != string::npos)
        command |= ControlCmd::STOP;
    if (CCommon::StringNatchWholeWord(cmd_line, wstring(L"-ff")) != string::npos)
        command |= ControlCmd::FF;
    if (CCommon::StringNatchWholeWord(cmd_line, wstring(L"-rew")) != string::npos)
        command |= ControlCmd::REW;
    if (CCommon::StringNatchWholeWord(cmd_line, wstring(L"-vol_up")) != string::npos)
        command |= ControlCmd::VOLUME_UP;
    if (CCommon::StringNatchWholeWord(cmd_line, wstring(L"-vol_down")) != string::npos)
        command |= ControlCmd::VOLUME_DOWM;
    if (CCommon::StringNatchWholeWord(cmd_line, wstring(L"-mini")) != string::npos)
        command |= ControlCmd::MINI_MODE;
	return command!=ControlCmd::NONE;
}

bool CCommon::CreateFileShortcut(LPCTSTR lpszLnkFileDir, LPCTSTR lpszFileName, LPCTSTR lpszLnkFileName, LPCTSTR lpszWorkDir, WORD wHotkey, LPCTSTR lpszDescription, int iShowCmd, LPCTSTR lpszArguments, int nIconOffset)
{
	if (lpszLnkFileDir == NULL)
		return false;

	HRESULT hr;
	IShellLink     *pLink;  //IShellLink对象指针
	IPersistFile   *ppf; //IPersisFil对象指针

	//创建IShellLink对象
	hr = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (void**)&pLink);
	if (FAILED(hr))
		return false;

	//从IShellLink对象中获取IPersistFile接口
	hr = pLink->QueryInterface(IID_IPersistFile, (void**)&ppf);
	if (FAILED(hr))
	{
		pLink->Release();
		return false;
	}

	TCHAR file_path[MAX_PATH];		//当前进程exe文件路径
	GetModuleFileName(NULL, file_path, MAX_PATH);
	LPCTSTR pFilePath;				//快捷方式目标

	//目标
	if (lpszFileName == NULL)
		pFilePath = file_path;
	else
		pFilePath = lpszFileName;
	pLink->SetPath(pFilePath);

	//工作目录
	if (lpszWorkDir != NULL)
	{
		pLink->SetWorkingDirectory(lpszWorkDir);
	}
	else
	{
		//设置工作目录为快捷方式目标所在位置
		_tstring workDir = pFilePath;
		int index = workDir.find_last_of(_T("\\/"));
		if (index != workDir.npos)
			workDir = workDir.substr(0, index);
		pLink->SetWorkingDirectory(workDir.c_str());
	}

	//快捷键
	if (wHotkey != 0)
		pLink->SetHotkey(wHotkey);

	//备注
	if (lpszDescription != NULL)
		pLink->SetDescription(lpszDescription);

	//显示方式
	pLink->SetShowCmd(iShowCmd);

	//参数
	if (lpszArguments != NULL)
		pLink->SetArguments(lpszArguments);

	//图标
	if (nIconOffset > 0)
		pLink->SetIconLocation(pFilePath, nIconOffset);

	//快捷方式的路径 + 名称
	_tstring shortcutName;
	shortcutName = lpszLnkFileDir;
	if(!shortcutName.empty() && shortcutName.back()!=_T('\\') && shortcutName.back() != _T('/'))
		shortcutName.push_back(_T('\\'));

	if (lpszLnkFileName != NULL) //指定了快捷方式的名称
	{
		shortcutName += lpszLnkFileName;
	}
	else
	{
		//没有指定名称，就从取指定文件的文件名作为快捷方式名称。
		_tstring fileName = pFilePath;

		int index1, index2;
		index1 = fileName.find_last_of(_T("\\/"));
		index2 = fileName.rfind(_T('.'));

		if (index1 == _tstring::npos || index2 == _tstring::npos || index1 >= index2)
		{
			ppf->Release();
			pLink->Release();
			return false;
		}
		fileName = fileName.substr(index1 + 1, index2 - index1 - 1);
		fileName += _T(".lnk");
		shortcutName += fileName;
	}

	//保存快捷方式到指定目录下
	hr = ppf->Save(shortcutName.c_str(), TRUE);

	ppf->Release();
	pLink->Release();
	return SUCCEEDED(hr);
}

void CCommon::GetFiles(wstring file_name, vector<wstring>& files)
{
	files.clear();
	//文件句柄
	intptr_t hFile = 0;
	//文件信息（用Unicode保存使用_wfinddata_t，多字节字符集使用_finddata_t）
	_wfinddata_t fileinfo;
	if ((hFile = _wfindfirst(file_name.c_str(), &fileinfo)) != -1)
	{
		do
		{
			files.push_back(fileinfo.name);
		} while (_wfindnext(hFile, &fileinfo) == 0);
	}
	_findclose(hFile);
}

void CCommon::GetImageFiles(wstring file_name, vector<wstring>& files)
{
	files.clear();
	//文件句柄
	intptr_t hFile = 0;
	//文件信息（用Unicode保存使用_wfinddata_t，多字节字符集使用_finddata_t）
	_wfinddata_t fileinfo;
	if ((hFile = _wfindfirst(file_name.c_str(), &fileinfo)) != -1)
	{
		do
		{
			if (FileIsImage(fileinfo.name))
				files.push_back(fileinfo.name);
		} while (_wfindnext(hFile, &fileinfo) == 0);
	}
	_findclose(hFile);
}

bool CCommon::IsFolderMatchKeyWord(wstring dir, const wstring& key_word)
{
    

    //文件句柄
    intptr_t hFile = 0;
    //文件信息
    _wfinddata_t fileinfo;
    if (dir.back() != '\\' && dir.back() != '/')
        dir.push_back('\\');

    wstring folder_name;
    size_t index = dir.rfind(L'\\', dir.size() - 2);
    folder_name = dir.substr(index + 1, dir.size() - index - 2);

    if ((hFile = _wfindfirst((dir + L"*").c_str(), &fileinfo)) != -1)
    {
        do
        {
            wstring file_name = fileinfo.name;
            if (file_name == L"." || file_name == L"..")
                continue;

            if (CCommon::IsFolder(dir + file_name))        //当前是文件夹，则递归调用
            {
                if (IsFolderMatchKeyWord(dir + file_name, key_word))
                    return true;
            }
            //else
            //{
            //    if (StringFindNoCase(file_name, key_word) != wstring::npos)
            //        return true;
            //}
        } while (_wfindnext(hFile, &fileinfo) == 0);

        if (StringFindNoCase(folder_name, key_word) != wstring::npos)
            return true;
    }
    _findclose(hFile);
    return false;
}

bool CCommon::FileIsImage(const wstring & file_name)
{
	size_t index;
	index = file_name.find_last_of(L'.');
	wstring type;
	if (index != string::npos)
		type = file_name.substr(index);			//获取扩展名
	std::transform(type.begin(), type.end(), type.begin(), tolower);		//将扩展名转换成小写
	return (type == L".jpg" || type == L".jpeg" || type == L".png" || type == L".gif" || type == L".bmp");
}

wstring CCommon::GetRandomString(int length)
{
	wstring result;
	SYSTEMTIME current_time;
	GetLocalTime(&current_time);			//获取当前时间
	srand(current_time.wMilliseconds);		//用当前时间的毫秒数设置产生随机数的种子
	int char_type;		//当前要生成的字符类型 0：数字；1：小写字母；2：大写字母
	for (int i{}; i < length; i++)
	{
		char_type = rand() % 3;		//随机确定要生成的字符类型
		wchar_t current_char;
		switch (char_type)
		{
		case 0:
			current_char = L'0' + (rand() % 10);
			break;
		case 1:
			current_char = L'a' + (rand() % 26);
			break;
		case 2:
			current_char = L'A' + (rand() % 26);
			break;
		}
		result.push_back(current_char);
	}
	return result;
}

bool CCommon::IsFileNameNumbered(const wstring & file_name, int& number, size_t& index)
{
	if (file_name.empty())
		return false;
	if (file_name.back() != L')')
		return false;
	size_t size{ file_name.size() };
	//size_t index;
	index = file_name.rfind(L'(');		//往前查找右括号
	if (index == wstring::npos || index == 0)		//左括号不能在字符串开头
		return false;
	wstring number_str{ file_name.substr(index + 1, size - index - 2) };		//获取两个括号之间的文本
	if (StrIsNumber(number_str))				//判断文本是否是数字
	{
		number = _wtoi(number_str.c_str());
		return true;
	}
	else
	{
		return false;
	}
}

void CCommon::SizeZoom(CSize & size, int side)
{
	float width_height_ratio{ static_cast<float>(size.cx) / size.cy };	//长宽比
	if (width_height_ratio > 1)		//长宽比大于1：1
	{
		size.cx = side;
		size.cy = static_cast<int>(side / width_height_ratio);
	}
	else if (width_height_ratio < 1)
	{
		size.cy = side;
		size.cx = static_cast<int>(side * width_height_ratio);
	}
	else
	{
		size.cx = size.cy = side;
	}
}

COLORREF CCommon::GetWindowsThemeColor()
{
#ifdef COMPILE_IN_WIN_XP
	return RGB(0, 120, 215);
#else
	DWORD crColorization;
	BOOL fOpaqueBlend;
	COLORREF theme_color{};
	HRESULT result = DwmGetColorizationColor(&crColorization, &fOpaqueBlend);
	if (result == S_OK)
	{
		BYTE r, g, b;
		r = (crColorization >> 16) % 256;
		g = (crColorization >> 8) % 256;
		b = crColorization % 256;
		theme_color = RGB(r, g, b);
	}
	return theme_color;
#endif
}


int CCommon::AppendMenuOp(HMENU hDst, HMENU hSrc)
{
	int iCnt = 0;
	ASSERT(hDst && hSrc);

	for (int iSrc = 0, iDst = GetMenuItemCount(hDst); iSrc < GetMenuItemCount(hSrc); iSrc++)
	{
		TCHAR szMenuStr[256] = { 0 };
		MENUITEMINFO mInfo = { 0 };
		mInfo.cbSize = sizeof(mInfo);
		mInfo.fMask = 0
			| MIIM_CHECKMARKS //Retrieves or sets the hbmpChecked and hbmpUnchecked members. 
			| MIIM_DATA //Retrieves or sets the dwItemData member. 
			| MIIM_ID //Retrieves or sets the wID member. 
			| MIIM_STATE //Retrieves or sets the fState member. 
			| MIIM_SUBMENU //Retrieves or sets the hSubMenu member. 
			| MIIM_TYPE //Retrieves or sets the fType and dwTypeData members. 
			| 0;
		mInfo.dwTypeData = szMenuStr;
		mInfo.cch = _countof(szMenuStr);

		VERIFY(GetMenuItemInfo(hSrc, iSrc, TRUE, &mInfo));

		if (mInfo.hSubMenu)
		{
			HMENU hSub = CreatePopupMenu();
			AppendMenuOp(hSub, mInfo.hSubMenu);
			mInfo.hSubMenu = hSub;
		}

		InsertMenuItem(hDst, iDst++, TRUE, &mInfo);
		iCnt++;
	}

	return iCnt;
}

bool CCommon::IsMenuItemInMenu(CMenu* pMenu, UINT id)
{
	if (pMenu == nullptr)
		return false;

	int item_count = pMenu->GetMenuItemCount();
	for (int i = 0; i < item_count; i++)
	{
		if (pMenu->GetMenuItemID(i) == id)
			return true;
        CMenu* pSubMenu = pMenu->GetSubMenu(i);
        if (IsMenuItemInMenu(pSubMenu, id))
            return true;
	}
	return false;
}

int CCommon::GetMenuItemPosition(CMenu* pMenu, UINT id)
{
    int pos = -1;
    int item_count = pMenu->GetMenuItemCount();
    for (int i = 0; i < item_count; i++)
    {
        if (pMenu->GetMenuItemID(i) == id)
        {
            pos = i;
            break;
        }
    }
    return pos;
}

CString CCommon::LoadText(UINT id, LPCTSTR back_str)
{
	CString str;
	str.LoadString(id);
	if (back_str != nullptr)
		str += back_str;
	return str;
}

CString CCommon::LoadText(LPCTSTR front_str, UINT id, LPCTSTR back_str)
{
	CString str;
	str.LoadString(id);
	if (back_str != nullptr)
		str += back_str;
	if (front_str != nullptr)
		str = front_str + str;
	return str;
}

CString CCommon::StringFormat(LPCTSTR format_str, const std::initializer_list<CVariant>& paras)
{
	CString str_rtn = format_str;
	int index = 1;
	for (const auto& para : paras)
	{
		CString para_str = para.ToString();
		CString format_para;
		format_para.Format(_T("<%%%d%%>"), index);
		str_rtn.Replace(format_para, para_str);

		index++;
	}
	return str_rtn;
}

CString CCommon::LoadTextFormat(UINT id, const std::initializer_list<CVariant>& paras)
{
	CString str;
	str.LoadString(id);
	return StringFormat(str.GetString(), paras);
}


void CCommon::SetThreadLanguage(Language language)
{
	switch (language)
	{
	case Language::ENGLISH: SetThreadUILanguage(MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US)); break;
	case Language::SIMPLIFIED_CHINESE: SetThreadUILanguage(MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED)); break;
	//case Language::TRADITIONAL_CHINESE: SetThreadUILanguage(MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL)); break;
	default: break;
	}
}

void CCommon::WStringCopy(wchar_t * str_dest, int dest_size, const wchar_t * str_source, int source_size)
{
	if (dest_size <= 0)
		return;
	if (source_size <= 0 || str_source == nullptr)
	{
		str_dest[0] = L'\0';
		return;
	}
	int i;
	for (i = 0; i < dest_size && i < source_size && str_source[i] != L'\0'; i++)
		str_dest[i] = str_source[i];
	//确保目标字符串末尾有一个\0
	int copy_cnt = i;
	if (copy_cnt < dest_size)
		str_dest[copy_cnt] = L'\0';
	else
		str_dest[dest_size - 1] = L'\0';
}

void CCommon::NormalizeFont(LOGFONT & font)
{
	wstring name;
	wstring style;
	name = font.lfFaceName;
	if (name.empty())
		return;
	if (name.back() == L' ')
		name.pop_back();
	size_t index = name.rfind(L' ');
	if (index == wstring::npos)
		return;
	style = name.substr(index + 1);
	bool style_acquired = false;
	if (style == L"Light")
	{
		font.lfWeight = FW_LIGHT;
		style_acquired = true;
	}
	else if (style == L"Semilight")
	{
		font.lfWeight = 350;
		style_acquired = true;
	}
	else if (style == L"Semibold")
	{
		font.lfWeight = FW_SEMIBOLD;
		style_acquired = true;
	}
	else if (style == L"Bold")
	{
		font.lfWeight = FW_BOLD;
		style_acquired = true;
	}
	else if (style == L"Black")
	{
		font.lfWeight = FW_BLACK;
		style_acquired = true;
	}

	if (style_acquired)
	{
		name = name.substr(0, index);
	}
	//wcsncpy_s(font.lfFaceName, name.c_str(), 32);
	WStringCopy(font.lfFaceName, 32, name.c_str(), name.size());
}

int CCommon::GetMenuBarHeight(HWND hWnd)
{
	MENUBARINFO menuInfo{};
	menuInfo.cbSize = sizeof(MENUBARINFO);
	int rtn = GetMenuBarInfo(hWnd, OBJID_MENU, 0, &menuInfo);

	int menu_bar_height = 0;
	if (rtn != 0)
		menu_bar_height = menuInfo.rcBar.bottom - menuInfo.rcBar.top;

	return menu_bar_height;
}

double CCommon::DoubleRound(double dVal, int format)
{
	auto ipow = std::pow(10, format);
	return std::floor(dVal * ipow + 0.5) / ipow;
}

int CCommon::IconSizeNormalize(int size)
{
	//查找标准图标大小列表中和指定大小最接近的一个，然后将其返回

	const vector<int> standard_size{ 16, 20, 24, 32, 48, 64, 128, 256, 512 };
	int min_diff = MAXINT;
	int index = 0;
	for (size_t i = 0; i < standard_size.size(); i++)
	{
		int diff = std::abs(size - standard_size[i]);
		if (diff == min_diff && diff > 2)
			return size;

		if (diff < min_diff)
		{
			min_diff = diff;
			index = i;
		}
	}
	return standard_size[index];
}

void CCommon::SetWindowOpacity(HWND hWnd, int opacity)
{
	SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
	::SetLayeredWindowAttributes(hWnd, 0, opacity * 255 / 100, LWA_ALPHA);  //透明度取值范围为0~255
}

bool CCommon::StringIsVersion(LPCTSTR str)
{
	CString version_str{ str };
	return (version_str.GetLength() == 4 || version_str.GetLength() == 5) && version_str[1] == _T('.') && CharIsNumber(version_str[0]) && CharIsNumber(version_str[2]) && CharIsNumber(version_str[3]);
}

bool CCommon::GetFileContent(const wchar_t * file_path, string & contents_buff, bool binary, size_t max_size)
{
    std::ifstream file{ file_path, (binary ? std::ios::binary : std::ios::in) };
    if (file.fail())
        return false;
    contents_buff.clear();
    while (!file.eof())
    {
        contents_buff.push_back(file.get());
        if (contents_buff.size() > max_size)
            break;
    }
    contents_buff.pop_back();
    return true;
}

bool CCommon::SaveDataToFile(const string& data, const wstring& file_path)
{
	ofstream out_put{ file_path, std::ios::binary };
	if (out_put.fail())
		return false;
	out_put << data;
	out_put.close();
	return true;
}

void CCommon::DoOpenFileDlg(const wstring& filter, vector<wstring>& path_list, CWnd* pParent)
{
    path_list.clear();
    //构造打开文件对话框
    CFileDialog fileDlg(TRUE, NULL, NULL, OFN_ALLOWMULTISELECT, filter.c_str(), pParent);
    //设置保存文件名的字符缓冲的大小为128kB（如果以平均一个文件名长度为32字节计算，最多可以打开大约4096个文件）
    fileDlg.m_ofn.nMaxFile = 128 * 1024;
    LPTSTR ch = new TCHAR[fileDlg.m_ofn.nMaxFile];
    fileDlg.m_ofn.lpstrFile = ch;
    //对内存块清零
    ZeroMemory(fileDlg.m_ofn.lpstrFile, sizeof(TCHAR) * fileDlg.m_ofn.nMaxFile);
    //显示打开文件对话框
    if (IDOK == fileDlg.DoModal())
    {
        POSITION posFile = fileDlg.GetStartPosition();
        while (posFile != NULL)
        {
            path_list.push_back(fileDlg.GetNextPathName(posFile).GetString());
        }
    }
    delete[] ch;

}

void CCommon::SetDialogFont(CWnd* pDlg, CFont* pFont)
{
    if (pDlg->GetSafeHwnd() != NULL)
    {
        CWnd *pWndChild;
        pWndChild = pDlg->GetWindow(GW_CHILD);
        while (pWndChild)
        {
            pWndChild->SetFont(pFont);
            pWndChild = pWndChild->GetWindow(GW_HWNDNEXT);
        }
    }
}

void CCommon::FileAutoRename(wstring& file_path)
{
	while (CCommon::FileExist(file_path))
	{
		//判断文件名的末尾是否符合“(数字)”的形式
		wstring file_name;		//文件名（不含扩展名）
		CFilePathHelper c_file_path(file_path);
		file_name = c_file_path.GetFileNameWithoutExtension();
		wstring ext{ c_file_path.GetFileExtension() };
		int num;
		size_t index;
		bool is_numbered{ CCommon::IsFileNameNumbered(file_name, num, index) };		//文件名的末尾是否符合“(数字)”的形式
		if (!is_numbered)		//如果文件名末尾没有“(数字)”，则在末尾添加“ (1)”
		{
			file_name += L" (1)";
		}
		else		//否则，将原来的数字加1
		{
			file_name = file_name.substr(0, index);
			CString num_str;
			num_str.Format(_T("(%d)"), num + 1);
			file_name += num_str;
		}
		file_path = c_file_path.GetDir() + file_name + L'.' + ext;
	}

}

int CCommon::StringCompareInLocalLanguage(const wstring& str1, const wstring& str2, bool no_case)
{
    int rtn = CompareStringEx(LOCALE_NAME_USER_DEFAULT, (no_case ? NORM_IGNORECASE : 0), str1.c_str(), str1.size(), str2.c_str(), str2.size(), NULL, NULL, 0);
    if (rtn == CSTR_EQUAL)
        return 0;
    else if (rtn == CSTR_GREATER_THAN)
        return 1;
    else
        return -1;
}

