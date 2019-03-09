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
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.ChangeLayer = new System.Windows.Forms.Button();
            this.ChangeTag = new System.Windows.Forms.Button();
            this.label11 = new System.Windows.Forms.Label();
            this.comboBox1 = new System.Windows.Forms.ComboBox();
            this.label10 = new System.Windows.Forms.Label();
            this.ObjTagText = new System.Windows.Forms.TextBox();
            this.button1 = new System.Windows.Forms.Button();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.LogMessage = new System.Windows.Forms.GroupBox();
            this.treeView1 = new System.Windows.Forms.TreeView();
            this.LayerList = new System.Windows.Forms.ListBox();
            this.groupBox5 = new System.Windows.Forms.GroupBox();
            this.Inspector = new System.Windows.Forms.GroupBox();
            this.InspectorTab = new System.Windows.Forms.TabControl();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.Component = new System.Windows.Forms.TabPage();
            this.Effect = new System.Windows.Forms.TabPage();
            this.button4 = new System.Windows.Forms.Button();
            this.setMesh = new System.Windows.Forms.Button();
            this.MeshList = new System.Windows.Forms.ListBox();
            this.groupBox7 = new System.Windows.Forms.GroupBox();
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
            this.FileSave = new System.Windows.Forms.Button();
            this.FileLoad = new System.Windows.Forms.Button();
            this.Renderer = new System.Windows.Forms.GroupBox();
            this.ClipSave = new System.Windows.Forms.Button();
            this.ClipLoad = new System.Windows.Forms.Button();
            this.MshLoad = new System.Windows.Forms.Button();
            this.label12 = new System.Windows.Forms.Label();
            this.label13 = new System.Windows.Forms.Label();
            this.label14 = new System.Windows.Forms.Label();
            this.label15 = new System.Windows.Forms.Label();
            this.label16 = new System.Windows.Forms.Label();
            this.AnimationOptionBox = new System.Windows.Forms.ComboBox();
            this.AnimationTagBox = new System.Windows.Forms.TextBox();
            this.AniStartFrameBox = new System.Windows.Forms.TextBox();
            this.AniEndFrameBox = new System.Windows.Forms.TextBox();
            this.AniTimeBox = new System.Windows.Forms.TextBox();
            this.AddClipBtn = new System.Windows.Forms.Button();
            this.ModifyClipBtn = new System.Windows.Forms.Button();
            this.DivideClipBox = new System.Windows.Forms.CheckBox();
            this.ClipList = new System.Windows.Forms.ListBox();
            ((System.ComponentModel.ISupportInitialize)(this.DeviceWindow)).BeginInit();
            this.groupBox2.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.groupBox5.SuspendLayout();
            this.Inspector.SuspendLayout();
            this.InspectorTab.SuspendLayout();
            this.tabPage1.SuspendLayout();
            this.groupBox7.SuspendLayout();
            this.groupBox10.SuspendLayout();
            this.groupBox8.SuspendLayout();
            this.groupBox9.SuspendLayout();
            this.ObjectList.SuspendLayout();
            this.Renderer.SuspendLayout();
            this.SuspendLayout();
            // 
            // DeviceWindow
            // 
            this.DeviceWindow.Location = new System.Drawing.Point(6, 29);
            this.DeviceWindow.Name = "DeviceWindow";
            this.DeviceWindow.Size = new System.Drawing.Size(679, 511);
            this.DeviceWindow.TabIndex = 0;
            this.DeviceWindow.TabStop = false;
            this.DeviceWindow.MouseClick += new System.Windows.Forms.MouseEventHandler(this.Scene_Click);
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.ChangeLayer);
            this.groupBox2.Controls.Add(this.ChangeTag);
            this.groupBox2.Controls.Add(this.label11);
            this.groupBox2.Controls.Add(this.comboBox1);
            this.groupBox2.Controls.Add(this.label10);
            this.groupBox2.Controls.Add(this.ObjTagText);
            this.groupBox2.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.groupBox2.Location = new System.Drawing.Point(871, 0);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(318, 79);
            this.groupBox2.TabIndex = 2;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Create Object";
            // 
            // ChangeLayer
            // 
            this.ChangeLayer.Location = new System.Drawing.Point(182, 47);
            this.ChangeLayer.Name = "ChangeLayer";
            this.ChangeLayer.Size = new System.Drawing.Size(126, 23);
            this.ChangeLayer.TabIndex = 16;
            this.ChangeLayer.Text = "Change Layer";
            this.ChangeLayer.UseVisualStyleBackColor = true;
            // 
            // ChangeTag
            // 
            this.ChangeTag.Location = new System.Drawing.Point(213, 18);
            this.ChangeTag.Name = "ChangeTag";
            this.ChangeTag.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.ChangeTag.Size = new System.Drawing.Size(95, 23);
            this.ChangeTag.TabIndex = 15;
            this.ChangeTag.Text = "Change Tag";
            this.ChangeTag.UseVisualStyleBackColor = true;
            this.ChangeTag.Click += new System.EventHandler(this.ChangeTag_Click);
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.Location = new System.Drawing.Point(16, 49);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(49, 17);
            this.label11.TabIndex = 14;
            this.label11.Text = "Layer :";
            // 
            // comboBox1
            // 
            this.comboBox1.BackColor = System.Drawing.SystemColors.Control;
            this.comboBox1.FormattingEnabled = true;
            this.comboBox1.Location = new System.Drawing.Point(73, 46);
            this.comboBox1.Name = "comboBox1";
            this.comboBox1.Size = new System.Drawing.Size(103, 25);
            this.comboBox1.TabIndex = 13;
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(26, 21);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(39, 17);
            this.label10.TabIndex = 12;
            this.label10.Text = "Tag :";
            // 
            // ObjTagText
            // 
            this.ObjTagText.BackColor = System.Drawing.SystemColors.Control;
            this.ObjTagText.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.ObjTagText.Font = new System.Drawing.Font("맑은 고딕", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.ObjTagText.ForeColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.ObjTagText.Location = new System.Drawing.Point(73, 18);
            this.ObjTagText.Name = "ObjTagText";
            this.ObjTagText.Size = new System.Drawing.Size(134, 23);
            this.ObjTagText.TabIndex = 1;
            this.ObjTagText.TextChanged += new System.EventHandler(this.textBox1_TextChanged);
            // 
            // button1
            // 
            this.button1.Font = new System.Drawing.Font("맑은 고딕", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.button1.Location = new System.Drawing.Point(31, 417);
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
            this.groupBox3.Location = new System.Drawing.Point(168, -4);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(691, 546);
            this.groupBox3.TabIndex = 3;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Scene";
            // 
            // LogMessage
            // 
            this.LogMessage.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.LogMessage.Location = new System.Drawing.Point(168, 542);
            this.LogMessage.Name = "LogMessage";
            this.LogMessage.Size = new System.Drawing.Size(691, 86);
            this.LogMessage.TabIndex = 4;
            this.LogMessage.TabStop = false;
            this.LogMessage.Text = "Log Message";
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
            this.LayerList.Location = new System.Drawing.Point(11, 19);
            this.LayerList.Name = "LayerList";
            this.LayerList.Size = new System.Drawing.Size(121, 154);
            this.LayerList.TabIndex = 6;
            this.LayerList.SelectedIndexChanged += new System.EventHandler(this.LayerList_SelectedIndexChanged);
            // 
            // groupBox5
            // 
            this.groupBox5.Controls.Add(this.LayerList);
            this.groupBox5.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.groupBox5.Location = new System.Drawing.Point(12, 448);
            this.groupBox5.Name = "groupBox5";
            this.groupBox5.Size = new System.Drawing.Size(150, 180);
            this.groupBox5.TabIndex = 7;
            this.groupBox5.TabStop = false;
            this.groupBox5.Text = "Layer List";
            // 
            // Inspector
            // 
            this.Inspector.Controls.Add(this.InspectorTab);
            this.Inspector.Font = new System.Drawing.Font("맑은 고딕", 10F, System.Drawing.FontStyle.Bold);
            this.Inspector.ForeColor = System.Drawing.SystemColors.Desktop;
            this.Inspector.Location = new System.Drawing.Point(1195, 0);
            this.Inspector.Name = "Inspector";
            this.Inspector.Size = new System.Drawing.Size(301, 628);
            this.Inspector.TabIndex = 8;
            this.Inspector.TabStop = false;
            this.Inspector.Text = "Inspector";
            // 
            // InspectorTab
            // 
            this.InspectorTab.Controls.Add(this.tabPage1);
            this.InspectorTab.Controls.Add(this.Component);
            this.InspectorTab.Controls.Add(this.Effect);
            this.InspectorTab.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.InspectorTab.Location = new System.Drawing.Point(6, 24);
            this.InspectorTab.Multiline = true;
            this.InspectorTab.Name = "InspectorTab";
            this.InspectorTab.SelectedIndex = 0;
            this.InspectorTab.Size = new System.Drawing.Size(295, 596);
            this.InspectorTab.TabIndex = 0;
            // 
            // tabPage1
            // 
            this.tabPage1.BackColor = System.Drawing.SystemColors.Control;
            this.tabPage1.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.tabPage1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.tabPage1.Controls.Add(this.ClipList);
            this.tabPage1.Controls.Add(this.DivideClipBox);
            this.tabPage1.Controls.Add(this.ModifyClipBtn);
            this.tabPage1.Controls.Add(this.AddClipBtn);
            this.tabPage1.Controls.Add(this.AniTimeBox);
            this.tabPage1.Controls.Add(this.AniEndFrameBox);
            this.tabPage1.Controls.Add(this.AniStartFrameBox);
            this.tabPage1.Controls.Add(this.AnimationTagBox);
            this.tabPage1.Controls.Add(this.AnimationOptionBox);
            this.tabPage1.Controls.Add(this.label16);
            this.tabPage1.Controls.Add(this.label15);
            this.tabPage1.Controls.Add(this.label14);
            this.tabPage1.Controls.Add(this.label13);
            this.tabPage1.Controls.Add(this.label12);
            this.tabPage1.Controls.Add(this.MshLoad);
            this.tabPage1.Controls.Add(this.ClipLoad);
            this.tabPage1.Controls.Add(this.ClipSave);
            this.tabPage1.ForeColor = System.Drawing.SystemColors.ControlText;
            this.tabPage1.Location = new System.Drawing.Point(4, 26);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage1.Size = new System.Drawing.Size(287, 566);
            this.tabPage1.TabIndex = 0;
            this.tabPage1.Text = "Animation";
            // 
            // Component
            // 
            this.Component.BackColor = System.Drawing.SystemColors.Control;
            this.Component.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.Component.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.Component.ForeColor = System.Drawing.SystemColors.ControlText;
            this.Component.Location = new System.Drawing.Point(4, 26);
            this.Component.Name = "Component";
            this.Component.Padding = new System.Windows.Forms.Padding(3);
            this.Component.Size = new System.Drawing.Size(287, 566);
            this.Component.TabIndex = 1;
            this.Component.Text = "Component";
            // 
            // Effect
            // 
            this.Effect.BackColor = System.Drawing.SystemColors.Control;
            this.Effect.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.Effect.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.Effect.ForeColor = System.Drawing.SystemColors.ControlText;
            this.Effect.Location = new System.Drawing.Point(4, 26);
            this.Effect.Name = "Effect";
            this.Effect.Padding = new System.Windows.Forms.Padding(3);
            this.Effect.Size = new System.Drawing.Size(287, 566);
            this.Effect.TabIndex = 2;
            this.Effect.Text = "Effect";
            // 
            // button4
            // 
            this.button4.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.button4.Location = new System.Drawing.Point(153, 307);
            this.button4.Name = "button4";
            this.button4.Size = new System.Drawing.Size(163, 46);
            this.button4.TabIndex = 3;
            this.button4.Text = "Add Render Component";
            this.button4.UseVisualStyleBackColor = true;
            this.button4.Click += new System.EventHandler(this.AddRenderComponent);
            // 
            // setMesh
            // 
            this.setMesh.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.setMesh.Location = new System.Drawing.Point(153, 262);
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
            this.MeshList.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.MeshList.FormattingEnabled = true;
            this.MeshList.ItemHeight = 17;
            this.MeshList.Location = new System.Drawing.Point(8, 26);
            this.MeshList.Name = "MeshList";
            this.MeshList.Size = new System.Drawing.Size(140, 327);
            this.MeshList.TabIndex = 1;
            this.MeshList.SelectedIndexChanged += new System.EventHandler(this.MeshList_SelectedIndexChanged);
            // 
            // groupBox7
            // 
            this.groupBox7.Controls.Add(this.groupBox10);
            this.groupBox7.Controls.Add(this.groupBox8);
            this.groupBox7.Controls.Add(this.groupBox9);
            this.groupBox7.Font = new System.Drawing.Font("맑은 고딕", 10F, System.Drawing.FontStyle.Bold);
            this.groupBox7.Location = new System.Drawing.Point(871, 80);
            this.groupBox7.Name = "groupBox7";
            this.groupBox7.Size = new System.Drawing.Size(318, 185);
            this.groupBox7.TabIndex = 0;
            this.groupBox7.TabStop = false;
            this.groupBox7.Text = "Transform";
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
            this.RotationZ.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.RotationZ.Location = new System.Drawing.Point(228, 15);
            this.RotationZ.Name = "RotationZ";
            this.RotationZ.Size = new System.Drawing.Size(80, 25);
            this.RotationZ.TabIndex = 20;
            this.RotationZ.TextChanged += new System.EventHandler(this.RotationZ_TextChanged);
            this.RotationZ.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.Transform_Press);
            // 
            // RotationY
            // 
            this.RotationY.BackColor = System.Drawing.SystemColors.Control;
            this.RotationY.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.RotationY.Location = new System.Drawing.Point(123, 15);
            this.RotationY.Name = "RotationY";
            this.RotationY.Size = new System.Drawing.Size(84, 25);
            this.RotationY.TabIndex = 19;
            this.RotationY.TextChanged += new System.EventHandler(this.RotationY_TextChanged);
            this.RotationY.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.Transform_Press);
            // 
            // RotationX
            // 
            this.RotationX.BackColor = System.Drawing.SystemColors.Control;
            this.RotationX.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.RotationX.Location = new System.Drawing.Point(20, 17);
            this.RotationX.Name = "RotationX";
            this.RotationX.Size = new System.Drawing.Size(80, 25);
            this.RotationX.TabIndex = 18;
            this.RotationX.TextChanged += new System.EventHandler(this.RotationX_TextChanged);
            this.RotationX.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.Transform_Press);
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
            this.PositionZ.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.PositionZ.Location = new System.Drawing.Point(229, 17);
            this.PositionZ.Name = "PositionZ";
            this.PositionZ.Size = new System.Drawing.Size(80, 25);
            this.PositionZ.TabIndex = 13;
            this.PositionZ.TextChanged += new System.EventHandler(this.PositionZ_TextChanged);
            this.PositionZ.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.Transform_Press);
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
            this.PositionY.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.PositionY.Location = new System.Drawing.Point(123, 17);
            this.PositionY.Name = "PositionY";
            this.PositionY.Size = new System.Drawing.Size(87, 25);
            this.PositionY.TabIndex = 11;
            this.PositionY.TextChanged += new System.EventHandler(this.PositionY_TextChanged);
            this.PositionY.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.Transform_Press);
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
            this.PositionX.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.PositionX.Location = new System.Drawing.Point(19, 17);
            this.PositionX.Name = "PositionX";
            this.PositionX.Size = new System.Drawing.Size(86, 25);
            this.PositionX.TabIndex = 2;
            this.PositionX.TextChanged += new System.EventHandler(this.PositionX_TextChanged);
            this.PositionX.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.Transform_Press);
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
            this.ScaleZ.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.ScaleZ.ForeColor = System.Drawing.SystemColors.ControlText;
            this.ScaleZ.Location = new System.Drawing.Point(228, 16);
            this.ScaleZ.Name = "ScaleZ";
            this.ScaleZ.Size = new System.Drawing.Size(80, 25);
            this.ScaleZ.TabIndex = 17;
            this.ScaleZ.TextChanged += new System.EventHandler(this.ScaleZ_TextChanged);
            this.ScaleZ.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.Transform_Press);
            // 
            // ScaleY
            // 
            this.ScaleY.BackColor = System.Drawing.SystemColors.Control;
            this.ScaleY.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.ScaleY.ForeColor = System.Drawing.SystemColors.ControlText;
            this.ScaleY.Location = new System.Drawing.Point(123, 17);
            this.ScaleY.Name = "ScaleY";
            this.ScaleY.Size = new System.Drawing.Size(87, 25);
            this.ScaleY.TabIndex = 16;
            this.ScaleY.TextChanged += new System.EventHandler(this.ScaleY_TextChanged);
            this.ScaleY.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.Transform_Press);
            // 
            // ScaleX
            // 
            this.ScaleX.BackColor = System.Drawing.SystemColors.Control;
            this.ScaleX.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.ScaleX.ForeColor = System.Drawing.SystemColors.ControlText;
            this.ScaleX.Location = new System.Drawing.Point(20, 17);
            this.ScaleX.Name = "ScaleX";
            this.ScaleX.Size = new System.Drawing.Size(80, 25);
            this.ScaleX.TabIndex = 15;
            this.ScaleX.TextChanged += new System.EventHandler(this.ScaleX_TextChanged);
            this.ScaleX.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.Transform_Press);
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
            this.ObjectList.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.ObjectList.Location = new System.Drawing.Point(11, 77);
            this.ObjectList.Name = "ObjectList";
            this.ObjectList.Size = new System.Drawing.Size(150, 365);
            this.ObjectList.TabIndex = 9;
            this.ObjectList.TabStop = false;
            this.ObjectList.Text = "Hierachy";
            // 
            // ObjList
            // 
            this.ObjList.BackColor = System.Drawing.SystemColors.Control;
            this.ObjList.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.ObjList.ForeColor = System.Drawing.SystemColors.ControlText;
            this.ObjList.FormattingEnabled = true;
            this.ObjList.ItemHeight = 17;
            this.ObjList.Location = new System.Drawing.Point(5, 24);
            this.ObjList.Name = "ObjList";
            this.ObjList.Size = new System.Drawing.Size(139, 310);
            this.ObjList.TabIndex = 0;
            this.ObjList.SelectedIndexChanged += new System.EventHandler(this.ObjList_SelectedIndexChanged);
            // 
            // FileSave
            // 
            this.FileSave.Location = new System.Drawing.Point(11, 12);
            this.FileSave.Name = "FileSave";
            this.FileSave.Size = new System.Drawing.Size(71, 47);
            this.FileSave.TabIndex = 10;
            this.FileSave.Text = "File Save";
            this.FileSave.UseVisualStyleBackColor = true;
            // 
            // FileLoad
            // 
            this.FileLoad.Location = new System.Drawing.Point(86, 12);
            this.FileLoad.Name = "FileLoad";
            this.FileLoad.Size = new System.Drawing.Size(75, 47);
            this.FileLoad.TabIndex = 11;
            this.FileLoad.Text = "File Load";
            this.FileLoad.UseVisualStyleBackColor = true;
            // 
            // Renderer
            // 
            this.Renderer.Controls.Add(this.setMesh);
            this.Renderer.Controls.Add(this.MeshList);
            this.Renderer.Controls.Add(this.button4);
            this.Renderer.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.Renderer.Location = new System.Drawing.Point(864, 270);
            this.Renderer.Name = "Renderer";
            this.Renderer.Size = new System.Drawing.Size(325, 358);
            this.Renderer.TabIndex = 0;
            this.Renderer.TabStop = false;
            this.Renderer.Text = "Renderer";
            // 
            // ClipSave
            // 
            this.ClipSave.Location = new System.Drawing.Point(5, 503);
            this.ClipSave.Name = "ClipSave";
            this.ClipSave.Size = new System.Drawing.Size(75, 23);
            this.ClipSave.TabIndex = 0;
            this.ClipSave.Text = "Clip Save";
            this.ClipSave.UseVisualStyleBackColor = true;
            this.ClipSave.Click += new System.EventHandler(this.ClipSave_Click);
            // 
            // ClipLoad
            // 
            this.ClipLoad.Location = new System.Drawing.Point(86, 503);
            this.ClipLoad.Name = "ClipLoad";
            this.ClipLoad.Size = new System.Drawing.Size(75, 23);
            this.ClipLoad.TabIndex = 13;
            this.ClipLoad.Text = "Clip Load";
            this.ClipLoad.UseVisualStyleBackColor = true;
            this.ClipLoad.Click += new System.EventHandler(this.ClipLoad_Click);
            // 
            // MshLoad
            // 
            this.MshLoad.Location = new System.Drawing.Point(165, 503);
            this.MshLoad.Name = "MshLoad";
            this.MshLoad.Size = new System.Drawing.Size(118, 23);
            this.MshLoad.TabIndex = 16;
            this.MshLoad.Text = "Msh File Load";
            this.MshLoad.UseVisualStyleBackColor = true;
            this.MshLoad.Click += new System.EventHandler(this.MshLoad_Click);
            // 
            // label12
            // 
            this.label12.AutoSize = true;
            this.label12.Location = new System.Drawing.Point(80, 368);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(44, 17);
            this.label12.TabIndex = 12;
            this.label12.Text = "Name";
            // 
            // label13
            // 
            this.label13.AutoSize = true;
            this.label13.Location = new System.Drawing.Point(44, 397);
            this.label13.Name = "label13";
            this.label13.Size = new System.Drawing.Size(80, 17);
            this.label13.TabIndex = 17;
            this.label13.Text = "Start Frame";
            // 
            // label14
            // 
            this.label14.AutoSize = true;
            this.label14.Location = new System.Drawing.Point(50, 423);
            this.label14.Name = "label14";
            this.label14.Size = new System.Drawing.Size(74, 17);
            this.label14.TabIndex = 18;
            this.label14.Text = "End Frame";
            // 
            // label15
            // 
            this.label15.AutoSize = true;
            this.label15.Location = new System.Drawing.Point(15, 449);
            this.label15.Name = "label15";
            this.label15.Size = new System.Drawing.Size(109, 17);
            this.label15.TabIndex = 19;
            this.label15.Text = "Animation Time";
            // 
            // label16
            // 
            this.label16.AutoSize = true;
            this.label16.Location = new System.Drawing.Point(3, 477);
            this.label16.Name = "label16";
            this.label16.Size = new System.Drawing.Size(121, 17);
            this.label16.TabIndex = 20;
            this.label16.Text = "Animation Option";
            // 
            // AnimationOptionBox
            // 
            this.AnimationOptionBox.BackColor = System.Drawing.SystemColors.Control;
            this.AnimationOptionBox.FormattingEnabled = true;
            this.AnimationOptionBox.Location = new System.Drawing.Point(129, 474);
            this.AnimationOptionBox.Name = "AnimationOptionBox";
            this.AnimationOptionBox.Size = new System.Drawing.Size(149, 25);
            this.AnimationOptionBox.TabIndex = 21;
            this.AnimationOptionBox.SelectedIndexChanged += new System.EventHandler(this.AnimationOption_SelectedIndexChanged);
            // 
            // AnimationTagBox
            // 
            this.AnimationTagBox.BackColor = System.Drawing.SystemColors.Control;
            this.AnimationTagBox.Location = new System.Drawing.Point(130, 366);
            this.AnimationTagBox.Name = "AnimationTagBox";
            this.AnimationTagBox.Size = new System.Drawing.Size(149, 25);
            this.AnimationTagBox.TabIndex = 12;
            this.AnimationTagBox.TextChanged += new System.EventHandler(this.AnimationTagBox_TextChanged);
            // 
            // AniStartFrameBox
            // 
            this.AniStartFrameBox.BackColor = System.Drawing.SystemColors.Control;
            this.AniStartFrameBox.Location = new System.Drawing.Point(130, 394);
            this.AniStartFrameBox.Name = "AniStartFrameBox";
            this.AniStartFrameBox.Size = new System.Drawing.Size(148, 25);
            this.AniStartFrameBox.TabIndex = 22;
            this.AniStartFrameBox.TextChanged += new System.EventHandler(this.AniStartFrameBox_TextChanged);
            // 
            // AniEndFrameBox
            // 
            this.AniEndFrameBox.BackColor = System.Drawing.SystemColors.Control;
            this.AniEndFrameBox.Location = new System.Drawing.Point(130, 421);
            this.AniEndFrameBox.Name = "AniEndFrameBox";
            this.AniEndFrameBox.Size = new System.Drawing.Size(148, 25);
            this.AniEndFrameBox.TabIndex = 23;
            this.AniEndFrameBox.TextChanged += new System.EventHandler(this.AniEndFrameBox_TextChanged);
            // 
            // AniTimeBox
            // 
            this.AniTimeBox.BackColor = System.Drawing.SystemColors.Control;
            this.AniTimeBox.Location = new System.Drawing.Point(129, 447);
            this.AniTimeBox.Name = "AniTimeBox";
            this.AniTimeBox.Size = new System.Drawing.Size(149, 25);
            this.AniTimeBox.TabIndex = 24;
            this.AniTimeBox.TextChanged += new System.EventHandler(this.AniTimeBox_TextChanged);
            // 
            // AddClipBtn
            // 
            this.AddClipBtn.Location = new System.Drawing.Point(6, 306);
            this.AddClipBtn.Name = "AddClipBtn";
            this.AddClipBtn.Size = new System.Drawing.Size(74, 48);
            this.AddClipBtn.TabIndex = 12;
            this.AddClipBtn.Text = "Add Clip";
            this.AddClipBtn.UseVisualStyleBackColor = true;
            // 
            // ModifyClipBtn
            // 
            this.ModifyClipBtn.Location = new System.Drawing.Point(92, 306);
            this.ModifyClipBtn.Name = "ModifyClipBtn";
            this.ModifyClipBtn.Size = new System.Drawing.Size(73, 48);
            this.ModifyClipBtn.TabIndex = 25;
            this.ModifyClipBtn.Text = "Modify Clip";
            this.ModifyClipBtn.UseVisualStyleBackColor = true;
            // 
            // DivideClipBox
            // 
            this.DivideClipBox.AutoSize = true;
            this.DivideClipBox.Location = new System.Drawing.Point(178, 321);
            this.DivideClipBox.Name = "DivideClipBox";
            this.DivideClipBox.Size = new System.Drawing.Size(96, 21);
            this.DivideClipBox.TabIndex = 26;
            this.DivideClipBox.Text = "Divide Clip";
            this.DivideClipBox.UseVisualStyleBackColor = true;
            this.DivideClipBox.CheckedChanged += new System.EventHandler(this.DivideClipBox_CheckedChanged);
            // 
            // ClipList
            // 
            this.ClipList.BackColor = System.Drawing.SystemColors.Control;
            this.ClipList.FormattingEnabled = true;
            this.ClipList.ItemHeight = 17;
            this.ClipList.Location = new System.Drawing.Point(6, 2);
            this.ClipList.Name = "ClipList";
            this.ClipList.Size = new System.Drawing.Size(272, 293);
            this.ClipList.TabIndex = 27;
            // 
            // EditorForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.SystemColors.Control;
            this.ClientSize = new System.Drawing.Size(1508, 634);
            this.Controls.Add(this.FileLoad);
            this.Controls.Add(this.FileSave);
            this.Controls.Add(this.groupBox7);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.groupBox5);
            this.Controls.Add(this.Inspector);
            this.Controls.Add(this.LogMessage);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.ObjectList);
            this.Controls.Add(this.Renderer);
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
            this.Inspector.ResumeLayout(false);
            this.InspectorTab.ResumeLayout(false);
            this.tabPage1.ResumeLayout(false);
            this.tabPage1.PerformLayout();
            this.groupBox7.ResumeLayout(false);
            this.groupBox10.ResumeLayout(false);
            this.groupBox10.PerformLayout();
            this.groupBox8.ResumeLayout(false);
            this.groupBox8.PerformLayout();
            this.groupBox9.ResumeLayout(false);
            this.groupBox9.PerformLayout();
            this.ObjectList.ResumeLayout(false);
            this.Renderer.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.PictureBox DeviceWindow;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.GroupBox LogMessage;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.TreeView treeView1;
        private System.Windows.Forms.TextBox ObjTagText;
        private System.Windows.Forms.ListBox LayerList;
        private System.Windows.Forms.GroupBox groupBox5;
        private System.Windows.Forms.GroupBox Inspector;
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
        private System.Windows.Forms.TextBox RotationZ;
        private System.Windows.Forms.TextBox RotationY;
        private System.Windows.Forms.TextBox RotationX;
        private System.Windows.Forms.ListBox MeshList;
        private System.Windows.Forms.Button setMesh;
        private System.Windows.Forms.Button button4;
        private System.Windows.Forms.Button FileSave;
        private System.Windows.Forms.Button FileLoad;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.ComboBox comboBox1;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.GroupBox Renderer;
        private System.Windows.Forms.TabPage tabPage1;
        private System.Windows.Forms.TabPage Component;
        private System.Windows.Forms.TabPage Effect;
        private System.Windows.Forms.TabControl InspectorTab;
        private System.Windows.Forms.Button ChangeTag;
        private System.Windows.Forms.Button ChangeLayer;
        private System.Windows.Forms.ComboBox AnimationOptionBox;
        private System.Windows.Forms.Label label16;
        private System.Windows.Forms.Label label15;
        private System.Windows.Forms.Label label14;
        private System.Windows.Forms.Label label13;
        private System.Windows.Forms.Label label12;
        private System.Windows.Forms.Button MshLoad;
        private System.Windows.Forms.Button ClipLoad;
        private System.Windows.Forms.Button ClipSave;
        private System.Windows.Forms.TextBox AniTimeBox;
        private System.Windows.Forms.TextBox AniEndFrameBox;
        private System.Windows.Forms.TextBox AniStartFrameBox;
        private System.Windows.Forms.TextBox AnimationTagBox;
        private System.Windows.Forms.Button ModifyClipBtn;
        private System.Windows.Forms.Button AddClipBtn;
        private System.Windows.Forms.CheckBox DivideClipBox;
        private System.Windows.Forms.ListBox ClipList;
    }
}

