using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Text;
using System.Globalization;
using System.Windows.Forms;

namespace AinDecompiler
{
    [Flags]
    public enum StringExclusionReason
    {
        None = 0x00,
        IsFunctionName = 0x01,
        IsInsideTestFunction = 0x02,
        IsInsideSystemFunction = 0x04,
        IsUnused = 0x08,
        IsAssert = 0x10,
        IsEvent = 0x20,
    }

    public static class StringExclusionReasonExtensions
    {
        public static string GetText(this StringExclusionReason exclusionReason)
        {
            return StringExportImport.GetExcludeReasonText(exclusionReason);
        }
    }

    public class StringExportImport
    {
        AinFile ainFile;
        public StringExportImport(AinFile ainFile)
        {
            this.ainFile = ainFile;
        }
        public Dictionary<int, StringExclusionReason> stringsToExclude = new Dictionary<int, StringExclusionReason>();

        public bool ApplyFiltersOnImport;
        public bool ExcludeFunctionNames;
        public bool ExcludeUnusedStrings;
        public bool ExcludeTestFunctions;
        public bool ExcludeSystemFunctions;
        public bool ExcludeAsserts;
        public bool ExcludeEventFlags;
        public bool KeepExcludedAsComments;

        /// <summary>
        /// Gets the number of the first message, for compatiblity with the other export/import tool
        /// </summary>
        /// <returns></returns>
        public int GetFirstMessageIdNumber()
        {
            //VERS int
            //KEYC int
            //CODE byte array
            //FUNC function name, each argument name
            //GLOB variable name
            //GSET initial string value if it exists
            //STRT struct name, each member name
            //MSG0 all messages
            int functionsSum = ainFile.Functions.Sum(f => f.Parameters.Count + 1);
            int globalsSum = ainFile.Globals.Count;
            int globalSettingsSum = ainFile.GlobalInitialValues.Sum(gs => (gs.StringValue == null) ? 0 : 1);
            int structsSum = ainFile.Structs.Sum(s => s.Members.Count + 1);
            int allSum = functionsSum + globalsSum + globalSettingsSum + structsSum;
            return allSum;
        }

        /// <summary>
        /// Gets the number of the first string, for compatiblity with the other export/import tool
        /// </summary>
        /// <returns></returns>
        public int GetFirstStringIdNumber()
        {
            //MAIN int
            //MSGF int
            //HLL0 library names, function names, argument names
            //SWI0 nothing here but string indexes
            //GVER int
            //STR0 all strings
            int firstSum = GetFirstMessageIdNumber() + ainFile.Messages.Count;
            int librariesSum = ainFile.Libraries.Sum(l => 1 + l.Functions.Sum(f => 1 + f.Parameters.Count));
            int allSum = firstSum + librariesSum;
            return allSum;
        }

        public int GetFirstGlobalGroupIdNumber()
        {
            //STR0 all strings
            //FNAM
            //OJMP int
            //FNCT (not counted)
            //DELG (not counted)
            //OBJG <--
            int stringsSum = GetFirstStringIdNumber() + ainFile.Strings.Count;
            int filenames = ainFile.Filenames.Count;
            //int functionTypesSum = ainFile.FunctionTypes.Sum(f => 1 + f.Arguments.Length);
            //int delegatesSum = ainFile.Delegates.Sum(f => 1 + f.Arguments.Length);
            return stringsSum + filenames;
        }

        int exclusionFlags = -1;
        bool alreadyBuilt = false;

