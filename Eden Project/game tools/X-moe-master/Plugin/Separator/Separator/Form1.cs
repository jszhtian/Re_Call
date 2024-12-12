using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Management;
using System.IO;

namespace Separator
{
    public partial class Form1 : Form
    {
        bool SelectScript = false;
        bool SelectFile  = true;
        bool SelectFolder = false;
        bool SelectPluginFlag = false;
        bool SelectSPArgsFlag = false;
        string FileName = "";
        string FolderName = "";
        string ScriptName = "";
        string SPArgs = "";
        string PluginName = "";
        uint CPULogicCores = 1;
        public Form1()
        {
            InitializeComponent();
            checkBox1.Checked = true;
            checkBox2.Checked = false;
            checkBox3.Checked = false;
            checkBox4.Checked = false;
            checkBox5.Checked = false;
            this.button2.Enabled = true;
            this.button3.Enabled = false;
            this.button4.Enabled = false;
            this.textBox4.Enabled = false;
            this.textBox5.Enabled = false;

            //Get CPU Cores
            ManagementClass WMIClass = new ManagementClass(new ManagementPath("Win32_Processor"));
            ManagementObjectCollection moc = WMIClass.GetInstances();
            foreach(ManagementObject mo in moc)
            {
                PropertyDataCollection po = mo.Properties;
                CPULogicCores = (uint)po["NumberOfLogicalProcessors"].Value;
                break;
            }
            for(uint i = 1; i <= CPULogicCores; i++)
            {
                if (i == 1)
                {
                    comboBox1.Items.Add(i.ToString() + " Core");
                }
                else
                {
                    comboBox1.Items.Add(i.ToString() + " Cores");
                }
            }
            comboBox1.SelectedIndex = 0;
            if(string.IsNullOrEmpty(this.FileName) &&
               string.IsNullOrEmpty(this.FolderName) &&
               string.IsNullOrEmpty(this.ScriptName))
            {
                this.button1.Enabled = false;
            }
            else
            {
                this.button1.Enabled = true;
            }
        }

        //Select a File Box
        private void checkBox1_CheckedChanged(object sender, EventArgs e)
        {
            if (checkBox1.Checked == true)
            {
                this.SelectFile = true;
                this.SelectFolder = false;
                this.SelectScript = false;
                this.checkBox1.Checked = true;
                this.checkBox2.Checked = false;
                this.checkBox3.Checked = false;
                this.textBox1.Enabled = true;
                this.textBox2.Enabled = false;
                this.textBox3.Enabled = false;
                this.button2.Enabled = true;
                this.button3.Enabled = false;
                this.button4.Enabled = false;
            }
            else
            {
                this.SelectFile = false;
                this.textBox1.Enabled = false;
                this.button2.Enabled = false;

                this.button1.Enabled = false;
            }

        }

        //Select a Folder Box
        private void checkBox2_CheckedChanged(object sender, EventArgs e)
        {
            if (checkBox2.Checked == true)
            {
                this.SelectFile = false;
                this.SelectFolder = true;
                this.SelectScript = false;
                this.checkBox1.Checked = false;
                this.checkBox2.Checked = true;
                this.checkBox3.Checked = false;
                this.textBox1.Enabled = false;
                this.textBox2.Enabled = true;
                this.textBox3.Enabled = false;
                this.button2.Enabled = false;
                this.button3.Enabled = true;
                this.button4.Enabled = false;
            }
            else
            {
                this.SelectFolder = false;
                this.textBox2.Enabled = false;
                this.button3.Enabled = false;

                this.button1.Enabled = false;
            }
        }
        
        //Select a Script Box
        private void checkBox3_CheckedChanged(object sender, EventArgs e)
        {
            if (checkBox3.Checked == true)
            {
                this.SelectFile = false;
                this.SelectFolder = false;
                this.SelectScript = true;
                this.checkBox1.Checked = false;
                this.checkBox2.Checked = false;
                this.checkBox3.Checked = true;
                this.textBox1.Enabled = false;
                this.textBox2.Enabled = false;
                this.textBox3.Enabled = true;
                this.button2.Enabled = false;
                this.button3.Enabled = false;
                this.button4.Enabled = true;
            }
            else
            {
                this.SelectScript = false;
                this.textBox3.Enabled = false;
                this.button4.Enabled = false;

                this.button1.Enabled = false;
            }
        }

