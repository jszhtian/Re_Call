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
    public partial class ExportImportStringsForm : Form
    {
        AinFile ainFile;
        StringExportImport exportImport;
        public ExportImportStringsForm(AinFile ainFile)
            : this()
        {
            this.ainFile = ainFile;
            exportImport = new StringExportImport(ainFile);
        }

        private ExportImportStringsForm()
        {
            InitializeComponent();
        }

        private void ExportStringsButton_Click(object sender, EventArgs e)
        {
            ExportStringsOnly();
        }

        private void ExportStringsAndMessages()
        {
            ReadCheckBoxes();

            using (var sfd = new SaveFileDialog())
            {
                sfd.Filter = "Text Files (*.txt)|*.txt|All Files (*.*)|*.*";
                if (sfd.ShowDialog() == DialogResult.OK)
                {
                    string fileName = sfd.FileName;
                    exportImport.ExportTextFile(fileName);

                    //var importExportStrings = new StringExportImportOld(ainFile);
                    //importExportStrings.Export(sfd.FileName);
                }
            }
        }

        private void ReadCheckBoxes()
        {
            exportImport.ApplyFiltersOnImport = this.applyFiltersOnImportCheckBox.Checked;
            exportImport.ExcludeFunctionNames = this.excludeFunctionNamesCheckBox.Checked;
            exportImport.ExcludeSystemFunctions = this.excludeSystemCheckBox.Checked;
            exportImport.ExcludeTestFunctions = this.excludeTestCheckBox.Checked;
            exportImport.ExcludeUnusedStrings = this.excludeUnusedStringsCheckBox.Checked;
            exportImport.KeepExcludedAsComments = this.keepCommentsCheckBox.Checked;
            exportImport.ExcludeAsserts = this.excludeAssertsCheckBox.Checked;
            exportImport.ExcludeEventFlags = this.excludeEventFlagNamesCheckBox.Checked;
        }

        private void ImportStringsAndMessages()
        {
            ReadCheckBoxes();

            using (var ofd = new OpenFileDialog())
            {
                ofd.Filter = "Text Files (*.txt)|*.txt|All Files (*.*)|*.*";
                if (ofd.ShowDialog() == DialogResult.OK)
                {
                    using (var sfd = new SaveFileDialog())
                    {
                        sfd.Filter = "AIN Files (*.ain)|*.ain;*.ain_|All Files (*.*)|*.*";
                        if (sfd.ShowDialog() == DialogResult.OK)
                        {
                            string textFileName = ofd.FileName;
                            string ainFileName = sfd.FileName;
                            exportImport.ImportTextFile(textFileName, ainFileName);

                            //var importExportStrings = new StringExportImportOld(ainFile);
                            //importExportStrings.Import(ofd.FileName, sfd.FileName);
                        }
                    }
                }
            }
        }

        private void ExportImportStringsForm_Load(object sender, EventArgs e)
        {
            int firstMessageIdNumber = exportImport.GetFirstMessageIdNumber();
            int messagesCount = ainFile.Messages.Count;
            messagesLabel.Text = "There are " + messagesCount + " messages, numbered " + firstMessageIdNumber + " through " + (firstMessageIdNumber + messagesCount - 1) + ".";
            int firstStringIdNumber = exportImport.GetFirstStringIdNumber();
            int stringsCount = ainFile.Strings.Count;
            stringsLabel.Text = "There are " + stringsCount + " strings, numbered " + firstStringIdNumber + " through " + (firstStringIdNumber + stringsCount - 1) + ".";
        }

        private void importStringsMessagesButton_Click(object sender, EventArgs e)
        {
            ImportStringsAndMessages();
        }

        private void exportStringsAndMessagesButton_Click(object sender, EventArgs e)
        {
            ExportStringsAndMessages();
        }

        private void exportMessagesButton_Click(object sender, EventArgs e)
        {
            ExportMessagesOnly();
        }

        private void ExportMessagesOnly()
        {
            ReadCheckBoxes();

            using (var sfd = new SaveFileDialog())
            {
                sfd.Filter = "Text Files (*.txt)|*.txt|All Files (*.*)|*.*";
                if (sfd.ShowDialog() == DialogResult.OK)
                {
                    string fileName = sfd.FileName;
                    exportImport.ExportMessagesOnly(fileName);
                }
            }
        }
        private void ExportStringsOnly()
        {
            ReadCheckBoxes();

            using (var sfd = new SaveFileDialog())
            {
                sfd.Filter = "Text Files (*.txt)|*.txt|All Files (*.*)|*.*";
                if (sfd.ShowDialog() == DialogResult.OK)
                {
                    string fileName = sfd.FileName;
                    exportImport.ExportStringsOnly(fileName);
                }
            }
        }

    }
}
