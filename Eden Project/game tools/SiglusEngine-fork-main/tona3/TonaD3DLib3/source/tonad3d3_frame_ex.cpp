#include	"tonad3d3_pch.h"
#include	"tonad3d3_frame_ex.h"

namespace NT3
{

#define		SAFE_DELETE(p)			do	{	if (p)	{	delete (p);		(p) = NULL;	}	} while(0)
#define		SAFE_DELETE_ARRAY(p)	do	{	if (p)	{	delete[] (p);	(p) = NULL;	}	} while(0)
#define		SAFE_RELEASE(p)			do	{	if (p)	{	(p)->Release(); (p) = NULL;	}	} while(0)

int get_size_from_d3ddecltype(BYTE type)
{
	int size = 0;
	switch(type) {
		case D3DDECLTYPE_FLOAT1		:	size = 4;	break;
		case D3DDECLTYPE_FLOAT2		:	size = 8;	break;  
		case D3DDECLTYPE_FLOAT3		:	size = 12;	break;
		case D3DDECLTYPE_FLOAT4		:	size = 16;	break;
		case D3DDECLTYPE_D3DCOLOR	:	size = 16;	break;
		case D3DDECLTYPE_UBYTE4		:	size = 4;	break;
		case D3DDECLTYPE_SHORT2		:	size = 2;	break;
		case D3DDECLTYPE_SHORT4		:	size = 4;	break;
		default						:	size = 0;	break;

	// The following types are valid only with vertex shaders >= 2.0
	//D3DDECLTYPE_UBYTE4N   =  8,  // Each of 4 bytes is normalized by dividing to 255.0
	//D3DDECLTYPE_SHORT2N   =  9,  // 2D signed short normalized (v[0]/32767.0,v[1]/32767.0,0,1)
	//D3DDECLTYPE_SHORT4N   = 10,  // 4D signed short normalized (v[0]/32767.0,v[1]/32767.0,v[2]/32767.0,v[3]/32767.0)
	//D3DDECLTYPE_USHORT2N  = 11,  // 2D unsigned short normalized (v[0]/65535.0,v[1]/65535.0,0,1)
	//D3DDECLTYPE_USHORT4N  = 12,  // 4D unsigned short normalized (v[0]/65535.0,v[1]/65535.0,v[2]/65535.0,v[3]/65535.0)
	//D3DDECLTYPE_UDEC3     = 13,  // 3D unsigned 10 10 10 format expanded to (value, value, value, 1)
	//D3DDECLTYPE_DEC3N     = 14,  // 3D signed 10 10 10 format normalized and expanded to (v[0]/511.0, v[1]/511.0, v[2]/511.0, 1)
	//D3DDECLTYPE_FLOAT16_2 = 15,  // Two 16-bit floating point values, expanded to (value, value, 0, 1)
	//D3DDECLTYPE_FLOAT16_4 = 16,  // Four 16-bit floating point values
	//D3DDECLTYPE_UNUSED    = 17,  // When the type field in a decl is unused.
	}

	return size;
}

// ********************************************************************************************************************************
// D3D �t���[���K�w
// ================================================================================================================================

// ****************************************************************
// D3D �t���[���K�w�F�t���[�����\�z����
// ================================================================
HRESULT C_d3d_allocate_hierarchy::CreateFrame(LPCSTR Name, LPD3DXFRAME *ppNewFrame)
{
	// �t���[�����쐬
	S_d3d_frame* p_frame = new S_d3d_frame;
	// �S��0�ŏ���������
	ZeroMemory(p_frame, sizeof(S_d3d_frame));

	// ���O���R�s�[����(�֐�������new���Ă�)
	p_frame->Name = allocate_name(Name);

	// �s�񏉊���
    D3DXMatrixIdentity(&p_frame->TransformationMatrix);			// �ϊ��s��
    D3DXMatrixIdentity(&p_frame->CombinedTransformationMatrix);	// �����ς݂̕ϊ��s��

	// �V�����t���[����߂�l�ɐݒ肷��
	*ppNewFrame = (D3DXFRAME *)p_frame;

	return D3D_OK;
}

// ****************************************************************
// D3D �t���[���K�w�F�t���[�����������
// ================================================================
HRESULT C_d3d_allocate_hierarchy::DestroyFrame(LPD3DXFRAME pFrameToFree)
{
	//SAFE_DELETE(pFrameToFree->pMeshContainer);	// ���b�V���R���e�i
	//SAFE_DELETE_ARRAY(pFrameToFree->Name);		// ���O

	return D3D_OK;
}

// ****************************************************************
// D3D �t���[���K�w�F���b�V���R���e�i���\�z����
// ================================================================
HRESULT C_d3d_allocate_hierarchy::CreateMeshContainer(
		LPCSTR Name, 
		CONST D3DXMESHDATA *pMeshData, 
		CONST D3DXMATERIAL *pMaterials, 
		CONST D3DXEFFECTINSTANCE *pEffectInstances, 
		DWORD NumMaterials, 
		CONST DWORD *pAdjacency, 
		LPD3DXSKININFO pSkinInfo, 
		LPD3DXMESHCONTAINER *ppNewMeshContainer
		)
{
	HRESULT hr;

	// ������₷�����b�V���f�[�^�ւ̃|�C���^���擾���Ă���
	ID3DXMesh* p_mesh = pMeshData->pMesh;

	// ���ʏ�̃��b�V���ȊO�͖��Ή�
	if (pMeshData->Type != D3DXMESHTYPE_MESH)	{
		set_last_error(_T("�v���O���b�V�u���b�V����p�b�`���b�V���ɂ͑Ή����Ă��܂���B"), _T("CreateMeshContainer"));
		return E_FAIL;
	}

	// ��FVF ���ݒ肳��Ă��Ȃ����b�V���͖��Ή�
	if (p_mesh->GetFVF() == 0)	{
		set_last_error(_T("FVF ���ݒ肳��Ă��Ȃ����b�V���͖��Ή��ł��B"), _T("CreateMeshContainer"));
		return E_FAIL;
	}

	// �V�������b�V���R���e�i�����
	/*
	D3DXMESHCONTAINER* p_mc = NULL;
	// �X�L�����b�V���̏ꍇ�̏���
	if(pSkinInfo) {
		p_mc = new S_d3d_skinned_mesh_container;
		ZeroMemory(p_mc, sizeof(S_d3d_skinned_mesh_container));
	}
	// �X�L�����b�V���ȊO�̏ꍇ�̏���
	else {
		p_mc = new D3DXMESHCONTAINER;
		ZeroMemory(p_mc, sizeof(D3DXMESHCONTAINER));
	}
	*/
	S_d3d_skinned_mesh_container* p_mc = NULL;
	p_mc = new S_d3d_skinned_mesh_container;
	ZeroMemory(p_mc, sizeof(S_d3d_skinned_mesh_container));

	// ���O���R�s�[
	p_mc->Name = allocate_name(Name);



	// ���b�V�������擾����(FVF���g��Ȃ���)
	D3DVERTEXELEMENT9 pDecl[MAX_FVF_DECL_SIZE];
	LPD3DVERTEXELEMENT9 pDeclCur;
	hr = p_mesh->GetDeclaration(pDecl);
	if (FAILED(hr))	{
		set_last_error_d3d(_T("GetDeclaration �Ɏ��s���܂����B"), _T("GetDeclaration"), hr);
		return hr;
	}
	pDeclCur = pDecl;
	// ���b�V���������ۂɒ��ׂ�
	bool have_normal = false;		// �@���x�N�g�����
	bool have_tangent = false;		// �ڃx�N�g�����
									// �ڃx�N�g���̓e�N�X�`��������ꂽ���f���Ɋւ���@���x�N�g�����v�Z����
									// �܂�A�e�N�X�`���𒣂�Ȃ����f���ł͓����Ȃ�
	bool have_binormal = false;		// �]�@���x�N�g�����
	bool have_texcoord = false;
	while (pDeclCur->Stream != 0xff) {
		// �@���x�N�g�����̗L��
		if ((pDeclCur->Usage == D3DDECLUSAGE_NORMAL) && (pDeclCur->UsageIndex == 0))
			have_normal = true;
		// �ڃx�N�g�����̗L��
		if ((pDeclCur->Usage == D3DDECLUSAGE_TANGENT) && (pDeclCur->UsageIndex == 0))
			have_tangent = true;
		// �]�@���x�N�g�����̗L��
		if ((pDeclCur->Usage == D3DDECLUSAGE_BINORMAL) && (pDeclCur->UsageIndex == 0))
			have_binormal = true;
		// �̗L��
		if ((pDeclCur->Usage == D3DDECLUSAGE_TEXCOORD) && (pDeclCur->UsageIndex == 0))
			have_texcoord = true;

		// �C���f�b�N�X��i�߂�
		pDeclCur++;
	}
	// �����ꂩ�̏�񂪌����Ă���Βǉ�����
	if(!have_normal || !have_tangent || !have_binormal || !have_texcoord) {
		p_mc->MeshData.Type = D3DXMESHTYPE_MESH;

		// �V�������_�t�H�[�}�b�g
		//D3DVERTEXELEMENT9 new_decl[MAX_FVF_DECL_SIZE];
		ARRAY<D3DVERTEXELEMENT9> new_decl;
		// �܂��͊����̏���ǉ�����
		pDeclCur = pDecl;
		WORD last_offset;
		BYTE last_type;
		while (pDeclCur->Stream != 0xff) {
			new_decl.push_back(*pDeclCur);
			last_offset = pDeclCur->Offset;
			last_type = pDeclCur->Type;
			// �C���f�b�N�X��i�߂�
			pDeclCur++;
		}
		// ���_�t�H�[�}�b�g�̒ǉ�
		if(!have_normal) {
			D3DVERTEXELEMENT9 add_decl;
			//add_decl = D3DVERTEXELEMENT9(0, last_offset + get_size_from_d3ddecltype(last_type), D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0);
			add_decl.Stream = 0;
			add_decl.Offset = last_offset + get_size_from_d3ddecltype(last_type);
			add_decl.Type = D3DDECLTYPE_FLOAT3;
			add_decl.Method = D3DDECLMETHOD_DEFAULT;
			add_decl.Usage = D3DDECLUSAGE_NORMAL;
			add_decl.UsageIndex = 0;
			last_offset = last_offset + get_size_from_d3ddecltype(last_type);
			last_type = D3DDECLTYPE_FLOAT3;
			new_decl.push_back(add_decl);
		}
		if(!have_tangent) {
			D3DVERTEXELEMENT9 add_decl;
			//add_decl = D3DVERTEXELEMENT9(0, last_offset + get_size_from_d3ddecltype(last_type), D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0);
			add_decl.Stream = 0;
			add_decl.Offset = last_offset + get_size_from_d3ddecltype(last_type);
			add_decl.Type = D3DDECLTYPE_FLOAT3;
			add_decl.Method = D3DDECLMETHOD_DEFAULT;
			add_decl.Usage = D3DDECLUSAGE_TANGENT;
			add_decl.UsageIndex = 0;
			last_offset = last_offset + get_size_from_d3ddecltype(last_type);
			last_type = D3DDECLTYPE_FLOAT3;
			new_decl.push_back(add_decl);
		}
		if(!have_binormal) {
			D3DVERTEXELEMENT9 add_decl;
			//add_decl = D3DVERTEXELEMENT9(0, last_offset + get_size_from_d3ddecltype(last_type), D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0);
			add_decl.Stream = 0;
			add_decl.Offset = last_offset + get_size_from_d3ddecltype(last_type);
			add_decl.Type = D3DDECLTYPE_FLOAT3;
			add_decl.Method = D3DDECLMETHOD_DEFAULT;
			add_decl.Usage = D3DDECLUSAGE_BINORMAL;
			add_decl.UsageIndex = 0;
			last_offset = last_offset + get_size_from_d3ddecltype(last_type);
			last_type = D3DDECLTYPE_FLOAT3;
			new_decl.push_back(add_decl);
		}
		if(!have_texcoord) {
			D3DVERTEXELEMENT9 add_decl;
			//add_decl = D3DVERTEXELEMENT9(0, last_offset + get_size_from_d3ddecltype(last_type), D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0);
			add_decl.Stream = 0;
			add_decl.Offset = last_offset + get_size_from_d3ddecltype(last_type);
			add_decl.Type = D3DDECLTYPE_FLOAT2;
			add_decl.Method = D3DDECLMETHOD_DEFAULT;
			add_decl.Usage = D3DDECLUSAGE_TEXCOORD;
			add_decl.UsageIndex = 0;
			last_offset = last_offset + get_size_from_d3ddecltype(last_type);
			last_type = D3DDECLTYPE_FLOAT2;
			new_decl.push_back(add_decl);
		}
		// #define D3DDECL_END() {0xFF,0,D3DDECLTYPE_UNUSED,0,0,0}
		D3DVERTEXELEMENT9 add_decl;
		//add_decl = D3DVERTEXELEMENT9(0, last_offset + get_size_from_d3ddecltype(last_type), D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0);
		add_decl.Stream = 0xFF;
		add_decl.Offset = 0;
		add_decl.Type = D3DDECLTYPE_UNUSED;
		add_decl.Method = 0;
		add_decl.Usage = 0;
		add_decl.UsageIndex = 0;
		new_decl.push_back(add_decl);

		// �V�������_�t�H�[�}�b�g�̃��b�V���𐶐�����
		hr = p_mesh->CloneMesh(p_mesh->GetOptions(), new_decl.get(), G_d3d.device.body(), &p_mc->MeshData.pMesh);
		if (FAILED(hr))	{
			set_last_error_d3d(_T("���b�V���̕����Ɏ��s���܂����B"), _T("CloneMesh"), hr);
			return E_FAIL;
		}

		// ���b�V���ւ̃|�C���^���Ď擾
		p_mesh = p_mc->MeshData.pMesh;

		// �@���x�N�g�����Z�o����
		hr = D3DXComputeNormals(p_mesh, NULL);
		if(FAILED(hr)) {
			set_last_error_d3d(_T("D3DXComputeNormals"), hr);
			return E_FAIL;
		}
		// �ڃx�N�g���Ə]�@���x�N�g�����Z�o����
		hr = D3DXComputeTangent(p_mesh, 0, 0, 0, TRUE, NULL);
		if(FAILED(hr)) {
			set_last_error_d3d(_T("D3DXComputeTangent"), hr);
			return E_FAIL;
		}
	}
	// �S�Ă̏�񂪂���΂��̂܂܃R�s�[����
	else {
		// ���b�V���i�ւ̃|�C���^�j���R�s�[����
		p_mc->MeshData.Type = D3DXMESHTYPE_MESH;
		p_mc->MeshData.pMesh = p_mesh;
		p_mesh->AddRef();	// �Q�ƃJ�E���^�𑝂₷�I
	}
	//// ���_�f�N�����[�V�����̐���
	//D3DVERTEXELEMENT9 pFinDecl[MAX_FVF_DECL_SIZE];
	//hr = p_mesh->GetDeclaration(pFinDecl);
	//if (FAILED(hr))	{
	//	set_last_error_d3d(_T("GetDeclaration �Ɏ��s���܂����B"), _T("GetDeclaration"), hr);
	//	return hr;
	//}
	//G_d3d.device.body()->CreateVertexDeclaration(pFinDecl, &p_mc->pDecl);
	//if (FAILED(hr))	{
	//	set_last_error_d3d(_T("CreateVertexDeclaration �Ɏ��s���܂����B"), _T("CreateVertexDeclaration"), hr);
	//	return hr;
	//}

	//// ���b�V�������X�V�����f������
	//hr = p_mesh->UpdateSemantics(pDecl);
	//if (FAILED(hr))	{
	//	set_last_error_d3d(_T("UpdateSemantics �Ɏ��s���܂����B"), _T("UpdateSemantics"), hr);
	//	return hr;
	//}



	//// ���b�V���f�[�^
	//// �@��������ꍇ�͂��̂܂܃R�s�[����
	//if (p_mesh->GetFVF() & D3DFVF_NORMAL)	{
	//	// �ڃx�N�g�����Z�o����(��Ƀo���v�}�b�v�p)
	//	//hr = D3DXComputeTangent(p_mesh, 0, 0, 0, TRUE, NULL);
	//	//if(FAILED(hr)) { 
	//	//	set_last_error_d3d(_T("D3DXComputeTangent"), hr);
	//	//}

	//	// ���b�V���i�ւ̃|�C���^�j���R�s�[����
	//	p_mc->MeshData.Type = D3DXMESHTYPE_MESH;
	//	p_mc->MeshData.pMesh = p_mesh;
	//	p_mesh->AddRef();	// �Q�ƃJ�E���^�𑝂₷�I
	//}
	//// �@�����Ȃ��ꍇ�͖@�����v�Z����
	//else	{
	//	p_mc->MeshData.Type = D3DXMESHTYPE_MESH;

	//	// ���b�V���ɖ@�����Ȃ������̂Œǉ�����
	//	hr = p_mesh->CloneMeshFVF(p_mesh->GetOptions(), p_mesh->GetFVF() | D3DFVF_NORMAL, G_d3d.device.body(), &p_mc->MeshData.pMesh);
	//	if (FAILED(hr))	{
	//		set_last_error_d3d(_T("���b�V���̕����Ɏ��s���܂����B"), _T("CloneMeshFVF"), hr);
	//		return E_FAIL;
	//	}

	//	// ���b�V���ւ̃|�C���^���Ď擾
	//	p_mesh = p_mc->MeshData.pMesh;

	//	// ���ۂɖ@�����v�Z����
	//	D3DXComputeNormals(p_mesh, NULL);

	//	// �ڃx�N�g�����Z�o����(��Ƀo���v�}�b�v�p)
	//	//hr = D3DXComputeTangent(p_mesh, 0, 0, 0, TRUE, NULL);
	//	//if(FAILED(hr)) { 
	//	//	set_last_error_d3d(_T("D3DXComputeTangent"), hr);
	//	//}
	//}


	// �ʂ̐����擾���Ă���
	int face_cnt = p_mesh->GetNumFaces();

	// �}�e���A�����̐ݒ�(�Œ�1�͊m�ۂ���)
	p_mc->NumMaterials = std::max<DWORD>(1, NumMaterials);
	// �}�e���A���z��̊m��
	p_mc->pMaterials = new D3DXMATERIAL[p_mc->NumMaterials];

	// �אڐ��o�b�t�@�̊m��
	p_mc->pAdjacency = new DWORD[face_cnt * 3];
	// �אڐ��o�b�t�@���R�s�[����
	memcpy(p_mc->pAdjacency, pAdjacency, sizeof(DWORD) * face_cnt * 3);

	// �}�e���A���������
	if(NumMaterials > 0) {
		// �}�e���A���z����R�s�[����
		memcpy(p_mc->pMaterials, pMaterials, sizeof(D3DXMATERIAL) * NumMaterials);

		for(int i = 0; i < (int)NumMaterials; i++) {
			if(pMaterials[i].pTextureFilename) {
				// �e�N�X�`�������� C_d3d_frame::create_frame �Ƃ��Ŗ��O����s���̂Ŗ��O���R�s�[���Ă���
				p_mc->pMaterials[i].pTextureFilename = allocate_name(pMaterials[i].pTextureFilename);
			}
			// �w�t�@�C���ł̓A���r�G���g�͒�`�ł��Ȃ��̂ŁA�f�B�t���[�Y���R�s�[���Ă���/*�v�C��*/
			p_mc->pMaterials[i].MatD3D.Ambient = p_mc->pMaterials[i].MatD3D.Diffuse;
		}
	}
	// �}�e���A�����Ȃ����
	else {
		// �K���Ȓl������

	}


	// �G�t�F�N�g
	/*
	p_mc->pEffects = new D3DXEFFECTINSTANCE;
	ZeroMemory(p_mc->pEffects, sizeof(D3DXEFFECTINSTANCE));
	if (pEffectInstances->pEffectFilename)	{
		dst->pEffects->pEffectFilename = new char[strlen(pEffectInstances->pEffectFilename) + 1];
		strcpy_s(dst->pEffects->pEffectFilename, strlen(pEffectInstances->pEffectFilename) + 1, pEffectInstances->pEffectFilename);
	}
	dst->pEffects->NumDefaults = pEffectInstances->NumDefaults;
	dst->pEffects->pDefaults = new D3DXEFFECTDEFAULT[pEffectInstances->NumDefaults];
	for (int i = 0; i < (int)pEffectInstances->NumDefaults; i++)	{
		D3DXEFFECTDEFAULT* dst_default = &dst->pEffects->pDefaults[i];
		D3DXEFFECTDEFAULT* src_default = &pEffectInstances->pDefaults[i];

		dst_default->pParamName = new char[strlen(src_default->pParamName) + 1];
		strcpy_s(dst_default->pParamName, strlen(src_default->pParamName) + 1, src_default->pParamName);
		dst_default->NumBytes = src_default->NumBytes;
		dst_default->Type = src_default->Type;
		if (src_default->Type <= D3DXEDT_DWORD)	{
			dst_default->pValue = new BYTE[src_default->NumBytes];
			memcpy(dst_default->pValue, src_default->pValue, src_default->NumBytes);
		}
	}
	*/

	// ���̃��b�V���R���e�i(�ǂ̃T���v�������Ă�DrawFrame�Ŏg���Ă���̂�(NULL�`�F�b�N�ł͂������)�������ł͉������Ă��Ȃ�)
	// ZeroMemory���Ă邯�ǃR�����g���c�����߂�NULL������Ă���
	p_mc->pNextMeshContainer = NULL;

	// �X�L��
	if (pSkinInfo)	{

		// �X�L���i�ւ̃|�C���^�j���R�s�[
		p_mc->pSkinInfo = pSkinInfo;
		pSkinInfo->AddRef();			// �Q�ƃJ�E���^�𑝂₷�I

		/*
		// �_�E���L���X�g���s��(C�L���X�g���g���Ă���̂̓R���p�C�����ʂ�Ȃ���������)
		// class����Ȃ��������R���p�C���I�v�V�����Ȃ̂��͕s��(���̂����e�X�g����)
		S_d3d_skinned_mesh_container* p_smc = (S_d3d_skinned_mesh_container*)p_mc;

		// ���̃��b�V���i�ւ̃|�C���^�j���R�s�[
		p_smc->pOrigMesh = p_mesh;
		p_mesh->AddRef();				// �Q�ƃJ�E���^�𑝂₷�I

		// �X�L�����b�V������ʉ�����H
		hr = generate_skinned_mesh(p_smc);
		*/

		// ���̃��b�V���i�ւ̃|�C���^�j���R�s�[
		p_mc->pOrigMesh = p_mesh;
		p_mesh->AddRef();				// �Q�ƃJ�E���^�𑝂₷�I
		// �X�L�����b�V������ʉ�����H
		hr = generate_skinned_mesh(p_mc);

		if(FAILED(hr)) {
			return hr;
		}
	}

	// ���[�J���ɐ����������b�V���R���e�i��߂�l�ɐݒ肷��
	*ppNewMeshContainer = p_mc;

	return D3D_OK;
}


// ****************************************************************
// ���b�V���F�t���[���K�w
// ================================================================
HRESULT C_d3d_allocate_hierarchy::generate_skinned_mesh(S_d3d_skinned_mesh_container *p_mc)
{
	HRESULT hr = S_OK;

	// �X�L�����Ȃ��ꍇ�������Ȃ�
	if (!p_mc->pSkinInfo)
		return E_FAIL;	//return S_OK;

	// ���b�V���ƃ{�[���R���r�l�[�V�����o�b�t�@�͕K�v�Ȃ��̂ŉ������(����Ƃ������������H����ĂȂ����ۂ�)
	SAFE_RELEASE(p_mc->MeshData.pMesh);
	SAFE_RELEASE(p_mc->pBoneCombinationBuf);


	// �s��p���b�g �X�L�j���O�Ɏg�p�\�ȃ{�[���s��̐������߂�
	UINT MaxMatrices = 26; 
	DWORD NumBones = p_mc->pSkinInfo->GetNumBones();
	p_mc->NumPaletteEntries = std::min<DWORD>(MaxMatrices, NumBones);

	// ���b�V�����󂯎��A���_�P�ʂ̃u�����h�̏d�݂ƃC���f�b�N�X�ƃ{�[���̑g�ݍ��킹�e�[�u����
	// �K�p�����V�������b�V����Ԃ��܂��B���̃e�[�u���́A�{�[�� �p���b�g�ƃ��b�V���̃T�u�Z�b�g�̊֌W��\���܂��B
	hr = p_mc->pSkinInfo->ConvertToIndexedBlendedMesh(
							p_mc->pOrigMesh,							// ���̓��b�V��
							D3DXMESH_MANAGED | D3DXMESHOPT_VERTEXCACHE, // ���݂͎g���Ă��܂���
							p_mc->NumPaletteEntries,					// �s��p���b�g �X�L�j���O�Ɏg�p�\�ȃ{�[���s��̐�
							p_mc->pAdjacency,							// ���̓��b�V���̗אڐ����
							NULL,										// �o�̓��b�V���̗אڐ����
							NULL,										// �e�ʂ̐V�����C���f�b�N�X���i�[����]����o�b�t�@�ւ̃|�C���^�BNULL �ł����܂��܂���B
							NULL,										// �e���_�̐V�����C���f�b�N�X���i�[����AID3DXBuffer �C���^�[�t�F�C�X�ւ̃|�C���^�̃A�h���X�BNULL �ł����܂��܂���B
							&p_mc->NumInfl,								// 1�̒��_�ɉe������{�[���̍ő吔
							&p_mc->NumAttributeGroups,					// �{�[���R���r�l�[�V�����̐�
							&p_mc->pBoneCombinationBuf,					// �{�[���R���r�l�[�V�����e�[�u���Ƃ����{�[���ƒ��_�̑Ή��\�̔z��
							&p_mc->MeshData.pMesh						// �X�L�����b�V���p�ɍœK�������o�̓��b�V��
						   );
	if (FAILED(hr))	{
		set_last_error_d3d(_T("ConvertToIndexedBlendedMesh �Ɏ��s���܂����B"), _T("ConvertToIndexedBlendedMesh"), hr);
		return hr;
	}



	//// FVF has to match our declarator. Vertex shaders are not as forgiving as FF pipeline
	//// D3DFVF_TEX1 ���K�{�Ȃ̂͂�����������/*�v�C��*/
	//DWORD NewFVF = (p_mc->MeshData.pMesh->GetFVF() & D3DFVF_POSITION_MASK) | D3DFVF_NORMAL | D3DFVF_TEX1 | D3DFVF_LASTBETA_UBYTE4;
	//if (NewFVF != p_mc->MeshData.pMesh->GetFVF()) {
	//	LPD3DXMESH temp_mesh = NULL;
	//	hr = p_mc->MeshData.pMesh->CloneMeshFVF(p_mc->MeshData.pMesh->GetOptions(), NewFVF, G_d3d.device.body(), &temp_mesh);
	//	if(FAILED(hr)) {
	//		set_last_error_d3d(_T("CloneMeshFVF �Ɏ��s���܂����B"), _T("CloneMeshFVF"), hr);
	//		return hr;
	//	}
	//	else {
	//		p_mc->MeshData.pMesh->Release();
	//		p_mc->MeshData.pMesh = temp_mesh;
	//		temp_mesh = NULL;
	//	}
	//}
	//DWORD fvf = p_mc->MeshData.pMesh->GetFVF();

	

	// Geforce3�����UBYTE4�ɑΉ����Ă���ۂ�(D3DCOLOR�ւ̕ϊ��s�v)�̂ŉ��̃\�[�X�̓R�����g�A�E�g���Ă���
	/*
    // Interpret the UBYTE4 as a D3DCOLOR.
    // The GeForce3 doesn't support the UBYTE4 decl type.  So, we convert any
    // blend indices to a D3DCOLOR semantic, and later in the shader convert
    // it back using the D3DCOLORtoUBYTE4() intrinsic.  Note that we don't
    // convert any data, just the declaration.
	D3DVERTEXELEMENT9 pDecl[MAX_FVF_DECL_SIZE];
	LPD3DVERTEXELEMENT9 pDeclCur;
	hr = p_mc->MeshData.pMesh->GetDeclaration(pDecl);
	if (FAILED(hr))	{
		set_last_error_d3d(_T("GetDeclaration �Ɏ��s���܂����B"), _T("GetDeclaration"), hr);
		return hr;
	}

	// the vertex shader is expecting to interpret the UBYTE4 as a D3DCOLOR, so update the type 
	//   NOTE: this cannot be done with CloneMesh, that would convert the UBYTE4 data to float and then to D3DCOLOR
	//          this is more of a "cast" operation
	pDeclCur = pDecl;
	while (pDeclCur->Stream != 0xff) {
		if ((pDeclCur->Usage == D3DDECLUSAGE_BLENDINDICES) && (pDeclCur->UsageIndex == 0))
			pDeclCur->Type = D3DDECLTYPE_D3DCOLOR;
		pDeclCur++;
	}

	hr = p_mc->MeshData.pMesh->UpdateSemantics(pDecl);
	if (FAILED(hr))	{
		set_last_error_d3d(_T("UpdateSemantics �Ɏ��s���܂����B"), _T("UpdateSemantics"), hr);
		return hr;
	}
	*/

	// allocate a buffer for bone matrices, but only if another mesh has not allocated one of the same size or larger
	if( NumBones > 0 ) {
		// �s�񃊃X�g���m�ۂ���
		p_mc->SkinnedBlendMatrices.reserve(NumBones);
		p_mc->SkinnedBlendMatrices.resize(NumBones);
	}


	/*
	// If the device can only do 2 matrix blends, ConvertToBlendedMesh cannot approximate all meshes to it
	// Thus we split the mesh in two parts: The part that uses at most 2 matrices and the rest. The first is
	// drawn using the device's HW vertex processing and the rest is drawn using SW vertex processing.
	D3DXBONECOMBINATION* bone_combination = (D3DXBONECOMBINATION*)p_mc->pBoneCombinationBuf->GetBufferPointer();

	// �P�̒��_�ɉe������{�[���̍ő吔�𒲂ׁA
	// �f�o�C�X�̔\�͂𒴂��Ă��Ȃ����ǂ����𔻒肷��
	int group_no = 0;
	for (group_no = 0; group_no < (int)p_mc->NumAttributeGroups; group_no ++)	{

		// �e������{�[���̐��𒲂ׂ�
		DWORD infl_cnt = 0;
		for (DWORD infl_no = 0; infl_no < p_mc->NumInfl; infl_no++)	{
			if (bone_combination[group_no].BoneId[infl_no] != UINT_MAX)
				++infl_cnt;
		}

		// �f�o�C�X�̔\�͂𒴂����{�[������������ΏI��
		//if (infl_cnt > G_d3d.device.device_caps().MaxVertexBlendMatrices)
		//	break;
	}

	// �f�o�C�X�̔\�͂𒴂����{�[���������������ꍇ�A
	// ���b�V�����\�t�g�E�F�A�`��ɂ���
	if (group_no < (int)p_mc->NumAttributeGroups)	{
		ID3DXMesh* p_mesh_tmp = NULL;
		hr = p_mc->MeshData.pMesh->CloneMeshFVF(D3DXMESH_SOFTWAREPROCESSING | p_mc->MeshData.pMesh->GetOptions(), p_mc->MeshData.pMesh->GetFVF(), G_d3d.device.body(), &p_mesh_tmp);
		if (FAILED(hr))	{
			set_last_error_d3d(_T("CloneMeshFVF �Ɏ��s���܂����B"), _T("CloneMeshFVF"), hr);
			return hr;
		}

		// �V�������b�V���ɓ���ւ���
		p_mc->MeshData.pMesh->Release();
		p_mc->MeshData.pMesh = p_mesh_tmp;
		p_mesh_tmp = NULL;
	}
	*/

	return S_OK;
}

// ****************************************************************
// D3D �t���[���K�w�F���b�V���R���e�i���������
// ================================================================
HRESULT C_d3d_allocate_hierarchy::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerToFree)
{
	// �X�L�����b�V���̏ꍇ
	if (pMeshContainerToFree->pSkinInfo) {
		destroy_skinned_mesh_container((S_d3d_skinned_mesh_container*)pMeshContainerToFree);
	}

	// ���O
	SAFE_DELETE_ARRAY(pMeshContainerToFree->Name);

	// ���_�f�N�����[�V����
	//SAFE_RELEASE(((S_d3d_skinned_mesh_container*)pMeshContainerToFree)->pDecl);

	// ���b�V��
	SAFE_RELEASE(pMeshContainerToFree->MeshData.pMesh);
	SAFE_RELEASE(pMeshContainerToFree->MeshData.pPMesh);
	SAFE_RELEASE(pMeshContainerToFree->MeshData.pPatchMesh);

	// �}�e���A��
	for (int i = 0; i < (int)pMeshContainerToFree->NumMaterials; i++) {
		SAFE_DELETE_ARRAY(pMeshContainerToFree->pMaterials[i].pTextureFilename);
	}
	SAFE_DELETE_ARRAY(pMeshContainerToFree->pMaterials);
	SAFE_DELETE_ARRAY(pMeshContainerToFree->pAdjacency);

	// �G�t�F�N�g
	/*
	for (int i = 0; i < (int)pMeshContainerToFree->pEffects->NumDefaults; i++)	{
		SAFE_RELEASE_ARRAY(pMeshContainerToFree->pEffects->pDefaults[i].pParamName);
		SAFE_RELEASE_ARRAY(pMeshContainerToFree->pEffects->pDefaults[i].pValue);
	}
	SAFE_RELEASE_ARRAY(pMeshContainerToFree->pEffects->pDefaults);
	SAFE_RELEASE_ARRAY(pMeshContainerToFree->pEffects->pEffectFilename);
	SAFE_RELEASE(pMeshContainerToFree->pEffects);
	*/

	// ���\�[�X�ɂ͂Ȃ�����l�q��
	//SAFE_DELETE(pMeshContainerToFree);
	
	return D3D_OK;
}

