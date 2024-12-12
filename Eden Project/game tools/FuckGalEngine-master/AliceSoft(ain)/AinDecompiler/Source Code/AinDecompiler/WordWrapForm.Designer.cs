namespace AinDecompiler
{
    partial class WordWrapForm
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
            System.Windows.Forms.Label maxCharactersPerLineNormalLabel;
            System.Windows.Forms.Label maxCharactersPerLineReducedLabel;
            System.Windows.Forms.Label maxLinesPerMessageLabel;
            System.Windows.Forms.Label nextLineFunctionCodeLabel;
            System.Windows.Forms.Label nextLineFunctionNameLabel;
            System.Windows.Forms.Label nextMessageFunctionCodeLabel;
            System.Windows.Forms.Label nextMessageFunctionNameLabel;
            System.Windows.Forms.Label reduceMarginFunctionNameLabel;
            this.maxCharactersPerLineNormalTextBox = new System.Windows.Forms.TextBox();
            this.bindingSource = new System.Windows.Forms.BindingSource(this.components);
            this.maxCharactersPerLineReducedTextBox = new System.Windows.Forms.TextBox();
            this.maxLinesPerMessageTextBox = new System.Windows.Forms.TextBox();
            this.nextLineFunctionCodeTextBox = new System.Windows.Forms.TextBox();
            this.nextLineFunctionNameTextBox = new System.Windows.Forms.TextBox();
            this.nextMessageFunctionCodeTextBox = new System.Windows.Forms.TextBox();
            this.nextMessageFunctionNameTextBox = new System.Windows.Forms.TextBox();
            this.reduceMarginFunctionNameTextBox = new System.Windows.Forms.TextBox();
            this.okButton = new System.Windows.Forms.Button();
            this.cancelButton = new System.Windows.Forms.Button();
            this.toolTip1 = new System.Windows.Forms.ToolTip(this.components);
            this.panel1 = new System.Windows.Forms.Panel();
            this.toolStrip1 = new System.Windows.Forms.ToolStrip();
            this.toolStripDropDownButton1 = new System.Windows.Forms.ToolStripDropDownButton();
            this.button1 = new System.Windows.Forms.Button();
            this.useVariableWidthFontCheckBox = new System.Windows.Forms.CheckBox();
            this.maintainIndentationCheckBox = new System.Windows.Forms.CheckBox();
            this.removeLineBreaksIfWordWrappingCheckBox = new System.Windows.Forms.CheckBox();
            this.selectFontButton = new System.Windows.Forms.Button();
            maxCharactersPerLineNormalLabel = new System.Windows.Forms.Label();
            maxCharactersPerLineReducedLabel = new System.Windows.Forms.Label();
            maxLinesPerMessageLabel = new System.Windows.Forms.Label();
            nextLineFunctionCodeLabel = new System.Windows.Forms.Label();
            nextLineFunctionNameLabel = new System.Windows.Forms.Label();
            nextMessageFunctionCodeLabel = new System.Windows.Forms.Label();
            nextMessageFunctionNameLabel = new System.Windows.Forms.Label();
            reduceMarginFunctionNameLabel = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.bindingSource)).BeginInit();
            this.panel1.SuspendLayout();
            this.toolStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // maxCharactersPerLineNormalLabel
            // 
            maxCharactersPerLineNormalLabel.AutoSize = true;
            maxCharactersPerLineNormalLabel.Location = new System.Drawing.Point(53, 93);
            maxCharactersPerLineNormalLabel.Name = "maxCharactersPerLineNormalLabel";
            maxCharactersPerLineNormalLabel.Size = new System.Drawing.Size(126, 13);
            maxCharactersPerLineNormalLabel.TabIndex = 6;
            maxCharactersPerLineNormalLabel.Text = "Max Characters Per Line:";
            // 
            // maxCharactersPerLineReducedLabel
            // 
            maxCharactersPerLineReducedLabel.AutoSize = true;
            maxCharactersPerLineReducedLabel.Location = new System.Drawing.Point(6, 244);
            maxCharactersPerLineReducedLabel.Name = "maxCharactersPerLineReducedLabel";
            maxCharactersPerLineReducedLabel.Size = new System.Drawing.Size(173, 13);
            maxCharactersPerLineReducedLabel.TabIndex = 8;
            maxCharactersPerLineReducedLabel.Text = "Max Characters Per Line Reduced:";
            // 
            // maxLinesPerMessageLabel
            // 
            maxLinesPerMessageLabel.AutoSize = true;
            maxLinesPerMessageLabel.Location = new System.Drawing.Point(56, 67);
            maxLinesPerMessageLabel.Name = "maxLinesPerMessageLabel";
            maxLinesPerMessageLabel.Size = new System.Drawing.Size(123, 13);
            maxLinesPerMessageLabel.TabIndex = 4;
            maxLinesPerMessageLabel.Text = "Max Lines Per Message:";
            // 
            // nextLineFunctionCodeLabel
            // 
            nextLineFunctionCodeLabel.AutoSize = true;
            nextLineFunctionCodeLabel.Location = new System.Drawing.Point(70, 121);
            nextLineFunctionCodeLabel.Name = "nextLineFunctionCodeLabel";
            nextLineFunctionCodeLabel.Size = new System.Drawing.Size(109, 13);
            nextLineFunctionCodeLabel.TabIndex = 17;
            nextLineFunctionCodeLabel.Text = "Next Line ASM Code:";
            // 
            // nextLineFunctionNameLabel
            // 
            nextLineFunctionNameLabel.AutoSize = true;
            nextLineFunctionNameLabel.Location = new System.Drawing.Point(49, 15);
            nextLineFunctionNameLabel.Name = "nextLineFunctionNameLabel";
            nextLineFunctionNameLabel.Size = new System.Drawing.Size(130, 13);
            nextLineFunctionNameLabel.TabIndex = 0;
            nextLineFunctionNameLabel.Text = "Next Line Function Name:";
            // 
            // nextMessageFunctionCodeLabel
            // 
            nextMessageFunctionCodeLabel.AutoSize = true;
            nextMessageFunctionCodeLabel.Location = new System.Drawing.Point(47, 162);
            nextMessageFunctionCodeLabel.Name = "nextMessageFunctionCodeLabel";
            nextMessageFunctionCodeLabel.Size = new System.Drawing.Size(132, 13);
            nextMessageFunctionCodeLabel.TabIndex = 19;
            nextMessageFunctionCodeLabel.Text = "Next Message ASM Code:";
            // 
            // nextMessageFunctionNameLabel
            // 
            nextMessageFunctionNameLabel.AutoSize = true;
            nextMessageFunctionNameLabel.Location = new System.Drawing.Point(26, 41);
            nextMessageFunctionNameLabel.Name = "nextMessageFunctionNameLabel";
            nextMessageFunctionNameLabel.Size = new System.Drawing.Size(153, 13);
            nextMessageFunctionNameLabel.TabIndex = 2;
            nextMessageFunctionNameLabel.Text = "Next Message Function Name:";
            // 
            // reduceMarginFunctionNameLabel
            // 
            reduceMarginFunctionNameLabel.AutoSize = true;
            reduceMarginFunctionNameLabel.Location = new System.Drawing.Point(16, 203);
            reduceMarginFunctionNameLabel.Name = "reduceMarginFunctionNameLabel";
            reduceMarginFunctionNameLabel.Size = new System.Drawing.Size(163, 13);
            reduceMarginFunctionNameLabel.TabIndex = 21;
            reduceMarginFunctionNameLabel.Text = "Reduce Margin Function Names:";
            // 
            // maxCharactersPerLineNormalTextBox
            // 
            this.maxCharactersPerLineNormalTextBox.DataBindings.Add(new System.Windows.Forms.Binding("Text", this.bindingSource, "MaxCharactersPerLineNormal", true));
            this.maxCharactersPerLineNormalTextBox.Location = new System.Drawing.Point(185, 90);
            this.maxCharactersPerLineNormalTextBox.Name = "maxCharactersPerLineNormalTextBox";
            this.maxCharactersPerLineNormalTextBox.Size = new System.Drawing.Size(100, 20);
            this.maxCharactersPerLineNormalTextBox.TabIndex = 7;
            // 
            // bindingSource
            // 
            this.bindingSource.AllowNew = false;
            this.bindingSource.DataSource = typeof(AinDecompiler.WordWrapOptions);
            // 
            // maxCharactersPerLineReducedTextBox
            // 
            this.maxCharactersPerLineReducedTextBox.DataBindings.Add(new System.Windows.Forms.Binding("Text", this.bindingSource, "MaxCharactersPerLineReduced", true));
            this.maxCharactersPerLineReducedTextBox.Location = new System.Drawing.Point(185, 241);
            this.maxCharactersPerLineReducedTextBox.Name = "maxCharactersPerLineReducedTextBox";
            this.maxCharactersPerLineReducedTextBox.Size = new System.Drawing.Size(100, 20);
            this.maxCharactersPerLineReducedTextBox.TabIndex = 9;
            // 
            // maxLinesPerMessageTextBox
            // 
            this.maxLinesPerMessageTextBox.DataBindings.Add(new System.Windows.Forms.Binding("Text", this.bindingSource, "MaxLinesPerMessage", true));
            this.maxLinesPerMessageTextBox.Location = new System.Drawing.Point(185, 64);
            this.maxLinesPerMessageTextBox.Name = "maxLinesPerMessageTextBox";
            this.maxLinesPerMessageTextBox.Size = new System.Drawing.Size(100, 20);
            this.maxLinesPerMessageTextBox.TabIndex = 5;
            // 
            // nextLineFunctionCodeTextBox
            // 
            this.nextLineFunctionCodeTextBox.AcceptsReturn = true;
            this.nextLineFunctionCodeTextBox.DataBindings.Add(new System.Windows.Forms.Binding("Text", this.bindingSource, "NextLineFunctionCode", true));
            this.nextLineFunctionCodeTextBox.Location = new System.Drawing.Point(185, 118);
            this.nextLineFunctionCodeTextBox.Multiline = true;
            this.nextLineFunctionCodeTextBox.Name = "nextLineFunctionCodeTextBox";
            this.nextLineFunctionCodeTextBox.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.nextLineFunctionCodeTextBox.Size = new System.Drawing.Size(194, 35);
            this.nextLineFunctionCodeTextBox.TabIndex = 18;
            // 
            // nextLineFunctionNameTextBox
            // 
            this.nextLineFunctionNameTextBox.DataBindings.Add(new System.Windows.Forms.Binding("Text", this.bindingSource, "NextLineFunctionName", true));
            this.nextLineFunctionNameTextBox.Location = new System.Drawing.Point(185, 12);
            this.nextLineFunctionNameTextBox.Name = "nextLineFunctionNameTextBox";
            this.nextLineFunctionNameTextBox.Size = new System.Drawing.Size(100, 20);
            this.nextLineFunctionNameTextBox.TabIndex = 1;
            // 
            // nextMessageFunctionCodeTextBox
            // 
            this.nextMessageFunctionCodeTextBox.AcceptsReturn = true;
            this.nextMessageFunctionCodeTextBox.DataBindings.Add(new System.Windows.Forms.Binding("Text", this.bindingSource, "NextMessageFunctionCode", true));
            this.nextMessageFunctionCodeTextBox.Location = new System.Drawing.Point(185, 159);
            this.nextMessageFunctionCodeTextBox.Multiline = true;
            this.nextMessageFunctionCodeTextBox.Name = "nextMessageFunctionCodeTextBox";
            this.nextMessageFunctionCodeTextBox.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.nextMessageFunctionCodeTextBox.Size = new System.Drawing.Size(194, 35);
            this.nextMessageFunctionCodeTextBox.TabIndex = 20;
            // 
            // nextMessageFunctionNameTextBox
            // 
            this.nextMessageFunctionNameTextBox.DataBindings.Add(new System.Windows.Forms.Binding("Text", this.bindingSource, "NextMessageFunctionName", true));
            this.nextMessageFunctionNameTextBox.Location = new System.Drawing.Point(185, 38);
            this.nextMessageFunctionNameTextBox.Name = "nextMessageFunctionNameTextBox";
            this.nextMessageFunctionNameTextBox.Size = new System.Drawing.Size(100, 20);
            this.nextMessageFunctionNameTextBox.TabIndex = 3;
            // 
            // reduceMarginFunctionNameTextBox
            // 
            this.reduceMarginFunctionNameTextBox.AcceptsReturn = true;
            this.reduceMarginFunctionNameTextBox.DataBindings.Add(new System.Windows.Forms.Binding("Text", this.bindingSource, "ReduceMarginFunctionName", true));
            this.reduceMarginFunctionNameTextBox.Location = new System.Drawing.Point(185, 200);
            this.reduceMarginFunctionNameTextBox.Multiline = true;
            this.reduceMarginFunctionNameTextBox.Name = "reduceMarginFunctionNameTextBox";
            this.reduceMarginFunctionNameTextBox.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.reduceMarginFunctionNameTextBox.Size = new System.Drawing.Size(194, 35);
            this.reduceMarginFunctionNameTextBox.TabIndex = 22;
            // 
            // okButton
            // 
            this.okButton.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.okButton.Location = new System.Drawing.Point(223, 336);
            this.okButton.Name = "okButton";
            this.okButton.Size = new System.Drawing.Size(75, 23);
            this.okButton.TabIndex = 15;
            this.okButton.Text = "OK";
            this.okButton.UseVisualStyleBackColor = true;
            // 
            // cancelButton
            // 
            this.cancelButton.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.cancelButton.Location = new System.Drawing.Point(304, 336);
            this.cancelButton.Name = "cancelButton";
            this.cancelButton.Size = new System.Drawing.Size(75, 23);
            this.cancelButton.TabIndex = 16;
            this.cancelButton.Text = "Cancel";
            this.cancelButton.UseVisualStyleBackColor = true;
            // 
            // toolTip1
            // 
            this.toolTip1.AutoPopDelay = 10000;
            this.toolTip1.InitialDelay = 10;
            this.toolTip1.ReshowDelay = 10;
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.toolStrip1);
            this.panel1.Location = new System.Drawing.Point(304, 0);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(89, 28);
            this.panel1.TabIndex = 0;
            // 
            // toolStrip1
            // 
            this.toolStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripDropDownButton1});
            this.toolStrip1.Location = new System.Drawing.Point(0, 0);
            this.toolStrip1.Name = "toolStrip1";
            this.toolStrip1.Size = new System.Drawing.Size(89, 25);
            this.toolStrip1.TabIndex = 0;
            this.toolStrip1.Text = "toolStrip1";
            // 
            // toolStripDropDownButton1
            // 
            this.toolStripDropDownButton1.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.toolStripDropDownButton1.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripDropDownButton1.Name = "toolStripDropDownButton1";
            this.toolStripDropDownButton1.Size = new System.Drawing.Size(52, 22);
            this.toolStripDropDownButton1.Text = "Import";
            this.toolStripDropDownButton1.DropDownOpening += new System.EventHandler(this.toolStripDropDownButton1_DropDownOpening);
            // 
            // button1
            // 
            this.button1.DialogResult = System.Windows.Forms.DialogResult.Yes;
            this.button1.Location = new System.Drawing.Point(12, 336);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(205, 23);
            this.button1.TabIndex = 14;
            this.button1.Text = "&Save options but don\'t word wrap";
            this.button1.UseVisualStyleBackColor = true;
            // 
            // useVariableWidthFontCheckBox
            // 
            this.useVariableWidthFontCheckBox.AutoSize = true;
            this.useVariableWidthFontCheckBox.DataBindings.Add(new System.Windows.Forms.Binding("CheckState", this.bindingSource, "UseVariableWidthFont", true));
            this.useVariableWidthFontCheckBox.Location = new System.Drawing.Point(133, 267);
            this.useVariableWidthFontCheckBox.Name = "useVariableWidthFontCheckBox";
            this.useVariableWidthFontCheckBox.Size = new System.Drawing.Size(143, 17);
            this.useVariableWidthFontCheckBox.TabIndex = 10;
            this.useVariableWidthFontCheckBox.Text = "Use Variaible-Width Font";
            this.useVariableWidthFontCheckBox.UseVisualStyleBackColor = true;
            // 
            // maintainIndentationCheckBox
            // 
            this.maintainIndentationCheckBox.AutoSize = true;
            this.maintainIndentationCheckBox.DataBindings.Add(new System.Windows.Forms.Binding("CheckState", this.bindingSource, "MaintainIndentation", true));
            this.maintainIndentationCheckBox.Location = new System.Drawing.Point(133, 290);
            this.maintainIndentationCheckBox.Name = "maintainIndentationCheckBox";
            this.maintainIndentationCheckBox.Size = new System.Drawing.Size(224, 17);
            this.maintainIndentationCheckBox.TabIndex = 12;
            this.maintainIndentationCheckBox.Text = "Maintain Indentation after 「 or wide space";
            this.maintainIndentationCheckBox.UseVisualStyleBackColor = true;
            // 
            // removeLineBreaksIfWordWrappingCheckBox
            // 
            this.removeLineBreaksIfWordWrappingCheckBox.AutoSize = true;
            this.removeLineBreaksIfWordWrappingCheckBox.DataBindings.Add(new System.Windows.Forms.Binding("CheckState", this.bindingSource, "RemoveLineBreaksIfWordWrapping", true));
            this.removeLineBreaksIfWordWrappingCheckBox.Location = new System.Drawing.Point(133, 313);
            this.removeLineBreaksIfWordWrappingCheckBox.Name = "removeLineBreaksIfWordWrappingCheckBox";
            this.removeLineBreaksIfWordWrappingCheckBox.Size = new System.Drawing.Size(226, 17);
            this.removeLineBreaksIfWordWrappingCheckBox.TabIndex = 13;
            this.removeLineBreaksIfWordWrappingCheckBox.Text = "Remove Line Breaks when word wrapping";
            this.removeLineBreaksIfWordWrappingCheckBox.UseVisualStyleBackColor = true;
            // 
            // selectFontButton
            // 
            this.selectFontButton.Location = new System.Drawing.Point(282, 263);
            this.selectFontButton.Name = "selectFontButton";
            this.selectFontButton.Size = new System.Drawing.Size(95, 23);
            this.selectFontButton.TabIndex = 11;
            this.selectFontButton.Text = "Select Font...";
            this.selectFontButton.UseVisualStyleBackColor = true;
            this.selectFontButton.Click += new System.EventHandler(this.selectFontButton_Click);
            // 
            // WordWrapForm
            // 
            this.AcceptButton = this.okButton;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.cancelButton;
            this.ClientSize = new System.Drawing.Size(391, 369);
            this.Controls.Add(this.selectFontButton);
            this.Controls.Add(this.removeLineBreaksIfWordWrappingCheckBox);
            this.Controls.Add(this.maintainIndentationCheckBox);
            this.Controls.Add(this.useVariableWidthFontCheckBox);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.panel1);
            this.Controls.Add(this.cancelButton);
            this.Controls.Add(this.okButton);
            this.Controls.Add(reduceMarginFunctionNameLabel);
            this.Controls.Add(this.reduceMarginFunctionNameTextBox);
            this.Controls.Add(nextMessageFunctionNameLabel);
            this.Controls.Add(this.nextMessageFunctionNameTextBox);
            this.Controls.Add(nextMessageFunctionCodeLabel);
            this.Controls.Add(this.nextMessageFunctionCodeTextBox);
            this.Controls.Add(nextLineFunctionNameLabel);
            this.Controls.Add(this.nextLineFunctionNameTextBox);
            this.Controls.Add(nextLineFunctionCodeLabel);
            this.Controls.Add(this.nextLineFunctionCodeTextBox);
            this.Controls.Add(maxLinesPerMessageLabel);
            this.Controls.Add(this.maxLinesPerMessageTextBox);
            this.Controls.Add(maxCharactersPerLineReducedLabel);
            this.Controls.Add(this.maxCharactersPerLineReducedTextBox);
            this.Controls.Add(maxCharactersPerLineNormalLabel);
            this.Controls.Add(this.maxCharactersPerLineNormalTextBox);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.Name = "WordWrapForm";
            this.Text = "Word Wrap Messages";
            this.Load += new System.EventHandler(this.WordWrapForm_Load);
            ((System.ComponentModel.ISupportInitialize)(this.bindingSource)).EndInit();
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.toolStrip1.ResumeLayout(false);
            this.toolStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.BindingSource bindingSource;
        private System.Windows.Forms.TextBox maxCharactersPerLineNormalTextBox;
        private System.Windows.Forms.TextBox maxCharactersPerLineReducedTextBox;
        private System.Windows.Forms.TextBox maxLinesPerMessageTextBox;
        private System.Windows.Forms.TextBox nextLineFunctionCodeTextBox;
        private System.Windows.Forms.TextBox nextLineFunctionNameTextBox;
        private System.Windows.Forms.TextBox nextMessageFunctionCodeTextBox;
        private System.Windows.Forms.TextBox nextMessageFunctionNameTextBox;
        private System.Windows.Forms.TextBox reduceMarginFunctionNameTextBox;
        private System.Windows.Forms.Button okButton;
        private System.Windows.Forms.Button cancelButton;
        private System.Windows.Forms.ToolTip toolTip1;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.ToolStrip toolStrip1;
        private System.Windows.Forms.ToolStripDropDownButton toolStripDropDownButton1;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.CheckBox useVariableWidthFontCheckBox;
        private System.Windows.Forms.CheckBox maintainIndentationCheckBox;
        private System.Windows.Forms.CheckBox removeLineBreaksIfWordWrappingCheckBox;
        private System.Windows.Forms.Button selectFontButton;
    }
}