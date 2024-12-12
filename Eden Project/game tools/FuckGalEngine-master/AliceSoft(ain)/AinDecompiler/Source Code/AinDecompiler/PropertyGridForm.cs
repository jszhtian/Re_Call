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
    public partial class PropertyGridForm : Form
    {
        PropertyGridWrapper wrapper = new PropertyGridWrapper();
        public object ObjectToEdit { get; set; }

        public PropertyGridForm()
        {
            InitializeComponent();
        }

        public PropertyGridForm(object myObject, string windowTitle)
            : this()
        {
            this.Text = windowTitle;
            try
            {
                this.ObjectToEdit = myObject;
                this.wrapper.Object = myObject;
                this.propertyGrid1.SelectedObject = this.wrapper;
                this.propertyGrid1.SelectedGridItem.Expanded = true;
            }
            catch (Exception ex)
            {
                this.propertyGrid1.SelectedObject = null;
            }
        }

        private void okButton_Click(object sender, EventArgs e)
        {
            this.ObjectToEdit = wrapper.Object;
            //applyButton_Click(applyButton, EventArgs.Empty);
        }

        private void PropertyGridForm_Load(object sender, EventArgs e)
        {

        }

        private void closeButton_Click(object sender, EventArgs e)
        {
            Close();
        }
    }
}
