#include	"pch.h"




// �ʏ�\��
void tcgd_normal(void);
// �X�v���C�g�\��
void tcgd_sprite(void);
// �����ʏ�\��
void tcgd_normal_clr(void);
// �����X�v���C�g�\��
void tcgd_sprite_clr(void);
// �ϐF�ʏ�\��
void tcgd_normal_change(void);
// �ϐF�X�v���C�g�\��
void tcgd_sprite_change(void);
// �ϐF�����ʏ�\��
void tcgd_normal_change_clr(void);
// �ϐF�����X�v���C�g�\��
void tcgd_sprite_change_clr(void);
// ���Òʏ�\��
void tcgd_normal_bright(void);
// ���ÃX�v���C�g�\��
void tcgd_sprite_bright(void);
// ���Ó����ʏ�\��
void tcgd_normal_bright_clr(void);
// ���Ó����X�v���C�g�\��
void tcgd_sprite_bright_clr(void);
// �ϐF���Òʏ�\��
void tcgd_normal_change_bright(void);
// �ϐF���ÃX�v���C�g�\��
void tcgd_sprite_change_bright(void);
// �ϐF���Ó����ʏ�\��
void tcgd_normal_change_bright_clr(void);
// �ϐF���Ó����X�v���C�g�\��
void tcgd_sprite_change_bright_clr(void);
// ���m�ʏ�\��
void tcgd_normal_mono(void);
// ���m�X�v���C�g�\��
void tcgd_sprite_mono(void);
// ���m�����ʏ�\��
void tcgd_normal_mono_clr(void);
// ���m�����X�v���C�g�\��
void tcgd_sprite_mono_clr(void);
// ���m�ϐF�ʏ�\��
void tcgd_normal_mono_change(void);
// ���m�ϐF�X�v���C�g�\��
void tcgd_sprite_mono_change(void);
// ���m�ϐF�����ʏ�\��
void tcgd_normal_mono_change_clr(void);
// ���m�ϐF�����X�v���C�g�\��
void tcgd_sprite_mono_change_clr(void);
// ���m���Òʏ�\��
void tcgd_normal_mono_bright(void);
// ���m���ÃX�v���C�g�\��
void tcgd_sprite_mono_bright(void);
// ���m���Ó����ʏ�\��
void tcgd_normal_mono_bright_clr(void);
// ���m���Ó����X�v���C�g�\��
void tcgd_sprite_mono_bright_clr(void);
// ���m�ϐF���Òʏ�\��
void tcgd_normal_mono_change_bright(void);
// ���m�ϐF���ÃX�v���C�g�\��
void tcgd_sprite_mono_change_bright(void);
// ���m�ϐF���Ó����ʏ�\��
void tcgd_normal_mono_change_bright_clr(void);
// ���m�ϐF���Ó����X�v���C�g�\��
void tcgd_sprite_mono_change_bright_clr(void);
// ���]�ʏ�\��
void tcgd_normal_reverse(void);
// ���]�X�v���C�g�\��
void tcgd_sprite_reverse(void);
// ���]�����ʏ�\��
void tcgd_normal_reverse_clr(void);
// ���]�����X�v���C�g�\��
void tcgd_sprite_reverse_clr(void);
// ���]�ϐF�ʏ�\��
void tcgd_normal_reverse_change(void);
// ���]�ϐF�X�v���C�g�\��
void tcgd_sprite_reverse_change(void);
// ���]�ϐF�����ʏ�\��
void tcgd_normal_reverse_change_clr(void);
// ���]�ϐF�����X�v���C�g�\��
void tcgd_sprite_reverse_change_clr(void);
// ���]���Òʏ�\��
void tcgd_normal_reverse_bright(void);
// ���]���ÃX�v���C�g�\��
void tcgd_sprite_reverse_bright(void);
// ���]���Ó����ʏ�\��
void tcgd_normal_reverse_bright_clr(void);
// ���]���Ó����X�v���C�g�\��
void tcgd_sprite_reverse_bright_clr(void);
// ���]�ϐF���Òʏ�\��
void tcgd_normal_reverse_change_bright(void);
// ���]�ϐF���ÃX�v���C�g�\��
void tcgd_sprite_reverse_change_bright(void);
// ���]�ϐF���Ó����ʏ�\��
void tcgd_normal_reverse_change_bright_clr(void);
// ���]�ϐF���Ó����X�v���C�g�\��
void tcgd_sprite_reverse_change_bright_clr(void);
// ���]���m�ʏ�\��
void tcgd_normal_reverse_mono(void);
// ���]���m�X�v���C�g�\��
void tcgd_sprite_reverse_mono(void);
// ���]���m�����ʏ�\��
void tcgd_normal_reverse_mono_clr(void);
// ���]���m�����X�v���C�g�\��
void tcgd_sprite_reverse_mono_clr(void);
// ���]���m�ϐF�ʏ�\��
void tcgd_normal_reverse_mono_change(void);
// ���]���m�ϐF�X�v���C�g�\��
void tcgd_sprite_reverse_mono_change(void);
// ���]���m�ϐF�����ʏ�\��
void tcgd_normal_reverse_mono_change_clr(void);
// ���]���m�ϐF�����X�v���C�g�\��
void tcgd_sprite_reverse_mono_change_clr(void);
// ���]���m���Òʏ�\��
void tcgd_normal_reverse_mono_bright(void);
// ���]���m���ÃX�v���C�g�\��
void tcgd_sprite_reverse_mono_bright(void);
// ���]���m���Ó����ʏ�\��
void tcgd_normal_reverse_mono_bright_clr(void);
// ���]���m���Ó����X�v���C�g�\��
void tcgd_sprite_reverse_mono_bright_clr(void);
// ���]���m�ϐF���Òʏ�\��
void tcgd_normal_reverse_mono_change_bright(void);
// ���]���m�ϐF���ÃX�v���C�g�\��
void tcgd_sprite_reverse_mono_change_bright(void);
// ���]���m�ϐF���Ó����ʏ�\��
void tcgd_normal_reverse_mono_change_bright_clr(void);
// ���]���m�ϐF���Ó����X�v���C�g�\��
void tcgd_sprite_reverse_mono_change_bright_clr(void);

// �^�C���ʏ�\��
void tcgd_normal_tile(void);
// �^�C���X�v���C�g�\��
void tcgd_sprite_tile(void);
// �^�C�������ʏ�\��
void tcgd_normal_clr_tile(void);
// �^�C�������X�v���C�g�\��
void tcgd_sprite_clr_tile(void);
// �^�C���ϐF�ʏ�\��
void tcgd_normal_change_tile(void);
// �^�C���ϐF�X�v���C�g�\��
void tcgd_sprite_change_tile(void);
// �^�C���ϐF�����ʏ�\��
void tcgd_normal_change_clr_tile(void);
// �^�C���ϐF�����X�v���C�g�\��
void tcgd_sprite_change_clr_tile(void);
// �^�C�����Òʏ�\��
void tcgd_normal_bright_tile(void);
// �^�C�����ÃX�v���C�g�\��
void tcgd_sprite_bright_tile(void);
// �^�C�����Ó����ʏ�\��
void tcgd_normal_bright_clr_tile(void);
// �^�C�����Ó����X�v���C�g�\��
void tcgd_sprite_bright_clr_tile(void);
// �^�C���ϐF���Òʏ�\��
void tcgd_normal_change_bright_tile(void);
// �^�C���ϐF���ÃX�v���C�g�\��
void tcgd_sprite_change_bright_tile(void);
// �^�C���ϐF���Ó����ʏ�\��
void tcgd_normal_change_bright_clr_tile(void);
// �^�C���ϐF���Ó����X�v���C�g�\��
void tcgd_sprite_change_bright_clr_tile(void);
// �^�C�����m�ʏ�\��
void tcgd_normal_mono_tile(void);
// �^�C�����m�X�v���C�g�\��
void tcgd_sprite_mono_tile(void);
// �^�C�����m�����ʏ�\��
void tcgd_normal_mono_clr_tile(void);
// �^�C�����m�����X�v���C�g�\��
void tcgd_sprite_mono_clr_tile(void);
// �^�C�����m�ϐF�ʏ�\��
void tcgd_normal_mono_change_tile(void);
// �^�C�����m�ϐF�X�v���C�g�\��
void tcgd_sprite_mono_change_tile(void);
// �^�C�����m�ϐF�����ʏ�\��
void tcgd_normal_mono_change_clr_tile(void);
// �^�C�����m�ϐF�����X�v���C�g�\��
void tcgd_sprite_mono_change_clr_tile(void);
// �^�C�����m���Òʏ�\��
void tcgd_normal_mono_bright_tile(void);
// �^�C�����m���ÃX�v���C�g�\��
void tcgd_sprite_mono_bright_tile(void);
// �^�C�����m���Ó����ʏ�\��
void tcgd_normal_mono_bright_clr_tile(void);
// �^�C�����m���Ó����X�v���C�g�\��
void tcgd_sprite_mono_bright_clr_tile(void);
// �^�C�����m�ϐF���Òʏ�\��
void tcgd_normal_mono_change_bright_tile(void);
// �^�C�����m�ϐF���ÃX�v���C�g�\��
void tcgd_sprite_mono_change_bright_tile(void);
// �^�C�����m�ϐF���Ó����ʏ�\��
void tcgd_normal_mono_change_bright_clr_tile(void);
// �^�C�����m�ϐF���Ó����X�v���C�g�\��
void tcgd_sprite_mono_change_bright_clr_tile(void);
// �^�C�����]�ʏ�\��
void tcgd_normal_reverse_tile(void);
// �^�C�����]�X�v���C�g�\��
void tcgd_sprite_reverse_tile(void);
// �^�C�����]�����ʏ�\��
void tcgd_normal_reverse_clr_tile(void);
// �^�C�����]�����X�v���C�g�\��
void tcgd_sprite_reverse_clr_tile(void);
// �^�C�����]�ϐF�ʏ�\��
void tcgd_normal_reverse_change_tile(void);
// �^�C�����]�ϐF�X�v���C�g�\��
void tcgd_sprite_reverse_change_tile(void);
// �^�C�����]�ϐF�����ʏ�\��
void tcgd_normal_reverse_change_clr_tile(void);
// �^�C�����]�ϐF�����X�v���C�g�\��
void tcgd_sprite_reverse_change_clr_tile(void);
// �^�C�����]���Òʏ�\��
void tcgd_normal_reverse_bright_tile(void);
// �^�C�����]���ÃX�v���C�g�\��
void tcgd_sprite_reverse_bright_tile(void);
// �^�C�����]���Ó����ʏ�\��
void tcgd_normal_reverse_bright_clr_tile(void);
// �^�C�����]���Ó����X�v���C�g�\��
void tcgd_sprite_reverse_bright_clr_tile(void);
// �^�C�����]�ϐF���Òʏ�\��
void tcgd_normal_reverse_change_bright_tile(void);
// �^�C�����]�ϐF���ÃX�v���C�g�\��
void tcgd_sprite_reverse_change_bright_tile(void);
// �^�C�����]�ϐF���Ó����ʏ�\��
void tcgd_normal_reverse_change_bright_clr_tile(void);
// �^�C�����]�ϐF���Ó����X�v���C�g�\��
void tcgd_sprite_reverse_change_bright_clr_tile(void);
// �^�C�����]���m�ʏ�\��
void tcgd_normal_reverse_mono_tile(void);
// �^�C�����]���m�X�v���C�g�\��
void tcgd_sprite_reverse_mono_tile(void);
// �^�C�����]���m�����ʏ�\��
void tcgd_normal_reverse_mono_clr_tile(void);
// �^�C�����]���m�����X�v���C�g�\��
void tcgd_sprite_reverse_mono_clr_tile(void);
// �^�C�����]���m�ϐF�ʏ�\��
void tcgd_normal_reverse_mono_change_tile(void);
// �^�C�����]���m�ϐF�X�v���C�g�\��
void tcgd_sprite_reverse_mono_change_tile(void);
// �^�C�����]���m�ϐF�����ʏ�\��
void tcgd_normal_reverse_mono_change_clr_tile(void);
// �^�C�����]���m�ϐF�����X�v���C�g�\��
void tcgd_sprite_reverse_mono_change_clr_tile(void);
// �^�C�����]���m���Òʏ�\��
void tcgd_normal_reverse_mono_bright_tile(void);
// �^�C�����]���m���ÃX�v���C�g�\��
void tcgd_sprite_reverse_mono_bright_tile(void);
// �^�C�����]���m���Ó����ʏ�\��
void tcgd_normal_reverse_mono_bright_clr_tile(void);
// �^�C�����]���m���Ó����X�v���C�g�\��
void tcgd_sprite_reverse_mono_bright_clr_tile(void);
// �^�C�����]���m�ϐF���Òʏ�\��
void tcgd_normal_reverse_mono_change_bright_tile(void);
// �^�C�����]���m�ϐF���ÃX�v���C�g�\��
void tcgd_sprite_reverse_mono_change_bright_tile(void);
// �^�C�����]���m�ϐF���Ó����ʏ�\��
void tcgd_normal_reverse_mono_change_bright_clr_tile(void);
// �^�C�����]���m�ϐF���Ó����X�v���C�g�\��
void tcgd_sprite_reverse_mono_change_bright_clr_tile(void);

// �ʏ�\���i���Z�j
void tcgd_normal_add(void);
// �X�v���C�g�\���i���Z�j
void tcgd_sprite_add(void);
// �����ʏ�\���i���Z�j
void tcgd_normal_clr_add(void);
// �����X�v���C�g�\���i���Z�j
void tcgd_sprite_clr_add(void);
// �ϐF�ʏ�\���i���Z�j
void tcgd_normal_change_add(void);
// �ϐF�X�v���C�g�\���i���Z�j
void tcgd_sprite_change_add(void);
// �ϐF�����ʏ�\���i���Z�j
void tcgd_normal_change_clr_add(void);
// �ϐF�����X�v���C�g�\���i���Z�j
void tcgd_sprite_change_clr_add(void);
// ���Òʏ�\���i���Z�j
void tcgd_normal_bright_add(void);
// ���ÃX�v���C�g�\���i���Z�j
void tcgd_sprite_bright_add(void);
// ���Ó����ʏ�\���i���Z�j
void tcgd_normal_bright_clr_add(void);
// ���Ó����X�v���C�g�\���i���Z�j
void tcgd_sprite_bright_clr_add(void);
// �ϐF���Òʏ�\���i���Z�j
void tcgd_normal_change_bright_add(void);
// �ϐF���ÃX�v���C�g�\���i���Z�j
void tcgd_sprite_change_bright_add(void);
// �ϐF���Ó����ʏ�\���i���Z�j
void tcgd_normal_change_bright_clr_add(void);
// �ϐF���Ó����X�v���C�g�\���i���Z�j
void tcgd_sprite_change_bright_clr_add(void);
// ���m�ʏ�\���i���Z�j
void tcgd_normal_mono_add(void);
// ���m�X�v���C�g�\���i���Z�j
void tcgd_sprite_mono_add(void);
// ���m�����ʏ�\���i���Z�j
void tcgd_normal_mono_clr_add(void);
// ���m�����X�v���C�g�\���i���Z�j
void tcgd_sprite_mono_clr_add(void);
// ���m�ϐF�ʏ�\���i���Z�j
void tcgd_normal_mono_change_add(void);
// ���m�ϐF�X�v���C�g�\���i���Z�j
void tcgd_sprite_mono_change_add(void);
// ���m�ϐF�����ʏ�\���i���Z�j
void tcgd_normal_mono_change_clr_add(void);
// ���m�ϐF�����X�v���C�g�\���i���Z�j
void tcgd_sprite_mono_change_clr_add(void);
// ���m���Òʏ�\���i���Z�j
void tcgd_normal_mono_bright_add(void);
// ���m���ÃX�v���C�g�\���i���Z�j
void tcgd_sprite_mono_bright_add(void);
// ���m���Ó����ʏ�\���i���Z�j
void tcgd_normal_mono_bright_clr_add(void);
// ���m���Ó����X�v���C�g�\���i���Z�j
void tcgd_sprite_mono_bright_clr_add(void);
// ���m�ϐF���Òʏ�\���i���Z�j
void tcgd_normal_mono_change_bright_add(void);
// ���m�ϐF���ÃX�v���C�g�\���i���Z�j
void tcgd_sprite_mono_change_bright_add(void);
// ���m�ϐF���Ó����ʏ�\���i���Z�j
void tcgd_normal_mono_change_bright_clr_add(void);
// ���m�ϐF���Ó����X�v���C�g�\���i���Z�j
void tcgd_sprite_mono_change_bright_clr_add(void);
// ���]�ʏ�\���i���Z�j
void tcgd_normal_reverse_add(void);
// ���]�X�v���C�g�\���i���Z�j
void tcgd_sprite_reverse_add(void);
// ���]�����ʏ�\���i���Z�j
void tcgd_normal_reverse_clr_add(void);
// ���]�����X�v���C�g�\���i���Z�j
void tcgd_sprite_reverse_clr_add(void);
// ���]�ϐF�ʏ�\���i���Z�j
void tcgd_normal_reverse_change_add(void);
// ���]�ϐF�X�v���C�g�\���i���Z�j
void tcgd_sprite_reverse_change_add(void);
// ���]�ϐF�����ʏ�\���i���Z�j
void tcgd_normal_reverse_change_clr_add(void);
// ���]�ϐF�����X�v���C�g�\���i���Z�j
void tcgd_sprite_reverse_change_clr_add(void);
// ���]���Òʏ�\���i���Z�j
void tcgd_normal_reverse_bright_add(void);
// ���]���ÃX�v���C�g�\���i���Z�j
void tcgd_sprite_reverse_bright_add(void);
// ���]���Ó����ʏ�\���i���Z�j
void tcgd_normal_reverse_bright_clr_add(void);
// ���]���Ó����X�v���C�g�\���i���Z�j
void tcgd_sprite_reverse_bright_clr_add(void);
// ���]�ϐF���Òʏ�\���i���Z�j
void tcgd_normal_reverse_change_bright_add(void);
// ���]�ϐF���ÃX�v���C�g�\���i���Z�j
void tcgd_sprite_reverse_change_bright_add(void);
// ���]�ϐF���Ó����ʏ�\���i���Z�j
void tcgd_normal_reverse_change_bright_clr_add(void);
// ���]�ϐF���Ó����X�v���C�g�\���i���Z�j
void tcgd_sprite_reverse_change_bright_clr_add(void);
// ���]���m�ʏ�\���i���Z�j
void tcgd_normal_reverse_mono_add(void);
// ���]���m�X�v���C�g�\���i���Z�j
void tcgd_sprite_reverse_mono_add(void);
// ���]���m�����ʏ�\���i���Z�j
void tcgd_normal_reverse_mono_clr_add(void);
// ���]���m�����X�v���C�g�\���i���Z�j
void tcgd_sprite_reverse_mono_clr_add(void);
// ���]���m�ϐF�ʏ�\���i���Z�j
void tcgd_normal_reverse_mono_change_add(void);
// ���]���m�ϐF�X�v���C�g�\���i���Z�j
void tcgd_sprite_reverse_mono_change_add(void);
// ���]���m�ϐF�����ʏ�\���i���Z�j
void tcgd_normal_reverse_mono_change_clr_add(void);
// ���]���m�ϐF�����X�v���C�g�\���i���Z�j
void tcgd_sprite_reverse_mono_change_clr_add(void);
// ���]���m���Òʏ�\���i���Z�j
void tcgd_normal_reverse_mono_bright_add(void);
// ���]���m���ÃX�v���C�g�\���i���Z�j
void tcgd_sprite_reverse_mono_bright_add(void);
// ���]���m���Ó����ʏ�\���i���Z�j
void tcgd_normal_reverse_mono_bright_clr_add(void);
// ���]���m���Ó����X�v���C�g�\���i���Z�j
void tcgd_sprite_reverse_mono_bright_clr_add(void);
// ���]���m�ϐF���Òʏ�\���i���Z�j
void tcgd_normal_reverse_mono_change_bright_add(void);
// ���]���m�ϐF���ÃX�v���C�g�\���i���Z�j
void tcgd_sprite_reverse_mono_change_bright_add(void);
// ���]���m�ϐF���Ó����ʏ�\���i���Z�j
void tcgd_normal_reverse_mono_change_bright_clr_add(void);
// ���]���m�ϐF���Ó����X�v���C�g�\���i���Z�j
void tcgd_sprite_reverse_mono_change_bright_clr_add(void);

