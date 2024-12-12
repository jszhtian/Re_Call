﻿// PropertyDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MusicPlayer2.h"
#include "PropertyDlg.h"
#include "afxdialogex.h"
#include "COSUPlayerHelper.h"


// CPropertyDlg 对话框

IMPLEMENT_DYNAMIC(CPropertyDlg, CDialog)

CPropertyDlg::CPropertyDlg(vector<SongInfo>& all_song_info, CWnd* pParent /*=NULL*/, bool read_only)
    : CDialog(IDD_PROPERTY_DIALOG, pParent), m_all_song_info{ all_song_info }, m_read_only{ read_only }
{
    m_song_num = all_song_info.size();
}

CPropertyDlg::~CPropertyDlg()
{
}

void CPropertyDlg::ShowInfo()
{
	//显示文件名
	m_file_name_edit.SetWindowText(m_all_song_info[m_index].GetFileName().c_str());

	//显示文件路径
	m_file_path_edit.SetWindowText((m_all_song_info[m_index].file_path).c_str());

	//显示文件类型
	wstring file_type;
	CFilePathHelper file_path{ m_all_song_info[m_index].file_path };
	file_type = file_path.GetFileExtension();
	//if (file_type == _T("mp3"))
	//	m_file_type_edit.SetWindowText(_T("MP3音频文件"));
	//else if (file_type == _T("wma"))
	//	m_file_type_edit.SetWindowText(_T("Windows Media 音频文件"));
	//else if (file_type == _T("wav"))
	//	m_file_type_edit.SetWindowText(_T("WAV音频文件"));
	//else if (file_type == _T("mid"))
	//	m_file_type_edit.SetWindowText(_T("MIDI序列"));
	//else if (file_type == _T("ogg"))
	//	m_file_type_edit.SetWindowText(_T("OGG音频文件"));
	//else if (file_type == _T("m4a"))
	//	m_file_type_edit.SetWindowText(_T("MPEG-4 音频文件"));
	//else
	//	m_file_type_edit.SetWindowText((file_type + _T("文件")).c_str());
	m_file_type_edit.SetWindowText((CAudioCommon::GetAudioDescriptionByExtension(file_type)).c_str());

	//显示文件长度
	wstring song_length;
	if (m_all_song_info[m_index].lengh.isZero())
		song_length = CCommon::LoadText(IDS_CANNOT_GET_SONG_LENGTH);
	else
		song_length = m_all_song_info[m_index].lengh.toString2();
	m_song_length_edit.SetWindowText(song_length.c_str());

	//显示文件大小
	size_t file_size;
	file_size = CCommon::GetFileSize(m_all_song_info[m_index].file_path);
	CString size_info;
	if (file_size < 1024)
		size_info.Format(_T("%u B"), file_size);
	else if (file_size < 1024 * 1024)
		size_info.Format(_T("%.2f KB"), file_size / 1024.0f);
	else if (file_size < 1024 * 1024 * 1024)
		size_info.Format(_T("%.2f MB"), file_size / 1024.0f / 1024.0f);
	else
		size_info.Format(_T("%.2f GB"), file_size / 1024.0f / 1024.0f / 1024.0f);
	m_file_size_edit.SetWindowText(size_info);

	//显示比特率
	CString info;
	if (file_size == 0 || m_all_song_info[m_index].bitrate == 0)		//文件大小为0、文件长度为0或文件为midi音乐时不显示比特率
	{
		info = _T("-");
	}
	else
	{
		info.Format(_T("%d Kbps"), m_all_song_info[m_index].bitrate);
	}
	m_bit_rate_edit.SetWindowText(info);

	//显示歌词路径
	if(m_all_song_info[m_index].IsSameSong(CPlayer::GetInstance().GetCurrentSongInfo()) && CPlayer::GetInstance().IsInnerLyric())
		m_lyric_file_edit.SetWindowText(CCommon::LoadText(IDS_INNER_LYRIC));
	else if(!m_all_song_info[m_index].lyric_file.empty())
		m_lyric_file_edit.SetWindowText(m_all_song_info[m_index].lyric_file.c_str());
	else
		m_lyric_file_edit.SetWindowText(CCommon::LoadText(IDS_NO_MATCHED_LYRIC));

	//显示音频信息
	//CString info;
	m_title_edit.SetWindowText(m_all_song_info[m_index].GetTitle().c_str());
	m_artist_edit.SetWindowText(m_all_song_info[m_index].GetArtist().c_str());
	m_album_edit.SetWindowText(m_all_song_info[m_index].GetAlbum().c_str());
	m_year_edit.SetWindowText(m_all_song_info[m_index].GetYear().c_str());
	if (m_all_song_info[m_index].track != 0)
		info.Format(_T("%d"), m_all_song_info[m_index].track);
	else
		info = _T("");
	m_track_edit.SetWindowText(info);
	m_genre_combo.SetWindowText(m_all_song_info[m_index].GetGenre().c_str());
	m_comment_edit.SetWindowText(m_all_song_info[m_index].comment.c_str());

	//显示标签类型
	if (file_type == _T("mp3"))
	{
		CString tag_type_str{ CCommon::LoadText(IDS_MP3_TAG_TYPE) };
		switch (m_all_song_info[m_index].tag_type)
		{
		case 0: tag_type_str += CCommon::LoadText(IDS_UNKNOW); break;
		case 1: tag_type_str += _T("ID3v1"); break;
		case 2: tag_type_str += _T("ID3v2"); break;
		}
		SetDlgItemText(IDC_TAG_TYPE_STATIC, tag_type_str);
	}
	else
	{
		SetDlgItemText(IDC_TAG_TYPE_STATIC, _T(""));
	}


	//显示页数
	CString item_info;
	item_info.Format(_T("%d/%d"), m_index + 1, m_song_num);
	SetDlgItemText(IDC_ITEM_STATIC, item_info);
}

