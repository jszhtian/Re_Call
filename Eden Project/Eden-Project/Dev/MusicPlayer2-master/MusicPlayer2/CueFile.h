﻿#pragma once
#include <vector>
#include "Time.h"
#include "SongInfo.h"
#include "Common.h"

class CCueFile
{
public:
    CCueFile(const std::wstring& file_path);
    ~CCueFile();
    void SetTotalLength(Time length);       //设置cue对应音频文件的总长度（需要在解析完成后调用GetAudioFileName获取解析到的音频文件路径，再获取该音频文件的长度）
    const std::vector<SongInfo>& GetAnalysisResult() const;
    std::wstring GetAudioFileName() const;

private:
    void DoAnalysis();
    Time PhaseIndex(size_t pos);
    string GetCommand(const string& str, size_t pos = 0);

private:
    std::wstring m_file_path;
    std::string m_file_content;
    CodeType m_code_type{ CodeType::AUTO };
    std::wstring m_audio_file_name;
    std::vector<SongInfo> m_result;
};

