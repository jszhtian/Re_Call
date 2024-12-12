using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.IO;
using System.Text;
using System.Windows.Forms;
using System.Reflection;

namespace AinDecompiler
{
    public partial class WordWrapForm : Form
    {
        WordWrapOptions _wordWrapOptions;
        public WordWrapOptions WordWrapOptions
        {
            get
            {
                return _wordWrapOptions;
            }
            set
            {
                _wordWrapOptions = value;
                bindingSource.DataSource = value;
            }
        }

        public WordWrapForm(WordWrapOptions wordWrapOptions) : this()
        {
            this.WordWrapOptions = wordWrapOptions;
        }

        public WordWrapForm()
        {
            InitializeComponent();
        }

        private void WordWrapForm_Load(object sender, EventArgs e)
        {
            if (!this.bindingSource.IsBindingSuspended)
            {
                var dataSourceType = bindingSource.DataSource.GetType();
                foreach (Control control in this.Controls)
                {
                    var bindings = control.DataBindings;
                    if (bindings.Count > 0)
                    {
                        var binding = bindings[0];
                        string memberName = binding.BindingMemberInfo.BindingMember;
                        var member = dataSourceType.GetMember(memberName, BindingFlags.FlattenHierarchy | BindingFlags.Instance | BindingFlags.Public).FirstOrDefault();
                        if (member != null)
                        {
                            var descriptionAttribute = (DescriptionAttribute)(member.GetCustomAttributes(typeof(DescriptionAttribute), true).FirstOrDefault());
                            if (descriptionAttribute != null)
                            {
                                toolTip1.SetToolTip(control, descriptionAttribute.Description);
                            }
                        }
                    }
                }
            }
        }

        private void toolStripDropDownButton1_DropDownOpening(object sender, EventArgs e)
        {
            var names = RegistryUtility.ListKeys("WordWrapOptions\\").OrderBy(n => n).ToArray();
            var toolStripItem = (ToolStripDropDownButton)sender;
            toolStripItem.DropDownItems.Clear();
            var items = names.Select((n) =>
                {
                    var item = new ToolStripMenuItem(n);
                    item.Tag = n;
                    item.Click += new EventHandler(item_Click);
                    return item;
                }).ToArray();
            toolStripItem.DropDownItems.AddRange(items);
        }

        void item_Click(object sender, EventArgs e)
        {
            var item = (ToolStripItem)sender;
            var otherWordWrapOptions = new WordWrapOptions();
            string path = "WordWrapOptions\\" + item.Tag;
            RegistryUtility.GetObject(path, otherWordWrapOptions);
            this.WordWrapOptions.AssignFrom(otherWordWrapOptions);
            this.bindingSource.ResetBindings(false);
        }

        private void selectFontButton_Click(object sender, EventArgs e)
        {
            using (var selectFontForm = new SelectFontForm())
            {
                selectFontForm.TemplateFontName = this.WordWrapOptions.TemplateFontName;
                selectFontForm.TemplateFontSize = this.WordWrapOptions.TemplateFontSize;
                selectFontForm.TemplateFontBold = this.WordWrapOptions.TemplateFontBold;
                selectFontForm.TemplateKanjiWidth = this.WordWrapOptions.TemplateKanjiWidth;
                if (selectFontForm.ShowDialog() == DialogResult.OK)
                {
                    this.WordWrapOptions.TemplateFontName = selectFontForm.TemplateFontName;
                    this.WordWrapOptions.TemplateFontSize = selectFontForm.TemplateFontSize;
                    this.WordWrapOptions.TemplateFontBold = selectFontForm.TemplateFontBold;
                    this.WordWrapOptions.TemplateKanjiWidth = selectFontForm.TemplateKanjiWidth;
                }
            }
        }
    }
}
