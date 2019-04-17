using System;
using System.Windows.Forms;
using System.Collections.Generic;
using EngineWrapper;

namespace WinFormEditor
{
    public class Layer
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

        public void ChangeLayerComboBox(string _tag, ComboBox _comboBox)
        {
            CoreWrapper wrapper = m_editForm.GetWrapper();
            m_editForm.GetObjInfo()[_tag].strLayerTag = _comboBox.SelectedItem.ToString();
            wrapper.ChangeObjectInLayer(m_editForm.GetObjInfo()[_tag].strLayerTag);
        }
    }
}