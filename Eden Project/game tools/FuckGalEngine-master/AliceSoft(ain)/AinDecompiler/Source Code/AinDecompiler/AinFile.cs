using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Text;
using System.Reflection;
using System.Diagnostics;
using System.Windows.Forms;
using System.Collections.ObjectModel;
using System.ComponentModel;

namespace AinDecompiler
{
    /// <summary>
    /// An AIN file from AliceSoft games.
    /// </summary>
    public class AinFile
    {
        //used so the debugger can find an AIN file somewhere even if the currently executing function can't see one
        [Browsable(false)]
        public static AinFile defaultInstance = null;

        /// <summary>
        /// The list of filenames used by the EOF instruction
        /// </summary>
        public StringCollection Filenames = new StringCollection();    //FNAM
        /// <summary>
        /// The list of functions (functions contain parameters and local variables)
        /// </summary>
        public FunctionCollection Functions = new FunctionCollection();  //FUNC
        /// <summary>
        /// The list of strings (some strings represent function names)
        /// </summary>
        public StringCollection Strings = new StringCollection();      //STR0
        /// <summary>
        /// The list of dialog messages displayed by the game
        /// </summary>
        public StringCollection Messages = new StringCollection();     //MSG0
        /// <summary>
        /// The code for the game.  Instructions are 2 bytes wide, instruction arguments are 4 bytes each.  See Instruction.cs for more information.
        /// </summary>
        public byte[] Code;           //CODE
        /// <summary>
        /// The version of the game.  Version 4 is the first version, Version 5 adds groups for global variables, Version 6 adds many new instructions.
        /// </summary>
        public int Version;           //VERS
        /// <summary>
        /// Whether or not to use a compressed (AI2) file instead of an uncompressed AIN file
        /// </summary>
        public bool IsAi2File;
        /// <summary>
        /// The encryption key used for the game.  In version 6, always seems to be zeroes.
        /// </summary>
        public int KeyCode;           //KEYC
        /// <summary>
        /// The list of global variables.  In version 5 and later, global variables can be put into separate groups when being saved and loaded.
        /// </summary>
        public GlobalCollection Globals = new GlobalCollection();      //GLOB
        /// <summary>
        /// The initial values for global variables.  May be an int, bool, float, or a string.
        /// </summary>
        public GlobalInitialValueCollection GlobalInitialValues = new GlobalInitialValueCollection();   //GSET
        /// <summary>
        /// The list of structs or classes.
        /// </summary>
        public StructCollection Structs = new StructCollection();      //STRT
        /// <summary>
        /// The ID number of the main function (entry point)
        /// </summary>
        public int MAIN;              //MAIN
        /// <summary>
        /// The ID number of the function called when a message is displayed.
        /// </summary>
        public int MSGF;              //MSGF
        /// <summary>
        /// The list of libraries, which contain the library functions
        /// </summary>
        public HllLibraryCollection Libraries = new HllLibraryCollection();     //HLL0
        /// <summary>
        /// The list of switch blocks.  Used by the SWITCH and STRSWITCH instructions.
        /// </summary>
        public SwitchBlockCollection Switches = new SwitchBlockCollection();     //SWI0
        /// <summary>
        /// The version of the game.  100 means 1.00, 107 means 1.07, etc...
        /// </summary>
        public int GameVersion;       //GVER
        /// <summary>
        /// The ID number for the onjump function, called when using the jump statement
        /// </summary>
        public int OJMP;              //OJMP
        /// <summary>
        /// The list of function types
        /// </summary>
        public FuncTypeCollection FunctionTypes = new FuncTypeCollection();  //FNCT
        /// <summary>
        /// The list of global variable groups used when the game saves and loads saved games.
        /// </summary>
        public StringCollection GlobalGroupNames = new StringCollection();  //OBJG
        /// <summary>
        /// The list of delegates (function pointers that can contain multiple functions)
        /// </summary>
        public FuncTypeCollection Delegates = new FuncTypeCollection();           //DELG

        /// <summary>
        /// A dictionary mapping struct names to their index in the Structs list
        /// </summary>
        [Browsable(false)]
        public Dictionary<string, int> StructNameToIndex = new Dictionary<string, int>();
        /// <summary>
        /// A dictionary mapping functions names to their index in the Functions list
        /// </summary>
        [Browsable(false)]
        public Dictionary<string, int> FunctionNameToIndex = new Dictionary<string, int>();
        /// <summary>
        /// A dictionary mapping global names to their index in the Globals list.  Both dotted and non-dotted forms are used.
        /// </summary>
        [Browsable(false)]
        public Dictionary<string, int> GlobalNameToIndex = new Dictionary<string, int>();
        /// <summary>
        /// A dictionary mapping system call names to their index in the system calls list.
        /// </summary>
        [Browsable(false)]
        public static readonly Dictionary<string, int> SystemCallNameToIndex = CreateSystemCallsDictionary();
        /// <summary>
        /// The list of system calls.  They all begin with "system." in their names.
        /// </summary>
        [Browsable(false)]
        public readonly static FunctionCollection SystemCalls = new FunctionCollection(CreateSystemCalls());

        /// <summary>
        /// Whether or not this AIN file is an "old version" seen in games like Daibanchou.
        /// This is identified by an old file version, and the lack of any directories named "system".
        /// Causes slightly different code generation behavior.
        /// </summary>
        public bool IsOldVersion = false;

        /// <summary>
        /// The filename that the AIN file was loaded with.
        /// </summary>
        public string OriginalFilename = "";

        /// <summary>
        /// The MetadataFile associated with this AinFile.  Contains variable replacement names, enumeration types, default arguments, etc.
        /// </summary>
        [Browsable(false)]
        public MetadataFile MetadataFile;

        /// <summary>
        /// A cache containing decompiled code so that searches do not have to decompile the same function multiple times.
        /// </summary>
        [Browsable(false)]
        public DecompiledCodeCache DecompiledCodeCache;

        /// <summary>
        /// A cache containing partial or complete results of searches, so that less data needs to be scanned during searches.
        /// </summary>
        [Browsable(false)]
        public CodeSearchCache CodeSearchCache;

        /// <summary>
        /// Whether or not we are displaying Alternative Names instead of normal names.  Make sure this is set to false everywhere outside the explorer.
        /// </summary>
        [Browsable(false)]
        public bool DisplayAlternativeNames;

        /// <summary>
        /// The set of alternative names by lookup table, can be used instead of the names from the metadata.
        /// </summary>
        [Browsable(false)]
        public AlternativeNames AlternativeNames = new AlternativeNames();

