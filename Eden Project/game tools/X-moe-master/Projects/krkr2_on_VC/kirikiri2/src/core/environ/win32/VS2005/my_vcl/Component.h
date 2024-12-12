#pragma once

#include "vcl_base.h"

//----------------------------------------------------------------------------
class TComponent : public System::TObject
{
	std::vector<TComponent*> m_Components;
protected:
	void DeleteChildren();
public:
	TComponent(TComponent* owner=NULL);
	virtual ~TComponent();

	PROPERTY_ARY_R(TComponent*,Components);
	PROPERTY_R(int,ComponentCount);
	PROPERTY_VAR0(TComponent*, Owner);
	PROPERTY_VAR0(int, Tag); // �v���O���}���C�ӂɎg���Ă�������(�V�X�e���͗��p���Ȃ�)
};

//----------------------------------------------------------------------------
// VCL��TForm�̏������͓�߂���B
// �Č��͂ł��ĂȂ����A���̊֌W�̃w���p�֐�
template <class T>
static void _create_VCL_obj( T** pp, TComponent* Owner) { *pp = new T(Owner); }
