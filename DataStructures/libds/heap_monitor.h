#pragma once

/**
 * @brief Creates default-constructed object at @p address.
 */
template<typename T>
T* placement_new(T* address)
{
	return new (address) T();
}

/**
 * @brief Creates copy-constructed object from @p origin at @p address.
 */
template<typename T>
T* placement_copy(T* address, const T& origin)
{
	return new (address) T(origin);
}

#if (defined(_WIN32) || defined(_WIN64)) \
    && !(defined(__MINGW32__) || defined(__MINGW64__))
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#define new new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )

#define initHeapMonitor() _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF)

#endif