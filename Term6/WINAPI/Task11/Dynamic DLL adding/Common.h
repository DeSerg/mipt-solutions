#pragma once

#include <Windows.h>
#include <cstdlib>
#include <string.h>
#include <tchar.h>
#include "windef.h"
#include "shlwapi.h"
#include <Winuser.h>
#include <Winbase.h>
#include <iostream>
#include <Richedit.h>
#include <windef.h>
#include <CommCtrl.h>
#include <sstream>
#include <cctype>

#include "resource.h"

#ifdef UNICODE
typedef std::wostringstream tstringstream;
#else
typedef std::ostringstream tstringstream;
#endif

typedef int(__cdecl *MYPROC)(LPWSTR);