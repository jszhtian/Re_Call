﻿#pragma once
#include "SongInfo.h"
#include "FormatConvertDlg.h"
#include <functional>
class CMusicPlayerCmdHelper
{
public:
    CMusicPlayerCmdHelper(CWnd* pOwner = nullptr);
    ~CMusicPlayerCmdHelper();

    void VeiwOnline(SongInfo& song);
    void FormatConvert(const std::vector<SongInfo>& songs);

    //执行添加到新建播放列表命令，成功返回true
    //get_song_list: 获取要添加的文件列表的回调函数，函数原型为 void Func(std::vector<SongInfo>&)
    //playlist_path: 接收新播放列表的路径
    //default_name：新播放列表的默认名称
    bool OnAddToNewPlaylist(std::function<void(std::vector<SongInfo>&)> get_song_list, std::wstring& playlist_path, const std::wstring& default_name = L"");

    void OnAddToPlaylistCommand(std::function<void(std::vector<SongInfo>&)> get_song_list, DWORD command);

    //从磁盘删除歌曲
    bool DeleteSongsFromDisk(const std::vector<SongInfo>& files);

	//查找匹配的歌词文件（暂未使用）
	void SearchLyricFiles(const SongInfo& song, std::vector<std::wstring>& result, bool fuzzy_match);
	std::wstring SearchLyricFile(const SongInfo& song, bool fuzzy_match);

    //更新媒体库，返回新增的歌曲数。（此函数执行时间可能会较长，应该在后台线程中执行）
    static int UpdateMediaLib();

    static int CleanUpSongData();
    static int CleanUpRecentFolders();

private:
    CWnd* GetOwner();

private:
    CWnd* m_pOwner{};
};

