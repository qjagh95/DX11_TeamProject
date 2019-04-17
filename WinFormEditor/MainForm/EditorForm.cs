﻿using System;
using System.IO;
using System.Drawing;
using System.Windows.Forms;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using EngineWrapper;

namespace WinFormEditor
{
    public enum eLightType      { LT_DIR, LT_POINT, LT_SPOT, LT_MAX };
    public enum eTransform      { T_LOCAL, T_WORLD, T_MAX };
    public enum eTransformType  { TT_SCALE, TT_ROTATE, TT_POSITION, TT_MAX };

    public partial class EditorForm : Form
    {
        // CoreWrapper(CLR) 객체
        public CoreWrapper m_coreWrapper = new CoreWrapper();

        // Member
        private string m_strClipName    = "";
        private string m_strStartFrame  = "";
        private string m_strEndFrame    = "";
        private string m_strAniTime     = "";
        private string m_strDeleteClip  = "";

        // Instacne
        private FileSaveLoad m_file                 = null;
        private Layer m_layer                       = null;
        private GameObject m_gameObj                = null;
        private Transform m_transform               = null;
        private Renderer m_renderer                 = null;
        private Light m_light                       = null;
        Dictionary<string, ObjectInfo> m_objInfo    = null;

        private void Run(object sender, EventArgs e)
        {
            while (this.Created == true)
            {
                m_coreWrapper.Logic();

                // Update Transform
                if (m_coreWrapper.IsGizmoClick())
                {
                    m_transform.m_isUpdateTr = true;
                }
                m_transform.UpdateTransform();
                
                // Navigation
                TB_NaviIndex.Text = Convert.ToString(m_coreWrapper.GetSelectNaviIndex());
                if (m_coreWrapper.GetSelectNaviMove() == true)
                {
                    TB_TileFlag.Text = "Tile 이동 가능";
                }
                else
                {
                    TB_TileFlag.Text = "Tile 이동 불가";
                }

                Application.DoEvents();
            }
        }

        public EditorForm()
        {
            InitializeComponent();

            PB_DeviceWindow.MouseWheel += new MouseEventHandler(Mouse_Wheel);

            //hInstance, hwnd얻어옴.
            IntPtr hinstance = System.Runtime.InteropServices.Marshal.GetHINSTANCE(PB_DeviceWindow.GetType().Module);
            IntPtr DeviceHwnd = PB_DeviceWindow.Handle;
            m_coreWrapper.Init(DeviceHwnd, hinstance);

            // Auto Save
            Timer timer = new Timer
            {
                Interval = 30 * 1000
            };
            timer.Tick += new EventHandler(AutoSave);
            timer.Start();

            // 객체 초기화
            m_file = new FileSaveLoad();
            m_file.Init(this);
            m_layer = new Layer();
            m_layer.Init(this);
            m_gameObj = new GameObject();
            m_gameObj.Init(this);
            m_transform = new Transform();
            m_transform.Init(this);
            m_renderer = new Renderer();
            m_renderer.Init(this);
            m_light = new Light();
            m_light.Init(this);

            // Dictionary 생성
            m_objInfo = new Dictionary<string, ObjectInfo>();

            // 초기화 및 로드
            ItemInit();
            LoadData(false);
            CB_GridShow.Checked = true;

            // Connection Run Funtion
            Application.Idle += Run;
        }

        private void Delete(object sender, FormClosedEventArgs e)
        {
            m_coreWrapper.Delete();
        }

        public void AddLogString(string _strLog)
        {
            string strLogTime = System.DateTime.Now.ToString();
            LB_LogMessage.Items.Insert(0, "[" + strLogTime + "] " + _strLog);
        }

        public CoreWrapper GetWrapper()
        {
            return m_coreWrapper;
        }

        public Light GetLightInst()
        {
            return m_light;
        }

        public ListBox GetObjectListBox()
        {
            return LB_ObjectList;
        }

        public Button GetAddRenderComponentBtn()
        {
            return Btn_AddComponent;
        }

        public Button GetAddLightComponentBtn()
        {
            return Btn_AddLightComponent;
        }

        public List<object> GetLightTools()
        {
            List<object> listLightTools = new List<object>();
            listLightTools.Add(Btn_LightColor);
            listLightTools.Add(CB_LightType);
            listLightTools.Add(CB_WireFrame);
            listLightTools.Add(TB_SpcularR);
            listLightTools.Add(TB_SpcularG);
            listLightTools.Add(TB_SpcularB);
            listLightTools.Add(TB_SpcularPower);
            listLightTools.Add(TB_Range);
            listLightTools.Add(TB_RangeValue);
            return listLightTools;
        }

        public Dictionary<eTransformType, List<TextBox>> GetTransformTextBox(eTransform _eTr)
        {
            Dictionary<eTransformType, List<TextBox>> dicListTr =
                new Dictionary<eTransformType, List<TextBox>>();

            List<TextBox> listScaleTr = new List<TextBox>();
            List<TextBox> listRotateTr = new List<TextBox>();
            List<TextBox> listPositionTr = new List<TextBox>();

            if (_eTr == eTransform.T_LOCAL)
            {
                // Scale
                listScaleTr.Add(LScaleX);
                listScaleTr.Add(LScaleY);
                listScaleTr.Add(LScaleZ);
                dicListTr.Add(eTransformType.TT_SCALE, listScaleTr);
                // Rotate
                listRotateTr.Add(LRotateX);
                listRotateTr.Add(LRotateY);
                listRotateTr.Add(LRotateZ);
                dicListTr.Add(eTransformType.TT_ROTATE, listRotateTr);
                // Position
                listPositionTr.Add(LPositionX);
                listPositionTr.Add(LPositionY);
                listPositionTr.Add(LPositionZ);
                dicListTr.Add(eTransformType.TT_POSITION, listPositionTr);
            }
            else if (_eTr == eTransform.T_WORLD)
            {
                // Scale
                listScaleTr.Add(ScaleX);
                listScaleTr.Add(ScaleY);
                listScaleTr.Add(ScaleZ);
                dicListTr.Add(eTransformType.TT_SCALE, listScaleTr);
                // Rotate
                listRotateTr.Add(RotateX);
                listRotateTr.Add(RotateY);
                listRotateTr.Add(RotateZ);
                dicListTr.Add(eTransformType.TT_ROTATE, listRotateTr);
                // Position
                listPositionTr.Add(PositionX);
                listPositionTr.Add(PositionY);
                listPositionTr.Add(PositionZ);
                dicListTr.Add(eTransformType.TT_POSITION, listPositionTr);
            }

            return dicListTr;
        }

