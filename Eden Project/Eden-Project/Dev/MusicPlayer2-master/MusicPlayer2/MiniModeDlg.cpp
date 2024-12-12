﻿// MiniModeDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MusicPlayer2.h"
#include "MiniModeDlg.h"
#include "afxdialogex.h"


// CMiniModeDlg 对话框

IMPLEMENT_DYNAMIC(CMiniModeDlg, CDialogEx)

CMiniModeDlg::CMiniModeDlg(int& item_selected, vector<int>& items_selected, CWnd* pParent /*=NULL*/)
    : CDialogEx(IDD_MINI_DIALOG, pParent), m_item_selected{ item_selected }, m_items_selected{ items_selected }
{
    //更改窗口的类名
    WNDCLASS wc;
    ::GetClassInfo(AfxGetInstanceHandle(), _T("#32770"), &wc);
    wc.lpszClassName = _T("MiniDlg_ByH87M");
    AfxRegisterClass(&wc);
}

CMiniModeDlg::~CMiniModeDlg()
{
}

void CMiniModeDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST2, m_playlist_ctrl);
}

void CMiniModeDlg::SaveConfig() const
{
    CIniHelper ini(theApp.m_config_path);
    ini.WriteInt(L"mini_mode", L"position_x", m_position_x);
    ini.WriteInt(L"mini_mode", L"position_y", m_position_y);
    ini.WriteBool(L"mini_mode", L"always_on_top", m_always_on_top);
    ini.Save();
}

void CMiniModeDlg::LoadConfig()
{
    CIniHelper ini(theApp.m_config_path);
    m_position_x = ini.GetInt(L"mini_mode", L"position_x", -1);
    m_position_y = ini.GetInt(_T("mini_mode"), _T("position_y"), -1);
    m_always_on_top = ini.GetBool(_T("mini_mode"), _T("always_on_top"), true);
}

void CMiniModeDlg::CheckWindowPos()
{
    CRect rect;
    GetWindowRect(rect);
    if (m_screen_rect.Width() <= rect.Width() || m_screen_rect.Height() <= rect.Height())
        return;
    if (rect.left < m_screen_rect.left)
    {
        rect.MoveToX(m_screen_rect.left);
        MoveWindow(rect);
    }
    if (rect.top < m_screen_rect.top)
    {
        rect.MoveToY(m_screen_rect.top);
        MoveWindow(rect);
    }
    if (rect.right > m_screen_rect.right)
    {
        rect.MoveToX(m_screen_rect.right - rect.Width());
        MoveWindow(rect);
    }
    if (rect.bottom > m_screen_rect.bottom)
    {
        rect.MoveToY(m_screen_rect.bottom - rect.Height());
        MoveWindow(rect);
    }
}

void CMiniModeDlg::UpdateSongTipInfo()
{
    CString song_tip_info;
    song_tip_info += CCommon::LoadText(IDS_NOW_PLAYING, _T(": "));
    song_tip_info += CPlayer::GetInstance().GetFileName().c_str();
    song_tip_info += _T("\r\n");
    song_tip_info += CCommon::LoadText(IDS_TITLE, _T(": "));
    song_tip_info += CPlayer::GetInstance().GetPlayList()[CPlayer::GetInstance().GetIndex()].title.c_str();
    song_tip_info += _T("\r\n");
    song_tip_info += CCommon::LoadText(IDS_ARTIST, _T(": "));
    song_tip_info += CPlayer::GetInstance().GetPlayList()[CPlayer::GetInstance().GetIndex()].artist.c_str();
    song_tip_info += _T("\r\n");
    song_tip_info += CCommon::LoadText(IDS_ALBUM, _T(": "));
    song_tip_info += CPlayer::GetInstance().GetPlayList()[CPlayer::GetInstance().GetIndex()].album.c_str();
    m_ui.UpdateSongInfoTip(song_tip_info);
}

void CMiniModeDlg::SetTitle()
{
    CString title;
    title = CPlayListCtrl::GetDisplayStr(CPlayer::GetInstance().GetCurrentSongInfo(), theApp.m_media_lib_setting_data.display_format).c_str();
    if (!title.IsEmpty())
        title += _T(" - ");
    title += APP_NAME;
    SetWindowText(title);
}


