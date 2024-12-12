using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Text;
using System.Diagnostics;
using System.Threading;
using System.Windows.Forms;

namespace AinDecompiler
{
    public class TextImportExport
    {
        AinFile ainFile;
        public EnumerationType AnnotateEnumerationType
        {
            get
            {
                return _annotateEnumerationType;
            }
            set
            {
                _annotateEnumerationType = value;
                GetVariablesUsingEnumeartionType();
            }
        }

        private void GetVariablesUsingEnumeartionType()
        {
            VariablesUsingEnumerationType.Clear();
            if (_annotateEnumerationType == null)
            {
                return;
            }
            foreach (var pair in ainFile.MetadataFile.Metadata)
            {
                var variable = pair.Key;
                var metaData = pair.Value;
                if (metaData.EnumerationType == _annotateEnumerationType)
                {
                    VariablesUsingEnumerationType.Add(variable);
                }
                var function = variable.Parent as Function;
                if (function != null)
                {
                    VariablesUsingEnumerationType.Add(function);
                }
            }

        }

        EnumerationType _annotateEnumerationType = null;
        HashSet<IVariable> VariablesUsingEnumerationType = new HashSet<IVariable>();

        public bool IncludeMessages = true;
        public bool IncludeStrings = true;
        public bool SeparateStrings = false;
        StringExportImport stringExportImport = null;
        public WordWrapOptions wordWrapOptions;
        //Replacer replacer;
        public HashSet<string> StringsToMatch = new HashSet<string>();
        bool useStringsToMatch = false;

        public TextImportExport(AinFile ainFile)
        {
            this.ainFile = ainFile;
            this.stringExportImport = new StringExportImport(ainFile);
            this.stringExportImport.GetExclusionList();
            //this.replacer = new Replacer(ainFile);
        }

        public void SaveText(string fileName, Encoding encoding)
        {
            using (var fileStream = new FileStream(fileName, FileMode.Create, FileAccess.Write, FileShare.ReadWrite))
            {
                using (var streamWriter = new StreamWriter(fileStream, encoding))
                {
                    SaveText(streamWriter);
                    streamWriter.Flush();
                    fileStream.Flush();
                    streamWriter.Close();
                    fileStream.Close();
                }
            }
        }

        public void SaveText(string fileName)
        {
            SaveText(fileName, Encoding.UTF8);  //utf8 with BOM
        }

        public void SaveText(TextWriter tw)
        {
            //iterate through the functions
            //enumerate strings and messages used
            //skip anything that uses no strings, no messages, or only empty strings
            //display "FUNCTION x functionName"
            //display "#x strings, x messages

            foreach (var function in ainFile.Functions)
            {
                var functionLines = GetTextFromFunction(function);
                if (functionLines.Count > 2)
                {
                    foreach (var line in functionLines)
                    {
                        tw.WriteLine(line);
                    }
                }
            }
        }

