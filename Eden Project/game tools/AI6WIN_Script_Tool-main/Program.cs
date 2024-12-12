// Base on crskycode's code.
// Add support by AyamiKaze
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GSIWin_Script_Tool
{
    class Program
    {
        static void Main(string[] args)
        {
            ///*
            if (args.Length != 2)
            {
                Console.WriteLine("Usage:");
                Console.WriteLine("  Export strings     : ScriptTool -e [file|folder]");
                Console.WriteLine("  Export all strings : ScriptTool -a [file|folder]");
                Console.WriteLine("  Rebuild script     : ScriptTool -b [file|folder]");
                Console.WriteLine("Press any key to continue...");
                Console.ReadKey();
                return;
            }

            var mode = args[0];
            var path = args[1];
            //*/

            //var mode = "-e";
            //var path = "opening.new";
            switch (mode)
            {
                case "-e":
                case "-a":
                {
                    bool exportAll = (mode == "-a");

                    void ExportString(string filePath)
                    {
                        Console.WriteLine($"Exporting strings from {Path.GetFileName(filePath)}");

                        try
                        {
                            var script = new Script();
                            script.Load(filePath);
                            script.ExportStrings(Path.ChangeExtension(filePath, "txt"), exportAll);
                        }
                        catch (Exception e)
                        {
                            Console.WriteLine(e.Message);
                        }
                    }

                    if (Util.PathIsFolder(path))
                    {
                        foreach (var item in Directory.EnumerateFiles(path, "*.MES"))
                        {
                            ExportString(item);
                        }
                    }
                    else
                    {
                        ExportString(path);
                    }

                    break;
                }
                case "-b":
                {
                    void RebuildScript(string filePath)
                    {
                        Console.WriteLine($"Rebuilding script {Path.GetFileName(filePath)}");

                        try
                        {
                            string textFilePath = Path.ChangeExtension(filePath, "txt");
                            //string newFilePath = Path.GetDirectoryName(filePath) + @"\rebuild\" + Path.GetFileName(filePath);
                            string newFilePath = Path.ChangeExtension(filePath, "new");
                            var script = new Script();
                            script.Load(filePath);
                            script.ImportStrings(textFilePath);
                            script.Save(newFilePath);
                        }
                        catch (Exception e)
                        {
                            Console.WriteLine(e.Message);
                        }
                    }

                    if (Util.PathIsFolder(path))
                    {
                        foreach (var item in Directory.EnumerateFiles(path, "*.MES"))
                        {
                            RebuildScript(item);
                        }
                    }
                    else
                    {
                        RebuildScript(path);
                    }

                    break;
                }
            }
        }
    }
}
