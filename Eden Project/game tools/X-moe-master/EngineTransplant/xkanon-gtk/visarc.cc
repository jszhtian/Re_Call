#define CMDNAME "visarc"
#define VERSION "1.00"

/*****************************************
**	Visual Arts の圧縮書庫ファイルを
**	展開する
**
**	usage : visarc x <arcfile> <file> [<file> ...]
**		visarc l <arcfile>
**		visarc g <arcfile> <graphic file>
**		visarc m <arcfile> <mask file>
**
******************************************
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

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

#include <stdio.h>
#include <unistd.h>
#include <string.h>

// use only file subsystem
#define NO_AYU_SYSTEM
#include "file.h"
#include "file.cc"
#ifdef HAVE_LIBPNG
#include <png.h>
typedef enum {gray, rgb, rgba} color_mode;
#endif

void usage(void) {
	fprintf(stderr, "usage : visarc <cmd> [-2k|-raf] <arcfile> [<file1> [<file2> [...] ]]\n");
	fprintf(stderr, "           cmd :   x : extract\n");
	fprintf(stderr, "                   l : list all files\n");
	fprintf(stderr, "           option:  -2k : file is avg2000/reallive's archive\n");
	fprintf(stderr, "                    -raf: file is 'sorechiru/raffresia' archive\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "usage2: visarc <cmd> <pdt-file> [<output-file>]\n");
	fprintf(stderr, "           cmd :   r : unpack pdt file and save as raw data\n");
	fprintf(stderr, "                   m : unpack mask and save as raw data\n");
#ifdef HAVE_LIBPNG
	fprintf(stderr, "                   p : unpack pdt file and save as png file\n");
	fprintf(stderr, "                   g : unpack mask and save as gray scale png file\n");
	fprintf(stderr, "                   a : unpack pdt file and save as png RGBA file\n");
#endif // HAVE_LIBPNG
	fprintf(stderr, "\n");
}

enum {ARC_AVG32, ARC_AVG2k, ARC_Raffresia} is_arc_2k = ARC_AVG32;

void List(char* path) {
	ARCFILE* file = is_arc_2k==ARC_AVG32 ? new ARCFILE(path) : (is_arc_2k == ARC_AVG32 ? new SCN2kFILE(path) : new RaffresiaFILE(path) );
	file->Init();
	file->ListFiles(stdout);
	delete file;
	return;
}

void ExtractOne(ARCFILE* arc, char* file) {
	ARCINFO* info = arc->Find(file,"");
	if (info == 0) {
		fprintf(stderr, "Cannot find file %s in archive\n",file);
		return;
	}
	FILE* out = fopen(file, "w");
	if (out == 0) {
		delete info;
		fprintf(stderr, "Cannot open output file %s\n",file);
		return;
	}
	
	fprintf(stdout, "Extracting %s ... ",file);
	int size = info->Size();
	const char* data = info->Read();
	fwrite(data, size, 1, out);
	fclose(out);
	fprintf(stdout, "done\n");
	delete info;
	return;
}

void Extract(char* path, char** files, int fnum) {
	ARCFILE* file = is_arc_2k==ARC_AVG32 ? new ARCFILE(path) : (is_arc_2k == ARC_AVG32 ? new SCN2kFILE(path) : new RaffresiaFILE(path) );
	file->Init();
	if (files != 0 && fnum != 0) {
		int i; for (i=0; i<fnum; i++)
			ExtractOne(file, files[i]);
	} else {
		file->InitList();
		char* path; while( (path=file->ListItem()) != 0)
			ExtractOne(file, path);
	}
	delete file;
	return;
}

void ChangeExt(char* path, char* new_ext, char* buf) {
	char* name = strrchr(path, DIR_SPLIT);
	if (name == 0) name = path;
	else name++;
	int path_len = name - path;

	char* ext = strrchr(name, '.');
	int ext_len;
	if (ext) ext_len = ext - name;
	else ext_len = strlen(name);

	strncpy(buf, path, path_len+ext_len);
	strcpy(buf+path_len+ext_len, new_ext);
}

char* ReadFile(char* fname, int* len) {
	FILE* in = fopen(fname, "rb");
	if (in == 0) return 0;
	fseek(in,0,2); size_t s = ftell(in); fseek(in,0,0);
	char* buf = new char[s];
	fread(buf,s,1,in);
	fclose(in);
	if (len) *len = s;
	return buf;
}

void ExtractGraphic(char* path,char* outpath = 0) {
	char buf[1024]; char* fname = buf;
	int len;
	char* dat = ReadFile(path, &len);
	if (dat == 0) {
		fprintf(stderr, "Cannot open PDT file : %s\n",path);
		return;
	}
	GRPCONV* conv = GRPCONV::AssignConverter(dat, len, path);
	if (conv == 0) {
		fprintf(stderr, "Invalid format\n");
		return;
	}
	char* data = (char*)conv->Read();
	if (data == 0) {
		fprintf(stderr, "Insufficient memory or bad format\n");
		delete conv;
		return;
	}
	if (outpath == 0) ChangeExt(path,".raw", buf); // path をつくる
	else fname = outpath;
	FILE* out = fopen(fname, "wb"); // ファイルを開く
	if (out == 0) {
		fprintf(stderr, "Cannot open raw file : %s\n",buf);
		delete conv;
		return;
	}
	fwrite(data, 4, conv->Width()*conv->Height(), out);
	fclose(out);
	
	delete conv;
	return;
}

void ExtractMask(char* path,char* outpath = 0) {
	char buf[1024]; char* fname = buf;
	int len;
	char* dat = ReadFile(path, &len);
	if (dat == 0) {
		fprintf(stderr, "Cannot open PDT file : %s\n",path);
		return;
	}
	GRPCONV* conv = GRPCONV::AssignConverter(dat, len, path);
	if (conv == 0) {
		fprintf(stderr, "Invalid format\n");
		return;
	}
	char* data = (char*)conv->ReadMask();
	if (data == 0) {
		fprintf(stderr, "file %s does not contain a mask.\n", path);
		return;
	}
	if (outpath == 0) ChangeExt(path,".mask.raw", buf); // path をつくる
	else fname = outpath;
	FILE* out = fopen(fname, "wb"); // ファイルを開く
	if (out == 0) {
		fprintf(stderr, "Cannot open raw file : %s\n",buf);
		delete conv;
		return;
	}
	fwrite(data, 1, conv->Width()*conv->Height(), out);
	fclose(out);
	
	delete conv;
	return;
}

#ifdef HAVE_LIBPNG
void create_png(FILE* stream, char* path, char* desc, int width, int height, char* data, color_mode cmode) {
	png_structp png_ptr;
	png_infop info_ptr;

	/* create struct */
	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (png_ptr == NULL) return;

	/* initialize information */
	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL) {
		png_destroy_write_struct(&png_ptr, (png_infop*)NULL);
		return;
	}

	if (setjmp(png_jmpbuf(png_ptr))) {
		/* error occured !! */
		png_destroy_write_struct(&png_ptr,&info_ptr);
		fprintf(stderr, "Get error while processing PNG from file %s\n",path);
		return;
	}

	/* initialize I/O (for stream) */
	png_init_io(png_ptr, stream);

	/* initialize headers */
	png_set_IHDR(png_ptr, info_ptr,
		width, height, 8 /* bit_dept */,
		cmode == gray ? PNG_COLOR_TYPE_GRAY :
			cmode == rgb ? PNG_COLOR_TYPE_RGB : PNG_COLOR_TYPE_RGB_ALPHA,
		PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
	/* create text information */
	png_text info_text[3];
	info_text[0].key = "Title";
	info_text[0].text= path;
	info_text[0].compression = PNG_TEXT_COMPRESSION_NONE;
	info_text[1].key = "Author";
	info_text[1].text= CMDNAME " version " VERSION;
	info_text[1].compression = PNG_TEXT_COMPRESSION_NONE;
	info_text[2].key = "Description";
	info_text[2].text= desc;
	info_text[2].compression = PNG_TEXT_COMPRESSION_NONE;
	png_set_text(png_ptr, info_ptr, info_text, 3);

	/* write information */
	png_write_info(png_ptr, info_ptr);

	/* write body */
	if (cmode == gray) {
		/* gray scale */
		int i; for (i=0; i<height; i++) {
			png_write_rows(png_ptr, (png_bytep*)&data, 1);
			data += width;
		}
	} else if (cmode == rgb) {
		/* rgb image ; input is 32bpp -> output is 24 bpp.*/
		char* row = new char[width*3];
		int i; for (i=0; i<height; i++) {
			char* row_ptr = row;
			int j; for (j=0; j<width; j++) {
				row_ptr[0] = data[2];
				row_ptr[1] = data[1];
				row_ptr[2] = data[0];
				row_ptr += 3; data += 4;
			}
			png_write_rows(png_ptr, (png_byte**)&row, 1);
		}
	} else /* if (cmode == rgba) */ {
		/* rgba image ; input/output is 32bpp.*/
		char* row = new char[width*4];
		int i; for (i=0; i<height; i++) {
			char* row_ptr = row;
			int j; for (j=0; j<width; j++) {
				row_ptr[0] = data[2];
				row_ptr[1] = data[1];
				row_ptr[2] = data[0];
				row_ptr[3] = data[3];
				row_ptr += 4; data += 4;
			}
			png_write_rows(png_ptr, (png_byte**)&row, 1);
		}
	}
	png_write_end(png_ptr, info_ptr);
	png_destroy_write_struct(&png_ptr, &info_ptr);
	return;
}

