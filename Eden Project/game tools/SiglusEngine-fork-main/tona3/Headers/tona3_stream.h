#pragma		once

#include	"tona3_array.h"
#include	"tona3_string.h"

namespace NT3
{

// ****************************************************************
// �X�g���[�����N���X
// ================================================================
class C_stream_base
{
public:
	C_stream_base()	{	seek = 0;	}

	// ================================================================
	// �o�b�t�@����
	// ----------------------------------------------------------------
	void	clear()				{	buffer.clear();	seek = 0;	}
	bool	empty()				{	return buffer.empty();		}
	int		size()				{	return (int)buffer.size();	}
	void	resize(int size)	{	buffer.resize(size);		}

	// �V�[�N
	void	set_seek(int seek_ = 0)		{	seek = seek_;	}
	int		get_seek()					{	return seek;	}
	void	add_seek(int cnt)			{	seek += cnt;	}

	// �o�b�t�@�̎擾
	BUFFER& get_buffer()		{	return buffer;				}
	BUFFER::iterator begin()	{	return buffer.begin();		}
	BUFFER::iterator end()		{	return buffer.end();		}
	BYTE* get()					{	return buffer.get();		}
	BYTE* get(int n)			{	return buffer.get() + n;	}
	BYTE* get_end()				{	return buffer.get_end();	}
	BYTE* get_cur_ptr()			{	return buffer.get() + seek;	}
	BYTE& operator [] (int n)	{	return buffer[n];			}

protected:
	BUFFER			buffer;
	int				seek;
};

// ****************************************************************
// �X�g���[��
// ================================================================
class C_stream : public C_stream_base
{
public:

	// ================================================================
	// �f�[�^��ǉ�
	// ----------------------------------------------------------------

	// �f�[�^��ǉ�
	template <typename TYPE>
	void push(TYPE data)
	{
		resize(seek + sizeof(TYPE));
		memcpy(buffer.get() + seek, &data, sizeof(data));
		seek += sizeof(TYPE);
	}

	// �o�C�i���f�[�^��ǉ�
	void push(const void* data, int size)
	{
		if (size > 0)	{
			buffer.resize(seek + size);
			memcpy(buffer.get() + seek, data, size);
			seek += size;
		}
	}

	// ��f�[�^��ǉ�
	void push_dirty(int size)
	{
		if (size > 0)	{
			buffer.resize(seek + size);
			seek += size;
		}
	}

	// �z���ǉ�
	template <typename TYPE>
	void push_array(const TYPE* data, int cnt)
	{
		push((const void *)data, sizeof(TYPE) * cnt); 
	}

	// �������ǉ�
	template <typename TYPE>
	void push_str(const TYPE& str)
	{
		typedef TYPE::value_type CHAR_TYPE;
		int size = (int)str.size() * sizeof(CHAR_TYPE);	// �������ރT�C�Y
		push(str.c_str(), size);						// ��������
	}

	void push_str(const TCHAR* str)
	{
		push_str(TSTR(str));
	}

	// �������ǉ��i'\0' ��ǉ��j
	template <typename TYPE>
	void push_str_null(const TYPE& str)
	{
		push_str(str);					// ���������������
		push<TYPE::value_type>(0);		// '\0' ����������
	}

	// �������ǉ��i������ǉ��j
	template <typename TYPE>
	void push_str_len(const TYPE& str)
	{
		push<int>((int)str.size());		// ���������ŏ��ɐς�
		push_str(str);					// ���������������
	}

	// ================================================================
	// �f�[�^����������
	// ----------------------------------------------------------------
	void write(const void* data, int size)
	{
		if (size > 0)	{
			memcpy(buffer.get() + seek, data, size);
			seek += size;
		}
	}

	// ================================================================
	// �f�[�^���擾
	// ----------------------------------------------------------------

	// �f�[�^���擾�i�����Ŏ擾�j
	template <typename TYPE>
	void pop(TYPE& data)
	{
		data = *(TYPE *)(buffer.get() + seek);
		seek += sizeof(TYPE);
	}

	// �f�[�^���擾�i�߂�l�Ŏ擾�j
	template <typename TYPE>
	TYPE pop_ret()
	{
		seek += sizeof(TYPE);
		return *(TYPE *)(buffer.get() + seek - sizeof(TYPE));
	}

	// �o�C�i���f�[�^���擾
	void pop(void* buf, int size)
	{
		if (size > 0)	{
			memcpy(buf, buffer.get() + seek, size);
			seek += size;
		}
	}

