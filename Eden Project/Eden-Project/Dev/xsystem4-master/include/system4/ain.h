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

#ifndef SYSTEM4_AIN_H
#define SYSTEM4_AIN_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

typedef uint32_t ain_addr_t;

struct string;

enum ain_error {
	AIN_SUCCESS,
	AIN_FILE_ERROR,
	AIN_UNRECOGNIZED_FORMAT,
	AIN_INVALID,
	AIN_MAX_ERROR
};

enum ain_data_type {
	AIN_VOID = 0,
	AIN_INT = 10,
	AIN_FLOAT = 11,
	AIN_STRING = 12,
	AIN_STRUCT = 13,
	AIN_ARRAY_INT = 14,
	AIN_ARRAY_FLOAT = 15,
	AIN_ARRAY_STRING = 16,
	AIN_ARRAY_STRUCT = 17,
	AIN_REF_INT = 18,
	AIN_REF_FLOAT = 19,
	AIN_REF_STRING = 20,
	AIN_REF_STRUCT = 21,
	AIN_REF_ARRAY_INT = 22,
	AIN_REF_ARRAY_FLOAT = 23,
	AIN_REF_ARRAY_STRING = 24,
	AIN_REF_ARRAY_STRUCT = 25,
	AIN_IMAIN_SYSTEM = 26,
	AIN_FUNC_TYPE = 27,
	AIN_ARRAY_FUNC_TYPE = 30,
	AIN_REF_FUNC_TYPE = 31,
	AIN_REF_ARRAY_FUNC_TYPE = 32,
	AIN_BOOL = 47,
	AIN_ARRAY_BOOL = 50,
	AIN_REF_BOOL = 51,
	AIN_REF_ARRAY_BOOL = 52,
	AIN_LONG_INT = 55,
	AIN_ARRAY_LONG_INT = 58,
	AIN_REF_LONG_INT = 59,
	AIN_REF_ARRAY_LONG_INT = 60,
	AIN_DELEGATE = 63,
	AIN_ARRAY_DELEGATE = 66,
	AIN_UNKNOWN_TYPE_67 = 67, // delegate?
	AIN_REF_ARRAY_DELEGATE = 69,
	AIN_UNKNOWN_TYPE_74 = 74, // generic array value?
	AIN_UNKNOWN_TYPE_75 = 75, // generic array value? (ref?)
	AIN_ARRAY = 79,
	AIN_REF_ARRAY = 80,
	AIN_ITERATOR = 82, // array iterator
	AIN_ENUM1 = 86, // return value of EnumType::Parse; has type 91 in array type
	AIN_UNKNOWN_TYPE_87 = 87,
	AIN_IFACE = 89, // 2 values: [0] = struct page, [1] = vtable offset to inteface methods
	AIN_ENUM2 = 91, // used as array type of type 86; has enum type in struct type
	AIN_ENUM3 = 92, // used as argument type, and array type in EnumType::GetList; has enum type in struct type
	AIN_REF_ENUM = 93,
	AIN_UNKNOWN_TYPE_95 = 95, // function?
	AIN_UNKNOWN_TYPE_100 = 100,
};

#define AIN_ARRAY_TYPE				\
	AIN_ARRAY_INT:				\
	case AIN_ARRAY_FLOAT:			\
	case AIN_ARRAY_STRING:			\
	case AIN_ARRAY_STRUCT:			\
	case AIN_ARRAY_FUNC_TYPE:		\
	case AIN_ARRAY_BOOL:			\
	case AIN_ARRAY_LONG_INT:		\
	case AIN_ARRAY_DELEGATE

#define AIN_REF_ARRAY_TYPE	       \
	AIN_REF_ARRAY_INT:	       \
	case AIN_REF_ARRAY_FLOAT:      \
	case AIN_REF_ARRAY_STRING:     \
	case AIN_REF_ARRAY_STRUCT:     \
	case AIN_REF_ARRAY_FUNC_TYPE:  \
	case AIN_REF_ARRAY_BOOL:       \
	case AIN_REF_ARRAY_LONG_INT:   \
	case AIN_REF_ARRAY_DELEGATE


#define AIN_REF_TYPE				\
	AIN_REF_INT:				\
	case AIN_REF_FLOAT:			\
	case AIN_REF_STRING:			\
	case AIN_REF_STRUCT:			\
	case AIN_REF_ARRAY_INT:			\
	case AIN_REF_ARRAY_FLOAT:		\
	case AIN_REF_ARRAY_STRING:		\
	case AIN_REF_ARRAY_STRUCT:		\
	case AIN_REF_FUNC_TYPE:			\
	case AIN_REF_ARRAY_FUNC_TYPE:		\
	case AIN_REF_BOOL:			\
	case AIN_REF_ARRAY_BOOL:		\
	case AIN_REF_LONG_INT:			\
	case AIN_REF_ARRAY_LONG_INT:		\
	case AIN_REF_ARRAY_DELEGATE:		\
	case AIN_REF_ARRAY

enum ain_variable_type {
	AIN_VAR_LOCAL,
	AIN_VAR_MEMBER
};

struct ain_type {
	enum ain_data_type data;  // data type
	int32_t struc;            // struct type (index into ain->structures)
	int32_t rank;             // array rank (if array type)
	struct ain_type *array_type;
};

struct ain_variable {
	char *name;
	char *name2;
	struct ain_type type;
	int32_t has_initval;
	union {
		char *s;
		int32_t i;
		float f;
	} initval;
	int32_t group_index;
	enum ain_variable_type var_type;
};