// ****************************************************************
// D3D �t���[���K�w�F�X�L�����b�V���R���e�i�p�̃f�[�^�݂̂��������
// ================================================================
HRESULT C_d3d_allocate_hierarchy::destroy_skinned_mesh_container(S_d3d_skinned_mesh_container* p_mc)
{
	SAFE_RELEASE(p_mc->pOrigMesh);
	SAFE_RELEASE(p_mc->pBoneCombinationBuf);
	p_mc->SkinnedBlendMatrices.clear();

	SAFE_RELEASE(p_mc->pSkinInfo);

	return D3D_OK;
}

// ****************************************************************
// D3D �t���[���K�w�F���O�p�̗̈���m��
// ================================================================
char* C_d3d_allocate_hierarchy::allocate_name(const char* src)
{
	if (!src)
		return NULL;

	int len = (int)strlen(src);

	char* dst = new char[len + 1];
	strcpy_s(dst, len + 1, src);
	return dst;
}






// ********************************************************************************************************************************
// �t���[��
// ================================================================================================================================

// ****************************************************************
// �t���[�����쐬����
// ================================================================
bool C_d3d_frame::create_frame(C_d3d_frame* root_frame, S_d3d_frame* p_frame, ARRAY< BSP<C_d3d_mesh> >& mesh_list_ret, ARRAY<TSTR>& texture_name_list_ret)
{
	// �t���[���ւ̃|�C���^���L�����Ă���
	m_frame = p_frame;

	// ���b�V��������ꍇ�I
	S_d3d_skinned_mesh_container* p_mc = (S_d3d_skinned_mesh_container*)p_frame->pMeshContainer;
	while (p_mc)	{

		// �V�������b�V�����쐬
		BSP<C_d3d_mesh> mesh(new C_d3d_mesh(p_mc->MeshData.pMesh));

		// �}�e���A����ݒ�
		for (int i = 0; i < (int)p_mc->NumMaterials; i++)	{

			// �}�e���A����ǉ�
			C_d3d_mtrl mtrl;
			mtrl.mtrl = p_mc->pMaterials[i].MatD3D;
			mesh->add_material(mtrl);

			// �e�N�X�`�������X�g�Ƀe�N�X�`������ǉ�
			MBSTR name = p_mc->pMaterials[i].pTextureFilename ? p_mc->pMaterials[i].pTextureFilename : "";
			texture_name_list_ret.push_back(MBSTR_to_TSTR(name));
		}

		// �X�L��
		if (m_frame->pMeshContainer->pSkinInfo)	{
			ID3DXSkinInfo* skin = m_frame->pMeshContainer->pSkinInfo;

			// �{�[���I�t�Z�b�g�s��̃R�s�[
			m_offset_matrix_list.resize(skin->GetNumBones());
			for (int i = 0; i < (int)skin->GetNumBones(); i++)
				memcpy(&m_offset_matrix_list[i], skin->GetBoneOffsetMatrix(i), sizeof(D3DXMATRIX));

			// �{�[���s��̃R�s�[
			m_root_matrix_list.resize(skin->GetNumBones());
			for (int i = 0; i < (int)skin->GetNumBones(); i++)	{
				S_d3d_frame* p_parent_frame = (S_d3d_frame*)D3DXFrameFind(root_frame->m_frame, skin->GetBoneName(i));
				m_root_matrix_list[i] = &p_parent_frame->CombinedTransformationMatrix;
			}
		}

		// ���b�V�����X�g�Ƀ��b�V����ǉ�
		m_mesh_list.push_back(mesh);
		mesh_list_ret.push_back(mesh);

		// ���̃��b�V����
		p_mc = (S_d3d_skinned_mesh_container *)p_mc->pNextMeshContainer;
	}

	// �Z�킪����ꍇ
	if (p_frame->pFrameSibling)	{
		m_sibling = BSP<C_d3d_frame>(new C_d3d_frame);
		m_sibling->create_frame(root_frame, (S_d3d_frame *)p_frame->pFrameSibling, mesh_list_ret, texture_name_list_ret);
	}

	// �q��������ꍇ
	if (p_frame->pFrameFirstChild)	{
		m_first_child = BSP<C_d3d_frame>(new C_d3d_frame);
		m_first_child->create_frame(root_frame, (S_d3d_frame *)p_frame->pFrameFirstChild, mesh_list_ret, texture_name_list_ret);
	}



	return true;
}

