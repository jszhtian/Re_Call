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

#ifndef SYSTEM4_HLL_H
#define SYSTEM4_HLL_H

/*
 * DSL for implementing libraries.
 */

#include "vm.h"
#include "system4.h"

#define HLL_UNIMPLEMENTED(rtype, libname, fname, ...)		\
	static rtype libname ## _ ## fname(__VA_ARGS__) {	\
		VM_ERROR("Unimplemented HLL function: " #libname "." #fname); \
	}

#define HLL_WARN_UNIMPLEMENTED(rval, rtype, libname, fname, ...)\
	static rtype libname ## _ ## fname(__VA_ARGS__) {	\
		WARNING("Unimplemented HLL function: " #libname "." #fname); \
		return rval;					\
	}

#define HLL_EXPORT(fname, funptr) { .name = #fname, .fun = funptr }

#define HLL_LIBRARY(lname, ...)				\
	struct static_library lib_ ## lname = {		\
		.name = #lname,				\
		.functions = {				\
			__VA_ARGS__,			\
			{ .name = NULL, .fun = NULL }	\
		}					\
	}

#endif /* SYSTEM4_HLL_H */
