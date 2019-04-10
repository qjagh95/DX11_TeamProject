using System;
using System.IO;
using System.Windows.Forms;
using System.Collections.Generic;
using EngineWrapper;
using System.Collections;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

namespace WinFormEditor
{
    public partial class EditorForm : Form
    {
        // CoreWrapper(CLR) 객체
        public CoreWrapper coreWrapper = new CoreWrapper();

        enum eTransformType { TT_SCALE, TT_ROTATE, TT_POSITION, TT_MAX, };
        enum eLightType { LT_DIR, LT_POINT, LT_SPOT };

        // Form
        Form m_dataRemoveForm = new Form();

        // Unique Key
        Dictionary<string, ObjectInfo> m_objInfo;

        // Member
        private int m_createObjCnt = 0;
        private bool m_isChangeText = false;
        private string m_strClipName = "";
        private string m_strStartFrame = "";
        private string m_strEndFrame = "";
        private string m_strAniTime = "";
        private string m_strDeleteClip = "";
        private EventHandler m_event = null;

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

            PB_DeviceWindow.MouseWheel += new MouseEventHandler(Mouse_Wheel);

            //hInstance, hwnd얻어옴.
            IntPtr hinstance = System.Runtime.InteropServices.Marshal.GetHINSTANCE(PB_DeviceWindow.GetType().Module);
            IntPtr DeviceHwnd = PB_DeviceWindow.Handle;
            coreWrapper.Init(DeviceHwnd, hinstance);

            // Connection Run Funtion
            Application.Idle += Run;

            // Dictionary 생성
            m_objInfo = new Dictionary<string, ObjectInfo>();

            // 데이터 로드
            ClearData();
            LoadObjectList();
            LoadMeshList();
            LoadAnmFile();
            LoadLayerComboBoxList();
            LoadTransform(0);

            CB_WireFrame.Checked = true;
            CB_isDebugTarget.Checked = true;
            CB_LightType.SelectedIndex = 0;
            TB_Range.Value = 10;
            TB_LightXDir.Value = 0;
            TB_LightYDir.Value = -1;
            TB_LightZDir.Value = 0;
        }

        private void Delete(object sender, FormClosedEventArgs e)
        {
            coreWrapper.Delete();
        }

        private void ClearData()
        {
            // 데이터 지우기
            m_objInfo.Clear();

            // 아이템 모두 지우기
            LB_MeshList.Items.Clear();
            LB_ObjectList.Items.Clear();
        }

        /*******************************************************************************************************/

        private void ObjectAllClear(object sender, MouseEventArgs e)
        {
            Point parentPoint = Location;

            // Form
            m_dataRemoveForm = new Form();
            m_dataRemoveForm.Owner = this;
            m_dataRemoveForm.StartPosition = FormStartPosition.Manual;
            m_dataRemoveForm.Size = new Size(218, 120);
            m_dataRemoveForm.Location = new Point(parentPoint.X + 13, parentPoint.Y + 125);
            m_dataRemoveForm.Show();

            // Label
            Label labelRemoveInfo = new Label();
            labelRemoveInfo.AutoSize = true;
            labelRemoveInfo.Font = new Font("맑은 고딕", 10, FontStyle.Bold);
            labelRemoveInfo.Text = "데이터를 삭제하시겠습니까?";
            labelRemoveInfo.TextAlign = ContentAlignment.MiddleCenter;
            labelRemoveInfo.Left = 8;
            labelRemoveInfo.Top = 11;
            m_dataRemoveForm.Controls.Add(labelRemoveInfo);

            // Button (예)
            Button btnRemove = new Button();
            btnRemove.Location = new Point(20, 38);
            btnRemove.Size = new Size(75, 35);
            btnRemove.Font = new Font("맑은 고딕", 10, FontStyle.Bold);
            btnRemove.Text = "Remove";
            btnRemove.ForeColor = Color.Red;
            btnRemove.TabStop = false;
            btnRemove.FlatAppearance.BorderSize = 1;
            btnRemove.Click += new System.EventHandler(BtnRemove);
            m_dataRemoveForm.Controls.Add(btnRemove);

            // Button (아니오)
            Button btnCancel = new Button();
            btnCancel.Location = new Point(105, 38);
            btnCancel.Size = new Size(75, 35);
            btnCancel.Font = new Font("맑은 고딕", 10, FontStyle.Bold);
            btnCancel.Text = "Cancel";
            btnCancel.ForeColor = Color.Blue;
            btnCancel.TabStop = false;
            btnCancel.FlatAppearance.BorderSize = 1;
            btnCancel.Click += new EventHandler(BtnCancel);
            m_dataRemoveForm.Controls.Add(btnCancel);
        }

