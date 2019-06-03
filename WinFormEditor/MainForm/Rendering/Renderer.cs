using System;
using System.Drawing;
using System.Windows.Forms;
using EngineWrapper;

namespace WinFormEditor
{
    public class Renderer
    {
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

        public void AddComponent(Button _btn)
        {
            if(m_editForm.GetObjectListBox().SelectedItem == null)
            {
                MessageBox.Show("선택된 오브젝트가 없습니다.");
                return;
            }
            _btn.BackColor = Color.LawnGreen;

            // 컴포넌트 등록
            CoreWrapper wrapper = m_editForm.GetWrapper();
            wrapper.AddRenderComponent();
            m_editForm.AddLogString("Renderer 컴포넌트가 등록되었습니다.");
        }

        public void SetMesh(Button _btn, ListBox _meshList, ListBox _fileMesh, TextBox _textBox)
        {
            CoreWrapper wrapper = m_editForm.GetWrapper();
            if (m_editForm.GetObjectListBox().SelectedItem == null)
            {
                MessageBox.Show("선택된 오브젝트가 없습니다.");
                return;
            }

            if (wrapper.FindRendererComponent() == false)
            {
                MessageBox.Show("Renderer 컴포넌트가 없습니다.");
                return;
            }

            if (_meshList.SelectedItem == null && _fileMesh.SelectedItem == null)
            {
                MessageBox.Show("선택된 메시가 없습니다.");
                return;
            }
            else
            {
                string meshName = SetRenderingInfo();
                wrapper.SetMesh(meshName);
                m_editForm.AddLogString(meshName + "가 설정되었습니다.");
            }
        }

        public string SetRenderingInfo()
        {
            CoreWrapper wrapper = m_editForm.GetWrapper();
            ListBox meshListBox = m_editForm.GetMeshListBox();
            ListBox fileMeshListBox = m_editForm.GetFileMeshListBox();
            TextBox meshNameTextBox = m_editForm.GetSelecteMeshNameTextBox();
            if (wrapper.FindRendererComponent() == true)
            {
                ListBox objListBox = m_editForm.GetObjectListBox();
                string strSelectedTag = objListBox.SelectedItem.ToString();
                string meshName = m_editForm.GetObjInfo()[strSelectedTag].meshInfo.m_strMeshName;

                // Mesh List
                int findIndex = meshListBox.FindString(meshName);
                if(findIndex != -1)
                {
                    meshListBox.SelectedIndex = findIndex;
                }
                else if(findIndex == -1)
                {
                    // FileMesh List
                    findIndex = fileMeshListBox.FindString(meshName);
                    if (findIndex != -1)
                    {
                        fileMeshListBox.SelectedIndex = findIndex;
                    }
                }

                // MeshName
                meshNameTextBox.Text = meshName;

                // BumpScale
                m_editForm.GetBumpScaleTextBox().Text = Convert.ToString(wrapper.GetBumpScale());

                // Selected Button
                m_editForm.GetAddRenderComponentBtn().BackColor = Color.LawnGreen;

                return meshName;
            }
            else
            {
                meshNameTextBox.Text = "None";
                meshListBox.SelectedItem = null;
                fileMeshListBox.SelectedItem = null;
                m_editForm.GetAddRenderComponentBtn().BackColor = SystemColors.ControlLight;
                return "";
            }
        }
    }
}