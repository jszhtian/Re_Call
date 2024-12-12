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
    public partial class ReassembleForm : Form
    {
        public ReassembleForm()
        {
            InitializeComponent();
        }

        private void LoadAINFileButton_Click(object sender, EventArgs e)
        {
            OpenFile();
        }

        private void OpenFile()
        {
            var openFileDialog = new OpenFileDialog();
            openFileDialog.RestoreDirectory = false;
            openFileDialog.FileName = "main.jam";
            openFileDialog.Filter = "JAM Files (*.jam)|*.jam;*.jaf|All Files (*.*)|*.*";
            if (openFileDialog.ShowDialog() == DialogResult.OK)
            {
                var saveFileDialog = new SaveFileDialog();
                saveFileDialog.RestoreDirectory = false;
                saveFileDialog.Filter = "AIN Files (*.ain)|*.ain;*.ain_|All Files (*.*)|*.*";
                if (saveFileDialog.ShowDialog() == DialogResult.OK)
                {
                    Build(openFileDialog.FileName, saveFileDialog.FileName);
                }
            }
        }

        private void Build(string inputProjectFileName, string outputAinFileName)
        {
            bool encrypt = this.EncryptCheckBox.Checked;
            //todo: encrypt it
            AssemblerProjectReader reader = new AssemblerProjectReader();
            reader.LoadProject(inputProjectFileName);
            var ainFile = reader.MakeAinFile();
            if (encrypt)
            {
                ainFile.WriteAndEncryptAinFile(outputAinFileName);
            }
            else
            {
                ainFile.WriteAinFile(outputAinFileName);
            }
        }

    }
}