        public void GetExclusionList()
        {
            this.exclusionFlags = (this.ExcludeFunctionNames ? (int)StringExclusionReason.IsFunctionName : 0) |
                (this.ExcludeSystemFunctions ? (int)StringExclusionReason.IsInsideSystemFunction : 0) |
                (this.ExcludeTestFunctions ? (int)StringExclusionReason.IsInsideTestFunction : 0) |
                (this.ExcludeUnusedStrings ? (int)StringExclusionReason.IsUnused : 0) |
                (this.ExcludeAsserts ? (int)StringExclusionReason.IsAssert : 0) |
                (this.ExcludeEventFlags ? (int)StringExclusionReason.IsEvent : 0);

            if (alreadyBuilt)
            {
                return;
            }
            alreadyBuilt = true;

            //if (currentExclusionFlags == newExclusionFlags)
            //{
            //    return;
            //}

            //currentExclusionFlags = newExclusionFlags;
            stringsToExclude.Clear();

            //build list of "System" functions
            var functionEnumerator = new FunctionEnumerator(ainFile);
            var filesAndFunctions = functionEnumerator.GetFilesAndFunctions();
            var systemFiles = filesAndFunctions.Where(f => f.name.StartsWith("System\\", StringComparison.OrdinalIgnoreCase));
            var systemFunctions = new HashSet<int>(systemFiles.SelectMany(f => f.children).Select(res => res.id));

            Dictionary<int, HashSet<int>> assertStringNumberToFunctionIndex = GetStringsUsedInAsserts();

            HashSet<Function> functionsThatCallEv = new HashSet<Function>();
            //get functions that call "EV()"
            var evFunction = ainFile.GetFunction("EV");
            if (evFunction != null)
            {
                functionsThatCallEv = ainFile.CodeSearchCache.FunctionsCache.GetUsedBy(evFunction);
            }

            //Is it used as part of a function call to EV?
            foreach (var function in functionsThatCallEv)
            {
                int address = function.Address;
                int codeLength = ainFile.Code.Length;
                int evFunctionIndex = evFunction.Index;
                int evFunctionArgs = evFunction.ParameterCount;
                int evFunctionOffset = evFunctionArgs * 6;
                while (address < codeLength)
                {
                    var instructionInfo = Decompiler.Peek(ainFile.Code, address);
                    if (instructionInfo.instruction == Instruction.CALLFUNC && instructionInfo.word1 == evFunctionIndex)
                    {
                        var ins1 = Decompiler.Peek(ainFile.Code, instructionInfo.CurrentAddress - evFunctionOffset);
                        if (ins1.instruction == Instruction.S_PUSH)
                        {
                            stringsToExclude.SetBit(ins1.word1, StringExclusionReason.IsEvent);
                        }
                    }
                    else if (instructionInfo.instruction == Instruction.FUNC || instructionInfo.instruction == Instruction.ENDFUNC)
                    {
                        break;
                    }
                    address = instructionInfo.nextAddress;
                }

                //var expression = ainFile.DecompiledCodeCache.GetDecompiledCode(function);
                //var childExpressions = expression.GetChildExpressions();
                //var evCalls = childExpressions.Where(e => e.ExpressionType == Instruction.CALLFUNC && e.Variable == evFunction);
                //var stringNumbers = evCalls.SelectMany(evCall => evCall.GetChildExpressions())
                //    .Where(e => e.ExpressionType == Instruction.S_PUSH)
                //    .Select(e => e.Value);
                //foreach (var stringNumber in stringNumbers)
                //{
                //    stringsToExclude.SetBit(stringNumber, ExclusionReason.IsEvent);
                //}
            }

            for (int i = 0; i < ainFile.Strings.Count; i++)
            {
                string str = ainFile.Strings[i];
                if (String.IsNullOrEmpty(str)) continue;
                //Is it a function name?
                if (ainFile.FunctionNameToIndex.ContainsKey(str))
                {
                    stringsToExclude.SetBit(i, StringExclusionReason.IsFunctionName);
                }
                var usedBy = ainFile.CodeSearchCache.StringsCache.GetUsedBy(str);
                //Is it used only by a function containing TEST or DEBUG in its name?
                if (usedBy.All(f => f.Name.Contains("テスト") || f.Name.Contains("デバッグ"))) //TEST or DEBUG
                {
                    stringsToExclude.SetBit(i, StringExclusionReason.IsInsideTestFunction);
                }
                //Is it used only by a system function?
                if (usedBy.All(f => systemFunctions.Contains(f.Index)))
                {
                    stringsToExclude.SetBit(i, StringExclusionReason.IsInsideSystemFunction);
                }
                //Is it never used?
                if (usedBy.Count == 0)
                {
                    stringsToExclude.SetBit(i, StringExclusionReason.IsUnused);
                }
                //Is it used only in an assert?
                if (assertStringNumberToFunctionIndex.ContainsKey(i))
                {
                    var assertFunctionIndexes = assertStringNumberToFunctionIndex[i];
                    var usedByFunctionIndexes = usedBy.Select(f => f.Index);
                    if (assertFunctionIndexes.SetEquals(usedByFunctionIndexes))
                    {
                        stringsToExclude.SetBit(i, StringExclusionReason.IsAssert);
                    }
                }

            }

            if (stringsToExclude.ContainsKey(0))
            {
                stringsToExclude.Remove(0);
            }



            ////part 1: function names
            //if (ExcludeFunctionNames)
            //{
            //    for (int i = 0; i < ainFile.Strings.Count; i++)
            //    {
            //        string str = ainFile.Strings[i];
            //        if (ainFile.FunctionNameToIndex.ContainsKey(str))
            //        {
            //            stringsToExclude.SetBit(i, ExclusionReason.IsFunctionName);
            //        }
            //    }
            //}

            ////part 2: Test functions
            //if (ExcludeTestFunctions)
            //{
            //    for (int i = 0; i < ainFile.Strings.Count; i++)
            //    {
            //        string str = ainFile.Strings[i];
            //        var usedBy = ainFile.CodeSearchCache.StringsCache.GetUsedBy(str);
            //        if (usedBy.All(f => f.Name.Contains("テスト") || f.Name.Contains("デバッグ"))) //TEST or DEBUG
            //        {
            //            stringsToExclude.SetBit(i, ExclusionReason.IsInsideTestFunction);
            //        }
            //    }
            //}

            ////part 3: System functions
            //if (ExcludeSystemFunctions)
            //{
            //    for (int i = 0; i < ainFile.Strings.Count; i++)
            //    {
            //        string str = ainFile.Strings[i];
            //        var usedBy = ainFile.CodeSearchCache.StringsCache.GetUsedBy(str);
            //        if (usedBy.All(f => systemFunctions.Contains(f.Index)))
            //        {
            //            stringsToExclude.SetBit(i, ExclusionReason.IsInsideSystemFunction);
            //        }
            //    }
            //}

            ////part 4: Unused strings
            //if (ExcludeUnusedStrings)
            //{
            //    for (int i = 0; i < ainFile.Strings.Count; i++)
            //    {
            //        string str = ainFile.Strings[i];
            //        var usedBy = ainFile.CodeSearchCache.StringsCache.GetUsedBy(str);
            //        if (usedBy.Count == 0)
            //        {
            //            stringsToExclude.SetBit(i, ExclusionReason.IsUnused);
            //        }
            //    }
            //}
        }