        private List<string> GetTextFromFunction(Function function)
        {
            int numberOfNonCommentedLines = 0;
            int numberOfStrings = 0;
            int numberOfMessages = 0;

            //Expression expression = null;
            //int expressionLastAddress = function.Address;

            useStringsToMatch = StringsToMatch != null && StringsToMatch.Count > 0;
            //List<string> strings = new List<string>();
            //List<string> messages = new List<string>();
            List<string> functionLines = new List<string>();
            string functionLineString = "FUNCTION " + /*function.Index.ToString() + " " + */ AssemblerProjectWriter.SanitizeVariableName(function.Name);

            functionLines.Add(functionLineString);
            functionLines.Add("#x strings, x messages");  //this line gets changed later (it's index 1)
            int address = function.Address;
            while (address < ainFile.Code.Length)
            {
                var instructionInfo = ainFile.Peek(address);
                if (instructionInfo.instruction == Instruction.ENDFUNC || instructionInfo.instruction == Instruction.FUNC)
                {
                    break;
                }
                if (this.AnnotateEnumerationType != null && instructionInfo.instruction == Instruction.CALLFUNC)
                {
                    var func = ainFile.GetFunction(instructionInfo.word1);
                    if (VariablesUsingEnumerationType.Contains(func))
                    {
                        var parameter = GetParameterThatUsesEnumerationType(func);
                        if (parameter != null)
                        {
                            int i = parameter.Index;
                            int addr = instructionInfo.CurrentAddress - (func.ParameterCount) * 6 + i * 6;
                            var ins2 = ainFile.Peek(addr);
                            if (ins2.instruction == Instruction.PUSH)
                            {
                                string enumerationValue = this.AnnotateEnumerationType.GetOrDefault(ins2.word1, ins2.word1.ToString());
                                functionLines.Add("");
                                functionLines.Add("#" + enumerationValue);
                            }
                        }

                    }
                }
                if (instructionInfo.instruction == Instruction.MSG)
                {
                    //if (this.AnnotateEnumerationType != null)
                    //{
                    //    if (expression == null)
                    //    {
                    //        expression = ainFile.DecompiledCodeCache.GetDecompiledCode(function);
                    //    }
                    //    CatchUpToAddress(ref expression, functionLines, address);
                    //}
                    int messageNumber = instructionInfo.word1;
                    string message = ainFile.GetMessage(messageNumber);
                    if (message != null)
                    {
                        if (useStringsToMatch == false || StringsToMatch.Contains(message))
                        {
                            string messageLine = "m " + numberOfMessages.ToString("000") + " " + StringExportImport.EscapeText(message);
                            functionLines.Add(messageLine);
                        }
                        numberOfMessages++;
                        numberOfNonCommentedLines++;
                    }
                }
                else if (instructionInfo.instruction == Instruction.STRSWITCH)
                {
                    int switchBlockNumber = instructionInfo.word1;
                    var switchBlock = ainFile.Switches.GetOrNull(switchBlockNumber);
                    if (switchBlock != null)
                    {
                        foreach (var switchCase in switchBlock.SwitchCases)
                        {
                            int stringNumber = switchCase.Value;
                            string str = ainFile.GetString(stringNumber);
                            if (str != null)
                            {
                                AddString(ref numberOfNonCommentedLines, ref numberOfStrings, functionLines, stringNumber, str);
                            }
                        }
                    }
                }
                else
                {
                    int indexOfStringArgument = instructionInfo.instruction.IndexOfStringArgument();
                    if (indexOfStringArgument != -1)
                    {
                        int stringNumber = instructionInfo.words[indexOfStringArgument];
                        string str = ainFile.GetString(stringNumber);
                        if (str != null)
                        {
                            AddString(ref numberOfNonCommentedLines, ref numberOfStrings, functionLines, stringNumber, str);
                        }
                    }
                }
                address = instructionInfo.nextAddress;
            }
            functionLines[1] = "#" + numberOfStrings.ToString() + " strings, " + numberOfMessages.ToString() + " messages";
            if (numberOfNonCommentedLines == 0)
            {
                functionLines.Clear();
            }
            return functionLines;
        }

        private Variable GetParameterThatUsesEnumerationType(Function func)
        {
            var parameter = func.Parameters.Where(v =>
            {
                var metaData = v.GetMetadata();
                if (metaData != null && metaData.EnumerationType == this.AnnotateEnumerationType)
                {
                    return true;
                }
                return false;
            }).FirstOrDefault();
            return parameter;
        }

