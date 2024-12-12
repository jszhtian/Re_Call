using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.IO;
using System.Text;
using System.Windows.Forms;

namespace scriptthingy
{
    public partial class MainForm : Form
    {
        public MainForm()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            GetSettings();
        }

        private void GetSettings()
        {
            scriptFileTextBox.Text = RegistryUtility.GetSetting("ScriptFileName", scriptFileTextBox.Text);
            pathTextBox.Text = RegistryUtility.GetSetting("TextPath", pathTextBox.Text);
            IndexFilePasswordTextBox.Text = RegistryUtility.GetSetting("IndexPassword", "1qaz2wsx3edc4rfv5tgb6yhn7ujm8ik,9ol.0p;/-@:^[]");
            DataFilePasswordTextBox.Text = RegistryUtility.GetSetting("DataPassword", "EAGLS_SYSTEM");
            UseDatFilesCheckBox.Checked = RegistryUtility.GetSetting("UseDatFiles", false);
        }

        void log(string text)
        {
            logTextBox.Text += text + "\r\n";
            logTextBox.SelectionLength = 0;
            logTextBox.SelectionStart = logTextBox.Text.Length;
            logTextBox.Refresh();

        }

        private void ReplaceScript(string fileName, string exportPath, bool useDatFiles)
        {
            string indexFileName = Path.ChangeExtension(fileName, ".idx");
            string dataFileName = Path.ChangeExtension(fileName, ".pak");

            var indexBytes = File.ReadAllBytes(indexFileName);

            var shiftJisEncoding = Encoding.GetEncoding("shift-jis");

            var indexKey = ScriptFile.FindIndexFileKey(indexBytes);
            if (indexKey == null)
            {
                log("Could not guess Index Password!  Using user-provided password.");
                indexKey = shiftJisEncoding.GetBytes(IndexFilePasswordTextBox.Text);
            }
            else
            {
                log("Index password is " + shiftJisEncoding.GetString(indexKey));
                IndexFilePasswordTextBox.Text = shiftJisEncoding.GetString(indexKey);
            }
            ScriptFile.DecryptIndexFile(indexBytes, indexKey);

            var dataBytes = File.ReadAllBytes(dataFileName);
            var dataKey = ScriptFile.FindDataFileKey(dataBytes, indexBytes);
            if (dataKey == null)
            {
                log("Could not guess Data Password!  Using user-provided password.");
                dataKey = shiftJisEncoding.GetBytes(DataFilePasswordTextBox.Text);
            }
            else
            {
                log("Data password is " + shiftJisEncoding.GetString(dataKey));
                DataFilePasswordTextBox.Text = shiftJisEncoding.GetString(dataKey);
            }
            ScriptFile.DecryptFiles(indexBytes, dataBytes, dataKey);

            byte[] newIndexFile, newDataFile;

            ScriptFile.ReplaceAllFiles(indexBytes, dataBytes, exportPath, out newIndexFile, out newDataFile, useDatFiles);
            ScriptFile.DecryptFiles(newIndexFile, newDataFile, dataKey);
            ScriptFile.DecryptIndexFile(newIndexFile, indexKey);

            File.WriteAllBytes(indexFileName, newIndexFile);
            File.WriteAllBytes(dataFileName, newDataFile);

            log("Finished replacing files!");
        }

        private void ExtractScript(string fileName, string exportPath, bool useDatFiles)
        {
            string indexFileName = Path.ChangeExtension(fileName, ".idx");
            string dataFileName = Path.ChangeExtension(fileName, ".pak");

            var indexBytes = File.ReadAllBytes(indexFileName);

            var shiftJisEncoding = Encoding.GetEncoding("shift-jis");

            var indexKey = ScriptFile.FindIndexFileKey(indexBytes);
            if (indexKey == null)
            {
                log("Could not guess Index Password!  Using user-provided password.");
                indexKey = shiftJisEncoding.GetBytes(IndexFilePasswordTextBox.Text);
            }
            else
            {
                log("Index password is " + shiftJisEncoding.GetString(indexKey));
                IndexFilePasswordTextBox.Text = shiftJisEncoding.GetString(indexKey);
            }
            ScriptFile.DecryptIndexFile(indexBytes, indexKey);

            var dataBytes = File.ReadAllBytes(dataFileName);
            var dataKey = shiftJisEncoding.GetBytes(DataFilePasswordTextBox.Text);//ScriptFile.FindDataFileKey(dataBytes, indexBytes);
            /*
            if (dataKey == null)
            {
                log("Could not guess Data Password!  Using user-provided password.");
                dataKey = shiftJisEncoding.GetBytes(DataFilePasswordTextBox.Text);
            }
            else
            {
                log("Data password is " + shiftJisEncoding.GetString(dataKey));
                DataFilePasswordTextBox.Text = shiftJisEncoding.GetString(dataKey);
            }
            */
            ScriptFile.DecryptFiles(indexBytes, dataBytes, dataKey);

            ScriptFile.DumpAllFiles(indexBytes, dataBytes, exportPath, useDatFiles);
            log("Finished extracting files!");
        }

        private void openScriptButton_Click(object sender, EventArgs e)
        {
            OpenScript();
        }

