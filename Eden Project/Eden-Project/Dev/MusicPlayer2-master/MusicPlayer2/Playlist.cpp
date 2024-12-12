﻿#include "stdafx.h"
#include "Playlist.h"
#include "Common.h"
#include "FilePathHelper.h"

const vector<wstring> CPlaylistFile::m_surpported_playlist{ PLAYLIST_EXTENSION, L".m3u", L".m3u8" };

/*
播放列表文件格式说明
每行一个曲目，每一行的格式为：
文件路径|是否为cue音轨|cue音轨起始时间|cue音轨结束时间|标题|艺术家|唱片集|曲目序号|比特率|流派|年份|注释
目前除了cue音轨外，其他曲目只保存文件路径
*/

CPlaylistFile::CPlaylistFile()
{
}


CPlaylistFile::~CPlaylistFile()
{
}

wstring DeleteInvalidCh(const wstring& str)
{
    wstring result = str;
    CCommon::StringCharacterReplace(result, L'|', L'_');
    return result;
}

void CPlaylistFile::LoadFromFile(const wstring & file_path)
{
    ifstream stream{ file_path };
    if (stream.fail())
        return;

	//判断文件编码
	bool utf8{};
	wstring file_extension = CFilePathHelper(file_path).GetFileExtension();
	utf8 = (file_extension != L"m3u");

    string current_line;
    while (!stream.eof())
    {
        std::getline(stream, current_line);
		DisposePlaylistFileLine(current_line, utf8);
    }

}

void CPlaylistFile::SaveToFile(const wstring & file_path, Type type) const
{
    ofstream stream{ file_path };
    if(type == PL_PLAYLIST)
    {
        for (const auto& item : m_playlist)
        {
            stream << CCommon::UnicodeToStr(item.file_path, CodeType::UTF8_NO_BOM);
            if (item.is_cue)
            {
                CString buff;
                buff.Format(L"|%d|%d|%d|%s|%s|%s|%d|%d|%s|%s|%s", item.is_cue, item.start_pos.toInt(), item.end_pos.toInt(),
                    DeleteInvalidCh(item.title).c_str(), DeleteInvalidCh(item.artist).c_str(), DeleteInvalidCh(item.album).c_str(),
                    item.track, item.bitrate,
                    DeleteInvalidCh(item.genre).c_str(), DeleteInvalidCh(item.year).c_str(), DeleteInvalidCh(item.comment).c_str());
                stream << CCommon::UnicodeToStr(buff.GetString(), CodeType::UTF8_NO_BOM);
            }
            stream << std::endl;
        }
    }
    else if (type == PL_M3U || type == PL_M3U8)
    {
        CodeType code_type{ CodeType::ANSI };
        if (type == PL_M3U8)
            code_type = CodeType::UTF8_NO_BOM;

        stream << "#EXTM3U" << std::endl;
        for (const auto& item : m_playlist)
        {
            CString buff;
            buff.Format(_T("#EXTINF:%d,%s - %s"), item.lengh.toInt() / 1000, item.GetArtist().c_str(), item.GetTitle().c_str());
            stream << CCommon::UnicodeToStr(buff.GetString(), code_type) << std::endl;
            stream << CCommon::UnicodeToStr(item.file_path, code_type) << std::endl;
        }
    }
}

vector<SongInfo> CPlaylistFile::GetPlaylist() const
{
    return m_playlist;
}

void CPlaylistFile::AddFiles(const vector<wstring>& files)
{
    for (const auto& file : files)
    {
        SongInfo item;
        item.file_path = file;
        m_playlist.push_back(item);
    }
}

void CPlaylistFile::AddFiles(const vector<SongInfo>& files)
{
    for (const auto& file : files)
    {
        m_playlist.push_back(file);
    }

}

void CPlaylistFile::FromSongList(const vector<SongInfo>& song_list)
{
    m_playlist = song_list;
}

void CPlaylistFile::ToSongList(vector<SongInfo>& song_list)
{
    for (const auto& item : m_playlist)
    {
        song_list.push_back(item);
    }
}

bool CPlaylistFile::IsFileInPlaylist(const SongInfo& file)
{
    auto iter = std::find_if(m_playlist.begin(), m_playlist.end(), [&file](const SongInfo& item)
    {
        if (file.is_cue)
            return file.file_path == item.file_path && file.track == item.track;
        else
            return file.file_path == item.file_path;
    });
    return iter != m_playlist.end();
}

void CPlaylistFile::RemoveFile(const wstring& file)
{
    auto iter = std::find_if(m_playlist.begin(), m_playlist.end(), [&file](const SongInfo& item)
    {
        return file == item.file_path;
    });
    if (iter != m_playlist.end())
    {
        m_playlist.erase(iter);
    }
}

bool CPlaylistFile::IsPlaylistFile(const wstring& file_path)
{
    wstring file_extension = CFilePathHelper(file_path).GetFileExtension(false, true);
    return CCommon::IsItemInVector(m_surpported_playlist, file_extension);
}

bool CPlaylistFile::IsPlaylistExt(wstring ext)
{
    if (ext.empty())
        return false;
    if (ext.front() != L'.')
        ext = L'.' + ext;
    return CCommon::IsItemInVector(m_surpported_playlist, ext);
}

void CPlaylistFile::DisposePlaylistFileLine(const string& str_current_line, bool utf8)
{
	string current_line = str_current_line;
	CCommon::DeleteStringBom(current_line);
	if (!current_line.empty() && current_line.front() == '\"')
		current_line = current_line.substr(1);
	if (!current_line.empty() && current_line.back() == '\"')
		current_line.pop_back();

	if (current_line.size() > 3)
	{
		SongInfo item;
		wstring current_line_wcs = CCommon::StrToUnicode(current_line, utf8 ? CodeType::UTF8 : CodeType::ANSI);
		size_t index = current_line_wcs.find(L'|');
		item.file_path = current_line_wcs.substr(0, index);

		if (index < current_line_wcs.size() - 1)
		{
			vector<wstring> result;
			CCommon::StringSplit(current_line_wcs, L'|', result, false);
			if (result.size() >= 2)
				item.is_cue = (_wtoi(result[1].c_str()) != 0);
			if (result.size() >= 3)
				item.start_pos.fromInt(_wtoi(result[2].c_str()));
			if (result.size() >= 4)
				item.end_pos.fromInt(_wtoi(result[3].c_str()));
			item.lengh = item.end_pos - item.start_pos;
			if (result.size() >= 5)
				item.title = result[4];
			if (result.size() >= 6)
				item.artist = result[5];
			if (result.size() >= 7)
				item.album = result[6];
			if (result.size() >= 8)
				item.track = _wtoi(result[7].c_str());
			if (result.size() >= 9)
				item.bitrate = _wtoi(result[8].c_str());
			if (result.size() >= 10)
				item.genre = result[9];
			if (result.size() >= 11)
				item.year = result[10];
			if (result.size() >= 12)
				item.comment = result[11];
		}
		if(CCommon::FileExist(item.file_path))
			m_playlist.push_back(item);
	}
}
