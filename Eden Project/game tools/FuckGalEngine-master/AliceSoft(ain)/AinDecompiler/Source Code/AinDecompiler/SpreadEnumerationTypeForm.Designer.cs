namespace AinDecompiler
{
    partial class SpreadEnumerationTypeForm
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
            this.panel1 = new System.Windows.Forms.Panel();
            this.closeButton = new System.Windows.Forms.Button();
            this.spreadButton = new System.Windows.Forms.Button();
            this.resultsTreeView = new System.Windows.Forms.TreeView();
            this.panel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.spreadButton);
            this.panel1.Controls.Add(this.closeButton);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.panel1.Location = new System.Drawing.Point(0, 224);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(292, 42);
            this.panel1.TabIndex = 0;
            // 
            // closeButton
            // 
            this.closeButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.closeButton.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.closeButton.Location = new System.Drawing.Point(205, 7);
            this.closeButton.Name = "closeButton";
            this.closeButton.Size = new System.Drawing.Size(75, 23);
            this.closeButton.TabIndex = 1;
            this.closeButton.Text = "Close";
            this.closeButton.UseVisualStyleBackColor = true;
            // 
            // spreadButton
            // 
            this.spreadButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.spreadButton.Location = new System.Drawing.Point(54, 7);
            this.spreadButton.Name = "spreadButton";
            this.spreadButton.Size = new System.Drawing.Size(145, 23);
            this.spreadButton.TabIndex = 2;
            this.spreadButton.Text = "Spread to checked items";
            this.spreadButton.UseVisualStyleBackColor = true;
            this.spreadButton.Click += new System.EventHandler(this.spreadButton_Click);
            // 
            // resultsTreeView
            // 
            this.resultsTreeView.CheckBoxes = true;
            this.resultsTreeView.Dock = System.Windows.Forms.DockStyle.Fill;
            this.resultsTreeView.Location = new System.Drawing.Point(0, 0);
            this.resultsTreeView.Name = "resultsTreeView";
            this.resultsTreeView.Size = new System.Drawing.Size(292, 224);
            this.resultsTreeView.TabIndex = 1;
            this.resultsTreeView.BeforeExpand += new System.Windows.Forms.TreeViewCancelEventHandler(this.resultsTreeView_BeforeExpand);
            // 
            // SpreadEnumerationTypeForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(292, 266);
            this.Controls.Add(this.resultsTreeView);
            this.Controls.Add(this.panel1);
            this.Name = "SpreadEnumerationTypeForm";
            this.Text = "Spread Enumeration Type";
            this.Load += new System.EventHandler(this.SpreadEnumerationTypeForm_Load);
            this.panel1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Button spreadButton;
        private System.Windows.Forms.Button closeButton;
        private System.Windows.Forms.TreeView resultsTreeView;
    }
}