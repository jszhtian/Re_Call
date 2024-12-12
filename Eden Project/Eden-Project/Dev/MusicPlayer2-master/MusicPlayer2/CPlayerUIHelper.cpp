﻿#include "stdafx.h"
#include "CPlayerUIHelper.h"
#include "MusicPlayer2.h"


CPlayerUIHelper::CPlayerUIHelper()
{
}


CPlayerUIHelper::~CPlayerUIHelper()
{
}

UIColors CPlayerUIHelper::GetUIColors(const ColorTable & colorTable, bool dark)
{
    UIColors colors{};
    //if (theApp.m_app_setting_data.dark_mode)
    if (dark)
    {
        colors.color_text = ColorTable::WHITE;
        colors.color_text_lable = theApp.m_app_setting_data.theme_color.light2;
        colors.color_text_2 = theApp.m_app_setting_data.theme_color.light1;
        colors.color_text_heighlight = theApp.m_app_setting_data.theme_color.light2;
        colors.color_back = GRAY(96);
        colors.color_lyric_back = theApp.m_app_setting_data.theme_color.dark3;
        colors.color_control_bar_back = theApp.m_app_setting_data.theme_color.dark2;
        colors.color_spectrum = theApp.m_app_setting_data.theme_color.light2;
        colors.color_spectrum_cover = theApp.m_app_setting_data.theme_color.original_color;
        colors.color_spectrum_back = theApp.m_app_setting_data.theme_color.dark1;
        colors.color_button_back = theApp.m_app_setting_data.theme_color.dark3;
        colors.color_button_pressed = theApp.m_app_setting_data.theme_color.light2;
    }
    else
    {
        colors.color_text = theApp.m_app_setting_data.theme_color.dark2;
        colors.color_text_lable = theApp.m_app_setting_data.theme_color.original_color;
        colors.color_text_2 = theApp.m_app_setting_data.theme_color.light1;
        colors.color_text_heighlight = theApp.m_app_setting_data.theme_color.dark1;
        colors.color_back = ColorTable::WHITE;
        colors.color_lyric_back = theApp.m_app_setting_data.theme_color.light3;
        colors.color_control_bar_back = theApp.m_app_setting_data.theme_color.light3;
        colors.color_spectrum = theApp.m_app_setting_data.theme_color.original_color;
        colors.color_spectrum_cover = theApp.m_app_setting_data.theme_color.original_color;
        colors.color_spectrum_back = theApp.m_app_setting_data.theme_color.light3;
        colors.color_button_back = theApp.m_app_setting_data.theme_color.light2;
        colors.color_button_pressed = theApp.m_app_setting_data.theme_color.dark1;
    }

    return colors;
}

bool CPlayerUIHelper::IsMidiLyric()
{
    return CPlayer::GetInstance().IsMidi() && theApp.m_general_setting_data.midi_use_inner_lyric && !CPlayer::GetInstance().MidiNoLyric();
}

bool CPlayerUIHelper::IsDrawStatusBar()
{
    return theApp.m_ui_data.always_show_statusbar || CPlayer::GetInstance().m_loading || (theApp.IsMeidaLibUpdating() && theApp.m_media_num_added > 0)
		/*|| CPlayer::GetInstance().GetABRepeatMode() != CPlayer::AM_NONE*/;

}
