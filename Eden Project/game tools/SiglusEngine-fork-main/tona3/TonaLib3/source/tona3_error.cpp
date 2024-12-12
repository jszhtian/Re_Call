#include	"tona3_pch.h"
#include	"tona3_array.h"
#include	"tona3_string.h"
#include	"tona3_error.h"

// ****************************************************************
// ���ƂȂR�F�G���[
// ================================================================
namespace NT3
{

// ****************************************************************
// CRT�iC RunTime�j�G���[
// ================================================================

// CRT �G���[���b�Z�[�W�̍ő咷
const int CRT_ERROR_LEN_MAX = 128;

// CRT �G���[���\����
struct S_crt_error
{
	TCHAR*	name;
	TCHAR*	detail;
};

// CRT �G���[���
S_crt_error G_crt_error[] = 
{
	{_T("0"), _T("�G���[�ڍׂ��擾�ł��܂���ł����B")},
	{_T("EPERM"), _T("���삪���ۂ���܂����B")},
	{_T("ENOENT"), _T("�t�@�C���܂��̓f�B���N�g����������܂���ł����B")},
	{_T("ESRCH"), _T("�v���Z�X��������܂���ł����B")},
	{_T("EINTR"), _T("Interrupted function")},
	{_T("EIO"), _T("I/O �G���[�ł��B")},
	{_T("ENXIO"), _T("�f�o�C�X�܂��̓A�h���X��������܂���ł����B")},
	{_T("E2BIG"), _T("�������X�g���������܂��B")},
	{_T("ENOEXEC"), _T("Exec format error")},
	{_T("EBADF"), _T("�t�@�C���w��q���s���ł��B")},
	{_T("ECHILD"), _T("No spawned processes")},
	{_T("EAGAIN"), _T("No more processes or not enough memory or maximum nesting level reached")},
	{_T("ENOMEM"), _T("Not enough memory")},
	{_T("EACCES"), _T("�A�N�Z�X�����ۂ���܂����B")},
	{_T("EFAULT"), _T("Bad address")},
	{_T("No.15"), _T("No.15")},
	{_T("EBUSY"), _T("Device or resource busy")},
	{_T("EEXIST"), _T("File exists")},
	{_T("EXDEV"), _T("Cross-device link")},
	{_T("ENODEV"), _T("No such device")},
	{_T("ENOTDIR"), _T("Not a directory")},
	{_T("EISDIR"), _T("Is a directory")},
	{_T("EINVAL"), _T("�����ȃp�����[�^�ł��B")},
	{_T("ENFILE"), _T("Too many files open in system")},
	{_T("EMFILE"), _T("Too many open files")},
	{_T("ENOTTY"), _T("Inappropriate I/O control operation")},
	{_T("No.26"), _T("No.26")},
	{_T("EFBIG"), _T("File too large")},
	{_T("ENOSPC"), _T("No space left on device")},
	{_T("ESPIPE"), _T("Invalid seek")},
	{_T("EROFS"), _T("Read-only file system")},
	{_T("EMLINK"), _T("Too many links")},
	{_T("EPIPE"), _T("Broken pipe")},
	{_T("EDOM"), _T("Math argument")},
	{_T("ERANGE"), _T("Result too large")},
	{_T("No.35"), _T("No.35")},
	{_T("EDEADLK"), _T("Resource deadlock would occur")},
	{_T("No.37"), _T("No.37")},
	{_T("ENAMETOOLONG"), _T("Filename too long")},
	{_T("ENOLCK"), _T("No locks available")},
	{_T("ENOSYS"), _T("Function not supported")},
	{_T("ENOTEMPTY"), _T("Directory not empty")},
	{_T("EILSEQ"), _T("Illegal byte sequence")},
	{_T("No.43"), _T("No.43")},
	{_T("No.44"), _T("No.44")},
	{_T("No.45"), _T("No.45")},
	{_T("No.46"), _T("No.46")},
	{_T("No.47"), _T("No.47")},
	{_T("No.48"), _T("No.48")},
	{_T("No.49"), _T("No.49")},
	{_T("No.50"), _T("No.50")},
	{_T("No.51"), _T("No.51")},
	{_T("No.52"), _T("No.52")},
	{_T("No.53"), _T("No.53")},
	{_T("No.54"), _T("No.54")},
	{_T("No.55"), _T("No.55")},
	{_T("No.56"), _T("No.56")},
	{_T("No.57"), _T("No.57")},
	{_T("No.58"), _T("No.58")},
	{_T("No.59"), _T("No.59")},
	{_T("No.60"), _T("No.60")},
	{_T("No.61"), _T("No.61")},
	{_T("No.62"), _T("No.62")},
	{_T("No.63"), _T("No.63")},
	{_T("No.64"), _T("No.64")},
	{_T("No.65"), _T("No.65")},
	{_T("No.66"), _T("No.66")},
	{_T("No.67"), _T("No.67")},
	{_T("No.68"), _T("No.68")},
	{_T("No.69"), _T("No.69")},
	{_T("No.70"), _T("No.70")},
	{_T("No.71"), _T("No.71")},
	{_T("No.72"), _T("No.72")},
	{_T("No.73"), _T("No.73")},
	{_T("No.74"), _T("No.74")},
	{_T("No.75"), _T("No.75")},
	{_T("No.76"), _T("No.76")},
	{_T("No.77"), _T("No.77")},
	{_T("No.78"), _T("No.78")},
	{_T("No.79"), _T("No.79")},
	{_T("STRUNCATE"), _T("String was truncated")},
};

// �G���[�ԍ����� CRT �G���[�̃��b�Z�[�W���擾����
TSTR get_crt_error_msg(int error_no)
{
	TSTR error_str;

	if (0 <= error_no && error_no < sizeof(G_crt_error) / sizeof(G_crt_error[0]))	{
		error_str = G_crt_error[error_no].detail;
	}
	else	{

		// �p�ӂ��ĂȂ������̂ŃG���[���b�Z�[�W���擾����
		TCHAR error_str_buf[CRT_ERROR_LEN_MAX];
		_wcserror_s(error_str_buf, CRT_ERROR_LEN_MAX, error_no);
		error_str = error_str_buf;
	}

	return TSTR(error_str);
}

// �Ō�̃G���[���� CRT �G���[�̃G���[�ԍ����擾����
int get_crt_last_error_no()
{
	int error_no;
	_get_errno(&error_no);
	return error_no;
}

// �Ō�̃G���[���� CRT �G���[�̃G���[���b�Z�[�W���擾����
TSTR get_crt_last_error_msg()
{
	int error_no;
	_get_errno(&error_no);

	return get_crt_error_msg(error_no);
}

// ****************************************************************
// Windows API �G���[
// ================================================================

// �G���[�ԍ����� Windows API �G���[�̃��b�Z�[�W���擾����
TSTR get_win_error_msg(DWORD error_code)
{
	TSTR msg;
	LPTSTR msg_buf = NULL;

	// �����Ń��������m�ۂ���܂��I
	DWORD res = FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		error_code,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&msg_buf,
		0, 
		NULL);

