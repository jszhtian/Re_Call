/* opt_games.cc
 *   ゲームごとに特定のオプション
 */

/*
 *
 *  Copyright (C) 2001-   Kazunori Ueno(JAGARL) <jagarl@creator.club.ne.jp>
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

#include<string.h>
#include<stdlib.h>
#include"system.h"
#include"senario.h"

extern const char** game_opt[];

void set_game(const char* key, AyuSys& local_system) {
	if (key == 0) return;
	/* key を探す */
	char key_euc[1024];
	kconv((unsigned char*)key, (unsigned char*)key_euc);
	const char*** opt = game_opt;
	while(*opt != 0 && strcmp(**opt, key_euc) != 0) opt++;
	if (*opt == 0) return;
	/* opt のコピー */
	int i; for (i=0; (*opt)[i] != 0; i++) ;
	int argc = i; int argc_orig = i;
	char** copy_opt = new char*[argc+1];
	for (i=0; i<argc; i++) {
		copy_opt[i] = strdup( (*opt)[i]);
	}
	copy_opt[argc] = 0;
	parse_option(&argc, &copy_opt, local_system, 0, 0, 0);
	/* GOptionParserは文字列を取り込まないので開放してよい
	**
	** original comment:
	**   parse_option で一部の文字列がシステムに取り込まれている
	**   ことがあるので、opt は消去しない
	**   (数十バイトのメモリリークは無視)
	*/
	for (i=0; i<argc_orig; i++)
	 	free(copy_opt[i]);

	delete[] copy_opt;
}

/* 始めの引数は gameexe.ini 中の REGNAME= に指定されているもの。\ を二重にするのを忘れないこと。
** 残りは普通に xayusys を起動するのに必要な引数。末尾の 0 を忘れないこと
*/
const char* opt_adieu[] = { "CRAFT\\ADIEU", "--avg32-version", "3", "--mouse-pos", "lt", "--pcmrate", "22050", 0}; /* さよならを教えて */
const char* opt_air[] = { "KEY\\AIR", "--avg32-version", "3", 0}; /* AIR */
const char* opt_air2[] = { "KEY\\DEMO\\AIR_01", "--avg32-version", "2", 0}; /* AIR デモ */
const char* opt_air_all[] = { "KEY\\AIR_ALL", "--avg32-version", "3", 0}; /* AIR全年齢対象版 */
const char* opt_ssd[] = { "13CM\\SUKI", "--avg32-version", "0", "--mouse-pos", "lt", "--pcmrate", "22050", 0}; /* 好き好き大好き */
const char* opt_floreal[] = { "13CM\\フロレアール", "--avg32-version", "1", "--mouse-pos", "lt", "--pcmrate", "22050", 0}; /* フロレアール　〜　好き好き大好き */
const char* opt_kanon[] = {"KEY\\KANON", "--avg32-version", "1", "--patch", "opening", 0}; /* KANON */
const char* opt_kanon_all[] = {"KEY\\KANON_ALL", "--avg32-version", "2", "--patch", "opening", 0}; /* KANON 全年齢版 */
const char* opt_senseoff[] = {"OTHERWISE\\SENSEOFF", "--avg32-version", "2", "--mouse-pos","lt", 0}; /* sense off */
const char* opt_senseoff2[] = {"OTHERWISE\\SENSEOFF_T", "--avg32-version", "3", "--mouse-pos","lt", 0}; /* sense off ドラマCD */
const char* opt_senseoff3[] = {"OTHERWISE\\SO_DEMO", "--avg32-version", "2", "--mouse-pos","lt", 0}; /* sense off デモ */
const char* opt_susuki2[] = { "KUR-MAR-TER\\DEMO\\SUSUKI_N", "--avg32-version", "3", 0}; /* ススキノハラの約束デモ */
const char* opt_bless[] = { "BASIL\\BLESS", "--avg32-version", "3", "--mouse-pos", "lt", 0}; /* bless */
const char* opt_bless2[] = { "BASIL\\SORE", "--avg32-version", "3", "--mouse-pos", "lt", 0}; /* それは舞い散る桜のように */
const char* opt_bless3[] = { "NAVEL\\SHUFFLE", "--avg32-version", "3", "--mouse-pos", "lt", 0}; /* SHUFFLE! */
const char* opt_renchu2[] = { "SAGAPLANETS\\DEMO\\REN", "--avg32-version", "3", 0}; /* 恋愛Chu! デモ */
const char* opt_kfdemo[] = {"OTHERWISE\\KF_DEMO", "--avg32-version", "3", 0}; /* 未来にキスを デモ */
const char* opt_hinauta[] = {"enfini\\雛ちゃんの唄声", "--avg32-version", "3", "--mouse-pos","lt","--pcmrate","22050", 0}; /* 雛ちゃんの唄声 */
const char* opt_misswo[] = {"OTHERWISE\\KISS_THE_FUTURE", "--avg32-version", "3", "--mouse-pos", "lt", 0}; /* 未来にキスを */

const char** game_opt[] = {
	opt_adieu,
	opt_air,
	opt_air2,
	opt_air_all,
	opt_ssd,
	opt_floreal,
	opt_kanon,
	opt_kanon_all,
	opt_senseoff,
	opt_senseoff2,
	opt_senseoff3,
	opt_susuki2,
	opt_bless,
	opt_bless2,
	opt_bless3,
	opt_renchu2,
	opt_kfdemo,
	opt_hinauta,
	opt_misswo,
	0
};
