// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

#define _USE_MATH_DEFINES
#include <math.h>

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>
#include "WinSock2.h"
#include <WS2tcpip.h>

// C RunTime Header Files
#include <iostream>
#include <string>

#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include "cprocessing.h"

#endif //PCH_H