	// �z����擾
	template <typename TYPE>
	void pop_array(TYPE* buf, int read_cnt)
	{
		pop((void *)buf, sizeof(TYPE) * read_cnt);
	}

	// ��������擾�i�����Ŏ擾�j
	template <typename TYPE>
	void pop_str_null(TYPE& str)
	{
		str = pop_str_null<TYPE>();
	}

	// ��������擾�i�߂�l�Ŏ擾�j
	template <typename TYPE>
	TYPE pop_str_null_ret()
	{
		typedef TYPE::value_type CHAR_TYPE;

		// '\0' ��T��
		int cnt = 0;
		CHAR_TYPE* p;
		for (p = (CHAR_TYPE *)(buffer.get() + seek); *p != _T('\0'); p++, cnt++);

		// ��������쐬
		TYPE str((CHAR_TYPE *)(buffer.get() + seek), p);
		seek += (cnt + 1) * sizeof(CHAR_TYPE);		// +1: '\0' �̕�

		return str;
	}

	// ��������擾�i�����Ŏ擾�j
	template <typename TYPE>
	void pop_str_len(TYPE& str)
	{
		str = pop_str_len_ret<TYPE>();
	}

	// ��������擾�i�߂�l�Ŏ擾�j
	template <typename TYPE>
	TYPE pop_str_len_ret()
	{
		typedef TYPE::value_type CHAR_TYPE;

		int size = pop_ret<int>() * sizeof(CHAR_TYPE);		// �f�[�^�����擾

		seek += size;
		return TYPE((CHAR_TYPE *)(buffer.get() + seek - size), (CHAR_TYPE *)(buffer.get() + seek));
	}
};

// ****************************************************************
// �X�^�b�N
// ================================================================
class Cstack : public C_stream_base
{
public:

	// �f�[�^��ǉ�
	template <typename TYPE>
	void push(TYPE data)
	{
		buffer.resize(seek + sizeof(TYPE));
		*(TYPE *)(buffer.get() + seek) = data;
		seek += sizeof(TYPE);
	}

	// �������ǉ�
	void push_str_len(CTSTR& str)
	{
		int size = (int)str.size() * sizeof(TCHAR);
		buffer.resize(seek + size);
		memcpy(buffer.get() + seek, str.c_str(), size);
		seek += (int)size;

		push((int)str.size());		// ���������Ō�ɐς�
	}

	// �o�C�i����������
	void push(const void* data, int size)
	{
		buffer.resize(seek + size);
		memcpy(buffer.get() + seek, data, size);
		seek += size;
	}

	// �f�[�^���擾�i�����Ɏ擾�j
	template <typename TYPE>
	void pop(TYPE* data)
	{
		seek -= sizeof(TYPE);
		*data = *(TYPE *)(buffer.get() + seek);
	}

	// �f�[�^���擾�i�߂�l�Ŏ擾�j
	template <typename TYPE>
	TYPE pop_ret()
	{
		seek -= sizeof(TYPE);
		return *(TYPE *)(buffer.get() + seek);
	}

	// ��������擾�i�����Ŏ擾�j
	void pop_str_len(TSTR& str)
	{
		int size = pop_ret<int>() * sizeof(TCHAR);		// �f�[�^�����擾

		seek -= size;
		str = TSTR((TCHAR *)(buffer.get() + seek), (TCHAR *)(buffer.get() + seek + size));
	}

	// ��������擾�i�߂�l�Ŏ擾�j
	TSTR pop_str_len_ret()
	{
		int size = pop_ret<int>() * sizeof(TCHAR);		// �f�[�^�����擾

		seek -= size;
		return TSTR((TCHAR *)(buffer.get() + seek), (TCHAR *)(buffer.get() + seek + size));
	}

	// �ŏ��̗v�f���擾
	template <typename TYPE>
	TYPE front()
	{
		return *(TYPE *)(buffer.get());
	}
	// �Ō�̗v�f���擾
	template <typename TYPE>
	TYPE back()
	{
		return *(TYPE *)(buffer.get() + seek - sizeof(TYPE));
	}
	// �Ō�̕�������擾
	TSTR back_str()
	{
		int size = back<int>() * sizeof(TCHAR);		// �f�[�^�����擾
		return TSTR((TCHAR *)(buffer.get() + seek - size - sizeof(int)), (TCHAR *)(buffer.get() + seek - sizeof(int)));
	}
};

}
