/*  senario_dumo.cc
 *     シナリオファイルの内容を表示するプログラム
 *
 *     オプションなどは普通と同じ
 *     引数無しで起動するとファイルのリストが出る
 *     なお、出力はすべて SJIS コード
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

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include "senario.h"
#include "initial.h"


int main(int argc, char *argv[])
{
	Initialize::Exec();
#ifdef DEFAULT_DATAPATH
	char* datpath = DEFAULT_DATAPATH;
#else
	char* datpath = "/tmp/kanon";
#endif
	/* まずはじめに gameexe.ini の存在するパスを調べる */
	int i;
	for (i=1; i<argc; i++) {
		if (strcmp(argv[i], "--path") == 0) {
			datpath = argv[i+1];
		}
	}
	// config ファイルの読み込み
	if (file_searcher.InitRoot(datpath) == -1) {
		fprintf(stderr, "Cannot use %s as root directory ; it cannot be read or there is no dat/ directory.\n",datpath);
		return -1;
	}
	global_system.LoadInitFile();
	parse_option(argc,argv,global_system);
	if (argc < 2) {
		// シナリオをリストアップする
		SENARIO* s = new SENARIO(0, global_system);
		int* list = SENARIO::ListSeens();
		if (list == 0) {
			printf("Error : cannot open senario\n");
			return -1;
		}
		for (i=0; list[i]!=-1; i++) {
			char* name = s->GetTitle(list[i]);
			if (name)
				printf("%3d : %s\n",list[i], name);
			else
				printf("%3d : (No title)\n",list[i]);
		}
		delete list;
		delete s;
		return 0;
	} else if (argc == 2 && strcmp(argv[1],"all") == 0) {
		// 全ファイルをダンプ
		SENARIO* s = new SENARIO(0, global_system);
		int* list = SENARIO::ListSeens();
		if (list == 0) {
			printf("Error : cannot open senario\n");
			return -1;
		}
		for (i=0; list[i]!=-1; i++) {
			printf("Senario number %d\n",list[i]);
			GlobalStackItem item; item.SetGlobal(list[i],0);
			s->Play(item);
		}
		delete list;
		delete s;
		return 0;
	}
	for (i=1; i<argc; i++) {
		int n = atoi(argv[i]);
		if (n == 0) continue;
		SENARIO* s = new SENARIO(0, global_system);
		GlobalStackItem item; item.SetGlobal(n,0);
		s->Play(item);
		delete s;
	}
	return 0;
}

#include "system_graphics_stab.cc"
#include "system_music_stab.cc"
#include "window_stab.cc"

// senario の save まわり

int SENARIO::IsSavefileExist(void) { return 1;}
void SENARIO::CreateSaveFile(void) {}
void SENARIO::ReadSaveHeader(void) {}
void SENARIO::WriteSaveHeader(void) {}
void SENARIO::ClearReadFlag(void) {}
void SENARIO::WriteReadFlag(void) {}
void SENARIO::ReadReadFlag(void) {}
