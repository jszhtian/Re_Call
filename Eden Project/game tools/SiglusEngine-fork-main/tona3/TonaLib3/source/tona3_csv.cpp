#include	"tona3_pch.h"
#include	"tona3_csv.h"
#include	"tona3_file.h"
#include	"tona3_filesys.h"

namespace NT3
{

	// ****************************************************************
	// CSV �t�@�C��
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
	// CSV �t�@�C����ǂݍ���
	// ================================================================
	bool C_csv::load_csv(CTSTR& file_path)
	{
		int type = 0;

		m_csv_data.clear();
		m_row_cnt = 0;
		m_column_cnt = 0;

		// �^�C�v
		if (get_extension(file_path) == _T("csv"))
		{
			type = 0;	// type 0�FShift-JIS �� csv�B
		}
		else
		{
			type = 1;	// type 1�FUTF-16 �̃^�u��؂�e�L�X�g�B
		}

		// �t�@�C�����J��
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

		TSTR item;				// �A�C�e��
		ARRAY<TSTR> row;		// ���[
		bool quote = false;		// �_�u���R�[�e�[�V�����t���O

		// �S�Ă̍s�ɑ΂��Č���
		// �_�u���R�[�e�[�V�������́A���s�����Ă��������p������_�ɒ��ӁI
		for (ARRAY<TSTR>::iterator line_itr = csv_data.begin(); line_itr != csv_data.end(); ++line_itr)	{
			TSTR line_data = *line_itr;

			for (TSTR::iterator p = line_data.begin(); ; ++p)	{

				if (false);

				// �s�̏I���
				else if (p == line_data.end())	{
					// �_�u���R�[�e�[�V�������̏ꍇ
					if (quote)	{
						break;		// �p�����Ď��̍s��
					}
					// �_�u���R�[�e�[�V����������Ȃ��ꍇ
					else	{
						row.push_back(item);	// �A�C�e���m��
						item.clear();
						break;
					}
				}
				// type 0: �J���}���������ꍇ
				else if (*p == _T(',') && !quote && type == 0)	{
					row.push_back(item);	// �A�C�e���m��
					item.clear();
				}
				// type 1: �^�u���������ꍇ
				else if (*p == _T('\t') && !quote && type == 1)	{
					row.push_back(item);	// �A�C�e���m��
					item.clear();
				}
				// �_�u���R�[�e�[�V�������������ꍇ
				else if (*p == _T('"'))	{
					// �_�u���R�[�e�[�V�����n�܂�̏ꍇ
					if (!quote)	{
						quote = true;		// �_�u���R�[�e�[�V�����t���O�𗧂Ă�
					}
					// �_�u���R�[�e�[�V�������̏ꍇ
					else	{
						// ���̕������_�u���R�[�e�[�V�����̏ꍇ
						if (*(p + 1) == _T('"'))	{
							item.push_back(*p);		// �_�u���R�[�e�[�V�������P�����ǉ�
							++p;					// �]���ɂP�����i��
						}
						// �P�Ƃ̏ꍇ
						else	{
							quote = false;		// �_�u���R�[�e�[�V�����t���O�I��
						}
					}
				}
				// �ʏ�̕���
				else	{
					item.push_back(*p);
				}
			}

			// �_�u���R�[�e�[�V�������łȂ��ꍇ
			if (!quote)	{
				// ���[�̊m��
				m_csv_data.push_back(row);
				row.clear();
			}
		}

		m_row_cnt = (int)m_csv_data.size();

		// ��2012/12/14
		// csv �́A�P�U�s�P�ʂŃR�����������Z�b�g�����炵���c
		// �擪�s�ł͂Ȃ��A��Ԓ����s����ɂ��Ȃ��Ƃ����Ȃ�

	#if 1

		// �����s�����߂�
		int max_column_cnt = 0;
		for (int i = 0; i < m_row_cnt; i++)	{
			if ((int)m_csv_data[i].size() > max_column_cnt)	{
				max_column_cnt = (int)m_csv_data[i].size();
			}
		}

		// �����s�ɍ��킹�āA�Z���s�ɂ͋󕶂�����
		for (int i = 0; i < m_row_cnt; i++)	{
			m_csv_data[i].resize(max_column_cnt);
		}

		// �R������
		m_column_cnt = max_column_cnt;

	#else
		m_column_cnt = m_row_cnt > 0 ? (int)m_csv_data[0].size() : 0;
	#endif

		return true;
	}

	// ****************************************************************
	// �A�C�e���̎擾
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
