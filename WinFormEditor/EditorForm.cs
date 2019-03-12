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
        public static string m_strDeleteClip;

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

            DirectoryInfo Pat;
            string FullPath = Directory.GetCurrentDirectory();
            Pat = Directory.GetParent(FullPath).Parent.Parent;

            FullPath = Pat.FullName;
            FullPath += "\\3DClient\\Bin\\MeshData\\";

            foreach (string f in Directory.GetFiles(FullPath, "*.msh"))
            {
                coreWrapper.LoadMeshFromFullPath(Path.GetFileNameWithoutExtension(f), f);
                MeshList.Items.Add(Path.GetFileNameWithoutExtension(f));
            }
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
            AnimationOptionBox.Items.Add("AO_ONCE_RETURN");
            AnimationOptionBox.Items.Add("AO_ONCE_DESTROY");

            // ResourcManager에서 생성된 메시 목록을 불러온다.
            string[] arrString = coreWrapper.GetMeshNameList();
            for (int i = 0; i < arrString.Length; ++i)
            {
                MeshList.Items.Add(arrString[i].ToString());
            }
        }

        //Create Object
        private void button1_Click(object sender, EventArgs e)
        {
            if(LayerList.SelectedItem == null)
            {
                AddLogString("Error! 선택된 Layer가 없습니다.");
                return;
            }

            coreWrapper.EditCreateObject("NewObject", m_strLayerTag);
            ObjList.Items.Add("NewObject");
            string strTemp = m_strLayerTag;

            strTemp += "Layer에 Object를 생성합니다.";
            AddLogString(strTemp);
        }

        //Object Change Tag
        private void textBox1_TextChanged(object sender, EventArgs e)
        {
            string ss = ObjTagText.Text.ToString();
            m_strChangeObjTag = ss;
        }

        private void LayerList_SelectedIndexChanged(object sender, EventArgs e)
        {
            string strTemp = m_strLayerTag;

            ObjList.Items.Clear();
            strTemp += "Layer 에서";
            m_strLayerTag = (string)LayerList.SelectedItem;

            strTemp += m_strLayerTag;
            strTemp += "Layer로 변경합니다.";

            AddLogString(strTemp);

            coreWrapper.CurrentLayerGetObjTag(m_strLayerTag);

            int iSize = coreWrapper.GetVecListObjSize();

            for (int i = 0; i < iSize; ++i)
            {
                string ss = coreWrapper.GetIndexFromObjListTag(i);
                ObjList.Items.Add(ss);
            }
        }

        private void ObjList_SelectedIndexChanged(object sender, EventArgs e)
        {
            if(ObjList.SelectedItem == null)
            {
                AddLogString("Error! 선택된 Object가 없습니다.");
                return;
            }
            int iIndex = ObjList.SelectedIndex;
            coreWrapper.SetIndexFromActiveObj(iIndex, m_strLayerTag);

            AddLogString("활성화된 Object가 " + ObjList.SelectedItem.ToString() + " 로 변경됩니다.");
        }

        private void PositionX_TextChanged(object sender, EventArgs e)
        {
            string tempstr = PositionX.Text.ToString();
            m_strPositionX = tempstr;

            if (m_strPositionX.Length == 1 && m_strPositionX[0] == '-')
            {
                AddLogString("PositionX Text Box에 잘못된 입력이 들어갔습니다.");
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
                AddLogString("Error! PositionY Text Box에 잘못된 입력이 들어갔습니다.");
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
                AddLogString("Error! PositionZ Text Box에 잘못된 입력이 들어갔습니다.");
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
            if ((e.KeyChar >= (char)48 && e.KeyChar <= (char)57) || e.KeyChar == (char)46
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
                AddLogString("Error! ScaleX Text Box에 잘못된 입력이 들어갔습니다.");
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
                AddLogString("Error! ScaleY Text Box에 잘못된 입력이 들어갔습니다.");
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
                AddLogString("Error! ScaleZ Text Box에 잘못된 입력이 들어갔습니다.");
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
                AddLogString("Error! RotationX Text Box에 잘못된 입력이 들어갔습니다.");
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
                AddLogString("Error! RotationY Text Box에 잘못된 입력이 들어갔습니다.");
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
                AddLogString("Error! RotationZ Text Box에 잘못된 입력이 들어갔습니다.");
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
            if(MeshList.SelectedItem == null)
            {
                AddLogString("Error! 선택된 Mesh가 없습니다");
                return;
            }
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
           if(ObjList.SelectedItem == null)
            {
                AddLogString("Error! 선택된 Object가 없습니다");
                return;
            }

            coreWrapper.SelectObjChangeTag(m_strChangeObjTag);
            ObjList.Items[ObjList.SelectedIndex] = m_strChangeObjTag;
            ObjTagText.Clear();
        }

        private void AnimationOption_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void DivideClipBox_CheckedChanged(object sender, EventArgs e)
        {
            if (DivideClipBox.Checked == true)
            {
                coreWrapper.SetDivideClip();
            }
            else
            {
                coreWrapper.DeleteDivideClip();
            }
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
            string strFilePath;
            SaveFileDialog SaveDiag = new SaveFileDialog();

            DirectoryInfo Pat;
            string FullPath = Directory.GetCurrentDirectory();
            Pat = Directory.GetParent(FullPath).Parent.Parent;

            FullPath = Pat.FullName;
            FullPath += "\\3DClient\\Bin\\MeshData\\";
            SaveDiag.InitialDirectory = FullPath;
            SaveDiag.Filter = "ClipFile(*.anm)|*.anm|FbxFile(*.fbx)|*.fbx|모든파일(*.*)|*.*||";
            SaveDiag.RestoreDirectory = true;

            if (SaveDiag.ShowDialog() == DialogResult.OK)
            {
                strFilePath = SaveDiag.FileName;

                coreWrapper.ClipSave(strFilePath);
            }
        }

        private void ClipLoad_Click(object sender, EventArgs e)
        {
            string filePath;
            OpenFileDialog opdig = new OpenFileDialog();

            DirectoryInfo Pat;
            string FullPath = Directory.GetCurrentDirectory();
            Pat = Directory.GetParent(FullPath).Parent.Parent;

            FullPath = Pat.FullName;
            FullPath += "\\3DClient\\Bin\\MeshData\\";
            opdig.InitialDirectory = FullPath;
            opdig.Filter = "ClipFile(*.anm)|*.anm|FbxFile(*.fbx)|*.fbx|모든파일(*.*)|*.*||";
            opdig.RestoreDirectory = true;

            if (opdig.ShowDialog() == DialogResult.OK)
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

        private void MshLoad_Click(object sender, EventArgs e)
        {
            string filePath;
            string fileName;
            OpenFileDialog opdig = new OpenFileDialog();

            DirectoryInfo Pat;
            string FullPath = Directory.GetCurrentDirectory();
            Pat = Directory.GetParent(FullPath).Parent.Parent;

            FullPath = Pat.FullName;
            FullPath += "\\3DClient\\Bin\\MeshData\\";
            opdig.InitialDirectory = FullPath;
            opdig.Filter = "MeshFile(*.msh)|*.msh|모든파일(*.*)|*.*||";
            opdig.RestoreDirectory = true;

            if (opdig.ShowDialog() == DialogResult.OK)
            {
                filePath = opdig.FileName;
                fileName = Path.GetFileNameWithoutExtension(filePath);

                coreWrapper.LoadMeshFromFullPath(fileName, filePath);
                MeshList.Items.Add(fileName);
            }
        }

        private void Scene_Click(object sender, MouseEventArgs e)
        {
            DeviceWindow.Focus();
        }

        private void AddClipBtn_Click(object sender, EventArgs e)
        {
            if (DivideClipBox.Checked == true)
            {
                int iAnimationOption = AnimationOptionBox.SelectedIndex;

                int iStartFrame = Convert.ToInt32(m_strStartFrame);
                int iEndFrame = Convert.ToInt32(m_strEndFrame);
                double dPlayTime = Convert.ToDouble(m_strAniTime);

                // ListBox의 이름을 변경해주면 된다.
                if (coreWrapper.AddClip(m_strClipName, iAnimationOption, iStartFrame, iEndFrame, dPlayTime) == true)
                {
                    ClipList.Items.Add(m_strClipName);
                }
            }
        }

        private void ModifyClipBtn_Click(object sender, EventArgs e)
        {
            if (DivideClipBox.Checked == true)
            {
                int iIndex = ClipList.SelectedIndex;

                string strOriginName = ClipList.SelectedItem.ToString();

                string strChangeName = m_strClipName;

                int iAnimationOption = AnimationOptionBox.SelectedIndex;

                int iStartFrame = Convert.ToInt32(m_strStartFrame);
                int iEndFrame = Convert.ToInt32(m_strEndFrame);
                double dPlayTime = Convert.ToDouble(m_strAniTime);



                // ListBox의 이름을 변경해주면 된다.
                if (coreWrapper.ModifyClip(strOriginName, strChangeName, iAnimationOption, iStartFrame, iEndFrame, dPlayTime) == true)
                {
                    ClipList.Items.Remove(ClipList.SelectedItem);
                    ClipList.Items.Add(m_strClipName);
                }
            }
        }

        private void ClipList_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (ClipList.SelectedItem == null)
            {
                AddLogString("Error! 선택된 Index에 Clip 항목이 없습니다.");
                return;
            }
            
            coreWrapper.ChangeClip(ClipList.SelectedItem.ToString());

            string strTemp = ClipList.SelectedItem.ToString();
            strTemp += " Clip으로 변경 되었습니다.";

            AddLogString(strTemp);

            m_strDeleteClip = ClipList.SelectedItem.ToString();
            DeleteClipText.Text = m_strDeleteClip;
        }

        private void DeleteClipText_TextChanged(object sender, EventArgs e)
        {
            string tempstr = DeleteClipText.Text.ToString();
            m_strDeleteClip = tempstr;
        }

        private void DeleteClipBtn_Click(object sender, EventArgs e)
        {
            if(ClipList.SelectedItem == null || ClipList.SelectedItem.ToString() == "")
            {
                AddLogString("Error! DeleteClip Box에 Text 정보가 없습니다.");
                return;
            }

            if(ClipList.Items.Count == 1)
            {
                AddLogString("Error! AnimationClip은 1개이하로 삭제할 수 없습니다.");
                return;
            }

            coreWrapper.DeleteClip(ClipList.SelectedItem.ToString());
            ClipList.Items.Remove(ClipList.SelectedItem);
        }

        void AddLogString(string _strLog)
        {
            LogMessage.Items.Insert(0 , _strLog);
        }
    }
}