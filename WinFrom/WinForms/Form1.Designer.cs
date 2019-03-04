namespace WinForms
{
    partial class Form1
    {
        /// <summary>
        /// 필수 디자이너 변수입니다.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 사용 중인 모든 리소스를 정리합니다.
        /// </summary>
        /// <param name="disposing">관리되는 리소스를 삭제해야 하면 true이고, 그렇지 않으면 false입니다.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form 디자이너에서 생성한 코드

        /// <summary>
        /// 디자이너 지원에 필요한 메서드입니다. 
        /// 이 메서드의 내용을 코드 편집기로 수정하지 마세요.
        /// </summary>
        private void InitializeComponent()
        {
            this.button1 = new System.Windows.Forms.Button();
            this.button2 = new System.Windows.Forms.Button();
            this.button3 = new System.Windows.Forms.Button();
            this.btnTextLabel = new System.Windows.Forms.Label();
            this.ChangeTextBox = new System.Windows.Forms.TextBox();
            this.SuspendLayout();
            // 
            // button1
            // 
            this.button1.Font = new System.Drawing.Font("메이플스토리", 20.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.button1.Location = new System.Drawing.Point(12, 12);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(191, 50);
            this.button1.TabIndex = 0;
            this.button1.Text = "Button 1";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.ClickBtn1);
            // 
            // button2
            // 
            this.button2.Font = new System.Drawing.Font("메이플스토리", 20.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.button2.Location = new System.Drawing.Point(209, 12);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(191, 50);
            this.button2.TabIndex = 1;
            this.button2.Text = "Button 2";
            this.button2.UseVisualStyleBackColor = true;
            this.button2.Click += new System.EventHandler(this.ClickBtn2);
            // 
            // button3
            // 
            this.button3.Font = new System.Drawing.Font("메이플스토리", 20.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.button3.Location = new System.Drawing.Point(406, 12);
            this.button3.Name = "button3";
            this.button3.Size = new System.Drawing.Size(191, 50);
            this.button3.TabIndex = 2;
            this.button3.Text = "Button 3";
            this.button3.UseVisualStyleBackColor = true;
            this.button3.Click += new System.EventHandler(this.ClickBtn3);
            // 
            // btnTextLabel
            // 
            this.btnTextLabel.AutoSize = true;
            this.btnTextLabel.Font = new System.Drawing.Font("메이플스토리", 32.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.btnTextLabel.Location = new System.Drawing.Point(116, 76);
            this.btnTextLabel.Name = "btnTextLabel";
            this.btnTextLabel.Size = new System.Drawing.Size(395, 48);
            this.btnTextLabel.TabIndex = 3;
            this.btnTextLabel.Text = "Buttno None Click";
            this.btnTextLabel.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // ChangeTextBox
            // 
            this.ChangeTextBox.Font = new System.Drawing.Font("메이플스토리", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.ChangeTextBox.Location = new System.Drawing.Point(12, 152);
            this.ChangeTextBox.Name = "ChangeTextBox";
            this.ChangeTextBox.Size = new System.Drawing.Size(144, 25);
            this.ChangeTextBox.TabIndex = 4;
            this.ChangeTextBox.TextChanged += new System.EventHandler(this.textBox1_TextChanged);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1373, 624);
            this.Controls.Add(this.ChangeTextBox);
            this.Controls.Add(this.btnTextLabel);
            this.Controls.Add(this.button3);
            this.Controls.Add(this.button2);
            this.Controls.Add(this.button1);
            this.Name = "Form1";
            this.Text = "Form1";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.Button button3;
        private System.Windows.Forms.Label btnTextLabel;
        private System.Windows.Forms.TextBox ChangeTextBox;
    }
}

