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

        public void SetMesh(Button _btn, ListBox _meshList, ListBox _fileMesh)
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
                string strMeshName = "";
                if (_meshList.SelectedItem != null)
                {
                    strMeshName = _meshList.SelectedItem.ToString();
                }
                else if (_fileMesh.SelectedItem != null)
                {
                    strMeshName = _fileMesh.SelectedItem.ToString();
                }
                wrapper.SetMesh(strMeshName);
                m_editForm.AddLogString(strMeshName + "가 설정되었습니다.");
            }
        }
    }
}