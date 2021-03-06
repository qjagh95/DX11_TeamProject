﻿using System;
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
            ((Button)listTools[0]).Enabled      = _isEnable; // Btn_LightColor
            ((ComboBox)listTools[1]).Enabled    = _isEnable; // CB_LightType
            ((CheckBox)listTools[2]).Enabled    = _isEnable; // CB_WireFrame
            ((TextBox)listTools[3]).Enabled     = _isEnable; // TB_SpcularR
            ((TextBox)listTools[4]).Enabled     = _isEnable; // TB_SpcularG
            ((TextBox)listTools[5]).Enabled     = _isEnable; // TB_SpcularB
            ((TextBox)listTools[6]).Enabled     = _isEnable; // TB_SpcularPower
            ((TrackBar)listTools[7]).Enabled    = _isEnable; // TB_Range
            ((TextBox)listTools[8]).Enabled     = _isEnable; // TB_RangeValue
            ((TrackBar)listTools[9]).Enabled    = _isEnable; // TrackBar_In
            ((TextBox)listTools[10]).Enabled    = _isEnable; // TB_InAngle
            ((TrackBar)listTools[11]).Enabled   = _isEnable; // TrackBar_Out
            ((TextBox)listTools[12]).Enabled    = _isEnable; // TB_OutAngle

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

                // SpotLight In, Out Angle
                ((TrackBar)listTools[9]).Value = (int)wrapper.GetInAngle();
                ((TextBox)listTools[10]).Text = ((TrackBar)listTools[9]).Value.ToString();
                ((TrackBar)listTools[11]).Value = (int)wrapper.GetOutAngle();
                ((TextBox)listTools[12]).Text = ((TrackBar)listTools[11]).Value.ToString();
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

        public void Add_6_Way_GlobalLight()
        {
            CoreWrapper wrapper = m_editForm.GetWrapper();
            string[] arrLightTag = wrapper.GetGlobalLightList();

            // Default_GlobalLight_ 6개의 오브젝트를 삭제한다.
            ListBox objListBox = m_editForm.GetObjectListBox();
            for (int i = 0; i < arrLightTag.Length; ++i)
            {
                string strTag = arrLightTag[i];
                int findIndex = objListBox.FindString(strTag);
                if (findIndex != ListBox.NoMatches)
                {
                    objListBox.Items.RemoveAt(findIndex);
                    m_editForm.GetObjInfo().Remove(strTag);
                }
            }

            // LightLayer에 Default_GlobalLight_ 오브젝트가 있는지 검사 후 없다면 생성한다.
            wrapper.CreateDefaultGlobalLight();

            // 추가
            Transform trInst = m_editForm.GetTransformInst();
            float[] arrFLScale, arrFLRot, arrFLPos, arrFWScale, arrFWRot, arrFWPos, arrFPivot;
            for (int i = 0; i < arrLightTag.Length; ++i)
            {
                string strTag = arrLightTag[i];

                // Local Transform
                arrFLScale = trInst.ObjGetLocalTransform(strTag, "Light", eTransformType.TT_SCALE);
                arrFLRot   = trInst.ObjGetLocalTransform(strTag, "Light", eTransformType.TT_ROTATE);
                arrFLPos   = trInst.ObjGetLocalTransform(strTag, "Light", eTransformType.TT_POSITION);

                // World Transform
                arrFWScale = trInst.ObjGetWorldTransform(strTag, "Light", eTransformType.TT_SCALE);
                arrFWRot   = trInst.ObjGetWorldTransform(strTag, "Light", eTransformType.TT_ROTATE);
                arrFWPos   = trInst.ObjGetWorldTransform(strTag, "Light", eTransformType.TT_POSITION);

                // World Pivot
                arrFPivot = trInst.ObjGetWorldPivot(strTag, "Light");

                ObjectInfo info = new ObjectInfo
                {
                    strLayerTag     = "Light",

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
                m_editForm.GetObjInfo().Add(strTag, info);

                // 리스트 박스 아이템 추가
                objListBox.Items.Add(strTag);
            }
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