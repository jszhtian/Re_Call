using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.IO;
using System.Text;
using System.Windows.Forms;

namespace AinDecompiler
{
    public partial class TextPromptForm : Form
    {
        public string FileName = "";
        public bool ShowOkButton
        {
            get
            {
                return okButton.Visible;
            }
            set
            {
                if (value == true)
                {
                    cancelButton.Text = "Cancel";
                }
                else
                {
                    cancelButton.Text = "Close";
                }
                okButton.Visible = value;
            }
        }

        public TextPromptForm()
        {
            InitializeComponent();
        }

        private void openTextFileButton_Click(object sender, EventArgs e)
        {
            using (var openFileDialog = new OpenFileDialog())
            {
                openFileDialog.DefaultExt = "txt";
                openFileDialog.Filter = "Text Files (*.txt)|*.txt|All Files (*.*)|*.*";
                if (openFileDialog.ShowDialog() == DialogResult.OK)
                {
                    this.FileName = openFileDialog.FileName;
                    var encoding = EncodingDetector.DetectEncoding(openFileDialog.FileName);
                    scintilla1.Text = File.ReadAllText(openFileDialog.FileName, encoding);
                }
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            using (var saveFileDialog = new SaveFileDialog())
            {
                saveFileDialog.DefaultExt = "txt";
                saveFileDialog.Filter = "Text Files (*.txt)|*.txt|All Files (*.*)|*.*";
                saveFileDialog.FileName = this.FileName;
                if (saveFileDialog.ShowDialog() == DialogResult.OK)
                {
                    File.WriteAllText(saveFileDialog.FileName, scintilla1.Text, Extensions.TextEncoding);
                }
            }
        }

        private void TextPromptForm_Load(object sender, EventArgs e)
        {

        }
    }
}