// �^�C���ʏ�\���i���Z�j
void tcgd_normal_tile_add(void);
// �^�C���X�v���C�g�\���i���Z�j
void tcgd_sprite_tile_add(void);
// �^�C�������ʏ�\���i���Z�j
void tcgd_normal_clr_tile_add(void);
// �^�C�������X�v���C�g�\���i���Z�j
void tcgd_sprite_clr_tile_add(void);
// �^�C���ϐF�ʏ�\���i���Z�j
void tcgd_normal_change_tile_add(void);
// �^�C���ϐF�X�v���C�g�\���i���Z�j
void tcgd_sprite_change_tile_add(void);
// �^�C���ϐF�����ʏ�\���i���Z�j
void tcgd_normal_change_clr_tile_add(void);
// �^�C���ϐF�����X�v���C�g�\���i���Z�j
void tcgd_sprite_change_clr_tile_add(void);
// �^�C�����Òʏ�\���i���Z�j
void tcgd_normal_bright_tile_add(void);
// �^�C�����ÃX�v���C�g�\���i���Z�j
void tcgd_sprite_bright_tile_add(void);
// �^�C�����Ó����ʏ�\���i���Z�j
void tcgd_normal_bright_clr_tile_add(void);
// �^�C�����Ó����X�v���C�g�\���i���Z�j
void tcgd_sprite_bright_clr_tile_add(void);
// �^�C���ϐF���Òʏ�\���i���Z�j
void tcgd_normal_change_bright_tile_add(void);
// �^�C���ϐF���ÃX�v���C�g�\���i���Z�j
void tcgd_sprite_change_bright_tile_add(void);
// �^�C���ϐF���Ó����ʏ�\���i���Z�j
void tcgd_normal_change_bright_clr_tile_add(void);
// �^�C���ϐF���Ó����X�v���C�g�\���i���Z�j
void tcgd_sprite_change_bright_clr_tile_add(void);
// �^�C�����m�ʏ�\���i���Z�j
void tcgd_normal_mono_tile_add(void);
// �^�C�����m�X�v���C�g�\���i���Z�j
void tcgd_sprite_mono_tile_add(void);
// �^�C�����m�����ʏ�\���i���Z�j
void tcgd_normal_mono_clr_tile_add(void);
// �^�C�����m�����X�v���C�g�\���i���Z�j
void tcgd_sprite_mono_clr_tile_add(void);
// �^�C�����m�ϐF�ʏ�\���i���Z�j
void tcgd_normal_mono_change_tile_add(void);
// �^�C�����m�ϐF�X�v���C�g�\���i���Z�j
void tcgd_sprite_mono_change_tile_add(void);
// �^�C�����m�ϐF�����ʏ�\���i���Z�j
void tcgd_normal_mono_change_clr_tile_add(void);
// �^�C�����m�ϐF�����X�v���C�g�\���i���Z�j
void tcgd_sprite_mono_change_clr_tile_add(void);
// �^�C�����m���Òʏ�\���i���Z�j
void tcgd_normal_mono_bright_tile_add(void);
// �^�C�����m���ÃX�v���C�g�\���i���Z�j
void tcgd_sprite_mono_bright_tile_add(void);
// �^�C�����m���Ó����ʏ�\���i���Z�j
void tcgd_normal_mono_bright_clr_tile_add(void);
// �^�C�����m���Ó����X�v���C�g�\���i���Z�j
void tcgd_sprite_mono_bright_clr_tile_add(void);
// �^�C�����m�ϐF���Òʏ�\���i���Z�j
void tcgd_normal_mono_change_bright_tile_add(void);
// �^�C�����m�ϐF���ÃX�v���C�g�\���i���Z�j
void tcgd_sprite_mono_change_bright_tile_add(void);
// �^�C�����m�ϐF���Ó����ʏ�\���i���Z�j
void tcgd_normal_mono_change_bright_clr_tile_add(void);
// �^�C�����m�ϐF���Ó����X�v���C�g�\���i���Z�j
void tcgd_sprite_mono_change_bright_clr_tile_add(void);
// �^�C�����]�ʏ�\���i���Z�j
void tcgd_normal_reverse_tile_add(void);
// �^�C�����]�X�v���C�g�\���i���Z�j
void tcgd_sprite_reverse_tile_add(void);
// �^�C�����]�����ʏ�\���i���Z�j
void tcgd_normal_reverse_clr_tile_add(void);
// �^�C�����]�����X�v���C�g�\���i���Z�j
void tcgd_sprite_reverse_clr_tile_add(void);
// �^�C�����]�ϐF�ʏ�\���i���Z�j
void tcgd_normal_reverse_change_tile_add(void);
// �^�C�����]�ϐF�X�v���C�g�\���i���Z�j
void tcgd_sprite_reverse_change_tile_add(void);
// �^�C�����]�ϐF�����ʏ�\���i���Z�j
void tcgd_normal_reverse_change_clr_tile_add(void);
// �^�C�����]�ϐF�����X�v���C�g�\���i���Z�j
void tcgd_sprite_reverse_change_clr_tile_add(void);
// �^�C�����]���Òʏ�\���i���Z�j
void tcgd_normal_reverse_bright_tile_add(void);
// �^�C�����]���ÃX�v���C�g�\���i���Z�j
void tcgd_sprite_reverse_bright_tile_add(void);
// �^�C�����]���Ó����ʏ�\���i���Z�j
void tcgd_normal_reverse_bright_clr_tile_add(void);
// �^�C�����]���Ó����X�v���C�g�\���i���Z�j
void tcgd_sprite_reverse_bright_clr_tile_add(void);
// �^�C�����]�ϐF���Òʏ�\���i���Z�j
void tcgd_normal_reverse_change_bright_tile_add(void);
// �^�C�����]�ϐF���ÃX�v���C�g�\���i���Z�j
void tcgd_sprite_reverse_change_bright_tile_add(void);
// �^�C�����]�ϐF���Ó����ʏ�\���i���Z�j
void tcgd_normal_reverse_change_bright_clr_tile_add(void);
// �^�C�����]�ϐF���Ó����X�v���C�g�\���i���Z�j
void tcgd_sprite_reverse_change_bright_clr_tile_add(void);
// �^�C�����]���m�ʏ�\���i���Z�j
void tcgd_normal_reverse_mono_tile_add(void);
// �^�C�����]���m�X�v���C�g�\���i���Z�j
void tcgd_sprite_reverse_mono_tile_add(void);
// �^�C�����]���m�����ʏ�\���i���Z�j
void tcgd_normal_reverse_mono_clr_tile_add(void);
// �^�C�����]���m�����X�v���C�g�\���i���Z�j
void tcgd_sprite_reverse_mono_clr_tile_add(void);
// �^�C�����]���m�ϐF�ʏ�\���i���Z�j
void tcgd_normal_reverse_mono_change_tile_add(void);
// �^�C�����]���m�ϐF�X�v���C�g�\���i���Z�j
void tcgd_sprite_reverse_mono_change_tile_add(void);
// �^�C�����]���m�ϐF�����ʏ�\���i���Z�j
void tcgd_normal_reverse_mono_change_clr_tile_add(void);
// �^�C�����]���m�ϐF�����X�v���C�g�\���i���Z�j
void tcgd_sprite_reverse_mono_change_clr_tile_add(void);
// �^�C�����]���m���Òʏ�\���i���Z�j
void tcgd_normal_reverse_mono_bright_tile_add(void);
// �^�C�����]���m���ÃX�v���C�g�\���i���Z�j
void tcgd_sprite_reverse_mono_bright_tile_add(void);
// �^�C�����]���m���Ó����ʏ�\���i���Z�j
void tcgd_normal_reverse_mono_bright_clr_tile_add(void);
// �^�C�����]���m���Ó����X�v���C�g�\���i���Z�j
void tcgd_sprite_reverse_mono_bright_clr_tile_add(void);
// �^�C�����]���m�ϐF���Òʏ�\���i���Z�j
void tcgd_normal_reverse_mono_change_bright_tile_add(void);
// �^�C�����]���m�ϐF���ÃX�v���C�g�\���i���Z�j
void tcgd_sprite_reverse_mono_change_bright_tile_add(void);
// �^�C�����]���m�ϐF���Ó����ʏ�\���i���Z�j
void tcgd_normal_reverse_mono_change_bright_clr_tile_add(void);
// �^�C�����]���m�ϐF���Ó����X�v���C�g�\���i���Z�j
void tcgd_sprite_reverse_mono_change_bright_clr_tile_add(void);

// �ʏ�\���i���Z�j
void tcgd_normal_sub(void);
// �X�v���C�g�\���i���Z�j
void tcgd_sprite_sub(void);
// �����ʏ�\���i���Z�j
void tcgd_normal_clr_sub(void);
// �����X�v���C�g�\���i���Z�j
void tcgd_sprite_clr_sub(void);
// �ϐF�ʏ�\���i���Z�j
void tcgd_normal_change_sub(void);
// �ϐF�X�v���C�g�\���i���Z�j
void tcgd_sprite_change_sub(void);
// �ϐF�����ʏ�\���i���Z�j
void tcgd_normal_change_clr_sub(void);
// �ϐF�����X�v���C�g�\���i���Z�j
void tcgd_sprite_change_clr_sub(void);
// ���Òʏ�\���i���Z�j
void tcgd_normal_bright_sub(void);
// ���ÃX�v���C�g�\���i���Z�j
void tcgd_sprite_bright_sub(void);
// ���Ó����ʏ�\���i���Z�j
void tcgd_normal_bright_clr_sub(void);
// ���Ó����X�v���C�g�\���i���Z�j
void tcgd_sprite_bright_clr_sub(void);
// �ϐF���Òʏ�\���i���Z�j
void tcgd_normal_change_bright_sub(void);
// �ϐF���ÃX�v���C�g�\���i���Z�j
void tcgd_sprite_change_bright_sub(void);
// �ϐF���Ó����ʏ�\���i���Z�j
void tcgd_normal_change_bright_clr_sub(void);
// �ϐF���Ó����X�v���C�g�\���i���Z�j
void tcgd_sprite_change_bright_clr_sub(void);
// ���m�ʏ�\���i���Z�j
void tcgd_normal_mono_sub(void);
// ���m�X�v���C�g�\���i���Z�j
void tcgd_sprite_mono_sub(void);
// ���m�����ʏ�\���i���Z�j
void tcgd_normal_mono_clr_sub(void);
// ���m�����X�v���C�g�\���i���Z�j
void tcgd_sprite_mono_clr_sub(void);
// ���m�ϐF�ʏ�\���i���Z�j
void tcgd_normal_mono_change_sub(void);
// ���m�ϐF�X�v���C�g�\���i���Z�j
void tcgd_sprite_mono_change_sub(void);
// ���m�ϐF�����ʏ�\���i���Z�j
void tcgd_normal_mono_change_clr_sub(void);
// ���m�ϐF�����X�v���C�g�\���i���Z�j
void tcgd_sprite_mono_change_clr_sub(void);
// ���m���Òʏ�\���i���Z�j
void tcgd_normal_mono_bright_sub(void);
// ���m���ÃX�v���C�g�\���i���Z�j
void tcgd_sprite_mono_bright_sub(void);
// ���m���Ó����ʏ�\���i���Z�j
void tcgd_normal_mono_bright_clr_sub(void);
// ���m���Ó����X�v���C�g�\���i���Z�j
void tcgd_sprite_mono_bright_clr_sub(void);
// ���m�ϐF���Òʏ�\���i���Z�j
void tcgd_normal_mono_change_bright_sub(void);
// ���m�ϐF���ÃX�v���C�g�\���i���Z�j
void tcgd_sprite_mono_change_bright_sub(void);
// ���m�ϐF���Ó����ʏ�\���i���Z�j
void tcgd_normal_mono_change_bright_clr_sub(void);
// ���m�ϐF���Ó����X�v���C�g�\���i���Z�j
void tcgd_sprite_mono_change_bright_clr_sub(void);
// ���]�ʏ�\���i���Z�j
void tcgd_normal_reverse_sub(void);
// ���]�X�v���C�g�\���i���Z�j
void tcgd_sprite_reverse_sub(void);
// ���]�����ʏ�\���i���Z�j
void tcgd_normal_reverse_clr_sub(void);
// ���]�����X�v���C�g�\���i���Z�j
void tcgd_sprite_reverse_clr_sub(void);
// ���]�ϐF�ʏ�\���i���Z�j
void tcgd_normal_reverse_change_sub(void);
// ���]�ϐF�X�v���C�g�\���i���Z�j
void tcgd_sprite_reverse_change_sub(void);
// ���]�ϐF�����ʏ�\���i���Z�j
void tcgd_normal_reverse_change_clr_sub(void);
// ���]�ϐF�����X�v���C�g�\���i���Z�j
void tcgd_sprite_reverse_change_clr_sub(void);
// ���]���Òʏ�\���i���Z�j
void tcgd_normal_reverse_bright_sub(void);
// ���]���ÃX�v���C�g�\���i���Z�j
void tcgd_sprite_reverse_bright_sub(void);
// ���]���Ó����ʏ�\���i���Z�j
void tcgd_normal_reverse_bright_clr_sub(void);
// ���]���Ó����X�v���C�g�\���i���Z�j
void tcgd_sprite_reverse_bright_clr_sub(void);
// ���]�ϐF���Òʏ�\���i���Z�j
void tcgd_normal_reverse_change_bright_sub(void);
// ���]�ϐF���ÃX�v���C�g�\���i���Z�j
void tcgd_sprite_reverse_change_bright_sub(void);
// ���]�ϐF���Ó����ʏ�\���i���Z�j
void tcgd_normal_reverse_change_bright_clr_sub(void);
// ���]�ϐF���Ó����X�v���C�g�\���i���Z�j
void tcgd_sprite_reverse_change_bright_clr_sub(void);
// ���]���m�ʏ�\���i���Z�j
void tcgd_normal_reverse_mono_sub(void);
// ���]���m�X�v���C�g�\���i���Z�j
void tcgd_sprite_reverse_mono_sub(void);
// ���]���m�����ʏ�\���i���Z�j
void tcgd_normal_reverse_mono_clr_sub(void);
// ���]���m�����X�v���C�g�\���i���Z�j
void tcgd_sprite_reverse_mono_clr_sub(void);
// ���]���m�ϐF�ʏ�\���i���Z�j
void tcgd_normal_reverse_mono_change_sub(void);
// ���]���m�ϐF�X�v���C�g�\���i���Z�j
void tcgd_sprite_reverse_mono_change_sub(void);
// ���]���m�ϐF�����ʏ�\���i���Z�j
void tcgd_normal_reverse_mono_change_clr_sub(void);
// ���]���m�ϐF�����X�v���C�g�\���i���Z�j
void tcgd_sprite_reverse_mono_change_clr_sub(void);
// ���]���m���Òʏ�\���i���Z�j
void tcgd_normal_reverse_mono_bright_sub(void);
// ���]���m���ÃX�v���C�g�\���i���Z�j
void tcgd_sprite_reverse_mono_bright_sub(void);
// ���]���m���Ó����ʏ�\���i���Z�j
void tcgd_normal_reverse_mono_bright_clr_sub(void);
// ���]���m���Ó����X�v���C�g�\���i���Z�j
void tcgd_sprite_reverse_mono_bright_clr_sub(void);
// ���]���m�ϐF���Òʏ�\���i���Z�j
void tcgd_normal_reverse_mono_change_bright_sub(void);
// ���]���m�ϐF���ÃX�v���C�g�\���i���Z�j
void tcgd_sprite_reverse_mono_change_bright_sub(void);
// ���]���m�ϐF���Ó����ʏ�\���i���Z�j
void tcgd_normal_reverse_mono_change_bright_clr_sub(void);
// ���]���m�ϐF���Ó����X�v���C�g�\���i���Z�j
void tcgd_sprite_reverse_mono_change_bright_clr_sub(void);

