/* image_pdt.h
 *	pdt ファイルのイメージを読み取り、
 *	それを DI_Image の形にして返す
 *	必要に応じてファイルのキャッシュも行う
**/
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




#ifndef __KANON_IMAGE_PDT_H__
#define __KANON_IMAGE_PDT_H__

#include "file.h"
#include "system.h"

#ifndef MaxPDTImage
#  define MaxPDTImage 32 /* PDT image のキャッシュの最大数 */
#endif
class PDT_Item;
class PDT_Reader {
	int is_used;
	int max_image;
	AyuSys& local_system;
	PDT_Item* head_cache;
	int DeleteCache(void);
	void AppendCache(PDT_Item* item);
	PDT_Item* SearchItem(char* fname);
	int Hash(char* fname);
public:
	PDT_Reader(int max,AyuSys& sys) : local_system(sys) {
		max_image = max;
		is_used = 0;
		head_cache = 0;
	}
	~PDT_Reader();
	DI_ImageMask* Search(char* fname);
	void Preread(char* fname);
	void ClearAllCache(void);
};
#endif /* !defined(__KANON_IMAGE_PDT_H__) */
