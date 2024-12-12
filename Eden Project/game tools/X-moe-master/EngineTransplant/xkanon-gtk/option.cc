/* option.cc
 *   コマンドライン・オプションを得る
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
#include<gtk/gtk.h>
#include"system.h"
#include"senario.h"

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

void parse_option(int* argc, char*** argv, AyuSys& local_system,
		  char** fontname, int* _fontsize, char** _savepath) {
	GOptionContext *context;
	gboolean no_music = FALSE;
	gboolean no_mixer = FALSE;
	gboolean with_text = FALSE;
	gchar* path = NULL;
	gchar* savepath = NULL;
	gchar* title = NULL;
	gint avg32_version = -1;
	gchar* mouse_pos = NULL;
	gchar* patch = NULL;
	gint speed = 0;
	gint pcmrate = 0;
	gchar* font = NULL;
	gint fontsize = 0;
	gchar* pcmdev = NULL;
	gchar* mixdev = NULL;
	gchar* cddev = NULL;
	gboolean help = FALSE;
#ifdef VERSION
	gboolean version = FALSE;
#endif
	const GOptionEntry entries[] = {
		{ "no-music", 0, 0, G_OPTION_ARG_NONE, &no_music, "Disable music", NULL },
		{ "no-mixer", 0, 0, G_OPTION_ARG_NONE, &no_mixer, "Disable mixer", NULL },
		{ "with-text", 0, 0, G_OPTION_ARG_NONE, &with_text, "Dump text", NULL },
		{ "path", 'p', 0, G_OPTION_ARG_STRING, &path, "Specify directory where SEEN.TXT is in", NULL },
		{ "savepath", 0, 0, G_OPTION_ARG_STRING, &savepath, "Specify location of save files", NULL },
		{ "title", 0, 0, G_OPTION_ARG_STRING, &title, "Set Window caption", NULL },
		{ "avg32-version", 0, 0, G_OPTION_ARG_INT, &avg32_version, "Specify AVG32 version", NULL },
		{ "mouse-pos", 0, 0, G_OPTION_ARG_STRING, &mouse_pos, "Set pointer position of mouse cursor", NULL },
		{ "patch", 0, 0, G_OPTION_ARG_STRING, &patch, "Apply patch on xayusys_gtk2", NULL },
		{ "speed", 0, 0, G_OPTION_ARG_INT, &speed, "Set text speed", NULL },
		{ "pcmrate", 0, 0, G_OPTION_ARG_INT, &pcmrate, "Set PCM rate", NULL },
		{ "font", 'f', 0, G_OPTION_ARG_STRING, &font, "Specify font", NULL },
		{ "fontsize", 0, 0, G_OPTION_ARG_INT, &fontsize, "Specify font size", NULL },
		{ "pcmdev", 0, 0, G_OPTION_ARG_FILENAME, &pcmdev, "Specify audio device", NULL },
		{ "mixdev", 0, 0, G_OPTION_ARG_FILENAME, &mixdev, "Specify mixer device", NULL },
		{ "cddev", 0, 0, G_OPTION_ARG_FILENAME, &cddev, "Specify CDROM device", NULL },
		{ "help", 'h', 0, G_OPTION_ARG_NONE, &help, "Show help message", NULL },
#ifdef VERSION
		{ "version", 'v', 0, G_OPTION_ARG_NONE, &version, "Show version information", NULL },
#endif
		{ NULL },
	};
	GError* err = NULL;

	// parse option
	context = g_option_context_new("xkanon GTK2");
	g_option_context_add_main_entries(context, entries, NULL);
	g_option_context_add_group(context, gtk_get_option_group(TRUE));
	// g_option_context_set_help_enabled(context, FALSE);
	g_option_context_set_help_enabled(context, TRUE);
	g_option_context_parse(context, argc, argv, &err);

	if (err) {
		g_printerr("Error handling option: %s\n", err->message);
		g_error_free(err);
		exit(1);
	}
	if (help) {
		/* set_help_enabled(context, TRUE)だと文字列をUTF-8の
		 * まま出してしまうので、このようにメッセージを出す
		 */
		// const gchar* msg = g_option_context_get_help(context, TRUE, NULL);
		// g_print("%s", msg);
		exit(0);
	}
# ifdef VERSION
	if (version) {
		g_print("xayusys version " VERSION "\n");
		exit(0);
	}
#endif
	g_option_context_free(context);

	// option evaluation
	if (no_music)
		local_system.DisableMusic();
	if (no_mixer)
		local_system.SetWaveMixer(0);
	if (with_text)
		local_system.SetTextDump();
	if (path)
		;
	if (savepath && _savepath)
		*_savepath = savepath;
	if (title) {
		char buf[200];
		kconv_rev((unsigned char*)(title), (unsigned char*)(buf));
		local_system.config->SetParaStr("#CAPTION",buf);
	}
	if (avg32_version >= 0 && avg32_version <= 3)
		local_system.SetVersion(avg32_version);
	else if (local_system.Version() == -1) // version string is not set yet
		local_system.SetVersion(1);
	if (mouse_pos) {
		if (strcmp(mouse_pos, "lt") == 0) {
			local_system.SetMouseCursorPos(0,0);
		} else if (strcmp(mouse_pos, "lb") == 0) {
			local_system.SetMouseCursorPos(0,32);
		} else if (strcmp(mouse_pos, "rt") == 0) {
			local_system.SetMouseCursorPos(32,0);
		} else if (strcmp(mouse_pos, "rb") == 0) {
			local_system.SetMouseCursorPos(32,32);
		}
	}
	if (patch)
		SENARIO_PATCH::AddPatch(patch);
	if (speed > 0) {
		if (speed > 1000) speed = 1000;
		local_system.SetTextSpeed(speed);
	}
	if (pcmrate > 0)
		local_system.SetPCMRate(pcmrate);
	if (font && fontname)
		*fontname = font;
	if (fontsize > 0 && _fontsize)
		*_fontsize = fontsize;
	if (pcmdev)
		local_system.SetPCMDevice(pcmdev);
	if (mixdev)
		local_system.SetMixDevice(mixdev);
	if (cddev)
		local_system.SetCDROMDevice(cddev);
}

