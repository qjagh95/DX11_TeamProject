using System;
using System.Windows.Forms;
using System.Collections.Generic;
using EngineWrapper;

namespace WinFormEditor
{
    public class Transform
    {
        // Member
        public bool  m_isUpdateTr { set; get; }
        EventHandler m_event = null;

        // Instance
        private EditorForm m_editForm = null;

        public void Init(EditorForm _editForm)
        {
            m_editForm = _editForm;
            if(m_editForm == null)
            {
                m_editForm.AddLogString("EditForm is null");
            }

            m_isUpdateTr = true;
        }

        public void UpdateTransform()
        {
            LoadTransform();
        }

        public void LoadTransform()
        {
            if(m_isUpdateTr == false)
            {
                return;
            }

            ListBox listBox = m_editForm.GetObjectListBox();
            if (listBox.SelectedItem == null)
            {
                return;
            }

            string strTag = listBox.Items[listBox.SelectedIndex].ToString();
            string strLayerTag = m_editForm.GetObjInfo()[strTag].strLayerTag;

            // Local
            Dictionary<eTransformType, List<TextBox>> dicListTrL = m_editForm.GetTransformTextBox(eTransform.T_LOCAL);
            float[] arrSacle    = ObjGetLocalTransform(strTag, strLayerTag, eTransformType.TT_SCALE);
            float[] arrRotate   = ObjGetLocalTransform(strTag, strLayerTag, eTransformType.TT_ROTATE);
            float[] arrPosition = ObjGetLocalTransform(strTag, strLayerTag, eTransformType.TT_POSITION);

            // - Scale
            ObjectInfo.Vector3 vScale = new ObjectInfo.Vector3(arrSacle[0], arrSacle[1], arrSacle[2]);
            DecimalFormat(vScale);
            m_editForm.GetObjInfo()[strTag].vecLScale = vScale;
            dicListTrL[eTransformType.TT_SCALE][0].Text = Convert.ToString(vScale.x);
            dicListTrL[eTransformType.TT_SCALE][1].Text = Convert.ToString(vScale.y);
            dicListTrL[eTransformType.TT_SCALE][2].Text = Convert.ToString(vScale.z);

            // - Rotate
            ObjectInfo.Vector3 vRotate = new ObjectInfo.Vector3(arrRotate[0], arrRotate[1], arrRotate[2]);
            DecimalFormat(vRotate);
            m_editForm.GetObjInfo()[strTag].vecLRotate = vRotate;
            dicListTrL[eTransformType.TT_ROTATE][0].Text = Convert.ToString(vRotate.x);
            dicListTrL[eTransformType.TT_ROTATE][1].Text = Convert.ToString(vRotate.y);
            dicListTrL[eTransformType.TT_ROTATE][2].Text = Convert.ToString(vRotate.z);

            // - Position
            ObjectInfo.Vector3 vPos = new ObjectInfo.Vector3(arrPosition[0], arrPosition[1], arrPosition[2]);
            DecimalFormat(vPos);
            m_editForm.GetObjInfo()[strTag].vecLPosition = vPos;
            dicListTrL[eTransformType.TT_POSITION][0].Text = Convert.ToString(vPos.x);
            dicListTrL[eTransformType.TT_POSITION][1].Text = Convert.ToString(vPos.y);
            dicListTrL[eTransformType.TT_POSITION][2].Text = Convert.ToString(vPos.z);

            /************************************************************************************************************/

            // World
            Dictionary<eTransformType, List<TextBox>> dicListTrW = m_editForm.GetTransformTextBox(eTransform.T_WORLD);
            arrSacle    = ObjGetWorldTransform(strTag, strLayerTag, eTransformType.TT_SCALE);
            arrRotate   = ObjGetWorldTransform(strTag, strLayerTag, eTransformType.TT_ROTATE);
            arrPosition = ObjGetWorldTransform(strTag, strLayerTag, eTransformType.TT_POSITION);

            // - Scale
            vScale = new ObjectInfo.Vector3(arrSacle[0], arrSacle[1], arrSacle[2]);
            DecimalFormat(vScale);
            m_editForm.GetObjInfo()[strTag].vecWScale = vScale;
            dicListTrW[eTransformType.TT_SCALE][0].Text = Convert.ToString(vScale.x);
            dicListTrW[eTransformType.TT_SCALE][1].Text = Convert.ToString(vScale.y);
            dicListTrW[eTransformType.TT_SCALE][2].Text = Convert.ToString(vScale.z);

            // - Rotate
            vRotate = new ObjectInfo.Vector3(arrRotate[0], arrRotate[1], arrRotate[2]);
            DecimalFormat(vRotate);
            m_editForm.GetObjInfo()[strTag].vecWRotate = vRotate;
            dicListTrW[eTransformType.TT_ROTATE][0].Text = Convert.ToString(vRotate.x);
            dicListTrW[eTransformType.TT_ROTATE][1].Text = Convert.ToString(vRotate.y);
            dicListTrW[eTransformType.TT_ROTATE][2].Text = Convert.ToString(vRotate.z);

            // - Position
            vPos = new ObjectInfo.Vector3(arrPosition[0], arrPosition[1], arrPosition[2]);
            DecimalFormat(vPos);
            m_editForm.GetObjInfo()[strTag].vecWPosition = vPos;
            dicListTrW[eTransformType.TT_POSITION][0].Text = Convert.ToString(vPos.x);
            dicListTrW[eTransformType.TT_POSITION][1].Text = Convert.ToString(vPos.y);
            dicListTrW[eTransformType.TT_POSITION][2].Text = Convert.ToString(vPos.z);
        }

