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
    public partial class ErrorsListForm : Form
    {
        public ErrorsListForm()
        {
            InitializeComponent();
        }

        public void SetErrorMessage(IList<string> errors)
        {
            this.textBox1.Lines = errors.ToArray();
        }
    }
}
