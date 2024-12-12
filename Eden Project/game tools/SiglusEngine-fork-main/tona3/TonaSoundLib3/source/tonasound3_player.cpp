#include	"tonasound3_pch.h"
#include	"tonasound3_player.h"

namespace NT3
{

// ****************************************************************
// �v���C���[
// ================================================================
C_player::C_player()
{
	m_h_semaphore = NULL;
	m_ds_buffer = NULL;
}

C_player::~C_player()
{
	release(true);	// ���������� true !!!
}

// ****************************************************************
// ������
// ================================================================
bool C_player::init()
{
	release(true);	// ���������� true !!!

	// �Z�}�t�H�쐬
	m_h_semaphore = CreateSemaphore(NULL, 1, 1, NULL);

	// �p�����[�^�̏�����
	init_param();

	return true;
}

// ****************************************************************
// �p�����[�^��������
// ================================================================
void C_player::init_param()
{
	m_ds_buffer = NULL;
	m_ds_buffer_size = 0;
	m_block_size = 0;
	m_block_cnt = 0;
	m_filled_block_cnt = 0;
	m_play_pos_on_buffer_last = 0;
	m_play_pos_on_sound_last = 0;
	m_last_sample[0] = 0;
	m_last_sample[1] = 0;
	m_last_sample[2] = 0;
	m_last_sample[3] = 0;
	m_play_start_time = 0;

	m_loop = false;
	m_play_start_pos = 0;
	m_play_end_pos = 0;
	m_play_restart_pos = 0;

	m_playing = false;
	m_pause_flag = false;
	m_fade_in_flag = false;
	m_fade_in_start_time = 0;
	m_fade_in_time_len = 0;
	m_fade_in_start_value = 0;
	m_fade_in_cur_value = 0;
	m_fade_out_flag = false;
	m_fade_out_no_release = false;
	m_fade_out_start_time = 0;
	m_fade_out_time_len = 0;
	m_fade_out_start_value = 0;
	m_fade_out_cur_value = 0;

	for (int i = 0; i < VOLUME_CHANNEL_CNT; i++)	{
		volume_channel[i].volume = VOLUME_MAX;
		volume_channel[i].fade_ing = false;
		volume_channel[i].fade_start_time = 0;
		volume_channel[i].fade_end_time = 0;
		volume_channel[i].fade_start_value = 0;
		volume_channel[i].fade_end_value = 0;
	}
}

// ****************************************************************
// �v���C���[�̍쐬
// ================================================================
bool C_player::create_player()
{
	HRESULT hr;

	// �v���C���[�����
	release_player();

	// ���[�_�[����T�E���h�̃p�����[�^���擾
	int channel_cnt = m_stream->get_channel_cnt();
	int bit_cnt_per_sample = m_stream->get_bit_cnt_per_sample();
	int sample_cnt_per_sec = m_stream->get_sample_cnt_per_sec();

	// �o�b�t�@�T�C�Y�v�Z
	m_ds_buffer_size = channel_cnt * (bit_cnt_per_sample / 8) * sample_cnt_per_sec * 8;	// �W�b��
	m_block_cnt = 2;
	m_block_size = m_ds_buffer_size / m_block_cnt;

	// wav �t�H�[�}�b�g�̍쐬
	WAVEFORMATEX wfex;
	ZeroMemory(&wfex, sizeof(WAVEFORMATEX));
	wfex.nChannels = channel_cnt;
	wfex.nSamplesPerSec = sample_cnt_per_sec;
	wfex.wBitsPerSample = bit_cnt_per_sample;
	wfex.nAvgBytesPerSec = (bit_cnt_per_sample / 8) * sample_cnt_per_sec * channel_cnt;
	wfex.nBlockAlign = (bit_cnt_per_sample / 8) * channel_cnt;
	wfex.wFormatTag = WAVE_FORMAT_PCM;
	wfex.cbSize = 0;

	// �o�b�t�@�̐ݒ�
	DSBUFFERDESC dsb_desc;
	ZeroMemory(&dsb_desc, sizeof(DSBUFFERDESC));
	dsb_desc.dwSize = sizeof(DSBUFFERDESC);
	dsb_desc.dwFlags = DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_GLOBALFOCUS | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLPAN | DSBCAPS_CTRLFREQUENCY;
	dsb_desc.dwBufferBytes = m_ds_buffer_size;
	dsb_desc.lpwfxFormat = &wfex;

	// �o�b�t�@�̍쐬
	hr = G_ds->CreateSoundBuffer(&dsb_desc, &m_ds_buffer, NULL);
	if (FAILED(hr))
		return false;

	// �Đ��͈͂�ݒ�
	m_play_start_pos = 0;
	m_play_end_pos = m_stream->get_data_size();
	m_play_restart_pos = 0;

	// �J�[�\�����o�b�t�@�̍ŏ��ɃZ�b�g
	m_ds_buffer->SetCurrentPosition(0);
	// �������݃u���b�N�ʒu���Z�b�g
	m_filled_block_cnt = 0;

	// �ŏ��̃u���b�N�𖄂߂�
	fill_buffer();

	// �X�e�[�g�̍X�V
	update_state();

	// �{�����[���̍X�V
	update_volume();

	return true;
}

// ****************************************************************
// ���
// ================================================================
bool C_player::release(bool CHECK_SEMAPHORE)
{
	{
		SEMAPHORE;	// �Z�}�t�H�`�F�b�N

		release_sound(false);
	}

	// �Z�}�t�H�폜
	if (m_h_semaphore)
	{
		CloseHandle(m_h_semaphore);
		m_h_semaphore = NULL;
	}

	return true;
}

// ****************************************************************
// �T�E���h�̉��
// ================================================================
bool C_player::release_sound(bool CHECK_SEMAPHORE)
{
	SEMAPHORE;	// �Z�}�t�H�`�F�b�N

	release_player();
	release_stream();

	return true;
}

// ****************************************************************
// �X�g���[���̉��
// ================================================================
void C_player::release_stream()
{
	m_stream.reset();
}

// ****************************************************************
// �v���C���[�̉��
// ================================================================
void C_player::release_player()
{
	// �o�b�t�@�̉��
	if (G_ds.is_ready() && m_ds_buffer)
	{
		m_ds_buffer->Release();
		m_ds_buffer = NULL;
	}

	// �v���C���[�Ɋւ���p�����[�^��������
	m_ds_buffer = NULL;
	m_ds_buffer_size = 0;
	m_block_size = 0;
	m_block_cnt = 0;
	m_filled_block_cnt = 0;
	m_play_pos_on_buffer_last = 0;
	m_play_pos_on_sound_last = 0;
	m_last_sample[0] = 0;
	m_last_sample[1] = 0;
	m_last_sample[2] = 0;
	m_last_sample[3] = 0;
	m_play_start_time = 0;

	m_playing = false;
	m_pause_flag = false;
	m_fade_in_flag = false;
	m_fade_in_start_time = 0;
	m_fade_in_time_len = 0;
	m_fade_in_start_value = 0;
	m_fade_in_cur_value = 0;
	m_fade_out_flag = false;
	m_fade_out_no_release = false;
	m_fade_out_start_time = 0;
	m_fade_out_time_len = 0;
	m_fade_out_start_value = 0;
	m_fade_out_cur_value = 0;
}

// ****************************************************************
// �X�g���[�����Z�b�g
// ================================================================
bool C_player::set_stream(BSP<C_sound_stream_base> stream, bool CHECK_SEMAPHORE)
{
	SEMAPHORE;	// �Z�}�t�H�`�F�b�N

	// �T�E���h�����
	release_player();
	release_stream();

	// �X�g���[���������ł��Ă��Ȃ������玸�s
	if (!stream || !stream->is_ready())	
		return false;

	// �X�g���[���̐ݒ�
	m_stream = stream;

	// �v���C���[�̍쐬
	if (!create_player())
		return false;

	return true;
}

// ****************************************************************
// �t���[������
// ================================================================
bool C_player::frame(bool CHECK_SEMAPHORE)
{
	SEMAPHORE;	// �Z�}�t�H�`�F�b�N

	// �Đ�����A�Ȃ��� GetCurrentPosition ���s���Ȓl��Ԃ����Ƃ�����܂��B
	// ���̂��߁A�Đ����J�n���Ă��� 200 �ؕb�̊Ԃ́A�o�b�t�@���X�V���܂���B
	if (::GetTickCount() - m_play_start_time >= 200)
	{
		// �o�b�t�@�𖄂߂�
		fill_buffer();

		// �X�e�[�^�X�X�V
		update_state();
	}

	// �{�����[���̍X�V
	update_volume();

	return true;
}

// ****************************************************************
// �o�b�t�@�𖄂߂�
// ================================================================
bool C_player::fill_buffer()
{
	if (!m_ds_buffer || m_ds_buffer_size == 0)
		return false;

	// �v���C�ʒu�̎擾�i�o�C�g�j
	DWORD play_pos_on_buffer;
	m_ds_buffer->GetCurrentPosition(&play_pos_on_buffer, NULL);

	// �v���C�ʒu���p�[�Z���g�œ���
	DWORD percent = play_pos_on_buffer * 100 / m_ds_buffer_size;
	
	// �v���C�ʒu���O�񖄂߂��u���b�N�ɓ�������A���̃u���b�N�𖄂߂�
	if (false);
	else if (m_filled_block_cnt == 0)	{
		block_copy(0);	// �P����ǂ�łȂ��ꍇ�͕K���ǂ�
	}
	else if (m_filled_block_cnt == 1)	{
		if (0 <= percent && percent < 50)	{
			block_copy(1);
		}
	}
	else if (m_filled_block_cnt == 2)	{
		if (50 <= percent && percent < 100)	{
			block_copy(0);
		}
	}

	return true;
}

// ****************************************************************
// �o�b�t�@�Ƀu���b�N�f�[�^���R�s�[
// ================================================================
bool C_player::block_copy(int block_no)
{
	HRESULT hr;

	if (!m_stream || !m_stream->is_ready())
		return false;

	// �Đ����I�����Ă���Ȃ��~�i�m�C�Y��h�����߂̗\�������j
	if (m_playing)	{
		if ((!m_loop && get_play_pos() >= m_play_end_pos)
			|| (m_fade_out_flag && (int)(::GetTickCount() - m_fade_out_start_time) > m_fade_out_time_len))	{

			release_player();	// �v���C���[�����
			//reset(false);		// ��~���Ċ����߂�
			return true;
		}
	}

	// �o�b�t�@�����b�N
	LPBYTE lp_block_add_1, lp_block_add_2;
	DWORD block_size_1, block_size_2;
	hr = m_ds_buffer->Lock(m_block_size * block_no, m_block_size, (LPVOID *)&lp_block_add_1, &block_size_1, (LPVOID *)&lp_block_add_2, &block_size_2, 0);
	if (FAILED(hr))
		return false;

	// �o�b�t�@�Ƀf�[�^���R�s�[
	block_copy(lp_block_add_1, block_size_1);	// �o�b�t�@�̑O���Ƀf�[�^���R�s�[
	block_copy(lp_block_add_2, block_size_2);	// �o�b�t�@�̌㔼�Ƀf�[�^���R�s�[

	// �o�b�t�@���A�����b�N
	m_ds_buffer->Unlock(lp_block_add_1, block_size_1, lp_block_add_2, block_size_2);

	// ���߂��u���b�N��
	m_filled_block_cnt = block_no + 1;

	return true;
}

// ****************************************************************
// �o�b�t�@�Ƀu���b�N�f�[�^���R�s�[�i�����p�j
// ================================================================
bool C_player::block_copy(BYTE* data_ptr, int copy_size)
{
	int offset = 0;		// �X�V�I�t�Z�b�g

	// �o�b�t�@�Ƀf�[�^���R�s�[
	for (offset = 0; offset < copy_size; )	{

		// �ǂݍ��ނׂ��T�C�Y���v�Z
		int read_pos = m_stream->get_read_pos();
		int to_read_size = std::min(copy_size - offset, m_play_end_pos - read_pos);
		// �T�E���h��ǂݍ���
		int read_size = m_stream->read(data_ptr + offset, to_read_size);

#if 0
		// �����H���Ă݂�
		int zure = 3000;	// short* �ł̃I�t�Z�b�g
		short* s = std::max((short *)(backup.get() + start_pos + offset), (short *)backup.get() + zure);
		for (short* p = s; p < (short *)(backup.get() + start_pos + offset + read_size); p++)	{
			int result = *p;
			result = result + (int)((double)(*(p - zure)) / 1.5);
			result = limit(-32000, result, 32000);
			*p = (short)result;
		}
#endif

		// �ǂݍ��݈ʒu�̈ړ�
		offset += read_size;
		// �Ō�ɓǂݍ��񂾉����o���Ă���
		if (read_size > 0)	{
			int byte_cnt = m_stream->get_bit_cnt_per_sample() / 8 * m_stream->get_channel_cnt();
			memcpy(m_last_sample, data_ptr + offset - byte_cnt, byte_cnt);
		}
		// �w�肵���T�C�Y�����ǂݍ��߂Ȃ������ꍇ
		if (copy_size != offset)	{
			// ���[�v�Đ��Ȃ�t�@�C���������߂�
			if (m_loop)	{
				if (m_play_restart_pos >= m_play_end_pos)	{
					break;		// �����߂��Ȃ��̂œǂݍ��ݏI��
				}
				else	{
					m_stream->set_read_pos(m_play_restart_pos);
				}
			}
			// ���[�v�Đ��łȂ��Ȃ�ǂݍ��ݏI��
			else	{
				break;
			}
		}
	}

	// �ǂݍ��񂾃T�C�Y�ƃR�s�[�����T�C�Y���Ⴄ�ꍇ
	if (!m_loop && copy_size != offset)	{
		// �c��̕������Ō�ɓǂݍ��񂾒l�Ŗ��߂�
		int byte_cnt = m_stream->get_bit_cnt_per_sample() / 8 * m_stream->get_channel_cnt();
		if (byte_cnt == 1)	fill_memory(data_ptr + offset, (copy_size - offset) / 1, *(BYTE *)m_last_sample);
		if (byte_cnt == 2)	fill_memory_2(data_ptr + offset, (copy_size - offset) / 2, *(WORD *)m_last_sample);
		if (byte_cnt == 4)	fill_memory_4(data_ptr + offset, (copy_size - offset) / 4, *(DWORD *)m_last_sample);
	}


	return true;
}

#if 0
// ****************************************************************
// �o�b�t�@�Ƀu���b�N�f�[�^���R�s�[�i�����p�j
// ================================================================
bool C_player::block_copy(BYTE* data_ptr, int copy_size)
{
	int offset = 0;		// �X�V�I�t�Z�b�g

	// �o�b�t�@�Ƀf�[�^���R�s�[
	for (offset = 0; offset < copy_size; )	{
		// �ǂݍ��ނׂ��T�C�Y���v�Z
		int read_pos = reader->get_read_pos();
		int to_read_size = std::min(copy_size - offset, play_end_pos - read_pos);
		// �T�E���h��ǂݍ���
		int read_size = reader->read(data_ptr + offset, to_read_size);

		// �ǂݍ��݈ʒu�̈ړ�
		offset += read_size;
		// �Ō�ɓǂݍ��񂾉����o���Ă���
		if (read_size > 0)	{
			int byte_cnt = reader->get_bit_cnt_per_sample() / 8 * reader->get_channel_cnt();
			memcpy(last_sample, data_ptr + offset - byte_cnt, byte_cnt);
		}
		// �w�肵���T�C�Y�����ǂݍ��߂Ȃ������ꍇ
		if (copy_size != offset)	{
			if (loop)	reader->set_read_pos(play_restart_pos);		// ���[�v�Đ��Ȃ�t�@�C���������߂�
			else		break;										// ���[�v�Đ��łȂ��Ȃ�ǂݍ��ݏI��
		}
	}

	// �ǂݍ��񂾃T�C�Y�ƃR�s�[�����T�C�Y���Ⴄ�ꍇ
	if (!loop && copy_size != offset)	{
		// �c��̕������Ō�ɓǂݍ��񂾒l�Ŗ��߂�
		int byte_cnt = reader->get_bit_cnt_per_sample() / 8 * reader->get_channel_cnt();
		if (byte_cnt == 1)	fill_memory(data_ptr + offset, (copy_size - offset) / 1, *(BYTE *)last_sample);
		if (byte_cnt == 2)	fill_memory_2(data_ptr + offset, (copy_size - offset) / 2, *(WORD *)last_sample);
		if (byte_cnt == 4)	fill_memory_4(data_ptr + offset, (copy_size - offset) / 4, *(DWORD *)last_sample);
	}

	return true;
}
#endif

// ****************************************************************
// �Đ�
// ================================================================
bool C_player::play(bool CHECK_SEMAPHORE)
{
	SEMAPHORE;	// �Z�}�t�H�`�F�b�N

	HRESULT hr;

	if (!m_stream || !m_ds_buffer)
		return false;

	// �͈̓`�F�b�N
	if (m_play_start_pos >= m_play_end_pos)	{
		if (m_loop)		m_play_start_pos = m_play_restart_pos;
		else			return false;
	}

	// ���t�J�n
	hr = m_ds_buffer->Play(0, 0, DSBPLAY_LOOPING);
	if (FAILED(hr))
		return false;

	// �t���O�n�m
	m_playing = true;
	m_play_start_time = ::GetTickCount();
	m_pause_flag = false;
	m_fade_in_flag = false;
	m_fade_in_start_time = 0;
	m_fade_in_time_len = 0;
	m_fade_in_start_value = 0;
	m_fade_in_cur_value = 0;

	return true;
}

// ****************************************************************
// �����̂�
// ================================================================
bool C_player::ready(bool CHECK_SEMAPHORE)
{
	SEMAPHORE;	// �Z�}�t�H�`�F�b�N

	HRESULT hr;

	if (!m_stream || !m_ds_buffer)
		return false;

	// ���t�J�n
	hr = m_ds_buffer->Play(0, 0, DSBPLAY_LOOPING);
	if (FAILED(hr))
		return false;

	// ���t��~
	hr = m_ds_buffer->Stop();
	if (FAILED(hr))
		return false;

	// �t���O�n�m
	m_playing = true;
	m_play_start_time = ::GetTickCount();
	m_pause_flag = true;
	m_fade_in_flag = false;
	m_fade_in_start_time = 0;
	m_fade_in_time_len = 0;
	m_fade_in_start_value = 0;
	m_fade_in_cur_value = 0;

	return true;
}

// ****************************************************************
// �Đ��i�t�F�[�h�C���j
// ================================================================
bool C_player::fade_in(int fade_time, bool CHECK_SEMAPHORE)
{
	SEMAPHORE;	// �Z�}�t�H�`�F�b�N

	HRESULT hr;

	if (!m_stream || !m_ds_buffer)
		return false;

	// ���t�J�n
	hr = m_ds_buffer->Play(0, 0, DSBPLAY_LOOPING);
	if (FAILED(hr))
		return false;

	// �t�F�[�h�A�E�g�͏I��������
	m_fade_out_flag = false;

	// �t���O�̐ݒ�
	m_playing = true;
	m_play_start_time = ::GetTickCount();
	m_pause_flag = false;
	m_fade_in_start_time = ::GetTickCount();
	m_fade_in_time_len = fade_time;
	m_fade_in_start_value = 0;	// �t�F�[�h�C���͕K���ŏ�����͂��߂�
	m_fade_in_flag = true;

	// �{�����[�����X�V�i�t�F�[�h�C���̂��߂ɕK�{�j
	update_volume();

	return true;
}

// ****************************************************************
// ��~
// ================================================================
bool C_player::stop(bool CHECK_SEMAPHORE)
{
	SEMAPHORE;	// �Z�}�t�H�`�F�b�N

	if (!set_play_pos(0))
		return false;

	return true;
}

// ****************************************************************
// �ꎞ��~
// ================================================================
bool C_player::pause(bool CHECK_SEMAPHORE)
{
	SEMAPHORE;	// �Z�}�t�H�`�F�b�N

	HRESULT hr;

	if (!m_stream || !m_ds_buffer)
		return false;

	// ���t��~
	hr = m_ds_buffer->Stop();
	if (FAILED(hr))
		return false;

	return true;
}

// ****************************************************************
// �t�F�[�h�A�E�g�i�I����Ɉꎞ��~�j
// ================================================================
bool C_player::fade_out_and_pause(int time_len, bool CHECK_SEMAPHORE)
{
	SEMAPHORE;	// �Z�}�t�H�`�F�b�N

	if (!m_stream || !m_ds_buffer)
		return false;

	// �t�F�[�h�A�E�g���̏ꍇ
	if (m_fade_out_flag)
	{
		// ��ɏI�����̃t�F�[�h�A�E�g��D�悷��
		int fade_out_end_time_old = m_fade_out_start_time + m_fade_out_time_len;
		int fade_out_end_time_new = ::GetTickCount() + time_len;
		if (fade_out_end_time_old - fade_out_end_time_new > 0)
		{
			// �t�F�[�h�A�E�g�J�n
			m_fade_out_flag = true;
			m_fade_out_no_release = true;
			m_fade_out_start_time = ::GetTickCount();
			m_fade_out_time_len = time_len;
			m_fade_out_start_value = m_fade_out_cur_value;
		}
	}

	// �t�F�[�h�A�E�g���łȂ��ꍇ
	else
	{
		// �t�F�[�h�A�E�g�J�n
		m_fade_out_flag = true;
		m_fade_out_no_release = true;
		m_fade_out_start_time = ::GetTickCount();
		m_fade_out_time_len = time_len;
		m_fade_out_start_value = VOLUME_MAX;
	}

	// �{�����[���X�V
	update_volume();

	return true;
}

// ****************************************************************
// �t�F�[�h�A�E�g�i�I����ɉ���j
// ================================================================
bool C_player::fade_out_and_release(int time_len, bool CHECK_SEMAPHORE)
{
	SEMAPHORE;	// �Z�}�t�H�`�F�b�N

	if (!m_stream || !m_ds_buffer)
		return false;

	// �t�F�[�h�A�E�g���̏ꍇ
	if (m_fade_out_flag)
	{
		// ��ɏI�����̃t�F�[�h�A�E�g��D�悷��
		int fade_out_end_time_old = m_fade_out_start_time + m_fade_out_time_len;
		int fade_out_end_time_new = ::GetTickCount() + time_len;
		if (fade_out_end_time_old - fade_out_end_time_new > 0)
		{
			// �t�F�[�h�A�E�g�J�n
			m_fade_out_flag = true;
			m_fade_out_no_release = false;
			m_fade_out_start_time = ::GetTickCount();
			m_fade_out_time_len = time_len;
			m_fade_out_start_value = m_fade_out_cur_value;
		}
	}

	// �t�F�[�h�A�E�g���łȂ��ꍇ
	else
	{
		// �t�F�[�h�A�E�g�J�n
		m_fade_out_flag = true;
		m_fade_out_no_release = false;
		m_fade_out_start_time = ::GetTickCount();
		m_fade_out_time_len = time_len;
		m_fade_out_start_value = VOLUME_MAX;
	}

	// �{�����[���X�V
	update_volume();

	return true;
}

// ****************************************************************
// �Đ��ʒu��ݒ�
// ================================================================
bool C_player::set_play_pos(int pos)
{
	HRESULT hr;

	if (!m_stream || !m_ds_buffer)
		return false;

	// ���t�J�n
	hr = m_ds_buffer->Stop();
	if (FAILED(hr))
		return false;

	// �ǂݍ��݈ʒu���ړ�
	m_stream->set_read_pos(pos);
	m_ds_buffer->SetCurrentPosition(0);
	m_filled_block_cnt = 0;
	m_play_pos_on_buffer_last = 0;
	m_play_pos_on_sound_last = pos;

	// �ŏ��̃u���b�N�𖄂߂�
	fill_buffer();

	// �t���O�n�e�e
	m_playing = false;

	return true;
}

void C_player::set_play_pos_by_sample(int pos_sample, bool CHECK_SEMAPHORE)
{
	SEMAPHORE;	// �Z�}�t�H�`�F�b�N

	if (!m_stream || !m_ds_buffer)
		return;

	set_play_pos(pos_sample * (m_stream->get_bit_cnt_per_sample() / 8) * m_stream->get_channel_cnt());
}

void C_player::set_play_pos_by_millsecond(int pos_mills, bool CHECK_SEMAPHORE)
{
	SEMAPHORE;	// �Z�}�t�H�`�F�b�N

	if (!m_stream || !m_ds_buffer)
		return;

	set_play_pos((__int64)pos_mills * m_stream->get_sample_cnt_per_sec() / 1000 * get_fragment_size(false));
}

// ****************************************************************
// �Đ��ʒu���擾
// ================================================================
int C_player::get_play_pos()
{
	if (!m_stream || !m_ds_buffer)
		return 0;

	return m_play_pos_on_sound_last;
}

int C_player::get_play_pos_by_sample(bool CHECK_SEMAPHORE)
{
	SEMAPHORE;	// �Z�}�t�H�`�F�b�N

	if (!m_stream || !m_ds_buffer)
		return 0;

	return get_play_pos() / get_fragment_size(false);
}

int C_player::get_play_pos_by_millsecond(bool CHECK_SEMAPHORE)
{
	SEMAPHORE;	// �Z�}�t�H�`�F�b�N

	if (!m_stream || !m_ds_buffer)
		return 0;

	return (int)((__int64)get_play_pos() / get_fragment_size(false) * 1000 / m_stream->get_sample_cnt_per_sec());
}

// ****************************************************************
// �Đ��͈͂̐ݒ�
// ================================================================
void C_player::set_range(int start_pos_sample, int end_pos_sample, int restart_pos_sample, bool CHECK_SEMAPHORE)
{
	SEMAPHORE;	// �Z�}�t�H�`�F�b�N

	if (!m_stream || !m_ds_buffer)
		return;

	// �͈͐���
#if 1
	end_pos_sample = end_pos_sample == SOUND_END_POS_AUTO ? get_sample_cnt(false) : limit(0, end_pos_sample, get_sample_cnt(false));
	start_pos_sample = limit(0, start_pos_sample, end_pos_sample);
	restart_pos_sample = limit(0, restart_pos_sample, end_pos_sample);
#else
	start_pos_sample = limit(0, start_pos_sample, get_sample_cnt(false));
	end_pos_sample = end_pos_sample == -1 ? get_sample_cnt(false) : limit(start_pos_sample + 1, end_pos_sample, get_sample_cnt(false));
	restart_pos_sample = limit(start_pos_sample, restart_pos_sample, end_pos_sample - 1);
#endif

	// �o�C�g���ɕϊ�
	m_play_start_pos = start_pos_sample * get_channel_cnt(false) * get_bit_cnt_per_sample(false) / 8;
	m_play_end_pos = end_pos_sample * get_channel_cnt(false) * get_bit_cnt_per_sample(false) / 8;
	m_play_restart_pos = restart_pos_sample * get_channel_cnt(false) * get_bit_cnt_per_sample(false) / 8;

	// �Đ��ʒu�̐ݒ�
	set_play_pos(m_play_start_pos);
}

// ****************************************************************
// ���[�v�̐ݒ�
// ================================================================
void C_player::set_loop(bool loop, bool CHECK_SEMAPHORE)
{
	SEMAPHORE;	// �Z�}�t�H�`�F�b�N

	m_loop = loop;
}

// ****************************************************************
// �{�����[����ݒ�
// ================================================================
void C_player::set_volume(int channel_no, int volume, int time, bool CHECK_SEMAPHORE)
{
	SEMAPHORE;	// �Z�}�t�H�`�F�b�N

	if (0 <= channel_no && channel_no < VOLUME_CHANNEL_CNT)	{

		// �͈͂𐧌�
		volume = limit<int>(VOLUME_MIN, volume, VOLUME_MAX);

		// �{�����[���t�F�[�h���~
		volume_channel[channel_no].fade_ing = false;

		// �{�����[���t�F�[�h��ݒ�
		if (time > 0)	{

			// �{�����[���t�F�[�h��ݒ�
			volume_channel[channel_no].fade_ing = true;
			volume_channel[channel_no].fade_start_time = ::GetTickCount();
			volume_channel[channel_no].fade_end_time = volume_channel[channel_no].fade_start_time + time;
			volume_channel[channel_no].fade_start_value = volume_channel[channel_no].volume;
			volume_channel[channel_no].fade_end_value = volume;
		}

		// �{�����[����ݒ�
		volume_channel[channel_no].volume = limit<int>(VOLUME_MIN, volume, VOLUME_MAX);

		// �{�����[���̍X�V
		update_volume();
	}
}

// ****************************************************************
// �{�����[�����擾
// ================================================================
int C_player::get_volume(int channel_no, bool CHECK_SEMAPHORE)
{
	SEMAPHORE;	// �Z�}�t�H�`�F�b�N

	if (0 <= channel_no && channel_no < VOLUME_CHANNEL_CNT)	{
		return volume_channel[channel_no].volume;
	}

	return 0;
}

// ****************************************************************
// ���v�{�����[�����擾
// ================================================================
int C_player::get_total_volume(bool CHECK_SEMAPHORE)
{
	SEMAPHORE;	// �Z�}�t�H�`�F�b�N

	int volume = 255;
	for (int channel_no = 0; channel_no < VOLUME_CHANNEL_CNT; channel_no++)	{
		volume = volume * volume_channel[channel_no].volume / 255;
	}

	return volume;
}

// ****************************************************************
// �X�e�[�g�̍X�V
// ================================================================
bool C_player::update_state()
{
	HRESULT hr;

	if (!m_stream || !m_ds_buffer)
		return false;

	// ================================================================
	// �v���C�ʒu
	// ----------------------------------------------------------------

	// �o�b�t�@��ł̃v���C�ʒu���擾�ibyte�j
	int play_pos_on_buffer, play_pos_on_sound;
	hr = m_ds_buffer->GetCurrentPosition((DWORD *)&play_pos_on_buffer, NULL);
	if (FAILED(hr))	{
		set_last_error(_T("GetCurrentPosition �Ɏ��s���܂����B"), _T("GetCurrentPosition"));
		return false;
	}

	// �o�b�t�@��ł̐i�񂾋����𒲂ׂ�
	int play_len_on_buffer = (play_pos_on_buffer - m_play_pos_on_buffer_last + m_ds_buffer_size) % m_ds_buffer_size;
	//if (play_len_on_buffer > (int)ds_buffer_size / 2)
	//	return false;	// �Đ��J�n������ƌ�A�Ƃ��ǂ� play_pos_on_buffer ����� 0 �ɖ߂�܂��B���������邽�߂̉������ł��B
	//if (play_pos_on_buffer == 0)
	//	return false;	// ���̑΍����@��ς��Ă݂܂����B�ʒu�� 0 �̂Ƃ��͖�������悤�ɂ��܂��B���퓮�쎞�A���܂��� 0 ���Ԃ��Ă��Ė�������Ă��A���̃t���[���� 0 �ȊO���Ԃ��Ă���Ǝv���̂Ŗ��Ȃ��ł��B

	// �T�E���h��ł̃v���C�ʒu���擾�ibyte�j
	play_pos_on_sound = m_play_pos_on_sound_last + play_len_on_buffer;

	// �v���C�ʒu���C��
	if ((int)play_pos_on_sound > m_play_end_pos)	{
		if (m_loop)	{
			play_pos_on_sound = (play_pos_on_sound - m_play_end_pos) % (m_play_end_pos - m_play_restart_pos) + m_play_restart_pos;
		}
		else	{
			// �v���C���[�����
			release_player();
			release_stream();
		}
	}

	// �v���C�ʒu��ޔ�
	m_play_pos_on_sound_last = play_pos_on_sound;
	m_play_pos_on_buffer_last = play_pos_on_buffer;

	return true;
}

// ****************************************************************
// �{�����[���̍X�V
// ================================================================
bool C_player::update_volume()
{
	if (!m_stream || !m_ds_buffer)
		return false;

	int total_volume = VOLUME_MAX;

	// �e�`�����l���̃{�����[����K�p����
	for (int channel_no = 0; channel_no < VOLUME_CHANNEL_CNT; channel_no++)	{
		S_volume_channel* channel = &volume_channel[channel_no];

		// �{�����[���t�F�[�h��
		if (channel->fade_ing)	{
			DWORD now_time = ::GetTickCount();
			total_volume = total_volume * linear_limit<int>(now_time, channel->fade_start_time, channel->fade_start_value, channel->fade_end_time, channel->fade_end_value) / VOLUME_MAX;
			if ((int)(now_time - channel->fade_end_time) >= 0)
				channel->fade_ing = false;
		}
		// �{�����[���t�F�[�h���łȂ�
		else	{
			total_volume = total_volume * channel->volume / VOLUME_MAX;
		}
	}

	// �t�F�[�h�C��
	if (m_fade_in_flag)	{
		DWORD now_time = ::GetTickCount();
		int past_time = now_time - m_fade_in_start_time;
		m_fade_in_cur_value = linear_limit<int, int>(past_time, 0, m_fade_in_start_value, m_fade_in_time_len, VOLUME_MAX);
		total_volume = linear_limit<int, int>(m_fade_in_cur_value, VOLUME_MIN, VOLUME_MIN, VOLUME_MAX, total_volume);
		if ((int)(now_time - (m_fade_in_start_time + m_fade_in_time_len)) >= 0)
			m_fade_in_flag = false;
	}

	// �t�F�[�h�A�E�g
	if (m_fade_out_flag)	{
		DWORD now_time = ::GetTickCount();
		int past_time = now_time - m_fade_out_start_time;
		m_fade_out_cur_value = linear_limit<int, int>(past_time, 0, m_fade_out_start_value, m_fade_out_time_len, VOLUME_MIN);
		total_volume = linear_limit<int, int>(m_fade_out_cur_value, 0, 0, VOLUME_MAX, total_volume);
		if ((int)(now_time - (m_fade_out_start_time + m_fade_out_time_len)) >= 0)	{
			m_fade_out_flag = false;

			if (m_fade_out_no_release)
			{
				pause(false);		// �t�F�[�h�A�E�g���I������̂ňꎞ��~
			}
			else
			{
				release_player();	// �t�F�[�h�A�E�g���I������̂Ńv���C���[�����
			}
		}
	}

	// �O�̂��ߕ␳
	total_volume = limit<int>(VOLUME_MIN, total_volume, VOLUME_MAX);

	// DirectSound �̃{�����[���ɕ␳
	int dsb_volume;
	if (total_volume == 0)	{
		dsb_volume = DSBVOLUME_MIN;		// log(0) ������邽�߂ɗ�O����
	}
	else	{
		dsb_volume = (int)(log((double)total_volume / (VOLUME_MAX - VOLUME_MIN)) / log(10.0f) * 5000);	// 5000 �Ƃ����l�ɐ[���Ӗ��͂Ȃ��ł��B���Œ��������o�B
		dsb_volume = limit(DSBVOLUME_MIN, dsb_volume, DSBVOLUME_MAX);
	}

	// �{�����[���̐ݒ�
	if (m_ds_buffer)
		m_ds_buffer->SetVolume(dsb_volume);

	return true;
}

// ****************************************************************
// �v���C�}�l�[�W��
// ================================================================
C_play_manager::C_play_manager()
{
	h_thread = NULL;
	thread_id = 0;
	stop_thread = false;

	// �Z�}�t�H�쐬
	h_list_semaphore = CreateSemaphore(NULL, 1, 1, NULL);
}

C_play_manager::~C_play_manager()
{
	// �Z�}�t�H�`�F�b�N
	WaitForSingleObject(h_list_semaphore, INFINITE);

	// �Z�}�t�H���
	ReleaseSemaphore(h_list_semaphore, 1, NULL);

	// �Z�}�t�H�폜
	if (h_list_semaphore)	{
		CloseHandle(h_list_semaphore);
		h_list_semaphore = NULL;
	}
}

// ****************************************************************
// ������
// ================================================================
bool C_play_manager::init()
{
	// �X�g���[�~���O�Ď��p�̃X���b�h���쐬
	h_thread = CreateThread(NULL, 0, thread_func, this, 0, &thread_id);
	if (!h_thread)
		return false;

	return true;
}

// ****************************************************************
// ���
// ================================================================
void C_play_manager::release()
{
	// �Z�}�t�H�`�F�b�N
	WaitForSingleObject(h_list_semaphore, INFINITE);

	// �v���C���[���X�g������
	player_list.clear();

	// �Z�}�t�H���
	ReleaseSemaphore(h_list_semaphore, 1, NULL);

	// �Z�}�t�H�폜
	if (h_list_semaphore)	{
		CloseHandle(h_list_semaphore);
		h_list_semaphore = NULL;
	}

	// �X���b�h���~����
	stop_thread = true;

	// �X���b�h�̒�~����������̂�҂�
	DWORD start = GetTickCount();
	while (stop_thread)	{
		DWORD now = GetTickCount();
		if (now > start + 2000)	// �Q�b���Ă�
			break;				// ������蔲����

		Sleep(100);
	}

	CloseHandle(h_thread);
	h_thread = NULL;
}

// ****************************************************************
// �v���C���[�̓o�^
// ================================================================
bool C_play_manager::regist_player(C_player* player)
{
	// �Z�}�t�H�`�F�b�N
	WaitForSingleObject(h_list_semaphore, INFINITE);

	// �w�肳�ꂽ�v�f���������A������Ȃ���Γo�^
	PLAYERLIST::iterator itr = std::find(player_list.begin(), player_list.end(), player);
	if (itr == player_list.end())
		player_list.push_back(player);

	// �Z�}�t�H���
	ReleaseSemaphore(h_list_semaphore, 1, NULL);

	return true;
}

// ****************************************************************
// �v���C���[�̓o�^����
// ================================================================
bool C_play_manager::delete_player(C_player* player)
{	
	// �Z�}�t�H�`�F�b�N
	WaitForSingleObject(h_list_semaphore, INFINITE);

	// �w�肳�ꂽ�v���C���[��S�č폜
	player_list.erase(std::remove(player_list.begin(), player_list.end(), player), player_list.end());

	// �Z�}�t�H���
	ReleaseSemaphore(h_list_semaphore, 1, NULL);

	return true;
}

// ****************************************************************
// �Ď��X���b�h
// ================================================================
DWORD WINAPI C_play_manager::thread_func(LPVOID pv_ref)
{
	C_play_manager* manager = (C_play_manager *)pv_ref;

	// �X���b�h�̃��C���v���Z�X�����I�ɌĂ�
	while (!manager->stop_thread)	{
		manager->thread_main();
		Sleep(20);
	}

	// �X���b�h�I��
	manager->stop_thread = false;
	ExitThread(true);

	return 0;
}

// ****************************************************************
// �X���b�h�Ď�
// ================================================================
void C_play_manager::thread_main()
{
	// �Z�}�t�H�`�F�b�N
	WaitForSingleObject(h_list_semaphore, INFINITE);

	// �S�Ẵv���C���[�ɂ��ăt���[������
	PLAYERLIST::iterator itr;
	for (itr = player_list.begin(); itr != player_list.end(); ++itr)
		(*itr)->frame();

	// �Z�}�t�H���
	ReleaseSemaphore(h_list_semaphore, 1, NULL);
}

}