// ****************************************************************
// ���[�J���̕ϊ��s����X�V����
// ================================================================
bool C_d3d_frame::update_matrix(const D3DXMATRIX* parent_matrix)
{
	if (!m_frame)
		return false;

	// �`�惊�X�g�ɒǉ�����
	//if(m_frame->pMeshContainer) {
	//	m_draw_list.push_back(m_frame->pMeshContainer);
	//}

	// ���[���h�ϊ��s���e�ƍ���
	if (parent_matrix) {
		// �e�t���[���̕ϊ��s�񂪂���΍�������
		D3DXMatrixMultiply(&m_frame->CombinedTransformationMatrix, &m_frame->TransformationMatrix, parent_matrix);
	}
	else {
		// �e�t���[���̕ϊ��s�񂪂Ȃ���΂��̃t���[���̕ϊ��s��ɂ���
		m_frame->CombinedTransformationMatrix = m_frame->TransformationMatrix;
	}

	// �Z�킪����ꍇ
	if (m_sibling)
		m_sibling->update_matrix(parent_matrix);

	// �q��������ꍇ
	if (m_first_child)
		m_first_child->update_matrix(&m_frame->CombinedTransformationMatrix);



	return true;
}







// ********************************************************************************************************************************
// �t���[���Z�b�g
// ================================================================================================================================

