#include <windows.h>
#include "tp_stub.h"
#include "sqplus.h"

void store(HSQUIRRELVM v, iTJSDispatch2 *dispatch);
void store(HSQUIRRELVM v, tTJSVariant &variant);
void store(tTJSVariant &result, HSQOBJECT &obj);
void store(tTJSVariant &result, HSQUIRRELVM v, int idx=-1);
void store(tTJSVariant &result, SquirrelObject &obj);

static const SQUserPointer TJSTYPETAG = (SQUserPointer)"TJSTYPETAG";

/**
 * IDispatch �p iTJSDispatch2 ���b�p�[
 */
class iTJSDispatch2Wrapper : public tTJSDispatch
{
protected:
	/// �����ێ��p
	HSQOBJECT obj;

public:
	/**
	 * �R���X�g���N�^
	 * @param obj IDispatch
	 */
	iTJSDispatch2Wrapper(HSQOBJECT obj) : obj(obj) {
		sq_addref(SquirrelVM::GetVMPtr(), &obj);
	}
	
	/**
	 * �f�X�g���N�^
	 */
	~iTJSDispatch2Wrapper() {
		if(SquirrelVM::GetVMPtr()) {
			sq_release(SquirrelVM::GetVMPtr(), &obj);
		}
	}

public:

	tjs_error TJS_INTF_METHOD CreateNew(
		tjs_uint32 flag,
		const tjs_char * membername,
		tjs_uint32 *hint,
		iTJSDispatch2 **result,
		tjs_int numparams,
		tTJSVariant **param,
		iTJSDispatch2 *objthis
		)
	{
		if (membername) {
			return TJS_E_MEMBERNOTFOUND;
		}
		if (obj._type == OT_CLASS) {
			int ret = S_FALSE;
			sq_pushobject(SquirrelVM::GetVMPtr(), obj);
			// this ��������
			sq_pushroottable(SquirrelVM::GetVMPtr());
			// �p�����[�^�Q
			for (int i=0;i<numparams;i++) {
				store(SquirrelVM::GetVMPtr(), *param[i]);
			}
			if (result) {
				// �A��l������ꍇ
				if (SQ_SUCCEEDED(sq_call(SquirrelVM::GetVMPtr(), numparams + 1, SQTrue, SQFalse))) {
					ret = S_OK;
					{
						HSQOBJECT x;
						sq_resetobject(&x);
						sq_getstackobj(SquirrelVM::GetVMPtr(),-1,&x);
						*result = new iTJSDispatch2Wrapper(x);
					}
					sq_pop(SquirrelVM::GetVMPtr(), 1);
				}
			} else {
				if (SQ_SUCCEEDED(sq_call(SquirrelVM::GetVMPtr(), numparams + 1, SQTrue, SQFalse))) {
					ret = S_OK;
				}
			}
			sq_pop(SquirrelVM::GetVMPtr(), 1);
			return ret;
		}
		return TJS_E_NOTIMPL;
	}

	
	tjs_error TJS_INTF_METHOD FuncCall(
		tjs_uint32 flag,
		const tjs_char * membername,
		tjs_uint32 *hint,
		tTJSVariant *result,
		tjs_int numparams,
		tTJSVariant **param,
		iTJSDispatch2 *objthis
		) {
		if (membername == NULL) {
			if (obj._type == OT_CLOSURE ||
				obj._type == OT_NATIVECLOSURE ||
				obj._type == OT_GENERATOR) {
				int ret = S_FALSE;
				sq_pushobject(SquirrelVM::GetVMPtr(), obj);
				// this ��������
				sq_pushroottable(SquirrelVM::GetVMPtr());
				// �p�����[�^�Q
				for (int i=0;i<numparams;i++) {
					store(SquirrelVM::GetVMPtr(), *param[i]);
				}
				if (result) {
					// �A��l������ꍇ
					if (SQ_SUCCEEDED(sq_call(SquirrelVM::GetVMPtr(), numparams + 1, SQTrue, SQFalse))) {
						ret = S_OK;
						store(*result, SquirrelVM::GetVMPtr());
						sq_pop(SquirrelVM::GetVMPtr(), 1);
					}
				} else {
					if (SQ_SUCCEEDED(sq_call(SquirrelVM::GetVMPtr(), numparams + 1, SQTrue, SQFalse))) {
						ret = S_OK;
					}
				}
				sq_pop(SquirrelVM::GetVMPtr(), 1);
				return ret;
			}
			return TJS_E_NOTIMPL;
		} else {
			int ret = S_FALSE;
			sq_pushobject(SquirrelVM::GetVMPtr(), obj);
			sq_pushstring(SquirrelVM::GetVMPtr(), membername,-1);
			if(SQ_SUCCEEDED(sq_get(SquirrelVM::GetVMPtr(),-2))) {
				// this
				sq_pushobject(SquirrelVM::GetVMPtr(), obj);
				// �p�����[�^�Q
				for (int i=0;i<numparams;i++) {
					store(SquirrelVM::GetVMPtr(), *param[i]);
				}
				if (result) {
					// �A��l������ꍇ
					if (SQ_SUCCEEDED(sq_call(SquirrelVM::GetVMPtr(), numparams + 1, SQTrue, SQFalse))) {
						ret = S_OK;
						store(*result, SquirrelVM::GetVMPtr());
						sq_pop(SquirrelVM::GetVMPtr(), 1);
					}
				} else {
					if (SQ_SUCCEEDED(sq_call(SquirrelVM::GetVMPtr(), numparams + 1, SQTrue, SQFalse))) {
						ret = S_OK;
					}
				}
			}
			sq_pop(SquirrelVM::GetVMPtr(), 1);
			return ret;
		}
	}