        public TextBox GetTB()
        {
            return PositionX;
        }

        public Dictionary<string, ObjectInfo> GetObjInfo()
        {
            return m_objInfo;
        }

        public GameObject GetGameObjectInst()
        {
            return m_gameObj;
        }

        public Transform GetTransformInst()
        {
            return m_transform;
        }

        private void ItemInit()
        {
            CB_WireFrame.Checked = true;
            CB_isDebugTarget.Checked = false;
            DebugRenderTarget_OnOff(null, null);
            CB_LightType.SelectedIndex = 0;
            TB_Range.Value = 10;
        }

        public void LoadData(bool _isFileLoad)
        {
            ClearData();
            LoadObjList(_isFileLoad);
            LoadMeshList();
            LoadAnmFile();
        }

        private void ClearData()
        {
            // 데이터 지우기
            m_objInfo.Clear();

            // 아이템 목록 지우기
            LB_ObjectList.Items.Clear();
            LB_LogMessage.Items.Clear();
            LB_MeshList.Items.Clear();
            LB_FileMesh.Items.Clear();
            LB_AniList.Items.Clear();
            LB_BoneList.Items.Clear();
            ClipList.Items.Clear();
        }

        private void LoadObjList(bool _isFileLoad)
        {
            // 레이어 이름 목록
            string[] arrLayerTag = m_coreWrapper.GetLayerList();
            for (int i = 0; i < arrLayerTag.Length; ++i)
            {
                // 콤보 박스에 (레이어 태그)아이템 추가
                CB_LayerList.Items.Add(arrLayerTag[i]);

                // 오브젝트 이름 목록
                string[] arrObjTag = m_coreWrapper.GetSelectLayerObjList(arrLayerTag[i]);
                for (int j = 0; j < arrObjTag.Length; ++j)
                {
                    string strLayer = arrLayerTag[i];
                    string strTag   = arrObjTag[j];

                    // Local Transform
                    float[] arrFLScale = m_transform.ObjGetLocalTransform(strTag, strLayer, eTransformType.TT_SCALE);
                    float[] arrFLRot   = m_transform.ObjGetLocalTransform(strTag, strLayer, eTransformType.TT_ROTATE);
                    float[] arrFLPos   = m_transform.ObjGetLocalTransform(strTag, strLayer, eTransformType.TT_POSITION);

                    // World Transform
                    float[] arrFWScale = m_transform.ObjGetWorldTransform(strTag, strLayer, eTransformType.TT_SCALE);
                    float[] arrFWRot   = m_transform.ObjGetWorldTransform(strTag, strLayer, eTransformType.TT_ROTATE);
                    float[] arrFWPos   = m_transform.ObjGetWorldTransform(strTag, strLayer, eTransformType.TT_POSITION);

                    // 데이터 생성 및 추가
                    ObjectInfo info = new ObjectInfo
                    {
                        strLayerTag     = arrLayerTag[i],
                        vecLScale       = new ObjectInfo.Vector3(arrFLScale),
                        vecLRotate      = new ObjectInfo.Vector3(arrFLRot),
                        vecLPosition    = new ObjectInfo.Vector3(arrFLPos),
                        vecWScale       = new ObjectInfo.Vector3(arrFWScale),
                        vecWRotate      = new ObjectInfo.Vector3(arrFWRot),
                        vecWPosition    = new ObjectInfo.Vector3(arrFWPos)
                    };
                    m_objInfo.Add(arrObjTag[j], info);

                    // 아이템 추가
                    LB_ObjectList.Items.Add(arrObjTag[j]);
                }
            }
        }

        private void LoadMeshList()
        {
            // 아이템 추가
            string[] arrString = m_coreWrapper.GetMeshNameList();
            for (int i = 0; i < arrString.Length; ++i)
            {
                LB_MeshList.Items.Add(arrString[i].ToString());
            }

            // 아이템 추가
            // .msh 파일 로드
            string curDirectory = Directory.GetCurrentDirectory();
            string path = Directory.GetParent(curDirectory).Parent.Parent.FullName;
            path += "\\3DClient\\Bin\\MeshData\\";
            foreach (string f in Directory.GetFiles(path, "*.msh"))
            {
                m_coreWrapper.LoadMeshFromFullPath(Path.GetFileNameWithoutExtension(f), f);
                LB_FileMesh.Items.Add(Path.GetFileNameWithoutExtension(f));
            }
        }

        private void LoadAnmFile()
        {
            // 아이템 추가
            string curDirectory = Directory.GetCurrentDirectory();
            string path = Directory.GetParent(curDirectory).Parent.Parent.FullName;
            path += "\\3DClient\\Bin\\MeshData\\";
            foreach (string f in Directory.GetFiles(path, "*.anm"))
            {
                m_coreWrapper.LoadMeshFromFullPath(Path.GetFileNameWithoutExtension(f), f);
                LB_AniList.Items.Add(Path.GetFileNameWithoutExtension(f));
            }
        }

        /*******************************************************************************************************/
        // 파일 저장, 불러오기

        private void AutoSave(object sender, System.EventArgs e)
        {
            m_file.FileAutoSave();
        }

        private void Btn_SaveLoadClick(object sender, EventArgs e)
        {
            Button btn = (Button)sender;
            if (btn.Name == "Btn_FileSave")
            {
                m_file.FileSave_Click();
            }
            else if (btn.Name == "Btn_FileLoad")
            {
                m_file.FileLoad_Click();
            }
        }

