using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace TextEncodingTool
{
    public partial class Main : Form
    {
        public Main()
        {
            InitializeComponent();
        }

        private void Main_Load(object sender, EventArgs e)
        {
            Encoding.RegisterProvider(CodePagesEncodingProvider.Instance);

            _comboBox_codePage.Items.Add("utf-8");
            _comboBox_codePage.Items.Add("utf-16le");
            _comboBox_codePage.Items.Add("utf-16be");
            _comboBox_codePage.Items.Add("gbk");
            _comboBox_codePage.Items.Add("shift-jis");

            _comboBox_codeStyle.Items.Add("Hex Space");
            _comboBox_codeStyle.Items.Add("Hex");
            _comboBox_codeStyle.Items.Add("C String");
            _comboBox_codeStyle.Items.Add("C Array");

            _comboBox_codeStyle.SelectedIndex = 0;
        }

        private void Button_encode_Click(object sender, EventArgs e)
        {
            if (_comboBox_codePage.SelectedIndex == -1)
                return;

            if (_textBox_text.TextLength == 0)
                return;

            byte[] bytes;
            try
            {
                Encoding encoding = Encoding.GetEncoding(_comboBox_codePage.SelectedItem as string);
                bytes = encoding.GetBytes(_textBox_text.Text);
            }
            catch (Exception x)
            {
                MessageBox.Show(this, x.Message, "Warning", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;
            }

            StringBuilder sb = new StringBuilder();

            switch (_comboBox_codeStyle.SelectedIndex)
            {
                case 0: // Hex Space
                {
                    for (int i = 0; i < bytes.Length; i++)
                    {
                        if (i != 0)
                            sb.Append(' ');
                        sb.Append(bytes[i].ToString("X2"));
                    }
                    break;
                }
                case 1: // Hex
                {
                    for (int i = 0; i < bytes.Length; i++)
                    {
                        sb.Append(bytes[i].ToString("X2"));
                    }
                    break;
                }
                case 2: // C String
                {
                    sb.Append('"');
                    for (int i = 0; i < bytes.Length; i++)
                    {
                        sb.Append($"\\x{bytes[i]:X2}");
                    }
                    sb.Append('"');
                    break;
                }
                case 3: // C Array
                {
                    sb.Append("{ ");
                    for (int i = 0; i < bytes.Length; i++)
                    {
                        if (i != 0)
                            sb.Append(", ");
                        sb.Append($"0x{bytes[i]:X2}");
                    }
                    sb.Append(" }");
                    break;
                }
            }

            _textBox_code.Text = sb.ToString();

            if (_checkBox_autoCopy.Checked)
            {
                Clipboard.SetText(_textBox_code.Text);
            }
        }

        private void Button_decode_Click(object sender, EventArgs e)
        {
            if (_comboBox_codePage.SelectedIndex == -1)
                return;

            if (_textBox_code.TextLength == 0)
                return;

            StringBuilder sb = new StringBuilder(_textBox_code.Text.Length);

            for (int i = 0; i < _textBox_code.Text.Length; i++)
            {
                char c = _textBox_code.Text[i];
                if (c != ' ' && !(c >= '0' && c <= '9') && !(c >= 'A' && c <= 'Z'))
                {
                    MessageBox.Show(this, "Invalid code.", "Warning", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                    return;
                }
                if (c == ' ')
                    continue;

                sb.Append(c);
            }

            try
            {
                byte[] bytes = Convert.FromHexString(sb.ToString());
                Encoding encoding = Encoding.GetEncoding(_comboBox_codePage.SelectedItem as string);
                _textBox_text.Text = encoding.GetString(bytes);
            }
            catch (Exception x)
            {
                MessageBox.Show(this, x.Message, "Warning", MessageBoxButtons.OK, MessageBoxIcon.Warning);
            }
        }
    }
}
