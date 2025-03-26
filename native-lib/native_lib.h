#pragma once

#ifdef _WIN32
#define EXPORT_API __declspec(dllexport)
#else
#define EXPORT_API
#endif

extern "C"
{
EXPORT_API bool start(const char* gst_pipeline);
EXPORT_API bool isReady();
EXPORT_API unsigned char* getFrame(int filter_type);
EXPORT_API void unRefBuffer();
EXPORT_API void lockMutex();
EXPORT_API void unlockMutex();
EXPORT_API int getWidth();
EXPORT_API int getHeight();
EXPORT_API int getDataSize();
EXPORT_API void end();
}