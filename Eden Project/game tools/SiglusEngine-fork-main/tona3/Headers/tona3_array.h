#pragma		once

namespace NT3
{

// ****************************************************************
// ���ϒ��z�� ARRAY
// ================================================================
//		std::vector ���p���B
//		�F�X�Ƃ�₱�������Ƃ����Ă܂����Astd::vector �Ƃ̈Ⴂ�́A
//		get() ���\�b�h�Ńo�b�t�@�ւ̃|�C���^���擾�ł��邱�Ƃ����ł��B
// ================================================================
//		ex. ARRAY<char> buf(256);	// �T�C�Y 256 �� char �^�z��
//		ex. HogeAPI(buf.get());		// WinAPI �Ȃǂ� char * ��n��
// ================================================================
#define	ARRAY	vector_ex
#define	BUFFER	ARRAY<BYTE>

template <typename data_type, class allocator_type = std::allocator<data_type> >
class vector_ex : public std::vector<data_type, allocator_type>
{
public:
	// ������₷�����邽�߁A�^���`���Ă����܂��B
	typedef std::vector<data_type, allocator_type>	base_type;		// ���̌^
	typedef vector_ex<data_type, allocator_type>	this_type;		// ���g�̌^

	// �R���X�g���N�^
	// ���N���X�̃R���X�g���N�^�S�Ă��I�[�o�[���C�h����K�v������܂��B
	// �A���P�[�^���w��ł���R���X�g���N�^�̓I�[�o�[���C�h���Ă��܂���B�K�v�ɂȂ�������܂��B
	vector_ex()	: vector()	{}
	explicit vector_ex(size_type _Count) : vector(_Count)	{}
		// �������P�̃R���X�g���N�^�́A�ϊ��R���X�g���N�^�Ƃ��ē����A�Öق̌^�ϊ��������Ă��܂��܂��B
		// ���������邽�߂Ɏg���Ă���̂� explicit �ł��B
	vector_ex(size_type _Count, const data_type& _Val) : vector(_Count, _Val)	{}
	vector_ex(const this_type& _Right) : vector(_Right)	{}
	vector_ex(size_type _Count, const data_type& _Val, const allocator_type& _Al) : vector(_Count, _Val, _Al)	{}

	// �C�e���[�^����̃R���X�g���N�^
	template<class _Iter> vector_ex(_Iter _First, _Iter _Last) : vector(_First, _Last)	{}

	// �o�b�t�@�ւ̃|�C���^�̎擾 get()
	// vector �̃o�b�t�@�͘A�����Ă��邱�Ƃ��ۏ؂���Ă��܂��B
	// �o�b�t�@����̎��� NULL ��Ԃ��܂��B
	data_type* get()	{
		return empty() ? NULL : &base_type::operator[](0);
	}
	const data_type* get() const	{
		return empty() ? NULL : &base_type::operator[](0);
	}
	data_type* get(int pos)	{
		return pos >= size() ? NULL : &base_type::operator[](pos);
	}
	const data_type* get(int pos) const	{
		return pos >= size() ? NULL : &base_type::operator[](pos);
	}
	data_type* get_end()	{
		return empty() ? NULL : &base_type::operator[](0) + size();
	}
	const data_type* get_end() const	{
		return empty() ? NULL : &base_type::operator[](0) + size();
	}
};


}
