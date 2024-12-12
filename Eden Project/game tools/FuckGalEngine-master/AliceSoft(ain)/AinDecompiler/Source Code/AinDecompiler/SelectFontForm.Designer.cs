namespace AinDecompiler
{
    partial class SelectFontForm
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
            this.FontNameComboBox1 = new System.Windows.Forms.ComboBox();
            this.FontSizeTextBox1 = new System.Windows.Forms.TextBox();
            this.lblTakeScreenshot = new System.Windows.Forms.Label();
            this.FontBoldCheckBox1 = new System.Windows.Forms.CheckBox();
            this.lblKanjiWidth = new System.Windows.Forms.Label();
            this.lblSimilarFont = new System.Windows.Forms.Label();
            this.FontNameComboBox2 = new System.Windows.Forms.ComboBox();
            this.GetFromFontButton = new System.Windows.Forms.Button();
            this.KanjiWidthTextBox = new System.Windows.Forms.TextBox();
            this.FontSizeTextBox2 = new System.Windows.Forms.TextBox();
            this.FontBoldCheckbox2 = new System.Windows.Forms.CheckBox();
            this.cancelButton = new System.Windows.Forms.Button();
            this.OkButton = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // FontNameComboBox1
            // 
            this.FontNameComboBox1.FormattingEnabled = true;
            this.FontNameComboBox1.Location = new System.Drawing.Point(12, 53);
            this.FontNameComboBox1.Name = "FontNameComboBox1";
            this.FontNameComboBox1.Size = new System.Drawing.Size(173, 21);
            this.FontNameComboBox1.TabIndex = 1;
            // 
            // FontSizeTextBox1
            // 
            this.FontSizeTextBox1.Location = new System.Drawing.Point(191, 54);
            this.FontSizeTextBox1.Name = "FontSizeTextBox1";
            this.FontSizeTextBox1.Size = new System.Drawing.Size(52, 20);
            this.FontSizeTextBox1.TabIndex = 2;
            // 
            // lblTakeScreenshot
            // 
            this.lblTakeScreenshot.Location = new System.Drawing.Point(12, 9);
            this.lblTakeScreenshot.Name = "lblTakeScreenshot";
            this.lblTakeScreenshot.Size = new System.Drawing.Size(324, 42);
            this.lblTakeScreenshot.TabIndex = 0;
            this.lblTakeScreenshot.Text = "Take a screenshot of the game, and use MS Paint to figure out which font most clo" +
                "sely resembles the game text.  Look especially at the widths of characters.";
            // 
            // FontBoldCheckBox1
            // 
            this.FontBoldCheckBox1.AutoSize = true;
            this.FontBoldCheckBox1.Location = new System.Drawing.Point(249, 55);
            this.FontBoldCheckBox1.Name = "FontBoldCheckBox1";
            this.FontBoldCheckBox1.Size = new System.Drawing.Size(47, 17);
            this.FontBoldCheckBox1.TabIndex = 3;
            this.FontBoldCheckBox1.Text = "Bold";
            this.FontBoldCheckBox1.UseVisualStyleBackColor = true;
            // 
            // lblKanjiWidth
            // 
            this.lblKanjiWidth.AutoSize = true;
            this.lblKanjiWidth.Location = new System.Drawing.Point(12, 86);
            this.lblKanjiWidth.Name = "lblKanjiWidth";
            this.lblKanjiWidth.Size = new System.Drawing.Size(328, 13);
            this.lblKanjiWidth.TabIndex = 4;
            this.lblKanjiWidth.Text = "Width of a Kanji character in pixels:  (pixels between two characters)";
            // 
            // lblSimilarFont
            // 
            this.lblSimilarFont.AutoSize = true;
            this.lblSimilarFont.Location = new System.Drawing.Point(13, 142);
            this.lblSimilarFont.Name = "lblSimilarFont";
            this.lblSimilarFont.Size = new System.Drawing.Size(200, 13);
            this.lblSimilarFont.TabIndex = 7;
            this.lblSimilarFont.Text = "Or determine the width from a similar font:";
            // 
            // FontNameComboBox2
            // 
            this.FontNameComboBox2.FormattingEnabled = true;
            this.FontNameComboBox2.Location = new System.Drawing.Point(13, 158);
            this.FontNameComboBox2.Name = "FontNameComboBox2";
            this.FontNameComboBox2.Size = new System.Drawing.Size(173, 21);
            this.FontNameComboBox2.TabIndex = 8;
            // 
            // GetFromFontButton
            // 
            this.GetFromFontButton.Location = new System.Drawing.Point(70, 103);
            this.GetFromFontButton.Name = "GetFromFontButton";
            this.GetFromFontButton.Size = new System.Drawing.Size(173, 23);
            this.GetFromFontButton.TabIndex = 6;
            this.GetFromFontButton.Text = "Get from selected font below";
            this.GetFromFontButton.UseVisualStyleBackColor = true;
            this.GetFromFontButton.Click += new System.EventHandler(this.GetFromFontButton_Click);
            // 
            // KanjiWidthTextBox
            // 
            this.KanjiWidthTextBox.Location = new System.Drawing.Point(12, 105);
            this.KanjiWidthTextBox.Name = "KanjiWidthTextBox";
            this.KanjiWidthTextBox.Size = new System.Drawing.Size(52, 20);
            this.KanjiWidthTextBox.TabIndex = 5;
            // 
            // FontSizeTextBox2
            // 
            this.FontSizeTextBox2.Location = new System.Drawing.Point(192, 158);
            this.FontSizeTextBox2.Name = "FontSizeTextBox2";
            this.FontSizeTextBox2.Size = new System.Drawing.Size(52, 20);
            this.FontSizeTextBox2.TabIndex = 9;
            // 
            // FontBoldCheckbox2
            // 
            this.FontBoldCheckbox2.AutoSize = true;
            this.FontBoldCheckbox2.Location = new System.Drawing.Point(250, 160);
            this.FontBoldCheckbox2.Name = "FontBoldCheckbox2";
            this.FontBoldCheckbox2.Size = new System.Drawing.Size(47, 17);
            this.FontBoldCheckbox2.TabIndex = 10;
            this.FontBoldCheckbox2.Text = "Bold";
            this.FontBoldCheckbox2.UseVisualStyleBackColor = true;
            // 
            // cancelButton
            // 
            this.cancelButton.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.cancelButton.Location = new System.Drawing.Point(262, 190);
            this.cancelButton.Name = "cancelButton";
            this.cancelButton.Size = new System.Drawing.Size(75, 23);
            this.cancelButton.TabIndex = 12;
            this.cancelButton.Text = "Cancel";
            this.cancelButton.UseVisualStyleBackColor = true;
            // 
            // OkButton
            // 
            this.OkButton.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.OkButton.Location = new System.Drawing.Point(181, 190);
            this.OkButton.Name = "OkButton";
            this.OkButton.Size = new System.Drawing.Size(75, 23);
            this.OkButton.TabIndex = 11;
            this.OkButton.Text = "OK";
            this.OkButton.UseVisualStyleBackColor = true;
            // 
            // SelectFontForm
            // 
            this.AcceptButton = this.OkButton;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.cancelButton;
            this.ClientSize = new System.Drawing.Size(348, 227);
            this.Controls.Add(this.OkButton);
            this.Controls.Add(this.cancelButton);
            this.Controls.Add(this.FontBoldCheckbox2);
            this.Controls.Add(this.FontSizeTextBox2);
            this.Controls.Add(this.KanjiWidthTextBox);
            this.Controls.Add(this.GetFromFontButton);
            this.Controls.Add(this.FontNameComboBox2);
            this.Controls.Add(this.lblSimilarFont);
            this.Controls.Add(this.lblKanjiWidth);
            this.Controls.Add(this.FontBoldCheckBox1);
            this.Controls.Add(this.lblTakeScreenshot);
            this.Controls.Add(this.FontSizeTextBox1);
            this.Controls.Add(this.FontNameComboBox1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "SelectFontForm";
            this.Text = "Select Template Font";
            this.Load += new System.EventHandler(this.SelectFontForm_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ComboBox FontNameComboBox1;
        private System.Windows.Forms.TextBox FontSizeTextBox1;
        private System.Windows.Forms.Label lblTakeScreenshot;
        private System.Windows.Forms.CheckBox FontBoldCheckBox1;
        private System.Windows.Forms.Label lblKanjiWidth;
        private System.Windows.Forms.Label lblSimilarFont;
        private System.Windows.Forms.ComboBox FontNameComboBox2;
        private System.Windows.Forms.Button GetFromFontButton;
        private System.Windows.Forms.TextBox KanjiWidthTextBox;
        private System.Windows.Forms.TextBox FontSizeTextBox2;
        private System.Windows.Forms.CheckBox FontBoldCheckbox2;
        private System.Windows.Forms.Button cancelButton;
        private System.Windows.Forms.Button OkButton;
    }
}