using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

//Import API
namespace Separator
{
    class APIWarpper
    {
        [DllImport("SeparatorCore.dll", CharSet = CharSet.Unicode)]
        public static extern int SeparatorEntry(int type, StringBuilder Name, 
            StringBuilder PluginName, StringBuilder Args, uint CoreCount);

        [DllImport("SeparatorCore.dll", CharSet = CharSet.Unicode)]
        public static extern int GetTaskLife();

        [DllImport("HighDPIPlugin.dll", CharSet = CharSet.Unicode)]
        public static extern int DisableHighDPI();
    }
}
