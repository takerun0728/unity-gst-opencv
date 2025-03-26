#include "native_lib.h"
#include "gst/gst.h"
#include "gst/app/gstappsink.h"
#include "opencv2/opencv.hpp"

#ifdef __ANDROID__
extern "C"
{
	GST_PLUGIN_STATIC_DECLARE(coreelements);
	GST_PLUGIN_STATIC_DECLARE(app);
	GST_PLUGIN_STATIC_DECLARE(videotestsrc);
}

void register_gst_plugins() {
    GST_PLUGIN_STATIC_REGISTER(coreelements);
    GST_PLUGIN_STATIC_REGISTER(app);
    GST_PLUGIN_STATIC_REGISTER(videotestsrc);
}
#else
void register_gst_plugins() {}
#endif

GstElement* g_pipeline;
GstElement* g_sink;
GstBuffer* g_buffer;
std::mutex g_mutex;
bool g_is_ready;
int g_width;
int g_height;
int g_size;

static GstFlowReturn on_new_sample(GstElement *sink, gpointer user_data)
{
	GstSample* sample = gst_app_sink_pull_sample(GST_APP_SINK(sink));
	if (!sample) return GST_FLOW_ERROR;
	{
		std::lock_guard<std::mutex> lock(g_mutex);
		unRefBuffer();
		g_buffer = gst_buffer_ref(gst_sample_get_buffer(sample));
		GstCaps* caps = gst_sample_get_caps(sample);
		GstStructure* s = gst_caps_get_structure(caps, 0);
		gst_structure_get_int(s, "width", &g_width);
		gst_structure_get_int(s, "height", &g_height);
		gst_sample_unref(sample);
		g_size = g_width * g_height * 3;
		g_is_ready = true;
	}
	
	return GST_FLOW_OK;
}

bool start(const char* gst_pipeline)
{
	gst_init(nullptr, nullptr);
	register_gst_plugins();

	g_pipeline = gst_parse_launch(gst_pipeline, nullptr);
	if (!g_pipeline) {
		return false;
	}
	g_sink = gst_bin_get_by_name(GST_BIN(g_pipeline), "sink");
    g_object_set(g_sink, "emit-signals", TRUE, "sync", FALSE, nullptr);
    g_signal_connect(g_sink, "new-sample", G_CALLBACK(on_new_sample), nullptr);
    gst_element_set_state(g_pipeline, GST_STATE_PLAYING);
	
	return true;
}

//from isReady to unRefBuffer must be enclosed by lock and unlock
bool isReady()
{
	return g_is_ready;
}

unsigned char* getFrame(int filter_type) //0: None, 1: Edge, Must be called if isReady is True
{
	GstMapInfo map;
	gst_buffer_map(g_buffer, &map, GST_MAP_READ);

	if (filter_type)
	{
		cv::Mat mat(cv::Size(g_width, g_height), CV_8UC3, (char*)map.data);
		switch (filter_type)
		{
			case 1:
				cv::bitwise_not(mat, mat);
				break;
			case 2:
				cv::blur(mat, mat, cv::Size(10, 10));
				break;
		}
	}
	
	gst_buffer_unmap(g_buffer, &map);
	return (unsigned char*)map.data;
}

void unRefBuffer()
{
	if (g_is_ready) gst_buffer_unref(g_buffer);
	g_is_ready = false;
}

void lockMutex()
{
	g_mutex.lock();
}

void unlockMutex()
{
	g_mutex.unlock();
}

int getWidth()
{
	return g_width;
}

int getHeight()
{
	return g_height;
}

int getDataSize()
{
	return g_size;
}

void end()
{
	gst_element_set_state(g_pipeline, GST_STATE_NULL);
	gst_object_unref(g_pipeline);
	gst_object_unref(g_sink);
	unRefBuffer();
}