        //private void CatchUpToAddress(ref Expression expression, List<string> functionLines, int address)
        //{
        //    while (expression != null && expression.address < address)
        //    {
        //        string enumerationName = null;
        //        var variable = expression.Variable.Canonicalize();
        //        if (variable != null && VariablesUsingEnumerationType.Contains(variable))
        //        {
        //            var metaData = variable.GetMetadata();
        //            if (metaData.EnumerationType == AnnotateEnumerationType)
        //            {
        //                var otherExpression = expression.GetOtherSideOfBinaryExpression();
        //                if (otherExpression != null)
        //                {
        //                    otherExpression = otherExpression.SkipChildCastOperations();
        //                    if (otherExpression.ExpressionType == Instruction.PUSH)
        //                    {
        //                        enumerationName = AnnotateEnumerationType.GetOrDefault(otherExpression.Value, otherExpression.Value.ToString());
        //                    }
        //                    else if (otherExpression.ExpressionType == Instruction.SH_LOCALASSIGN)
        //                    {
        //                        enumerationName = AnnotateEnumerationType.GetOrDefault(otherExpression.Value, otherExpression.Value.ToString());
        //                    }
        //                }
        //            }
        //            else
        //            {
        //                var func = expression as IFunction;
        //                if (func != null)
        //                {
        //                    for (int i = 0; i < func.ParameterCount; i++)
        //                    {
        //                        var para = func.GetNonVoidFunctionParameter(i);
        //                        if (para == null) break;
        //                        var metaData2 = para.GetMetadata();
        //                        if (metaData2.EnumerationType == AnnotateEnumerationType)
        //                        {
        //                            var arg = expression.GetFunctionCallArgument(i);
        //                            if (arg != null)
        //                            {
        //                                arg = arg.SkipChildCastOperations();
        //                                if (arg.ExpressionType == Instruction.PUSH)
        //                                {
        //                                    enumerationName = AnnotateEnumerationType.GetOrDefault(arg.Value, arg.Value.ToString());
        //                                }
        //                            }
        //                        }
        //                    }
        //                }
        //            }
        //        }
        //        if (enumerationName != null)
        //        {
        //            functionLines.Add("#" + enumerationName);
        //        }
        //        expression = expression.GetNextExpression();
        //    }
        //}

        private void AddString(ref int numberOfNonCommentedLines, ref int numberOfStrings, List<string> functionLines, int stringNumber, string str)
        {
            StringExclusionReason exclude = stringExportImport.stringsToExclude.GetOrDefault(stringNumber, StringExclusionReason.None);
            string stringLine = "s " + numberOfStrings.ToString("000") + " " + StringExportImport.EscapeText(str);
            if (exclude != StringExclusionReason.None)
            {
                stringLine = "#" + stringLine + "#\t\t" + exclude.GetText();
            }
            else
            {
                if (str != "")
                {
                    numberOfNonCommentedLines++;
                }
            }
            if (!(stringNumber == 0 && str == ""))
            {
                if (useStringsToMatch == false || StringsToMatch.Contains(str))
                {
                    if (this.IncludeStrings == true)
                    {
                        functionLines.Add(stringLine);
                    }
                }
            }
            numberOfStrings++;
        }

        public void ReplaceText(string textFileName, string outputFileName)
        {
            Encoding encoding = EncodingDetector.DetectEncoding(textFileName);
            using (var fileStream = new FileStream(textFileName, FileMode.Open, FileAccess.Read, FileShare.ReadWrite))
            {
                using (var streamReader = new StreamReader(fileStream, encoding))
                {
                    ReplaceText(streamReader, textFileName, outputFileName);
                }
            }
        }

        public void ReplaceText(StreamReader sr, string textFileName, string outputFileName)
        {
            var replacer = new Replacer(ainFile, outputFileName, wordWrapOptions);
            replacer.ReplaceText(new TextReaderWrapper(sr), textFileName);
        }

        class Replacer
        {
            bool InsideFunction = false;
            bool hasOutputFunction = false;
            bool _wordWrap;

            public bool WordWrap
            {
                get
                {
                    return _wordWrap;
                }
                set
                {
                    _wordWrap = value;
                }
            }

            AinFile ainFile;
            AinFile originalAinFile;
            string outputFileName;
            WordWrapOptions wordWrapOptions = null;
            public Replacer(AinFile ainFile, string outputFileName, WordWrapOptions wordWrapOptions)
            {
                this.ainFile = ainFile;
                this.originalAinFile = ainFile;
                this.outputFileName = outputFileName;
                this.wordWrapOptions = wordWrapOptions;
                if (this.wordWrapOptions != null)
                {
                    this.WordWrap = true;
                    this.wordWrapper = new WordWrapper(ainFile, wordWrapOptions);
                }
            }