void CPropertyDlg::SetEditReadOnly(bool read_only)
{
	m_title_edit.SetReadOnly(read_only);
	m_artist_edit.SetReadOnly(read_only);
	m_album_edit.SetReadOnly(read_only);
	m_track_edit.SetReadOnly(read_only);
	m_year_edit.SetReadOnly(read_only);
	//m_genre_edit.SetReadOnly(read_only);
	//((CEdit*)m_genre_combo.GetWindow(GW_CHILD))->SetReadOnly(read_only);		//将combo box设为只读
	m_genre_combo.SetReadOnly(read_only);
	m_comment_edit.SetReadOnly(read_only);
}

void CPropertyDlg::SetWreteEnable()
{
	//目前暂时只支持MP3的ID3V1标签写入
	CFilePathHelper file_path{ m_all_song_info[m_index].file_path };
	m_write_enable = (!m_all_song_info[m_index].is_cue && !COSUPlayerHelper::IsOsuFile(file_path.GetFilePath()) && file_path.GetFileExtension() == L"mp3" && m_all_song_info[m_index].tag_type != 2);
    m_write_enable &= !m_read_only;
    SetEditReadOnly(!m_write_enable);
	m_save_button.EnableWindow(m_write_enable && m_modified);
	if (m_write_enable)
		m_year_edit.SetLimitText(4);
	else
		m_year_edit.SetLimitText(-1);
}


void CPropertyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FILE_NAME_EDIT, m_file_name_edit);
	DDX_Control(pDX, IDC_FILE_PATH_EDIT, m_file_path_edit);
	DDX_Control(pDX, IDC_FILE_TYPE_EDIT, m_file_type_edit);
	DDX_Control(pDX, IDC_SONG_LENGTH_EDIT, m_song_length_edit);
	DDX_Control(pDX, IDC_FILE_SIZE_EDIT, m_file_size_edit);
	DDX_Control(pDX, IDC_BIT_RATE_EDIT, m_bit_rate_edit);
	DDX_Control(pDX, IDC_TITEL_EDIT, m_title_edit);
	DDX_Control(pDX, IDC_ARTIST_EDIT, m_artist_edit);
	DDX_Control(pDX, IDC_ALBUM_EDIT, m_album_edit);
	DDX_Control(pDX, IDC_TRACK_EDIT, m_track_edit);
	DDX_Control(pDX, IDC_YEAR_EDIT, m_year_edit);
	//DDX_Control(pDX, IDC_GENRE_EDIT, m_genre_edit);
	DDX_Control(pDX, IDC_COMMENT_EDIT, m_comment_edit);
	DDX_Control(pDX, IDC_LYRIC_FILE_EDIT, m_lyric_file_edit);
	DDX_Control(pDX, IDC_SAVE_TO_FILE_BUTTON, m_save_button);
	DDX_Control(pDX, IDC_GENRE_COMBO, m_genre_combo);
}


