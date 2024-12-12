/* system_music_stab.cc
 *     音楽関係の操作を無効にするための stab ファイル
 */
/*
 *
 *  Copyright (C) 2000-   Kazunori Ueno(JAGARL) <jagarl@creator.club.ne.jp>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
*/

#include"system.h"

void AyuSys::SetCDROMDevice(char* dev){}
void AyuSys::SetPCMDevice(char* dev){}
void AyuSys::SetMixDevice(char* dev){}
void AyuSys::SetPCMRate(int rate) {}

void AyuSys::PlayCDROM(char* track){}
void AyuSys::PlayKoe(const char* p){}
void AyuSys::StopKoe(void){}
bool AyuSys::IsStopKoe(void){}

void AyuSys::PlayMovie(char* track, int x1, int y1, int x2, int y2, int c){}
void AyuSys::StopMovie(void){}
void AyuSys::PauseMovie(void){}
void AyuSys::ResumeMovie(void){}
void AyuSys::WaitStopMovie(int c){}

void AyuSys::StopCDROM(void) {}

void AyuSys::FadeCDROM(int) { }
void AyuSys::WaitStopCDROM(void) { }

void AyuSys::PlayWave(char* fname) {
	return;
}

void AyuSys::StopWave(void) {
}
void AyuSys::WaitStopWave(void) { }
void AyuSys::PlaySE(int number) {}
void AyuSys::StopSE(void) {}
void AyuSys::WaitStopSE(void) {};

void AyuSys::InitMusic(void)
{
}

void AyuSys::FinalizeMusic(void)
{
}

TrackName::TrackName(void) {
}
TrackName::~TrackName() {
}
void TrackName::AddCDROM(char* name, int track) {
}
void TrackName::AddWave(char* name, char* track) {
}
void TrackName::AddSE(int n, char* t) {
}
int TrackName::CDTrack(char* name) {
	return -1;
}
const char* TrackName::WaveTrack(char* name) {
	return 0;
}
const char* TrackName::SETrack(int n) {
	return 0;
}
void AyuSys::SyncMusicState(void) {
}
void AyuSys::DisableMusic(void) {
}
void AyuSys::SetWaveMixer(int n) {
}

extern "C" void mus_exit(int is_abort) {}