            WordWrapper wordWrapper;

            Dictionary<string, Dictionary<int, string>> stringEntries = new Dictionary<string, Dictionary<int, string>>();
            Dictionary<string, Dictionary<int, string>> messageEntries = new Dictionary<string, Dictionary<int, string>>();
            Dictionary<string, string> CodePatches = new Dictionary<string, string>();
            HashSet<string> IncludedFiles = new HashSet<string>();

            string currentFunctionName;
            Dictionary<int, string> stringDictionary;
            Dictionary<int, string> messageDictionary;
            Dictionary<int, string> numberedStrings = new Dictionary<int, string>();

            public void ReplaceText(TextReaderWrapper tr, string textFileName)
            {
                stringEntries.Clear();
                messageEntries.Clear();
                numberedStrings.Clear();
                CodePatches.Clear();

                currentFunctionName = "";
                stringDictionary = stringEntries.GetOrAddNew(currentFunctionName);
                messageDictionary = messageEntries.GetOrAddNew(currentFunctionName);

                ReadReplacementFile(tr, textFileName);

                ExportAndMerge();
            }

            private void ReadReplacementFile(TextReaderWrapper tr, string textFileName)
            {
                textFileName = Path.GetFullPath(textFileName);
                if (IncludedFiles.Contains(textFileName.ToUpperInvariant()))
                {
                    return;
                }
                IncludedFiles.Set(textFileName.ToUpperInvariant());

                string line;
                while (true)
                {
                    line = tr.ReadLine();
                    if (line == null)
                    {
                        break;
                    }

                    //remove initial whitespace
                    line = line.TrimStart();

                    //check for "#include"
                    if (line.StartsWith("#include "))
                    {
                        string filenameToInclude = line.Substring("#include ".Length);
                        //check for quotes?
                        if (filenameToInclude.StartsWith("\"") && filenameToInclude.EndsWith("\""))
                        {
                            filenameToInclude = filenameToInclude.Substring(1, filenameToInclude.Length - 2);
                        }
                        string basePath = tr.DirectoryName;
                        filenameToInclude = Path.Combine(basePath, filenameToInclude);
                        if (!File.Exists(filenameToInclude))
                        {
                            throw new FileNotFoundException("Cannot find file: " + filenameToInclude, filenameToInclude);
                        }

                        if (File.Exists(filenameToInclude) && !IncludedFiles.Contains(filenameToInclude.ToUpperInvariant()))
                        {
                            IncludedFiles.Add(filenameToInclude.ToUpperInvariant());
                            var encoding = EncodingDetector.DetectEncoding(filenameToInclude);
                            tr.IncludeTextReader(new StreamReader(filenameToInclude, encoding));
                        }
                        continue;
                    }


                    //remove commented text
                    int indexOfComment = line.IndexOf('#');
                    if (indexOfComment >= 0)
                    {
                        line = line.Substring(0, indexOfComment);
                    }

                    //reading one of these lines:
                    //CODE
                    //function x functionName  (or func, f)
                    //string x text (or str, s)
                    //message x text (or msg, m)
                    //id x text (or i)
                    //x text (same as id x text)

                    if (line.Trim().ToUpperInvariant() == "CODE")
                    {
                        StringBuilder codeText = new StringBuilder();
                        while (true)
                        {
                            line = tr.ReadLine();
                            string lineTrim = line.Trim();
                            if (lineTrim.StartsWith("#include"))
                            {
                                string filenameToInclude = lineTrim.Substring("#include ".Length);
                                //check for quotes?
                                if (filenameToInclude.StartsWith("\"") && filenameToInclude.EndsWith("\""))
                                {
                                    filenameToInclude = filenameToInclude.Substring(1, filenameToInclude.Length - 2);
                                }
                                string basePath = tr.DirectoryName;
                                filenameToInclude = Path.Combine(basePath, filenameToInclude);

                                if (!File.Exists(filenameToInclude))
                                {
                                    throw new FileNotFoundException("Cannot find file: " + filenameToInclude, filenameToInclude);
                                }

                                if (File.Exists(filenameToInclude) && !IncludedFiles.Contains(filenameToInclude.ToUpperInvariant()))
                                {
                                    IncludedFiles.Add(filenameToInclude.ToUpperInvariant());
                                    var encoding = EncodingDetector.DetectEncoding(filenameToInclude);
                                    tr.IncludeTextReader(new StreamReader(filenameToInclude, encoding));
                                }
                                continue;
                            }

                            if (lineTrim.ToUpperInvariant() == "ENDCODE")
                            {
                                CodePatches.Set(currentFunctionName, codeText.ToString());
                                break;
                            }
                            else
                            {
                                codeText.AppendLine(line);
                            }
                        }
                        continue;
                    }

                    //find first space
                    int spaceIndex = line.IndexOf(' ');
                    if (spaceIndex == -1)
                    {
                        continue;
                    }
                    string tagName = line.Substring(0, spaceIndex);
                    line = line.Substring(spaceIndex + 1);
                    int number;
                    //if it starts with a number, it's a legacy text replacement
                    if (IntUtil.TryParse(tagName, out number) == true)
                    {
                        tagName = "id";
                    }
                    else
                    {
                        bool isFunction = false;
                        string tagNameLower = tagName.ToLowerInvariant();
                        if (tagNameLower == "f" || tagNameLower == "func" || tagNameLower == "function")
                        {
                            isFunction = true;
                        }

                        line = line.TrimStart();
                        spaceIndex = line.IndexOf(' ');
                        if (spaceIndex == -1)
                        {
                            if (isFunction)
                            {
                                number = -1;
                            }
                            else
                            {
                                continue;
                            }
                        }
                        else
                        {
                            string numberString = line.Substring(0, spaceIndex);
                            line = line.Substring(spaceIndex + 1);

                            if (IntUtil.TryParse(numberString, out number) == false)
                            {
                                if (isFunction)
                                {
                                    line = numberString + " " + line;
                                    number = -1;
                                }
                                else
                                {
                                    continue;
                                }
                            }
                        }
                    }

                    line = StringExportImport.UnescapeText(line);

                    switch (tagName.ToLowerInvariant())
                    {
                        case "f":
                        case "func":
                        case "function":
                            string nextFunctionName = line.Trim();
                            var function = ainFile.GetFunction(number);
                            if (function == null)
                            {
                                function = ainFile.GetFunction(nextFunctionName);
                                if (function == null)
                                {
                                    continue;
                                }
                            }
                            currentFunctionName = function.Name;
                            stringDictionary = stringEntries.GetOrAddNew(currentFunctionName);
                            messageDictionary = messageEntries.GetOrAddNew(currentFunctionName);
                            break;
                        case "string":
                        case "str":
                        case "s":
                            stringDictionary.Set(number, line);
                            break;
                        case "msg":
                        case "message":
                        case "m":
                            messageDictionary.Set(number, line);
                            break;
                        case "i":
                        case "id":
                            numberedStrings.Set(number, line);
                            break;
                    }
                }
            }

