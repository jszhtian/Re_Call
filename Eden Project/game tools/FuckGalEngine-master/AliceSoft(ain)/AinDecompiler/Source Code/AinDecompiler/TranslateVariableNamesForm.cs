using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.IO;
using System.Text;
using System.Windows.Forms;
using TranslateParserThingy;

namespace AinDecompiler
{
    public partial class TranslateVariableNamesForm : Form
    {
        AinFile ainFile = null;
        List<string> variableNames = null;
        List<string> alternativeNames = null;
        HashSet<string> seenNames = null;

        public TranslateVariableNamesForm(AinFile ainFile)
            : this()
        {
            this.ainFile = ainFile;
        }

        private TranslateVariableNamesForm()
        {
            InitializeComponent();
        }

        private void TranslateVariableNamesForm_Load(object sender, EventArgs e)
        {

        }

        bool TryAddName(string name)
        {
            if (TranslateParserThingy.TranslatorExtensions.IndexOfNonAscii(name) == -1)
            {
                return false;
            }

            if (seenNames.Contains(name))
            {
                return false;
            }

            seenNames.Add(name);
            variableNames.Add(name);
            alternativeNames.Add(name);
            return true;
        }

        void AddNamesToBeUppercase()
        {
            foreach (var function in ainFile.Functions)
            {
                TryAddName(function.Name);
            }

            foreach (var global in ainFile.Globals)
            {
                TryAddName(global.Name);
            }

            foreach (var structInfo in ainFile.Structs)
            {
                TryAddName(structInfo.Name);
                foreach (var member in structInfo.Members)
                {
                    TryAddName(member.Name);
                }
            }

            foreach (var hll in ainFile.Libraries)
            {
                TryAddName(hll.LibraryName);
                foreach (var function in hll.Functions)
                {
                    TryAddName(function.Name);
                    foreach (var arg in function.Parameters)
                    {
                        TryAddName(arg.Name);
                    }
                }
            }

            foreach (var globalGroupName in ainFile.GlobalGroupNames)
            {
                TryAddName(globalGroupName);
            }

        }

        void AddNamesToBeLowercase()
        {
            foreach (var function in ainFile.Functions)
            {
                foreach (var variable in function.Parameters)
                {
                    TryAddName(variable.Name);
                }
            }
        }

        private bool TranslateVariableNames()
        {
            variableNames = new List<string>();
            alternativeNames = new List<string>();
            seenNames = new HashSet<string>();

            AddNamesToBeUppercase();

            //this.Text = "Translating " + variableNames.Count + " names...";
            TranslateParserThingy.Translator.TranslationOptions.UseCommentCharacter = false;

            var str1 = variableNames.Join("\r\n");
            var resutltStr1 = TranslateParserThingy.Translator.TranslateText(str1);
            if (resutltStr1 == null || resutltStr1 == str1)
            {
                return false;
            }
            var result1 = resutltStr1.Split("\r\n");

            for (int i = 0; i < result1.Length; i++)
            {
                bool wantUppercase = true;
                char firstInputChar = variableNames[i][0];
                if (firstInputChar >= 'a' && firstInputChar <= 'z')
                {
                    wantUppercase = false;
                }
                alternativeNames[i] = FormatVariableName(result1[i], wantUppercase);
            }

            var varnames1 = variableNames;
            var altnames1 = alternativeNames;

            variableNames = new List<string>();
            alternativeNames = new List<string>();
            AddNamesToBeLowercase();

            //this.Text = "Translating " + variableNames.Count + " names...";

            TranslateParserThingy.Translator.TranslationOptions.UseCommentCharacter = false;
            var str2 = variableNames.Join("\r\n");

            var resultStr2 = Translator.TranslateText(str2);
            if (resultStr2 == null || str2 == resultStr2)
            {
                return false;
            }
            var result2 = resultStr2.Split("\r\n");

            if (result2.Length != variableNames.Count)
            {
                //bad thing
            }

            for (int i = 0; i < result2.Length; i++)
            {
                bool wantUppercase = false;
                char firstInputchar = variableNames[i][0];
                if (firstInputchar >= 'A' && firstInputchar <= 'Z')
                {
                    wantUppercase = true;
                }
                alternativeNames[i] = FormatVariableName(result2[i], wantUppercase);
            }

            var varnames2 = variableNames;
            var altnames2 = alternativeNames;

            variableNames = varnames1;
            alternativeNames = altnames1;

            variableNames.AddRange(varnames2);
            alternativeNames.AddRange(altnames2);

            return true;
        }