	tjs_error TJS_INTF_METHOD PropGet(
		tjs_uint32 flag,
		const tjs_char * membername,
		tjs_uint32 *hint,
		tTJSVariant *result,
		iTJSDispatch2 *objthis
) {
		// �v���p�e�B�͂Ȃ�
		if (!membername) {
			return TJS_E_NOTIMPL;
		}
		int ret = S_FALSE;
		sq_pushobject(SquirrelVM::GetVMPtr(), obj);
		sq_pushstring(SquirrelVM::GetVMPtr(), membername,-1);
		if(SQ_SUCCEEDED(sq_get(SquirrelVM::GetVMPtr(),-2))) {
			ret = S_OK;
			if (result) {
				store(*result, SquirrelVM::GetVMPtr());
			}	
			sq_pop(SquirrelVM::GetVMPtr(),1);
		}
		sq_pop(SquirrelVM::GetVMPtr(),1);
		return ret;
	}

	tjs_error TJS_INTF_METHOD PropSet(
		tjs_uint32 flag,
		const tjs_char *membername,
		tjs_uint32 *hint,
		const tTJSVariant *param,
		iTJSDispatch2 *objthis
		) {
		// �v���p�e�B�͂Ȃ�
		if (!membername) {
			return TJS_E_NOTIMPL;
		}
		int ret = S_FALSE;
		sq_pushobject(SquirrelVM::GetVMPtr(), obj);
		sq_pushstring(SquirrelVM::GetVMPtr(), membername,-1);
		store(SquirrelVM::GetVMPtr(), (tTJSVariant&)*param);
		if ((flag & TJS_MEMBERENSURE)) {
			if(SQ_SUCCEEDED(sq_newslot(SquirrelVM::GetVMPtr(),-3, SQFalse))) {
				ret = S_OK;
			}
		} else {
			if(SQ_SUCCEEDED(sq_set(SquirrelVM::GetVMPtr(),-3))) {
				ret = S_OK;
			}
		}
		sq_pop(SquirrelVM::GetVMPtr(),1);
		return ret;
	}
};


//----------------------------------------------------------------------------
// iTJSDispatch2 �� UserData �Ƃ��ĕێ����邽�߂̋@�\
//----------------------------------------------------------------------------

/**
 * iTJSDispatch2 �p�I�u�W�F�N�g�J������
 */