        //Select a Text Edit
        private void textBox1_TextChanged(object sender, EventArgs e)
        {
            this.FileName = textBox1.Text;
            if(!string.IsNullOrEmpty(this.FileName))
            {
                this.button1.Enabled = true;
            }
        }

        //Select a Folder Button
        private void button3_Click(object sender, EventArgs e)
        {
            FolderBrowserDialog dlg = new FolderBrowserDialog();
            dlg.Description = "Select a Path";
            if(dlg.ShowDialog() == DialogResult.OK)
            {
                this.FolderName = dlg.SelectedPath;
                this.textBox2.Text = dlg.SelectedPath;
            }
            else
            {
                this.FolderName = "";
                this.textBox2.Text = "";
            }
        }

        //Select a Folder Edit
        private void textBox2_TextChanged(object sender, EventArgs e)
        {
            this.FolderName = textBox2.Text;
            if (!string.IsNullOrEmpty(this.FolderName))
            {
                this.button1.Enabled = true;
            }
        }

        //Select a Script Edit
        private void textBox3_TextChanged(object sender, EventArgs e)
        {
            this.ScriptName = textBox3.Text;
            if (!string.IsNullOrEmpty(this.ScriptName))
            {
                this.button1.Enabled = true;
            }
        }

        //Select a Text Button
        private void button2_Click(object sender, EventArgs e)
        {
            OpenFolderDialog openDlg = new OpenFolderDialog();
            if (openDlg.ShowDialog(@"All File(*.*)|*.*", "Select a File") == DialogResult.OK)
            {
                this.FileName = openDlg.Path;
                this.textBox1.Text = openDlg.Path;
            }
            else
            {
                this.FileName = "";
                this.textBox1.Text = "";
            }
        }

        //Select a Script Button
        private void button4_Click(object sender, EventArgs e)
        {
            OpenFolderDialog openDlg = new OpenFolderDialog();
            if (openDlg.ShowDialog(@"X'moe Script(*.axs)|*.axs", "Select a Script") == DialogResult.OK)
            {
                this.ScriptName = openDlg.Path;
                this.textBox3.Text = openDlg.Path;
            }
            else
            {
                this.ScriptName = "";
                this.textBox3.Text = "";
            }
        }

        //Select Plugin Click
        private void checkBox4_CheckedChanged(object sender, EventArgs e)
        {
            if(checkBox4.Checked)
            {
                this.textBox4.Enabled = true;
                this.SelectPluginFlag = true;
            }
            else
            {
                this.textBox4.Enabled = false;
                this.SelectPluginFlag = false;
            }
        }

        //Special Args Click
        private void checkBox5_CheckedChanged(object sender, EventArgs e)
        {
            if(this.checkBox5.Checked)
            {
                this.textBox5.Enabled = true;
                this.SelectSPArgsFlag = true;
            }
            else
            {
                this.textBox5.Enabled = false;
                this.SelectSPArgsFlag = false;
            }
        }

        //Select Plugin Edit
        private void textBox4_TextChanged(object sender, EventArgs e)
        {
            this.PluginName = this.textBox4.Text;
        }

        //Select Args Edit
        private void textBox5_TextChanged(object sender, EventArgs e)
        {
            this.SPArgs = this.textBox5.Text;
        }