	if (res > 0 && msg_buf)	{

		// ���b�Z�[�W�̕ϊ�
		msg = msg_buf;

		// �����Y�ꂸ�ɁI
		LocalFree(msg_buf);

		// �����̉��s�����
		if (msg.size() >= 2 && msg[msg.size() - 1] == _T('\n') && msg[msg.size() - 2] == _T('\r'))
			msg = msg.substr(0, msg.size() - 2);
	}

	return msg;
}

// �Ō�̃G���[���� Windows �̃G���[�ԍ����擾����
int get_win_last_error_no()
{
	return (int)GetLastError();
}

// �Ō�̃G���[���� Windows �̃G���[���b�Z�[�W���擾����
TSTR get_win_last_error_msg()
{
	return get_win_error_msg(get_win_last_error_no());
}

// ****************************************************************
// HRESULT �G���[
// ================================================================

// �G���[�ԍ����� HRESULT �G���[�̃��b�Z�[�W���擾����
TSTR get_hresult_error_msg(HRESULT hr)
{
	switch (hr)	{

		// winerror.h
		case S_OK:						return _T("S_OK");
		case S_FALSE:					return _T("S_FALSE");

		case E_UNEXPECTED:				return _T("E_UNEXPECTED");
		case E_NOTIMPL:					return _T("E_NOTIMPL");
		case E_OUTOFMEMORY:				return _T("E_OUTOFMEMORY");
		case E_INVALIDARG:				return _T("E_INVALIDARG");
		case E_NOINTERFACE:				return _T("E_NOINTERFACE");
		case E_POINTER:					return _T("E_POINTER");
		case E_HANDLE:					return _T("E_HANDLE");
		case E_ABORT:					return _T("E_ABORT");
		case E_FAIL:					return _T("E_FAIL");
		case E_ACCESSDENIED:			return _T("E_ACCESSDENIED");
	
		case REGDB_E_CLASSNOTREG:		return _T("REGDB_E_CLASSNOTREG");
		case CLASS_E_NOAGGREGATION:		return _T("CLASS_E_NOAGGREGATION");

		default:	return _T("???");
	}
}

