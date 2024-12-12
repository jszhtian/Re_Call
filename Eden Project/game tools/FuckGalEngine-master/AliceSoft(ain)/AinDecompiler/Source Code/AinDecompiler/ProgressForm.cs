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
    public partial class ProgressForm : Form
    {
        public ProgressForm()
        {
            InitializeComponent();
        }

        private void cancelButton_Click(object sender, EventArgs e)
        {
            if (OkayToClose)
            {
                this.Close();
            }
            this.DialogResult = DialogResult.None;
            if (CancelButtonPressed != null)
            {
                CancelButtonPressed(this, e);
            }
        }

        private void ProgressForm_Load(object sender, EventArgs e)
        {

        }

        public event EventHandler CancelButtonPressed;
        public string LabelText
        {
            get
            {
                return label.Text;
            }
            set
            {
                label.Text = value;
            }
        }

        public void SetProgress(int progress)
        {
            if (progress < 0)
            {
                progress = 0;
            }
            if (progress > 100)
            {
                progress = 100;
            }
            this.progressBar.Value = progress;
        }

        public bool OkayToClose = false;

        private void ProgressForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (OkayToClose)
            {
                e.Cancel = false;
                return;
            }

            if (e.CloseReason == CloseReason.UserClosing)
            {
                e.Cancel = true;
            }
        }
    }
}
