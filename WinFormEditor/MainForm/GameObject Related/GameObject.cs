using System;
using System.Drawing;
using System.Windows.Forms;
using EngineWrapper;

namespace WinFormEditor
{
    public class GameObject
    {
        // Member
        private int  m_createObjCnt = 0;
        private int  m_beforeSelectedIndex = -1;
        private bool m_isLoadTransform = false;
        Form m_dataRemoveForm = new Form();

        // Instance
        private EditorForm m_editForm = null;

        public void Init(EditorForm _editForm)
        {
            m_editForm = _editForm;
            if (m_editForm == null)
            {
                m_editForm.AddLogString("EditForm is null");
            }
        }

        public void SetPickingObjectInfo(ListBox _listBox, ListBox _meshListBox, ListBox _fileMeshList, TextBox _textBox)
        {
            CoreWrapper wrapper = m_editForm.GetWrapper();
            string[] arrInfo = wrapper.GetActiveObjectInfo();

            // Object ListBox 정보를 갱신한다.
            string strTag = arrInfo[0];
            string strLayerTag = arrInfo[1];
            int findIndex = _listBox.FindString(strTag);
            _listBox.SelectedIndex = findIndex;

            // Mesh ListBox 정보를 갱신한다.
            if (wrapper.FindRendererComponent() == true)
            {
                string strMeshName = "";
                if (_meshListBox.SelectedItem != null)
                {
                    strMeshName = _meshListBox.SelectedItem.ToString();
                }
                else if (_fileMeshList.SelectedItem != null)
                {
                    strMeshName = _fileMeshList.SelectedItem.ToString();
                }
                _textBox.Text = strMeshName;
            }
            else
            {
                _textBox.Text = "None";
                _meshListBox.SelectedItem = null;
                _fileMeshList.SelectedItem = null;
            }
        }

        public string GetSelectObjectTag()
        {
            ListBox _listBox = m_editForm.GetObjectListBox();
            if (_listBox.SelectedItem != null)
            {
               return _listBox.SelectedItem.ToString();
            }

            return "";
        }

        public void SelectedObject(Transform _instTr, ListBox _listBox, TextBox _textBox, ComboBox _comboBox)
        {
            if (_listBox.SelectedItem == null)
            {
                return;
            }

            // 선택된 오브젝트가 이전 오브젝트와 다를경우 Transform 정보를 1회 갱신한다.
            if (m_beforeSelectedIndex != _listBox.SelectedIndex)
            {
                if (m_isLoadTransform == false)
                {
                    m_isLoadTransform = true;
                    m_editForm.GetTransformInst().m_isUpdateTr = true;
                }
            }

            CoreWrapper wrapper = m_editForm.GetWrapper();

            // 오브젝트 선택
            int itemIndex = _listBox.SelectedIndex;
            string strTag = _listBox.Items[itemIndex].ToString();
            string strLayerTag = m_editForm.GetObjInfo()[strTag].strLayerTag;
            wrapper.SetActiveObject(strTag, strLayerTag);
            m_beforeSelectedIndex = itemIndex;

            // 부모 오브젝트 검사
            string strParentTag = wrapper.GetParentTag();
            TextBox parentTextBox = m_editForm.GetParentTextBox();
            parentTextBox.Text = strParentTag;

            // Transform
            m_editForm.ReadChildWorldPosition();
            _instTr.LoadTransform();
            m_isLoadTransform = false;
            m_editForm.GetTransformInst().m_isUpdateTr = false;

            // Light 컴포넌트가 있는지 검사한다.
            bool isFind = wrapper.FindLightComponent();
            m_editForm.GetAddLightComponentBtn().BackColor = SystemColors.ControlLight;
            if (isFind == true)
            {
                m_editForm.GetAddLightComponentBtn().BackColor = Color.LawnGreen;
                m_editForm.GetLightInst().SetLightToolsEnable(true);
            }
            else
            {
                m_editForm.GetLightInst().SetLightToolsEnable(false);
            }

            // Render 컴포넌트가 있는지 검사한다.
            m_editForm.GetRenderInst().SetRenderingInfo();

            // 오브젝트 텍스트 박스 
            _textBox.Text = strTag;

            // 레이어 콤보 박스
            if (strLayerTag == "Stage")         { _comboBox.SelectedIndex = 0; }
            else if (strLayerTag == "Default")  { _comboBox.SelectedIndex = 1; }
            else if (strLayerTag == "UI")       { _comboBox.SelectedIndex = 3; }
        }