// �^�C���ʏ�\���i���Z�j
void tcgd_normal_tile_sub(void);
// �^�C���X�v���C�g�\���i���Z�j
void tcgd_sprite_tile_sub(void);
// �^�C�������ʏ�\���i���Z�j
void tcgd_normal_clr_tile_sub(void);
// �^�C�������X�v���C�g�\���i���Z�j
void tcgd_sprite_clr_tile_sub(void);
// �^�C���ϐF�ʏ�\���i���Z�j
void tcgd_normal_change_tile_sub(void);
// �^�C���ϐF�X�v���C�g�\���i���Z�j
void tcgd_sprite_change_tile_sub(void);
// �^�C���ϐF�����ʏ�\���i���Z�j
void tcgd_normal_change_clr_tile_sub(void);
// �^�C���ϐF�����X�v���C�g�\���i���Z�j
void tcgd_sprite_change_clr_tile_sub(void);
// �^�C�����Òʏ�\���i���Z�j
void tcgd_normal_bright_tile_sub(void);
// �^�C�����ÃX�v���C�g�\���i���Z�j
void tcgd_sprite_bright_tile_sub(void);
// �^�C�����Ó����ʏ�\���i���Z�j
void tcgd_normal_bright_clr_tile_sub(void);
// �^�C�����Ó����X�v���C�g�\���i���Z�j
void tcgd_sprite_bright_clr_tile_sub(void);
// �^�C���ϐF���Òʏ�\���i���Z�j
void tcgd_normal_change_bright_tile_sub(void);
// �^�C���ϐF���ÃX�v���C�g�\���i���Z�j
void tcgd_sprite_change_bright_tile_sub(void);
// �^�C���ϐF���Ó����ʏ�\���i���Z�j
void tcgd_normal_change_bright_clr_tile_sub(void);
// �^�C���ϐF���Ó����X�v���C�g�\���i���Z�j
void tcgd_sprite_change_bright_clr_tile_sub(void);
// �^�C�����m�ʏ�\���i���Z�j
void tcgd_normal_mono_tile_sub(void);
// �^�C�����m�X�v���C�g�\���i���Z�j
void tcgd_sprite_mono_tile_sub(void);
// �^�C�����m�����ʏ�\���i���Z�j
void tcgd_normal_mono_clr_tile_sub(void);
// �^�C�����m�����X�v���C�g�\���i���Z�j
void tcgd_sprite_mono_clr_tile_sub(void);
// �^�C�����m�ϐF�ʏ�\���i���Z�j
void tcgd_normal_mono_change_tile_sub(void);
// �^�C�����m�ϐF�X�v���C�g�\���i���Z�j
void tcgd_sprite_mono_change_tile_sub(void);
// �^�C�����m�ϐF�����ʏ�\���i���Z�j
void tcgd_normal_mono_change_clr_tile_sub(void);
// �^�C�����m�ϐF�����X�v���C�g�\���i���Z�j
void tcgd_sprite_mono_change_clr_tile_sub(void);
// �^�C�����m���Òʏ�\���i���Z�j
void tcgd_normal_mono_bright_tile_sub(void);
// �^�C�����m���ÃX�v���C�g�\���i���Z�j
void tcgd_sprite_mono_bright_tile_sub(void);
// �^�C�����m���Ó����ʏ�\���i���Z�j
void tcgd_normal_mono_bright_clr_tile_sub(void);
// �^�C�����m���Ó����X�v���C�g�\���i���Z�j
void tcgd_sprite_mono_bright_clr_tile_sub(void);
// �^�C�����m�ϐF���Òʏ�\���i���Z�j
void tcgd_normal_mono_change_bright_tile_sub(void);
// �^�C�����m�ϐF���ÃX�v���C�g�\���i���Z�j
void tcgd_sprite_mono_change_bright_tile_sub(void);
// �^�C�����m�ϐF���Ó����ʏ�\���i���Z�j
void tcgd_normal_mono_change_bright_clr_tile_sub(void);
// �^�C�����m�ϐF���Ó����X�v���C�g�\���i���Z�j
void tcgd_sprite_mono_change_bright_clr_tile_sub(void);
// �^�C�����]�ʏ�\���i���Z�j
void tcgd_normal_reverse_tile_sub(void);
// �^�C�����]�X�v���C�g�\���i���Z�j
void tcgd_sprite_reverse_tile_sub(void);
// �^�C�����]�����ʏ�\���i���Z�j
void tcgd_normal_reverse_clr_tile_sub(void);
// �^�C�����]�����X�v���C�g�\���i���Z�j
void tcgd_sprite_reverse_clr_tile_sub(void);
// �^�C�����]�ϐF�ʏ�\���i���Z�j
void tcgd_normal_reverse_change_tile_sub(void);
// �^�C�����]�ϐF�X�v���C�g�\���i���Z�j
void tcgd_sprite_reverse_change_tile_sub(void);
// �^�C�����]�ϐF�����ʏ�\���i���Z�j
void tcgd_normal_reverse_change_clr_tile_sub(void);
// �^�C�����]�ϐF�����X�v���C�g�\���i���Z�j
void tcgd_sprite_reverse_change_clr_tile_sub(void);
// �^�C�����]���Òʏ�\���i���Z�j
void tcgd_normal_reverse_bright_tile_sub(void);
// �^�C�����]���ÃX�v���C�g�\���i���Z�j
void tcgd_sprite_reverse_bright_tile_sub(void);
// �^�C�����]���Ó����ʏ�\���i���Z�j
void tcgd_normal_reverse_bright_clr_tile_sub(void);
// �^�C�����]���Ó����X�v���C�g�\���i���Z�j
void tcgd_sprite_reverse_bright_clr_tile_sub(void);
// �^�C�����]�ϐF���Òʏ�\���i���Z�j
void tcgd_normal_reverse_change_bright_tile_sub(void);
// �^�C�����]�ϐF���ÃX�v���C�g�\���i���Z�j
void tcgd_sprite_reverse_change_bright_tile_sub(void);
// �^�C�����]�ϐF���Ó����ʏ�\���i���Z�j
void tcgd_normal_reverse_change_bright_clr_tile_sub(void);
// �^�C�����]�ϐF���Ó����X�v���C�g�\���i���Z�j
void tcgd_sprite_reverse_change_bright_clr_tile_sub(void);
// �^�C�����]���m�ʏ�\���i���Z�j
void tcgd_normal_reverse_mono_tile_sub(void);
// �^�C�����]���m�X�v���C�g�\���i���Z�j
void tcgd_sprite_reverse_mono_tile_sub(void);
// �^�C�����]���m�����ʏ�\���i���Z�j
void tcgd_normal_reverse_mono_clr_tile_sub(void);
// �^�C�����]���m�����X�v���C�g�\���i���Z�j
void tcgd_sprite_reverse_mono_clr_tile_sub(void);
// �^�C�����]���m�ϐF�ʏ�\���i���Z�j
void tcgd_normal_reverse_mono_change_tile_sub(void);
// �^�C�����]���m�ϐF�X�v���C�g�\���i���Z�j
void tcgd_sprite_reverse_mono_change_tile_sub(void);
// �^�C�����]���m�ϐF�����ʏ�\���i���Z�j
void tcgd_normal_reverse_mono_change_clr_tile_sub(void);
// �^�C�����]���m�ϐF�����X�v���C�g�\���i���Z�j
void tcgd_sprite_reverse_mono_change_clr_tile_sub(void);
// �^�C�����]���m���Òʏ�\���i���Z�j
void tcgd_normal_reverse_mono_bright_tile_sub(void);
// �^�C�����]���m���ÃX�v���C�g�\���i���Z�j
void tcgd_sprite_reverse_mono_bright_tile_sub(void);
// �^�C�����]���m���Ó����ʏ�\���i���Z�j
void tcgd_normal_reverse_mono_bright_clr_tile_sub(void);
// �^�C�����]���m���Ó����X�v���C�g�\���i���Z�j
void tcgd_sprite_reverse_mono_bright_clr_tile_sub(void);
// �^�C�����]���m�ϐF���Òʏ�\���i���Z�j
void tcgd_normal_reverse_mono_change_bright_tile_sub(void);
// �^�C�����]���m�ϐF���ÃX�v���C�g�\���i���Z�j
void tcgd_sprite_reverse_mono_change_bright_tile_sub(void);
// �^�C�����]���m�ϐF���Ó����ʏ�\���i���Z�j
void tcgd_normal_reverse_mono_change_bright_clr_tile_sub(void);
// �^�C�����]���m�ϐF���Ó����X�v���C�g�\���i���Z�j
void tcgd_sprite_reverse_mono_change_bright_clr_tile_sub(void);

// �ʏ�\���i�q�f�a�t�B���^�[�j
void tcgd_normal_rgbfilter(void);
// �X�v���C�g�\���i�q�f�a�t�B���^�[�j
void tcgd_sprite_rgbfilter(void);
// �����ʏ�\���i�q�f�a�t�B���^�[�j
void tcgd_normal_clr_rgbfilter(void);
// �����X�v���C�g�\���i�q�f�a�t�B���^�[�j
void tcgd_sprite_clr_rgbfilter(void);
// �ϐF�ʏ�\���i�q�f�a�t�B���^�[�j
void tcgd_normal_change_rgbfilter(void);
// �ϐF�X�v���C�g�\���i�q�f�a�t�B���^�[�j
void tcgd_sprite_change_rgbfilter(void);
// �ϐF�����ʏ�\���i�q�f�a�t�B���^�[�j
void tcgd_normal_change_clr_rgbfilter(void);
// �ϐF�����X�v���C�g�\���i�q�f�a�t�B���^�[�j
void tcgd_sprite_change_clr_rgbfilter(void);
// ���Òʏ�\���i�q�f�a�t�B���^�[�j
void tcgd_normal_bright_rgbfilter(void);
// ���ÃX�v���C�g�\���i�q�f�a�t�B���^�[�j
void tcgd_sprite_bright_rgbfilter(void);
// ���Ó����ʏ�\���i�q�f�a�t�B���^�[�j
void tcgd_normal_bright_clr_rgbfilter(void);
// ���Ó����X�v���C�g�\���i�q�f�a�t�B���^�[�j
void tcgd_sprite_bright_clr_rgbfilter(void);
// �ϐF���Òʏ�\���i�q�f�a�t�B���^�[�j
void tcgd_normal_change_bright_rgbfilter(void);
// �ϐF���ÃX�v���C�g�\���i�q�f�a�t�B���^�[�j
void tcgd_sprite_change_bright_rgbfilter(void);
// �ϐF���Ó����ʏ�\���i�q�f�a�t�B���^�[�j
void tcgd_normal_change_bright_clr_rgbfilter(void);
// �ϐF���Ó����X�v���C�g�\���i�q�f�a�t�B���^�[�j
void tcgd_sprite_change_bright_clr_rgbfilter(void);
// ���m�ʏ�\���i�q�f�a�t�B���^�[�j
void tcgd_normal_mono_rgbfilter(void);
// ���m�X�v���C�g�\���i�q�f�a�t�B���^�[�j
void tcgd_sprite_mono_rgbfilter(void);
// ���m�����ʏ�\���i�q�f�a�t�B���^�[�j
void tcgd_normal_mono_clr_rgbfilter(void);
// ���m�����X�v���C�g�\���i�q�f�a�t�B���^�[�j
void tcgd_sprite_mono_clr_rgbfilter(void);
// ���m�ϐF�ʏ�\���i�q�f�a�t�B���^�[�j
void tcgd_normal_mono_change_rgbfilter(void);
// ���m�ϐF�X�v���C�g�\���i�q�f�a�t�B���^�[�j
void tcgd_sprite_mono_change_rgbfilter(void);
// ���m�ϐF�����ʏ�\���i�q�f�a�t�B���^�[�j
void tcgd_normal_mono_change_clr_rgbfilter(void);
// ���m�ϐF�����X�v���C�g�\���i�q�f�a�t�B���^�[�j
void tcgd_sprite_mono_change_clr_rgbfilter(void);
// ���m���Òʏ�\���i�q�f�a�t�B���^�[�j
void tcgd_normal_mono_bright_rgbfilter(void);
// ���m���ÃX�v���C�g�\���i�q�f�a�t�B���^�[�j
void tcgd_sprite_mono_bright_rgbfilter(void);
// ���m���Ó����ʏ�\���i�q�f�a�t�B���^�[�j
void tcgd_normal_mono_bright_clr_rgbfilter(void);
// ���m���Ó����X�v���C�g�\���i�q�f�a�t�B���^�[�j
void tcgd_sprite_mono_bright_clr_rgbfilter(void);
// ���m�ϐF���Òʏ�\���i�q�f�a�t�B���^�[�j
void tcgd_normal_mono_change_bright_rgbfilter(void);
// ���m�ϐF���ÃX�v���C�g�\���i�q�f�a�t�B���^�[�j
void tcgd_sprite_mono_change_bright_rgbfilter(void);
// ���m�ϐF���Ó����ʏ�\���i�q�f�a�t�B���^�[�j
void tcgd_normal_mono_change_bright_clr_rgbfilter(void);
// ���m�ϐF���Ó����X�v���C�g�\���i�q�f�a�t�B���^�[�j
void tcgd_sprite_mono_change_bright_clr_rgbfilter(void);
// ���]�ʏ�\���i�q�f�a�t�B���^�[�j
void tcgd_normal_reverse_rgbfilter(void);
// ���]�X�v���C�g�\���i�q�f�a�t�B���^�[�j
void tcgd_sprite_reverse_rgbfilter(void);
// ���]�����ʏ�\���i�q�f�a�t�B���^�[�j
void tcgd_normal_reverse_clr_rgbfilter(void);
// ���]�����X�v���C�g�\���i�q�f�a�t�B���^�[�j
void tcgd_sprite_reverse_clr_rgbfilter(void);
// ���]�ϐF�ʏ�\���i�q�f�a�t�B���^�[�j
void tcgd_normal_reverse_change_rgbfilter(void);
// ���]�ϐF�X�v���C�g�\���i�q�f�a�t�B���^�[�j
void tcgd_sprite_reverse_change_rgbfilter(void);
// ���]�ϐF�����ʏ�\���i�q�f�a�t�B���^�[�j
void tcgd_normal_reverse_change_clr_rgbfilter(void);
// ���]�ϐF�����X�v���C�g�\���i�q�f�a�t�B���^�[�j
void tcgd_sprite_reverse_change_clr_rgbfilter(void);
// ���]���Òʏ�\���i�q�f�a�t�B���^�[�j
void tcgd_normal_reverse_bright_rgbfilter(void);
// ���]���ÃX�v���C�g�\���i�q�f�a�t�B���^�[�j
void tcgd_sprite_reverse_bright_rgbfilter(void);
// ���]���Ó����ʏ�\���i�q�f�a�t�B���^�[�j
void tcgd_normal_reverse_bright_clr_rgbfilter(void);
// ���]���Ó����X�v���C�g�\���i�q�f�a�t�B���^�[�j
void tcgd_sprite_reverse_bright_clr_rgbfilter(void);
// ���]�ϐF���Òʏ�\���i�q�f�a�t�B���^�[�j
void tcgd_normal_reverse_change_bright_rgbfilter(void);
// ���]�ϐF���ÃX�v���C�g�\���i�q�f�a�t�B���^�[�j
void tcgd_sprite_reverse_change_bright_rgbfilter(void);
// ���]�ϐF���Ó����ʏ�\���i�q�f�a�t�B���^�[�j
void tcgd_normal_reverse_change_bright_clr_rgbfilter(void);
// ���]�ϐF���Ó����X�v���C�g�\���i�q�f�a�t�B���^�[�j
void tcgd_sprite_reverse_change_bright_clr_rgbfilter(void);
// ���]���m�ʏ�\���i�q�f�a�t�B���^�[�j
void tcgd_normal_reverse_mono_rgbfilter(void);
// ���]���m�X�v���C�g�\���i�q�f�a�t�B���^�[�j
void tcgd_sprite_reverse_mono_rgbfilter(void);
// ���]���m�����ʏ�\���i�q�f�a�t�B���^�[�j
void tcgd_normal_reverse_mono_clr_rgbfilter(void);
// ���]���m�����X�v���C�g�\���i�q�f�a�t�B���^�[�j
void tcgd_sprite_reverse_mono_clr_rgbfilter(void);
// ���]���m�ϐF�ʏ�\���i�q�f�a�t�B���^�[�j
void tcgd_normal_reverse_mono_change_rgbfilter(void);
// ���]���m�ϐF�X�v���C�g�\���i�q�f�a�t�B���^�[�j
void tcgd_sprite_reverse_mono_change_rgbfilter(void);
// ���]���m�ϐF�����ʏ�\���i�q�f�a�t�B���^�[�j
void tcgd_normal_reverse_mono_change_clr_rgbfilter(void);
// ���]���m�ϐF�����X�v���C�g�\���i�q�f�a�t�B���^�[�j
void tcgd_sprite_reverse_mono_change_clr_rgbfilter(void);
// ���]���m���Òʏ�\���i�q�f�a�t�B���^�[�j
void tcgd_normal_reverse_mono_bright_rgbfilter(void);
// ���]���m���ÃX�v���C�g�\���i�q�f�a�t�B���^�[�j
void tcgd_sprite_reverse_mono_bright_rgbfilter(void);
// ���]���m���Ó����ʏ�\���i�q�f�a�t�B���^�[�j
void tcgd_normal_reverse_mono_bright_clr_rgbfilter(void);
// ���]���m���Ó����X�v���C�g�\���i�q�f�a�t�B���^�[�j
void tcgd_sprite_reverse_mono_bright_clr_rgbfilter(void);
// ���]���m�ϐF���Òʏ�\���i�q�f�a�t�B���^�[�j
void tcgd_normal_reverse_mono_change_bright_rgbfilter(void);
// ���]���m�ϐF���ÃX�v���C�g�\���i�q�f�a�t�B���^�[�j
void tcgd_sprite_reverse_mono_change_bright_rgbfilter(void);
// ���]���m�ϐF���Ó����ʏ�\���i�q�f�a�t�B���^�[�j
void tcgd_normal_reverse_mono_change_bright_clr_rgbfilter(void);
// ���]���m�ϐF���Ó����X�v���C�g�\���i�q�f�a�t�B���^�[�j
void tcgd_sprite_reverse_mono_change_bright_clr_rgbfilter(void);

