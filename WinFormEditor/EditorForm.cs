using System;
using System.IO;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using EngineWrapper;

namespace WinFormEditor
{
    public partial class EditorForm : Form
    {
        /////////////////////////////////////////////////////////////////////////
        public CoreWrapper coreWrapper = new CoreWrapper();

        public static string m_strChangeObjTag;
        public static string m_strLayerTag;
        public static int    m_iObjListIndex;
        public static string m_strPositionX;
        public static string m_strPositionY;
        public static string m_strPositionZ;
        public static string m_strScaleX;
        public static string m_strScaleY;
        public static string m_strScaleZ;
        public static string m_strRotX;
        public static string m_strRotY;
        public static string m_strRotZ;
        public static string m_strClipName;
        public static string m_strStartFrame;
        public static string m_strEndFrame;
        public static string m_strAniTime;

        private void Run(object sender, EventArgs e)
        {
            while (this.Created == true)
            {
                coreWrapper.Logic();
                Application.DoEvents();
            }
        }

        public EditorForm()
        {

            InitializeComponent();

            //hInstance, hwnd얻어옴.
            IntPtr hinstance = System.Runtime.InteropServices.Marshal.GetHINSTANCE(DeviceWindow.GetType().Module);
            IntPtr DeviceHwnd = DeviceWindow.Handle;
            coreWrapper.Init(DeviceHwnd, hinstance);

            //Run함수 연결
            Application.Idle += Run;
        }

        private void Delete(object sender, FormClosedEventArgs e)
        {
            coreWrapper.Delete();
        }

        private void groupBox1_Enter(object sender, EventArgs e)
        {
     
        }

