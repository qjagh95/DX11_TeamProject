using System;
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

        public static string m_strCreateObjTag;
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

            // ResourcManager에서 생성된 메시 목록을 불러온다.
            string[] arrString = coreWrapper.GetMeshNameList();
            for (int i = 0; i < arrString.Length; ++i)
            {
                MeshList.Items.Add(arrString[i].ToString());
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
           coreWrapper.EditCreateObject(m_strCreateObjTag , m_strLayerTag);
           ObjList.Items.Add(m_strCreateObjTag);
        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {
            string ss = textBox1.Text.ToString();
            m_strCreateObjTag = ss;
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
            string ss = PositionX.Text.ToString();
            m_strPositionX = ss;
        }

        private void PositionY_TextChanged(object sender, EventArgs e)
        {
            string ss = PositionY.Text.ToString();
            m_strPositionY = ss;
        }

        private void PositionZ_TextChanged(object sender, EventArgs e)
        {
            string ss = PositionZ.Text.ToString();
            m_strPositionZ = ss;
        }

        private void button2_Click(object sender, EventArgs e)
        {
            double dPosX = Convert.ToDouble(m_strPositionX);
            double dPosY = Convert.ToDouble(m_strPositionY);
            double dPosZ = Convert.ToDouble(m_strPositionZ);
            coreWrapper.ActiveObjSetPos(dPosX, dPosY, dPosZ);
        }

        private void PositionX_Press(object sender, KeyPressEventArgs e)
        {
            //if (!(char.IsDigit(e.KeyChar) || e.KeyChar == Convert.ToChar(Keys.Back)))
            //{
            //    e.Handled = true;
            //}
        }

        private void ScaleX_TextChanged(object sender, EventArgs e)
        {
            string ss = ScaleX.Text.ToString();
            m_strScaleX = ss;
        }

        private void ScaleY_TextChanged(object sender, EventArgs e)
        {
            string ss = ScaleY.Text.ToString();
            m_strScaleY = ss;
        }

        private void ScaleZ_TextChanged(object sender, EventArgs e)
        {
            string ss = ScaleZ.Text.ToString();
            m_strScaleZ = ss;
        }

        private void RotationX_TextChanged(object sender, EventArgs e)
        {
            string ss = RotationX.Text.ToString();
            m_strRotX = ss;
        }

        private void RotationY_TextChanged(object sender, EventArgs e)
        {
            string ss = RotationY.Text.ToString();
            m_strRotY = ss;
        }

        private void RotationZ_TextChanged(object sender, EventArgs e)
        {
            string ss = RotationZ.Text.ToString();
            m_strRotZ = ss;
        }

        private void SetScale_Click(object sender, EventArgs e)
        {
            double dScaleX = Convert.ToDouble(m_strScaleX);
            double dScaleY = Convert.ToDouble(m_strScaleY);
            double dScaleZ = Convert.ToDouble(m_strScaleZ);
            coreWrapper.ActiveObjSetScale(dScaleX, dScaleY, dScaleZ);
        }

        private void SetRotation_Click(object sender, EventArgs e)
        {
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
            coreWrapper.SetMesh();
        }

        private void AddRenderComponent(object sender, EventArgs e)
        {
            // 'Renderer' 컴포넌트 추가

        }




        /*
숫자만 입력받게 하는 코드
if (!(char.IsDigit(e.KeyChar) || e.KeyChar == Convert.ToChar(Keys.Back)))
{
e.Handled = true;
}
*/

        //로직부분 건들XX 이 이후로 추가.
        /////////////////////////////////////////////////////////////////////////

    }
}