        private Dictionary<int, HashSet<int>> GetStringsUsedInAsserts()
        {
            Dictionary<int, HashSet<int>> assertStringNumberToFunctionIndex = new Dictionary<int, HashSet<int>>();
            //get strings that are used by an assert instruction
            {
                foreach (var function in ainFile.Functions)
                {
                    int address = function.Address;
                    int codeLength = ainFile.Code.Length;
                    while (address < codeLength)
                    {
                        var instructionInfo = Decompiler.Peek(ainFile.Code, address);
                        if (instructionInfo.instruction == Instruction.ASSERT)
                        {
                            //s_push, s_push, push, assert
                            var ins1 = Decompiler.Peek(ainFile.Code, instructionInfo.CurrentAddress - 18);
                            var ins2 = Decompiler.Peek(ainFile.Code, instructionInfo.CurrentAddress - 12);
                            var ins3 = Decompiler.Peek(ainFile.Code, instructionInfo.CurrentAddress - 6);
                            if (ins1.instruction == Instruction.S_PUSH && ins2.instruction == Instruction.S_PUSH && ins3.instruction == Instruction.PUSH)
                            {
                                assertStringNumberToFunctionIndex.Set(ins1.word1, function.Index);
                                assertStringNumberToFunctionIndex.Set(ins2.word1, function.Index);
                            }
                        }
                        else if (instructionInfo.instruction == Instruction.FUNC || instructionInfo.instruction == Instruction.ENDFUNC)
                        {
                            break;
                        }
                        address = instructionInfo.nextAddress;
                    }
                }
            }
            return assertStringNumberToFunctionIndex;
        }

