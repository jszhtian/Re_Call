#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>

#include "GIM.h"  //GIM structure

typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned int   u32;


u8 texbuff[4*1024*1024];
u8 pltbuff[1*1024*1024];

////////////////////////////////////////////////////////////////////////////


char color_format[][16] = {
	"0 : RGBA5650",
	"1 : RGBA5551",
	"2 : RGBA4444",
	"3 : RGBA8888",
	"4 : INDEX4  ",
	"5 : INDEX8  ",
	"6 : INDEX16 ",
	"7 : INDEX32 ",
	"8 : DXT1    ",
	"9 : DXT3    ",
	"10: DXT5    ",
	"11: BAD_format",
	"12: BAD_format",
	"13: BAD_format",
	"14: BAD_format",
	"15: BAD_format",
};


////////////////////////////////////////////////////////////////////////////

int save_png(char *base_name, int format, int bpp, u8 *data, int llen, int width, int height, u8 *plt, int nplt)
{
	png_structp png_ptr;
	png_infop info_ptr;
	char png_name[256];
	FILE *fp;

	int i;

	sprintf(png_name, "%s.png", base_name);
	fp = fopen(png_name, "wb");

	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if(png_ptr==NULL){
		printf("PNG error create png_ptr!\n");
		return -1;
	}

	info_ptr = png_create_info_struct(png_ptr);
	if(info_ptr==NULL){
		printf("PNG error create info_ptr!\n");
		png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
		return (-1);
	}

	png_init_io(png_ptr, fp);

	/* write header */
	png_set_IHDR(png_ptr, info_ptr, width, height, bpp,
				format, PNG_INTERLACE_NONE,
				PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

	if(nplt) {
		png_colorp pcolor;
		char *palpha;
		int alpha;

		/* write palette */
		pcolor = (png_colorp)malloc(nplt*sizeof(png_color));
		for(i=0; i<nplt; i++){
			pcolor[i].red   = plt[i*4+0];
			pcolor[i].green = plt[i*4+1];
			pcolor[i].blue  = plt[i*4+2];
		}
		png_set_PLTE(png_ptr, info_ptr, pcolor, nplt);
		free(pcolor);

		/* write alpha of palette */
		alpha = 0;
		palpha = (char*)malloc(nplt);
		for(i=0; i<nplt; i++){
			palpha[i] = plt[i*4+3];
			alpha += plt[i*4+3];
		}
		if(alpha){
			png_set_tRNS(png_ptr, info_ptr, (png_bytep)palpha, nplt, (png_color_16p)0);
		}
		free(palpha);
	}

	png_write_info(png_ptr, info_ptr);

	/* write image data */
	for(i=0; i<height; i++){
		png_write_row(png_ptr, data+i*llen);
	}
	png_write_end(png_ptr, info_ptr);

	png_destroy_write_struct(&png_ptr, &info_ptr);
	fclose(fp);

	return 0;
}

////////////////////////////////////////////////////////////////////////////



void plt_to_rgba(u8 *plt, int ptype, int num)
{
	int i;
	u8 *dst = (u8*)pltbuff;

	for(i=0; i<num; i++){
		switch(ptype){
		case GIM_FORMAT_RGBA5650:
			dst[i*4+0] = (plt[0]&0x1f)<<3;
			dst[i*4+1] = ((plt[1]&0x07)<<5) | (plt[0]&0xe0)>>3;
			dst[i*4+2] = (plt[1]&0xf8);
			dst[i*4+3] = 0;
			plt += 2;
			break;
		case GIM_FORMAT_RGBA5551:
			dst[i*4+0] = (plt[0]&0x1f)<<3;
			dst[i*4+1] = ((plt[1]&0x03)<<6) | (plt[0]&0xe0)>>2;
			dst[i*4+2] = (plt[1]&0x7c)<<1;
			dst[i*4+3] = (plt[1]&0x80)? 0xff : 0x00;
			plt += 2;
			break;
		case GIM_FORMAT_RGBA4444:
			dst[i*4+0] = (plt[0]&0x0f)<<4;
			dst[i*4+1] = (plt[0]&0xf0);
			dst[i*4+2] = (plt[1]&0x0f)<<4;
			dst[i*4+3] = (plt[1]&0xf0);
			plt += 2;
			break;
		case GIM_FORMAT_RGBA8888:
			dst[i*4+0] = plt[0];
			dst[i*4+1] = plt[1];
			dst[i*4+2] = plt[2];
			dst[i*4+3] = plt[3];
			plt += 4;
			break;
		default:
			printf("Unkown palette type! %02x\n", ptype);
			exit(1);
		}
	}
}

void RGBA5650_to_png(char *name, u8 *src, int width, int height, int pitch, u8 *plt, int ptype, int nplt, int order)
{
	u8 *dst = texbuff;
	int h, v, x, y, ofs;

	int tw = (order)? 8 : width;
	int th = (order)? 8 : height;
	int bpp = 24;
	int format = PNG_COLOR_TYPE_RGB;
	int dlen = pitch*3/2;
	if((width*2)<16) tw = 16/2;

	for(v=0; v<height; v+=th){
		for(h=0; h<width; h+=tw){
			for (y=0,ofs=0; y<th; y++){
				for (x=0; x<tw; x+=1){
					dst[ofs+x*3+0] = (src[0]&0x1f)<<3;
					dst[ofs+x*3+1] = ((src[1]&0x07)<<5) | (src[0]&0xe0)>>3;
					dst[ofs+x*3+2] = (src[1]&0xf8);
					src += 2;
				}
                ofs += dlen;
            }
            dst += tw*bpp/8;
        }
        dst += dlen*(th-1);
    }

	save_png(name, format, bpp/3, texbuff, dlen, width, height, NULL, 0);
}

void RGBA5551_to_png(char *name, u8 *src, int width, int height, int pitch, u8 *plt, int ptype, int nplt, int order)
{
	u8 *dst = texbuff;
	int h, v, x, y, ofs;

	int tw = (order)? 8 : width;
	int th = (order)? 8 : height;
	int bpp = 32;
	int format = PNG_COLOR_TYPE_RGB_ALPHA;
	int dlen = pitch*2;
	if((width*2)<16) tw = 16/2;

	for(v=0; v<height; v+=th){
		for(h=0; h<width; h+=tw){
			for (y=0,ofs=0; y<th; y++){
				for (x=0; x<tw; x+=1){
					dst[ofs+x*4+0] = (src[0]&0x1f)<<3;
					dst[ofs+x*4+1] = ((src[1]&0x03)<<6) | (src[0]&0xe0)>>2;
					dst[ofs+x*4+2] = (src[1]&0x7c)<<1;
					dst[ofs+x*4+3] = (src[1]&0x80)? 0xff : 0x00;
					src += 2;
				}
                ofs += dlen;
            }
            dst += tw*bpp/8;
        }
        dst += dlen*(th-1);
    }

	save_png(name, format, bpp/4, texbuff, dlen, width, height, NULL, 0);
}

void RGBA4444_to_png(char *name, u8 *src, int width, int height, int pitch, u8 *plt, int ptype, int nplt, int order)
{
	u8 *dst = texbuff;
	int h, v, x, y, ofs;

	int tw = (order)? 8 : width;
	int th = (order)? 8 : height;
	int bpp = 32;
	int format = PNG_COLOR_TYPE_RGB_ALPHA;
	int dlen = pitch*2;
	if((width*2)<16) tw = 16/2;

	for(v=0; v<height; v+=th){
		for(h=0; h<width; h+=tw){
			for (y=0,ofs=0; y<th; y++){
				for (x=0; x<tw; x+=1){
					dst[ofs+x*4+0] = (src[0]&0x0f)<<4;
					dst[ofs+x*4+1] = (src[0]&0xf0);
					dst[ofs+x*4+2] = (src[1]&0x0f)<<4;
					dst[ofs+x*4+3] = (src[1]&0xf0);
					src += 2;
				}
                ofs += dlen;
            }
            dst += tw*bpp/8;
        }
        dst += dlen*(th-1);
    }

	save_png(name, format, bpp/4, texbuff, dlen, width, height, NULL, 0);
}

void RGBA8888_to_png(char *name, u8 *src, int width, int height, int pitch, u8 *plt, int ptype, int nplt, int order)
{
	u8 *dst = texbuff;
	int h, v, x, y, ofs;

	int tw = (order)? 4 : width;
	int th = (order)? 8 : height;
	int bpp = 32;
	int format = PNG_COLOR_TYPE_RGB_ALPHA;
	int dlen = pitch;
	if((width*4)<16) tw = 16/4;

	for(v=0; v<height; v+=th){
		for(h=0; h<width; h+=tw){
			for (y=0,ofs=0; y<th; y++){
				for (x=0; x<tw; x+=1){
					dst[ofs+x*4+0] = src[0];
					dst[ofs+x*4+1] = src[1];
					dst[ofs+x*4+2] = src[2];
					dst[ofs+x*4+3] = src[3];
					src += 4;
				}
                ofs += dlen;
            }
            dst += tw*bpp/8;
        }
        dst += dlen*(th-1);
    }

	save_png(name, format, bpp/4, texbuff, dlen, width, height, NULL, 0);
}
// bpp/4 -> color bits for each components.

void INDEX4_to_png(char *name, u8 *src, int width, int height, int pitch, u8 *plt, int ptype, int nplt, int order)
{
	u8 *dst = texbuff;
	int h, v, x, y, ofs;

	int tw = (order)? 32 : width;
	int th = (order)? 8 : height;
	int exp = tw%32;
	if(exp!=0)
		tw+=(32-exp);
	int bpp = 4;
	int format = PNG_COLOR_TYPE_PALETTE;;
	int dlen = pitch;
//	if((width/2)<16) tw = 16*2;

	for(v=0; v<height; v+=th){
		for(h=0; h<width; h+=tw){
			for (y=0,ofs=0; y<th; y++){
				for (x=0; x<tw; x+=2){
					dst[ofs+x/2] = (src[0]<<4) | (src[0]>>4);
					src++;
				}
                ofs += dlen;
            }
            dst += tw*bpp/8;
        }
        dst += dlen*(th-1);
    }

	plt_to_rgba(plt, ptype, nplt);
	save_png(name, format, bpp, texbuff, dlen, width, height, pltbuff, nplt);
}

void INDEX8_to_png(char *name, u8 *src, int width, int height, int pitch, u8 *plt, int ptype, int nplt, int order)
{
	u8 *dst = texbuff;
	int h, v, x, y, ofs;

	int tw = (order)? 16 : width;
	int th = (order)? 8 : height;
	int exp = tw%16;
	if(exp!=0)
		tw+=(16-exp);
	int bpp = 8;
	int format = PNG_COLOR_TYPE_PALETTE;;
	int dlen = pitch;
	if((width)<16) tw = 16;

	for(v=0; v<height; v+=th){
		for(h=0; h<width; h+=tw){
			for (y=0,ofs=0; y<th; y++){
				for (x=0; x<tw; x+=1){
					dst[ofs+x] = *src++;
				}
                ofs += dlen;
            }
            dst += tw*bpp/8;
        }
        dst += dlen*(th-1);
    }

	plt_to_rgba(plt, ptype, nplt);
	save_png(name, format, bpp, texbuff, dlen, width, height, pltbuff, nplt);
}

void INDEX16_to_png(char *name, u8 *src, int width, int height, int pitch, u8 *plt, int ptype, int nplt, int order)
{
	printf("INDEX16 not implent ...\n");
}

void INDEX32_to_png(char *name, u8 *src, int width, int height, int pitch, u8 *plt, int ptype, int nplt, int order)
{
	printf("INDEX32 not implent ...\n");
}

void DXT1_to_png(char *name, u8 *src, int width, int height, int pitch, u8 *plt, int ptype, int nplt, int order)
{
	printf("DXT1 not implent ...\n");
}

void DXT3_to_png(char *name, u8 *src, int width, int height, int pitch, u8 *plt, int ptype, int nplt, int order)
{
	printf("DXT3 not implent ...\n");
}

void DXT5_to_png(char *name, u8 *src, int width, int height, int pitch, u8 *plt, int ptype, int nplt, int order)
{
	printf("DXT5 not implent ...\n");
}

void (*img_to_png[]) (char *name, u8 *src, int width, int height, int pitch, u8 *plt, int ptype, int nplt, int order) =
{
	RGBA5650_to_png,  // 0
	RGBA5551_to_png,  // 1
	RGBA4444_to_png,  // 2
	RGBA8888_to_png,  // 3
	INDEX4_to_png,    // 4
	INDEX8_to_png,    // 5
	INDEX16_to_png,   // 6
	INDEX32_to_png,   // 7
	DXT1_to_png,      // 8
	DXT3_to_png,      // 9
	DXT5_to_png,      // 10
	NULL,             // 11
	NULL,             // 12
	NULL,             // 13
	NULL,             // 14
	NULL,             // 15
};


////////////////////////////////////////////////////////////////////////////

GimChunk *chunk_next(GimChunk *chunk)
{
	return (GimChunk*)( (char *)chunk+chunk->next_offs);
}

GimChunk *chunk_child(GimChunk *chunk)
{
	return (GimChunk*)( (char *)chunk+chunk->child_offs);
}

void *chunk_data(GimChunk *chunk)
{
	return (GimChunk*)( (char *)chunk+chunk->data_offs);
}

GimChunk *chunk_find_child(GimChunk *chunk, u32 type)
{
	GimChunk *p, *end;

	if(chunk==NULL)
		return NULL;

	end = chunk_next(chunk);
	for(p=chunk_child(chunk); p<end; p=chunk_next(p)){
		if(type==p->type)
			return p;
	}

	return NULL;
}

int check_gim(u8 *buf)
{
	GimChunk *c;

	if(strncmp((char*)buf, "MIG.00.1PSP", 11))
		return 0;

	c = (GimChunk*)(buf+16);
	if(c->type!=GIM_FILE)
		return 0;

	c = chunk_find_child(c, GIM_PICTURE);
	if(c==NULL)
		return 0;

	c = chunk_find_child(c, GIM_IMAGE);
	if(c==NULL)
		return 0;

	return 1;
}

void dump_gim(u8 *buf, char *name)
{
	GimChunk *file_chunk, *pic_chunk, *img_chunk, *pal_chunk;
	GimImageHeader *img;
	GimPaletteHeader *pal;
	int f, l, i, mask, format;
	int *img_pt, *pal_pt;
	int pal_type, pal_num, pal_id;
	int pal_level, pal_frame;
	u8 *pal_data;
	int width, height, pitch;
	char tname[512];

	file_chunk = (GimChunk*)(buf+16);
	pic_chunk = chunk_find_child(file_chunk, GIM_PICTURE);

	img_chunk = chunk_find_child(pic_chunk, GIM_IMAGE);
	img = (GimImageHeader*)chunk_data(img_chunk);

	pal_chunk = chunk_find_child(pic_chunk, GIM_PALETTE);
	if(pal_chunk){
		pal = (GimPaletteHeader*)chunk_data(pal_chunk);
		pal_pt = (int*)((u8*)pal+pal->offsets);
		pal_num = pal->width;
		pal_type = pal->format;
	}else{
		pal = NULL;
		pal_pt = NULL;
		pal_num = 0;
		pal_type = 0;
	}

	if(pal_chunk){
		printf("PALETTE: ");
		mask = pal->pitch_align-1;
		format = pal->format&0x0f;
		printf(" type:%s %dx%dx%d align:%d level:%d frame:%d\n", color_format[format],
						pal->width, pal->height, pal->bpp, mask+1, pal->level_count, pal->frame_count);
	}

	printf("IMAGE:   ");

	mask = img->pitch_align-1;
	format = img->format&0x0f;
	printf(" type:%s order:%d  %dx%dx%d align:%d level:%d frame:%d\n", color_format[format],
			img->order, img->width, img->height, img->bpp, mask+1, img->level_count, img->frame_count);

	printf("----------------\n");
	img_pt = (int*)((u8*)img+img->offsets);
	i = 0;

	for(f=0; f<img->frame_count; f++){
		for(l=0; l<img->level_count; l++){
			if(img->level_type==GIM_TYPE_MIPMAP){
				width = img->width>>l;
				height = img->height>>l;
			}else{
				width = img->width;
				height = img->height;
			}
			pitch = (width*img->bpp/8+mask)&~mask;

			if(pal_pt){
				pal_level = (l>=pal->level_count)? 0: l;
				pal_frame = (f>=pal->frame_count)? 0: f;
				pal_id = pal_frame*pal->level_count+pal_level;
				pal_data = (u8*)pal+pal_pt[pal_id];
			}else{
				pal_data = NULL;
				pal_frame = 0;
				pal_level = 0;
			}

			if(img->frame_count>1){
				if(img->level_count>1){
					sprintf(tname, "%s.F%dL%d", name, f, l);
				}else{
					sprintf(tname, "%s.F%d", name, f);
				}
			}else{
				if(img->level_count>1){
					sprintf(tname, "%s.L%d", name, l);
				}else{
					sprintf(tname, "%s", name);
				}
			}

			printf("    image %2d: %dx%d \tpitch %d\t%s.png\n", i, width, height, pitch, tname);
			img_to_png[format](tname, (u8*)img+img_pt[i], width, height, pitch, pal_data, pal_type, pal_num, img->order);
			memset(texbuff, 0, 4*1024*1024);
			memset(pltbuff, 0, 1*1024*1024);

			i++;
		}
	}
	printf("\n\n");
}

////////////////////////////////////////////////////////////////////////////

int process_file(char *fname)
{
	FILE *fp;
	u8 *dat_buf;
	char tname[2560];
	int i, n, size, gim_size;

	fp = fopen(fname, "rb");
	if(fp==NULL){
		printf("Open file %s failed!\n", fname);
		return -1;
	}

	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	dat_buf =(u8*) malloc(size); //make sure you have enough memory to dump your gim files.
	fread(dat_buf, size, 1, fp);
	fclose(fp);

	i = 0;
	n = 0;
	while(i<size){
		if(check_gim(dat_buf+i)==0){
			
			i += 16;
			continue;
		}
#ifdef SAVE_GIM
		if(i==0)
			break;
#endif

		if(n==0)
			printf("Process %s ...\n", fname);

		gim_size = *(u32*)(dat_buf+i+0x14);
		gim_size += 0x10;

#ifdef SAVE_GIM
		sprintf(tname, "%s.%d.gim", fname, n);
		fp = fopen(tname, "wb");
		fwrite(dat_buf+i, gim_size, 1, fp);
		fclose(fp);
		printf("\tSave %s\n", tname);
#else
		sprintf(tname, "%s.%d", fname, n);
		dump_gim(dat_buf+i, tname);
#endif
		i += gim_size;
		if(i%0x10)
			i+=(0x10-i%0x10);
		n += 1;
	}

	free(dat_buf);
	return 0;
}

////////////////////////////////////////////////////////////////////////////

int process_dir(char *dname)
{
	DIR *pdir;
	struct dirent *d;
	struct stat statbuf;
	char fname[256];
	int i, ndir;

	/* process file */
	memset(&statbuf, 0, sizeof(statbuf));
	stat(dname, &statbuf);
	if((statbuf.st_mode&S_IFMT) != S_IFDIR){
		return process_file(dname);
	}

	/* open directory */
	pdir = opendir(dname);
	if(pdir==NULL){
		printf("Can't open directory <%s>\n", dname);
		return -1;
	}

	/* get number of files in dircetory */
	ndir = 0;
	while((d=readdir(pdir))){
		ndir++;
	}
	d = (dirent*)malloc(sizeof(struct dirent)*ndir);

	/* read dirent first */
	rewinddir(pdir);
	for(i=0; i<ndir; i++){
		memcpy(&d[i], readdir(pdir), sizeof(struct dirent));
	}

	/* process each files */
	printf("Enter directory <%s> ...\n", dname);
	for(i=0; i<ndir; i++){
		if( d[i].d_name[0]=='.' &&( d[i].d_name[1] =='\0' || (d[i].d_name[1] == '.' && d[i].d_name[2] == '\0') ))
			continue;

		if(dname[0]=='.'){
			sprintf(fname, "%s", d[i].d_name);
		}else{
			sprintf(fname, "%s/%s", dname, d[i].d_name);
		}
		process_dir(fname);
	}
	printf("Leave directory <%s> ...\n", dname);

	free(d);
	closedir(pdir);
	return 0;
}

int main(int argc, char *argv[])
{
	if(argc==1){
		return process_dir(".");
	}else{
		return process_dir(argv[1]);
	}
}

