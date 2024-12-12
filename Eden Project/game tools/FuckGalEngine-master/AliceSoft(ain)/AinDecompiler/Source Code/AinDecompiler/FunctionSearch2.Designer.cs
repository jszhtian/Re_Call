namespace AinDecompiler
{
    partial class FunctionSearchForm2
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.ResultsTreeView = new System.Windows.Forms.TreeView();
            this.contextMenuStrip1 = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.copyNameToClipboardToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.showInExplorerToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.showInExplorerInNewTabToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.SearchQueryTextBox = new System.Windows.Forms.TextBox();
            this.panel1 = new System.Windows.Forms.Panel();
            this.searchKindComboBox = new System.Windows.Forms.ComboBox();
            this.SearchButton = new System.Windows.Forms.Button();
            this.contextMenuStrip1.SuspendLayout();
            this.panel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // ResultsTreeView
            // 
            this.ResultsTreeView.ContextMenuStrip = this.contextMenuStrip1;
            this.ResultsTreeView.Dock = System.Windows.Forms.DockStyle.Fill;
            this.ResultsTreeView.Location = new System.Drawing.Point(0, 23);
            this.ResultsTreeView.Name = "ResultsTreeView";
            this.ResultsTreeView.Size = new System.Drawing.Size(501, 166);
            this.ResultsTreeView.TabIndex = 1;
            this.ResultsTreeView.BeforeExpand += new System.Windows.Forms.TreeViewCancelEventHandler(this.ResultsTreeView_BeforeExpand);
            this.ResultsTreeView.MouseDown += new System.Windows.Forms.MouseEventHandler(this.ResultsTreeView_MouseDown);
            this.ResultsTreeView.KeyDown += new System.Windows.Forms.KeyEventHandler(this.ResultsTreeView_KeyDown);
            // 
            // contextMenuStrip1
            // 
            this.contextMenuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.copyNameToClipboardToolStripMenuItem,
            this.showInExplorerToolStripMenuItem,
            this.showInExplorerInNewTabToolStripMenuItem});
            this.contextMenuStrip1.Name = "contextMenuStrip1";
            this.contextMenuStrip1.Size = new System.Drawing.Size(211, 70);
            this.contextMenuStrip1.Opening += new System.ComponentModel.CancelEventHandler(this.contextMenuStrip1_Opening);
            // 
            // copyNameToClipboardToolStripMenuItem
            // 
            this.copyNameToClipboardToolStripMenuItem.Name = "copyNameToClipboardToolStripMenuItem";
            this.copyNameToClipboardToolStripMenuItem.Size = new System.Drawing.Size(210, 22);
            this.copyNameToClipboardToolStripMenuItem.Text = "&Copy Name to clipboard";
            this.copyNameToClipboardToolStripMenuItem.Click += new System.EventHandler(this.copyNameToClipboardToolStripMenuItem_Click);
            // 
            // showInExplorerToolStripMenuItem
            // 
            this.showInExplorerToolStripMenuItem.Name = "showInExplorerToolStripMenuItem";
            this.showInExplorerToolStripMenuItem.Size = new System.Drawing.Size(210, 22);
            this.showInExplorerToolStripMenuItem.Text = "Show in &Explorer";
            this.showInExplorerToolStripMenuItem.Click += new System.EventHandler(this.showInExplorerToolStripMenuItem_Click);
            // 
            // showInExplorerInNewTabToolStripMenuItem
            // 
            this.showInExplorerInNewTabToolStripMenuItem.Name = "showInExplorerInNewTabToolStripMenuItem";
            this.showInExplorerInNewTabToolStripMenuItem.Size = new System.Drawing.Size(210, 22);
            this.showInExplorerInNewTabToolStripMenuItem.Text = "Show in Explorer in New &Tab";
            this.showInExplorerInNewTabToolStripMenuItem.Click += new System.EventHandler(this.showInExplorerInNewTabToolStripMenuItem_Click);
            // 
            // SearchQueryTextBox
            // 
            this.SearchQueryTextBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.SearchQueryTextBox.Location = new System.Drawing.Point(0, 0);
            this.SearchQueryTextBox.Name = "SearchQueryTextBox";
            this.SearchQueryTextBox.Size = new System.Drawing.Size(341, 20);
            this.SearchQueryTextBox.TabIndex = 0;
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.SearchQueryTextBox);
            this.panel1.Controls.Add(this.searchKindComboBox);
            this.panel1.Controls.Add(this.SearchButton);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Top;
            this.panel1.Location = new System.Drawing.Point(0, 0);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(501, 23);
            this.panel1.TabIndex = 0;
            // 
            // searchKindComboBox
            // 
            this.searchKindComboBox.Dock = System.Windows.Forms.DockStyle.Right;
            this.searchKindComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.searchKindComboBox.FormattingEnabled = true;
            this.searchKindComboBox.Location = new System.Drawing.Point(341, 0);
            this.searchKindComboBox.Name = "searchKindComboBox";
            this.searchKindComboBox.Size = new System.Drawing.Size(85, 21);
            this.searchKindComboBox.TabIndex = 2;
            this.searchKindComboBox.SelectedValueChanged += new System.EventHandler(this.searchKindComboBox_SelectedValueChanged);
            // 
            // SearchButton
            // 
            this.SearchButton.Dock = System.Windows.Forms.DockStyle.Right;
            this.SearchButton.Location = new System.Drawing.Point(426, 0);
            this.SearchButton.Name = "SearchButton";
            this.SearchButton.Size = new System.Drawing.Size(75, 23);
            this.SearchButton.TabIndex = 1;
            this.SearchButton.Text = "Search";
            this.SearchButton.UseVisualStyleBackColor = true;
            this.SearchButton.Click += new System.EventHandler(this.SearchButton_Click);
            // 
            // FunctionSearchForm2
            // 
            this.AcceptButton = this.SearchButton;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(501, 189);
            this.Controls.Add(this.ResultsTreeView);
            this.Controls.Add(this.panel1);
            this.Name = "FunctionSearchForm2";
            this.Text = "Function Search";
            this.Load += new System.EventHandler(this.FunctionSearch_Load);
            this.contextMenuStrip1.ResumeLayout(false);
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TreeView ResultsTreeView;
        private System.Windows.Forms.TextBox SearchQueryTextBox;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Button SearchButton;
        private System.Windows.Forms.ContextMenuStrip contextMenuStrip1;
        private System.Windows.Forms.ToolStripMenuItem copyNameToClipboardToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem showInExplorerToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem showInExplorerInNewTabToolStripMenuItem;
        private System.Windows.Forms.ComboBox searchKindComboBox;
    }
}