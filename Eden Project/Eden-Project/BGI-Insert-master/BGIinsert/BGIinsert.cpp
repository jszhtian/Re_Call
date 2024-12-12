// BGIinsert.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
using namespace std;

struct chsblock
{
	int order;
	wstring rawtext;
	wstring transtext;
};

LPWSTR ctowJP(char* str)
{
	DWORD dwMinSize;
	dwMinSize = MultiByteToWideChar(932, 0, str, -1, NULL, 0); //计算长度
	LPWSTR out = new wchar_t[dwMinSize];
	MultiByteToWideChar(932, 0, str, -1, out, dwMinSize);//转换
	return out;
}

LPWSTR ctowGBK(char* str)
{
	DWORD dwMinSize;
	dwMinSize = MultiByteToWideChar(936, 0, str, -1, NULL, 0); //计算长度
	LPWSTR out = new wchar_t[dwMinSize];
	MultiByteToWideChar(936, 0, str, -1, out, dwMinSize);//转换
	return out;
}

LPWSTR ctowUTF8(char* str)
{
	DWORD dwMinSize;
	dwMinSize = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0); //计算长度
	LPWSTR out = new wchar_t[dwMinSize];
	MultiByteToWideChar(CP_UTF8, 0, str, -1, out, dwMinSize);//转换
	return out;
}

char* wtocGBK(LPCTSTR str)
{
	DWORD dwMinSize;
	dwMinSize = WideCharToMultiByte(936, NULL, str, -1, NULL, 0, NULL, FALSE); //计算长度
	char *out = new char[dwMinSize];
	WideCharToMultiByte(936, NULL, str, -1, out, dwMinSize, NULL, FALSE);//转换
	return out;
}

char* wtocUTF8(LPCTSTR str)
{
	DWORD dwMinSize;
	dwMinSize = WideCharToMultiByte(CP_UTF8, NULL, str, -1, NULL, 0, NULL, FALSE); //计算长度
	char *out = new char[dwMinSize];
	WideCharToMultiByte(CP_UTF8, NULL, str, -1, out, dwMinSize, NULL, FALSE);//转换
	return out;
}

