#pragma		once

#include	"tonad3d3_mesh.h"
#include	"tonad3d3_animation_controller.h"

namespace NT3 {

// D3DX���g����������D3DX�̖����K���ɏ]��

// ****************************************************************
// D3D �t���[���F�t���[���\���̂��g��
// ================================================================
//	struct D3DXFRAME
//	{
//		LPSTR                   Name;					// �t���[���̖��O
//		D3DXMATRIX              TransformationMatrix;	// �ϊ��s��
//		LPD3DXMESHCONTAINER     pMeshContainer;			// ���b�V���R���e�i
//		struct _D3DXFRAME       *pFrameSibling;			// �Z��t���[��
//		struct _D3DXFRAME       *pFrameFirstChild;		// �q���t���[��
//	};
// ================================================================
struct S_d3d_frame : public D3DXFRAME {
	D3DXMATRIX		CombinedTransformationMatrix;		// ���������ϊ��s��
														// �X�L�����b�V���ϊ����܂܂Ȃ��s��
};

// ****************************************************************
// D3D �t���[���F���b�V���R���e�i�\���̂��g��
// ================================================================
//	struct D3DXMESHCONTAINER
//	{
//		LPSTR                   Name;					// ���b�V���̖��O
//		D3DXMESHDATA            MeshData;				// ���b�V���f�[�^
//		LPD3DXMATERIAL          pMaterials;				// �}�e���A��
//		LPD3DXEFFECTINSTANCE    pEffects;				// �G�t�F�N�g
//		DWORD                   NumMaterials;			// �}�e���A����
//		DWORD                  *pAdjacency;				// �אڏ��
//		LPD3DXSKININFO          pSkinInfo;				// �X�L��
// 
//		struct _D3DXMESHCONTAINER *pNextMeshContainer;
//	};
// ================================================================
struct S_d3d_skinned_mesh_container : public D3DXMESHCONTAINER {
	// �X�L�����b�V���A�j���[�V���������Ȃ��ꍇ�� D3DXMESHCONTAINER ���g�p���鎖(����ώg�p����)
	LPD3DXMESH				pOrigMesh;					// �I���W�i���̃��b�V��

	DWORD					NumInfl;					// �P�̒��_�ɉe������{�[���̍ő吔
	DWORD					NumAttributeGroups;			// �����O���[�v��

	LPD3DXBUFFER			pBoneCombinationBuf;		// �{�[���R���r�l�[�V�����e�[�u���Ƃ����{�[���ƒ��_�̑Ή��\���z��Ƃ��ē���ϐ�

	DWORD					NumPaletteEntries;			// �X�L���A�j���u�����h�s��̍����Ɏg��
														// std::min<DWORD>(MaxMatrices, pMeshContainer->pSkinInfo->GetNumBones());
	ARRAY<D3DXMATRIX>		SkinnedBlendMatrices;		// �X�L���A�j���u�����h�s��̔z��


	// FVF���g��Ȃ��悤�ɂ���
	//LPDIRECT3DVERTEXDECLARATION9	pDecl;				// ���_�錾
};

// ****************************************************************
// D3D �t���[���K�w
// ================================================================
//		ID3DXAllocateHierarchy �͏������z�֐������N���X�ł��B
//		������p�����āA�ȉ��̂S���I�[�o�[���[�h����N���X�����K�v������܂��B
//			CreateFrame, CreateMeshContainer, DestroyFrame, DestroyMeshContainer 
// ================================================================
class C_d3d_allocate_hierarchy : public ID3DXAllocateHierarchy {
public:
	C_d3d_allocate_hierarchy()			{}
	virtual ~C_d3d_allocate_hierarchy()	{}

	virtual HRESULT __stdcall	CreateFrame(LPCSTR Name, LPD3DXFRAME* ppNewFrame);
	virtual HRESULT __stdcall	CreateMeshContainer(
		LPCSTR Name, 
		CONST D3DXMESHDATA* pMeshData, 
		CONST D3DXMATERIAL* pMaterials, 
		CONST D3DXEFFECTINSTANCE* pEffectInstances, 
		DWORD NumMaterials, 
		CONST DWORD* pAdjacency, 
		LPD3DXSKININFO pSkinInfo, 
		LPD3DXMESHCONTAINER* ppNewMeshContainer
		);

	virtual HRESULT __stdcall	DestroyFrame(LPD3DXFRAME pFrameToFree);
	virtual HRESULT __stdcall	DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerToFree); 

private:
	char*	allocate_name(const char* src);
	HRESULT	generate_skinned_mesh(S_d3d_skinned_mesh_container* p_mc);
	HRESULT destroy_skinned_mesh_container(S_d3d_skinned_mesh_container* p_mc);
};


