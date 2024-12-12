#include "functiondef.h"

ptr_CreateFontA old_CreateFontA = NULL;
ptr_CreateWindowExA old_CreateWindowExA = NULL;
ptr_MessageboxA old_MessageboxA = NULL;
ptr_CreateFileA old_CreateFileA = NULL;
ptr_ZwCreateFile old_ZwCreateFile = NULL;
ptr_FindFirstFileA old_FindFirstFileA = NULL;
ptr_NtOpenFile old_NtOpenFile = NULL;
ptr_AppendMenuA old_AppendMenuA = NULL;
ptr_SendMessageA old_SendMessageA = NULL;
ptr_GetGlyphOutlineA old_GetGlyphOutlineA = NULL;

DWORD BaseAddr = (DWORD)GetModuleHandle(NULL);


void make_console()
{
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
}


//�����ṩ��·������ȡ·�������ƺͺ�׺
void getFileNameAttr(std::wstring *fpath, std::wstring *fname, std::wstring *suffix, const WCHAR *base, int length) {
	if (fpath) fpath->clear();
	if (fname) fname->clear();
	if (suffix) suffix->clear();
	if (length <= 0) return;

	int ack = lstrlenW(base);

	const WCHAR *buf = base + length - 1;
	while (buf > base && buf[0] != L'\\') buf--;
	if (buf[0] == L'\\') {   //��׼·��
		if (fpath) fpath->assign(base, buf - base);
		if (fname) fname->assign(buf + 1, base + length - buf - 1);
	}
	else {  //ֻ���ļ���
		if (fname) fname->assign(base);
	}

	if (suffix) {
		buf = base + length - 1;
		while (buf > base && buf[0] != L'\\' && buf[0] != L'.') buf--;
		if (buf[0] == L'.') suffix->assign(buf + 1, base + length - buf - 1);  //��׺������.
	}
}


//�жϲ�����Ŀ¼�Ƿ�λ��ָ��Ŀ¼��
bool isAtWordDir(std::wstring *dstpath, PUNICODE_STRING path) {
	if (!path || !dstpath) return false;
	int wlen = dstpath->length();
	if (wlen > path->Length / 2) return false;
	//�Ƚ�
	const WCHAR* dst = dstpath->c_str();
 	for (size_t ii = 0; ii < wlen; ii++){
		if (towupper(dst[ii]) != towupper(path->Buffer[ii])) return false;
	}
	//�жϺ�һ�������Ƿ��� �ָ���
	if (path->Length / 2 > wlen && path->Buffer[wlen] != L'\\') return false;
	return true;
}


std::string getWorkDirA() {
	std::wstring wdir = getWorkDirW();
	return UnicodeToANSI(CP_ACP, wdir.c_str(), wdir.length());
}

std::wstring getWorkDirW() {
	WCHAR *tmp = new WCHAR[1024];
	memset(tmp, 0, sizeof(WCHAR) * 1024);
	GetModuleFileNameW(NULL, tmp, 1024 - 4);

	std::wstring s;
	getFileNameAttr(&s, nullptr, nullptr, tmp, lstrlenW(tmp));
	delete[] tmp;
	return s;
}


void getFileNameAttrA(std::string *fpath, std::string *fname, std::string *suffix, const char *base, int length) {
	if (fpath) fpath->clear();
	if (fname) fname->clear();
	if (suffix) suffix->clear();
	if (!base || length == 0) return;

	const char *buf = base + length - 1;
	while (buf > base && buf[0] != '\\') buf--;
	if (buf[0] == '\\') {   //��׼·��
		if (fpath) fpath->assign(base, buf - base);
		if (fname) fname->assign(buf + 1, base + length - buf - 1);
	}
	else {  //ֻ���ļ���
		if (fname) fname->assign(base);
	}

	if (suffix) {
		buf = base + length - 1;
		while (buf > base && buf[0] != '\\' && buf[0] != '.') buf--;
		if (buf[0] == '.') suffix->assign(buf + 1, base + length - buf - 1);  //��׺������.
	}
}



std::string UnicodeToANSI(UINT codepage, const WCHAR* ws, int wlen) {
	DWORD dwMinSize;
	std::string s;
	dwMinSize = WideCharToMultiByte(codepage, NULL, ws, wlen, NULL, 0, NULL, FALSE); //���㳤��
	if (dwMinSize == 0) return s;

	char *tmp = new char[dwMinSize];
	WideCharToMultiByte(codepage, NULL, ws, wlen, tmp, dwMinSize, NULL, FALSE);//ת��
	s.assign(tmp, dwMinSize);
	delete[] tmp;
	return s;
}



std::wstring ANSIToUnicode(UINT codepage, const char* s, int len) {
	DWORD dwMinSize;
	std::wstring ws;
	dwMinSize = MultiByteToWideChar(codepage, NULL, s, len, NULL, 0);
	if (dwMinSize == 0) return ws;

	WCHAR *tmp = new WCHAR[dwMinSize];
	MultiByteToWideChar(codepage, NULL, s, len, tmp, dwMinSize);
	ws.assign(tmp, dwMinSize);
	delete[] tmp;
	return ws;
}


std::string ANSItoANSI(UINT srcpage, const char *s, int len, UINT dstpage) {
	std::wstring ws = ANSIToUnicode(srcpage, s, len);
	return UnicodeToANSI(dstpage, ws.c_str(), ws.length());
}


//���Ŀ¼�Ƿ�Ϊָ���Ĺ���Ŀ¼���ǵĻ������滻
std::string replaceSrcDir(std::string *wdir, std::string *dstdir, const char *src) {
	std::string s;
	if (!src || !wdir) return s;
	int len = wdir->length();
	if (len > strlen(src)) return s;  //����Ŀ¼���ṩ�ַ���Ҫ��

	//�ȽϿ�ͷ�Ƿ���wdir��ʼ
	const char* buf = wdir->c_str();
	for (int ii = 0; ii < len; ii++) {
		int a, b;
		if (buf[ii] <= 0x7f) {
			a = tolower(buf[ii]);
			ii++;
		}
		else {
			a = *(short*)(buf + ii);
			ii += 2;
		}
		if (src[ii] <= 0x7f) b = tolower(src[ii]);
		else b = *(short*)(src + ii);

		if (a != b) return s;
	}

	//���·��
	buf = src + len;
	if (buf[0] == '\\') buf++;
	s.assign(buf);
	if (dstdir) s = *wdir + "\\" + *dstdir + "\\" + s;
	else s = *wdir + "\\" + s;
	return s;
}