using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;
using System.Threading;
using System.Diagnostics;
using System.Reflection;
using System.IO;
using System.Globalization;

namespace AinDecompiler
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Thread.CurrentThread.CurrentCulture = CultureInfo.InvariantCulture;

            //initialization of static classes that needs to be done after all other static initialization - doing it here
            ArgumentKinds.InitArgumentKinds();

            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);

            //parse arguments:
            //* file.ain - open the file
            //* file.ain  file2.jam - disassemble the file (using default settings, including multi-file mode)
            //* file.jam  file2.ain - assemble the file
            var args = Environment.GetCommandLineArgs();
            string fileName1 = "";
            string fileName2 = "";

            if (args.Length > 1)
            {
                fileName1 = args[1];
            }

            if (args.Length > 2)
            {
                fileName2 = args[2];
            }

            ExplorerForm explorerForm = null;

            string ext1 = Path.GetExtension(fileName1).ToLowerInvariant();
            string ext2 = Path.GetExtension(fileName2).ToLowerInvariant();

            bool fileName1IsAinFile = ext1 == ".ain" || ext1 == ".ain_";
            bool fileName2IsAinFile = ext2 == ".ain" || ext2 == ".ain_";
            bool fileName1IsJamFile = ext1 == ".jam";
            bool fileName2IsJamFile = ext2 == ".jam";

            if (fileName1IsAinFile && fileName2IsJamFile)
            {
                CreateProject(fileName1, fileName2);
            }
            else if (fileName1IsJamFile && fileName2IsAinFile)
            {
                BuildProject(fileName1, fileName2);
            }
            else if (fileName1IsAinFile)
            {
                explorerForm = new ExplorerForm(fileName1);
                Application.Run(explorerForm);
            }
            else
            {
                explorerForm = new ExplorerForm();
                Application.Run(explorerForm);
            }
        }

        private static void BuildProject(string inputProjectFilename, string outputAinFilename)
        {
            var reader = new AssemblerProjectReader();
            reader.LoadProject(inputProjectFilename);
            var ainFile = reader.MakeAinFile();
            if (outputAinFilename.EndsWith("_"))
            {
                ainFile.WriteAinFile(outputAinFilename);
            }
            else
            {
                ainFile.WriteAndEncryptAinFile(outputAinFilename);
            }
        }

        private static void CreateProject(string inputAinFilename, string outputProjectFilename)
        {
            var ainFile = new AinFile(inputAinFilename);
            ainFile.LoadAlternativeNames();
            //var alternativeNames = new AlternativeNames(inputAinFilename);
            var writer = new AssemblerProjectWriter(ainFile);
            //writer.AlternativeNames = alternativeNames;
            writer.SaveAsProject(outputProjectFilename, false);
        }
    }

    public static partial class Util
    {
        public static void RunWhenIdle(Action action)
        {
            Action<object, EventArgs> handler = null;

            handler = (sender, e) =>
            {
                Application.Idle -= new EventHandler(handler);
                action();
            };

            Application.Idle += new EventHandler(handler);
        }

    }
}