// ****************************************************************
// �t���[��
// ================================================================
C_d3d_frame_set::C_d3d_frame_set()
{
	init();
}

C_d3d_frame_set::~C_d3d_frame_set()
{
	release();
}

void C_d3d_frame_set::init()
{
	release();
}

void C_d3d_frame_set::release()
{
	if (m_frame_root)	{
		D3DXFrameDestroy(m_frame_root->body(), &m_allocate_hierarchy);
		m_frame_root.reset();
	}

//	animation_controller = NULL;
}


// ****************************************************************
// �w�t�@�C�����烁�b�V����ǂݍ���
// ================================================================
bool C_d3d_frame_set::load_x(CTSTR& file_path, ARRAY< BSP<C_d3d_mesh> >& mesh_list, ARRAY<TSTR>& texture_name_list)
{
	HRESULT hr;

	init();

	ID3DXAnimationController* p_animation_controller = NULL;
	S_d3d_frame* p_frame_root = NULL;

	// �w�t�@�C������K�w��ǂݍ���ł݂�I
	hr = D3DXLoadMeshHierarchyFromX(file_path.c_str(), D3DXMESH_MANAGED, G_d3d.device.body(), &m_allocate_hierarchy, NULL, (D3DXFRAME **)&p_frame_root, &p_animation_controller);
	if (FAILED(hr))	{
		set_last_error_d3d(_T("���b�V���ɊK�w������܂���ł����B"), _T("D3DXLoadMeshHierarchyFromX"), hr);
		return false;
	}

	if (p_animation_controller) {
		m_animation_controller.set_animation_controller(p_animation_controller);
//		animation_controller = BIP<ID3DXAnimationController>(p_animation_controller, false);
	}

	// �K�w���ǂ߂��ꍇ�̏���

	// �e�t���[�����\�z���Ă���
	m_frame_root = BSP<C_d3d_frame>(new C_d3d_frame);
	m_frame_root->create_frame(m_frame_root.get(), p_frame_root, mesh_list, texture_name_list);

	// �S���b�V���Ɉꊇ�ݒ�̎��Ɏg�����X�g�ɓo�^/*�v�C��*/
	for(int i = 0; i < (int)mesh_list.size(); i++) {
		m_mesh_list_for_parm_set.push_back(mesh_list[i].get());
	}

	return true;
}