// ****************************************************************
// WBEM �G���[
// ================================================================

// WBEM �̃G���[���b�Z�[�W���擾����
TSTR get_wbem_error_msg(HRESULT hr)
{
	switch (hr)	{

		// wbemcli.h
		case WBEM_E_FAILED:							return _T("WBEM_E_FAILED");
		case WBEM_E_NOT_FOUND:						return _T("WBEM_E_NOT_FOUND");
		case WBEM_E_ACCESS_DENIED:					return _T("WBEM_E_ACCESS_DENIED");
		case WBEM_E_PROVIDER_FAILURE:				return _T("WBEM_E_PROVIDER_FAILURE");
		case WBEM_E_TYPE_MISMATCH:					return _T("WBEM_E_TYPE_MISMATCH");
		case WBEM_E_OUT_OF_MEMORY:					return _T("WBEM_E_OUT_OF_MEMORY");
		case WBEM_E_INVALID_CONTEXT:				return _T("WBEM_E_INVALID_CONTEXT");
		case WBEM_E_INVALID_PARAMETER:				return _T("WBEM_E_INVALID_PARAMETER");
		case WBEM_E_NOT_AVAILABLE:					return _T("WBEM_E_NOT_AVAILABLE");
		case WBEM_E_CRITICAL_ERROR:					return _T("WBEM_E_CRITICAL_ERROR");
		case WBEM_E_INVALID_STREAM:					return _T("WBEM_E_INVALID_STREAM");
		case WBEM_E_NOT_SUPPORTED:					return _T("WBEM_E_NOT_SUPPORTED");
		case WBEM_E_INVALID_SUPERCLASS:				return _T("WBEM_E_INVALID_SUPERCLASS");
		case WBEM_E_INVALID_NAMESPACE:				return _T("WBEM_E_INVALID_NAMESPACE");
		case WBEM_E_INVALID_OBJECT:					return _T("WBEM_E_INVALID_OBJECT");
		case WBEM_E_INVALID_CLASS:					return _T("WBEM_E_INVALID_CLASS");
		case WBEM_E_PROVIDER_NOT_FOUND:				return _T("WBEM_E_PROVIDER_NOT_FOUND");
		case WBEM_E_INVALID_PROVIDER_REGISTRATION:	return _T("WBEM_E_INVALID_PROVIDER_REGISTRATION");
		case WBEM_E_PROVIDER_LOAD_FAILURE:			return _T("WBEM_E_PROVIDER_LOAD_FAILURE");
		case WBEM_E_INITIALIZATION_FAILURE:			return _T("WBEM_E_INITIALIZATION_FAILURE");
		case WBEM_E_TRANSPORT_FAILURE:				return _T("WBEM_E_TRANSPORT_FAILURE");
		case WBEM_E_INVALID_OPERATION:				return _T("WBEM_E_INVALID_OPERATION");
		case WBEM_E_INVALID_QUERY:					return _T("WBEM_E_INVALID_QUERY");
		case WBEM_E_INVALID_QUERY_TYPE:				return _T("WBEM_E_INVALID_QUERY_TYPE");
		case WBEM_E_ALREADY_EXISTS:					return _T("WBEM_E_ALREADY_EXISTS");
		case WBEM_E_OVERRIDE_NOT_ALLOWED:			return _T("WBEM_E_OVERRIDE_NOT_ALLOWED");
		case WBEM_E_PROPAGATED_QUALIFIER:			return _T("WBEM_E_PROPAGATED_QUALIFIER");
		case WBEM_E_PROPAGATED_PROPERTY:			return _T("WBEM_E_PROPAGATED_PROPERTY");
		case WBEM_E_UNEXPECTED:						return _T("WBEM_E_UNEXPECTED");
		case WBEM_E_ILLEGAL_OPERATION:				return _T("WBEM_E_ILLEGAL_OPERATION");
		case WBEM_E_CANNOT_BE_KEY:					return _T("WBEM_E_CANNOT_BE_KEY");
		case WBEM_E_INCOMPLETE_CLASS:				return _T("WBEM_E_INCOMPLETE_CLASS");
		case WBEM_E_INVALID_SYNTAX:					return _T("WBEM_E_INVALID_SYNTAX");
		case WBEM_E_NONDECORATED_OBJECT:			return _T("WBEM_E_NONDECORATED_OBJECT");
		case WBEM_E_READ_ONLY:						return _T("WBEM_E_READ_ONLY");
		case WBEM_E_PROVIDER_NOT_CAPABLE:			return _T("WBEM_E_PROVIDER_NOT_CAPABLE");
		case WBEM_E_CLASS_HAS_CHILDREN:				return _T("WBEM_E_CLASS_HAS_CHILDREN");
		case WBEM_E_CLASS_HAS_INSTANCES:			return _T("WBEM_E_CLASS_HAS_INSTANCES");
		case WBEM_E_QUERY_NOT_IMPLEMENTED:			return _T("WBEM_E_QUERY_NOT_IMPLEMENTED");
		case WBEM_E_ILLEGAL_NULL:					return _T("WBEM_E_ILLEGAL_NULL");
		case WBEM_E_INVALID_QUALIFIER_TYPE:			return _T("WBEM_E_INVALID_QUALIFIER_TYPE");
		case WBEM_E_INVALID_PROPERTY_TYPE:			return _T("WBEM_E_INVALID_PROPERTY_TYPE");
		case WBEM_E_VALUE_OUT_OF_RANGE:				return _T("WBEM_E_VALUE_OUT_OF_RANGE");
		case WBEM_E_CANNOT_BE_SINGLETON:			return _T("WBEM_E_CANNOT_BE_SINGLETON");
		case WBEM_E_INVALID_CIM_TYPE:				return _T("WBEM_E_INVALID_CIM_TYPE");
		case WBEM_E_INVALID_METHOD:					return _T("WBEM_E_INVALID_METHOD");
		case WBEM_E_INVALID_METHOD_PARAMETERS:		return _T("WBEM_E_INVALID_METHOD_PARAMETERS");
		case WBEM_E_SYSTEM_PROPERTY:				return _T("WBEM_E_SYSTEM_PROPERTY");
		case WBEM_E_INVALID_PROPERTY:				return _T("WBEM_E_INVALID_PROPERTY");
		case WBEM_E_CALL_CANCELLED:					return _T("WBEM_E_CALL_CANCELLED");
		case WBEM_E_SHUTTING_DOWN:					return _T("WBEM_E_SHUTTING_DOWN");
		case WBEM_E_PROPAGATED_METHOD:				return _T("WBEM_E_PROPAGATED_METHOD");
		case WBEM_E_UNSUPPORTED_PARAMETER:			return _T("WBEM_E_UNSUPPORTED_PARAMETER");
		case WBEM_E_MISSING_PARAMETER_ID:			return _T("WBEM_E_MISSING_PARAMETER_ID");
		case WBEM_E_INVALID_PARAMETER_ID:			return _T("WBEM_E_INVALID_PARAMETER_ID");
		case WBEM_E_NONCONSECUTIVE_PARAMETER_IDS:	return _T("WBEM_E_NONCONSECUTIVE_PARAMETER_IDS");
		case WBEM_E_PARAMETER_ID_ON_RETVAL:			return _T("WBEM_E_PARAMETER_ID_ON_RETVAL");
		case WBEM_E_INVALID_OBJECT_PATH:			return _T("WBEM_E_INVALID_OBJECT_PATH");
		case WBEM_E_OUT_OF_DISK_SPACE:				return _T("WBEM_E_OUT_OF_DISK_SPACE");
		case WBEM_E_BUFFER_TOO_SMALL:				return _T("WBEM_E_BUFFER_TOO_SMALL");
		case WBEM_E_UNSUPPORTED_PUT_EXTENSION:		return _T("WBEM_E_UNSUPPORTED_PUT_EXTENSION");
		case WBEM_E_UNKNOWN_OBJECT_TYPE:			return _T("WBEM_E_UNKNOWN_OBJECT_TYPE");
		case WBEM_E_UNKNOWN_PACKET_TYPE:			return _T("WBEM_E_UNKNOWN_PACKET_TYPE");
		case WBEM_E_MARSHAL_VERSION_MISMATCH:		return _T("WBEM_E_MARSHAL_VERSION_MISMATCH");
		case WBEM_E_MARSHAL_INVALID_SIGNATURE:		return _T("WBEM_E_MARSHAL_INVALID_SIGNATURE");
		case WBEM_E_INVALID_QUALIFIER:				return _T("WBEM_E_INVALID_QUALIFIER");
		case WBEM_E_INVALID_DUPLICATE_PARAMETER:	return _T("WBEM_E_INVALID_DUPLICATE_PARAMETER");
		case WBEM_E_TOO_MUCH_DATA:					return _T("WBEM_E_TOO_MUCH_DATA");
		case WBEM_E_SERVER_TOO_BUSY:				return _T("WBEM_E_SERVER_TOO_BUSY");
		case WBEM_E_INVALID_FLAVOR:					return _T("WBEM_E_INVALID_FLAVOR");
		case WBEM_E_CIRCULAR_REFERENCE:				return _T("WBEM_E_CIRCULAR_REFERENCE");
		case WBEM_E_UNSUPPORTED_CLASS_UPDATE:		return _T("WBEM_E_UNSUPPORTED_CLASS_UPDATE");
		case WBEM_E_CANNOT_CHANGE_KEY_INHERITANCE:	return _T("WBEM_E_CANNOT_CHANGE_KEY_INHERITANCE");
		case WBEM_E_CANNOT_CHANGE_INDEX_INHERITANCE:return _T("WBEM_E_CANNOT_CHANGE_INDEX_INHERITANCE");
		case WBEM_E_TOO_MANY_PROPERTIES:			return _T("WBEM_E_TOO_MANY_PROPERTIES");
		case WBEM_E_UPDATE_TYPE_MISMATCH:			return _T("WBEM_E_UPDATE_TYPE_MISMATCH");
		case WBEM_E_UPDATE_OVERRIDE_NOT_ALLOWED:	return _T("WBEM_E_UPDATE_OVERRIDE_NOT_ALLOWED");
		case WBEM_E_UPDATE_PROPAGATED_METHOD:		return _T("WBEM_E_UPDATE_PROPAGATED_METHOD");
		case WBEM_E_METHOD_NOT_IMPLEMENTED:			return _T("WBEM_E_METHOD_NOT_IMPLEMENTED");
		case WBEM_E_METHOD_DISABLED:				return _T("WBEM_E_METHOD_DISABLED");
		case WBEM_E_REFRESHER_BUSY:					return _T("WBEM_E_REFRESHER_BUSY");
		case WBEM_E_UNPARSABLE_QUERY:				return _T("WBEM_E_UNPARSABLE_QUERY");
		case WBEM_E_NOT_EVENT_CLASS:				return _T("WBEM_E_NOT_EVENT_CLASS");
		case WBEM_E_MISSING_GROUP_WITHIN:			return _T("WBEM_E_MISSING_GROUP_WITHIN");
		case WBEM_E_MISSING_AGGREGATION_LIST:		return _T("WBEM_E_MISSING_AGGREGATION_LIST");
		case WBEM_E_PROPERTY_NOT_AN_OBJECT:			return _T("WBEM_E_PROPERTY_NOT_AN_OBJECT");
		case WBEM_E_AGGREGATING_BY_OBJECT:			return _T("WBEM_E_AGGREGATING_BY_OBJECT");
		case WBEM_E_UNINTERPRETABLE_PROVIDER_QUERY:	return _T("WBEM_E_UNINTERPRETABLE_PROVIDER_QUERY");
		case WBEM_E_BACKUP_RESTORE_WINMGMT_RUNNING:	return _T("WBEM_E_BACKUP_RESTORE_WINMGMT_RUNNING");
		case WBEM_E_QUEUE_OVERFLOW:					return _T("WBEM_E_QUEUE_OVERFLOW");
		case WBEM_E_PRIVILEGE_NOT_HELD:				return _T("WBEM_E_PRIVILEGE_NOT_HELD");
		case WBEM_E_INVALID_OPERATOR:				return _T("WBEM_E_INVALID_OPERATOR");
		case WBEM_E_LOCAL_CREDENTIALS:				return _T("WBEM_E_LOCAL_CREDENTIALS");
		case WBEM_E_CANNOT_BE_ABSTRACT:				return _T("WBEM_E_CANNOT_BE_ABSTRACT");
		case WBEM_E_AMENDED_OBJECT:					return _T("WBEM_E_AMENDED_OBJECT");
		case WBEM_E_CLIENT_TOO_SLOW:				return _T("WBEM_E_CLIENT_TOO_SLOW");
		case WBEM_E_NULL_SECURITY_DESCRIPTOR:		return _T("WBEM_E_NULL_SECURITY_DESCRIPTOR");
		case WBEM_E_TIMED_OUT:						return _T("WBEM_E_TIMED_OUT");
		case WBEM_E_INVALID_ASSOCIATION:			return _T("WBEM_E_INVALID_ASSOCIATION");
		case WBEM_E_AMBIGUOUS_OPERATION:			return _T("WBEM_E_AMBIGUOUS_OPERATION");
		case WBEM_E_QUOTA_VIOLATION:				return _T("WBEM_E_QUOTA_VIOLATION");
		case WBEM_E_RESERVED_001:					return _T("WBEM_E_RESERVED_001");
		case WBEM_E_RESERVED_002:					return _T("WBEM_E_RESERVED_002");
		case WBEM_E_UNSUPPORTED_LOCALE:				return _T("WBEM_E_UNSUPPORTED_LOCALE");
		case WBEM_E_HANDLE_OUT_OF_DATE:				return _T("WBEM_E_HANDLE_OUT_OF_DATE");
		case WBEM_E_CONNECTION_FAILED:				return _T("WBEM_E_CONNECTION_FAILED");
		case WBEM_E_INVALID_HANDLE_REQUEST:			return _T("WBEM_E_INVALID_HANDLE_REQUEST");
		case WBEM_E_PROPERTY_NAME_TOO_WIDE:			return _T("WBEM_E_PROPERTY_NAME_TOO_WIDE");
		case WBEM_E_CLASS_NAME_TOO_WIDE:			return _T("WBEM_E_CLASS_NAME_TOO_WIDE");
		case WBEM_E_METHOD_NAME_TOO_WIDE:			return _T("WBEM_E_METHOD_NAME_TOO_WIDE");
		case WBEM_E_QUALIFIER_NAME_TOO_WIDE:		return _T("WBEM_E_QUALIFIER_NAME_TOO_WIDE");
		case WBEM_E_RERUN_COMMAND:					return _T("WBEM_E_RERUN_COMMAND");
		case WBEM_E_DATABASE_VER_MISMATCH:			return _T("WBEM_E_DATABASE_VER_MISMATCH");
		case WBEM_E_VETO_DELETE:					return _T("WBEM_E_VETO_DELETE");
		case WBEM_E_VETO_PUT:						return _T("WBEM_E_VETO_PUT");
		case WBEM_E_INVALID_LOCALE:					return _T("WBEM_E_INVALID_LOCALE");
		case WBEM_E_PROVIDER_SUSPENDED:				return _T("WBEM_E_PROVIDER_SUSPENDED");
		case WBEM_E_SYNCHRONIZATION_REQUIRED:		return _T("WBEM_E_SYNCHRONIZATION_REQUIRED");
		case WBEM_E_NO_SCHEMA:						return _T("WBEM_E_NO_SCHEMA");
		case WBEM_E_PROVIDER_ALREADY_REGISTERED:	return _T("WBEM_E_PROVIDER_ALREADY_REGISTERED");
		case WBEM_E_PROVIDER_NOT_REGISTERED:		return _T("WBEM_E_PROVIDER_NOT_REGISTERED");
		case WBEM_E_FATAL_TRANSPORT_ERROR:			return _T("WBEM_E_FATAL_TRANSPORT_ERROR");
		case WBEM_E_ENCRYPTED_CONNECTION_REQUIRED:	return _T("WBEM_E_ENCRYPTED_CONNECTION_REQUIRED");
		case WBEM_E_PROVIDER_TIMED_OUT:				return _T("WBEM_E_PROVIDER_TIMED_OUT");
		case WBEM_E_NO_KEY:							return _T("WBEM_E_NO_KEY");
		case WBEM_E_PROVIDER_DISABLED:				return _T("WBEM_E_PROVIDER_DISABLED");

		default:	return get_hresult_error_msg(hr);
	}
}