        /// <summary>
        /// Creates a blank AIN file.  Call ReadAinFile next.
        /// </summary>
        public AinFile()
        {
            defaultInstance = this;
            this.DecompiledCodeCache = new DecompiledCodeCache(this);
            this.CodeSearchCache = new CodeSearchCache(this);
            this.MetadataFile = new MetadataFile(this);
        }

        /// <summary>
        /// Creates an AIN file from a non-encrypted file.
        /// </summary>
        /// <param name="fileName">The file to load.</param>
        public AinFile(string fileName)
            : this()
        {
            ReadAinFile(fileName);
        }

        /// <summary>
        /// Reads an AIN file from the disk.  Prompts to decrypt the AIN file if necessary.
        /// </summary>
        /// <param name="fileName"></param>
        /// <returns></returns>
        public bool ReadAndDecryptAinFile(string inputFileName)
        {
            //If we read the file successfully, exit now.  Otherwise try to read a different file or decrypt it.
            if (!ReadAinFile(inputFileName))
            {
                string fileNameOfEncryptedFile = inputFileName;
                string fileNameOfDecryptedFile = inputFileName;

                //does encrypted copy exist right next to it?
                string path = Path.GetDirectoryName(inputFileName);
                string baseFileName = Path.GetFileNameWithoutExtension(inputFileName);

                //decrypted filename ends with ".ain_" because Crage creates that.
                fileNameOfDecryptedFile = Path.Combine(path, (baseFileName + ".ain_"));

                if (Path.GetExtension(inputFileName).Equals(".ain_", StringComparison.InvariantCultureIgnoreCase))
                {
                    fileNameOfEncryptedFile = Path.Combine(path, (baseFileName + ".ain"));
                }

                bool wantToDecrypt = false;

                //If file is already decrypted, and it loads successfully, we're done, don't decrypt anything.
                if (File.Exists(fileNameOfDecryptedFile))
                {
                    if (File.Exists(fileNameOfEncryptedFile))
                    {
                        var fileInfo1 = new FileInfo(fileNameOfEncryptedFile);
                        var fileInfo2 = new FileInfo(fileNameOfDecryptedFile);
                        if (fileInfo1.LastWriteTimeUtc > fileInfo2.LastWriteTimeUtc)
                        {
                            if (MessageBox.Show("File is already decrypted, but original file is newer.  Re-decrypt AIN file?", "Re-decrypt AIN file?", MessageBoxButtons.YesNo, MessageBoxIcon.Question, MessageBoxDefaultButton.Button1) == DialogResult.Yes)
                            {
                                wantToDecrypt = true;
                            }
                            else
                            {

                            }
                        }
                    }

                    if (wantToDecrypt == false)
                    {
                        if (ReadAinFile(fileNameOfDecryptedFile))
                        {
                            using (var fs = File.OpenRead(fileNameOfEncryptedFile))
                            {
                                var br = new BinaryReader(fs);
                                var first8bytes = br.ReadBytes(8);
                                byte[] ai2Signature = new byte[] { (byte)'A', (byte)'I', (byte)'2', 0, 0, 0, 0, 0 };
                                if (first8bytes.SequenceEqual(ai2Signature))
                                {
                                    this.IsAi2File = true;
                                }
                            }
                            return true;
                        }
                    }
                }

                if (wantToDecrypt == false)
                {
                    if (MessageBox.Show("AIN file is encrypted.  Decrypt it?", "Decrypt AIN file?", MessageBoxButtons.YesNo, MessageBoxIcon.Question, MessageBoxDefaultButton.Button1) == DialogResult.Yes)
                    {
                        wantToDecrypt = true;
                    }
                }


                if (wantToDecrypt == true)
                {
                    if (File.Exists(fileNameOfDecryptedFile))
                    {
                        File.Delete(fileNameOfDecryptedFile);
                    }

                    //Try using arc_conv to decompress the file.
                    var processStartInfo = new ProcessStartInfo();
                    processStartInfo.CreateNoWindow = true;
                    processStartInfo.UseShellExecute = false;

                    string toolsDirectory = Path.Combine(Path.GetDirectoryName(Application.ExecutablePath), "tools");

                    processStartInfo.FileName = Path.Combine(toolsDirectory, "arc_conv.exe");
                    processStartInfo.Arguments = "--mod ai2 \"" + fileNameOfEncryptedFile + "\" \"" + fileNameOfDecryptedFile + "\"";
                    var process = Process.Start(processStartInfo);
                    process.WaitForExit();

                    if (File.Exists(fileNameOfDecryptedFile))
                    {
                        if (ReadAinFile(fileNameOfDecryptedFile))
                        {
                            this.IsAi2File = true;
                            return true;
                        }
                        else
                        {
                            MessageBox.Show("Error: File was decrypted, but still failed to read it!", "Error", MessageBoxButtons.OK, MessageBoxIcon.Stop);
                            return false;
                        }
                    }

                    //Try using Crage to decrypt the file.
                    processStartInfo = new ProcessStartInfo();
                    processStartInfo.CreateNoWindow = true;
                    processStartInfo.UseShellExecute = false;
                    processStartInfo.FileName = Path.Combine(Path.Combine(toolsDirectory, "crage"), "crage.exe");
                    processStartInfo.WorkingDirectory = Path.Combine(toolsDirectory, "crage");
                    processStartInfo.Arguments = "-p \"" + fileNameOfEncryptedFile + "\" -F -o \"" + path + "\"";

                    process = Process.Start(processStartInfo);
                    process.WaitForExit();

                    if (File.Exists(fileNameOfDecryptedFile))
                    {
                        if (ReadAinFile(fileNameOfDecryptedFile))
                        {
                            return true;
                        }
                        else
                        {
                            MessageBox.Show("Error: File was decrypted, but still failed to read it!", "Error", MessageBoxButtons.OK, MessageBoxIcon.Stop);
                            return false;
                        }
                    }
                    MessageBox.Show("Error: Failed to decrypt file!", "Error", MessageBoxButtons.OK, MessageBoxIcon.Stop);
                    return false;
                }
                else
                {
                    return false;
                }
            }
            else
            {
                return true;
            }
            return false;
        }