// ****************************************************************
// �A�j���[�V������i�߂�
// ================================================================
//bool C_d3d_frame_set::advance_animation(double past_time)
//{
//	if (!animation_controller)
//		return false;
//
//	// �A�j���[�V������i�߂�
//	animation_controller->AdvanceTime(past_time, NULL);
//
//	return true;
//}

// ****************************************************************
// �ϊ��s����X�V����
// ================================================================
bool C_d3d_frame_set::update_matrix(const D3DXMATRIX* matrix)
{
	if (!m_frame_root)
		return false;

	// �ϊ��s����X�V����
	m_frame_root->update_matrix(matrix);

	return true;
}

// ****************************************************************
// �A�j���[�V������i�߂�
// ================================================================
void C_d3d_frame_set::update_animation_controller()
{
	m_animation_controller.update();
}

// ****************************************************************
// �t���[�������g���Ă��̃N���X�̃t���[���Z�b�g������t���[������������
// ================================================================
C_d3d_frame* C_d3d_frame_set::search_frame_by_name(MBSTR& frame_name)
{
	// ���[�g���猟��
	C_d3d_frame* p_frame = m_frame_root.get();
	return search_frame_hierarchy_by_name(frame_name, p_frame);
}
// ****************************************************************
// �t���[�������g���Ĉ����̃t���[���Z�b�g������t���[������������
// ================================================================
C_d3d_frame* C_d3d_frame_set::search_frame_by_name(MBSTR& frame_name, C_d3d_frame_set* p_frame_set)
{
	// �����̃��[�g���猟��
	C_d3d_frame* p_frame = p_frame_set->m_frame_root.get();
	return search_frame_hierarchy_by_name(frame_name, p_frame);
}
// ****************************************************************
// �t���[�������g���ăt���[���Z�b�g������t���[������������
// ================================================================
C_d3d_frame* C_d3d_frame_set::search_frame_hierarchy_by_name(MBSTR& frame_name, C_d3d_frame* p_frame)
{
	C_d3d_frame* return_frame = NULL;

	if(strcmp(p_frame->body()->Name, frame_name.c_str()) == 0)
		return p_frame;	// ��������

	if(p_frame->m_sibling) {
		return_frame = search_frame_hierarchy_by_name(frame_name, p_frame->m_sibling.get());
		if(return_frame)
			return return_frame;
	}

	if(p_frame->m_first_child) {
		return_frame = search_frame_hierarchy_by_name(frame_name, p_frame->m_first_child.get());
		if(return_frame)
			return return_frame;
	}

	return return_frame;
}