static SQInteger
tjsDispatchRelease(SQUserPointer up, SQInteger size)
{
	iTJSDispatch2 *dispatch	= *((iTJSDispatch2**)up);
	dispatch->Release();
	return 1;
}

/**
 * iTJSDispatch2 �p�v���p�e�B�̎擾
 * @param v squirrel VM
 */
static SQInteger
get(HSQUIRRELVM v)
{
	StackHandler stack(v);
	SQUserPointer up = stack.GetUserData(1, TJSTYPETAG);
	if (up) {
		iTJSDispatch2 *dispatch	= *((iTJSDispatch2**)up);
		tTJSVariant result;
		if (SUCCEEDED(dispatch->PropGet(0, stack.GetString(2), NULL, &result, dispatch))) {
			store(v, result);
			return 1;
		}
	}
	return 0;
}

/**
 * iTJSDispatch2 �p�v���p�e�B�̐ݒ�
 * @param v squirrel VM
 */
static SQInteger
set(HSQUIRRELVM v)
{
	StackHandler stack(v);
	SQUserPointer up = stack.GetUserData(1, TJSTYPETAG);
	if (up) {
		iTJSDispatch2 *dispatch	= *((iTJSDispatch2**)up);
		tTJSVariant result;
		store(result, stack.GetObjectHandle(3));
		dispatch->PropSet(TJS_MEMBERENSURE, stack.GetString(2), NULL, &result, dispatch);
	}
	return 0;
}

/**
 * iTJSDispatch2 �p���\�b�h�܂��̓R���X�g���N�^�̌Ăяo��
 * @param v squirrel VM
 */
static SQInteger
call(HSQUIRRELVM v)
{
	// param1 �I�u�W�F�N�g
	// param2 �I���W�i���I�u�W�F�N�g
	// param3 �` �{���̈�����

	StackHandler stack(v);
	SQUserPointer up = stack.GetUserData(1, TJSTYPETAG);
	int ret = 0;
	if (up) {
		iTJSDispatch2 *dispatch	= *((iTJSDispatch2**)up);

		// this ���擾
		iTJSDispatch2 *thisobj = NULL;
		up = stack.GetUserData(2, TJSTYPETAG);
		if (up) {
			thisobj = *((iTJSDispatch2**)up);
		}
		
		int argc = stack.GetParamCount() - 2;
		
		// �����ϊ�
		tTJSVariant **args = new tTJSVariant*[argc];
		for (int i=0;i<argc;i++) {
			args[i] = new tTJSVariant();
			store(*args[i], stack.GetObjectHandle(i+3));
		}

		if (dispatch->IsInstanceOf(0, NULL, NULL, L"Class", dispatch) == TJS_S_TRUE) {
			// �N���X�I�u�W�F�N�g�̏ꍇ�R���X�g���N�^�Ƃ݂Ȃ�
			iTJSDispatch2 *instance = NULL;
			if (SUCCEEDED(dispatch->CreateNew(0, NULL, NULL, &instance, argc, args, thisobj))) {
				store(v, instance);
				ret = 1;
			}
		} else {
			// ���\�b�h�Ăяo��
			tTJSVariant result;
			if (SUCCEEDED(dispatch->FuncCall(0, NULL, NULL, &result, argc, args, thisobj))) {
				if (result.Type() != tvtVoid) {
					store(v, result);
					ret = 1;
				}
			}
		}
			
		// �����j��
		for (int i=0;i<argc;i++) {
			delete args[i];
		}
		delete[] args;
	}
	return ret;
}

/**
 * iTJSDispatch2 �� squirrel �̋�Ԃɓ�������
 * @param v squirrel VM
 * @param dispatch iTJSDispatch2
 */
