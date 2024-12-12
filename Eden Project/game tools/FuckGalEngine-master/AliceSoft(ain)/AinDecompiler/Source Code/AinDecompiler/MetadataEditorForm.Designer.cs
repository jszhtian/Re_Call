namespace AinDecompiler
{
    partial class MetadataEditorForm
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
            this.VariableTextBox = new System.Windows.Forms.TextBox();
            this.lblVariable = new System.Windows.Forms.Label();
            this.lblAlternativeName = new System.Windows.Forms.Label();
            this.AlternativeNameTextBox = new System.Windows.Forms.TextBox();
            this.lblEnumerationType = new System.Windows.Forms.Label();
            this.EnumerationTypeComboBox = new System.Windows.Forms.ComboBox();
            this.lblDefaultValue = new System.Windows.Forms.Label();
            this.DefaultValueTextBox = new System.Windows.Forms.TextBox();
            this.EditEnumerationTypeButton = new System.Windows.Forms.Button();
            this.DescriptionTextBox = new System.Windows.Forms.TextBox();
            this.lblDescription = new System.Windows.Forms.Label();
            this.OkButton = new System.Windows.Forms.Button();
            this.cancelButton = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // VariableTextBox
            // 
            this.VariableTextBox.Location = new System.Drawing.Point(66, 6);
            this.VariableTextBox.Name = "VariableTextBox";
            this.VariableTextBox.ReadOnly = true;
            this.VariableTextBox.Size = new System.Drawing.Size(214, 20);
            this.VariableTextBox.TabIndex = 1;
            // 
            // lblVariable
            // 
            this.lblVariable.AutoSize = true;
            this.lblVariable.Location = new System.Drawing.Point(12, 9);
            this.lblVariable.Name = "lblVariable";
            this.lblVariable.Size = new System.Drawing.Size(48, 13);
            this.lblVariable.TabIndex = 0;
            this.lblVariable.Text = "Variable:";
            // 
            // lblAlternativeName
            // 
            this.lblAlternativeName.AutoSize = true;
            this.lblAlternativeName.Location = new System.Drawing.Point(12, 36);
            this.lblAlternativeName.Name = "lblAlternativeName";
            this.lblAlternativeName.Size = new System.Drawing.Size(89, 13);
            this.lblAlternativeName.TabIndex = 2;
            this.lblAlternativeName.Text = "&Alternative name:";
            // 
            // AlternativeNameTextBox
            // 
            this.AlternativeNameTextBox.Location = new System.Drawing.Point(107, 32);
            this.AlternativeNameTextBox.Name = "AlternativeNameTextBox";
            this.AlternativeNameTextBox.Size = new System.Drawing.Size(173, 20);
            this.AlternativeNameTextBox.TabIndex = 3;
            // 
            // lblEnumerationType
            // 
            this.lblEnumerationType.AutoSize = true;
            this.lblEnumerationType.Location = new System.Drawing.Point(12, 87);
            this.lblEnumerationType.Name = "lblEnumerationType";
            this.lblEnumerationType.Size = new System.Drawing.Size(96, 13);
            this.lblEnumerationType.TabIndex = 6;
            this.lblEnumerationType.Text = "Enumeration &Type:";
            // 
            // EnumerationTypeComboBox
            // 
            this.EnumerationTypeComboBox.FormattingEnabled = true;
            this.EnumerationTypeComboBox.Location = new System.Drawing.Point(114, 84);
            this.EnumerationTypeComboBox.Name = "EnumerationTypeComboBox";
            this.EnumerationTypeComboBox.Size = new System.Drawing.Size(166, 21);
            this.EnumerationTypeComboBox.TabIndex = 7;
            // 
            // lblDefaultValue
            // 
            this.lblDefaultValue.AutoSize = true;
            this.lblDefaultValue.Location = new System.Drawing.Point(12, 61);
            this.lblDefaultValue.Name = "lblDefaultValue";
            this.lblDefaultValue.Size = new System.Drawing.Size(74, 13);
            this.lblDefaultValue.TabIndex = 4;
            this.lblDefaultValue.Text = "Default &Value:";
            // 
            // DefaultValueTextBox
            // 
            this.DefaultValueTextBox.Location = new System.Drawing.Point(92, 58);
            this.DefaultValueTextBox.Name = "DefaultValueTextBox";
            this.DefaultValueTextBox.Size = new System.Drawing.Size(188, 20);
            this.DefaultValueTextBox.TabIndex = 5;
            // 
            // EditEnumerationTypeButton
            // 
            this.EditEnumerationTypeButton.Location = new System.Drawing.Point(114, 111);
            this.EditEnumerationTypeButton.Name = "EditEnumerationTypeButton";
            this.EditEnumerationTypeButton.Size = new System.Drawing.Size(166, 23);
            this.EditEnumerationTypeButton.TabIndex = 8;
            this.EditEnumerationTypeButton.Text = "&Edit Enumeration Type...";
            this.EditEnumerationTypeButton.UseVisualStyleBackColor = true;
            // 
            // DescriptionTextBox
            // 
            this.DescriptionTextBox.AcceptsReturn = true;
            this.DescriptionTextBox.Location = new System.Drawing.Point(12, 140);
            this.DescriptionTextBox.Multiline = true;
            this.DescriptionTextBox.Name = "DescriptionTextBox";
            this.DescriptionTextBox.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.DescriptionTextBox.Size = new System.Drawing.Size(268, 95);
            this.DescriptionTextBox.TabIndex = 10;
            // 
            // lblDescription
            // 
            this.lblDescription.AutoSize = true;
            this.lblDescription.Location = new System.Drawing.Point(12, 124);
            this.lblDescription.Name = "lblDescription";
            this.lblDescription.Size = new System.Drawing.Size(63, 13);
            this.lblDescription.TabIndex = 9;
            this.lblDescription.Text = "&Description:";
            // 
            // OkButton
            // 
            this.OkButton.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.OkButton.Location = new System.Drawing.Point(124, 241);
            this.OkButton.Name = "OkButton";
            this.OkButton.Size = new System.Drawing.Size(75, 23);
            this.OkButton.TabIndex = 11;
            this.OkButton.Text = "OK";
            this.OkButton.UseVisualStyleBackColor = true;
            // 
            // cancelButton
            // 
            this.cancelButton.Location = new System.Drawing.Point(205, 241);
            this.cancelButton.Name = "cancelButton";
            this.cancelButton.Size = new System.Drawing.Size(75, 23);
            this.cancelButton.TabIndex = 12;
            this.cancelButton.Text = "Cancel";
            this.cancelButton.UseVisualStyleBackColor = true;
            // 
            // MetadataEditorForm
            // 
            this.AcceptButton = this.OkButton;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.cancelButton;
            this.ClientSize = new System.Drawing.Size(292, 276);
            this.Controls.Add(this.cancelButton);
            this.Controls.Add(this.OkButton);
            this.Controls.Add(this.lblDescription);
            this.Controls.Add(this.DescriptionTextBox);
            this.Controls.Add(this.EditEnumerationTypeButton);
            this.Controls.Add(this.DefaultValueTextBox);
            this.Controls.Add(this.lblDefaultValue);
            this.Controls.Add(this.EnumerationTypeComboBox);
            this.Controls.Add(this.lblEnumerationType);
            this.Controls.Add(this.AlternativeNameTextBox);
            this.Controls.Add(this.lblAlternativeName);
            this.Controls.Add(this.lblVariable);
            this.Controls.Add(this.VariableTextBox);
            this.Name = "MetadataEditorForm";
            this.Text = "Edit Metadata";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox VariableTextBox;
        private System.Windows.Forms.Label lblVariable;
        private System.Windows.Forms.Label lblAlternativeName;
        private System.Windows.Forms.TextBox AlternativeNameTextBox;
        private System.Windows.Forms.Label lblEnumerationType;
        private System.Windows.Forms.ComboBox EnumerationTypeComboBox;
        private System.Windows.Forms.Label lblDefaultValue;
        private System.Windows.Forms.TextBox DefaultValueTextBox;
        private System.Windows.Forms.Button EditEnumerationTypeButton;
        private System.Windows.Forms.TextBox DescriptionTextBox;
        private System.Windows.Forms.Label lblDescription;
        private System.Windows.Forms.Button OkButton;
        private System.Windows.Forms.Button cancelButton;
    }
}