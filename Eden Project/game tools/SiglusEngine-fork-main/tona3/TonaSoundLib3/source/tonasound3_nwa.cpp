#include	"tonasound3_pch.h"
#include	"tonasound3_nwa.h"

#include	<nwa.h>

namespace NT3
{

// ****************************************************************
// C_nwa_stream 実装構造体
// ================================================================
struct C_nwa_stream::Simpl
{
	// nwa 情報
	Cnwa_unpack		nwa;		// nwa 情報
};

// ****************************************************************
// nwa
// ================================================================
C_nwa_stream::C_nwa_stream()
{
	// 実装の作成
	impl = BSP<Simpl>(new Simpl);
}

C_nwa_stream::~C_nwa_stream()
{
	close();

	// 実装の破壊
	impl.reset();
}

// ****************************************************************
// nwa を開く
// ================================================================
bool C_nwa_stream::open(CTSTR& file_path, DWORD offset, DWORD size)
{
	// nwa を閉じる
	close();

	// nwa を開く
	if (!impl->nwa.open((TCHAR *)file_path.c_str(), offset))
		return false;

	// wave 情報をセット
	set_param(impl->nwa.get_info_channels(), impl->nwa.get_info_bits_per_sample(), impl->nwa.get_info_samples_per_sec(), impl->nwa.get_info_original_size());

	return true;
}

// ****************************************************************
// nwa を閉じる
// ================================================================
void C_nwa_stream::close()
{
	// nwa を閉じる
	impl->nwa.close();

	// 基底の初期化
	C_sound_stream_base::init();
}

// ****************************************************************
// nwa の読み込み
// ================================================================
int C_nwa_stream::read(BYTE* buf, int to_read_size)
{
	if (buf == NULL)
		return 0;

	// nwa では読み込むサイズはサンプル数で指定
	return impl->nwa.read_sample(buf, to_read_size / get_fragment_size()) * get_fragment_size();
}


// ****************************************************************
// 読み込み位置を設定
// ================================================================
bool C_nwa_stream::set_read_pos(int pos)
{
	// nwa では位置はサンプル数で指定
	impl->nwa.set_read_sample_pos(pos / get_fragment_size());
	return true;
}

// ****************************************************************
// 読み込み位置を取得
// ================================================================
int C_nwa_stream::get_read_pos()
{
	// nwa では位置はサンプル数で取得
	return impl->nwa.get_read_sample_pos() * get_fragment_size();
}

}
