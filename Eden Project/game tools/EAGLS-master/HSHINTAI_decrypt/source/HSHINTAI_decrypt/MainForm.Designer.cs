namespace scriptthingy
{
    partial class MainForm
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
            this.lblScriptPath = new System.Windows.Forms.Label();
            this.scriptFileTextBox = new System.Windows.Forms.TextBox();
            this.openScriptButton = new System.Windows.Forms.Button();
            this.lblPath = new System.Windows.Forms.Label();
            this.pathTextBox = new System.Windows.Forms.TextBox();
            this.openPathButton = new System.Windows.Forms.Button();
            this.extractScriptButton = new System.Windows.Forms.Button();
            this.replaceScriptButton = new System.Windows.Forms.Button();
            this.logTextBox = new System.Windows.Forms.TextBox();
            this.IndexFilePasswordTextBox = new System.Windows.Forms.TextBox();
            this.lblIndexFilePassword = new System.Windows.Forms.Label();
            this.lblDataFilePassword = new System.Windows.Forms.Label();
            this.DataFilePasswordTextBox = new System.Windows.Forms.TextBox();
            this.UseDatFilesCheckBox = new System.Windows.Forms.CheckBox();
            this.SuspendLayout();
            // 
            // lblScriptPath
            // 
            this.lblScriptPath.Location = new System.Drawing.Point(19, 12);
            this.lblScriptPath.Name = "lblScriptPath";
            this.lblScriptPath.Size = new System.Drawing.Size(160, 26);
            this.lblScriptPath.TabIndex = 0;
            this.lblScriptPath.Text = "Path to &Script File (.IDX and .PAK file are loaded together)";
            // 
            // scriptFileTextBox
            // 
            this.scriptFileTextBox.Location = new System.Drawing.Point(185, 14);
            this.scriptFileTextBox.Name = "scriptFileTextBox";
            this.scriptFileTextBox.Size = new System.Drawing.Size(302, 20);
            this.scriptFileTextBox.TabIndex = 1;
            // 
            // openScriptButton
            // 
            this.openScriptButton.Location = new System.Drawing.Point(493, 12);
            this.openScriptButton.Name = "openScriptButton";
            this.openScriptButton.Size = new System.Drawing.Size(75, 23);
            this.openScriptButton.TabIndex = 2;
            this.openScriptButton.Text = "Open...";
            this.openScriptButton.UseVisualStyleBackColor = true;
            this.openScriptButton.Click += new System.EventHandler(this.openScriptButton_Click);
            // 
            // lblPath
            // 
            this.lblPath.AutoSize = true;
            this.lblPath.Location = new System.Drawing.Point(6, 47);
            this.lblPath.Name = "lblPath";
            this.lblPath.Size = new System.Drawing.Size(173, 13);
            this.lblPath.TabIndex = 3;
            this.lblPath.Text = "&Path to hold extracted/edited script";
            // 
            // pathTextBox
            // 
            this.pathTextBox.Location = new System.Drawing.Point(185, 44);
            this.pathTextBox.Name = "pathTextBox";
            this.pathTextBox.Size = new System.Drawing.Size(302, 20);
            this.pathTextBox.TabIndex = 4;
            // 
            // openPathButton
            // 
            this.openPathButton.Location = new System.Drawing.Point(493, 42);
            this.openPathButton.Name = "openPathButton";
            this.openPathButton.Size = new System.Drawing.Size(75, 23);
            this.openPathButton.TabIndex = 5;
            this.openPathButton.Text = "Open...";
            this.openPathButton.UseVisualStyleBackColor = true;
            this.openPathButton.Click += new System.EventHandler(this.openPathButton_Click);
            // 
            // extractScriptButton
            // 
            this.extractScriptButton.Location = new System.Drawing.Point(302, 127);
            this.extractScriptButton.Name = "extractScriptButton";
            this.extractScriptButton.Size = new System.Drawing.Size(129, 23);
            this.extractScriptButton.TabIndex = 11;
            this.extractScriptButton.Text = "&Extract Script";
            this.extractScriptButton.UseVisualStyleBackColor = true;
            this.extractScriptButton.Click += new System.EventHandler(this.extractScriptButton_Click);
            // 
            // replaceScriptButton
            // 
            this.replaceScriptButton.Location = new System.Drawing.Point(437, 127);
            this.replaceScriptButton.Name = "replaceScriptButton";
            this.replaceScriptButton.Size = new System.Drawing.Size(131, 23);
            this.replaceScriptButton.TabIndex = 12;
            this.replaceScriptButton.Text = "&Replace Script";
            this.replaceScriptButton.UseVisualStyleBackColor = true;
            this.replaceScriptButton.Click += new System.EventHandler(this.replaceScriptButton_Click);
            // 
            // logTextBox
            // 
            this.logTextBox.Location = new System.Drawing.Point(9, 156);
            this.logTextBox.MaxLength = 2000000;
            this.logTextBox.Multiline = true;
            this.logTextBox.Name = "logTextBox";
            this.logTextBox.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.logTextBox.Size = new System.Drawing.Size(559, 132);
            this.logTextBox.TabIndex = 13;
            // 
            // IndexFilePasswordTextBox
            // 
            this.IndexFilePasswordTextBox.Location = new System.Drawing.Point(185, 75);
            this.IndexFilePasswordTextBox.Name = "IndexFilePasswordTextBox";
            this.IndexFilePasswordTextBox.Size = new System.Drawing.Size(383, 20);
            this.IndexFilePasswordTextBox.TabIndex = 7;
            // 
            // lblIndexFilePassword
            // 
            this.lblIndexFilePassword.Location = new System.Drawing.Point(25, 70);
            this.lblIndexFilePassword.Name = "lblIndexFilePassword";
            this.lblIndexFilePassword.Size = new System.Drawing.Size(154, 28);
            this.lblIndexFilePassword.TabIndex = 6;
            this.lblIndexFilePassword.Text = "Password for &Index File\r\n(used when autodetection fails)";
            // 
            // lblDataFilePassword
            // 
            this.lblDataFilePassword.Location = new System.Drawing.Point(25, 98);
            this.lblDataFilePassword.Name = "lblDataFilePassword";
            this.lblDataFilePassword.Size = new System.Drawing.Size(154, 28);
            this.lblDataFilePassword.TabIndex = 8;
            this.lblDataFilePassword.Text = "Password for &Data File\r\n(used when autodetection fails)";
            // 
            // DataFilePasswordTextBox
            // 
            this.DataFilePasswordTextBox.Location = new System.Drawing.Point(185, 101);
            this.DataFilePasswordTextBox.Name = "DataFilePasswordTextBox";
            this.DataFilePasswordTextBox.Size = new System.Drawing.Size(383, 20);
            this.DataFilePasswordTextBox.TabIndex = 9;
            // 
            // UseDatFilesCheckBox
            // 
            this.UseDatFilesCheckBox.AutoSize = true;
            this.UseDatFilesCheckBox.Location = new System.Drawing.Point(46, 131);
            this.UseDatFilesCheckBox.Name = "UseDatFilesCheckBox";
            this.UseDatFilesCheckBox.Size = new System.Drawing.Size(250, 17);
            this.UseDatFilesCheckBox.TabIndex = 10;
            this.UseDatFilesCheckBox.Text = "Extract/Replace .DAT files instead of .TXT files";
            this.UseDatFilesCheckBox.UseVisualStyleBackColor = true;
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(580, 299);
            this.Controls.Add(this.UseDatFilesCheckBox);
            this.Controls.Add(this.DataFilePasswordTextBox);
            this.Controls.Add(this.lblDataFilePassword);
            this.Controls.Add(this.lblIndexFilePassword);
            this.Controls.Add(this.IndexFilePasswordTextBox);
            this.Controls.Add(this.logTextBox);
            this.Controls.Add(this.replaceScriptButton);
            this.Controls.Add(this.extractScriptButton);
            this.Controls.Add(this.openPathButton);
            this.Controls.Add(this.pathTextBox);
            this.Controls.Add(this.lblPath);
            this.Controls.Add(this.openScriptButton);
            this.Controls.Add(this.scriptFileTextBox);
            this.Controls.Add(this.lblScriptPath);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;
            this.Name = "MainForm";
            this.Text = "HSHINTAI Script Extractor/Inserter";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label lblScriptPath;
        private System.Windows.Forms.TextBox scriptFileTextBox;
        private System.Windows.Forms.Button openScriptButton;
        private System.Windows.Forms.Label lblPath;
        private System.Windows.Forms.TextBox pathTextBox;
        private System.Windows.Forms.Button openPathButton;
        private System.Windows.Forms.Button extractScriptButton;
        private System.Windows.Forms.Button replaceScriptButton;
        private System.Windows.Forms.TextBox logTextBox;
        private System.Windows.Forms.TextBox IndexFilePasswordTextBox;
        private System.Windows.Forms.Label lblIndexFilePassword;
        private System.Windows.Forms.Label lblDataFilePassword;
        private System.Windows.Forms.TextBox DataFilePasswordTextBox;
        private System.Windows.Forms.CheckBox UseDatFilesCheckBox;

    }
}