        private void Btn_MshFileLoad(object sender, EventArgs e)
        {
            m_file.LoadMshFile(LB_FileMesh);
        }

        /*******************************************************************************************************/
        // 레이어

        private void ChangeLayerComboBox(object sender, EventArgs e)
        {
            if (LB_ObjectList.SelectedItem != null)
            {
                int index = LB_ObjectList.SelectedIndex;
                string strTag = LB_ObjectList.Items[index].ToString();
                string strLayerTag = m_objInfo[strTag].strLayerTag;
                m_layer.ChangeLayerComboBox(strTag, CB_LayerList);
                return;
            }
        }

        /*******************************************************************************************************/
        // 오브젝트 선택, 생성, 삭제, 수정

        private void ChangeSelectedObject(object sender, EventArgs e)
        {
            // 선택 오브젝트
            m_gameObj.SelectedObject(m_transform, LB_ObjectList, TB_ObjectTag, CB_LayerList);
        }

        private void Btn_CreateObjectClick(object sender, EventArgs e)
        {
            m_gameObj.CreateObject(LB_ObjectList);
        }

        private void Btn_DeleteObjectClick(object sender, EventArgs e)
        {
            // 'GlobalLight', 'FreeCamObj' 오브젝트는 삭제 방지
            string strTag = LB_ObjectList.SelectedItem.ToString();
            if (strTag == "GlobalLight" || strTag == "FreeCamObj")
            {
                MessageBox.Show("삭제할 수 없는 오브젝트입니다.");
                return;
            }

            EventHandler textChangedEvent = ChangeLayerComboBox;
            EventHandler SelectedIndexChangedEvent = ChangeSelectedObject;
            m_gameObj.DeleteObject(LB_ObjectList, SelectedIndexChangedEvent,
                                   TB_ObjectTag, CB_LayerList, textChangedEvent);
        }

        private void KeyDown_DeleteObject(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Delete)
            {
                if (LB_ObjectList.SelectedItem == null)
                {
                    return;
                }
                string strTag = LB_ObjectList.SelectedItem.ToString();
                Btn_DeleteObjectClick(strTag, e);
            }
        }

        private void ObjectAllClear(object sender, MouseEventArgs e)
        {
            Point parentPoint = Location;

            // 폼 생성
            Form dataRemoveForm = new Form()
            {
                Owner = this,
                Size = new Size(218, 120),
                StartPosition = FormStartPosition.Manual,
                Location = new Point(parentPoint.X + 13, parentPoint.Y + 125)
            };
            dataRemoveForm.Show();

            // 삭제
            m_gameObj.DeleteAllObject(dataRemoveForm);
        }

        private void ChangeObjectTagEnter(object sender, KeyPressEventArgs e)
        {
            // 'Enter' 입력
            if (Convert.ToInt32(e.KeyChar) == 13)
            {
                EventHandler changeSelectedItem = ChangeSelectedObject;
                m_gameObj.ChangeTag(LB_ObjectList, changeSelectedItem, TB_ObjectTag);
            }
        }

        /*******************************************************************************************************/
        // Transform

        private void KeyPressCheck(object sender, KeyPressEventArgs e)
        {
            // -, .(소수점), BK(BackSpace, 8번), 0 ~9(숫자)를 제외한 문자는 검사하지 않는다.
            int changeASCII = Convert.ToInt32(e.KeyChar);
            if ((changeASCII >= '0' && changeASCII <= '9') ||
                 changeASCII == '-' || changeASCII == '.' || changeASCII == 8)
            {
                // Transform을 갱신하지 않는다.
                m_transform.m_isUpdateTr = false;
            }
            else
            {
                // 위 키가 아닌 잘못된 형식의 값은 입력될 수 없다.
                e.Handled = true;
            }
        }

        private void ChangeScale(object sender, EventArgs e)
        {
            ChangeSelectedObject(null, null);

            // Local, World 검사
            string strName = ((TextBox)sender).Name;
            strName = strName.Substring(0, 1);
            if (strName == "L" /* Local */)
            {
                if (LScaleX.Text != "" && LScaleY.Text != "" && LScaleZ.Text != "")
                {
                    if (LScaleX.Text != "-" && LScaleY.Text != "-" && LScaleZ.Text != "-")
                    {
                        m_transform.ChangeLocalScale((TextBox)sender, LScaleX, LScaleY, LScaleZ);
                    }
                }
            }
            else
            {
                if (ScaleX.Text != "" && ScaleY.Text != "" && ScaleZ.Text != "")
                {
                    if (ScaleX.Text != "-" && ScaleY.Text != "-" && ScaleZ.Text != "-")
                    {
                        m_transform.ChangeWorldScale((TextBox)sender, ScaleX, ScaleY, ScaleZ);
                    }
                }
            }
        }

        private void ChangeRotate(object sender, EventArgs e)
        {
            ChangeSelectedObject(null, null);

            // Local, World 검사
            string strName = ((TextBox)sender).Name;
            strName = strName.Substring(0, 1);
            if (strName == "L" /* Local */)
            {
                if (LRotateX.Text != "" && LRotateY.Text != "" && LRotateZ.Text != "")
                {
                    if (LRotateX.Text != "-" && LRotateY.Text != "-" && LRotateZ.Text != "-")
                    {
                        m_transform.ChangeLocalRotate((TextBox)sender, LRotateX, LRotateY, LRotateZ);
                    }
                }
            }
            else
            {
                if (RotateX.Text != "" && RotateY.Text != "" && RotateZ.Text != "")
                {
                    if (RotateX.Text != "-" && RotateY.Text != "-" && RotateZ.Text != "-")
                    {
                        m_transform.ChangeWorldRotate((TextBox)sender, RotateX, RotateY, RotateZ);
                    }
                }
            }
        }

