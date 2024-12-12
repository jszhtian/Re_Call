
namespace PackTool.GUI
{
    partial class Main
    {
        /// <summary>
        ///  Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        ///  Clean up any resources being used.
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
        ///  Required method for Designer support - do not modify
        ///  the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Main));
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this._Button_Extract = new System.Windows.Forms.Button();
            this._CheckBox_ExtractDecryptScript = new System.Windows.Forms.CheckBox();
            this._TextBox_ExtractScriptKey = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
            this._TextBox_ExtractArchiveKey = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this._Button_BrowseExtractOutput = new System.Windows.Forms.Button();
            this._TextBox_ExtractOutput = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this._Button_BrowseExtractInput = new System.Windows.Forms.Button();
            this._TextBox_ExtractInput = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this._Button_PackReadme = new System.Windows.Forms.Button();
            this._Button_Pack = new System.Windows.Forms.Button();
            this._CheckBox_PackEncryptScript = new System.Windows.Forms.CheckBox();
            this._TextBox_PackScriptKey = new System.Windows.Forms.TextBox();
            this.label6 = new System.Windows.Forms.Label();
            this._TextBox_PackArchiveKey = new System.Windows.Forms.TextBox();
            this.label7 = new System.Windows.Forms.Label();
            this._Button_BrowsePackOutput = new System.Windows.Forms.Button();
            this._TextBox_PackOutput = new System.Windows.Forms.TextBox();
            this.label8 = new System.Windows.Forms.Label();
            this._Button_BrowsePackInput = new System.Windows.Forms.Button();
            this._TextBox_PackInput = new System.Windows.Forms.TextBox();
            this.label9 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.textBox3 = new System.Windows.Forms.TextBox();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this._Button_ExtractReadme = new System.Windows.Forms.Button();
            this.tabControl1.SuspendLayout();
            this.tabPage1.SuspendLayout();
            this.tabPage2.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            this.SuspendLayout();
            // 
            // tabControl1
            // 
            this.tabControl1.Controls.Add(this.tabPage1);
            this.tabControl1.Controls.Add(this.tabPage2);
            this.tabControl1.Location = new System.Drawing.Point(0, 78);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(536, 248);
            this.tabControl1.TabIndex = 0;
            // 
            // tabPage1
            // 
            this.tabPage1.Controls.Add(this._Button_ExtractReadme);
            this.tabPage1.Controls.Add(this._Button_Extract);
            this.tabPage1.Controls.Add(this._CheckBox_ExtractDecryptScript);
            this.tabPage1.Controls.Add(this._TextBox_ExtractScriptKey);
            this.tabPage1.Controls.Add(this.label5);
            this.tabPage1.Controls.Add(this._TextBox_ExtractArchiveKey);
            this.tabPage1.Controls.Add(this.label4);
            this.tabPage1.Controls.Add(this._Button_BrowseExtractOutput);
            this.tabPage1.Controls.Add(this._TextBox_ExtractOutput);
            this.tabPage1.Controls.Add(this.label2);
            this.tabPage1.Controls.Add(this._Button_BrowseExtractInput);
            this.tabPage1.Controls.Add(this._TextBox_ExtractInput);
            this.tabPage1.Controls.Add(this.label1);
            this.tabPage1.Location = new System.Drawing.Point(4, 26);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage1.Size = new System.Drawing.Size(528, 218);
            this.tabPage1.TabIndex = 0;
            this.tabPage1.Text = "解包";
            this.tabPage1.UseVisualStyleBackColor = true;
            // 
            // _Button_Extract
            // 
            this._Button_Extract.Location = new System.Drawing.Point(449, 182);
            this._Button_Extract.Name = "_Button_Extract";
            this._Button_Extract.Size = new System.Drawing.Size(69, 24);
            this._Button_Extract.TabIndex = 11;
            this._Button_Extract.Text = "解包";
            this._Button_Extract.UseVisualStyleBackColor = true;
            this._Button_Extract.Click += new System.EventHandler(this.Button_Extract_Click);
            // 
            // _CheckBox_ExtractDecryptScript
            // 
            this._CheckBox_ExtractDecryptScript.AutoSize = true;
            this._CheckBox_ExtractDecryptScript.Location = new System.Drawing.Point(70, 118);
            this._CheckBox_ExtractDecryptScript.Name = "_CheckBox_ExtractDecryptScript";
            this._CheckBox_ExtractDecryptScript.Size = new System.Drawing.Size(75, 21);
            this._CheckBox_ExtractDecryptScript.TabIndex = 8;
            this._CheckBox_ExtractDecryptScript.Text = "解密脚本";
            this._CheckBox_ExtractDecryptScript.UseVisualStyleBackColor = true;
            this._CheckBox_ExtractDecryptScript.CheckedChanged += new System.EventHandler(this.CheckBox_ExtractDecryptScript_CheckedChanged);
            // 
            // _TextBox_ExtractScriptKey
            // 
            this._TextBox_ExtractScriptKey.Location = new System.Drawing.Point(70, 148);
            this._TextBox_ExtractScriptKey.Name = "_TextBox_ExtractScriptKey";
            this._TextBox_ExtractScriptKey.Size = new System.Drawing.Size(100, 23);
            this._TextBox_ExtractScriptKey.TabIndex = 10;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(9, 151);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(56, 17);
            this.label5.TabIndex = 9;
            this.label5.Text = "脚本密钥";
            // 
            // _TextBox_ExtractArchiveKey
            // 
            this._TextBox_ExtractArchiveKey.Location = new System.Drawing.Point(70, 83);
            this._TextBox_ExtractArchiveKey.Name = "_TextBox_ExtractArchiveKey";
            this._TextBox_ExtractArchiveKey.Size = new System.Drawing.Size(100, 23);
            this._TextBox_ExtractArchiveKey.TabIndex = 7;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(9, 86);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(56, 17);
            this.label4.TabIndex = 6;
            this.label4.Text = "封包密钥";
            // 
            // _Button_BrowseExtractOutput
            // 
            this._Button_BrowseExtractOutput.Location = new System.Drawing.Point(449, 48);
            this._Button_BrowseExtractOutput.Name = "_Button_BrowseExtractOutput";
            this._Button_BrowseExtractOutput.Size = new System.Drawing.Size(69, 24);
            this._Button_BrowseExtractOutput.TabIndex = 5;
            this._Button_BrowseExtractOutput.Text = "浏览...";
            this._Button_BrowseExtractOutput.UseVisualStyleBackColor = true;
            this._Button_BrowseExtractOutput.Click += new System.EventHandler(this.Button_BrowseExtractOutput_Click);
            // 
            // _TextBox_ExtractOutput
            // 
            this._TextBox_ExtractOutput.Location = new System.Drawing.Point(70, 49);
            this._TextBox_ExtractOutput.Name = "_TextBox_ExtractOutput";
            this._TextBox_ExtractOutput.Size = new System.Drawing.Size(373, 23);
            this._TextBox_ExtractOutput.TabIndex = 4;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(9, 52);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(56, 17);
            this.label2.TabIndex = 3;
            this.label2.Text = "输出目录";
            // 
            // _Button_BrowseExtractInput
            // 
            this._Button_BrowseExtractInput.Location = new System.Drawing.Point(449, 13);
            this._Button_BrowseExtractInput.Name = "_Button_BrowseExtractInput";
            this._Button_BrowseExtractInput.Size = new System.Drawing.Size(69, 24);
            this._Button_BrowseExtractInput.TabIndex = 2;
            this._Button_BrowseExtractInput.Text = "浏览...";
            this._Button_BrowseExtractInput.UseVisualStyleBackColor = true;
            this._Button_BrowseExtractInput.Click += new System.EventHandler(this.Button_BrowseExtractInput_Click);
            // 
            // _TextBox_ExtractInput
            // 
            this._TextBox_ExtractInput.Location = new System.Drawing.Point(70, 14);
            this._TextBox_ExtractInput.Name = "_TextBox_ExtractInput";
            this._TextBox_ExtractInput.Size = new System.Drawing.Size(373, 23);
            this._TextBox_ExtractInput.TabIndex = 1;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(9, 17);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(56, 17);
            this.label1.TabIndex = 0;
            this.label1.Text = "封包文件";
            // 
            // tabPage2
            // 
            this.tabPage2.Controls.Add(this._Button_PackReadme);
            this.tabPage2.Controls.Add(this._Button_Pack);
            this.tabPage2.Controls.Add(this._CheckBox_PackEncryptScript);
            this.tabPage2.Controls.Add(this._TextBox_PackScriptKey);
            this.tabPage2.Controls.Add(this.label6);
            this.tabPage2.Controls.Add(this._TextBox_PackArchiveKey);
            this.tabPage2.Controls.Add(this.label7);
            this.tabPage2.Controls.Add(this._Button_BrowsePackOutput);
            this.tabPage2.Controls.Add(this._TextBox_PackOutput);
            this.tabPage2.Controls.Add(this.label8);
            this.tabPage2.Controls.Add(this._Button_BrowsePackInput);
            this.tabPage2.Controls.Add(this._TextBox_PackInput);
            this.tabPage2.Controls.Add(this.label9);
            this.tabPage2.Location = new System.Drawing.Point(4, 26);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage2.Size = new System.Drawing.Size(528, 218);
            this.tabPage2.TabIndex = 1;
            this.tabPage2.Text = "封包";
            this.tabPage2.UseVisualStyleBackColor = true;
            // 
            // _Button_PackReadme
            // 
            this._Button_PackReadme.Location = new System.Drawing.Point(449, 82);
            this._Button_PackReadme.Name = "_Button_PackReadme";
            this._Button_PackReadme.Size = new System.Drawing.Size(69, 24);
            this._Button_PackReadme.TabIndex = 24;
            this._Button_PackReadme.Text = "说明";
            this._Button_PackReadme.UseVisualStyleBackColor = true;
            this._Button_PackReadme.Click += new System.EventHandler(this.Button_Readme_Click);
            // 
            // _Button_Pack
            // 
            this._Button_Pack.Location = new System.Drawing.Point(449, 182);
            this._Button_Pack.Name = "_Button_Pack";
            this._Button_Pack.Size = new System.Drawing.Size(69, 24);
            this._Button_Pack.TabIndex = 23;
            this._Button_Pack.Text = "封包";
            this._Button_Pack.UseVisualStyleBackColor = true;
            this._Button_Pack.Click += new System.EventHandler(this.Button_Pack_Click);
            // 
            // _CheckBox_PackEncryptScript
            // 
            this._CheckBox_PackEncryptScript.AutoSize = true;
            this._CheckBox_PackEncryptScript.Location = new System.Drawing.Point(70, 118);
            this._CheckBox_PackEncryptScript.Name = "_CheckBox_PackEncryptScript";
            this._CheckBox_PackEncryptScript.Size = new System.Drawing.Size(75, 21);
            this._CheckBox_PackEncryptScript.TabIndex = 20;
            this._CheckBox_PackEncryptScript.Text = "加密脚本";
            this._CheckBox_PackEncryptScript.UseVisualStyleBackColor = true;
            this._CheckBox_PackEncryptScript.CheckedChanged += new System.EventHandler(this.CheckBox_PackEncryptScript_CheckedChanged);
            // 
            // _TextBox_PackScriptKey
            // 
            this._TextBox_PackScriptKey.Location = new System.Drawing.Point(70, 148);
            this._TextBox_PackScriptKey.Name = "_TextBox_PackScriptKey";
            this._TextBox_PackScriptKey.Size = new System.Drawing.Size(100, 23);
            this._TextBox_PackScriptKey.TabIndex = 22;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(9, 151);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(56, 17);
            this.label6.TabIndex = 21;
            this.label6.Text = "脚本密钥";
            // 
            // _TextBox_PackArchiveKey
            // 
            this._TextBox_PackArchiveKey.Location = new System.Drawing.Point(70, 83);
            this._TextBox_PackArchiveKey.Name = "_TextBox_PackArchiveKey";
            this._TextBox_PackArchiveKey.Size = new System.Drawing.Size(100, 23);
            this._TextBox_PackArchiveKey.TabIndex = 19;
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(9, 86);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(56, 17);
            this.label7.TabIndex = 18;
            this.label7.Text = "封包密钥";
            // 
            // _Button_BrowsePackOutput
            // 
            this._Button_BrowsePackOutput.Location = new System.Drawing.Point(449, 48);
            this._Button_BrowsePackOutput.Name = "_Button_BrowsePackOutput";
            this._Button_BrowsePackOutput.Size = new System.Drawing.Size(69, 24);
            this._Button_BrowsePackOutput.TabIndex = 17;
            this._Button_BrowsePackOutput.Text = "浏览...";
            this._Button_BrowsePackOutput.UseVisualStyleBackColor = true;
            this._Button_BrowsePackOutput.Click += new System.EventHandler(this.Button_BrowsePackOutput_Click);
            // 
            // _TextBox_PackOutput
            // 
            this._TextBox_PackOutput.Location = new System.Drawing.Point(70, 49);
            this._TextBox_PackOutput.Name = "_TextBox_PackOutput";
            this._TextBox_PackOutput.Size = new System.Drawing.Size(373, 23);
            this._TextBox_PackOutput.TabIndex = 16;
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(9, 52);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(56, 17);
            this.label8.TabIndex = 15;
            this.label8.Text = "输出文件";
            // 
            // _Button_BrowsePackInput
            // 
            this._Button_BrowsePackInput.Location = new System.Drawing.Point(449, 13);
            this._Button_BrowsePackInput.Name = "_Button_BrowsePackInput";
            this._Button_BrowsePackInput.Size = new System.Drawing.Size(69, 24);
            this._Button_BrowsePackInput.TabIndex = 14;
            this._Button_BrowsePackInput.Text = "浏览...";
            this._Button_BrowsePackInput.UseVisualStyleBackColor = true;
            this._Button_BrowsePackInput.Click += new System.EventHandler(this.Button_BrowsePackInput_Click);
            // 
            // _TextBox_PackInput
            // 
            this._TextBox_PackInput.Location = new System.Drawing.Point(70, 14);
            this._TextBox_PackInput.Name = "_TextBox_PackInput";
            this._TextBox_PackInput.Size = new System.Drawing.Size(373, 23);
            this._TextBox_PackInput.TabIndex = 13;
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(9, 17);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(56, 17);
            this.label9.TabIndex = 12;
            this.label9.Text = "文件目录";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(-39, 91);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(43, 17);
            this.label3.TabIndex = 3;
            this.label3.Text = "label2";
            // 
            // textBox3
            // 
            this.textBox3.Location = new System.Drawing.Point(10, 88);
            this.textBox3.Name = "textBox3";
            this.textBox3.Size = new System.Drawing.Size(373, 23);
            this.textBox3.TabIndex = 4;
            // 
            // pictureBox1
            // 
            this.pictureBox1.Image = ((System.Drawing.Image)(resources.GetObject("pictureBox1.Image")));
            this.pictureBox1.Location = new System.Drawing.Point(0, 0);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(534, 77);
            this.pictureBox1.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pictureBox1.TabIndex = 1;
            this.pictureBox1.TabStop = false;
            // 
            // _Button_ExtractReadme
            // 
            this._Button_ExtractReadme.Location = new System.Drawing.Point(449, 82);
            this._Button_ExtractReadme.Name = "_Button_ExtractReadme";
            this._Button_ExtractReadme.Size = new System.Drawing.Size(69, 24);
            this._Button_ExtractReadme.TabIndex = 25;
            this._Button_ExtractReadme.Text = "说明";
            this._Button_ExtractReadme.UseVisualStyleBackColor = true;
            this._Button_ExtractReadme.Click += new System.EventHandler(this.Button_Readme_Click);
            // 
            // Main
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 17F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(534, 325);
            this.Controls.Add(this.pictureBox1);
            this.Controls.Add(this.tabControl1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;
            this.Name = "Main";
            this.Text = "NonColor 封包工具";
            this.Load += new System.EventHandler(this.Main_Load);
            this.tabControl1.ResumeLayout(false);
            this.tabPage1.ResumeLayout(false);
            this.tabPage1.PerformLayout();
            this.tabPage2.ResumeLayout(false);
            this.tabPage2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.TabPage tabPage1;
        private System.Windows.Forms.TabPage tabPage2;
        private System.Windows.Forms.CheckBox _CheckBox_ExtractDecryptScript;
        private System.Windows.Forms.TextBox _TextBox_ExtractScriptKey;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.TextBox _TextBox_ExtractArchiveKey;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Button _Button_BrowseExtractOutput;
        private System.Windows.Forms.TextBox _TextBox_ExtractOutput;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button _Button_BrowseExtractInput;
        private System.Windows.Forms.TextBox _TextBox_ExtractInput;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox textBox3;
        private System.Windows.Forms.Button _Button_Extract;
        private System.Windows.Forms.Button _Button_Pack;
        private System.Windows.Forms.CheckBox _CheckBox_PackEncryptScript;
        private System.Windows.Forms.TextBox _TextBox_PackScriptKey;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.TextBox _TextBox_PackArchiveKey;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Button _Button_BrowsePackOutput;
        private System.Windows.Forms.TextBox _TextBox_PackOutput;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Button _Button_BrowsePackInput;
        private System.Windows.Forms.TextBox _TextBox_PackInput;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.Button _Button_PackReadme;
        private System.Windows.Forms.Button _Button_ExtractReadme;
    }
}

