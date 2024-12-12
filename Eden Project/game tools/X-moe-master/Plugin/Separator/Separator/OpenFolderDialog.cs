using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Windows.Forms.Design;

namespace Separator
{
    class OpenFolderDialog
    {
        OpenFileDialog fDialog = new OpenFileDialog();
        public OpenFolderDialog()
        {
        }

        public DialogResult ShowDialog(string description, string Title)
        {
            fDialog.Multiselect = false;
            fDialog.Title = Title;
            fDialog.Filter = description;
            return fDialog.ShowDialog();
        }

        public string Path
        {
            get
            {
                return fDialog.FileName;
            }
        }

        public void Dispose()
        {
            fDialog.Dispose();
        }

    }
}
