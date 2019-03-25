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
            this.Label_Position = new System.Windows.Forms.Label();
            this.Label_PosZ = new System.Windows.Forms.Label();
            this.PositionY = new System.Windows.Forms.TextBox();
            this.Label_PosX = new System.Windows.Forms.Label();
            this.Label_PosY = new System.Windows.Forms.Label();
            this.PositionX = new System.Windows.Forms.TextBox();
            this.GB_Hierachy = new System.Windows.Forms.GroupBox();
            this.CheckBox_Gizmos = new System.Windows.Forms.CheckBox();
            this.Btn_ObjectDelete = new System.Windows.Forms.Button();
            this.LB_ObjectList = new System.Windows.Forms.ListBox();
            this.Btn_FileSave = new System.Windows.Forms.Button();
            this.Btn_FileLoad = new System.Windows.Forms.Button();
            this.LB_LogMessage = new System.Windows.Forms.ListBox();
            this.Btn_SetAnimation = new System.Windows.Forms.Button();
            this.Btn_AddComponent = new System.Windows.Forms.Button();
            this.Label_MeshList = new System.Windows.Forms.Label();
            this.Btn_AddMesh = new System.Windows.Forms.Button();
            this.GB_Renderer = new System.Windows.Forms.GroupBox();
            this.label1 = new System.Windows.Forms.Label();
            this.LB_AniList = new System.Windows.Forms.ListBox();
            this.Btn_AllClear = new System.Windows.Forms.Button();
            this.ClearCount = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.PB_DeviceWindow)).BeginInit();
            this.GB_Scene.SuspendLayout();
            this.Inspector.SuspendLayout();
            this.InspectorTab.SuspendLayout();
            this.Animation.SuspendLayout();
            this.GB_Transform.SuspendLayout();
            this.GB_Hierachy.SuspendLayout();
            this.GB_Renderer.SuspendLayout();
            this.SuspendLayout();
            // 
            // PB_DeviceWindow
            // 
            this.PB_DeviceWindow.Location = new System.Drawing.Point(6, 24);
            this.PB_DeviceWindow.Name = "PB_DeviceWindow";
            this.PB_DeviceWindow.Size = new System.Drawing.Size(857, 637);
            this.PB_DeviceWindow.TabIndex = 0;
            this.PB_DeviceWindow.TabStop = false;
            this.PB_DeviceWindow.MouseClick += new System.Windows.Forms.MouseEventHandler(this.Scene_Click);
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.label11.ForeColor = System.Drawing.Color.Black;
            this.label11.Location = new System.Drawing.Point(6, 581);
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
            this.CB_LayerList.Location = new System.Drawing.Point(57, 578);
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
            this.label10.Location = new System.Drawing.Point(16, 548);
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
            this.TB_ObjectTag.Location = new System.Drawing.Point(57, 545);
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
            this.Btn_ObjectCreate.Size = new System.Drawing.Size(95, 28);
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
            this.GB_Scene.Size = new System.Drawing.Size(863, 667);
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
            this.Inspector.Location = new System.Drawing.Point(1389, 12);
            this.Inspector.Name = "Inspector";
            this.Inspector.Size = new System.Drawing.Size(301, 761);
            this.Inspector.TabIndex = 8;
            this.Inspector.TabStop = false;
            this.Inspector.Text = "Inspector";
            // 
            // InspectorTab
            // 
            this.InspectorTab.Controls.Add(this.Animation);
            this.InspectorTab.Controls.Add(this.Component);
            this.InspectorTab.Controls.Add(this.Effect);
            this.InspectorTab.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.InspectorTab.Location = new System.Drawing.Point(6, 24);
            this.InspectorTab.Multiline = true;
            this.InspectorTab.Name = "InspectorTab";
            this.InspectorTab.SelectedIndex = 0;
            this.InspectorTab.Size = new System.Drawing.Size(295, 731);
            this.InspectorTab.TabIndex = 0;
            // 
            // Animation
            // 
            this.Animation.BackColor = System.Drawing.SystemColors.Control;
            this.Animation.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
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
            this.Animation.Location = new System.Drawing.Point(4, 26);
            this.Animation.Name = "Animation";
            this.Animation.Padding = new System.Windows.Forms.Padding(3);
            this.Animation.Size = new System.Drawing.Size(287, 701);
            this.Animation.TabIndex = 0;
            this.Animation.Text = "Animation";
            // 
            // DeleteClipBtn
            // 
            this.DeleteClipBtn.Location = new System.Drawing.Point(167, 532);
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
            this.DeleteClipText.Location = new System.Drawing.Point(5, 533);
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
            this.ClipList.Size = new System.Drawing.Size(272, 293);
            this.ClipList.TabIndex = 27;
            this.ClipList.SelectedIndexChanged += new System.EventHandler(this.ClipList_SelectedIndexChanged);
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
            // ModifyClipBtn
            // 
            this.ModifyClipBtn.Location = new System.Drawing.Point(92, 306);
            this.ModifyClipBtn.Name = "ModifyClipBtn";
            this.ModifyClipBtn.Size = new System.Drawing.Size(73, 48);
            this.ModifyClipBtn.TabIndex = 25;
            this.ModifyClipBtn.Text = "Modify Clip";
            this.ModifyClipBtn.UseVisualStyleBackColor = true;
            this.ModifyClipBtn.Click += new System.EventHandler(this.ModifyClipBtn_Click);
            // 
            // AddClipBtn
            // 
            this.AddClipBtn.Location = new System.Drawing.Point(6, 306);
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
            this.AniTimeBox.Location = new System.Drawing.Point(129, 447);
            this.AniTimeBox.Name = "AniTimeBox";
            this.AniTimeBox.Size = new System.Drawing.Size(149, 25);
            this.AniTimeBox.TabIndex = 24;
            this.AniTimeBox.TextChanged += new System.EventHandler(this.AniTimeBox_TextChanged);
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
            // AniStartFrameBox
            // 
            this.AniStartFrameBox.BackColor = System.Drawing.SystemColors.Control;
            this.AniStartFrameBox.Location = new System.Drawing.Point(130, 394);
            this.AniStartFrameBox.Name = "AniStartFrameBox";
            this.AniStartFrameBox.Size = new System.Drawing.Size(148, 25);
            this.AniStartFrameBox.TabIndex = 22;
            this.AniStartFrameBox.TextChanged += new System.EventHandler(this.AniStartFrameBox_TextChanged);
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
            // label16
            // 
            this.label16.AutoSize = true;
            this.label16.Location = new System.Drawing.Point(3, 477);
            this.label16.Name = "label16";
            this.label16.Size = new System.Drawing.Size(121, 17);
            this.label16.TabIndex = 20;
            this.label16.Text = "Animation Option";
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
            // label14
            // 
            this.label14.AutoSize = true;
            this.label14.Location = new System.Drawing.Point(50, 423);
            this.label14.Name = "label14";
            this.label14.Size = new System.Drawing.Size(74, 17);
            this.label14.TabIndex = 18;
            this.label14.Text = "End Frame";
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
            // label12
            // 
            this.label12.AutoSize = true;
            this.label12.Location = new System.Drawing.Point(80, 368);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(44, 17);
            this.label12.TabIndex = 12;
            this.label12.Text = "Name";
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
            // Component
            // 
            this.Component.BackColor = System.Drawing.SystemColors.Control;
            this.Component.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.Component.ForeColor = System.Drawing.SystemColors.ControlText;
            this.Component.Location = new System.Drawing.Point(4, 26);
            this.Component.Name = "Component";
            this.Component.Padding = new System.Windows.Forms.Padding(3);
            this.Component.Size = new System.Drawing.Size(287, 701);
            this.Component.TabIndex = 1;
            this.Component.Text = "Component";
            // 
            // Effect
            // 
            this.Effect.BackColor = System.Drawing.SystemColors.Control;
            this.Effect.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.Effect.ForeColor = System.Drawing.SystemColors.ControlText;
            this.Effect.Location = new System.Drawing.Point(4, 26);
            this.Effect.Name = "Effect";
            this.Effect.Padding = new System.Windows.Forms.Padding(3);
            this.Effect.Size = new System.Drawing.Size(287, 701);
            this.Effect.TabIndex = 2;
            this.Effect.Text = "Effect";
            // 
            // LB_MeshList
            // 
            this.LB_MeshList.BackColor = System.Drawing.SystemColors.Control;
            this.LB_MeshList.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.LB_MeshList.FormattingEnabled = true;
            this.LB_MeshList.HorizontalScrollbar = true;
            this.LB_MeshList.ItemHeight = 17;
            this.LB_MeshList.Location = new System.Drawing.Point(6, 36);
            this.LB_MeshList.Name = "LB_MeshList";
            this.LB_MeshList.Size = new System.Drawing.Size(279, 225);
            this.LB_MeshList.TabIndex = 1;
            this.LB_MeshList.SelectedIndexChanged += new System.EventHandler(this.MeshList_SelectedIndexChanged);
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
            this.GB_Transform.Controls.Add(this.Label_Position);
            this.GB_Transform.Controls.Add(this.Label_PosZ);
            this.GB_Transform.Controls.Add(this.PositionY);
            this.GB_Transform.Controls.Add(this.Label_PosX);
            this.GB_Transform.Controls.Add(this.Label_PosY);
            this.GB_Transform.Controls.Add(this.PositionX);
            this.GB_Transform.Font = new System.Drawing.Font("맑은 고딕", 10F, System.Drawing.FontStyle.Bold);
            this.GB_Transform.ForeColor = System.Drawing.Color.Red;
            this.GB_Transform.Location = new System.Drawing.Point(1092, 12);
            this.GB_Transform.Name = "GB_Transform";
            this.GB_Transform.Size = new System.Drawing.Size(291, 187);
            this.GB_Transform.TabIndex = 0;
            this.GB_Transform.TabStop = false;
            this.GB_Transform.Text = "Transform";
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
            this.PositionZ.TextChanged += new System.EventHandler(this.ChangePosition);
            this.PositionZ.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.KeyPressCheck);
            // 
            // Label_Position
            // 
            this.Label_Position.AutoSize = true;
            this.Label_Position.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.Label_Position.ForeColor = System.Drawing.Color.Black;
            this.Label_Position.Location = new System.Drawing.Point(11, 24);
            this.Label_Position.Name = "Label_Position";
            this.Label_Position.Size = new System.Drawing.Size(59, 17);
            this.Label_Position.TabIndex = 0;
            this.Label_Position.Text = "Position";
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
            this.GB_Hierachy.Controls.Add(this.CheckBox_Gizmos);
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
            this.GB_Hierachy.Size = new System.Drawing.Size(206, 674);
            this.GB_Hierachy.TabIndex = 9;
            this.GB_Hierachy.TabStop = false;
            this.GB_Hierachy.Text = "Hierachy";
            // 
            // CheckBox_Gizmos
            // 
            this.CheckBox_Gizmos.AutoSize = true;
            this.CheckBox_Gizmos.ForeColor = System.Drawing.Color.Black;
            this.CheckBox_Gizmos.Location = new System.Drawing.Point(9, 612);
            this.CheckBox_Gizmos.Name = "CheckBox_Gizmos";
            this.CheckBox_Gizmos.Size = new System.Drawing.Size(113, 21);
            this.CheckBox_Gizmos.TabIndex = 8;
            this.CheckBox_Gizmos.Text = "Add Gizmos ?";
            this.CheckBox_Gizmos.UseVisualStyleBackColor = true;
            // 
            // Btn_ObjectDelete
            // 
            this.Btn_ObjectDelete.Font = new System.Drawing.Font("맑은 고딕", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.Btn_ObjectDelete.ForeColor = System.Drawing.Color.Black;
            this.Btn_ObjectDelete.Location = new System.Drawing.Point(105, 639);
            this.Btn_ObjectDelete.Name = "Btn_ObjectDelete";
            this.Btn_ObjectDelete.Size = new System.Drawing.Size(95, 28);
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
            this.LB_ObjectList.Size = new System.Drawing.Size(195, 514);
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
            this.LB_LogMessage.Size = new System.Drawing.Size(863, 88);
            this.LB_LogMessage.TabIndex = 12;
            // 
            // Btn_SetAnimation
            // 
            this.Btn_SetAnimation.ForeColor = System.Drawing.Color.Black;
            this.Btn_SetAnimation.Location = new System.Drawing.Point(127, 531);
            this.Btn_SetAnimation.Name = "Btn_SetAnimation";
            this.Btn_SetAnimation.Size = new System.Drawing.Size(158, 31);
            this.Btn_SetAnimation.TabIndex = 4;
            this.Btn_SetAnimation.Text = "Animation Component";
            this.Btn_SetAnimation.UseVisualStyleBackColor = true;
            this.Btn_SetAnimation.Click += new System.EventHandler(this.AnimationBtn_Click);
            // 
            // Btn_AddComponent
            // 
            this.Btn_AddComponent.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.Btn_AddComponent.ForeColor = System.Drawing.Color.Black;
            this.Btn_AddComponent.Location = new System.Drawing.Point(6, 265);
            this.Btn_AddComponent.Name = "Btn_AddComponent";
            this.Btn_AddComponent.Size = new System.Drawing.Size(162, 31);
            this.Btn_AddComponent.TabIndex = 3;
            this.Btn_AddComponent.Text = "Render Component";
            this.Btn_AddComponent.UseVisualStyleBackColor = true;
            this.Btn_AddComponent.Click += new System.EventHandler(this.AddRenderComponent);
            // 
            // Label_MeshList
            // 
            this.Label_MeshList.AutoSize = true;
            this.Label_MeshList.ForeColor = System.Drawing.Color.Black;
            this.Label_MeshList.Location = new System.Drawing.Point(3, 19);
            this.Label_MeshList.Name = "Label_MeshList";
            this.Label_MeshList.Size = new System.Drawing.Size(69, 17);
            this.Label_MeshList.TabIndex = 5;
            this.Label_MeshList.Text = "Mesh List";
            // 
            // Btn_AddMesh
            // 
            this.Btn_AddMesh.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.Btn_AddMesh.ForeColor = System.Drawing.Color.Black;
            this.Btn_AddMesh.Location = new System.Drawing.Point(174, 265);
            this.Btn_AddMesh.Name = "Btn_AddMesh";
            this.Btn_AddMesh.Size = new System.Drawing.Size(111, 31);
            this.Btn_AddMesh.TabIndex = 2;
            this.Btn_AddMesh.Text = "Set Mesh";
            this.Btn_AddMesh.UseVisualStyleBackColor = true;
            this.Btn_AddMesh.Click += new System.EventHandler(this.SetMesh_Click);
            // 
            // GB_Renderer
            // 
            this.GB_Renderer.Controls.Add(this.Btn_SetAnimation);
            this.GB_Renderer.Controls.Add(this.label1);
            this.GB_Renderer.Controls.Add(this.LB_AniList);
            this.GB_Renderer.Controls.Add(this.LB_MeshList);
            this.GB_Renderer.Controls.Add(this.Btn_AddMesh);
            this.GB_Renderer.Controls.Add(this.Label_MeshList);
            this.GB_Renderer.Controls.Add(this.Btn_AddComponent);
            this.GB_Renderer.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.GB_Renderer.ForeColor = System.Drawing.Color.Red;
            this.GB_Renderer.Location = new System.Drawing.Point(1092, 205);
            this.GB_Renderer.Name = "GB_Renderer";
            this.GB_Renderer.Size = new System.Drawing.Size(291, 568);
            this.GB_Renderer.TabIndex = 0;
            this.GB_Renderer.TabStop = false;
            this.GB_Renderer.Text = "Renderer";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.ForeColor = System.Drawing.Color.Black;
            this.label1.Location = new System.Drawing.Point(3, 302);
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
            this.LB_AniList.Location = new System.Drawing.Point(6, 319);
            this.LB_AniList.Name = "LB_AniList";
            this.LB_AniList.Size = new System.Drawing.Size(279, 208);
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
            // ClearCount
            // 
            this.ClearCount.AutoSize = true;
            this.ClearCount.BackColor = System.Drawing.SystemColors.Window;
            this.ClearCount.Font = new System.Drawing.Font("맑은 고딕", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.ClearCount.ForeColor = System.Drawing.Color.Red;
            this.ClearCount.Location = new System.Drawing.Point(123, 18);
            this.ClearCount.Name = "ClearCount";
            this.ClearCount.Size = new System.Drawing.Size(23, 25);
            this.ClearCount.TabIndex = 14;
            this.ClearCount.Text = "0";
            // 
            // EditorForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.SystemColors.Control;
            this.ClientSize = new System.Drawing.Size(1694, 779);
            this.Controls.Add(this.ClearCount);
            this.Controls.Add(this.Btn_AllClear);
            this.Controls.Add(this.GB_Hierachy);
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
            this.GB_Transform.ResumeLayout(false);
            this.GB_Transform.PerformLayout();
            this.GB_Hierachy.ResumeLayout(false);
            this.GB_Hierachy.PerformLayout();
            this.GB_Renderer.ResumeLayout(false);
            this.GB_Renderer.PerformLayout();
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
        private System.Windows.Forms.Label Label_Position;
        private System.Windows.Forms.Button Btn_SetAnimation;
        private System.Windows.Forms.Button Btn_AddComponent;
        private System.Windows.Forms.Label Label_MeshList;
        private System.Windows.Forms.Button Btn_AddMesh;
        private System.Windows.Forms.GroupBox GB_Renderer;
        private System.Windows.Forms.Button Btn_ObjectDelete;
        private System.Windows.Forms.ListBox LB_AniList;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.CheckBox CheckBox_Gizmos;
        private System.Windows.Forms.Button Btn_AllClear;
        private System.Windows.Forms.Label ClearCount;
    }
}

