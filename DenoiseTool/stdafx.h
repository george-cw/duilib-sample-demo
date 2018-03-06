// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <string>
using namespace std;

// TODO: reference additional headers your program requires here
namespace std
{
    #ifdef _UNICODE 
        typedef wstring tstring;
    #else
        typedef string tstring;
    #endif
};

// TODO: reference additional headers your program requires here
#include "..\Duilib\UIlib.h"
// TODO: reference additional headers your program requires here
#ifdef _DEBUG
#   ifdef _UNICODE
#       pragma comment(lib, "..\\binD\\DuiLib_ud.lib")
#   else
#       pragma comment(lib, "..\\binD\\DuiLib_d.lib")
#   endif
#else
#   ifdef _UNICODE
#       pragma comment(lib, "..\\bin\\DuiLib_u.lib")
#   else
#       pragma comment(lib, "..\\bin\\DuiLib.lib")
#   endif
#endif

using namespace DuiLib;


#include "CommonFunction.h"
#include "Resource.h"