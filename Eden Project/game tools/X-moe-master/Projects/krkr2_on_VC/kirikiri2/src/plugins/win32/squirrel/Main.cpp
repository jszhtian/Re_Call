#include <windows.h>
#include "tp_stub.h"
#include "sqplus.h"

#define KIRIKIRI_GLOBAL L"krkr"
#define SQUIRREL_GLOBAL L"sqglobal"

static const char *copyright =
"------ Squirrel Copyright START ------\n"
"Copyright (c) 2003-2007 Alberto Demichelis\n"
"------ Squirrel Copyright END ------\n";

/**
 * ���O�o�͗p
 */
static void log(const tjs_char *format, ...)
{
	va_list args;
	va_start(args, format);
	tjs_char msg[1024];
	_vsnwprintf(msg, 1024, format, args);
	TVPAddLog(msg);
	va_end(args);
}

/**
 * ���O�o�͗p for squirrel
 */
static void PrintFunc(HSQUIRRELVM v, const SQChar* format, ...)
{
	va_list args;
	va_start(args, format);
	tjs_char msg[1024];
	_vsnwprintf(msg, 1024, format, args);
	TVPAddLog(msg);
	va_end(args);
}

//---------------------------------------------------------------------------

static void
addMember(iTJSDispatch2 *dispatch, const tjs_char *name, iTJSDispatch2 *member)
{
	tTJSVariant var = tTJSVariant(member);
	member->Release();
	dispatch->PropSet(
		TJS_MEMBERENSURE, // �����o���Ȃ������ꍇ�ɂ͍쐬����悤�ɂ���t���O
		name, // �����o�� ( ���Ȃ炸 TJS_W( ) �ň͂� )
		NULL, // �q���g ( �{���̓����o���̃n�b�V���l�����ANULL �ł��悢 )
		&var, // �o�^����l
		dispatch // �R���e�L�X�g
		);
}

static iTJSDispatch2*
getMember(iTJSDispatch2 *dispatch, const tjs_char *name)
{
	tTJSVariant val;
	if (TJS_FAILED(dispatch->PropGet(TJS_IGNOREPROP,
									 name,
									 NULL,
									 &val,
									 dispatch))) {
		ttstr msg = TJS_W("can't get member:");
		msg += name;
		TVPThrowExceptionMessage(msg.c_str());
	}
	return val.AsObject();
}

static bool
isValidMember(iTJSDispatch2 *dispatch, const tjs_char *name)
{
	return dispatch->IsValid(TJS_IGNOREPROP,
							 name,
							 NULL,
							 dispatch) == TJS_S_TRUE;
}

static void
delMember(iTJSDispatch2 *dispatch, const tjs_char *name)
{
	dispatch->DeleteMember(
		0, // �t���O ( 0 �ł悢 )
		name, // �����o��
		NULL, // �q���g
		dispatch // �R���e�L�X�g
		);
}

//---------------------------------------------------------------------------

extern void store(tTJSVariant &result, SquirrelObject &variant);
extern void store(tTJSVariant &result, HSQUIRRELVM v, int idx=-1);
extern void registglobal(HSQUIRRELVM v, const SQChar *name, iTJSDispatch2 *dispatch);

//---------------------------------------------------------------------------

/**
 */
class tExecFunction : public tTJSDispatch
{
public:	
	/// �R���X�g���N�^
	tExecFunction() {
	}
	/// �f�X�g���N�^
	~tExecFunction() {
	}
	
	tjs_error TJS_INTF_METHOD FuncCall(
		tjs_uint32 flag, const tjs_char * membername, tjs_uint32 *hint,
		tTJSVariant *result,
		tjs_int numparams, tTJSVariant **param, iTJSDispatch2 *objthis) {

		// �l�C�e�B�u�I�u�W�F�N�g�̎擾
		if (!objthis) return TJS_E_NATIVECLASSCRASH;
		if (membername) return TJS_E_MEMBERNOTFOUND;
		if (numparams <= 0) return TJS_E_BADPARAMCOUNT;

		try {
			SquirrelObject script = SquirrelVM::CompileBuffer(param[0]->GetString());
			SquirrelObject ret    = SquirrelVM::RunScript(script);
			if (result) {
				store(*result, ret);
			}
		} catch(SquirrelError e) {
			TVPThrowExceptionMessage(e.desc);
		}
		return S_OK;
	}
};

/**
 */
class tExecStorageFunction : public tTJSDispatch
{
public:	
	/// �R���X�g���N�^
	tExecStorageFunction() {
	}
	/// �f�X�g���N�^
	~tExecStorageFunction() {
	}
	
	tjs_error TJS_INTF_METHOD FuncCall(
		tjs_uint32 flag, const tjs_char * membername, tjs_uint32 *hint,
		tTJSVariant *result,
		tjs_int numparams, tTJSVariant **param, iTJSDispatch2 *objthis) {

		// �l�C�e�B�u�I�u�W�F�N�g�̎擾
		if (!objthis) return TJS_E_NATIVECLASSCRASH;
		if (membername) return TJS_E_MEMBERNOTFOUND;
		if (numparams <= 0) return TJS_E_BADPARAMCOUNT;

		iTJSTextReadStream * stream = TVPCreateTextStreamForRead(param[0]->GetString(), TJS_W(""));
		try {
			ttstr data;
			stream->Read(data, 0);
			SquirrelObject script = SquirrelVM::CompileBuffer(data.c_str());
			SquirrelObject ret    = SquirrelVM::RunScript(script);
			if (result) {
				store(*result, ret);
			}
		} catch(SquirrelError e) {
			stream->Destruct();
			TVPThrowExceptionMessage(e.desc);
		} catch(...) {
			stream->Destruct();
			throw;
		}
		stream->Destruct();
		return S_OK;
	}
};