            private void ExportAndMerge()
            {
                currentFunctionName = "";
                stringDictionary = null;
                messageDictionary = null;
                stringNumber = 0;
                messageNumber = 0;

                //var ainFile = this.ainFile;
                if (numberedStrings.Count > 0)
                {
                    this.ainFile = ainFile.Clone();
                    var stringExportImport = new StringExportImport(ainFile);
                    int firstMessage = stringExportImport.GetFirstMessageIdNumber();
                    int lastMessage = firstMessage + ainFile.Messages.Count;
                    int firstString = stringExportImport.GetFirstStringIdNumber();
                    int lastString = firstString + ainFile.Strings.Count;

                    foreach (var pair in numberedStrings)
                    {
                        int number = pair.Key;
                        string message = pair.Value;
                        if (number >= firstMessage && number < lastMessage)
                        {
                            ainFile.Messages[number - firstMessage] = message;
                        }
                        else if (number >= firstString && number < lastString)
                        {
                            ainFile.Strings[number - firstString] = message;
                        }
                    }


                }

                using (TemporaryFile tempFile = new TemporaryFile("jam", true))
                {
                    var saver = new ExplorerForm.SaveProjectInBackground();
                    var options = new CodeDisplayOptions();
                    options.AnnotateWithDecompiledCode = false;
                    options.MergeDuplicateStrings = true;
                    var writer = new AssemblerProjectWriter(ainFile, options);
                    writer.BeforeWritingInstruction += new EventHandler<InstructionInfoEventArgs>(writer_BeforeWritingInstruction);
                    writer.BeforeWritingString += new EventHandler<InstructionInfoEventArgs>(writer_BeforeWritingString);
                    saver.SaveAsProject(writer, tempFile.FileName, true);

                    //WaitForFileLock(tempFile.FileName, 2000);

                    //tempFile.FileName

                    var builder = new ExplorerForm.BuildProjectInBackground();
                    builder.Build(tempFile.FileName, this.outputFileName, true);
                }
            }