        private void BtnRemove(object sender, EventArgs e)
        {
            // 오브젝트 전부 삭제
            coreWrapper.DeleteAllObject();

            // 아이템 전부 삭제
            m_createObjCnt = 0;
            LB_ObjectList.Items.Clear();
            m_dataRemoveForm.Close();
            MessageBox.Show("데이터를 전부 삭제했습니다.");
        }

        private void BtnCancel(object sender, EventArgs e)
        {
            m_dataRemoveForm.Close();
        }

        private void LoadObjectList()
        {
            // 데이터 지우기
            m_objInfo.Clear();

            // 아이템 모두 지우기
            LB_ObjectList.Items.Clear();

            // 오브젝트 목록
            string[] arrLayerTag = coreWrapper.GetLayerList();
            for (int i = 0; i < arrLayerTag.Length; ++i)
            {
                string[] arrObjTag = coreWrapper.GetSelectLayerObjList(arrLayerTag[i]);
                for (int j = 0; j < arrObjTag.Length; ++j)
                {
                    // 데이터 추가
                    ObjectInfo info = new ObjectInfo();
                    info.strLayerTag = arrLayerTag[i];
                    info.vecScale = new ObjectInfo.Vector3();
                    info.vecRotate = new ObjectInfo.Vector3();
                    info.vecPosition = new ObjectInfo.Vector3();
                    m_objInfo.Add(arrObjTag[j], info);

                    // 아이템 추가
                    LB_ObjectList.Items.Add(arrObjTag[j]);
                }
            }
        }

        private void LoadAnmFile()
        {
            string curDirectory = Directory.GetCurrentDirectory();
            string path = Directory.GetParent(curDirectory).Parent.Parent.FullName;
            path += "\\3DClient\\Bin\\MeshData\\";
            foreach (string f in Directory.GetFiles(path, "*.anm"))
            {
                coreWrapper.LoadMeshFromFullPath(Path.GetFileNameWithoutExtension(f), f);
                LB_AniList.Items.Add(Path.GetFileNameWithoutExtension(f));
            }
        }

        private void LoadMeshList()
        {
            // 메시 목록
            string[] arrString = coreWrapper.GetMeshNameList();
            for (int i = 0; i < arrString.Length; ++i)
            {
                LB_MeshList.Items.Add(arrString[i].ToString());
            }

            // .msh 파일 로드
            string curDirectory = Directory.GetCurrentDirectory();
            string path = Directory.GetParent(curDirectory).Parent.Parent.FullName;
            path += "\\3DClient\\Bin\\MeshData\\";
            foreach (string f in Directory.GetFiles(path, "*.msh"))
            {
                coreWrapper.LoadMeshFromFullPath(Path.GetFileNameWithoutExtension(f), f);
                LB_FileMesh.Items.Add(Path.GetFileNameWithoutExtension(f));
            }
        }

        private void LoadLayerComboBoxList()
        {
            string[] arrLayerTag = coreWrapper.GetLayerList();
            for (int i = 0; i < arrLayerTag.Length; ++i)
            {
                CB_LayerList.Items.Add(arrLayerTag[i].ToString());
            }
        }