BEGIN_MESSAGE_MAP(CPropertyDlg, CDialog)
	ON_BN_CLICKED(IDC_PREVIOUS_BUTTON, &CPropertyDlg::OnBnClickedPreviousButton)
	ON_BN_CLICKED(IDC_NEXT_BUTTON, &CPropertyDlg::OnBnClickedNextButton)
	ON_WM_MOUSEWHEEL()
	ON_EN_CHANGE(IDC_TITEL_EDIT, &CPropertyDlg::OnEnChangeTitelEdit)
	ON_EN_CHANGE(IDC_ARTIST_EDIT, &CPropertyDlg::OnEnChangeArtistEdit)
	ON_EN_CHANGE(IDC_ALBUM_EDIT, &CPropertyDlg::OnEnChangeAlbumEdit)
	ON_EN_CHANGE(IDC_TRACK_EDIT, &CPropertyDlg::OnEnChangeTrackEdit)
	ON_EN_CHANGE(IDC_YEAR_EDIT, &CPropertyDlg::OnEnChangeYearEdit)
	ON_EN_CHANGE(IDC_COMMENT_EDIT, &CPropertyDlg::OnEnChangeCommentEdit)
	//ON_CBN_EDITCHANGE(IDC_GENRE_COMBO, &CPropertyDlg::OnCbnEditchangeGenreCombo)
	ON_BN_CLICKED(IDC_SAVE_TO_FILE_BUTTON, &CPropertyDlg::OnBnClickedSaveToFileButton)
	ON_CBN_SELCHANGE(IDC_GENRE_COMBO, &CPropertyDlg::OnCbnSelchangeGenreCombo)
	//ON_BN_CLICKED(IDC_BUTTON3, &CPropertyDlg::OnBnClickedButton3)
END_MESSAGE_MAP()


// CPropertyDlg 消息处理程序


BOOL CPropertyDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_modified = false;
	m_genre_modified = false;
	m_list_refresh = false;

	//初始化流派列表
	for (int i{}; i < GENRE_MAX; i++)
	{
		m_genre_combo.AddString(GENRE_TABLE[i]);
	}
	CRect rect;
	m_genre_combo.SetMinVisibleItems(15);		//设置下拉列表的高度

	SetWreteEnable();
	ShowInfo();
	m_genre_combo.SetEditReadOnly();

    if (m_read_only)
	{
		m_save_button.ShowWindow(SW_HIDE);
		CWnd* pWnd = GetDlgItem(IDC_LYRIC_FILE_STATIC);
		if(pWnd!=nullptr)
			pWnd->ShowWindow(SW_HIDE);
		m_lyric_file_edit.ShowWindow(SW_HIDE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CPropertyDlg::OnBnClickedPreviousButton()
{
	// TODO: 在此添加控件通知处理程序代码
	m_modified = false;
	m_genre_modified = false;
	m_index--;
	if (m_index < 0) m_index = m_song_num - 1;
	if (m_index < 0) m_index = 0;
	SetWreteEnable();
	ShowInfo();
}


void CPropertyDlg::OnBnClickedNextButton()
{
	// TODO: 在此添加控件通知处理程序代码
	m_modified = false;
	m_genre_modified = false;
	m_index++;
	if (m_index >= m_song_num) m_index = 0;
	SetWreteEnable();
	ShowInfo();
}


BOOL CPropertyDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_UP)
		{
			OnBnClickedPreviousButton();
			return TRUE;
		}
		if (pMsg->wParam == VK_DOWN)
		{
			OnBnClickedNextButton();
			return TRUE;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}


BOOL CPropertyDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//通过鼠标滚轮翻页
	if (zDelta > 0)
	{
		OnBnClickedPreviousButton();
	}
	if (zDelta < 0)
	{
		OnBnClickedNextButton();
	}

	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}


void CPropertyDlg::OnEnChangeTitelEdit()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	m_modified = (m_title_edit.GetModify() != 0);
	m_list_refresh = m_modified;
	m_save_button.EnableWindow(m_write_enable && m_modified);
}


