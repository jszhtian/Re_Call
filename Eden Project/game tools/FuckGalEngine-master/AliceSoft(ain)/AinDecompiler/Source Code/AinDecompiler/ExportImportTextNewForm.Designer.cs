namespace AinDecompiler
{
    partial class ExportImportTextNewForm
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
            this.useWordWrappingCheckBox = new System.Windows.Forms.CheckBox();
            this.wordWrapSettingsButton = new System.Windows.Forms.Button();
            this.exportMessagesAndStringsButton = new System.Windows.Forms.Button();
            this.importMessagesAndStringsButton = new System.Windows.Forms.Button();
            this.migratePatchButton = new System.Windows.Forms.Button();
            this.exportMatchingStringsButton = new System.Windows.Forms.Button();
            this.ExportMessagesButton = new System.Windows.Forms.Button();
            this.checkBox1 = new System.Windows.Forms.CheckBox();
            this.comboBox1 = new System.Windows.Forms.ComboBox();
            this.SuspendLayout();
            // 
            // useWordWrappingCheckBox
            // 
            this.useWordWrappingCheckBox.AutoSize = true;
            this.useWordWrappingCheckBox.Checked = true;
            this.useWordWrappingCheckBox.CheckState = System.Windows.Forms.CheckState.Checked;
            this.useWordWrappingCheckBox.Location = new System.Drawing.Point(12, 12);
            this.useWordWrappingCheckBox.Name = "useWordWrappingCheckBox";
            this.useWordWrappingCheckBox.Size = new System.Drawing.Size(123, 17);
            this.useWordWrappingCheckBox.TabIndex = 0;
            this.useWordWrappingCheckBox.Text = "Use Word Wrapping";
            this.useWordWrappingCheckBox.UseVisualStyleBackColor = true;
            // 
            // wordWrapSettingsButton
            // 
            this.wordWrapSettingsButton.Location = new System.Drawing.Point(141, 8);
            this.wordWrapSettingsButton.Name = "wordWrapSettingsButton";
            this.wordWrapSettingsButton.Size = new System.Drawing.Size(122, 23);
            this.wordWrapSettingsButton.TabIndex = 1;
            this.wordWrapSettingsButton.Text = "Word Wrap Settings...";
            this.wordWrapSettingsButton.UseVisualStyleBackColor = true;
            this.wordWrapSettingsButton.Click += new System.EventHandler(this.wordWrapSettingsButton_Click);
            // 
            // exportMessagesAndStringsButton
            // 
            this.exportMessagesAndStringsButton.Location = new System.Drawing.Point(12, 35);
            this.exportMessagesAndStringsButton.Name = "exportMessagesAndStringsButton";
            this.exportMessagesAndStringsButton.Size = new System.Drawing.Size(251, 23);
            this.exportMessagesAndStringsButton.TabIndex = 2;
            this.exportMessagesAndStringsButton.Text = "Export Messages and Strings...";
            this.exportMessagesAndStringsButton.UseVisualStyleBackColor = true;
            this.exportMessagesAndStringsButton.Click += new System.EventHandler(this.exportMessagesAndStringsButton_Click);
            // 
            // importMessagesAndStringsButton
            // 
            this.importMessagesAndStringsButton.Location = new System.Drawing.Point(12, 93);
            this.importMessagesAndStringsButton.Name = "importMessagesAndStringsButton";
            this.importMessagesAndStringsButton.Size = new System.Drawing.Size(251, 23);
            this.importMessagesAndStringsButton.TabIndex = 3;
            this.importMessagesAndStringsButton.Text = "Import Messages and Strings...";
            this.importMessagesAndStringsButton.UseVisualStyleBackColor = true;
            this.importMessagesAndStringsButton.Click += new System.EventHandler(this.importMessagesAndStringsButton_Click);
            // 
            // migratePatchButton
            // 
            this.migratePatchButton.Location = new System.Drawing.Point(12, 122);
            this.migratePatchButton.Name = "migratePatchButton";
            this.migratePatchButton.Size = new System.Drawing.Size(251, 23);
            this.migratePatchButton.TabIndex = 4;
            this.migratePatchButton.Text = "Migrate patch to a new version of the Game...";
            this.migratePatchButton.UseVisualStyleBackColor = true;
            // 
            // exportMatchingStringsButton
            // 
            this.exportMatchingStringsButton.Location = new System.Drawing.Point(12, 64);
            this.exportMatchingStringsButton.Name = "exportMatchingStringsButton";
            this.exportMatchingStringsButton.Size = new System.Drawing.Size(251, 23);
            this.exportMatchingStringsButton.TabIndex = 5;
            this.exportMatchingStringsButton.Text = "Export Matching Strings...";
            this.exportMatchingStringsButton.UseVisualStyleBackColor = true;
            this.exportMatchingStringsButton.Click += new System.EventHandler(this.exportMatchingStringsButton_Click);
            // 
            // ExportMessagesButton
            // 
            this.ExportMessagesButton.Location = new System.Drawing.Point(12, 151);
            this.ExportMessagesButton.Name = "ExportMessagesButton";
            this.ExportMessagesButton.Size = new System.Drawing.Size(251, 23);
            this.ExportMessagesButton.TabIndex = 6;
            this.ExportMessagesButton.Text = "Export Messages to Multiple Files...";
            this.ExportMessagesButton.UseVisualStyleBackColor = true;
            this.ExportMessagesButton.Click += new System.EventHandler(this.ExportMessagesButton_Click);
            // 
            // checkBox1
            // 
            this.checkBox1.AutoSize = true;
            this.checkBox1.Location = new System.Drawing.Point(12, 182);
            this.checkBox1.Name = "checkBox1";
            this.checkBox1.Size = new System.Drawing.Size(120, 17);
            this.checkBox1.TabIndex = 7;
            this.checkBox1.Text = "Annotate with enum";
            this.checkBox1.UseVisualStyleBackColor = true;
            // 
            // comboBox1
            // 
            this.comboBox1.FormattingEnabled = true;
            this.comboBox1.Location = new System.Drawing.Point(138, 180);
            this.comboBox1.Name = "comboBox1";
            this.comboBox1.Size = new System.Drawing.Size(121, 21);
            this.comboBox1.TabIndex = 8;
            // 
            // ExportImportTextNewForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(275, 247);
            this.Controls.Add(this.comboBox1);
            this.Controls.Add(this.checkBox1);
            this.Controls.Add(this.ExportMessagesButton);
            this.Controls.Add(this.exportMatchingStringsButton);
            this.Controls.Add(this.migratePatchButton);
            this.Controls.Add(this.importMessagesAndStringsButton);
            this.Controls.Add(this.exportMessagesAndStringsButton);
            this.Controls.Add(this.wordWrapSettingsButton);
            this.Controls.Add(this.useWordWrappingCheckBox);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.Name = "ExportImportTextNewForm";
            this.Text = "Import/Export Text";
            this.Load += new System.EventHandler(this.ExportImportTextNewForm_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.CheckBox useWordWrappingCheckBox;
        private System.Windows.Forms.Button wordWrapSettingsButton;
        private System.Windows.Forms.Button exportMessagesAndStringsButton;
        private System.Windows.Forms.Button importMessagesAndStringsButton;
        private System.Windows.Forms.Button migratePatchButton;
        private System.Windows.Forms.Button exportMatchingStringsButton;
        private System.Windows.Forms.Button ExportMessagesButton;
        private System.Windows.Forms.CheckBox checkBox1;
        private System.Windows.Forms.ComboBox comboBox1;
    }
}