        //Start Button
        //Check all Infomation..
        private void button1_Click(object sender, EventArgs e)
        {
            if(!File.Exists("SeparatorCore.dll"))
            {
                MessageBox.Show("Cannot find SeparatorCore.dll in current path!");
                return;
            }
            if(this.SelectFile && !string.IsNullOrEmpty(this.FileName))
            {
                StringBuilder sFileName = new StringBuilder(this.FileName);
                StringBuilder sArgs = new StringBuilder(this.SPArgs);
                StringBuilder sPlugin = new StringBuilder(this.PluginName);

                //this.Hide();
                //Wait
                this.Enabled = false;
                APIWarpper.SeparatorEntry(TaskInfo.FileTask, sFileName, sPlugin, sArgs, this.CPULogicCores);
                this.Enabled = true;
                //this.Show();
            }
            else if(this.SelectFolder && !string.IsNullOrEmpty(this.FolderName))
            {
                StringBuilder sFolderName = new StringBuilder(this.FolderName);
                StringBuilder sArgs = new StringBuilder(this.SPArgs);
                StringBuilder sPlugin = new StringBuilder(this.PluginName);

                APIWarpper.SeparatorEntry(TaskInfo.FolderTask, sFolderName, sPlugin, sArgs, this.CPULogicCores);
            }
            else if(this.SelectScript && !string.IsNullOrEmpty(this.ScriptName))
            {
                StringBuilder sScriptName = new StringBuilder(this.ScriptName);
                StringBuilder sArgs = new StringBuilder(this.SPArgs);
                StringBuilder sPlugin = new StringBuilder(this.PluginName);

                APIWarpper.SeparatorEntry(TaskInfo.ScriptTask, sScriptName, sPlugin, sArgs, this.CPULogicCores);
            }
            else
            {
                MessageBox.Show("Cannot launch task!");
                return;
            }
        }

        //CPU Cores, For MultiTask
        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            CPULogicCores = (uint)this.comboBox1.SelectedIndex + 1;
        }

        private void Form1_DragOver(object sender, DragEventArgs e)
        {
            
        }

        private void Form1_DragDrop(object sender, DragEventArgs e)
        {

        }

        private void textBox1_DragDrop(object sender, DragEventArgs e)
        {
            if (((System.Array)e.Data.GetData(DataFormats.FileDrop)).GetLength(0) != 1)
            {
                MessageBox.Show("You must drop only one file at the same time!");
                return;
            }
            string Path = ((System.Array)e.Data.GetData(DataFormats.FileDrop)).GetValue(0).ToString();
            this.FileName = Path;
            this.textBox1.Text = Path;
        }

        private void textBox1_DragEnter(object sender, DragEventArgs e)
        {
            if(e.Data.GetDataPresent(DataFormats.FileDrop, false) == true)
            {
                e.Effect = DragDropEffects.All;
            }
        }

        private void textBox3_KeyDown(object sender, KeyEventArgs e)
        {

        }

        private void textBox3_DragDrop(object sender, DragEventArgs e)
        {
            if (((System.Array)e.Data.GetData(DataFormats.FileDrop)).GetLength(0) != 1)
            {
                MessageBox.Show("You must drop only one script at the same time!");
                return;
            }
            string Path = ((System.Array)e.Data.GetData(DataFormats.FileDrop)).GetValue(0).ToString();
            this.ScriptName = Path;
            this.textBox3.Text = Path;
        }

        private void textBox3_DragEnter(object sender, DragEventArgs e)
        {
            if (e.Data.GetDataPresent(DataFormats.FileDrop, false) == true)
            {
                e.Effect = DragDropEffects.All;
            }
        }

        private void textBox2_DragDrop(object sender, DragEventArgs e)
        {
            if (((System.Array)e.Data.GetData(DataFormats.FileDrop)).GetLength(0) != 1)
            {
                MessageBox.Show("You must drop only one folder at the same time!");
                return;
            }
            string Path = ((System.Array)e.Data.GetData(DataFormats.FileDrop)).GetValue(0).ToString();
            this.FolderName = Path;
            this.textBox2.Text = Path;
        }

        private void textBox2_DragEnter(object sender, DragEventArgs e)
        {
            if (e.Data.GetDataPresent(DataFormats.FileDrop, false) == true)
            {
                if (Directory.Exists(((System.Array)e.Data.GetData(DataFormats.FileDrop)).GetValue(0).ToString()))
                {
                    e.Effect = DragDropEffects.All;
                }
            }
        }
    }
}
