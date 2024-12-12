using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Text;
using System.Windows.Forms;

namespace AinDecompiler
{
    public static partial class FormsExtensions
    {
        public static int AddWithSeparator(this ToolStripItemCollection collection, ToolStripItem item)
        {
            if (collection.Count > 0)
            {
                collection.Add(new ToolStripSeparator());
            }
            return collection.Add(item);
        }
    }
}
