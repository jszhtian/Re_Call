using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Text;
using System.Text.Json;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace PackTool.GUI
{
    public partial class Main : Form
    {
        public Main()
        {
            InitializeComponent();
        }

        private void Main_Load(object sender, EventArgs e)
        {
            ReadConfig();

            _TextBox_ExtractScriptKey.Enabled = _CheckBox_ExtractDecryptScript.Checked;
            _TextBox_PackScriptKey.Enabled = _CheckBox_PackEncryptScript.Checked;
        }

        private void CheckBox_ExtractDecryptScript_CheckedChanged(object sender, EventArgs e)
        {
            _TextBox_ExtractScriptKey.Enabled = _CheckBox_ExtractDecryptScript.Checked;
        }

        private void CheckBox_PackEncryptScript_CheckedChanged(object sender, EventArgs e)
        {
            _TextBox_PackScriptKey.Enabled = _CheckBox_PackEncryptScript.Checked;
        }

        private void Button_BrowseExtractInput_Click(object sender, EventArgs e)
        {
            var dlg = new OpenFileDialog();
            if (!string.IsNullOrEmpty(_TextBox_ExtractInput.Text))
            {
                string path = Path.GetDirectoryName(_TextBox_ExtractInput.Text);
                if (Directory.Exists(path))
                {
                    dlg.InitialDirectory = path;
                    dlg.FileName = Path.GetFileName(_TextBox_ExtractInput.Text);
                }
            }
            if (dlg.ShowDialog(this) != DialogResult.OK)
                return;
            _TextBox_ExtractInput.Text = dlg.FileName;
        }

        private void Button_BrowseExtractOutput_Click(object sender, EventArgs e)
        {
            var dlg = new FolderBrowserDialog();
            if (!string.IsNullOrEmpty(_TextBox_ExtractOutput.Text) && Directory.Exists(_TextBox_ExtractOutput.Text))
                dlg.SelectedPath = _TextBox_ExtractOutput.Text;
            if (dlg.ShowDialog(this) != DialogResult.OK)
                return;
            _TextBox_ExtractOutput.Text = dlg.SelectedPath;
        }

        private void Button_Extract_Click(object sender, EventArgs e)
        {
            if (string.IsNullOrEmpty(_TextBox_ExtractInput.Text))
            {
                MessageBox.Show(this, "请选择要解包的文件。", "提示", MessageBoxButtons.OK, MessageBoxIcon.Information);
                return;
            }
            if (string.IsNullOrEmpty(_TextBox_ExtractOutput.Text))
            {
                MessageBox.Show(this, "请选择解包输出目录。", "提示", MessageBoxButtons.OK, MessageBoxIcon.Information);
                return;
            }
            if (string.IsNullOrEmpty(_TextBox_ExtractArchiveKey.Text))
            {
                MessageBox.Show(this, "请输入封包密钥。", "提示", MessageBoxButtons.OK, MessageBoxIcon.Information);
                return;
            }
            if (_CheckBox_ExtractDecryptScript.Checked && string.IsNullOrEmpty(_TextBox_ExtractScriptKey.Text))
            {
                MessageBox.Show(this, "请输入脚本密钥。", "提示", MessageBoxButtons.OK, MessageBoxIcon.Information);
                return;
            }

            DialogResult dr = MessageBox.Show(this, "是否开始解包？", "提示", MessageBoxButtons.YesNo, MessageBoxIcon.Question);
            if (dr != DialogResult.Yes)
            {
                return;
            }

            try
            {
                string filePath = _TextBox_ExtractInput.Text;
                string outputPath = _TextBox_ExtractOutput.Text;
                uint archiveKey = uint.Parse(_TextBox_ExtractArchiveKey.Text, NumberStyles.HexNumber);
                bool decryptScript = _CheckBox_ExtractDecryptScript.Checked;
                uint scriptKey = decryptScript ? uint.Parse(_TextBox_ExtractScriptKey.Text, NumberStyles.HexNumber) : 0;

                var pack = new Pack();
                pack.Extract(filePath, outputPath, archiveKey, decryptScript, scriptKey);

                MessageBox.Show(this, "解包完成。", "提示", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
            catch (Exception x)
            {
                MessageBox.Show(this, $"{x.Message}\n{x.StackTrace}", "错误", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void Button_BrowsePackInput_Click(object sender, EventArgs e)
        {
            var dlg = new FolderBrowserDialog();
            if (!string.IsNullOrEmpty(_TextBox_PackInput.Text) && Directory.Exists(_TextBox_PackInput.Text))
                dlg.SelectedPath = _TextBox_PackInput.Text;
            if (dlg.ShowDialog(this) != DialogResult.OK)
                return;
            _TextBox_PackInput.Text = dlg.SelectedPath;
        }

        private void Button_BrowsePackOutput_Click(object sender, EventArgs e)
        {
            var dlg = new SaveFileDialog();
            if (!string.IsNullOrEmpty(_TextBox_PackOutput.Text))
            {
                string path = Path.GetDirectoryName(_TextBox_PackOutput.Text);
                if (Directory.Exists(path))
                {
                    dlg.InitialDirectory = path;
                    dlg.FileName = Path.GetFileName(_TextBox_PackOutput.Text);
                }
            }
            if (dlg.ShowDialog(this) != DialogResult.OK)
                return;
            _TextBox_PackOutput.Text = dlg.FileName;
        }

        private void Button_Pack_Click(object sender, EventArgs e)
        {
            if (string.IsNullOrEmpty(_TextBox_PackInput.Text))
            {
                MessageBox.Show(this, "请选择要封包的目录。", "提示", MessageBoxButtons.OK, MessageBoxIcon.Information);
                _TextBox_PackInput.Focus();
                return;
            }
            if (string.IsNullOrEmpty(_TextBox_PackOutput.Text))
            {
                MessageBox.Show(this, "请选择输出文件路径。", "提示", MessageBoxButtons.OK, MessageBoxIcon.Information);
                _TextBox_PackOutput.Focus();
                return;
            }
            if (string.IsNullOrEmpty(_TextBox_PackArchiveKey.Text))
            {
                MessageBox.Show(this, "请输入封包密钥。", "提示", MessageBoxButtons.OK, MessageBoxIcon.Information);
                _TextBox_PackArchiveKey.Focus();
                return;
            }
            if (_CheckBox_PackEncryptScript.Checked && string.IsNullOrEmpty(_TextBox_PackScriptKey.Text))
            {
                MessageBox.Show(this, "请输入脚本密钥。", "提示", MessageBoxButtons.OK, MessageBoxIcon.Information);
                _TextBox_PackScriptKey.Focus();
                return;
            }

            DialogResult dr = MessageBox.Show(this, "是否开始封包？", "提示", MessageBoxButtons.YesNo, MessageBoxIcon.Question);
            if (dr != DialogResult.Yes)
            {
                return;
            }

            try
            {
                string inputPath = _TextBox_PackInput.Text;
                string filePath = _TextBox_PackOutput.Text;
                uint archiveKey = uint.Parse(_TextBox_PackArchiveKey.Text, NumberStyles.HexNumber);
                bool encryptScript = _CheckBox_PackEncryptScript.Checked;
                uint scriptKey = encryptScript ? uint.Parse(_TextBox_PackScriptKey.Text, NumberStyles.HexNumber) : 0;

                var pack = new Pack();
                pack.Create(inputPath, filePath, archiveKey, encryptScript, scriptKey);

                MessageBox.Show(this, "封包完成。", "提示", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
            catch (Exception x)
            {
                MessageBox.Show(this, $"{x.Message}\n{x.StackTrace}", "错误", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void Button_Readme_Click(object sender, EventArgs e)
        {
            StringBuilder sb = new StringBuilder(500);
            sb.AppendLine("一、脚本只支持ANSI编码，即：GBK、Shift-JIS等，不支持UTF-8和UTF-16。");
            sb.AppendLine("二、每个脚本文件的编码必须相同。");
            sb.AppendLine("三、解包出来的文件，其文件名不可更改，否则封包时将会出错。");
            sb.AppendLine("四、解包和封包都需要正确的密钥。");
            sb.AppendLine("五、解包和封包脚本时需要额外的密钥。");

            MessageBox.Show(this, sb.ToString(), "说明", MessageBoxButtons.OK, MessageBoxIcon.Information);
        }

        private void ReadConfig()
        {
            try
            {
                string filePath = Path.Combine(Application.StartupPath, "config.json");
                string json = File.ReadAllText(filePath);
                var config = JsonSerializer.Deserialize<AppConfig>(json);

                _TextBox_ExtractArchiveKey.Text = config.ArchiveKey;
                _TextBox_ExtractScriptKey.Text = config.ScriptKey;
                _TextBox_PackArchiveKey.Text = config.ArchiveKey;
                _TextBox_PackScriptKey.Text = config.ScriptKey;
            }
            catch (Exception)
            {
            }
        }
    }
}
