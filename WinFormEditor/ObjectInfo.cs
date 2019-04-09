using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WinFormEditor
{
    public class ObjectInfo
    {
        public class Vector3
        {
            public float x = 0.0f;
            public float y = 0.0f;
            public float z = 0.0f;

            public Vector3(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f)
            {
                x = _x;
                y = _y;
                z = _z;
            }
        }

        public class Vector4
        {
            public float x = 0.0f;
            public float y = 0.0f;
            public float z = 0.0f;
            public float w = 0.0f;

            public Vector4(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f, float _w = 0.0f)
            {
                x = _x;
                y = _y;
                z = _z;
                w = _w;
            }
        }

        public string  strLayerTag  { set; get; }
        public Vector3 vecScale     { set; get; }
        public Vector3 vecRotate    { set; get; }
        public Vector3 vecPosition  { set; get; }
        public Vector4 vecColor     { set; get; }

        public void ClearData()
        {
            strLayerTag = "";
            Vector3[] arrVector = { vecScale, vecRotate, vecPosition };
            for(int i = 0; i < 3; ++i)
            {
                arrVector[i].x = 0.0f;
                arrVector[i].y = 0.0f;
                arrVector[i].z = 0.0f;
            }
        }
    }
}