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
    public partial class FindDefaultValueForm : Form
    {
        private IHistogramEntry[] histogramEntries;
        public IHistogramEntry SelectedEntry;

        public FindDefaultValueForm()
        {
            InitializeComponent();
        }

        public FindDefaultValueForm(IHistogramEntry[] entries) : this()
        {
            this.histogramEntries = entries;
        }

        private void FindDefaultValueForm_Load(object sender, EventArgs e)
        {
            if (this.histogramEntries != null)
            {
                foreach (var entry in this.histogramEntries)
                {
                    string text = entry.GetValueString();
                    string countText = entry.Count.ToString();
                    string rateText = (entry.Rate * 100.0).ToString("#.##") + "%";
                    
                    var newItem = new ListViewItem(new string[]{text,countText,rateText});
                    newItem.Tag = entry;
                    this.resultsListView.Items.Add(newItem);
                }
                if (this.resultsListView.Items.Count > 0)
                {
                    this.resultsListView.SelectedIndices.Add(0);
                }
            }
        }

        private void FindDefaultValueForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            if ((e.CloseReason == CloseReason.None || e.CloseReason == CloseReason.UserClosing) && this.DialogResult == DialogResult.OK)
            {
                var selectedItem = this.resultsListView.SelectedItems.OfType<ListViewItem>().FirstOrDefault();
                if (selectedItem != null)
                {
                    this.SelectedEntry = selectedItem.Tag as IHistogramEntry;
                }
                else
                {
                    e.Cancel = true;
                    MessageBox.Show("Select a default value from the list before clicking OK.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Stop);
                }
            }
        }

        private void resultsListView_DoubleClick(object sender, EventArgs e)
        {
            var selectedItem = this.resultsListView.SelectedItems.OfType<ListViewItem>().FirstOrDefault();
            if (selectedItem != null)
            {
                this.DialogResult = DialogResult.OK;
            }
        }
    }
}
