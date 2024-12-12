using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;

namespace Separator
{
    static class Program
    {
        /// <summary>
        /// 应用程序的主入口点。
        /// </summary>
        [STAThread]
        static void Main()
        {
            bool TestInfo = false;
            Mutex mutex = new Mutex(true, "SeparatorOneInstanceModule", out TestInfo);
            if(!TestInfo)
            {
                MessageBox.Show("Separator is running!");
                System.Environment.Exit(-1);
            }
            if(!File.Exists("HighDPIPlugin.dll"))
            {
                MessageBox.Show("Cannot find HignDPIPlugin.dll");
                System.Environment.Exit(-1);
            }
            APIWarpper.DisableHighDPI();
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new Form1());
            mutex.WaitOne();
        }
    }
}
