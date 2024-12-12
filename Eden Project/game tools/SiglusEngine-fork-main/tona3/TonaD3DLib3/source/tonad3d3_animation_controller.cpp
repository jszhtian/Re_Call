#include	"tonad3d3_pch.h"
#include	"tonad3d3_animation_controller.h"



namespace NT3
{

// ****************************************************************
// アニメーションコントローラ
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
	// アニメーションコントローラがあるかどうかチェックする
	if (m_pAnimCont) {
		// アニメーション時間を進める
		// ここで更新する行列はメッシュ空間でメッシュを動かす「ローカル変換行列」
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

	// アニメーショントラックを交互に変更する
	UINT newAnimTrack = (m_AnimTrackNo == 0 ? 1 : 0);			// 新しいトラック

	LPD3DXANIMATIONSET animationSet = NULL;
	// アニメーションセットを取得する
	HRESULT hr = m_pAnimCont->GetAnimationSet(no, &animationSet);
	if (hr != S_OK) {
		// 失敗したら強制的に0にする
		no = 0;
		hr = m_pAnimCont->GetAnimationSet(no, &animationSet);
	}
	// 次のトラックにアニメーションセットを登録する
	m_pAnimCont->SetTrackAnimationSet(newAnimTrack, animationSet);
	// アニメーションセットは，もう使わないので解放する
	animationSet->Release();

	// 元のアニメーションを解放する
	// アニメーショントラックから，イベントをすべて削除する
	m_pAnimCont->UnkeyAllTrackEvents(m_AnimTrackNo);
	// アニメーショントラックを無効にするイベントキーを設定する
	m_pAnimCont->KeyTrackEnable(m_AnimTrackNo, FALSE, m_AnimTime + m_AnimShiftTime);
	// アニメーショントラックの再生速度を変更するイベントキーを設定する
	m_pAnimCont->KeyTrackSpeed(m_AnimTrackNo, 0.0f, m_AnimTime, m_AnimShiftTime, D3DXTRANSITION_LINEAR);
	// アニメーショントラックの重みを変更するイベントキーを設定する重みを0にする
	m_pAnimCont->KeyTrackWeight(m_AnimTrackNo, 0.0f, m_AnimTime, m_AnimShiftTime, D3DXTRANSITION_LINEAR);

	// 新しいアニメーションを設定する
	// アニメーショントラックから，イベントをすべて削除する
	m_pAnimCont->UnkeyAllTrackEvents(newAnimTrack);
	// アニメーショントラックを有効にするイベントキーを設定する
	m_pAnimCont->KeyTrackEnable(newAnimTrack, TRUE, m_AnimTime + m_AnimShiftTime);
	// アニメーショントラックの再生速度を変更するイベントキーを設定する
	m_pAnimCont->KeyTrackSpeed(newAnimTrack, 1.0f, m_AnimTime, m_AnimShiftTime, D3DXTRANSITION_LINEAR);
	// アニメーショントラックの重みを変更するイベントキーを設定する
	// 重みは複数のトラックを同時に再生するときに乗数として使う
	m_pAnimCont->KeyTrackWeight(newAnimTrack, 1.0f, m_AnimTime, m_AnimShiftTime, D3DXTRANSITION_LINEAR);

	// 新しいアニメーショントラックに切り替える
	m_AnimNo = no;
	m_AnimTrackNo = newAnimTrack;
}

bool C_d3d_animation_controller::copy(C_d3d_animation_controller& source)
{
	HRESULT hr;

	// コピー元のアニメーションコントローラをチェック
	if (source.m_pAnimCont.get()) {
		ID3DXAnimationController* p_ac = NULL;
		// アニメーションコントローラをコピーする
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
		// アニメーショントラック数を取得する
		DWORD numTracks = this->m_pAnimCont->GetMaxNumTracks();
		// すべてのアニメーショントラックを使用不可にする
		for (DWORD i = 0; i < numTracks; i++) {
			this->m_pAnimCont->SetTrackEnable(i, FALSE);
		}
		// 最初のアニメーションを設定する
		this->m_AnimNo = 0;
		this->m_AnimTrackNo = 0;
		this->m_AnimTime = 0;
		this->m_AnimTimeHold = 0;
		this->m_AnimShiftTime = 0.0f;
        // 時間を先頭に戻す
		this->m_pAnimCont->ResetTime();
		this->m_pAnimCont->AdvanceTime(0, NULL);
		this->m_pAnimCont->SetTrackEnable(source.m_AnimTrackNo, TRUE);
		this->m_pAnimCont->SetTrackWeight(source.m_AnimTrackNo, 1.0f);
		// アニメーショントラックの再生速度を変更するイベントキーを設定する
		this->m_pAnimCont->SetTrackSpeed(source.m_AnimTrackNo, 1.0f);
	}

	return true;
}

void C_d3d_animation_controller::set_animation_controller(ID3DXAnimationController* p_ctrl)
{
	// tでもfでも問題なく動いてしまう・・・(trueだとAddRef)/*要修正*/
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