// ****************************************************************
// �p�����[�^�̐ݒ�(�Ώۃt���[���̂�)
// ================================================================
void C_d3d_frame_set::set_parameter_to_target(C_d3d_frame* p_frame, C_d3d_frame_hierarchy_parameter_setter* p_setter)
{
	if(!p_frame)
		return;

	// �Ώۃt���[���ɐݒ�
	p_setter->set_up_parameter_to_frame(p_frame);
}
// ****************************************************************
// �p�����[�^�̐ݒ�(�Ώۃt���[���ƑΏۃt���[���̎q���ȉ��S��)
// ================================================================
void C_d3d_frame_set::set_parameter_to_target_and_target_below_child(C_d3d_frame* p_frame, C_d3d_frame_hierarchy_parameter_setter* p_setter)
{
	if(!p_frame)
		return;

	// �Ώۃt���[���ɐݒ�
	p_setter->set_up_parameter_to_frame(p_frame);

	// �Ώۃt���[���̎q�ȉ��ɐݒ肵�Ă���
	if(p_frame->m_first_child) {
		set_parameter_hierarchy(p_frame->m_first_child.get(), p_setter);
	}

	return;
}
// ****************************************************************
// �p�����[�^�̐ݒ�(�Ώۃt���[���̎q���ȉ��S�āB�������Ώۃt���[��������)
// ================================================================
void C_d3d_frame_set::set_parameter_to_target_below_child(C_d3d_frame* p_frame, C_d3d_frame_hierarchy_parameter_setter* p_setter)
{
	if(!p_frame)
		return;

	// �Ώۃt���[���̎q�ȉ��ɐݒ肵�Ă���
	if(p_frame->m_first_child) {
		set_parameter_hierarchy(p_frame->m_first_child.get(), p_setter);
	}

	return;
}
// ****************************************************************
// �p�����[�^�̐ݒ�
// ================================================================
void C_d3d_frame_set::set_parameter_hierarchy(C_d3d_frame* p_frame, C_d3d_frame_hierarchy_parameter_setter* p_setter)
{
	if(!p_frame)
		return;

	p_setter->set_up_parameter_to_frame(p_frame);

	if(p_frame->m_sibling) {
		set_parameter_hierarchy(p_frame->m_sibling.get(), p_setter);
	}
	if(p_frame->m_first_child) {
		set_parameter_hierarchy(p_frame->m_first_child.get(), p_setter);
	}

	return;
}





// ********************************************************************************************************************************
// �K�w�\���ɑ΂���p�����[�^�ݒ�p�N���X�Ȃ�
// ================================================================================================================================
// �ݒ�p(�t���[���ɑ΂���)�̃p�����[�^�����̃N���X�ɃZ�b�g����
void C_d3d_frame_hierarchy_parameter_setter::set_parameter_for_set_up(FRAME_HIERARCHY_PARAMETER_TYPE type, int data)
{
	// �o�^�ς݂̏ꍇ
	if(int_map.find(type) != int_map.end()) {
		// �폜����
		int_map.erase(type);
	}
	// �ⓚ���p�ŏ㏑������(���݂�����x���o�����炢�ɂ͂��邩��)
	int_map.insert(std::make_pair(type, data));
}
void C_d3d_frame_hierarchy_parameter_setter::set_parameter_for_set_up(FRAME_HIERARCHY_PARAMETER_TYPE type, bool data)
{
	// �o�^�ς݂̏ꍇ
	if(bool_map.find(type) != bool_map.end()) {
		// �폜����
		bool_map.erase(type);
	}
	// �ⓚ���p�ŏ㏑������(���݂�����x���o�����炢�ɂ͂��邩��)
	bool_map.insert(std::make_pair(type, data));
}
void C_d3d_frame_hierarchy_parameter_setter::set_parameter_for_set_up(FRAME_HIERARCHY_PARAMETER_TYPE type, float data)
{
	// �o�^�ς݂̏ꍇ
	if(float_map.find(type) != float_map.end()) {
		// �폜����
		float_map.erase(type);
	}
	// �ⓚ���p�ŏ㏑������(���݂�����x���o�����炢�ɂ͂��邩��)
	float_map.insert(std::make_pair(type, data));
}
void C_d3d_frame_hierarchy_parameter_setter::set_parameter_for_set_up(FRAME_HIERARCHY_PARAMETER_TYPE type, const D3DXVECTOR4& data)
{
	// �o�^�ς݂̏ꍇ
	if(vector4_map.find(type) != vector4_map.end()) {
		// �폜����
		vector4_map.erase(type);
	}
	// �ⓚ���p�ŏ㏑������(���݂�����x���o�����炢�ɂ͂��邩��)
	vector4_map.insert(std::make_pair(type, data));
}
void C_d3d_frame_hierarchy_parameter_setter::set_parameter_for_set_up(FRAME_HIERARCHY_PARAMETER_TYPE type, const ARRAY<int>& data)
{
	// �o�^�ς݂̏ꍇ
	if(int_array_map.find(type) != int_array_map.end()) {
		// �폜����
		int_array_map.erase(type);
	}
	// �ⓚ���p�ŏ㏑������(���݂�����x���o�����炢�ɂ͂��邩��)
	int_array_map.insert(std::make_pair(type, data));
}

// �t���[���Ƀp�����[�^��ݒ肷��(���̃N���X�ɃZ�b�g���ꂽ���̂�)
void C_d3d_frame_hierarchy_parameter_setter::set_up_parameter_to_frame(C_d3d_frame* p_frame)
{
	FRAME_HIERARCHY_PARAMETER_TYPE_INT_MAP::iterator iit;

	// MP__LIGHTING_TYPE
	iit = int_map.find(MP__LIGHTING_TYPE);
	if(iit != int_map.end()) {
		for(int i = 0; i < (int)p_frame->m_mesh_list.size(); i++) {
			p_frame->m_mesh_list[i].get()->get_mesh_param_ptr()->lighting_type = (LIGHTING_TYPE)iit->second;
		}
	}

	// MP__SHADING_TYPE
	iit = int_map.find(MP__SHADING_TYPE);
	if(iit != int_map.end()) {
		for(int i = 0; i < (int)p_frame->m_mesh_list.size(); i++) {
			p_frame->m_mesh_list[i].get()->get_mesh_param_ptr()->shading_type = (SHADING_TYPE)iit->second;
		}
	}

}






#if 0// �o�b�N�A�b�v