static SQInteger getThread(HSQUIRRELVM v)
{
	// �V�����X���b�h�����
	HSQUIRRELVM newvm = sq_newthread(v, 1024);

	// �X���b�h�ɑ΂��ăX�N���v�g�����[�h����
	const SQChar *s;
	sq_getstring(v, 2, &s);
	iTJSTextReadStream * stream = TVPCreateTextStreamForRead(s, TJS_W(""));
	try {
		ttstr data;
		stream->Read(data, 0);
		if (!SQ_SUCCEEDED(sq_compilebuffer(newvm, data.c_str(), data.length(), NULL, 1))) {
			const SQChar *s;
			sq_getlasterror(newvm);
			sq_getstring(newvm,-1,&s);
			log(s);
		}
	} catch(...) {
		stream->Destruct();
		throw;
	}
	stream->Destruct();
	return 1;
}

//---------------------------------------------------------------------------

#pragma argsused
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason,
	void* lpReserved)
{
	return 1;
}

//---------------------------------------------------------------------------
static tjs_int GlobalRefCountAtInit = 0;
extern "C" HRESULT _stdcall V2Link(iTVPFunctionExporter *exporter)
{
	// �X�^�u�̏�����(�K���L�q����)
	TVPInitImportStub(exporter);

	TVPAddImportantLog(ttstr(copyright));
	
	// squirrel ������
	SquirrelVM::Init();

	{
		HSQUIRRELVM v = SquirrelVM::GetVMPtr();

		// getThread �̓o�^
		sq_pushroottable(v);
		sq_pushstring(v, L"getThread", -1);
		sq_newclosure(v, getThread, 0);
		sq_createslot(v, -3); 
		sq_pop(v, 1);

		// print �̓o�^
		sq_setprintfunc(v, PrintFunc);
	}
	
	// TJS �̃O���[�o���I�u�W�F�N�g���擾����
	iTJSDispatch2 * global = TVPGetScriptDispatch();
	
	if (global) {

		// Scripts �I�u�W�F�N�g���擾
		tTJSVariant varScripts;
		TVPExecuteExpression(TJS_W("Scripts"), &varScripts);
		iTJSDispatch2 *dispatch = varScripts.AsObjectNoAddRef();
		if (dispatch) {
			addMember(dispatch, TJS_W("execSQ"), new tExecFunction());
			addMember(dispatch, TJS_W("execStorageSQ"), new tExecStorageFunction());
		}
		
		// Squirrel �� �O���[�o���ɋg���g���� �O���[�o����o�^����
		registglobal(SquirrelVM::GetVMPtr(), KIRIKIRI_GLOBAL, global);

		// �g���g���̃O���[�o���� Squirrel �̃O���[�o����o�^����
		{
			tTJSVariant result = tTJSVariant();
			
			HSQUIRRELVM v = SquirrelVM::GetVMPtr();
			sq_pushroottable(v);
			store(result, v, -1);
			sq_pop(v, 1);
			
			global->PropSet(
				TJS_MEMBERENSURE,
				SQUIRREL_GLOBAL,
				NULL,
				&result,
				global
				);
		}
		
		global->Release();
	}
			
	// ���̎��_�ł� TVPPluginGlobalRefCount �̒l��
	GlobalRefCountAtInit = TVPPluginGlobalRefCount;
	// �Ƃ��čT���Ă����BTVPPluginGlobalRefCount �͂��̃v���O�C������
	// �Ǘ�����Ă��� tTJSDispatch �h���I�u�W�F�N�g�̎Q�ƃJ�E���^�̑��v�ŁA
	// ������ɂ͂���Ɠ������A����������Ȃ��Ȃ��ĂȂ��ƂȂ�Ȃ��B
	// �����Ȃ��ĂȂ���΁A�ǂ����ʂ̂Ƃ���Ŋ֐��Ȃǂ��Q�Ƃ���Ă��āA
	// �v���O�C���͉���ł��Ȃ��ƌ������ƂɂȂ�B

	return S_OK;
}
//---------------------------------------------------------------------------
extern "C" HRESULT _stdcall V2Unlink()
{
	// �g���g��������A�v���O�C����������悤�Ƃ���Ƃ��ɌĂ΂��֐��B

	// �������炩�̏����Ńv���O�C��������ł��Ȃ��ꍇ��
	// ���̎��_�� E_FAIL ��Ԃ��悤�ɂ���B
	// �����ł́ATVPPluginGlobalRefCount �� GlobalRefCountAtInit ����
	// �傫���Ȃ��Ă���Ύ��s�Ƃ������Ƃɂ���B
	if(TVPPluginGlobalRefCount > GlobalRefCountAtInit) return E_FAIL;
		// E_FAIL ���A��ƁAPlugins.unlink ���\�b�h�͋U��Ԃ�

	// - �܂��ATJS �̃O���[�o���I�u�W�F�N�g���擾����
	iTJSDispatch2 * global = TVPGetScriptDispatch();

	// - global �� DeleteMember ���\�b�h��p���A�I�u�W�F�N�g���폜����
	if (global)	{

		// Scripts �I�u�W�F�N�g���擾
		tTJSVariant varScripts;
		TVPExecuteExpression(TJS_W("Scripts"), &varScripts);
		iTJSDispatch2 *dispatch = varScripts.AsObjectNoAddRef();
		if (dispatch) {
			delMember(dispatch, TJS_W("execSQ"));
			delMember(dispatch, TJS_W("execStorageSQ"));
		}

		delMember(global, SQUIRREL_GLOBAL);
		
		global->Release();
	}

	SquirrelVM::Shutdown();
	
	// �X�^�u�̎g�p�I��(�K���L�q����)
	TVPUninitImportStub();

	return S_OK;
}