// ****************************************************************
// �ƂȂR�G���[�F�X���b�h�Z�[�t��
// ================================================================

// �Ō�̃G���[���L������A�X���b�h���Ƃ̃O���[�o���ϐ�
__declspec(thread) ARRAY<S_ERROR>* GT_last_error_list = NULL;

// �Ō�̃G���[���N���A
void clear_last_error_thread_safe()
{
	if (GT_last_error_list == NULL)
		GT_last_error_list = new ARRAY<S_ERROR>();			// ��������悤�c

	GT_last_error_list->clear();
}

// �Ō�̃G���[��ݒ�
void set_last_error_thread_safe(const S_ERROR* p_error)
{
	if (GT_last_error_list == NULL)
		GT_last_error_list = new ARRAY<S_ERROR>();			// ��������悤�c

	GT_last_error_list->clear();
	GT_last_error_list->push_back(*p_error);
}

// �Ō�̃G���[��ǉ�
void add_last_error_thread_safe(const S_ERROR* p_error)
{
	if (GT_last_error_list == NULL)
		GT_last_error_list = new ARRAY<S_ERROR>();			// ��������悤�c

	GT_last_error_list->push_back(*p_error);
}

// �G���[�����邩����
bool is_error_thread_safe()
{
	if (GT_last_error_list == NULL)
		return false;	// �G���[�͂Ȃ�

	if (GT_last_error_list->empty())
		return false;	// �G���[�͂Ȃ�

	return true;		// �G���[����
}

