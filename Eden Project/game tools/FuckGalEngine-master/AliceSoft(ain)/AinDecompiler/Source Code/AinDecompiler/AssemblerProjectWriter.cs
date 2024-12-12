using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Text;
using System.Diagnostics;
using System.Globalization;
using System.ComponentModel;

namespace AinDecompiler
{
    public class AssemblerProjectWriter
    {
        public MyBackgroundWorker worker = null;
        bool StopEmittingCode = false;
        bool Cancel
        {
            get
            {
                return worker != null && worker.CancellationPending;
            }
        }
        TinyStopwatch stopwatch = new TinyStopwatch();


        AinFile ainFile;
        ExpressionDisplayer displayer = new ExpressionDisplayer();
        Decompiler decompiler;
        Dictionary<string, Dictionary<int, int>> DuplicateStrings = new Dictionary<string, Dictionary<int, int>>();
        Dictionary<string, Dictionary<int, int>> DuplicateMessages = new Dictionary<string, Dictionary<int, int>>();

        public bool ShowGlobalPrefix { get { return Options.ShowGlobalPrefix; } set { Options.ShowGlobalPrefix = value; } }

        int lastSeenStructType;

        //public AlternativeNames AlternativeNames = new AlternativeNames();
        public CodeDisplayOptions Options = new CodeDisplayOptions();

        public AssemblerProjectWriter()
        {
            stopwatch.Start();
        }

        public AssemblerProjectWriter(AinFile ainFile)
            : this()
        {
            ReadAinFile(ainFile);
        }

        public AssemblerProjectWriter(AinFile ainFile, CodeDisplayOptions options)
            : this(ainFile)
        {
            this.Options = options ?? new CodeDisplayOptions();
        }

        public void ReadAinFile(AinFile ainFile)
        {
            this.ainFile = ainFile;
            displayer.ainFile = ainFile;
            decompiler = new Decompiler(ainFile);
        }

        MyIndentedTextWriter tw;
        MyIndentedTextWriter mainIndentedTextWriter;

        public string GetCombinedFunction(int functionAddress)
        {
            var sw = new StringWriter(CultureInfo.InvariantCulture);
            this.tw = new MyIndentedTextWriter(sw);

            int startAddress = functionAddress - 6;
            int endAddress = decompiler.FindEndAddress(functionAddress);

            this.FindLabels(startAddress, endAddress);

            this.WriteCombinedFunction(functionAddress);
            this.tw.Flush();
            sw.Flush();
            return sw.GetStringBuilder().ToString();
        }

        public string GetCombinedFunction(int functionAddress, string[] lines, int[] addressForLineNumber)
        {
            var sw = new StringWriter(CultureInfo.InvariantCulture);
            this.tw = new MyIndentedTextWriter(sw);

            int startAddress = functionAddress - 6;
            int endAddress = decompiler.FindEndAddress(functionAddress);

            this.FindLabels(startAddress, endAddress);
            this.WriteCombinedFunction(startAddress, endAddress, lines, addressForLineNumber);

            this.tw.Flush();
            sw.Flush();
            return sw.GetStringBuilder().ToString();
        }

        private void WriteCombinedFunction(int functionAddress)
        {
            int endAddress = decompiler.FindEndAddress(functionAddress);
            int startAddress = functionAddress - 6;

            Expression expression = null;
            ExpressionDisplayer displayer = new ExpressionDisplayer(ainFile, this.Options);
            string decompiledFunction = null;
            ExpressionMap expressionMap = null;

            try
            {
                expression = decompiler.DecompileFunction(functionAddress);
                displayer.expressionMap = new ExpressionMap();
                decompiledFunction = displayer.PrintExpression2(expression);
                expressionMap = displayer.expressionMap;
            }
            catch (Exception ex)
            {

            }

            if (expressionMap != null)
            {
                string[] lines = decompiledFunction.Split(new[] { "\r\n" }, StringSplitOptions.None);
                int[] addressForLineNumber = expressionMap.GetAddressPerLineNumber();

                this.WriteCombinedFunction(startAddress, endAddress, lines, addressForLineNumber);
            }
            else
            {
                WriteFunction(startAddress, endAddress);
            }
        }

        private void WriteCombinedFunction(int startAddress, int endAddress, string[] lines, int[] addressForLineNumber)
        {
            int currentAddress = startAddress;
            int lineNumber = 0;

            while (true)
            {
                //grab from text file until we reach currentAddress
                int nextLineAddress = endAddress;
                while (true)
                {
                    if (lineNumber >= addressForLineNumber.Length)
                    {
                        nextLineAddress = endAddress;
                        break;
                    }

                    int lineAddress = addressForLineNumber[lineNumber];
                    if (lineAddress <= currentAddress)
                    {
                        string line = lines[lineNumber];
                        if (LineIsAMessageLine(line))
                        {

                        }
                        else
                        {
                            //output a line
                            tw.Write("//");
                            tw.WriteLine(line);
                        }

                        //next line
                        lineNumber++;
                    }
                    else
                    {
                        nextLineAddress = lineAddress;
                        break;
                    }
                }

                //grab from disassembly
                if (nextLineAddress > endAddress)
                {
                    nextLineAddress = endAddress;
                }
                currentAddress = WriteFunction(currentAddress, nextLineAddress);
                //currentAddress = nextLineAddress;
                if (currentAddress >= endAddress)
                {
                    //display remaining lines
                    for (; lineNumber < lines.Length; lineNumber++)
                    {
                        tw.Write("//");
                        tw.WriteLine(lines[lineNumber]);
                    }
                    break;
                }
            }
        }

