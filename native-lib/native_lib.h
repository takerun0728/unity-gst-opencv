﻿#pragma once

#ifdef _WIN32
#define EXPORT_API __declspec(dllexport)
#else
#define EXPORT_API __attribute__((visibility("default")))
#endif

extern "C" EXPORT_API int testAdd(int a, int b);
