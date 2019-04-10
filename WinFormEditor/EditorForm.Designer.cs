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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(EditorForm));
            this.PB_DeviceWindow = new System.Windows.Forms.PictureBox();
            this.label11 = new System.Windows.Forms.Label();
            this.CB_LayerList = new System.Windows.Forms.ComboBox();
            this.label10 = new System.Windows.Forms.Label();
            this.TB_ObjectTag = new System.Windows.Forms.TextBox();
            this.Btn_ObjectCreate = new System.Windows.Forms.Button();
            this.GB_Scene = new System.Windows.Forms.GroupBox();
            this.treeView1 = new System.Windows.Forms.TreeView();
            this.Inspector = new System.Windows.Forms.GroupBox();
            this.InspectorTab = new System.Windows.Forms.TabControl();
            this.Animation = new System.Windows.Forms.TabPage();
            this.label21 = new System.Windows.Forms.Label();
            this.LB_BoneList = new System.Windows.Forms.ListBox();
            this.DeleteClipBtn = new System.Windows.Forms.Button();
            this.DeleteClipText = new System.Windows.Forms.TextBox();
            this.ClipList = new System.Windows.Forms.ListBox();
            this.DivideClipBox = new System.Windows.Forms.CheckBox();
            this.ModifyClipBtn = new System.Windows.Forms.Button();
            this.AddClipBtn = new System.Windows.Forms.Button();
            this.AniTimeBox = new System.Windows.Forms.TextBox();
            this.AniEndFrameBox = new System.Windows.Forms.TextBox();
            this.AniStartFrameBox = new System.Windows.Forms.TextBox();
            this.AnimationTagBox = new System.Windows.Forms.TextBox();
            this.AnimationOptionBox = new System.Windows.Forms.ComboBox();
            this.label16 = new System.Windows.Forms.Label();
            this.label15 = new System.Windows.Forms.Label();
            this.label14 = new System.Windows.Forms.Label();
            this.label13 = new System.Windows.Forms.Label();
            this.label12 = new System.Windows.Forms.Label();
            this.MshLoad = new System.Windows.Forms.Button();
            this.ClipLoad = new System.Windows.Forms.Button();
            this.ClipSave = new System.Windows.Forms.Button();
            this.Component = new System.Windows.Forms.TabPage();
            this.Effect = new System.Windows.Forms.TabPage();
            this.RenderPage = new System.Windows.Forms.TabPage();
            this.LightPage = new System.Windows.Forms.TabPage();
            this.CB_WireFrame = new System.Windows.Forms.CheckBox();
            this.label34 = new System.Windows.Forms.Label();
            this.TB_Range = new System.Windows.Forms.TrackBar();
            this.TB_SpcularPower = new System.Windows.Forms.TextBox();
            this.label33 = new System.Windows.Forms.Label();
            this.label32 = new System.Windows.Forms.Label();
            this.label30 = new System.Windows.Forms.Label();
            this.label29 = new System.Windows.Forms.Label();
            this.TB_SpcularG = new System.Windows.Forms.TextBox();
            this.TB_SpcularB = new System.Windows.Forms.TextBox();
            this.label28 = new System.Windows.Forms.Label();
            this.TB_SpcularR = new System.Windows.Forms.TextBox();
            this.BT_DifColor = new System.Windows.Forms.Button();
            this.label31 = new System.Windows.Forms.Label();
            this.label27 = new System.Windows.Forms.Label();
            this.label26 = new System.Windows.Forms.Label();
            this.label25 = new System.Windows.Forms.Label();
            this.TB_LightZDir = new System.Windows.Forms.TrackBar();
            this.label24 = new System.Windows.Forms.Label();
            this.TB_LightYDir = new System.Windows.Forms.TrackBar();
            this.label23 = new System.Windows.Forms.Label();
            this.TB_LightXDir = new System.Windows.Forms.TrackBar();
            this.button1 = new System.Windows.Forms.Button();
            this.label22 = new System.Windows.Forms.Label();
            this.CB_LightType = new System.Windows.Forms.ComboBox();
            this.NavigationPage = new System.Windows.Forms.TabPage();
            this.LB_MeshList = new System.Windows.Forms.ListBox();
            this.GB_Transform = new System.Windows.Forms.GroupBox();
            this.RotateZ = new System.Windows.Forms.TextBox();
            this.Label_RotZ = new System.Windows.Forms.Label();
            this.RotateY = new System.Windows.Forms.TextBox();
            this.Label_RotX = new System.Windows.Forms.Label();
            this.Label_RotY = new System.Windows.Forms.Label();
            this.RotateX = new System.Windows.Forms.TextBox();
            this.Label_Rotate = new System.Windows.Forms.Label();
            this.ScaleZ = new System.Windows.Forms.TextBox();
            this.Label_ScaleZ = new System.Windows.Forms.Label();
            this.ScaleY = new System.Windows.Forms.TextBox();
            this.Label_ScaleX = new System.Windows.Forms.Label();
            this.Label_ScaleY = new System.Windows.Forms.Label();
            this.ScaleX = new System.Windows.Forms.TextBox();
            this.Label_Scale = new System.Windows.Forms.Label();
            this.PositionZ = new System.Windows.Forms.TextBox();
            this.Label_WorldPosition = new System.Windows.Forms.Label();
            this.Label_PosZ = new System.Windows.Forms.Label();
            this.PositionY = new System.Windows.Forms.TextBox();
            this.Label_PosX = new System.Windows.Forms.Label();
            this.Label_PosY = new System.Windows.Forms.Label();
            this.PositionX = new System.Windows.Forms.TextBox();
            this.GB_Hierachy = new System.Windows.Forms.GroupBox();
            this.Btn_ObjectDelete = new System.Windows.Forms.Button();
            this.LB_ObjectList = new System.Windows.Forms.ListBox();
            this.Btn_FileSave = new System.Windows.Forms.Button();
            this.Btn_FileLoad = new System.Windows.Forms.Button();
            this.LB_LogMessage = new System.Windows.Forms.ListBox();
            this.Btn_AddComponent = new System.Windows.Forms.Button();
            this.Label_BasicMesh = new System.Windows.Forms.Label();
            this.Btn_AddMesh = new System.Windows.Forms.Button();
            this.GB_Renderer = new System.Windows.Forms.GroupBox();
            this.CB_isDebugTarget = new System.Windows.Forms.CheckBox();
            this.FileMesh = new System.Windows.Forms.Label();
            this.LB_FileMesh = new System.Windows.Forms.ListBox();
            this.label1 = new System.Windows.Forms.Label();
            this.LB_AniList = new System.Windows.Forms.ListBox();
            this.Btn_AllClear = new System.Windows.Forms.Button();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.textBox2 = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.textBox3 = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.textBox4 = new System.Windows.Forms.TextBox();
            this.label6 = new System.Windows.Forms.Label();
            this.textBox5 = new System.Windows.Forms.TextBox();
            this.label7 = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.textBox6 = new System.Windows.Forms.TextBox();
            this.label9 = new System.Windows.Forms.Label();
            this.textBox7 = new System.Windows.Forms.TextBox();
            this.label17 = new System.Windows.Forms.Label();
            this.label18 = new System.Windows.Forms.Label();
            this.textBox8 = new System.Windows.Forms.TextBox();
            this.label19 = new System.Windows.Forms.Label();
            this.label20 = new System.Windows.Forms.Label();
            this.textBox9 = new System.Windows.Forms.TextBox();
            ((System.ComponentModel.ISupportInitialize)(this.PB_DeviceWindow)).BeginInit();
            this.GB_Scene.SuspendLayout();
            this.Inspector.SuspendLayout();
            this.InspectorTab.SuspendLayout();
            this.Animation.SuspendLayout();
            this.LightPage.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.TB_Range)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.TB_LightZDir)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.TB_LightYDir)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.TB_LightXDir)).BeginInit();
            this.GB_Transform.SuspendLayout();
            this.GB_Hierachy.SuspendLayout();
            this.GB_Renderer.SuspendLayout();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // PB_DeviceWindow
            // 
            this.PB_DeviceWindow.Location = new System.Drawing.Point(6, 24);
            this.PB_DeviceWindow.Name = "PB_DeviceWindow";
            this.PB_DeviceWindow.Size = new System.Drawing.Size(1069, 637);
            this.PB_DeviceWindow.TabIndex = 0;
            this.PB_DeviceWindow.TabStop = false;
            this.PB_DeviceWindow.MouseClick += new System.Windows.Forms.MouseEventHandler(this.Scene_Click);
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.label11.ForeColor = System.Drawing.Color.Black;
            this.label11.Location = new System.Drawing.Point(6, 604);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(49, 17);
            this.label11.TabIndex = 14;
            this.label11.Text = "Layer :";
            // 
            // CB_LayerList
            // 
            this.CB_LayerList.BackColor = System.Drawing.SystemColors.Control;
            this.CB_LayerList.Cursor = System.Windows.Forms.Cursors.Default;
            this.CB_LayerList.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.CB_LayerList.FormattingEnabled = true;
            this.CB_LayerList.Location = new System.Drawing.Point(57, 601);
            this.CB_LayerList.Name = "CB_LayerList";
            this.CB_LayerList.Size = new System.Drawing.Size(143, 25);
            this.CB_LayerList.TabIndex = 13;
            this.CB_LayerList.TextChanged += new System.EventHandler(this.ChangeLayerComboBox);
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.label10.ForeColor = System.Drawing.Color.Black;
            this.label10.Location = new System.Drawing.Point(16, 571);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(39, 17);
            this.label10.TabIndex = 12;
            this.label10.Text = "Tag :";
            // 
            // TB_ObjectTag
            // 
            this.TB_ObjectTag.BackColor = System.Drawing.SystemColors.Control;
            this.TB_ObjectTag.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.TB_ObjectTag.Font = new System.Drawing.Font("맑은 고딕", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.TB_ObjectTag.ForeColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.TB_ObjectTag.Location = new System.Drawing.Point(57, 568);
            this.TB_ObjectTag.Name = "TB_ObjectTag";
            this.TB_ObjectTag.Size = new System.Drawing.Size(143, 23);
            this.TB_ObjectTag.TabIndex = 1;
            this.TB_ObjectTag.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.ChangeObjectTagEnter);
            // 
            // Btn_ObjectCreate
            // 
            this.Btn_ObjectCreate.Font = new System.Drawing.Font("맑은 고딕", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.Btn_ObjectCreate.ForeColor = System.Drawing.Color.Black;
            this.Btn_ObjectCreate.Location = new System.Drawing.Point(5, 639);
            this.Btn_ObjectCreate.Name = "Btn_ObjectCreate";
            this.Btn_ObjectCreate.Size = new System.Drawing.Size(95, 77);
            this.Btn_ObjectCreate.TabIndex = 5;
            this.Btn_ObjectCreate.Text = "Create";
            this.Btn_ObjectCreate.UseVisualStyleBackColor = true;
            this.Btn_ObjectCreate.Click += new System.EventHandler(this.Btn_CreateObject);
            // 
            // GB_Scene
            // 
            this.GB_Scene.Controls.Add(this.PB_DeviceWindow);
            this.GB_Scene.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.GB_Scene.Location = new System.Drawing.Point(223, 12);
            this.GB_Scene.Name = "GB_Scene";
            this.GB_Scene.Size = new System.Drawing.Size(1082, 667);
            this.GB_Scene.TabIndex = 3;
            this.GB_Scene.TabStop = false;
            this.GB_Scene.Text = "Scene";
            // 
            // treeView1
            // 
            this.treeView1.LineColor = System.Drawing.Color.Empty;
            this.treeView1.Location = new System.Drawing.Point(0, 0);
            this.treeView1.Name = "treeView1";
            this.treeView1.Size = new System.Drawing.Size(121, 97);
            this.treeView1.TabIndex = 0;
            // 
            // Inspector
            // 
            this.Inspector.Controls.Add(this.InspectorTab);
            this.Inspector.Font = new System.Drawing.Font("맑은 고딕", 10F, System.Drawing.FontStyle.Bold);
            this.Inspector.ForeColor = System.Drawing.Color.Red;
            this.Inspector.Location = new System.Drawing.Point(1612, 12);
            this.Inspector.Name = "Inspector";
            this.Inspector.Size = new System.Drawing.Size(301, 803);
            this.Inspector.TabIndex = 8;
            this.Inspector.TabStop = false;
            this.Inspector.Text = "Inspector";
            // 
            // InspectorTab
            // 
            this.InspectorTab.Controls.Add(this.Animation);
            this.InspectorTab.Controls.Add(this.Component);
            this.InspectorTab.Controls.Add(this.Effect);
            this.InspectorTab.Controls.Add(this.RenderPage);
            this.InspectorTab.Controls.Add(this.LightPage);
            this.InspectorTab.Controls.Add(this.NavigationPage);
            this.InspectorTab.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.InspectorTab.Location = new System.Drawing.Point(6, 24);
            this.InspectorTab.Multiline = true;
            this.InspectorTab.Name = "InspectorTab";
            this.InspectorTab.SelectedIndex = 0;
            this.InspectorTab.Size = new System.Drawing.Size(295, 798);
            this.InspectorTab.TabIndex = 0;
            // 
            // Animation
            // 
            this.Animation.BackColor = System.Drawing.SystemColors.Control;
            this.Animation.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.Animation.Controls.Add(this.label21);
            this.Animation.Controls.Add(this.LB_BoneList);
            this.Animation.Controls.Add(this.DeleteClipBtn);
            this.Animation.Controls.Add(this.DeleteClipText);
            this.Animation.Controls.Add(this.ClipList);
            this.Animation.Controls.Add(this.DivideClipBox);
            this.Animation.Controls.Add(this.ModifyClipBtn);
            this.Animation.Controls.Add(this.AddClipBtn);
            this.Animation.Controls.Add(this.AniTimeBox);
            this.Animation.Controls.Add(this.AniEndFrameBox);
            this.Animation.Controls.Add(this.AniStartFrameBox);
            this.Animation.Controls.Add(this.AnimationTagBox);
            this.Animation.Controls.Add(this.AnimationOptionBox);
            this.Animation.Controls.Add(this.label16);
            this.Animation.Controls.Add(this.label15);
            this.Animation.Controls.Add(this.label14);
            this.Animation.Controls.Add(this.label13);
            this.Animation.Controls.Add(this.label12);
            this.Animation.Controls.Add(this.MshLoad);
            this.Animation.Controls.Add(this.ClipLoad);
            this.Animation.Controls.Add(this.ClipSave);
            this.Animation.ForeColor = System.Drawing.SystemColors.Desktop;
            this.Animation.Location = new System.Drawing.Point(4, 48);
            this.Animation.Name = "Animation";
            this.Animation.Padding = new System.Windows.Forms.Padding(3);
            this.Animation.Size = new System.Drawing.Size(287, 746);
            this.Animation.TabIndex = 0;
            this.Animation.Text = "Animation";
            // 
            // label21
            // 
            this.label21.AutoSize = true;
            this.label21.Location = new System.Drawing.Point(8, 260);
            this.label21.Name = "label21";
            this.label21.Size = new System.Drawing.Size(80, 17);
            this.label21.TabIndex = 31;
            this.label21.Text = "Bone Name";
            // 
            // LB_BoneList
            // 
            this.LB_BoneList.BackColor = System.Drawing.SystemColors.Control;
            this.LB_BoneList.FormattingEnabled = true;
            this.LB_BoneList.ItemHeight = 17;
            this.LB_BoneList.Location = new System.Drawing.Point(8, 282);
            this.LB_BoneList.Name = "LB_BoneList";
            this.LB_BoneList.Size = new System.Drawing.Size(267, 140);
            this.LB_BoneList.TabIndex = 30;
            // 
            // DeleteClipBtn
            // 
            this.DeleteClipBtn.Location = new System.Drawing.Point(167, 660);
            this.DeleteClipBtn.Name = "DeleteClipBtn";
            this.DeleteClipBtn.Size = new System.Drawing.Size(107, 26);
            this.DeleteClipBtn.TabIndex = 29;
            this.DeleteClipBtn.Text = "Delete Clip";
            this.DeleteClipBtn.UseVisualStyleBackColor = true;
            this.DeleteClipBtn.Click += new System.EventHandler(this.DeleteClipBtn_Click);
            // 
            // DeleteClipText
            // 
            this.DeleteClipText.BackColor = System.Drawing.SystemColors.Control;
            this.DeleteClipText.ForeColor = System.Drawing.SystemColors.Desktop;
            this.DeleteClipText.Location = new System.Drawing.Point(5, 661);
            this.DeleteClipText.Name = "DeleteClipText";
            this.DeleteClipText.Size = new System.Drawing.Size(156, 25);
            this.DeleteClipText.TabIndex = 28;
            this.DeleteClipText.TextChanged += new System.EventHandler(this.DeleteClipText_TextChanged);
            // 
            // ClipList
            // 
            this.ClipList.BackColor = System.Drawing.SystemColors.Control;
            this.ClipList.FormattingEnabled = true;
            this.ClipList.ItemHeight = 17;
            this.ClipList.Location = new System.Drawing.Point(6, 2);
            this.ClipList.Name = "ClipList";
            this.ClipList.Size = new System.Drawing.Size(272, 242);
            this.ClipList.TabIndex = 27;
            this.ClipList.SelectedIndexChanged += new System.EventHandler(this.ClipList_SelectedIndexChanged);
            // 
            // DivideClipBox
            // 
            this.DivideClipBox.AutoSize = true;
            this.DivideClipBox.Location = new System.Drawing.Point(178, 449);
            this.DivideClipBox.Name = "DivideClipBox";
            this.DivideClipBox.Size = new System.Drawing.Size(96, 21);
            this.DivideClipBox.TabIndex = 26;
            this.DivideClipBox.Text = "Divide Clip";
            this.DivideClipBox.UseVisualStyleBackColor = true;
            this.DivideClipBox.CheckedChanged += new System.EventHandler(this.DivideClipBox_CheckedChanged);
            // 
            // ModifyClipBtn
            // 
            this.ModifyClipBtn.Location = new System.Drawing.Point(92, 434);
            this.ModifyClipBtn.Name = "ModifyClipBtn";
            this.ModifyClipBtn.Size = new System.Drawing.Size(73, 48);
            this.ModifyClipBtn.TabIndex = 25;
            this.ModifyClipBtn.Text = "Modify Clip";
            this.ModifyClipBtn.UseVisualStyleBackColor = true;
            this.ModifyClipBtn.Click += new System.EventHandler(this.ModifyClipBtn_Click);
            // 
            // AddClipBtn
            // 
            this.AddClipBtn.Location = new System.Drawing.Point(6, 434);
            this.AddClipBtn.Name = "AddClipBtn";
            this.AddClipBtn.Size = new System.Drawing.Size(74, 48);
            this.AddClipBtn.TabIndex = 12;
            this.AddClipBtn.Text = "Add Clip";
            this.AddClipBtn.UseVisualStyleBackColor = true;
            this.AddClipBtn.Click += new System.EventHandler(this.AddClipBtn_Click);
            // 
            // AniTimeBox
            // 
            this.AniTimeBox.BackColor = System.Drawing.SystemColors.Control;
            this.AniTimeBox.Location = new System.Drawing.Point(129, 575);
            this.AniTimeBox.Name = "AniTimeBox";
            this.AniTimeBox.Size = new System.Drawing.Size(149, 25);
            this.AniTimeBox.TabIndex = 24;
            this.AniTimeBox.TextChanged += new System.EventHandler(this.AniTimeBox_TextChanged);
            // 
            // AniEndFrameBox
            // 
            this.AniEndFrameBox.BackColor = System.Drawing.SystemColors.Control;
            this.AniEndFrameBox.Location = new System.Drawing.Point(130, 549);
            this.AniEndFrameBox.Name = "AniEndFrameBox";
            this.AniEndFrameBox.Size = new System.Drawing.Size(148, 25);
            this.AniEndFrameBox.TabIndex = 23;
            this.AniEndFrameBox.TextChanged += new System.EventHandler(this.AniEndFrameBox_TextChanged);
            // 
            // AniStartFrameBox
            // 
            this.AniStartFrameBox.BackColor = System.Drawing.SystemColors.Control;
            this.AniStartFrameBox.Location = new System.Drawing.Point(130, 522);
            this.AniStartFrameBox.Name = "AniStartFrameBox";
            this.AniStartFrameBox.Size = new System.Drawing.Size(148, 25);
            this.AniStartFrameBox.TabIndex = 22;
            this.AniStartFrameBox.TextChanged += new System.EventHandler(this.AniStartFrameBox_TextChanged);
            // 
            // AnimationTagBox
            // 
            this.AnimationTagBox.BackColor = System.Drawing.SystemColors.Control;
            this.AnimationTagBox.Location = new System.Drawing.Point(130, 494);
            this.AnimationTagBox.Name = "AnimationTagBox";
            this.AnimationTagBox.Size = new System.Drawing.Size(149, 25);
            this.AnimationTagBox.TabIndex = 12;
            this.AnimationTagBox.TextChanged += new System.EventHandler(this.AnimationTagBox_TextChanged);
            // 
            // AnimationOptionBox
            // 
            this.AnimationOptionBox.BackColor = System.Drawing.SystemColors.Control;
            this.AnimationOptionBox.FormattingEnabled = true;
            this.AnimationOptionBox.Location = new System.Drawing.Point(129, 602);
            this.AnimationOptionBox.Name = "AnimationOptionBox";
            this.AnimationOptionBox.Size = new System.Drawing.Size(149, 25);
            this.AnimationOptionBox.TabIndex = 21;
            this.AnimationOptionBox.SelectedIndexChanged += new System.EventHandler(this.AnimationOption_SelectedIndexChanged);
            // 
            // label16
            // 
            this.label16.AutoSize = true;
            this.label16.Location = new System.Drawing.Point(3, 605);
            this.label16.Name = "label16";
            this.label16.Size = new System.Drawing.Size(121, 17);
            this.label16.TabIndex = 20;
            this.label16.Text = "Animation Option";
            // 
            // label15
            // 
            this.label15.AutoSize = true;
            this.label15.Location = new System.Drawing.Point(15, 577);
            this.label15.Name = "label15";
            this.label15.Size = new System.Drawing.Size(109, 17);
            this.label15.TabIndex = 19;
            this.label15.Text = "Animation Time";
            // 
            // label14
            // 
            this.label14.AutoSize = true;
            this.label14.Location = new System.Drawing.Point(50, 551);
            this.label14.Name = "label14";
            this.label14.Size = new System.Drawing.Size(74, 17);
            this.label14.TabIndex = 18;
            this.label14.Text = "End Frame";
            // 
            // label13
            // 
            this.label13.AutoSize = true;
            this.label13.Location = new System.Drawing.Point(44, 525);
            this.label13.Name = "label13";
            this.label13.Size = new System.Drawing.Size(80, 17);
            this.label13.TabIndex = 17;
            this.label13.Text = "Start Frame";
            // 
            // label12
            // 
            this.label12.AutoSize = true;
            this.label12.Location = new System.Drawing.Point(80, 496);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(44, 17);
            this.label12.TabIndex = 12;
            this.label12.Text = "Name";
            // 
            // MshLoad
            // 
            this.MshLoad.Location = new System.Drawing.Point(165, 631);
            this.MshLoad.Name = "MshLoad";
            this.MshLoad.Size = new System.Drawing.Size(118, 23);
            this.MshLoad.TabIndex = 16;
            this.MshLoad.Text = "Msh File Load";
            this.MshLoad.UseVisualStyleBackColor = true;
            this.MshLoad.Click += new System.EventHandler(this.MshLoad_Click);
            // 
            // ClipLoad
            // 
            this.ClipLoad.Location = new System.Drawing.Point(86, 631);
            this.ClipLoad.Name = "ClipLoad";
            this.ClipLoad.Size = new System.Drawing.Size(75, 23);
            this.ClipLoad.TabIndex = 13;
            this.ClipLoad.Text = "Clip Load";
            this.ClipLoad.UseVisualStyleBackColor = true;
            this.ClipLoad.Click += new System.EventHandler(this.ClipLoad_Click);
            // 
            // ClipSave
            // 
            this.ClipSave.Location = new System.Drawing.Point(5, 631);
            this.ClipSave.Name = "ClipSave";
            this.ClipSave.Size = new System.Drawing.Size(75, 23);
            this.ClipSave.TabIndex = 0;
            this.ClipSave.Text = "Clip Save";
            this.ClipSave.UseVisualStyleBackColor = true;
            this.ClipSave.Click += new System.EventHandler(this.ClipSave_Click);
            // 
            // Component
            // 
            this.Component.BackColor = System.Drawing.SystemColors.Control;
            this.Component.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.Component.ForeColor = System.Drawing.SystemColors.ControlText;
            this.Component.Location = new System.Drawing.Point(4, 48);
            this.Component.Name = "Component";
            this.Component.Padding = new System.Windows.Forms.Padding(3);
            this.Component.Size = new System.Drawing.Size(287, 746);
            this.Component.TabIndex = 1;
            this.Component.Text = "Component";
            // 
            // Effect
            // 
            this.Effect.BackColor = System.Drawing.SystemColors.Control;
            this.Effect.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.Effect.ForeColor = System.Drawing.SystemColors.ControlText;
            this.Effect.Location = new System.Drawing.Point(4, 48);
            this.Effect.Name = "Effect";
            this.Effect.Padding = new System.Windows.Forms.Padding(3);
            this.Effect.Size = new System.Drawing.Size(287, 746);
            this.Effect.TabIndex = 2;
            this.Effect.Text = "Effect";
            // 
            // RenderPage
            // 
            this.RenderPage.BackColor = System.Drawing.SystemColors.Control;
            this.RenderPage.Location = new System.Drawing.Point(4, 48);
            this.RenderPage.Name = "RenderPage";
            this.RenderPage.Size = new System.Drawing.Size(287, 746);
            this.RenderPage.TabIndex = 4;
            this.RenderPage.Text = "Render";
            // 
            // LightPage
            // 
            this.LightPage.BackColor = System.Drawing.SystemColors.Control;
            this.LightPage.Controls.Add(this.CB_WireFrame);
            this.LightPage.Controls.Add(this.label34);
            this.LightPage.Controls.Add(this.TB_Range);
            this.LightPage.Controls.Add(this.TB_SpcularPower);
            this.LightPage.Controls.Add(this.label33);
            this.LightPage.Controls.Add(this.label32);
            this.LightPage.Controls.Add(this.label30);
            this.LightPage.Controls.Add(this.label29);
            this.LightPage.Controls.Add(this.TB_SpcularG);
            this.LightPage.Controls.Add(this.TB_SpcularB);
            this.LightPage.Controls.Add(this.label28);
            this.LightPage.Controls.Add(this.TB_SpcularR);
            this.LightPage.Controls.Add(this.BT_DifColor);
            this.LightPage.Controls.Add(this.label31);
            this.LightPage.Controls.Add(this.label27);
            this.LightPage.Controls.Add(this.label26);
            this.LightPage.Controls.Add(this.label25);
            this.LightPage.Controls.Add(this.TB_LightZDir);
            this.LightPage.Controls.Add(this.label24);
            this.LightPage.Controls.Add(this.TB_LightYDir);
            this.LightPage.Controls.Add(this.label23);
            this.LightPage.Controls.Add(this.TB_LightXDir);
            this.LightPage.Controls.Add(this.button1);
            this.LightPage.Controls.Add(this.label22);
            this.LightPage.Controls.Add(this.CB_LightType);
            this.LightPage.Location = new System.Drawing.Point(4, 48);
            this.LightPage.Name = "LightPage";
            this.LightPage.Size = new System.Drawing.Size(287, 746);
            this.LightPage.TabIndex = 5;
            this.LightPage.Text = "Light";
            // 
            // CB_WireFrame
            // 
            this.CB_WireFrame.AutoSize = true;
            this.CB_WireFrame.ForeColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.CB_WireFrame.Location = new System.Drawing.Point(6, 5);
            this.CB_WireFrame.Name = "CB_WireFrame";
            this.CB_WireFrame.Size = new System.Drawing.Size(132, 21);
            this.CB_WireFrame.TabIndex = 46;
            this.CB_WireFrame.Text = "WireFrameOnOff";
            this.CB_WireFrame.UseVisualStyleBackColor = true;
            this.CB_WireFrame.CheckedChanged += new System.EventHandler(this.CB_WireFrame_CheckedChanged);
            // 
            // label34
            // 
            this.label34.AutoSize = true;
            this.label34.ForeColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.label34.Location = new System.Drawing.Point(7, 379);
            this.label34.Name = "label34";
            this.label34.Size = new System.Drawing.Size(46, 17);
            this.label34.TabIndex = 45;
            this.label34.Text = "Range";
            // 
            // TB_Range
            // 
            this.TB_Range.Location = new System.Drawing.Point(13, 399);
            this.TB_Range.Maximum = 1000;
            this.TB_Range.Name = "TB_Range";
            this.TB_Range.Size = new System.Drawing.Size(244, 45);
            this.TB_Range.TabIndex = 44;
            this.TB_Range.Scroll += new System.EventHandler(this.TB_Range_Scroll);
            // 
            // TB_SpcularPower
            // 
            this.TB_SpcularPower.BackColor = System.Drawing.SystemColors.Control;
            this.TB_SpcularPower.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.TB_SpcularPower.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.TB_SpcularPower.Location = new System.Drawing.Point(236, 337);
            this.TB_SpcularPower.Name = "TB_SpcularPower";
            this.TB_SpcularPower.Size = new System.Drawing.Size(42, 25);
            this.TB_SpcularPower.TabIndex = 43;
            this.TB_SpcularPower.TextChanged += new System.EventHandler(this.TB_SpcularPower_TextChanged);
            this.TB_SpcularPower.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.Specular_Press);
            // 
            // label33
            // 
            this.label33.AutoSize = true;
            this.label33.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.label33.ForeColor = System.Drawing.Color.Black;
            this.label33.Location = new System.Drawing.Point(125, 341);
            this.label33.Name = "label33";
            this.label33.Size = new System.Drawing.Size(16, 17);
            this.label33.TabIndex = 42;
            this.label33.Text = "B";
            // 
            // label32
            // 
            this.label32.AutoSize = true;
            this.label32.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.label32.ForeColor = System.Drawing.Color.Black;
            this.label32.Location = new System.Drawing.Point(63, 341);
            this.label32.Name = "label32";
            this.label32.Size = new System.Drawing.Size(17, 17);
            this.label32.TabIndex = 41;
            this.label32.Text = "G";
            // 
            // label30
            // 
            this.label30.AutoSize = true;
            this.label30.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.label30.ForeColor = System.Drawing.Color.Black;
            this.label30.Location = new System.Drawing.Point(189, 340);
            this.label30.Name = "label30";
            this.label30.Size = new System.Drawing.Size(46, 17);
            this.label30.TabIndex = 40;
            this.label30.Text = "Power";
            // 
            // label29
            // 
            this.label29.AutoSize = true;
            this.label29.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.label29.ForeColor = System.Drawing.Color.Black;
            this.label29.Location = new System.Drawing.Point(2, 342);
            this.label29.Name = "label29";
            this.label29.Size = new System.Drawing.Size(16, 17);
            this.label29.TabIndex = 39;
            this.label29.Text = "R";
            // 
            // TB_SpcularG
            // 
            this.TB_SpcularG.BackColor = System.Drawing.SystemColors.Control;
            this.TB_SpcularG.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.TB_SpcularG.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.TB_SpcularG.Location = new System.Drawing.Point(80, 337);
            this.TB_SpcularG.Name = "TB_SpcularG";
            this.TB_SpcularG.Size = new System.Drawing.Size(42, 25);
            this.TB_SpcularG.TabIndex = 38;
            this.TB_SpcularG.TextChanged += new System.EventHandler(this.TB_SpcularG_TextChanged);
            this.TB_SpcularG.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.Specular_Press);
            // 
            // TB_SpcularB
            // 
            this.TB_SpcularB.BackColor = System.Drawing.SystemColors.Control;
            this.TB_SpcularB.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.TB_SpcularB.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.TB_SpcularB.Location = new System.Drawing.Point(141, 337);
            this.TB_SpcularB.Name = "TB_SpcularB";
            this.TB_SpcularB.Size = new System.Drawing.Size(42, 25);
            this.TB_SpcularB.TabIndex = 37;
            this.TB_SpcularB.TextChanged += new System.EventHandler(this.TB_SpcularB_TextChanged);
            this.TB_SpcularB.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.Specular_Press);
            // 
            // label28
            // 
            this.label28.AutoSize = true;
            this.label28.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.label28.ForeColor = System.Drawing.Color.Black;
            this.label28.Location = new System.Drawing.Point(5, 310);
            this.label28.Name = "label28";
            this.label28.Size = new System.Drawing.Size(53, 17);
            this.label28.TabIndex = 35;
            this.label28.Text = "Spcular";
            // 
            // TB_SpcularR
            // 
            this.TB_SpcularR.BackColor = System.Drawing.SystemColors.Control;
            this.TB_SpcularR.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.TB_SpcularR.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.TB_SpcularR.Location = new System.Drawing.Point(18, 337);
            this.TB_SpcularR.Name = "TB_SpcularR";
            this.TB_SpcularR.Size = new System.Drawing.Size(42, 25);
            this.TB_SpcularR.TabIndex = 36;
            this.TB_SpcularR.TextChanged += new System.EventHandler(this.TB_SpcularR_TextChanged);
            this.TB_SpcularR.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.Specular_Press);
            // 
            // BT_DifColor
            // 
            this.BT_DifColor.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.BT_DifColor.ForeColor = System.Drawing.Color.Black;
            this.BT_DifColor.Location = new System.Drawing.Point(15, 242);
            this.BT_DifColor.Name = "BT_DifColor";
            this.BT_DifColor.Size = new System.Drawing.Size(256, 59);
            this.BT_DifColor.TabIndex = 34;
            this.BT_DifColor.Text = "Diffuse Color";
            this.BT_DifColor.UseVisualStyleBackColor = true;
            this.BT_DifColor.Click += new System.EventHandler(this.BT_DifColor_Click);
            // 
            // label31
            // 
            this.label31.AutoSize = true;
            this.label31.ForeColor = System.Drawing.Color.Black;
            this.label31.Location = new System.Drawing.Point(3, 363);
            this.label31.Name = "label31";
            this.label31.Size = new System.Drawing.Size(283, 17);
            this.label31.TabIndex = 17;
            this.label31.Text = "-------------------------------------------------------";
            // 
            // label27
            // 
            this.label27.AutoSize = true;
            this.label27.ForeColor = System.Drawing.Color.Black;
            this.label27.Location = new System.Drawing.Point(2, 59);
            this.label27.Name = "label27";
            this.label27.Size = new System.Drawing.Size(283, 17);
            this.label27.TabIndex = 10;
            this.label27.Text = "-------------------------------------------------------";
            // 
            // label26
            // 
            this.label26.AutoSize = true;
            this.label26.ForeColor = System.Drawing.Color.Black;
            this.label26.Location = new System.Drawing.Point(4, 221);
            this.label26.Name = "label26";
            this.label26.Size = new System.Drawing.Size(283, 17);
            this.label26.TabIndex = 9;
            this.label26.Text = "-------------------------------------------------------";
            // 
            // label25
            // 
            this.label25.AutoSize = true;
            this.label25.ForeColor = System.Drawing.SystemColors.Desktop;
            this.label25.Location = new System.Drawing.Point(6, 172);
            this.label25.Name = "label25";
            this.label25.Size = new System.Drawing.Size(40, 17);
            this.label25.TabIndex = 8;
            this.label25.Text = "Z Dir";
            // 
            // TB_LightZDir
            // 
            this.TB_LightZDir.Location = new System.Drawing.Point(11, 191);
            this.TB_LightZDir.Maximum = 100;
            this.TB_LightZDir.Minimum = -100;
            this.TB_LightZDir.Name = "TB_LightZDir";
            this.TB_LightZDir.Size = new System.Drawing.Size(244, 45);
            this.TB_LightZDir.TabIndex = 7;
            this.TB_LightZDir.Scroll += new System.EventHandler(this.TB_LightZDir_Scroll);
            // 
            // label24
            // 
            this.label24.AutoSize = true;
            this.label24.ForeColor = System.Drawing.SystemColors.Desktop;
            this.label24.Location = new System.Drawing.Point(6, 122);
            this.label24.Name = "label24";
            this.label24.Size = new System.Drawing.Size(40, 17);
            this.label24.TabIndex = 6;
            this.label24.Text = "Y Dir";
            // 
            // TB_LightYDir
            // 
            this.TB_LightYDir.Location = new System.Drawing.Point(13, 141);
            this.TB_LightYDir.Maximum = 100;
            this.TB_LightYDir.Minimum = -100;
            this.TB_LightYDir.Name = "TB_LightYDir";
            this.TB_LightYDir.Size = new System.Drawing.Size(244, 45);
            this.TB_LightYDir.TabIndex = 5;
            this.TB_LightYDir.Scroll += new System.EventHandler(this.TB_LightYDir_Scroll);
            // 
            // label23
            // 
            this.label23.AutoSize = true;
            this.label23.ForeColor = System.Drawing.SystemColors.Desktop;
            this.label23.Location = new System.Drawing.Point(7, 72);
            this.label23.Name = "label23";
            this.label23.Size = new System.Drawing.Size(40, 17);
            this.label23.TabIndex = 4;
            this.label23.Text = "X Dir";
            // 
            // TB_LightXDir
            // 
            this.TB_LightXDir.Location = new System.Drawing.Point(14, 91);
            this.TB_LightXDir.Maximum = 100;
            this.TB_LightXDir.Minimum = -100;
            this.TB_LightXDir.Name = "TB_LightXDir";
            this.TB_LightXDir.Size = new System.Drawing.Size(244, 45);
            this.TB_LightXDir.TabIndex = 3;
            this.TB_LightXDir.Scroll += new System.EventHandler(this.TB_LightXDir_Scroll);
            // 
            // button1
            // 
            this.button1.ForeColor = System.Drawing.Color.Purple;
            this.button1.Location = new System.Drawing.Point(5, 459);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(277, 111);
            this.button1.TabIndex = 2;
            this.button1.Text = "Light Component 추가";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.LightButton_Click);
            // 
            // label22
            // 
            this.label22.AutoSize = true;
            this.label22.ForeColor = System.Drawing.Color.Black;
            this.label22.Location = new System.Drawing.Point(4, 36);
            this.label22.Name = "label22";
            this.label22.Size = new System.Drawing.Size(70, 17);
            this.label22.TabIndex = 1;
            this.label22.Text = "LightType";
            // 
            // CB_LightType
            // 
            this.CB_LightType.BackColor = System.Drawing.SystemColors.Control;
            this.CB_LightType.FormattingEnabled = true;
            this.CB_LightType.Items.AddRange(new object[] {
            "Direction",
            "Point",
            "Spot"});
            this.CB_LightType.Location = new System.Drawing.Point(80, 34);
            this.CB_LightType.Name = "CB_LightType";
            this.CB_LightType.Size = new System.Drawing.Size(178, 25);
            this.CB_LightType.TabIndex = 0;
            this.CB_LightType.SelectedIndexChanged += new System.EventHandler(this.CB_LightType_SelectedIndexChanged);
            // 
            // NavigationPage
            // 
            this.NavigationPage.BackColor = System.Drawing.SystemColors.Control;
            this.NavigationPage.Location = new System.Drawing.Point(4, 48);
            this.NavigationPage.Name = "NavigationPage";
            this.NavigationPage.Size = new System.Drawing.Size(287, 746);
            this.NavigationPage.TabIndex = 6;
            this.NavigationPage.Text = "Navigation";
            // 
            // LB_MeshList
            // 
            this.LB_MeshList.BackColor = System.Drawing.SystemColors.Control;
            this.LB_MeshList.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.LB_MeshList.FormattingEnabled = true;
            this.LB_MeshList.HorizontalScrollbar = true;
            this.LB_MeshList.ItemHeight = 17;
            this.LB_MeshList.Location = new System.Drawing.Point(11, 46);
            this.LB_MeshList.Name = "LB_MeshList";
            this.LB_MeshList.Size = new System.Drawing.Size(124, 225);
            this.LB_MeshList.TabIndex = 1;
            this.LB_MeshList.Click += new System.EventHandler(this.BasicMeshClick);
            // 
            // GB_Transform
            // 
            this.GB_Transform.Controls.Add(this.RotateZ);
            this.GB_Transform.Controls.Add(this.Label_RotZ);
            this.GB_Transform.Controls.Add(this.RotateY);
            this.GB_Transform.Controls.Add(this.Label_RotX);
            this.GB_Transform.Controls.Add(this.Label_RotY);
            this.GB_Transform.Controls.Add(this.RotateX);
            this.GB_Transform.Controls.Add(this.Label_Rotate);
            this.GB_Transform.Controls.Add(this.ScaleZ);
            this.GB_Transform.Controls.Add(this.Label_ScaleZ);
            this.GB_Transform.Controls.Add(this.ScaleY);
            this.GB_Transform.Controls.Add(this.Label_ScaleX);
            this.GB_Transform.Controls.Add(this.Label_ScaleY);
            this.GB_Transform.Controls.Add(this.ScaleX);
            this.GB_Transform.Controls.Add(this.Label_Scale);
            this.GB_Transform.Controls.Add(this.PositionZ);
            this.GB_Transform.Controls.Add(this.Label_WorldPosition);
            this.GB_Transform.Controls.Add(this.Label_PosZ);
            this.GB_Transform.Controls.Add(this.PositionY);
            this.GB_Transform.Controls.Add(this.Label_PosX);
            this.GB_Transform.Controls.Add(this.Label_PosY);
            this.GB_Transform.Controls.Add(this.PositionX);
            this.GB_Transform.Font = new System.Drawing.Font("맑은 고딕", 10F, System.Drawing.FontStyle.Bold);
            this.GB_Transform.ForeColor = System.Drawing.Color.Red;
            this.GB_Transform.Location = new System.Drawing.Point(1313, 12);
            this.GB_Transform.Name = "GB_Transform";
            this.GB_Transform.Size = new System.Drawing.Size(291, 186);
            this.GB_Transform.TabIndex = 0;
            this.GB_Transform.TabStop = false;
            this.GB_Transform.Text = "World Transform";
            // 
            // RotateZ
            // 
            this.RotateZ.BackColor = System.Drawing.SystemColors.Control;
            this.RotateZ.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.RotateZ.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.RotateZ.Location = new System.Drawing.Point(214, 150);
            this.RotateZ.Name = "RotateZ";
            this.RotateZ.Size = new System.Drawing.Size(64, 25);
            this.RotateZ.TabIndex = 27;
            this.RotateZ.TextChanged += new System.EventHandler(this.ChangeRotate);
            this.RotateZ.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.KeyPressCheck);
            // 
            // Label_RotZ
            // 
            this.Label_RotZ.AutoSize = true;
            this.Label_RotZ.Font = new System.Drawing.Font("맑은 고딕", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.Label_RotZ.ForeColor = System.Drawing.Color.Black;
            this.Label_RotZ.Location = new System.Drawing.Point(199, 157);
            this.Label_RotZ.Name = "Label_RotZ";
            this.Label_RotZ.Size = new System.Drawing.Size(14, 13);
            this.Label_RotZ.TabIndex = 26;
            this.Label_RotZ.Text = "Z";
            // 
            // RotateY
            // 
            this.RotateY.BackColor = System.Drawing.SystemColors.Control;
            this.RotateY.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.RotateY.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.RotateY.Location = new System.Drawing.Point(127, 150);
            this.RotateY.Name = "RotateY";
            this.RotateY.Size = new System.Drawing.Size(64, 25);
            this.RotateY.TabIndex = 25;
            this.RotateY.TextChanged += new System.EventHandler(this.ChangeRotate);
            this.RotateY.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.KeyPressCheck);
            // 
            // Label_RotX
            // 
            this.Label_RotX.AutoSize = true;
            this.Label_RotX.Font = new System.Drawing.Font("맑은 고딕", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.Label_RotX.ForeColor = System.Drawing.Color.Black;
            this.Label_RotX.Location = new System.Drawing.Point(23, 157);
            this.Label_RotX.Name = "Label_RotX";
            this.Label_RotX.Size = new System.Drawing.Size(14, 13);
            this.Label_RotX.TabIndex = 22;
            this.Label_RotX.Text = "X";
            // 
            // Label_RotY
            // 
            this.Label_RotY.AutoSize = true;
            this.Label_RotY.Font = new System.Drawing.Font("맑은 고딕", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.Label_RotY.ForeColor = System.Drawing.Color.Black;
            this.Label_RotY.Location = new System.Drawing.Point(110, 157);
            this.Label_RotY.Name = "Label_RotY";
            this.Label_RotY.Size = new System.Drawing.Size(14, 13);
            this.Label_RotY.TabIndex = 24;
            this.Label_RotY.Text = "Y";
            // 
            // RotateX
            // 
            this.RotateX.BackColor = System.Drawing.SystemColors.Control;
            this.RotateX.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.RotateX.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.RotateX.Location = new System.Drawing.Point(40, 151);
            this.RotateX.Name = "RotateX";
            this.RotateX.Size = new System.Drawing.Size(64, 25);
            this.RotateX.TabIndex = 23;
            this.RotateX.TextChanged += new System.EventHandler(this.ChangeRotate);
            this.RotateX.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.KeyPressCheck);
            // 
            // Label_Rotate
            // 
            this.Label_Rotate.AutoSize = true;
            this.Label_Rotate.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.Label_Rotate.ForeColor = System.Drawing.Color.Black;
            this.Label_Rotate.Location = new System.Drawing.Point(11, 130);
            this.Label_Rotate.Name = "Label_Rotate";
            this.Label_Rotate.Size = new System.Drawing.Size(91, 17);
            this.Label_Rotate.TabIndex = 21;
            this.Label_Rotate.Text = "World Rotate";
            // 
            // ScaleZ
            // 
            this.ScaleZ.BackColor = System.Drawing.SystemColors.Control;
            this.ScaleZ.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.ScaleZ.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.ScaleZ.Location = new System.Drawing.Point(214, 98);
            this.ScaleZ.Name = "ScaleZ";
            this.ScaleZ.Size = new System.Drawing.Size(64, 25);
            this.ScaleZ.TabIndex = 20;
            this.ScaleZ.TextChanged += new System.EventHandler(this.ChangeScale);
            this.ScaleZ.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.KeyPressCheck);
            // 
            // Label_ScaleZ
            // 
            this.Label_ScaleZ.AutoSize = true;
            this.Label_ScaleZ.Font = new System.Drawing.Font("맑은 고딕", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.Label_ScaleZ.ForeColor = System.Drawing.Color.Black;
            this.Label_ScaleZ.Location = new System.Drawing.Point(199, 104);
            this.Label_ScaleZ.Name = "Label_ScaleZ";
            this.Label_ScaleZ.Size = new System.Drawing.Size(14, 13);
            this.Label_ScaleZ.TabIndex = 19;
            this.Label_ScaleZ.Text = "Z";
            // 
            // ScaleY
            // 
            this.ScaleY.BackColor = System.Drawing.SystemColors.Control;
            this.ScaleY.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.ScaleY.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.ScaleY.Location = new System.Drawing.Point(127, 98);
            this.ScaleY.Name = "ScaleY";
            this.ScaleY.Size = new System.Drawing.Size(64, 25);
            this.ScaleY.TabIndex = 18;
            this.ScaleY.TextChanged += new System.EventHandler(this.ChangeScale);
            this.ScaleY.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.KeyPressCheck);
            // 
            // Label_ScaleX
            // 
            this.Label_ScaleX.AutoSize = true;
            this.Label_ScaleX.Font = new System.Drawing.Font("맑은 고딕", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.Label_ScaleX.ForeColor = System.Drawing.Color.Black;
            this.Label_ScaleX.Location = new System.Drawing.Point(23, 106);
            this.Label_ScaleX.Name = "Label_ScaleX";
            this.Label_ScaleX.Size = new System.Drawing.Size(14, 13);
            this.Label_ScaleX.TabIndex = 15;
            this.Label_ScaleX.Text = "X";
            // 
            // Label_ScaleY
            // 
            this.Label_ScaleY.AutoSize = true;
            this.Label_ScaleY.Font = new System.Drawing.Font("맑은 고딕", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.Label_ScaleY.ForeColor = System.Drawing.Color.Black;
            this.Label_ScaleY.Location = new System.Drawing.Point(110, 104);
            this.Label_ScaleY.Name = "Label_ScaleY";
            this.Label_ScaleY.Size = new System.Drawing.Size(14, 13);
            this.Label_ScaleY.TabIndex = 17;
            this.Label_ScaleY.Text = "Y";
            // 
            // ScaleX
            // 
            this.ScaleX.BackColor = System.Drawing.SystemColors.Control;
            this.ScaleX.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.ScaleX.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.ScaleX.Location = new System.Drawing.Point(40, 99);
            this.ScaleX.Name = "ScaleX";
            this.ScaleX.Size = new System.Drawing.Size(64, 25);
            this.ScaleX.TabIndex = 16;
            this.ScaleX.TextChanged += new System.EventHandler(this.ChangeScale);
            this.ScaleX.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.KeyPressCheck);
            // 
            // Label_Scale
            // 
            this.Label_Scale.AutoSize = true;
            this.Label_Scale.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.Label_Scale.ForeColor = System.Drawing.Color.Black;
            this.Label_Scale.Location = new System.Drawing.Point(11, 77);
            this.Label_Scale.Name = "Label_Scale";
            this.Label_Scale.Size = new System.Drawing.Size(82, 17);
            this.Label_Scale.TabIndex = 14;
            this.Label_Scale.Text = "World Scale";
            // 
            // PositionZ
            // 
            this.PositionZ.BackColor = System.Drawing.SystemColors.Control;
            this.PositionZ.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.PositionZ.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.PositionZ.Location = new System.Drawing.Point(214, 45);
            this.PositionZ.Name = "PositionZ";
            this.PositionZ.Size = new System.Drawing.Size(64, 25);
            this.PositionZ.TabIndex = 13;
            this.PositionZ.TextChanged += new System.EventHandler(this.ChangePosition);
            this.PositionZ.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.KeyPressCheck);
            // 
            // Label_WorldPosition
            // 
            this.Label_WorldPosition.AutoSize = true;
            this.Label_WorldPosition.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.Label_WorldPosition.ForeColor = System.Drawing.Color.Black;
            this.Label_WorldPosition.Location = new System.Drawing.Point(11, 24);
            this.Label_WorldPosition.Name = "Label_WorldPosition";
            this.Label_WorldPosition.Size = new System.Drawing.Size(102, 17);
            this.Label_WorldPosition.TabIndex = 0;
            this.Label_WorldPosition.Text = "World Position";
            // 
            // Label_PosZ
            // 
            this.Label_PosZ.AutoSize = true;
            this.Label_PosZ.Font = new System.Drawing.Font("맑은 고딕", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.Label_PosZ.ForeColor = System.Drawing.Color.Black;
            this.Label_PosZ.Location = new System.Drawing.Point(199, 51);
            this.Label_PosZ.Name = "Label_PosZ";
            this.Label_PosZ.Size = new System.Drawing.Size(14, 13);
            this.Label_PosZ.TabIndex = 12;
            this.Label_PosZ.Text = "Z";
            // 
            // PositionY
            // 
            this.PositionY.BackColor = System.Drawing.SystemColors.Control;
            this.PositionY.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.PositionY.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.PositionY.Location = new System.Drawing.Point(127, 45);
            this.PositionY.Name = "PositionY";
            this.PositionY.Size = new System.Drawing.Size(64, 25);
            this.PositionY.TabIndex = 11;
            this.PositionY.TextChanged += new System.EventHandler(this.ChangePosition);
            this.PositionY.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.KeyPressCheck);
            // 
            // Label_PosX
            // 
            this.Label_PosX.AutoSize = true;
            this.Label_PosX.Font = new System.Drawing.Font("맑은 고딕", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.Label_PosX.ForeColor = System.Drawing.Color.Black;
            this.Label_PosX.Location = new System.Drawing.Point(23, 51);
            this.Label_PosX.Name = "Label_PosX";
            this.Label_PosX.Size = new System.Drawing.Size(14, 13);
            this.Label_PosX.TabIndex = 1;
            this.Label_PosX.Text = "X";
            // 
            // Label_PosY
            // 
            this.Label_PosY.AutoSize = true;
            this.Label_PosY.Font = new System.Drawing.Font("맑은 고딕", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.Label_PosY.ForeColor = System.Drawing.Color.Black;
            this.Label_PosY.Location = new System.Drawing.Point(110, 51);
            this.Label_PosY.Name = "Label_PosY";
            this.Label_PosY.Size = new System.Drawing.Size(14, 13);
            this.Label_PosY.TabIndex = 10;
            this.Label_PosY.Text = "Y";
            // 
            // PositionX
            // 
            this.PositionX.BackColor = System.Drawing.SystemColors.Control;
            this.PositionX.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.PositionX.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.PositionX.Location = new System.Drawing.Point(40, 45);
            this.PositionX.Name = "PositionX";
            this.PositionX.Size = new System.Drawing.Size(64, 25);
            this.PositionX.TabIndex = 2;
            this.PositionX.TextChanged += new System.EventHandler(this.ChangePosition);
            this.PositionX.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.KeyPressCheck);
            // 
            // GB_Hierachy
            // 
            this.GB_Hierachy.Controls.Add(this.label11);
            this.GB_Hierachy.Controls.Add(this.CB_LayerList);
            this.GB_Hierachy.Controls.Add(this.label10);
            this.GB_Hierachy.Controls.Add(this.TB_ObjectTag);
            this.GB_Hierachy.Controls.Add(this.Btn_ObjectDelete);
            this.GB_Hierachy.Controls.Add(this.LB_ObjectList);
            this.GB_Hierachy.Controls.Add(this.Btn_ObjectCreate);
            this.GB_Hierachy.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.GB_Hierachy.ForeColor = System.Drawing.Color.Red;
            this.GB_Hierachy.Location = new System.Drawing.Point(11, 99);
            this.GB_Hierachy.Name = "GB_Hierachy";
            this.GB_Hierachy.Size = new System.Drawing.Size(206, 722);
            this.GB_Hierachy.TabIndex = 9;
            this.GB_Hierachy.TabStop = false;
            this.GB_Hierachy.Text = "Hierachy";
            // 
            // Btn_ObjectDelete
            // 
            this.Btn_ObjectDelete.Font = new System.Drawing.Font("맑은 고딕", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.Btn_ObjectDelete.ForeColor = System.Drawing.Color.Black;
            this.Btn_ObjectDelete.Location = new System.Drawing.Point(105, 639);
            this.Btn_ObjectDelete.Name = "Btn_ObjectDelete";
            this.Btn_ObjectDelete.Size = new System.Drawing.Size(95, 77);
            this.Btn_ObjectDelete.TabIndex = 6;
            this.Btn_ObjectDelete.Text = "Delete";
            this.Btn_ObjectDelete.UseVisualStyleBackColor = true;
            this.Btn_ObjectDelete.Click += new System.EventHandler(this.Btn_DeleteObject);
            // 
            // LB_ObjectList
            // 
            this.LB_ObjectList.BackColor = System.Drawing.SystemColors.Control;
            this.LB_ObjectList.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.LB_ObjectList.ForeColor = System.Drawing.SystemColors.ControlText;
            this.LB_ObjectList.FormattingEnabled = true;
            this.LB_ObjectList.ItemHeight = 17;
            this.LB_ObjectList.Location = new System.Drawing.Point(5, 24);
            this.LB_ObjectList.Name = "LB_ObjectList";
            this.LB_ObjectList.Size = new System.Drawing.Size(195, 531);
            this.LB_ObjectList.TabIndex = 0;
            this.LB_ObjectList.SelectedIndexChanged += new System.EventHandler(this.ObjectList_ChangeSelectedItem);
            this.LB_ObjectList.KeyDown += new System.Windows.Forms.KeyEventHandler(this.KeyDown_DeleteObject);
            // 
            // Btn_FileSave
            // 
            this.Btn_FileSave.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.Btn_FileSave.Location = new System.Drawing.Point(11, 12);
            this.Btn_FileSave.Name = "Btn_FileSave";
            this.Btn_FileSave.Size = new System.Drawing.Size(99, 39);
            this.Btn_FileSave.TabIndex = 10;
            this.Btn_FileSave.Text = "File Save";
            this.Btn_FileSave.UseVisualStyleBackColor = true;
            this.Btn_FileSave.Click += new System.EventHandler(this.FileSave_Click);
            // 
            // Btn_FileLoad
            // 
            this.Btn_FileLoad.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.Btn_FileLoad.Location = new System.Drawing.Point(11, 54);
            this.Btn_FileLoad.Name = "Btn_FileLoad";
            this.Btn_FileLoad.Size = new System.Drawing.Size(99, 39);
            this.Btn_FileLoad.TabIndex = 11;
            this.Btn_FileLoad.Text = "File Load";
            this.Btn_FileLoad.UseVisualStyleBackColor = true;
            this.Btn_FileLoad.Click += new System.EventHandler(this.FileLoad_Click);
            // 
            // LB_LogMessage
            // 
            this.LB_LogMessage.BackColor = System.Drawing.SystemColors.Control;
            this.LB_LogMessage.FormattingEnabled = true;
            this.LB_LogMessage.ItemHeight = 12;
            this.LB_LogMessage.Location = new System.Drawing.Point(223, 685);
            this.LB_LogMessage.Name = "LB_LogMessage";
            this.LB_LogMessage.Size = new System.Drawing.Size(655, 136);
            this.LB_LogMessage.TabIndex = 12;
            // 
            // Btn_AddComponent
            // 
            this.Btn_AddComponent.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.Btn_AddComponent.ForeColor = System.Drawing.Color.Black;
            this.Btn_AddComponent.Location = new System.Drawing.Point(10, 292);
            this.Btn_AddComponent.Name = "Btn_AddComponent";
            this.Btn_AddComponent.Size = new System.Drawing.Size(139, 31);
            this.Btn_AddComponent.TabIndex = 3;
            this.Btn_AddComponent.Text = "Render Component";
            this.Btn_AddComponent.UseVisualStyleBackColor = true;
            this.Btn_AddComponent.Click += new System.EventHandler(this.AddRenderComponent);
            // 
            // Label_BasicMesh
            // 
            this.Label_BasicMesh.AutoSize = true;
            this.Label_BasicMesh.ForeColor = System.Drawing.Color.Black;
            this.Label_BasicMesh.Location = new System.Drawing.Point(10, 21);
            this.Label_BasicMesh.Name = "Label_BasicMesh";
            this.Label_BasicMesh.Size = new System.Drawing.Size(73, 17);
            this.Label_BasicMesh.TabIndex = 5;
            this.Label_BasicMesh.Text = "BasicMesh";
            // 
            // Btn_AddMesh
            // 
            this.Btn_AddMesh.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.Btn_AddMesh.ForeColor = System.Drawing.Color.Black;
            this.Btn_AddMesh.Location = new System.Drawing.Point(160, 292);
            this.Btn_AddMesh.Name = "Btn_AddMesh";
            this.Btn_AddMesh.Size = new System.Drawing.Size(116, 31);
            this.Btn_AddMesh.TabIndex = 2;
            this.Btn_AddMesh.Text = "Set Mesh";
            this.Btn_AddMesh.UseVisualStyleBackColor = true;
            this.Btn_AddMesh.Click += new System.EventHandler(this.SetMesh_Click);
            // 
            // GB_Renderer
            // 
            this.GB_Renderer.Controls.Add(this.CB_isDebugTarget);
            this.GB_Renderer.Controls.Add(this.FileMesh);
            this.GB_Renderer.Controls.Add(this.LB_FileMesh);
            this.GB_Renderer.Controls.Add(this.LB_MeshList);
            this.GB_Renderer.Controls.Add(this.Btn_AddMesh);
            this.GB_Renderer.Controls.Add(this.Label_BasicMesh);
            this.GB_Renderer.Controls.Add(this.Btn_AddComponent);
            this.GB_Renderer.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.GB_Renderer.ForeColor = System.Drawing.Color.Red;
            this.GB_Renderer.Location = new System.Drawing.Point(1315, 415);
            this.GB_Renderer.Name = "GB_Renderer";
            this.GB_Renderer.Size = new System.Drawing.Size(291, 406);
            this.GB_Renderer.TabIndex = 0;
            this.GB_Renderer.TabStop = false;
            this.GB_Renderer.Text = "Renderer";
            // 
            // CB_isDebugTarget
            // 
            this.CB_isDebugTarget.AutoSize = true;
            this.CB_isDebugTarget.ForeColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.CB_isDebugTarget.Location = new System.Drawing.Point(10, 352);
            this.CB_isDebugTarget.Name = "CB_isDebugTarget";
            this.CB_isDebugTarget.Size = new System.Drawing.Size(105, 21);
            this.CB_isDebugTarget.TabIndex = 8;
            this.CB_isDebugTarget.Text = "TargetOnOff";
            this.CB_isDebugTarget.UseVisualStyleBackColor = true;
            this.CB_isDebugTarget.CheckedChanged += new System.EventHandler(this.CB_isDebugTarget_CheckedChanged);
            // 
            // FileMesh
            // 
            this.FileMesh.AutoSize = true;
            this.FileMesh.ForeColor = System.Drawing.Color.Black;
            this.FileMesh.Location = new System.Drawing.Point(145, 22);
            this.FileMesh.Name = "FileMesh";
            this.FileMesh.Size = new System.Drawing.Size(64, 17);
            this.FileMesh.TabIndex = 7;
            this.FileMesh.Text = "FileMesh";
            // 
            // LB_FileMesh
            // 
            this.LB_FileMesh.BackColor = System.Drawing.SystemColors.Control;
            this.LB_FileMesh.FormattingEnabled = true;
            this.LB_FileMesh.ItemHeight = 17;
            this.LB_FileMesh.Location = new System.Drawing.Point(146, 47);
            this.LB_FileMesh.Name = "LB_FileMesh";
            this.LB_FileMesh.Size = new System.Drawing.Size(135, 225);
            this.LB_FileMesh.TabIndex = 6;
            this.LB_FileMesh.Click += new System.EventHandler(this.FileMeshClick);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.ForeColor = System.Drawing.Color.Black;
            this.label1.Location = new System.Drawing.Point(886, 679);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(85, 12);
            this.label1.TabIndex = 7;
            this.label1.Text = "Animation List";
            // 
            // LB_AniList
            // 
            this.LB_AniList.BackColor = System.Drawing.SystemColors.Control;
            this.LB_AniList.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.LB_AniList.FormattingEnabled = true;
            this.LB_AniList.HorizontalScrollbar = true;
            this.LB_AniList.ItemHeight = 17;
            this.LB_AniList.Location = new System.Drawing.Point(888, 698);
            this.LB_AniList.Name = "LB_AniList";
            this.LB_AniList.Size = new System.Drawing.Size(410, 123);
            this.LB_AniList.TabIndex = 6;
            // 
            // Btn_AllClear
            // 
            this.Btn_AllClear.Image = ((System.Drawing.Image)(resources.GetObject("Btn_AllClear.Image")));
            this.Btn_AllClear.Location = new System.Drawing.Point(116, 12);
            this.Btn_AllClear.Name = "Btn_AllClear";
            this.Btn_AllClear.Size = new System.Drawing.Size(101, 81);
            this.Btn_AllClear.TabIndex = 13;
            this.Btn_AllClear.UseVisualStyleBackColor = true;
            this.Btn_AllClear.MouseDown += new System.Windows.Forms.MouseEventHandler(this.ObjectAllClear);
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.textBox1);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.textBox2);
            this.groupBox1.Controls.Add(this.label3);
            this.groupBox1.Controls.Add(this.label4);
            this.groupBox1.Controls.Add(this.textBox3);
            this.groupBox1.Controls.Add(this.label5);
            this.groupBox1.Controls.Add(this.textBox4);
            this.groupBox1.Controls.Add(this.label6);
            this.groupBox1.Controls.Add(this.textBox5);
            this.groupBox1.Controls.Add(this.label7);
            this.groupBox1.Controls.Add(this.label8);
            this.groupBox1.Controls.Add(this.textBox6);
            this.groupBox1.Controls.Add(this.label9);
            this.groupBox1.Controls.Add(this.textBox7);
            this.groupBox1.Controls.Add(this.label17);
            this.groupBox1.Controls.Add(this.label18);
            this.groupBox1.Controls.Add(this.textBox8);
            this.groupBox1.Controls.Add(this.label19);
            this.groupBox1.Controls.Add(this.label20);
            this.groupBox1.Controls.Add(this.textBox9);
            this.groupBox1.Font = new System.Drawing.Font("맑은 고딕", 10F, System.Drawing.FontStyle.Bold);
            this.groupBox1.ForeColor = System.Drawing.Color.Red;
            this.groupBox1.Location = new System.Drawing.Point(1313, 218);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(294, 186);
            this.groupBox1.TabIndex = 28;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Local Transform";
            // 
            // textBox1
            // 
            this.textBox1.BackColor = System.Drawing.SystemColors.Control;
            this.textBox1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.textBox1.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.textBox1.Location = new System.Drawing.Point(212, 150);
            this.textBox1.Name = "textBox1";
            this.textBox1.Size = new System.Drawing.Size(64, 25);
            this.textBox1.TabIndex = 27;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("맑은 고딕", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.label2.ForeColor = System.Drawing.Color.Black;
            this.label2.Location = new System.Drawing.Point(197, 157);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(14, 13);
            this.label2.TabIndex = 26;
            this.label2.Text = "Z";
            // 
            // textBox2
            // 
            this.textBox2.BackColor = System.Drawing.SystemColors.Control;
            this.textBox2.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.textBox2.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.textBox2.Location = new System.Drawing.Point(126, 150);
            this.textBox2.Name = "textBox2";
            this.textBox2.Size = new System.Drawing.Size(64, 25);
            this.textBox2.TabIndex = 25;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Font = new System.Drawing.Font("맑은 고딕", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.label3.ForeColor = System.Drawing.Color.Black;
            this.label3.Location = new System.Drawing.Point(21, 157);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(14, 13);
            this.label3.TabIndex = 22;
            this.label3.Text = "X";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Font = new System.Drawing.Font("맑은 고딕", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.label4.ForeColor = System.Drawing.Color.Black;
            this.label4.Location = new System.Drawing.Point(109, 157);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(14, 13);
            this.label4.TabIndex = 24;
            this.label4.Text = "Y";
            // 
            // textBox3
            // 
            this.textBox3.BackColor = System.Drawing.SystemColors.Control;
            this.textBox3.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.textBox3.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.textBox3.Location = new System.Drawing.Point(38, 151);
            this.textBox3.Name = "textBox3";
            this.textBox3.Size = new System.Drawing.Size(64, 25);
            this.textBox3.TabIndex = 23;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.label5.ForeColor = System.Drawing.Color.Black;
            this.label5.Location = new System.Drawing.Point(11, 130);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(85, 17);
            this.label5.TabIndex = 21;
            this.label5.Text = "Local Rotate";
            // 
            // textBox4
            // 
            this.textBox4.BackColor = System.Drawing.SystemColors.Control;
            this.textBox4.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.textBox4.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.textBox4.Location = new System.Drawing.Point(212, 98);
            this.textBox4.Name = "textBox4";
            this.textBox4.Size = new System.Drawing.Size(64, 25);
            this.textBox4.TabIndex = 20;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Font = new System.Drawing.Font("맑은 고딕", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.label6.ForeColor = System.Drawing.Color.Black;
            this.label6.Location = new System.Drawing.Point(197, 104);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(14, 13);
            this.label6.TabIndex = 19;
            this.label6.Text = "Z";
            // 
            // textBox5
            // 
            this.textBox5.BackColor = System.Drawing.SystemColors.Control;
            this.textBox5.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.textBox5.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.textBox5.Location = new System.Drawing.Point(126, 98);
            this.textBox5.Name = "textBox5";
            this.textBox5.Size = new System.Drawing.Size(64, 25);
            this.textBox5.TabIndex = 18;
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Font = new System.Drawing.Font("맑은 고딕", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.label7.ForeColor = System.Drawing.Color.Black;
            this.label7.Location = new System.Drawing.Point(21, 106);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(14, 13);
            this.label7.TabIndex = 15;
            this.label7.Text = "X";
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Font = new System.Drawing.Font("맑은 고딕", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.label8.ForeColor = System.Drawing.Color.Black;
            this.label8.Location = new System.Drawing.Point(109, 104);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(14, 13);
            this.label8.TabIndex = 17;
            this.label8.Text = "Y";
            // 
            // textBox6
            // 
            this.textBox6.BackColor = System.Drawing.SystemColors.Control;
            this.textBox6.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.textBox6.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.textBox6.Location = new System.Drawing.Point(38, 99);
            this.textBox6.Name = "textBox6";
            this.textBox6.Size = new System.Drawing.Size(64, 25);
            this.textBox6.TabIndex = 16;
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.label9.ForeColor = System.Drawing.Color.Black;
            this.label9.Location = new System.Drawing.Point(11, 77);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(76, 17);
            this.label9.TabIndex = 14;
            this.label9.Text = "Local Scale";
            // 
            // textBox7
            // 
            this.textBox7.BackColor = System.Drawing.SystemColors.Control;
            this.textBox7.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.textBox7.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.textBox7.Location = new System.Drawing.Point(212, 45);
            this.textBox7.Name = "textBox7";
            this.textBox7.Size = new System.Drawing.Size(64, 25);
            this.textBox7.TabIndex = 13;
            // 
            // label17
            // 
            this.label17.AutoSize = true;
            this.label17.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.label17.ForeColor = System.Drawing.Color.Black;
            this.label17.Location = new System.Drawing.Point(11, 24);
            this.label17.Name = "label17";
            this.label17.Size = new System.Drawing.Size(96, 17);
            this.label17.TabIndex = 0;
            this.label17.Text = "Local Position";
            // 
            // label18
            // 
            this.label18.AutoSize = true;
            this.label18.Font = new System.Drawing.Font("맑은 고딕", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.label18.ForeColor = System.Drawing.Color.Black;
            this.label18.Location = new System.Drawing.Point(197, 51);
            this.label18.Name = "label18";
            this.label18.Size = new System.Drawing.Size(14, 13);
            this.label18.TabIndex = 12;
            this.label18.Text = "Z";
            // 
            // textBox8
            // 
            this.textBox8.BackColor = System.Drawing.SystemColors.Control;
            this.textBox8.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.textBox8.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.textBox8.Location = new System.Drawing.Point(126, 45);
            this.textBox8.Name = "textBox8";
            this.textBox8.Size = new System.Drawing.Size(64, 25);
            this.textBox8.TabIndex = 11;
            // 
            // label19
            // 
            this.label19.AutoSize = true;
            this.label19.Font = new System.Drawing.Font("맑은 고딕", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.label19.ForeColor = System.Drawing.Color.Black;
            this.label19.Location = new System.Drawing.Point(21, 51);
            this.label19.Name = "label19";
            this.label19.Size = new System.Drawing.Size(14, 13);
            this.label19.TabIndex = 1;
            this.label19.Text = "X";
            // 
            // label20
            // 
            this.label20.AutoSize = true;
            this.label20.Font = new System.Drawing.Font("맑은 고딕", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.label20.ForeColor = System.Drawing.Color.Black;
            this.label20.Location = new System.Drawing.Point(109, 51);
            this.label20.Name = "label20";
            this.label20.Size = new System.Drawing.Size(14, 13);
            this.label20.TabIndex = 10;
            this.label20.Text = "Y";
            // 
            // textBox9
            // 
            this.textBox9.BackColor = System.Drawing.SystemColors.Control;
            this.textBox9.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.textBox9.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.textBox9.Location = new System.Drawing.Point(38, 45);
            this.textBox9.Name = "textBox9";
            this.textBox9.Size = new System.Drawing.Size(64, 25);
            this.textBox9.TabIndex = 2;
            // 
            // EditorForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.SystemColors.Control;
            this.ClientSize = new System.Drawing.Size(1911, 827);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.Btn_AllClear);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.GB_Hierachy);
            this.Controls.Add(this.LB_AniList);
            this.Controls.Add(this.LB_LogMessage);
            this.Controls.Add(this.Btn_FileLoad);
            this.Controls.Add(this.Btn_FileSave);
            this.Controls.Add(this.GB_Transform);
            this.Controls.Add(this.Inspector);
            this.Controls.Add(this.GB_Scene);
            this.Controls.Add(this.GB_Renderer);
            this.ForeColor = System.Drawing.SystemColors.WindowText;
            this.Name = "EditorForm";
            this.Text = "MotherFatherChildren";
            this.TransparencyKey = System.Drawing.Color.Gray;
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.Delete);
            this.Load += new System.EventHandler(this.EditorForm_Load);
            ((System.ComponentModel.ISupportInitialize)(this.PB_DeviceWindow)).EndInit();
            this.GB_Scene.ResumeLayout(false);
            this.Inspector.ResumeLayout(false);
            this.InspectorTab.ResumeLayout(false);
            this.Animation.ResumeLayout(false);
            this.Animation.PerformLayout();
            this.LightPage.ResumeLayout(false);
            this.LightPage.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.TB_Range)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.TB_LightZDir)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.TB_LightYDir)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.TB_LightXDir)).EndInit();
            this.GB_Transform.ResumeLayout(false);
            this.GB_Transform.PerformLayout();
            this.GB_Hierachy.ResumeLayout(false);
            this.GB_Hierachy.PerformLayout();
            this.GB_Renderer.ResumeLayout(false);
            this.GB_Renderer.PerformLayout();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.PictureBox PB_DeviceWindow;
        private System.Windows.Forms.GroupBox GB_Scene;
        private System.Windows.Forms.Button Btn_ObjectCreate;
        private System.Windows.Forms.TreeView treeView1;
        private System.Windows.Forms.TextBox TB_ObjectTag;
        private System.Windows.Forms.GroupBox Inspector;
        private System.Windows.Forms.GroupBox GB_Hierachy;
        private System.Windows.Forms.ListBox LB_ObjectList;
        private System.Windows.Forms.GroupBox GB_Transform;
        private System.Windows.Forms.Label Label_PosX;
        private System.Windows.Forms.Label Label_PosZ;
        private System.Windows.Forms.TextBox PositionY;
        private System.Windows.Forms.Label Label_PosY;
        private System.Windows.Forms.TextBox PositionX;
        private System.Windows.Forms.TextBox PositionZ;
        private System.Windows.Forms.ListBox LB_MeshList;
        private System.Windows.Forms.Button Btn_FileSave;
        private System.Windows.Forms.Button Btn_FileLoad;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.ComboBox CB_LayerList;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.TabPage Animation;
        private System.Windows.Forms.TabPage Component;
        private System.Windows.Forms.TabPage Effect;
        private System.Windows.Forms.TabControl InspectorTab;
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
        private System.Windows.Forms.Button DeleteClipBtn;
        private System.Windows.Forms.TextBox DeleteClipText;
        private System.Windows.Forms.ListBox LB_LogMessage;
        private System.Windows.Forms.TextBox RotateZ;
        private System.Windows.Forms.Label Label_RotZ;
        private System.Windows.Forms.TextBox RotateY;
        private System.Windows.Forms.Label Label_RotX;
        private System.Windows.Forms.Label Label_RotY;
        private System.Windows.Forms.TextBox RotateX;
        private System.Windows.Forms.Label Label_Rotate;
        private System.Windows.Forms.TextBox ScaleZ;
        private System.Windows.Forms.Label Label_ScaleZ;
        private System.Windows.Forms.TextBox ScaleY;
        private System.Windows.Forms.Label Label_ScaleX;
        private System.Windows.Forms.Label Label_ScaleY;
        private System.Windows.Forms.TextBox ScaleX;
        private System.Windows.Forms.Label Label_Scale;
        private System.Windows.Forms.Label Label_WorldPosition;
        private System.Windows.Forms.Button Btn_AddComponent;
        private System.Windows.Forms.Label Label_BasicMesh;
        private System.Windows.Forms.Button Btn_AddMesh;
        private System.Windows.Forms.GroupBox GB_Renderer;
        private System.Windows.Forms.Button Btn_ObjectDelete;
        private System.Windows.Forms.ListBox LB_AniList;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button Btn_AllClear;
        private System.Windows.Forms.Label FileMesh;
        private System.Windows.Forms.ListBox LB_FileMesh;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox textBox2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox textBox3;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.TextBox textBox4;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.TextBox textBox5;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.TextBox textBox6;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.TextBox textBox7;
        private System.Windows.Forms.Label label17;
        private System.Windows.Forms.Label label18;
        private System.Windows.Forms.TextBox textBox8;
        private System.Windows.Forms.Label label19;
        private System.Windows.Forms.Label label20;
        private System.Windows.Forms.TextBox textBox9;
        private System.Windows.Forms.Label label21;
        private System.Windows.Forms.ListBox LB_BoneList;
        private System.Windows.Forms.TabPage RenderPage;
        private System.Windows.Forms.TabPage LightPage;
        private System.Windows.Forms.TabPage NavigationPage;
        private System.Windows.Forms.Label label22;
        private System.Windows.Forms.ComboBox CB_LightType;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.TrackBar TB_LightXDir;
        private System.Windows.Forms.Label label25;
        private System.Windows.Forms.TrackBar TB_LightZDir;
        private System.Windows.Forms.Label label24;
        private System.Windows.Forms.TrackBar TB_LightYDir;
        private System.Windows.Forms.Label label23;
        private System.Windows.Forms.Label label27;
        private System.Windows.Forms.Label label26;
        private System.Windows.Forms.Label label31;
        private System.Windows.Forms.Button BT_DifColor;
        private System.Windows.Forms.Label label28;
        private System.Windows.Forms.TextBox TB_SpcularR;
        private System.Windows.Forms.TextBox TB_SpcularPower;
        private System.Windows.Forms.Label label33;
        private System.Windows.Forms.Label label32;
        private System.Windows.Forms.Label label30;
        private System.Windows.Forms.Label label29;
        private System.Windows.Forms.TextBox TB_SpcularG;
        private System.Windows.Forms.TextBox TB_SpcularB;
        private System.Windows.Forms.Label label34;
        private System.Windows.Forms.TrackBar TB_Range;
        private System.Windows.Forms.CheckBox CB_WireFrame;
        private System.Windows.Forms.CheckBox CB_isDebugTarget;
    }
}

