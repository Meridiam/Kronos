namespace USBCam
{
    partial class FUSBCam
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.BOK = new System.Windows.Forms.Button();
            this.BCancel = new System.Windows.Forms.Button();
            this.GBROI = new System.Windows.Forms.GroupBox();
            this.LStartY = new System.Windows.Forms.Label();
            this.LStartX = new System.Windows.Forms.Label();
            this.NUpDownY = new System.Windows.Forms.NumericUpDown();
            this.NUpDownX = new System.Windows.Forms.NumericUpDown();
            this.GBColorGains = new System.Windows.Forms.GroupBox();
            this.CBBlackWhite = new System.Windows.Forms.CheckBox();
            this.CBAutoWhiteBalance = new System.Windows.Forms.CheckBox();
            this.LBlue = new System.Windows.Forms.Label();
            this.NUpDownBlue = new System.Windows.Forms.NumericUpDown();
            this.LGreen = new System.Windows.Forms.Label();
            this.NUpDownGreen = new System.Windows.Forms.NumericUpDown();
            this.LRed = new System.Windows.Forms.Label();
            this.NUpDownRed = new System.Windows.Forms.NumericUpDown();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.LLEDIntensity1 = new System.Windows.Forms.Label();
            this.TBLightIntensity1 = new System.Windows.Forms.TextBox();
            this.LLEDBright = new System.Windows.Forms.Label();
            this.LLedDim = new System.Windows.Forms.Label();
            this.SBLightIntensity1 = new System.Windows.Forms.TrackBar();
            this.CBShowAutoExpRegion = new System.Windows.Forms.CheckBox();
            this.CBAutoExposure = new System.Windows.Forms.CheckBox();
            this.NUpDownExposureTime = new System.Windows.Forms.NumericUpDown();
            this.label1 = new System.Windows.Forms.Label();
            this.GBFrameGrab = new System.Windows.Forms.GroupBox();
            this.BLiveImage = new System.Windows.Forms.Button();
            this.CBContSnap = new System.Windows.Forms.CheckBox();
            this.BGetCurrentFrame = new System.Windows.Forms.Button();
            this.LResolution = new System.Windows.Forms.Label();
            this.CBResolution = new System.Windows.Forms.ComboBox();
            this.BContFG = new System.Windows.Forms.Button();
            this.CBCameraSelect = new System.Windows.Forms.ComboBox();
            this.LCamera = new System.Windows.Forms.Label();
            this.GBCameraSelect = new System.Windows.Forms.GroupBox();
            this.TBSerialNo = new System.Windows.Forms.TextBox();
            this.TBModuleNo = new System.Windows.Forms.TextBox();
            this.LSerialNo = new System.Windows.Forms.Label();
            this.LModuleNo = new System.Windows.Forms.Label();
            this.PBImage = new System.Windows.Forms.PictureBox();
            this.BSaveImage = new System.Windows.Forms.Button();
            this.timerContFG = new System.Windows.Forms.Timer(this.components);
            this.Ppic = new System.Windows.Forms.Panel();
            this.SFDsaveImage = new System.Windows.Forms.SaveFileDialog();
            this.BSaveSettings = new System.Windows.Forms.Button();
            this.CallBackMessageLabel = new System.Windows.Forms.Label();
            this.timer1 = new System.Windows.Forms.Timer(this.components);
            this.GBROI.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.NUpDownY)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.NUpDownX)).BeginInit();
            this.GBColorGains.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.NUpDownBlue)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.NUpDownGreen)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.NUpDownRed)).BeginInit();
            this.groupBox1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.SBLightIntensity1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.NUpDownExposureTime)).BeginInit();
            this.GBFrameGrab.SuspendLayout();
            this.GBCameraSelect.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.PBImage)).BeginInit();
            this.Ppic.SuspendLayout();
            this.SuspendLayout();
            // 
            // BOK
            // 
            this.BOK.Location = new System.Drawing.Point(351, 548);
            this.BOK.Name = "BOK";
            this.BOK.Size = new System.Drawing.Size(90, 23);
            this.BOK.TabIndex = 0;
            this.BOK.Text = "OK";
            this.BOK.UseVisualStyleBackColor = true;
            this.BOK.Click += new System.EventHandler(this.BOK_Click);
            // 
            // BCancel
            // 
            this.BCancel.Location = new System.Drawing.Point(455, 548);
            this.BCancel.Name = "BCancel";
            this.BCancel.Size = new System.Drawing.Size(90, 23);
            this.BCancel.TabIndex = 1;
            this.BCancel.Text = "Cancel";
            this.BCancel.UseVisualStyleBackColor = true;
            this.BCancel.Click += new System.EventHandler(this.BCancel_Click);
            // 
            // GBROI
            // 
            this.GBROI.Controls.Add(this.LStartY);
            this.GBROI.Controls.Add(this.LStartX);
            this.GBROI.Controls.Add(this.NUpDownY);
            this.GBROI.Controls.Add(this.NUpDownX);
            this.GBROI.Location = new System.Drawing.Point(30, 216);
            this.GBROI.Name = "GBROI";
            this.GBROI.Size = new System.Drawing.Size(304, 50);
            this.GBROI.TabIndex = 2;
            this.GBROI.TabStop = false;
            this.GBROI.Text = "Region of Interest";
            // 
            // LStartY
            // 
            this.LStartY.AutoSize = true;
            this.LStartY.Location = new System.Drawing.Point(104, 24);
            this.LStartY.Name = "LStartY";
            this.LStartY.Size = new System.Drawing.Size(39, 13);
            this.LStartY.TabIndex = 3;
            this.LStartY.Text = "Start Y";
            // 
            // LStartX
            // 
            this.LStartX.AutoSize = true;
            this.LStartX.Location = new System.Drawing.Point(12, 24);
            this.LStartX.Name = "LStartX";
            this.LStartX.Size = new System.Drawing.Size(39, 13);
            this.LStartX.TabIndex = 2;
            this.LStartX.Text = "Start X";
            // 
            // NUpDownY
            // 
            this.NUpDownY.Location = new System.Drawing.Point(147, 20);
            this.NUpDownY.Name = "NUpDownY";
            this.NUpDownY.Size = new System.Drawing.Size(44, 20);
            this.NUpDownY.TabIndex = 1;
            this.NUpDownY.ValueChanged += new System.EventHandler(this.NUpDownY_ValueChanged);
            // 
            // NUpDownX
            // 
            this.NUpDownX.Location = new System.Drawing.Point(53, 20);
            this.NUpDownX.Name = "NUpDownX";
            this.NUpDownX.Size = new System.Drawing.Size(44, 20);
            this.NUpDownX.TabIndex = 0;
            this.NUpDownX.ValueChanged += new System.EventHandler(this.NUpDownX_ValueChanged);
            // 
            // GBColorGains
            // 
            this.GBColorGains.Controls.Add(this.CBBlackWhite);
            this.GBColorGains.Controls.Add(this.CBAutoWhiteBalance);
            this.GBColorGains.Controls.Add(this.LBlue);
            this.GBColorGains.Controls.Add(this.NUpDownBlue);
            this.GBColorGains.Controls.Add(this.LGreen);
            this.GBColorGains.Controls.Add(this.NUpDownGreen);
            this.GBColorGains.Controls.Add(this.LRed);
            this.GBColorGains.Controls.Add(this.NUpDownRed);
            this.GBColorGains.Location = new System.Drawing.Point(30, 276);
            this.GBColorGains.Name = "GBColorGains";
            this.GBColorGains.Size = new System.Drawing.Size(304, 91);
            this.GBColorGains.TabIndex = 3;
            this.GBColorGains.TabStop = false;
            this.GBColorGains.Text = "Color Gains";
            // 
            // CBBlackWhite
            // 
            this.CBBlackWhite.AutoSize = true;
            this.CBBlackWhite.Location = new System.Drawing.Point(155, 60);
            this.CBBlackWhite.Name = "CBBlackWhite";
            this.CBBlackWhite.Size = new System.Drawing.Size(93, 17);
            this.CBBlackWhite.TabIndex = 7;
            this.CBBlackWhite.Text = "Black && White";
            this.CBBlackWhite.UseVisualStyleBackColor = true;
            this.CBBlackWhite.CheckedChanged += new System.EventHandler(this.CBBlackWhite_CheckedChanged);
            // 
            // CBAutoWhiteBalance
            // 
            this.CBAutoWhiteBalance.AutoSize = true;
            this.CBAutoWhiteBalance.Location = new System.Drawing.Point(18, 60);
            this.CBAutoWhiteBalance.Name = "CBAutoWhiteBalance";
            this.CBAutoWhiteBalance.Size = new System.Drawing.Size(121, 17);
            this.CBAutoWhiteBalance.TabIndex = 6;
            this.CBAutoWhiteBalance.Text = "Auto White Balance";
            this.CBAutoWhiteBalance.UseVisualStyleBackColor = true;
            this.CBAutoWhiteBalance.CheckedChanged += new System.EventHandler(this.CBAutoWhiteBalance_CheckedChanged);
            // 
            // LBlue
            // 
            this.LBlue.AutoSize = true;
            this.LBlue.Location = new System.Drawing.Point(208, 32);
            this.LBlue.Name = "LBlue";
            this.LBlue.Size = new System.Drawing.Size(28, 13);
            this.LBlue.TabIndex = 5;
            this.LBlue.Text = "Blue";
            // 
            // NUpDownBlue
            // 
            this.NUpDownBlue.Location = new System.Drawing.Point(242, 28);
            this.NUpDownBlue.Maximum = new decimal(new int[] {
            128,
            0,
            0,
            0});
            this.NUpDownBlue.Name = "NUpDownBlue";
            this.NUpDownBlue.Size = new System.Drawing.Size(44, 20);
            this.NUpDownBlue.TabIndex = 4;
            this.NUpDownBlue.ValueChanged += new System.EventHandler(this.NUpDownBlue_ValueChanged);
            // 
            // LGreen
            // 
            this.LGreen.AutoSize = true;
            this.LGreen.Location = new System.Drawing.Point(103, 31);
            this.LGreen.Name = "LGreen";
            this.LGreen.Size = new System.Drawing.Size(36, 13);
            this.LGreen.TabIndex = 3;
            this.LGreen.Text = "Green";
            // 
            // NUpDownGreen
            // 
            this.NUpDownGreen.Location = new System.Drawing.Point(147, 28);
            this.NUpDownGreen.Maximum = new decimal(new int[] {
            128,
            0,
            0,
            0});
            this.NUpDownGreen.Name = "NUpDownGreen";
            this.NUpDownGreen.Size = new System.Drawing.Size(44, 20);
            this.NUpDownGreen.TabIndex = 2;
            this.NUpDownGreen.ValueChanged += new System.EventHandler(this.NUpDownGreen_ValueChanged);
            // 
            // LRed
            // 
            this.LRed.AutoSize = true;
            this.LRed.Location = new System.Drawing.Point(15, 31);
            this.LRed.Name = "LRed";
            this.LRed.Size = new System.Drawing.Size(27, 13);
            this.LRed.TabIndex = 1;
            this.LRed.Text = "Red";
            // 
            // NUpDownRed
            // 
            this.NUpDownRed.Location = new System.Drawing.Point(53, 28);
            this.NUpDownRed.Maximum = new decimal(new int[] {
            128,
            0,
            0,
            0});
            this.NUpDownRed.Name = "NUpDownRed";
            this.NUpDownRed.Size = new System.Drawing.Size(44, 20);
            this.NUpDownRed.TabIndex = 0;
            this.NUpDownRed.ValueChanged += new System.EventHandler(this.NUpDownRed_ValueChanged);
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.LLEDIntensity1);
            this.groupBox1.Controls.Add(this.TBLightIntensity1);
            this.groupBox1.Controls.Add(this.LLEDBright);
            this.groupBox1.Controls.Add(this.LLedDim);
            this.groupBox1.Controls.Add(this.SBLightIntensity1);
            this.groupBox1.Controls.Add(this.CBShowAutoExpRegion);
            this.groupBox1.Controls.Add(this.CBAutoExposure);
            this.groupBox1.Controls.Add(this.NUpDownExposureTime);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Location = new System.Drawing.Point(30, 106);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(304, 104);
            this.groupBox1.TabIndex = 4;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Lighting and Exposure";
            // 
            // LLEDIntensity1
            // 
            this.LLEDIntensity1.AutoSize = true;
            this.LLEDIntensity1.Location = new System.Drawing.Point(18, 75);
            this.LLEDIntensity1.Name = "LLEDIntensity1";
            this.LLEDIntensity1.Size = new System.Drawing.Size(70, 13);
            this.LLEDIntensity1.TabIndex = 8;
            this.LLEDIntensity1.Text = "LED Intensity";
            // 
            // TBLightIntensity1
            // 
            this.TBLightIntensity1.Location = new System.Drawing.Point(109, 71);
            this.TBLightIntensity1.Name = "TBLightIntensity1";
            this.TBLightIntensity1.Size = new System.Drawing.Size(36, 20);
            this.TBLightIntensity1.TabIndex = 7;
            // 
            // LLEDBright
            // 
            this.LLEDBright.AutoSize = true;
            this.LLEDBright.Location = new System.Drawing.Point(115, 19);
            this.LLEDBright.Name = "LLEDBright";
            this.LLEDBright.Size = new System.Drawing.Size(34, 13);
            this.LLEDBright.TabIndex = 6;
            this.LLEDBright.Text = "Bright";
            // 
            // LLedDim
            // 
            this.LLedDim.AutoSize = true;
            this.LLedDim.Location = new System.Drawing.Point(15, 19);
            this.LLedDim.Name = "LLedDim";
            this.LLedDim.Size = new System.Drawing.Size(25, 13);
            this.LLedDim.TabIndex = 5;
            this.LLedDim.Text = "Dim";
            // 
            // SBLightIntensity1
            // 
            this.SBLightIntensity1.Location = new System.Drawing.Point(9, 35);
            this.SBLightIntensity1.Maximum = 100;
            this.SBLightIntensity1.Name = "SBLightIntensity1";
            this.SBLightIntensity1.Size = new System.Drawing.Size(144, 45);
            this.SBLightIntensity1.TabIndex = 4;
            this.SBLightIntensity1.TickFrequency = 5;
            this.SBLightIntensity1.Scroll += new System.EventHandler(this.SBLightIntensity1_Scroll);
            // 
            // CBShowAutoExpRegion
            // 
            this.CBShowAutoExpRegion.AutoSize = true;
            this.CBShowAutoExpRegion.Location = new System.Drawing.Point(156, 76);
            this.CBShowAutoExpRegion.Name = "CBShowAutoExpRegion";
            this.CBShowAutoExpRegion.Size = new System.Drawing.Size(136, 17);
            this.CBShowAutoExpRegion.TabIndex = 3;
            this.CBShowAutoExpRegion.Text = "Show Auto Exp Region";
            this.CBShowAutoExpRegion.UseVisualStyleBackColor = true;
            this.CBShowAutoExpRegion.CheckedChanged += new System.EventHandler(this.CBShowAutoExpRegion_CheckedChanged);
            // 
            // CBAutoExposure
            // 
            this.CBAutoExposure.AutoSize = true;
            this.CBAutoExposure.Location = new System.Drawing.Point(156, 53);
            this.CBAutoExposure.Name = "CBAutoExposure";
            this.CBAutoExposure.Size = new System.Drawing.Size(95, 17);
            this.CBAutoExposure.TabIndex = 2;
            this.CBAutoExposure.Text = "Auto Exposure";
            this.CBAutoExposure.UseVisualStyleBackColor = true;
            // 
            // NUpDownExposureTime
            // 
            this.NUpDownExposureTime.Location = new System.Drawing.Point(242, 24);
            this.NUpDownExposureTime.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.NUpDownExposureTime.Name = "NUpDownExposureTime";
            this.NUpDownExposureTime.Size = new System.Drawing.Size(44, 20);
            this.NUpDownExposureTime.TabIndex = 1;
            this.NUpDownExposureTime.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.NUpDownExposureTime.ValueChanged += new System.EventHandler(this.NUpDownExposureTime_ValueChanged);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(169, 28);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(71, 13);
            this.label1.TabIndex = 0;
            this.label1.Text = "Cam Exp (ms)";
            // 
            // GBFrameGrab
            // 
            this.GBFrameGrab.Controls.Add(this.BLiveImage);
            this.GBFrameGrab.Controls.Add(this.CBContSnap);
            this.GBFrameGrab.Controls.Add(this.BGetCurrentFrame);
            this.GBFrameGrab.Controls.Add(this.LResolution);
            this.GBFrameGrab.Controls.Add(this.CBResolution);
            this.GBFrameGrab.Controls.Add(this.BContFG);
            this.GBFrameGrab.Location = new System.Drawing.Point(30, 380);
            this.GBFrameGrab.Name = "GBFrameGrab";
            this.GBFrameGrab.Size = new System.Drawing.Size(304, 116);
            this.GBFrameGrab.TabIndex = 5;
            this.GBFrameGrab.TabStop = false;
            this.GBFrameGrab.Text = "Frame Grab";
            // 
            // BLiveImage
            // 
            this.BLiveImage.Location = new System.Drawing.Point(197, 54);
            this.BLiveImage.Name = "BLiveImage";
            this.BLiveImage.Size = new System.Drawing.Size(89, 23);
            this.BLiveImage.TabIndex = 9;
            this.BLiveImage.Text = "Live Image";
            this.BLiveImage.UseVisualStyleBackColor = true;
            this.BLiveImage.Click += new System.EventHandler(this.BLiveImage_Click);
            // 
            // CBContSnap
            // 
            this.CBContSnap.AutoSize = true;
            this.CBContSnap.Enabled = false;
            this.CBContSnap.Location = new System.Drawing.Point(14, 58);
            this.CBContSnap.Name = "CBContSnap";
            this.CBContSnap.Size = new System.Drawing.Size(107, 17);
            this.CBContSnap.TabIndex = 8;
            this.CBContSnap.Text = "Continuous Snap";
            this.CBContSnap.UseVisualStyleBackColor = true;
            this.CBContSnap.CheckedChanged += new System.EventHandler(this.CBContSnap_CheckedChanged);
            // 
            // BGetCurrentFrame
            // 
            this.BGetCurrentFrame.Location = new System.Drawing.Point(197, 25);
            this.BGetCurrentFrame.Name = "BGetCurrentFrame";
            this.BGetCurrentFrame.Size = new System.Drawing.Size(90, 23);
            this.BGetCurrentFrame.TabIndex = 7;
            this.BGetCurrentFrame.Text = "Snap Image";
            this.BGetCurrentFrame.UseVisualStyleBackColor = true;
            this.BGetCurrentFrame.Click += new System.EventHandler(this.BGetCurrentFrame_Click);
            // 
            // LResolution
            // 
            this.LResolution.AutoSize = true;
            this.LResolution.Location = new System.Drawing.Point(15, 86);
            this.LResolution.Name = "LResolution";
            this.LResolution.Size = new System.Drawing.Size(80, 13);
            this.LResolution.TabIndex = 6;
            this.LResolution.Text = "Resolution (px):";
            // 
            // CBResolution
            // 
            this.CBResolution.FormattingEnabled = true;
            this.CBResolution.Items.AddRange(new object[] {
            "32 x 32",
            "64 x 64",
            "160 x 120",
            "320 x 240",
            "640 x 480",
            "800 x 600",
            "1024 x 768",
            "1280 x 1024",
            "1600 x 1200",
            "2048 x 1536"});
            this.CBResolution.Location = new System.Drawing.Point(102, 82);
            this.CBResolution.MaxDropDownItems = 10;
            this.CBResolution.Name = "CBResolution";
            this.CBResolution.Size = new System.Drawing.Size(84, 21);
            this.CBResolution.TabIndex = 5;
            this.CBResolution.SelectedIndexChanged += new System.EventHandler(this.CBResolution_SelectedIndexChanged);
            // 
            // BContFG
            // 
            this.BContFG.Location = new System.Drawing.Point(15, 25);
            this.BContFG.Name = "BContFG";
            this.BContFG.Size = new System.Drawing.Size(90, 23);
            this.BContFG.TabIndex = 0;
            this.BContFG.Text = "Start Cont FG";
            this.BContFG.UseVisualStyleBackColor = true;
            this.BContFG.Click += new System.EventHandler(this.BContFG_Click);
            // 
            // CBCameraSelect
            // 
            this.CBCameraSelect.FormattingEnabled = true;
            this.CBCameraSelect.Location = new System.Drawing.Point(106, 21);
            this.CBCameraSelect.Name = "CBCameraSelect";
            this.CBCameraSelect.Size = new System.Drawing.Size(85, 21);
            this.CBCameraSelect.TabIndex = 6;
            this.CBCameraSelect.SelectedIndexChanged += new System.EventHandler(this.CBCameraSelect_SelectedIndexChanged);
            // 
            // LCamera
            // 
            this.LCamera.AutoSize = true;
            this.LCamera.Location = new System.Drawing.Point(15, 25);
            this.LCamera.Name = "LCamera";
            this.LCamera.Size = new System.Drawing.Size(71, 13);
            this.LCamera.TabIndex = 7;
            this.LCamera.Text = "USB Camera:";
            // 
            // GBCameraSelect
            // 
            this.GBCameraSelect.Controls.Add(this.TBSerialNo);
            this.GBCameraSelect.Controls.Add(this.TBModuleNo);
            this.GBCameraSelect.Controls.Add(this.LSerialNo);
            this.GBCameraSelect.Controls.Add(this.LModuleNo);
            this.GBCameraSelect.Controls.Add(this.CBCameraSelect);
            this.GBCameraSelect.Controls.Add(this.LCamera);
            this.GBCameraSelect.Location = new System.Drawing.Point(30, 12);
            this.GBCameraSelect.Name = "GBCameraSelect";
            this.GBCameraSelect.Size = new System.Drawing.Size(304, 80);
            this.GBCameraSelect.TabIndex = 8;
            this.GBCameraSelect.TabStop = false;
            this.GBCameraSelect.Text = "Select Camera && Start Engine";
            // 
            // TBSerialNo
            // 
            this.TBSerialNo.Location = new System.Drawing.Point(212, 49);
            this.TBSerialNo.Name = "TBSerialNo";
            this.TBSerialNo.ReadOnly = true;
            this.TBSerialNo.Size = new System.Drawing.Size(85, 20);
            this.TBSerialNo.TabIndex = 11;
            this.TBSerialNo.Text = "N/A";
            // 
            // TBModuleNo
            // 
            this.TBModuleNo.Location = new System.Drawing.Point(76, 49);
            this.TBModuleNo.Name = "TBModuleNo";
            this.TBModuleNo.ReadOnly = true;
            this.TBModuleNo.Size = new System.Drawing.Size(85, 20);
            this.TBModuleNo.TabIndex = 10;
            this.TBModuleNo.Text = "N/A";
            // 
            // LSerialNo
            // 
            this.LSerialNo.AutoSize = true;
            this.LSerialNo.Location = new System.Drawing.Point(160, 54);
            this.LSerialNo.Name = "LSerialNo";
            this.LSerialNo.Size = new System.Drawing.Size(53, 13);
            this.LSerialNo.TabIndex = 9;
            this.LSerialNo.Text = "Serial No.";
            // 
            // LModuleNo
            // 
            this.LModuleNo.AutoSize = true;
            this.LModuleNo.Location = new System.Drawing.Point(15, 54);
            this.LModuleNo.Name = "LModuleNo";
            this.LModuleNo.Size = new System.Drawing.Size(62, 13);
            this.LModuleNo.TabIndex = 8;
            this.LModuleNo.Text = "Module No.";
            // 
            // PBImage
            // 
            this.PBImage.Location = new System.Drawing.Point(3, -1);
            this.PBImage.Name = "PBImage";
            this.PBImage.Size = new System.Drawing.Size(640, 480);
            this.PBImage.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.PBImage.TabIndex = 9;
            this.PBImage.TabStop = false;
            // 
            // BSaveImage
            // 
            this.BSaveImage.Location = new System.Drawing.Point(559, 548);
            this.BSaveImage.Name = "BSaveImage";
            this.BSaveImage.Size = new System.Drawing.Size(90, 23);
            this.BSaveImage.TabIndex = 10;
            this.BSaveImage.Text = "Save Image...";
            this.BSaveImage.UseVisualStyleBackColor = true;
            this.BSaveImage.Click += new System.EventHandler(this.BSaveImage_Click);
            // 
            // timerContFG
            // 
            this.timerContFG.Tick += new System.EventHandler(this.timerContFG_Tick);
            // 
            // Ppic
            // 
            this.Ppic.AutoScroll = true;
            this.Ppic.Controls.Add(this.PBImage);
            this.Ppic.Location = new System.Drawing.Point(347, 16);
            this.Ppic.Name = "Ppic";
            this.Ppic.Size = new System.Drawing.Size(649, 483);
            this.Ppic.TabIndex = 12;
            // 
            // SFDsaveImage
            // 
            this.SFDsaveImage.AddExtension = false;
            this.SFDsaveImage.DefaultExt = "*.jpg";
            this.SFDsaveImage.FileName = "Current Image";
            this.SFDsaveImage.Filter = "JPEG (*.jpg)|*.jpg|Bitmap (*.bmp)|*.bmp|PNG (*.png)|*.png|TIFF (*.tif)|*.tif|All " +
                "files (*.*)|*.*";
            this.SFDsaveImage.InitialDirectory = "\"D:\\RSX Images\"";
            this.SFDsaveImage.Title = "Save Image";
            // 
            // BSaveSettings
            // 
            this.BSaveSettings.Location = new System.Drawing.Point(663, 548);
            this.BSaveSettings.Name = "BSaveSettings";
            this.BSaveSettings.Size = new System.Drawing.Size(90, 23);
            this.BSaveSettings.TabIndex = 13;
            this.BSaveSettings.Text = "Save Settings";
            this.BSaveSettings.UseVisualStyleBackColor = true;
            this.BSaveSettings.Click += new System.EventHandler(this.BSaveSettings_Click);
            // 
            // CallBackMessageLabel
            // 
            this.CallBackMessageLabel.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.CallBackMessageLabel.ImageAlign = System.Drawing.ContentAlignment.TopLeft;
            this.CallBackMessageLabel.Location = new System.Drawing.Point(27, 510);
            this.CallBackMessageLabel.MaximumSize = new System.Drawing.Size(500, 20);
            this.CallBackMessageLabel.Name = "CallBackMessageLabel";
            this.CallBackMessageLabel.Size = new System.Drawing.Size(378, 19);
            this.CallBackMessageLabel.TabIndex = 14;
            this.CallBackMessageLabel.Text = "Call Back Message";
            // 
            // timer1
            // 
            this.timer1.Enabled = true;
            this.timer1.Tick += new System.EventHandler(this.timer1_Tick);
            // 
            // FUSBCam
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1016, 594);
            this.Controls.Add(this.CallBackMessageLabel);
            this.Controls.Add(this.BSaveSettings);
            this.Controls.Add(this.Ppic);
            this.Controls.Add(this.BSaveImage);
            this.Controls.Add(this.GBCameraSelect);
            this.Controls.Add(this.GBFrameGrab);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.GBColorGains);
            this.Controls.Add(this.GBROI);
            this.Controls.Add(this.BCancel);
            this.Controls.Add(this.BOK);
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "FUSBCam";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "USB Camera Configuration";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.FUSBCam_FormClosed);
            this.GBROI.ResumeLayout(false);
            this.GBROI.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.NUpDownY)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.NUpDownX)).EndInit();
            this.GBColorGains.ResumeLayout(false);
            this.GBColorGains.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.NUpDownBlue)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.NUpDownGreen)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.NUpDownRed)).EndInit();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.SBLightIntensity1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.NUpDownExposureTime)).EndInit();
            this.GBFrameGrab.ResumeLayout(false);
            this.GBFrameGrab.PerformLayout();
            this.GBCameraSelect.ResumeLayout(false);
            this.GBCameraSelect.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.PBImage)).EndInit();
            this.Ppic.ResumeLayout(false);
            this.Ppic.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button BOK;
        private System.Windows.Forms.Button BCancel;
        private System.Windows.Forms.GroupBox GBROI;
        private System.Windows.Forms.GroupBox GBColorGains;
        private System.Windows.Forms.Label LStartY;
        private System.Windows.Forms.Label LStartX;
        private System.Windows.Forms.NumericUpDown NUpDownY;
        private System.Windows.Forms.NumericUpDown NUpDownX;
        private System.Windows.Forms.Label LGreen;
        private System.Windows.Forms.NumericUpDown NUpDownGreen;
        private System.Windows.Forms.Label LRed;
        private System.Windows.Forms.NumericUpDown NUpDownRed;
        private System.Windows.Forms.CheckBox CBAutoWhiteBalance;
        private System.Windows.Forms.Label LBlue;
        private System.Windows.Forms.NumericUpDown NUpDownBlue;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.NumericUpDown NUpDownExposureTime;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.CheckBox CBBlackWhite;
        private System.Windows.Forms.CheckBox CBAutoExposure;
        private System.Windows.Forms.CheckBox CBShowAutoExpRegion;
        private System.Windows.Forms.GroupBox GBFrameGrab;
        private System.Windows.Forms.Button BContFG;
        private System.Windows.Forms.ComboBox CBCameraSelect;
        private System.Windows.Forms.Label LCamera;
        private System.Windows.Forms.GroupBox GBCameraSelect;
        private System.Windows.Forms.Label LSerialNo;
        private System.Windows.Forms.Label LModuleNo;
        private System.Windows.Forms.TextBox TBSerialNo;
        private System.Windows.Forms.TextBox TBModuleNo;
        private System.Windows.Forms.PictureBox PBImage;
        private System.Windows.Forms.Label LResolution;
        private System.Windows.Forms.ComboBox CBResolution;
        private System.Windows.Forms.Button BSaveImage;
        private System.Windows.Forms.Timer timerContFG;
        private System.Windows.Forms.Button BGetCurrentFrame;
        private System.Windows.Forms.Panel Ppic;
        private System.Windows.Forms.CheckBox CBContSnap;
        private System.Windows.Forms.SaveFileDialog SFDsaveImage;
        private System.Windows.Forms.TrackBar SBLightIntensity1;
        private System.Windows.Forms.Label LLedDim;
        private System.Windows.Forms.TextBox TBLightIntensity1;
        private System.Windows.Forms.Label LLEDBright;
        private System.Windows.Forms.Label LLEDIntensity1;
        private System.Windows.Forms.Button BLiveImage;
        private System.Windows.Forms.Button BSaveSettings;
        private System.Windows.Forms.Label CallBackMessageLabel;
        private System.Windows.Forms.Timer timer1;
    }
}