void CMiniModeDlg::SetAlwaysOnTop()
{
    if (m_always_on_top)
        SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);			//设置置顶
    else
        SetWindowPos(&wndNoTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);		//取消置顶
}

BEGIN_MESSAGE_MAP(CMiniModeDlg, CDialogEx)
    ON_WM_LBUTTONDOWN()
    ON_WM_TIMER()
    ON_WM_DESTROY()
    ON_WM_MOVE()
    ON_WM_RBUTTONUP()
    ON_COMMAND(ID_MINI_MODE_EXIT, &CMiniModeDlg::OnMiniModeExit)
    ON_WM_INITMENU()
    ON_WM_MOUSEWHEEL()
    ON_WM_LBUTTONDBLCLK()
    ON_NOTIFY(NM_DBLCLK, IDC_LIST2, &CMiniModeDlg::OnNMDblclkList2)
    ON_NOTIFY(NM_RCLICK, IDC_LIST2, &CMiniModeDlg::OnNMRClickList2)
    ON_WM_PAINT()
    //ON_STN_CLICKED(IDC_MINI_PROGRESS_STATIC, &CMiniModeDlg::OnStnClickedMiniProgressStatic)
    ON_WM_MOUSEMOVE()
    ON_WM_LBUTTONUP()
    ON_COMMAND(ID_SHOW_PLAY_LIST, &CMiniModeDlg::OnShowPlayList)
    ON_WM_MOUSELEAVE()
    ON_WM_SETCURSOR()
    ON_COMMAND(ID_MINI_MIDE_MINIMIZE, &CMiniModeDlg::OnMiniMideMinimize)
    ON_MESSAGE(WM_LIST_ITEM_DRAGGED, &CMiniModeDlg::OnListItemDragged)
    ON_COMMAND(ID_MINI_MODE_ALWAYS_ON_TOP, &CMiniModeDlg::OnMiniModeAlwaysOnTop)
END_MESSAGE_MAP()


// CMiniModeDlg 消息处理程序


void CMiniModeDlg::UpdatePlayPauseButton()
{
    m_ui.UpdatePlayPauseButtonTip();
}

void CMiniModeDlg::ShowPlaylist()
{
    m_playlist_ctrl.ShowPlaylist(theApp.m_media_lib_setting_data.display_format);
    SetPlayListColor();
}

void CMiniModeDlg::SetPlayListColor()
{
    m_playlist_ctrl.SetHightItem(CPlayer::GetInstance().GetIndex());
    //m_playlist_ctrl.SetColor(theApp.m_app_setting_data.theme_color);
    m_playlist_ctrl.EnsureVisible(CPlayer::GetInstance().GetIndex(), FALSE);
}
//
//void CMiniModeDlg::SetDefaultBackGround(CImage * pImage)
//{
//	m_ui_data.pDefaultBackground = pImage;
//}

//void CMiniModeDlg::SetDisplayFormat(DisplayFormat * pDisplayFormat)
//{
//	m_ui_data.pDisplayFormat = pDisplayFormat;
//}
//
BOOL CMiniModeDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  在此添加额外的初始化
    m_playlist_ctrl.SetFont(theApp.m_pMainWnd->GetFont());

    ::SystemParametersInfo(SPI_GETWORKAREA, 0, &m_screen_rect, 0);   // 获得工作区大小

    LoadConfig();

    m_pDC = GetDC();
    m_ui.Init(m_pDC);

    //初始化鼠标提示
    m_Mytip.Create(this, TTS_ALWAYSTIP);
    m_Mytip.SetMaxTipWidth(800);

    m_ui.SetToolTip(&m_Mytip);
    //UpdateSongTipInfo();

    //初始化窗口位置
    if (m_position_x != -1 && m_position_y != -1)
        SetWindowPos(nullptr, m_position_x, m_position_y, m_ui_data.widnow_width, m_ui_data.window_height, SWP_NOZORDER);
    else
        SetWindowPos(nullptr, 0, 0, m_ui_data.widnow_width, m_ui_data.window_height, SWP_NOMOVE | SWP_NOZORDER);
    CheckWindowPos();

    SetAlwaysOnTop();

    //初始化播放列表控件的位置
    CRect playlist_rect;
    int margin = 0;
    playlist_rect.left = margin;
    playlist_rect.right = m_ui_data.widnow_width - margin;
    playlist_rect.top = m_ui_data.window_height + margin;
    playlist_rect.bottom = m_ui_data.window_height2 - margin;
    m_playlist_ctrl.MoveWindow(playlist_rect);
    m_playlist_ctrl.AdjustColumnWidth();

    //装载右键菜单
    m_menu.LoadMenu(IDR_MINI_MODE_MENU);

    //设置定时器
    SetTimer(TIMER_ID_MINI, TIMER_ELAPSE_MINI, NULL);	//设置主定时器
    SetTimer(TIMER_ID_MINI2, TIMER_ELAPSE, NULL);		//设置用于界面刷新的定时器

    //显示播放列表
    ShowPlaylist();

    SetDragEnable();
    m_playlist_ctrl.EnableWindow(!CPlayer::GetInstance().m_loading);

    //设置窗口不透明度
    SetTransparency();

    m_show_playlist = false;
    m_ui_data.m_show_volume = false;
    m_first_start = true;

    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}


void CMiniModeDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    if(!m_ui.PointInControlArea(point))
        PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));

    m_ui.LButtonDown(point);

    CDialogEx::OnLButtonDown(nFlags, point);
}


void CMiniModeDlg::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    if (nIDEvent == TIMER_ID_MINI)
    {
        //更新鼠标提示
        static int index{};
        static wstring song_name{};
        //如果当前播放的歌曲发生变化，就更新鼠标提示信息
        if (index != CPlayer::GetInstance().GetIndex() || song_name != CPlayer::GetInstance().GetFileName())
        {
            UpdateSongTipInfo();
            SetTitle();
            //m_Mytip.UpdateTipText(m_song_tip_info, this);
            index = CPlayer::GetInstance().GetIndex();
            song_name = CPlayer::GetInstance().GetFileName();
            m_ui.DrawInfo(true);
        }

    }
    if (nIDEvent == TIMER_ID_MINI2)
    {
        if (!IsIconic() && IsWindowVisible())		//窗口最小化或隐藏时不绘制，以降低CPU利用率
            m_ui.DrawInfo(false);

        if (m_first_start)
        {
            UpdateSongTipInfo();
            SetTitle();
        }
        m_first_start = false;
    }
    if (nIDEvent == 11)
    {
        m_ui_data.m_show_volume = false;
        KillTimer(11);
    }
    CDialogEx::OnTimer(nIDEvent);
}


void CMiniModeDlg::SetVolume(bool up)
{
    if (up)
        CPlayer::GetInstance().MusicControl(Command::VOLUME_UP);
    else
        CPlayer::GetInstance().MusicControl(Command::VOLUME_DOWN);
    //ShowVolume(CPlayer::GetInstance().GetVolume());
    KillTimer(11);
    SetTimer(11, 1500, NULL);		//显示音量后设置一个1500毫秒的定时器（音量显示保持1.5秒）
    m_ui_data.m_show_volume = true;
}

void CMiniModeDlg::SetTransparency()
{
    CCommon::SetWindowOpacity(m_hWnd, theApp.m_app_setting_data.window_transparency);
}

void CMiniModeDlg::SetDragEnable()
{
    m_playlist_ctrl.SetDragEnable(CPlayer::GetInstance().IsPlaylistMode() && !theApp.m_media_lib_setting_data.disable_drag_sort);
}

