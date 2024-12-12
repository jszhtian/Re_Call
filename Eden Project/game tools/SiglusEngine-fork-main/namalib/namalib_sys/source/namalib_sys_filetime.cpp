#include	"pch.h"
#include	"namalib_sys_filetime.h"

namespace Nnama
{

// ****************************************************************
// ファイルタイムクリア
// ================================================================
void Cnamalib_filetime::clear_time()
{
	create_time.dwHighDateTime = 0;
	create_time.dwLowDateTime = 0;

	access_time.dwHighDateTime = 0;
	access_time.dwLowDateTime = 0;

	write_time.dwHighDateTime = 0;
	write_time.dwLowDateTime = 0;
}

// ****************************************************************
// ファイルタイム設定
// ================================================================
void Cnamalib_filetime::set_time(FILETIME* create_time_, FILETIME* access_time_, FILETIME* write_time_)
{
	if (create_time_)	{
		create_time.dwHighDateTime = create_time_->dwHighDateTime;
		create_time.dwLowDateTime = create_time_->dwLowDateTime;
	}

	if (access_time_)	{
		access_time.dwHighDateTime = access_time_->dwHighDateTime;
		access_time.dwLowDateTime = access_time_->dwLowDateTime;
	}

	if (write_time_)	{
		write_time.dwHighDateTime = write_time_->dwHighDateTime;
		write_time.dwLowDateTime = write_time_->dwLowDateTime;
	}
}

void Cnamalib_filetime::set_create_time(FILETIME* create_time_)
{
	create_time.dwHighDateTime = create_time_->dwHighDateTime;
	create_time.dwLowDateTime = create_time_->dwLowDateTime;
}

void Cnamalib_filetime::set_access_time(FILETIME* access_time_)
{
	access_time.dwHighDateTime = access_time_->dwHighDateTime;
	access_time.dwLowDateTime = access_time_->dwLowDateTime;
}

void Cnamalib_filetime::set_write_time(FILETIME* write_time_)
{
	write_time.dwHighDateTime = write_time_->dwHighDateTime;
	write_time.dwLowDateTime = write_time_->dwLowDateTime;
}

void Cnamalib_filetime::set_time(Cnamalib_filetime* ft)
{
	create_time.dwHighDateTime = ft->create_time.dwHighDateTime;
	create_time.dwLowDateTime = ft->create_time.dwLowDateTime;

	access_time.dwHighDateTime = ft->access_time.dwHighDateTime;
	access_time.dwLowDateTime = ft->access_time.dwLowDateTime;

	write_time.dwHighDateTime = ft->write_time.dwHighDateTime;
	write_time.dwLowDateTime = ft->write_time.dwLowDateTime;
}

void Cnamalib_filetime::set_create_time(Cnamalib_filetime* ft)
{
	create_time.dwHighDateTime = ft->create_time.dwHighDateTime;
	create_time.dwLowDateTime = ft->create_time.dwLowDateTime;
}

void Cnamalib_filetime::set_access_time(Cnamalib_filetime* ft)
{
	access_time.dwHighDateTime = ft->access_time.dwHighDateTime;
	access_time.dwLowDateTime = ft->access_time.dwLowDateTime;
}

void Cnamalib_filetime::set_write_time(Cnamalib_filetime* ft)
{
	write_time.dwHighDateTime = ft->write_time.dwHighDateTime;
	write_time.dwLowDateTime = ft->write_time.dwLowDateTime;
}

// ****************************************************************
// ファイルからファイルタイム取得
// ================================================================
bool Cnamalib_filetime::get_file_time(CTSTR& file_path)
{
	clear_time();
	return ::file_get_time(file_path, &create_time, &access_time, &write_time);
}

// ****************************************************************
// ファイルのファイルタイム設定
// ================================================================
bool Cnamalib_filetime::set_file_time(CTSTR& file_path)
{
	return ::file_set_time(file_path, &create_time, &access_time, &write_time);
}

bool Cnamalib_filetime::set_file_write_time(CTSTR& file_path)
{
	return ::file_set_time(file_path, NULL, NULL, &write_time);
}

// ****************************************************************
// ファイルタイム比較（ret : true=違う false=同じ）
// ================================================================
bool Cnamalib_filetime::comp_write_time(FILETIME* ft)
{
	if (write_time.dwHighDateTime != ft->dwHighDateTime || write_time.dwLowDateTime != ft->dwLowDateTime)	{
		return false;
	}
	return true;
}

bool Cnamalib_filetime::comp_write_time(Cnamalib_filetime* ft)
{
	if (write_time.dwHighDateTime != ft->write_time.dwHighDateTime || write_time.dwLowDateTime != ft->write_time.dwLowDateTime)	{
		return false;
	}
	return true;
}

}