        /// <summary>
        /// Reads an AIN file from the disk.  Must not be encrypted or compressed.
        /// </summary>
        /// <param name="fileName">The name of the file to read.</param>
        /// <returns>Returns true if the file was successfully read, otherwise returns false.</returns>
        public bool ReadAinFile(string fileName)
        {
            if (!File.Exists(fileName))
            {
                return false;
            }

            using (var ainFileStream = new FileStream(fileName, FileMode.Open, FileAccess.Read, FileShare.ReadWrite))
            {
                var br = new BinaryReader(ainFileStream);
                if (ReadAinFile(br))
                {
                    this.OriginalFilename = fileName;

                    LoadMetadata();
                    LoadAlternativeNames();
                    return true;
                }
                else
                {
                    return false;
                }
            }
        }

        /// <summary>
        /// Reads an AIN file given a binary reader.  Must not be encrypted or compressed.
        /// </summary>
        /// <param name="br">The binary reader</param>
        /// <returns>Returns true if the file was successfully read, otherwise returns false.</returns>
        public bool ReadAinFile(BinaryReader br)
        {
            while (br.BaseStream.Position >= 0 && br.BaseStream.Position < br.BaseStream.Length)
            {
                //If it fails to read a known tag, assume it has hit the end of the file
                if (!ReadTag(br))
                {
                    //If a version tag wasn't successfully read, then the file failed to load.
                    if (Version == 0)
                    {
                        return false;
                    }
                    return false;
                }
            }
            DetectOldVersion();

            //Set index, parent, and root values for everything
            ApplyIndexesAndParents();
            //create the lookup tables (dictionaries)
            CreateLookups();
            Expression.defaultAinFile = this;
            return true;
        }

        private void DetectOldVersion()
        {
            if (this.Version <= 4)
            {
                //any filenames start with "system" or "システム"?  It's not an old version.
                if (Filenames.Any(f =>
                    f.StartsWith("system\\", StringComparison.OrdinalIgnoreCase) ||
                    f.StartsWith("システム\\")))
                {
                    this.IsOldVersion = false;
                }
                else
                {
                    this.IsOldVersion = true;
                }
            }
        }

        /// <summary>
        /// Wrties an AIN file to the disk.
        /// </summary>
        /// <param name="fileName">The filename to save to.</param>
        public void WriteAinFile(string fileName)
        {
            var ms = new MemoryStream();
            var bw = new BinaryWriter(ms);
            WriteAinFile(bw);
            File.WriteAllBytes(fileName, ms.ToArray());
        }

        /// <summary>
        /// Writes an AIN file to the disk given a binary writer.
        /// </summary>
        /// <param name="bw">The binary writer to write to.</param>
        public void WriteAinFile(BinaryWriter bw)
        {
            //These are the tags we want to save, and the order to save them in.
            string[] tags = new string[]
            {
                "VERS","KEYC","CODE","FUNC","GLOB","GSET","STRT","MSG0","MAIN","MSGF","HLL0","SWI0","GVER","STR0","FNAM","OJMP","FNCT","DELG","OBJG",
            };

            foreach (var tagName in tags)
            {
                if (!(tagName == "OBJG" && this.Version <= 4) &&  //version 4 does not have OBJG
                    !(tagName == "DELG" && (this.Delegates == null || this.Delegates.Count == 0)))    //Only save "DELG" tag if it exists
                {
                    WriteTag(tagName, bw);
                }
            }
        }

        /// <summary>
        /// Creates the lookup tables (dictionaries) used to find Functions and Structs by name.
        /// </summary>
        public void CreateLookups()
        {
            //Create the lookup table for Structs
            StructNameToIndex.Clear();
            for (int i = 0; i < Structs.Count; i++)
            {
                var structInfo = Structs[i];
                StructNameToIndex.Set(structInfo.Name, i);
            }

            //Create the lookup table for Functions
            FunctionNameToIndex.Clear();
            for (int i = 0; i < Functions.Count; i++)
            {
                var function = Functions[i];
                FunctionNameToIndex.Set(function.Name, i);
            }

            //Create the lookup table for globals
            GlobalNameToIndex.Clear();
            for (int i = 0; i < Globals.Count; i++)
            {
                var global = Globals[i];
                GlobalNameToIndex.Set(global.Name, i);
                if (global.GroupName != null)
                {
                    GlobalNameToIndex.Set(global.GroupName + "." + global.Name, i);
                }
            }

        }

        /// <summary>
        /// Creates the list of System calls.  This applies to all AIN files.
        /// </summary>
        /// <returns>The list of system calls.</returns>
        private static Function[] CreateSystemCalls()
        {
            return new Function[]{
                //These are official names from the SDK documentation
                Function.Create(DataType.Void, "system.Exit", DataType.Int, "nResult"),
                Function.Create(DataType.Int, "system.GlobalSave", DataType.String, "szKeyName", DataType.String, "szFileName"),
                Function.Create(DataType.Int, "system.GlobalLoad", DataType.String, "szKeyName", DataType.String, "szFileName"),
                Function.Create(DataType.Int, "system.LockPeek"),
                Function.Create(DataType.Int, "system.UnlockPeek"),
                Function.Create(DataType.Void, "system.Reset"),
                Function.Create(DataType.String, "system.Output", DataType.String, "szText"),
                Function.Create(DataType.String, "system.MsgBox", DataType.String, "szText"),
                Function.Create(DataType.Int, "system.ResumeSave", DataType.String, "szKeyName", DataType.String, "szFileName", DataType.RefInt, "nResult"),
                Function.Create(DataType.Void, "system.ResumeLoad", DataType.String, "szKeyName", DataType.String, "szFileName"),
                Function.Create(DataType.Int, "system.ExistFile", DataType.String, "szFileName"),
                Function.Create(DataType.Void, "system.OpenWeb", DataType.String, "szURL"),
                Function.Create(DataType.String, "system.GetSaveFolderName"),
                Function.Create(DataType.Int, "system.GetTime"),
                Function.Create(DataType.String, "system.GetGameName"),
                Function.Create(DataType.String, "system.Error", DataType.String, "szText"),
                Function.Create(DataType.Int, "system.ExistSaveFile", DataType.String, "szFileName"),
                Function.Create(DataType.Int, "system.IsDebugMode"),
                Function.Create(DataType.Int, "system.MsgBoxOkCancel", DataType.String, "szText"),
                Function.Create(DataType.String, "system.GetFuncStackName", DataType.Int, "nIndex"),
                Function.Create(DataType.Void, "system.Peek"),
                Function.Create(DataType.Void, "system.Sleep", DataType.Int, "nSleep"),
                Function.Create(DataType.Bool, "system.ResumeWriteComment", DataType.String, "szKeyName", DataType.String, "szFileName", DataType.RefArrayString, "aszComment"),
                Function.Create(DataType.Bool, "system.ResumeReadComment", DataType.String, "szKeyName", DataType.String, "szFileName", DataType.RefArrayString, "aszComment"),
                //Past this point, parameter names are guesses
                Function.Create(DataType.Int, "system.GroupSave", DataType.String, "szKeyName", DataType.String, "szFileName", DataType.String, "szGroupName", DataType.Int, "nSaveNumber"),
                Function.Create(DataType.Int, "system.GroupLoad", DataType.String, "szKeyName", DataType.String, "szFileName", DataType.String, "szGroupName", DataType.Int, "nSaveNumber"),
                Function.Create(DataType.Int, "system.DeleteSaveFile", DataType.String, "szFileName"),
                Function.Create(DataType.Bool, "system.ExistFunc", DataType.String, "szFuncName"),
                Function.Create(DataType.Int, "system.CopySaveFile", DataType.String, "szDestFileName", DataType.String, "szSourceFileName"),
            };
        }

