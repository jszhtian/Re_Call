//使用inline hook
#include "LoliHook.h"
#include "beaengine/beaengine.h"
#include <unordered_map>
#include <Windows.h>


namespace LoliHook {
	//命令的类型
	enum ASM_TYPE {
		ASM_NORMAL,
		ASM_SHORT_JMP,
		ASM_NUMBER_JMP,  //立即数跳转
		ASM_ADRESS_JMP,  //jmp [地址]
		ASM_CALL         //call xxx
	};

	enum {
		MAX_FUNC_SIZE = 256,
		MAX_MEMORY_SIZE = 512000,
	};

	class ApiHookData{
	public:
		std::string dllname;
		std::string funcname;
		char *packData = NULL;  //包含原api的几个字节和一个jmp命令，这个内存由分配函数分配
		int packLen = 0;     //packData的长度

		~ApiHookData() {
			//if (packData) delete[] packData;
			packLen = 0;
		}
	};

	struct MemoryHandle{  //内存区间段申请的管理结构体
		LPVOID ptr = NULL;
		int   size = 0;   //当前已经分配的大小
	};

	//已经hook的地址记录
	std::unordered_map<intptr_t, ApiHookData*> recordMap;

	//记录内存区间段
	std::unordered_map<int32_t, MemoryHandle*> memoryMap;

	//错误信息
	const char *lasterr = NULL;
	const char *GetLastError() { return lasterr; }

