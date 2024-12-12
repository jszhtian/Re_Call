/* Copyright (C) 2019 Nunuhara Cabbage <nunuhara@haniwa.technology>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://gnu.org/licenses/>.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <unistd.h>
#include <sys/stat.h>
#include "system4.h"

#if (defined(_WIN32) || defined(__WIN32__))
#include <Windows.h>
#include <direct.h>
static int make_dir(const char *path, possibly_unused int mode)
{
	int nr_wchars = MultiByteToWideChar(CP_UTF8, 0, path, -1, NULL, 0);
	wchar_t *wpath = xmalloc(nr_wchars * sizeof(wchar_t));
	MultiByteToWideChar(CP_UTF8, 0, path, -1, wpath, nr_wchars);

	int r = _wmkdir(wpath);
	free(wpath);
	return r;
}
#else
#define make_dir(path, mode) mkdir(path, mode)
#endif

FILE *file_open_utf8(const char *path, const char *mode)
{
#ifdef _WIN32
	int nr_wchars = MultiByteToWideChar(CP_UTF8, 0, path, -1, NULL, 0);
	wchar_t *wpath = xmalloc(nr_wchars * sizeof(wchar_t));
	MultiByteToWideChar(CP_UTF8, 0, path, -1, wpath, nr_wchars);

	wchar_t wmode[64];
	mbstowcs(wmode, mode, 64);

	FILE *f = _wfopen(wpath, wmode);
	free(wpath);
	return f;
#else
	return fopen(path, mode);
#endif
}

void *file_read(const char *path, size_t *len_out)
{
	FILE *fp;
	long len;
	uint8_t *buf;

	if (!(fp = fopen(path, "rb")))
		return NULL;

	fseek(fp, 0, SEEK_END);
	len = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	buf = xmalloc(len + 1);
	if (fread(buf, len, 1, fp) != 1) {
		free(buf);
		return NULL;
	}
	if (fclose(fp)) {
		free(buf);
		return NULL;
	}

	if (len_out)
		*len_out = len;

	buf[len] = 0;
	return buf;
}

bool file_exists(const char *path)
{
	return access(path, F_OK) != -1;
}

// Adapted from http://stackoverflow.com/a/2336245/119527
int mkdir_p(const char *path)
{
	const size_t len = strlen(path);
	char _path[PATH_MAX];
	char *p;

	errno = 0;

	// Copy string so its mutable
	if (len > sizeof(_path)-1) {
		errno = ENAMETOOLONG;
		return -1;
	}
	strcpy(_path, path);

	// Iterate the string
	for (p = _path + 1; *p; p++) {
		if (*p == '/') {
			// Temporarily truncate
			*p = '\0';

			if (make_dir(_path, S_IRWXU) != 0) {
				if (errno != EEXIST)
					return -1;
			}

			*p = '/';
		}
	}

	if (make_dir(_path, S_IRWXU) != 0) {
		if (errno != EEXIST)
			return -1;
	}

	return 0;
}