        /// <summary>
        /// Builds the dictionary mapping system call names to their indexes
        /// </summary>
        /// <returns>The dictionary that maps system call names to their indexes</returns>
        private static Dictionary<string, int> CreateSystemCallsDictionary()
        {
            Dictionary<string, int> dic = new Dictionary<string, int>();
            var calls = CreateSystemCalls();
            for (int i = 0; i < calls.Length; i++)
            {
                dic.Add(calls[i].Name, i);
            }
            return dic;
        }

        /// <summary>
        /// Reads a tag from the AIN file.  If it can not find a known tag, returns false.
        /// </summary>
        /// <param name="br">The binary reader</param>
        /// <returns>True if the tag was read, False if the tag was unknown or failed to read.</returns>
        private bool ReadTag(BinaryReader br)
        {
            int length;
            int count;
            string tagName = br.ReadStringFixedSize(4);
            switch (tagName)
            {
                case "VERS":
                    Version = br.ReadInt32();
                    break;
                case "KEYC":
                    KeyCode = br.ReadInt32();
                    break;
                case "CODE":
                    length = br.ReadInt32();
                    Code = br.ReadBytes(length);
                    break;
                case "FUNC":
                    length = br.ReadInt32();
                    Functions.Clear();
                    Functions.AddRange(Util.Read<Function>(br, length));
                    break;
                case "GLOB":
                    length = br.ReadInt32();
                    Globals.Clear();
                    Globals.AddRange(Util.Read<Global>(br, length, Version));
                    break;
                case "GSET":
                    length = br.ReadInt32();
                    GlobalInitialValues.Clear();
                    GlobalInitialValues.AddRange(Util.Read<GlobalInitialValue>(br, length));
                    break;
                case "STRT":
                    length = br.ReadInt32();
                    Structs.Clear();
                    Structs.AddRange(Util.Read<Struct>(br, length));
                    break;
                case "MSG0":
                    length = br.ReadInt32();
                    Messages.Clear();
                    Messages.AddRange(Util.ReadStrings(br, length));
                    break;
                case "MAIN":
                    MAIN = br.ReadInt32();
                    break;
                case "MSGF":
                    MSGF = br.ReadInt32();
                    break;
                case "HLL0":
                    length = br.ReadInt32();
                    Libraries.Clear();
                    Libraries.AddRange(Util.Read<HllLibrary>(br, length));
                    break;
                case "SWI0":
                    length = br.ReadInt32();
                    Switches.Clear();
                    Switches.AddRange(Util.Read<SwitchBlock>(br, length));
                    break;
                case "GVER":
                    GameVersion = br.ReadInt32();
                    break;
                case "STR0":
                    length = br.ReadInt32();
                    Strings.Clear();
                    Strings.AddRange(Util.ReadStrings(br, length));
                    break;
                case "FNAM":
                    length = br.ReadInt32();
                    Filenames.Clear();
                    Filenames.AddRange(Util.ReadStrings(br, length));
                    break;
                case "OJMP":
                    OJMP = br.ReadInt32();
                    break;
                case "FNCT":
                    length = br.ReadInt32() - 4;
                    count = br.ReadInt32();
                    FunctionTypes.Clear();
                    FunctionTypes.AddRange(Util.Read<FunctionType>(br, count));
                    break;
                case "DELG":
                    length = br.ReadInt32() - 4;
                    count = br.ReadInt32();
                    this.Delegates.Clear();
                    this.Delegates.AddRange(Util.Read<FunctionType>(br, count));
                    break;
                case "OBJG":
                    length = br.ReadInt32();
                    GlobalGroupNames.Clear();
                    GlobalGroupNames.AddRange(Util.ReadStrings(br, length));
                    break;
                default:
                    return false;
            }
            return true;
        }

        /// <summary>
        /// Writes a TAG to the AIN file.
        /// </summary>
        /// <param name="tagName">The name of the tag, also selects what tag data gets written.</param>
        /// <param name="bw">The binary writer to write to.</param>
        private void WriteTag(string tagName, BinaryWriter bw)
        {
            bw.WriteStringFixedSize(tagName, 4);
            switch (tagName)
            {
                case "VERS":
                    bw.Write(Version);
                    break;
                case "KEYC":
                    bw.Write(KeyCode);
                    break;
                case "CODE":
                    bw.Write(Code.Length);
                    bw.Write(Code);
                    break;
                case "FUNC":
                    bw.Write(Functions.Count);
                    Util.Write(bw, Functions);
                    break;
                case "GLOB":
                    bw.Write(Globals.Count);
                    Util.Write(bw, Globals, Version);
                    break;
                case "GSET":
                    bw.Write(GlobalInitialValues.Count);
                    Util.Write(bw, GlobalInitialValues);
                    break;
                case "STRT":
                    bw.Write(Structs.Count);
                    Util.Write(bw, Structs);
                    break;
                case "MSG0":
                    bw.Write(Messages.Count);
                    Util.WriteStrings(bw, Messages);
                    break;
                case "MAIN":
                    bw.Write(MAIN);
                    break;
                case "MSGF":
                    bw.Write(MSGF);
                    break;
                case "HLL0":
                    bw.Write(Libraries.Count);
                    Util.Write(bw, Libraries);
                    break;
                case "SWI0":
                    bw.Write(Switches.Count);
                    Util.Write(bw, Switches);
                    break;
                case "GVER":
                    bw.Write(GameVersion);
                    break;
                case "STR0":
                    bw.Write(Strings.Count);
                    Util.WriteStrings(bw, Strings);
                    break;
                case "FNAM":
                    bw.Write(Filenames.Count);
                    Util.WriteStrings(bw, Filenames);
                    break;
                case "OJMP":
                    bw.Write(OJMP);
                    break;
                case "FNCT":
                    {
                        int startPosition = (int)bw.BaseStream.Position;
                        bw.Write(0); //write zero, later change this to the actual length - 4
                        bw.Write(FunctionTypes.Count);
                        Util.Write(bw, FunctionTypes);
                        int endPosition = (int)bw.BaseStream.Position;
                        bw.BaseStream.Position = startPosition;
                        bw.Write(endPosition - startPosition);
                        bw.BaseStream.Position = endPosition;
                    }
                    break;
                case "DELG":
                    {
                        int startPosition = (int)bw.BaseStream.Position;
                        bw.Write(0);  //write zero, later change this to the actual length - 4
                        bw.Write(Delegates.Count);
                        Util.Write(bw, Delegates);
                        int endPosition = (int)bw.BaseStream.Position;
                        bw.BaseStream.Position = startPosition;
                        bw.Write(endPosition - startPosition);
                        bw.BaseStream.Position = endPosition;
                    }
                    break;
                case "OBJG":
                    bw.Write(GlobalGroupNames.Count);
                    Util.WriteStrings(bw, GlobalGroupNames);
                    break;
            }
        }

