#pragma		once

#include	"tonasound3_stream.h"

namespace NT3
{

	// ****************************************************************
	// ogg ストリーム
	//		サウンドストリームを継承。
	//		ファイルからストリーミング読み込みが出来ます。
	// ================================================================
	class C_ogg_stream : public C_sound_stream_base
	{
	public:
		C_ogg_stream(BYTE xor);
		~C_ogg_stream();

		// 基本関数
		bool	open(CTSTR& file_path, DWORD offset = 0, DWORD size = 0);	// 開く
		void	close();													// 閉じる
		int		read(BYTE* buf, int size);									// データを読み込む
		bool	set_read_pos(int pos);										// 読みこみ位置を設定
		int		get_read_pos();												// 読みこみ位置を取得

	private:

		void	init();			// 初期化

		struct		Simpl;
		BSP<Simpl>	impl;

		// ogg 情報
		int			read_pos;	// 読みこみ位置

		bool	read_err_check(int res);
	};

}