BOOL CMiniModeDlg::PreTranslateMessage(MSG* pMsg)
{
    // TODO: 在此添加专用代码和/或调用基类

    if (pMsg->message == WM_KEYDOWN)
    {
        if (pMsg->wParam == VK_ESCAPE)
        {
            OnOK();		//按ESC键返回主窗口
            return TRUE;
        }
        if (pMsg->wParam == VK_RETURN) return TRUE;

        //按上下方向键调整音量
        if (pMsg->wParam == VK_UP)
        {
            SetVolume(true);
            return TRUE;
        }
        if (pMsg->wParam == VK_DOWN)
        {
            SetVolume(false);
            return TRUE;
        }

        //按Ctrl+X退出
        if ((GetKeyState(VK_CONTROL) & 0x80) && pMsg->wParam == 'X')
        {
            OnCancel();
            return TRUE;
        }
        //按Ctrl+M回到主窗口
        if ((GetKeyState(VK_CONTROL) & 0x80) && pMsg->wParam == 'M')
        {
            OnOK();
            return TRUE;
        }

        if (pMsg->wParam == VK_APPS)		//按菜单键弹出主菜单
        {
            CRect rect;
            GetWindowRect(rect);
            theApp.m_menu_set.m_main_popup_menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, rect.left, rect.bottom, this);
            return TRUE;
        }

    }

    //将此窗口的其他键盘消息转发给主窗口
    if (pMsg->message == WM_KEYDOWN)
    {
        ::PostMessage(theApp.m_pMainWnd->m_hWnd, WM_KEYDOWN, pMsg->wParam, pMsg->lParam);
        return TRUE;
    }

    if (pMsg->message == WM_MOUSEMOVE)
    {
        //POINT point = pMsg->pt;
        //ScreenToClient(&point);

        //if (!m_ui.PointInControlArea(point))
        m_Mytip.RelayEvent(pMsg);
    }

    return CDialogEx::PreTranslateMessage(pMsg);
}


void CMiniModeDlg::OnDestroy()
{
    CDialogEx::OnDestroy();

    // TODO: 在此处添加消息处理程序代码
    CRect rect;
    GetWindowRect(rect);
    m_position_x = rect.left;
    m_position_y = rect.top;
    SaveConfig();
    KillTimer(TIMER_ID_MINI);
    m_menu.DestroyMenu();
    ReleaseDC(m_pDC);
}


void CMiniModeDlg::OnMove(int x, int y)
{
    CDialogEx::OnMove(x, y);

    // TODO: 在此处添加消息处理程序代码
    CheckWindowPos();
}


void CMiniModeDlg::OnRButtonUp(UINT nFlags, CPoint point)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    //设置点击鼠标右键弹出菜单
    CPoint point1;	//定义一个用于确定光标位置的位置
    GetCursorPos(&point1);	//获取当前光标的位置，以便使得菜单可以跟随光标
    if (nFlags == MK_SHIFT)		//按住Shift键点击鼠标右键时，弹出系统菜单
    {
        theApp.m_menu_set.m_main_menu_popup.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point1.x, point1.y, this);
    }
    else
    {
        CMenu* pContextMenu = m_menu.GetSubMenu(0);
        pContextMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point1.x, point1.y, this); //在指定位置显示弹出菜单
    }

    CDialogEx::OnRButtonUp(nFlags, point);
}


void CMiniModeDlg::OnMiniModeExit()
{
    // TODO: 在此添加命令处理程序代码
    //执行退出命令时让对话框返回IDCANCEL，主窗口检测到这个返回值时就退出程序。
    OnCancel();
}


void CMiniModeDlg::OnInitMenu(CMenu* pMenu)
{
    CDialogEx::OnInitMenu(pMenu);

    // TODO: 在此处添加消息处理程序代码


    //设置复选菜单项的勾选
    pMenu->CheckMenuItem(ID_SHOW_PLAY_LIST, MF_BYCOMMAND | (m_show_playlist ? MF_CHECKED : MF_UNCHECKED));
    pMenu->CheckMenuItem(ID_ADD_REMOVE_FROM_FAVOURITE, MF_BYCOMMAND | (CPlayer::GetInstance().IsFavourite() ? MF_CHECKED : MF_UNCHECKED));
    pMenu->CheckMenuItem(ID_MINI_MODE_ALWAYS_ON_TOP, MF_BYCOMMAND | (m_always_on_top ? MF_CHECKED : MF_UNCHECKED));

    //设置播放列表右键菜单的默认菜单项
    pMenu->SetDefaultItem(ID_PLAY_ITEM);

    theApp.m_pMainWnd->SendMessage(WM_SET_MENU_STATE, (WPARAM)pMenu);
}


BOOL CMiniModeDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    if (zDelta > 0)
    {
        SetVolume(true);
    }
    if (zDelta < 0)
    {
        SetVolume(false);
    }

    return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}


void CMiniModeDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    //if(!m_ui.PointInControlArea(point))
    //	OnOK();

    CDialogEx::OnLButtonDblClk(nFlags, point);
}


void CMiniModeDlg::OnNMDblclkList2(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
    // TODO: 在此添加控件通知处理程序代码
    int row = pNMItemActivate->iItem;
    CPlayer::GetInstance().PlayTrack(row);
    //SwitchTrack();
    SetPlayListColor();
    //RePaint();
    *pResult = 0;
}


void CMiniModeDlg::OnNMRClickList2(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
    // TODO: 在此添加控件通知处理程序代码
    m_item_selected = pNMItemActivate->iItem;	//获取鼠标选中的项目
    m_playlist_ctrl.GetItemSelected(m_items_selected);

    CMenu* pContextMenu = theApp.m_menu_set.m_list_popup_menu.GetSubMenu(0); //获取第一个弹出菜单
    m_playlist_ctrl.ShowPopupMenu(pContextMenu, pNMItemActivate->iItem, this);
    *pResult = 0;
}


void CMiniModeDlg::OnPaint()
{
    CPaintDC dc(this); // device context for painting
    // TODO: 在此处添加消息处理程序代码
    // 不为绘图消息调用 CDialogEx::OnPaint()
    m_ui.DrawInfo();
}


//void CMiniModeDlg::OnStnClickedMiniProgressStatic()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	//int pos = m_progress_bar.GetPos();
//	//int song_pos = static_cast<__int64>(pos) * CPlayer::GetInstance().GetSongLength() / 1000;
//	//CPlayer::GetInstance().SeekTo(song_pos);
//}


void CMiniModeDlg::OnMouseMove(UINT nFlags, CPoint point)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    m_ui.MouseMove(point);

    CDialogEx::OnMouseMove(nFlags, point);
}


void CMiniModeDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    m_ui.LButtonUp(point);

    CDialogEx::OnLButtonUp(nFlags, point);
}


void CMiniModeDlg::OnShowPlayList()
{
    // TODO: 在此添加命令处理程序代码
    if (m_show_playlist)
    {
        SetWindowPos(nullptr, 0, 0, m_ui_data.widnow_width, m_ui_data.window_height, SWP_NOMOVE | SWP_NOZORDER);
        CheckWindowPos();
        m_show_playlist = false;
    }
    else
    {
        SetWindowPos(nullptr, 0, 0, m_ui_data.widnow_width, m_ui_data.window_height2, SWP_NOMOVE | SWP_NOZORDER);
        CheckWindowPos();
        m_show_playlist = true;
    }
}


void CMiniModeDlg::OnMouseLeave()
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    m_ui.MouseLeave();

    CDialogEx::OnMouseLeave();
}


BOOL CMiniModeDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    if (m_ui.SetCursor())
        return TRUE;
    else
        return CDialogEx::OnSetCursor(pWnd, nHitTest, message);
}


void CMiniModeDlg::OnMiniMideMinimize()
{
    // TODO: 在此添加命令处理程序代码
    ShowWindow(HIDE_WINDOW);
}


afx_msg LRESULT CMiniModeDlg::OnListItemDragged(WPARAM wParam, LPARAM lParam)
{
    theApp.m_pMainWnd->SendMessage(WM_LIST_ITEM_DRAGGED, wParam, lParam);
    return 0;
}


BOOL CMiniModeDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
    // TODO: 在此添加专用代码和/或调用基类
    WORD command = LOWORD(wParam);
    if (command >= ID_ADD_TO_DEFAULT_PLAYLIST && command <= ID_ADD_TO_MY_FAVOURITE + ADD_TO_PLAYLIST_MAX_SIZE)
    {
        theApp.m_pMainWnd->SendMessage(WM_COMMAND, wParam, lParam);
    }

    return CDialogEx::OnCommand(wParam, lParam);
}


void CMiniModeDlg::OnMiniModeAlwaysOnTop()
{
    // TODO: 在此添加命令处理程序代码
    m_always_on_top = !m_always_on_top;
    SetAlwaysOnTop();
}