            //private static void WaitForFileLock(string fileName, int timeout)
            //{
            //    //verify file isn't locked
            //    Stopwatch stopWatch = new Stopwatch();
            //    stopWatch.Start();
            //    while (stopWatch.ElapsedMilliseconds < timeout)
            //    {
            //        try
            //        {
            //            using (var dummyStream = new FileStream(fileName, FileMode.Open, FileAccess.Read, FileShare.ReadWrite))
            //            {

            //            }
            //            break;
            //        }
            //        catch (IOException)
            //        {

            //        }
            //        Thread.Sleep(100);
            //    }

            //    try
            //    {
            //        using (var dummyStream = new FileStream(fileName, FileMode.Open, FileAccess.Read, FileShare.ReadWrite))
            //        {

            //        }
            //    }
            //    catch (IOException ex)
            //    {
            //        throw new IOException("File " + fileName + " is locked!", ex);
            //    }
            //}

            int stringNumber = 0;
            int messageNumber = 0;

            void writer_BeforeWritingInstruction(object sender, InstructionInfoEventArgs e)
            {
                var instructionInfo = e.InstructionInfo;
                var instruction = instructionInfo.instruction;
                int word1 = instructionInfo.word1;
                if (InsideFunction)
                {
                    if (!hasOutputFunction)
                    {
                        if (this.CodePatches.ContainsKey(currentFunctionName))
                        {
                            var codePatch = this.CodePatches[currentFunctionName];
                            e.WriteLine(codePatch);
                        }
                        hasOutputFunction = true;
                    }

                    if (instruction == Instruction.FUNC || instruction == Instruction.ENDFUNC || instruction == Instruction.EOF)
                    {
                        //will emit the ENDFUNC or next FUNC instruction
                        InsideFunction = false;
                        e.StopEmittingCode = false;
                    }
                    else
                    {
                        e.Handled = true;
                        e.StopEmittingCode = true;
                        return;
                    }
                }


                if (instruction == Instruction.FUNC)
                {
                    int functionNumber = instructionInfo.word1;
                    var function = ainFile.GetFunction(functionNumber);
                    if (function != null)
                    {
                        stringNumber = 0;
                        messageNumber = 0;
                        currentFunctionName = function.Name;
                        stringDictionary = stringEntries.GetOrNull(currentFunctionName);
                        messageDictionary = messageEntries.GetOrNull(currentFunctionName);

                        if (this.CodePatches.ContainsKey(currentFunctionName))
                        {
                            //var codePatch = this.CodePatches[currentFunctionName];
                            //e.WriteLine(codePatch);
                            e.Handled = false;
                            e.StopEmittingCode = false;
                            //will emit the FUNC instruction, but then not the rest
                            this.InsideFunction = true;
                            this.hasOutputFunction = false;
                        }
                    }
                }
                else /*if (stringDictionary != null || messageDictionary != null)*/
                {
                    if (instruction == Instruction.MSG)
                    {
                        string originalMessage = originalAinFile.GetMessage(word1);
                        string newMessage = null;
                        if (messageDictionary != null)
                        {
                            newMessage = messageDictionary.GetOrNull(messageNumber);
                        }

                        if (newMessage != null && newMessage != originalMessage)
                        {
                            e.Text = newMessage;
                        }

                        if (this.WordWrap)
                        {
                            if (this.wordWrapper.HasRemainingText == false && e.Text == originalMessage)
                            {
                                //don't wrap text because we match the original and don't have remaining text
                            }
                            else
                            {
                                wordWrapper.projectWriter_BeforeWritingInstruction(sender, e);
                            }
                        }
                        if (e.Dirty && !e.Handled)
                        {
                            //recheck this!
                            newMessage = e.Text;
                            e.WriteLine("\tMSG '" + AssemblerProjectWriter.EscapeMessage(newMessage) + "'");
                            e.Handled = true;
                        }
                        messageNumber++;
                    }
                    //string instructions are handled by writer_BeforeWritingString
                    else if (instruction == Instruction.CALLFUNC)
                    {
                        //for text wrapping (to do later)
                        if (this.WordWrap)
                        {
                            wordWrapper.projectWriter_BeforeWritingInstruction(sender, e);
                        }
                    }
                }
            }

