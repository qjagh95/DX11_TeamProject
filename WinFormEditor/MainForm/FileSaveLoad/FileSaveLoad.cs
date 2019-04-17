using System;
using System.IO;
using System.Windows.Forms;
using EngineWrapper;

namespace WinFormEditor
{
    public class FileSaveLoad : Form
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

        public void FileAutoSave()
        {
            CoreWrapper wrapper = m_editForm.GetWrapper();

            // 파일명
            SaveFileDialog saveFile = new SaveFileDialog();
            string curDirectory = Directory.GetCurrentDirectory();
            string path = Directory.GetParent(curDirectory).Parent.Parent.FullName;
            path += "\\3DClient\\bin\\Data";
            string strFullName = path + "\\" + "TempSave.dat";
            wrapper.FileSave(strFullName);
            m_editForm.AddLogString("자동 저장을 완료했습니다.");
        }

        public void FileSave_Click()
        {
            // Title  : Dialog 창 이름 설정
            // Filter : 파일 형식 부분
            // AddExtension : 확장명 추가 여부
            SaveFileDialog saveFile     = new SaveFileDialog();
            string curDirectory         = Directory.GetCurrentDirectory();
            string path                 = Directory.GetParent(curDirectory).Parent.Parent.FullName;
            path                        += "\\3DClient\\bin\\Data";
            saveFile.InitialDirectory   = path;
            saveFile.Title              = "다른 이름으로 저장";
            saveFile.Filter             = "데이터 파일(*.dat)|*.dat|모든파일(*.*)|*.*";
            saveFile.DefaultExt         = "dat";
            saveFile.AddExtension       = true;
            if (saveFile.ShowDialog() == DialogResult.OK)
            {
                // 저장 완료
                CoreWrapper wrapper = m_editForm.GetWrapper();
                wrapper.FileSave(saveFile.FileName);
            }
        }

        public void FileLoad_Click()
        {
            OpenFileDialog openFile     = new OpenFileDialog();
            string curDirectory         = Directory.GetCurrentDirectory();
            string path                 = Directory.GetParent(curDirectory).Parent.Parent.FullName;
            path                        += "\\3DClient\\bin\\Data";
            openFile.InitialDirectory   = path;
            openFile.Title              = "파일 불러오기";
            openFile.FileName           = ".dat";
            openFile.Filter             = "데이터 파일(*.dat) | *.dat; | 모든 파일(*.*) | *.*";
            if (openFile.ShowDialog() == DialogResult.OK)
            {
                // 불러오기 성공
                CoreWrapper wrapper = m_editForm.GetWrapper();
                wrapper.FileLoad(openFile.FileName);

                // 데이터 로드
                m_editForm.LoadData(true);
            }
        }

        public void LoadMshFile(ListBox _listBox)
        {
            OpenFileDialog openFile = new OpenFileDialog();
            string curDirectory = Directory.GetCurrentDirectory();
            string path = Directory.GetParent(curDirectory).Parent.Parent.FullName;
            path += "\\3DClient\\Bin\\MeshData\\";
            openFile.InitialDirectory = path;
            openFile.Title = "파일 불러오기";
            openFile.FileName = ".msh";
            openFile.Filter = "MeshFile(*.msh)|*.msh|모든파일(*.*)|*.*||";
            openFile.RestoreDirectory = true;
            if (openFile.ShowDialog() == DialogResult.OK)
            {
                string strFilePath = openFile.FileName;
                string strFileName = Path.GetFileNameWithoutExtension(strFilePath);

                CoreWrapper wrapper = m_editForm.GetWrapper();
                wrapper.LoadMeshFromFullPath(strFileName, strFilePath);
                _listBox.Items.Add(strFileName);
            }
        }
    }
}