        private void EditorForm_Load(object sender, EventArgs e)
        {
            LayerList.Items.Add("Stage");
            LayerList.Items.Add("Default");
            LayerList.Items.Add("UI");

            AnimationOptionBox.Items.Add("AO_LOOP");
            AnimationOptionBox.Items.Add("AO_ONCE_DESTROY");

            // ResourcManager에서 생성된 메시 목록을 불러온다.
            string[] arrString = coreWrapper.GetMeshNameList();
            for (int i = 0; i < arrString.Length; ++i)
            {
                MeshList.Items.Add(arrString[i].ToString());
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
           coreWrapper.EditCreateObject("NewObject" , m_strLayerTag);
           ObjList.Items.Add("NewObject");
        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {
            string ss = ObjTagText.Text.ToString();
            m_strChangeObjTag = ss;
        }

        private void LayerList_SelectedIndexChanged(object sender, EventArgs e)
        {
            ObjList.Items.Clear();

            m_strLayerTag = (string)LayerList.SelectedItem;

            coreWrapper.CurrentLayerGetObjTag(m_strLayerTag);

            int iSize = coreWrapper.GetVecListObjSize();

            for(int i = 0; i< iSize; ++i)
            {
                string ss = coreWrapper.GetIndexFromObjListTag(i);
                ObjList.Items.Add(ss);
            }
        }

        private void ObjList_SelectedIndexChanged(object sender, EventArgs e)
        {
            int iIndex = ObjList.SelectedIndex;
            coreWrapper.SetIndexFromActiveObj(iIndex, m_strLayerTag);
        }

        private void PositionX_TextChanged(object sender, EventArgs e)
        {
            string tempstr = PositionX.Text.ToString();
            m_strPositionX = tempstr;

            if (m_strPositionX.Length == 1 && m_strPositionX[0] == '-')
            {
                return;
            }

            if (m_strPositionX == "")
            {
                return;
            }
            double dPosX = Convert.ToDouble(m_strPositionX);
            double dPosY = Convert.ToDouble(m_strPositionY);
            double dPosZ = Convert.ToDouble(m_strPositionZ);
            coreWrapper.ActiveObjSetPos(dPosX, dPosY, dPosZ);
        }

        private void PositionY_TextChanged(object sender, EventArgs e)
        {
            string tempstr = PositionY.Text.ToString();
            m_strPositionY = tempstr;

            if (m_strPositionY.Length == 1 && m_strPositionY[0] == '-')
            {
                return;
            }

            if (m_strPositionY == "")
            {
                return;
            }

            double dPosX = Convert.ToDouble(m_strPositionX);
            double dPosY = Convert.ToDouble(m_strPositionY);
            double dPosZ = Convert.ToDouble(m_strPositionZ);
            coreWrapper.ActiveObjSetPos(dPosX, dPosY, dPosZ);
        }

        private void PositionZ_TextChanged(object sender, EventArgs e)
        {
            string tempstr = PositionZ.Text.ToString();
            m_strPositionZ = tempstr;

            if (m_strPositionZ.Length == 1 && m_strPositionZ[0] == '-')
            {
                return;
            }

            if (m_strPositionZ == "")
            {
                return;
            }
            double dPosX = Convert.ToDouble(m_strPositionX);
            double dPosY = Convert.ToDouble(m_strPositionY);
            double dPosZ = Convert.ToDouble(m_strPositionZ);
            coreWrapper.ActiveObjSetPos(dPosX, dPosY, dPosZ);
        }

        private void Transform_Press(object sender, KeyPressEventArgs e)
        {
            if ((e.KeyChar >= (char)48 && e.KeyChar <= (char)57) ||  e.KeyChar == (char)46
                  || e.KeyChar == (char)8 || e.KeyChar == (char)127)
            {

            }
            else
            {
                e.Handled = true;
            }
        }

        private void ScaleX_TextChanged(object sender, EventArgs e)
        {
            string tempstr = ScaleX.Text.ToString();
            m_strScaleX = tempstr;

            if (m_strScaleX.Length == 1 && m_strScaleX[0] == '-')
            {
                return;
            }

            if (m_strScaleX == "")
            {
                m_strScaleX = "0";
            }
            double dScaleX = Convert.ToDouble(m_strScaleX);
            double dScaleY = Convert.ToDouble(m_strScaleY);
            double dScaleZ = Convert.ToDouble(m_strScaleZ);
            coreWrapper.ActiveObjSetScale(dScaleX, dScaleY, dScaleZ);
        }

        private void ScaleY_TextChanged(object sender, EventArgs e)
        {
            string tempstr = ScaleY.Text.ToString();
            m_strScaleY = tempstr;
       
            if (m_strScaleY.Length == 1 && m_strScaleY[0] == '-')
            {
                return;
            }


            if (m_strScaleY == "")
            {
                return;
            }
            double dScaleX = Convert.ToDouble(m_strScaleX);
            double dScaleY = Convert.ToDouble(m_strScaleY);
            double dScaleZ = Convert.ToDouble(m_strScaleZ);
            coreWrapper.ActiveObjSetScale(dScaleX, dScaleY, dScaleZ);
        }

        private void ScaleZ_TextChanged(object sender, EventArgs e)
        {
            string tempstr = ScaleZ.Text.ToString();
            m_strScaleZ = tempstr;

            if (m_strScaleZ.Length == 1 && m_strScaleZ[0] == '-')
            {
                return;
            }


            if (m_strScaleZ == "")
            {
                return;
            }
            double dScaleX = Convert.ToDouble(m_strScaleX);
            double dScaleY = Convert.ToDouble(m_strScaleY);
            double dScaleZ = Convert.ToDouble(m_strScaleZ);
            coreWrapper.ActiveObjSetScale(dScaleX, dScaleY, dScaleZ);
        }

        private void RotationX_TextChanged(object sender, EventArgs e)
        {
            string tempstr = RotationX.Text.ToString();
            m_strRotX = tempstr;

            if (m_strRotX.Length == 1 && m_strRotX[0] == '-')
            {
                return;
            }


            if (m_strRotX == "")
            {
                return;
            }

            double dRotX = Convert.ToDouble(m_strRotX);
            double dRotY = Convert.ToDouble(m_strRotY);
            double dRotZ = Convert.ToDouble(m_strRotZ);
            coreWrapper.ActiveObjSetRot(dRotX, dRotY, dRotZ);
        }

        private void RotationY_TextChanged(object sender, EventArgs e)
        {
            string tempstr = RotationY.Text.ToString();
            m_strRotY = tempstr;

            if (m_strRotY.Length == 1 && m_strRotY[0] == '-')
            {
                return;
            }


            if (m_strRotY == "")
            {
                return;
            }

            double dRotX = Convert.ToDouble(m_strRotX);
            double dRotY = Convert.ToDouble(m_strRotY);
            double dRotZ = Convert.ToDouble(m_strRotZ);
            coreWrapper.ActiveObjSetRot(dRotX, dRotY, dRotZ);
        }

        private void RotationZ_TextChanged(object sender, EventArgs e)
        {
            string tempstr = RotationZ.Text.ToString();
            m_strRotZ = tempstr;

            if (m_strRotZ.Length == 1 && m_strRotZ[0] == '-')
            {
                return;
            }


            if (m_strRotZ == "")
            {
                return;
            }

            double dRotX = Convert.ToDouble(m_strRotX);
            double dRotY = Convert.ToDouble(m_strRotY);
            double dRotZ = Convert.ToDouble(m_strRotZ);
            coreWrapper.ActiveObjSetRot(dRotX, dRotY, dRotZ);
        }

        private void MeshList_SelectedIndexChanged(object sender, EventArgs e)
        {
            // Mesh ListBox안에 선택된 목록이 바뀔 때 호출된다.

        }

        private void SetMesh_Click(object sender, EventArgs e)
        {
            // '메시 등록' 버튼을 클릭 시 호출된다.
            string strTemp = MeshList.SelectedItem.ToString();
            coreWrapper.SetMesh(strTemp);
        }

        private void AddRenderComponent(object sender, EventArgs e)
        {
            // 'Renderer' 컴포넌트 추가
            coreWrapper.AddRenderComponent();
        }

        private void ChangeTag_Click(object sender, EventArgs e)
        {
            coreWrapper.SelectObjChangeTag(m_strChangeObjTag);
            ObjList.Items[ObjList.SelectedIndex] = m_strChangeObjTag;
            ObjTagText.Clear();
        }

        private void AnimationOption_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void DivideClipBox_CheckedChanged(object sender, EventArgs e)
        {
        }

        private void AnimationTagBox_TextChanged(object sender, EventArgs e)
        {
            string tempstr = AnimationTagBox.Text.ToString();
            m_strClipName = tempstr;
        }

        private void AniStartFrameBox_TextChanged(object sender, EventArgs e)
        {
            string tempstr = AniStartFrameBox.Text.ToString();
            m_strStartFrame = tempstr;
        }

        private void AniEndFrameBox_TextChanged(object sender, EventArgs e)
        {
            string tempstr = AniEndFrameBox.Text.ToString();
            m_strEndFrame = tempstr;
        }

        private void AniTimeBox_TextChanged(object sender, EventArgs e)
        {
            string tempstr = AniTimeBox.Text.ToString();
            m_strAniTime = tempstr;
        }

        private void ClipSave_Click(object sender, EventArgs e)
        {

        }

        private void ClipLoad_Click(object sender, EventArgs e)
        {
            string filePath;
            OpenFileDialog opdig = new OpenFileDialog();

            opdig.InitialDirectory = Directory.GetCurrentDirectory();    
            opdig.Filter = "ClipFile(*.anm)|*.anm|FbxFile(*.fbx)|*.fbx|모든파일(*.*)|*.*||";
            opdig.RestoreDirectory = true;

            if(opdig.ShowDialog() == DialogResult.OK)
            {
                filePath = opdig.FileName;

                coreWrapper.LoadClipFromFullPath(filePath);

                string[] arrString = coreWrapper.GetClipTagList();
                for (int i = 0; i < arrString.Length; ++i)
                {
                    ClipList.Items.Add(arrString[i].ToString());
                }

            }
        }

        private void FbxLoad_Click(object sender, EventArgs e)
        {
            string filePath;
            string fileName;
            OpenFileDialog opdig = new OpenFileDialog();

            opdig.InitialDirectory = Directory.GetCurrentDirectory();
            opdig.Filter = "ClipFile(*.anm)|*.anm|FbxFile(*.fbx)|*.fbx|모든파일(*.*)|*.*||";
            opdig.RestoreDirectory = true;

            if (opdig.ShowDialog() == DialogResult.OK)
            {
                filePath = opdig.FileName;
                fileName = Path.GetFileNameWithoutExtension(filePath);

                coreWrapper.LoadMeshFromFullPath(fileName, filePath);
                MeshList.Items.Add(fileName);
            }
        }

        private void MshSave_Click(object sender, EventArgs e)
        {

        }

        private void MshLoad_Click(object sender, EventArgs e)
        {
            string filePath;
            string fileName;
            OpenFileDialog opdig = new OpenFileDialog();

            opdig.InitialDirectory = Directory.GetCurrentDirectory();
            opdig.Filter = "MeshFile(*.msh)|*.msh|모든파일(*.*)|*.*||";
            opdig.RestoreDirectory = true;

            if (opdig.ShowDialog() == DialogResult.OK)
            {
                filePath = opdig.FileName;
                fileName = Path.GetFileNameWithoutExtension(filePath);

                coreWrapper.LoadMeshFromFullPath(fileName , filePath);
                MeshList.Items.Add(fileName);
            }
        }

        private void Scene_Click(object sender, MouseEventArgs e)
        {
            DeviceWindow.Focus();
        }
    }
}