        public float[] ObjGetLocalTransform(string _strTag, string _strLayerTag, eTransformType _type)
        {
            CoreWrapper wrapper = m_editForm.GetWrapper();
            float[] arrFTr = wrapper.GetLocalTransform(_strTag, _strLayerTag, (int)_type);
            return arrFTr;
        }

        public float[] ObjGetWorldTransform(string _strTag, string _strLayerTag, eTransformType _type)
        {
            CoreWrapper wrapper = m_editForm.GetWrapper();
            float[] arrFTr = wrapper.GetWorldTransform(_strTag, _strLayerTag, (int)_type);
            return arrFTr;
        }

        public void SetEventHandler(EventHandler _event)
        {
            m_event = _event;
        }

        private void DecimalFormat(ObjectInfo.Vector3 _vec)
        {
            string cvtVecX = string.Format("{0:0.##}", _vec.x);
            string cvtVecY = string.Format("{0:0.##}", _vec.y);
            string cvtVecZ = string.Format("{0:0.##}", _vec.z);
            _vec.x = (float)Convert.ToDouble(cvtVecX);
            _vec.y = (float)Convert.ToDouble(cvtVecY);
            _vec.z = (float)Convert.ToDouble(cvtVecZ);
        }

        public void ChangeLocalScale(TextBox _sender, TextBox _localX, TextBox _localY, TextBox _localZ)
        {
            if (StringFormCheck(_sender) == true)
            {
                CoreWrapper wrapper = m_editForm.GetWrapper();
                double scaleX = Convert.ToDouble(_localX.Text);
                double scaleY = Convert.ToDouble(_localY.Text);
                double scaleZ = Convert.ToDouble(_localZ.Text);
                wrapper.SetLocalScale(scaleX, scaleY, scaleZ);
            }
        }

        public void ChangeLocalRotate(TextBox _sender, TextBox _localX, TextBox _localY, TextBox _localZ)
        {
            if (StringFormCheck(_sender) == true)
            {
                CoreWrapper wrapper = m_editForm.GetWrapper();
                double rotX = Convert.ToDouble(_localX.Text);
                double rotY = Convert.ToDouble(_localY.Text);
                double rotZ = Convert.ToDouble(_localZ.Text);
                wrapper.SetLocalRotate(rotX, rotY, rotZ);
            }
        }

        public void ChangeLocalPosition(TextBox _sender, TextBox _localX, TextBox _localY, TextBox _localZ)
        {
            if (StringFormCheck(_sender) == true)
            {
                CoreWrapper wrapper = m_editForm.GetWrapper();
                double posX = Convert.ToDouble(_localX.Text);
                double posY = Convert.ToDouble(_localY.Text);
                double posZ = Convert.ToDouble(_localZ.Text);
                wrapper.SetLocalPosition(posX, posY, posZ);
            }
        }

        public void ChangeWorldScale(TextBox _sender, TextBox _worldX, TextBox _worldY, TextBox _worldZ)
        {
            if (StringFormCheck(_sender) == true)
            {
                CoreWrapper wrapper = m_editForm.GetWrapper();
                double scaleX = Convert.ToDouble(_worldX.Text);
                double scaleY = Convert.ToDouble(_worldY.Text);
                double scaleZ = Convert.ToDouble(_worldZ.Text);
                wrapper.SetWorldScale(scaleX, scaleY, scaleZ);
            }
        }

        public void ChangeWorldRotate(TextBox _sender, TextBox _worldX, TextBox _worldY, TextBox _worldZ)
        {
            if (StringFormCheck(_sender) == true)
            {
                CoreWrapper wrapper = m_editForm.GetWrapper();
                double rotX = Convert.ToDouble(_worldX.Text);
                double rotY = Convert.ToDouble(_worldY.Text);
                double rotZ = Convert.ToDouble(_worldZ.Text);
                wrapper.SetWorldRotate(rotX, rotY, rotZ);
            }
        }

        public void ChangeWorldPosition(TextBox _sender, TextBox _worldX, TextBox _worldY, TextBox _worldZ)
        {
            if (StringFormCheck(_sender) == true)
            {
                CoreWrapper wrapper = m_editForm.GetWrapper();
                double posX = Convert.ToDouble(_worldX.Text);
                double posY = Convert.ToDouble(_worldY.Text);
                double posZ = Convert.ToDouble(_worldZ.Text);
                wrapper.SetWorldPosition(posX, posY, posZ);
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
            int decimalCnt  = 0;
            int negativeCnt = 0;
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
                    subStr = _sender.Text.Substring(i - 1, 1);
                    if (subStr.CompareTo("-") == 0)
                    {
                        SetBeforeTBText(_sender);
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
                            SetBeforeTBText(_sender);
                            return false;
                        }
                    }
                }

                // -(음수) 문자가 문자열 0번째 위치가 아닌지 검사한다. 
                if (i > 0 && subStr.CompareTo("-") == 0)
                {
                    SetBeforeTBText(_sender);
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

            // TextBox 이벤트 삭제 { ... } 등록
            if(m_event == null)
            {
                return;
            }
            _textBox.TextChanged -= m_event;
            {
                if (_textBox.Text.Length > 0)
                {
                    int strLength = _textBox.Text.Length;
                    _textBox.Text = _textBox.Text.Substring(0, strLength - 1);
                    _textBox.Select(_textBox.Text.Length, 0);
                }
            }
            _textBox.TextChanged += m_event;
        }
    }
}