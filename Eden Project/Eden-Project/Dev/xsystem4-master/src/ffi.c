/* Copyright (C) 2019 Nunuhara Cabbage <nunuhara@haniwa.technology>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://gnu.org/licenses/>.
 */

#include <string.h>
#include <ffi.h>
#include "system4/ain.h"
#include "vm.h"
#include "vm/heap.h"
#include "vm/page.h"

#define HLL_MAX_ARGS 64

struct hll_function {
	void *fun;
	ffi_cif cif;
	unsigned int nr_args;
	ffi_type **args;
	ffi_type *return_type;
};

static struct hll_function **libraries;

bool library_exists(int libno)
{
	return libraries[libno];
}

bool library_function_exists(int libno, int fno)
{
	return libraries[libno] && libraries[libno][fno].fun;
}

void hll_call(int libno, int fno)
{
	struct ain_hll_function *f = &ain->libraries[libno].functions[fno];

	if (!libraries[libno])
		VM_ERROR("Unimplemented HLL function: %s.%s", ain->libraries[libno].name, f->name);

	struct hll_function *fun = &libraries[libno][fno];
	if (!fun->fun)
		VM_ERROR("Unimplemented HLL function: %s.%s", ain->libraries[libno].name, f->name);

	void *args[HLL_MAX_ARGS];
	void *ptrs[HLL_MAX_ARGS];
	for (int i = f->nr_arguments - 1; i >= 0; i--) {
		switch (f->arguments[i].type.data) {
		case AIN_REF_INT:
		case AIN_REF_LONG_INT:
		case AIN_REF_BOOL:
		case AIN_REF_FLOAT: {
			// need to create pointer for immediate ref types
			stack_ptr -= 2;
			int pageno = stack[stack_ptr].i;
			int varno  = stack[stack_ptr+1].i;
			ptrs[i] = &heap[pageno].page->values[varno];
			args[i] = &ptrs[i];
			break;
		}
		case AIN_STRING:
			stack_ptr--;
			args[i] = &heap[stack[stack_ptr].i].s;
			break;
		case AIN_REF_STRING:
			stack_ptr--;
			ptrs[i] = &heap[stack[stack_ptr].i].s;
			args[i] = &ptrs[i];
			break;
		case AIN_STRUCT:
		case AIN_ARRAY_TYPE:
			stack_ptr--;
			args[i] = &heap[stack[stack_ptr].i].page;
			break;
		case AIN_REF_STRUCT:
		case AIN_REF_ARRAY_TYPE:
			stack_ptr--;
			ptrs[i] = &heap[stack[stack_ptr].i].page;
			args[i] = &ptrs[i];
			break;
		default:
			stack_ptr--;
			args[i] = &stack[stack_ptr];
			break;
		}
	}

	union vm_value r;
	ffi_call(&fun->cif, (void*)fun->fun, &r, args);

	for (int i = 0, j = 0; i < f->nr_arguments; i++, j++) {
		// XXX: We don't increase the ref count when passing ref arguments to HLL
		//      functions, so we need to avoid decreasing it via variable_fini
		switch (f->arguments[i].type.data) {
		case AIN_REF_INT:
		case AIN_REF_LONG_INT:
		case AIN_REF_BOOL:
		case AIN_REF_FLOAT:
			j++;
			break;
		case AIN_REF_STRING:
		case AIN_REF_STRUCT:
		case AIN_REF_FUNC_TYPE:
		case AIN_REF_ARRAY_TYPE:
			break;
		default:
			variable_fini(stack[stack_ptr+j], f->arguments[i].type.data);
			break;
		}
	}

	if (f->return_type.data != AIN_VOID) {
		if (f->return_type.data == AIN_STRING) {
			int slot = heap_alloc_slot(VM_STRING);
			heap[slot].s = r.ref;
		} else {
			stack_push(r);
		}
	}
}

extern struct static_library lib_ACXLoader;
extern struct static_library lib_AliceLogo;
extern struct static_library lib_AliceLogo2;
extern struct static_library lib_AliceLogo3;
extern struct static_library lib_Confirm2;
extern struct static_library lib_DrawGraph;
extern struct static_library lib_DrawPluginManager;
extern struct static_library lib_File;
extern struct static_library lib_Math;
extern struct static_library lib_MsgLogManager;
extern struct static_library lib_MsgLogViewer;
extern struct static_library lib_MsgSkip;
extern struct static_library lib_OutputLog;
extern struct static_library lib_PlayMovie;
extern struct static_library lib_SACT2;
extern struct static_library lib_SengokuRanceFont;
extern struct static_library lib_SystemServiceEx;

