#define UILIB_STATIC				// 声明静态宏：UILIB_STATIC（必须在UIlib.h之前声明）			
#include "UIlib.h"		                // 引入头文件：UIlib.h						
using namespace DuiLib;	                        // 使用命名空间：DuiLib
#pragma comment(lib, "DuiLib_Static_u.lib")	// 加载静态库：DuiLib_Static_ud.lib      

const _TCHAR* GameTitle = _T("【靴下汉化组】放学后的灰姑娘 简体中文化补丁v1.0 安装程序");

class CFrameWindowWnd : public CWindowWnd, public INotifyUI
{
public:
    CFrameWindowWnd() { };
    LPCTSTR GetWindowClassName() const { return _T("AyamiUI"); };
    UINT GetClassStyle() const { return UI_CLASSSTYLE_FRAME | CS_DBLCLKS; };
    void OnFinalMessage(HWND /*hWnd*/) { delete this; };

    void Notify(TNotifyUI& msg)
    {
        if (msg.sType == _T("click")) {
            if (msg.pSender->GetName() == _T("Install")) {
            }
        }
    }

    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        if (uMsg == WM_CREATE) {
            m_pm.Init(m_hWnd);
            CDialogBuilder builder;
            CControlUI* pRoot = builder.Create(_T("Setup.xml"), 0, NULL, &m_pm);
            ASSERT(pRoot && "Failed to parse XML");
            m_pm.AttachDialog(pRoot);

            m_pm.AddNotifier(this);
            return 0;
        }
        else if (uMsg == WM_DESTROY) {
            ::PostQuitMessage(0);
        }
        /*
        else if (uMsg == WM_NCACTIVATE) {
            if (!::IsIconic(m_hWnd)) {
                return (wParam == 0) ? TRUE : FALSE;
            }
        }
        else if (uMsg == WM_NCCALCSIZE) {
            return 0;
        }
        else if (uMsg == WM_NCPAINT) {
            return 0;
        }
        */
        LRESULT lRes = 0;
        if (m_pm.MessageHandler(uMsg, wParam, lParam, lRes)) return lRes;
        return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
    }


public:
    CPaintManagerUI m_pm;
};

// 程序入口及Duilib初始化部分
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int nCmdShow)
{
    CPaintManagerUI::SetInstance(hInstance);
    CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath());

    CFrameWindowWnd* pFrame = new CFrameWindowWnd();
    if (pFrame == NULL) return 0;
    pFrame->Create(NULL, GameTitle, UI_WNDSTYLE_DIALOG, WS_EX_WINDOWEDGE);
    pFrame->ShowWindow(true);
    CPaintManagerUI::MessageLoop();

    return 0;
}
