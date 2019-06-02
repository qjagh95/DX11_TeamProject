using System;
using System.IO;
using System.Drawing;
using System.Windows.Forms;
using System.Collections.Generic;
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

                m_coreWrapper.ObjectColliderEnable(CB_ColliderEnable.Checked && !CB_PickColliderEnable.Checked);
                m_coreWrapper.PickingColliderEnable(CB_PickColliderEnable.Checked && !CB_ColliderEnable.Checked);

                // 오브젝트 Picking 검사한다.
                // Picking을 검사해서, 있다면 한번만 정보를 갱신 한다.
                // Picking된 오브젝트의 정보(Tag, LayerTag)를 가져온다. 
                if (m_coreWrapper.IsPicking() == true)
                {
                    m_coreWrapper.SetPickingFalse();
                    m_gameObj.SetPickingObjectInfo(LB_ObjectList, LB_MeshList, LB_FileMesh, TB_SelecteMesh);
                }

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
            CB_GizmoEnable.Checked = true;
            CB_PickColliderEnable.Checked = true;

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

            List<TextBox> listScaleTr    = new List<TextBox>();
            List<TextBox> listRotateTr   = new List<TextBox>();
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

        public TextBox[] GetPivotTextBox()
        {
            TextBox[] arrTextBox = { PivotX, PivotY, PivotZ };
            return arrTextBox;
        }

        public TextBox GetParentTextBox()
        {
            return TB_ParentName;
        }

        public ListBox GetMeshListBox()
        {
            return LB_MeshList;
        }

        public ListBox GetFileMeshListBox()
        {
            return LB_FileMesh;
        }

        public ListBox GetColliderListBox()
        {
            return LB_ColliderList;
        }

        public TextBox GetSelecteMeshNameTextBox()
        {
            return TB_SelecteMesh;
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

        public Renderer GetRenderInst()
        {
            return m_renderer;
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
            ClearData(_isFileLoad);
            LoadObjList();

            // 처음에 툴을 실행시킬 때만 로드를 한다.
            if(_isFileLoad == false)
            {
                LoadMeshList();
                LoadAnmFile();
            }

            // 오브젝트 메쉬 이름 저장
            string strTag = "";
            string strMeshName = "";
            for (int i = 0; i < LB_ObjectList.Items.Count; ++i)
            {
                strTag = LB_ObjectList.Items[i].ToString();
                strMeshName = m_coreWrapper.GetMeshName(strTag);
                if(m_objInfo[strTag].meshInfo != null)
                {
                    // MeshName
                    ObjectInfo.MeshInfo _meshInfo = new ObjectInfo.MeshInfo("");
                    m_objInfo[strTag].meshInfo = _meshInfo;
                }
                m_objInfo[strTag].meshInfo.m_strMeshName = strMeshName;
            }
        }

        private void ClearData(bool _isFileLoad)
        {
            // 데이터 지우기
            m_objInfo.Clear();

            // 아이템 목록 지우기
            LB_ObjectList.Items.Clear();
            LB_LogMessage.Items.Clear();
            if(_isFileLoad == false)
            {
                ClipList.Items.Clear();
                LB_BoneList.Items.Clear();
                LB_MeshList.Items.Clear();
                LB_FileMesh.Items.Clear();
                LB_AniList.Items.Clear();
            }
        }

        private void LoadGlobalLight()
        {
            string[] strLightList = m_coreWrapper.GetGlobalLightList();

            // Add Object ListBox
            for (int i = 0; i < strLightList.Length; ++i)
            {
                LB_ObjectList.Items.Add(strLightList[i]);
            }
        }

        private void LoadObjList()
        {
            m_objInfo.Clear();

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

                    // World Pivot
                    float[] arrFPivot  = m_transform.ObjGetWorldPivot(strTag, strLayer);

                    // 데이터 생성 및 추가
                    ObjectInfo info = new ObjectInfo
                    {
                        // Member
                        strLayerTag     = arrLayerTag[i],

                        // MeshName
                        meshInfo        = new ObjectInfo.MeshInfo(""),

                        // Vector
                        vecLScale       = new ObjectInfo.Vector3(arrFLScale),
                        vecLRotate      = new ObjectInfo.Vector3(arrFLRot),
                        vecLPosition    = new ObjectInfo.Vector3(arrFLPos),
                        vecWScale       = new ObjectInfo.Vector3(arrFWScale),
                        vecWRotate      = new ObjectInfo.Vector3(arrFWRot),
                        vecWPosition    = new ObjectInfo.Vector3(arrFWPos),
                        vecWPivot       = new ObjectInfo.Vector3(arrFPivot),
                        vecColor        = new ObjectInfo.Vector4(),
                    };

                    // 오브젝트 정보 추가
                    m_objInfo.Add(arrObjTag[j], info);

                    // 리스트 박스 아이템 추가
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
                bool isLoad = m_coreWrapper.LoadMeshFromFullPath(Path.GetFileNameWithoutExtension(f), f);
                if(isLoad == true)
                {
                    LB_FileMesh.Items.Add(Path.GetFileNameWithoutExtension(f));
                }
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

        public void ChangeSelectedObject(object sender, EventArgs e)
        {
            // 선택 오브젝트
            m_gameObj.SelectedObject(m_transform, LB_ObjectList, TB_ObjectTag, CB_LayerList);
        }

        private void Btn_CreateObjectClick(object sender, EventArgs e)
        {
            m_gameObj.CreateObject(LB_ObjectList, false);
        }

        private void Btn_DeleteObjectClick(object sender, EventArgs e)
        {
            string strTag = LB_ObjectList.SelectedItem.ToString();

            // 기타 오브젝트는 자신이 속한 레이어에서 삭제시켜준다.
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

        private void Btn_AddChild(object sender, EventArgs e)
        {
            if (LB_ObjectList.SelectedItem == null)
            {
                MessageBox.Show("선택된 오브젝트가 없습니다.");
                return;
            }
            m_gameObj.AddChild(LB_ObjectList, TB_ParentName);
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

        private void ColliderKeyPressCheck(object sender, KeyPressEventArgs e)
        {
            // -, .(소수점), BK(BackSpace, 8번), 0 ~9(숫자)를 제외한 문자는 검사하지 않는다.
            int changeASCII = Convert.ToInt32(e.KeyChar);
            if ((changeASCII >= '0' && changeASCII <= '9') ||
                 changeASCII == '-' || changeASCII == '.' || changeASCII == 8)
            {
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
                        ReadChildWorldPosition();
                    }
                }
            }
        }

        public void ReadChildWorldPosition()
        {
            m_transform.ReadChildWorldPosition(TB_ChildPosX, TB_ChildPosY, TB_ChildPosZ);
        }

        public void SetWorldPivot(object sender, EventArgs e)
        {
            ChangeSelectedObject(null, null);
            if (PivotX.Text != "" && PivotY.Text != "" && PivotZ.Text != "")
            {
                if (PivotX.Text != "-" && PivotY.Text != "-" && PivotZ.Text != "-")
                {
                    m_transform.ChangeWorldPivot((TextBox)sender, PivotX, PivotY, PivotZ);
                    ReadChildWorldPosition();
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
            m_renderer.SetMesh(Btn_AddComponent, LB_MeshList, LB_FileMesh, TB_SelecteMesh);

            // '메시 등록' 버튼을 클릭 시 호출된다.
            if (LB_MeshList.SelectedItem == null && LB_FileMesh.SelectedItem == null)
            {
                AddLogString("Error! 선택된 Mesh가 없습니다");
                return;
            }
        }

        private void BasicMeshClick(object sender, EventArgs e)
        {
            if (LB_ObjectList.SelectedItem == null)
            {
                return;
            }

            // 포커스 해제
            if (LB_FileMesh.SelectedItem != null)
            {
                LB_FileMesh.SelectedItem = null;
            }

            // Mesh
            string strMeshName = ((ListBox)sender).SelectedItem.ToString();
            ObjectInfo.MeshInfo meshInfo = m_objInfo[LB_ObjectList.SelectedItem.ToString()].meshInfo;
            if (meshInfo == null)
            {
                meshInfo = new ObjectInfo.MeshInfo(strMeshName);
                return;
            }
            meshInfo.m_strMeshName = strMeshName;
        }

        private void FileMeshClick(object sender, EventArgs e)
        {
            if (LB_ObjectList.SelectedItem == null)
            {
                return;
            }

            if (LB_MeshList.SelectedItem != null)
            {
                LB_MeshList.SelectedItem = null;
            }

            // FileMesh
            string strMeshName = ((ListBox)sender).SelectedItem.ToString();
            ObjectInfo.MeshInfo meshInfo = m_objInfo[LB_ObjectList.SelectedItem.ToString()].meshInfo;
			if (meshInfo == null)
			{
				meshInfo = new ObjectInfo.MeshInfo(strMeshName);
				return;
			}

            meshInfo.m_strMeshName = strMeshName;
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

        private void Btn_Add_6_Way_GlobalLight(object sender, EventArgs e)
        {
            // 6개의 전역광(Global Light) 추가
            m_light.Add_6_Way_GlobalLight();
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

                m_coreWrapper.SetBrushXSize(dTemp);
            }
        }

        private void TB_BrushZSize_TextChanged(object sender, EventArgs e)
        {
            if (TB_BrushZSize.Text != "")
            {
                int dTemp = Convert.ToInt32(TB_BrushZSize.Text);

                m_coreWrapper.SetBrushZSize(dTemp);
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

        private void LoadLayerComboBoxList()
        {
            string[] arrLayerTag = m_coreWrapper.GetLayerList();
            for (int i = 0; i < arrLayerTag.Length; ++i)
            {
                CB_LayerList.Items.Add(arrLayerTag[i].ToString());
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

        private void CB_PickColliderEnable_CheckedChanged(object sender, EventArgs e)
        {
        }

        private void CB_GizmoEnable_CheckedChanged(object sender, EventArgs e)
        {
            m_coreWrapper.GizmoEnable(CB_GizmoEnable.Checked);
        }

        private void CB_ChangeStageSection_Click(object sender, EventArgs e)
        {
            m_coreWrapper.SetStageSection(CB_StageSection.SelectedIndex);
        }

        private void CB_StageSection_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void BT_ColliderOBB_Click(object sender, EventArgs e)
        {
            if (LB_ObjectList.SelectedItem == null)
            {
                MessageBox.Show("선택된 Object가 없습니다.");
                return;
            }

            if (LB_ColliderID.SelectedItem == null || TB_ColliderTag.Text == "" || TB_OBBXLength.Text == ""
                || TB_OBBYLength.Text == "" || TB_OBBZLength.Text == "")
            {
                MessageBox.Show("Collider 생성 값에 잘못된 값이 입력되었습니다..");

                return;
            }

            double dRelativeCenterX;
            double dRelativeCenterY;
            double dRelativeCenterZ;
            double dXLength;
            double dYLength;
            double dZLength;

            if(TB_RelativeCenterX.Text == "")
            {
                dRelativeCenterX = 0;
            }
            else
            {
                dRelativeCenterX = Convert.ToDouble(TB_RelativeCenterX.Text);
            }

            if (TB_RelativeCenterY.Text == "")
            {
                dRelativeCenterY = 0;
            }
            else
            {
                dRelativeCenterY = Convert.ToDouble(TB_RelativeCenterY.Text);
            }


            if (TB_RelativeCenterZ.Text == "")
            {
                dRelativeCenterZ = 0;
            }
            else
            {
                dRelativeCenterZ = Convert.ToDouble(TB_RelativeCenterX.Text);
            }

            dXLength = Convert.ToDouble(TB_OBBXLength.Text);
            dYLength = Convert.ToDouble(TB_OBBYLength.Text);
            dZLength = Convert.ToDouble(TB_OBBZLength.Text);

            m_coreWrapper.AddColliderOBB(dRelativeCenterX, dRelativeCenterY, dRelativeCenterZ, dXLength, dYLength, dZLength, LB_ColliderID.SelectedIndex,
                TB_ColliderTag.Text , TB_CollTypeTag.Text , TB_CollExceptTag.Text, CB_MeshScale.Checked);

            LB_ColliderList.Items.Add(TB_ColliderTag.Text);
            //////////////////////Clear///////////////////////////////////////////
            TB_RelativeCenterX.Clear();
            TB_RelativeCenterY.Clear();
            TB_RelativeCenterZ.Clear();
            TB_OBBXLength.Clear();
            TB_OBBYLength.Clear();
            TB_OBBZLength.Clear();
            LB_ColliderID.ClearSelected();
            TB_ColliderTag.Clear();
            TB_CollTypeTag.Clear();
            TB_CollExceptTag.Clear();
        }

        private void BT_ColliderSphere_Click(object sender, EventArgs e)
        {
            if(LB_ObjectList.SelectedItem == null)
            {
                MessageBox.Show("선택된 Object가 없습니다.");
                return;
            }

            if (LB_ColliderID.SelectedItem == null || TB_ColliderTag.Text == "" || TB_SphereRadius.Text == "")
            {
                MessageBox.Show("Collider 생성 값에 잘못된 값이 입력되었습니다..");

                return;
            }

            double dSphereCenterX;
            double dSphereCenterY;
            double dSphereCenterZ;
            double dRadius;

            dRadius = Convert.ToDouble(TB_SphereRadius.Text);

            if (TB_SphereCenterX.Text == "")
            {
                dSphereCenterX = 0;
            }
            else
            {
                dSphereCenterX = Convert.ToDouble(TB_SphereCenterX.Text);
            }

            if (TB_SphereCenterY.Text == "")
            {
                dSphereCenterY = 0;
            }
            else
            {
                dSphereCenterY = Convert.ToDouble(TB_SphereCenterY.Text);
            }


            if (TB_SphereCenterZ.Text == "")
            {
                dSphereCenterZ = 0;
            }
            else
            {
                dSphereCenterZ = Convert.ToDouble(TB_SphereCenterZ.Text);
            }

            m_coreWrapper.AddColliderSphere(dSphereCenterX , dSphereCenterY , dSphereCenterZ , dRadius , LB_ColliderID.SelectedIndex,
                TB_ColliderTag.Text, TB_CollTypeTag.Text, TB_CollExceptTag.Text, CB_MeshScale.Checked);
            LB_ColliderList.Items.Add(TB_ColliderTag.Text);

            //////////////////////Clear///////////////////////////////////////////
            TB_SphereCenterX.Clear();
            TB_SphereCenterY.Clear();
            TB_SphereCenterZ.Clear();
            TB_SphereRadius.Clear();
            LB_ColliderID.ClearSelected();
            TB_ColliderTag.Clear();
            TB_CollTypeTag.Clear();
            TB_CollExceptTag.Clear();
        }

        private void BT_CollOBBUpdate_Click(object sender, EventArgs e)
        {
            if (LB_ObjectList.SelectedItem == null)
            {
                MessageBox.Show("선택된 Object가 없습니다.");
                return;
            }
            if(m_coreWrapper.GetCollType() == 4)
            {
                MessageBox.Show("선택된 Collider의 Type이 다릅니다.");
                return;

            }
            if (LB_ColliderList.SelectedItem == null)
            {
                MessageBox.Show("선택된 Collider가 없습니다.");
                return;
            }
            if(TB_OBBXLength.Text == "" || TB_OBBYLength.Text == "" || TB_OBBZLength.Text == "")
            {
                MessageBox.Show("Collider Info 값에 잘못된 값이 입력되었습니다..");
                return;
            }
            double dRelativeCenterX;
            double dRelativeCenterY;
            double dRelativeCenterZ;
            double dXLength;
            double dYLength;
            double dZLength;

            if (TB_RelativeCenterX.Text == "")
            {
                dRelativeCenterX = 0;
            }
            else
            {
                dRelativeCenterX = Convert.ToDouble(TB_RelativeCenterX.Text);
            }

            if (TB_RelativeCenterY.Text == "")
            {
                dRelativeCenterY = 0;
            }
            else
            {
                dRelativeCenterY = Convert.ToDouble(TB_RelativeCenterY.Text);
            }


            if (TB_RelativeCenterZ.Text == "")
            {
                dRelativeCenterZ = 0;
            }
            else
            {
                dRelativeCenterZ = Convert.ToDouble(TB_RelativeCenterX.Text);
            }

            dXLength = Convert.ToDouble(TB_OBBXLength.Text);
            dYLength = Convert.ToDouble(TB_OBBYLength.Text);
            dZLength = Convert.ToDouble(TB_OBBZLength.Text);

            m_coreWrapper.SetOBBColliderInfo(dRelativeCenterX, dRelativeCenterY, dRelativeCenterZ, dXLength, dYLength, dZLength , CB_MeshScale.Checked);
        }

        private void BT_CollSphere_Click(object sender, EventArgs e)
        {
            if (LB_ObjectList.SelectedItem == null)
            {
                MessageBox.Show("선택된 Object가 없습니다.");
                return;
            }

            if (m_coreWrapper.GetCollType() == 5)
            {
                MessageBox.Show("선택된 Collider의 Type이 다릅니다.");
                return;

            }

            if (LB_ColliderList.SelectedItem == null)
            {
                MessageBox.Show("선택된 Collider가 없습니다.");
                return;
            }

            if (TB_SphereRadius.Text == "")
            {
                MessageBox.Show("Collider Info 값에 잘못된 값이 입력되었습니다..");
                return;
            }
            double dSphereCenterX;
            double dSphereCenterY;
            double dSphereCenterZ;
            double dRadius;

            dRadius = Convert.ToDouble(TB_SphereRadius.Text);

            if (TB_SphereCenterX.Text == "")
            {
                dSphereCenterX = 0;
            }
            else
            {
                dSphereCenterX = Convert.ToDouble(TB_SphereCenterX.Text);
            }

            if (TB_SphereCenterY.Text == "")
            {
                dSphereCenterY = 0;
            }
            else
            {
                dSphereCenterY = Convert.ToDouble(TB_SphereCenterY.Text);
            }


            if (TB_SphereCenterZ.Text == "")
            {
                dSphereCenterZ = 0;
            }
            else
            {
                dSphereCenterZ = Convert.ToDouble(TB_SphereCenterZ.Text);
            }

            m_coreWrapper.SetSphereColliderInfo(dSphereCenterX, dSphereCenterY, dSphereCenterZ, dRadius, CB_MeshScale.Checked);
        }

        private void LB_ColliderList_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (LB_ColliderList.SelectedItem == null)
                return;

            TB_RelativeCenterX.Clear();
            TB_RelativeCenterY.Clear();
            TB_RelativeCenterZ.Clear();
            TB_OBBXLength.Clear();
            TB_OBBYLength.Clear();
            TB_OBBZLength.Clear();
            TB_SphereCenterX.Clear();
            TB_SphereCenterY.Clear();
            TB_SphereCenterZ.Clear();
            TB_SphereRadius.Clear();

            if (m_coreWrapper.FindActiveCollider(LB_ColliderList.SelectedItem.ToString()) == true)
            {
                if(m_coreWrapper.GetCollType() == 4) // Sphere
                {
                    float[] fArrCenter = m_coreWrapper.GetSphereCenter();
                    float fRadius = m_coreWrapper.GetSphereRadius();

                    TB_SphereCenterX.Text = Convert.ToString(fArrCenter[0]);
                    TB_SphereCenterY.Text = Convert.ToString(fArrCenter[1]);
                    TB_SphereCenterZ.Text = Convert.ToString(fArrCenter[2]);
                    TB_SphereRadius.Text = Convert.ToString(fRadius);
                }
                else if(m_coreWrapper.GetCollType() == 5) // OBB3D
                {
                    float[] fArrCenter = m_coreWrapper.GetOBBCenter();
                    float[] fArrLength = m_coreWrapper.GetOBBLength();

                    TB_RelativeCenterX.Text = Convert.ToString(fArrCenter[0]);
                    TB_RelativeCenterY.Text = Convert.ToString(fArrCenter[1]);
                    TB_RelativeCenterZ.Text = Convert.ToString(fArrCenter[2]);
                    TB_OBBXLength.Text = Convert.ToString(fArrLength[0]);
                    TB_OBBYLength.Text = Convert.ToString(fArrLength[1]);
                    TB_OBBZLength.Text = Convert.ToString(fArrLength[2]);

                }
            }
        }

        private void LB_ColliderID_SelectedIndexChanged(object sender, EventArgs e)
        {

        }
    }
}