        //private bool StringIsOnlyUsedByTestFunctions(string str)
        //{
        //    var usedBy = ainFile.CodeSearchCache.StringsCache.GetUsedBy(str);
        //    bool usedByTestFunction = usedBy.All(f => f.Name.Contains("テスト"));
        //    return usedByTestFunction;
        //}


        Dictionary<int, string> lineNumbersAndStrings = new Dictionary<int, string>();
        void ParseTextFile(string[] lines)
        {
            lineNumbersAndStrings.Clear();
            foreach (var originalLine in lines)
            {
                string line = originalLine;
                {
                    int commentIndex = line.IndexOf('#');
                    if (commentIndex >= 0)
                    {
                        line = line.Substring(0, commentIndex);
                    }
                    if (line.Length == 0) continue;
                }
                line = UnescapeText(line);

                int indexOfFirstSpace = line.IndexOf(' ');
                if (indexOfFirstSpace >= 0)
                {
                    string numberString = line.Substring(0, indexOfFirstSpace);
                    string restOfLine = line.Substring(indexOfFirstSpace + 1);
                    int lineNumber = -1;
                    if (int.TryParse(numberString, NumberStyles.None, CultureInfo.InvariantCulture, out lineNumber))
                    {
                        lineNumbersAndStrings.Set(lineNumber, restOfLine);
                    }
                }
            }
        }

        public void ImportTextFile(string fileName, string outputFileName)
        {
            var encoding = EncodingDetector.DetectEncoding(fileName);
            string[] lines = File.ReadAllLines(fileName, encoding);

            ParseTextFile(lines);
            int firstMessageIdNumber = GetFirstMessageIdNumber();
            int lastMessageIdNumber = firstMessageIdNumber + ainFile.Messages.Count - 1;
            int firstStringIdNumber = GetFirstStringIdNumber();
            int lastStringIdNumber = firstStringIdNumber + ainFile.Strings.Count - 1;

            //old system also exported filenames and global group names, we never want to change these, but don't throw error messages if these are seen.
            int firstExtraNumber = firstStringIdNumber + ainFile.Strings.Count;
            int lastExtraNumber = firstExtraNumber + ainFile.Filenames.Count + ainFile.GlobalGroupNames.Count - 1;

            foreach (var key in lineNumbersAndStrings.Keys)
            {
                if (!((key >= firstMessageIdNumber && key <= lastMessageIdNumber) ||
                    (key >= firstStringIdNumber && key <= lastStringIdNumber) ||
                    (key >= firstExtraNumber && key <= lastExtraNumber)))
                {
                    string prompt = "Invalid line number " + key + "." + "\r\n" +
                        "Line numbers must be between " + firstMessageIdNumber + " and " + lastMessageIdNumber + " (messages) " +
                        "or between " + firstStringIdNumber + " and " + lastStringIdNumber + " (strings)." + "\r\n" +
                        "Import aborted.";
                    return;
                }
            }


            var ainCopy = ainFile.Clone();

            GetExclusionList();

            foreach (var pair in lineNumbersAndStrings)
            {
                string text = pair.Value;
                int idNumber = pair.Key;
                int messageIndex = idNumber - firstMessageIdNumber;
                int stringIndex = idNumber - firstStringIdNumber;
                if (messageIndex >= 0 && messageIndex < ainCopy.Messages.Count)
                {
                    ainCopy.Messages[messageIndex] = text;
                }
                if (stringIndex >= 0 && stringIndex < ainCopy.Strings.Count)
                {
                    if (ApplyFiltersOnImport && stringsToExclude.ContainsKey(stringIndex))
                    {
                        //it's filtered, do nothing
                    }
                    else
                    {
                        ainCopy.Strings[stringIndex] = text;
                    }
                }
            }

            ainCopy.WriteAndEncryptAinFile(outputFileName);
        }

