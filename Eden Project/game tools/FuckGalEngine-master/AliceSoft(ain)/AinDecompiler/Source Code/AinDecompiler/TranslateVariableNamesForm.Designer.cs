namespace AinDecompiler
{
    partial class TranslateVariableNamesForm
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
            this.translateNamesRadioButton = new System.Windows.Forms.RadioButton();
            this.exportNamesRadioButton = new System.Windows.Forms.RadioButton();
            this.cancelButton = new System.Windows.Forms.Button();
            this.exportButton = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // translateNamesRadioButton
            // 
            this.translateNamesRadioButton.AutoSize = true;
            this.translateNamesRadioButton.Checked = true;
            this.translateNamesRadioButton.Location = new System.Drawing.Point(12, 12);
            this.translateNamesRadioButton.Name = "translateNamesRadioButton";
            this.translateNamesRadioButton.Size = new System.Drawing.Size(255, 17);
            this.translateNamesRadioButton.TabIndex = 0;
            this.translateNamesRadioButton.TabStop = true;
            this.translateNamesRadioButton.Text = "&Translate variable names with ATLAS and export";
            this.translateNamesRadioButton.UseVisualStyleBackColor = true;
            // 
            // exportNamesRadioButton
            // 
            this.exportNamesRadioButton.AutoSize = true;
            this.exportNamesRadioButton.Location = new System.Drawing.Point(12, 35);
            this.exportNamesRadioButton.Name = "exportNamesRadioButton";
            this.exportNamesRadioButton.Size = new System.Drawing.Size(266, 17);
            this.exportNamesRadioButton.TabIndex = 1;
            this.exportNamesRadioButton.Text = "&Export Japanese variable names without translating";
            this.exportNamesRadioButton.UseVisualStyleBackColor = true;
            // 
            // cancelButton
            // 
            this.cancelButton.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.cancelButton.Location = new System.Drawing.Point(205, 62);
            this.cancelButton.Name = "cancelButton";
            this.cancelButton.Size = new System.Drawing.Size(75, 23);
            this.cancelButton.TabIndex = 3;
            this.cancelButton.Text = "Cancel";
            this.cancelButton.UseVisualStyleBackColor = true;
            // 
            // exportButton
            // 
            this.exportButton.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.exportButton.Location = new System.Drawing.Point(124, 62);
            this.exportButton.Name = "exportButton";
            this.exportButton.Size = new System.Drawing.Size(75, 23);
            this.exportButton.TabIndex = 2;
            this.exportButton.Text = "Export...";
            this.exportButton.UseVisualStyleBackColor = true;
            this.exportButton.Click += new System.EventHandler(this.exportButton_Click);
            // 
            // TranslateVariableNamesForm
            // 
            this.AcceptButton = this.exportButton;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.cancelButton;
            this.ClientSize = new System.Drawing.Size(292, 99);
            this.Controls.Add(this.exportButton);
            this.Controls.Add(this.cancelButton);
            this.Controls.Add(this.exportNamesRadioButton);
            this.Controls.Add(this.translateNamesRadioButton);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.Name = "TranslateVariableNamesForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Translate/Export Variable Names";
            this.Load += new System.EventHandler(this.TranslateVariableNamesForm_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.RadioButton translateNamesRadioButton;
        private System.Windows.Forms.RadioButton exportNamesRadioButton;
        private System.Windows.Forms.Button cancelButton;
        private System.Windows.Forms.Button exportButton;
    }
}