struct ain_function {
	ain_addr_t address;
	char *name;
	bool is_label;
	struct ain_type return_type;
	int32_t nr_args;
	int32_t nr_vars;
	int32_t is_lambda; // XXX: if this = 1, then function is a lambda; but not all lambdas have this = 1
	int32_t crc;
	int32_t struct_type;
	int32_t enum_type;
	struct ain_variable *vars;
};

struct ain_initval {
	int32_t global_index;
	int32_t data_type;
	union {
		char *string_value;
		int32_t int_value;
		float float_value;
	};
};

struct ain_interface {
	int32_t struct_type;
	int32_t uk;
};

struct ain_struct {
	char *name;
	int32_t nr_interfaces;
	struct ain_interface *interfaces;
	int32_t constructor;
	int32_t destructor;
	int32_t nr_members;
	struct ain_variable *members;
};

struct ain_hll_argument {
	char *name;
	struct ain_type type;
};

struct ain_hll_function {
	char *name;
	struct ain_type return_type;
	int32_t nr_arguments;
	struct ain_hll_argument *arguments;
};

struct ain_library {
	char *name;
	int32_t nr_functions;
	struct ain_hll_function *functions;
};

enum ain_switch_type {
	AIN_SWITCH_INT    = 2,
	AIN_SWITCH_STRING = 4,
};

struct ain_switch_case {
	int32_t value;
	int32_t address;
	struct ain_switch *parent;
};

struct ain_switch {
	enum ain_switch_type case_type;
	int32_t default_address;
	int32_t nr_cases;
	struct ain_switch_case *cases;
};

struct ain_function_type {
	char *name;
	struct ain_type return_type;
	int32_t nr_arguments;
	int32_t nr_variables;
	struct ain_variable *variables;
};

struct ain_enum {
	char *name;
	int nr_symbols;
	char **symbols;
};

struct ain_section {
	uint32_t addr;
	uint32_t size;
	bool present;
};

struct ain {
	char *ain_path;
	int32_t version;
	int32_t keycode;
	uint8_t *code;
	size_t code_size;
	int32_t nr_functions;
	struct ain_function *functions;
	int32_t nr_globals;
	struct ain_variable *globals;
	int32_t nr_initvals;
	struct ain_initval *global_initvals;
	int32_t nr_structures;
	struct ain_struct *structures;
	int32_t nr_messages;
	int32_t msg1_uk;
	struct string **messages;
	int32_t main;
	int32_t alloc;
	int32_t msgf;
	int32_t nr_libraries;
	struct ain_library *libraries;
	int32_t nr_switches;
	struct ain_switch *switches;
	int32_t game_version;
	int32_t nr_strings;
	struct string **strings;
	int32_t nr_filenames;
	char **filenames;
	int32_t ojmp;
	int32_t nr_function_types;
	int32_t fnct_size;
	struct ain_function_type *function_types;
	int32_t nr_delegates;
	int32_t delg_size;
	struct ain_function_type *delegates;
	int32_t nr_global_groups;
	char **global_group_names;
	int32_t nr_enums;
	struct ain_enum *enums;

	// file map
	struct ain_section VERS;
	struct ain_section KEYC;
	struct ain_section CODE;
	struct ain_section FUNC;
	struct ain_section GLOB;
	struct ain_section GSET;
	struct ain_section STRT;
	struct ain_section MSG0;
	struct ain_section MSG1;
	struct ain_section MAIN;
	struct ain_section MSGF;
	struct ain_section HLL0;
	struct ain_section SWI0;
	struct ain_section GVER;
	struct ain_section STR0;
	struct ain_section FNAM;
	struct ain_section OJMP;
	struct ain_section FNCT;
	struct ain_section DELG;
	struct ain_section OBJG;
	struct ain_section ENUM;

	struct hash_table *_func_ht;
	struct hash_table *_struct_ht;
};

const char *ain_strerror(int error);
const char *ain_strtype(struct ain *ain, enum ain_data_type type, int struct_type);
char *ain_strtype_d(struct ain *ain, struct ain_type *v);
const char *ain_variable_to_string(struct ain *ain, struct ain_variable *v);
uint8_t *ain_read(const char *path, long *len, int *error);
struct ain *ain_open(const char *path, int *error);
void ain_decrypt(uint8_t *buf, size_t len);

struct ain_function *ain_get_function(struct ain *ain, char *name);
int ain_get_function_index(struct ain *ain, struct ain_function *f);
struct ain_struct *ain_get_struct(struct ain *ain, char *name);
int ain_get_struct_no(struct ain *ain, char *name);
int ain_add_struct(struct ain *ain, char *name);
struct ain_variable *ain_add_global(struct ain *ain, char *name);
void ain_add_initval(struct ain *ain, struct ain_initval *init);

void ain_free(struct ain *ain);
void ain_free_functions(struct ain *ain);
void ain_free_globals(struct ain *ain);
void ain_free_initvals(struct ain *ain);
void ain_free_structures(struct ain *ain);
void ain_free_messages(struct ain *ain);
void ain_free_libraries(struct ain *ain);
void ain_free_switches(struct ain *ain);
void ain_free_strings(struct ain *ain);
void ain_free_filenames(struct ain *ain);
void ain_free_function_types(struct ain *ain);
void ain_free_delegates(struct ain *ain);
void ain_free_global_groups(struct ain *ain);
void ain_free_enums(struct ain *ain);

static inline bool ain_array_data_type(int32_t type)
{
	switch (type) {
	case AIN_ARRAY:
	case AIN_REF_ARRAY:
	case AIN_ITERATOR:
	case AIN_ENUM1:
	case AIN_UNKNOWN_TYPE_87:
		return true;
	default:
		return false;
	}
}

#endif /* SYSTEM4_AIN_H */
