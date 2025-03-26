using NUnit.Framework.Constraints;
using System;
using System.Runtime.InteropServices;
using Unity.VisualScripting;
using UnityEngine;
using UnityEngine.Rendering;

public class GstScreenUpdate : MonoBehaviour
{
    private const string LIB_NAME = "NativeGstOpenCV";
    private const int WIDTH = 1920;
    private const int HEIGHT = 1080;
    private const int PERIOD = 100;
    private const int DATA_SIZE = WIDTH * HEIGHT * 3;

    private Material _material;
    private Texture2D _texture;
    private int cnt = 0;
    private int mode = 0;

    [DllImport(LIB_NAME)]
    private static extern bool start(string gst_pipeline);
    [DllImport(LIB_NAME)]
    private static extern bool isReady();
    [DllImport(LIB_NAME)]
    private static extern IntPtr getFrame(int filter_type);
    [DllImport(LIB_NAME)]
    private static extern void unRefBuffer();
    [DllImport(LIB_NAME)]
    private static extern void lockMutex();
    [DllImport(LIB_NAME)]
    private static extern void unlockMutex();
    [DllImport(LIB_NAME)]
    private static extern int getWidth();
    [DllImport(LIB_NAME)]
    private static extern int getHeight();
    [DllImport(LIB_NAME)]
    private static extern int getDataSize();
    [DllImport(LIB_NAME)]
    private static extern void end();

    void Start()
    {　　　　　　　　　　　　　　　　　　　　　　
        _material = GetComponent<Renderer>().material;
        SetTexture(WIDTH, HEIGHT);
        bool status = start("videotestsrc ! video/x-raw,width=1920,height=1080,format=RGB ! appsink name=sink");
    }

    private void SetTexture(int width, int height)
    {
        _texture = new Texture2D(width, height, TextureFormat.RGB24, false);
        _material.mainTexture = _texture;
    }

    private void FixedUpdate()
    {
        cnt++;
        if (cnt == PERIOD)
        {
            if (mode == 0) mode = 1;
            else if (mode == 1) mode = 2;
            else if (mode == 2) mode = 0;
            cnt = 0;
        }
    }

    void Update()
    {
        lockMutex();
        if (isReady())
        {
            int width = getWidth();
            int height = getHeight();
            int size = getDataSize();

            if ((_texture.width != width) || (_texture.height != height))
                SetTexture(width, height);

            IntPtr framePtr = getFrame(mode);
            _texture.LoadRawTextureData(framePtr, size);
            _texture.Apply();
            unRefBuffer();
        }
        unlockMutex();
    }

    private void OnApplicationQuit()
    {
        end();
    }
}
