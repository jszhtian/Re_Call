namespace AinDecompiler
{
    partial class ExplorerForm
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(ExplorerForm));
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.TreeView = new System.Windows.Forms.TreeView();
            this.contextMenuStripForTreeview = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.copyNameToClipboardToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.getCallsAndCalledByInformationToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.SelectionTabControl = new System.Windows.Forms.TabControl();
            this.DecompiledCodeTabPage = new System.Windows.Forms.TabPage();
            this.DecompiledCodeTextBox = new ScintillaNET.Scintilla();
            this.contextMenuStripForTextEditor = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.DisassembledTabPage = new System.Windows.Forms.TabPage();
            this.DisassembledCodeTextBox = new ScintillaNET.Scintilla();
            this.CombinedViewTabPage = new System.Windows.Forms.TabPage();
            this.CombinedViewTextBox = new ScintillaNET.Scintilla();
            this.ExplorerTabControl = new System.Windows.Forms.TabControl();
            this.contextMenuStripForTab = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.closeTabToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.newTabToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.backButton = new System.Windows.Forms.ToolStripButton();
            this.forwardButton = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparatorX = new System.Windows.Forms.ToolStripSeparator();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.openToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.recentFilesToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem2 = new System.Windows.Forms.ToolStripSeparator();
            this.openProjectToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveAsProjectToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem3 = new System.Windows.Forms.ToolStripSeparator();
            this.exportToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
            this.exitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.editToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.copyToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator4 = new System.Windows.Forms.ToolStripSeparator();
            this.selectAllToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem5 = new System.Windows.Forms.ToolStripSeparator();
            this.findTextToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.showFindToolbarToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.goToLineToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.goToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.backToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.forwardToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.refreshToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem4 = new System.Windows.Forms.ToolStripSeparator();
            this.newTabToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.closeTabToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem1 = new System.Windows.Forms.ToolStripSeparator();
            this.historyToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem6 = new System.Windows.Forms.ToolStripSeparator();
            this.decompiledToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.disassembledToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.combinedToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem7 = new System.Windows.Forms.ToolStripSeparator();
            this.nextTabToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.previousTabToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.findToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.findFunctionsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.findGlobalsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.findClassStructMembersToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.findStringsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.findMessagesToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.findIntsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.findLocalVariablesToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.findLibraryCallsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.findSystemCallsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.translateVariableNamesToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.replaceAllNamesToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.viewEditAINStructuresToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem8 = new System.Windows.Forms.ToolStripSeparator();
            this.exportImportTextnewToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.exportImportStringsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.wordwrapAndSaveAINToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.debugCommandToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.optionsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.displaythisAroundLocalMembersToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.showAllCastingToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.removeExtraReturnsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.declareVariablesAtBeginningOfFunctionToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.verboseToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.showGlobalVariableprefixesToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem9 = new System.Windows.Forms.ToolStripSeparator();
            this.showAlternativetranslatedVariableNamesToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.helpToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.aboutToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.debugToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.testCodegeneratorToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.testCodeGeneratorallFunctionsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.testCodeGeneratorTobuildNewAINToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.testCompilerToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.testtracerToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.testdefaultArgumentFinderToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.testSpeedOfStringExportToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.makeSengokuRanceEnumToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.useModifiedShiftJISEncodingForEuropeanTextToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem11 = new System.Windows.Forms.ToolStripSeparator();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            this.contextMenuStripForTreeview.SuspendLayout();
            this.SelectionTabControl.SuspendLayout();
            this.DecompiledCodeTabPage.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.DecompiledCodeTextBox)).BeginInit();
            this.DisassembledTabPage.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.DisassembledCodeTextBox)).BeginInit();
            this.CombinedViewTabPage.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.CombinedViewTextBox)).BeginInit();
            this.contextMenuStripForTab.SuspendLayout();
            this.menuStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // splitContainer1
            // 
            this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer1.Location = new System.Drawing.Point(0, 27);
            this.splitContainer1.Name = "splitContainer1";
            // 
            // splitContainer1.Panel1
            // 
            this.splitContainer1.Panel1.Controls.Add(this.TreeView);
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.Controls.Add(this.SelectionTabControl);
            this.splitContainer1.Panel2.Controls.Add(this.ExplorerTabControl);
            this.splitContainer1.Size = new System.Drawing.Size(965, 612);
            this.splitContainer1.SplitterDistance = 240;
            this.splitContainer1.TabIndex = 0;
            // 
            // TreeView
            // 
            this.TreeView.ContextMenuStrip = this.contextMenuStripForTreeview;
            this.TreeView.Dock = System.Windows.Forms.DockStyle.Fill;
            this.TreeView.HideSelection = false;
            this.TreeView.Location = new System.Drawing.Point(0, 0);
            this.TreeView.Name = "TreeView";
            this.TreeView.Size = new System.Drawing.Size(240, 612);
            this.TreeView.TabIndex = 0;
            this.TreeView.AfterSelect += new System.Windows.Forms.TreeViewEventHandler(this.treeView1_AfterSelect);
            this.TreeView.MouseDown += new System.Windows.Forms.MouseEventHandler(this.TreeView_MouseDown);
            // 
            // contextMenuStripForTreeview
            // 
            this.contextMenuStripForTreeview.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.copyNameToClipboardToolStripMenuItem,
            this.getCallsAndCalledByInformationToolStripMenuItem});
            this.contextMenuStripForTreeview.Name = "contextMenuStrip1";
            this.contextMenuStripForTreeview.Size = new System.Drawing.Size(301, 48);
            this.contextMenuStripForTreeview.Opening += new System.ComponentModel.CancelEventHandler(this.contextMenuStrip1_Opening);
            // 
            // copyNameToClipboardToolStripMenuItem
            // 
            this.copyNameToClipboardToolStripMenuItem.Name = "copyNameToClipboardToolStripMenuItem";
            this.copyNameToClipboardToolStripMenuItem.Size = new System.Drawing.Size(300, 22);
            this.copyNameToClipboardToolStripMenuItem.Text = "&Copy Name to clipboard";
            this.copyNameToClipboardToolStripMenuItem.Click += new System.EventHandler(this.copyNameToClipboardToolStripMenuItem_Click);
            // 
            // getCallsAndCalledByInformationToolStripMenuItem
            // 
            this.getCallsAndCalledByInformationToolStripMenuItem.Name = "getCallsAndCalledByInformationToolStripMenuItem";
            this.getCallsAndCalledByInformationToolStripMenuItem.Size = new System.Drawing.Size(300, 22);
            this.getCallsAndCalledByInformationToolStripMenuItem.Text = "Get \'Calls Functions\' and \'Called By\' &Information";
            this.getCallsAndCalledByInformationToolStripMenuItem.Click += new System.EventHandler(this.getCallsAndCalledByInformationToolStripMenuItem_Click);
            // 
            // SelectionTabControl
            // 
            this.SelectionTabControl.Controls.Add(this.DecompiledCodeTabPage);
            this.SelectionTabControl.Controls.Add(this.DisassembledTabPage);
            this.SelectionTabControl.Controls.Add(this.CombinedViewTabPage);
            this.SelectionTabControl.Dock = System.Windows.Forms.DockStyle.Fill;
            this.SelectionTabControl.Location = new System.Drawing.Point(0, 20);
            this.SelectionTabControl.Name = "SelectionTabControl";
            this.SelectionTabControl.SelectedIndex = 0;
            this.SelectionTabControl.Size = new System.Drawing.Size(721, 592);
            this.SelectionTabControl.TabIndex = 0;
            // 
            // DecompiledCodeTabPage
            // 
            this.DecompiledCodeTabPage.Controls.Add(this.DecompiledCodeTextBox);
            this.DecompiledCodeTabPage.Location = new System.Drawing.Point(4, 22);
            this.DecompiledCodeTabPage.Name = "DecompiledCodeTabPage";
            this.DecompiledCodeTabPage.Padding = new System.Windows.Forms.Padding(3);
            this.DecompiledCodeTabPage.Size = new System.Drawing.Size(713, 566);
            this.DecompiledCodeTabPage.TabIndex = 0;
            this.DecompiledCodeTabPage.Text = "Decompiled";
            this.DecompiledCodeTabPage.UseVisualStyleBackColor = true;
            // 
            // DecompiledCodeTextBox
            // 
            this.DecompiledCodeTextBox.ConfigurationManager.Language = "cs";
            this.DecompiledCodeTextBox.ContextMenuStrip = this.contextMenuStripForTextEditor;
            this.DecompiledCodeTextBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.DecompiledCodeTextBox.Font = new System.Drawing.Font("Consolas", 11.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.DecompiledCodeTextBox.IsBraceMatching = true;
            this.DecompiledCodeTextBox.IsReadOnly = true;
            this.DecompiledCodeTextBox.Location = new System.Drawing.Point(3, 3);
            this.DecompiledCodeTextBox.Name = "DecompiledCodeTextBox";
            this.DecompiledCodeTextBox.Size = new System.Drawing.Size(707, 560);
            this.DecompiledCodeTextBox.Styles.BraceBad.Size = 9F;
            this.DecompiledCodeTextBox.Styles.BraceLight.Size = 9F;
            this.DecompiledCodeTextBox.Styles.ControlChar.Size = 9F;
            this.DecompiledCodeTextBox.Styles.Default.BackColor = System.Drawing.SystemColors.Window;
            this.DecompiledCodeTextBox.Styles.Default.Size = 9F;
            this.DecompiledCodeTextBox.Styles.IndentGuide.Size = 9F;
            this.DecompiledCodeTextBox.Styles.LastPredefined.Size = 9F;
            this.DecompiledCodeTextBox.Styles.LineNumber.Size = 9F;
            this.DecompiledCodeTextBox.Styles.Max.Size = 9F;
            this.DecompiledCodeTextBox.TabIndex = 0;
            this.DecompiledCodeTextBox.Scroll += new System.EventHandler<System.Windows.Forms.ScrollEventArgs>(this.DecompiledCodeTextBox_Scroll);
            this.DecompiledCodeTextBox.MouseDown += new System.Windows.Forms.MouseEventHandler(this.DecompiledCodeTextBox_MouseDown);
            this.DecompiledCodeTextBox.MouseLeave += new System.EventHandler(this.DecompiledCodeTextBox_MouseLeave);
            this.DecompiledCodeTextBox.MouseMove += new System.Windows.Forms.MouseEventHandler(this.DecompiledCodeTextBox_MouseMove);
            // 
            // contextMenuStripForTextEditor
            // 
            this.contextMenuStripForTextEditor.Name = "contextMenuStrip2";
            this.contextMenuStripForTextEditor.Size = new System.Drawing.Size(61, 4);
            this.contextMenuStripForTextEditor.Opening += new System.ComponentModel.CancelEventHandler(this.contextMenuStrip2_Opening);
            // 
            // DisassembledTabPage
            // 
            this.DisassembledTabPage.Controls.Add(this.DisassembledCodeTextBox);
            this.DisassembledTabPage.Location = new System.Drawing.Point(4, 22);
            this.DisassembledTabPage.Name = "DisassembledTabPage";
            this.DisassembledTabPage.Padding = new System.Windows.Forms.Padding(3);
            this.DisassembledTabPage.Size = new System.Drawing.Size(713, 566);
            this.DisassembledTabPage.TabIndex = 1;
            this.DisassembledTabPage.Text = "Disassembled";
            this.DisassembledTabPage.UseVisualStyleBackColor = true;
            // 
            // DisassembledCodeTextBox
            // 
            this.DisassembledCodeTextBox.ContextMenuStrip = this.contextMenuStripForTextEditor;
            this.DisassembledCodeTextBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.DisassembledCodeTextBox.Font = new System.Drawing.Font("Consolas", 11.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.DisassembledCodeTextBox.Location = new System.Drawing.Point(3, 3);
            this.DisassembledCodeTextBox.Name = "DisassembledCodeTextBox";
            this.DisassembledCodeTextBox.Size = new System.Drawing.Size(707, 560);
            this.DisassembledCodeTextBox.Styles.BraceBad.Size = 9F;
            this.DisassembledCodeTextBox.Styles.BraceLight.Size = 9F;
            this.DisassembledCodeTextBox.Styles.ControlChar.Size = 9F;
            this.DisassembledCodeTextBox.Styles.Default.BackColor = System.Drawing.SystemColors.Window;
            this.DisassembledCodeTextBox.Styles.Default.Size = 9F;
            this.DisassembledCodeTextBox.Styles.IndentGuide.Size = 9F;
            this.DisassembledCodeTextBox.Styles.LastPredefined.Size = 9F;
            this.DisassembledCodeTextBox.Styles.LineNumber.Size = 9F;
            this.DisassembledCodeTextBox.Styles.Max.Size = 9F;
            this.DisassembledCodeTextBox.TabIndex = 0;
            // 
            // CombinedViewTabPage
            // 
            this.CombinedViewTabPage.Controls.Add(this.CombinedViewTextBox);
            this.CombinedViewTabPage.Location = new System.Drawing.Point(4, 22);
            this.CombinedViewTabPage.Name = "CombinedViewTabPage";
            this.CombinedViewTabPage.Padding = new System.Windows.Forms.Padding(3);
            this.CombinedViewTabPage.Size = new System.Drawing.Size(713, 566);
            this.CombinedViewTabPage.TabIndex = 2;
            this.CombinedViewTabPage.Text = "Combined";
            this.CombinedViewTabPage.UseVisualStyleBackColor = true;
            // 
            // CombinedViewTextBox
            // 
            this.CombinedViewTextBox.ContextMenuStrip = this.contextMenuStripForTextEditor;
            this.CombinedViewTextBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.CombinedViewTextBox.Font = new System.Drawing.Font("Consolas", 11.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.CombinedViewTextBox.Location = new System.Drawing.Point(3, 3);
            this.CombinedViewTextBox.Name = "CombinedViewTextBox";
            this.CombinedViewTextBox.Size = new System.Drawing.Size(707, 560);
            this.CombinedViewTextBox.Styles.BraceBad.Size = 9F;
            this.CombinedViewTextBox.Styles.BraceLight.Size = 9F;
            this.CombinedViewTextBox.Styles.ControlChar.Size = 9F;
            this.CombinedViewTextBox.Styles.Default.BackColor = System.Drawing.SystemColors.Window;
            this.CombinedViewTextBox.Styles.Default.Size = 9F;
            this.CombinedViewTextBox.Styles.IndentGuide.Size = 9F;
            this.CombinedViewTextBox.Styles.LastPredefined.Size = 9F;
            this.CombinedViewTextBox.Styles.LineNumber.Size = 9F;
            this.CombinedViewTextBox.Styles.Max.Size = 9F;
            this.CombinedViewTextBox.TabIndex = 1;
            // 
            // ExplorerTabControl
            // 
            this.ExplorerTabControl.ContextMenuStrip = this.contextMenuStripForTab;
            this.ExplorerTabControl.Dock = System.Windows.Forms.DockStyle.Top;
            this.ExplorerTabControl.Location = new System.Drawing.Point(0, 0);
            this.ExplorerTabControl.Name = "ExplorerTabControl";
            this.ExplorerTabControl.SelectedIndex = 0;
            this.ExplorerTabControl.Size = new System.Drawing.Size(721, 20);
            this.ExplorerTabControl.TabIndex = 1;
            this.ExplorerTabControl.Visible = false;
            this.ExplorerTabControl.MouseDown += new System.Windows.Forms.MouseEventHandler(this.ExplorerTabControl_MouseDown);
            this.ExplorerTabControl.MouseUp += new System.Windows.Forms.MouseEventHandler(this.ExplorerTabControl_MouseUp);
            this.ExplorerTabControl.SelectedIndexChanged += new System.EventHandler(this.ExplorerTabControl_SelectedIndexChanged);
            // 
            // contextMenuStripForTab
            // 
            this.contextMenuStripForTab.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.closeTabToolStripMenuItem1,
            this.newTabToolStripMenuItem1});
            this.contextMenuStripForTab.Name = "contextMenuStripForTab";
            this.contextMenuStripForTab.Size = new System.Drawing.Size(164, 48);
            this.contextMenuStripForTab.Text = "Tab Menu";
            // 
            // closeTabToolStripMenuItem1
            // 
            this.closeTabToolStripMenuItem1.Name = "closeTabToolStripMenuItem1";
            this.closeTabToolStripMenuItem1.ShortcutKeyDisplayString = "Ctrl+W";
            this.closeTabToolStripMenuItem1.Size = new System.Drawing.Size(163, 22);
            this.closeTabToolStripMenuItem1.Text = "&Close Tab";
            this.closeTabToolStripMenuItem1.Click += new System.EventHandler(this.closeTabToolStripMenuItem1_Click);
            // 
            // newTabToolStripMenuItem1
            // 
            this.newTabToolStripMenuItem1.Name = "newTabToolStripMenuItem1";
            this.newTabToolStripMenuItem1.ShortcutKeyDisplayString = "Ctrl+T";
            this.newTabToolStripMenuItem1.Size = new System.Drawing.Size(163, 22);
            this.newTabToolStripMenuItem1.Text = "&New Tab";
            this.newTabToolStripMenuItem1.Click += new System.EventHandler(this.newTabToolStripMenuItem1_Click);
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.backButton,
            this.forwardButton,
            this.toolStripSeparatorX,
            this.fileToolStripMenuItem,
            this.editToolStripMenuItem,
            this.goToolStripMenuItem,
            this.toolsToolStripMenuItem,
            this.optionsToolStripMenuItem,
            this.helpToolStripMenuItem,
            this.debugToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(965, 27);
            this.menuStrip1.TabIndex = 1;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // backButton
            // 
            this.backButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.backButton.Enabled = false;
            this.backButton.Image = global::AinDecompiler.Properties.Resources.back1;
            this.backButton.Name = "backButton";
            this.backButton.Size = new System.Drawing.Size(23, 20);
            this.backButton.Text = "Back";
            this.backButton.Click += new System.EventHandler(this.backButton_Click);
            // 
            // forwardButton
            // 
            this.forwardButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.forwardButton.Enabled = false;
            this.forwardButton.Image = global::AinDecompiler.Properties.Resources.back2;
            this.forwardButton.Name = "forwardButton";
            this.forwardButton.Size = new System.Drawing.Size(23, 20);
            this.forwardButton.Text = "Forward";
            this.forwardButton.Click += new System.EventHandler(this.forwardButton_Click);
            // 
            // toolStripSeparatorX
            // 
            this.toolStripSeparatorX.Name = "toolStripSeparatorX";
            this.toolStripSeparatorX.Size = new System.Drawing.Size(6, 23);
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.openToolStripMenuItem,
            this.recentFilesToolStripMenuItem,
            this.toolStripMenuItem2,
            this.openProjectToolStripMenuItem,
            this.saveAsProjectToolStripMenuItem,
            this.toolStripMenuItem3,
            this.exportToolStripMenuItem,
            this.toolStripSeparator2,
            this.exitToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(35, 23);
            this.fileToolStripMenuItem.Text = "&File";
            this.fileToolStripMenuItem.DropDownOpening += new System.EventHandler(this.fileToolStripMenuItem_DropDownOpening);
            // 
            // openToolStripMenuItem
            // 
            this.openToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("openToolStripMenuItem.Image")));
            this.openToolStripMenuItem.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.openToolStripMenuItem.Name = "openToolStripMenuItem";
            this.openToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.O)));
            this.openToolStripMenuItem.Size = new System.Drawing.Size(221, 22);
            this.openToolStripMenuItem.Text = "&Open AIN File...";
            this.openToolStripMenuItem.Click += new System.EventHandler(this.openToolStripMenuItem_Click);
            // 
            // recentFilesToolStripMenuItem
            // 
            this.recentFilesToolStripMenuItem.Name = "recentFilesToolStripMenuItem";
            this.recentFilesToolStripMenuItem.Size = new System.Drawing.Size(221, 22);
            this.recentFilesToolStripMenuItem.Text = "Open &Recent File";
            // 
            // toolStripMenuItem2
            // 
            this.toolStripMenuItem2.Name = "toolStripMenuItem2";
            this.toolStripMenuItem2.Size = new System.Drawing.Size(218, 6);
            // 
            // openProjectToolStripMenuItem
            // 
            this.openProjectToolStripMenuItem.Name = "openProjectToolStripMenuItem";
            this.openProjectToolStripMenuItem.Size = new System.Drawing.Size(221, 22);
            this.openProjectToolStripMenuItem.Text = "Open and Build &Project...";
            this.openProjectToolStripMenuItem.Click += new System.EventHandler(this.openProjectToolStripMenuItem_Click);
            // 
            // saveAsProjectToolStripMenuItem
            // 
            this.saveAsProjectToolStripMenuItem.Name = "saveAsProjectToolStripMenuItem";
            this.saveAsProjectToolStripMenuItem.Size = new System.Drawing.Size(221, 22);
            this.saveAsProjectToolStripMenuItem.Text = "&Save As Project...";
            this.saveAsProjectToolStripMenuItem.Click += new System.EventHandler(this.saveAsProjectToolStripMenuItem_Click);
            // 
            // toolStripMenuItem3
            // 
            this.toolStripMenuItem3.Name = "toolStripMenuItem3";
            this.toolStripMenuItem3.Size = new System.Drawing.Size(218, 6);
            // 
            // exportToolStripMenuItem
            // 
            this.exportToolStripMenuItem.Name = "exportToolStripMenuItem";
            this.exportToolStripMenuItem.Size = new System.Drawing.Size(221, 22);
            this.exportToolStripMenuItem.Text = "Export decompiled .JAF files...";
            this.exportToolStripMenuItem.Click += new System.EventHandler(this.exportToolStripMenuItem_Click);
            // 
            // toolStripSeparator2
            // 
            this.toolStripSeparator2.Name = "toolStripSeparator2";
            this.toolStripSeparator2.Size = new System.Drawing.Size(218, 6);
            // 
            // exitToolStripMenuItem
            // 
            this.exitToolStripMenuItem.Name = "exitToolStripMenuItem";
            this.exitToolStripMenuItem.Size = new System.Drawing.Size(221, 22);
            this.exitToolStripMenuItem.Text = "E&xit";
            this.exitToolStripMenuItem.Click += new System.EventHandler(this.exitToolStripMenuItem_Click);
            // 
            // editToolStripMenuItem
            // 
            this.editToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.copyToolStripMenuItem,
            this.toolStripSeparator4,
            this.selectAllToolStripMenuItem,
            this.toolStripMenuItem5,
            this.findTextToolStripMenuItem,
            this.showFindToolbarToolStripMenuItem,
            this.goToLineToolStripMenuItem});
            this.editToolStripMenuItem.Name = "editToolStripMenuItem";
            this.editToolStripMenuItem.Size = new System.Drawing.Size(37, 23);
            this.editToolStripMenuItem.Text = "&Edit";
            this.editToolStripMenuItem.DropDownOpening += new System.EventHandler(this.editToolStripMenuItem_DropDownOpening);
            // 
            // copyToolStripMenuItem
            // 
            this.copyToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("copyToolStripMenuItem.Image")));
            this.copyToolStripMenuItem.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.copyToolStripMenuItem.Name = "copyToolStripMenuItem";
            this.copyToolStripMenuItem.ShortcutKeyDisplayString = "Ctrl+C";
            this.copyToolStripMenuItem.Size = new System.Drawing.Size(198, 22);
            this.copyToolStripMenuItem.Text = "&Copy";
            this.copyToolStripMenuItem.Click += new System.EventHandler(this.copyToolStripMenuItem_Click);
            // 
            // toolStripSeparator4
            // 
            this.toolStripSeparator4.Name = "toolStripSeparator4";
            this.toolStripSeparator4.Size = new System.Drawing.Size(195, 6);
            // 
            // selectAllToolStripMenuItem
            // 
            this.selectAllToolStripMenuItem.Name = "selectAllToolStripMenuItem";
            this.selectAllToolStripMenuItem.ShortcutKeyDisplayString = "Ctrl+A";
            this.selectAllToolStripMenuItem.Size = new System.Drawing.Size(198, 22);
            this.selectAllToolStripMenuItem.Text = "Select &All";
            this.selectAllToolStripMenuItem.Click += new System.EventHandler(this.selectAllToolStripMenuItem_Click);
            // 
            // toolStripMenuItem5
            // 
            this.toolStripMenuItem5.Name = "toolStripMenuItem5";
            this.toolStripMenuItem5.Size = new System.Drawing.Size(195, 6);
            // 
            // findTextToolStripMenuItem
            // 
            this.findTextToolStripMenuItem.Name = "findTextToolStripMenuItem";
            this.findTextToolStripMenuItem.ShortcutKeyDisplayString = "Ctrl+F";
            this.findTextToolStripMenuItem.Size = new System.Drawing.Size(198, 22);
            this.findTextToolStripMenuItem.Text = "&Find Text...";
            this.findTextToolStripMenuItem.Click += new System.EventHandler(this.findTextToolStripMenuItem_Click);
            // 
            // showFindToolbarToolStripMenuItem
            // 
            this.showFindToolbarToolStripMenuItem.Name = "showFindToolbarToolStripMenuItem";
            this.showFindToolbarToolStripMenuItem.ShortcutKeyDisplayString = "Ctrl+I";
            this.showFindToolbarToolStripMenuItem.Size = new System.Drawing.Size(198, 22);
            this.showFindToolbarToolStripMenuItem.Text = "Show F&ind Toolbar";
            this.showFindToolbarToolStripMenuItem.Click += new System.EventHandler(this.showFindToolbarToolStripMenuItem_Click);
            // 
            // goToLineToolStripMenuItem
            // 
            this.goToLineToolStripMenuItem.Name = "goToLineToolStripMenuItem";
            this.goToLineToolStripMenuItem.ShortcutKeyDisplayString = "Ctrl+G";
            this.goToLineToolStripMenuItem.Size = new System.Drawing.Size(198, 22);
            this.goToLineToolStripMenuItem.Text = "Go to Line...";
            this.goToLineToolStripMenuItem.Click += new System.EventHandler(this.goToLineToolStripMenuItem_Click);
            // 
            // goToolStripMenuItem
            // 
            this.goToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.backToolStripMenuItem,
            this.forwardToolStripMenuItem,
            this.refreshToolStripMenuItem,
            this.toolStripMenuItem4,
            this.newTabToolStripMenuItem,
            this.closeTabToolStripMenuItem,
            this.toolStripMenuItem1,
            this.historyToolStripMenuItem,
            this.toolStripMenuItem6,
            this.decompiledToolStripMenuItem,
            this.disassembledToolStripMenuItem,
            this.combinedToolStripMenuItem,
            this.toolStripMenuItem7,
            this.nextTabToolStripMenuItem,
            this.previousTabToolStripMenuItem});
            this.goToolStripMenuItem.Name = "goToolStripMenuItem";
            this.goToolStripMenuItem.Size = new System.Drawing.Size(32, 23);
            this.goToolStripMenuItem.Text = "&Go";
            // 
            // backToolStripMenuItem
            // 
            this.backToolStripMenuItem.Name = "backToolStripMenuItem";
            this.backToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Alt | System.Windows.Forms.Keys.Left)));
            this.backToolStripMenuItem.Size = new System.Drawing.Size(216, 22);
            this.backToolStripMenuItem.Text = "&Back";
            this.backToolStripMenuItem.Click += new System.EventHandler(this.backToolStripMenuItem_Click);
            // 
            // forwardToolStripMenuItem
            // 
            this.forwardToolStripMenuItem.Name = "forwardToolStripMenuItem";
            this.forwardToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Alt | System.Windows.Forms.Keys.Right)));
            this.forwardToolStripMenuItem.Size = new System.Drawing.Size(216, 22);
            this.forwardToolStripMenuItem.Text = "&Forward";
            this.forwardToolStripMenuItem.Click += new System.EventHandler(this.forwardToolStripMenuItem_Click);
            // 
            // refreshToolStripMenuItem
            // 
            this.refreshToolStripMenuItem.Name = "refreshToolStripMenuItem";
            this.refreshToolStripMenuItem.ShortcutKeys = System.Windows.Forms.Keys.F5;
            this.refreshToolStripMenuItem.Size = new System.Drawing.Size(216, 22);
            this.refreshToolStripMenuItem.Text = "&Refresh";
            this.refreshToolStripMenuItem.Click += new System.EventHandler(this.refreshToolStripMenuItem_Click);
            // 
            // toolStripMenuItem4
            // 
            this.toolStripMenuItem4.Name = "toolStripMenuItem4";
            this.toolStripMenuItem4.Size = new System.Drawing.Size(213, 6);
            // 
            // newTabToolStripMenuItem
            // 
            this.newTabToolStripMenuItem.Name = "newTabToolStripMenuItem";
            this.newTabToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.T)));
            this.newTabToolStripMenuItem.Size = new System.Drawing.Size(216, 22);
            this.newTabToolStripMenuItem.Text = "&New Tab";
            this.newTabToolStripMenuItem.Click += new System.EventHandler(this.newTabToolStripMenuItem_Click);
            // 
            // closeTabToolStripMenuItem
            // 
            this.closeTabToolStripMenuItem.Name = "closeTabToolStripMenuItem";
            this.closeTabToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.W)));
            this.closeTabToolStripMenuItem.Size = new System.Drawing.Size(216, 22);
            this.closeTabToolStripMenuItem.Text = "&Close Tab";
            this.closeTabToolStripMenuItem.Click += new System.EventHandler(this.closeTabToolStripMenuItem_Click);
            // 
            // toolStripMenuItem1
            // 
            this.toolStripMenuItem1.Name = "toolStripMenuItem1";
            this.toolStripMenuItem1.Size = new System.Drawing.Size(213, 6);
            // 
            // historyToolStripMenuItem
            // 
            this.historyToolStripMenuItem.Name = "historyToolStripMenuItem";
            this.historyToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.H)));
            this.historyToolStripMenuItem.Size = new System.Drawing.Size(216, 22);
            this.historyToolStripMenuItem.Text = "&History...";
            this.historyToolStripMenuItem.Click += new System.EventHandler(this.historyToolStripMenuItem_Click);
            // 
            // toolStripMenuItem6
            // 
            this.toolStripMenuItem6.Name = "toolStripMenuItem6";
            this.toolStripMenuItem6.Size = new System.Drawing.Size(213, 6);
            // 
            // decompiledToolStripMenuItem
            // 
            this.decompiledToolStripMenuItem.Name = "decompiledToolStripMenuItem";
            this.decompiledToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Alt | System.Windows.Forms.Keys.D)));
            this.decompiledToolStripMenuItem.Size = new System.Drawing.Size(216, 22);
            this.decompiledToolStripMenuItem.Text = "&Decompiled";
            this.decompiledToolStripMenuItem.Click += new System.EventHandler(this.decompiledToolStripMenuItem_Click);
            // 
            // disassembledToolStripMenuItem
            // 
            this.disassembledToolStripMenuItem.Name = "disassembledToolStripMenuItem";
            this.disassembledToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Alt | System.Windows.Forms.Keys.S)));
            this.disassembledToolStripMenuItem.Size = new System.Drawing.Size(216, 22);
            this.disassembledToolStripMenuItem.Text = "Di&sassembled";
            this.disassembledToolStripMenuItem.Click += new System.EventHandler(this.disassembledToolStripMenuItem_Click);
            // 
            // combinedToolStripMenuItem
            // 
            this.combinedToolStripMenuItem.Name = "combinedToolStripMenuItem";
            this.combinedToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Alt | System.Windows.Forms.Keys.C)));
            this.combinedToolStripMenuItem.Size = new System.Drawing.Size(216, 22);
            this.combinedToolStripMenuItem.Text = "&Combined";
            this.combinedToolStripMenuItem.Click += new System.EventHandler(this.combinedToolStripMenuItem_Click);
            // 
            // toolStripMenuItem7
            // 
            this.toolStripMenuItem7.Name = "toolStripMenuItem7";
            this.toolStripMenuItem7.Size = new System.Drawing.Size(213, 6);
            // 
            // nextTabToolStripMenuItem
            // 
            this.nextTabToolStripMenuItem.Name = "nextTabToolStripMenuItem";
            this.nextTabToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.Tab)));
            this.nextTabToolStripMenuItem.Size = new System.Drawing.Size(216, 22);
            this.nextTabToolStripMenuItem.Text = "Ne&xt Tab";
            this.nextTabToolStripMenuItem.Click += new System.EventHandler(this.nextTabToolStripMenuItem_Click);
            // 
            // previousTabToolStripMenuItem
            // 
            this.previousTabToolStripMenuItem.Name = "previousTabToolStripMenuItem";
            this.previousTabToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)(((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.Shift)
                        | System.Windows.Forms.Keys.Tab)));
            this.previousTabToolStripMenuItem.Size = new System.Drawing.Size(216, 22);
            this.previousTabToolStripMenuItem.Text = "&Previous Tab";
            this.previousTabToolStripMenuItem.Click += new System.EventHandler(this.previousTabToolStripMenuItem_Click);
            // 
            // toolsToolStripMenuItem
            // 
            this.toolsToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.findToolStripMenuItem,
            this.translateVariableNamesToolStripMenuItem,
            this.replaceAllNamesToolStripMenuItem,
            this.viewEditAINStructuresToolStripMenuItem,
            this.toolStripMenuItem8,
            this.exportImportTextnewToolStripMenuItem,
            this.exportImportStringsToolStripMenuItem,
            this.wordwrapAndSaveAINToolStripMenuItem,
            this.debugCommandToolStripMenuItem});
            this.toolsToolStripMenuItem.Name = "toolsToolStripMenuItem";
            this.toolsToolStripMenuItem.Size = new System.Drawing.Size(44, 23);
            this.toolsToolStripMenuItem.Text = "&Tools";
            this.toolsToolStripMenuItem.DropDownOpening += new System.EventHandler(this.toolsToolStripMenuItem_DropDownOpening);
            // 
            // findToolStripMenuItem
            // 
            this.findToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.findFunctionsToolStripMenuItem,
            this.findGlobalsToolStripMenuItem,
            this.findClassStructMembersToolStripMenuItem,
            this.findStringsToolStripMenuItem,
            this.findMessagesToolStripMenuItem,
            this.findIntsToolStripMenuItem,
            this.findLocalVariablesToolStripMenuItem,
            this.findLibraryCallsToolStripMenuItem,
            this.findSystemCallsToolStripMenuItem});
            this.findToolStripMenuItem.Name = "findToolStripMenuItem";
            this.findToolStripMenuItem.Size = new System.Drawing.Size(279, 22);
            this.findToolStripMenuItem.Text = "&Find";
            // 
            // findFunctionsToolStripMenuItem
            // 
            this.findFunctionsToolStripMenuItem.Name = "findFunctionsToolStripMenuItem";
            this.findFunctionsToolStripMenuItem.Size = new System.Drawing.Size(213, 22);
            this.findFunctionsToolStripMenuItem.Text = "Find &Functions...";
            this.findFunctionsToolStripMenuItem.Click += new System.EventHandler(this.findFunctionsToolStripMenuItem_Click_1);
            // 
            // findGlobalsToolStripMenuItem
            // 
            this.findGlobalsToolStripMenuItem.Name = "findGlobalsToolStripMenuItem";
            this.findGlobalsToolStripMenuItem.Size = new System.Drawing.Size(213, 22);
            this.findGlobalsToolStripMenuItem.Text = "Find &Globals...";
            this.findGlobalsToolStripMenuItem.Click += new System.EventHandler(this.findGlobalsToolStripMenuItem_Click);
            // 
            // findClassStructMembersToolStripMenuItem
            // 
            this.findClassStructMembersToolStripMenuItem.Name = "findClassStructMembersToolStripMenuItem";
            this.findClassStructMembersToolStripMenuItem.Size = new System.Drawing.Size(213, 22);
            this.findClassStructMembersToolStripMenuItem.Text = "Find &Class/Struct members...";
            this.findClassStructMembersToolStripMenuItem.Click += new System.EventHandler(this.findClassStructMembersToolStripMenuItem_Click);
            // 
            // findStringsToolStripMenuItem
            // 
            this.findStringsToolStripMenuItem.Name = "findStringsToolStripMenuItem";
            this.findStringsToolStripMenuItem.Size = new System.Drawing.Size(213, 22);
            this.findStringsToolStripMenuItem.Text = "Find &Strings...";
            this.findStringsToolStripMenuItem.Click += new System.EventHandler(this.findStringsToolStripMenuItem_Click);
            // 
            // findMessagesToolStripMenuItem
            // 
            this.findMessagesToolStripMenuItem.Name = "findMessagesToolStripMenuItem";
            this.findMessagesToolStripMenuItem.Size = new System.Drawing.Size(213, 22);
            this.findMessagesToolStripMenuItem.Text = "Find &Messages...";
            this.findMessagesToolStripMenuItem.Click += new System.EventHandler(this.findMessagesToolStripMenuItem_Click);
            // 
            // findIntsToolStripMenuItem
            // 
            this.findIntsToolStripMenuItem.Name = "findIntsToolStripMenuItem";
            this.findIntsToolStripMenuItem.Size = new System.Drawing.Size(213, 22);
            this.findIntsToolStripMenuItem.Text = "Find &Ints...";
            this.findIntsToolStripMenuItem.Click += new System.EventHandler(this.findIntsToolStripMenuItem_Click);
            // 
            // findLocalVariablesToolStripMenuItem
            // 
            this.findLocalVariablesToolStripMenuItem.Name = "findLocalVariablesToolStripMenuItem";
            this.findLocalVariablesToolStripMenuItem.Size = new System.Drawing.Size(213, 22);
            this.findLocalVariablesToolStripMenuItem.Text = "Find &Local variables...";
            this.findLocalVariablesToolStripMenuItem.Click += new System.EventHandler(this.findLocalVariablesToolStripMenuItem_Click);
            // 
            // findLibraryCallsToolStripMenuItem
            // 
            this.findLibraryCallsToolStripMenuItem.Name = "findLibraryCallsToolStripMenuItem";
            this.findLibraryCallsToolStripMenuItem.Size = new System.Drawing.Size(213, 22);
            this.findLibraryCallsToolStripMenuItem.Text = "Find Library calls...";
            this.findLibraryCallsToolStripMenuItem.Click += new System.EventHandler(this.findLibraryCallsToolStripMenuItem_Click);
            // 
            // findSystemCallsToolStripMenuItem
            // 
            this.findSystemCallsToolStripMenuItem.Name = "findSystemCallsToolStripMenuItem";
            this.findSystemCallsToolStripMenuItem.Size = new System.Drawing.Size(213, 22);
            this.findSystemCallsToolStripMenuItem.Text = "Find System calls...";
            this.findSystemCallsToolStripMenuItem.Click += new System.EventHandler(this.findSystemCallsToolStripMenuItem_Click);
            // 
            // translateVariableNamesToolStripMenuItem
            // 
            this.translateVariableNamesToolStripMenuItem.Name = "translateVariableNamesToolStripMenuItem";
            this.translateVariableNamesToolStripMenuItem.Size = new System.Drawing.Size(279, 22);
            this.translateVariableNamesToolStripMenuItem.Text = "Translate Variable Names...";
            this.translateVariableNamesToolStripMenuItem.Click += new System.EventHandler(this.translateVariableNamesToolStripMenuItem_Click);
            // 
            // replaceAllNamesToolStripMenuItem
            // 
            this.replaceAllNamesToolStripMenuItem.Name = "replaceAllNamesToolStripMenuItem";
            this.replaceAllNamesToolStripMenuItem.Size = new System.Drawing.Size(279, 22);
            this.replaceAllNamesToolStripMenuItem.Text = "Replace All &Names with translated versions";
            this.replaceAllNamesToolStripMenuItem.Visible = false;
            this.replaceAllNamesToolStripMenuItem.Click += new System.EventHandler(this.replaceAllNamesToolStripMenuItem_Click);
            // 
            // viewEditAINStructuresToolStripMenuItem
            // 
            this.viewEditAINStructuresToolStripMenuItem.Name = "viewEditAINStructuresToolStripMenuItem";
            this.viewEditAINStructuresToolStripMenuItem.Size = new System.Drawing.Size(279, 22);
            this.viewEditAINStructuresToolStripMenuItem.Text = "View AIN &structures...";
            this.viewEditAINStructuresToolStripMenuItem.Click += new System.EventHandler(this.viewEditAINStructuresToolStripMenuItem_Click);
            // 
            // toolStripMenuItem8
            // 
            this.toolStripMenuItem8.Name = "toolStripMenuItem8";
            this.toolStripMenuItem8.Size = new System.Drawing.Size(276, 6);
            // 
            // exportImportTextnewToolStripMenuItem
            // 
            this.exportImportTextnewToolStripMenuItem.Name = "exportImportTextnewToolStripMenuItem";
            this.exportImportTextnewToolStripMenuItem.Size = new System.Drawing.Size(279, 22);
            this.exportImportTextnewToolStripMenuItem.Text = "&Export/Import Text (new)...";
            this.exportImportTextnewToolStripMenuItem.Click += new System.EventHandler(this.exportImportTextnewToolStripMenuItem_Click);
            // 
            // exportImportStringsToolStripMenuItem
            // 
            this.exportImportStringsToolStripMenuItem.Name = "exportImportStringsToolStripMenuItem";
            this.exportImportStringsToolStripMenuItem.Size = new System.Drawing.Size(279, 22);
            this.exportImportStringsToolStripMenuItem.Text = "Export/Import text (&legacy)...";
            this.exportImportStringsToolStripMenuItem.Click += new System.EventHandler(this.exportImportStringsToolStripMenuItem_Click);
            // 
            // wordwrapAndSaveAINToolStripMenuItem
            // 
            this.wordwrapAndSaveAINToolStripMenuItem.Name = "wordwrapAndSaveAINToolStripMenuItem";
            this.wordwrapAndSaveAINToolStripMenuItem.Size = new System.Drawing.Size(279, 22);
            this.wordwrapAndSaveAINToolStripMenuItem.Text = "&Wordwrap and Save AIN...";
            this.wordwrapAndSaveAINToolStripMenuItem.Click += new System.EventHandler(this.wordwrapAndSaveAINToolStripMenuItem_Click);
            // 
            // debugCommandToolStripMenuItem
            // 
            this.debugCommandToolStripMenuItem.Name = "debugCommandToolStripMenuItem";
            this.debugCommandToolStripMenuItem.Size = new System.Drawing.Size(279, 22);
            this.debugCommandToolStripMenuItem.Text = "Debug Command";
            this.debugCommandToolStripMenuItem.Click += new System.EventHandler(this.debugCommandToolStripMenuItem_Click);
            // 
            // optionsToolStripMenuItem
            // 
            this.optionsToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.displaythisAroundLocalMembersToolStripMenuItem,
            this.showAllCastingToolStripMenuItem,
            this.removeExtraReturnsToolStripMenuItem,
            this.declareVariablesAtBeginningOfFunctionToolStripMenuItem,
            this.verboseToolStripMenuItem,
            this.showGlobalVariableprefixesToolStripMenuItem,
            this.toolStripMenuItem9,
            this.showAlternativetranslatedVariableNamesToolStripMenuItem,
            this.toolStripMenuItem11,
            this.useModifiedShiftJISEncodingForEuropeanTextToolStripMenuItem});
            this.optionsToolStripMenuItem.Name = "optionsToolStripMenuItem";
            this.optionsToolStripMenuItem.Size = new System.Drawing.Size(41, 23);
            this.optionsToolStripMenuItem.Text = "&View";
            // 
            // displaythisAroundLocalMembersToolStripMenuItem
            // 
            this.displaythisAroundLocalMembersToolStripMenuItem.Checked = true;
            this.displaythisAroundLocalMembersToolStripMenuItem.CheckOnClick = true;
            this.displaythisAroundLocalMembersToolStripMenuItem.CheckState = System.Windows.Forms.CheckState.Checked;
            this.displaythisAroundLocalMembersToolStripMenuItem.Name = "displaythisAroundLocalMembersToolStripMenuItem";
            this.displaythisAroundLocalMembersToolStripMenuItem.Size = new System.Drawing.Size(312, 22);
            this.displaythisAroundLocalMembersToolStripMenuItem.Text = "Display \"&this.\" before local class members";
            this.displaythisAroundLocalMembersToolStripMenuItem.Click += new System.EventHandler(this.displaythisAroundLocalMembersToolStripMenuItem_Click);
            // 
            // showAllCastingToolStripMenuItem
            // 
            this.showAllCastingToolStripMenuItem.CheckOnClick = true;
            this.showAllCastingToolStripMenuItem.Name = "showAllCastingToolStripMenuItem";
            this.showAllCastingToolStripMenuItem.Size = new System.Drawing.Size(312, 22);
            this.showAllCastingToolStripMenuItem.Text = "Show all type &conversion";
            this.showAllCastingToolStripMenuItem.Click += new System.EventHandler(this.showAllCastingToolStripMenuItem_Click);
            // 
            // removeExtraReturnsToolStripMenuItem
            // 
            this.removeExtraReturnsToolStripMenuItem.Checked = true;
            this.removeExtraReturnsToolStripMenuItem.CheckOnClick = true;
            this.removeExtraReturnsToolStripMenuItem.CheckState = System.Windows.Forms.CheckState.Checked;
            this.removeExtraReturnsToolStripMenuItem.Name = "removeExtraReturnsToolStripMenuItem";
            this.removeExtraReturnsToolStripMenuItem.Size = new System.Drawing.Size(312, 22);
            this.removeExtraReturnsToolStripMenuItem.Text = "Remove extra &returns";
            this.removeExtraReturnsToolStripMenuItem.Click += new System.EventHandler(this.removeExtraReturnsToolStripMenuItem_Click);
            // 
            // declareVariablesAtBeginningOfFunctionToolStripMenuItem
            // 
            this.declareVariablesAtBeginningOfFunctionToolStripMenuItem.CheckOnClick = true;
            this.declareVariablesAtBeginningOfFunctionToolStripMenuItem.Name = "declareVariablesAtBeginningOfFunctionToolStripMenuItem";
            this.declareVariablesAtBeginningOfFunctionToolStripMenuItem.Size = new System.Drawing.Size(312, 22);
            this.declareVariablesAtBeginningOfFunctionToolStripMenuItem.Text = "&Declare variables at beginning of function";
            this.declareVariablesAtBeginningOfFunctionToolStripMenuItem.Click += new System.EventHandler(this.declareVariablesAtBeginningOfFunctionToolStripMenuItem_Click);
            // 
            // verboseToolStripMenuItem
            // 
            this.verboseToolStripMenuItem.CheckOnClick = true;
            this.verboseToolStripMenuItem.Name = "verboseToolStripMenuItem";
            this.verboseToolStripMenuItem.Size = new System.Drawing.Size(312, 22);
            this.verboseToolStripMenuItem.Text = "&Verbose";
            this.verboseToolStripMenuItem.Click += new System.EventHandler(this.verboseToolStripMenuItem_Click);
            // 
            // showGlobalVariableprefixesToolStripMenuItem
            // 
            this.showGlobalVariableprefixesToolStripMenuItem.CheckOnClick = true;
            this.showGlobalVariableprefixesToolStripMenuItem.Name = "showGlobalVariableprefixesToolStripMenuItem";
            this.showGlobalVariableprefixesToolStripMenuItem.Size = new System.Drawing.Size(312, 22);
            this.showGlobalVariableprefixesToolStripMenuItem.Text = "Show &global variable groups";
            this.showGlobalVariableprefixesToolStripMenuItem.Click += new System.EventHandler(this.showGlobalVariableprefixesToolStripMenuItem_Click);
            // 
            // toolStripMenuItem9
            // 
            this.toolStripMenuItem9.Name = "toolStripMenuItem9";
            this.toolStripMenuItem9.Size = new System.Drawing.Size(309, 6);
            // 
            // showAlternativetranslatedVariableNamesToolStripMenuItem
            // 
            this.showAlternativetranslatedVariableNamesToolStripMenuItem.CheckOnClick = true;
            this.showAlternativetranslatedVariableNamesToolStripMenuItem.Name = "showAlternativetranslatedVariableNamesToolStripMenuItem";
            this.showAlternativetranslatedVariableNamesToolStripMenuItem.Size = new System.Drawing.Size(312, 22);
            this.showAlternativetranslatedVariableNamesToolStripMenuItem.Text = "Show &Alternative (translated) variable names";
            this.showAlternativetranslatedVariableNamesToolStripMenuItem.Click += new System.EventHandler(this.showAlternativetranslatedVariableNamesToolStripMenuItem_Click);
            // 
            // helpToolStripMenuItem
            // 
            this.helpToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.aboutToolStripMenuItem});
            this.helpToolStripMenuItem.Name = "helpToolStripMenuItem";
            this.helpToolStripMenuItem.Size = new System.Drawing.Size(40, 23);
            this.helpToolStripMenuItem.Text = "&Help";
            // 
            // aboutToolStripMenuItem
            // 
            this.aboutToolStripMenuItem.Name = "aboutToolStripMenuItem";
            this.aboutToolStripMenuItem.Size = new System.Drawing.Size(115, 22);
            this.aboutToolStripMenuItem.Text = "&About...";
            this.aboutToolStripMenuItem.Click += new System.EventHandler(this.aboutToolStripMenuItem_Click);
            // 
            // debugToolStripMenuItem
            // 
            this.debugToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.testCodegeneratorToolStripMenuItem,
            this.testCodeGeneratorallFunctionsToolStripMenuItem,
            this.testCodeGeneratorTobuildNewAINToolStripMenuItem,
            this.testCompilerToolStripMenuItem,
            this.testtracerToolStripMenuItem,
            this.testdefaultArgumentFinderToolStripMenuItem,
            this.testSpeedOfStringExportToolStripMenuItem,
            this.makeSengokuRanceEnumToolStripMenuItem});
            this.debugToolStripMenuItem.Name = "debugToolStripMenuItem";
            this.debugToolStripMenuItem.Size = new System.Drawing.Size(50, 23);
            this.debugToolStripMenuItem.Text = "&Debug";
            this.debugToolStripMenuItem.Visible = false;
            // 
            // testCodegeneratorToolStripMenuItem
            // 
            this.testCodegeneratorToolStripMenuItem.Name = "testCodegeneratorToolStripMenuItem";
            this.testCodegeneratorToolStripMenuItem.Size = new System.Drawing.Size(254, 22);
            this.testCodegeneratorToolStripMenuItem.Text = "Test code &generator";
            this.testCodegeneratorToolStripMenuItem.Click += new System.EventHandler(this.testCodegeneratorToolStripMenuItem_Click);
            // 
            // testCodeGeneratorallFunctionsToolStripMenuItem
            // 
            this.testCodeGeneratorallFunctionsToolStripMenuItem.Name = "testCodeGeneratorallFunctionsToolStripMenuItem";
            this.testCodeGeneratorallFunctionsToolStripMenuItem.Size = new System.Drawing.Size(254, 22);
            this.testCodeGeneratorallFunctionsToolStripMenuItem.Text = "Test code generator (&all functions)";
            this.testCodeGeneratorallFunctionsToolStripMenuItem.Click += new System.EventHandler(this.testCodeGeneratorallFunctionsToolStripMenuItem_Click);
            // 
            // testCodeGeneratorTobuildNewAINToolStripMenuItem
            // 
            this.testCodeGeneratorTobuildNewAINToolStripMenuItem.Name = "testCodeGeneratorTobuildNewAINToolStripMenuItem";
            this.testCodeGeneratorTobuildNewAINToolStripMenuItem.Size = new System.Drawing.Size(254, 22);
            this.testCodeGeneratorTobuildNewAINToolStripMenuItem.Text = "Test code generator to &build new AIN";
            this.testCodeGeneratorTobuildNewAINToolStripMenuItem.Click += new System.EventHandler(this.testCodeGeneratorTobuildNewAINToolStripMenuItem_Click);
            // 
            // testCompilerToolStripMenuItem
            // 
            this.testCompilerToolStripMenuItem.Name = "testCompilerToolStripMenuItem";
            this.testCompilerToolStripMenuItem.Size = new System.Drawing.Size(254, 22);
            this.testCompilerToolStripMenuItem.Text = "Test &compiler";
            this.testCompilerToolStripMenuItem.Click += new System.EventHandler(this.testCompilerToolStripMenuItem_Click);
            // 
            // testtracerToolStripMenuItem
            // 
            this.testtracerToolStripMenuItem.Name = "testtracerToolStripMenuItem";
            this.testtracerToolStripMenuItem.Size = new System.Drawing.Size(254, 22);
            this.testtracerToolStripMenuItem.Text = "Test &tracer";
            this.testtracerToolStripMenuItem.Click += new System.EventHandler(this.testtracerToolStripMenuItem_Click);
            // 
            // testdefaultArgumentFinderToolStripMenuItem
            // 
            this.testdefaultArgumentFinderToolStripMenuItem.Name = "testdefaultArgumentFinderToolStripMenuItem";
            this.testdefaultArgumentFinderToolStripMenuItem.Size = new System.Drawing.Size(254, 22);
            this.testdefaultArgumentFinderToolStripMenuItem.Text = "Test &default argument finder";
            this.testdefaultArgumentFinderToolStripMenuItem.Click += new System.EventHandler(this.testdefaultArgumentFinderToolStripMenuItem_Click);
            // 
            // testSpeedOfStringExportToolStripMenuItem
            // 
            this.testSpeedOfStringExportToolStripMenuItem.Name = "testSpeedOfStringExportToolStripMenuItem";
            this.testSpeedOfStringExportToolStripMenuItem.Size = new System.Drawing.Size(254, 22);
            this.testSpeedOfStringExportToolStripMenuItem.Text = "Test speed of &String Export";
            this.testSpeedOfStringExportToolStripMenuItem.Click += new System.EventHandler(this.testSpeedOfStringExportToolStripMenuItem_Click);
            // 
            // makeSengokuRanceEnumToolStripMenuItem
            // 
            this.makeSengokuRanceEnumToolStripMenuItem.Name = "makeSengokuRanceEnumToolStripMenuItem";
            this.makeSengokuRanceEnumToolStripMenuItem.Size = new System.Drawing.Size(254, 22);
            this.makeSengokuRanceEnumToolStripMenuItem.Text = "Make &Enums for Sengoku Rance";
            this.makeSengokuRanceEnumToolStripMenuItem.Click += new System.EventHandler(this.makeSengokuRanceEnumToolStripMenuItem_Click);
            // 
            // useModifiedShiftJISEncodingForEuropeanTextToolStripMenuItem
            // 
            this.useModifiedShiftJISEncodingForEuropeanTextToolStripMenuItem.CheckOnClick = true;
            this.useModifiedShiftJISEncodingForEuropeanTextToolStripMenuItem.Name = "useModifiedShiftJISEncodingForEuropeanTextToolStripMenuItem";
            this.useModifiedShiftJISEncodingForEuropeanTextToolStripMenuItem.Size = new System.Drawing.Size(312, 22);
            this.useModifiedShiftJISEncodingForEuropeanTextToolStripMenuItem.Text = "Use Modified Shift-JIS encoding for accented text";
            this.useModifiedShiftJISEncodingForEuropeanTextToolStripMenuItem.CheckStateChanged += new System.EventHandler(this.useModifiedShiftJISEncodingForEuropeanTextToolStripMenuItem_CheckStateChanged);
            // 
            // toolStripMenuItem11
            // 
            this.toolStripMenuItem11.Name = "toolStripMenuItem11";
            this.toolStripMenuItem11.Size = new System.Drawing.Size(309, 6);
            // 
            // ExplorerForm
            // 
            this.AllowDrop = true;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(965, 639);
            this.Controls.Add(this.splitContainer1);
            this.Controls.Add(this.menuStrip1);
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "ExplorerForm";
            this.Text = "AIN Decompiler and Explorer tool";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.DragDrop += new System.Windows.Forms.DragEventHandler(this.ExplorerForm_DragDrop);
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.ExplorerForm_FormClosing);
            this.DragOver += new System.Windows.Forms.DragEventHandler(this.ExplorerForm_DragOver);
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel2.ResumeLayout(false);
            this.splitContainer1.ResumeLayout(false);
            this.contextMenuStripForTreeview.ResumeLayout(false);
            this.SelectionTabControl.ResumeLayout(false);
            this.DecompiledCodeTabPage.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.DecompiledCodeTextBox)).EndInit();
            this.DisassembledTabPage.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.DisassembledCodeTextBox)).EndInit();
            this.CombinedViewTabPage.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.CombinedViewTextBox)).EndInit();
            this.contextMenuStripForTab.ResumeLayout(false);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.SplitContainer splitContainer1;
        private System.Windows.Forms.TabControl SelectionTabControl;
        private System.Windows.Forms.TabPage DecompiledCodeTabPage;
        private System.Windows.Forms.TabPage DisassembledTabPage;
        private System.Windows.Forms.TreeView TreeView;
        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem openToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
        private System.Windows.Forms.ToolStripMenuItem exitToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem editToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem copyToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator4;
        private System.Windows.Forms.ToolStripMenuItem selectAllToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem toolsToolStripMenuItem;
        private System.Windows.Forms.ContextMenuStrip contextMenuStripForTreeview;
        private System.Windows.Forms.ToolStripMenuItem copyNameToClipboardToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem getCallsAndCalledByInformationToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem replaceAllNamesToolStripMenuItem;
        private ScintillaNET.Scintilla DecompiledCodeTextBox;
        private ScintillaNET.Scintilla DisassembledCodeTextBox;
        private System.Windows.Forms.ContextMenuStrip contextMenuStripForTextEditor;
        private System.Windows.Forms.ToolStripMenuItem exportToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem optionsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem displaythisAroundLocalMembersToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem showAllCastingToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem removeExtraReturnsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem findToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem findFunctionsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem findGlobalsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem findClassStructMembersToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem findStringsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem findMessagesToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem findIntsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem findLocalVariablesToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem viewEditAINStructuresToolStripMenuItem;
        private System.Windows.Forms.TabControl ExplorerTabControl;
        private System.Windows.Forms.ContextMenuStrip contextMenuStripForTab;
        private System.Windows.Forms.ToolStripButton forwardButton;
        private System.Windows.Forms.ToolStripButton backButton;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparatorX;
        private System.Windows.Forms.ToolStripMenuItem goToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem backToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem forwardToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem newTabToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem historyToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem closeTabToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem declareVariablesAtBeginningOfFunctionToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem verboseToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem closeTabToolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem newTabToolStripMenuItem1;
        private System.Windows.Forms.TabPage CombinedViewTabPage;
        private ScintillaNET.Scintilla CombinedViewTextBox;
        private System.Windows.Forms.ToolStripMenuItem showGlobalVariableprefixesToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripMenuItem2;
        private System.Windows.Forms.ToolStripMenuItem openProjectToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem saveAsProjectToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripMenuItem3;
        private System.Windows.Forms.ToolStripSeparator toolStripMenuItem4;
        private System.Windows.Forms.ToolStripSeparator toolStripMenuItem5;
        private System.Windows.Forms.ToolStripMenuItem findTextToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem goToLineToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem showFindToolbarToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem translateVariableNamesToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem helpToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem aboutToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripMenuItem6;
        private System.Windows.Forms.ToolStripMenuItem decompiledToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem disassembledToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem combinedToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripMenuItem7;
        private System.Windows.Forms.ToolStripMenuItem nextTabToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem previousTabToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripMenuItem8;
        private System.Windows.Forms.ToolStripMenuItem exportImportStringsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem wordwrapAndSaveAINToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem debugCommandToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem findLibraryCallsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem exportImportTextnewToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem findSystemCallsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem refreshToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem debugToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem makeSengokuRanceEnumToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem testCompilerToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem testCodegeneratorToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem testtracerToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem testdefaultArgumentFinderToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem testSpeedOfStringExportToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem testCodeGeneratorallFunctionsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem testCodeGeneratorTobuildNewAINToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem recentFilesToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripMenuItem9;
        private System.Windows.Forms.ToolStripMenuItem showAlternativetranslatedVariableNamesToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripMenuItem11;
        private System.Windows.Forms.ToolStripMenuItem useModifiedShiftJISEncodingForEuropeanTextToolStripMenuItem;
    }
}

