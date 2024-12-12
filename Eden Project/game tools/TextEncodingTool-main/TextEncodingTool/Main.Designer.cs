
namespace TextEncodingTool
{
    partial class Main
    {
        /// <summary>
        ///  Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        ///  Clean up any resources being used.
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
        ///  Required method for Designer support - do not modify
        ///  the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this._textBox_text = new System.Windows.Forms.TextBox();
            this._comboBox_codePage = new System.Windows.Forms.ComboBox();
            this._button_encode = new System.Windows.Forms.Button();
            this._button_decode = new System.Windows.Forms.Button();
            this._textBox_code = new System.Windows.Forms.TextBox();
            this._comboBox_codeStyle = new System.Windows.Forms.ComboBox();
            this._checkBox_autoCopy = new System.Windows.Forms.CheckBox();
            this.SuspendLayout();
            // 
            // _textBox_text
            // 
            this._textBox_text.Location = new System.Drawing.Point(12, 12);
            this._textBox_text.Multiline = true;
            this._textBox_text.Name = "_textBox_text";
            this._textBox_text.Size = new System.Drawing.Size(776, 176);
            this._textBox_text.TabIndex = 0;
            // 
            // _comboBox_codePage
            // 
            this._comboBox_codePage.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this._comboBox_codePage.FormattingEnabled = true;
            this._comboBox_codePage.Location = new System.Drawing.Point(12, 194);
            this._comboBox_codePage.Name = "_comboBox_codePage";
            this._comboBox_codePage.Size = new System.Drawing.Size(240, 25);
            this._comboBox_codePage.TabIndex = 1;
            // 
            // _button_encode
            // 
            this._button_encode.Location = new System.Drawing.Point(564, 194);
            this._button_encode.Name = "_button_encode";
            this._button_encode.Size = new System.Drawing.Size(109, 25);
            this._button_encode.TabIndex = 2;
            this._button_encode.Text = "👇编码";
            this._button_encode.UseVisualStyleBackColor = true;
            this._button_encode.Click += new System.EventHandler(this.Button_encode_Click);
            // 
            // _button_decode
            // 
            this._button_decode.Location = new System.Drawing.Point(679, 194);
            this._button_decode.Name = "_button_decode";
            this._button_decode.Size = new System.Drawing.Size(109, 25);
            this._button_decode.TabIndex = 3;
            this._button_decode.Text = "👆解码";
            this._button_decode.UseVisualStyleBackColor = true;
            this._button_decode.Click += new System.EventHandler(this.Button_decode_Click);
            // 
            // _textBox_code
            // 
            this._textBox_code.Location = new System.Drawing.Point(12, 225);
            this._textBox_code.Multiline = true;
            this._textBox_code.Name = "_textBox_code";
            this._textBox_code.Size = new System.Drawing.Size(776, 213);
            this._textBox_code.TabIndex = 4;
            // 
            // _comboBox_codeStyle
            // 
            this._comboBox_codeStyle.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this._comboBox_codeStyle.FormattingEnabled = true;
            this._comboBox_codeStyle.Location = new System.Drawing.Point(258, 194);
            this._comboBox_codeStyle.Name = "_comboBox_codeStyle";
            this._comboBox_codeStyle.Size = new System.Drawing.Size(130, 25);
            this._comboBox_codeStyle.TabIndex = 5;
            // 
            // _checkBox_autoCopy
            // 
            this._checkBox_autoCopy.AutoSize = true;
            this._checkBox_autoCopy.Location = new System.Drawing.Point(394, 196);
            this._checkBox_autoCopy.Name = "_checkBox_autoCopy";
            this._checkBox_autoCopy.Size = new System.Drawing.Size(147, 21);
            this._checkBox_autoCopy.TabIndex = 6;
            this._checkBox_autoCopy.Text = "编码结果复制到剪切板";
            this._checkBox_autoCopy.UseVisualStyleBackColor = true;
            // 
            // Main
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 17F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(800, 450);
            this.Controls.Add(this._checkBox_autoCopy);
            this.Controls.Add(this._comboBox_codeStyle);
            this.Controls.Add(this._textBox_code);
            this.Controls.Add(this._button_decode);
            this.Controls.Add(this._button_encode);
            this.Controls.Add(this._comboBox_codePage);
            this.Controls.Add(this._textBox_text);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;
            this.Name = "Main";
            this.Text = "文本编码工具   作者：展鸿丶";
            this.Load += new System.EventHandler(this.Main_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox _textBox_text;
        private System.Windows.Forms.ComboBox _comboBox_codePage;
        private System.Windows.Forms.Button _button_encode;
        private System.Windows.Forms.Button _button_decode;
        private System.Windows.Forms.TextBox _textBox_code;
        private System.Windows.Forms.ComboBox _comboBox_codeStyle;
        private System.Windows.Forms.CheckBox _checkBox_autoCopy;
    }
}

