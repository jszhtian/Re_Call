#include	"tonad3d3_pch.h"
#include	"tonad3d3_animation_controller.h"



namespace NT3
{

// ****************************************************************
// �A�j���[�V�����R���g���[��
// ================================================================
C_d3d_animation_controller::C_d3d_animation_controller()
:m_pAnimCont(NULL)
,m_AnimNo(0)
,m_AnimTrackNo(0)
,m_AnimTimeHold(0)
,m_AnimTime(0)
,m_AnimShiftTime(0)
,m_IsAnim(true)
{
}

C_d3d_animation_controller::~C_d3d_animation_controller()
{
}

void C_d3d_animation_controller::update()
{
	// �A�j���[�V�����R���g���[�������邩�ǂ����`�F�b�N����
	if (m_pAnimCont) {
		// �A�j���[�V�������Ԃ�i�߂�
		// �����ōX�V����s��̓��b�V����ԂŃ��b�V���𓮂����u���[�J���ϊ��s��v
		m_pAnimCont->AdvanceTime(m_AnimTime - m_AnimTimeHold, NULL);
		m_AnimTimeHold = m_AnimTime;

		if (m_IsAnim) {
			m_AnimTime += 60.0f / 4800.0f;		 
		}	
	}
}

void C_d3d_animation_controller::change_animation_no(UINT no)
{
	if (!m_pAnimCont) {
		return;
	}

	if(m_pAnimCont->GetNumAnimationSets() - 1 < no) {
		no = 0;
	}

	// �A�j���[�V�����g���b�N�����݂ɕύX����
	UINT newAnimTrack = (m_AnimTrackNo == 0 ? 1 : 0);			// �V�����g���b�N

	LPD3DXANIMATIONSET animationSet = NULL;
	// �A�j���[�V�����Z�b�g���擾����
	HRESULT hr = m_pAnimCont->GetAnimationSet(no, &animationSet);
	if (hr != S_OK) {
		// ���s�����狭���I��0�ɂ���
		no = 0;
		hr = m_pAnimCont->GetAnimationSet(no, &animationSet);
	}
	// ���̃g���b�N�ɃA�j���[�V�����Z�b�g��o�^����
	m_pAnimCont->SetTrackAnimationSet(newAnimTrack, animationSet);
	// �A�j���[�V�����Z�b�g�́C�����g��Ȃ��̂ŉ������
	animationSet->Release();

	// ���̃A�j���[�V�������������
	// �A�j���[�V�����g���b�N����C�C�x���g�����ׂč폜����
	m_pAnimCont->UnkeyAllTrackEvents(m_AnimTrackNo);
	// �A�j���[�V�����g���b�N�𖳌��ɂ���C�x���g�L�[��ݒ肷��
	m_pAnimCont->KeyTrackEnable(m_AnimTrackNo, FALSE, m_AnimTime + m_AnimShiftTime);
	// �A�j���[�V�����g���b�N�̍Đ����x��ύX����C�x���g�L�[��ݒ肷��
	m_pAnimCont->KeyTrackSpeed(m_AnimTrackNo, 0.0f, m_AnimTime, m_AnimShiftTime, D3DXTRANSITION_LINEAR);
	// �A�j���[�V�����g���b�N�̏d�݂�ύX����C�x���g�L�[��ݒ肷��d�݂�0�ɂ���
	m_pAnimCont->KeyTrackWeight(m_AnimTrackNo, 0.0f, m_AnimTime, m_AnimShiftTime, D3DXTRANSITION_LINEAR);

	// �V�����A�j���[�V������ݒ肷��
	// �A�j���[�V�����g���b�N����C�C�x���g�����ׂč폜����
	m_pAnimCont->UnkeyAllTrackEvents(newAnimTrack);
	// �A�j���[�V�����g���b�N��L���ɂ���C�x���g�L�[��ݒ肷��
	m_pAnimCont->KeyTrackEnable(newAnimTrack, TRUE, m_AnimTime + m_AnimShiftTime);
	// �A�j���[�V�����g���b�N�̍Đ����x��ύX����C�x���g�L�[��ݒ肷��
	m_pAnimCont->KeyTrackSpeed(newAnimTrack, 1.0f, m_AnimTime, m_AnimShiftTime, D3DXTRANSITION_LINEAR);
	// �A�j���[�V�����g���b�N�̏d�݂�ύX����C�x���g�L�[��ݒ肷��
	// �d�݂͕����̃g���b�N�𓯎��ɍĐ�����Ƃ��ɏ搔�Ƃ��Ďg��
	m_pAnimCont->KeyTrackWeight(newAnimTrack, 1.0f, m_AnimTime, m_AnimShiftTime, D3DXTRANSITION_LINEAR);

	// �V�����A�j���[�V�����g���b�N�ɐ؂�ւ���
	m_AnimNo = no;
	m_AnimTrackNo = newAnimTrack;
}

bool C_d3d_animation_controller::copy(C_d3d_animation_controller& source)
{
	HRESULT hr;

	// �R�s�[���̃A�j���[�V�����R���g���[�����`�F�b�N
	if (source.m_pAnimCont.get()) {
		ID3DXAnimationController* p_ac = NULL;
		// �A�j���[�V�����R���g���[�����R�s�[����
		hr = source.m_pAnimCont->CloneAnimationController(
												source.m_pAnimCont->GetMaxNumAnimationOutputs(),
												source.m_pAnimCont->GetMaxNumAnimationSets(),
												source.m_pAnimCont->GetMaxNumTracks(),
												source.m_pAnimCont->GetMaxNumEvents(),
												&p_ac);
		if(FAILED(hr)) {
			set_last_error_d3d(_T("CloneAnimationController"), hr);
			return false;
		}

		this->m_pAnimCont = BIP<ID3DXAnimationController>(p_ac, false);
	}

	if (this->m_pAnimCont.get()) {
		// �A�j���[�V�����g���b�N�����擾����
		DWORD numTracks = this->m_pAnimCont->GetMaxNumTracks();
		// ���ׂẴA�j���[�V�����g���b�N���g�p�s�ɂ���
		for (DWORD i = 0; i < numTracks; i++) {
			this->m_pAnimCont->SetTrackEnable(i, FALSE);
		}
		// �ŏ��̃A�j���[�V������ݒ肷��
		this->m_AnimNo = 0;
		this->m_AnimTrackNo = 0;
		this->m_AnimTime = 0;
		this->m_AnimTimeHold = 0;
		this->m_AnimShiftTime = 0.0f;
        // ���Ԃ�擪�ɖ߂�
		this->m_pAnimCont->ResetTime();
		this->m_pAnimCont->AdvanceTime(0, NULL);
		this->m_pAnimCont->SetTrackEnable(source.m_AnimTrackNo, TRUE);
		this->m_pAnimCont->SetTrackWeight(source.m_AnimTrackNo, 1.0f);
		// �A�j���[�V�����g���b�N�̍Đ����x��ύX����C�x���g�L�[��ݒ肷��
		this->m_pAnimCont->SetTrackSpeed(source.m_AnimTrackNo, 1.0f);
	}

	return true;
}

void C_d3d_animation_controller::set_animation_controller(ID3DXAnimationController* p_ctrl)
{
	// t�ł�f�ł����Ȃ������Ă��܂��E�E�E(true����AddRef)/*�v�C��*/
	m_pAnimCont = BIP<ID3DXAnimationController>(p_ctrl, true);
}

ID3DXAnimationController* C_d3d_animation_controller::get_animation_controller()
{
	return m_pAnimCont.get();
}

UINT C_d3d_animation_controller::get_animation_no()
{
	return m_AnimNo;
}

bool C_d3d_animation_controller::get_is_animation()
{
	return m_IsAnim;
}



void C_d3d_animation_controller::release_animation_controller()
{
	m_pAnimCont = NULL;
}


}