        /// <summary>
        /// Writes an AIN file, and encrypts or compresss it.  Also deletes the unencrypted copy.
        /// </summary>
        /// <param name="fileName">The filename to write to.</param>
        public void WriteAndEncryptAinFile(string fileName)
        {
            //Save the unencrypted copy
            WriteAinFile(fileName);
            string path = Path.GetDirectoryName(fileName);
            string baseFileName = Path.GetFileNameWithoutExtension(fileName);
            string otherFileName;
            otherFileName = Path.Combine(path, (baseFileName + ".ain_"));

            string toolsDirectory = Path.Combine(Path.GetDirectoryName(Application.ExecutablePath), "tools");

            if (this.Version < 6 && !this.IsAi2File)
            {
                //Use Crage to encrypt the file
                var processStartInfo = new ProcessStartInfo();
                processStartInfo.CreateNoWindow = true;
                processStartInfo.UseShellExecute = false;
                processStartInfo.FileName = Path.Combine(Path.Combine(toolsDirectory, "crage"), "crage.exe");
                processStartInfo.WorkingDirectory = Path.Combine(toolsDirectory, "crage");
                processStartInfo.Arguments = "-p \"" + fileName + "\" -F -o \"" + path + "\"";

                var process = Process.Start(processStartInfo);
                process.WaitForExit();

                //If it's sucessful, delete the file, then rename the encrypted version.
                if (File.Exists(otherFileName))
                {
                    File.Delete(fileName);
                    File.Move(otherFileName, fileName);
                }
            }
            else
            {
                //Use arc_conv to compress the file.
                var processStartInfo = new ProcessStartInfo();
                processStartInfo.CreateNoWindow = true;
                processStartInfo.UseShellExecute = false;

                processStartInfo.FileName = Path.Combine(toolsDirectory, "arc_conv.exe");
                processStartInfo.Arguments = "--mod ai2 \"" + fileName + "\" \"" + otherFileName + "\"";
                var process = Process.Start(processStartInfo);
                process.WaitForExit();

                //If it's successful, delete the file, then rename the compressed version.
                if (File.Exists(otherFileName))
                {
                    File.Delete(fileName);
                    File.Move(otherFileName, fileName);
                    return;
                }
            }
        }

        /// <summary>
        /// Sets the index for every element of an array.
        /// </summary>
        /// <param name="list">The list to set indexes of</param>
        void SetIndexes<T>(IEnumerable<T> list) where T : IWithIndex
        {
            if (list != null)
            {
                int i = 0;
                foreach (var item in list)
                {
                    item.Index = i;
                    i++;
                }
            }
        }

        /// <summary>
        /// Sets the Parent for every element of an array.
        /// </summary>
        /// <param name="list">The list to set the parents of</param>
        /// <param name="parent">The parent object to use</param>
        void SetParents<T>(IEnumerable<T> list, object parent) where T : IWithParent
        {
            if (list != null)
            {
                foreach (var item in list)
                {
                    item.Parent = parent;
                }
            }
        }

        /// <summary>
        /// Sets the root for every element of an array.  (Root is the AinFile object)
        /// </summary>
        /// <param name="list">The list to set the roots of.</param>
        /// <param name="rootObject">The root object to use</param>
        void SetRoots<T>(IEnumerable<T> list, AinFile rootObject) where T : IWithRoot<AinFile>
        {
            if (list != null)
            {
                foreach (var item in list)
                {
                    item.Root = rootObject;
                }
            }
        }

        /// <summary>
        /// Sets the index and parents to a list of items recursively (base list has no parent)
        /// </summary>
        /// <typeparam name="T">The type of the list item</typeparam>
        /// <param name="list">The list of items to set.</param>
        void ApplyIndexesAndParents<T>(IEnumerable<T> list)
        {
            ApplyIndexesAndParents(list, null);
        }

        /// <summary>
        /// Sets the index, parents, and Root to a list of items recursively
        /// </summary>
        /// <typeparam name="T">The type of the list item</typeparam>
        /// <param name="list">The list of items t oset.</param>
        /// <param name="parentObject">The parent object to set (possibly null)</param>
        void ApplyIndexesAndParents<T>(IEnumerable<T> list, object parentObject)
        {
            //Set indexes, parents, and roots of the current list.
            SetIndexes(list.OfType<IWithIndex>());
            SetParents(list.OfType<IWithParent>(), parentObject);
            SetRoots(list.OfType<IWithRoot<AinFile>>(), this);

            //If each element of the list has has child elements (is a 'function'), apply to those too.
            var functions = list.OfType<IFunction>();
            if (functions != null)
            {
                foreach (var function in functions)
                {
                    ApplyIndexesAndParents(function.Parameters, function);
                }
            }
        }

        /// <summary>
        /// Sets indexes, parents, and Root to all the arrays of this class.
        /// </summary>
        public void ApplyIndexesAndParents()
        {
            ApplyIndexesAndParents(this.Functions);
            ApplyIndexesAndParents(this.FunctionTypes);
            ApplyIndexesAndParents(this.GlobalInitialValues);
            ApplyIndexesAndParents(this.Globals);
            ApplyIndexesAndParents(this.Libraries);
            ApplyIndexesAndParents(this.Structs);
            ApplyIndexesAndParents(this.Switches);
            ApplyIndexesAndParents(this.Delegates);
            foreach (var switchBlock in this.Switches)
            {
                ApplyIndexesAndParents(switchBlock.SwitchCases);
            }
        }

