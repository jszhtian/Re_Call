#pragma		once

#include	"tonasound3_stream.h"

namespace NT3
{

	// ****************************************************************
	// wave ヘッダ
	// ================================================================
	struct wave_header
	{
		BYTE	ck_riff[4];
		DWORD	riff_size;
		BYTE	riff_type[4];
		BYTE	ck_fmt[4];
		DWORD	fmt_size;
		WORD	fmt_id;
		WORD	channel_cnt;
		DWORD	sample_cnt_per_sec;
		DWORD	byte_cnt_per_sec;
		WORD	block_size;
		WORD	bit_cnt_per_sample;
		BYTE	ck_data[4];
		DWORD	data_size;
	};

	// ****************************************************************
	// wave ストリーム
	//		サウンドストリームを継承。
	//		ファイルからストリーミング読み込みが出来ます。
	// ================================================================
	class C_wave_stream : public C_sound_stream_base
	{
	public:
		C_wave_stream();
		~C_wave_stream();

		// 基本関数
		bool	open(CTSTR& file_path, DWORD offset = 0, DWORD size = 0);		// 開く
		void	close();										// 閉じる
		int		read(BYTE* buf, int size);						// データを読み込む
		bool	set_read_pos(int pos);							// 読みこみ位置を設定
		int		get_read_pos();									// 読みこみ位置を取得

		// 情報を取得
		BYTE*	get_icop_ptr()		{	return icop_data.get();			}	// "ICOP" チャンクデータ
		int		get_icop_size()		{	return (int)icop_data.size();	}	// "ICOP" チャンクデータサイズ
		BYTE*	get_icrd_ptr()		{	return icrd_data.get();			}	// "ICRD" チャンクデータ
		int		get_icrd_size()		{	return (int)icrd_data.size();	}	// "ICRD" チャンクデータサイズ

	private:

		void	init();		// 初期化

		HMMIO		h_mmio;			// MMIO ハンドル
		MMCKINFO	ck_riff;		// "RIFF" チャンク
		MMCKINFO	ck_fmt;			// "fmt" チャンク
		MMCKINFO	ck_list;		// "LIST" チャンク
		MMCKINFO	ck_icop;		// "ICOP" チャンク
		MMCKINFO	ck_icrd;		// "ICRD" チャンク
		MMCKINFO	ck_data;		// "data" チャンク

		BUFFER		icop_data;		// ICOP チャンクのデータ
		BUFFER		icrd_data;		// ICRD チャンクのデータ
		int			read_pos;		// 読みこみ位置
	};
};
