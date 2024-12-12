#pragma once


namespace NT3
{

// �V�F�[�_�֐��C���^�[�t�F�[�X�N���X
// �����Ɍ����Ə������z�����̃N���X�ł͂Ȃ��̂ŃC���^�[�t�F�[�X�ł͂Ȃ�
class C_d3d_effect_func_interface {
protected:
	// �֐���
	std::string funcName;

	C_d3d_effect_func_interface(std::string name)
		:funcName(name)
	{
	}
public:
	virtual ~C_d3d_effect_func_interface(){}

	virtual std::string initCode() = 0;
	virtual std::string realCode() = 0;

	// �֐������擾
	std::string getFuncName() const { return funcName; }
};


}