void ExtractPngGraphic(char* path,char* outpath = 0) {
	char buf[1024]; char* fname = buf;
	int len;
	char* dat = ReadFile(path, &len);
	if (dat == 0) {
		fprintf(stderr, "Cannot open PDT file : %s\n",path);
		return;
	}
	GRPCONV* conv = GRPCONV::AssignConverter(dat, len, path);
	if (conv == 0) {
		fprintf(stderr, "Invalid format\n");
		return;
	}
	char* data = (char*)conv->Read();
	if (data == 0) {
		fprintf(stderr, "Insufficient memory\n");
		delete conv;
		return;
	}
	if (outpath == 0) ChangeExt(path,".png", buf); // path をつくる
	else fname = outpath;
	FILE* out = fopen(fname, "wb"); // ファイルを開く
	if (out == 0) {
		fprintf(stderr, "Cannot open raw file : %s\n",buf);
		delete conv;
		return;
	}
	create_png(out, path, "", conv->Width(), conv->Height(), data, rgb);
	fclose(out);
	
	delete conv;
	return;
}

void ExtractPngMask(char* path,char* outpath = 0) {
	char buf[1024]; char* fname = buf;
	int len;
	char* dat = ReadFile(path, &len);
	if (dat == 0) {
		fprintf(stderr, "Cannot open PDT file : %s\n",path);
		return;
	}
	GRPCONV* conv = GRPCONV::AssignConverter(dat, len, path);
	if (conv == 0) {
		fprintf(stderr, "Invalid format\n");
		return;
	}
	char* data = (char*)conv->ReadMask();
	if (data == 0) {
		fprintf(stderr, "file %s does not contain a mask.\n",path);
		delete conv;
		return;
	}
	if (outpath == 0) ChangeExt(path,".mask.png", buf); // path をつくる
	else fname = outpath;
	FILE* out = fopen(fname, "wb"); // ファイルを開く
	if (out == 0) {
		fprintf(stderr, "Cannot open raw file : %s\n",fname);
		delete conv;
		return;
	}
	create_png(out, path, "", conv->Width(), conv->Height(), data, gray);
	fclose(out);
	
	delete conv;
	return;
}