        string FormatVariableName(string variableName, bool initialUppercase)
        {
            StringBuilder sb = new StringBuilder();
            bool capitalizeNext = initialUppercase;
            for (int i = 0; i < variableName.Length; i++)
            {
                char c = variableName[i];
                if ((c >= '0' && c <= '9') ||
                    (c >= 'A' && c <= 'Z') ||
                    (c >= 'a' && c <= 'z') ||
                    (c == '_' || c == '@' || c == '#' || c == '`'))
                {
                    if (capitalizeNext)
                    {
                        c = char.ToUpperInvariant(c);
                        capitalizeNext = false;
                    }
                    sb.Append(c);
                }
                else if (c == ' ')
                {
                    capitalizeNext = true;
                }
                else if (c == '/' || c == '%' || c == '*' || c == '+' || c == '-')
                {
                    string jascii = TranslateParserThingy.TextUtility.AsciiToJascii(c.ToString());
                    sb.Append(jascii);
                    capitalizeNext = true;
                }
                else if (c == '．' || c == 'ー')
                {

                }
                else if (c >= 0x80)
                {
                    string romaji = TranslateParserThingy.TextUtility.Romanize(c.ToString());
                    sb.Append(romaji);
                }

            }
            return sb.ToString();
        }

        private void ReadNames()
        {
            variableNames = new List<string>();
            alternativeNames = new List<string>();
            seenNames = new HashSet<string>();

            AddNamesToBeUppercase();
            AddNamesToBeLowercase();
        }

        private void SaveVariableNames()
        {
            string fileName = GetOutputFileName();
            if (fileName != null)
            {
                SaveVariableNames(fileName);
            }
        }

        private string GetOutputFileName()
        {
            string defaultFileName;
            string originalDirectory = Path.GetDirectoryName(ainFile.OriginalFilename);
            string originalFilename = Path.GetFileNameWithoutExtension(ainFile.OriginalFilename);
            defaultFileName = Path.Combine(originalDirectory, originalFilename + "_names.txt");

            var saveFileDialog = new SaveFileDialog();
            saveFileDialog.FileName = defaultFileName;
            saveFileDialog.Filter = "Text Files (*.txt)|*.txt|All Files (*.*)|*.*";
            if (saveFileDialog.ShowDialog() == DialogResult.OK)
            {
                return saveFileDialog.FileName;
            }
            return null;
        }

        private void SaveVariableNames(string fileName)
        {
            var utf8 = new UTF8Encoding(false);
            using (var fs = new FileStream(fileName, FileMode.Create, FileAccess.Write, FileShare.ReadWrite))
            {
                var tw = new StreamWriter(fs, utf8);
                int length = Math.Min(variableNames.Count, alternativeNames.Count);
                for (int i = 0; i < length; i++)
                {
                    string originalName = variableNames[i];
                    string alternativeName = alternativeNames[i];
                    tw.Write(originalName);
                    tw.Write("\t");
                    tw.Write(alternativeName);
                    tw.WriteLine();
                }
                tw.Flush();
            }
        }

        private void exportButton_Click(object sender, EventArgs e)
        {
            if (this.exportNamesRadioButton.Checked == true)
            {
                ReadNames();
                SaveVariableNames();
                this.DialogResult = DialogResult.OK;
                Close();
            }
            else
            {
                ReadNames();
                string fileName = GetOutputFileName();
                if (fileName != null)
                {
                    if (TranslateVariableNames())
                    {
                        SaveVariableNames(fileName);
                        this.DialogResult = DialogResult.OK;
                        Close();
                    }
                    else
                    {
                        this.DialogResult = DialogResult.None;
                    }
                }
                else
                {
                    this.DialogResult = DialogResult.None;
                }
            }
        }

    }
}