        /*******************************************************************************************************/
        // Layer, Object
        // 최종 수정 : 2019.03.24 (김동규)
        // 문제 있으면 바로 말해주세요 !!

        private void ObjectList_ChangeSelectedItem(object sender, EventArgs e) // 19.03.24
        {
            if (LB_ObjectList.SelectedItem == null)
            {
                return;
            }

            // 오브젝트 선택
            int itemIndex = LB_ObjectList.SelectedIndex;
            string strObjectTag = LB_ObjectList.Items[itemIndex].ToString();
            string strLayerTag = m_objInfo[strObjectTag].strLayerTag;
            coreWrapper.SetActiveObject(strObjectTag, strLayerTag);

            // Transform
            LoadTransform(itemIndex);

            // 오브젝트 텍스트 박스 
            TB_ObjectTag.Text = strObjectTag;

            // 레이어 콤보 박스
            int comboBoxIndex = 0;
            if (strLayerTag == "Stage")
            {
                comboBoxIndex = 0;
            }
            else if (strLayerTag == "Default")
            {
                comboBoxIndex = 1;
            }
            else if (strLayerTag == "UI")
            {
                comboBoxIndex = 2;
            }

            CB_LayerList.SelectedIndex = comboBoxIndex;
        }

        private void Btn_CreateObject(object sender, EventArgs e)
        {
            // 오브젝트 생성
            string strObjectTag = "GameObject_" + m_createObjCnt;
            string strLayerTag = "Default";
            coreWrapper.CreateObject(strObjectTag, strLayerTag);
            ++m_createObjCnt;

            // 데이터 추가
            ObjectInfo info = new ObjectInfo();
            info.strLayerTag = strLayerTag;
            info.vecScale = new ObjectInfo.Vector3();
            info.vecRotate = new ObjectInfo.Vector3();
            info.vecPosition = new ObjectInfo.Vector3();
            m_objInfo.Add(strObjectTag, info);

            // 아이템 추가
            LB_ObjectList.Items.Add(strObjectTag);

            // 만들어진 오브젝트 포커스 활성화
            int itemIndex = LB_ObjectList.Items.Count - 1;
            LB_ObjectList.SelectedItem = LB_ObjectList.Items[itemIndex];
            AddLogString("오브젝트가 생성되었습니다.");
        }

        private void Btn_DeleteObject(object sender, EventArgs e)
        {
            if (LB_ObjectList.SelectedItem == null)
            {
                MessageBox.Show("선택된 오브젝트가 없습니다.");
                return;
            }

            int SelectIndex = LB_ObjectList.SelectedIndex;

            // 오브젝트, 데이터 아이템 삭제
            LB_ObjectList.SelectedIndexChanged -= ObjectList_ChangeSelectedItem;

            string strObjectTag = LB_ObjectList.SelectedItem.ToString();
            string strLayerTag = m_objInfo[strObjectTag].strLayerTag;

            coreWrapper.DeleteObject(strObjectTag, strLayerTag);
            m_objInfo.Remove(strObjectTag);
            LB_ObjectList.Items.Remove(LB_ObjectList.SelectedItem);
            LB_ObjectList.SelectedIndexChanged += ObjectList_ChangeSelectedItem;

            // 오브젝트 정보 초기화
            if (LB_ObjectList.SelectedIndex == -1)
            {
                // ComboBox 이벤트 제거
                CB_LayerList.TextChanged -= ChangeLayerComboBox;
                CB_LayerList.SelectedItem = null;
                CB_LayerList.TextChanged += ChangeLayerComboBox;
            }

            // 오브젝트 텍스트 박스 
            TB_ObjectTag.Text = "";

            // 레이어 콤보 박스
            CB_LayerList.SelectedItem = null;

            // 포커스 해제
            LB_ObjectList.SelectedItem = null;
            AddLogString("오브젝트가 삭제되었습니다.");

            if (SelectIndex - 1 < 0)
                SelectIndex = 0;

            LB_ObjectList.SelectedIndex = SelectIndex - 1;
        }