        public void CreateObject(ListBox _listBox, bool _isChild = false)
        {
            CoreWrapper wrapper = m_editForm.GetWrapper();

            // 생성
            string strLayerTag  = "Default";
            string strObjectTag = "GameObject_" + m_createObjCnt;
            while(true)
            {
                if(m_editForm.GetObjInfo().ContainsKey(strObjectTag) == true)
                {
                    m_createObjCnt++;
                    strObjectTag = "GameObject_" + m_createObjCnt;
                }
                else
                {
                    break;
                }
            }
            wrapper.CreateObject(strObjectTag, strLayerTag, _isChild);
            ++m_createObjCnt;

            // 데이터 추가
            ObjectInfo info = new ObjectInfo
            {
                strLayerTag     = strLayerTag,
                meshInfo        = new ObjectInfo.MeshInfo(""),
                vecLScale       = new ObjectInfo.Vector3(),
                vecLRotate      = new ObjectInfo.Vector3(),
                vecLPosition    = new ObjectInfo.Vector3(),
                vecWScale       = new ObjectInfo.Vector3(),
                vecWRotate      = new ObjectInfo.Vector3(),
                vecWPosition    = new ObjectInfo.Vector3(),
                vecWPivot       = new ObjectInfo.Vector3(),
                vecColor        = new ObjectInfo.Vector4(),
            };
            m_editForm.GetObjInfo().Add(strObjectTag, info);
            _listBox.Items.Add(strObjectTag);

            // 포커스 활성화
            int itemIndex = _listBox.Items.Count - 1;
            _listBox.SelectedItem = _listBox.Items[itemIndex];
            m_editForm.AddLogString(strObjectTag + " 오브젝트가 생성되었습니다.");
        }

        public void DeleteObject(ListBox _listBox, EventHandler _listboxEvent,
                                 TextBox _textBox, ComboBox _comboBox, EventHandler _comboBoxEvent)
        {
            if (_listBox.SelectedItem == null)
            {
                MessageBox.Show( "선택된 오브젝트가 없습니다.");
                return;
            }

            CoreWrapper wrapper = m_editForm.GetWrapper();
            int SelectIndex = _listBox.SelectedIndex;

            // ListBox 이벤트 삭제 { ... } 등록
            _listBox.SelectedIndexChanged -= _listboxEvent;
            {
                string strObjectTag = _listBox.SelectedItem.ToString();
                string strLayerTag = m_editForm.GetObjInfo()[strObjectTag].strLayerTag;

                // 삭제
                wrapper.DeleteObject(strObjectTag, strLayerTag);
                m_editForm.GetObjInfo().Remove(strObjectTag);
                _listBox.Items.Remove(_listBox.SelectedItem);
            }
            _listBox.SelectedIndexChanged += _listboxEvent;

            // 오브젝트 정보 초기화
            if (_listBox.SelectedIndex == -1)
            {
                // ComboBox 이벤트 삭제 { ... } 등록
                _comboBox.TextChanged -= _comboBoxEvent;
                _comboBox.SelectedItem = null;
                _comboBox.TextChanged += _comboBoxEvent;
            }

            _textBox.Text = "";
            _comboBox.SelectedItem = null;

            // 포커스 해제
            _listBox.SelectedItem = null;

            // 더이상 삭제 될 오브젝트가 없다.
            if (SelectIndex - 1 < 0)
            {
                SelectIndex = 0;
            }
            _listBox.SelectedIndex = SelectIndex - 1;
            m_editForm.AddLogString("오브젝트가 삭제되었습니다.");
        }

        public void DeleteAllObject(Form _form)
        {
            m_dataRemoveForm = _form;

            // Label
            Label labelRemoveInfo = new Label
            {
                AutoSize    = true,
                Font        = new Font("맑은 고딕", 10, FontStyle.Bold),
                Text        = "데이터를 삭제하시겠습니까?",
                TextAlign   = ContentAlignment.MiddleCenter,
                Left        = 8,
                Top         = 11
            };
            m_dataRemoveForm.Controls.Add(labelRemoveInfo);

            // Button(예)
            Button btnRemove = new Button
            {
                Location    = new Point(20, 38),
                Size        = new Size(75, 35),
                Font        = new Font("맑은 고딕", 10, FontStyle.Bold),
                Text        = "Remove",
                ForeColor   = Color.Red,
                TabStop     = false
            };
            btnRemove.FlatAppearance.BorderSize = 1;
            btnRemove.Click += new System.EventHandler(BtnRemove);
            m_dataRemoveForm.Controls.Add(btnRemove);

            // Button(아니오)
            Button btnCancel = new Button
            {
                Location    = new Point(105, 38),
                Size        = new Size(75, 35),
                Font        = new Font("맑은 고딕", 10, FontStyle.Bold),
                Text        = "Cancel",
                ForeColor   = Color.Blue,
                TabStop     = false
            };
            btnCancel.FlatAppearance.BorderSize = 1;
            btnCancel.Click += new EventHandler(BtnCancel);
            m_dataRemoveForm.Controls.Add(btnCancel);
        }

