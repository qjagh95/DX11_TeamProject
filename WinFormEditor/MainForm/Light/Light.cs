using System;
using System.Drawing;
using System.Windows.Forms;
using System.Collections.Generic;
using EngineWrapper;

namespace WinFormEditor
{
    public class Light
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

            // 조명 도구 초기화
            SetLightToolsEnable(false);
        }

        public void SetLightToolsEnable(bool _isEnable)
        {
            List<object> listTools = m_editForm.GetLightTools();
            ((Button)listTools[0]).Enabled      = _isEnable;
            ((ComboBox)listTools[1]).Enabled    = _isEnable;
            ((CheckBox)listTools[2]).Enabled    = _isEnable;
            ((TextBox)listTools[3]).Enabled     = _isEnable;
            ((TextBox)listTools[4]).Enabled     = _isEnable;
            ((TextBox)listTools[5]).Enabled     = _isEnable;
            ((TextBox)listTools[6]).Enabled     = _isEnable;
            ((TrackBar)listTools[7]).Enabled    = _isEnable;
            ((TextBox)listTools[8]).Enabled     = _isEnable;

            // 속성 값 적용
            if (_isEnable == true)
            {
                CoreWrapper wrapper = m_editForm.GetWrapper();
                
                // Type
                eLightType lightType = (eLightType)wrapper.GetLightType();
                switch (lightType)
                {
                    case eLightType.LT_DIR:
                    {
                        break;
                    }
                    case eLightType.LT_POINT:
                    {
                        break;
                    }
                    case eLightType.LT_SPOT:
                    {
                        break;
                    }
                }

                // Specular
                float[] arrFSpecular = wrapper.GetSpecular();
                ((TextBox)listTools[3]).Text = Convert.ToString(arrFSpecular[0]);
                ((TextBox)listTools[4]).Text = Convert.ToString(arrFSpecular[1]);
                ((TextBox)listTools[5]).Text = Convert.ToString(arrFSpecular[2]);
                ((TextBox)listTools[6]).Text = Convert.ToString(arrFSpecular[3]);

                // Range
                ((TextBox)listTools[8]).Text = ((TrackBar)listTools[7]).Value.ToString();
            }
        }

        public void AddComponent(Button _btn)
        {
            if (m_editForm.GetObjectListBox().SelectedItem == null)
            {
                MessageBox.Show("선택된 오브젝트가 없습니다.");
                return;
            }
            _btn.BackColor = Color.LawnGreen;

            // 컴포넌트 등록
            CoreWrapper wrapper = m_editForm.GetWrapper();
            wrapper.AddLightComponent();
            SetLightToolsEnable(true);
            m_editForm.AddLogString("Light 컴포넌트가 등록되었습니다.");
        }

        public void ChangeLightRange(int _range, TextBox _textBox)
        {
            CoreWrapper wrapper = m_editForm.GetWrapper();
            wrapper.SetLightRange(_range);
            _textBox.Text = Convert.ToString(_range);
        }

        public void LightWireFrame_OnOff(bool _isFlag)
        {
            CoreWrapper wrapper = m_editForm.GetWrapper();
            wrapper.SetLightWireFrame(_isFlag);
        }
        
        public void ChangeSpecularValue(TextBox _sender)
        {
            if(_sender.Text == "")
            {
                return;
            }

            if (StringFormCheck(_sender) == true)
            {
                CoreWrapper wrapper = m_editForm.GetWrapper();
                double value = Convert.ToDouble(_sender.Text);
                if (value < 0.0 || value > 1.0)
                {
                    if (value < 0.0) { value = 0.0; }
                    if (value > 1.0) { value = 1.0; }
                    _sender.Text = Convert.ToString(value);
                }
                string strSpecularName = _sender.Name;
                if(strSpecularName == "TB_SpcularR")
                {
                    wrapper.SetLightSpclularR(value);
                }
                else if (strSpecularName == "TB_SpcularG")
                {
                    wrapper.SetLightSpclularG(value);
                }
                else if (strSpecularName == "TB_SpcularB")
                {
                    wrapper.SetLightSpclularB(value);
                }
                else if (strSpecularName == "TB_SpcularPower")
                {
                    wrapper.SetLightSpclularW(value);
                }
            }
        }

        private bool StringFormCheck(TextBox _sender)
        {
            // 선택된 오브젝트가 있는지 검사한다.
            if (m_editForm.GetObjectListBox().SelectedItem == null)
            {
                SetBeforeTBText(_sender);
                return false;
            }

            // 데이터 형식 검사
            int decimalCnt = 0;
            for (int i = 0; i < _sender.Text.Length; ++i)
            {
                string subStr = _sender.Text.Substring(i, 1);

                // 문자열 0번째 위치에 .(소수점) 표시가 들어오는지 검사한다.
                if (i == 0 && subStr.CompareTo(".") == 0)
                {
                    SetBeforeTBText(_sender);
                    return false;
                }
                else if (i > 0 && subStr.CompareTo(".") == 0)
                {
                    if (decimalCnt == 0)
                    {
                        ++decimalCnt;
                        continue;
                    }
                    else
                    {
                        SetBeforeTBText(_sender);
                        return false;
                    }
                }
            }
            return true;
        }

        private void SetBeforeTBText(TextBox _textBox)
        {
            // 선택된 오브젝트가 있는지 검사한다.
            if (m_editForm.GetObjectListBox().SelectedItem == null)
            {
                MessageBox.Show("선택된 오브젝트가 없습니다.");
            }
            else
            {
                MessageBox.Show("잘못된 형식의 값이 들어왔습니다.");
            }
        }
    }
}