// ****************************************************************
// D3D �t���[���K�w�F���b�V���R���e�i���\�z����
// ================================================================
HRESULT C_d3d_allocate_hierarchy::CreateMeshContainer(
		LPCSTR Name, 
		CONST D3DXMESHDATA *pMeshData, 
		CONST D3DXMATERIAL *pMaterials, 
		CONST D3DXEFFECTINSTANCE *pEffectInstances, 
		DWORD NumMaterials, 
		CONST DWORD *pAdjacency, 
		LPD3DXSKININFO pSkinInfo, 
		LPD3DXMESHCONTAINER *ppNewMeshContainer
		)
{
	HRESULT hr;

	// ������₷�����b�V���f�[�^�ւ̃|�C���^���擾���Ă���
	ID3DXMesh* p_mesh = pMeshData->pMesh;

	// ���ʏ�̃��b�V���ȊO�͖��Ή�
	if (pMeshData->Type != D3DXMESHTYPE_MESH)	{
		set_last_error(_T("�v���O���b�V�u���b�V����p�b�`���b�V���ɂ͑Ή����Ă��܂���B"), _T("CreateMeshContainer"));
		return E_FAIL;
	}

	// ��FVF ���ݒ肳��Ă��Ȃ����b�V���͖��Ή�
	if (p_mesh->GetFVF() == 0)	{
		set_last_error(_T("FVF ���ݒ肳��Ă��Ȃ����b�V���͖��Ή��ł��B"), _T("CreateMeshContainer"));
		return E_FAIL;
	}

	// �V�������b�V���R���e�i�����
	/*
	D3DXMESHCONTAINER* p_mc = NULL;
	// �X�L�����b�V���̏ꍇ�̏���
	if(pSkinInfo) {
		p_mc = new S_d3d_skinned_mesh_container;
		ZeroMemory(p_mc, sizeof(S_d3d_skinned_mesh_container));
	}
	// �X�L�����b�V���ȊO�̏ꍇ�̏���
	else {
		p_mc = new D3DXMESHCONTAINER;
		ZeroMemory(p_mc, sizeof(D3DXMESHCONTAINER));
	}
	*/
	S_d3d_skinned_mesh_container* p_mc = NULL;
	p_mc = new S_d3d_skinned_mesh_container;
	ZeroMemory(p_mc, sizeof(S_d3d_skinned_mesh_container));

	// ���O���R�s�[
	p_mc->Name = allocate_name(Name);

	// ���b�V���f�[�^

	// �@��������ꍇ�͂��̂܂܃R�s�[����
	if (p_mesh->GetFVF() & D3DFVF_NORMAL)	{
		// �ڃx�N�g�����Z�o����(��Ƀo���v�}�b�v�p)
		//hr = D3DXComputeTangent(p_mesh, 0, 0, 0, TRUE, NULL);
		//if(FAILED(hr)) { 
		//	set_last_error_d3d(_T("D3DXComputeTangent"), hr);
		//}

		// ���b�V���i�ւ̃|�C���^�j���R�s�[����
		p_mc->MeshData.Type = D3DXMESHTYPE_MESH;
		p_mc->MeshData.pMesh = p_mesh;
		p_mesh->AddRef();	// �Q�ƃJ�E���^�𑝂₷�I
	}
	// �@�����Ȃ��ꍇ�͖@�����v�Z����
	else	{
		p_mc->MeshData.Type = D3DXMESHTYPE_MESH;

		// ���b�V���ɖ@�����Ȃ������̂Œǉ�����
		hr = p_mesh->CloneMeshFVF(p_mesh->GetOptions(), p_mesh->GetFVF() | D3DFVF_NORMAL, G_d3d.device.body(), &p_mc->MeshData.pMesh);
		if (FAILED(hr))	{
			set_last_error_d3d(_T("���b�V���̕����Ɏ��s���܂����B"), _T("CloneMeshFVF"), hr);
			return E_FAIL;
		}

		// ���b�V���ւ̃|�C���^���Ď擾
		p_mesh = p_mc->MeshData.pMesh;

		// ���ۂɖ@�����v�Z����
		D3DXComputeNormals(p_mesh, NULL);

		// �ڃx�N�g�����Z�o����(��Ƀo���v�}�b�v�p)
		//hr = D3DXComputeTangent(p_mesh, 0, 0, 0, TRUE, NULL);
		//if(FAILED(hr)) { 
		//	set_last_error_d3d(_T("D3DXComputeTangent"), hr);
		//}

	}

	// �ʂ̐����擾���Ă���
	int face_cnt = p_mesh->GetNumFaces();

	// �}�e���A�����̐ݒ�(�Œ�1�͊m�ۂ���)
	p_mc->NumMaterials = std::max<DWORD>(1, NumMaterials);
	// �}�e���A���z��̊m��
	p_mc->pMaterials = new D3DXMATERIAL[p_mc->NumMaterials];

	// �אڐ��o�b�t�@�̊m��
	p_mc->pAdjacency = new DWORD[face_cnt * 3];
	// �אڐ��o�b�t�@���R�s�[����
	memcpy(p_mc->pAdjacency, pAdjacency, sizeof(DWORD) * face_cnt * 3);

	// �}�e���A���������
	if(NumMaterials > 0) {
		// �}�e���A���z����R�s�[����
		memcpy(p_mc->pMaterials, pMaterials, sizeof(D3DXMATERIAL) * NumMaterials);

		for(int i = 0; i < (int)NumMaterials; i++) {
			if(pMaterials[i].pTextureFilename) {
				// �e�N�X�`�������� C_d3d_frame::create_frame �Ƃ��Ŗ��O����s���̂Ŗ��O���R�s�[���Ă���
				p_mc->pMaterials[i].pTextureFilename = allocate_name(pMaterials[i].pTextureFilename);
			}
			// �w�t�@�C���ł̓A���r�G���g�͒�`�ł��Ȃ��̂ŁA�f�B�t���[�Y���R�s�[���Ă���/*�v�C��*/
			p_mc->pMaterials[i].MatD3D.Ambient = p_mc->pMaterials[i].MatD3D.Diffuse;
		}
	}
	// �}�e���A�����Ȃ����
	else {
		// �K���Ȓl������

	}


	// �G�t�F�N�g
	/*
	p_mc->pEffects = new D3DXEFFECTINSTANCE;
	ZeroMemory(p_mc->pEffects, sizeof(D3DXEFFECTINSTANCE));
	if (pEffectInstances->pEffectFilename)	{
		dst->pEffects->pEffectFilename = new char[strlen(pEffectInstances->pEffectFilename) + 1];
		strcpy_s(dst->pEffects->pEffectFilename, strlen(pEffectInstances->pEffectFilename) + 1, pEffectInstances->pEffectFilename);
	}
	dst->pEffects->NumDefaults = pEffectInstances->NumDefaults;
	dst->pEffects->pDefaults = new D3DXEFFECTDEFAULT[pEffectInstances->NumDefaults];
	for (int i = 0; i < (int)pEffectInstances->NumDefaults; i++)	{
		D3DXEFFECTDEFAULT* dst_default = &dst->pEffects->pDefaults[i];
		D3DXEFFECTDEFAULT* src_default = &pEffectInstances->pDefaults[i];

		dst_default->pParamName = new char[strlen(src_default->pParamName) + 1];
		strcpy_s(dst_default->pParamName, strlen(src_default->pParamName) + 1, src_default->pParamName);
		dst_default->NumBytes = src_default->NumBytes;
		dst_default->Type = src_default->Type;
		if (src_default->Type <= D3DXEDT_DWORD)	{
			dst_default->pValue = new BYTE[src_default->NumBytes];
			memcpy(dst_default->pValue, src_default->pValue, src_default->NumBytes);
		}
	}
	*/

	// ���̃��b�V���R���e�i(�ǂ̃T���v�������Ă�DrawFrame�Ŏg���Ă���̂ɏ������ł͉������Ă��Ȃ�)
	// ZeroMemory���Ă邯�ǃR�����g���c�����߂�NULL������Ă���
	p_mc->pNextMeshContainer = NULL;

	// �X�L��
	if (pSkinInfo)	{

		// �X�L���i�ւ̃|�C���^�j���R�s�[
		p_mc->pSkinInfo = pSkinInfo;
		pSkinInfo->AddRef();			// �Q�ƃJ�E���^�𑝂₷�I

		/*
		// �_�E���L���X�g���s��(C�L���X�g���g���Ă���̂̓R���p�C�����ʂ�Ȃ���������)
		// class����Ȃ��������R���p�C���I�v�V�����Ȃ̂��͕s��(���̂����e�X�g����)
		S_d3d_skinned_mesh_container* p_smc = (S_d3d_skinned_mesh_container*)p_mc;

		// ���̃��b�V���i�ւ̃|�C���^�j���R�s�[
		p_smc->pOrigMesh = p_mesh;
		p_mesh->AddRef();				// �Q�ƃJ�E���^�𑝂₷�I

		// �X�L�����b�V������ʉ�����H
		hr = generate_skinned_mesh(p_smc);
		*/

		// ���̃��b�V���i�ւ̃|�C���^�j���R�s�[
		p_mc->pOrigMesh = p_mesh;
		p_mesh->AddRef();				// �Q�ƃJ�E���^�𑝂₷�I
		// �X�L�����b�V������ʉ�����H
		hr = generate_skinned_mesh(p_mc);

		if(FAILED(hr)) {
			return hr;
		}
	}

	// ���[�J���ɐ����������b�V���R���e�i��߂�l�ɐݒ肷��
	*ppNewMeshContainer = p_mc;

	return D3D_OK;
}