// �^�C���ʏ�\���i�q�f�a�t�B���^�[�j
void tcgd_normal_tile_rgbfilter(void);
// �^�C���X�v���C�g�\���i�q�f�a�t�B���^�[�j
void tcgd_sprite_tile_rgbfilter(void);
// �^�C�������ʏ�\���i�q�f�a�t�B���^�[�j
void tcgd_normal_clr_tile_rgbfilter(void);
// �^�C�������X�v���C�g�\���i�q�f�a�t�B���^�[�j
void tcgd_sprite_clr_tile_rgbfilter(void);
// �^�C���ϐF�ʏ�\���i�q�f�a�t�B���^�[�j
void tcgd_normal_change_tile_rgbfilter(void);
// �^�C���ϐF�X�v���C�g�\���i�q�f�a�t�B���^�[�j
void tcgd_sprite_change_tile_rgbfilter(void);
// �^�C���ϐF�����ʏ�\���i�q�f�a�t�B���^�[�j
void tcgd_normal_change_clr_tile_rgbfilter(void);
// �^�C���ϐF�����X�v���C�g�\���i�q�f�a�t�B���^�[�j
void tcgd_sprite_change_clr_tile_rgbfilter(void);
// �^�C�����Òʏ�\���i�q�f�a�t�B���^�[�j
void tcgd_normal_bright_tile_rgbfilter(void);
// �^�C�����ÃX�v���C�g�\���i�q�f�a�t�B���^�[�j
void tcgd_sprite_bright_tile_rgbfilter(void);
// �^�C�����Ó����ʏ�\���i�q�f�a�t�B���^�[�j
void tcgd_normal_bright_clr_tile_rgbfilter(void);
// �^�C�����Ó����X�v���C�g�\���i�q�f�a�t�B���^�[�j
void tcgd_sprite_bright_clr_tile_rgbfilter(void);
// �^�C���ϐF���Òʏ�\���i�q�f�a�t�B���^�[�j
void tcgd_normal_change_bright_tile_rgbfilter(void);
// �^�C���ϐF���ÃX�v���C�g�\���i�q�f�a�t�B���^�[�j
void tcgd_sprite_change_bright_tile_rgbfilter(void);
// �^�C���ϐF���Ó����ʏ�\���i�q�f�a�t�B���^�[�j
void tcgd_normal_change_bright_clr_tile_rgbfilter(void);
// �^�C���ϐF���Ó����X�v���C�g�\���i�q�f�a�t�B���^�[�j
void tcgd_sprite_change_bright_clr_tile_rgbfilter(void);
// �^�C�����m�ʏ�\���i�q�f�a�t�B���^�[�j
void tcgd_normal_mono_tile_rgbfilter(void);
// �^�C�����m�X�v���C�g�\���i�q�f�a�t�B���^�[�j
void tcgd_sprite_mono_tile_rgbfilter(void);
// �^�C�����m�����ʏ�\���i�q�f�a�t�B���^�[�j
void tcgd_normal_mono_clr_tile_rgbfilter(void);
// �^�C�����m�����X�v���C�g�\���i�q�f�a�t�B���^�[�j
void tcgd_sprite_mono_clr_tile_rgbfilter(void);
// �^�C�����m�ϐF�ʏ�\���i�q�f�a�t�B���^�[�j
void tcgd_normal_mono_change_tile_rgbfilter(void);
// �^�C�����m�ϐF�X�v���C�g�\���i�q�f�a�t�B���^�[�j
void tcgd_sprite_mono_change_tile_rgbfilter(void);
// �^�C�����m�ϐF�����ʏ�\���i�q�f�a�t�B���^�[�j
void tcgd_normal_mono_change_clr_tile_rgbfilter(void);
// �^�C�����m�ϐF�����X�v���C�g�\���i�q�f�a�t�B���^�[�j
void tcgd_sprite_mono_change_clr_tile_rgbfilter(void);
// �^�C�����m���Òʏ�\���i�q�f�a�t�B���^�[�j
void tcgd_normal_mono_bright_tile_rgbfilter(void);
// �^�C�����m���ÃX�v���C�g�\���i�q�f�a�t�B���^�[�j
void tcgd_sprite_mono_bright_tile_rgbfilter(void);
// �^�C�����m���Ó����ʏ�\���i�q�f�a�t�B���^�[�j
void tcgd_normal_mono_bright_clr_tile_rgbfilter(void);
// �^�C�����m���Ó����X�v���C�g�\���i�q�f�a�t�B���^�[�j
void tcgd_sprite_mono_bright_clr_tile_rgbfilter(void);
// �^�C�����m�ϐF���Òʏ�\���i�q�f�a�t�B���^�[�j
void tcgd_normal_mono_change_bright_tile_rgbfilter(void);
// �^�C�����m�ϐF���ÃX�v���C�g�\���i�q�f�a�t�B���^�[�j
void tcgd_sprite_mono_change_bright_tile_rgbfilter(void);
// �^�C�����m�ϐF���Ó����ʏ�\���i�q�f�a�t�B���^�[�j
void tcgd_normal_mono_change_bright_clr_tile_rgbfilter(void);
// �^�C�����m�ϐF���Ó����X�v���C�g�\���i�q�f�a�t�B���^�[�j
void tcgd_sprite_mono_change_bright_clr_tile_rgbfilter(void);
// �^�C�����]�ʏ�\���i�q�f�a�t�B���^�[�j
void tcgd_normal_reverse_tile_rgbfilter(void);
// �^�C�����]�X�v���C�g�\���i�q�f�a�t�B���^�[�j
void tcgd_sprite_reverse_tile_rgbfilter(void);
// �^�C�����]�����ʏ�\���i�q�f�a�t�B���^�[�j
void tcgd_normal_reverse_clr_tile_rgbfilter(void);
// �^�C�����]�����X�v���C�g�\���i�q�f�a�t�B���^�[�j
void tcgd_sprite_reverse_clr_tile_rgbfilter(void);
// �^�C�����]�ϐF�ʏ�\���i�q�f�a�t�B���^�[�j
void tcgd_normal_reverse_change_tile_rgbfilter(void);
// �^�C�����]�ϐF�X�v���C�g�\���i�q�f�a�t�B���^�[�j
void tcgd_sprite_reverse_change_tile_rgbfilter(void);
// �^�C�����]�ϐF�����ʏ�\���i�q�f�a�t�B���^�[�j
void tcgd_normal_reverse_change_clr_tile_rgbfilter(void);
// �^�C�����]�ϐF�����X�v���C�g�\���i�q�f�a�t�B���^�[�j
void tcgd_sprite_reverse_change_clr_tile_rgbfilter(void);
// �^�C�����]���Òʏ�\���i�q�f�a�t�B���^�[�j
void tcgd_normal_reverse_bright_tile_rgbfilter(void);
// �^�C�����]���ÃX�v���C�g�\���i�q�f�a�t�B���^�[�j
void tcgd_sprite_reverse_bright_tile_rgbfilter(void);
// �^�C�����]���Ó����ʏ�\���i�q�f�a�t�B���^�[�j
void tcgd_normal_reverse_bright_clr_tile_rgbfilter(void);
// �^�C�����]���Ó����X�v���C�g�\���i�q�f�a�t�B���^�[�j
void tcgd_sprite_reverse_bright_clr_tile_rgbfilter(void);
// �^�C�����]�ϐF���Òʏ�\���i�q�f�a�t�B���^�[�j
void tcgd_normal_reverse_change_bright_tile_rgbfilter(void);
// �^�C�����]�ϐF���ÃX�v���C�g�\���i�q�f�a�t�B���^�[�j
void tcgd_sprite_reverse_change_bright_tile_rgbfilter(void);
// �^�C�����]�ϐF���Ó����ʏ�\���i�q�f�a�t�B���^�[�j
void tcgd_normal_reverse_change_bright_clr_tile_rgbfilter(void);
// �^�C�����]�ϐF���Ó����X�v���C�g�\���i�q�f�a�t�B���^�[�j
void tcgd_sprite_reverse_change_bright_clr_tile_rgbfilter(void);
// �^�C�����]���m�ʏ�\���i�q�f�a�t�B���^�[�j
void tcgd_normal_reverse_mono_tile_rgbfilter(void);
// �^�C�����]���m�X�v���C�g�\���i�q�f�a�t�B���^�[�j
void tcgd_sprite_reverse_mono_tile_rgbfilter(void);
// �^�C�����]���m�����ʏ�\���i�q�f�a�t�B���^�[�j
void tcgd_normal_reverse_mono_clr_tile_rgbfilter(void);
// �^�C�����]���m�����X�v���C�g�\���i�q�f�a�t�B���^�[�j
void tcgd_sprite_reverse_mono_clr_tile_rgbfilter(void);
// �^�C�����]���m�ϐF�ʏ�\���i�q�f�a�t�B���^�[�j
void tcgd_normal_reverse_mono_change_tile_rgbfilter(void);
// �^�C�����]���m�ϐF�X�v���C�g�\���i�q�f�a�t�B���^�[�j
void tcgd_sprite_reverse_mono_change_tile_rgbfilter(void);
// �^�C�����]���m�ϐF�����ʏ�\���i�q�f�a�t�B���^�[�j
void tcgd_normal_reverse_mono_change_clr_tile_rgbfilter(void);
// �^�C�����]���m�ϐF�����X�v���C�g�\���i�q�f�a�t�B���^�[�j
void tcgd_sprite_reverse_mono_change_clr_tile_rgbfilter(void);
// �^�C�����]���m���Òʏ�\���i�q�f�a�t�B���^�[�j
void tcgd_normal_reverse_mono_bright_tile_rgbfilter(void);
// �^�C�����]���m���ÃX�v���C�g�\���i�q�f�a�t�B���^�[�j
void tcgd_sprite_reverse_mono_bright_tile_rgbfilter(void);
// �^�C�����]���m���Ó����ʏ�\���i�q�f�a�t�B���^�[�j
void tcgd_normal_reverse_mono_bright_clr_tile_rgbfilter(void);
// �^�C�����]���m���Ó����X�v���C�g�\���i�q�f�a�t�B���^�[�j
void tcgd_sprite_reverse_mono_bright_clr_tile_rgbfilter(void);
// �^�C�����]���m�ϐF���Òʏ�\���i�q�f�a�t�B���^�[�j
void tcgd_normal_reverse_mono_change_bright_tile_rgbfilter(void);
// �^�C�����]���m�ϐF���ÃX�v���C�g�\���i�q�f�a�t�B���^�[�j
void tcgd_sprite_reverse_mono_change_bright_tile_rgbfilter(void);
// �^�C�����]���m�ϐF���Ó����ʏ�\���i�q�f�a�t�B���^�[�j
void tcgd_normal_reverse_mono_change_bright_clr_tile_rgbfilter(void);
// �^�C�����]���m�ϐF���Ó����X�v���C�g�\���i�q�f�a�t�B���^�[�j
void tcgd_sprite_reverse_mono_change_bright_clr_tile_rgbfilter(void);


// �ʏ�\���i�����x�����F���[�h�R�j
void tcgd_normal_mod03_alphablend(void);
// �ϐF�ʏ�\���i�����x�����F���[�h�R�j
void tcgd_normal_change_mod03_alphablend(void);
// �X�v���C�g�\���i�����x�����F���[�h�R�j
void tcgd_sprite_mod03_alphablend(void);
// �ϐF�X�v���C�g�\���i�����x�����F���[�h�R�j
void tcgd_sprite_change_mod03_alphablend(void);


// �g��^�C���\���F���[�h�X�X
void tcgd_normal_clr_tile_wave(void);




