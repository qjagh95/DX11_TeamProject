﻿namespace WinFormEditor
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
            this.LightPage = new System.Windows.Forms.TabPage();
            this.TB_RangeValue = new System.Windows.Forms.TextBox();
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
            this.Btn_LightColor = new System.Windows.Forms.Button();
            this.label31 = new System.Windows.Forms.Label();
            this.label26 = new System.Windows.Forms.Label();
            this.Btn_AddLightComponent = new System.Windows.Forms.Button();
            this.label22 = new System.Windows.Forms.Label();
            this.CB_LightType = new System.Windows.Forms.ComboBox();
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
            this.ClipLoad = new System.Windows.Forms.Button();
            this.ClipSave = new System.Windows.Forms.Button();
            this.NavigationPage = new System.Windows.Forms.TabPage();
            this.label38 = new System.Windows.Forms.Label();
            this.TB_TileFlag = new System.Windows.Forms.TextBox();
            this.label37 = new System.Windows.Forms.Label();
            this.TB_NaviIndex = new System.Windows.Forms.TextBox();
            this.label36 = new System.Windows.Forms.Label();
            this.CB_isNaviEditor = new System.Windows.Forms.CheckBox();
            this.TB_NumZ = new System.Windows.Forms.TextBox();
            this.label35 = new System.Windows.Forms.Label();
            this.TB_NumX = new System.Windows.Forms.TextBox();
            this.BT_DeleteNavi = new System.Windows.Forms.Button();
            this.BT_CreateNavi = new System.Windows.Forms.Button();
            this.BT_NaviLoad = new System.Windows.Forms.Button();
            this.BT_NaviSave = new System.Windows.Forms.Button();
            this.Effect = new System.Windows.Forms.TabPage();
            this.RenderPage = new System.Windows.Forms.TabPage();
            this.MshLoad = new System.Windows.Forms.Button();
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
            this.CB_ObjectSave = new System.Windows.Forms.CheckBox();
            this.Btn_ObjectDelete = new System.Windows.Forms.Button();
            this.LB_ObjectList = new System.Windows.Forms.ListBox();
            this.Btn_FileSave = new System.Windows.Forms.Button();
            this.Btn_FileLoad = new System.Windows.Forms.Button();
            this.LB_LogMessage = new System.Windows.Forms.ListBox();
            this.Btn_AddComponent = new System.Windows.Forms.Button();
            this.Label_BasicMesh = new System.Windows.Forms.Label();
            this.Btn_AddMesh = new System.Windows.Forms.Button();
            this.GB_Renderer = new System.Windows.Forms.GroupBox();
            this.FileMesh = new System.Windows.Forms.Label();
            this.CB_TargetChange = new System.Windows.Forms.CheckBox();
            this.LB_FileMesh = new System.Windows.Forms.ListBox();
            this.CB_isDebugTarget = new System.Windows.Forms.CheckBox();
            this.label1 = new System.Windows.Forms.Label();
            this.LB_AniList = new System.Windows.Forms.ListBox();
            this.Btn_AllClear = new System.Windows.Forms.Button();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.LRotateZ = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.LRotateY = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.LRotateX = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.LScaleZ = new System.Windows.Forms.TextBox();
            this.label6 = new System.Windows.Forms.Label();
            this.LScaleY = new System.Windows.Forms.TextBox();
            this.label7 = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.LScaleX = new System.Windows.Forms.TextBox();
            this.label9 = new System.Windows.Forms.Label();
            this.LPositionZ = new System.Windows.Forms.TextBox();
            this.label17 = new System.Windows.Forms.Label();
            this.label18 = new System.Windows.Forms.Label();
            this.LPositionY = new System.Windows.Forms.TextBox();
            this.label19 = new System.Windows.Forms.Label();
            this.label20 = new System.Windows.Forms.Label();
            this.LPositionX = new System.Windows.Forms.TextBox();
            this.label40 = new System.Windows.Forms.Label();
            this.GridSize = new System.Windows.Forms.Label();
            this.TB_GridSize = new System.Windows.Forms.TextBox();
            this.CB_GridShow = new System.Windows.Forms.CheckBox();
            this.label39 = new System.Windows.Forms.Label();
            this.TB_BrushSize = new System.Windows.Forms.TextBox();
            ((System.ComponentModel.ISupportInitialize)(this.PB_DeviceWindow)).BeginInit();
            this.GB_Scene.SuspendLayout();
            this.Inspector.SuspendLayout();
            this.InspectorTab.SuspendLayout();
            this.LightPage.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.TB_Range)).BeginInit();
            this.Animation.SuspendLayout();
            this.NavigationPage.SuspendLayout();
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
            this.label11.Location = new System.Drawing.Point(6, 634);
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
            this.CB_LayerList.Location = new System.Drawing.Point(57, 631);
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
            this.label10.Location = new System.Drawing.Point(16, 601);
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
            this.TB_ObjectTag.Location = new System.Drawing.Point(57, 598);
            this.TB_ObjectTag.Name = "TB_ObjectTag";
            this.TB_ObjectTag.Size = new System.Drawing.Size(143, 23);
            this.TB_ObjectTag.TabIndex = 1;
            this.TB_ObjectTag.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.ChangeObjectTagEnter);
            // 
            // Btn_ObjectCreate
            // 
            this.Btn_ObjectCreate.Font = new System.Drawing.Font("맑은 고딕", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.Btn_ObjectCreate.ForeColor = System.Drawing.Color.Black;
            this.Btn_ObjectCreate.Location = new System.Drawing.Point(5, 687);
            this.Btn_ObjectCreate.Name = "Btn_ObjectCreate";
            this.Btn_ObjectCreate.Size = new System.Drawing.Size(95, 29);
            this.Btn_ObjectCreate.TabIndex = 5;
            this.Btn_ObjectCreate.Text = "Create";
            this.Btn_ObjectCreate.UseVisualStyleBackColor = true;
            this.Btn_ObjectCreate.Click += new System.EventHandler(this.Btn_CreateObjectClick);
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
            this.Inspector.Size = new System.Drawing.Size(329, 809);
            this.Inspector.TabIndex = 8;
            this.Inspector.TabStop = false;
            this.Inspector.Text = "Inspector";
            // 
            // InspectorTab
            // 
            this.InspectorTab.Controls.Add(this.LightPage);
            this.InspectorTab.Controls.Add(this.Animation);
            this.InspectorTab.Controls.Add(this.NavigationPage);
            this.InspectorTab.Controls.Add(this.Effect);
            this.InspectorTab.Controls.Add(this.RenderPage);
            this.InspectorTab.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.InspectorTab.Location = new System.Drawing.Point(7, 24);
            this.InspectorTab.Multiline = true;
            this.InspectorTab.Name = "InspectorTab";
            this.InspectorTab.SelectedIndex = 0;
            this.InspectorTab.Size = new System.Drawing.Size(317, 779);
            this.InspectorTab.TabIndex = 0;
            // 
            // LightPage
            // 
            this.LightPage.BackColor = System.Drawing.SystemColors.Control;
            this.LightPage.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.LightPage.Controls.Add(this.TB_RangeValue);
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
            this.LightPage.Controls.Add(this.Btn_LightColor);
            this.LightPage.Controls.Add(this.label31);
            this.LightPage.Controls.Add(this.label26);
            this.LightPage.Controls.Add(this.Btn_AddLightComponent);
            this.LightPage.Controls.Add(this.label22);
            this.LightPage.Controls.Add(this.CB_LightType);
            this.LightPage.ForeColor = System.Drawing.SystemColors.ControlText;
            this.LightPage.Location = new System.Drawing.Point(4, 26);
            this.LightPage.Name = "LightPage";
            this.LightPage.Size = new System.Drawing.Size(309, 749);
            this.LightPage.TabIndex = 5;
            this.LightPage.Text = "Light";
            // 
            // TB_RangeValue
            // 
            this.TB_RangeValue.Location = new System.Drawing.Point(247, 210);
            this.TB_RangeValue.Name = "TB_RangeValue";
            this.TB_RangeValue.ReadOnly = true;
            this.TB_RangeValue.Size = new System.Drawing.Size(53, 25);
            this.TB_RangeValue.TabIndex = 47;
            // 
            // CB_WireFrame
            // 
            this.CB_WireFrame.AutoSize = true;
            this.CB_WireFrame.ForeColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.CB_WireFrame.Location = new System.Drawing.Point(11, 87);
            this.CB_WireFrame.Name = "CB_WireFrame";
            this.CB_WireFrame.Size = new System.Drawing.Size(143, 21);
            this.CB_WireFrame.TabIndex = 46;
            this.CB_WireFrame.Text = "WireFrame On/Off";
            this.CB_WireFrame.UseVisualStyleBackColor = true;
            this.CB_WireFrame.CheckedChanged += new System.EventHandler(this.LightWireFrame_OnOff);
            // 
            // label34
            // 
            this.label34.AutoSize = true;
            this.label34.ForeColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.label34.Location = new System.Drawing.Point(8, 186);
            this.label34.Name = "label34";
            this.label34.Size = new System.Drawing.Size(46, 17);
            this.label34.TabIndex = 45;
            this.label34.Text = "Range";
            // 
            // TB_Range
            // 
            this.TB_Range.Location = new System.Drawing.Point(11, 209);
            this.TB_Range.Maximum = 1000;
            this.TB_Range.Name = "TB_Range";
            this.TB_Range.Size = new System.Drawing.Size(240, 45);
            this.TB_Range.TabIndex = 44;
            this.TB_Range.Scroll += new System.EventHandler(this.ChangeLightValue);
            // 
            // TB_SpcularPower
            // 
            this.TB_SpcularPower.BackColor = System.Drawing.SystemColors.Control;
            this.TB_SpcularPower.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.TB_SpcularPower.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.TB_SpcularPower.Location = new System.Drawing.Point(257, 150);
            this.TB_SpcularPower.Name = "TB_SpcularPower";
            this.TB_SpcularPower.Size = new System.Drawing.Size(43, 25);
            this.TB_SpcularPower.TabIndex = 43;
            this.TB_SpcularPower.TextChanged += new System.EventHandler(this.ChangeSpecularValue);
            this.TB_SpcularPower.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.SpecularKeyPress);
            // 
            // label33
            // 
            this.label33.AutoSize = true;
            this.label33.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.label33.ForeColor = System.Drawing.Color.Blue;
            this.label33.Location = new System.Drawing.Point(144, 154);
            this.label33.Name = "label33";
            this.label33.Size = new System.Drawing.Size(16, 17);
            this.label33.TabIndex = 42;
            this.label33.Text = "B";
            // 
            // label32
            // 
            this.label32.AutoSize = true;
            this.label32.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.label32.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(192)))), ((int)(((byte)(0)))));
            this.label32.Location = new System.Drawing.Point(79, 154);
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
            this.label30.Location = new System.Drawing.Point(208, 155);
            this.label30.Name = "label30";
            this.label30.Size = new System.Drawing.Size(46, 17);
            this.label30.TabIndex = 40;
            this.label30.Text = "Power";
            // 
            // label29
            // 
            this.label29.AutoSize = true;
            this.label29.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.label29.ForeColor = System.Drawing.Color.Red;
            this.label29.Location = new System.Drawing.Point(15, 155);
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
            this.TB_SpcularG.Location = new System.Drawing.Point(99, 150);
            this.TB_SpcularG.Name = "TB_SpcularG";
            this.TB_SpcularG.Size = new System.Drawing.Size(42, 25);
            this.TB_SpcularG.TabIndex = 38;
            this.TB_SpcularG.TextChanged += new System.EventHandler(this.ChangeSpecularValue);
            this.TB_SpcularG.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.SpecularKeyPress);
            // 
            // TB_SpcularB
            // 
            this.TB_SpcularB.BackColor = System.Drawing.SystemColors.Control;
            this.TB_SpcularB.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.TB_SpcularB.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.TB_SpcularB.Location = new System.Drawing.Point(163, 150);
            this.TB_SpcularB.Name = "TB_SpcularB";
            this.TB_SpcularB.Size = new System.Drawing.Size(42, 25);
            this.TB_SpcularB.TabIndex = 37;
            this.TB_SpcularB.TextChanged += new System.EventHandler(this.ChangeSpecularValue);
            this.TB_SpcularB.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.SpecularKeyPress);
            // 
            // label28
            // 
            this.label28.AutoSize = true;
            this.label28.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.label28.ForeColor = System.Drawing.Color.Black;
            this.label28.Location = new System.Drawing.Point(8, 126);
            this.label28.Name = "label28";
            this.label28.Size = new System.Drawing.Size(60, 17);
            this.label28.TabIndex = 35;
            this.label28.Text = "Specular";
            // 
            // TB_SpcularR
            // 
            this.TB_SpcularR.BackColor = System.Drawing.SystemColors.Control;
            this.TB_SpcularR.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.TB_SpcularR.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.TB_SpcularR.Location = new System.Drawing.Point(34, 150);
            this.TB_SpcularR.Name = "TB_SpcularR";
            this.TB_SpcularR.Size = new System.Drawing.Size(42, 25);
            this.TB_SpcularR.TabIndex = 36;
            this.TB_SpcularR.TextChanged += new System.EventHandler(this.ChangeSpecularValue);
            this.TB_SpcularR.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.SpecularKeyPress);
            // 
            // Btn_LightColor
            // 
            this.Btn_LightColor.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.Btn_LightColor.ForeColor = System.Drawing.Color.Black;
            this.Btn_LightColor.Location = new System.Drawing.Point(189, 12);
            this.Btn_LightColor.Name = "Btn_LightColor";
            this.Btn_LightColor.Size = new System.Drawing.Size(111, 32);
            this.Btn_LightColor.TabIndex = 34;
            this.Btn_LightColor.Text = "Light Color";
            this.Btn_LightColor.UseVisualStyleBackColor = true;
            this.Btn_LightColor.Click += new System.EventHandler(this.BT_DifColor_Click);
            // 
            // label31
            // 
            this.label31.AutoSize = true;
            this.label31.ForeColor = System.Drawing.Color.Black;
            this.label31.Location = new System.Drawing.Point(8, 107);
            this.label31.Name = "label31";
            this.label31.Size = new System.Drawing.Size(298, 17);
            this.label31.TabIndex = 17;
            this.label31.Text = "----------------------------------------------------------";
            // 
            // label26
            // 
            this.label26.AutoSize = true;
            this.label26.ForeColor = System.Drawing.Color.Black;
            this.label26.Location = new System.Drawing.Point(12, 720);
            this.label26.Name = "label26";
            this.label26.Size = new System.Drawing.Size(283, 17);
            this.label26.TabIndex = 9;
            this.label26.Text = "-------------------------------------------------------";
            // 
            // Btn_AddLightComponent
            // 
            this.Btn_AddLightComponent.ForeColor = System.Drawing.Color.Black;
            this.Btn_AddLightComponent.Location = new System.Drawing.Point(7, 12);
            this.Btn_AddLightComponent.Name = "Btn_AddLightComponent";
            this.Btn_AddLightComponent.Size = new System.Drawing.Size(176, 32);
            this.Btn_AddLightComponent.TabIndex = 2;
            this.Btn_AddLightComponent.Text = "Add Light Component";
            this.Btn_AddLightComponent.UseVisualStyleBackColor = true;
            this.Btn_AddLightComponent.Click += new System.EventHandler(this.AddLightComponent);
            // 
            // label22
            // 
            this.label22.AutoSize = true;
            this.label22.ForeColor = System.Drawing.Color.Black;
            this.label22.Location = new System.Drawing.Point(8, 59);
            this.label22.Name = "label22";
            this.label22.Size = new System.Drawing.Size(78, 17);
            this.label22.TabIndex = 1;
            this.label22.Text = "LightType :";
            // 
            // CB_LightType
            // 
            this.CB_LightType.BackColor = System.Drawing.SystemColors.Control;
            this.CB_LightType.FormattingEnabled = true;
            this.CB_LightType.Items.AddRange(new object[] {
            "Direction",
            "Point",
            "Spot"});
            this.CB_LightType.Location = new System.Drawing.Point(90, 55);
            this.CB_LightType.Name = "CB_LightType";
            this.CB_LightType.Size = new System.Drawing.Size(110, 25);
            this.CB_LightType.TabIndex = 0;
            this.CB_LightType.SelectedIndexChanged += new System.EventHandler(this.CB_LightType_SelectedIndexChanged);
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
            this.Animation.Controls.Add(this.ClipLoad);
            this.Animation.Controls.Add(this.ClipSave);
            this.Animation.ForeColor = System.Drawing.SystemColors.ControlText;
            this.Animation.Location = new System.Drawing.Point(4, 26);
            this.Animation.Name = "Animation";
            this.Animation.Padding = new System.Windows.Forms.Padding(3);
            this.Animation.Size = new System.Drawing.Size(309, 749);
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
            this.LB_BoneList.Size = new System.Drawing.Size(239, 140);
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
            this.ClipList.Location = new System.Drawing.Point(6, 5);
            this.ClipList.Name = "ClipList";
            this.ClipList.Size = new System.Drawing.Size(244, 242);
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
            this.AnimationTagBox.Size = new System.Drawing.Size(135, 25);
            this.AnimationTagBox.TabIndex = 12;
            this.AnimationTagBox.TextChanged += new System.EventHandler(this.AnimationTagBox_TextChanged);
            // 
            // AnimationOptionBox
            // 
            this.AnimationOptionBox.BackColor = System.Drawing.SystemColors.Control;
            this.AnimationOptionBox.FormattingEnabled = true;
            this.AnimationOptionBox.Items.AddRange(new object[] {
            "LOOP",
            "ONCE_RETURN",
            "ONCE_DESTROY"});
            this.AnimationOptionBox.Location = new System.Drawing.Point(129, 602);
            this.AnimationOptionBox.Name = "AnimationOptionBox";
            this.AnimationOptionBox.Size = new System.Drawing.Size(149, 25);
            this.AnimationOptionBox.TabIndex = 21;
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
            // NavigationPage
            // 
            this.NavigationPage.BackColor = System.Drawing.SystemColors.Control;
            this.NavigationPage.Controls.Add(this.label40);
            this.NavigationPage.Controls.Add(this.GridSize);
            this.NavigationPage.Controls.Add(this.TB_GridSize);
            this.NavigationPage.Controls.Add(this.CB_GridShow);
            this.NavigationPage.Controls.Add(this.label39);
            this.NavigationPage.Controls.Add(this.TB_BrushSize);
            this.NavigationPage.Controls.Add(this.label38);
            this.NavigationPage.Controls.Add(this.TB_TileFlag);
            this.NavigationPage.Controls.Add(this.label37);
            this.NavigationPage.Controls.Add(this.TB_NaviIndex);
            this.NavigationPage.Controls.Add(this.label36);
            this.NavigationPage.Controls.Add(this.CB_isNaviEditor);
            this.NavigationPage.Controls.Add(this.TB_NumZ);
            this.NavigationPage.Controls.Add(this.label35);
            this.NavigationPage.Controls.Add(this.TB_NumX);
            this.NavigationPage.Controls.Add(this.BT_DeleteNavi);
            this.NavigationPage.Controls.Add(this.BT_CreateNavi);
            this.NavigationPage.Controls.Add(this.BT_NaviLoad);
            this.NavigationPage.Controls.Add(this.BT_NaviSave);
            this.NavigationPage.Location = new System.Drawing.Point(4, 48);
            this.NavigationPage.Name = "NavigationPage";
            this.NavigationPage.Size = new System.Drawing.Size(287, 746);
            this.NavigationPage.TabIndex = 6;
            this.NavigationPage.Text = "Navigation";
            // label40
            // 
            this.label40.AutoSize = true;
            this.label40.ForeColor = System.Drawing.Color.Black;
            this.label40.Location = new System.Drawing.Point(7, 221);
            this.label40.Name = "label40";
            this.label40.Size = new System.Drawing.Size(127, 17);
            this.label40.TabIndex = 18;
            this.label40.Text = "(낮을수록 촘촘해짐)";
            // 
            // GridSize
            // 
            this.GridSize.AutoSize = true;
            this.GridSize.ForeColor = System.Drawing.Color.Black;
            this.GridSize.Location = new System.Drawing.Point(7, 198);
            this.GridSize.Name = "GridSize";
            this.GridSize.Size = new System.Drawing.Size(58, 17);
            this.GridSize.TabIndex = 17;
            this.GridSize.Text = "GridSize";
            // 
            // TB_GridSize
            // 
            this.TB_GridSize.BackColor = System.Drawing.SystemColors.Control;
            this.TB_GridSize.Location = new System.Drawing.Point(103, 193);
            this.TB_GridSize.Name = "TB_GridSize";
            this.TB_GridSize.Size = new System.Drawing.Size(165, 25);
            this.TB_GridSize.TabIndex = 16;
            this.TB_GridSize.TextChanged += new System.EventHandler(this.TB_GridSize_TextChanged);
            this.TB_GridSize.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.txtInterval_KeyPress);
            // 
            // CB_GridShow
            // 
            this.CB_GridShow.AutoSize = true;
            this.CB_GridShow.ForeColor = System.Drawing.Color.Black;
            this.CB_GridShow.Location = new System.Drawing.Point(10, 264);
            this.CB_GridShow.Name = "CB_GridShow";
            this.CB_GridShow.Size = new System.Drawing.Size(86, 21);
            this.CB_GridShow.TabIndex = 15;
            this.CB_GridShow.Text = "GridShow";
            this.CB_GridShow.UseVisualStyleBackColor = true;
            this.CB_GridShow.CheckedChanged += new System.EventHandler(this.CB_GridShow_CheckedChanged);
            // 
            // label39
            // 
            this.label39.AutoSize = true;
            this.label39.ForeColor = System.Drawing.Color.Black;
            this.label39.Location = new System.Drawing.Point(7, 153);
            this.label39.Name = "label39";
            this.label39.Size = new System.Drawing.Size(71, 17);
            this.label39.TabIndex = 14;
            this.label39.Text = "Blush Size";
            // 
            // TB_BrushSize
            // 
            this.TB_BrushSize.BackColor = System.Drawing.SystemColors.Control;
            this.TB_BrushSize.Location = new System.Drawing.Point(103, 148);
            this.TB_BrushSize.Name = "TB_BrushSize";
            this.TB_BrushSize.Size = new System.Drawing.Size(165, 25);
            this.TB_BrushSize.TabIndex = 13;
            this.TB_BrushSize.TextChanged += new System.EventHandler(this.TB_BrushSize_TextChanged);
            this.TB_BrushSize.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.txtInterval_KeyPress);
            // 
            // label38
            // 
            this.label38.AutoSize = true;
            this.label38.ForeColor = System.Drawing.Color.Black;
            this.label38.Location = new System.Drawing.Point(4, 106);
            this.label38.Name = "label38";
            this.label38.Size = new System.Drawing.Size(93, 17);
            this.label38.TabIndex = 12;
            this.label38.Text = "이동여부(Tab)";
            // 
            // TB_TileFlag
            // 
            this.TB_TileFlag.Location = new System.Drawing.Point(103, 103);
            this.TB_TileFlag.Name = "TB_TileFlag";
            this.TB_TileFlag.ReadOnly = true;
            this.TB_TileFlag.Size = new System.Drawing.Size(165, 25);
            this.TB_TileFlag.TabIndex = 11;
            // 
            // label37
            // 
            this.label37.AutoSize = true;
            this.label37.ForeColor = System.Drawing.Color.Black;
            this.label37.Location = new System.Drawing.Point(4, 62);
            this.label37.Name = "label37";
            this.label37.Size = new System.Drawing.Size(78, 17);
            this.label37.TabIndex = 10;
            this.label37.Text = "선택 인덱스";
            // 
            // TB_NaviIndex
            // 
            this.TB_NaviIndex.Location = new System.Drawing.Point(103, 58);
            this.TB_NaviIndex.Name = "TB_NaviIndex";
            this.TB_NaviIndex.ReadOnly = true;
            this.TB_NaviIndex.Size = new System.Drawing.Size(165, 25);
            this.TB_NaviIndex.TabIndex = 9;
            // 
            // label36
            // 
            this.label36.AutoSize = true;
            this.label36.ForeColor = System.Drawing.Color.Black;
            this.label36.Location = new System.Drawing.Point(142, 19);
            this.label36.Name = "label36";
            this.label36.Size = new System.Drawing.Size(46, 17);
            this.label36.TabIndex = 8;
            this.label36.Text = "NumZ";
            // 
            // CB_isNaviEditor
            // 
            this.CB_isNaviEditor.AutoSize = true;
            this.CB_isNaviEditor.ForeColor = System.Drawing.Color.Black;
            this.CB_isNaviEditor.Location = new System.Drawing.Point(14, 315);
            this.CB_isNaviEditor.Name = "CB_isNaviEditor";
            this.CB_isNaviEditor.Size = new System.Drawing.Size(128, 21);
            this.CB_isNaviEditor.TabIndex = 4;
            this.CB_isNaviEditor.Text = "NaviEditorMode";
            this.CB_isNaviEditor.UseVisualStyleBackColor = true;
            this.CB_isNaviEditor.CheckedChanged += new System.EventHandler(this.CB_isNaviEditor_CheckedChanged);
            // 
            // TB_NumZ
            // 
            this.TB_NumZ.BackColor = System.Drawing.SystemColors.Control;
            this.TB_NumZ.Location = new System.Drawing.Point(191, 15);
            this.TB_NumZ.Name = "TB_NumZ";
            this.TB_NumZ.Size = new System.Drawing.Size(77, 25);
            this.TB_NumZ.TabIndex = 7;
            this.TB_NumZ.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.txtInterval_KeyPress);
            // 
            // label35
            // 
            this.label35.AutoSize = true;
            this.label35.ForeColor = System.Drawing.Color.Black;
            this.label35.Location = new System.Drawing.Point(4, 19);
            this.label35.Name = "label35";
            this.label35.Size = new System.Drawing.Size(46, 17);
            this.label35.TabIndex = 6;
            this.label35.Text = "NumX";
            // 
            // TB_NumX
            // 
            this.TB_NumX.BackColor = System.Drawing.SystemColors.Control;
            this.TB_NumX.Location = new System.Drawing.Point(53, 15);
            this.TB_NumX.Name = "TB_NumX";
            this.TB_NumX.Size = new System.Drawing.Size(77, 25);
            this.TB_NumX.TabIndex = 5;
            this.TB_NumX.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.txtInterval_KeyPress);
            // 
            // BT_DeleteNavi
            // 
            this.BT_DeleteNavi.ForeColor = System.Drawing.Color.Black;
            this.BT_DeleteNavi.Location = new System.Drawing.Point(153, 348);
            this.BT_DeleteNavi.Name = "BT_DeleteNavi";
            this.BT_DeleteNavi.Size = new System.Drawing.Size(124, 52);
            this.BT_DeleteNavi.TabIndex = 3;
            this.BT_DeleteNavi.Text = "DeleteLandScape";
            this.BT_DeleteNavi.UseVisualStyleBackColor = true;
            this.BT_DeleteNavi.Click += new System.EventHandler(this.BT_DeleteNavi_Click);
            // 
            // BT_CreateNavi
            // 
            this.BT_CreateNavi.ForeColor = System.Drawing.Color.Black;
            this.BT_CreateNavi.Location = new System.Drawing.Point(14, 349);
            this.BT_CreateNavi.Name = "BT_CreateNavi";
            this.BT_CreateNavi.Size = new System.Drawing.Size(125, 52);
            this.BT_CreateNavi.TabIndex = 2;
            this.BT_CreateNavi.Text = "CreateLandScape";
            this.BT_CreateNavi.UseVisualStyleBackColor = true;
            this.BT_CreateNavi.Click += new System.EventHandler(this.BT_CreateNavi_Click);
            // 
            // BT_NaviLoad
            // 
            this.BT_NaviLoad.ForeColor = System.Drawing.Color.Black;
            this.BT_NaviLoad.Location = new System.Drawing.Point(153, 420);
            this.BT_NaviLoad.Name = "BT_NaviLoad";
            this.BT_NaviLoad.Size = new System.Drawing.Size(124, 52);
            this.BT_NaviLoad.TabIndex = 1;
            this.BT_NaviLoad.Text = "Load";
            this.BT_NaviLoad.UseVisualStyleBackColor = true;
            this.BT_NaviLoad.Click += new System.EventHandler(this.BT_NaviLoad_Click);
            // 
            // BT_NaviSave
            // 
            this.BT_NaviSave.ForeColor = System.Drawing.Color.Black;
            this.BT_NaviSave.Location = new System.Drawing.Point(14, 420);
            this.BT_NaviSave.Name = "BT_NaviSave";
            this.BT_NaviSave.Size = new System.Drawing.Size(125, 52);
            this.BT_NaviSave.TabIndex = 0;
            this.BT_NaviSave.Text = "Save";
            this.BT_NaviSave.UseVisualStyleBackColor = true;
            this.BT_NaviSave.Click += new System.EventHandler(this.BT_NaviSave_Click);
            // 
            // Effect
            // 
            this.Effect.BackColor = System.Drawing.SystemColors.Control;
            this.Effect.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.Effect.ForeColor = System.Drawing.SystemColors.ControlText;
            this.Effect.Location = new System.Drawing.Point(4, 26);
            this.Effect.Name = "Effect";
            this.Effect.Padding = new System.Windows.Forms.Padding(3);
            this.Effect.Size = new System.Drawing.Size(309, 749);
            this.Effect.TabIndex = 2;
            this.Effect.Text = "Effect";
            // 
            // RenderPage
            // 
            this.RenderPage.BackColor = System.Drawing.SystemColors.Control;
            this.RenderPage.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.RenderPage.ForeColor = System.Drawing.SystemColors.ControlText;
            this.RenderPage.Location = new System.Drawing.Point(4, 26);
            this.RenderPage.Name = "RenderPage";
            this.RenderPage.Size = new System.Drawing.Size(309, 749);
            this.RenderPage.TabIndex = 4;
            this.RenderPage.Text = "Render";
            // 
            // MshLoad
            // 
            this.MshLoad.ForeColor = System.Drawing.Color.Black;
            this.MshLoad.Location = new System.Drawing.Point(169, 373);
            this.MshLoad.Name = "MshLoad";
            this.MshLoad.Size = new System.Drawing.Size(118, 42);
            this.MshLoad.TabIndex = 16;
            this.MshLoad.Text = "Msh File Load";
            this.MshLoad.UseVisualStyleBackColor = true;
            this.MshLoad.Click += new System.EventHandler(this.Btn_MshFileLoad);
            // 
            // LB_MeshList
            // 
            this.LB_MeshList.BackColor = System.Drawing.SystemColors.Control;
            this.LB_MeshList.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.LB_MeshList.FormattingEnabled = true;
            this.LB_MeshList.HorizontalScrollbar = true;
            this.LB_MeshList.ItemHeight = 17;
            this.LB_MeshList.Location = new System.Drawing.Point(9, 79);
            this.LB_MeshList.Name = "LB_MeshList";
            this.LB_MeshList.Size = new System.Drawing.Size(278, 123);
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
            this.GB_Transform.Location = new System.Drawing.Point(1311, 203);
            this.GB_Transform.Name = "GB_Transform";
            this.GB_Transform.Size = new System.Drawing.Size(295, 186);
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
            this.RotateZ.Text = "0";
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
            this.RotateY.Text = "0";
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
            this.RotateX.Text = "0";
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
            this.Label_Rotate.Size = new System.Drawing.Size(48, 17);
            this.Label_Rotate.TabIndex = 21;
            this.Label_Rotate.Text = "Rotate";
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
            this.ScaleZ.Text = "0";
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
            this.ScaleY.Text = "0";
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
            this.ScaleX.Text = "0";
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
            this.Label_Scale.Size = new System.Drawing.Size(39, 17);
            this.Label_Scale.TabIndex = 14;
            this.Label_Scale.Text = "Scale";
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
            this.PositionZ.Text = "0";
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
            this.Label_WorldPosition.Size = new System.Drawing.Size(59, 17);
            this.Label_WorldPosition.TabIndex = 0;
            this.Label_WorldPosition.Text = "Position";
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
            this.PositionY.Text = "0";
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
            this.PositionX.Text = "0";
            this.PositionX.TextChanged += new System.EventHandler(this.ChangePosition);
            this.PositionX.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.KeyPressCheck);
            // 
            // GB_Hierachy
            // 
            this.GB_Hierachy.Controls.Add(this.CB_ObjectSave);
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
            // CB_ObjectSave
            // 
            this.CB_ObjectSave.AutoSize = true;
            this.CB_ObjectSave.ForeColor = System.Drawing.Color.Black;
            this.CB_ObjectSave.Location = new System.Drawing.Point(9, 662);
            this.CB_ObjectSave.Name = "CB_ObjectSave";
            this.CB_ObjectSave.Size = new System.Drawing.Size(100, 21);
            this.CB_ObjectSave.TabIndex = 15;
            this.CB_ObjectSave.Text = "Object Save";
            this.CB_ObjectSave.UseVisualStyleBackColor = true;
            // 
            // Btn_ObjectDelete
            // 
            this.Btn_ObjectDelete.Font = new System.Drawing.Font("맑은 고딕", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.Btn_ObjectDelete.ForeColor = System.Drawing.Color.Black;
            this.Btn_ObjectDelete.Location = new System.Drawing.Point(105, 687);
            this.Btn_ObjectDelete.Name = "Btn_ObjectDelete";
            this.Btn_ObjectDelete.Size = new System.Drawing.Size(95, 29);
            this.Btn_ObjectDelete.TabIndex = 6;
            this.Btn_ObjectDelete.Text = "Delete";
            this.Btn_ObjectDelete.UseVisualStyleBackColor = true;
            this.Btn_ObjectDelete.Click += new System.EventHandler(this.Btn_DeleteObjectClick);
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
            this.LB_ObjectList.Size = new System.Drawing.Size(195, 565);
            this.LB_ObjectList.TabIndex = 0;
            this.LB_ObjectList.SelectedIndexChanged += new System.EventHandler(this.ChangeSelectedObject);
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
            this.Btn_FileSave.Click += new System.EventHandler(this.Btn_SaveLoadClick);
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
            this.Btn_FileLoad.Click += new System.EventHandler(this.Btn_SaveLoadClick);
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
            this.Btn_AddComponent.Location = new System.Drawing.Point(8, 23);
            this.Btn_AddComponent.Name = "Btn_AddComponent";
            this.Btn_AddComponent.Size = new System.Drawing.Size(183, 31);
            this.Btn_AddComponent.TabIndex = 3;
            this.Btn_AddComponent.Text = "Add Render Component";
            this.Btn_AddComponent.UseVisualStyleBackColor = true;
            this.Btn_AddComponent.Click += new System.EventHandler(this.AddRenderComponent);
            // 
            // Label_BasicMesh
            // 
            this.Label_BasicMesh.AutoSize = true;
            this.Label_BasicMesh.ForeColor = System.Drawing.Color.Black;
            this.Label_BasicMesh.Location = new System.Drawing.Point(7, 58);
            this.Label_BasicMesh.Name = "Label_BasicMesh";
            this.Label_BasicMesh.Size = new System.Drawing.Size(100, 17);
            this.Label_BasicMesh.TabIndex = 5;
            this.Label_BasicMesh.Text = "BasicMesh List";
            // 
            // Btn_AddMesh
            // 
            this.Btn_AddMesh.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.Btn_AddMesh.ForeColor = System.Drawing.Color.Black;
            this.Btn_AddMesh.Location = new System.Drawing.Point(197, 23);
            this.Btn_AddMesh.Name = "Btn_AddMesh";
            this.Btn_AddMesh.Size = new System.Drawing.Size(92, 31);
            this.Btn_AddMesh.TabIndex = 2;
            this.Btn_AddMesh.Text = "Set Mesh";
            this.Btn_AddMesh.UseVisualStyleBackColor = true;
            this.Btn_AddMesh.Click += new System.EventHandler(this.Btn_SetMeshClick);
            // 
            // GB_Renderer
            // 
            this.GB_Renderer.Controls.Add(this.FileMesh);
            this.GB_Renderer.Controls.Add(this.CB_TargetChange);
            this.GB_Renderer.Controls.Add(this.LB_FileMesh);
            this.GB_Renderer.Controls.Add(this.CB_isDebugTarget);
            this.GB_Renderer.Controls.Add(this.LB_MeshList);
            this.GB_Renderer.Controls.Add(this.Btn_AddMesh);
            this.GB_Renderer.Controls.Add(this.Label_BasicMesh);
            this.GB_Renderer.Controls.Add(this.Btn_AddComponent);
            this.GB_Renderer.Controls.Add(this.MshLoad);
            this.GB_Renderer.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.GB_Renderer.ForeColor = System.Drawing.Color.Red;
            this.GB_Renderer.Location = new System.Drawing.Point(1311, 395);
            this.GB_Renderer.Name = "GB_Renderer";
            this.GB_Renderer.Size = new System.Drawing.Size(295, 426);
            this.GB_Renderer.TabIndex = 0;
            this.GB_Renderer.TabStop = false;
            this.GB_Renderer.Text = "Renderer";
            // 
            // FileMesh
            // 
            this.FileMesh.AutoSize = true;
            this.FileMesh.ForeColor = System.Drawing.Color.Black;
            this.FileMesh.Location = new System.Drawing.Point(7, 205);
            this.FileMesh.Name = "FileMesh";
            this.FileMesh.Size = new System.Drawing.Size(91, 17);
            this.FileMesh.TabIndex = 7;
            this.FileMesh.Text = "FileMesh List";
            // 
            // CB_TargetChange
            // 
            this.CB_TargetChange.AutoSize = true;
            this.CB_TargetChange.ForeColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.CB_TargetChange.Location = new System.Drawing.Point(9, 395);
            this.CB_TargetChange.Name = "CB_TargetChange";
            this.CB_TargetChange.Size = new System.Drawing.Size(150, 21);
            this.CB_TargetChange.TabIndex = 9;
            this.CB_TargetChange.Text = "Cam Target Change";
            this.CB_TargetChange.UseVisualStyleBackColor = true;
            this.CB_TargetChange.CheckedChanged += new System.EventHandler(this.TargetChange_OnOff);
            // 
            // LB_FileMesh
            // 
            this.LB_FileMesh.BackColor = System.Drawing.SystemColors.Control;
            this.LB_FileMesh.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.LB_FileMesh.FormattingEnabled = true;
            this.LB_FileMesh.ItemHeight = 17;
            this.LB_FileMesh.Location = new System.Drawing.Point(9, 225);
            this.LB_FileMesh.Name = "LB_FileMesh";
            this.LB_FileMesh.Size = new System.Drawing.Size(278, 140);
            this.LB_FileMesh.TabIndex = 6;
            this.LB_FileMesh.Click += new System.EventHandler(this.FileMeshClick);
            // 
            // CB_isDebugTarget
            // 
            this.CB_isDebugTarget.AutoSize = true;
            this.CB_isDebugTarget.ForeColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.CB_isDebugTarget.Location = new System.Drawing.Point(9, 372);
            this.CB_isDebugTarget.Name = "CB_isDebugTarget";
            this.CB_isDebugTarget.Size = new System.Drawing.Size(116, 21);
            this.CB_isDebugTarget.TabIndex = 8;
            this.CB_isDebugTarget.Text = "Target On/Off";
            this.CB_isDebugTarget.UseVisualStyleBackColor = true;
            this.CB_isDebugTarget.CheckedChanged += new System.EventHandler(this.DebugRenderTarget_OnOff);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.label1.ForeColor = System.Drawing.Color.Black;
            this.label1.Location = new System.Drawing.Point(885, 680);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(100, 17);
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
            this.LB_AniList.Size = new System.Drawing.Size(417, 123);
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
            this.groupBox1.Controls.Add(this.LRotateZ);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.LRotateY);
            this.groupBox1.Controls.Add(this.label3);
            this.groupBox1.Controls.Add(this.label4);
            this.groupBox1.Controls.Add(this.LRotateX);
            this.groupBox1.Controls.Add(this.label5);
            this.groupBox1.Controls.Add(this.LScaleZ);
            this.groupBox1.Controls.Add(this.label6);
            this.groupBox1.Controls.Add(this.LScaleY);
            this.groupBox1.Controls.Add(this.label7);
            this.groupBox1.Controls.Add(this.label8);
            this.groupBox1.Controls.Add(this.LScaleX);
            this.groupBox1.Controls.Add(this.label9);
            this.groupBox1.Controls.Add(this.LPositionZ);
            this.groupBox1.Controls.Add(this.label17);
            this.groupBox1.Controls.Add(this.label18);
            this.groupBox1.Controls.Add(this.LPositionY);
            this.groupBox1.Controls.Add(this.label19);
            this.groupBox1.Controls.Add(this.label20);
            this.groupBox1.Controls.Add(this.LPositionX);
            this.groupBox1.Font = new System.Drawing.Font("맑은 고딕", 10F, System.Drawing.FontStyle.Bold);
            this.groupBox1.ForeColor = System.Drawing.Color.Red;
            this.groupBox1.Location = new System.Drawing.Point(1311, 12);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(295, 186);
            this.groupBox1.TabIndex = 28;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Local Transform";
            // 
            // LRotateZ
            // 
            this.LRotateZ.BackColor = System.Drawing.SystemColors.Control;
            this.LRotateZ.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.LRotateZ.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.LRotateZ.Location = new System.Drawing.Point(212, 150);
            this.LRotateZ.Name = "LRotateZ";
            this.LRotateZ.Size = new System.Drawing.Size(64, 25);
            this.LRotateZ.TabIndex = 27;
            this.LRotateZ.Text = "0";
            this.LRotateZ.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.KeyPressCheck);
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
            // LRotateY
            // 
            this.LRotateY.BackColor = System.Drawing.SystemColors.Control;
            this.LRotateY.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.LRotateY.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.LRotateY.Location = new System.Drawing.Point(126, 150);
            this.LRotateY.Name = "LRotateY";
            this.LRotateY.Size = new System.Drawing.Size(64, 25);
            this.LRotateY.TabIndex = 25;
            this.LRotateY.Text = "0";
            this.LRotateY.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.KeyPressCheck);
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
            // LRotateX
            // 
            this.LRotateX.BackColor = System.Drawing.SystemColors.Control;
            this.LRotateX.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.LRotateX.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.LRotateX.Location = new System.Drawing.Point(38, 151);
            this.LRotateX.Name = "LRotateX";
            this.LRotateX.Size = new System.Drawing.Size(64, 25);
            this.LRotateX.TabIndex = 23;
            this.LRotateX.Text = "0";
            this.LRotateX.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.KeyPressCheck);
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.label5.ForeColor = System.Drawing.Color.Black;
            this.label5.Location = new System.Drawing.Point(11, 130);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(48, 17);
            this.label5.TabIndex = 21;
            this.label5.Text = "Rotate";
            // 
            // LScaleZ
            // 
            this.LScaleZ.BackColor = System.Drawing.SystemColors.Control;
            this.LScaleZ.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.LScaleZ.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.LScaleZ.Location = new System.Drawing.Point(212, 98);
            this.LScaleZ.Name = "LScaleZ";
            this.LScaleZ.Size = new System.Drawing.Size(64, 25);
            this.LScaleZ.TabIndex = 20;
            this.LScaleZ.Text = "0";
            this.LScaleZ.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.KeyPressCheck);
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
            // LScaleY
            // 
            this.LScaleY.BackColor = System.Drawing.SystemColors.Control;
            this.LScaleY.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.LScaleY.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.LScaleY.Location = new System.Drawing.Point(126, 98);
            this.LScaleY.Name = "LScaleY";
            this.LScaleY.Size = new System.Drawing.Size(64, 25);
            this.LScaleY.TabIndex = 18;
            this.LScaleY.Text = "0";
            this.LScaleY.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.KeyPressCheck);
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
            // LScaleX
            // 
            this.LScaleX.BackColor = System.Drawing.SystemColors.Control;
            this.LScaleX.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.LScaleX.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.LScaleX.Location = new System.Drawing.Point(38, 99);
            this.LScaleX.Name = "LScaleX";
            this.LScaleX.Size = new System.Drawing.Size(64, 25);
            this.LScaleX.TabIndex = 16;
            this.LScaleX.Text = "0";
            this.LScaleX.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.KeyPressCheck);
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.label9.ForeColor = System.Drawing.Color.Black;
            this.label9.Location = new System.Drawing.Point(11, 77);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(39, 17);
            this.label9.TabIndex = 14;
            this.label9.Text = "Scale";
            // 
            // LPositionZ
            // 
            this.LPositionZ.BackColor = System.Drawing.SystemColors.Control;
            this.LPositionZ.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.LPositionZ.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.LPositionZ.Location = new System.Drawing.Point(212, 45);
            this.LPositionZ.Name = "LPositionZ";
            this.LPositionZ.Size = new System.Drawing.Size(64, 25);
            this.LPositionZ.TabIndex = 13;
            this.LPositionZ.Text = "0";
            this.LPositionZ.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.KeyPressCheck);
            // 
            // label17
            // 
            this.label17.AutoSize = true;
            this.label17.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.label17.ForeColor = System.Drawing.Color.Black;
            this.label17.Location = new System.Drawing.Point(11, 24);
            this.label17.Name = "label17";
            this.label17.Size = new System.Drawing.Size(59, 17);
            this.label17.TabIndex = 0;
            this.label17.Text = "Position";
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
            // LPositionY
            // 
            this.LPositionY.BackColor = System.Drawing.SystemColors.Control;
            this.LPositionY.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.LPositionY.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.LPositionY.Location = new System.Drawing.Point(126, 45);
            this.LPositionY.Name = "LPositionY";
            this.LPositionY.Size = new System.Drawing.Size(64, 25);
            this.LPositionY.TabIndex = 11;
            this.LPositionY.Text = "0";
            this.LPositionY.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.KeyPressCheck);
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
            // LPositionX
            // 
            this.LPositionX.BackColor = System.Drawing.SystemColors.Control;
            this.LPositionX.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.LPositionX.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.LPositionX.Location = new System.Drawing.Point(38, 45);
            this.LPositionX.Name = "LPositionX";
            this.LPositionX.Size = new System.Drawing.Size(64, 25);
            this.LPositionX.TabIndex = 2;
            this.LPositionX.Text = "0";
            this.LPositionX.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.KeyPressCheck);
            // 
            // EditorForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(96F, 96F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Dpi;
            this.AutoScroll = true;
            this.BackColor = System.Drawing.SystemColors.Control;
            this.ClientSize = new System.Drawing.Size(1948, 827);
            this.Controls.Add(this.GB_Transform);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.Btn_AllClear);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.GB_Hierachy);
            this.Controls.Add(this.LB_AniList);
            this.Controls.Add(this.LB_LogMessage);
            this.Controls.Add(this.Btn_FileLoad);
            this.Controls.Add(this.Btn_FileSave);
            this.Controls.Add(this.Inspector);
            this.Controls.Add(this.GB_Scene);
            this.Controls.Add(this.GB_Renderer);
            this.ForeColor = System.Drawing.SystemColors.WindowText;
            this.Name = "EditorForm";
            this.Text = "MotherFatherChildren";
            this.TransparencyKey = System.Drawing.Color.Gray;
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.Delete);
            ((System.ComponentModel.ISupportInitialize)(this.PB_DeviceWindow)).EndInit();
            this.GB_Scene.ResumeLayout(false);
            this.Inspector.ResumeLayout(false);
            this.InspectorTab.ResumeLayout(false);
            this.LightPage.ResumeLayout(false);
            this.LightPage.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.TB_Range)).EndInit();
            this.Animation.ResumeLayout(false);
            this.Animation.PerformLayout();
            this.NavigationPage.ResumeLayout(false);
            this.NavigationPage.PerformLayout();
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
        private System.Windows.Forms.TextBox LRotateZ;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox LRotateY;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox LRotateX;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.TextBox LScaleZ;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.TextBox LScaleY;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.TextBox LScaleX;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.TextBox LPositionZ;
        private System.Windows.Forms.Label label17;
        private System.Windows.Forms.Label label18;
        private System.Windows.Forms.TextBox LPositionY;
        private System.Windows.Forms.Label label19;
        private System.Windows.Forms.Label label20;
        private System.Windows.Forms.TextBox LPositionX;
        private System.Windows.Forms.Label label21;
        private System.Windows.Forms.ListBox LB_BoneList;
        private System.Windows.Forms.TabPage RenderPage;
        private System.Windows.Forms.TabPage LightPage;
        private System.Windows.Forms.TabPage NavigationPage;
        private System.Windows.Forms.Label label22;
        private System.Windows.Forms.ComboBox CB_LightType;
        private System.Windows.Forms.Button Btn_AddLightComponent;
        private System.Windows.Forms.Label label31;
        private System.Windows.Forms.Button Btn_LightColor;
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
        private System.Windows.Forms.CheckBox CB_TargetChange;
        private System.Windows.Forms.Label label36;
        private System.Windows.Forms.TextBox TB_NumZ;
        private System.Windows.Forms.Label label35;
        private System.Windows.Forms.TextBox TB_NumX;
        private System.Windows.Forms.CheckBox CB_isNaviEditor;
        private System.Windows.Forms.Button BT_DeleteNavi;
        private System.Windows.Forms.Button BT_CreateNavi;
        private System.Windows.Forms.Button BT_NaviLoad;
        private System.Windows.Forms.Button BT_NaviSave;
        private System.Windows.Forms.CheckBox CB_ObjectSave;
        private System.Windows.Forms.Label label38;
        private System.Windows.Forms.TextBox TB_TileFlag;
        private System.Windows.Forms.Label label37;
        private System.Windows.Forms.TextBox TB_NaviIndex;
        private System.Windows.Forms.Label label26;
        private System.Windows.Forms.TextBox TB_RangeValue;
        private System.Windows.Forms.TextBox TB_BrushSize;
        private System.Windows.Forms.CheckBox CB_GridShow;
        private System.Windows.Forms.Label label40;
        private System.Windows.Forms.Label label39;
        private System.Windows.Forms.Label GridSize;
        private System.Windows.Forms.TextBox TB_GridSize;
    }
}
