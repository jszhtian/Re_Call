namespace AinDecompiler
{
    partial class EditMetadata2
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
            this.OkButton = new System.Windows.Forms.Button();
            this.lblDescription = new System.Windows.Forms.Label();
            this.DescriptionTextBox = new System.Windows.Forms.TextBox();
            this.EditEnumerationTypeButton = new System.Windows.Forms.Button();
            this.DefaultValueTextBox = new System.Windows.Forms.TextBox();
            this.lblDefaultValue = new System.Windows.Forms.Label();
            this.EnumerationTypeComboBox = new System.Windows.Forms.ComboBox();
            this.lblEnumerationType = new System.Windows.Forms.Label();
            this.AlternativeNameTextBox = new System.Windows.Forms.TextBox();
            this.lblAlternativeName = new System.Windows.Forms.Label();
            this.lblVariable = new System.Windows.Forms.Label();
            this.VariableTextBox = new System.Windows.Forms.TextBox();
            this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            this.spreadEnumerationTypeButton = new System.Windows.Forms.Button();
            this.findDefaultValueButton = new System.Windows.Forms.Button();
            this.panel1 = new System.Windows.Forms.Panel();
            this.tableLayoutPanel1.SuspendLayout();
            this.panel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // cancelButton
            // 
            this.cancelButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.cancelButton.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.cancelButton.Location = new System.Drawing.Point(279, 3);
            this.cancelButton.Name = "cancelButton";
            this.cancelButton.Size = new System.Drawing.Size(75, 23);
            this.cancelButton.TabIndex = 1;
            this.cancelButton.Text = "Cancel";
            this.cancelButton.UseVisualStyleBackColor = true;
            this.cancelButton.Click += new System.EventHandler(this.cancelButton_Click);
            // 
            // OkButton
            // 
            this.OkButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.OkButton.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.OkButton.Location = new System.Drawing.Point(198, 3);
            this.OkButton.Name = "OkButton";
            this.OkButton.Size = new System.Drawing.Size(75, 23);
            this.OkButton.TabIndex = 0;
            this.OkButton.Text = "OK";
            this.OkButton.UseVisualStyleBackColor = true;
            this.OkButton.Click += new System.EventHandler(this.OkButton_Click);
            // 
            // lblDescription
            // 
            this.lblDescription.AutoSize = true;
            this.lblDescription.Location = new System.Drawing.Point(10, 146);
            this.lblDescription.Name = "lblDescription";
            this.lblDescription.Size = new System.Drawing.Size(63, 13);
            this.lblDescription.TabIndex = 11;
            this.lblDescription.Text = "&Description:";
            // 
            // DescriptionTextBox
            // 
            this.DescriptionTextBox.AcceptsReturn = true;
            this.tableLayoutPanel1.SetColumnSpan(this.DescriptionTextBox, 3);
            this.DescriptionTextBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.DescriptionTextBox.Location = new System.Drawing.Point(10, 162);
            this.DescriptionTextBox.Multiline = true;
            this.DescriptionTextBox.Name = "DescriptionTextBox";
            this.DescriptionTextBox.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.DescriptionTextBox.Size = new System.Drawing.Size(344, 95);
            this.DescriptionTextBox.TabIndex = 12;
            // 
            // EditEnumerationTypeButton
            // 
            this.EditEnumerationTypeButton.Dock = System.Windows.Forms.DockStyle.Fill;
            this.EditEnumerationTypeButton.Location = new System.Drawing.Point(113, 120);
            this.EditEnumerationTypeButton.Name = "EditEnumerationTypeButton";
            this.EditEnumerationTypeButton.Size = new System.Drawing.Size(160, 23);
            this.EditEnumerationTypeButton.TabIndex = 10;
            this.EditEnumerationTypeButton.Text = "&Edit Enumeration Type...";
            this.EditEnumerationTypeButton.UseVisualStyleBackColor = true;
            this.EditEnumerationTypeButton.Click += new System.EventHandler(this.EditEnumerationTypeButton_Click);
            // 
            // DefaultValueTextBox
            // 
            this.DefaultValueTextBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.DefaultValueTextBox.Location = new System.Drawing.Point(113, 62);
            this.DefaultValueTextBox.Name = "DefaultValueTextBox";
            this.DefaultValueTextBox.ReadOnly = true;
            this.DefaultValueTextBox.Size = new System.Drawing.Size(160, 20);
            this.DefaultValueTextBox.TabIndex = 5;
            // 
            // lblDefaultValue
            // 
            this.lblDefaultValue.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lblDefaultValue.Location = new System.Drawing.Point(10, 59);
            this.lblDefaultValue.Name = "lblDefaultValue";
            this.lblDefaultValue.Size = new System.Drawing.Size(97, 29);
            this.lblDefaultValue.TabIndex = 4;
            this.lblDefaultValue.Text = "Default &Value:";
            this.lblDefaultValue.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // EnumerationTypeComboBox
            // 
            this.EnumerationTypeComboBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.EnumerationTypeComboBox.FormattingEnabled = true;
            this.EnumerationTypeComboBox.Location = new System.Drawing.Point(113, 91);
            this.EnumerationTypeComboBox.Name = "EnumerationTypeComboBox";
            this.EnumerationTypeComboBox.Size = new System.Drawing.Size(160, 21);
            this.EnumerationTypeComboBox.TabIndex = 8;
            // 
            // lblEnumerationType
            // 
            this.lblEnumerationType.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lblEnumerationType.Location = new System.Drawing.Point(10, 88);
            this.lblEnumerationType.Name = "lblEnumerationType";
            this.lblEnumerationType.Size = new System.Drawing.Size(97, 29);
            this.lblEnumerationType.TabIndex = 7;
            this.lblEnumerationType.Text = "Enumeration &Type:";
            this.lblEnumerationType.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // AlternativeNameTextBox
            // 
            this.tableLayoutPanel1.SetColumnSpan(this.AlternativeNameTextBox, 2);
            this.AlternativeNameTextBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.AlternativeNameTextBox.Location = new System.Drawing.Point(113, 36);
            this.AlternativeNameTextBox.Name = "AlternativeNameTextBox";
            this.AlternativeNameTextBox.Size = new System.Drawing.Size(241, 20);
            this.AlternativeNameTextBox.TabIndex = 3;
            // 
            // lblAlternativeName
            // 
            this.lblAlternativeName.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lblAlternativeName.Location = new System.Drawing.Point(10, 33);
            this.lblAlternativeName.Name = "lblAlternativeName";
            this.lblAlternativeName.Size = new System.Drawing.Size(97, 26);
            this.lblAlternativeName.TabIndex = 2;
            this.lblAlternativeName.Text = "&Alternative name:";
            this.lblAlternativeName.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // lblVariable
            // 
            this.lblVariable.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lblVariable.Location = new System.Drawing.Point(10, 7);
            this.lblVariable.Name = "lblVariable";
            this.lblVariable.Size = new System.Drawing.Size(97, 26);
            this.lblVariable.TabIndex = 0;
            this.lblVariable.Text = "Variable:";
            this.lblVariable.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // VariableTextBox
            // 
            this.tableLayoutPanel1.SetColumnSpan(this.VariableTextBox, 2);
            this.VariableTextBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.VariableTextBox.Location = new System.Drawing.Point(113, 10);
            this.VariableTextBox.Name = "VariableTextBox";
            this.VariableTextBox.ReadOnly = true;
            this.VariableTextBox.Size = new System.Drawing.Size(241, 20);
            this.VariableTextBox.TabIndex = 1;
            // 
            // tableLayoutPanel1
            // 
            this.tableLayoutPanel1.ColumnCount = 3;
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 103F));
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle());
            this.tableLayoutPanel1.Controls.Add(this.spreadEnumerationTypeButton, 2, 3);
            this.tableLayoutPanel1.Controls.Add(this.lblVariable, 0, 0);
            this.tableLayoutPanel1.Controls.Add(this.VariableTextBox, 1, 0);
            this.tableLayoutPanel1.Controls.Add(this.lblAlternativeName, 0, 1);
            this.tableLayoutPanel1.Controls.Add(this.DescriptionTextBox, 0, 6);
            this.tableLayoutPanel1.Controls.Add(this.lblDescription, 0, 5);
            this.tableLayoutPanel1.Controls.Add(this.AlternativeNameTextBox, 1, 1);
            this.tableLayoutPanel1.Controls.Add(this.lblDefaultValue, 0, 2);
            this.tableLayoutPanel1.Controls.Add(this.EditEnumerationTypeButton, 1, 4);
            this.tableLayoutPanel1.Controls.Add(this.DefaultValueTextBox, 1, 2);
            this.tableLayoutPanel1.Controls.Add(this.EnumerationTypeComboBox, 1, 3);
            this.tableLayoutPanel1.Controls.Add(this.lblEnumerationType, 0, 3);
            this.tableLayoutPanel1.Controls.Add(this.findDefaultValueButton, 2, 2);
            this.tableLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel1.Location = new System.Drawing.Point(0, 0);
            this.tableLayoutPanel1.Name = "tableLayoutPanel1";
            this.tableLayoutPanel1.Padding = new System.Windows.Forms.Padding(7);
            this.tableLayoutPanel1.RowCount = 7;
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.Size = new System.Drawing.Size(364, 262);
            this.tableLayoutPanel1.TabIndex = 0;
            // 
            // spreadEnumerationTypeButton
            // 
            this.spreadEnumerationTypeButton.Location = new System.Drawing.Point(279, 91);
            this.spreadEnumerationTypeButton.Name = "spreadEnumerationTypeButton";
            this.spreadEnumerationTypeButton.Size = new System.Drawing.Size(75, 23);
            this.spreadEnumerationTypeButton.TabIndex = 9;
            this.spreadEnumerationTypeButton.Text = "&Spread...";
            this.spreadEnumerationTypeButton.UseVisualStyleBackColor = true;
            this.spreadEnumerationTypeButton.Click += new System.EventHandler(this.spreadEnumerationTypeButton_Click);
            // 
            // findDefaultValueButton
            // 
            this.findDefaultValueButton.Location = new System.Drawing.Point(279, 62);
            this.findDefaultValueButton.Name = "findDefaultValueButton";
            this.findDefaultValueButton.Size = new System.Drawing.Size(75, 23);
            this.findDefaultValueButton.TabIndex = 6;
            this.findDefaultValueButton.Text = "&Find...";
            this.findDefaultValueButton.UseVisualStyleBackColor = true;
            this.findDefaultValueButton.Click += new System.EventHandler(this.findDefaultValueButton_Click);
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.OkButton);
            this.panel1.Controls.Add(this.cancelButton);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.panel1.Location = new System.Drawing.Point(0, 262);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(364, 36);
            this.panel1.TabIndex = 1;
            // 
            // EditMetadata2
            // 
            this.AcceptButton = this.OkButton;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.cancelButton;
            this.ClientSize = new System.Drawing.Size(364, 298);
            this.Controls.Add(this.tableLayoutPanel1);
            this.Controls.Add(this.panel1);
            this.Name = "EditMetadata2";
            this.Text = "Edit Metadata ";
            this.Load += new System.EventHandler(this.EditMetadata2_Load);
            this.tableLayoutPanel1.ResumeLayout(false);
            this.tableLayoutPanel1.PerformLayout();
            this.panel1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button cancelButton;
        private System.Windows.Forms.Button OkButton;
        private System.Windows.Forms.Label lblDescription;
        private System.Windows.Forms.TextBox DescriptionTextBox;
        private System.Windows.Forms.Button EditEnumerationTypeButton;
        private System.Windows.Forms.TextBox DefaultValueTextBox;
        private System.Windows.Forms.Label lblDefaultValue;
        private System.Windows.Forms.ComboBox EnumerationTypeComboBox;
        private System.Windows.Forms.Label lblEnumerationType;
        private System.Windows.Forms.TextBox AlternativeNameTextBox;
        private System.Windows.Forms.Label lblAlternativeName;
        private System.Windows.Forms.Label lblVariable;
        private System.Windows.Forms.TextBox VariableTextBox;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Button findDefaultValueButton;
        private System.Windows.Forms.Button spreadEnumerationTypeButton;
    }
}