#pragma once

#include <winsdkver.h>
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x0601

#include <afxwin.h>
#include <Box2D\Box2D.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include <random>
#include <memory>

#ifndef __cplusplus_winrt
struct Exception
{
	HRESULT result;
	explicit Exception(HRESULT const value) : result(value) {}
};
#endif

inline void HR(HRESULT const result)
{
	ASSERT(S_OK == result);
	if (S_OK != result)
#ifndef __cplusplus_winrt
		throw Exception(result);
#else
		throw Platform::Exception::CreateException(result);
#endif
}