        private void KeyDown_DeleteObject(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Delete)
            {
                if (LB_ObjectList.SelectedItem == null)
                {
                    return;
                }

                // Delete Key로 오브젝트 목록을 삭제하는 기능.
                string strObjectTag = LB_ObjectList.SelectedItem.ToString();
                Btn_DeleteObject(strObjectTag, e);
            }
        }

        private void ChangeObjectTagEnter(object sender, KeyPressEventArgs e)
        {
            // 'Enter' 입력
            if (Convert.ToInt32(e.KeyChar) == 13)
            {
                if (LB_ObjectList.SelectedItem == null)
                {
                    MessageBox.Show("선택된 오브젝트가 없습니다.");
                    TB_ObjectTag.Text = "";
                    return;
                }
                // 오브젝트의 이름 중복
                if (m_objInfo.ContainsKey(TB_ObjectTag.Text) == true)
                {
                    MessageBox.Show("같은 이름의 오브젝트가 존재합니다.");
                    return;
                }

                // 기존 데이터 보관 및 삭제
                int itemIndex = LB_ObjectList.SelectedIndex;
                string strObjectTag = LB_ObjectList.Items[itemIndex].ToString();
                ObjectInfo tempData = new ObjectInfo();
                tempData.strLayerTag = m_objInfo[strObjectTag].strLayerTag;
                tempData.vecScale = new ObjectInfo.Vector3();
                tempData.vecRotate = new ObjectInfo.Vector3();
                tempData.vecPosition = new ObjectInfo.Vector3();
                m_objInfo.Remove(strObjectTag);

                // 변경
                strObjectTag = TB_ObjectTag.Text;
                coreWrapper.ChangeObjectTag(strObjectTag);

                // 데이터 추가
                m_objInfo.Add(strObjectTag, tempData);

                // 이벤트 제어
                LB_ObjectList.SelectedIndexChanged -= ObjectList_ChangeSelectedItem;
                LB_ObjectList.Items[LB_ObjectList.SelectedIndex] = TB_ObjectTag.Text;
                LB_ObjectList.SelectedIndexChanged += ObjectList_ChangeSelectedItem;
            }
        }

        private void ChangeLayerComboBox(object sender, EventArgs e)
        {
            if (LB_ObjectList.SelectedItem != null)
            {
                int itemIndex = LB_ObjectList.SelectedIndex;
                string strObjectTag = LB_ObjectList.Items[itemIndex].ToString();
                string strLayerTag = m_objInfo[strObjectTag].strLayerTag;
                m_objInfo[strObjectTag].strLayerTag = CB_LayerList.SelectedItem.ToString();
                coreWrapper.ChangeObjectInLayer(m_objInfo[strObjectTag].strLayerTag);
                return;
            }
        }

        /*******************************************************************************************************/
        // Key Check
        // 최종 수정 : 2019.03.24 (김동규)
        // 문제 있으면 바로 말해주세요 !!

        private void KeyPressCheck(object sender, KeyPressEventArgs e)
        {
            // -, .(소수점), BK(BackSpace, 8번), 0 ~9(숫자)를 제외한 문자는 검사하지 않는다.
            int changeASCII = Convert.ToInt32(e.KeyChar);
            if ((changeASCII >= '0' && changeASCII <= '9') ||
                 changeASCII == '-' || changeASCII == '.' || changeASCII == 8)
            {
                m_isChangeText = true;
            }
            else
            {
                // 위 키가 아닌 잘못된 형식의 값은 입력될 수 없다.
                e.Handled = true;
            }
        }

        private bool CheckStringForm(TextBox _textBox)
        {
            // 변경된 문자열 처리
            if (m_isChangeText == true)
            {
                m_isChangeText = false;

                int negativeCnt = 0;
                int decimalCnt = 0;

                // 문자열을 검사한다.
                for (int i = 0; i < _textBox.Text.Length; ++i)
                {
                    string subStr = _textBox.Text.Substring(i, 1);

                    // 문자열 0번째 위치에 .(소수점) 표시가 들어오는지 검사한다.
                    if (i == 0 && subStr.CompareTo(".") == 0)
                    {
                        SetBeforeTBText(_textBox);
                        return false;
                    }
                    else if (i > 0 && subStr.CompareTo(".") == 0)
                    {
                        subStr = _textBox.Text.Substring(i - 1, 1);
                        if (subStr.CompareTo("-") == 0)
                        {
                            SetBeforeTBText(_textBox);
                            return false;
                        }
                        else
                        {
                            if (decimalCnt == 0)
                            {
                                ++decimalCnt;
                                continue;
                            }
                            else
                            {
                                SetBeforeTBText(_textBox);
                                return false;
                            }
                        }
                    }

                    // -(음수) 문자가 문자열 0번째 위치가 아닌지 검사한다. 
                    if (i > 0 && subStr.CompareTo("-") == 0)
                    {
                        SetBeforeTBText(_textBox);
                        return false;
                    }
                    else if (i == 0 && subStr.CompareTo("-") == 0)
                    {
                        if (negativeCnt == 0)
                        {
                            ++negativeCnt;
                            continue;
                        }
                        else
                        {
                            SetBeforeTBText(_textBox);
                            return false;
                        }
                    }
                }
                return true;
            }
            return false;
        }

        private void SetBeforeTBText(TextBox _textBox)
        {
            // 오브젝트가 없는 경우
            if (LB_ObjectList.SelectedItem == null)
            {
                MessageBox.Show("선택된 오브젝트가 없습니다.");
            }
            else
            {
                MessageBox.Show("잘못된 형식의 값이 들어왔습니다.");
            }

            // ChangePosition 이벤트 삭제
            _textBox.TextChanged -= m_event;

            if (_textBox.Text.Length > 0)
            {

                int strLength = _textBox.Text.Length;
                _textBox.Text = _textBox.Text.Substring(0, strLength - 1);
                _textBox.Select(_textBox.Text.Length, 0);
            }

            // ChangePosition 이벤트 추가
            _textBox.TextChanged += m_event;
        }

        /*******************************************************************************************************/
        // Transform Component
        // 최종 수정 : 2019.03.24 (김동규)
        // 문제 있으면 바로 말해주세요 !!

        private bool Data_StringFormCheck(TextBox _sender, TextBox _tbX, TextBox _tbY, TextBox _tbZ)
        {
            // 선택된 오브젝트 여부 검사
            // if(g_strObjTag != "") 조건으로도 가능
            if (LB_ObjectList.SelectedItem == null)
            {
                SetBeforeTBText(_sender);
                return false;
            }

            // 음수를 표기할때 음수 자체만으로 데이터를 변환할 수 없다.
            if (_tbX.Text == "-" || _tbY.Text == "-" || _tbZ.Text == "-")
            {
                return false;
            }

            // 형식 검사
            return CheckStringForm(_sender);
        }

        private void ChangePosition(object sender, EventArgs e)
        {
            m_event = ChangePosition;

            // 오브젝트 위치 변경
            if (PositionX.Text != "" && PositionY.Text != "" && PositionZ.Text != "")
            {
                bool isCheck = Data_StringFormCheck((TextBox)sender, PositionX, PositionY, PositionZ);
                if (isCheck == true)
                {
                    double dx = Convert.ToDouble(PositionX.Text);
                    double dy = Convert.ToDouble(PositionY.Text);
                    double dz = Convert.ToDouble(PositionZ.Text);
                    coreWrapper.ActiveObjSetPosition(dx, dy, dz);
                    AddLogString(LB_ObjectList.SelectedItem.ToString() + "의 위치가 (" + dx + ", " + dy + ", " + dz + ")로 변경되었습니다.");
                }
            }
        }

        private void ChangeScale(object sender, EventArgs e)
        {
            m_event = ChangeScale;

            // 크기 변경
            if (ScaleX.Text != "" && ScaleY.Text != "" && ScaleZ.Text != "")
            {
                bool isCheck = Data_StringFormCheck((TextBox)sender, ScaleX, ScaleY, ScaleZ);
                if (isCheck == true)
                {
                    double dx = Convert.ToDouble(ScaleX.Text);
                    double dy = Convert.ToDouble(ScaleY.Text);
                    double dz = Convert.ToDouble(ScaleZ.Text);
                    coreWrapper.ActiveObjSetScale(dx, dy, dz);
                    AddLogString(LB_ObjectList.SelectedItem.ToString() + "의 크기가 (" + dx + ", " + dy + ", " + dz + ")로 변경되었습니다.");
                }
            }
        }

        private void ChangeRotate(object sender, EventArgs e)
        {
            m_event = ChangeRotate;

            // 회전 변경
            if (RotateX.Text != "" && RotateY.Text != "" && RotateZ.Text != "")
            {
                bool isCheck = Data_StringFormCheck((TextBox)sender, RotateX, RotateY, RotateZ);
                if (isCheck == true)
                {
                    double dx = Convert.ToDouble(RotateX.Text);
                    double dy = Convert.ToDouble(RotateY.Text);
                    double dz = Convert.ToDouble(RotateZ.Text);
                    coreWrapper.ActiveObjSetRotate(dx, dy, dz);
                    AddLogString(LB_ObjectList.SelectedItem.ToString() + "의 각도가 (" + dx + ", " + dy + ", " + dz + ")로 변경되었습니다.");
                }
            }
        }

        private void LoadTransform(int _index)
        {
            if (LB_ObjectList.SelectedItem == null)
            {
                return;
            }

            string strObjectTag = LB_ObjectList.Items[_index].ToString();
            string strLayerTag = m_objInfo[strObjectTag].strLayerTag;

            // Get World Transform(Scale, Rotate, Position)
            float[] arrSacle = coreWrapper.GetWorldTransform(strObjectTag, strLayerTag, (int)eTransformType.TT_SCALE);
            float[] arrRotate = coreWrapper.GetWorldTransform(strObjectTag, strLayerTag, (int)eTransformType.TT_ROTATE);
            float[] arrPosition = coreWrapper.GetWorldTransform(strObjectTag, strLayerTag, (int)eTransformType.TT_POSITION);

            // Position
            ObjectInfo.Vector3 vPos = new ObjectInfo.Vector3(arrPosition[0], arrPosition[1], arrPosition[2]);
            m_objInfo[strObjectTag].vecPosition = vPos;
            PositionX.Text = Convert.ToString(vPos.x);
            PositionY.Text = Convert.ToString(vPos.y);
            PositionZ.Text = Convert.ToString(vPos.z);

            // Scale
            ObjectInfo.Vector3 vScale = new ObjectInfo.Vector3(arrSacle[0], arrSacle[1], arrSacle[2]);
            m_objInfo[strObjectTag].vecScale = vScale;
            ScaleX.Text = Convert.ToString(vScale.x);
            ScaleY.Text = Convert.ToString(vScale.y);
            ScaleZ.Text = Convert.ToString(vScale.z);

            // Rotate
            ObjectInfo.Vector3 vRotate = new ObjectInfo.Vector3(arrRotate[0], arrRotate[1], arrRotate[2]);
            m_objInfo[strObjectTag].vecRotate = vRotate;
            RotateX.Text = Convert.ToString(vRotate.x);
            RotateY.Text = Convert.ToString(vRotate.y);
            RotateZ.Text = Convert.ToString(vRotate.z);
        }

        /*******************************************************************************************************/
        // Animation Component
        // 최종 수정 : 2019.??.?? (천진호)
        // 문제 있으면 바로 말해주세요 !!



        /*******************************************************************************************************/
        // 파일 저장, 불러오기
        // 최종 수정 : 2019.03.24 (김동규)
        // 문제 있으면 바로 말해주세요 !!

        private void FileSave_Click(object sender, EventArgs e)
        {
            // SaveFileDialog 창 설정
            // Title  : Dialog 창 이름 설정
            // Filter : 파일 형식 부분
            // AddExtension : 확장명 추가 여부
            SaveFileDialog saveFile = new SaveFileDialog();
            string curDirectory = Directory.GetCurrentDirectory();
            string path = Directory.GetParent(curDirectory).Parent.Parent.FullName;
            path += "\\3DClient\\bin\\Data";
            saveFile.InitialDirectory = path;
            saveFile.Title = "다른 이름으로 저장";
            saveFile.Filter = "데이터 파일(*.dat)|*.dat|모든파일(*.*)|*.*";
            saveFile.DefaultExt = "dat";
            saveFile.AddExtension = true;
            if (saveFile.ShowDialog() == DialogResult.OK)
            {
                // 저장
                coreWrapper.FileSave(saveFile.FileName);
            }
        }

        private void FileLoad_Click(object sender, EventArgs e)
        {
            OpenFileDialog openFile = new OpenFileDialog();
            string curDirectory = Directory.GetCurrentDirectory();
            string path = Directory.GetParent(curDirectory).Parent.Parent.FullName;
            path += "\\3DClient\\bin\\Data";
            openFile.InitialDirectory = path;
            openFile.Title = "파일 불러오기";
            openFile.FileName = ".dat";
            openFile.Filter = "데이터 파일(*.dat) | *.dat; | 모든 파일(*.*) | *.*";
            if (openFile.ShowDialog() == DialogResult.OK)
            {
                // 불러오기
                coreWrapper.FileLoad(openFile.FileName);

                // 로드
                ClearData();
                LoadObjectList();
                LoadMeshList();
                LoadAnmFile();
            }
        }

        private void EditorForm_Load(object sender, EventArgs e)
        {
            AnimationOptionBox.Items.Add("AO_LOOP");
            AnimationOptionBox.Items.Add("AO_ONCE_RETURN");
            AnimationOptionBox.Items.Add("AO_ONCE_DESTROY");
        }

        private void SetMesh_Click(object sender, EventArgs e)
        {
            // '메시 등록' 버튼을 클릭 시 호출된다.
            if (LB_MeshList.SelectedItem == null && LB_FileMesh.SelectedItem == null)
            {
                AddLogString("Error! 선택된 Mesh가 없습니다");
                return;
            }

            string strTemp;
            if (LB_MeshList.SelectedItem != null)
            {
                strTemp = LB_MeshList.SelectedItem.ToString();
                coreWrapper.SetMesh(strTemp);
            }
            else if (LB_FileMesh.SelectedItem != null)
            {
                strTemp = LB_FileMesh.SelectedItem.ToString();
                coreWrapper.SetMesh(strTemp);
            }

        }

        private void AddRenderComponent(object sender, EventArgs e)
        {
            // 'Renderer' 컴포넌트 추가
            coreWrapper.AddRenderComponent();
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
                LB_FileMesh.Items.Add(fileName);
            }
        }

        private void Scene_Click(object sender, MouseEventArgs e)
        {
            PB_DeviceWindow.Focus();
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

            coreWrapper.DeleteClip(ClipList.SelectedItem.ToString());
            ClipList.Items.Remove(ClipList.SelectedItem);
        }

        void AddLogString(string _strLog)
        {
            string strLogTime = System.DateTime.Now.ToString();
            LB_LogMessage.Items.Insert(0, "[" + strLogTime + "] " + _strLog);
        }

        private void AnimationBtn_Click(object sender, EventArgs e)
        {
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

        private void BasicMeshClick(object sender, EventArgs e)
        {
            if (LB_FileMesh.SelectedItem != null)
                LB_FileMesh.SelectedItem = null;
        }

        private void FileMeshClick(object sender, EventArgs e)
        {
            if (LB_MeshList.SelectedItem != null)
                LB_MeshList.SelectedItem = null;
        }

        private void Mouse_Wheel(object sender, MouseEventArgs e)
        {
            coreWrapper.SetMouseWheel(e.Delta);
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

        public void LightAmbColor()
        {
            ColorDialog cd = new ColorDialog();

            if (cd.ShowDialog() == DialogResult.OK)
                coreWrapper.SetLightAmbient(cd.Color.R, cd.Color.G, cd.Color.B);
            else
                return;
        }

        public void LightDiffuse()
        {
            ColorDialog cd = new ColorDialog();

            if (cd.ShowDialog() == DialogResult.OK)
            {
                coreWrapper.SetLightDiffuse(cd.Color.R, cd.Color.G, cd.Color.B);
                AddLogString("색상값 변경 R : " + cd.Color.R.ToString() + "G : " + cd.Color.R.ToString() + "B : " + cd.Color.R.ToString());
            }
            else
                return;
        }

        private void LightButton_Click(object sender, EventArgs e)
        {
            coreWrapper.AddLightComponent();
            AddLogString("LightComponent 추가");
        }

        private void BT_DifColor_Click(object sender, EventArgs e)
        {
            LightDiffuse();
            AddLogString("ColorEditorOpen");
        }

        private void CB_LightType_SelectedIndexChanged(object sender, EventArgs e)
        {
            coreWrapper.SetLightType(CB_LightType.SelectedIndex);
            AddLogString("LightType이 " + CB_LightType.SelectedItem.ToString() + "로 변경되었습니다.");
        }

        private void TB_LightXDir_Scroll(object sender, EventArgs e)
        {
            coreWrapper.SetLightDirX(TB_LightXDir.Value);
        }

        private void TB_LightYDir_Scroll(object sender, EventArgs e)
        {
            coreWrapper.SetLightDirY(TB_LightYDir.Value);
        }

        private void TB_LightZDir_Scroll(object sender, EventArgs e)
        {
            coreWrapper.SetLightDirZ(TB_LightZDir.Value);
        }

        private void TB_Range_Scroll(object sender, EventArgs e)
        {
            coreWrapper.SetLightRange(TB_Range.Value);
        }

        private void CB_WireFrame_CheckedChanged(object sender, EventArgs e)
        {
            coreWrapper.SetLightWireFrame(CB_WireFrame.Checked);
        }

        private void CB_isDebugTarget_CheckedChanged(object sender, EventArgs e)
        {
            coreWrapper.SetTargetOnOff(CB_isDebugTarget.Checked);
        }

        private void TB_SpcularR_TextChanged(object sender, EventArgs e)
        {
            double dR = Convert.ToDouble(TB_SpcularR.Text);
            coreWrapper.SetLightSpclularR(dR);
        }

        private void TB_SpcularG_TextChanged(object sender, EventArgs e)
        {
            double dR = Convert.ToDouble(TB_SpcularG.Text);
            coreWrapper.SetLightSpclularG(dR);
        }

        private void TB_SpcularB_TextChanged(object sender, EventArgs e)
        {
            double dR = Convert.ToDouble(TB_SpcularB.Text);
            coreWrapper.SetLightSpclularB(dR);
        }

        private void TB_SpcularPower_TextChanged(object sender, EventArgs e)
        {
            double dR = Convert.ToDouble(TB_SpcularPower.Text);
            coreWrapper.SetLightSpclularW(dR);
        }
    }
}