void CPropertyDlg::OnEnChangeArtistEdit()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	m_modified = (m_artist_edit.GetModify() != 0);
	m_list_refresh = m_modified;
	m_save_button.EnableWindow(m_write_enable && m_modified);
}


void CPropertyDlg::OnEnChangeAlbumEdit()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	m_modified = (m_album_edit.GetModify() != 0);
	m_save_button.EnableWindow(m_write_enable && m_modified);
}


void CPropertyDlg::OnEnChangeTrackEdit()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	m_modified = (m_track_edit.GetModify() != 0);
	m_save_button.EnableWindow(m_write_enable && m_modified);
}


void CPropertyDlg::OnEnChangeYearEdit()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	m_modified = (m_year_edit.GetModify() != 0);
	m_save_button.EnableWindow(m_write_enable && m_modified);
}


void CPropertyDlg::OnEnChangeCommentEdit()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	m_modified = (m_comment_edit.GetModify() != 0);
	m_save_button.EnableWindow(m_write_enable && m_modified);
}


//void CPropertyDlg::OnCbnEditchangeGenreCombo()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	m_modified = true;
//	m_save_button.EnableWindow(m_write_enable && m_modified);
//}


void CPropertyDlg::OnBnClickedSaveToFileButton()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!m_write_enable) return;
	CWaitCursor wait_cursor;
	SongInfo song_info;
	CString str_temp;
	m_title_edit.GetWindowText(str_temp);
	song_info.title = str_temp;
	m_artist_edit.GetWindowText(str_temp);
	song_info.artist = str_temp;
	m_album_edit.GetWindowText(str_temp);
	song_info.album = str_temp;
	m_track_edit.GetWindowText(str_temp);
	song_info.track = static_cast<BYTE>(_wtoi(str_temp));
	m_year_edit.GetWindowText(str_temp);
	song_info.year = str_temp;
	if (m_genre_modified)
		song_info.genre_idx = static_cast<BYTE>(m_genre_combo.GetCurSel());
	else
		song_info.genre_idx = m_all_song_info[m_index].genre_idx;		//如果流派没有修改，则将原来的流派号写回文件中
	m_comment_edit.GetWindowText(str_temp);
	song_info.comment = str_temp;

	bool text_cut_off;
	wstring file_path;
	file_path = m_all_song_info[m_index].file_path;
	if (!CAudioTag::WriteMp3Tag(file_path.c_str(), song_info, text_cut_off))
	{
		MessageBox(CCommon::LoadText(IDS_CANNOT_WRITE_TO_FILE), NULL, MB_ICONWARNING | MB_OK);
	}
	else
	{
		//重新从文件读取该歌曲的标签
		HSTREAM hStream;
		hStream = BASS_StreamCreateFile(FALSE, file_path.c_str(), 0, 0, BASS_SAMPLE_FLOAT);
		//CAudioCommon::GetAudioTags(hStream, AudioType::AU_MP3, CPlayer::GetInstance().GetCurrentDir(), m_all_song_info[m_index]);
		CAudioTag audio_tag(hStream, file_path, m_all_song_info[m_index]);
		audio_tag.GetAudioTag(true);
		BASS_StreamFree(hStream);
		theApp.m_song_data[m_all_song_info[m_index].file_path].CopyAudioTag(m_all_song_info[m_index]);
		theApp.SetSongDataModified();

		m_modified = false;
		m_save_button.EnableWindow(m_write_enable && m_modified);
	}
}


void CPropertyDlg::OnCbnSelchangeGenreCombo()
{
	// TODO: 在此添加控件通知处理程序代码
	m_modified = true;
	m_genre_modified = true;
	m_save_button.EnableWindow(m_write_enable && m_modified);
}


//用于测试
//void CPropertyDlg::OnBnClickedButton3()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	wstring str = CCommon::GetRandomString(32);
//
//	//重新从文件读取该歌曲的标签
//	wstring file_path;
//	file_path = CPlayer::GetInstance().GetCurrentDir() + m_all_song_info[m_index].file_name;
//	HSTREAM hStream;
//	hStream = BASS_StreamCreateFile(FALSE, file_path.c_str(), 0, 0, BASS_SAMPLE_FLOAT);
//	CAudioCommon::GetAudioTags(hStream, AudioType::AU_MP3, m_all_song_info[m_index]);
//	BASS_StreamFree(hStream);
//}
