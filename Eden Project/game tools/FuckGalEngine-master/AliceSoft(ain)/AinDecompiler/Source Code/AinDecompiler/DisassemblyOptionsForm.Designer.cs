namespace AinDecompiler
{
    partial class DisassemblyOptionsForm
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
            this.okButton = new System.Windows.Forms.Button();
            this.cancelButton = new System.Windows.Forms.Button();
            this.numberOfFilesGroupBox = new System.Windows.Forms.GroupBox();
            this.ExportToSingleFileRadioButton = new System.Windows.Forms.RadioButton();
            this.ExportToMultipleFilesRadioButton = new System.Windows.Forms.RadioButton();
            this.duplicateMessagesAndStringsGroupBox = new System.Windows.Forms.GroupBox();
            this.ForceOrderOfStringsToBePreservedRadioButton = new System.Windows.Forms.RadioButton();
            this.PreserveOriginalDuplicationRadioButton = new System.Windows.Forms.RadioButton();
            this.MergeAllDuplicateTextRadioButton = new System.Windows.Forms.RadioButton();
            this.orphanedFunctionsGroupBox = new System.Windows.Forms.GroupBox();
            this.RemoveLeftoversFromEarilerTranslationProjectsRadioButton = new System.Windows.Forms.RadioButton();
            this.KeepAllFunctionsRadioButton = new System.Windows.Forms.RadioButton();
            this.assemblyCodeAnnotationGroupBox = new System.Windows.Forms.GroupBox();
            this.NoAnnotaitonRadioButton = new System.Windows.Forms.RadioButton();
            this.AnnotateWithDecompiledSourceCodeRadioButton = new System.Windows.Forms.RadioButton();
            this.numberOfFilesGroupBox.SuspendLayout();
            this.duplicateMessagesAndStringsGroupBox.SuspendLayout();
            this.orphanedFunctionsGroupBox.SuspendLayout();
            this.assemblyCodeAnnotationGroupBox.SuspendLayout();
            this.SuspendLayout();
            // 
            // okButton
            // 
            this.okButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.okButton.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.okButton.Location = new System.Drawing.Point(124, 338);
            this.okButton.Name = "okButton";
            this.okButton.Size = new System.Drawing.Size(75, 23);
            this.okButton.TabIndex = 4;
            this.okButton.Text = "OK";
            this.okButton.UseVisualStyleBackColor = true;
            // 
            // cancelButton
            // 
            this.cancelButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.cancelButton.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.cancelButton.Location = new System.Drawing.Point(205, 338);
            this.cancelButton.Name = "cancelButton";
            this.cancelButton.Size = new System.Drawing.Size(75, 23);
            this.cancelButton.TabIndex = 5;
            this.cancelButton.Text = "Cancel";
            this.cancelButton.UseVisualStyleBackColor = true;
            // 
            // numberOfFilesGroupBox
            // 
            this.numberOfFilesGroupBox.Controls.Add(this.ExportToSingleFileRadioButton);
            this.numberOfFilesGroupBox.Controls.Add(this.ExportToMultipleFilesRadioButton);
            this.numberOfFilesGroupBox.Location = new System.Drawing.Point(12, 12);
            this.numberOfFilesGroupBox.Name = "numberOfFilesGroupBox";
            this.numberOfFilesGroupBox.Size = new System.Drawing.Size(268, 66);
            this.numberOfFilesGroupBox.TabIndex = 0;
            this.numberOfFilesGroupBox.TabStop = false;
            this.numberOfFilesGroupBox.Text = "&Number of files";
            // 
            // ExportToSingleFileRadioButton
            // 
            this.ExportToSingleFileRadioButton.AutoSize = true;
            this.ExportToSingleFileRadioButton.Location = new System.Drawing.Point(12, 39);
            this.ExportToSingleFileRadioButton.Name = "ExportToSingleFileRadioButton";
            this.ExportToSingleFileRadioButton.Size = new System.Drawing.Size(135, 17);
            this.ExportToSingleFileRadioButton.TabIndex = 1;
            this.ExportToSingleFileRadioButton.Text = "Export to &single file only";
            this.ExportToSingleFileRadioButton.UseVisualStyleBackColor = true;
            // 
            // ExportToMultipleFilesRadioButton
            // 
            this.ExportToMultipleFilesRadioButton.AutoSize = true;
            this.ExportToMultipleFilesRadioButton.Checked = true;
            this.ExportToMultipleFilesRadioButton.Location = new System.Drawing.Point(12, 16);
            this.ExportToMultipleFilesRadioButton.Name = "ExportToMultipleFilesRadioButton";
            this.ExportToMultipleFilesRadioButton.Size = new System.Drawing.Size(126, 17);
            this.ExportToMultipleFilesRadioButton.TabIndex = 0;
            this.ExportToMultipleFilesRadioButton.TabStop = true;
            this.ExportToMultipleFilesRadioButton.Text = "Export to &multiple files";
            this.ExportToMultipleFilesRadioButton.UseVisualStyleBackColor = true;
            // 
            // duplicateMessagesAndStringsGroupBox
            // 
            this.duplicateMessagesAndStringsGroupBox.Controls.Add(this.ForceOrderOfStringsToBePreservedRadioButton);
            this.duplicateMessagesAndStringsGroupBox.Controls.Add(this.PreserveOriginalDuplicationRadioButton);
            this.duplicateMessagesAndStringsGroupBox.Controls.Add(this.MergeAllDuplicateTextRadioButton);
            this.duplicateMessagesAndStringsGroupBox.Location = new System.Drawing.Point(12, 84);
            this.duplicateMessagesAndStringsGroupBox.Name = "duplicateMessagesAndStringsGroupBox";
            this.duplicateMessagesAndStringsGroupBox.Size = new System.Drawing.Size(268, 88);
            this.duplicateMessagesAndStringsGroupBox.TabIndex = 1;
            this.duplicateMessagesAndStringsGroupBox.TabStop = false;
            this.duplicateMessagesAndStringsGroupBox.Text = "&Duplicate messages and strings";
            // 
            // ForceOrderOfStringsToBePreservedRadioButton
            // 
            this.ForceOrderOfStringsToBePreservedRadioButton.AutoSize = true;
            this.ForceOrderOfStringsToBePreservedRadioButton.Location = new System.Drawing.Point(12, 62);
            this.ForceOrderOfStringsToBePreservedRadioButton.Name = "ForceOrderOfStringsToBePreservedRadioButton";
            this.ForceOrderOfStringsToBePreservedRadioButton.Size = new System.Drawing.Size(219, 17);
            this.ForceOrderOfStringsToBePreservedRadioButton.TabIndex = 2;
            this.ForceOrderOfStringsToBePreservedRadioButton.Text = "Force the order of strings to be &preserved";
            this.ForceOrderOfStringsToBePreservedRadioButton.UseVisualStyleBackColor = true;
            // 
            // PreserveOriginalDuplicationRadioButton
            // 
            this.PreserveOriginalDuplicationRadioButton.AutoSize = true;
            this.PreserveOriginalDuplicationRadioButton.Checked = true;
            this.PreserveOriginalDuplicationRadioButton.Location = new System.Drawing.Point(12, 39);
            this.PreserveOriginalDuplicationRadioButton.Name = "PreserveOriginalDuplicationRadioButton";
            this.PreserveOriginalDuplicationRadioButton.Size = new System.Drawing.Size(157, 17);
            this.PreserveOriginalDuplicationRadioButton.TabIndex = 1;
            this.PreserveOriginalDuplicationRadioButton.TabStop = true;
            this.PreserveOriginalDuplicationRadioButton.Text = "Preserve &original duplication";
            this.PreserveOriginalDuplicationRadioButton.UseVisualStyleBackColor = true;
            // 
            // MergeAllDuplicateTextRadioButton
            // 
            this.MergeAllDuplicateTextRadioButton.AutoSize = true;
            this.MergeAllDuplicateTextRadioButton.Location = new System.Drawing.Point(12, 16);
            this.MergeAllDuplicateTextRadioButton.Name = "MergeAllDuplicateTextRadioButton";
            this.MergeAllDuplicateTextRadioButton.Size = new System.Drawing.Size(134, 17);
            this.MergeAllDuplicateTextRadioButton.TabIndex = 0;
            this.MergeAllDuplicateTextRadioButton.Text = "Merge all d&uplicate text";
            this.MergeAllDuplicateTextRadioButton.UseVisualStyleBackColor = true;
            // 
            // orphanedFunctionsGroupBox
            // 
            this.orphanedFunctionsGroupBox.Controls.Add(this.RemoveLeftoversFromEarilerTranslationProjectsRadioButton);
            this.orphanedFunctionsGroupBox.Controls.Add(this.KeepAllFunctionsRadioButton);
            this.orphanedFunctionsGroupBox.Location = new System.Drawing.Point(12, 259);
            this.orphanedFunctionsGroupBox.Name = "orphanedFunctionsGroupBox";
            this.orphanedFunctionsGroupBox.Size = new System.Drawing.Size(268, 70);
            this.orphanedFunctionsGroupBox.TabIndex = 3;
            this.orphanedFunctionsGroupBox.TabStop = false;
            this.orphanedFunctionsGroupBox.Text = "&Orphaned Functions";
            // 
            // RemoveLeftoversFromEarilerTranslationProjectsRadioButton
            // 
            this.RemoveLeftoversFromEarilerTranslationProjectsRadioButton.AutoSize = true;
            this.RemoveLeftoversFromEarilerTranslationProjectsRadioButton.Location = new System.Drawing.Point(12, 42);
            this.RemoveLeftoversFromEarilerTranslationProjectsRadioButton.Name = "RemoveLeftoversFromEarilerTranslationProjectsRadioButton";
            this.RemoveLeftoversFromEarilerTranslationProjectsRadioButton.Size = new System.Drawing.Size(253, 17);
            this.RemoveLeftoversFromEarilerTranslationProjectsRadioButton.TabIndex = 1;
            this.RemoveLeftoversFromEarilerTranslationProjectsRadioButton.Text = "&Remove leftovers from earlier translation projects";
            this.RemoveLeftoversFromEarilerTranslationProjectsRadioButton.UseVisualStyleBackColor = true;
            // 
            // KeepAllFunctionsRadioButton
            // 
            this.KeepAllFunctionsRadioButton.AutoSize = true;
            this.KeepAllFunctionsRadioButton.Checked = true;
            this.KeepAllFunctionsRadioButton.Location = new System.Drawing.Point(12, 19);
            this.KeepAllFunctionsRadioButton.Name = "KeepAllFunctionsRadioButton";
            this.KeepAllFunctionsRadioButton.Size = new System.Drawing.Size(109, 17);
            this.KeepAllFunctionsRadioButton.TabIndex = 0;
            this.KeepAllFunctionsRadioButton.TabStop = true;
            this.KeepAllFunctionsRadioButton.Text = "&Keep all functions";
            this.KeepAllFunctionsRadioButton.UseVisualStyleBackColor = true;
            // 
            // assemblyCodeAnnotationGroupBox
            // 
            this.assemblyCodeAnnotationGroupBox.Controls.Add(this.NoAnnotaitonRadioButton);
            this.assemblyCodeAnnotationGroupBox.Controls.Add(this.AnnotateWithDecompiledSourceCodeRadioButton);
            this.assemblyCodeAnnotationGroupBox.Location = new System.Drawing.Point(12, 178);
            this.assemblyCodeAnnotationGroupBox.Name = "assemblyCodeAnnotationGroupBox";
            this.assemblyCodeAnnotationGroupBox.Size = new System.Drawing.Size(268, 75);
            this.assemblyCodeAnnotationGroupBox.TabIndex = 2;
            this.assemblyCodeAnnotationGroupBox.TabStop = false;
            this.assemblyCodeAnnotationGroupBox.Text = "Assembly &code annotation";
            // 
            // NoAnnotaitonRadioButton
            // 
            this.NoAnnotaitonRadioButton.AutoSize = true;
            this.NoAnnotaitonRadioButton.Checked = true;
            this.NoAnnotaitonRadioButton.Location = new System.Drawing.Point(12, 42);
            this.NoAnnotaitonRadioButton.Name = "NoAnnotaitonRadioButton";
            this.NoAnnotaitonRadioButton.Size = new System.Drawing.Size(92, 17);
            this.NoAnnotaitonRadioButton.TabIndex = 1;
            this.NoAnnotaitonRadioButton.TabStop = true;
            this.NoAnnotaitonRadioButton.Text = "&No annotation";
            this.NoAnnotaitonRadioButton.UseVisualStyleBackColor = true;
            // 
            // AnnotateWithDecompiledSourceCodeRadioButton
            // 
            this.AnnotateWithDecompiledSourceCodeRadioButton.AutoSize = true;
            this.AnnotateWithDecompiledSourceCodeRadioButton.Location = new System.Drawing.Point(12, 19);
            this.AnnotateWithDecompiledSourceCodeRadioButton.Name = "AnnotateWithDecompiledSourceCodeRadioButton";
            this.AnnotateWithDecompiledSourceCodeRadioButton.Size = new System.Drawing.Size(209, 17);
            this.AnnotateWithDecompiledSourceCodeRadioButton.TabIndex = 0;
            this.AnnotateWithDecompiledSourceCodeRadioButton.Text = "&Annotate with decompiled source code";
            this.AnnotateWithDecompiledSourceCodeRadioButton.UseVisualStyleBackColor = true;
            // 
            // DisassemblyOptionsForm
            // 
            this.AcceptButton = this.okButton;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.cancelButton;
            this.ClientSize = new System.Drawing.Size(292, 373);
            this.Controls.Add(this.assemblyCodeAnnotationGroupBox);
            this.Controls.Add(this.orphanedFunctionsGroupBox);
            this.Controls.Add(this.duplicateMessagesAndStringsGroupBox);
            this.Controls.Add(this.numberOfFilesGroupBox);
            this.Controls.Add(this.cancelButton);
            this.Controls.Add(this.okButton);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.Name = "DisassemblyOptionsForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Disassemble to Project Settings";
            this.numberOfFilesGroupBox.ResumeLayout(false);
            this.numberOfFilesGroupBox.PerformLayout();
            this.duplicateMessagesAndStringsGroupBox.ResumeLayout(false);
            this.duplicateMessagesAndStringsGroupBox.PerformLayout();
            this.orphanedFunctionsGroupBox.ResumeLayout(false);
            this.orphanedFunctionsGroupBox.PerformLayout();
            this.assemblyCodeAnnotationGroupBox.ResumeLayout(false);
            this.assemblyCodeAnnotationGroupBox.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button okButton;
        private System.Windows.Forms.Button cancelButton;
        private System.Windows.Forms.GroupBox numberOfFilesGroupBox;
        private System.Windows.Forms.RadioButton ExportToMultipleFilesRadioButton;
        private System.Windows.Forms.GroupBox duplicateMessagesAndStringsGroupBox;
        private System.Windows.Forms.RadioButton PreserveOriginalDuplicationRadioButton;
        private System.Windows.Forms.GroupBox orphanedFunctionsGroupBox;
        private System.Windows.Forms.RadioButton KeepAllFunctionsRadioButton;
        private System.Windows.Forms.GroupBox assemblyCodeAnnotationGroupBox;
        private System.Windows.Forms.RadioButton NoAnnotaitonRadioButton;
        public System.Windows.Forms.RadioButton ExportToSingleFileRadioButton;
        public System.Windows.Forms.RadioButton ForceOrderOfStringsToBePreservedRadioButton;
        public System.Windows.Forms.RadioButton MergeAllDuplicateTextRadioButton;
        public System.Windows.Forms.RadioButton RemoveLeftoversFromEarilerTranslationProjectsRadioButton;
        public System.Windows.Forms.RadioButton AnnotateWithDecompiledSourceCodeRadioButton;
    }
}