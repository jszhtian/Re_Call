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
    public partial class EditMetadata2 : Form
    {
        AinFile ainFile;
        IVariable variable;
        string[] enumerationTypes;
        InitialValue currentDefaultValue;

        public EditMetadata2()
        {
            InitializeComponent();
        }

        public EditMetadata2(IVariable variable)
            : this()
        {
            this.ainFile = variable.Root;
            this.variable = variable;
        }

        private void EditMetadata2_Load(object sender, EventArgs e)
        {
            LoadEnumerationTypes();
            ReadMetadata();
        }

        private void LoadEnumerationTypes()
        {
            if (ainFile == null)
            {
                return;
            }
            this.enumerationTypes = Enumerable.Repeat("", 1).Concat(ainFile.MetadataFile.EnumerationTypes.Keys.OrderBy(t => t)).ToArray();
            EnumerationTypeComboBox.BeginUpdate();
            string oldText = EnumerationTypeComboBox.Text;
            EnumerationTypeComboBox.DataSource = enumerationTypes;
            EnumerationTypeComboBox.Text = oldText;
            EnumerationTypeComboBox.EndUpdate();
        }

        bool SetMetadata()
        {
            //quit if we're not attached to anything
            if (variable == null || ainFile == null)
            {
                return false;
            }

            bool dirty = false;

            //Alternative name
            string defaultAlternativeName = ainFile.AlternativeNames.GetOrNull(variable.Name);
            Metadata metaData = ainFile.MetadataFile.Metadata.GetOrNew(variable);
            string alternativeName = AlternativeNameTextBox.Text;
            if (alternativeName == defaultAlternativeName || alternativeName == "")
            {
                alternativeName = null;
            }
            if (metaData.ReplacementName != alternativeName)
            {
                dirty = true;
            }
            metaData.ReplacementName = alternativeName;

            //default value
            if (findDefaultValueButton.Enabled == true)
            {
                var defaultValue = metaData.DefaultValue;
                if (defaultValue == null)
                {
                    if (currentDefaultValue != null)
                    {
                        dirty = true;
                    }
                    metaData.DefaultValue = currentDefaultValue;
                }
                else
                {
                    if (!defaultValue.Equals(currentDefaultValue))
                    {
                        dirty = true;
                        metaData.DefaultValue = currentDefaultValue;
                    }
                }
            }

            //enumeration type
            if (variable.DataType.GetTypeOfArrayElement().IsInteger())
            {
                var enumerationType = GetSelectedEnumerationType();
                if (metaData.EnumerationType != enumerationType)
                {
                    dirty = true;
                }
                metaData.EnumerationType = enumerationType;
            }

            string description = DescriptionTextBox.Text;
            if (String.IsNullOrEmpty(description))
            {
                description = null;
            }
            if (metaData.Description != description)
            {
                dirty = true;
            }
            metaData.Description = description;

            //set the member
            if (metaData.DefaultValue == null && metaData.Description == null && metaData.EnumerationType == null && metaData.ReplacementName == null)
            {
                if (ainFile.MetadataFile.Metadata.ContainsKey(variable))
                {
                    ainFile.MetadataFile.Metadata.Remove(variable);
                }
            }
            else
            {
                ainFile.MetadataFile.Metadata.Set(variable, metaData);
            }

            if (dirty)
            {
                ainFile.SaveMetadata();
            }
            return dirty;
        }



        private void ReadMetadata()
        {
            //quit if we're not attached to anything
            if (variable == null || ainFile == null)
            {
                return;
            }

            //display variable name declaration
            this.VariableTextBox.ReadOnly = false;
            this.VariableTextBox.Text = variable.ToString();
            this.VariableTextBox.SelectionStart = 0;
            this.VariableTextBox.SelectionLength = 0;
            this.VariableTextBox.ScrollToCaret();
            this.VariableTextBox.ReadOnly = true;

            //get metadata or use a blank variable
            Metadata metaData;
            if (ainFile.MetadataFile.Metadata.ContainsKey(variable))
            {
                metaData = ainFile.MetadataFile.Metadata[variable];
            }
            else
            {
                metaData = new Metadata();
            }

            //set alternative name - either the default for what is specified
            string defaultAlternativeName = ainFile.AlternativeNames.GetOrNull(variable.Name);
            AlternativeNameTextBox.Text = metaData.ReplacementName ?? defaultAlternativeName;

            //is this a function parameter?  Allow a default value
            if (variable.Parent is Function && variable.Index < variable.Parent.ParameterCount)
            {
                this.currentDefaultValue = metaData.DefaultValue;
            }
            else if (variable is Global)
            {
                //if it's a global, display the initial value specified in the AIN file, and don't allow editing
                this.currentDefaultValue = ainFile.GlobalInitialValues.Where(i => i.GlobalVariable == variable).FirstOrDefault();
            }
            else
            {
                //DefaultValueTextBox.ReadOnly = true;
                findDefaultValueButton.Enabled = false;
            }
            if (this.currentDefaultValue != null)
            {
                string defaultValueString = this.currentDefaultValue.GetValue();
                DefaultValueTextBox.Text = defaultValueString;
                if (variable is Global)
                {
                    //DefaultValueTextBox.ReadOnly = true;
                    findDefaultValueButton.Enabled = false;
                }
            }

            //get enumeration type
            EnumerationType enumerationType = null;
            if (variable.DataType.GetTypeOfArrayElement().IsInteger())
            {
                enumerationType = metaData.EnumerationType;
                if (enumerationType != null)
                {
                    EnumerationTypeComboBox.Text = enumerationType.Name;
                }
            }
            else
            {
                EnumerationTypeComboBox.Enabled = false;
                EditEnumerationTypeButton.Enabled = false;
                spreadEnumerationTypeButton.Enabled = false;
            }

            //get description
            DescriptionTextBox.Text = metaData.Description ?? "";
        }

        private void OkButton_Click(object sender, EventArgs e)
        {
            if (SetMetadata())
            {
                this.DialogResult = DialogResult.OK;
            }
            else
            {
                this.DialogResult = DialogResult.Cancel;
            }
            Close();
        }

        private void EditEnumerationTypeButton_Click(object sender, EventArgs e)
        {
            var enumerationType = GetSelectedEnumerationType();
            if (enumerationType == null)
            {
                enumerationType = new EnumerationType();
            }
            using (var editor = new EnumerationTypeEditor(ainFile, enumerationType.Name))
            {
                editor.ShowDialog();
                this.EnumerationTypeComboBox.Text = editor.EnumerationName;
                LoadEnumerationTypes();
            }
        }

        private EnumerationType GetSelectedEnumerationType()
        {
            string enumerationTypeName = EnumerationTypeComboBox.Text;
            var enumerationType = ainFile.MetadataFile.EnumerationTypes.GetOrNull(enumerationTypeName);
            return enumerationType;
        }

        private void cancelButton_Click(object sender, EventArgs e)
        {

        }

        private void findDefaultValueButton_Click(object sender, EventArgs e)
        {
            FindDefaultValue();
        }

        private void FindDefaultValue()
        {
            var variable = this.variable as Variable;
            if (variable == null)
            {
                return;
            }
            var defaultValueFinder = new DefaultArgumentFinder(this.ainFile);
            var defaultValues = defaultValueFinder.FindArgumentValues(variable);

            using (var findDefaultValueForm = new FindDefaultValueForm(defaultValues))
            {
                var dialogResult = findDefaultValueForm.ShowDialog();
                if (dialogResult == DialogResult.OK)
                {
                    var result = findDefaultValueForm.SelectedEntry;
                    if (result != null && !result.IsNull)
                    {
                        this.currentDefaultValue = result.ToInitialValue();
                        if (this.currentDefaultValue == null)
                        {
                            this.DefaultValueTextBox.Text = "";
                        }
                        else
                        {
                            this.DefaultValueTextBox.Text = currentDefaultValue.GetValueQuoted();
                        }
                    }
                }
                else if (dialogResult == DialogResult.Abort)
                {
                    this.currentDefaultValue = null;
                    this.DefaultValueTextBox.Text = "";
                }
            }
        }

        private void spreadEnumerationTypeButton_Click(object sender, EventArgs e)
        {
            SpreadEnumerationType();
        }

        private void SpreadEnumerationType()
        {
            var enumerationType = GetSelectedEnumerationType();
            if (enumerationType != null && variable != null && variable.DataType.GetTypeOfArrayElement().IsInteger())
            {
                using (var spreadEnumerationTypeForm = new SpreadEnumerationTypeForm(variable, enumerationType))
                {
                    spreadEnumerationTypeForm.ShowDialog();
                }
            }
        }
    }
}