        private void OpenScript()
        {
            using (var openDialog = new OpenFileDialog())
            {
                openDialog.Filter = "IDX/PAK Files (*.idx;*.pak)|*.idx;*.pak|All Files (*.*)|*.*";

                var oldFileName = scriptFileTextBox.Text;
                string oldDirectory = "";
                try
                {
                    oldDirectory = Path.GetDirectoryName(oldFileName);
                }
                catch (Exception ex)
                {

                }
                openDialog.FileName = oldFileName;
                if (oldDirectory != "")
                {
                    openDialog.InitialDirectory = oldDirectory;
                }

                if (openDialog.ShowDialog() == DialogResult.OK)
                {
                    scriptFileTextBox.Text = openDialog.FileName;
                }
            }
        }

        private void extractScriptButton_Click(object sender, EventArgs e)
        {
            ExtractScript();
        }

        private void ExtractScript()
        {
            string path = pathTextBox.Text;
            string scriptFile = scriptFileTextBox.Text;
            if (String.IsNullOrEmpty(path))
            {
                MessageBox.Show("Please provide a directory to extract files to.", "Script Extractor", MessageBoxButtons.OK, MessageBoxIcon.Stop);
                return;
            }
            if (String.IsNullOrEmpty(scriptFile))
            {
                MessageBox.Show("Please provide a script file to extract files from.", "Script Extractor", MessageBoxButtons.OK, MessageBoxIcon.Stop);
                return;
            }

            if (!Directory.Exists(path))
            {
                try
                {
                    Directory.CreateDirectory(path);
                }
                catch (Exception ex)
                {
                    MessageBox.Show("Failed to create directory: " + ex.Message, "Script Extractor", MessageBoxButtons.OK, MessageBoxIcon.Stop);
                    return;
                }
            }
            else
            {
                try
                {
                    int fileCount = Directory.GetFiles(path).Length;
                    if (fileCount > 0)
                    {
                        if (MessageBox.Show("The output directory is not empty.  Replace files?", "Script Extractor", MessageBoxButtons.YesNo, MessageBoxIcon.Question) == DialogResult.No)
                        {
                            return;
                        }
                    }
                }
                catch (Exception ex)
                {
                    MessageBox.Show("Failed to read contents of directory: " + ex.Message, "Script Extractor", MessageBoxButtons.OK, MessageBoxIcon.Stop);
                    return;
                }
            }

            this.UseWaitCursor = true;
            //FileContent.cipher = keyTextBox.Text;
            bool useDatFiles = this.UseDatFilesCheckBox.Checked;
            ExtractScript(scriptFile, path, useDatFiles);
            this.UseWaitCursor = false;
        }

        private void openPathButton_Click(object sender, EventArgs e)
        {
            OpenPath();

        }

        private void OpenPath()
        {
            using (var saveDialog = new SaveFileDialog())
            {
                saveDialog.ValidateNames = false;
                saveDialog.OverwritePrompt = false;
                saveDialog.CheckPathExists = true;
                saveDialog.Filter = "All Files (*.*)|*.*";
                var oldFileName = pathTextBox.Text;
                string oldDirectory = "";
                try
                {
                    oldDirectory = Path.GetDirectoryName(oldFileName + @"\");
                }
                catch (Exception ex)
                {

                }
                saveDialog.FileName = "PICK THIS DIRECTORY";
                if (oldDirectory != "")
                {
                    saveDialog.InitialDirectory = oldDirectory;
                }
                if (saveDialog.ShowDialog() == DialogResult.OK)
                {
                    pathTextBox.Text = Path.GetDirectoryName(saveDialog.FileName);
                }
            }
        }

        private void replaceScriptButton_Click(object sender, EventArgs e)
        {
            ReplaceScript();
        }

        private void ReplaceScript()
        {
            string path = pathTextBox.Text;
            string scriptFile = scriptFileTextBox.Text;
            if (String.IsNullOrEmpty(path))
            {
                MessageBox.Show("Please provide a directory to replace files from.", "Script Replacer", MessageBoxButtons.OK, MessageBoxIcon.Stop);
                return;
            }
            if (String.IsNullOrEmpty(scriptFile))
            {
                MessageBox.Show("Please provide a script file to replace data in.", "Script Replacer", MessageBoxButtons.OK, MessageBoxIcon.Stop);
                return;
            }

            if (!Directory.Exists(path))
            {
                MessageBox.Show("Text directory does not exist!", "Script Replacer", MessageBoxButtons.OK, MessageBoxIcon.Stop);
                return;
            }
            else
            {
                try
                {
                    int fileCount = Directory.GetFiles(path).Length;
                    if (fileCount == 0)
                    {
                        MessageBox.Show("Text directory contains no files!", "Script Replacer", MessageBoxButtons.OK, MessageBoxIcon.Stop);
                        return;
                    }
                }
                catch (Exception ex)
                {
                    MessageBox.Show("Failed to read contents of directory: " + ex.Message, "Script Replacer", MessageBoxButtons.OK, MessageBoxIcon.Stop);
                    return;
                }
            }

            this.UseWaitCursor = true;
            //FileContent.cipher = keyTextBox.Text;
            bool useDatFiles = this.UseDatFilesCheckBox.Checked;
            ReplaceScript(scriptFile, path, useDatFiles);
            this.UseWaitCursor = false;
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            SaveSettings();
        }

        private void SaveSettings()
        {
            RegistryUtility.SaveSetting("ScriptFileName", scriptFileTextBox.Text);
            RegistryUtility.SaveSetting("TextPath", pathTextBox.Text);
            RegistryUtility.SaveSetting("IndexPassword", IndexFilePasswordTextBox.Text);
            RegistryUtility.SaveSetting("DataPassword", DataFilePasswordTextBox.Text);
            RegistryUtility.SaveSetting("UseDatFiles", UseDatFilesCheckBox.Checked);
        }
    }
}
