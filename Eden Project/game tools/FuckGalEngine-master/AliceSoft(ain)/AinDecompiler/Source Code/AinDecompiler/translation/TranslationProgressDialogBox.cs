using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace TranslateParserThingy
{
    public partial class TranslationProgressDialogBox : Form
    {
        public string LabelText
        {
            get
            {
                return this.label.Text;
            }
            set
            {
                this.label.Text = value;
            }
        }

        public int Progress
        {
            get
            {
                return this.progressBar.Value;
            }
            set
            {
                this.progressBar.Value = value;
            }
        }

        public event EventHandler StopButtonClicked;

        public TranslationProgressDialogBox()
        {
            InitializeComponent();
        }

        private void stopButton_Click(object sender, EventArgs e)
        {
            if (StopButtonClicked != null)
            {
                StopButtonClicked(this, EventArgs.Empty);
            }
        }

        private void TranslationProgressDialogBox_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (e.CloseReason == CloseReason.UserClosing)
            {
                e.Cancel = true;
                stopButton_Click(this, EventArgs.Empty);
            }
        }
    }
}
