﻿#pragma once
#include "TabDlg.h"
#include "SongInfo.h"
#include "ListCtrlEx.h"

class CMediaLibTabDlg : public CTabDlg
{
	DECLARE_DYNAMIC(CMediaLibTabDlg)

public:
	CMediaLibTabDlg(UINT nIDTemplate, CWnd *pParent = NULL);
	~CMediaLibTabDlg();

public:
	virtual void GetSongsSelected(std::vector<wstring>& song_list) const;
	virtual void GetSongsSelected(std::vector<SongInfo>& song_list) const;
	virtual void GetCurrentSongList(std::vector<wstring>& song_list) const;
	virtual void GetCurrentSongList(std::vector<SongInfo>& song_list) const;

protected:
	virtual const CListCtrlEx& GetSongListCtrl() const = 0;
	virtual int GetItemSelected() const = 0;
	virtual const vector<int>& GetItemsSelected() const = 0;
	virtual void AfterDeleteFromDisk(const std::vector<SongInfo>& files) = 0;
	virtual int GetPathColIndex() const = 0;
	virtual wstring GetSelectedString() const = 0;

	virtual bool _OnAddToNewPlaylist(std::wstring& playlist_path);       //执行添加到新建播放列表命令，成功返回true，playlist_path用于接收新播放列表的路径

private:
	static UINT ViewOnlineThreadFunc(LPVOID lpParam);	//执行在线查看的线程函数

public:
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

	afx_msg void OnInitMenu(CMenu* pMenu);
	afx_msg void OnPlayItem();
	afx_msg void OnPlayItemInFolderMode();
	afx_msg void OnAddToNewPlaylist();
	afx_msg void OnAddToNewPalylistAndPlay();
	afx_msg void OnExploreOnline();
	afx_msg void OnExploreTrack();
	afx_msg void OnFormatConvert();
	afx_msg void OnDeleteFromDisk();
	afx_msg void OnItemProperty();
	afx_msg void OnCopyText();

};

