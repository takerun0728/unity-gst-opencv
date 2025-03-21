#include "native_lib.h"
#include "gst/gst.h"
#include "gst/app/gstappsink.h"

GstSample *sample;
GstBuffer *buffer;
GstMapInfo map;

int testAdd(int a, int b)
{
	return a + b;
}