        private void BtnRemove(object sender, EventArgs e)
        {
            CoreWrapper wrapper = m_editForm.GetWrapper();
            ListBox objListBox  = m_editForm.GetObjectListBox();

            // 아이템 삭제
            m_createObjCnt = 0;
            m_dataRemoveForm.Close();

            // 오브젝트 삭제
            string[] arrLayerTag = wrapper.GetLayerList();
            for (int i = 0; i < arrLayerTag.Length; ++i)
            {
                string[] arrObjTag = wrapper.GetSelectLayerObjList(arrLayerTag[i]);
                for (int j = 0; j < arrObjTag.Length; ++j)
                {
                    if (arrObjTag[j] != "GlobalLight" && arrObjTag[j] != "FreeCamObj")
                    {
                        objListBox.Items.Remove((string)arrObjTag[j]);
                        m_editForm.GetObjInfo().Remove(arrObjTag[j]);
                        wrapper.DeleteObject(arrObjTag[j], arrLayerTag[i]);
                    }
                }
            }
            MessageBox.Show("데이터를 전부 삭제했습니다.");
        }

        private void BtnCancel(object sender, EventArgs e)
        {
            m_dataRemoveForm.Close();
        }

        public void ChangeTag(ListBox _listBox, EventHandler _event, TextBox _textBox)
        {
            CoreWrapper wrapper = m_editForm.GetWrapper();

            if (_listBox.SelectedItem == null)
            {
                _textBox.Text = "";
                MessageBox.Show("선택된 오브젝트가 없습니다.");
                return;
            }
            // 오브젝트의 이름 중복
            if (m_editForm.GetObjInfo().ContainsKey(_textBox.Text) == true)
            {
                MessageBox.Show("같은 이름의 오브젝트가 존재합니다.");
                return;
            }

            // 기존 데이터 보관 및 삭제
            int itemIndex = _listBox.SelectedIndex;
            string strTag = _listBox.Items[itemIndex].ToString();
            ObjectInfo tempData     = new ObjectInfo();
            tempData.strLayerTag    = m_editForm.GetObjInfo()[strTag].strLayerTag;
			tempData.meshInfo       = new ObjectInfo.MeshInfo("");
            tempData.vecLPosition   = new ObjectInfo.Vector3();
            tempData.vecLRotate     = new ObjectInfo.Vector3();
            tempData.vecLScale      = new ObjectInfo.Vector3();
            tempData.vecWScale      = new ObjectInfo.Vector3();
            tempData.vecWRotate     = new ObjectInfo.Vector3();
            tempData.vecWPosition   = new ObjectInfo.Vector3();
            tempData.vecWPivot      = new ObjectInfo.Vector3();
            tempData.vecColor       = new ObjectInfo.Vector4();
            m_editForm.GetObjInfo().Remove(strTag);

            // 변경
            strTag = _textBox.Text;
            wrapper.ChangeObjectTag(strTag);

            // 데이터 추가
            m_editForm.GetObjInfo().Add(strTag, tempData);

            // ListBox 이벤트 삭제 { ... } 등록
            _listBox.SelectedIndexChanged -= _event;
            {
                _listBox.Items[_listBox.SelectedIndex] = _textBox.Text;
            }
            _listBox.SelectedIndexChanged += _event;
        }

        public void AddChild(ListBox _listBox, TextBox _textBox)
        {
            // 오브젝트 생성
            string strParentTag = _listBox.SelectedItem.ToString();
            _textBox.Text = strParentTag;
            CreateObject(_listBox, true);

            // Wrapper를 통하여 AddChild() 함수를 호출
            CoreWrapper wrapper = m_editForm.GetWrapper();

            string strLayerTag = m_editForm.GetObjInfo()[strParentTag].strLayerTag;
            wrapper.AddChild(strParentTag, strLayerTag);

            // Change ParentTag 
            strParentTag = wrapper.GetParentTag();
            TextBox parentTextBox = m_editForm.GetParentTextBox();
            parentTextBox.Text = strParentTag;
        }
    }
}