// �G���[���b�Z�[�W���擾
TSTR get_last_error_msg_thread_safe()
{
	TSTR error_str;

	if (GT_last_error_list == NULL)
		GT_last_error_list = new ARRAY<S_ERROR>();			// ��������悤�c

	// �S�ẴG���[�����s�łȂ���
	for (ARRAY<S_ERROR>::reverse_iterator itr = GT_last_error_list->rbegin(); itr != GT_last_error_list->rend(); ++itr)	{

		// �G���[�ӏ�
		if (!itr->error_func.empty())	{
			if (!error_str.empty())		error_str += _T("\n");		// ���s
			error_str += _T("�֐� ") + itr->error_func + _T(" �ɂ����ăG���[���������܂����B");
		}

		// �G���[�ڍ�
		TSTR detail_str;
		switch (itr->error_type)	{
			case ERROR_TYPE_CRT:
				detail_str = get_crt_error_msg(itr->error_no);
				break;
			case ERROR_TYPE_WIN:
				detail_str = get_win_error_msg(itr->error_no);
				break;
			case ERROR_TYPE_HRESULT:
				detail_str = _T("hr = ") + tostr((HRESULT)itr->error_no, 16) + _T(" (") + get_hresult_error_msg((HRESULT)itr->error_no) + _T(")");
				break;
			case ERROR_TYPE_WBEM:
				detail_str = _T("hr = ") + tostr((HRESULT)itr->error_no, 16) + _T(" (") + get_wbem_error_msg((HRESULT)itr->error_no) + _T(")");
				break;
			case ERROR_TYPE_OTHERS:
				detail_str = itr->error_str;
				break;
		}
		if (!detail_str.empty())	{
			if (!error_str.empty())		error_str += _T("\n");		// ���s
			error_str += detail_str;
		}
	}

	return error_str;
}