	//根据地址区间申请2G范围内内存，注意内存分配之后在执行整体释放之前不会回收
	char *LoliMalloc(intptr_t handle) {
		int32_t range = (handle >> 32) & 0xffffffff;
		if (sizeof(intptr_t) == 4) range = 0;

		MemoryHandle *mm = NULL;

		auto iter = memoryMap.find(range);
		if (iter == memoryMap.end()) {
			mm = new MemoryHandle;
			//尝试在区间段内分配内存
			for (int ii = 16; ii < 0xff; ii++) {
				char* adress;
				if(sizeof(intptr_t) == 4) adress = (char *)(ii << 24 | ii << 16);
				else {
					int64_t hi = handle & 0xffffffff00000000;
					hi += (ii << 24 | ii << 16);
					adress = (char *)hi;
				}
				mm->ptr = VirtualAlloc((LPVOID)adress, MAX_MEMORY_SIZE, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
				if (mm->ptr) break;
			}
			//内存分配失败
			if (!mm->ptr) return NULL;
			//printf("malloc adress is:%I64X\n", mm->ptr);
			memoryMap[range] = mm;
		}
		else mm = iter->second;
		//提供可用的内存地址
		if (mm->size >= MAX_MEMORY_SIZE) return NULL;
		mm->size += MAX_FUNC_SIZE;
		return (char*)mm->ptr + mm->size - MAX_FUNC_SIZE;
	}

	//改写文件地址
	bool OverWriteAdress(const char* adress, const char *buf, int len) {
		DWORD oldati1, oldati2;
		if (VirtualProtect((LPVOID)adress, len, PAGE_EXECUTE_READWRITE, &oldati1)) {
			SIZE_T wirtebyte = 0;
			WriteProcessMemory(INVALID_HANDLE_VALUE, (LPVOID)adress, buf, len, &wirtebyte);
			VirtualProtect((LPVOID)adress, len, oldati1, &oldati2);
			if (len != wirtebyte) return false;
			else return true;
		}
		return false;
	}

	//向指定内存地址写入一个跳转/call指令，isSys会使用OverWriteAdress复制
	void WriteJmpAsm(char op, char *dst, char *jmpdst, bool isSys) {
		char tmp[5];
		tmp[0] = op;
		intptr_t adress = jmpdst - dst - 5;
		memcpy(tmp + 1, &adress, 4);
		if (isSys) {
			OverWriteAdress(dst, tmp, 5);
		}
		else if (op == 0) memcpy(dst, tmp + 1, 4);  //只写入操作数
		else memcpy(dst, tmp, 5);
	}

	//检测当前命令的类型
	int TestAsmType(DISASM *engine) {
		if (engine->CompleteInstr[0] == 'j') {
			if (engine->CompleteInstr[1] != 'm') return ASM_SHORT_JMP;
			else {
				char *buf = &engine->CompleteInstr[2];
				while (*buf != 0) {
					if (*buf == '[') return ASM_ADRESS_JMP;
					buf++;
				}
				return ASM_NUMBER_JMP;
			}
		}
		else if (engine->CompleteInstr[0] == 'c' && engine->CompleteInstr[1] == 'a' && engine->CompleteInstr[2] == 'l') return ASM_CALL;
		else return ASM_NORMAL;
	}

	//开始32位hook
	intptr_t HookFunction32(UIntPtr adress, ApiHookData *data, void *newPtr) {
		//使用beaEngine引擎检测反汇编数据
		intptr_t dest;
		DISASM engine;
		engine.EIP = adress;
		engine.VirtualAddr = adress;
		engine.Options = MasmSyntax;
		engine.Archi = 32;
		while (engine.EIP - adress < 5) {  //只需要在开头来一个jmp
			int ulen = Disasm(&engine);

			switch (TestAsmType(&engine)) {
			case ASM_SHORT_JMP:
				lasterr = "ApiHook get a ASM_SHORT_JMP!";
				return NULL;
			case ASM_NUMBER_JMP:case ASM_CALL:
				memcpy(data->packData + data->packLen, (char*)engine.EIP, ulen);
				data->packLen += ulen;
				//重新计算地址
				WriteJmpAsm(0, data->packData + data->packLen - 4, (char *)engine.Instruction.AddrValue, false);
				break;
			default:  //无需处理的命令，复制即可
				memcpy(data->packData + data->packLen, (char*)engine.EIP, ulen);
				data->packLen += ulen;
				break;
			}
			engine.EIP += ulen;
			engine.VirtualAddr += ulen;
		}
		//接下需要一个回跳到原函数
		WriteJmpAsm(0xE9, data->packData + data->packLen, (char*)engine.EIP, false);
		data->packLen += 5;

		//直接写入一个跳转
		WriteJmpAsm(0xE9, (char*)adress, (char*)newPtr, true); //跳转到过渡

		return (intptr_t)data->packData;
	}

	//开始64位hook
	intptr_t HookFunction64(UIntPtr adress, ApiHookData *data , void *newPtr) {
		//使用beaEngine引擎检测反汇编数据
		intptr_t dest;
		DISASM engine;
		engine.EIP = adress;
		engine.VirtualAddr = adress;
		engine.Options = MasmSyntax;
		engine.Archi = 64;
		while (engine.EIP - adress < 5) {  //只需要在开头来一个jmp
			int ulen = Disasm(&engine);
			//核心问题在于地址转译
			switch (TestAsmType(&engine)) {
			case ASM_SHORT_JMP:
				lasterr = "ApiHook get a ASM_SHORT_JMP!";
				return NULL;
			default:  //无需处理的命令，复制即可
				memcpy(data->packData + data->packLen, (char*)engine.EIP, ulen);
				data->packLen += ulen;
				//如果有地址操作，则重新计算地址
				if(engine.Instruction.AddrValue != 0) WriteJmpAsm(0, data->packData + data->packLen - 4, (char *)engine.Instruction.AddrValue, false);
				break;
			}
			engine.EIP += ulen;
			engine.VirtualAddr += ulen;
		}
		//接下需要一个回跳到原函数
		WriteJmpAsm(0xE9, data->packData + data->packLen, (char*)engine.EIP, false);
		data->packLen += 5;

		//基本上不可能直接跳转到目的地，我们需要一个过渡
		//mov rax,xxxxxxxxx  jmp rax
		char jmpbyte[] = { 0xff,0x25,0,0,0,0 };
		memcpy(data->packData + MAX_FUNC_SIZE / 2, jmpbyte, 6);
		memcpy(data->packData + MAX_FUNC_SIZE / 2 + 6, &newPtr, 8);
		WriteJmpAsm(0xE9, (char*)adress, data->packData + MAX_FUNC_SIZE / 2, true); //跳转到过渡

		return (intptr_t)data->packData;
	}


	intptr_t ApiHook(const char *dllname, const char *funcname, void *newPtr) {
		auto dll = GetModuleHandleA(dllname);
		if (!dll) dll = LoadLibraryA(dllname);
		if (dll) {
			auto adress = GetProcAddress(dll, funcname);
			if (adress) {
				auto data = new ApiHookData();
				//记录dll名称和函数名称
				data->dllname.assign(dllname);
				data->funcname.assign(funcname);
				data->packData = LoliMalloc((intptr_t)dll);
				if (!data->packData) return NULL;
				if (sizeof(intptr_t) == 4) return HookFunction32((UIntPtr)adress, data, newPtr);
				else return HookFunction64((UIntPtr)adress, data, newPtr);
			}
			else {
				lasterr = "GetProcAddress faild!";
				return NULL;
			}
		}
		else {
			lasterr = "load dll faild!";
			return NULL;
		}
	}

	//直接hook地址
	intptr_t ApiHook(intptr_t adress, void *newPtr) {
		auto data = new ApiHookData();
		data->packData = LoliMalloc(adress);
		if (!data->packData) return NULL;
		if (sizeof(intptr_t) == 4) return HookFunction32((UIntPtr)adress, data, newPtr);
		else return HookFunction64((UIntPtr)adress, data, newPtr);
	}

	//结束hook，释放
	void FreeApiHook() {

	}


	//替换call指定地点的函数
	intptr_t CallHook(intptr_t adress, void *newPtr) {
		if (*(unsigned char*)adress != 0xe8)return NULL;

		auto data = new ApiHookData();

		//获取自身模块的基址，64位用
		HMODULE dll = NULL;
		GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPCSTR)(&CallHook), &dll);
		if (!dll) return NULL;


		//data->packData = LoliMalloc((intptr_t)dll);
		//if (!data->packData) return NULL;

		//首先获取原call的目标地址
		if(sizeof(intptr_t) == 4){ //32位比较简单
			//计算出原始地址
			intptr_t relative = *(intptr_t*)(adress + 1);
			intptr_t dstadress = adress + relative + 5;
			//写入新的地址
			WriteJmpAsm(0xe8, (char*)adress, (char*)newPtr, true);
			//printf("%x\n", *(int*)(adress + 1));
			//返回原来的call目标
			return dstadress;
		}
	}
}