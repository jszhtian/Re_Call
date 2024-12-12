#include	"tona3_pch.h"
#include	"tona3_array.h"
#include	"tona3_allocator.h"

// ****************************************************************
// ■とな３：アロケータ
// ================================================================
namespace NT3
{
	// グローバルなメモリマネージャ
	C_mem_manager<32>					G_mem_mng_32;
	C_mem_manager<128>					G_mem_mng_128;
	C_mem_manager<1024>					G_mem_mng_1KB;
	C_mem_manager<1024 * 4>				G_mem_mng_4KB;
	C_mem_manager<1024 * 16>			G_mem_mng_16KB;
	C_mem_manager<1024 * 64>			G_mem_mng_64KB;
	C_mem_manager<1024 * 256>			G_mem_mng_256KB;
	C_mem_manager<1024 * 1024>			G_mem_mng_1MB;
	C_mem_manager<1024 * 1024 * 4>		G_mem_mng_4MB;
	C_mem_manager<1024 * 1024 * 16>		G_mem_mng_16MB;
	C_mem_manager<1024 * 1024 * 64>		G_mem_mng_64MB;
	C_mem_manager<1024 * 1024 * 256>	G_mem_mng_256MB;
}

