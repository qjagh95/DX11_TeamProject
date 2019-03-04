using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Diagnostics;
using System.Windows.Forms;

using EditWrapper;

namespace WinForms
{
    public partial class Form1 : Form
    {
        WrapperManager instWrapper = new WrapperManager();

        public Form1()
        {
            InitializeComponent();
        }

        private void ClickBtn1(object sender, EventArgs e)
        {
            Debug.WriteLine("Click Button 1");
            this.btnTextLabel.Text = "Button 1";

            // String Test
            string strText = this.btnTextLabel.Text;
            instWrapper.TestFunction(strText);
        }   

        private void ClickBtn2(object sender, EventArgs e)
        {
            Debug.WriteLine("Click Button 2");
            this.btnTextLabel.Text = "Button 2";
        }

        private void ClickBtn3(object sender, EventArgs e)
        {
            Debug.WriteLine("Click Button 3");
            this.btnTextLabel.Text = "Button 3";
        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {
            string ss = sender.ToString();
            instWrapper.TestFunction(ss);
        }
    }
}