static struct static_library *static_libraries[] = {
	&lib_ACXLoader,
	&lib_AliceLogo,
	&lib_AliceLogo2,
	&lib_AliceLogo3,
	&lib_Confirm2,
	&lib_DrawGraph,
	&lib_DrawPluginManager,
	&lib_File,
	&lib_Math,
	&lib_MsgLogManager,
	&lib_MsgLogViewer,
	&lib_MsgSkip,
	&lib_OutputLog,
	&lib_PlayMovie,
	&lib_SACT2,
	&lib_SengokuRanceFont,
	&lib_SystemServiceEx,
	NULL
};

static ffi_type *ain_to_ffi_type(enum ain_data_type type)
{
	switch (type) {
	case AIN_VOID:
		return &ffi_type_void;
	case AIN_INT:
	case AIN_BOOL:
		return &ffi_type_sint32;
	case AIN_LONG_INT:
		return &ffi_type_sint64;
	case AIN_FLOAT:
		return &ffi_type_float;
	case AIN_STRING:
	case AIN_STRUCT:
	case AIN_FUNC_TYPE:
	case AIN_DELEGATE:
	case AIN_ARRAY_TYPE:
	case AIN_REF_TYPE:
	case AIN_IMAIN_SYSTEM: // ???
		return &ffi_type_pointer;
	default:
		ERROR("Unhandled type in HLL function: %s", ain_strtype(ain, type, -1));
	}
}

static void link_static_library_function(struct hll_function *dst, struct ain_hll_function *src, void *funcptr)
{
	dst->fun = funcptr;
	dst->nr_args = src->nr_arguments;
	dst->args = xcalloc(dst->nr_args, sizeof(ffi_type*));

	for (unsigned int i = 0; i < dst->nr_args; i++) {
		dst->args[i] = ain_to_ffi_type(src->arguments[i].type.data);
	}
	dst->return_type = ain_to_ffi_type(src->return_type.data);

	if (ffi_prep_cif(&dst->cif, FFI_DEFAULT_ABI, dst->nr_args, dst->return_type, dst->args) != FFI_OK)
		ERROR("Failed to link HLL function");
}

/*
 * "Link" a library that has been compiled into the xsystem4 executable.
 */
static struct hll_function *link_static_library(struct ain_library *ainlib, struct static_library *lib)
{
	struct hll_function *dst = xcalloc(ainlib->nr_functions, sizeof(struct hll_function));

	for (int i = 0; i < ainlib->nr_functions; i++) {
		for (int j = 0; lib->functions[j].name; j++) {
			if (!strcmp(ainlib->functions[i].name, lib->functions[j].name)) {
				link_static_library_function(&dst[i], &ainlib->functions[i], lib->functions[j].fun);
				break;
			}
		}
		if (!dst[i].fun)
			WARNING("Unimplemented library function: %s.%s", ainlib->name, ainlib->functions[i].name);
		else if (ainlib->functions[i].nr_arguments >= HLL_MAX_ARGS)
			ERROR("Too many arguments to library function: %s", ainlib->functions[i].name);
	}

	for (int i = 0; lib->functions[i].name; i++) {
		if (!strcmp(lib->functions[i].name, "_ModuleInit")) {
			((void(*)(void))lib->functions[i].fun)();
			break;
		}
	}
	return dst;
}

void link_libraries(void)
{
	libraries = xcalloc(ain->nr_libraries, sizeof(struct hll_function*));

	for (int i = 0; i < ain->nr_libraries; i++) {
		for (int j = 0; static_libraries[j]; j++) {
			if (!strcmp(ain->libraries[i].name, static_libraries[j]->name)) {
				libraries[i] = link_static_library(&ain->libraries[i], static_libraries[j]);
				break;
			}
		}
		if (!libraries[i])
			WARNING("Unimplemented library: %s", ain->libraries[i].name);
	}
}
