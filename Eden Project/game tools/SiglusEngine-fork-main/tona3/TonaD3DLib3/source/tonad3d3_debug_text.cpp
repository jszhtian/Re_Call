//============================================================================================
// Name : tonad3d3_debug_text.cpp
// Spec : �f�o�b�O�p������\��
// Author : ��،����Y
//============================================================================================
#include "tonad3d3_pch.h"
#include "tonad3d3_debug_text.h"


namespace NT3
{


// �B��̃C���X�^���X
C_d3d_debug_text G_debug_text;

// �R���X�g���N�^
C_d3d_debug_text::C_d3d_debug_text()
{
	m_color = 0xffffffff;		///< �e�L�X�g�J���[
	m_array.clear();
}

// �f�X�g���N�^
C_d3d_debug_text::~C_d3d_debug_text()
{
}

// ������
bool C_d3d_debug_text::init(bool release_flag)
{
	// ���
	if(release_flag)
		release();

	return true;
}

// ���
void C_d3d_debug_text::release()
{
	m_font = NULL;

	m_array.clear();
}

// �\�z
bool C_d3d_debug_text::create(bool init_flag)
{
	// ������
	if(init_flag) {
		if(!init(true))
			return false;
	}


	HRESULT hr;
	// �t�H���g��������
	ID3DXFont* p_font = NULL;

	// �t�H���g�I�u�W�F�N�g���쐬����
	hr = D3DXCreateFont(G_d3d.device.body(),
		14,										// �_���P�ʂ̕����̍���
		FW_DONTCARE,							// �_���P�ʂ̕����̕�
		FW_BOLD,								// �^�C�v�t�F�[�X�̏d�݁B�����Ȃ�
		NULL,									// �~�b�v�}�b�v ���x���̐�
		FALSE,									// �Α̂Ȃ� TRUE�A����ȊO�� FALSE
		SHIFTJIS_CHARSET,						// �t�H���g�̕����Z�b�g
		OUT_DEFAULT_PRECIS,						// 
		DEFAULT_QUALITY,						// 
		DEFAULT_PITCH | FF_DONTCARE,			// �t�H���g�̃s�b�`�ƃt�@�~���̃C���f�b�N�X
		_T("MS �S�V�b�N"),						// �^�C�v�t�F�[�X�����܂ޕ�����
		&p_font);
	if (FAILED(hr))	{
		set_last_error_d3d(_T("D3DXCreateFont"), hr);
		return false;
	}

	// �����o�ŕێ�����
	m_font = BIP<ID3DXFont>(p_font, false);

	// �F
	m_color = D3DCOLOR_XRGB(255, 255, 255);

	// �z��f�[�^������������
	m_array.clear();

	return true;
}

// �����̐F��ύX����
// [in] r �J���[R
// [in] g �J���[G
// [in] b �J���[B
void C_d3d_debug_text::set_color(int r, int g, int b)
{
	m_color = D3DCOLOR_XRGB(r, g, b);
}

// �e�L�X�g��`�悷��
void C_d3d_debug_text::draw()
{
	RECT rect;

	for(int i = 0; i < (int)m_array.size(); i++) {
		// �`��ʒu���w�肷��(����)
		rect.top  = m_array[i].y;
		rect.left = m_array[i].x;
		rect.right = 0;
		rect.bottom = 0;
		// �����`��̈���v�Z����
		m_font->DrawText(NULL, m_array[i].str.c_str(), -1, &rect, DT_CALCRECT, NULL);
		// ������`�悷��(����)
		m_font->DrawText(NULL, m_array[i].str.c_str(), -1, &rect, DT_LEFT | DT_TOP, m_color);
	}

	m_array.clear();
}


// �ҏW������������o�b�t�@�ɒ��߂�
// [in] x X���W
// [in] y Y���W
// [in] format �ϒ�����
void C_d3d_debug_text::printf(int x, int y, TCHAR* format, ...)
{
	const int buf_size = 1024;
	TCHAR newstr[buf_size];

	// ���������������
	va_list args;
	va_start(args, format);

	_vstprintf_s(&newstr[0], buf_size, format, args);
	//_vstprintf_s
	//my_vsnprintf_s(&newstr[0], DEBUGTEXT_LENGTH, format, args);
	
	va_end(args);
	
	/*
	// �f�[�^��ۑ�(�R���X�g���N�^���镪�x���͗l)
	S_debug_text_data data;
	data.str = newstr;
	data.x = x;
	data.y = y;
	*/

	// �f�[�^��ۑ�
	S_debug_text_data data = {newstr, x, y};
	m_array.push_back(data);
}

// �R���\�[���ɕҏW�����������`�悷��
void C_d3d_debug_text::printf(TCHAR* format, ...)
{
	const int buf_size = 1024;
	TCHAR newstr[buf_size];

	// �������������������
	va_list args;
	va_start(args, format);

	_vstprintf_s(&newstr[0], buf_size, format, args);
	va_end(args);

	// �f�o�b�O�o�͂���
	OutputDebugString(newstr);
}

bool C_d3d_debug_text::on_lost_device()
{
	HRESULT hr;

	hr = m_font->OnLostDevice();
	if (FAILED(hr))	{
		set_last_error_d3d(_T("OnLostDevice"), hr);
		return false;
	}

	return true;
}

bool C_d3d_debug_text::on_reset_device()
{
	HRESULT hr;

	hr = m_font->OnResetDevice();
	if (FAILED(hr))	{
		set_last_error_d3d(_T("OnResetDevice"), hr);
		return false;
	}

	return true;
}


}