        private void ChangePosition(object sender, EventArgs e)
        {
            ChangeSelectedObject(null, null);

            string strName = ((TextBox)sender).Name;
            strName = strName.Substring(0, 1);
            if (strName == "L" /* Local */)
            {
                if (LPositionX.Text != "" && LPositionY.Text != "" && LPositionZ.Text != "")
                {
                    if (LPositionX.Text != "-" && LPositionY.Text != "-" && LPositionZ.Text != "-")
                    {
                        m_transform.ChangeLocalPosition((TextBox)sender, LPositionX, LPositionY, LPositionZ);
                    }
                }
            }
            else
            {
                if (PositionX.Text != "" && PositionY.Text != "" && PositionZ.Text != "")
                {
                    // 음수 표기만으로는 형식에 어긋난 데이터로 취급한다.
                    if (PositionX.Text != "-" && PositionY.Text != "-" && PositionZ.Text != "-")
                    {
                        m_transform.ChangeWorldPosition((TextBox)sender, PositionX, PositionY, PositionZ);
                    }
                }
            }
        }

        /*******************************************************************************************************/
        // 렌더링

        private void AddRenderComponent(object sender, EventArgs e)
        {
            m_renderer.AddComponent((Button)sender);
        }

        private void Btn_SetMeshClick(object sender, EventArgs e)
        {
            m_renderer.SetMesh(Btn_AddComponent, LB_MeshList, LB_FileMesh);

            // '메시 등록' 버튼을 클릭 시 호출된다.
            if (LB_MeshList.SelectedItem == null && LB_FileMesh.SelectedItem == null)
            {
                AddLogString("Error! 선택된 Mesh가 없습니다");
                return;
            }
        }

        private void BasicMeshClick(object sender, EventArgs e)
        {
            // 포커스 해제
            if (LB_FileMesh.SelectedItem != null)
            {
                LB_FileMesh.SelectedItem = null;
            }
        }

        private void FileMeshClick(object sender, EventArgs e)
        {
            if (LB_MeshList.SelectedItem != null)
            {
                LB_MeshList.SelectedItem = null;
            }
        }

        private void DebugRenderTarget_OnOff(object sender, EventArgs e)
        {
            m_coreWrapper.SetTargetOnOff(CB_isDebugTarget.Checked);
        }

        private void TargetChange_OnOff(object sender, EventArgs e)
        {
            m_coreWrapper.ChangeTarget(CB_TargetChange.Checked);
        }

        /*******************************************************************************************************/
        // 조명

        private void AddLightComponent(object sender, EventArgs e)
        {
            m_light.AddComponent((Button)sender);
        }

        private void ChangeLightValue(object sender, EventArgs e)
        {
            m_light.ChangeLightRange(TB_Range.Value, TB_RangeValue);
        }

        private void LightWireFrame_OnOff(object sender, EventArgs e)
        {
            m_light.LightWireFrame_OnOff(CB_WireFrame.Checked);
        }

        private void SpecularKeyPress(object sender, KeyPressEventArgs e)
        {
            int changeASCII = Convert.ToInt32(e.KeyChar);
            if ((changeASCII >= '0' && changeASCII <= '9') || changeASCII == '.' || changeASCII == 8)
            {
            }
            else
            {
                // 위 키가 아닌 잘못된 형식의 값은 입력될 수 없다.
                e.Handled = true;
            }
        }

        private void ChangeSpecularValue(object sender, EventArgs e)
        {
            m_light.ChangeSpecularValue((TextBox)sender);
        }

        /*******************************************************************************************************/
        // 애니메이션

