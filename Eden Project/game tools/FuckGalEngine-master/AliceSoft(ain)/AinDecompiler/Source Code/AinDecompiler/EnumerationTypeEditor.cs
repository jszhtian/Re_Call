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
    public partial class EnumerationTypeEditor : Form
    {
        public string EnumerationName;
        AinFile ainFile;

        public EnumerationTypeEditor()
        {
            InitializeComponent();
        }

        public EnumerationTypeEditor(AinFile ainFile, string enumerationName)
            : this()
        {
            this.ainFile = ainFile;
            this.EnumerationName = enumerationName;
        }

        private void EnumerationTypeEditor_Load(object sender, EventArgs e)
        {
            scintilla1.NativeInterface.SetTabWidth(8);
            scintilla1.ConfigurationManager.Language = "cs";
            scintilla1.ConfigurationManager.Configure();
            LoadEnumeration();
        }

        void LoadEnumeration()
        {
            if (!String.IsNullOrEmpty(EnumerationName) && ainFile != null)
            {
                var types = ainFile.MetadataFile.EnumerationTypes;
                var enumeration = types.GetOrNull(EnumerationName);
                if (enumeration != null)
                {
                    StringBuilder sb = new StringBuilder();
                    foreach (var pair in enumeration.OrderBy(e => e.Key))
                    {
                        sb.AppendLine(pair.Key.ToString() + "\t" + pair.Value);
                    }
                    scintilla1.Text = sb.ToString();
                    EnumerationNameTextBox.Text = EnumerationName;
                }
            }
        }

        bool SaveEnumeration()
        {
            string newName = RemoveIllegalCharacters(EnumerationNameTextBox.Text);
            var types = ainFile.MetadataFile.EnumerationTypes;
            EnumerationType enumerationType;
            if (EnumerationName != newName)
            {
                if (types.ContainsKey(newName))
                {
                    var result = MessageBox.Show("Overwrite existing enumeration type, \"" + newName + "\"", "Confirmation", MessageBoxButtons.YesNo, MessageBoxIcon.Question, MessageBoxDefaultButton.Button2);
                    if (result == DialogResult.No)
                    {
                        return false;
                    }
                }
                if (String.IsNullOrEmpty(EnumerationName))
                {
                    enumerationType = types.GetOrAddNew(newName);
                    enumerationType.Name = newName;
                    enumerationType.Clear();
                }
                else
                {
                    enumerationType = types.GetOrAddNew(EnumerationName);
                    types.Remove(EnumerationName);
                    enumerationType.Name = newName;
                    types.Set(newName, enumerationType);
                    enumerationType.Clear();
                }
            }
            else
            {
                enumerationType = types.GetOrAddNew(newName);
                enumerationType.Name = newName;
                enumerationType.Clear();
            }
            this.EnumerationName = newName;

            var lines = scintilla1.Lines.OfType<ScintillaNET.Line>().Select(l => l.Text).ToArray();
            foreach (var line in lines)
            {
                var fields = line.Split('\t');
                if (fields.Length >= 2)
                {
                    string number = fields[0];
                    string name = fields[1];
                    name = RemoveIllegalCharacters(name);
                    int value;
                    if (int.TryParse(number, out value) && name != "")
                    {
                        enumerationType.Set(value, name);
                    }
                }
            }

            if (!ainFile.SaveMetadata())
            {
                MessageBox.Show("Unable to write metadata file!", "Error", MessageBoxButtons.OK, MessageBoxIcon.Stop);
                return false;
            }
            return true;
        }

        private string RemoveIllegalCharacters(string name)
        {
            StringBuilder sb = new StringBuilder();
            foreach (var c in name)
            {
                if (c >= '0' && c <= '9')
                {
                    if (sb.Length == 0)
                    {
                        sb.Append('_');
                    }
                    sb.Append(c);
                }
                else if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_')
                {
                    sb.Append(c);
                }
                else if (c >= 0x80 && !char.IsWhiteSpace(c))
                {
                    sb.Append(c);
                }
            }
            return sb.ToString();
        }

        private void EnumerationTypeEditor_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (e.CloseReason == CloseReason.UserClosing || e.CloseReason == CloseReason.None)
            {
                if (this.DialogResult == DialogResult.OK)
                {
                    if (SaveEnumeration())
                    {

                    }
                    else
                    {
                        e.Cancel = true;
                    }
                }
            }
        }

        private void applyButton_Click(object sender, EventArgs e)
        {
            SaveEnumeration();
        }
    }
}