        /// <summary>
        /// Clones an AIN file, but does not clone the code.  Used when replacing text with a translation, so the original copy remains in memory when exporting.
        /// </summary>
        /// <returns>A copy of the AIN file.</returns>
        public AinFile Clone()
        {
            //does not clone code
            var clone = new AinFile();
            clone.Code = this.Code;
            //clone.Dummy = this.Dummy;
            clone.Filenames.AddRange(this.Filenames.ToList());
            clone.FunctionNameToIndex = new Dictionary<string, int>();
            clone.GlobalNameToIndex = new Dictionary<string, int>();
            clone.Functions.AddRange(this.Functions.Select(f => f.Clone()));
            clone.FunctionTypes.AddRange(this.FunctionTypes.Select(f => f.Clone()));
            clone.Delegates.AddRange(this.Delegates.Select(f => f.Clone()));
            clone.GameVersion = this.GameVersion;
            clone.GlobalInitialValues.AddRange(this.GlobalInitialValues.Select(g => g.Clone()));
            clone.GlobalGroupNames.AddRange(this.GlobalGroupNames);
            clone.Globals.AddRange(this.Globals.Select(g => g.Clone()));
            clone.KeyCode = this.KeyCode;
            clone.Libraries.AddRange(this.Libraries.Select(l => l.Clone()));
            clone.MAIN = this.MAIN;
            clone.Messages.AddRange(this.Messages);
            clone.MSGF = this.MSGF;
            clone.OJMP = this.OJMP;
            clone.OriginalFilename = Path.Combine(Path.GetDirectoryName(this.OriginalFilename), Path.GetFileNameWithoutExtension(this.OriginalFilename) + "_replaced" + Path.GetExtension(this.OriginalFilename));
            clone.Strings.AddRange(this.Strings);
            clone.StructNameToIndex = new Dictionary<string, int>();
            clone.Structs.AddRange(this.Structs.Select(s => s.Clone()));
            clone.Switches.AddRange(this.Switches.Select(s => s.Clone()));
            clone.Version = this.Version;
            clone.ApplyIndexesAndParents();
            clone.CreateLookups();

            clone.AlternativeNames = this.AlternativeNames;
            clone.MetadataFile = this.MetadataFile.Clone(clone);
            clone.DisplayAlternativeNames = this.DisplayAlternativeNames;

            clone.IsOldVersion = this.IsOldVersion;
            return clone;
        }

        #region Getters
        /// <summary>
        /// Returns the string from the Strings array, or null if the string does not exist.
        /// </summary>
        /// <param name="stringNumber">index of the Strings array</param>
        /// <returns>A string from the strings array, or null</returns>
        public string GetString(int stringNumber)
        {
            if (stringNumber >= 0 && stringNumber < this.Strings.Count)
            {
                return this.Strings[stringNumber];
            }
            else
            {
                return null;
            }
        }

        /// <summary>
        /// Returns the global variable from the Globals array, or null if the global variable does not exist.
        /// </summary>
        /// <param name="globalNumber">index of the Globals array</param>
        /// <returns>A global variable from the Globals array, or null</returns>
        public Global GetGlobal(int globalNumber)
        {
            if (globalNumber >= 0 && globalNumber < this.Globals.Count)
            {
                return this.Globals[globalNumber];
            }
            else
            {
                return null;
            }
        }

        /// <summary>
        /// Returns the global variable from the Globals array by name, or null if the global variable is not found.
        /// </summary>
        /// <param name="globalName">Name of the Global variable</param>
        /// <returns>A global variable from the Globals array, or null</returns>
        public Global GetGlobal(string globalName)
        {
            if (globalName == null)
            {
                return null;
            }
            if (this.GlobalNameToIndex.ContainsKey(globalName))
            {
                int index = this.GlobalNameToIndex[globalName];
                return GetGlobal(index);
            }
            return null;
        }

        /// <summary>
        /// Returns the function from the Functions array, or null if the function does not exist.
        /// </summary>
        /// <param name="functionNumber">index of the Functions array</param>
        /// <returns>A function from the Functions array, or null</returns>
        public Function GetFunction(int functionNumber)
        {
            if (functionNumber >= 0 && functionNumber < this.Functions.Count)
            {
                return this.Functions[functionNumber];
            }
            else
            {
                return null;
            }
        }

        /// <summary>
        /// Returns the function from the Functions array by looking up its name in FunctionNameToIndex, or null if the function does not exist.
        /// </summary>
        /// <param name="functionName">Name of the function to look up</param>
        /// <returns>A function from the Functions array, or null</returns>
        public Function GetFunction(string functionName)
        {
            if (functionName == null)
            {
                return null;
            }
            if (this.FunctionNameToIndex.ContainsKey(functionName))
            {
                return GetFunction(this.FunctionNameToIndex[functionName]);
            }
            else
            {
                return null;
            }
        }

        /// <summary>
        /// Returns the struct from the Structs array, or null if the struct does not exist.
        /// </summary>
        /// <param name="structNumber">index of the Structs array</param>
        /// <returns>A struct from the Structs array, or null</returns>
        public Struct GetStruct(int structNumber)
        {
            if (structNumber >= 0 && structNumber < this.Structs.Count)
            {
                return this.Structs[structNumber];
            }
            else
            {
                return null;
            }
        }

        /// <summary>
        /// Returns the struct from the Structs array by looking up its name in StructNameToIndex, or null if the struct does not exist.
        /// </summary>
        /// <param name="sturctName">Name of the struct to look up</param>
        /// <returns>A struct from the Structs array, or null</returns>
        public Struct GetStruct(string sturctName)
        {
            if (sturctName == null)
            {
                return null;
            }
            if (this.StructNameToIndex.ContainsKey(sturctName))
            {
                return GetStruct(this.StructNameToIndex[sturctName]);
            }
            else
            {
                return null;
            }
        }

        public Variable GetStructMember(Struct structInfo, string memberName)
        {
            if (structInfo == null || memberName == null)
            {
                return null;
            }
            foreach (var memberInfo in structInfo.Members)
            {
                if (memberInfo.Name == memberName)
                {
                    return memberInfo;
                }
            }
            return null;
        }

        public Variable GetStructMember(Struct structInfo, int memberIndex)
        {
            if (structInfo == null)
            {
                return null;
            }
            if (memberIndex >= 0 && memberIndex < structInfo.Members.Count)
            {
                return structInfo.Members[memberIndex];
            }
            return null;
        }

        public Variable GetStructMember(string structName, string memberName)
        {
            return GetStructMember(GetStruct(structName), memberName);
        }

