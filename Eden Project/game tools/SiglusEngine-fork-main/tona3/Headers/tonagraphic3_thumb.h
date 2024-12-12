#pragma		once

#include	"tonagraphic3_graphic.h"

namespace NT3
{

// ****************************************************************
// イメージリスト
// ================================================================
class Cimage_list
{
public:
	Cimage_list();
	~Cimage_list();

	bool		create(int w, int h);
	bool		add_image(HBITMAP h_bitmap);
	void		destroy();

	HIMAGELIST	get()	{	return h_il;	}

private:

	HIMAGELIST	h_il;
};

}
