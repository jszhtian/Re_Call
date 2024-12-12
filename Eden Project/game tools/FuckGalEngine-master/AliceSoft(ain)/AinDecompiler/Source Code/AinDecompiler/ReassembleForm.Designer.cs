namespace AinDecompiler
{
    partial class ReassembleForm
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
            this.EncryptCheckBox = new System.Windows.Forms.CheckBox();
            this.BuildProjectButton = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // EncryptCheckBox
            // 
            this.EncryptCheckBox.AutoSize = true;
            this.EncryptCheckBox.Checked = true;
            this.EncryptCheckBox.CheckState = System.Windows.Forms.CheckState.Checked;
            this.EncryptCheckBox.Location = new System.Drawing.Point(12, 41);
            this.EncryptCheckBox.Name = "EncryptCheckBox";
            this.EncryptCheckBox.Size = new System.Drawing.Size(132, 17);
            this.EncryptCheckBox.TabIndex = 5;
            this.EncryptCheckBox.Text = "Encrypt output AIN file";
            this.EncryptCheckBox.UseVisualStyleBackColor = true;
            // 
            // BuildProjectButton
            // 
            this.BuildProjectButton.Location = new System.Drawing.Point(12, 12);
            this.BuildProjectButton.Name = "BuildProjectButton";
            this.BuildProjectButton.Size = new System.Drawing.Size(213, 23);
            this.BuildProjectButton.TabIndex = 3;
            this.BuildProjectButton.Text = "Build Project...";
            this.BuildProjectButton.UseVisualStyleBackColor = true;
            this.BuildProjectButton.Click += new System.EventHandler(this.LoadAINFileButton_Click);
            // 
            // ReassembleForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(263, 74);
            this.Controls.Add(this.EncryptCheckBox);
            this.Controls.Add(this.BuildProjectButton);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;
            this.Name = "ReassembleForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Assemble Project to AIN";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.CheckBox EncryptCheckBox;
        private System.Windows.Forms.Button BuildProjectButton;
    }
}