        private static bool LineIsAMessageLine(string line)
        {
            foreach (char c in line)
            {
                if (char.IsWhiteSpace(c))
                {

                }
                else
                {
                    if (c == '\'')
                    {
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }
            }
            return false;
        }

        //private void WriteFunctionOrder()
        //{
        //    return;

        //    List<int> functionNumbers = new List<int>();
        //    int address = 0;
        //    while (address < ainFile.Code.Length)
        //    {
        //        int codeFunctionNumber;
        //        var instructionInfo = decompiler.Peek(address);
        //        address = instructionInfo.nextAddress;

        //        if (instructionInfo.instruction == Instruction.FUNC)
        //        {
        //            codeFunctionNumber = instructionInfo.word1;
        //            functionNumbers.Add(codeFunctionNumber);
        //        }
        //    }

        //    HashSet<int> outOfOrder = new HashSet<int>();
        //    for (int i = 0; i < functionNumbers.Count; i++)
        //    {

        //    }

        //}

        private void MatchStrings()
        {
            if (Options.MergeDuplicateStrings)
            {
                return;
            }
            DuplicateStrings = CreateDuplicateDictionary(ainFile.Strings);
        }

        private void MatchMessages()
        {
            if (Options.MergeDuplicateStrings)
            {
                return;
            }
            DuplicateMessages = CreateDuplicateDictionary(ainFile.Messages);
        }

        private static Dictionary<string, Dictionary<int, int>> CreateDuplicateDictionary(IList<string> strings)
        {
            HashSet<string> seen = new HashSet<string>();
            Dictionary<string, Dictionary<int, int>> dupes = new Dictionary<string, Dictionary<int, int>>();

            for (int i = 0; i < strings.Count; i++)
            {
                string str = strings[i];
                if (seen.Contains(str))
                {
                    var dic = dupes.GetOrNew(str);
                    int instanceNumber = dic.Count + 1;
                    dic.Add(i, instanceNumber);
                    dupes.Set(str, dic);
                }
                else
                {
                    seen.Add(str);
                }
            }

            return dupes;
        }

        public void SaveAsProject(string path, bool singleFile)
        {
            this.ShowGlobalPrefix = Options.ShowGlobalPrefix;

            string fileName = Path.Combine(path, "main.jam");

            FileStream file = null;
            try
            {
                if (!singleFile)
                {
                    file = new FileStream(fileName, FileMode.Create, FileAccess.Write, FileShare.ReadWrite);
                }
                else
                {
                    file = new FileStream(path, FileMode.Create, FileAccess.Write, FileShare.ReadWrite);
                }
                var sw = new StreamWriter(file, Extensions.TextEncoding);
                OutputToStream(singleFile, sw, path);
            }
            finally
            {
                if (file != null)
                {
                    file.Dispose();
                }
            }
        }

        private void OutputToStream(bool singleFile, TextWriter sw, string path)
        {
            this.mainIndentedTextWriter = new MyIndentedTextWriter(sw, "\t");
            this.tw = mainIndentedTextWriter;

            tw.WriteLine("version = " + ainFile.Version);

            if ((ainFile.Version >= 6) != ainFile.IsAi2File)
            {
                tw.WriteLine("IsAi2File = " + ainFile.IsAi2File);
            }

            tw.WriteLine("keycode = 0x" + ainFile.KeyCode.ToString("X8"));
            tw.WriteLine("gameversion = " + ainFile.GameVersion);
            var mainFunc = ainFile.GetFunction(ainFile.MAIN);
            if (mainFunc != null)
            {
                tw.WriteLine("main = " + ainFile.Functions[ainFile.MAIN].Name);
            }
            else
            {
                tw.WriteLine("main = " + ainFile.MAIN.ToString());
            }
            var msgFunc = ainFile.GetFunction(ainFile.MSGF);
            if (msgFunc != null)
            {
                tw.WriteLine("msgf = " + ainFile.Functions[ainFile.MSGF].Name);
            }
            else
            {
                tw.WriteLine("msgf = " + ainFile.MSGF.ToString());
            }
            var ojmpFunc = ainFile.GetFunction(ainFile.OJMP);
            if (ojmpFunc != null)
            {
                tw.WriteLine("ojmp = " + ainFile.Functions[ainFile.OJMP].Name);
            }
            else
            {
                tw.WriteLine("ojmp = " + ainFile.OJMP.ToString());
            }

            MatchStrings();
            MatchMessages();

            FileStream file2;

            file2 = CreateAndIncludeFile(singleFile, "structs.jam", path);
            WriteStructs();
            FinishFile(singleFile, file2);

            file2 = CreateAndIncludeFile(singleFile, "functiontypes.jam", path);
            WriteFunctionTypes();
            FinishFile(singleFile, file2);

            if (!(ainFile.Delegates == null || ainFile.Delegates.Count == 0))
            {
                file2 = CreateAndIncludeFile(singleFile, "delegates.jam", path);
                WriteDelegates();
                FinishFile(singleFile, file2);
            }

            file2 = CreateAndIncludeFile(singleFile, "globals.jam", path);
            WriteGlobals();
            FinishFile(singleFile, file2);

            file2 = CreateAndIncludeFile(singleFile, "hll.jam", path);
            WriteHllLibraries();
            FinishFile(singleFile, file2);

            file2 = CreateAndIncludeFile(singleFile, "functions.jam", path);
            WriteFunctions();
            FinishFile(singleFile, file2);

            if (Options.ForceOriginalStringOrder)
            {
                file2 = CreateAndIncludeFile(singleFile, "strings.jam", path);
                WriteStrings();
                FinishFile(singleFile, file2);

                file2 = CreateAndIncludeFile(singleFile, "messages.jam", path);
                WriteMessages();
                FinishFile(singleFile, file2);
            }

            FindLabels();


            //WriteFunction(0, ainFile.Code.Length);

            var functionEnumerator = new FunctionEnumerator(ainFile);
            var filesAndFunctions = functionEnumerator.GetFilesAndFunctions();

            //list the real functions (so we can omit them if we are using the exclude orphans option
            HashSet<int> RealFunctionAddresses = new HashSet<int>();
            if (Options.RemoveOrphanFunctions)
            {
                foreach (var func in ainFile.Functions)
                {
                    RealFunctionAddresses.Set(func.Address);
                }
            }
            HashSet<int> FunctionsThatHaveBeenOutputted = new HashSet<int>();



            HashSet<string> usedFileNames = new HashSet<string>();

            int lastAddress = 0;

            foreach (var file in filesAndFunctions)
            {
                if (!singleFile)
                {
                    string fileName;
                    {
                        fileName = file.name;
                        string directoryName = Path.GetDirectoryName(fileName);
                        string baseName = Path.GetFileNameWithoutExtension(fileName);
                        int suffix = 0;
                        fileName = Path.Combine(directoryName, baseName + ".jam");
                        while (usedFileNames.Contains(fileName))
                        {
                            suffix++;
                            fileName = Path.Combine(directoryName, baseName + "-" + suffix.ToString() + ".jam");
                        }
                        usedFileNames.Add(fileName);
                    }
                    file2 = CreateAndIncludeFile(singleFile, fileName, path);
                }

                if (lastAddress < file.address)
                {
                    //Not executed, but here just in case
                    WriteFunction(lastAddress, file.address);
                    lastAddress = file.address;
                }

                if (file.children.Count > 0)
                {
                    foreach (var function in file.children)
                    {
                        if (lastAddress < function.address)
                        {
                            //Not executed, but here just in case
                            WriteFunction(lastAddress, function.address);
                            lastAddress = function.address;
                        }

                        if (lastAddress < function.lastAddress)
                        {
                            bool wantToOutputFunction = true;
                            int functionAddress = lastAddress + 6;
                            if (Options.RemoveOrphanFunctions)
                            {
                                if (FunctionsThatHaveBeenOutputted.Contains(function.id))
                                {
                                    wantToOutputFunction = false;
                                }
                                else
                                {
                                    //if this is a leftover function, replace with the real function.
                                    if (!RealFunctionAddresses.Contains(functionAddress))
                                    {
                                        //output the real function
                                        int realAddress = ainFile.Functions[function.id].Address;
                                        int realEndAddress = decompiler.FindEndAddress(realAddress);
                                        WriteFunction(realAddress - 6, realEndAddress);
                                        FunctionsThatHaveBeenOutputted.Set(function.id);
                                        wantToOutputFunction = false;
                                    }
                                }
                            }

                            if (stopwatch.ElapsedTime > 250 && worker != null)
                            {
                                if (Cancel)
                                {
                                    return;
                                }
                                stopwatch.Start();
                                int progress = (int)((long)functionAddress * 100L / (long)ainFile.Code.Length);
                                worker.ReportProgress(progress);
                            }

                            if (wantToOutputFunction)
                            {
                                FunctionsThatHaveBeenOutputted.Set(function.id);
                                //report progress
                                if (Options.AnnotateWithDecompiledCode)
                                {
                                    int endAddress = decompiler.FindEndAddress(functionAddress);
                                    WriteCombinedFunction(functionAddress);
                                    if (endAddress < function.lastAddress)
                                    {
                                        //Not executed, but here just in case
                                        WriteFunction(endAddress, function.lastAddress);
                                    }
                                }
                                else
                                {
                                    WriteFunction(lastAddress, function.lastAddress);
                                }
                            }
                            else
                            {
                                //Output any EOFs contained in the function
                                int address = lastAddress;
                                while (address < function.lastAddress)
                                {
                                    var instructionInfo = Decompiler.Peek(ainFile.Code, address);
                                    if (instructionInfo.instruction == Instruction.EOF)
                                    {
                                        WriteFunction(address, address + 6);
                                    }
                                    address = instructionInfo.nextAddress;
                                }
                            }
                            lastAddress = function.lastAddress;
                        }
                    }
                }
                if (lastAddress < file.lastAddress)
                {
                    //This is for the trailing information in the file (EOF instruction)
                    WriteFunction(lastAddress, file.lastAddress);
                    lastAddress = file.lastAddress;
                }

                FinishFile(singleFile, file2);
            }
            if (lastAddress < ainFile.Code.Length)
            {
                //For trailing ENDFUNCs at the end of the file
                WriteFunction(lastAddress, ainFile.Code.Length);
                lastAddress = ainFile.Code.Length;
            }

            tw.Flush();
        }

        private void FinishFile(bool singleFile, FileStream file2)
        {
            if (!singleFile)
            {
                this.tw.Flush();
                this.tw.Close();
                file2.Dispose();
                this.tw = this.mainIndentedTextWriter;
            }
        }

        private FileStream CreateAndIncludeFile(bool singleFile, string newFileName, string path)
        {
            FileStream file2 = null;
            if (!singleFile)
            {
                string newDirectory = Path.Combine(path, Path.GetDirectoryName(newFileName));

                if (!Directory.Exists(newDirectory))
                {
                    Directory.CreateDirectory(newDirectory);
                }

                tw.WriteLine("#include \"" + EscapeString(newFileName) + "\"");
                tw.Flush();
                file2 = new FileStream(Path.Combine(path, newFileName), FileMode.Create, FileAccess.Write, FileShare.ReadWrite);
                var newStreamWriter = new StreamWriter(file2, Extensions.TextEncoding);
                this.tw = new MyIndentedTextWriter(newStreamWriter);
            }
            return file2;
        }

        private void WriteFunctions()
        {
            tw.WriteLine("functions");
            tw.WriteLine("{");
            tw.Indent++;
            for (int i = 0; i < ainFile.Functions.Count; i++)
            {
                var function = ainFile.Functions[i];
                WriteFunctionHeader(i, false);
                tw.WriteLine();
                string labelName = GetFunctionLabelName(function);
                labels.Set(function.Address, labelName);
                tw.WriteLine(labelName + ", 0x" + function.Crc.ToString("X8") + ";");
            }
            tw.Indent--;
            tw.WriteLine("}");
        }

        private string GetFunctionLabelName(Function function)
        {
            string labelName = "_" + SafeCharacters(function.Name); // + "_" + function.address.ToString("X");
            return labelName;
        }

        private string SafeCharacters(string name)
        {
            StringBuilder sb = new StringBuilder();
            foreach (char c in name)
            {
                if (c >= 0 && c < 128)
                {
                    if ((c >= 'A' && c <= 'Z') ||
                        (c >= 'a' && c <= 'z') ||
                        (c >= '0' && c <= '9') ||
                        (c == '@' || c == '_'))
                    {
                        sb.Append(c);
                    }
                }
                else
                {
                    sb.Append(c);
                }
            }
            return sb.ToString();
        }

        Dictionary<int, string> labels = new Dictionary<int, string>();
        Dictionary<string, int> labelsInverse = new Dictionary<string, int>();

        void FindLabels()
        {
            FindLabels(0, ainFile.Code.Length);
        }

        void FindLabels(int startAddress, int endAddress)
        {
            int address = startAddress;
            int localSwitchBlockNumber = 0;
            string functionName = "";
            HashSet<int> localLabels = new HashSet<int>();
            Action finishLabels = () =>
            {
                int labelNumber = 0;
                foreach (int labelAddress in localLabels.OrderBy(v => v))
                {
                    string labelName = "_" + functionName + "_label" + labelNumber.ToString("X");
                    AddLabel(labelAddress, labelName);
                    labelNumber++;
                }
                localLabels.Clear();
            };


            while (address < endAddress)
            {
                var instructionInfo = Decompiler.Peek(ainFile.Code, address);
                int currentAddress = address;
                int word1 = instructionInfo.word1;
                address = instructionInfo.nextAddress;

                switch (instructionInfo.instruction)
                {
                    case Instruction.FUNC:
                        {
                            finishLabels();
                            var function = ainFile.GetFunction(word1);
                            if (function != null)
                            {
                                functionName = SafeCharacters(function.Name);
                            }
                            //functionName = SafeCharacters(ainFile.Functions[word1].Name);
                            localSwitchBlockNumber = 0;
                        }
                        break;
                    default:
                        if (ArgumentKinds.BranchInstructions.ContainsKey((int)instructionInfo.instruction))
                        {
                            int wordNumber = ArgumentKinds.BranchInstructions[(int)instructionInfo.instruction];
                            int word = instructionInfo.words[wordNumber];
                            if (!labels.ContainsKey(word))
                            {
                                localLabels.Set(word);
                            }
                        }
                        break;
                    case Instruction.IFNZ:
                    case Instruction.IFZ:
                    case Instruction.JUMP:
                        if (!labels.ContainsKey(word1))
                        {
                            localLabels.Set(word1);
                        }
                        break;
                    case Instruction.SWITCH:
                    case Instruction.STRSWITCH:
                        {
                            var switchBlock = ainFile.Switches[word1];
                            string switchLabelPrefix = "_" + functionName +
                                ((localSwitchBlockNumber == 0) ? "" :
                                ("_" + localSwitchBlockNumber.ToString("X"))
                                ) + "_case_";
                            foreach (var switchCase in switchBlock.SwitchCases)
                            {
                                int labelAddress = switchCase.TargetAddress;
                                int caseNumber = switchCase.Value;

                                string labelName;
                                if (instructionInfo.instruction == Instruction.STRSWITCH)
                                {
                                    if (caseNumber >= 0 && caseNumber < ainFile.Strings.Count)
                                    {
                                        labelName = switchLabelPrefix + SafeCharacters(ainFile.Strings[caseNumber]);
                                    }
                                    else
                                    {
                                        labelName = switchLabelPrefix + caseNumber.ToString("X");
                                    }
                                }
                                else
                                {
                                    labelName = switchLabelPrefix + caseNumber.ToString();
                                }

                                AddLabel(labelAddress, labelName);
                            }
                            {
                                string defaultCaseLabelName = "_" + functionName +
                                    ((localSwitchBlockNumber == 0) ? "" :
                                    ("_" + localSwitchBlockNumber.ToString("X"))
                                    ) + "_default";
                                int labelAddress = switchBlock.DefaultCaseAddress;
                                if (labelAddress != -1)
                                {
                                    AddLabel(labelAddress, defaultCaseLabelName);
                                }
                            }

                            localSwitchBlockNumber++;
                        }
                        break;
                }
            }
            finishLabels();

        }

        private void AddLabel(int labelAddress, string labelName)
        {
            if (!labels.ContainsKey(labelAddress))
            {
                labelName = FirstFreeLabelName(labelName);
                labels.Add(labelAddress, labelName);
                labelsInverse.Add(labelName, labelAddress);
            }
        }

        private string FirstFreeLabelName(string labelName)
        {
            if (labelsInverse.ContainsKey(labelName))
            {
                string labelPrefix = labelName;
                int labelPostfix = 1;
                do
                {
                    labelPostfix++;
                    labelName = labelPrefix + "_" + labelPostfix;
                } while (labelsInverse.ContainsKey(labelName));
            }
            return labelName;
        }

        HashSet<int> duplicatedLocalVariableNames = new HashSet<int>();

        int currentFunctionNumber = -1;
        int currentClassNumber = -1;


        public event EventHandler<InstructionInfoEventArgs> BeforeWritingInstruction;
        public event EventHandler<InstructionInfoEventArgs> BeforeWritingString;

        void WriteText(string text)
        {
            tw.Write(text);
        }

        void NewLine()
        {
            tw.WriteLine();
        }


        private int WriteFunction(int startAddress, int endAddress)
        {
            int address = startAddress;
            while (address < endAddress)
            {
                if (!this.StopEmittingCode)
                {
                    if (labels.ContainsKey(address))
                    {
                        var labelName = labels[address];
                        tw.WriteLine(labelName + ":");
                    }
                }
                tw.Indent++;

                var instructionInfo = Decompiler.Peek(ainFile.Code, address);
                var instruction = instructionInfo.instruction;
                int totalArguments = instructionInfo.totalArguments;
                int word1 = instructionInfo.word1;
                int word2 = instructionInfo.word2;
                int word3 = instructionInfo.word3;
                int nextAddress = instructionInfo.nextAddress;

                int nextInstructionAddress = nextAddress;
                if (nextInstructionAddress >= endAddress)
                {
                    nextInstructionAddress = address;
                }

                var nextInstructionInfo = Decompiler.Peek(ainFile.Code, nextInstructionAddress);
                var nextInstruction = nextInstructionInfo.instruction;
                int nextTotalArguments = nextInstructionInfo.totalArguments;
                int nextWord1 = nextInstructionInfo.word1;
                int nextWord2 = nextInstructionInfo.word2;
                int nextWord3 = nextInstructionInfo.word3;
                nextInstructionAddress = nextInstructionInfo.nextAddress;

                {
                    bool dirty, handled;
                    string stringValue = null;
                    OnBeforeWritingInstruction(instructionInfo, ref stringValue, out dirty, out handled, ref StopEmittingCode);
                    if (handled || StopEmittingCode)
                    {
                        goto SkipInstruction;
                    }
                }

                switch (instruction)
                {
                    default:
                        {
                            ProcessGenericInstruction(instructionInfo);
                        }
                        break;
                    case Instruction.FUNC:
                        {
                            int functionNumber = word1;
                            currentFunctionNumber = functionNumber;
                            currentClassNumber = -1;
                            Function function = null;
                            if (functionNumber >= 0 && functionNumber < ainFile.Functions.Count)
                            {
                                function = ainFile.Functions[functionNumber];
                            }

                            if (function != null)
                            {
                                //set currentClassNumber
                                string functionName = function.Name;
                                int atPosition = functionName.IndexOf('@');
                                if (atPosition == -1) atPosition = functionName.Length;
                                string className = functionName.Substring(0, atPosition);
                                if (ainFile.StructNameToIndex.ContainsKey(className))
                                {
                                    currentClassNumber = ainFile.StructNameToIndex[className];
                                }

                                Dictionary<string, int> seenVariableNames = new Dictionary<string, int>();
                                duplicatedLocalVariableNames.Clear();
                                for (int i = 0; i < function.Parameters.Count; i++)
                                {
                                    var variable = function.Parameters[i];
                                    string variableName = variable.Name;
                                    if (seenVariableNames.ContainsKey(variableName))
                                    {
                                        int i2 = seenVariableNames[variableName];
                                        duplicatedLocalVariableNames.Add(i);
                                        duplicatedLocalVariableNames.Add(i2);
                                    }
                                    else
                                    {
                                        seenVariableNames.Add(variableName, i);
                                    }
                                }
                            }
                            ProcessGenericInstruction(instructionInfo);
                            if (function != null)
                            {
                                tw.Write("\t// " + function.ToString());
                            }
                        }
                        break;
                    case Instruction.SWITCH:
                    case Instruction.STRSWITCH:
                        {
                            string instructionString = Decompiler.InstructionToString[(int)instruction];

                            tw.Write(instructionString);
                            tw.WriteLine();

                            var switchBlock = ainFile.Switches[word1];
                            int target = switchBlock.DefaultCaseAddress;
                            string label;
                            if (target != -1)
                            {
                                label = "0x" + target.ToString("X");
                                if (labels.ContainsKey(target))
                                {
                                    label = labels[target];
                                }
                                tw.Write("default: " + label);
                            }
                            foreach (var switchCase in switchBlock.SwitchCases)
                            {
                                tw.WriteLine();
                                target = switchCase.TargetAddress;
                                label = "0x" + target.ToString("X");
                                if (labels.ContainsKey(target))
                                {
                                    label = labels[target];
                                }

                                int caseValue = switchCase.Value;
                                string caseString;

                                if (instruction == Instruction.STRSWITCH)
                                {
                                    if (caseValue >= 0 && caseValue < ainFile.Strings.Count)
                                    {
                                        string stringValue = ainFile.GetString(caseValue);
                                        bool dirty;
                                        dirty = OnBeforeWritingString(instructionInfo, ref stringValue);

                                        caseString = "";
                                        if (!dirty)
                                        {
                                            int dupeNumber = GetStringDupeNumber(caseValue);
                                            if (dupeNumber != -1)
                                            {
                                                caseString = dupeNumber.ToString() + " ";
                                            }
                                        }
                                        caseString += "\"" + EscapeString(ainFile.Strings[caseValue]) + "\"";
                                    }
                                    else
                                    {
                                        caseString = "0x" + caseValue.ToString("X");
                                    }
                                }
                                else
                                {
                                    caseString = caseValue.ToString();
                                }

                                tw.Write("case " + caseString + ": " + label);
                            }
                        }
                        break;
                    case Instruction.SR_REF:
                    case Instruction.SR_REF2:
                    case Instruction.SR_REFREF:
                    case Instruction.SH_STRUCT_SR_REF:
                        {
                            //add any other instructions that include a struct number here
                            if (nextInstruction == Instruction.PUSH)
                            {
                                ProcessGenericInstruction(instructionInfo);
                                tw.WriteLine();

                                this.classNumber = this.lastSeenStructType;
                                ArgumentKind argumentKind = ArgumentKind.Member;
                                ProcessGenericInstructionOverride(nextInstructionInfo, argumentKind);

                                nextAddress = nextInstructionInfo.nextAddress;
                            }
                            else
                            {
                                ProcessGenericInstruction(instructionInfo);
                            }
                        }
                        break;
                    case Instruction.PUSHLOCALPAGE:
                    case Instruction.PUSHGLOBALPAGE:
                    case Instruction.PUSHSTRUCTPAGE:
                        {
                            if (nextInstruction == Instruction.PUSH &&
                                !(instruction == Instruction.PUSHSTRUCTPAGE && Decompiler.Peek(ainFile.Code, nextInstructionInfo.nextAddress).instruction == Instruction.DG_NEW_FROM_METHOD))
                            {
                                ProcessGenericInstruction(instructionInfo);
                                tw.WriteLine();
                                ArgumentKind argumentKind = ArgumentKind.Local;
                                if (instruction == Instruction.PUSHGLOBALPAGE) argumentKind = ArgumentKind.Global;
                                if (instruction == Instruction.PUSHSTRUCTPAGE) argumentKind = ArgumentKind.LocalMember;

                                ProcessGenericInstructionOverride(nextInstructionInfo, argumentKind);
                                nextAddress = nextInstructionInfo.nextAddress;
                            }
                            else
                            {
                                ProcessGenericInstruction(instructionInfo);
                            }
                        }
                        break;
                    case Instruction.MSG:
                        {
                            int messageNumber = word1;
                            if (messageNumber >= 0 && messageNumber < ainFile.Messages.Count)
                            {
                                int dupeNumber = GetMessageDupeNumber(messageNumber);
                                string messageText = ainFile.Messages[messageNumber];
                                if (dupeNumber != -1)
                                {
                                    tw.Write(dupeNumber.ToString() + " ");
                                }
                                tw.Write("'" + EscapeMessage(ainFile.Messages[messageNumber]) + "'");

                                if (nextInstruction == Instruction.CALLFUNC)
                                {
                                    bool handled = false;
                                    if (BeforeWritingInstruction != null)
                                    {
                                        var eventArgs = new InstructionInfoEventArgs(nextInstructionInfo, this.WriteText, this.NewLine);
                                        BeforeWritingInstruction(this, eventArgs);
                                        if (eventArgs.Handled)
                                        {
                                            handled = true;
                                        }
                                    }

                                    if (!handled)
                                    {
                                        if (nextWord1 >= 0 && nextWord1 < ainFile.Functions.Count)
                                        {
                                            var function = ainFile.Functions[nextWord1];
                                            if (function.ParameterCount == 0)
                                            {
                                                tw.Write(" " + SanitizeVariableName(function.Name));
                                                nextAddress = nextInstructionAddress;
                                            }
                                        }
                                    }
                                }
                            }
                            else
                            {
                                tw.Write("MSG 0x" + messageNumber.ToString("X") + "\t//");
                                tw.Write("Message number out of range");
                            }
                        }
                        break;
                }

            SkipInstruction:
                ;

                //ProcessGenericInstruction(instructionInfo);
                ////else
                //{



                //    switch (instruction)
                //    {
                //        case Instruction.EOF:
                //            tw.Write(instructionString2);
                //            tw.Write("\"" + EscapeString(ainFile.Filenames[word1]) + "\"");
                //            break;
                //        case Instruction.FUNC:
                //            {
                //                tw.Indent--;
                //                tw.Write(instructionString2);
                //                tw.Indent++;

                //                int functionNumber = word1;
                //                currentFunctionNumber = functionNumber;
                //                Function function = null;
                //                if (functionNumber >= 0 && functionNumber < ainFile.Functions.Count)
                //                {
                //                    function = ainFile.Functions[functionNumber];
                //                }

                //                Dictionary<string, int> seenVariableNames = new Dictionary<string, int>();
                //                duplicatedLocalVariableNames.Clear();
                //                for (int i = 0; i < function.Arguments.Length; i++)
                //                {
                //                    var variable = function.Arguments[i];
                //                    string variableName = variable.Name;
                //                    if (seenVariableNames.ContainsKey(variableName))
                //                    {
                //                        int i2 = seenVariableNames[variableName];
                //                        duplicatedLocalVariableNames.Add(i);
                //                        duplicatedLocalVariableNames.Add(i2);
                //                    }
                //                    else
                //                    {
                //                        seenVariableNames.Add(variableName, i);
                //                    }
                //                }

                //                string functionName = function.Name;
                //                string functionDisplayName = functionName;
                //                var token = new Token(functionDisplayName);
                //                if (token.IsNumber())
                //                {
                //                    functionDisplayName = "\"" + functionDisplayName + "\"";
                //                }


                //                tw.Write(" " + functionDisplayName + "\t");
                //                //tw.Write(" 0x" + functionNumber.ToString("X"));
                //                //tw.Write("\t");

                //                if (function != null && ainFile.AlternativeNames.ContainsKey(functionName))
                //                {
                //                    tw.Write("// " + ainFile.AlternativeNames[functionName]);
                //                }

                //                WriteFunctionHeader(functionNumber, true);
                //            }
                //            break;
                //        case Instruction.JUMP:
                //        case Instruction.IFZ:
                //        case Instruction.IFNZ:
                //            {
                //                tw.Write(instructionString2);
                //                string label = null;
                //                if (labels.ContainsKey(word1))
                //                {
                //                    label = labels[word1];
                //                    tw.Write(label);
                //                }
                //                else
                //                {
                //                    tw.Write("0x" + word1);
                //                }
                //            }
                //            break;
                //        case Instruction.SWITCH:
                //            {
                //                tw.Write(instructionString);
                //                tw.WriteLine();

                //                var switchBlock = ainFile.Switches[word1];
                //                int target = switchBlock.DefaultCaseAddress;
                //                string label;
                //                if (target != -1)
                //                {
                //                    label = "0x" + target.ToString("X");
                //                    if (labels.ContainsKey(target))
                //                    {
                //                        label = labels[target];
                //                    }
                //                    tw.Write("default: " + label);
                //                }
                //                foreach (var switchCase in switchBlock.SwitchCases)
                //                {
                //                    tw.WriteLine();
                //                    target = switchCase.TargetAddress;
                //                    label = "0x" + target.ToString("X");
                //                    if (labels.ContainsKey(target))
                //                    {
                //                        label = labels[target];
                //                    }
                //                    tw.Write("case " + switchCase.Value + ": " + label);
                //                }
                //            }
                //            break;
                //        case Instruction.CALLMETHOD:
                //        case Instruction.CALLFUNC:
                //        case Instruction.CALLSYS:
                //            {
                //                tw.Write(instructionString2);
                //                Function function = null;

                //                if (instruction == Instruction.CALLSYS)
                //                {
                //                    if (word1 < AinFile.SystemCalls.Length)
                //                    {
                //                        function = AinFile.SystemCalls[word1];
                //                    }
                //                }
                //                else
                //                {
                //                    if (word1 < ainFile.Functions.Count)
                //                    {
                //                        function = ainFile.Functions[word1];
                //                    }
                //                }
                //                if (function == null)
                //                {
                //                    tw.Write(word1);
                //                }
                //                else
                //                {
                //                    string displayedFunctionName = function.Name;
                //                    if (Token.IsNumber(displayedFunctionName))
                //                    {
                //                        displayedFunctionName = "\"" + displayedFunctionName + "\"";
                //                    }

                //                    tw.Write(displayedFunctionName);

                //                    if (ainFile.AlternativeNames.ContainsKey(function.Name))
                //                    {
                //                        tw.Write("\t//" + ainFile.AlternativeNames[function.Name]);
                //                    }

                //                    if (function.ArgumentCount > 0)
                //                    {
                //                        tw.Write("\t\t\t\t// ");
                //                        bool needComma = false;
                //                        for (int i = 0; i < function.ArgumentCount; i++)
                //                        {
                //                            Util.PrintComma(tw, ref needComma);
                //                            var arg = function.Arguments[i];
                //                            tw.Write(arg.Name);

                //                            if (ainFile.AlternativeNames.ContainsKey(arg.Name))
                //                            {
                //                                tw.Write(" (" + ainFile.AlternativeNames[arg.Name] + ")");
                //                            }
                //                        }
                //                    }
                //                }
                //            }
                //            break;
                //        case Instruction.CALLHLL:
                //            {
                //                tw.Write(instructionString2);
                //                var library = ainFile.Libraries[word1];
                //                var function = library.Functions[word2];

                //                tw.Write(library.LibraryName + "." + function.Name);
                //                if (function.ArgumentCount > 0)
                //                {
                //                    tw.Write("\t\t\t\t// ");
                //                    bool needComma = false;
                //                    for (int i = 0; i < function.ArgumentCount; i++)
                //                    {
                //                        Util.PrintComma(tw, ref needComma);
                //                        var arg = function.Arguments[i];
                //                        tw.Write(arg.Name);
                //                    }
                //                }

                //            }
                //            break;
                //        case Instruction.F_PUSH:
                //            {
                //                tw.Write(instructionString2);
                //                float floatValue = BitConverter.ToSingle(BitConverter.GetBytes(word1), 0);
                //                tw.Write(floatValue.FloatToString());
                //            }
                //            break;
                //        case Instruction.S_PUSH:
                //            {
                //                string str = ainFile.Strings[word1];
                //                int dupeNumber = -1;
                //                if (DuplicateStrings.ContainsKey(str))
                //                {
                //                    var dic = DuplicateStrings[str];
                //                    if (dic.ContainsKey(word1))
                //                    {
                //                        dupeNumber = dic[word1];
                //                    }
                //                }

                //                tw.Write(instructionString2);
                //                if (dupeNumber != -1)
                //                {
                //                    tw.Write(dupeNumber.ToString() + " ");
                //                }
                //                tw.Write("\"" + EscapeString(str) + "\"");
                //                if (ainFile.AlternativeNames.ContainsKey(str))
                //                {
                //                    tw.Write("\t//" + ainFile.AlternativeNames[str]);
                //                }
                //            }
                //            break;
                //        case Instruction.PUSH:
                //            {
                //                tw.Write(instructionString2);
                //                tw.Write(word1);
                //            }
                //            break;
                //        case Instruction.PUSHGLOBALPAGE:
                //        case Instruction.PUSHLOCALPAGE:
                //        case Instruction.PUSHSTRUCTPAGE:
                //        case Instruction.SH_GLOBALREF:
                //        case Instruction.SH_LOCALASSIGN:
                //        case Instruction.SH_LOCALCREATE:
                //        case Instruction.SH_LOCALDEC:
                //        case Instruction.SH_LOCALDELETE:
                //        case Instruction.SH_LOCALINC:
                //        case Instruction.SH_LOCALREF:
                //        case Instruction.SH_STRUCTREF:
                //            {
                //                bool handled = false;
                //                bool skipNext = false;
                //                bool isLocal = false;

                //                tw.Write(instructionString);
                //                int value = 0;

                //                if (instruction == Instruction.PUSHGLOBALPAGE || instruction == Instruction.PUSHLOCALPAGE || instruction == Instruction.PUSHSTRUCTPAGE)
                //                {
                //                    if (nextInstruction == Instruction.PUSH)
                //                    {
                //                        value = nextWord1;
                //                        handled = true;
                //                        skipNext = true;
                //                    }
                //                }
                //                else
                //                {
                //                    handled = true;
                //                    value = word1;
                //                }

                //                if (!handled)
                //                {
                //                    if (totalArguments > 0)
                //                    {
                //                        tw.Write(" " + word1);
                //                    }
                //                    break;
                //                }

                //                IVariable variable = null;
                //                if (instruction == Instruction.PUSHLOCALPAGE || instruction == Instruction.SH_LOCALASSIGN || instruction == Instruction.SH_LOCALCREATE ||
                //                    instruction == Instruction.SH_LOCALDEC || instruction == Instruction.SH_LOCALDELETE || instruction == Instruction.SH_LOCALINC ||
                //                    instruction == Instruction.SH_LOCALREF)
                //                {
                //                    var currentFunction = ainFile.Functions[currentFunctionNumber];
                //                    if (value < currentFunction.Arguments.Length)
                //                    {
                //                        variable = currentFunction.Arguments[value];
                //                        isLocal = true;
                //                    }
                //                }
                //                else if (instruction == Instruction.PUSHGLOBALPAGE || instruction == Instruction.SH_GLOBALREF)
                //                {
                //                    variable = ainFile.Globals[value];
                //                }
                //                else if (instruction == Instruction.PUSHSTRUCTPAGE || instruction == Instruction.SH_STRUCTREF)
                //                {
                //                    var currentFunction = ainFile.Functions[currentFunctionNumber];
                //                    string currentFunctionName = currentFunction.Name;
                //                    string currentClassName = currentFunction.Name.Split('@')[0];
                //                    if (ainFile.StructNameToIndex.ContainsKey(currentClassName))
                //                    {
                //                        var structInfo = ainFile.Structs[ainFile.StructNameToIndex[currentClassName]];
                //                        if (value >= 0 && value < structInfo.Members.Count)
                //                        {
                //                            variable = structInfo.Members[value];
                //                        }
                //                    }
                //                    else
                //                    {

                //                    }
                //                }

                //                if (skipNext)
                //                {
                //                    tw.WriteLine();
                //                    tw.Write(Decompiler.InstructionToString[(int)nextInstruction] + " ");
                //                    nextAddress = nextInstructionAddress;
                //                }
                //                else
                //                {
                //                    if (totalArguments > 0)
                //                    {
                //                        tw.Write(" ");
                //                    }
                //                }

                //                if (variable != null)
                //                {
                //                    bool duplicated = duplicatedLocalVariableNames.Contains(value);
                //                    //writing the number instead of the word because of duplicates
                //                    if (isLocal && duplicated)
                //                    {
                //                        tw.Write("0x" + value.ToString("X"));
                //                        if (instruction == Instruction.SH_LOCALASSIGN || instruction == Instruction.SH_LOCALCREATE)
                //                        {
                //                            tw.Write(" = " + word2);
                //                        }
                //                        tw.Write("\t//");
                //                    }
                //                    if (variable.GroupIndex != -1)
                //                    {
                //                        tw.Write(ainFile.GlobalGroupNames[variable.GroupIndex] + ".");
                //                    }
                //                    if (variable.Name.Contains(' ') || Token.IsNumber(variable.Name))
                //                    {
                //                        tw.Write("\"" + EscapeString(variable.Name) + "\"");
                //                    }
                //                    else
                //                    {
                //                        tw.Write(variable.Name);
                //                    }
                //                    if (!duplicated && (instruction == Instruction.SH_LOCALASSIGN || instruction == Instruction.SH_LOCALCREATE))
                //                    {
                //                        tw.Write(" = " + word2);
                //                    }

                //                    if (ainFile.AlternativeNames.ContainsKey(variable.Name))
                //                    {
                //                        tw.Write("\t//" + ainFile.AlternativeNames[variable.Name]);
                //                    }

                //                }
                //                else
                //                {
                //                    tw.Write(value);
                //                }
                //            }
                //            break;
                //        case Instruction.ENDFUNC:
                //            tw.Write(instructionString + " 0x" + word1.ToString("X"));
                //            break;
                //        case Instruction.MSG:
                //            {
                //                string str = ainFile.Messages[word1];
                //                int dupeNumber = -1;
                //                if (DuplicateMessages.ContainsKey(str))
                //                {
                //                    var dic = DuplicateMessages[str];
                //                    if (dic.ContainsKey(word1))
                //                    {
                //                        dupeNumber = dic[word1];
                //                    }
                //                }

                //                if (dupeNumber != -1)
                //                {
                //                    tw.Write(dupeNumber.ToString() + " ");
                //                }

                //                tw.Write("'" + EscapeMessage(ainFile.Messages[word1]) + "'");

                //                if (nextInstruction == Instruction.CALLFUNC)
                //                {
                //                    var function = ainFile.Functions[nextWord1];
                //                    if (function.ArgumentCount == 0)
                //                    {
                //                        tw.Write(" " + function.Name);
                //                        nextAddress = nextInstructionAddress;
                //                    }
                //                }
                //            }

                //            break;
                //        default:
                //            tw.Write(instructionString);
                //            if (totalArguments >= 1)
                //            {
                //                tw.Write(" 0x" + word1.ToString("X"));
                //            }
                //            if (totalArguments >= 2)
                //            {
                //                tw.Write(", 0x" + word2.ToString("X"));
                //            }
                //            if (totalArguments >= 3)
                //            {
                //                tw.Write(", 0x" + word3.ToString("X"));
                //            }
                //            break;
                //    }
                //}
                tw.WriteLine();
                tw.Indent--;
                address = nextAddress;
            }
            return address;
        }

        private bool OnBeforeWritingString(InstructionInfo instructionInfo, ref string stringValue)
        {
            bool dirty = false;
            if (this.BeforeWritingString != null)
            {
                var e = new InstructionInfoEventArgs(instructionInfo, stringValue);
                this.BeforeWritingString(this, e);
                dirty = e.Dirty;
                if (e.Dirty)
                {
                    stringValue = e.Text;
                }
                //handled = e.Handled;
            }
            return dirty;
        }

        private void OnBeforeWritingInstruction(InstructionInfo instructionInfo, ref string stringValue, out bool dirty, out bool handled, ref bool StopEmittingCode)
        {
            dirty = false;
            handled = false;
            if (this.BeforeWritingInstruction != null)
            {
                if (instructionInfo.instruction == Instruction.MSG)
                {
                    stringValue = ainFile.GetMessage(instructionInfo.word1);
                }
                else
                {
                    int indexOfStringArgument = instructionInfo.instruction.IndexOfStringArgument();
                    if (indexOfStringArgument >= 0)
                    {
                        stringValue = ainFile.GetString(instructionInfo.words[indexOfStringArgument]);
                    }
                }
                var e = new InstructionInfoEventArgs(instructionInfo, WriteText, NewLine, stringValue, StopEmittingCode);
                this.BeforeWritingInstruction(this, e);
                dirty = e.Dirty;
                handled = e.Handled;
                StopEmittingCode = e.StopEmittingCode;
                if (dirty)
                {
                    stringValue = e.Text;
                }
            }
        }


        private void ProcessGenericInstructionOverride(InstructionInfo instructionInfo, ArgumentKind argumentKind)
        {
            StringBuilder commentText = new StringBuilder();
            StringBuilder instructionText = new StringBuilder();

            var argumentKinds = new ArgumentKind[] { argumentKind };
            ProcessInstructionWithArgumentKinds(instructionInfo, commentText, instructionText, argumentKinds);

            PrintInstruction(instructionInfo, instructionText.ToString(), commentText.ToString());
        }

        private void ProcessGenericInstruction(InstructionInfo instructionInfo)
        {
            StringBuilder commentText = new StringBuilder();
            StringBuilder instructionText = new StringBuilder();

            if (ArgumentKinds.InstructionArgumentKinds.ContainsKey((int)instructionInfo.instruction))
            {
                var argumentKinds = ArgumentKinds.InstructionArgumentKinds[(int)instructionInfo.instruction];
                ProcessInstructionWithArgumentKinds(instructionInfo, commentText, instructionText, argumentKinds);
            }
            else if (instructionInfo.totalArguments > 0)
            {
                //shouldn't happen, but spit out each argument as a hex number
                for (int i = 0; i < instructionInfo.totalArguments; i++)
                {
                    if (instructionText.Length > 0)
                    {
                        instructionText.Append(" ");
                    }
                    instructionText.AppendHex(instructionInfo.words[i]);
                }
            }

            PrintInstruction(instructionInfo, instructionText.ToString(), commentText.ToString());
        }

        private void PrintInstruction(InstructionInfo instructionInfo, string instructionText, string commentText)
        {
            //write the instruction
            if (instructionInfo != null)
            {
                if (Decompiler.InstructionToString.ContainsKey((int)instructionInfo.instruction))
                {
                    tw.Write(Decompiler.InstructionToString[(int)instructionInfo.instruction]);
                }
                else
                {
                    tw.Write("0x" + ((int)instructionInfo.instruction).ToString("X"));
                }
            }

            if (instructionText.Length > 0)
            {
                tw.Write(" ");
                tw.Write(instructionText);
            }
            if (commentText.Length > 0)
            {
                tw.Write("\t//");
                tw.Write(commentText);
            }
        }

        static readonly ArgumentKind[] argumentKindsForLocalVariable = new ArgumentKind[] { ArgumentKind.Local };
        static readonly ArgumentKind[] argumentKindsForGlobalVariable = new ArgumentKind[] { ArgumentKind.Global };
        static readonly ArgumentKind[] argumentKindsForStructMember = new ArgumentKind[] { ArgumentKind.Member };

        int classNumber = -1;

        private void ProcessInstructionWithArgumentKinds(InstructionInfo instructionInfo, StringBuilder commentText, StringBuilder instructionText, ArgumentKind[] argumentKinds)
        {
            int libraryNumber = instructionInfo.word1;

            for (int wordNumber = 0; wordNumber < argumentKinds.Length; wordNumber++)
            {
                if (instructionText.Length > 0)
                {
                    instructionText.Append(" ");
                }
                if (commentText.Length > 0)
                {
                    commentText.Append("\t");
                }

                int word = instructionInfo.words[wordNumber];
                var argumentKind = argumentKinds[wordNumber];
                switch (argumentKind)
                {
                    case ArgumentKind.Int:
                        {
                            instructionText.Append(word);
                            //commentText.AppendHex(word);
                        }
                        break;
                    case ArgumentKind.AssignInt:
                        {
                            instructionText.Append("= " + word.ToString());
                            //commentText.AppendHex(word);
                        }
                        break;
                    case ArgumentKind.CodeAddress:
                        {
                            string label = null;
                            if (labels.ContainsKey(word))
                            {
                                label = labels[word];
                                instructionText.Append(label);
                            }
                            else
                            {
                                instructionText.AppendHex(word);
                                commentText.Append("Label not found");
                            }
                        }
                        break;
                    case ArgumentKind.Function:
                        {
                            int functionNumber = word;
                            Function function = null;
                            if (functionNumber >= 0 && functionNumber < ainFile.Functions.Count)
                            {
                                function = ainFile.Functions[functionNumber];
                                classNumber = function.StructType;
                                string functionName = function.Name;
                                string functionDisplayName = SanitizeVariableName(functionName);

                                instructionText.Append(functionDisplayName);

                                AppendAlternativeName(commentText, function);

                                //add function arguments to comment text
                                AppendFunctionArguments(commentText, function);
                            }
                            else
                            {
                                instructionText.AppendHex(word);
                                commentText.Append("Function number out of range");
                            }
                        }
                        break;
                    case ArgumentKind.Float:
                        {
                            float floatValue = BitConverter.ToSingle(BitConverter.GetBytes(word), 0);
                            instructionText.Append(floatValue.FloatToString());
                        }
                        break;
                    case ArgumentKind.String:
                        {
                            //is string defined?
                            int stringNumber = word;
                            if (stringNumber >= 0 && stringNumber < ainFile.Strings.Count)
                            {
                                string stringValue = ainFile.GetString(stringNumber);
                                bool dirty;
                                dirty = OnBeforeWritingString(instructionInfo, ref stringValue);

                                if (!dirty)
                                {
                                    int dupeNumber = GetStringDupeNumber(stringNumber);

                                    if (dupeNumber != -1)
                                    {
                                        instructionText.Append(dupeNumber.ToString() + " ");
                                    }
                                }
                                instructionText.Append("\"" + EscapeString(stringValue) + "\"");
                                AppendAlternativeString(commentText, stringValue);
                            }
                            else
                            {
                                instructionText.AppendHex(word);
                                commentText.Append("String number out of range");
                            }
                        }
                        break;
                    case ArgumentKind.StructType:
                        {
                            int structNumber = word;
                            //is struct defined?
                            if (structNumber >= 0 && structNumber < ainFile.Structs.Count)
                            {
                                var structInfo = ainFile.Structs[structNumber];
                                this.classNumber = structNumber;
                                string structTypeName = structInfo.Name;
                                string structTypeDisplayName = SanitizeVariableName(structTypeName);

                                instructionText.Append(structTypeDisplayName);

                                AppendAlternativeName(commentText, structInfo);
                            }
                            else
                            {
                                instructionText.AppendHex(word);
                                commentText.Append("Struct type number out of range");
                            }
                            this.lastSeenStructType = word;
                        }
                        break;
                    case ArgumentKind.Message:
                        {
                            int messageNumber = word;
                            if (messageNumber >= 0 && messageNumber < ainFile.Messages.Count)
                            {
                                int dupeNumber = GetMessageDupeNumber(messageNumber);
                                string messageText = ainFile.Messages[messageNumber];
                                if (dupeNumber != -1)
                                {
                                    instructionText.Append(dupeNumber.ToString() + " ");
                                }
                                instructionText.Append("'" + EscapeMessage(ainFile.Messages[word]) + "'");
                            }
                            else
                            {
                                instructionText.AppendHex(messageNumber);
                                commentText.Append("Message number out of range");
                            }
                        }
                        break;
                    case ArgumentKind.Library:
                        {
                            libraryNumber = word;
                        }
                        break;
                    case ArgumentKind.LibraryFunction:
                        {
                            bool handled = false;
                            int libraryFunctionNumber = word;
                            if (libraryNumber >= 0 && libraryNumber < ainFile.Libraries.Count)
                            {
                                var library = ainFile.Libraries[libraryNumber];
                                if (libraryFunctionNumber >= 0 && libraryFunctionNumber < library.Functions.Count)
                                {
                                    var libraryFunction = library.Functions[libraryFunctionNumber];
                                    instructionText.Append(SanitizeVariableName(library.LibraryName) + "." + SanitizeVariableName(libraryFunction.Name));

                                    AppendAlternativeName(commentText, libraryFunction);
                                    AppendFunctionArguments(commentText, libraryFunction);

                                    handled = true;
                                }
                            }
                            if (!handled)
                            {
                                instructionText.AppendHex(libraryNumber);
                                instructionText.Append(" ");
                                instructionText.AppendHex(libraryFunctionNumber);
                                commentText.Append("Library function out of range");
                            }
                        }
                        break;
                    case ArgumentKind.Global:
                        {
                            if (word >= 0 && word < ainFile.Globals.Count)
                            {
                                var variable = ainFile.Globals[word];
                                classNumber = variable.StructType;
                                StringBuilder name = new StringBuilder();
                                if (this.ShowGlobalPrefix && !String.IsNullOrEmpty(variable.GroupName))
                                {
                                    string globalGroupName = variable.GroupName;
                                    instructionText.Append(SanitizeVariableName(globalGroupName));
                                    instructionText.Append('.');
                                    instructionText.Append(SanitizeVariableName(variable.Name));

                                    AppendAlternativeName(commentText, variable.GroupIndex, variable);
                                }
                                else
                                {
                                    instructionText.Append(SanitizeVariableName(variable.Name));
                                    AppendAlternativeName(commentText, variable);
                                }
                            }
                            else
                            {
                                instructionText.AppendHex(word);
                                commentText.Append("Global variable number out of range");
                            }
                        }
                        break;
                    case ArgumentKind.Local:
                        {
                            bool handled = false;
                            if (currentFunctionNumber >= 0 && currentFunctionNumber < ainFile.Functions.Count)
                            {
                                var currentFunction = ainFile.Functions[currentFunctionNumber];
                                if (word >= 0 && word < currentFunction.Parameters.Count)
                                {
                                    handled = true;
                                    var variable = currentFunction.Parameters[word];
                                    classNumber = variable.StructType;
                                    if (!duplicatedLocalVariableNames.Contains(word))
                                    {
                                        instructionText.Append(SanitizeVariableName(variable.Name));
                                    }
                                    else
                                    {
                                        instructionText.AppendHex(word);
                                        commentText.Append(SanitizeVariableName(variable.Name));
                                    }
                                    AppendAlternativeName(commentText, variable);
                                }
                            }
                            if (!handled)
                            {
                                instructionText.AppendHex(word);
                                commentText.Append("Local variable out of range");
                            }
                        }
                        break;
                    case ArgumentKind.Switch:
                        {

                        }
                        break;
                    case ArgumentKind.Filename:
                        {
                            int filenameNumber = word;
                            if (filenameNumber >= 0 && filenameNumber < ainFile.Filenames.Count)
                            {
                                instructionText.Append("\"" + EscapeString(ainFile.Filenames[filenameNumber]) + "\"");
                            }
                            else
                            {
                                instructionText.AppendHex(word);
                            }
                        }
                        break;
                    case ArgumentKind.SystemCall:
                        {
                            int systemCallNumber = word;
                            if (systemCallNumber >= 0 && systemCallNumber < AinFile.SystemCalls.Count)
                            {
                                var systemCall = AinFile.SystemCalls[systemCallNumber];
                                instructionText.Append(systemCall.Name);  //no sanitize here
                                AppendFunctionArguments(commentText, systemCall);
                            }
                            else
                            {
                                instructionText.AppendHex(word);
                                commentText.Append("System call number out of range");
                            }
                        }
                        break;
                    case ArgumentKind.LocalMember:
                    case ArgumentKind.Member:
                        {
                            if (argumentKind == ArgumentKind.LocalMember)
                            {
                                classNumber = currentClassNumber;
                            }
                            if (classNumber >= 0 && classNumber < ainFile.Structs.Count)
                            {
                                var structInfo = ainFile.Structs[classNumber];
                                if (word >= 0 && word < structInfo.Members.Count)
                                {
                                    var member = structInfo.Members[word];
                                    instructionText.Append(SanitizeVariableName(member.Name));
                                    commentText.Append(SanitizeVariableName(structInfo.Name) + "\t");
                                    AppendAlternativeName(commentText, structInfo, member);
                                    classNumber = member.StructType;
                                }
                                else
                                {
                                    instructionText.AppendHex(word);
                                }
                            }
                            else
                            {
                                instructionText.AppendHex(word);
                            }
                        }
                        break;
                    case ArgumentKind.DelegateType:
                        {
                            if (ainFile.Delegates != null && word >= 0 && word < ainFile.Delegates.Count)
                            {
                                var delg = ainFile.Delegates[word];
                                instructionText.Append(SanitizeVariableName(delg.Name));
                            }
                            else
                            {
                                instructionText.AppendHex(word);
                                commentText.Append("Delegate number out of range");
                            }
                        }
                        break;
                }
            }
        }

        //private static Action GetNewLine(StringBuilder instructionText)
        //{
        //    Action newLine = () =>
        //    {
        //        instructionText.AppendLine();
        //    };
        //    return newLine;
        //}

        //private static Action<string> GetAppendInstructionText(StringBuilder instructionText)
        //{
        //    Action<string> appendInstructionText = (str) =>
        //    {
        //        instructionText.Append(str);
        //    };
        //    return appendInstructionText;
        //}



        private void AppendAlternativeName(StringBuilder commentText, Struct structInfo, Variable member)
        {
            string alternativeName = structInfo.GetAlternativeName() + "." + member.GetAlternativeName();
            string variableName = structInfo.Name + "." + member.Name;
            if (alternativeName != variableName)
            {
                commentText.Append(SanitizeVariableName(alternativeName));
            }
        }

        private void AppendAlternativeName(StringBuilder commentText, HllFunction libraryFunction)
        {
            string alternativeName = libraryFunction.ParentLibrary.GetAlternativeName() + "." + libraryFunction.GetAlternativeName();
            string variableName = libraryFunction.FullName;
            if (alternativeName != variableName)
            {
                commentText.Append(SanitizeVariableName(alternativeName));
            }
        }

        private void AppendAlternativeName(StringBuilder commentText, IVariable variable)
        {
            string alternativeName = variable.GetAlternativeName();
            string variableName = variable.Name;
            if (alternativeName != variableName)
            {
                commentText.Append(SanitizeVariableName(alternativeName));
            }
        }

        private void AppendAlternativeName(StringBuilder commentText, int globalGroupIndex, Global variable)
        {
            string alternativeName = ainFile.GlobalGroupNames.GetOrDefault(globalGroupIndex, "") + "." + variable.GetAlternativeName();
            string variableName = ainFile.GlobalGroupNames.GetOrDefault(globalGroupIndex, "") + "." + variable.Name;
            if (alternativeName != variableName)
            {
                commentText.Append(SanitizeVariableName(alternativeName));
            }
        }

        //private void AppendAlternativeName(StringBuilder commentText, string variableName)
        //{
        //    if (ainFile.AlternativeNames.ContainsKey(variableName))
        //    {
        //        variableName = ainFile.AlternativeNames[variableName];
        //        commentText.Append(SanitizeVariableName(variableName));
        //    }
        //}

        private void AppendAlternativeString(StringBuilder commentText, string stringValue)
        {
            if (ainFile.AlternativeNames.ContainsKey(stringValue))
            {
                stringValue = ainFile.AlternativeNames[stringValue];
                commentText.Append("\"" + EscapeString(stringValue) + "\"");
            }
        }

        //private void AppendAlternativeName(StringBuilder commentText, string firstPart, string variableName)
        //{
        //    if (ainFile.AlternativeNames.ContainsKey(firstPart) || ainFile.AlternativeNames.ContainsKey(variableName))
        //    {
        //        if (ainFile.AlternativeNames.ContainsKey(firstPart)) { firstPart = ainFile.AlternativeNames[firstPart]; }
        //        if (ainFile.AlternativeNames.ContainsKey(variableName)) { firstPart = ainFile.AlternativeNames[variableName]; }

        //        commentText.Append(SanitizeVariableName(firstPart) + "." + SanitizeVariableName(variableName));
        //    }
        //}

        private void AppendFunctionArguments(StringBuilder commentText, IFunction function)
        {
            if (function.ParameterCount > 0)
            {
                if (commentText.Length > 0)
                {
                    commentText.Append("\t");
                }
                bool needComma = false;
                for (int argNumber = 0; argNumber < function.ParameterCount; argNumber++)
                {
                    Util.PrintComma(commentText, ref needComma);
                    var arg = function.Parameters[argNumber];
                    commentText.Append(arg.Name);
                    string alternativeName = arg.GetAlternativeName();
                    if (alternativeName != arg.Name)
                    {
                        commentText.Append(" (" + SanitizeVariableName(alternativeName) + ")");
                    }
                }
            }
        }

        private int GetStringDupeNumber(int stringNumber)
        {
            string stringValue = ainFile.Strings[stringNumber];
            int dupeNumber = -1;
            if (DuplicateStrings.ContainsKey(stringValue))
            {
                var dic = DuplicateStrings[stringValue];
                if (dic.ContainsKey(stringNumber))
                {
                    dupeNumber = dic[stringNumber];
                }
            }
            return dupeNumber;
        }

        private int GetMessageDupeNumber(int messageNumber)
        {
            string messageText = ainFile.Messages[messageNumber];
            int dupeNumber = -1;
            if (DuplicateMessages.ContainsKey(messageText))
            {
                var dic = DuplicateMessages[messageText];
                if (dic.ContainsKey(messageNumber))
                {
                    dupeNumber = dic[messageNumber];
                }
            }
            return dupeNumber;
        }

        public static string SanitizeVariableName(string variableName)
        {
            string variableDisplayName;
            variableDisplayName = EscapeString(variableName);

            if (Token.IsNumber(variableDisplayName) || variableDisplayName.ContainsForbiddenCharacters() || variableDisplayName != variableName)
            {
                variableDisplayName = "\"" + EscapeString(variableDisplayName) + "\"";
            }
            return variableDisplayName;
        }

        private void WriteFunctionHeader(int functionNumber, bool comment)
        {
            var func = ainFile.Functions[functionNumber];

            if (comment)
            {
                tw.Write("//");
            }

            tw.Write(ExpressionDisplayer.GetDataTypeNameReal(func));
            tw.Write(" ");
            if (func.IsLabel != 0)
            {
                tw.Write("#");
            }
            tw.Write(func.Name);
            tw.Write("(");
            bool needComma = false;
            int i;
            for (i = 0; i < func.ParameterCount; i++)
            {
                Util.PrintComma(tw, ref needComma);
                var variable = func.Parameters[i];
                tw.Write(ExpressionDisplayer.GetDataTypeNameReal(variable) + " ");
                tw.Write(variable.Name);
            }
            tw.Write(")");
            if (i < func.Parameters.Count)
            {
                tw.WriteLine();
                if (comment)
                {
                    tw.Write("//");
                }
                tw.Write("localvars (");
                needComma = false;
                for (; i < func.Parameters.Count; i++)
                {
                    Util.PrintComma(tw, ref needComma);
                    var variable = func.Parameters[i];
                    tw.Write(ExpressionDisplayer.GetDataTypeNameReal(variable) + " ");
                    if (variable.Name.Contains(' '))
                    {
                        tw.Write("\"" + EscapeString(variable.Name) + "\"");
                    }
                    else
                    {
                        tw.Write(variable.Name);
                    }
                }
                tw.Write(")");
            }
        }

        private void WriteHllLibraries()
        {
            StringBuilder sb = new StringBuilder();

            tw.WriteLine("hll");
            tw.WriteLine("{");
            tw.Indent++;

            foreach (var library in ainFile.Libraries)
            {
                string libraryName = library.LibraryName;
                foreach (var function in library.Functions)
                {
                    sb.Length = 0;
                    sb.Append(ExpressionDisplayer.GetDataTypeNameReal(function) + " " + libraryName + "." + function.Name + "(");
                    bool needComma = false;
                    foreach (var arg in function.Parameters)
                    {
                        Util.PrintComma(sb, ref needComma);
                        sb.Append(ExpressionDisplayer.GetDataTypeNameReal(arg) + " " + arg.Name);
                    }
                    sb.Append(");");
                    tw.WriteLine(sb.ToString());
                }
            }
            tw.Indent--;
            tw.WriteLine("}");

        }

        private void WriteMessages()
        {
            tw.WriteLine("messages");
            tw.WriteLine("{");
            tw.Indent++;
            foreach (var str in ainFile.Messages)
            {
                tw.WriteLine("'" + EscapeMessage(str) + "'");
            }
            tw.Indent--;
            tw.WriteLine("}");
        }

        private void WriteStrings()
        {
            tw.WriteLine("strings");
            tw.WriteLine("{");
            tw.Indent++;
            foreach (var str in ainFile.Strings)
            {
                tw.WriteLine("\"" + EscapeString(str) + "\"");
            }
            tw.Indent--;
            tw.WriteLine("}");
        }

        public static string EscapeMessage(string stringToEscape)
        {
            return EscapeString(stringToEscape, '\'');
        }

        public static string EscapeString(string stringToEscape)
        {
            return EscapeString(stringToEscape, '"');
        }

        public static string EscapeString(string stringToEscape, char quotationCharacter)
        {
            StringBuilder sb = new StringBuilder(stringToEscape.Length);
            foreach (var c in stringToEscape)
            {
                if (c == '\r')
                {
                    sb.Append("\\r");
                }
                else if (c == '\n')
                {
                    sb.Append("\\n");
                }
                else if (c == '\t')
                {
                    sb.Append("\\t");
                }
                else if (c == quotationCharacter)
                {
                    sb.Append("\\" + quotationCharacter);
                }
                else if (c == '\\')
                {
                    sb.Append(@"\\");
                }
                else
                {
                    sb.Append(c);
                }
            }
            return sb.ToString();
        }

        public static string UnescapeString(string stringToUnescape)
        {
            StringBuilder sb = new StringBuilder(16);
            StringReader sr = new StringReader(stringToUnescape);

            while (true)
            {
                int charInt = sr.Read();
                if (charInt == -1)
                {
                    break;
                }
                char c = (char)charInt;

                if (c == '\\')
                {
                    charInt = sr.Read();
                    if (charInt == -1)
                    {
                        break;
                    }
                    c = (char)charInt;
                    if (c == 'r')
                    {
                        c = '\r';
                    }
                    else if (c == 'n')
                    {
                        c = '\n';
                    }
                    else if (c == 't')
                    {
                        c = '\t';
                    }
                    else if (c == '\r')
                    {
                        charInt = sr.Peek();
                        if (charInt == '\n')
                        {
                            sr.Read();
                            continue;
                        }
                        continue;
                    }
                    else if (c == '\n')
                    {
                        continue;
                    }
                }
                sb.Append(c);
            }
            return sb.ToString();
        }

        private void WriteGlobals()
        {
            StringBuilder sb = new StringBuilder();
            Dictionary<int, GlobalInitialValue> lookupInitializers = new Dictionary<int, GlobalInitialValue>();

            for (int i = 0; i < ainFile.GlobalInitialValues.Count; i++)
            {
                var globalInitialValue = ainFile.GlobalInitialValues[i];
                int id = globalInitialValue.GlobalIndex;
                lookupInitializers.Set(id, globalInitialValue);
            }

            tw.WriteLine("globalgroupnames");
            tw.WriteLine("{");
            tw.Indent++;
            for (int i = 0; i < ainFile.GlobalGroupNames.Count; i++)
            {
                tw.WriteLine(ainFile.GlobalGroupNames[i]);
            }
            tw.Indent--;
            tw.WriteLine("}");

            tw.WriteLine("globals");
            tw.WriteLine("{");
            tw.Indent++;

            for (int i = 0; i < ainFile.Globals.Count; i++)
            {
                var global = ainFile.Globals[i];
                sb.Length = 0;
                string typeName = ExpressionDisplayer.GetDataTypeNameReal(global);
                sb.Append(typeName + " ");
                if (global.GroupIndex != -1)
                {
                    sb.Append(ainFile.GlobalGroupNames[global.GroupIndex] + ".");
                }
                sb.Append(global.Name);

                if (lookupInitializers.ContainsKey(i))
                {
                    sb.Append(" = ");
                    var initializer = lookupInitializers[i];
                    if (initializer.DataType == DataType.String)
                    {
                        sb.Append("\"" + EscapeString(initializer.StringValue) + "\"");
                    }
                    else if (initializer.DataType == DataType.Bool)
                    {
                        sb.Append(initializer.IntValue == 0 ? "false" : "true");
                    }
                    else if (initializer.DataType == DataType.Float)
                    {
                        float floatValue = BitConverter.ToSingle(BitConverter.GetBytes(initializer.IntValue), 0);
                        sb.Append(floatValue.FloatToString());
                    }
                    else
                    {
                        sb.Append(initializer.IntValue);
                    }
                }
                sb.Append(";");


                tw.WriteLine(sb.ToString());
            }
            tw.Indent--;
            tw.WriteLine("}");


        }

        private void WriteFunctionTypes()
        {
            StringBuilder sb = new StringBuilder();
            foreach (var functionType in ainFile.FunctionTypes)
            {
                sb.Length = 0;
                sb.Append("functype ");
                string typeName = ExpressionDisplayer.GetDataTypeNameReal(functionType);
                sb.Append(typeName + " " + functionType.Name + " (");
                int i;
                bool needComma = false;
                for (i = 0; i < functionType.ParameterCount; i++)
                {
                    var arg = functionType.Parameters[i];
                    Util.PrintComma(sb, ref needComma);
                    sb.Append(ExpressionDisplayer.GetDataTypeNameReal(arg));
                }
                sb.Append(")");
                needComma = false;
                for (; i < functionType.Parameters.Count; i++)
                {
                    var arg = functionType.Parameters[i];
                    Util.PrintComma(sb, ref needComma);
                    sb.Append(ExpressionDisplayer.GetDataTypeNameReal(arg));
                }
                sb.Append(";");
                tw.WriteLine(sb.ToString());
            }
        }

        private void WriteDelegates()
        {
            if (ainFile.Delegates == null)
            {
                return;
            }
            StringBuilder sb = new StringBuilder();
            foreach (var delg in ainFile.Delegates)
            {
                sb.Length = 0;
                sb.Append("delegate ");
                string typeName = ExpressionDisplayer.GetDataTypeNameReal(delg);
                sb.Append(typeName + " " + delg.Name + " (");
                int i;
                bool needComma = false;
                for (i = 0; i < delg.ParameterCount; i++)
                {
                    var arg = delg.Parameters[i];
                    Util.PrintComma(sb, ref needComma);
                    sb.Append(ExpressionDisplayer.GetDataTypeNameReal(arg));
                }
                sb.Append(")");
                needComma = false;
                for (; i < delg.Parameters.Count; i++)
                {
                    var arg = delg.Parameters[i];
                    Util.PrintComma(sb, ref needComma);
                    sb.Append(ExpressionDisplayer.GetDataTypeNameReal(arg));
                }
                sb.Append(";");
                tw.WriteLine(sb.ToString());
            }
        }

        private void WriteStructs()
        {
            StringBuilder sb = new StringBuilder();
            string lastTypeName = "";
            string dataTypeName = "";
            bool needComma = false;
            IVariable member = null;

            Action BeginLine = null, ContinueLine = null, FinishLine = null;


            BeginLine = () =>
            {
                FinishLine();
                sb.Append(dataTypeName + " ");
                needComma = false;
                lastTypeName = dataTypeName;
            };

            ContinueLine = () =>
            {
                Util.PrintComma(sb, ref needComma);
                sb.Append(member.Name);
            };

            FinishLine = () =>
            {
                if (sb.Length > 0)
                {
                    sb.Append(";");
                    tw.WriteLine(sb.ToString());
                    sb.Length = 0;
                }
                lastTypeName = "";
            };

            foreach (var structInfo in ainFile.Structs)
            {
                tw.WriteLine("struct " + structInfo.Name);
                tw.WriteLine("{");
                tw.Indent++;

                for (int i = 0; i < structInfo.Members.Count; i++)
                {
                    member = structInfo.Members[i];
                    dataTypeName = ExpressionDisplayer.GetDataTypeNameReal(member);

                    if (dataTypeName != lastTypeName || sb.Length >= 70)
                    {
                        BeginLine();
                    }
                    ContinueLine();
                }
                FinishLine();
                tw.Indent--;
                tw.WriteLine("};");
                tw.WriteLine();
            }
        }
    }