        public void ExportTextFile(string outputFileName)
        {
            List<string> lines = new List<string>();
            ExportMessages(lines);
            lines.Add("");
            ExportStrings(lines);

            File.WriteAllLines(outputFileName, lines.ToArray(), Encoding.UTF8);  //utf8 with BOM
        }

        public void ExportStringsOnly(string outputFileName)
        {
            List<string> lines = new List<string>();
            ExportStrings(lines);

            File.WriteAllLines(outputFileName, lines.ToArray(), Encoding.UTF8);  //utf8 with BOM
        }

        public void ExportMessagesOnly(string outputFileName)
        {
            List<string> lines = new List<string>();
            ExportMessages(lines);

            File.WriteAllLines(outputFileName, lines.ToArray(), Encoding.UTF8);  //utf8 with BOM
        }

        private void ExportStrings(List<string> lines)
        {
            lines.Add("#STR0");
            GetExclusionList();
            int firstStringIdNumber = GetFirstStringIdNumber();
            for (int i = 0; i < ainFile.Strings.Count; i++)
            {
                string text = ainFile.Strings[i];
                text = EscapeText(text);
                int stringIdNumber = i + firstStringIdNumber;
                string idText = stringIdNumber.ToString() + " ";
                string line = idText + text;
                if (stringsToExclude.ContainsKey(i) && (0 != (stringsToExclude[i] & (StringExclusionReason)exclusionFlags)))
                {
                    if (this.KeepExcludedAsComments)
                    {
                        var reason = stringsToExclude[i];
                        string reasonText = GetExcludeReasonText(reason);
                        line = "#" + line + "#\t\t" + reasonText;
                        lines.Add(line);
                    }
                }
                else
                {
                    lines.Add(line);
                }
            }
        }

        private void ExportMessages(List<string> lines)
        {
            lines.Add("#MSG0");
            int firstMessageIdNumber = GetFirstMessageIdNumber();
            for (int i = 0; i < ainFile.Messages.Count; i++)
            {
                string text = ainFile.Messages[i];
                text = EscapeText(text);
                int stringIdNumber = i + firstMessageIdNumber;
                string idText = stringIdNumber.ToString() + " ";
                string line = idText + text;
                lines.Add(line);
            }
        }

        public static string EscapeText(string text)
        {
            text = text.Replace("#", "*23");
            text = text.Replace("\r", "*0D");
            text = text.Replace("\n", "*0A");
            text = text.Replace("\t", "*09");
            return text;
        }

        public static string UnescapeText(string text)
        {
            text = text.Replace("*23", "#");
            text = text.Replace("*0A", "\n");
            text = text.Replace("*0D", "\r");
            text = text.Replace("*09", "\t");
            return text;
        }

        public static string GetExcludeReasonText(StringExclusionReason reason)
        {
            //string reasonText = "";
            if (0 != (reason & StringExclusionReason.IsFunctionName))
                return "matches a function name";
            if (0 != (reason & StringExclusionReason.IsInsideSystemFunction))
                return "system function";
            if (0 != (reason & StringExclusionReason.IsInsideTestFunction))
                return "test function";
            if (0 != (reason & StringExclusionReason.IsUnused))
                return "not used";
            if (0 != (reason & StringExclusionReason.IsAssert))
                return "assert string";
            if (0 != (reason & StringExclusionReason.IsEvent))
                return "event flag name";
            return "";
            //return reasonText;
        }
    }
}
