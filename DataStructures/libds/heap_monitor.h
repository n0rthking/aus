#pragma once

#if (defined(_WIN32) || defined(_WIN64)) \
    && !(defined(__MINGW32__) || defined(__MINGW64__))
#define LIBDS_USE_HEAP_MONITOR
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#define new new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )

#define initHeapMonitor() _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF)

#endif