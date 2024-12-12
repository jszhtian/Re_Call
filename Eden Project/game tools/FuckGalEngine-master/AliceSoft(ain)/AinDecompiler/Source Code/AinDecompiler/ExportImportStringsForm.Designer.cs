namespace AinDecompiler
{
    partial class ExportImportStringsForm
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(ExportImportStringsForm));
            this.exportStringsButton = new System.Windows.Forms.Button();
            this.messagesLabel = new System.Windows.Forms.Label();
            this.stringsLabel = new System.Windows.Forms.Label();
            this.exportMessagesButton = new System.Windows.Forms.Button();
            this.importStringsMessagesButton = new System.Windows.Forms.Button();
            this.exportStringsAndMessagesButton = new System.Windows.Forms.Button();
            this.excludeFunctionNamesCheckBox = new System.Windows.Forms.CheckBox();
            this.excludeTestCheckBox = new System.Windows.Forms.CheckBox();
            this.keepCommentsCheckBox = new System.Windows.Forms.CheckBox();
            this.excludeSystemCheckBox = new System.Windows.Forms.CheckBox();
            this.excludeUnusedStringsCheckBox = new System.Windows.Forms.CheckBox();
            this.applyFiltersOnImportCheckBox = new System.Windows.Forms.CheckBox();
            this.excludeAssertsCheckBox = new System.Windows.Forms.CheckBox();
            this.excludeEventFlagNamesCheckBox = new System.Windows.Forms.CheckBox();
            this.toolTip1 = new System.Windows.Forms.ToolTip(this.components);
            this.label1 = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // exportStringsButton
            // 
            this.exportStringsButton.Location = new System.Drawing.Point(191, 357);
            this.exportStringsButton.Name = "exportStringsButton";
            this.exportStringsButton.Size = new System.Drawing.Size(175, 23);
            this.exportStringsButton.TabIndex = 14;
            this.exportStringsButton.Text = "Export only Strings...";
            this.exportStringsButton.UseVisualStyleBackColor = true;
            this.exportStringsButton.Click += new System.EventHandler(this.ExportStringsButton_Click);
            // 
            // messagesLabel
            // 
            this.messagesLabel.AutoSize = true;
            this.messagesLabel.Location = new System.Drawing.Point(8, 62);
            this.messagesLabel.Name = "messagesLabel";
            this.messagesLabel.Size = new System.Drawing.Size(297, 13);
            this.messagesLabel.TabIndex = 1;
            this.messagesLabel.Text = "There are 00000 messages, numbered 00000 through 00000.";
            // 
            // stringsLabel
            // 
            this.stringsLabel.AutoSize = true;
            this.stringsLabel.Location = new System.Drawing.Point(8, 82);
            this.stringsLabel.Name = "stringsLabel";
            this.stringsLabel.Size = new System.Drawing.Size(280, 13);
            this.stringsLabel.TabIndex = 2;
            this.stringsLabel.Text = "There are 00000 strings, numbered 00000 through 00000.";
            // 
            // exportMessagesButton
            // 
            this.exportMessagesButton.Location = new System.Drawing.Point(8, 357);
            this.exportMessagesButton.Name = "exportMessagesButton";
            this.exportMessagesButton.Size = new System.Drawing.Size(175, 23);
            this.exportMessagesButton.TabIndex = 13;
            this.exportMessagesButton.Text = "Export only Messages...";
            this.exportMessagesButton.UseVisualStyleBackColor = true;
            this.exportMessagesButton.Click += new System.EventHandler(this.exportMessagesButton_Click);
            // 
            // importStringsMessagesButton
            // 
            this.importStringsMessagesButton.Location = new System.Drawing.Point(8, 328);
            this.importStringsMessagesButton.Name = "importStringsMessagesButton";
            this.importStringsMessagesButton.Size = new System.Drawing.Size(175, 23);
            this.importStringsMessagesButton.TabIndex = 11;
            this.importStringsMessagesButton.Text = "Import Strings/Messages...";
            this.importStringsMessagesButton.UseVisualStyleBackColor = true;
            this.importStringsMessagesButton.Click += new System.EventHandler(this.importStringsMessagesButton_Click);
            // 
            // exportStringsAndMessagesButton
            // 
            this.exportStringsAndMessagesButton.Location = new System.Drawing.Point(191, 328);
            this.exportStringsAndMessagesButton.Name = "exportStringsAndMessagesButton";
            this.exportStringsAndMessagesButton.Size = new System.Drawing.Size(175, 23);
            this.exportStringsAndMessagesButton.TabIndex = 12;
            this.exportStringsAndMessagesButton.Text = "Export Strings and Messages...";
            this.exportStringsAndMessagesButton.UseVisualStyleBackColor = true;
            this.exportStringsAndMessagesButton.Click += new System.EventHandler(this.exportStringsAndMessagesButton_Click);
            // 
            // excludeFunctionNamesCheckBox
            // 
            this.excludeFunctionNamesCheckBox.AutoSize = true;
            this.excludeFunctionNamesCheckBox.Checked = true;
            this.excludeFunctionNamesCheckBox.CheckState = System.Windows.Forms.CheckState.Checked;
            this.excludeFunctionNamesCheckBox.Location = new System.Drawing.Point(11, 154);
            this.excludeFunctionNamesCheckBox.Name = "excludeFunctionNamesCheckBox";
            this.excludeFunctionNamesCheckBox.Size = new System.Drawing.Size(235, 17);
            this.excludeFunctionNamesCheckBox.TabIndex = 4;
            this.excludeFunctionNamesCheckBox.Text = "Exclude strings which match function names";
            this.excludeFunctionNamesCheckBox.UseVisualStyleBackColor = true;
            // 
            // excludeTestCheckBox
            // 
            this.excludeTestCheckBox.AutoSize = true;
            this.excludeTestCheckBox.Checked = true;
            this.excludeTestCheckBox.CheckState = System.Windows.Forms.CheckState.Checked;
            this.excludeTestCheckBox.Location = new System.Drawing.Point(10, 177);
            this.excludeTestCheckBox.Name = "excludeTestCheckBox";
            this.excludeTestCheckBox.Size = new System.Drawing.Size(350, 17);
            this.excludeTestCheckBox.TabIndex = 5;
            this.excludeTestCheckBox.Text = "Exclude strings from functions named テスト Test or  ディバッグ Debug";
            this.excludeTestCheckBox.UseVisualStyleBackColor = true;
            // 
            // keepCommentsCheckBox
            // 
            this.keepCommentsCheckBox.AutoSize = true;
            this.keepCommentsCheckBox.Checked = true;
            this.keepCommentsCheckBox.CheckState = System.Windows.Forms.CheckState.Checked;
            this.keepCommentsCheckBox.Location = new System.Drawing.Point(10, 297);
            this.keepCommentsCheckBox.Name = "keepCommentsCheckBox";
            this.keepCommentsCheckBox.Size = new System.Drawing.Size(185, 17);
            this.keepCommentsCheckBox.TabIndex = 10;
            this.keepCommentsCheckBox.Text = "Keep excluded stuff as comments";
            this.keepCommentsCheckBox.UseVisualStyleBackColor = true;
            // 
            // excludeSystemCheckBox
            // 
            this.excludeSystemCheckBox.AutoSize = true;
            this.excludeSystemCheckBox.Checked = true;
            this.excludeSystemCheckBox.CheckState = System.Windows.Forms.CheckState.Checked;
            this.excludeSystemCheckBox.Location = new System.Drawing.Point(10, 200);
            this.excludeSystemCheckBox.Name = "excludeSystemCheckBox";
            this.excludeSystemCheckBox.Size = new System.Drawing.Size(305, 17);
            this.excludeSystemCheckBox.TabIndex = 6;
            this.excludeSystemCheckBox.Text = "Exclude strings found only in files beginning with \"System\\\"";
            this.excludeSystemCheckBox.UseVisualStyleBackColor = true;
            // 
            // excludeUnusedStringsCheckBox
            // 
            this.excludeUnusedStringsCheckBox.AutoSize = true;
            this.excludeUnusedStringsCheckBox.Checked = true;
            this.excludeUnusedStringsCheckBox.CheckState = System.Windows.Forms.CheckState.Checked;
            this.excludeUnusedStringsCheckBox.Location = new System.Drawing.Point(10, 223);
            this.excludeUnusedStringsCheckBox.Name = "excludeUnusedStringsCheckBox";
            this.excludeUnusedStringsCheckBox.Size = new System.Drawing.Size(135, 17);
            this.excludeUnusedStringsCheckBox.TabIndex = 7;
            this.excludeUnusedStringsCheckBox.Text = "Exclude unused strings";
            this.excludeUnusedStringsCheckBox.UseVisualStyleBackColor = true;
            // 
            // applyFiltersOnImportCheckBox
            // 
            this.applyFiltersOnImportCheckBox.Checked = true;
            this.applyFiltersOnImportCheckBox.CheckState = System.Windows.Forms.CheckState.Checked;
            this.applyFiltersOnImportCheckBox.Location = new System.Drawing.Point(10, 99);
            this.applyFiltersOnImportCheckBox.Name = "applyFiltersOnImportCheckBox";
            this.applyFiltersOnImportCheckBox.Size = new System.Drawing.Size(369, 50);
            this.applyFiltersOnImportCheckBox.TabIndex = 3;
            this.applyFiltersOnImportCheckBox.Text = resources.GetString("applyFiltersOnImportCheckBox.Text");
            this.applyFiltersOnImportCheckBox.UseVisualStyleBackColor = true;
            // 
            // excludeAssertsCheckBox
            // 
            this.excludeAssertsCheckBox.AutoSize = true;
            this.excludeAssertsCheckBox.Checked = true;
            this.excludeAssertsCheckBox.CheckState = System.Windows.Forms.CheckState.Checked;
            this.excludeAssertsCheckBox.Location = new System.Drawing.Point(10, 246);
            this.excludeAssertsCheckBox.Name = "excludeAssertsCheckBox";
            this.excludeAssertsCheckBox.Size = new System.Drawing.Size(212, 17);
            this.excludeAssertsCheckBox.TabIndex = 8;
            this.excludeAssertsCheckBox.Text = "Exclude ASSERT text (code fragments)";
            this.excludeAssertsCheckBox.UseVisualStyleBackColor = true;
            // 
            // excludeEventFlagNamesCheckBox
            // 
            this.excludeEventFlagNamesCheckBox.AutoSize = true;
            this.excludeEventFlagNamesCheckBox.Checked = true;
            this.excludeEventFlagNamesCheckBox.CheckState = System.Windows.Forms.CheckState.Checked;
            this.excludeEventFlagNamesCheckBox.Location = new System.Drawing.Point(10, 269);
            this.excludeEventFlagNamesCheckBox.Name = "excludeEventFlagNamesCheckBox";
            this.excludeEventFlagNamesCheckBox.Size = new System.Drawing.Size(165, 17);
            this.excludeEventFlagNamesCheckBox.TabIndex = 9;
            this.excludeEventFlagNamesCheckBox.Text = "Exlcude names of event flags";
            this.excludeEventFlagNamesCheckBox.UseVisualStyleBackColor = true;
            // 
            // label1
            // 
            this.label1.Location = new System.Drawing.Point(8, 9);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(352, 43);
            this.label1.TabIndex = 0;
            this.label1.Text = "String numbers may change, so use this with caution.  New versions, word wrapping" +
                ", or disassembling and recompiling an AIN file may change the string numbers.";
            // 
            // ExportImportStringsForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(378, 391);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.excludeEventFlagNamesCheckBox);
            this.Controls.Add(this.excludeAssertsCheckBox);
            this.Controls.Add(this.applyFiltersOnImportCheckBox);
            this.Controls.Add(this.excludeUnusedStringsCheckBox);
            this.Controls.Add(this.excludeSystemCheckBox);
            this.Controls.Add(this.keepCommentsCheckBox);
            this.Controls.Add(this.excludeTestCheckBox);
            this.Controls.Add(this.excludeFunctionNamesCheckBox);
            this.Controls.Add(this.exportStringsAndMessagesButton);
            this.Controls.Add(this.importStringsMessagesButton);
            this.Controls.Add(this.exportMessagesButton);
            this.Controls.Add(this.stringsLabel);
            this.Controls.Add(this.messagesLabel);
            this.Controls.Add(this.exportStringsButton);
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "ExportImportStringsForm";
            this.Text = "Import/Export Text (legacy)";
            this.Load += new System.EventHandler(this.ExportImportStringsForm_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button exportStringsButton;
        private System.Windows.Forms.Label messagesLabel;
        private System.Windows.Forms.Label stringsLabel;
        private System.Windows.Forms.Button exportMessagesButton;
        private System.Windows.Forms.Button importStringsMessagesButton;
        private System.Windows.Forms.Button exportStringsAndMessagesButton;
        private System.Windows.Forms.CheckBox excludeFunctionNamesCheckBox;
        private System.Windows.Forms.CheckBox excludeTestCheckBox;
        private System.Windows.Forms.CheckBox keepCommentsCheckBox;
        private System.Windows.Forms.CheckBox excludeSystemCheckBox;
        private System.Windows.Forms.CheckBox excludeUnusedStringsCheckBox;
        private System.Windows.Forms.CheckBox applyFiltersOnImportCheckBox;
        private System.Windows.Forms.CheckBox excludeAssertsCheckBox;
        private System.Windows.Forms.CheckBox excludeEventFlagNamesCheckBox;
        private System.Windows.Forms.ToolTip toolTip1;
        private System.Windows.Forms.Label label1;
    }
}