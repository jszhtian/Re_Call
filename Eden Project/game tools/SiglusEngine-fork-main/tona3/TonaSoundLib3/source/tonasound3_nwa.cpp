#include	"tonasound3_pch.h"
#include	"tonasound3_nwa.h"

#include	<nwa.h>

namespace NT3
{

// ****************************************************************
// C_nwa_stream �����\����
// ================================================================
struct C_nwa_stream::Simpl
{
	// nwa ���
	Cnwa_unpack		nwa;		// nwa ���
};

// ****************************************************************
// nwa
// ================================================================
C_nwa_stream::C_nwa_stream()
{
	// �����̍쐬
	impl = BSP<Simpl>(new Simpl);
}

C_nwa_stream::~C_nwa_stream()
{
	close();

	// �����̔j��
	impl.reset();
}

// ****************************************************************
// nwa ���J��
// ================================================================
bool C_nwa_stream::open(CTSTR& file_path, DWORD offset, DWORD size)
{
	// nwa �����
	close();

	// nwa ���J��
	if (!impl->nwa.open((TCHAR *)file_path.c_str(), offset))
		return false;

	// wave �����Z�b�g
	set_param(impl->nwa.get_info_channels(), impl->nwa.get_info_bits_per_sample(), impl->nwa.get_info_samples_per_sec(), impl->nwa.get_info_original_size());

	return true;
}

// ****************************************************************
// nwa �����
// ================================================================
void C_nwa_stream::close()
{
	// nwa �����
	impl->nwa.close();

	// ���̏�����
	C_sound_stream_base::init();
}

// ****************************************************************
// nwa �̓ǂݍ���
// ================================================================
int C_nwa_stream::read(BYTE* buf, int to_read_size)
{
	if (buf == NULL)
		return 0;

	// nwa �ł͓ǂݍ��ރT�C�Y�̓T���v�����Ŏw��
	return impl->nwa.read_sample(buf, to_read_size / get_fragment_size()) * get_fragment_size();
}


// ****************************************************************
// �ǂݍ��݈ʒu��ݒ�
// ================================================================
bool C_nwa_stream::set_read_pos(int pos)
{
	// nwa �ł͈ʒu�̓T���v�����Ŏw��
	impl->nwa.set_read_sample_pos(pos / get_fragment_size());
	return true;
}

// ****************************************************************
// �ǂݍ��݈ʒu���擾
// ================================================================
int C_nwa_stream::get_read_pos()
{
	// nwa �ł͈ʒu�̓T���v�����Ŏ擾
	return impl->nwa.get_read_sample_pos() * get_fragment_size();
}

}
