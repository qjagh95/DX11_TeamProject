using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using EngineWrapper;

namespace WinFormEditor
{
    public partial class EditorForm : Form
    {
        /////////////////////////////////////////////////////////////////////////
        public CoreWrapper coreWrapper = new CoreWrapper();

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

            //hInstance, hwnd얻어옴.
            IntPtr hinstance = System.Runtime.InteropServices.Marshal.GetHINSTANCE(DeviceWindow.GetType().Module);
            IntPtr DeviceHwnd = DeviceWindow.Handle;
            coreWrapper.Init(DeviceHwnd, hinstance);

            //Run함수 연결
            Application.Idle += Run;
        }

        private void Delete(object sender, FormClosedEventArgs e)
        {
            coreWrapper.Delete();
        }

        /*
            숫자만 입력받게 하는 코드
            if (!(char.IsDigit(e.KeyChar) || e.KeyChar == Convert.ToChar(Keys.Back)))
            {
                e.Handled = true;
            }
        */

        //로직부분 건들XX 이 이후로 추가.
        /////////////////////////////////////////////////////////////////////////
        
    }
}