        public Variable GetStructMember(string structName, int memberIndex)
        {
            return GetStructMember(GetStruct(structName), memberIndex);
        }

        public Variable GetStructMember(int structIndex, int memberIndex)
        {
            return GetStructMember(GetStruct(structIndex), memberIndex);
        }

        public Variable GetStructMember(int structIndex, string memberName)
        {
            return GetStructMember(GetStruct(structIndex), memberName);
        }

        /// <summary>
        /// Returns a function parameter or local variable from a given function, or null if either parameter is out of bounds.
        /// </summary>
        /// <param name="functionNumber">index of the Functions array</param>
        /// <param name="index">index in the Parameters array</param>
        /// <returns>A variable from the function, or null</returns>
        public Variable GetFunctionParameter(int functionNumber, int index)
        {
            var function = GetFunction(functionNumber);
            return GetFunctionParameter(function, index);
        }

        /// <summary>
        /// Returns a function parameter or local variable from a given function, or null if either parameter is out of bounds.
        /// </summary>
        /// <param name="function">Function to get it from</param>
        /// <param name="index">index in the Parameters array</param>
        /// <returns>A variable from the function, or null</returns>
        public Variable GetFunctionParameter(Function function, int index)
        {
            if (function != null && index >= 0 && index < function.Parameters.Count)
            {
                return function.Parameters[index];
            }
            else
            {
                return null;
            }
        }

        /// <summary>
        /// Returns a function parameter or local variable from a given function, or null if the name is not found.  Stops at the first duplicate.
        /// </summary>
        /// <param name="function">Function to get it from</param>
        /// <param name="parameterName">Name of the parameter or variable</param>
        /// <returns>A variable from the function, or null</returns>
        public Variable GetFunctionParameter(Function function, string parameterName)
        {
            if (function == null)
            {
                return null;
            }
            foreach (var parameter in function.Parameters)
            {
                if (parameter.Name == parameterName)
                {
                    return parameter;
                }
            }
            return null;
        }

        /// <summary>
        /// Returns the first function parameter or local variable from a given function which matches the name, or null if the name is not found.
        /// </summary>
        /// <param name="function">Function to get it from</param>
        /// <param name="index">index in the Parameters array</param>
        /// <returns>The first matching variable from the function, or null</returns>
        public static Variable GetFunctionParameterByName(Function function, string name)
        {
            if (function == null)
            {
                return null;
            }
            for (int i = 0; i < function.Parameters.Count; i++)
            {
                var parameter = function.Parameters[i];
                if (parameter.Name == name)
                {
                    return parameter;
                }
            }
            return null;
        }

        /// <summary>
        /// Returns a function parameter or local variable from a given function, or null if either parameter is out of bounds.
        /// </summary>
        /// <param name="functionName">Name of the function to look up</param>
        /// <param name="index">index in the Parameters array</param>
        /// <returns>A variable from the function, or null</returns>
        public Variable GetFunctionParameter(string functionName, int index)
        {
            var function = GetFunction(functionName);
            return GetFunctionParameter(function, index);
        }

        /// <summary>
        /// Returns a system call from the SystemCalls array, or null if the system call does not exist.
        /// </summary>
        /// <param name="sysCallNumber">index of the system call number</param>
        /// <returns>A system call, or null</returns>
        public static Function GetSystemCall(int sysCallNumber)
        {
            if (sysCallNumber >= 0 && sysCallNumber < AinFile.SystemCalls.Count)
            {
                return AinFile.SystemCalls[sysCallNumber];
            }
            return null;
        }

        /// <summary>
        /// Returns a system call from the SystemCalls array, or null if the system call does not exist.
        /// </summary>
        /// <param name="sysCallNumber">index of the system call number</param>
        /// <returns>A system call, or null</returns>
        public static Variable GetSystemCallParameter(int sysCallNumber, int parameterIndex)
        {
            var systemCall = GetSystemCall(sysCallNumber);
            if (systemCall != null)
            {
                if (parameterIndex >= 0 && parameterIndex < systemCall.ParameterCount)
                {
                    return systemCall.Parameters[parameterIndex];
                }
            }
            return null;
        }

        /// <summary>
        /// Returns a system call from the SystemCalls array by name, or null if the system call does not exist.
        /// </summary>
        /// <param name="sysCallName">The name of the system call</param>
        /// <returns>A system call, or null</returns>
        public static Function GetSystemCall(string sysCallName)
        {
            if (SystemCallNameToIndex.ContainsKey(sysCallName))
            {
                return SystemCalls[SystemCallNameToIndex[sysCallName]];
            }
            sysCallName = "system." + sysCallName;
            if (SystemCallNameToIndex.ContainsKey(sysCallName))
            {
                return SystemCalls[SystemCallNameToIndex[sysCallName]];
            }
            return null;
        }

        /// <summary>
        /// Returns a library function from the LibraryFunctions array by indexes, or null if the library function does not exist.
        /// </summary>
        /// <param name="libraryNumber">Index of the library</param>
        /// <param name="libraryFunctionNumber">Index of the library function</param>
        /// <returns>A library call, or null</returns>
        public HllFunction GetLibraryFunction(int libraryNumber, int libraryFunctionNumber)
        {
            if (libraryNumber >= 0 && libraryNumber < Libraries.Count)
            {
                var library = Libraries[libraryNumber];
                if (libraryFunctionNumber >= 0 && libraryFunctionNumber < library.Functions.Count)
                {
                    return library.Functions[libraryFunctionNumber];
                }
            }
            return null;
        }

        /// <summary>
        /// Returns a parameter of a library function, or null if the parameter does not exist.
        /// </summary>
        /// <param name="libraryNumber">Index of the library</param>
        /// <param name="libraryFunctionNumber">Index of the library function</param>
        /// <param name="parameterIndex">Index of the parameter</param>
        /// <returns>The parameter of the library function, or null</returns>
        public HllFunctionParameter GetLibraryFunctionParameter(int libraryNumber, int libraryFunctionNumber, int parameterIndex)
        {
            var libraryFunction = GetLibraryFunction(libraryNumber, libraryFunctionNumber);
            if (libraryFunction != null)
            {
                if (parameterIndex >= 0 && parameterIndex < libraryFunction.ParameterCount)
                {
                    return libraryFunction.Parameters[parameterIndex];
                }
            }
            return null;
        }

