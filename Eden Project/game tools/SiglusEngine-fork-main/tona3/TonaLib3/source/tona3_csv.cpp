#include	"tona3_pch.h"
#include	"tona3_csv.h"
#include	"tona3_file.h"
#include	"tona3_filesys.h"

namespace NT3
{

	// ****************************************************************
	// CSV ファイル
	// ================================================================
	C_csv::C_csv()
	{
		m_row_cnt = 0;
		m_column_cnt = 0;
	}

	C_csv::~C_csv()
	{
	}

	// ****************************************************************
	// CSV ファイルを読み込む
	// ================================================================
	bool C_csv::load_csv(CTSTR& file_path)
	{
		int type = 0;

		m_csv_data.clear();
		m_row_cnt = 0;
		m_column_cnt = 0;

		// タイプ
		if (get_extension(file_path) == _T("csv"))
		{
			type = 0;	// type 0：Shift-JIS の csv。
		}
		else
		{
			type = 1;	// type 1：UTF-16 のタブ区切りテキスト。
		}

		// ファイルを開く
		ARRAY<TSTR> csv_data;
		if (type == 0)
		{
			if (!C_file::read_full_text(file_path, csv_data))
				return false;
		}
		else
		{
			if (!C_file::read_full_text_UTF16(file_path, csv_data))
				return false;
		}

		TSTR item;				// アイテム
		ARRAY<TSTR> row;		// ロー
		bool quote = false;		// ダブルコーテーションフラグ

		// 全ての行に対して検索
		// ダブルコーテーション中は、改行が来ても処理を継続する点に注意！
		for (ARRAY<TSTR>::iterator line_itr = csv_data.begin(); line_itr != csv_data.end(); ++line_itr)	{
			TSTR line_data = *line_itr;

			for (TSTR::iterator p = line_data.begin(); ; ++p)	{

				if (false);

				// 行の終わり
				else if (p == line_data.end())	{
					// ダブルコーテーション中の場合
					if (quote)	{
						break;		// 継続して次の行へ
					}
					// ダブルコーテーション中じゃない場合
					else	{
						row.push_back(item);	// アイテム確定
						item.clear();
						break;
					}
				}
				// type 0: カンマを見つけた場合
				else if (*p == _T(',') && !quote && type == 0)	{
					row.push_back(item);	// アイテム確定
					item.clear();
				}
				// type 1: タブを見つけた場合
				else if (*p == _T('\t') && !quote && type == 1)	{
					row.push_back(item);	// アイテム確定
					item.clear();
				}
				// ダブルコーテーションを見つけた場合
				else if (*p == _T('"'))	{
					// ダブルコーテーション始まりの場合
					if (!quote)	{
						quote = true;		// ダブルコーテーションフラグを立てる
					}
					// ダブルコーテーション中の場合
					else	{
						// 次の文字もダブルコーテーションの場合
						if (*(p + 1) == _T('"'))	{
							item.push_back(*p);		// ダブルコーテーションを１つだけ追加
							++p;					// 余分に１文字進む
						}
						// 単独の場合
						else	{
							quote = false;		// ダブルコーテーションフラグ終了
						}
					}
				}
				// 通常の文字
				else	{
					item.push_back(*p);
				}
			}

			// ダブルコーテーション中でない場合
			if (!quote)	{
				// ローの確定
				m_csv_data.push_back(row);
				row.clear();
			}
		}

		m_row_cnt = (int)m_csv_data.size();

		// ■2012/12/14
		// csv は、１６行単位でコラム数がリセットされるらしい…
		// 先頭行ではなく、一番長い行を基準にしないといけない

	#if 1

		// 長い行を求める
		int max_column_cnt = 0;
		for (int i = 0; i < m_row_cnt; i++)	{
			if ((int)m_csv_data[i].size() > max_column_cnt)	{
				max_column_cnt = (int)m_csv_data[i].size();
			}
		}

		// 長い行に合わせて、短い行には空文を入れる
		for (int i = 0; i < m_row_cnt; i++)	{
			m_csv_data[i].resize(max_column_cnt);
		}

		// コラム数
		m_column_cnt = max_column_cnt;

	#else
		m_column_cnt = m_row_cnt > 0 ? (int)m_csv_data[0].size() : 0;
	#endif

		return true;
	}

	// ****************************************************************
	// アイテムの取得
	// ================================================================
	TSTR C_csv::get_item(int row_no, int column_no)
	{
		int row_cnt = (int)m_csv_data.size();
		if (0 <= row_no && row_no < row_cnt)	{
			int column_cnt = (int)m_csv_data[row_no].size();
			if (0 <= column_no && column_no < column_cnt)	{
				return m_csv_data[row_no][column_no];
			}
		}

		return _T("");
	}

};