void
store(HSQUIRRELVM v, iTJSDispatch2 *dispatch)
{
	if (dispatch) {
		dispatch->AddRef();
		SQUserPointer up = sq_newuserdata(v, sizeof(iTJSDispatch2*));
		*((iTJSDispatch2**)up) = dispatch;

		// �^�O�o�^
		sq_settypetag(v, -1, TJSTYPETAG);
		
		// �J�����W�b�N��ǉ�
		sq_setreleasehook(v, -1, tjsDispatchRelease);

		// ���\�b�h�Q��ǉ�
		sq_newtable(v);

		sq_pushstring(v, L"_get", -1);
		sq_newclosure(v, get, 0);
		sq_createslot(v, -3);

		sq_pushstring(v, L"_set", -1);
		sq_newclosure(v, set, 0);
		sq_createslot(v, -3);

		sq_pushstring(v, L"_call", -1);
		sq_newclosure(v, call, 0);
		sq_createslot(v, -3);
		
		sq_setdelegate(v, -2);

	} else {
		sq_pushnull(v);
	}
}

/**
 * tTJSVariant �� squirrel �̋�Ԃɓ�������
 * @param v squirrel VM
 * @param variant tTJSVariant
 */
void
store(HSQUIRRELVM v, tTJSVariant &variant)
{
	switch (variant.Type()) {
	case tvtVoid:
		sq_pushnull(v);
		break;
	case tvtObject:
		store(v, variant.AsObjectNoAddRef());
		break;
	case tvtString:
		sq_pushstring(v, variant.GetString(), -1);
		break;
	case tvtOctet: // XXX
		sq_pushnull(v);
		break;
	case tvtInteger:
		sq_pushinteger(v, (int)(variant));
		break;
	case tvtReal:
		sq_pushfloat(v, (float)(double)(variant));
		break;
	}
}


/**
 * squirrel �̃I�u�W�F�N�g�� tTJSVariant �ɕϊ�����(�l�n���j
 * @param result ���ʊi�[��
 * @param stack squirrel �̃X�^�b�N�̃��b�p�[
 * @param idx �X�^�b�N�̂ǂ̕������擾���邩�̃C���f�b�N�X�w��
 */
void
store(tTJSVariant &result, HSQOBJECT &obj)
{
	result.Clear();
	switch (obj._type) {
	case OT_NULL:
		break;
	case OT_INTEGER:
		result = sq_objtointeger(&obj);
		break;
	case OT_FLOAT:
		result = (double)sq_objtofloat(&obj);
		break;
	case OT_BOOL:
		result = sq_objtobool(&obj) ? 1 : 0;
		break;
	case OT_STRING:
		result = sq_objtostring(&obj);
		break;
	case OT_USERDATA:
		{
			sq_pushobject(SquirrelVM::GetVMPtr(), obj);
			SQUserPointer data, typetag;
			sq_getuserdata(SquirrelVM::GetVMPtr(), -1, &data, &typetag);
			if (data && typetag == TJSTYPETAG) {
				result = *((iTJSDispatch2**)data);
			}
			sq_pop(SquirrelVM::GetVMPtr(), 1);
		}
		break;
	case OT_TABLE:
	case OT_ARRAY:
	case OT_CLOSURE:
	case OT_NATIVECLOSURE:
	case OT_GENERATOR:
	case OT_USERPOINTER:
	case OT_THREAD:
	case OT_CLASS:
	case OT_INSTANCE:
	case OT_WEAKREF:
		// ���b�s���O���K�v!
		{
			iTJSDispatch2 *tjsobj = new iTJSDispatch2Wrapper(obj);
			result = tjsobj;
			tjsobj->Release();
		}
		break;
	}
}

void
store(tTJSVariant &result, HSQUIRRELVM v, int idx)
{
	HSQOBJECT x;
	sq_resetobject(&x);
	sq_getstackobj(v,idx,&x);
	store(result, x);
}

void
store(tTJSVariant &result, SquirrelObject &obj)
{
	store(result, obj.GetObjectHandle());
}

// -------------------------------------------------------

/**
 * Squirrel �� �O���[�o����Ԃɂɓo�^�������s��
 */
void registglobal(HSQUIRRELVM v, const SQChar *name, iTJSDispatch2 *dispatch)
{
	sq_pushroottable(v);
	sq_pushstring(v, name, -1);
	store(v, dispatch);
	sq_createslot(v, -3); 
	sq_pop(v, 1);
}