// ****************************************************************
// �t���[��
// ================================================================
class C_d3d_frame {
public:
	bool	create_frame(C_d3d_frame* root_frame, S_d3d_frame* p_frame, ARRAY< BSP<C_d3d_mesh> >& mesh_list, ARRAY<TSTR>& texture_name_list);
	bool	update_matrix(const D3DXMATRIX* parent_matrix);

	S_d3d_frame*	body()			{	return m_frame;	}
	S_d3d_frame*	operator -> ()	{	return m_frame;	}

	ARRAY< BSP<C_d3d_mesh> >		m_mesh_list;			// ���b�V��
														// �t���[���ȉ��ɕ������b�V�����Ԃ牺����Ȃ�����ARRAY����Ȃ��ėǂ��C������
	BSP<C_d3d_frame>			m_sibling;				// �Z��t���[��
	BSP<C_d3d_frame>			m_first_child;			// �q�t���[��
	ARRAY<D3DXMATRIX>			m_offset_matrix_list;	// �I�t�Z�b�g�s��
	ARRAY<D3DXMATRIX*>			m_root_matrix_list;		// ���[�g�s��

private:
	S_d3d_frame*				m_frame;				// �I���W�i���̃t���[��
};

// ****************************************************************
// �t���[���Z�b�g
// ================================================================
class C_d3d_frame_hierarchy_parameter_setter;
class C_d3d_frame_set {
public:
	C_d3d_frame_set();
	~C_d3d_frame_set();

	void	init();		// ������
	void	release();	// ���

	bool	load_x(CTSTR& file_path, ARRAY< BSP<C_d3d_mesh> >& mesh_list, ARRAY<TSTR>& texture_name_list);
	//bool	advance_animation(double past_time);
	bool	update_matrix(const D3DXMATRIX* matrix);
	void	update_animation_controller();

	BSP<C_d3d_frame>				m_frame_root;				// ���[�g�̃t���[��
	C_d3d_allocate_hierarchy		m_allocate_hierarchy;		// �t���[���K�w
	//BIP<ID3DXAnimationController>	m_animation_controller;	// �A�j���[�V�����R���g���[��
	C_d3d_animation_controller		m_animation_controller;	// �A�j���[�V�����R���g���[��


	// �S���b�V���Ɉꊇ�ݒ�̎��Ɏg�����X�g/*�v�C��*/
	ARRAY<C_d3d_mesh*>				m_mesh_list_for_parm_set;



	// �t���[�������g���Ă��̃N���X�̃t���[���Z�b�g������t���[������������
	C_d3d_frame* search_frame_by_name(MBSTR& frame_name);
	// �t���[�������g���Ĉ����̃t���[���Z�b�g������t���[������������
	static C_d3d_frame* search_frame_by_name(MBSTR& frame_name, C_d3d_frame_set* p_frame_set);

	// �p�����[�^�̐ݒ�(�Ώۃt���[���̂�)
	static void set_parameter_to_target(C_d3d_frame* p_frame, C_d3d_frame_hierarchy_parameter_setter* p_setter);
	// �p�����[�^�̐ݒ�(�Ώۃt���[���ƑΏۃt���[���̎q���ȉ��S��)
	static void set_parameter_to_target_and_target_below_child(C_d3d_frame* p_frame, C_d3d_frame_hierarchy_parameter_setter* p_setter);
	// �p�����[�^�̐ݒ�(�Ώۃt���[���̎q���ȉ��S�āB�������Ώۃt���[��������)
	static void set_parameter_to_target_below_child(C_d3d_frame* p_frame, C_d3d_frame_hierarchy_parameter_setter* p_setter);
	// �������_�ł͑Ώۃt���[���̌Z����܂񂾉�������X�^�[�g������͎̂������Ă��Ȃ�

private:
	// �t���[�������g���ăt���[���Z�b�g������t���[������������
	static C_d3d_frame* search_frame_hierarchy_by_name(MBSTR& frame_name, C_d3d_frame* p_frame);

	// �p�����[�^�̐ݒ�
	static void set_parameter_hierarchy(C_d3d_frame* p_frame, C_d3d_frame_hierarchy_parameter_setter* p_setter);
};

// �T�|�[�g�֐�


// 
// �K�w�\����c�����₷������\����
// 

// ���b�V���ɃZ�b�g�p�̊֐���������H�֐��|�C���^�p
// C_d3d_frame::set_data(data, func)				// �������������郁�b�V���ɃZ�b�g
// C_d3d_frame::set_data_all(data, func)			// �����ȉ��̃t���[���S�Ă��������郁�b�V���ɃZ�b�g
// set_data_all()�݂����Ȋ֐��̒��ŊK�w�\����H���Ă�������������
// 10��ރf�[�^���Z�b�g����Ƃ���10��K�w�\����H��͖̂��ʂ������C������
// C_d3d_frame::set_data_all(Cdatalist dl) �݂����Ȃ̂������1��H�鎞�ɑS���Z�b�g����H
// Cdatalist�̓Z�b�g���邩������Ȃ��f�[�^�S��(���b�V���p�����[�^�Ƃ�)�ƁA�����1��1�Ή������ݒ肵�����t���O������
// 



