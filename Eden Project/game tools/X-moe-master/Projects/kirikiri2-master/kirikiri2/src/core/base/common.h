//---------------------------------------------------------------------------
/*
	TVP2 ( T Visual Presenter 2 )  A script authoring tool
	Copyright (C) 2000 W.Dee <dee@kikyou.info> and contributors

	See details of license at "license.txt"
*/
//---------------------------------------------------------------------------
// TVP2 common header file
//---------------------------------------------------------------------------

#ifndef __CommonH__
#define __CommonH__

#include "tjsConfig.h"

#include "config.h"

#ifdef TJS_SUPPORT_VCL
#include <vcl.h>
#endif


#include <string>


using namespace TJS;

typedef std::basic_string<tjs_char> stdstring;
typedef std::basic_string<tjs_nchar> stdnstring;



#endif

