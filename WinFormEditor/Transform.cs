using System;
using System.Windows.Forms;
using System.Collections.Generic;
using EngineWrapper;

namespace WinFormEditor
{
    public class Transform
    {
        enum eTransformType { TT_SCALE, TT_ROTATE, TT_POSITION, TT_MAX, };

        private EditorForm                      m_editForm = null;
        private CoreWrapper                     m_coreWrapper = null;
        private Dictionary<string, ObjectInfo>  m_objInfo = null;

        public void Init(EditorForm _editForm, CoreWrapper _wrapper)
        {
            m_editForm = _editForm;
            if(m_editForm == null)
            {
                m_editForm.AddLogString("EditForm is null");
            }

            m_coreWrapper = _wrapper;
            if(m_coreWrapper == null)
            {
                m_editForm.AddLogString("CoreWrapper is null");
            }
        }

        private float[] SelectObjGetWorldTransform(string _strTag, string _strLayerTag, eTransformType _type)
        {
            float[] arrFTr = m_coreWrapper.GetWorldTransform(_strTag, _strLayerTag, (int)_type);
            return arrFTr;
        }

        public void UpdateTransform()
        {
            m_objInfo = m_editForm.GetObjectInfo();
            string strTag = m_editForm.GetSelectObjectTag();
            if (strTag == "")
            {
                return;
            }
            string strLayerTag = m_objInfo[strTag].strLayerTag;

            // Select Object Get World Transform
            float[] arrFScale = SelectObjGetWorldTransform(strTag, strLayerTag, eTransformType.TT_SCALE);
            float[] arrFRotate = SelectObjGetWorldTransform(strTag, strLayerTag, eTransformType.TT_ROTATE);
            float[] arrFPosition = SelectObjGetWorldTransform(strTag, strLayerTag, eTransformType.TT_POSITION);
            
            // Change TextBox 
            // Position
            TextBox[,] arrTextBox = m_editForm.GetTransformTextBox();
            arrTextBox[0, 0].Text = Convert.ToString(arrFPosition[0]);
            arrTextBox[0, 1].Text = Convert.ToString(arrFPosition[1]);
            arrTextBox[0, 2].Text = Convert.ToString(arrFPosition[2]);
            // Scale
            arrTextBox[1, 0].Text = Convert.ToString(arrFScale[0]);
            arrTextBox[1, 1].Text = Convert.ToString(arrFScale[1]);
            arrTextBox[1, 2].Text = Convert.ToString(arrFScale[2]);
            // Rotate
            arrTextBox[2, 0].Text = Convert.ToString(arrFRotate[0]);
            arrTextBox[2, 1].Text = Convert.ToString(arrFRotate[1]);
            arrTextBox[2, 2].Text = Convert.ToString(arrFRotate[2]);
        }
    }
}
