namespace AinDecompiler
{
    partial class AssembleOptionsForm
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
            this.cancelButton = new System.Windows.Forms.Button();
            this.okButton = new System.Windows.Forms.Button();
            this.EncryptOutputAinCheckBox = new System.Windows.Forms.CheckBox();
            this.OpenGeneratedFileCheckBox = new System.Windows.Forms.CheckBox();
            this.SuspendLayout();
            // 
            // cancelButton
            // 
            this.cancelButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.cancelButton.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.cancelButton.Location = new System.Drawing.Point(205, 68);
            this.cancelButton.Name = "cancelButton";
            this.cancelButton.Size = new System.Drawing.Size(75, 23);
            this.cancelButton.TabIndex = 3;
            this.cancelButton.Text = "Cancel";
            this.cancelButton.UseVisualStyleBackColor = true;
            // 
            // okButton
            // 
            this.okButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.okButton.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.okButton.Location = new System.Drawing.Point(124, 68);
            this.okButton.Name = "okButton";
            this.okButton.Size = new System.Drawing.Size(75, 23);
            this.okButton.TabIndex = 2;
            this.okButton.Text = "OK";
            this.okButton.UseVisualStyleBackColor = true;
            // 
            // EncryptOutputAinCheckBox
            // 
            this.EncryptOutputAinCheckBox.AutoSize = true;
            this.EncryptOutputAinCheckBox.Checked = true;
            this.EncryptOutputAinCheckBox.CheckState = System.Windows.Forms.CheckState.Checked;
            this.EncryptOutputAinCheckBox.Location = new System.Drawing.Point(12, 12);
            this.EncryptOutputAinCheckBox.Name = "EncryptOutputAinCheckBox";
            this.EncryptOutputAinCheckBox.Size = new System.Drawing.Size(252, 17);
            this.EncryptOutputAinCheckBox.TabIndex = 0;
            this.EncryptOutputAinCheckBox.Text = "&Encrypt output AIN file (so the game can read it)";
            this.EncryptOutputAinCheckBox.UseVisualStyleBackColor = true;
            // 
            // OpenGeneratedFileCheckBox
            // 
            this.OpenGeneratedFileCheckBox.AutoSize = true;
            this.OpenGeneratedFileCheckBox.Location = new System.Drawing.Point(12, 35);
            this.OpenGeneratedFileCheckBox.Name = "OpenGeneratedFileCheckBox";
            this.OpenGeneratedFileCheckBox.Size = new System.Drawing.Size(192, 17);
            this.OpenGeneratedFileCheckBox.TabIndex = 1;
            this.OpenGeneratedFileCheckBox.Text = "&Open generated AIN file afterwards";
            this.OpenGeneratedFileCheckBox.UseVisualStyleBackColor = true;
            // 
            // AssembleOptionsForm
            // 
            this.AcceptButton = this.okButton;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.cancelButton;
            this.ClientSize = new System.Drawing.Size(292, 103);
            this.Controls.Add(this.OpenGeneratedFileCheckBox);
            this.Controls.Add(this.EncryptOutputAinCheckBox);
            this.Controls.Add(this.cancelButton);
            this.Controls.Add(this.okButton);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.Name = "AssembleOptionsForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Assemble Project Settings";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button cancelButton;
        private System.Windows.Forms.Button okButton;
        public System.Windows.Forms.CheckBox EncryptOutputAinCheckBox;
        public System.Windows.Forms.CheckBox OpenGeneratedFileCheckBox;
    }
}