            void writer_BeforeWritingString(object sender, InstructionInfoEventArgs e)
            {
                var instructionInfo = e.InstructionInfo;
                var instruction = instructionInfo.instruction;
                int indexOfStringArgument = instruction.IndexOfStringArgument();
                if (instruction.IndexOfStringArgument() >= 0)
                {
                    string originalString = ainFile.GetString(instructionInfo.words[indexOfStringArgument]);
                    string replacementString = null;
                    if (stringDictionary != null)
                    {
                        replacementString = stringDictionary.GetOrNull(stringNumber);
                    }
                    if (replacementString != null && originalString != replacementString)
                    {
                        e.Text = replacementString;
                    }
                    stringNumber++;
                }
            }
        }

        public void SaveTextToMultipleFiles(string outputFileName, Encoding encoding)
        {
            this.IncludeMessages = true;
            this.IncludeStrings = false;
            string baseDirectoryName = Path.GetDirectoryName(outputFileName);
            string textDirectory = Path.Combine(baseDirectoryName, "text");
            string rootFileName = Path.Combine(textDirectory, "root.txt");
            Directory.CreateDirectory(textDirectory);
            using (var rootFileStream = new FileStream(rootFileName, FileMode.Create, FileAccess.Write))
            {
                var rootTextWriter = new StreamWriter(rootFileStream, encoding);

                var enumerator = new FunctionEnumerator(ainFile);
                var results = enumerator.GetFilesAndFunctions();
                foreach (var result1 in results)
                {
                    string jafFileName = result1.name;
                    foreach (var functionNode in result1.children)
                    {
                        var function = ainFile.GetFunction(functionNode.name);
                        var text = GetTextFromFunction(function);

                        if (text.Count > 2)
                        {
                            string functionName = AssemblerProjectWriter.SanitizeVariableName(function.Name);
                            if (functionName.StartsWith("\""))
                            {

                            }
                            else
                            {
                                string includeFileName = Path.Combine(jafFileName, functionName + ".txt");
                                string fileName = Path.Combine(textDirectory, includeFileName);
                                string outputDirectory = Path.GetDirectoryName(fileName);
                                Directory.CreateDirectory(outputDirectory);
                                File.WriteAllLines(fileName, text.ToArray(), encoding);
                                rootTextWriter.WriteLine("#include \"" + includeFileName + "\"");
                            }
                        }
                    }
                }
                rootTextWriter.Flush();
                rootFileStream.Flush();
                rootTextWriter.Close();
                rootFileStream.Close();
            }

            using (var textWriter2 = new StreamWriter(outputFileName, false, encoding))
            {
                textWriter2.WriteLine("#include \"text/root.txt\"");
                textWriter2.Flush();
                textWriter2.Close();
            }
        }
    }
}