// ****************************************************************
// �ƂȂR�G���[�F�Ăяo����
// ================================================================

// �Ō�̃G���[��ݒ�
void set_last_error(E_ERROR_TYPE error_type, int error_no, const TSTR& error_func)
{
	S_ERROR error;
	error.error_type = error_type;
	error.error_no = error_no;
	error.error_func = error_func;

	// �G���[��ݒ�
	set_last_error_thread_safe(&error);
}

void set_last_error(const TSTR& error_str, const TSTR& error_func)
{
	S_ERROR error;
	error.error_type = ERROR_TYPE_OTHERS;
	error.error_str = error_str;
	error.error_func = error_func;

	// �G���[��ݒ�
	set_last_error_thread_safe(&error);
}

// �Ō�̃G���[��ǉ�
void add_last_error(const TSTR& error_str, const TSTR& error_func)
{
	S_ERROR error;
	error.error_type = ERROR_TYPE_OTHERS;
	error.error_str = error_str;
	error.error_func = error_func;

	// �G���[��ǉ�
	add_last_error_thread_safe(&error);
}

// �Ō�̃G���[�� CRT �G���[�ɐݒ�
void set_last_error_crt(const TSTR& error_func)
{
	set_last_error(ERROR_TYPE_CRT, get_crt_last_error_no(), error_func);
}