/*========================================================================================
	�f�q�o�]��
void true_color_grp_disp(
	UC *d_buf, int d_xl, int d_yl,
	UC *s_buf, int s_xl, int s_yl, int s_xc, int s_yc, int type,
	int x, int y,
	int ex1, int ey1, int ex2, int ey2,
	int screen_mod,
	UC *tile, UC t_shift, int t_add, int t_xl, int t_yl, int t_repx, int t_repy, int t_reverse,
	UC mono,
	UC reverse,
	UC c_r, UC c_g, UC c_b, UC c_rate,
	int p_r, int p_g, int p_b,
	int bright,
	UC clr,
	int disp_mod
);
		IN		UC *d_buf      : �]����|�C���^
				int d_xl       : �]����w�k
				int d_yl       : �]����x�k
				UC *s_buf      : �]�����|�C���^
				int s_xl       : �]�����w�k
				int s_yl       : �]�����x�k
				int s_xc       : �]�����w�b
				int s_yc       : �]�����x�b
				int type       : 0=�ʏ� 1=�X�v���C�g
				int x          : �]�����W
				int y          : �]�����W
				int ex1        : �]����G���A�w�P�i(ex1=ey1=ex2=ey2=-1)=�]����ő�̃G���A�j
				int ey1        : �]����G���A�x�P
				int ex2        : �]����G���A�w�Q
				int ey2        : �]����G���A�x�Q
				int screen_mod : 0=���̂܂� 1=�t�]
				UC *tile       : �^�C���`�b�v�f�[�^�ւ̃|�C���^�i0=�Ȃ��j
				UC t_shift     : �^�C���V�t�g�l  �O�`�W
				int t_add      : �^�C�����Z�l  �|�Q�T�T�`�Q�T�T�i0=�Ȃ�  ����=������  ����=�グ��j
				int t_xl       : �^�C���w�k
				int t_yl       : �^�C���x�k
				int t_repx     : �^�C�����W�C���w
				int t_repy     : �^�C�����W�C���x
				int t_reverse  : �^�C���p�^�[���̃��o�[�X
				UC mono        : ���m�N��  �O�|�Q�T�T�i0=�Ȃ�  255=���m�N���j
				UC reverse     : ���o�[�X  �O�|�Q�T�T�i0=�Ȃ�  255=���o�[�X�j
				UC c_r         : �ԕϐF�l�@�O�|�Q�T�T
				UC c_g         : �ΕϐF�l�@�O�|�Q�T�T
				UC c_b         : �ϐF�l�@�O�|�Q�T�T
				UC c_rate      : �ϐF���[�g�@�O�|�Q�T�T�i0=�Ȃ�  255=�x�^�j
				int p_r        : �Ԗ����[�g�@�|�Q�T�T�`�Q�T�T�i0=�Ȃ�  ����=�Ԗ�������  ����=�Ԗ��グ��j
				int p_g        : �Ζ����[�g�@�|�Q�T�T�`�Q�T�T�i0=�Ȃ�  ����=�Ζ�������  ����=�Ζ��グ��j
				int p_b        : �����[�g�@�|�Q�T�T�`�Q�T�T�i0=�Ȃ�  ����=��������  ����=���グ��j
				int bright     : ���邳  �|�Q�T�T�`�Q�T�T�i0=�Ȃ�  ����=�Â�  ����=���邭�j
				UC clr         : �����x�@�O�|�Q�T�T�i0=�\�����Ȃ�  255=�s�����j
				int disp_mod   : �`�惂�[�h(0=�ʏ�  1=���Z  2=���Z)
		OUT		�Ȃ�
----------------------------------------------------------------------------------------*/
void true_color_grp_disp(
	UC *d_buf, int d_xl, int d_yl,
	UC *s_buf, int s_xl, int s_yl, int s_xc, int s_yc, int type,
	int x, int y,
	int ex1, int ey1, int ex2, int ey2,
	int screen_mod,
	UC *tile, UC t_shift, int t_add, int t_xl, int t_yl, int t_repx, int t_repy, int t_reverse,
	UC mono,
	UC reverse,
	UC c_r, UC c_g, UC c_b, UC c_rate,
	int p_r, int p_g, int p_b,
	int bright,
	UC clr,
	int disp_mod
){
	int tmp;
	int s_free_x, d_free_x, cnt_x;
	int s_free_y, d_free_y, cnt_y;

	if(t_shift < 0){
		t_shift = 0;
	}
	else if(t_shift > 8){
		t_shift = 8;
	}
	if(t_add < -(255<<t_shift)){
		t_add = -(255<<t_shift);
	}
	else if(t_add > 255){
		t_add = 255;
	}

	if(s_buf==0 || d_buf==0 || clr==0 || (tile && t_add==-(255<<t_shift))){
		return;
	}

// �G���A�ݒ�
	ng_set_eria(d_xl, d_yl, &ex1, &ey1, &ex2, &ey2);
// �p�����[�^�w�b�A�x�b���C��
	x -= s_xc;
	y -= s_yc;
// �\���͈͊O�`�F�b�N�P
	if(x > ex2){return;}
	if(y > ey2){return;}
// ���W����
	if(x < 0){
		s_free_x = (-x);
		cnt_x    = s_xl - s_free_x;
		d_free_x = 0;
	}
	else{
		s_free_x = 0;
		cnt_x    = s_xl;
		d_free_x = x;
	}
	if(y < 0){
		s_free_y = (-y);
		cnt_y    = s_yl - s_free_y;
		d_free_y = 0;
	}
	else{
		s_free_y = 0;
		cnt_y    = s_yl;
		d_free_y = y;
	}
// �\���͈͊O�`�F�b�N�Q
	if(cnt_x <= 0){return;}
	if(cnt_y <= 0){return;}
// �n�_���~�b�g�C��
	if(ex1 > d_free_x){
		tmp = ex1 - d_free_x;
		s_free_x += tmp;
		cnt_x    -= tmp;
		d_free_x  = ex1;
	}
	if(ey1 > d_free_y){
		tmp = ey1 - d_free_y;
		s_free_y += tmp;
		cnt_y    -= tmp;
		d_free_y  = ey1;
	}
// �\���͈͊O�`�F�b�N�R
	if(ex2 < d_free_x){return;}
	if(ey2 < d_free_y){return;}
// �I�_���~�b�g�C��
	tmp = ex2 - d_free_x + 1;
	if(tmp < cnt_x){cnt_x = tmp;}
	tmp = ey2 - d_free_y + 1;
	if(tmp < cnt_y){cnt_y = tmp;}
// �\���͈͊O�`�F�b�N�S
	if(cnt_x <= 0){return;}
	if(cnt_y <= 0){return;}

// �A�h���X�Z�o
	s_buf += (s_free_x + (s_free_y * s_xl)) << 2;
	if(screen_mod == 0){
		d_buf += (d_free_x + (d_free_y * d_xl)) << 2;
	}
	else{
		d_buf += (d_free_x + ((d_yl-d_free_y-1) * d_xl)) << 2;
	}

// �`��p�����[�^
	GRPF_d_buf = d_buf;
	GRPF_d_xl  = d_xl << 2;
	GRPF_d_yl  = d_yl;

	GRPF_s_buf = s_buf;
	GRPF_s_xl  = s_xl << 2;
	GRPF_s_yl  = s_yl;

	GRPF_cnt_x_org = cnt_x;
	GRPF_cnt_y     = cnt_y;

	if(screen_mod == 0){
		GRPF_d_xl -= (cnt_x << 2);
		GRPF_s_xl -= (cnt_x << 2);
	}
	else{
		GRPF_d_xl  = -(GRPF_d_xl + (cnt_x << 2));
		GRPF_s_xl -= (cnt_x << 2);
	}

// ���]�p�����[�^
	CTPREV = *(Gclr_table_adr_p + (255 - reverse));
	GRPF_reverse = reverse;

// ���m�p�����[�^
	GRPF_mono    = (255 - mono) << 2;

// �ϐF�p�����[�^
	if(c_rate){
		GRPF_c_r = c_r;
		GRPF_c_g = c_g;
		GRPF_c_b = c_b;
		CTPBETA = *(Gclr_table_adr_p + (255 - c_rate));
	}

// �����x�p�����[�^
	GRPF_clr4 = (clr << 2);
	GRPF_clrR = 255 - clr;
	CTPCLR = *(Gclr_table_adr_p + clr);

// �F���p�����[�^
	if(bright < -255){
		bright = -255;
	}
	else if(bright > 255){
		bright = 255;
	}
	if(p_r < -255){
		p_r = -255;
	}
	else if(p_r > 255){
		p_r = 255;
	}
	if(p_g < -255){
		p_g = -255;
	}
	else if(p_g > 255){
		p_g = 255;
	}
	if(p_b < -255){
		p_b = -255;
	}
	else if(p_b > 255){
		p_b = 255;
	}
	if(bright < 0){
		p_r += (((255 + p_r) * bright) / 255);
		p_g += (((255 + p_g) * bright) / 255);
		p_b += (((255 + p_b) * bright) / 255);
	}
	else if(bright > 0){
		p_r += (((255 - p_r) * bright) / 255);
		p_g += (((255 - p_g) * bright) / 255);
		p_b += (((255 - p_b) * bright) / 255);
	}

	if(disp_mod == 2){		// ���Z���[�h
		p_r = -(p_r);
		p_g = -(p_g);
		p_b = -(p_b);
	}

	RGBFILTER_p_r = p_r;
	RGBFILTER_p_g = p_g;
	RGBFILTER_p_b = p_b;

	if(p_r >= 0){
		CTPR = *(Gclr_table_adr_p + (255 - p_r));
		GRPF_p_r = 255;
	}
	else{
		CTPR = *(Gclr_table_adr_p + (255 + p_r));
		GRPF_p_r = 0;
	}
	if(p_g >= 0){
		CTPG = *(Gclr_table_adr_p + (255 - p_g));
		GRPF_p_g = 255;
	}
	else{
		CTPG = *(Gclr_table_adr_p + (255 + p_g));
		GRPF_p_g = 0;
	}
	if(p_b >= 0){
		CTPB = *(Gclr_table_adr_p + (255 - p_b));
		GRPF_p_b = 255;
	}
	else{
		CTPB = *(Gclr_table_adr_p + (255 + p_b));
		GRPF_p_b = 0;
	}

// �^�C���p�����[�^
	if(tile){
		GRPF_t_add = t_add;
		GRPF_t_reverse = t_reverse;
		GRPF_t_shift = t_shift;
		GRPF_t_xl = t_xl;
		GRPF_t_yl = t_yl;

		if(d_free_x >= t_repx){
			GRPF_t_x = GRPF_t_x_org = (d_free_x - t_repx) % t_xl;
		}
		else{
			GRPF_t_x = GRPF_t_x_org = (t_xl - ((t_repx - d_free_x) % t_xl)) % t_xl;
		}
		if(d_free_y >= t_repy){
			GRPF_t_y                = (d_free_y - t_repy) % t_yl;
		}
		else{
			GRPF_t_y                = (t_yl - ((t_repy - d_free_y) % t_yl)) % t_yl;
		}

		GRPF_t_adr     = tile + (GRPF_t_y * t_xl);
		GRPF_t_adr_org = tile;
		GRPF_t_adr_max = tile + (t_xl * t_yl);
	}

//****************************************************************************************

	if(type == 0){
		if(clr == 255){
			if(c_rate == 0){
				if(p_r==0 && p_g==0 && p_b==0){
					if(mono == 0){
						if(reverse == 0){
							if(tile==0 || t_add==255){
// �ʏ�\��
								switch(disp_mod){
									case 0: tcgd_normal(); break;
									case 1: tcgd_normal_add(); break;
									case 2: tcgd_normal_sub(); break;
									case 3: tcgd_normal_mod03_alphablend(); break;
									case 4: tcgd_normal_rgbfilter(); break;
								}
							}
							else{
// �^�C���ʏ�\��
								switch(disp_mod){
									case 0: tcgd_normal_tile(); break;
									case 1: tcgd_normal_tile_add(); break;
									case 2: tcgd_normal_tile_sub(); break;
									case 3: break;
									case 4: tcgd_normal_tile_rgbfilter(); break;
									case 99: tcgd_normal(); break;
								}
							}
						}
						else{
							if(tile==0 || t_add==255){
// ���]�ʏ�\��
								switch(disp_mod){
									case 0: tcgd_normal_reverse(); break;
									case 1: tcgd_normal_reverse_add(); break;
									case 2: tcgd_normal_reverse_sub(); break;
									case 3: break;
									case 4: tcgd_normal_reverse_rgbfilter(); break;
								}
							}
							else{
// �^�C�����]�ʏ�\��
								switch(disp_mod){
									case 0: tcgd_normal_reverse_tile(); break;
									case 1: tcgd_normal_reverse_tile_add(); break;
									case 2: tcgd_normal_reverse_tile_sub(); break;
									case 3: break;
									case 4: tcgd_normal_reverse_tile_rgbfilter(); break;
								}
							}
						}
					}
					else{
						if(reverse == 0){
							if(tile==0 || t_add==255){
// ���m�ʏ�\��
								switch(disp_mod){
									case 0: tcgd_normal_mono(); break;
									case 1: tcgd_normal_mono_add(); break;
									case 2: tcgd_normal_mono_sub(); break;
									case 3: break;
									case 4: tcgd_normal_mono_rgbfilter(); break;
								}
							}
							else{
// �^�C�����m�ʏ�\��
								switch(disp_mod){
									case 0: tcgd_normal_mono_tile(); break;
									case 1: tcgd_normal_mono_tile_add(); break;
									case 2: tcgd_normal_mono_tile_sub(); break;
									case 3: break;
									case 4: tcgd_normal_mono_tile_rgbfilter(); break;
								}
							}
						}
						else{
							if(tile==0 || t_add==255){
// ���]���m�ʏ�\��
								switch(disp_mod){
									case 0: tcgd_normal_reverse_mono(); break;
									case 1: tcgd_normal_reverse_mono_add(); break;
									case 2: tcgd_normal_reverse_mono_sub(); break;
									case 3: break;
									case 4: tcgd_normal_reverse_mono_rgbfilter(); break;
								}
							}
							else{
// �^�C�����]���m�ʏ�\��
								switch(disp_mod){
									case 0: tcgd_normal_reverse_mono_tile(); break;
									case 1: tcgd_normal_reverse_mono_tile_add(); break;
									case 2: tcgd_normal_reverse_mono_tile_sub(); break;
									case 3: break;
									case 4: tcgd_normal_reverse_mono_tile_rgbfilter(); break;
								}
							}
						}
					}
				}
				else{
					if(mono == 0){
						if(reverse == 0){
							if(tile==0 || t_add==255){
// ���Òʏ�\��
								switch(disp_mod){
									case 0: tcgd_normal_bright(); break;
									case 1: tcgd_normal_bright_add(); break;
									case 2: tcgd_normal_bright_sub(); break;
									case 3: break;
									case 4: tcgd_normal_bright_rgbfilter(); break;
								}
							}
							else{
// �^�C�����Òʏ�\��
								switch(disp_mod){
									case 0: tcgd_normal_bright_tile(); break;
									case 1: tcgd_normal_bright_tile_add(); break;
									case 2: tcgd_normal_bright_tile_sub(); break;
									case 3: break;
									case 4: tcgd_normal_bright_tile_rgbfilter(); break;
								}
							}
						}
						else{
							if(tile==0 || t_add==255){
// ���]���Òʏ�\��
								switch(disp_mod){
									case 0: tcgd_normal_reverse_bright(); break;
									case 1: tcgd_normal_reverse_bright_add(); break;
									case 2: tcgd_normal_reverse_bright_sub(); break;
									case 3: break;
									case 4: tcgd_normal_reverse_bright_rgbfilter(); break;
								}
							}
							else{
// �^�C�����]���Òʏ�\��
								switch(disp_mod){
									case 0: tcgd_normal_reverse_bright_tile(); break;
									case 1: tcgd_normal_reverse_bright_tile_add(); break;
									case 2: tcgd_normal_reverse_bright_tile_sub(); break;
									case 3: break;
									case 4: tcgd_normal_reverse_bright_tile_rgbfilter(); break;
								}
							}
						}
					}
					else{
						if(reverse == 0){
							if(tile==0 || t_add==255){
// ���m���Òʏ�\��
								switch(disp_mod){
									case 0: tcgd_normal_mono_bright(); break;
									case 1: tcgd_normal_mono_bright_add(); break;
									case 2: tcgd_normal_mono_bright_sub(); break;
									case 3: break;
									case 4: tcgd_normal_mono_bright_rgbfilter(); break;
								}
							}
							else{
// �^�C�����m���Òʏ�\��
								switch(disp_mod){
									case 0: tcgd_normal_mono_bright_tile(); break;
									case 1: tcgd_normal_mono_bright_tile_add(); break;
									case 2: tcgd_normal_mono_bright_tile_sub(); break;
									case 3: break;
									case 4: tcgd_normal_mono_bright_tile_rgbfilter(); break;
								}
							}
						}
						else{
							if(tile==0 || t_add==255){
// ���]���m���Òʏ�\��
								switch(disp_mod){
									case 0: tcgd_normal_reverse_mono_bright(); break;
									case 1: tcgd_normal_reverse_mono_bright_add(); break;
									case 2: tcgd_normal_reverse_mono_bright_sub(); break;
									case 3: break;
									case 4: tcgd_normal_reverse_mono_bright_rgbfilter(); break;
								}
							}
							else{
// �^�C�����]���m���Òʏ�\��
								switch(disp_mod){
									case 0: tcgd_normal_reverse_mono_bright_tile(); break;
									case 1: tcgd_normal_reverse_mono_bright_tile_add(); break;
									case 2: tcgd_normal_reverse_mono_bright_tile_sub(); break;
									case 3: break;
									case 4: tcgd_normal_reverse_mono_bright_tile_rgbfilter(); break;
								}
							}
						}
					}
				}
			}
			else{
				if(p_r==0 && p_g==0 && p_b==0){
					if(mono == 0){
						if(reverse == 0){
							if(tile==0 || t_add==255){
// �ϐF�ʏ�\��
								switch(disp_mod){
									case 0: tcgd_normal_change(); break;
									case 1: tcgd_normal_change_add(); break;
									case 2: tcgd_normal_change_sub(); break;
									case 3: tcgd_normal_change_mod03_alphablend(); break;
									case 4: tcgd_normal_change_rgbfilter(); break;
								}
							}
							else{
// �^�C���ϐF�ʏ�\��
								switch(disp_mod){
									case 0: tcgd_normal_change_tile(); break;
									case 1: tcgd_normal_change_tile_add(); break;
									case 2: tcgd_normal_change_tile_sub(); break;
									case 3: break;
									case 4: tcgd_normal_change_tile_rgbfilter(); break;
								}
							}
						}
						else{
							if(tile==0 || t_add==255){
// ���]�ϐF�ʏ�\��
								switch(disp_mod){
									case 0: tcgd_normal_reverse_change(); break;
									case 1: tcgd_normal_reverse_change_add(); break;
									case 2: tcgd_normal_reverse_change_sub(); break;
									case 3: break;
									case 4: tcgd_normal_reverse_change_rgbfilter(); break;
								}
							}
							else{
// �^�C�����]�ϐF�ʏ�\��
								switch(disp_mod){
									case 0: tcgd_normal_reverse_change_tile(); break;
									case 1: tcgd_normal_reverse_change_tile_add(); break;
									case 2: tcgd_normal_reverse_change_tile_sub(); break;
									case 3: break;
									case 4: tcgd_normal_reverse_change_tile_rgbfilter(); break;
								}
							}
						}
					}
					else{
						if(reverse == 0){
							if(tile==0 || t_add==255){
// ���m�ϐF�ʏ�\��
								switch(disp_mod){
									case 0: tcgd_normal_mono_change(); break;
									case 1: tcgd_normal_mono_change_add(); break;
									case 2: tcgd_normal_mono_change_sub(); break;
									case 3: break;
									case 4: tcgd_normal_mono_change_rgbfilter(); break;
								}
							}
							else{
// �^�C�����m�ϐF�ʏ�\��
								switch(disp_mod){
									case 0: tcgd_normal_mono_change_tile(); break;
									case 1: tcgd_normal_mono_change_tile_add(); break;
									case 2: tcgd_normal_mono_change_tile_sub(); break;
									case 3: break;
									case 4: tcgd_normal_mono_change_tile_rgbfilter(); break;
								}
							}
						}
						else{
							if(tile==0 || t_add==255){
// ���]���m�ϐF�ʏ�\��
								switch(disp_mod){
									case 0: tcgd_normal_reverse_mono_change(); break;
									case 1: tcgd_normal_reverse_mono_change_add(); break;
									case 2: tcgd_normal_reverse_mono_change_sub(); break;
									case 3: break;
									case 4: tcgd_normal_reverse_mono_change_rgbfilter(); break;
								}
							}
							else{
// �^�C�����]���m�ϐF�ʏ�\��
								switch(disp_mod){
									case 0: tcgd_normal_reverse_mono_change_tile(); break;
									case 1: tcgd_normal_reverse_mono_change_tile_add(); break;
									case 2: tcgd_normal_reverse_mono_change_tile_sub(); break;
									case 3: break;
									case 4: tcgd_normal_reverse_mono_change_tile_rgbfilter(); break;
								}
							}
						}
					}
				}
				else{
					if(mono == 0){
						if(reverse == 0){
							if(tile==0 || t_add==255){
// �ϐF���Òʏ�\��
								switch(disp_mod){
									case 0: tcgd_normal_change_bright(); break;
									case 1: tcgd_normal_change_bright_add(); break;
									case 2: tcgd_normal_change_bright_sub(); break;
									case 3: break;
									case 4: tcgd_normal_change_bright_rgbfilter(); break;
								}
							}
							else{
// �^�C���ϐF���Òʏ�\��
								switch(disp_mod){
									case 0: tcgd_normal_change_bright_tile(); break;
									case 1: tcgd_normal_change_bright_tile_add(); break;
									case 2: tcgd_normal_change_bright_tile_sub(); break;
									case 3: break;
									case 4: tcgd_normal_change_bright_tile_rgbfilter(); break;
								}
							}
						}
						else{
							if(tile==0 || t_add==255){
// ���]�ϐF���Òʏ�\��
								switch(disp_mod){
									case 0: tcgd_normal_reverse_change_bright(); break;
									case 1: tcgd_normal_reverse_change_bright_add(); break;
									case 2: tcgd_normal_reverse_change_bright_sub(); break;
									case 3: break;
									case 4: tcgd_normal_reverse_change_bright_rgbfilter(); break;
								}
							}
							else{
// �^�C�����]�ϐF���Òʏ�\��
								switch(disp_mod){
									case 0: tcgd_normal_reverse_change_bright_tile(); break;
									case 1: tcgd_normal_reverse_change_bright_tile_add(); break;
									case 2: tcgd_normal_reverse_change_bright_tile_sub(); break;
									case 3: break;
									case 4: tcgd_normal_reverse_change_bright_tile_rgbfilter(); break;
								}
							}
						}
					}
					else{
						if(reverse == 0){
							if(tile==0 || t_add==255){
// ���m�ϐF���Òʏ�\��
								switch(disp_mod){
									case 0: tcgd_normal_mono_change_bright(); break;
									case 1: tcgd_normal_mono_change_bright_add(); break;
									case 2: tcgd_normal_mono_change_bright_sub(); break;
									case 3: break;
									case 4: tcgd_normal_mono_change_bright_rgbfilter(); break;
								}
							}
							else{
// �^�C�����m�ϐF���Òʏ�\��
								switch(disp_mod){
									case 0: tcgd_normal_mono_change_bright_tile(); break;
									case 1: tcgd_normal_mono_change_bright_tile_add(); break;
									case 2: tcgd_normal_mono_change_bright_tile_sub(); break;
									case 3: break;
									case 4: tcgd_normal_mono_change_bright_tile_rgbfilter(); break;
								}
							}
						}
						else{
							if(tile==0 || t_add==255){
// ���]���m�ϐF���Òʏ�\��
								switch(disp_mod){
									case 0: tcgd_normal_reverse_mono_change_bright(); break;
									case 1: tcgd_normal_reverse_mono_change_bright_add(); break;
									case 2: tcgd_normal_reverse_mono_change_bright_sub(); break;
									case 3: break;
									case 4: tcgd_normal_reverse_mono_change_bright_rgbfilter(); break;
								}
							}
							else{
// �^�C�����]���m�ϐF���Òʏ�\��
								switch(disp_mod){
									case 0: tcgd_normal_reverse_mono_change_bright_tile(); break;
									case 1: tcgd_normal_reverse_mono_change_bright_tile_add(); break;
									case 2: tcgd_normal_reverse_mono_change_bright_tile_sub(); break;
									case 3: break;
									case 4: tcgd_normal_reverse_mono_change_bright_tile_rgbfilter(); break;
								}
							}
						}
					}
				}
			}
		}
		else{
			if(c_rate == 0){
				if(p_r==0 && p_g==0 && p_b==0){
					if(mono == 0){
						if(reverse == 0){
							if(tile==0 || t_add==255){
// �����ʏ�\��
								switch(disp_mod){
									case 0: tcgd_normal_clr(); break;
									case 1: tcgd_normal_clr_add(); break;
									case 2: tcgd_normal_clr_sub(); break;
									case 3: break;
									case 4: tcgd_normal_clr_rgbfilter(); break;
								}
							}
							else{
// �^�C�������ʏ�\��
								switch(disp_mod){
									case 0: tcgd_normal_clr_tile(); break;
									case 1: tcgd_normal_clr_tile_add(); break;
									case 2: tcgd_normal_clr_tile_sub(); break;
									case 3: break;
									case 4: tcgd_normal_clr_tile_rgbfilter(); break;
									case 99: tcgd_normal_clr_tile_wave(); break;
								}
							}
						}
						else{
							if(tile==0 || t_add==255){
// ���]�����ʏ�\��
								switch(disp_mod){
									case 0: tcgd_normal_reverse_clr(); break;
									case 1: tcgd_normal_reverse_clr_add(); break;
									case 2: tcgd_normal_reverse_clr_sub(); break;
									case 3: break;
									case 4: tcgd_normal_reverse_clr_rgbfilter(); break;
								}
							}
							else{
// �^�C�����]�����ʏ�\��
								switch(disp_mod){
									case 0: tcgd_normal_reverse_clr_tile(); break;
									case 1: tcgd_normal_reverse_clr_tile_add(); break;
									case 2: tcgd_normal_reverse_clr_tile_sub(); break;
									case 3: break;
									case 4: tcgd_normal_reverse_clr_tile_rgbfilter(); break;
								}
							}
						}
					}
					else{
						if(reverse == 0){
							if(tile==0 || t_add==255){
// ���m�����ʏ�\��
								switch(disp_mod){
									case 0: tcgd_normal_mono_clr(); break;
									case 1: tcgd_normal_mono_clr_add(); break;
									case 2: tcgd_normal_mono_clr_sub(); break;
									case 3: break;
									case 4: tcgd_normal_mono_clr_rgbfilter(); break;
								}
							}
							else{
// �^�C�����m�����ʏ�\��
								switch(disp_mod){
									case 0: tcgd_normal_mono_clr_tile(); break;
									case 1: tcgd_normal_mono_clr_tile_add(); break;
									case 2: tcgd_normal_mono_clr_tile_sub(); break;
									case 3: break;
									case 4: tcgd_normal_mono_clr_tile_rgbfilter(); break;
								}
							}
						}
						else{
							if(tile==0 || t_add==255){
// ���]���m�����ʏ�\��
								switch(disp_mod){
									case 0: tcgd_normal_reverse_mono_clr(); break;
									case 1: tcgd_normal_reverse_mono_clr_add(); break;
									case 2: tcgd_normal_reverse_mono_clr_sub(); break;
									case 3: break;
									case 4: tcgd_normal_reverse_mono_clr_rgbfilter(); break;
								}
							}
							else{
// �^�C�����]���m�����ʏ�\��
								switch(disp_mod){
									case 0: tcgd_normal_reverse_mono_clr_tile(); break;
									case 1: tcgd_normal_reverse_mono_clr_tile_add(); break;
									case 2: tcgd_normal_reverse_mono_clr_tile_sub(); break;
									case 3: break;
									case 4: tcgd_normal_reverse_mono_clr_tile_rgbfilter(); break;
								}
							}
						}
					}
				}
				else{
					if(mono == 0){
						if(reverse == 0){
							if(tile==0 || t_add==255){
// ���Ó����ʏ�\��
								switch(disp_mod){
									case 0: tcgd_normal_bright_clr(); break;
									case 1: tcgd_normal_bright_clr_add(); break;
									case 2: tcgd_normal_bright_clr_sub(); break;
									case 3: break;
									case 4: tcgd_normal_bright_clr_rgbfilter(); break;
								}
							}
							else{
// �^�C�����Ó����ʏ�\��
								switch(disp_mod){
									case 0: tcgd_normal_bright_clr_tile(); break;
									case 1: tcgd_normal_bright_clr_tile_add(); break;
									case 2: tcgd_normal_bright_clr_tile_sub(); break;
									case 3: break;
									case 4: tcgd_normal_bright_clr_tile_rgbfilter(); break;
								}
							}
						}
						else{
							if(tile==0 || t_add==255){
// ���]���Ó����ʏ�\��
								switch(disp_mod){
									case 0: tcgd_normal_reverse_bright_clr(); break;
									case 1: tcgd_normal_reverse_bright_clr_add(); break;
									case 2: tcgd_normal_reverse_bright_clr_sub(); break;
									case 3: break;
									case 4: tcgd_normal_reverse_bright_clr_rgbfilter(); break;
								}
							}
							else{
// �^�C�����]���Ó����ʏ�\��
								switch(disp_mod){
									case 0: tcgd_normal_reverse_bright_clr_tile(); break;
									case 1: tcgd_normal_reverse_bright_clr_tile_add(); break;
									case 2: tcgd_normal_reverse_bright_clr_tile_sub(); break;
									case 3: break;
									case 4: tcgd_normal_reverse_bright_clr_tile_rgbfilter(); break;
								}
							}
						}
					}
					else{
						if(reverse == 0){
							if(tile==0 || t_add==255){
// ���m���Ó����ʏ�\��
								switch(disp_mod){
									case 0: tcgd_normal_mono_bright_clr(); break;
									case 1: tcgd_normal_mono_bright_clr_add(); break;
									case 2: tcgd_normal_mono_bright_clr_sub(); break;
									case 3: break;
									case 4: tcgd_normal_mono_bright_clr_rgbfilter(); break;
								}
							}
							else{
// �^�C�����m���Ó����ʏ�\��
								switch(disp_mod){
									case 0: tcgd_normal_mono_bright_clr_tile(); break;
									case 1: tcgd_normal_mono_bright_clr_tile_add(); break;
									case 2: tcgd_normal_mono_bright_clr_tile_sub(); break;
									case 3: break;
									case 4: tcgd_normal_mono_bright_clr_tile_rgbfilter(); break;
								}
							}
						}
						else{
							if(tile==0 || t_add==255){
// ���]���m���Ó����ʏ�\��
								switch(disp_mod){
									case 0: tcgd_normal_reverse_mono_bright_clr(); break;
									case 1: tcgd_normal_reverse_mono_bright_clr_add(); break;
									case 2: tcgd_normal_reverse_mono_bright_clr_sub(); break;
									case 3: break;
									case 4: tcgd_normal_reverse_mono_bright_clr_rgbfilter(); break;
								}
							}
							else{
// �^�C�����]���m���Ó����ʏ�\��
								switch(disp_mod){
									case 0: tcgd_normal_reverse_mono_bright_clr_tile(); break;
									case 1: tcgd_normal_reverse_mono_bright_clr_tile_add(); break;
									case 2: tcgd_normal_reverse_mono_bright_clr_tile_sub(); break;
									case 3: break;
									case 4: tcgd_normal_reverse_mono_bright_clr_tile_rgbfilter(); break;
								}
							}
						}
					}
				}
			}
			else{
				if(p_r==0 && p_g==0 && p_b==0){
					if(mono == 0){
						if(reverse == 0){
							if(tile==0 || t_add==255){
// �ϐF�����ʏ�\��
								switch(disp_mod){
									case 0: tcgd_normal_change_clr(); break;
									case 1: tcgd_normal_change_clr_add(); break;
									case 2: tcgd_normal_change_clr_sub(); break;
									case 3: break;
									case 4: tcgd_normal_change_clr_rgbfilter(); break;
								}
							}
							else{
// �^�C���ϐF�����ʏ�\��
								switch(disp_mod){
									case 0: tcgd_normal_change_clr_tile(); break;
									case 1: tcgd_normal_change_clr_tile_add(); break;
									case 2: tcgd_normal_change_clr_tile_sub(); break;
									case 3: break;
									case 4: tcgd_normal_change_clr_tile_rgbfilter(); break;
								}
							}
						}
						else{
							if(tile==0 || t_add==255){
// ���]�ϐF�����ʏ�\��
								switch(disp_mod){
									case 0: tcgd_normal_reverse_change_clr(); break;
									case 1: tcgd_normal_reverse_change_clr_add(); break;
									case 2: tcgd_normal_reverse_change_clr_sub(); break;
									case 3: break;
									case 4: tcgd_normal_reverse_change_clr_rgbfilter(); break;
								}
							}
							else{
// �^�C�����]�ϐF�����ʏ�\��
								switch(disp_mod){
									case 0: tcgd_normal_reverse_change_clr_tile(); break;
									case 1: tcgd_normal_reverse_change_clr_tile_add(); break;
									case 2: tcgd_normal_reverse_change_clr_tile_sub(); break;
									case 3: break;
									case 4: tcgd_normal_reverse_change_clr_tile_rgbfilter(); break;
								}
							}
						}
					}
					else{
						if(reverse == 0){
							if(tile==0 || t_add==255){
// ���m�ϐF�����ʏ�\��
								switch(disp_mod){
									case 0: tcgd_normal_mono_change_clr(); break;
									case 1: tcgd_normal_mono_change_clr_add(); break;
									case 2: tcgd_normal_mono_change_clr_sub(); break;
									case 3: break;
									case 4: tcgd_normal_mono_change_clr_rgbfilter(); break;
								}
							}
							else{
// �^�C�����m�ϐF�����ʏ�\��
								switch(disp_mod){
									case 0: tcgd_normal_mono_change_clr_tile(); break;
									case 1: tcgd_normal_mono_change_clr_tile_add(); break;
									case 2: tcgd_normal_mono_change_clr_tile_sub(); break;
									case 3: break;
									case 4: tcgd_normal_mono_change_clr_tile_rgbfilter(); break;
								}
							}
						}
						else{
							if(tile==0 || t_add==255){
// ���]���m�ϐF�����ʏ�\��
								switch(disp_mod){
									case 0: tcgd_normal_reverse_mono_change_clr(); break;
									case 1: tcgd_normal_reverse_mono_change_clr_add(); break;
									case 2: tcgd_normal_reverse_mono_change_clr_sub(); break;
									case 3: break;
									case 4: tcgd_normal_reverse_mono_change_clr_rgbfilter(); break;
								}
							}
							else{
// �^�C�����]���m�ϐF�����ʏ�\��
								switch(disp_mod){
									case 0: tcgd_normal_reverse_mono_change_clr_tile(); break;
									case 1: tcgd_normal_reverse_mono_change_clr_tile_add(); break;
									case 2: tcgd_normal_reverse_mono_change_clr_tile_sub(); break;
									case 3: break;
									case 4: tcgd_normal_reverse_mono_change_clr_tile_rgbfilter(); break;
								}
							}
						}
					}
				}
				else{
					if(mono == 0){
						if(reverse == 0){
							if(tile==0 || t_add==255){
// �ϐF���Ó����ʏ�\��
								switch(disp_mod){
									case 0: tcgd_normal_change_bright_clr(); break;
									case 1: tcgd_normal_change_bright_clr_add(); break;
									case 2: tcgd_normal_change_bright_clr_sub(); break;
									case 3: break;
									case 4: tcgd_normal_change_bright_clr_rgbfilter(); break;
								}
							}
							else{
// �^�C���ϐF���Ó����ʏ�\��
								switch(disp_mod){
									case 0: tcgd_normal_change_bright_clr_tile(); break;
									case 1: tcgd_normal_change_bright_clr_tile_add(); break;
									case 2: tcgd_normal_change_bright_clr_tile_sub(); break;
									case 3: break;
									case 4: tcgd_normal_change_bright_clr_tile_rgbfilter(); break;
								}
							}
						}
						else{
							if(tile==0 || t_add==255){
// ���]�ϐF���Ó����ʏ�\��
								switch(disp_mod){
									case 0: tcgd_normal_reverse_change_bright_clr(); break;
									case 1: tcgd_normal_reverse_change_bright_clr_add(); break;
									case 2: tcgd_normal_reverse_change_bright_clr_sub(); break;
									case 3: break;
									case 4: tcgd_normal_reverse_change_bright_clr_rgbfilter(); break;
								}
							}
							else{
// �^�C�����]�ϐF���Ó����ʏ�\��
								switch(disp_mod){
									case 0: tcgd_normal_reverse_change_bright_clr_tile(); break;
									case 1: tcgd_normal_reverse_change_bright_clr_tile_add(); break;
									case 2: tcgd_normal_reverse_change_bright_clr_tile_sub(); break;
									case 3: break;
									case 4: tcgd_normal_reverse_change_bright_clr_tile_rgbfilter(); break;
								}
							}
						}
					}
					else{
						if(reverse == 0){
							if(tile==0 || t_add==255){
// ���m�ϐF���Ó����ʏ�\��
								switch(disp_mod){
									case 0: tcgd_normal_mono_change_bright_clr(); break;
									case 1: tcgd_normal_mono_change_bright_clr_add(); break;
									case 2: tcgd_normal_mono_change_bright_clr_sub(); break;
									case 3: break;
									case 4: tcgd_normal_mono_change_bright_clr_rgbfilter(); break;
								}
							}
							else{
// �^�C�����m�ϐF���Ó����ʏ�\��
								switch(disp_mod){
									case 0: tcgd_normal_mono_change_bright_clr_tile(); break;
									case 1: tcgd_normal_mono_change_bright_clr_tile_add(); break;
									case 2: tcgd_normal_mono_change_bright_clr_tile_sub(); break;
									case 3: break;
									case 4: tcgd_normal_mono_change_bright_clr_tile_rgbfilter(); break;
								}
							}
						}
						else{
							if(tile==0 || t_add==255){
// ���]���m�ϐF���Ó����ʏ�\��
								switch(disp_mod){
									case 0: tcgd_normal_reverse_mono_change_bright_clr(); break;
									case 1: tcgd_normal_reverse_mono_change_bright_clr_add(); break;
									case 2: tcgd_normal_reverse_mono_change_bright_clr_sub(); break;
									case 3: break;
									case 4: tcgd_normal_reverse_mono_change_bright_clr_rgbfilter(); break;
								}
							}
							else{
// �^�C�����]���m�ϐF���Ó����ʏ�\��
								switch(disp_mod){
									case 0: tcgd_normal_reverse_mono_change_bright_clr_tile(); break;
									case 1: tcgd_normal_reverse_mono_change_bright_clr_tile_add(); break;
									case 2: tcgd_normal_reverse_mono_change_bright_clr_tile_sub(); break;
									case 3: break;
									case 4: tcgd_normal_reverse_mono_change_bright_clr_tile_rgbfilter(); break;
								}
							}
						}
					}
				}
			}
		}
	}
//****************************************************************************************
	else{
		if(clr == 255){
			if(c_rate == 0){
				if(p_r==0 && p_g==0 && p_b==0){
					if(mono == 0){
						if(reverse == 0){
							if(tile==0 || t_add==255){
// �X�v���C�g�\��
								switch(disp_mod){
									case 0: tcgd_sprite(); break;
									case 1: tcgd_sprite_add(); break;
									case 2: tcgd_sprite_sub(); break;
									case 3: tcgd_sprite_mod03_alphablend(); break;
									case 4: tcgd_sprite_rgbfilter(); break;
								}
							}
							else{
// �^�C���X�v���C�g�\��
								switch(disp_mod){
									case 0: tcgd_sprite_tile(); break;
									case 1: tcgd_sprite_tile_add(); break;
									case 2: tcgd_sprite_tile_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_tile_rgbfilter(); break;
								}
							}
						}
						else{
							if(tile==0 || t_add==255){
// ���]�X�v���C�g�\��
								switch(disp_mod){
									case 0: tcgd_sprite_reverse(); break;
									case 1: tcgd_sprite_reverse_add(); break;
									case 2: tcgd_sprite_reverse_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_reverse_rgbfilter(); break;
								}
							}
							else{
// �^�C�����]�X�v���C�g�\��
								switch(disp_mod){
									case 0: tcgd_sprite_reverse_tile(); break;
									case 1: tcgd_sprite_reverse_tile_add(); break;
									case 2: tcgd_sprite_reverse_tile_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_reverse_tile_rgbfilter(); break;
								}
							}
						}
					}
					else{
						if(reverse == 0){
							if(tile==0 || t_add==255){
// ���m�X�v���C�g�\��
								switch(disp_mod){
									case 0: tcgd_sprite_mono(); break;
									case 1: tcgd_sprite_mono_add(); break;
									case 2: tcgd_sprite_mono_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_mono_rgbfilter(); break;
								}
							}
							else{
// �^�C�����m�X�v���C�g�\��
								switch(disp_mod){
									case 0: tcgd_sprite_mono_tile(); break;
									case 1: tcgd_sprite_mono_tile_add(); break;
									case 2: tcgd_sprite_mono_tile_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_mono_tile_rgbfilter(); break;
								}
							}
						}
						else{
							if(tile==0 || t_add==255){
// ���]���m�X�v���C�g�\��
								switch(disp_mod){
									case 0: tcgd_sprite_reverse_mono(); break;
									case 1: tcgd_sprite_reverse_mono_add(); break;
									case 2: tcgd_sprite_reverse_mono_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_reverse_mono_rgbfilter(); break;
								}
							}
							else{
// �^�C�����]���m�X�v���C�g�\��
								switch(disp_mod){
									case 0: tcgd_sprite_reverse_mono_tile(); break;
									case 1: tcgd_sprite_reverse_mono_tile_add(); break;
									case 2: tcgd_sprite_reverse_mono_tile_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_reverse_mono_tile_rgbfilter(); break;
								}
							}
						}
					}
				}
				else{
					if(mono == 0){
						if(reverse == 0){
							if(tile==0 || t_add==255){
// ���ÃX�v���C�g�\��
								switch(disp_mod){
									case 0: tcgd_sprite_bright(); break;
									case 1: tcgd_sprite_bright_add(); break;
									case 2: tcgd_sprite_bright_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_bright_rgbfilter(); break;
								}
							}
							else{
// �^�C�����ÃX�v���C�g�\��
								switch(disp_mod){
									case 0: tcgd_sprite_bright_tile(); break;
									case 1: tcgd_sprite_bright_tile_add(); break;
									case 2: tcgd_sprite_bright_tile_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_bright_tile_rgbfilter(); break;
								}
							}
						}
						else{
							if(tile==0 || t_add==255){
// ���]���ÃX�v���C�g�\��
								switch(disp_mod){
									case 0: tcgd_sprite_reverse_bright(); break;
									case 1: tcgd_sprite_reverse_bright_add(); break;
									case 2: tcgd_sprite_reverse_bright_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_reverse_bright_rgbfilter(); break;
								}
							}
							else{
// �^�C�����]���ÃX�v���C�g�\��
								switch(disp_mod){
									case 0: tcgd_sprite_reverse_bright_tile(); break;
									case 1: tcgd_sprite_reverse_bright_tile_add(); break;
									case 2: tcgd_sprite_reverse_bright_tile_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_reverse_bright_tile_rgbfilter(); break;
								}
							}
						}
					}
					else{
						if(reverse == 0){
							if(tile==0 || t_add==255){
// ���m���ÃX�v���C�g�\��
								switch(disp_mod){
									case 0: tcgd_sprite_mono_bright(); break;
									case 1: tcgd_sprite_mono_bright_add(); break;
									case 2: tcgd_sprite_mono_bright_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_mono_bright_rgbfilter(); break;
								}
							}
							else{
// �^�C�����m���ÃX�v���C�g�\��
								switch(disp_mod){
									case 0: tcgd_sprite_mono_bright_tile(); break;
									case 1: tcgd_sprite_mono_bright_tile_add(); break;
									case 2: tcgd_sprite_mono_bright_tile_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_mono_bright_tile_rgbfilter(); break;
								}
							}
						}
						else{
							if(tile==0 || t_add==255){
// ���]���m���ÃX�v���C�g�\��
								switch(disp_mod){
									case 0: tcgd_sprite_reverse_mono_bright(); break;
									case 1: tcgd_sprite_reverse_mono_bright_add(); break;
									case 2: tcgd_sprite_reverse_mono_bright_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_reverse_mono_bright_rgbfilter(); break;
								}
							}
							else{
// �^�C�����]���m���ÃX�v���C�g�\��
								switch(disp_mod){
									case 0: tcgd_sprite_reverse_mono_bright_tile(); break;
									case 1: tcgd_sprite_reverse_mono_bright_tile_add(); break;
									case 2: tcgd_sprite_reverse_mono_bright_tile_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_reverse_mono_bright_tile_rgbfilter(); break;
								}
							}
						}
					}
				}
			}
			else{
				if(p_r==0 && p_g==0 && p_b==0){
					if(mono == 0){
						if(reverse == 0){
							if(tile==0 || t_add==255){
// �ϐF�X�v���C�g�\��
								switch(disp_mod){
									case 0: tcgd_sprite_change(); break;
									case 1: tcgd_sprite_change_add(); break;
									case 2: tcgd_sprite_change_sub(); break;
									case 3: tcgd_sprite_change_mod03_alphablend(); break;
									case 4: tcgd_sprite_change_rgbfilter(); break;
								}
							}
							else{
// �^�C���ϐF�ʏ�\��
								switch(disp_mod){
									case 0: tcgd_sprite_change_tile(); break;
									case 1: tcgd_sprite_change_tile_add(); break;
									case 2: tcgd_sprite_change_tile_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_change_tile_rgbfilter(); break;
								}
							}
						}
						else{
							if(tile==0 || t_add==255){
// ���]�ϐF�X�v���C�g�\��
								switch(disp_mod){
									case 0: tcgd_sprite_reverse_change(); break;
									case 1: tcgd_sprite_reverse_change_add(); break;
									case 2: tcgd_sprite_reverse_change_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_reverse_change_rgbfilter(); break;
								}
							}
							else{
// �^�C�����]�ϐF�X�v���C�g�\��
								switch(disp_mod){
									case 0: tcgd_sprite_reverse_change_tile(); break;
									case 1: tcgd_sprite_reverse_change_tile_add(); break;
									case 2: tcgd_sprite_reverse_change_tile_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_reverse_change_tile_rgbfilter(); break;
								}
							}
						}
					}
					else{
						if(reverse == 0){
							if(tile==0 || t_add==255){
// ���m�ϐF�X�v���C�g�\��
								switch(disp_mod){
									case 0: tcgd_sprite_mono_change(); break;
									case 1: tcgd_sprite_mono_change_add(); break;
									case 2: tcgd_sprite_mono_change_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_mono_change_rgbfilter(); break;
								}
							}
							else{
// �^�C�����m�ϐF�X�v���C�g�\��
								switch(disp_mod){
									case 0: tcgd_sprite_mono_change_tile(); break;
									case 1: tcgd_sprite_mono_change_tile_add(); break;
									case 2: tcgd_sprite_mono_change_tile_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_mono_change_tile_rgbfilter(); break;
								}
							}
						}
						else{
							if(tile==0 || t_add==255){
// ���]���m�ϐF�X�v���C�g�\��
								switch(disp_mod){
									case 0: tcgd_sprite_reverse_mono_change(); break;
									case 1: tcgd_sprite_reverse_mono_change_add(); break;
									case 2: tcgd_sprite_reverse_mono_change_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_reverse_mono_change_rgbfilter(); break;
								}
							}
							else{
// �^�C�����]���m�ϐF�X�v���C�g�\��
								switch(disp_mod){
									case 0: tcgd_sprite_reverse_mono_change_tile(); break;
									case 1: tcgd_sprite_reverse_mono_change_tile_add(); break;
									case 2: tcgd_sprite_reverse_mono_change_tile_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_reverse_mono_change_tile_rgbfilter(); break;
								}
							}
						}
					}
				}
				else{
					if(mono == 0){
						if(reverse == 0){
							if(tile==0 || t_add==255){
// �ϐF���ÃX�v���C�g�\��
								switch(disp_mod){
									case 0: tcgd_sprite_change_bright(); break;
									case 1: tcgd_sprite_change_bright_add(); break;
									case 2: tcgd_sprite_change_bright_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_change_bright_rgbfilter(); break;
								}
							}
							else{
// �^�C���ϐF���ÃX�v���C�g�\��
								switch(disp_mod){
									case 0: tcgd_sprite_change_bright_tile(); break;
									case 1: tcgd_sprite_change_bright_tile_add(); break;
									case 2: tcgd_sprite_change_bright_tile_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_change_bright_tile_rgbfilter(); break;
								}
							}
						}
						else{
							if(tile==0 || t_add==255){
// ���]�ϐF���ÃX�v���C�g�\��
								switch(disp_mod){
									case 0: tcgd_sprite_reverse_change_bright(); break;
									case 1: tcgd_sprite_reverse_change_bright_add(); break;
									case 2: tcgd_sprite_reverse_change_bright_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_reverse_change_bright_rgbfilter(); break;
								}
							}
							else{
// �^�C�����]�ϐF���ÃX�v���C�g�\��
								switch(disp_mod){
									case 0: tcgd_sprite_reverse_change_bright_tile(); break;
									case 1: tcgd_sprite_reverse_change_bright_tile_add(); break;
									case 2: tcgd_sprite_reverse_change_bright_tile_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_reverse_change_bright_tile_rgbfilter(); break;
								}
							}
						}
					}
					else{
						if(reverse == 0){
							if(tile==0 || t_add==255){
// ���m�ϐF���ÃX�v���C�g�\��
								switch(disp_mod){
									case 0: tcgd_sprite_mono_change_bright(); break;
									case 1: tcgd_sprite_mono_change_bright_add(); break;
									case 2: tcgd_sprite_mono_change_bright_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_mono_change_bright_rgbfilter(); break;
								}
							}
							else{
// �^�C�����m�ϐF���ÃX�v���C�g�\��
								switch(disp_mod){
									case 0: tcgd_sprite_mono_change_bright_tile(); break;
									case 1: tcgd_sprite_mono_change_bright_tile_add(); break;
									case 2: tcgd_sprite_mono_change_bright_tile_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_mono_change_bright_tile_rgbfilter(); break;
								}
							}
						}
						else{
							if(tile==0 || t_add==255){
// ���]���m�ϐF���ÃX�v���C�g�\��
								switch(disp_mod){
									case 0: tcgd_sprite_reverse_mono_change_bright(); break;
									case 1: tcgd_sprite_reverse_mono_change_bright_add(); break;
									case 2: tcgd_sprite_reverse_mono_change_bright_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_reverse_mono_change_bright_rgbfilter(); break;
								}
							}
							else{
// �^�C�����]���m�ϐF���ÃX�v���C�g�\��
								switch(disp_mod){
									case 0: tcgd_sprite_reverse_mono_change_bright_tile(); break;
									case 1: tcgd_sprite_reverse_mono_change_bright_tile_add(); break;
									case 2: tcgd_sprite_reverse_mono_change_bright_tile_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_reverse_mono_change_bright_tile_rgbfilter(); break;
								}
							}
						}
					}
				}
			}
		}
		else{
			if(c_rate == 0){
				if(p_r==0 && p_g==0 && p_b==0){
					if(mono == 0){
						if(reverse == 0){
							if(tile==0 || t_add==255){
// �����X�v���C�g�\��
								switch(disp_mod){
									case 0: tcgd_sprite_clr(); break;
									case 1: tcgd_sprite_clr_add(); break;
									case 2: tcgd_sprite_clr_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_clr_rgbfilter(); break;
								}
							}
							else{
// �^�C�������X�v���C�g�\��
								switch(disp_mod){
									case 0: tcgd_sprite_clr_tile(); break;
									case 1: tcgd_sprite_clr_tile_add(); break;
									case 2: tcgd_sprite_clr_tile_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_clr_tile_rgbfilter(); break;
								}
							}
						}
						else{
							if(tile==0 || t_add==255){
// ���]�����X�v���C�g�\��
								switch(disp_mod){
									case 0: tcgd_sprite_reverse_clr(); break;
									case 1: tcgd_sprite_reverse_clr_add(); break;
									case 2: tcgd_sprite_reverse_clr_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_reverse_clr_rgbfilter(); break;
								}
							}
							else{
// �^�C�����]�����X�v���C�g�\��
								switch(disp_mod){
									case 0: tcgd_sprite_reverse_clr_tile(); break;
									case 1: tcgd_sprite_reverse_clr_tile_add(); break;
									case 2: tcgd_sprite_reverse_clr_tile_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_reverse_clr_tile_rgbfilter(); break;
								}
							}
						}
					}
					else{
						if(reverse == 0){
							if(tile==0 || t_add==255){
// ���m�����X�v���C�g�\��
								switch(disp_mod){
									case 0: tcgd_sprite_mono_clr(); break;
									case 1: tcgd_sprite_mono_clr_add(); break;
									case 2: tcgd_sprite_mono_clr_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_mono_clr_rgbfilter(); break;
								}
							}
							else{
// �^�C�����m�����X�v���C�g�\��
								switch(disp_mod){
									case 0: tcgd_sprite_mono_clr_tile(); break;
									case 1: tcgd_sprite_mono_clr_tile_add(); break;
									case 2: tcgd_sprite_mono_clr_tile_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_mono_clr_tile_rgbfilter(); break;
								}
							}
						}
						else{
							if(tile==0 || t_add==255){
// ���]���m�����X�v���C�g�\��
								switch(disp_mod){
									case 0: tcgd_sprite_reverse_mono_clr(); break;
									case 1: tcgd_sprite_reverse_mono_clr_add(); break;
									case 2: tcgd_sprite_reverse_mono_clr_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_reverse_mono_clr_rgbfilter(); break;
								}
							}
							else{
// �^�C�����]���m�����X�v���C�g�\��
								switch(disp_mod){
									case 0: tcgd_sprite_reverse_mono_clr_tile(); break;
									case 1: tcgd_sprite_reverse_mono_clr_tile_add(); break;
									case 2: tcgd_sprite_reverse_mono_clr_tile_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_reverse_mono_clr_tile_rgbfilter(); break;
								}
							}
						}
					}
				}
				else{
					if(mono == 0){
						if(reverse == 0){
							if(tile==0 || t_add==255){
// ���Ó����X�v���C�g�\��
								switch(disp_mod){
									case 0: tcgd_sprite_bright_clr(); break;
									case 1: tcgd_sprite_bright_clr_add(); break;
									case 2: tcgd_sprite_bright_clr_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_bright_clr_rgbfilter(); break;
								}
							}
							else{
// �^�C�����Ó����X�v���C�g�\��
								switch(disp_mod){
									case 0: tcgd_sprite_bright_clr_tile(); break;
									case 1: tcgd_sprite_bright_clr_tile_add(); break;
									case 2: tcgd_sprite_bright_clr_tile_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_bright_clr_tile_rgbfilter(); break;
								}
							}
						}
						else{
							if(tile==0 || t_add==255){
// ���]���Ó����X�v���C�g�\��
								switch(disp_mod){
									case 0: tcgd_sprite_reverse_bright_clr(); break;
									case 1: tcgd_sprite_reverse_bright_clr_add(); break;
									case 2: tcgd_sprite_reverse_bright_clr_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_reverse_bright_clr_rgbfilter(); break;
								}
							}
							else{
// �^�C�����]���Ó����X�v���C�g�\��
								switch(disp_mod){
									case 0: tcgd_sprite_reverse_bright_clr_tile(); break;
									case 1: tcgd_sprite_reverse_bright_clr_tile_add(); break;
									case 2: tcgd_sprite_reverse_bright_clr_tile_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_reverse_bright_clr_tile_rgbfilter(); break;
								}
							}
						}
					}
					else{
						if(reverse == 0){
							if(tile==0 || t_add==255){
// ���m���Ó����X�v���C�g�\��
								switch(disp_mod){
									case 0: tcgd_sprite_mono_bright_clr(); break;
									case 1: tcgd_sprite_mono_bright_clr_add(); break;
									case 2: tcgd_sprite_mono_bright_clr_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_mono_bright_clr_rgbfilter(); break;
								}
							}
							else{
// �^�C�����m���Ó����X�v���C�g�\��
								switch(disp_mod){
									case 0: tcgd_sprite_mono_bright_clr_tile(); break;
									case 1: tcgd_sprite_mono_bright_clr_tile_add(); break;
									case 2: tcgd_sprite_mono_bright_clr_tile_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_mono_bright_clr_tile_rgbfilter(); break;
								}
							}
						}
						else{
							if(tile==0 || t_add==255){
// ���]���m���Ó����X�v���C�g�\��
								switch(disp_mod){
									case 0: tcgd_sprite_reverse_mono_bright_clr(); break;
									case 1: tcgd_sprite_reverse_mono_bright_clr_add(); break;
									case 2: tcgd_sprite_reverse_mono_bright_clr_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_reverse_mono_bright_clr_rgbfilter(); break;
								}
							}
							else{
// �^�C�����]���m���Ó����X�v���C�g�\��
								switch(disp_mod){
									case 0: tcgd_sprite_reverse_mono_bright_clr_tile(); break;
									case 1: tcgd_sprite_reverse_mono_bright_clr_tile_add(); break;
									case 2: tcgd_sprite_reverse_mono_bright_clr_tile_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_reverse_mono_bright_clr_tile_rgbfilter(); break;
								}
							}
						}
					}
				}
			}
			else{
				if(p_r==0 && p_g==0 && p_b==0){
					if(mono == 0){
						if(reverse == 0){
							if(tile==0 || t_add==255){
// �ϐF�����X�v���C�g�\��
								switch(disp_mod){
									case 0: tcgd_sprite_change_clr(); break;
									case 1: tcgd_sprite_change_clr_add(); break;
									case 2: tcgd_sprite_change_clr_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_change_clr_rgbfilter(); break;
								}
							}
							else{
// �^�C���ϐF�����X�v���C�g�\��
								switch(disp_mod){
									case 0: tcgd_sprite_change_clr_tile(); break;
									case 1: tcgd_sprite_change_clr_tile_add(); break;
									case 2: tcgd_sprite_change_clr_tile_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_change_clr_tile_rgbfilter(); break;
								}
							}
						}
						else{
							if(tile==0 || t_add==255){
// ���]�ϐF�����X�v���C�g�\��
								switch(disp_mod){
									case 0: tcgd_sprite_reverse_change_clr(); break;
									case 1: tcgd_sprite_reverse_change_clr_add(); break;
									case 2: tcgd_sprite_reverse_change_clr_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_reverse_change_clr_rgbfilter(); break;
								}
							}
							else{
// �^�C�����]�ϐF�����X�v���C�g�\��
								switch(disp_mod){
									case 0: tcgd_sprite_reverse_change_clr_tile(); break;
									case 1: tcgd_sprite_reverse_change_clr_tile_add(); break;
									case 2: tcgd_sprite_reverse_change_clr_tile_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_reverse_change_clr_tile_rgbfilter(); break;
								}
							}
						}
					}
					else{
						if(reverse == 0){
							if(tile==0 || t_add==255){
// ���m�ϐF�����X�v���C�g�\��
								switch(disp_mod){
									case 0: tcgd_sprite_mono_change_clr(); break;
									case 1: tcgd_sprite_mono_change_clr_add(); break;
									case 2: tcgd_sprite_mono_change_clr_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_mono_change_clr_rgbfilter(); break;
								}
							}
							else{
// �^�C�����m�ϐF�����X�v���C�g�\��
								switch(disp_mod){
									case 0: tcgd_sprite_mono_change_clr_tile(); break;
									case 1: tcgd_sprite_mono_change_clr_tile_add(); break;
									case 2: tcgd_sprite_mono_change_clr_tile_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_mono_change_clr_tile_rgbfilter(); break;
								}
							}
						}
						else{
							if(tile==0 || t_add==255){
// ���]���m�ϐF�����X�v���C�g�\��
								switch(disp_mod){
									case 0: tcgd_sprite_reverse_mono_change_clr(); break;
									case 1: tcgd_sprite_reverse_mono_change_clr_add(); break;
									case 2: tcgd_sprite_reverse_mono_change_clr_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_reverse_mono_change_clr_rgbfilter(); break;
								}
							}
							else{
// �^�C�����]���m�ϐF�����X�v���C�g�\��
								switch(disp_mod){
									case 0: tcgd_sprite_reverse_mono_change_clr_tile(); break;
									case 1: tcgd_sprite_reverse_mono_change_clr_tile_add(); break;
									case 2: tcgd_sprite_reverse_mono_change_clr_tile_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_reverse_mono_change_clr_tile_rgbfilter(); break;
								}
							}
						}
					}
				}
				else{
					if(mono == 0){
						if(reverse == 0){
							if(tile==0 || t_add==255){
// �ϐF���Ó����X�v���C�g�\��
								switch(disp_mod){
									case 0: tcgd_sprite_change_bright_clr(); break;
									case 1: tcgd_sprite_change_bright_clr_add(); break;
									case 2: tcgd_sprite_change_bright_clr_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_change_bright_clr_rgbfilter(); break;
								}
							}
							else{
// �^�C���ϐF���Ó����X�v���C�g�\��
								switch(disp_mod){
									case 0: tcgd_sprite_change_bright_clr_tile(); break;
									case 1: tcgd_sprite_change_bright_clr_tile_add(); break;
									case 2: tcgd_sprite_change_bright_clr_tile_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_change_bright_clr_tile_rgbfilter(); break;
								}
							}
						}
						else{
							if(tile==0 || t_add==255){
// ���]�ϐF���Ó����X�v���C�g�\��
								switch(disp_mod){
									case 0: tcgd_sprite_reverse_change_bright_clr(); break;
									case 1: tcgd_sprite_reverse_change_bright_clr_add(); break;
									case 2: tcgd_sprite_reverse_change_bright_clr_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_reverse_change_bright_clr_rgbfilter(); break;
								}
							}
							else{
// �^�C�����]�ϐF���Ó����X�v���C�g�\��
								switch(disp_mod){
									case 0: tcgd_sprite_reverse_change_bright_clr_tile(); break;
									case 1: tcgd_sprite_reverse_change_bright_clr_tile_add(); break;
									case 2: tcgd_sprite_reverse_change_bright_clr_tile_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_reverse_change_bright_clr_tile_rgbfilter(); break;
								}
							}
						}
					}
					else{
						if(reverse == 0){
							if(tile==0 || t_add==255){
// ���m�ϐF���Ó����X�v���C�g�\��
								switch(disp_mod){
									case 0: tcgd_sprite_mono_change_bright_clr(); break;
									case 1: tcgd_sprite_mono_change_bright_clr_add(); break;
									case 2: tcgd_sprite_mono_change_bright_clr_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_mono_change_bright_clr_rgbfilter(); break;
								}
							}
							else{
// �^�C�����m�ϐF���Ó����X�v���C�g�\��
								switch(disp_mod){
									case 0: tcgd_sprite_mono_change_bright_clr_tile(); break;
									case 1: tcgd_sprite_mono_change_bright_clr_tile_add(); break;
									case 2: tcgd_sprite_mono_change_bright_clr_tile_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_mono_change_bright_clr_tile_rgbfilter(); break;
								}
							}
						}
						else{
							if(tile==0 || t_add==255){
// ���]���m�ϐF���Ó����X�v���C�g�\��
								switch(disp_mod){
									case 0: tcgd_sprite_reverse_mono_change_bright_clr(); break;
									case 1: tcgd_sprite_reverse_mono_change_bright_clr_add(); break;
									case 2: tcgd_sprite_reverse_mono_change_bright_clr_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_reverse_mono_change_bright_clr_rgbfilter(); break;
								}
							}
							else{
// �^�C�����]���m�ϐF���Ó����X�v���C�g�\��
								switch(disp_mod){
									case 0: tcgd_sprite_reverse_mono_change_bright_clr_tile(); break;
									case 1: tcgd_sprite_reverse_mono_change_bright_clr_tile_add(); break;
									case 2: tcgd_sprite_reverse_mono_change_bright_clr_tile_sub(); break;
									case 3: break;
									case 4: tcgd_sprite_reverse_mono_change_bright_clr_tile_rgbfilter(); break;
								}
							}
						}
					}
				}
			}
		}
	}
}