void ExtractPngRgbaGraphic(char* path,char* outpath = 0) {
	char buf[1024]; char* fname = buf;
	int len;
	char* dat = ReadFile(path, &len);
	if (dat == 0) {
		fprintf(stderr, "Cannot open PDT file : %s\n",path);
		return;
	}
	GRPCONV* conv = GRPCONV::AssignConverter(dat, len, path);
	if (conv == 0) {
		fprintf(stderr, "Invalid format\n");
		return;
	}
	int masked = conv->IsMask();
	char* data = (char*)conv->Read();
	char* mask = masked ? (char*)conv->ReadMask() : 0;
	if (data == 0 || (masked && mask == 0)) {
		fprintf(stderr, "Insufficient memory\n");
		delete conv;
		return;
	}
	// data と mask の統合
	for (int i = 0; i < conv->Width() * conv->Height(); i++) {
		if (masked)
			data[4*i+3] = mask[i];
		else
			data[4*i+3] = 0xff;	// 不透明度を最大にする
	}
	if (outpath == 0) ChangeExt(path,".png", buf); // path をつくる
	else fname = outpath;
	FILE* out = fopen(fname, "wb"); // ファイルを開く
	if (out == 0) {
		fprintf(stderr, "Cannot open raw file : %s\n",buf);
		delete conv;
		return;
	}
	create_png(out, path, "", conv->Width(), conv->Height(), data, rgba);
  	fclose(out);
  	
	delete conv;
}
#endif // HAVE_LIBPNG

int main(int argc, char* argv[]) {
	int i; 
	fprintf(stderr, "%s version %s\n", CMDNAME, VERSION);
	if (argc < 3) { 
		usage(); return -1;
	}
	if (strlen(argv[1]) != 1) {
		usage(); return -1;
	}
	for (i=2; i<argc; i++) { /* search -2k option */
		if (strcmp(argv[i], "-2k") == 0) {
			is_arc_2k = ARC_AVG2k;
		} else if (strcmp(argv[i], "-raf") == 0) {
			is_arc_2k = ARC_Raffresia;
		} else continue;
		/* option を削る */
		argc--;
		int j; for (j=i; j<argc; j++) argv[j] = argv[j+1];
	}
	switch(argv[1][0]) {
		case 'x': case 'X':
			if (argc < 4) Extract(argv[2], 0, -1);
			else Extract(argv[2], argv+3, argc-3);
			break;
		case 'l': case 'L':
			List(argv[2]);
			break;
		case 'r': case 'R':
			if (argc < 4)
				ExtractGraphic(argv[2]);
			else
				ExtractGraphic(argv[2],argv[3]);
			break;
case 'n': { for (i=2; i<argc; i++) ExtractGraphic(argv[i],"/dev/null");break;}
		case 'm': case 'M':
			if (argc < 4)
				ExtractMask(argv[2]);
			else
				ExtractMask(argv[2],argv[3]);
			break;
#ifdef HAVE_LIBPNG
		case 'p': case 'P':
			if (argc < 4)
				ExtractPngGraphic(argv[2]);
			else
				ExtractPngGraphic(argv[2],argv[3]);
			break;
		case 'g': case 'G':
			if (argc < 4)
				ExtractPngMask(argv[2]);
			else
				ExtractPngMask(argv[2],argv[3]);
			break;
		case 'a': case 'A':
			if (argc < 4)
				ExtractPngRgbaGraphic(argv[2]);
			else
				ExtractPngRgbaGraphic(argv[2],argv[3]);
			break;
#endif // HAVE_LIBPNG
		default:
			usage(); return -1;
	}
	return 0;
}