bool checkfile(ScriptHeader check)
{
	if (strcmp((check.Magic),"BurikoCompiledScriptVer1.00")==0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

DWORD getsizeofimage(DWORD* buf, int maxrange)
{
	int lastposition=-1;
	/*
	//For New End
	for (int i=0;i<maxrange/4;i++)
	{
	if (buf[i]==0xF4)
	{
	lastposition = i;
	}
	}

	*/

	if (lastposition==-1)
	{
		for (int i = 0; i < maxrange / 4; i++)
		{
			if (buf[i] == 0x1b)
			{
				lastposition = i;
			}
		}
	}
	return lastposition;
}

wstring FixStringW(wstring& Line)
{
	wstring Name;
	for (auto it : Line)
	{
		if (it == 0x000A)
		{
			Name += L"\\n";
		}
		else
		{
			Name += it;
		}
	}
	return Name;
}

std::wstring ReplaceCR(const wstring& orignStr)
{
	size_t pos = 0;
	wstring tempStr = orignStr;
	wstring oldStr = L"\\n";
	wstring newStr = L"\n";
	wstring::size_type newStrLen = newStr.length();
	wstring::size_type oldStrLen = oldStr.length();
	while (true)
	{
		pos = tempStr.find(oldStr, pos);
		if (pos == wstring::npos) break;

		tempStr.replace(pos, oldStrLen, newStr);
		pos += newStrLen;

	}

	return tempStr;
}

int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		cout << argv[0] << " file [mode]" << endl;
		cout << "[Mode]" << endl;
		cout << "i->Insert" << endl;
		cout << "o->Output" << endl;
		getchar();
		return -1;
	}
	ScriptHeader shdr;
	fstream file1;
	file1.open(argv[1], ios::binary | ios::in);
	auto buf1 = new byte[sizeof(ScriptHeader)];
	file1.read((char*)buf1, sizeof(ScriptHeader));
	memcpy(&shdr, buf1, sizeof(ScriptHeader));
	delete[] buf1;
	buf1 = nullptr;
	auto res = checkfile(shdr);
	if (!res)
	{
		cout << "Mismatch File Format!" << endl;
		return -1;
	}
	file1.seekg(0, ios::end);
	DWORD szOfFile = file1.tellg();
	auto secHead = sizeof(shdr.Magic) + shdr.HeaderSize;
	file1.seekg(secHead, ios::beg);
	auto sectsize1 = szOfFile - secHead;
	auto buffer = new byte[sectsize1];
	memset(buffer, 0, sectsize1);
	file1.read((char*)buffer, sectsize1);
	auto szofImage = getsizeofimage((DWORD*)buffer, sectsize1);
	if (szofImage==-1)
	{
		cout << "Error detected!" << endl;
		return -1;
	}
	auto cmdptr = (DWORD*)buffer;
	if (*argv[2]=='o')
	{
		string fn = argv[1];
		fn += ".txt";
		fstream file2;
		file2.open(fn,ios::out);
		int counter = 0;
		for (DWORD i = 0; i < szofImage; i++)
		{
			if (cmdptr[i] == 0x03)
			{
				char* text =  (char*)cmdptr+cmdptr[i+1];
				char testchar = '\0';
				if (memcmp(text - 1, &testchar, 1) != 0)
				{
					continue;
				}
				if ((USHORT)*text>0x7F || *text == '<')
				{
					if (*text!='_')
					{
						if (!file2.is_open())
						{
							file2.open(fn, ios::out|ios::app);
						}
						wchar_t* filterwchar = ctowJP(text);
						wstring filtertext = FixStringW(wstring(filterwchar));
						char* outtext = wtocUTF8(filtertext.c_str());
						string outputprefix;
						string num = to_string(counter);
						stringstream inter;
						inter << setw(10) << setfill('0') << num;
						inter >> num;
						outputprefix += "<";
						outputprefix += num;
						outputprefix += ">";
						file2 << outputprefix << "//" << outtext << endl;
						file2 << outputprefix << outtext << endl;
						file2 << endl;
						delete filterwchar;
						delete outtext;
						counter++;
					}
				}
			}
			file2.close();
		}
	}

	if (*argv[2]=='i')
	{
		string fn = argv[1];
		fn += ".txt";
		ifstream file3(fn);
		vector<chsblock> newtextlist;
		while (!file3.eof())
		{
			string proc;
			//file3 >> proc;
			getline(file3, proc);
			if (proc[0]=='<'&&proc[12]=='/')
			{
				chsblock block;
				string num = proc.substr(1, 10);
				block.order = stoll(num);
				block.rawtext = ReplaceCR(ctowUTF8((char*)proc.substr(14).c_str()));
				//file3 >> proc;
				getline(file3, proc);
				block.transtext = ReplaceCR(ctowUTF8((char*)proc.substr(12).c_str()));
				newtextlist.push_back(block);
			}
		}
		auto outbuffer = new byte[6553600];
		memset(outbuffer, 0, 6553600);
		int counter = 0;
		int bytetotransfer = 0;
		for (DWORD i = 0; i < szofImage; i++)
		{
			if (cmdptr[i] == 0x03)
			{
				char* text = (char*)cmdptr + cmdptr[i + 1];
				char testchar = '\0';
				if (memcmp(text - 1, &testchar, 1) != 0)
				{
					continue;
				}
				if ((USHORT)*text > 0x7F || *text == '<')
				{
					if (*text != '_')
					{
						wchar_t* filtertext = ctowJP(text);
						int len = wcslen((wchar_t*)newtextlist.at(counter).transtext.c_str());
						int res=wcscmp(filtertext, (wchar_t*)newtextlist.at(counter).rawtext.c_str());
						//int res2 = wcscmp(filtertext, (wchar_t*)newtextlist.at(counter).transtext.c_str());
						
						if (res==0&&len!=0)//res2!=0&&
						{
							char* inserttext = wtocGBK((wchar_t*)newtextlist.at(counter).transtext.c_str());
							//char* inserttext = wtocUTF8((wchar_t*)newtextlist.at(counter).transtext.c_str());
							int stln=newtextlist.at(counter).transtext.length()*2;
							memcpy(outbuffer + bytetotransfer, inserttext, stln);
							cmdptr[i + 1] = sectsize1 + bytetotransfer;
							bytetotransfer += stln + 1;
						}
						
						counter++;
					}
				}
			}
		}
		fstream file4;
		string fn2 = argv[1];
		fn2 += ".new";
		file4.open(fn2, ios::binary | ios::out);
		auto prebuffer = new byte[secHead];
		file1.seekg(0, ios::beg);
		file1.read((char*)prebuffer, secHead);
		file4.write((char*)prebuffer, secHead);
		file4.write((char*)buffer, sectsize1);
		file4.write((char*)outbuffer, bytetotransfer);
		file4.close();
		
	}
	file1.close();
    return 0;
}

