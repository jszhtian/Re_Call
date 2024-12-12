#include	"tonagraphic3_pch.h"
#include	"tonagraphic3_g00.h"

#include	<g00.h>

namespace NT3
{

// ****************************************************************
// dib �`�b�v�� g00 �`�b�v��ǂݍ���
// ================================================================
bool load_g00_chip_to_dib(C_dib_chip* dib_chip, C_g00_chip* g00_chip)
{
	// g00 �`�b�v�̏����擾
	S_g00_chip_info chip_info;
	g00_chip->get_info(&chip_info);

	// dib �`�b�v�̍쐬
	if (!dib_chip->create(chip_info.width, chip_info.height, 32))
		return false;

	dib_chip->x = chip_info.x;
	dib_chip->y = chip_info.y;
	dib_chip->sprite = chip_info.sprite;

	// g00 �f�[�^�̓ǂݍ���
	g00_chip->get_data(dib_chip->get_ptr(), dib_chip->get_width() * 4);

	return true;
}

// ****************************************************************
// �s�N�`���� g00 �J�b�g��ǂݍ���
// ================================================================
bool load_g00_cut_to_pct(C_pct* pct, C_g00_cut* g00_cut)
{
	// �J�b�g�̏����擾
	S_g00_cut_info cut_info;
	g00_cut->get_info(&cut_info);

	// �`�b�v�̓ǂݍ���
	ARRAY< BSP<C_dib_chip> > chip_list;
	for (int chip_no = 0; chip_no < cut_info.chip_cnt; chip_no++)	{
		BSP<C_dib_chip> chip(new C_dib_chip);
		if (!load_g00_chip_to_dib(chip.get(), g00_cut->get_chip(chip_no)))
			return false;

		chip_list.push_back(chip);
	}

	// �s�N�`���̍쐬
	pct->create(chip_list, cut_info.width, cut_info.height, cut_info.center, cut_info.disp_rect);

	return true;
}

// ****************************************************************
// �A���o���� g00 ��ǂݍ���
// ================================================================
bool load_g00_to_album(C_album* album, CTSTR& file_path)
{
	// g00 �t�@�C����ǂݍ���
	BUFFER buffer;
	if (!C_file::read_full_data(file_path, buffer))
		return false;

	// g00 �̍쐬
	C_g00 g00;
	g00.set_data(buffer.get(), buffer.size());

	// g00 �̏���ǂݍ���
	S_g00_info g00_info;
	g00.get_info(&g00_info);

	// �J�b�g��ǂݍ���
	ARRAY< BSP<C_pct> > pct_list;
	for (int pct_no = 0; pct_no < g00_info.cut_cnt; pct_no++)	{
		BSP<C_pct> pct(new C_pct);
		if (!load_g00_cut_to_pct(pct.get(), g00.get_cut(pct_no)))
			return false;

		pct_list.push_back(pct);
	}

	// �A���o���̍쐬
	album->create(pct_list);

	return true;
}

// ****************************************************************
// �f�B�u�� g00 �`�b�v��ǂݍ���
// ================================================================
bool load_g00_chip_to_dib_expand(C_dib* dib, C_g00_chip* g00_chip)
{
	// g00 �`�b�v�̏����擾
	S_g00_chip_info chip_info;
	g00_chip->get_info(&chip_info);

	// g00 �f�[�^�̓ǂݍ���
	g00_chip->get_data(dib->get_ptr(chip_info.x, chip_info.y), dib->get_width() * 4);

	return true;
}

// ****************************************************************
// �f�B�u�� g00 �J�b�g��ǂݍ���
// ================================================================
bool load_g00_cut_to_dib_expand(C_dib* dib, C_g00_cut* g00_cut)
{
	// �J�b�g�̏����擾
	S_g00_cut_info cut_info;
	g00_cut->get_info(&cut_info);

	// �f�B�u���쐬
	if (!dib->create(cut_info.width, cut_info.height, 32))
		return false;

	dib->clear_color(C_argb(0, 0, 0, 0));

	// �`�b�v�̓ǂݍ���
	for (int chip_no = 0; chip_no < cut_info.chip_cnt; chip_no++)	{
		if (!load_g00_chip_to_dib_expand(dib, g00_cut->get_chip(chip_no)))
			return false;
	}

	return true;
}

// ****************************************************************
// �A���o���� g00 ��ǂݍ���
// ================================================================
bool load_g00_to_dib_expand(C_dib* dib, CTSTR& file_path, int cut_no)
{
	// g00 �t�@�C����ǂݍ���
	BUFFER buffer;
	if (!C_file::read_full_data(file_path, buffer))
		return false;

	// g00 �̍쐬
	C_g00 g00;
	g00.set_data(buffer.get(), buffer.size());

	// g00 �̏���ǂݍ���
	S_g00_info g00_info;
	g00.get_info(&g00_info);

	// �J�b�g��ǂݍ���
	if (!load_g00_cut_to_dib_expand(dib, g00.get_cut(cut_no)))
		return false;

	return true;
}


};
