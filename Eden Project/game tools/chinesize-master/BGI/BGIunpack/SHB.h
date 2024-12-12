#include <cstring>
#include "shlobj.h"
#include "windows.h"
bool SHBopen( wchar_t *dest , wchar_t *textinfo , int IsFile ) ;

bool SHBopen( wchar_t *dest , wchar_t *textinfo , int IsFile )
{
	BROWSEINFO bi ;
	bi.hwndOwner = NULL ;   // �����ھ��
	bi.pidlRoot = NULL ;        // Ҫ��ʾ���ļ�Ŀ¼�Ի���ĸ�(Root)
   bi.pszDisplayName = dest ;  // ���汻ѡȡ���ļ���·���Ļ�����
   bi.lpszTitle = textinfo  ;  // ��ʾλ�ڶԻ������ϲ��ı���
   if ( IsFile == 1 ) bi.ulFlags = BIF_BROWSEINCLUDEFILES ;  //ͬʱ��ʾ�ļ�
   else    bi.ulFlags = BIF_STATUSTEXT;    // ֻ��ʾĿ¼
   bi.lpfn = NULL ;        // �����¼��Ļص�����
   bi.lParam = 0 ;         // Ӧ�ó��򴫸��ص������Ĳ���
   bi.iImage= NULL ;    // �ļ��жԻ����ͼƬ����
   LPITEMIDLIST pIDList = SHBrowseForFolder( &bi ) ;
   if(pIDList)
       {
          SHGetPathFromIDList(pIDList, dest ) ;
       }

return true ;
}