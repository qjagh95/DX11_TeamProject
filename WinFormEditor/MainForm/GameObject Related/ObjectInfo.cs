using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WinFormEditor
{
    public class ObjectInfo
    {
        // Mesh
        public MeshInfo meshInfo    { set; get; }

        // Vector3
        public Vector3 vecLScale    { set; get; }
        public Vector3 vecLRotate   { set; get; }
        public Vector3 vecLPosition { set; get; }
        public Vector3 vecWScale    { set; get; }
        public Vector3 vecWRotate   { set; get; }
        public Vector3 vecWPosition { set; get; }
        public Vector3 vecWPivot    { set; get; }
        public Vector4 vecColor     { set; get; }

        // Member
        public string strLayerTag   { set; get; }

        public class MeshInfo
        {
            public string m_strMeshName { set; get; }
            public MeshInfo(string _str)
            {
                m_strMeshName = _str;
            }
        }

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

            public Vector3(float[] _arrF)
            {
                x = _arrF[0];
                y = _arrF[1];
                z = _arrF[2];
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

        public void ClearData()
        {
            strLayerTag = "";
            Vector3[] arrVector = { vecWScale, vecWRotate, vecWPosition };
            for (int i = 0; i < 3; ++i)
            {
                arrVector[i].x = 0.0f;
                arrVector[i].y = 0.0f;
                arrVector[i].z = 0.0f;
            }
        }
    }
}