void true_color_grp_disp_eazy(
	UC *d_buf, int d_xl, int d_yl,
	UC *s_buf, int s_xl, int s_yl, int s_xc, int s_yc, int type,
	int x, int y,
	int ex1, int ey1, int ex2, int ey2,
	UC clr
){
	int tmp;
	int s_free_x, d_free_x, cnt_x;
	int s_free_y, d_free_y, cnt_y;

	if(s_buf==0 || d_buf==0 || clr==0){
		return;
	}

// �G���A�ݒ�
	ng_set_eria(d_xl, d_yl, &ex1, &ey1, &ex2, &ey2);
// �p�����[�^�w�b�A�x�b���C��
	x -= s_xc;
	y -= s_yc;
// �\���͈͊O�`�F�b�N�P
	if(x > ex2){return;}
	if(y > ey2){return;}
// ���W����
	if(x < 0){
		s_free_x = (-x);
		cnt_x    = s_xl - s_free_x;
		d_free_x = 0;
	}
	else{
		s_free_x = 0;
		cnt_x    = s_xl;
		d_free_x = x;
	}
	if(y < 0){
		s_free_y = (-y);
		cnt_y    = s_yl - s_free_y;
		d_free_y = 0;
	}
	else{
		s_free_y = 0;
		cnt_y    = s_yl;
		d_free_y = y;
	}
// �\���͈͊O�`�F�b�N�Q
	if(cnt_x <= 0){return;}
	if(cnt_y <= 0){return;}
// �n�_���~�b�g�C��
	if(ex1 > d_free_x){
		tmp = ex1 - d_free_x;
		s_free_x += tmp;
		cnt_x    -= tmp;
		d_free_x  = ex1;
	}
	if(ey1 > d_free_y){
		tmp = ey1 - d_free_y;
		s_free_y += tmp;
		cnt_y    -= tmp;
		d_free_y  = ey1;
	}
// �\���͈͊O�`�F�b�N�R
	if(ex2 < d_free_x){return;}
	if(ey2 < d_free_y){return;}
// �I�_���~�b�g�C��
	tmp = ex2 - d_free_x + 1;
	if(tmp < cnt_x){cnt_x = tmp;}
	tmp = ey2 - d_free_y + 1;
	if(tmp < cnt_y){cnt_y = tmp;}
// �\���͈͊O�`�F�b�N�S
	if(cnt_x <= 0){return;}
	if(cnt_y <= 0){return;}

// �A�h���X�Z�o
	s_buf += (s_free_x + (s_free_y * s_xl)) << 2;
	d_buf += (d_free_x + (d_free_y * d_xl)) << 2;

// �`��p�����[�^
	GRPF_d_buf = d_buf;
	GRPF_d_xl  = d_xl << 2;
	GRPF_d_yl  = d_yl;

	GRPF_s_buf = s_buf;
	GRPF_s_xl  = s_xl << 2;
	GRPF_s_yl  = s_yl;

	GRPF_cnt_x_org = cnt_x;
	GRPF_cnt_y     = cnt_y;

	GRPF_d_xl -= (cnt_x << 2);
	GRPF_s_xl -= (cnt_x << 2);

// �����x�p�����[�^
	GRPF_clr4 = (clr << 2);
	GRPF_clrR = 255 - clr;
	CTPCLR = *(Gclr_table_adr_p + clr);

//****************************************************************************************

	if(type == 0){
		if(clr == 255){
// �ʏ�\��
tcgd_normal();
		}
		else{
// �����ʏ�\��
tcgd_normal_clr();
		}
	}
	else{
		if(clr == 255){
// �X�v���C�g�\��
tcgd_sprite();
		}
		else{
// �����X�v���C�g�\��
tcgd_sprite_clr();
		}
	}
}