        /// <summary>
        /// Returns a function type from the FuncitonTypes array, or null if the function type does not exist.
        /// </summary>
        /// <param name="funcTypeIndex">Index of the function type</param>
        /// <returns>The function type, or null</returns>
        public FunctionType GetFuncType(int funcTypeIndex)
        {
            if (funcTypeIndex >= 0 && funcTypeIndex < this.FunctionTypes.Count)
            {
                return FunctionTypes[funcTypeIndex];
            }
            return null;
        }

        /// <summary>
        /// Returns a parameter from a function type, or null if the parameter does not exist.
        /// </summary>
        /// <param name="funcTypeIndex">Index of the funciton type</param>
        /// <param name="parameterIndex">Index of the parameter</param>
        /// <returns>The function type parameter, or null</returns>
        public Variable GetFuncTypeParameter(int funcTypeIndex, int parameterIndex)
        {
            var funcType = GetFuncType(funcTypeIndex);
            if (funcType != null)
            {
                if (parameterIndex >= 0 && parameterIndex < funcType.ParameterCount)
                {
                    return funcType.Parameters[parameterIndex];
                }
            }
            return null;
        }

        #endregion

        /// <summary>
        /// Returns information of the instruction at the specified address
        /// </summary>
        /// <param name="address">The address to peek at</param>
        /// <returns>Information of the instruction</returns>
        public InstructionInfo Peek(int address)
        {
            return Decompiler.Peek(this.Code, address);
        }

        /// <summary>
        /// Returns a message from the Messages array, or null if the message does not exist
        /// </summary>
        /// <param name="messageNumber">Index of the array</param>
        /// <returns>The message, or null</returns>
        public string GetMessage(int messageNumber)
        {
            if (messageNumber >= 0 && messageNumber < this.Messages.Count)
            {
                return this.Messages[messageNumber];
            }
            else
            {
                return null;
            }
        }

        /// <summary>
        /// Loads the metadata (variable alternative names, enumeration types, default arguments, etc) from a file named like "ainfile_metadata.xml"
        /// </summary>
        /// <returns>True if the file was loaded, otherwise false</returns>
        public bool LoadMetadata()
        {
            string xmlFileName = GetXmlFilename();
            if (xmlFileName != null)
            {
                if (File.Exists(xmlFileName))
                {
                    try
                    {
                        this.MetadataFile.LoadFile(xmlFileName);
                        return true;
                    }
                    catch (IOException)
                    {

                    }
                    catch (UnauthorizedAccessException)
                    {

                    }
                }
            }
            return false;
        }

        /// <summary>
        /// Saves the metadata (variable alternative names, enumeration types, default arguments, etc) to a file named like "ainfile_metadata.xml".
        /// </summary>
        /// <returns>True if the file was saved, otherwise false</returns>
        public bool SaveMetadata()
        {
            string xmlFileName = GetXmlFilename();
            if (xmlFileName != null)
            {
                try
                {
                    this.MetadataFile.SaveFile(xmlFileName);
                    return true;
                }
                catch (IOException)
                {

                }
                catch (UnauthorizedAccessException)
                {

                }
            }
            return false;
        }

        /// <summary>
        /// Returns a filename like "ainfile_metadata.xml".
        /// </summary>
        /// <returns>The filename</returns>
        private string GetXmlFilename()
        {
            string fileName = this.OriginalFilename;
            if (string.IsNullOrEmpty(fileName))
            {
                return null;
            }
            string directoryName = Path.GetDirectoryName(fileName);
            string baseFileName = Path.GetFileNameWithoutExtension(fileName);
            string xmlFileName = Path.Combine(directoryName, baseFileName + "_metadata.xml");

            return xmlFileName;
        }

        /// <summary>
        /// Loads the alternative names from a file named like "ainfile_names.txt"
        /// </summary>
        /// <returns>True if loading succeeded, otherwise false.</returns>
        public bool LoadAlternativeNames()
        {
            string fileName = GetAlternativeNamesFilename();

            AlternativeNames.Clear();
            return AlternativeNames.LoadFile(fileName);
        }

        /// <summary>
        /// Returns a filename like "ainfile_names.txt"
        /// </summary>
        /// <returns>The filename</returns>
        private string GetAlternativeNamesFilename()
        {
            string fileName = this.OriginalFilename;
            if (string.IsNullOrEmpty(fileName))
            {
                return null;
            }
            string directoryName = Path.GetDirectoryName(fileName);
            string baseFileName = Path.GetFileNameWithoutExtension(fileName);
            string alternativeNamesFileName = Path.Combine(directoryName, baseFileName + "_names.txt");

            return alternativeNamesFileName;
        }
    }

    /// <summary>
    /// Utility class to output all the members of a class as a string.
    /// </summary>
    public static class ReflectionUtil
    {
        /// <summary>
        /// Gets a string containing the values of the fields of an object.
        /// </summary>
        /// <param name="obj">The object to get the fields of</param>
        /// <returns>A string containing the values of the object's fields</returns>
        public static string ToString(object obj)
        {
            StringBuilder sb = new StringBuilder();

            //get the type
            var type = obj.GetType();

            //get the public fields that aren't static
            var fields = type.GetFields(BindingFlags.Instance | BindingFlags.DeclaredOnly | BindingFlags.Public);

            //display them all
            bool needComma = false;
            foreach (var field in fields)
            {
                if (field.FieldType == typeof(int) || field.FieldType == typeof(string) || field.FieldType.IsEnum)
                {
                    Util.PrintComma(sb, ref needComma);
                    sb.Append(field.Name + " = " + (field.GetValue(obj) ?? "null").ToString());
                }
            }
            return sb.ToString();

        }

        /// <summary>
        /// Assigns all the public writable fields and properties from one object to another
        /// </summary>
        /// <typeparam name="TObj">Type of the object</typeparam>
        /// <param name="assignTo">Object to assign to</param>
        /// <param name="readFrom">Object to read from</param>
        public static void MemberwiseAssign<TObj>(TObj assignTo, TObj readFrom)
        {
            var fields = typeof(TObj).GetFields(BindingFlags.Instance | BindingFlags.FlattenHierarchy | BindingFlags.Public);
            var properties = typeof(TObj).GetProperties(BindingFlags.Instance | BindingFlags.FlattenHierarchy | BindingFlags.Public);

            foreach (var field in fields)
            {
                if (!field.IsInitOnly)
                {
                    object value = field.GetValue(readFrom);
                    field.SetValue(assignTo, value);
                }
            }
            foreach (var property in properties)
            {
                if (property.CanRead && property.CanWrite)
                {
                    object value = property.GetValue(readFrom, null);
                    property.SetValue(assignTo, value, null);
                }
            }
        }
    }
}
