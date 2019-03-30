#pragma once

#if defined(_WIN32) || defined(_WIN64)
#if defined(API_LIB)
#define API_EXPORT __declspec(dllexport)
#else
#define API_EXPORT __declspec(dllimport)
#endif
#else
#define API_EXPORT
#endif