// ****************************************************************
// D3D�t���[���K�w�p�����[�^�̎��
// ================================================================
enum FRAME_HIERARCHY_PARAMETER_TYPE {
	MP__LIGHTING_TYPE,
	MP__SHADING_TYPE,
};

typedef std::map<FRAME_HIERARCHY_PARAMETER_TYPE, int>			FRAME_HIERARCHY_PARAMETER_TYPE_INT_MAP;
typedef std::map<FRAME_HIERARCHY_PARAMETER_TYPE, bool>			FRAME_HIERARCHY_PARAMETER_TYPE_BOOL_MAP;
typedef std::map<FRAME_HIERARCHY_PARAMETER_TYPE, float>			FRAME_HIERARCHY_PARAMETER_TYPE_FLOAT_MAP;
typedef std::map<FRAME_HIERARCHY_PARAMETER_TYPE, D3DXVECTOR4>	FRAME_HIERARCHY_PARAMETER_TYPE_VECTOR4_MAP;
typedef std::map<FRAME_HIERARCHY_PARAMETER_TYPE, ARRAY<int> >	FRAME_HIERARCHY_PARAMETER_TYPE_INT_ARRAY_MAP;
//#define FRAME_HIERARCHY_PARAMETER_TYPE_INT_MAP			std::map<FRAME_HIERARCHY_PARAMETER_TYPE, int>
//#define FRAME_HIERARCHY_PARAMETER_TYPE_BOOL_MAP			std::map<FRAME_HIERARCHY_PARAMETER_TYPE, bool>
//#define FRAME_HIERARCHY_PARAMETER_TYPE_FLOAT_MAP		std::map<FRAME_HIERARCHY_PARAMETER_TYPE, float>
//#define FRAME_HIERARCHY_PARAMETER_TYPE_VECTOR4_MAP		std::map<FRAME_HIERARCHY_PARAMETER_TYPE, D3DXVECTOR4>
//#define FRAME_HIERARCHY_PARAMETER_TYPE_INT_ARRAY_MAP	std::map<FRAME_HIERARCHY_PARAMETER_TYPE, ARRAY<int>>
// ****************************************************************
// D3D�t���[���K�w�p�����[�^�ݒ�p�N���X
// ================================================================
class C_d3d_frame_hierarchy_parameter_setter {
	friend C_d3d_frame_set;
private:
	// �񋓌^��int�Ƃ��ď�������
	FRAME_HIERARCHY_PARAMETER_TYPE_INT_MAP			int_map;
	FRAME_HIERARCHY_PARAMETER_TYPE_BOOL_MAP			bool_map;
	FRAME_HIERARCHY_PARAMETER_TYPE_FLOAT_MAP		float_map;
	FRAME_HIERARCHY_PARAMETER_TYPE_VECTOR4_MAP		vector4_map;
	FRAME_HIERARCHY_PARAMETER_TYPE_INT_ARRAY_MAP	int_array_map;
public:
	// �ݒ�p(�t���[���ɑ΂���)�̃p�����[�^�����̃N���X�ɃZ�b�g����
	void set_parameter_for_set_up(FRAME_HIERARCHY_PARAMETER_TYPE type, int data);
	void set_parameter_for_set_up(FRAME_HIERARCHY_PARAMETER_TYPE type, bool data);
	void set_parameter_for_set_up(FRAME_HIERARCHY_PARAMETER_TYPE type, float data);
	void set_parameter_for_set_up(FRAME_HIERARCHY_PARAMETER_TYPE type, const D3DXVECTOR4& data);
	void set_parameter_for_set_up(FRAME_HIERARCHY_PARAMETER_TYPE type, const ARRAY<int>& data);
private:
	// ���ۂɃt���[���Ƀp�����[�^��ݒ肷��(���̃N���X�ɃZ�b�g���ꂽ���̂�)
	// C_d3d_frame_set�ȊO����͌Ăяo���Ȃ��悤�ɂ���(�j�]����悤�Ȃ�܂��l����)
	void set_up_parameter_to_frame(C_d3d_frame* p_frame);
};

// �g�����̗�
// ���[�g�t���[���ȉ����ׂĂɃp�����[�^��ݒ肷��ꍇ
// C_d3d_frame_hierarchy_parameter_setter setter;
// setter.set_parameter_for_set_up(MP__LIGHTING_TYPE, (int)LT_LAMBERT);
// setter.set_parameter_for_set_up(�Ă��Ƃ�);
// C_d3d_frame_set fs;
// fs.set_parameter_to_target_and_target_below_child(m_frame_root.get(), &setter);


}