        private void AddClipBtn_Click(object sender, EventArgs e)
        {
            if (DivideClipBox.Checked == true)
            {
                int iAnimationOption = AnimationOptionBox.SelectedIndex;

                int iStartFrame = Convert.ToInt32(m_strStartFrame);
                int iEndFrame = Convert.ToInt32(m_strEndFrame);
                double dPlayTime = Convert.ToDouble(m_strAniTime);

                // ListBox의 이름을 변경해주면 된다.
                if (m_coreWrapper.AddClip(m_strClipName, iAnimationOption, iStartFrame, iEndFrame, dPlayTime) == true)
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
                if (m_coreWrapper.ModifyClip(strOriginName, strChangeName, iAnimationOption, iStartFrame, iEndFrame, dPlayTime) == true)
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

            m_coreWrapper.ChangeClip(ClipList.SelectedItem.ToString());

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
            if (ClipList.SelectedItem == null || ClipList.SelectedItem.ToString() == "")
            {
                AddLogString("Error! DeleteClip Box에 Text 정보가 없습니다.");
                return;
            }

            if (ClipList.Items.Count == 1)
            {
                AddLogString("Error! AnimationClip은 1개이하로 삭제할 수 없습니다.");
                return;
            }

            m_coreWrapper.DeleteClip(ClipList.SelectedItem.ToString());
            ClipList.Items.Remove(ClipList.SelectedItem);
        }

        private void DivideClipBox_CheckedChanged(object sender, EventArgs e)
        {
            if (DivideClipBox.Checked == true)
            {
                m_coreWrapper.SetDivideClip();
            }
            else
            {
                m_coreWrapper.DeleteDivideClip();
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

                m_coreWrapper.ClipSave(strFilePath);
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

                m_coreWrapper.LoadClipFromFullPath(filePath);

                string[] arrString = m_coreWrapper.GetClipTagList();
                for (int i = 0; i < arrString.Length; ++i)
                {
                    ClipList.Items.Add(arrString[i].ToString());
                }
            }
        }


        /*******************************************************************************************************/
        // 네비게이션

        private void BT_CreateNavi_Click(object sender, EventArgs e)
        {
            if (TB_NumX.Text == "" || TB_NumZ.Text == "")
            {
                AddLogString("Error! Navi Number Text에 입력값이 없습니다.");
                return;
            }

            int iX = Convert.ToInt32(TB_NumX.Text);
            int iZ = Convert.ToInt32(TB_NumZ.Text);
            bool bSuccess = m_coreWrapper.CreateTestLandScape(iX, iZ);
            if (bSuccess)
            {
                //LB_ObjectList.Items.Add("LandTestObj");
                AddLogString("NaviLand생성");
            }
            else
            {
                AddLogString("NaviMesh가 이미 존재합니다");
            }
        }

        private void BT_DeleteNavi_Click(object sender, EventArgs e)
        {
            AddLogString("LandScape 제거");
            m_coreWrapper.DeleteNaviMesh();
        }

        private void BT_NaviSave_Click(object sender, EventArgs e)
        {
            string filePath;
            string fileName;
            SaveFileDialog opdig = new SaveFileDialog();

            DirectoryInfo Pat;
            string FullPath = Directory.GetCurrentDirectory();
            Pat = Directory.GetParent(FullPath).Parent.Parent;

            FullPath = Pat.FullName;
            FullPath += "\\3DClient\\Bin\\Data\\";
            opdig.InitialDirectory = FullPath;
            opdig.Filter = "NavFile(*.nav)|*.nav|모든파일(*.*)|*.*||";
            opdig.RestoreDirectory = true;

            if (opdig.ShowDialog() == DialogResult.OK)
            {
                filePath = opdig.FileName;
                fileName = Path.GetFileNameWithoutExtension(filePath);
                m_coreWrapper.SaveNavFile(filePath);
            }
        }

        private void BT_NaviLoad_Click(object sender, EventArgs e)
        {
            string filePath;
            string fileName;
            OpenFileDialog opdig = new OpenFileDialog();

            DirectoryInfo Pat;
            string FullPath = Directory.GetCurrentDirectory();
            Pat = Directory.GetParent(FullPath).Parent.Parent;

            FullPath = Pat.FullName;
            FullPath += "\\3DClient\\Bin\\Data\\";
            opdig.InitialDirectory = FullPath;
            opdig.Filter = "NavFile(*.nav)|*.nav|모든파일(*.*)|*.*||";
            opdig.RestoreDirectory = true;

            if (opdig.ShowDialog() == DialogResult.OK)
            {
                filePath = opdig.FileName;
                fileName = Path.GetFileNameWithoutExtension(filePath);
                m_coreWrapper.LoadNavFile(filePath);
            }
        }

        private void CB_isNaviEditor_CheckedChanged(object sender, EventArgs e)
        {
            m_coreWrapper.SetNaviEditorMode(CB_isNaviEditor.Checked);
        }

        private void TB_BrushSize_TextChanged(object sender, EventArgs e)
        {
            if (TB_BrushSize.Text != "")
            {
                int dTemp = Convert.ToInt32(TB_BrushSize.Text);

                m_coreWrapper.SetBrushSize(dTemp);
            }
        }

        private void CB_GridShow_CheckedChanged(object sender, EventArgs e)
        {
            bool Check = CB_GridShow.Checked;
            m_coreWrapper.SetGridShow(Check);
        }

        private void TB_GridSize_TextChanged(object sender, EventArgs e)
        {
            if (TB_GridSize.Text == "")
                return;

            int Val = Convert.ToInt32(TB_GridSize.Text);
            m_coreWrapper.SetGridSize(Val);
        }
        /*******************************************************************************************************/



        //private void BtnRemove(object sender, EventArgs e)
        //{
        //    // 오브젝트 전부 삭제
        //    m_coreWrapper.DeleteAllObject();

        //    // 아이템 전부 삭제
        //    m_createObjCnt = 0;
        //    LB_ObjectList.Items.Clear();
        //    m_dataRemoveForm.Close();
        //    MessageBox.Show("데이터를 전부 삭제했습니다.");
        //}

        //private void BtnCancel(object sender, EventArgs e)
        //{
        //    m_dataRemoveForm.Close();
        //}

        //private void LoadObjectList()
        //{
        //    // 데이터 지우기
        //    m_objInfo.Clear();

        //    // 아이템 모두 지우기
        //    LB_ObjectList.Items.Clear();

        //    // 오브젝트 목록
        //    string[] arrLayerTag = m_coreWrapper.GetLayerList();
        //    for (int i = 0; i < arrLayerTag.Length; ++i)
        //    {
        //        string[] arrObjTag = m_coreWrapper.GetSelectLayerObjList(arrLayerTag[i]);
        //        for (int j = 0; j < arrObjTag.Length; ++j)
        //        {
        //            // 데이터 추가
        //            ObjectInfo info = new ObjectInfo();
        //            info.strLayerTag = arrLayerTag[i];
        //            info.vecScale = new ObjectInfo.Vector3();
        //            info.vecRotate = new ObjectInfo.Vector3();
        //            info.vecPosition = new ObjectInfo.Vector3();
        //            m_objInfo.Add(arrObjTag[j], info);

        //            // 아이템 추가
        //            LB_ObjectList.Items.Add(arrObjTag[j]);
        //        }
        //    }
        //}

        //public string GetSelectObjectTag()
        //{
        //    string strTag = "";
        //    if (LB_ObjectList.SelectedItem != null)
        //    {
        //        strTag = LB_ObjectList.SelectedItem.ToString();
        //    }

        //    return strTag;
        //}

        

        

        

        private void LoadLayerComboBoxList()
        {
            string[] arrLayerTag = m_coreWrapper.GetLayerList();
            for (int i = 0; i < arrLayerTag.Length; ++i)
            {
                CB_LayerList.Items.Add(arrLayerTag[i].ToString());
            }
        }

        /*******************************************************************************************************/
        // Layer, Object
        // 최종 수정 : 2019.03.24 (김동규)
        // 문제 있으면 바로 말해주세요 !!

        //private void ObjectList_ChangeSelectedItem(object sender, EventArgs e) // 19.03.24
        //{
        //    if (LB_ObjectList.SelectedItem == null)
        //    {
        //        return;
        //    }

        //    // 오브젝트 선택
        //    int itemIndex = LB_ObjectList.SelectedIndex;
        //    string strObjectTag = LB_ObjectList.Items[itemIndex].ToString();
        //    string strLayerTag = m_objInfo[strObjectTag].strLayerTag;
        //    m_coreWrapper.SetActiveObject(strObjectTag, strLayerTag);

        //    // Transform
        //    LoadTransform(itemIndex);

        //    // 오브젝트 텍스트 박스 
        //    TB_ObjectTag.Text = strObjectTag;

        //    // 레이어 콤보 박스
        //    int comboBoxIndex = 0;
        //    if (strLayerTag == "Stage")
        //    {
        //        comboBoxIndex = 0;
        //    }
        //    else if (strLayerTag == "Default")
        //    {
        //        comboBoxIndex = 1;
        //    }
        //    else if (strLayerTag == "UI")
        //    {
        //        comboBoxIndex = 2;
        //    }
        //    CB_LayerList.SelectedIndex = comboBoxIndex;
        //}

        //private void Btn_CreateObject(object sender, EventArgs e)
        //{
        //    // 오브젝트 생성
        //    string strObjectTag = "GameObject_" + m_createObjCnt;
        //    string strLayerTag = "Default";
        //    m_coreWrapper.CreateObject(strObjectTag, strLayerTag);
        //    ++m_createObjCnt;

        //    // 데이터 추가
        //    ObjectInfo info = new ObjectInfo();
        //    info.strLayerTag = strLayerTag;
        //    info.vecScale = new ObjectInfo.Vector3();
        //    info.vecRotate = new ObjectInfo.Vector3();
        //    info.vecPosition = new ObjectInfo.Vector3();
        //    m_objInfo.Add(strObjectTag, info);

        //    // 아이템 추가
        //    LB_ObjectList.Items.Add(strObjectTag);

        //    // 만들어진 오브젝트 포커스 활성화
        //    int itemIndex = LB_ObjectList.Items.Count - 1;
        //    LB_ObjectList.SelectedItem = LB_ObjectList.Items[itemIndex];
        //    AddLogString("오브젝트가 생성되었습니다.");
        //}

        //private void Btn_DeleteObject(object sender, EventArgs e)
        //{
        //    if (LB_ObjectList.SelectedItem == null)
        //    {
        //        MessageBox.Show("선택된 오브젝트가 없습니다.");
        //        return;
        //    }

        //    int SelectIndex = LB_ObjectList.SelectedIndex;

        //    // 오브젝트, 데이터 아이템 삭제
        //    LB_ObjectList.SelectedIndexChanged -= ObjectList_ChangeSelectedItem;

        //    string strObjectTag = LB_ObjectList.SelectedItem.ToString();
        //    string strLayerTag = m_objInfo[strObjectTag].strLayerTag;

        //    m_coreWrapper.DeleteObject(strObjectTag, strLayerTag);
        //    m_objInfo.Remove(strObjectTag);
        //    LB_ObjectList.Items.Remove(LB_ObjectList.SelectedItem);
        //    LB_ObjectList.SelectedIndexChanged += ObjectList_ChangeSelectedItem;

        //    // 오브젝트 정보 초기화
        //    if (LB_ObjectList.SelectedIndex == -1)
        //    {
        //        // ComboBox 이벤트 제거
        //        CB_LayerList.TextChanged -= ChangeLayerComboBox;
        //        CB_LayerList.SelectedItem = null;
        //        CB_LayerList.TextChanged += ChangeLayerComboBox;
        //    }

        //    // 오브젝트 텍스트 박스 
        //    TB_ObjectTag.Text = "";

        //    // 레이어 콤보 박스
        //    CB_LayerList.SelectedItem = null;

        //    // 포커스 해제
        //    LB_ObjectList.SelectedItem = null;
        //    AddLogString("오브젝트가 삭제되었습니다.");

        //    if (SelectIndex - 1 < 0)
        //        SelectIndex = 0;

        //    LB_ObjectList.SelectedIndex = SelectIndex - 1;
        //}

        

        

        

        ///*******************************************************************************************************/
        //// Key Check
        //// 최종 수정 : 2019.03.24 (김동규)
        //// 문제 있으면 바로 말해주세요 !!

        

        //private bool CheckStringForm(TextBox _textBox)
        //{
        //    // 변경된 문자열 처리
        //    if (m_isChangeText == true)
        //    {
        //        m_isChangeText = false;

        //        int negativeCnt = 0;
        //        int decimalCnt = 0;

        //        // 문자열을 검사한다.
        //        for (int i = 0; i < _textBox.Text.Length; ++i)
        //        {
        //            string subStr = _textBox.Text.Substring(i, 1);

        //            // 문자열 0번째 위치에 .(소수점) 표시가 들어오는지 검사한다.
        //            if (i == 0 && subStr.CompareTo(".") == 0)
        //            {
        //                SetBeforeTBText(_textBox);
        //                return false;
        //            }
        //            else if (i > 0 && subStr.CompareTo(".") == 0)
        //            {
        //                subStr = _textBox.Text.Substring(i - 1, 1);
        //                if (subStr.CompareTo("-") == 0)
        //                {
        //                    SetBeforeTBText(_textBox);
        //                    return false;
        //                }
        //                else
        //                {
        //                    if (decimalCnt == 0)
        //                    {
        //                        ++decimalCnt;
        //                        continue;
        //                    }
        //                    else
        //                    {
        //                        SetBeforeTBText(_textBox);
        //                        return false;
        //                    }
        //                }
        //            }

        //            // -(음수) 문자가 문자열 0번째 위치가 아닌지 검사한다. 
        //            if (i > 0 && subStr.CompareTo("-") == 0)
        //            {
        //                SetBeforeTBText(_textBox);
        //                return false;
        //            }
        //            else if (i == 0 && subStr.CompareTo("-") == 0)
        //            {
        //                if (negativeCnt == 0)
        //                {
        //                    ++negativeCnt;
        //                    continue;
        //                }
        //                else
        //                {
        //                    SetBeforeTBText(_textBox);
        //                    return false;
        //                }
        //            }
        //        }
        //        return true;
        //    }
        //    return false;
        //}

        //private void SetBeforeTBText(TextBox _textBox)
        //{
        //    // 오브젝트가 없는 경우
        //    if (LB_ObjectList.SelectedItem == null)
        //    {
        //        MessageBox.Show("선택된 오브젝트가 없습니다.");
        //    }
        //    else
        //    {
        //        MessageBox.Show("잘못된 형식의 값이 들어왔습니다.");
        //    }

        //    // ChangePosition 이벤트 삭제
        //    _textBox.TextChanged -= m_event;

        //    if (_textBox.Text.Length > 0)
        //    {

        //        int strLength = _textBox.Text.Length;
        //        _textBox.Text = _textBox.Text.Substring(0, strLength - 1);
        //        _textBox.Select(_textBox.Text.Length, 0);
        //    }

        //    // ChangePosition 이벤트 추가
        //    _textBox.TextChanged += m_event;
        //}

        /*******************************************************************************************************/
        // Transform Component
        // 최종 수정 : 2019.03.24 (김동규)
        // 문제 있으면 바로 말해주세요 !!

        //private bool Data_StringFormCheck(TextBox _sender, TextBox _tbX, TextBox _tbY, TextBox _tbZ)
        //{
        //    // 선택된 오브젝트 여부 검사
        //    // if(g_strObjTag != "") 조건으로도 가능
        //    if (LB_ObjectList.SelectedItem == null)
        //    {
        //        SetBeforeTBText(_sender);
        //        return false;
        //    }

        //    // 음수를 표기할때 음수 자체만으로 데이터를 변환할 수 없다.
        //    if (_tbX.Text == "-" || _tbY.Text == "-" || _tbZ.Text == "-")
        //    {
        //        return false;
        //    }

        //    // 형식 검사
        //    return CheckStringForm(_sender);
        //}

        

        

        

        //private void LoadTransform(int _index)
        //{
        //    if (LB_ObjectList.SelectedItem == null)
        //    {
        //        return;
        //    }

        //    string strObjectTag = LB_ObjectList.Items[_index].ToString();
        //    string strLayerTag = m_objInfo[strObjectTag].strLayerTag;

        //    // Get World Transform(Scale, Rotate, Position)
        //    float[] arrSacle = m_coreWrapper.GetWorldTransform(strObjectTag, strLayerTag, (int)eTransformType.TT_SCALE);
        //    float[] arrRotate = m_coreWrapper.GetWorldTransform(strObjectTag, strLayerTag, (int)eTransformType.TT_ROTATE);
        //    float[] arrPosition = m_coreWrapper.GetWorldTransform(strObjectTag, strLayerTag, (int)eTransformType.TT_POSITION);

        //    // Position
        //    ObjectInfo.Vector3 vPos = new ObjectInfo.Vector3(arrPosition[0], arrPosition[1], arrPosition[2]);
        //    m_objInfo[strObjectTag].vecPosition = vPos;
        //    PositionX.Text = Convert.ToString(vPos.x);
        //    PositionY.Text = Convert.ToString(vPos.y);
        //    PositionZ.Text = Convert.ToString(vPos.z);

        //    // Scale
        //    ObjectInfo.Vector3 vScale = new ObjectInfo.Vector3(arrSacle[0], arrSacle[1], arrSacle[2]);
        //    m_objInfo[strObjectTag].vecScale = vScale;
        //    ScaleX.Text = Convert.ToString(vScale.x);
        //    ScaleY.Text = Convert.ToString(vScale.y);
        //    ScaleZ.Text = Convert.ToString(vScale.z);

        //    // Rotate
        //    ObjectInfo.Vector3 vRotate = new ObjectInfo.Vector3(arrRotate[0], arrRotate[1], arrRotate[2]);
        //    m_objInfo[strObjectTag].vecRotate = vRotate;
        //    RotateX.Text = Convert.ToString(vRotate.x);
        //    RotateY.Text = Convert.ToString(vRotate.y);
        //    RotateZ.Text = Convert.ToString(vRotate.z);
        //}

        /*******************************************************************************************************/
        // Animation Component
        // 최종 수정 : 2019.??.?? (천진호)
        // 문제 있으면 바로 말해주세요 !!



        /*******************************************************************************************************/
        // 파일 저장, 불러오기
        // 최종 수정 : 2019.03.24 (김동규)
        // 문제 있으면 바로 말해주세요 !!

        //private void FileSave_Click(object sender, EventArgs e)
        //{
        //    // SaveFileDialog 창 설정
        //    // Title  : Dialog 창 이름 설정
        //    // Filter : 파일 형식 부분
        //    // AddExtension : 확장명 추가 여부
        //    SaveFileDialog saveFile = new SaveFileDialog();
        //    string curDirectory = Directory.GetCurrentDirectory();
        //    string path = Directory.GetParent(curDirectory).Parent.Parent.FullName;
        //    path += "\\3DClient\\bin\\Data";
        //    saveFile.InitialDirectory = path;
        //    saveFile.Title = "다른 이름으로 저장";
        //    saveFile.Filter = "데이터 파일(*.dat)|*.dat|모든파일(*.*)|*.*";
        //    saveFile.DefaultExt = "dat";
        //    saveFile.AddExtension = true;
        //    if (saveFile.ShowDialog() == DialogResult.OK)
        //    {
        //        // 저장
        //        m_coreWrapper.FileSave(saveFile.FileName);
        //    }
        //}

        //private void FileLoad_Click(object sender, EventArgs e)
        //{
        //    OpenFileDialog openFile = new OpenFileDialog();
        //    string curDirectory = Directory.GetCurrentDirectory();
        //    string path = Directory.GetParent(curDirectory).Parent.Parent.FullName;
        //    path += "\\3DClient\\bin\\Data";
        //    openFile.InitialDirectory = path;
        //    openFile.Title = "파일 불러오기";
        //    openFile.FileName = ".dat";
        //    openFile.Filter = "데이터 파일(*.dat) | *.dat; | 모든 파일(*.*) | *.*";
        //    if (openFile.ShowDialog() == DialogResult.OK)
        //    {
        //        // 불러오기
        //        m_coreWrapper.FileLoad(openFile.FileName);

        //        // 로드
        //        ClearData();
        //        LoadObjectList();
        //        LoadMeshList();
        //        LoadAnmFile();
        //    }
        //}

        //private void SetMesh_Click(object sender, EventArgs e)
        //{
        //    // '메시 등록' 버튼을 클릭 시 호출된다.
        //    if (LB_MeshList.SelectedItem == null && LB_FileMesh.SelectedItem == null)
        //    {
        //        AddLogString("Error! 선택된 Mesh가 없습니다");
        //        return;
        //    }

        //    string strTemp;
        //    if (LB_MeshList.SelectedItem != null)
        //    {
        //        strTemp = LB_MeshList.SelectedItem.ToString();
        //        m_coreWrapper.SetMesh(strTemp);
        //    }
        //    else if (LB_FileMesh.SelectedItem != null)
        //    {
        //        strTemp = LB_FileMesh.SelectedItem.ToString();
        //        m_coreWrapper.SetMesh(strTemp);
        //    }
        //}

        


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

                m_coreWrapper.LoadMeshFromFullPath(fileName, filePath);
                LB_FileMesh.Items.Add(fileName);
            }
        }

        private void Scene_Click(object sender, MouseEventArgs e)
        {
            PB_DeviceWindow.Focus();
        }


        private void ClearLayerList()
        {
            //LB_LayerList.Items.Clear();
        }

        private void ClearObjectList()
        {

        }

        private void ChangeLayer_Click(object sender, EventArgs e)
        {

        }

        

       

        private void Mouse_Wheel(object sender, MouseEventArgs e)
        {
            m_coreWrapper.SetMouseWheel(e.Delta);
        }

        private void Transform_Press(object sender, KeyPressEventArgs e)
        {

        }

        private void Specular_Press(object sender, KeyPressEventArgs e)
        {
            if ((e.KeyChar >= (char)48 && e.KeyChar <= (char)57) || e.KeyChar == (char)46 || e.KeyChar == (char)8 || e.KeyChar == (char)127)
            {
            }
            else
            {
                e.Handled = true;
            }
        }

        private void txtInterval_KeyPress(object sender, KeyPressEventArgs e)
        {
            //숫자만 입력되도록 필터링
            if (!(char.IsDigit(e.KeyChar) || e.KeyChar == Convert.ToChar(Keys.Back)))    //숫자와 백스페이스를 제외한 나머지를 바로 처리
            {
                e.Handled = true;
            }
        }

        public void LightAmbColor()
        {
            ColorDialog cd = new ColorDialog();

            if (cd.ShowDialog() == DialogResult.OK)
                m_coreWrapper.SetLightAmbient(cd.Color.R, cd.Color.G, cd.Color.B);
            else
                return;
        }

        public void LightDiffuse()
        {
            ColorDialog cd = new ColorDialog();

            if (cd.ShowDialog() == DialogResult.OK)
            {
                m_coreWrapper.SetLightDiffuse(cd.Color.R, cd.Color.G, cd.Color.B);
                AddLogString("색상값 변경 R : " + cd.Color.R.ToString() + "G : " + cd.Color.R.ToString() + "B : " + cd.Color.R.ToString());
            }
            else
                return;
        }

        

        private void BT_DifColor_Click(object sender, EventArgs e)
        {
            LightDiffuse();
            AddLogString("ColorEditorOpen");
        }

        private void CB_LightType_SelectedIndexChanged(object sender, EventArgs e)
        {
            m_coreWrapper.SetLightType(CB_LightType.SelectedIndex);
            AddLogString("LightType이 " + CB_LightType.SelectedItem.ToString() + "로 변경되었습니다.");
        }

        //private void TB_LightXDir_Scroll(object sender, EventArgs e)
        //{
        //    m_coreWrapper.SetLightDirX(TB_LightXDir.Value);
        //}

        //private void TB_LightYDir_Scroll(object sender, EventArgs e)
        //{
        //    m_coreWrapper.SetLightDirY(TB_LightYDir.Value);
        //}

        //private void TB_LightZDir_Scroll(object sender, EventArgs e)
        //{
        //    m_coreWrapper.SetLightDirZ(TB_LightZDir.Value);
        //}

        //private void TB_Range_Scroll(object sender, EventArgs e)
        //{
        //    m_coreWrapper.SetLightRange(TB_Range.Value);
        //}

        //private void CB_WireFrame_CheckedChanged(object sender, EventArgs e)
        //{
        //    m_coreWrapper.SetLightWireFrame(CB_WireFrame.Checked);
        //}

        //private void CB_isDebugTarget_CheckedChanged(object sender, EventArgs e)
        //{
        //    m_coreWrapper.SetTargetOnOff(CB_isDebugTarget.Checked);
        //}

        //private void TB_SpcularR_TextChanged(object sender, EventArgs e)
        //{
        //    double dR = Convert.ToDouble(TB_SpcularR.Text);
        //    m_coreWrapper.SetLightSpclularR(dR);
        //}

        //private void TB_SpcularG_TextChanged(object sender, EventArgs e)
        //{
        //    double dR = Convert.ToDouble(TB_SpcularG.Text);
        //    m_coreWrapper.SetLightSpclularG(dR);
        //}

        //private void TB_SpcularB_TextChanged(object sender, EventArgs e)
        //{
        //    double dR = Convert.ToDouble(TB_SpcularB.Text);
        //    m_coreWrapper.SetLightSpclularB(dR);
        //}

        //private void TB_SpcularPower_TextChanged(object sender, EventArgs e)
        //{
        //    double dR = Convert.ToDouble(TB_SpcularPower.Text);
        //    m_coreWrapper.SetLightSpclularW(dR);
        //}

        //private void CB_TargetChange_CheckedChanged(object sender, EventArgs e)
        //{
        //    m_coreWrapper.ChangeTarget(CB_TargetChange.Checked);
        //}
    }
}