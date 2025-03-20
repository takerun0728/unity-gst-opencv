using System.Runtime.InteropServices;
using UnityEngine;

public class GstScreenUpdate : MonoBehaviour
{
    [DllImport("NativeGstOpenCV")]
    private static extern int testAdd(int a, int b);

    void Start()
    {
        
    }

    void Update()
    {
        int a = testAdd(1, 2);
        a++;
        Debug.Log(a);
    }
}