// ****************************************************************
// ���b�V���F�t���[���K�w
// ================================================================
HRESULT C_d3d_allocate_hierarchy::generate_skinned_mesh(S_d3d_skinned_mesh_container *p_mc)
{
	HRESULT hr = S_OK;

	// �X�L�����Ȃ��ꍇ�������Ȃ�
	if (!p_mc->pSkinInfo)
		return E_FAIL;	//return S_OK;

	// ���b�V���ƃ{�[���R���r�l�[�V�����o�b�t�@�͕K�v�Ȃ��̂ŉ������(����Ƃ������������H����ĂȂ����ۂ�)
	SAFE_RELEASE(p_mc->MeshData.pMesh);
	SAFE_RELEASE(p_mc->pBoneCombinationBuf);


	// �s��p���b�g �X�L�j���O�Ɏg�p�\�ȃ{�[���s��̐������߂�
	UINT MaxMatrices = 26; 
	p_mc->NumPaletteEntries = std::min<DWORD>(MaxMatrices, p_mc->pSkinInfo->GetNumBones());

	// ���b�V�����󂯎��A���_�P�ʂ̃u�����h�̏d�݂ƃC���f�b�N�X�ƃ{�[���̑g�ݍ��킹�e�[�u����
	// �K�p�����V�������b�V����Ԃ��܂��B���̃e�[�u���́A�{�[�� �p���b�g�ƃ��b�V���̃T�u�Z�b�g�̊֌W��\���܂��B
	hr = p_mc->pSkinInfo->ConvertToIndexedBlendedMesh(
							p_mc->pOrigMesh,							// ���̓��b�V��
							D3DXMESH_MANAGED | D3DXMESHOPT_VERTEXCACHE, // ���݂͎g���Ă��܂���
							p_mc->NumPaletteEntries,					// �s��p���b�g �X�L�j���O�Ɏg�p�\�ȃ{�[���s��̐�
							p_mc->pAdjacency,							// ���̓��b�V���̗אڐ����
							NULL,										// �o�̓��b�V���̗אڐ����
							NULL,										// �e�ʂ̐V�����C���f�b�N�X���i�[����]����o�b�t�@�ւ̃|�C���^�BNULL �ł����܂��܂���B
							NULL,										// �e���_�̐V�����C���f�b�N�X���i�[����AID3DXBuffer �C���^�[�t�F�C�X�ւ̃|�C���^�̃A�h���X�BNULL �ł����܂��܂���B
							&p_mc->NumInfl,								// 1�̒��_�ɉe������{�[���̍ő吔
							&p_mc->NumAttributeGroups,					// �{�[���R���r�l�[�V�����̐�
							&p_mc->pBoneCombinationBuf,					// �{�[���R���r�l�[�V�����e�[�u���Ƃ����{�[���ƒ��_�̑Ή��\�̔z��
							&p_mc->MeshData.pMesh						// �X�L�����b�V���p�ɍœK�������o�̓��b�V��
						   );
	if (FAILED(hr))	{
		set_last_error_d3d(_T("ConvertToIndexedBlendedMesh �Ɏ��s���܂����B"), _T("ConvertToIndexedBlendedMesh"), hr);
		return hr;
	}


	// FVF has to match our declarator. Vertex shaders are not as forgiving as FF pipeline
	// D3DFVF_TEX1 ���K�{�Ȃ̂͂�����������/*�v�C��*/
	DWORD NewFVF = (p_mc->MeshData.pMesh->GetFVF() & D3DFVF_POSITION_MASK) | D3DFVF_NORMAL | D3DFVF_TEX1 | D3DFVF_LASTBETA_UBYTE4;
	if (NewFVF != p_mc->MeshData.pMesh->GetFVF()) {
		LPD3DXMESH temp_mesh = NULL;
		hr = p_mc->MeshData.pMesh->CloneMeshFVF(p_mc->MeshData.pMesh->GetOptions(), NewFVF, G_d3d.device.body(), &temp_mesh);
		if(FAILED(hr)) {
			set_last_error_d3d(_T("CloneMeshFVF �Ɏ��s���܂����B"), _T("CloneMeshFVF"), hr);
			return hr;
		}
		else {
			p_mc->MeshData.pMesh->Release();
			p_mc->MeshData.pMesh = temp_mesh;
			temp_mesh = NULL;
		}
	}
	DWORD fvf = p_mc->MeshData.pMesh->GetFVF();

	

	// Geforce3�����UBYTE4�ɑΉ����Ă���ۂ�(D3DCOLOR�ւ̕ϊ��s�v)�̂ŉ��̃\�[�X�̓R�����g�A�E�g���Ă���
	/*
    // Interpret the UBYTE4 as a D3DCOLOR.
    // The GeForce3 doesn't support the UBYTE4 decl type.  So, we convert any
    // blend indices to a D3DCOLOR semantic, and later in the shader convert
    // it back using the D3DCOLORtoUBYTE4() intrinsic.  Note that we don't
    // convert any data, just the declaration.
	D3DVERTEXELEMENT9 pDecl[MAX_FVF_DECL_SIZE];
	LPD3DVERTEXELEMENT9 pDeclCur;
	hr = p_mc->MeshData.pMesh->GetDeclaration(pDecl);
	if (FAILED(hr))	{
		set_last_error_d3d(_T("GetDeclaration �Ɏ��s���܂����B"), _T("GetDeclaration"), hr);
		return hr;
	}

	// the vertex shader is expecting to interpret the UBYTE4 as a D3DCOLOR, so update the type 
	//   NOTE: this cannot be done with CloneMesh, that would convert the UBYTE4 data to float and then to D3DCOLOR
	//          this is more of a "cast" operation
	pDeclCur = pDecl;
	while (pDeclCur->Stream != 0xff) {
		if ((pDeclCur->Usage == D3DDECLUSAGE_BLENDINDICES) && (pDeclCur->UsageIndex == 0))
			pDeclCur->Type = D3DDECLTYPE_D3DCOLOR;
		pDeclCur++;
	}

	hr = p_mc->MeshData.pMesh->UpdateSemantics(pDecl);
	if (FAILED(hr))	{
		set_last_error_d3d(_T("UpdateSemantics �Ɏ��s���܂����B"), _T("UpdateSemantics"), hr);
		return hr;
	}
	*/

	// allocate a buffer for bone matrices, but only if another mesh has not allocated one of the same size or larger
	if( p_mc->pSkinInfo->GetNumBones() > 0 ) {
		// �s�񃊃X�g���m�ۂ���
		p_mc->SkinnedBlendMatrices.reserve(p_mc->pSkinInfo->GetNumBones());
		p_mc->SkinnedBlendMatrices.resize(p_mc->pSkinInfo->GetNumBones());
	}


	/*
	// If the device can only do 2 matrix blends, ConvertToBlendedMesh cannot approximate all meshes to it
	// Thus we split the mesh in two parts: The part that uses at most 2 matrices and the rest. The first is
	// drawn using the device's HW vertex processing and the rest is drawn using SW vertex processing.
	D3DXBONECOMBINATION* bone_combination = (D3DXBONECOMBINATION*)p_mc->pBoneCombinationBuf->GetBufferPointer();

	// �P�̒��_�ɉe������{�[���̍ő吔�𒲂ׁA
	// �f�o�C�X�̔\�͂𒴂��Ă��Ȃ����ǂ����𔻒肷��
	int group_no = 0;
	for (group_no = 0; group_no < (int)p_mc->NumAttributeGroups; group_no ++)	{

		// �e������{�[���̐��𒲂ׂ�
		DWORD infl_cnt = 0;
		for (DWORD infl_no = 0; infl_no < p_mc->NumInfl; infl_no++)	{
			if (bone_combination[group_no].BoneId[infl_no] != UINT_MAX)
				++infl_cnt;
		}

		// �f�o�C�X�̔\�͂𒴂����{�[������������ΏI��
		//if (infl_cnt > G_d3d.device.device_caps().MaxVertexBlendMatrices)
		//	break;
	}

	// �f�o�C�X�̔\�͂𒴂����{�[���������������ꍇ�A
	// ���b�V�����\�t�g�E�F�A�`��ɂ���
	if (group_no < (int)p_mc->NumAttributeGroups)	{
		ID3DXMesh* p_mesh_tmp = NULL;
		hr = p_mc->MeshData.pMesh->CloneMeshFVF(D3DXMESH_SOFTWAREPROCESSING | p_mc->MeshData.pMesh->GetOptions(), p_mc->MeshData.pMesh->GetFVF(), G_d3d.device.body(), &p_mesh_tmp);
		if (FAILED(hr))	{
			set_last_error_d3d(_T("CloneMeshFVF �Ɏ��s���܂����B"), _T("CloneMeshFVF"), hr);
			return hr;
		}

		// �V�������b�V���ɓ���ւ���
		p_mc->MeshData.pMesh->Release();
		p_mc->MeshData.pMesh = p_mesh_tmp;
		p_mesh_tmp = NULL;
	}
	*/

	return S_OK;
}

#endif



}
