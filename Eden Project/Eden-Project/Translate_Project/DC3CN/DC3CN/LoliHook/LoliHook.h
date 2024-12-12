//使用inline hook


#include <stdint.h>

namespace LoliHook {
	//hook指定dll中的指定函数，返回可以运行原版api的函数指针
	extern intptr_t ApiHook(const char *dllname, const char *funcname, void *newPtr);
	extern intptr_t ApiHook(intptr_t adress, void *newPtr);
	extern intptr_t CallHook(intptr_t adress, void *newPtr);
	extern const char *GetLastError();
	extern bool OverWriteAdress(const char* adress, const char *buf, int len);
}