    public static partial class Extensions
    {
        public static bool ContainsForbiddenCharacters(this string str)
        {
            foreach (var c in str)
            {
                if (MyTextReader.IsTokenCharacter(c) || char.IsWhiteSpace(c))
                {
                    return true;
                }
            }
            return false;
        }

        public static void AppendHex(this StringBuilder sb, int value)
        {
            sb.Append("0x" + value.ToString("X"));
        }

        public static string FloatToString(this float f)
        {
            return ((double)f).ToString(CultureInfo.InvariantCulture);
        }

        public static int FloatToInt(this float f)
        {
            return BitConverter.ToInt32(BitConverter.GetBytes(f), 0);
        }
    }

    public class InstructionInfoEventArgs : HandledEventArgs
    {
        public InstructionInfo InstructionInfo;
        string _text = null;
        bool _dirty = false;

        public bool StopEmittingCode
        {
            get;
            set;
        }

        public bool Dirty
        {
            get
            {
                return _dirty;
            }
            private set
            {
                _dirty = value;
            }
        }

        public string Text
        {
            get
            {
                return _text;
            }
            set
            {
                if (_text != value)
                {
                    Dirty = true;
                }
                _text = value;
            }
        }

        private Action<string> printFunction;
        private Action newLineFunction;

        public void WriteText(string text)
        {
            if (printFunction != null)
            {
                printFunction(text);
            }
        }

        public void WriteLine(string text)
        {
            WriteText(text);
            if (newLineFunction != null)
            {
                newLineFunction();
            }
        }

        public InstructionInfoEventArgs(InstructionInfo instructionInfo, Action<string> writeTextFunction, Action newLineFunction, string text, bool StopEmittingCode)
            : base(false)
        {
            this._text = text;
            this.printFunction = writeTextFunction;
            this.newLineFunction = newLineFunction;
            this.InstructionInfo = instructionInfo;
            this.StopEmittingCode = StopEmittingCode;
        }

        public InstructionInfoEventArgs(InstructionInfo instructionInfo, Action<string> writeTextFunction, Action newLineFunction)
            : this(instructionInfo, writeTextFunction, newLineFunction, null, false)
        {

        }

        public InstructionInfoEventArgs(InstructionInfo instructionInfo, string text)
            : this(instructionInfo, null, null, text, false)
        {

        }
    }
}
