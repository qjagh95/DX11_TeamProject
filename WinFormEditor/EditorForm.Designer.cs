namespace WinFormEditor
{
    partial class EditorForm
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
            this.DeviceWindow = new System.Windows.Forms.PictureBox();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.button1 = new System.Windows.Forms.Button();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.groupBox4 = new System.Windows.Forms.GroupBox();
            this.treeView1 = new System.Windows.Forms.TreeView();
            this.LayerList = new System.Windows.Forms.ListBox();
            this.groupBox5 = new System.Windows.Forms.GroupBox();
            this.groupBox6 = new System.Windows.Forms.GroupBox();
            this.button4 = new System.Windows.Forms.Button();
            this.setMesh = new System.Windows.Forms.Button();
            this.MeshList = new System.Windows.Forms.ListBox();
            this.groupBox7 = new System.Windows.Forms.GroupBox();
            this.button3 = new System.Windows.Forms.Button();
            this.SetScale = new System.Windows.Forms.Button();
            this.button2 = new System.Windows.Forms.Button();
            this.groupBox10 = new System.Windows.Forms.GroupBox();
            this.RotationZ = new System.Windows.Forms.TextBox();
            this.RotationY = new System.Windows.Forms.TextBox();
            this.RotationX = new System.Windows.Forms.TextBox();
            this.label7 = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.label9 = new System.Windows.Forms.Label();
            this.groupBox8 = new System.Windows.Forms.GroupBox();
            this.PositionZ = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.PositionY = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.PositionX = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.groupBox9 = new System.Windows.Forms.GroupBox();
            this.ScaleZ = new System.Windows.Forms.TextBox();
            this.ScaleY = new System.Windows.Forms.TextBox();
            this.ScaleX = new System.Windows.Forms.TextBox();
            this.label6 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.ObjectList = new System.Windows.Forms.GroupBox();
            this.ObjList = new System.Windows.Forms.ListBox();
            ((System.ComponentModel.ISupportInitialize)(this.DeviceWindow)).BeginInit();
            this.groupBox2.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.groupBox5.SuspendLayout();
            this.groupBox6.SuspendLayout();
            this.groupBox7.SuspendLayout();
            this.groupBox10.SuspendLayout();
            this.groupBox8.SuspendLayout();
            this.groupBox9.SuspendLayout();
            this.ObjectList.SuspendLayout();
            this.SuspendLayout();
            // 
            // DeviceWindow
            // 
            this.DeviceWindow.Location = new System.Drawing.Point(6, 29);
            this.DeviceWindow.Name = "DeviceWindow";
            this.DeviceWindow.Size = new System.Drawing.Size(835, 511);
            this.DeviceWindow.TabIndex = 0;
            this.DeviceWindow.TabStop = false;
            // 
            // groupBox1
            // 
            this.groupBox1.Font = new System.Drawing.Font("맑은 고딕", 15F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.groupBox1.ForeColor = System.Drawing.SystemColors.ControlText;
            this.groupBox1.Location = new System.Drawing.Point(865, -4);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(298, 292);
            this.groupBox1.TabIndex = 1;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Animation";
            this.groupBox1.Enter += new System.EventHandler(this.groupBox1_Enter);
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.textBox1);
            this.groupBox2.Controls.Add(this.button1);
            this.groupBox2.Font = new System.Drawing.Font("맑은 고딕", 15F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.groupBox2.Location = new System.Drawing.Point(865, 480);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(298, 68);
            this.groupBox2.TabIndex = 2;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "CreateObj";
            // 
            // textBox1
            // 
            this.textBox1.BackColor = System.Drawing.SystemColors.Control;
            this.textBox1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.textBox1.Font = new System.Drawing.Font("맑은 고딕", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.textBox1.ForeColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.textBox1.Location = new System.Drawing.Point(14, 33);
            this.textBox1.Name = "textBox1";
            this.textBox1.Size = new System.Drawing.Size(113, 23);
            this.textBox1.TabIndex = 1;
            this.textBox1.TextChanged += new System.EventHandler(this.textBox1_TextChanged);
            // 
            // button1
            // 
            this.button1.Font = new System.Drawing.Font("맑은 고딕", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.button1.Location = new System.Drawing.Point(133, 31);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(110, 25);
            this.button1.TabIndex = 5;
            this.button1.Text = "Create Object";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.DeviceWindow);
            this.groupBox3.Font = new System.Drawing.Font("맑은 고딕", 15F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.groupBox3.Location = new System.Drawing.Point(12, -4);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(847, 546);
            this.groupBox3.TabIndex = 3;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Edit Scene";
            // 
            // groupBox4
            // 
            this.groupBox4.Font = new System.Drawing.Font("맑은 고딕", 15F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.groupBox4.Location = new System.Drawing.Point(12, 542);
            this.groupBox4.Name = "groupBox4";
            this.groupBox4.Size = new System.Drawing.Size(847, 187);
            this.groupBox4.TabIndex = 4;
            this.groupBox4.TabStop = false;
            this.groupBox4.Text = "Log";
            // 
            // treeView1
            // 
            this.treeView1.LineColor = System.Drawing.Color.Empty;
            this.treeView1.Location = new System.Drawing.Point(0, 0);
            this.treeView1.Name = "treeView1";
            this.treeView1.Size = new System.Drawing.Size(121, 97);
            this.treeView1.TabIndex = 0;
            // 
            // LayerList
            // 
            this.LayerList.BackColor = System.Drawing.SystemColors.Control;
            this.LayerList.Font = new System.Drawing.Font("맑은 고딕", 9F, System.Drawing.FontStyle.Bold);
            this.LayerList.ForeColor = System.Drawing.SystemColors.ControlText;
            this.LayerList.FormattingEnabled = true;
            this.LayerList.ItemHeight = 15;
            this.LayerList.Location = new System.Drawing.Point(11, 34);
            this.LayerList.Name = "LayerList";
            this.LayerList.Size = new System.Drawing.Size(121, 139);
            this.LayerList.TabIndex = 6;
            this.LayerList.SelectedIndexChanged += new System.EventHandler(this.LayerList_SelectedIndexChanged);
            // 
            // groupBox5
            // 
            this.groupBox5.Controls.Add(this.LayerList);
            this.groupBox5.Font = new System.Drawing.Font("맑은 고딕", 15F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.groupBox5.Location = new System.Drawing.Point(865, 294);
            this.groupBox5.Name = "groupBox5";
            this.groupBox5.Size = new System.Drawing.Size(144, 180);
            this.groupBox5.TabIndex = 7;
            this.groupBox5.TabStop = false;
            this.groupBox5.Text = "Layer List";
            // 
            // groupBox6
            // 
            this.groupBox6.Controls.Add(this.button4);
            this.groupBox6.Controls.Add(this.setMesh);
            this.groupBox6.Controls.Add(this.MeshList);
            this.groupBox6.Controls.Add(this.groupBox7);
            this.groupBox6.Font = new System.Drawing.Font("맑은 고딕", 15F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.groupBox6.Location = new System.Drawing.Point(1169, -4);
            this.groupBox6.Name = "groupBox6";
            this.groupBox6.Size = new System.Drawing.Size(327, 552);
            this.groupBox6.TabIndex = 8;
            this.groupBox6.TabStop = false;
            this.groupBox6.Text = "Component";
            // 
            // button4
            // 
            this.button4.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.button4.Location = new System.Drawing.Point(154, 452);
            this.button4.Name = "button4";
            this.button4.Size = new System.Drawing.Size(163, 46);
            this.button4.TabIndex = 3;
            this.button4.Text = "Add Render Component";
            this.button4.UseVisualStyleBackColor = true;
            this.button4.Click += new System.EventHandler(this.AddRenderComponent);
            // 
            // setMesh
            // 
            this.setMesh.Location = new System.Drawing.Point(154, 504);
            this.setMesh.Name = "setMesh";
            this.setMesh.Size = new System.Drawing.Size(163, 36);
            this.setMesh.TabIndex = 2;
            this.setMesh.Text = "Set Mesh";
            this.setMesh.UseVisualStyleBackColor = true;
            this.setMesh.Click += new System.EventHandler(this.SetMesh_Click);
            // 
            // MeshList
            // 
            this.MeshList.BackColor = System.Drawing.SystemColors.Control;
            this.MeshList.FormattingEnabled = true;
            this.MeshList.ItemHeight = 28;
            this.MeshList.Location = new System.Drawing.Point(8, 256);
            this.MeshList.Name = "MeshList";
            this.MeshList.Size = new System.Drawing.Size(140, 284);
            this.MeshList.TabIndex = 1;
            this.MeshList.SelectedIndexChanged += new System.EventHandler(this.MeshList_SelectedIndexChanged);
            // 
            // groupBox7
            // 
            this.groupBox7.Controls.Add(this.button3);
            this.groupBox7.Controls.Add(this.SetScale);
            this.groupBox7.Controls.Add(this.button2);
            this.groupBox7.Controls.Add(this.groupBox10);
            this.groupBox7.Controls.Add(this.groupBox8);
            this.groupBox7.Controls.Add(this.groupBox9);
            this.groupBox7.Font = new System.Drawing.Font("맑은 고딕", 10F, System.Drawing.FontStyle.Bold);
            this.groupBox7.Location = new System.Drawing.Point(6, 33);
            this.groupBox7.Name = "groupBox7";
            this.groupBox7.Size = new System.Drawing.Size(315, 215);
            this.groupBox7.TabIndex = 0;
            this.groupBox7.TabStop = false;
            this.groupBox7.Text = "Transform";
            // 
            // button3
            // 
            this.button3.Location = new System.Drawing.Point(214, 184);
            this.button3.Name = "button3";
            this.button3.Size = new System.Drawing.Size(99, 31);
            this.button3.TabIndex = 11;
            this.button3.Text = "SetRotation";
            this.button3.UseVisualStyleBackColor = true;
            this.button3.Click += new System.EventHandler(this.SetRotation_Click);
            // 
            // SetScale
            // 
            this.SetScale.ImageAlign = System.Drawing.ContentAlignment.BottomCenter;
            this.SetScale.Location = new System.Drawing.Point(109, 184);
            this.SetScale.Name = "SetScale";
            this.SetScale.Size = new System.Drawing.Size(101, 30);
            this.SetScale.TabIndex = 1;
            this.SetScale.Text = "SetScale";
            this.SetScale.UseVisualStyleBackColor = true;
            this.SetScale.Click += new System.EventHandler(this.SetScale_Click);
            // 
            // button2
            // 
            this.button2.Location = new System.Drawing.Point(0, 184);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(105, 30);
            this.button2.TabIndex = 10;
            this.button2.Text = "SetPosition";
            this.button2.UseVisualStyleBackColor = true;
            this.button2.Click += new System.EventHandler(this.button2_Click);
            // 
            // groupBox10
            // 
            this.groupBox10.Controls.Add(this.RotationZ);
            this.groupBox10.Controls.Add(this.RotationY);
            this.groupBox10.Controls.Add(this.RotationX);
            this.groupBox10.Controls.Add(this.label7);
            this.groupBox10.Controls.Add(this.label8);
            this.groupBox10.Controls.Add(this.label9);
            this.groupBox10.Location = new System.Drawing.Point(0, 132);
            this.groupBox10.Name = "groupBox10";
            this.groupBox10.Size = new System.Drawing.Size(309, 47);
            this.groupBox10.TabIndex = 1;
            this.groupBox10.TabStop = false;
            this.groupBox10.Text = "Rotate";
            // 
            // RotationZ
            // 
            this.RotationZ.BackColor = System.Drawing.SystemColors.Control;
            this.RotationZ.Location = new System.Drawing.Point(228, 15);
            this.RotationZ.Name = "RotationZ";
            this.RotationZ.Size = new System.Drawing.Size(80, 25);
            this.RotationZ.TabIndex = 20;
            this.RotationZ.TextChanged += new System.EventHandler(this.RotationZ_TextChanged);
            // 
            // RotationY
            // 
            this.RotationY.BackColor = System.Drawing.SystemColors.Control;
            this.RotationY.Location = new System.Drawing.Point(123, 15);
            this.RotationY.Name = "RotationY";
            this.RotationY.Size = new System.Drawing.Size(84, 25);
            this.RotationY.TabIndex = 19;
            this.RotationY.TextChanged += new System.EventHandler(this.RotationY_TextChanged);
            // 
            // RotationX
            // 
            this.RotationX.BackColor = System.Drawing.SystemColors.Control;
            this.RotationX.Location = new System.Drawing.Point(20, 17);
            this.RotationX.Name = "RotationX";
            this.RotationX.Size = new System.Drawing.Size(80, 25);
            this.RotationX.TabIndex = 18;
            this.RotationX.TextChanged += new System.EventHandler(this.RotationX_TextChanged);
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Font = new System.Drawing.Font("맑은 고딕", 8F, System.Drawing.FontStyle.Bold);
            this.label7.Location = new System.Drawing.Point(213, 21);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(14, 13);
            this.label7.TabIndex = 15;
            this.label7.Text = "Z";
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Font = new System.Drawing.Font("맑은 고딕", 8F, System.Drawing.FontStyle.Bold);
            this.label8.Location = new System.Drawing.Point(107, 21);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(14, 13);
            this.label8.TabIndex = 16;
            this.label8.Text = "Y";
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Font = new System.Drawing.Font("맑은 고딕", 8F, System.Drawing.FontStyle.Bold);
            this.label9.Location = new System.Drawing.Point(6, 23);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(14, 13);
            this.label9.TabIndex = 17;
            this.label9.Text = "X";
            // 
            // groupBox8
            // 
            this.groupBox8.Controls.Add(this.PositionZ);
            this.groupBox8.Controls.Add(this.label3);
            this.groupBox8.Controls.Add(this.PositionY);
            this.groupBox8.Controls.Add(this.label2);
            this.groupBox8.Controls.Add(this.PositionX);
            this.groupBox8.Controls.Add(this.label1);
            this.groupBox8.Location = new System.Drawing.Point(0, 24);
            this.groupBox8.Name = "groupBox8";
            this.groupBox8.Size = new System.Drawing.Size(309, 48);
            this.groupBox8.TabIndex = 0;
            this.groupBox8.TabStop = false;
            this.groupBox8.Text = "Position";
            // 
            // PositionZ
            // 
            this.PositionZ.BackColor = System.Drawing.SystemColors.Control;
            this.PositionZ.Location = new System.Drawing.Point(229, 17);
            this.PositionZ.Name = "PositionZ";
            this.PositionZ.Size = new System.Drawing.Size(80, 25);
            this.PositionZ.TabIndex = 13;
            this.PositionZ.TextChanged += new System.EventHandler(this.PositionZ_TextChanged);
            this.PositionZ.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.PositionX_Press);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Font = new System.Drawing.Font("맑은 고딕", 8F, System.Drawing.FontStyle.Bold);
            this.label3.Location = new System.Drawing.Point(213, 22);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(14, 13);
            this.label3.TabIndex = 12;
            this.label3.Text = "Z";
            // 
            // PositionY
            // 
            this.PositionY.BackColor = System.Drawing.SystemColors.Control;
            this.PositionY.Location = new System.Drawing.Point(123, 17);
            this.PositionY.Name = "PositionY";
            this.PositionY.Size = new System.Drawing.Size(87, 25);
            this.PositionY.TabIndex = 11;
            this.PositionY.TextChanged += new System.EventHandler(this.PositionY_TextChanged);
            this.PositionY.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.PositionX_Press);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("맑은 고딕", 8F, System.Drawing.FontStyle.Bold);
            this.label2.Location = new System.Drawing.Point(107, 23);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(14, 13);
            this.label2.TabIndex = 10;
            this.label2.Text = "Y";
            // 
            // PositionX
            // 
            this.PositionX.BackColor = System.Drawing.SystemColors.Control;
            this.PositionX.Location = new System.Drawing.Point(19, 17);
            this.PositionX.Name = "PositionX";
            this.PositionX.Size = new System.Drawing.Size(86, 25);
            this.PositionX.TabIndex = 2;
            this.PositionX.TextChanged += new System.EventHandler(this.PositionX_TextChanged);
            this.PositionX.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.PositionX_Press);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("맑은 고딕", 8F, System.Drawing.FontStyle.Bold);
            this.label1.Location = new System.Drawing.Point(3, 24);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(14, 13);
            this.label1.TabIndex = 1;
            this.label1.Text = "X";
            // 
            // groupBox9
            // 
            this.groupBox9.Controls.Add(this.ScaleZ);
            this.groupBox9.Controls.Add(this.ScaleY);
            this.groupBox9.Controls.Add(this.ScaleX);
            this.groupBox9.Controls.Add(this.label6);
            this.groupBox9.Controls.Add(this.label5);
            this.groupBox9.Controls.Add(this.label4);
            this.groupBox9.Location = new System.Drawing.Point(0, 78);
            this.groupBox9.Name = "groupBox9";
            this.groupBox9.Size = new System.Drawing.Size(309, 48);
            this.groupBox9.TabIndex = 1;
            this.groupBox9.TabStop = false;
            this.groupBox9.Text = "Scale";
            // 
            // ScaleZ
            // 
            this.ScaleZ.BackColor = System.Drawing.SystemColors.Control;
            this.ScaleZ.ForeColor = System.Drawing.SystemColors.ControlText;
            this.ScaleZ.Location = new System.Drawing.Point(228, 16);
            this.ScaleZ.Name = "ScaleZ";
            this.ScaleZ.Size = new System.Drawing.Size(80, 25);
            this.ScaleZ.TabIndex = 17;
            this.ScaleZ.TextChanged += new System.EventHandler(this.ScaleZ_TextChanged);
            // 
            // ScaleY
            // 
            this.ScaleY.BackColor = System.Drawing.SystemColors.Control;
            this.ScaleY.ForeColor = System.Drawing.SystemColors.ControlText;
            this.ScaleY.Location = new System.Drawing.Point(123, 17);
            this.ScaleY.Name = "ScaleY";
            this.ScaleY.Size = new System.Drawing.Size(87, 25);
            this.ScaleY.TabIndex = 16;
            this.ScaleY.TextChanged += new System.EventHandler(this.ScaleY_TextChanged);
            // 
            // ScaleX
            // 
            this.ScaleX.BackColor = System.Drawing.SystemColors.Control;
            this.ScaleX.ForeColor = System.Drawing.SystemColors.ControlText;
            this.ScaleX.Location = new System.Drawing.Point(20, 17);
            this.ScaleX.Name = "ScaleX";
            this.ScaleX.Size = new System.Drawing.Size(80, 25);
            this.ScaleX.TabIndex = 15;
            this.ScaleX.TextChanged += new System.EventHandler(this.ScaleX_TextChanged);
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Font = new System.Drawing.Font("맑은 고딕", 8F, System.Drawing.FontStyle.Bold);
            this.label6.Location = new System.Drawing.Point(212, 23);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(14, 13);
            this.label6.TabIndex = 14;
            this.label6.Text = "Z";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Font = new System.Drawing.Font("맑은 고딕", 8F, System.Drawing.FontStyle.Bold);
            this.label5.Location = new System.Drawing.Point(106, 23);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(14, 13);
            this.label5.TabIndex = 14;
            this.label5.Text = "Y";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Font = new System.Drawing.Font("맑은 고딕", 8F, System.Drawing.FontStyle.Bold);
            this.label4.Location = new System.Drawing.Point(6, 23);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(14, 13);
            this.label4.TabIndex = 14;
            this.label4.Text = "X";
            // 
            // ObjectList
            // 
            this.ObjectList.Controls.Add(this.ObjList);
            this.ObjectList.Font = new System.Drawing.Font("맑은 고딕", 15F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.ObjectList.Location = new System.Drawing.Point(1015, 294);
            this.ObjectList.Name = "ObjectList";
            this.ObjectList.Size = new System.Drawing.Size(148, 180);
            this.ObjectList.TabIndex = 9;
            this.ObjectList.TabStop = false;
            this.ObjectList.Text = "ObjectList";
            // 
            // ObjList
            // 
            this.ObjList.BackColor = System.Drawing.SystemColors.Control;
            this.ObjList.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.ObjList.ForeColor = System.Drawing.SystemColors.ControlText;
            this.ObjList.FormattingEnabled = true;
            this.ObjList.ItemHeight = 17;
            this.ObjList.Location = new System.Drawing.Point(3, 30);
            this.ObjList.Name = "ObjList";
            this.ObjList.Size = new System.Drawing.Size(139, 140);
            this.ObjList.TabIndex = 0;
            this.ObjList.SelectedIndexChanged += new System.EventHandler(this.ObjList_SelectedIndexChanged);
            // 
            // EditorForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.SystemColors.Control;
            this.ClientSize = new System.Drawing.Size(1508, 738);
            this.Controls.Add(this.ObjectList);
            this.Controls.Add(this.groupBox6);
            this.Controls.Add(this.groupBox5);
            this.Controls.Add(this.groupBox4);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.ForeColor = System.Drawing.SystemColors.WindowText;
            this.Name = "EditorForm";
            this.Text = "Form1";
            this.TransparencyKey = System.Drawing.Color.White;
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.Delete);
            this.Load += new System.EventHandler(this.EditorForm_Load);
            ((System.ComponentModel.ISupportInitialize)(this.DeviceWindow)).EndInit();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.groupBox3.ResumeLayout(false);
            this.groupBox5.ResumeLayout(false);
            this.groupBox6.ResumeLayout(false);
            this.groupBox7.ResumeLayout(false);
            this.groupBox10.ResumeLayout(false);
            this.groupBox10.PerformLayout();
            this.groupBox8.ResumeLayout(false);
            this.groupBox8.PerformLayout();
            this.groupBox9.ResumeLayout(false);
            this.groupBox9.PerformLayout();
            this.ObjectList.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.PictureBox DeviceWindow;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.GroupBox groupBox4;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.TreeView treeView1;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.ListBox LayerList;
        private System.Windows.Forms.GroupBox groupBox5;
        private System.Windows.Forms.GroupBox groupBox6;
        private System.Windows.Forms.GroupBox ObjectList;
        private System.Windows.Forms.ListBox ObjList;
        private System.Windows.Forms.GroupBox groupBox7;
        private System.Windows.Forms.GroupBox groupBox8;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.GroupBox groupBox9;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox PositionY;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox PositionX;
        private System.Windows.Forms.TextBox PositionZ;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.GroupBox groupBox10;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.TextBox ScaleX;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox ScaleZ;
        private System.Windows.Forms.TextBox ScaleY;
        private System.Windows.Forms.Button SetScale;
        private System.Windows.Forms.TextBox RotationZ;
        private System.Windows.Forms.TextBox RotationY;
        private System.Windows.Forms.TextBox RotationX;
        private System.Windows.Forms.Button button3;
        private System.Windows.Forms.ListBox MeshList;
        private System.Windows.Forms.Button setMesh;
        private System.Windows.Forms.Button button4;
    }
}