// �Ō�̃G���[�� Windows �G���[�ɐݒ�
void set_last_error_win(const TSTR& error_func)
{
	set_last_error(ERROR_TYPE_WIN, get_win_last_error_no(), error_func);
}
void set_last_error_win(int error_no, const TSTR& error_func)
{
	set_last_error(ERROR_TYPE_WIN, error_no, error_func);
}

// �Ō�̃G���[�� HRESULT �G���[�ɐݒ�
void set_last_error_hr(HRESULT hr, const TSTR& error_func)
{
	set_last_error(ERROR_TYPE_HRESULT, (int)hr, error_func);
}

// �Ō�̃G���[�� WBEM �G���[�ɐݒ�
void set_last_error_wbem(HRESULT hr, const TSTR& error_func)
{
	set_last_error(ERROR_TYPE_WBEM, (int)hr, error_func);
}

// �Ō�̃G���[���u�����ȃp�����[�^�v�ɐݒ�
void set_last_error_invalid(const TSTR& error_func)
{
	// CRT �G���[�� EINVAL ���g��
	set_last_error(ERROR_TYPE_CRT, EINVAL, error_func);		// �����ȃp�����[�^
}

// �Ō�̃G���[�̃G���[���b�Z�[�W���擾
TSTR get_last_error_msg()
{
	return get_last_error_msg_thread_safe();
}

// �Ō�̃G���[���N���A
void clear_last_error()
{
	// �G���[���N���A
	clear_last_error_thread_safe();
}

// �Ō�̃G���[�����݂��邩�𔻒�
bool is_error()
{
	return is_error_thread_safe();
}

}
