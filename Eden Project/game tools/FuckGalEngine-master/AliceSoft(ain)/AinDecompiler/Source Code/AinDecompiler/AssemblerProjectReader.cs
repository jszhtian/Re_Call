using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Text;
using System.CodeDom.Compiler;
using System.Diagnostics;
using System.Globalization;

namespace AinDecompiler
{
    public class ParseError : Exception
    {
        public ParseError(string errorMessage)
            : base(errorMessage)
        {

        }
    }

    public partial class AssemblerProjectReader
    {
        /// <summary>
        /// The background worker, so we can report progress and check for cancellation
        /// </summary>
        public MyBackgroundWorker worker = null;
        /// <summary>
        /// The stopwatch, so we can see if we have run for 250ms, then need to report progress.
        /// </summary>
        private TinyStopwatch stopwatch = new TinyStopwatch();
        /// <summary>
        /// Whether or not the background worker wants to cancel
        /// </summary>
        bool Cancel
        {
            get
            {
                return worker != null && worker.CancellationPending;
            }
        }

        /// <summary>
        /// The code that is being written
        /// </summary>
        MemoryStream code = new MemoryStream();
        /// <summary>
        /// The binary writer for the code that is being written
        /// </summary>
        BinaryWriter bw = null;
        /// <summary>
        /// The list of Filenames for the AIN file
        /// </summary>
        List<string> filenames = new List<string>();
        /// <summary>
        /// A dictionary that relates filenames with their array index
        /// </summary>
        Dictionary<string, int> FilenameToIndex = new Dictionary<string, int>();
        /// <summary>
        /// The list of functions for the AIN file
        /// </summary>
        List<Function> functions = new List<Function>();
        /// <summary>
        /// The list of function types for the AIN file
        /// </summary>
        List<FunctionType> functionTypes = new List<FunctionType>();
        /// <summary>
        /// The list of delegate types for the AIN file
        /// </summary>
        List<FunctionType> delegates = new List<FunctionType>();
        /// <summary>
        /// The GameVersion variable for the AIN file
        /// </summary>
        int gameVersion;
        /// <summary>
        /// The list of global variables for the AIN file
        /// </summary>
        List<Global> globals = new List<Global>();
        /// <summary>
        /// The list of initial values for global variables for the AIN file
        /// </summary>
        List<GlobalInitialValue> globalInitialValues = new List<GlobalInitialValue>();
        /// <summary>
        /// The list of libraries for the AIN file
        /// </summary>
        List<HllLibrary> libraries = new List<HllLibrary>();
        /// <summary>
        /// The encryption key for the AIN file (0 for newer versions)
        /// </summary>
        int keyCode;
        /// <summary>
        /// The main function number for the AIN file
        /// </summary>
        int main;
        /// <summary>
        /// The list of messages for the AIN file
        /// </summary>
        List<string> messages = new List<string>();
        /// <summary>
        /// The function number to call for a message for the AIN file.
        /// </summary>
        int msgf;
        /// <summary>
        /// The list of global variables groups for the AIN file
        /// </summary>
        List<string> globalGroupNames = new List<string>();
        /// <summary>
        /// The OJMP for the AIN file (no clue what this does)
        /// </summary>
        int ojmp;
        /// <summary>
        /// The list of strings for the AIN file
        /// </summary>
        List<string> strings = new List<string>();
        /// <summary>
        /// The list of structs for the AIN file
        /// </summary>
        List<Struct> structs = new List<Struct>();
        /// <summary>
        /// The list of switch blocks for the AIN file
        /// </summary>
        List<SwitchBlock> switches = new List<SwitchBlock>();
        /// <summary>
        /// The AIN version for the AIN file
        /// </summary>
        int version;
        /// <summary>
        /// Whether or not to output a compressed AI2 file or not
        /// </summary>
        bool IsAi2File;
        /// <summary>
        /// The length of the FNCT data, minus 4.  Previously called "dummy".
        /// </summary>
        int fnctLength;

        /// <summary>
        /// Counter for the number of FUNC instructions seen, used for reporting progress  (divides by total number of functions * 2)
        /// </summary>
        int funcInstructionsSeen = 0;

        /// <summary>
        /// List index is function number, then it's a dictionary that maps local variable names to its index in the function
        /// </summary>
        List<Dictionary<string, int>> FunctionVariablesInverse = new List<Dictionary<string, int>>();
        /// <summary>
        /// List index is struct number, then it's a dictionary that maps struct member names to its index in the struct
        /// </summary>
        List<Dictionary<string, int>> StructVariablesInverse = new List<Dictionary<string, int>>();

        /// <summary>
        /// A dictionary that maps a struct's name to its index in the Structs list
        /// </summary>
        Dictionary<string, int> StructNameToIndex = new Dictionary<string, int>();
        /// <summary>
        /// A dictionary that maps a function's name to its index in the Functions list
        /// </summary>
        Dictionary<string, int> FunctionNameToIndex = new Dictionary<string, int>();
        /// <summary>
        /// A dictionary that maps a function type's name to its index in the FunctionTypes list
        /// </summary>
        Dictionary<string, int> FunctionTypeNameToIndex = new Dictionary<string, int>();
        /// <summary>
        /// A dictionary that maps a delegate's name to its index in the Delegates list
        /// </summary>
        Dictionary<string, int> DelegateNameToIndex = new Dictionary<string, int>();
        /// <summary>
        /// A dictionary that maps a global variable's name to its index in the Globals list.  Both dotted and non-dotted forms are mapped.
        /// </summary>
        Dictionary<string, int> GlobalNameToIndex = new Dictionary<string, int>();
        /// <summary>
        /// A dictionary that maps a global group name to its index in the Global group name list
        /// </summary>
        Dictionary<string, int> GlobalGroupNameToIndex = new Dictionary<string, int>();
        /// <summary>
        /// A dictionary that maps a library's name to its index in the Libraries list
        /// </summary>
        Dictionary<string, int> LibraryNameToIndex = new Dictionary<string, int>();
        /// <summary>
        /// A dictionary that maps a library function's name (with dot) to its library index and library function index.
        /// </summary>
        Dictionary<string, KeyValuePair<int, int>> LibraryFunctionNameToIndex = new Dictionary<string, KeyValuePair<int, int>>();

        /// <summary>
        /// A dictionary that maps a string to its index in the Strings list
        /// </summary>
        Dictionary<string, int> StringToIndex = new Dictionary<string, int>();
        /// <summary>
        /// A dictionary that maps a message to its index in the Messages list
        /// </summary>
        Dictionary<string, int> MessageToIndex = new Dictionary<string, int>();
        /// <summary>
        /// First key is the string, then it's a dictionary mapping the duplicate number to the string id.
        /// </summary>
        Dictionary<string, Dictionary<int, int>> DuplicateStringToIndex = new Dictionary<string, Dictionary<int, int>>();
        /// <summary>
        /// First key is the message, then it's a dictionary mapping the duplicate number to the message id.
        /// </summary>
        Dictionary<string, Dictionary<int, int>> DuplicateMessageToIndex = new Dictionary<string, Dictionary<int, int>>();

        //Dictionary<string, int> ReservedFunctions = new Dictionary<string, int>();
        //Dictionary<int, string> ReservedFunctionIndexes = new Dictionary<int, string>();

        /// <summary>
        /// The text reader we are reading the tokens from.  May be multiple files.
        /// </summary>
        MyTextReader tr = new MyTextReader();

        /// <summary>
        /// Indicates whether or not we are on the first pass, and are computing addresses of code, so we're not outputting anything.  On the first pass, missing names are not a problem.
        /// </summary>
        bool firstPass;

        /// <summary>
        /// A dictionary that maps lowercase instruction names to their enumeration value.
        /// </summary>
        static Dictionary<string, Instruction> InstructionNamesLowercase = BuildInstructionNamesDictionary();
        /// <summary>
        /// A dictionary that maps data type names to their data types.
        /// </summary>
        static Dictionary<string, DataType> DataTypeNames = BuildDataTypeNames();

        /// <summary>
        /// The list of errors that have been encountered.  If this has at least one item, a build has failed.  When this reaches 1500 items, the process aborts.
        /// </summary>
        List<string> errors = new List<string>();

        /// <summary>
        /// The last class number seen.  Used for composite instructions that refer to more than one struct/class.
        /// </summary>
        int classNumber;

        public AssemblerProjectReader()
        {
            bw = new BinaryWriter(code);
            this.stopwatch.Start();
        }

        private void AfterParse()
        {
            for (int i = 0; i < this.structs.Count; i++)
            {
                var structInfo = this.structs[i];
                string constructorName = structInfo.Name + "@0";
                string destructorName = structInfo.Name + "@1";
                if (FunctionNameToIndex.ContainsKey(constructorName))
                {
                    structInfo.Constructor = FunctionNameToIndex[constructorName];
                }
                if (FunctionNameToIndex.ContainsKey(destructorName))
                {
                    structInfo.Destructor = FunctionNameToIndex[destructorName];
                }
            }
        }

        public AinFile MakeAinFile()
        {
            AinFile ainFile = new AinFile();
            ainFile.Code = this.code.ToArray();
            ainFile.Filenames.AddRange(this.filenames);
            ainFile.FunctionNameToIndex = this.FunctionNameToIndex;
            ainFile.GlobalNameToIndex = this.GlobalNameToIndex;
            ainFile.Functions.AddRange(this.functions);
            ainFile.FunctionTypes.AddRange(this.functionTypes);
            ainFile.Delegates.AddRange(this.delegates);
            //if (ainFile.Delegates.Length == 0)
            //{
            //    ainFile.Delegates = null;
            //}
            ainFile.GameVersion = this.gameVersion;
            ainFile.GlobalInitialValues.AddRange(this.globalInitialValues);
            ainFile.GlobalGroupNames.AddRange(this.globalGroupNames);
            ainFile.Globals.AddRange(this.globals);
            ainFile.Libraries.AddRange(this.libraries);
            ainFile.KeyCode = this.keyCode;
            ainFile.MAIN = this.main;
            ainFile.Messages.AddRange(this.messages);
            ainFile.MSGF = this.msgf;
            ainFile.OJMP = this.ojmp;
            ainFile.Strings.AddRange(this.strings);
            ainFile.StructNameToIndex = this.StructNameToIndex;
            ainFile.Structs.AddRange(this.structs);
            ainFile.Switches.AddRange(this.switches);
            ainFile.Version = this.version;
            ainFile.IsAi2File = this.IsAi2File;

            ainFile.ApplyIndexesAndParents();

            return ainFile;
        }

        private static Dictionary<string, Instruction> BuildInstructionNamesDictionary()
        {
            var InstructionNamesLowercase = new Dictionary<string, Instruction>();
            var names = Enum.GetNames(typeof(Instruction))
                .Concat(Enum.GetNames(typeof(OldInstructions)))
                .ToArray();
            var values = Enum.GetValues(typeof(Instruction)).Cast<Instruction>()
                .Concat(Enum.GetValues(typeof(OldInstructions)).Cast<Instruction>())
                .ToArray();
            for (int i = 0; i < names.Length; i++)
            {
                string name = names[i].ToLowerInvariant();
                Instruction value = values[i];

                if ((int)value >= 0 && !InstructionNamesLowercase.ContainsKey(name))
                {
                    InstructionNamesLowercase.Add(name, value);
                }
            }

            //older name
            InstructionNamesLowercase.Add("cmd_switch", Instruction.SWITCH);

            return InstructionNamesLowercase;
        }

        private static Dictionary<string, DataType> BuildDataTypeNames()
        {
            var DataTypeNames = new Dictionary<string, DataType>();
            var names = Enum.GetNames(typeof(DataType))
                .Concat(Enum.GetNames(typeof(OldDataTypes)))
                .ToArray();
            var values = Enum.GetValues(typeof(DataType)).Cast<DataType>()
                .Concat(Enum.GetValues(typeof(OldDataTypes)).Cast<DataType>())
                .ToArray();
            for (int i = 0; i < names.Length; i++)
            {
                string name = names[i];
                DataType value = values[i];
                if (name == "IMainSystem") name = "Imainsystem";

                if ((int)value >= 0 && !DataTypeNames.ContainsKey(name))
                {
                    DataTypeNames.Add(name, value);
                }
            }
            return DataTypeNames;
        }


        public bool LoadProject(string path)
        {
            //Directory.SetCurrentDirectory(Path.GetDirectoryName(path));

            BeginFirstPass();
            IncludeFile(path);
            ParseFile();

            if (Cancel || errors.Count > 0)
            {
                return false;
            }

            BeginSecondPass();
            tr.ReplayLog();
            ParseFile();

            if (Cancel || errors.Count > 0)
            {
                return false;
            }

            BeginSecondPass();
            IncludeFile(path);
            ParseFile();

            if (Cancel || errors.Count > 0)
            {
                return false;
            }

            AfterParse();

            return true;
        }

        /// <summary>
        /// When reading structs, the highest struct index found so far.  The next struct will be written to at this index in the Structs array.
        /// </summary>
        int currentStruct;
        /// <summary>
        /// When reading switch blocks, the highest switch block index found so far.  The next switch block will be written to at this index in the Switch Blocks array.
        /// </summary>
        int currentSwitch;
        /// <summary>
        /// When reading global variables, the highest global variable index found so far.  The next global variable will be written to at this index in the Globals array.
        /// </summary>
        int currentGlobal;
        /// <summary>
        /// When reading function types, the highest function type index found so far.  The next function type will be written to at this index in the Function Types array.
        /// </summary>
        int currentFunctionType;
        /// <summary>
        /// When reading delegates, the highest delegate index found so far.  The next delegate will be written to at this index in the Delegates array.
        /// </summary>
        int currentDelegate;
        /// <summary>
        /// When reading functions (from a 'functions' block), the highest function index found so far.  The next function will be written to at this index in the Flobals array.
        /// </summary>
        int nextFunction;

        /// <summary>
        /// When building code, this is the most recently encountered FUNC instruction.  Local variables start from this function.
        /// </summary>
        int activeFunction;
        /// <summary>
        /// When reading filenames, the highest filename index found so far.  The next filename will be written to at this index in the Filenames array.
        /// </summary>
        int currentFilename;
        /// <summary>
        /// The current address to output code to and create labels at.
        /// </summary>
        int currentAddress;

        //int currentMessage;
        //int currentString;

        /// <summary>
        /// When reading global initial values, the highest global initial value index found so far.  The next global initial value will be written to at this index in the GlobalInitialValues array.
        /// </summary>
        int currentGlobalInitialValue;
        /// <summary>
        /// When reading global variable groups, the highest global variable group index found so far.  The next global variable group will be written to at this index in the GlobalGroupNames array.
        /// </summary>
        int currentGroupIndex;

        /// <summary>
        /// The list of discovered labels.  Built on the first pass, used on the second pass.
        /// </summary>
        Dictionary<string, int> Labels = new Dictionary<string, int>();

        /// <summary>
        /// When building code, this is the class number that corresponds to the most recently encountered FUNC instruction, or a value of -1 if the function is not a class method.
        /// </summary>
        int currentClassNumber;

        /// <summary>
        /// When reading an instruction with a StructType argument kind, makes the next PUSH instruction be a member of that struct instead of a number.
        /// </summary>
        int lastSeenStructType;

        private void BeginFirstPass()
        {
            firstPass = true;
            currentFilename = 0;
            currentFunctionType = 0;
            currentDelegate = 0;
            currentGlobal = 0;
            currentGlobalInitialValue = 0;
            currentStruct = 0;
            currentSwitch = 0;
            //currentMessage = 0;
            //currentString = 0;
            currentGroupIndex = 0;
            currentAddress = 0;
            nextFunction = 0;

            activeFunction = -1;
            currentClassNumber = -1;

            AddString("", -1);
            AddMessage("", -1);
            //ReservedFunctions.Add("NULL", 0);
            //ReservedFunctionIndexes.Add(0, "NULL");
        }

        private int AddMessage(string str, int dupeId)
        {
            return AddStr(str, dupeId, messages, MessageToIndex, DuplicateMessageToIndex, firstPass);
        }

        private int AddString(string str, int dupeId)
        {
            return AddStr(str, dupeId, strings, StringToIndex, DuplicateStringToIndex, firstPass);
        }

        private static int AddStr(string str, int dupeId, List<string> Strings, Dictionary<string, int> StringToIndex, Dictionary<string, Dictionary<int, int>> DuplicateStringToIndex, bool firstPass)
        {
            if (dupeId < 1)
            {
                if (StringToIndex.ContainsKey(str))
                {
                    return StringToIndex[str];
                }
                else
                {
                    int index = Strings.Count;
                    Strings.Add(str);
                    StringToIndex.Set(str, index);
                    return index;
                }
            }
            else
            {
                var dic = DuplicateStringToIndex.GetOrNew(str);
                if (dic.ContainsKey(dupeId))
                {
                    return dic[dupeId];
                }
                int index = Strings.Count;
                Strings.Add(str);
                dic.Add(dupeId, index);
                DuplicateStringToIndex.Set(str, dic);
                return index;
            }
        }

        void BeginSecondPass()
        {
            firstPass = false;
            activeFunction = 0;
            currentFilename = 0;
            this.FilenameToIndex.Clear();
            nextFunction = 0;
            currentFunctionType = 0;
            currentDelegate = 0;
            currentGlobal = 0;
            currentGlobalInitialValue = 0;
            currentStruct = 0;
            currentSwitch = 0;
            currentAddress = 0;
            //currentMessage = 0;
            //currentString = 0;
            currentGroupIndex = 0;
        }

        private void ParseFile()
        {
            while (true)
            {
                //report progress
                if ((stopwatch.ElapsedTime > 250) && worker != null)
                {
                    if (Cancel)
                    {
                        break;
                    }
                    stopwatch.Start();
                    int progress = 0;
                    if (functions.Count > 0)
                    {
                        progress = (funcInstructionsSeen * 100) / (functions.Count * 2);
                    }
                    worker.ReportProgress(progress, errors);
                }

                Token token = tr.ReadToken();
                if (Object.ReferenceEquals(token, null))
                {
                    break;
                }
                try
                {
                    HandleToken(token);
                }
                catch (FormatException ex)
                {
                    errors.Add(ex.Message);
                    if (errors.Count >= 1500)
                    {
                        errors.Add("Too many errors, aborting!");
                        break;
                    }
                }
            }
            if (worker != null)
            {
                stopwatch.Start();
                int progress = 0;
                if (functions.Count > 0)
                {
                    progress = (funcInstructionsSeen * 100) / (functions.Count * 2);
                }
                worker.ReportProgress(progress, errors);
            }
        }

        private bool HandleToken(Token token)
        {
            var nextToken = tr.PeekToken();
            if (Object.ReferenceEquals(nextToken, null))
            {
                nextToken = new Token("");
            }

            if (nextToken == ':')
            {
                tr.ReadToken();
                string labelName = token.Value;
                Labels.Set(labelName, currentAddress);
            }
            else if (InstructionNamesLowercase.ContainsKey(token.AsLowercase) && token.QuoteCharacter == 0)
            {
                var instruction = InstructionNamesLowercase[token.AsLowercase];
                ReadInstruction(instruction);
            }
            else if (token.QuoteCharacter == '\'' || (nextToken.QuoteCharacter == '\'' && token.QuoteCharacter == 0 && token.IsNumber()))
            {
                //it's a message
                tr.PutbackToken(token);
                ReadInstruction(Instruction.MSG);

                token = tr.PeekToken();
                if (FunctionNameToIndex.ContainsKey(token.Value))
                {
                    ReadInstruction(Instruction.CALLFUNC);
                }
                else
                {
                    token = tr.ReadToken();
                    if (token.Value.Length > 1)
                    {

                    }

                    if (!HandleToken(token))
                    {
                        if (!firstPass)
                        {
                            return false;
                        }
                        else
                        {
                            tr.PutbackToken(token);
                            ReadInstruction(Instruction.CALLFUNC);
                        }
                    }
                }
            }
            else if (token.AsLowercase == "struct")
            {
                StartLogging(token);
                ReadStruct();
                StopLogging();
            }
            else if (token.AsLowercase == "globals")
            {
                StartLogging(token);
                ReadGlobals();
                StopLogging();
            }
            else if (token.AsLowercase == "globalgroupnames")
            {
                StartLogging(token);
                ReadGlobalGroupNames();
                StopLogging();
            }
            else if (token.AsLowercase == "functype")
            {
                StartLogging(token);
                ReadFunctionType();
                StopLogging();
            }
            else if (token.AsLowercase == "delegate")
            {
                StartLogging(token);
                ReadDelegate();
                StopLogging();
            }
            else if (token.AsLowercase == "hll")
            {
                ReadHll();
            }
            else if (token.AsLowercase == "#include")
            {
                Token fileName = tr.ReadToken();
                IncludeFile(fileName.Value);
            }
            else if (token.AsLowercase == "functions")
            {
                StartLogging(token);
                ReadFunctions();
                StopLogging();
            }
            else if (token.AsLowercase == "strings")
            {
                //StartLogging(token);
                ReadStrings();
                //StopLogging();
            }
            else if (token.AsLowercase == "messages")
            {
                //StartLogging(token);
                ReadMessages();
                //StopLogging();
            }

            //tw.WriteLine("version = " + ainFile.Version);
            //tw.WriteLine("keycode = 0x" + ainFile.KeyCode.ToString("X8"));
            //tw.WriteLine("gameversion = " + ainFile.GameVersion);
            //tw.WriteLine("main = " + ainFile.MAIN);
            //tw.WriteLine("msgf = " + ainFile.MSGF);
            //tw.WriteLine("ojmp = " + ainFile.OJMP);
            //tw.WriteLine("dummy = " + ainFile.Dummy);
            else if (token.AsLowercase == "version")
            {
                version = GetNumberValue(token);
            }
            else if (token.AsLowercase == "isai2file")
            {
                IsAi2File = GetBoolValue(token);
            }
            else if (token.AsLowercase == "keycode")
            {
                keyCode = GetNumberValue(token);
            }
            else if (token.AsLowercase == "gameversion")
            {
                gameVersion = GetNumberValue(token);
            }
            else if (token.AsLowercase == "main")
            {
                main = GetFunctionNameValue(token);
            }
            else if (token.AsLowercase == "msgf")
            {
                msgf = GetFunctionNameValue(token);
            }
            else if (token.AsLowercase == "ojmp")
            {
                ojmp = GetFunctionNameValue(token);
            }
            else if (token.AsLowercase == "dummy")
            {
                //this isn't used anymore.
                fnctLength = GetNumberValue(token);
            }
            else
            {
                ThrowFormatException("unknown token " + token.Value);
                return false;
            }
            return true;
        }

        private void ReadMessages()
        {
            //only used when "preserve message order" is enabled
            this.messages.Clear();
            this.MessageToIndex.Clear();
            this.DuplicateMessageToIndex.Clear();

            Token separator = tr.ReadToken();
            if (separator != '{')
            {
                ThrowFormatException("Expected { after messages");
            }

            while (true)
            {
                separator = tr.PeekToken();
                if (separator == '}')
                {
                    tr.ReadToken();
                    break;
                }
                Token messageToken = tr.ReadToken();
                if (messageToken.QuoteCharacter != '\'')
                {
                    ThrowFormatException("Expected '' around message");
                }
                string str = messageToken.Value;
                if (this.MessageToIndex.ContainsKey(str))
                {
                    if (this.DuplicateMessageToIndex.ContainsKey(str))
                    {
                        var dic = DuplicateMessageToIndex[str];
                        int newDupeId = dic.Keys.Max() + 1;
                        AddMessage(str, newDupeId);
                    }
                    else
                    {
                        AddMessage(str, 1);
                    }
                }
                else
                {
                    AddMessage(str, 0);
                }
            }
        }

        private void ReadStrings()
        {
            //this is only used when "preserve string order" is enabled
            this.strings.Clear();
            this.StringToIndex.Clear();
            this.DuplicateStringToIndex.Clear();

            Token separator = tr.ReadToken();
            if (separator != '{')
            {
                ThrowFormatException("Expected { after strings");
            }

            while (true)
            {
                separator = tr.PeekToken();
                if (separator == '}')
                {
                    tr.ReadToken();
                    break;
                }
                Token stringToken = tr.ReadToken();
                if (stringToken.QuoteCharacter != '"')
                {
                    ThrowFormatException("Expected \"\" around string");
                }
                string str = stringToken.Value;
                if (this.StringToIndex.ContainsKey(str))
                {
                    if (this.DuplicateStringToIndex.ContainsKey(str))
                    {
                        var dic = DuplicateStringToIndex[str];
                        int newDupeId = dic.Keys.Max() + 1;
                        AddString(str, newDupeId);
                    }
                    else
                    {
                        AddString(str, 1);
                    }
                }
                else
                {
                    AddString(str, 0);
                }
            }
        }

        private void ReadFunctions()
        {
            Token separator = tr.ReadToken();
            if (separator != '{')
            {
                ThrowFormatException("expected { after functions");
            }

            while (true)
            {
                separator = tr.PeekToken();
                if (separator == '}')
                {
                    break;
                }

                int isLabel = 0;
                var dataType = ReadDataType();
                var functionName = tr.ReadToken().Value;
                if (functionName.StartsWith("#"))
                {
                    functionName = functionName.Substring(1);
                    isLabel = 1;
                }

                int functionIndex;
                if (FunctionNameToIndex.ContainsKey(functionName))
                {
                    functionIndex = FunctionNameToIndex[functionName];
                }
                else
                {
                    functionIndex = nextFunction;
                    nextFunction++;
                }

                //datatype [#]functionname([datatype argname][,datatype argname...])
                //[localvars (datatype argname[,datatype argname...])

                var function = functions.GetOrNew(functionIndex);
                function.Address = currentAddress + 6;
                List<Variable> variables = new List<Variable>();

                function.SetVariableType(dataType);
                function.Name = functionName;
                function.IsLabel = isLabel;

                var openParen = tr.ReadToken();
                if (openParen != '(')
                {
                    ThrowFormatException("expected ( after function name");
                }

                while (true)
                {
                    Token token = tr.PeekToken();
                    if (token == ')')
                    {
                        tr.ReadToken();
                        break;
                    }
                    dataType = ReadDataType();

                    if (dataType.DataType == DataType.Void)
                    {
                        //for some reason
                        dataType.StructType = 0;
                    }

                    var newVariable = new Variable();
                    newVariable.SetVariableType(dataType);

                    token = tr.ReadToken();
                    newVariable.Name = token.Value;

                    token = tr.PeekToken();
                    if (token == ',')
                    {
                        tr.ReadToken();
                    }
                    variables.Add(newVariable);
                }

                function.ParameterCount = variables.Count;

                Token localVars = tr.PeekToken();
                if (localVars.AsLowercase == "localvars")
                {
                    tr.ReadToken();
                    openParen = tr.ReadToken();
                    if (openParen != '(')
                    {
                        ThrowFormatException("expected ( after localvars");
                    }

                    while (true)
                    {
                        Token token = tr.PeekToken();
                        if (token == ')')
                        {
                            tr.ReadToken();
                            break;
                        }
                        dataType = ReadDataType();

                        if (dataType.DataType == DataType.Void)
                        {
                            //for some reason
                            dataType.StructType = 0;
                        }

                        var newVariable = new Variable();
                        newVariable.SetVariableType(dataType);
                        token = tr.ReadToken();
                        newVariable.Name = token.Value;

                        variables.Add(newVariable);

                        token = tr.PeekToken();
                        if (token == ',')
                        {
                            tr.ReadToken();
                        }
                    }
                }

                var label = tr.ReadToken();
                string labelName = label.Value;
                var address = Labels.GetOrNew(labelName);
                if (!firstPass && address == 0)
                {
                    ThrowFormatException("Can't find label " + labelName);
                }
                function.Address = address;

                var comma = tr.ReadToken();
                if (comma != ',')
                {
                    ThrowFormatException("expected comma after label name");
                }

                var crc = tr.ReadToken().ToInt();
                function.Crc = crc;

                var semicolon = tr.ReadToken();
                if (semicolon != ';')
                {
                    ThrowFormatException("expected semicolon after function declaration");
                }

                function.Parameters.Clear();
                function.Parameters.AddRange(variables);
                //function.totalVariables = variables.Count;
                functions.SetOrAdd(functionIndex, function);
                if (firstPass)
                {
                    FunctionNameToIndex.Set(function.Name, functionIndex);
                }

                var dic = FunctionVariablesInverse.GetOrNew(functionIndex);
                for (int i = 0; i < variables.Count; i++)
                {
                    dic.Set(variables[i].Name, i);
                }
                FunctionVariablesInverse.SetOrAdd(functionIndex, dic);
            }
            separator = tr.ReadToken();
            if (separator != '}')
            {
                ThrowFormatException("expected } after functions block");
            }
        }

        private void ReadHll()
        {
            //hll
            //{
            //  datatype libname.functionname([datatype arg1,...]);
            //}

            //Dictionary<string, int> libNameToIndex = this.LibraryNameToIndex;
            Dictionary<string, List<HllFunction>> libs = new Dictionary<string, List<HllFunction>>();
            int libIndex = this.LibraryNameToIndex.Count;

            Token separator = tr.ReadToken();
            if (separator != '{')
            {
                ThrowFormatException("expected { after 'hll'");
            }

            while (true)
            {
                separator = tr.PeekToken();
                if (separator == '}')
                {
                    break;
                }

                var dataType = ReadDataType();
                var libName = tr.ReadToken().Value;
                var dot = tr.ReadToken();
                if (dot != '.')
                {
                    ThrowFormatException("expected dot between library name and function name");
                }
                var functionName = tr.ReadToken().Value;
                var openParen = tr.ReadToken();

                var lib = libs.GetOrNew(libName);
                if (!this.LibraryNameToIndex.ContainsKey(libName))
                {
                    this.LibraryNameToIndex.Add(libName, libIndex);
                    libIndex++;
                }

                var function = new HllFunction();
                function.Name = functionName;
                function.SetVariableType(dataType);

                List<HllFunctionParameter> args = new List<HllFunctionParameter>();

                while (true)
                {
                    separator = tr.PeekToken();
                    if (separator == ')')
                    {
                        tr.ReadToken();
                        break;
                    }
                    if (separator == ',')
                    {
                        tr.ReadToken();
                    }

                    dataType = ReadDataType();
                    string argName = tr.ReadToken().Value;
                    var newArg = new HllFunctionParameter();
                    newArg.SetVariableType(dataType);
                    newArg.Name = argName;
                    args.Add(newArg);
                }

                var semicolon = tr.ReadToken();
                if (semicolon != ';')
                {
                    ThrowFormatException("expected semicolon after library declaration");
                }
                function.Parameters.Clear();
                function.Parameters.AddRange(args.ToArray());
                function.ParameterCount = args.Count;
                lib.Add(function);
                libs.Set(libName, lib);
            }

            foreach (var pair in this.LibraryNameToIndex)
            {
                int libIndex2 = pair.Value;
                string libName = pair.Key;

                if (libs.ContainsKey(libName))
                {
                    var functions = libs[libName];
                    var lib = libraries.GetOrNew(libIndex2);
                    lib.LibraryName = libName;
                    //lib.children = functions.Count;

                    //warning: overrides when defined multiple times
                    lib.Functions.Clear();
                    lib.Functions.AddRange(functions);
                    libraries.SetOrAdd(libIndex2, lib);

                    for (int functionIndex = 0; functionIndex < functions.Count; functionIndex++)
                    {
                        string fullName = lib.LibraryName + "." + functions[functionIndex].Name;
                        LibraryFunctionNameToIndex.Set(fullName, new KeyValuePair<int, int>(libIndex2, functionIndex));
                    }
                }
            }

            separator = tr.ReadToken();
            if (separator != '}')
            {
                ThrowFormatException("expected } after hll block");
            }
        }

        private void StopLogging()
        {

            if (firstPass)
            {
                tr.Logging = false;
            }

        }

        private void StartLogging(Token token)
        {
            if (firstPass)
            {
                tr.Logging = true;
                tr.PutbackToken(token);
                tr.ReadToken();
            }

        }

        void ProcessGenericInstruction(InstructionInfo instructionInfo)
        {
            if (ArgumentKinds.InstructionArgumentKinds.ContainsKey((int)instructionInfo.instruction))
            {
                var argumentKinds = ArgumentKinds.InstructionArgumentKinds[(int)instructionInfo.instruction];
                ProcessInstructionWithArgumentKinds(instructionInfo, argumentKinds);
            }
            else
            {
                var argumentKinds = new ArgumentKind[instructionInfo.totalArguments];
                for (int i = 0; i < argumentKinds.Length; i++)
                {
                    argumentKinds[i] = ArgumentKind.Int;
                }
                ProcessInstructionWithArgumentKinds(instructionInfo, argumentKinds);
            }
        }

        void ProcessGenericInstructionOverride(InstructionInfo instructionInfo, ArgumentKind argumentKind)
        {
            var argumentKinds = new ArgumentKind[] { argumentKind };
            ProcessInstructionWithArgumentKinds(instructionInfo, argumentKinds);
        }

        void ProcessInstructionWithArgumentKinds(InstructionInfo instructionInfo, ArgumentKind[] argumentKinds)
        {
            for (int i = 0; i < instructionInfo.totalArguments; i++)
            {
                int word;
                var argumentKind = argumentKinds[i];
                switch (argumentKind)
                {
                    case ArgumentKind.CodeAddress:
                        {
                            string labelName = tr.ReadToken().Value;
                            var labelAddress = Labels.GetOrNew(labelName);
                            word = labelAddress;
                            ValidateLabel(labelName, labelAddress);
                        }
                        break;
                    case ArgumentKind.Filename:
                        {
                            string fileName = tr.ReadToken().Value;
                            if (FilenameToIndex.ContainsKey(fileName))
                            {
                                word = FilenameToIndex[fileName];
                            }
                            else
                            {
                                word = currentFilename;
                                filenames.SetOrAdd(currentFilename, fileName);
                                FilenameToIndex.Set(fileName, word);
                                currentFilename++;
                            }
                        }
                        break;
                    case ArgumentKind.Float:
                        {
                            var floatNumber = tr.ReadToken();
                            word = BitConverter.ToInt32(BitConverter.GetBytes(floatNumber.ToFloat()), 0);
                        }
                        break;
                    case ArgumentKind.Function:
                        {
                            var functionName = tr.ReadToken();
                            if (functionName.IsNumber() && functionName.QuoteCharacter == 0)
                            {
                                word = functionName.ToInt();
                            }
                            else
                            {
                                if (FunctionNameToIndex.ContainsKey(functionName.Value))
                                {
                                    word = FunctionNameToIndex[functionName.Value];
                                    var function = functions[word];
                                    this.classNumber = function.StructType;
                                }
                                else
                                {
                                    word = -1;
                                    if (!firstPass)
                                    {
                                        ThrowFormatException("Function name not found: " + functionName.Value);
                                    }
                                }
                            }
                        }
                        break;
                    case ArgumentKind.Global:
                        {
                            word = ReadGlobalVariable();
                        }
                        break;
                    case ArgumentKind.Int:
                        {
                            var token = tr.ReadToken();
                            if (token.IsNumber() && token.QuoteCharacter == 0)
                            {
                                word = token.ToInt();
                            }
                            else
                            {
                                ThrowFormatException("Value is not an integer: " + token.Value);
                                word = -1;
                            }
                        }
                        break;
                    case ArgumentKind.Library:
                        {
                            if (i + 1 < argumentKinds.Length && argumentKinds[i + 1] == ArgumentKind.LibraryFunction)
                            {
                                var token = tr.ReadToken();
                                if (token.IsNumber() && token.QuoteCharacter == 0)
                                {
                                    word = token.ToInt();
                                    instructionInfo.words[i] = word;
                                    i++;
                                    token = tr.ReadToken();
                                    word = token.ToInt();
                                }
                                else
                                {
                                    var dot = tr.ReadToken();
                                    if (dot != '.')
                                    {
                                        ThrowFormatException("no dot after library name");
                                    }
                                    var token2 = tr.ReadToken();
                                    string libraryName = token.Value;
                                    string functionName = token2.Value;
                                    string fullName = libraryName + "." + functionName;
                                    if (LibraryFunctionNameToIndex.ContainsKey(fullName))
                                    {
                                        var pair = LibraryFunctionNameToIndex[fullName];
                                        word = pair.Key;
                                        instructionInfo.words[i] = word;
                                        i++;
                                        word = pair.Value;
                                    }
                                    else
                                    {
                                        word = -1;
                                        instructionInfo.words[i] = word;
                                        i++;
                                        word = -1;
                                        if (!firstPass)
                                        {
                                            ThrowFormatException("Library call not found: " + fullName);
                                        }
                                    }
                                }
                            }
                            else
                            {
                                throw new ArgumentException("Instruction must have LibraryFunction after Library");
                            }
                        }
                        break;
                    case ArgumentKind.LibraryFunction:
                        {
                            throw new ArgumentException("Instruction must have LibraryFunction after Library");
                        }
                        break;
                    case ArgumentKind.Local:
                        {
                            word = ReadLocalVariable();
                        }
                        break;
                    case ArgumentKind.LocalMember:
                    case ArgumentKind.Member:
                        {
                            if (argumentKind == ArgumentKind.LocalMember)
                            {
                                classNumber = this.currentClassNumber;
                            }
                            else
                            {

                            }
                            word = ReadMember();
                        }
                        break;
                    case ArgumentKind.Message:
                        {
                            var token = tr.ReadToken();
                            int dupeNumber = -1;
                            string str = "";
                            if (token.QuoteCharacter == 0 && token.IsNumber())
                            {
                                var token2 = tr.PeekToken();
                                if (token2.QuoteCharacter == '\'')
                                {
                                    word = -1;
                                    tr.ReadToken();
                                    dupeNumber = token.ToInt();
                                    str = token2.Value;
                                }
                                else
                                {
                                    //id number
                                    word = token.ToInt();
                                    goto DontAssignWord;
                                }
                            }
                            else
                            {
                                str = token.Value;
                            }

                            word = AddMessage(str, dupeNumber);
                        DontAssignWord:
                            ;
                        }
                        break;
                    case ArgumentKind.String:
                        {
                            word = ReadString();
                        }
                        break;
                    case ArgumentKind.StructType:
                        {
                            var token = tr.ReadToken();
                            if (!(token.IsNumber() && token.QuoteCharacter == 0))
                            {
                                if (StructNameToIndex.ContainsKey(token.Value))
                                {
                                    word = StructNameToIndex[token.Value];
                                }
                                else
                                {
                                    word = -1;
                                    if (!firstPass)
                                    {
                                        ThrowFormatException("Struct type name not found: " + token.Value);
                                    }
                                }
                            }
                            else
                            {
                                word = token.ToInt();
                            }
                            this.lastSeenStructType = word;
                            this.classNumber = word;
                        }
                        break;
                    case ArgumentKind.Switch:
                        {
                            throw new ArgumentException("Switch must be handled separately");
                        }
                        break;
                    case ArgumentKind.SystemCall:
                        {
                            var token = tr.ReadToken();
                            if (token.IsNumber() && token.QuoteCharacter == 0)
                            {
                                word = token.ToInt();
                            }
                            else
                            {
                                var dot = tr.ReadToken();
                                if (dot != '.')
                                {
                                    ThrowFormatException("no dot in system function call");
                                }

                                var token2 = tr.ReadToken();

                                string functionName = token.Value + dot.Value + token2.Value;
                                if (AinFile.SystemCallNameToIndex.ContainsKey(functionName))
                                {
                                    word = AinFile.SystemCallNameToIndex[functionName];
                                }
                                else
                                {
                                    ThrowFormatException("Unknown system call: " + functionName);
                                    word = -1;
                                }
                            }
                        }
                        break;
                    case ArgumentKind.AssignInt:
                        {
                            var equalsToken = tr.ReadToken();
                            if (equalsToken.Value != "=")
                            {
                                ThrowFormatException("No equals sign in assignment");
                            }
                            var number = tr.ReadToken();
                            if (number.IsNumber() && number.QuoteCharacter == 0)
                            {
                                word = number.ToInt();
                            }
                            else
                            {
                                ThrowFormatException("Number is not an integer: " + number.Value);
                                word = -1;
                            }
                        }
                        break;
                    case ArgumentKind.DelegateType:
                        {
                            var delegateNameToken = tr.ReadToken();
                            if (!delegateNameToken.IsNumber())
                            {
                                if (DelegateNameToIndex.ContainsKey(delegateNameToken.Value))
                                {
                                    word = DelegateNameToIndex[delegateNameToken.Value];
                                }
                                else
                                {
                                    if (!firstPass)
                                    {
                                        ThrowFormatException("Unknown delegate type " + delegateNameToken.Value);
                                    }
                                    word = -1;
                                }
                            }
                            else
                            {
                                word = delegateNameToken.ToInt();
                            }
                        }
                        break;
                    default:
                        {
                            throw new ArgumentException("Unknown argument type");
                        }
                }
                instructionInfo.words[i] = word;
            }
        }

        private int ReadString()
        {
            int word;
            var token = tr.ReadToken();
            int dupeNumber = -1;
            string str = "";
            if (token.QuoteCharacter == 0 && token.IsNumber())
            {
                var token2 = tr.PeekToken();
                if (token2.QuoteCharacter == '"')
                {
                    tr.ReadToken();
                    dupeNumber = token.ToInt();
                    str = token2.Value;
                }
                else
                {
                    //id number
                    word = token.ToInt();
                    goto DontAssignWord;
                }
            }
            else
            {
                str = token.Value;
            }

            word = AddString(str, dupeNumber);
        DontAssignWord:
            return word;
        }




        /// <summary>
        /// The last instruction seen before the current instruction.  Used when turning a PUSH instruction into a variable name.
        /// </summary>
        InstructionInfo lastInstructionInfo;
        //Instruction lastInstruction;

        private void ReadInstruction(Instruction instruction)
        {
            InstructionInfo instructionInfo = new InstructionInfo();
            instructionInfo.instruction = instruction;
            instructionInfo.totalArguments = instruction.GetNumberOfWords();

            switch (instruction)
            {
                default:
                    ProcessGenericInstruction(instructionInfo);
                    //if (instructionArguments >= 1)
                    //{
                    //    word1 = tr.ReadToken().ToInt();
                    //}
                    //if (instructionArguments >= 2)
                    //{
                    //    var separator = tr.PeekToken();
                    //    if (separator == ',')
                    //    {
                    //        tr.ReadToken();
                    //    }
                    //    word2 = tr.ReadToken().ToInt();
                    //}
                    //if (instructionArguments >= 3)
                    //{
                    //    var separator = tr.PeekToken();
                    //    if (separator == ',')
                    //    {
                    //        tr.ReadToken();
                    //        word3 = tr.ReadToken().ToInt();
                    //    }
                    //}
                    break;
                //case Instruction.EOF:
                //    {
                //        string fileName = tr.ReadToken().Value;
                //        word1 = currentFilename;
                //        filenames.SetOrAdd(currentFilename, fileName);
                //        currentFilename++;
                //    }
                //    break;
                //case Instruction.S_PUSH:
                //    {
                //        var token = tr.ReadToken();
                //        int dupeNumber = -1;
                //        string str = "";
                //        bool useIdNumber = false;
                //        if (token.QuoteCharacter == 0 && token.IsNumber())
                //        {
                //            var token2 = tr.PeekToken();
                //            if (token2.QuoteCharacter == '"')
                //            {
                //                tr.ReadToken();
                //                dupeNumber = token.ToInt();
                //                str = token2.Value;
                //            }
                //            else
                //            {
                //                //id number
                //                word1 = token.ToInt();
                //                useIdNumber = true;
                //            }
                //        }
                //        else
                //        {
                //            str = token.Value;
                //        }

                //        if (!useIdNumber)
                //        {
                //            word1 = AddString(str, dupeNumber);
                //        }

                //        //string stringValue = tr.ReadToken().Value;
                //        //word1 = currentString;
                //        //strings.SetOrAdd(currentString, stringValue);
                //        //currentString++;
                //    }
                //    break;
                //case Instruction.IFNZ:
                //case Instruction.IFZ:
                //case Instruction.JUMP:
                //    {
                //        string labelName = tr.ReadToken().Value;
                //        var labelAddress = Labels.GetOrNew(labelName);
                //        word1 = labelAddress;
                //        ValidateLabel(labelName, labelAddress);
                //    }
                //    break;
                case Instruction.SWITCH:
                case Instruction.STRSWITCH:
                    {
                        var switchBlock = switches.GetOrNew(currentSwitch);
                        List<SwitchCase> cases = new List<SwitchCase>();

                        while (true)
                        {
                            Token token = tr.PeekToken();
                            if (token.AsLowercase == "default")
                            {
                                tr.ReadToken();
                                var colon = tr.ReadToken();
                                if (colon != ':')
                                {
                                    ThrowFormatException("expected colon after default case");
                                }
                                string labelName = tr.ReadToken().Value;
                                var labelAddress = Labels.GetOrNew(labelName);
                                ValidateLabel(labelName, labelAddress);
                                switchBlock.DefaultCaseAddress = labelAddress;
                            }
                            else if (token.AsLowercase == "case")
                            {
                                tr.ReadToken();
                                int caseNumber;
                                if (instruction == Instruction.STRSWITCH)
                                {
                                    caseNumber = ReadString();
                                }
                                else
                                {
                                    var caseValueToken = tr.ReadToken();
                                    if (!caseValueToken.IsNumber())
                                    {
                                        ThrowFormatException("expected case number after case");
                                    }
                                    caseNumber = caseValueToken.ToInt();
                                }
                                var colon = tr.ReadToken();
                                if (colon != ':')
                                {
                                    ThrowFormatException("expected colon after case number");
                                }
                                string labelName = tr.ReadToken().Value;
                                var labelAddress = Labels.GetOrNew(labelName);
                                ValidateLabel(labelName, labelAddress);
                                var newCase = new SwitchCase();
                                newCase.TargetAddress = labelAddress;
                                newCase.Value = caseNumber;
                                cases.Add(newCase);
                            }
                            else
                            {
                                break;
                            }
                        }
                        switchBlock.SwitchCases.Clear();
                        switchBlock.SwitchCases.AddRange(cases);
                        if (instruction == Instruction.STRSWITCH)
                        {
                            switchBlock.CaseType2or4 = 4;
                        }

                        //switchBlock.CaseCount = cases.Count;
                        switches.SetOrAdd(currentSwitch, switchBlock);
                        instructionInfo.word1 = currentSwitch;
                        currentSwitch++;
                    }
                    break;
                case Instruction.FUNC:
                    {
                        ProcessGenericInstruction(instructionInfo);
                        activeFunction = instructionInfo.word1;
                        funcInstructionsSeen++;

                        if (activeFunction >= 0 && activeFunction < this.functions.Count)
                        {
                            var function = this.functions[activeFunction];
                            string className = function.GetClassName();
                            if (className != "" && StructNameToIndex.ContainsKey(className))
                            {
                                currentClassNumber = StructNameToIndex[className];
                            }
                            else
                            {
                                currentClassNumber = -1;
                            }
                        }

                        //if (false)
                        //{
                        //    ThrowFormatException("should not be here");

                        //    int isLabel = 0;
                        //    var dataType = ReadDataType();
                        //    var functionName = tr.ReadToken().Value;
                        //    if (functionName.StartsWith("#"))
                        //    {
                        //        functionName = functionName.Substring(1);
                        //        isLabel = 1;
                        //    }

                        //    //if (functionName == "push_back_int")
                        //    //{

                        //    //}

                        //    //datatype [#]functionname([datatype argname][,datatype argname...])
                        //    //[localvars (datatype argname[,datatype argname...])
                        //    int functionIndex = nextFunction;

                        //    //if (ReservedFunctions.ContainsKey(functionName))
                        //    //{
                        //    //    functionIndex = ReservedFunctions[functionName];
                        //    //}
                        //    //else
                        //    //{
                        //    //    if (ReservedFunctionIndexes.ContainsKey(functionIndex))
                        //    //    {
                        //    nextFunction++;
                        //    functionIndex = nextFunction;
                        //    //    }
                        //    //    nextFunction++;
                        //    //}

                        //    //if (nextFunction == 0x197B)
                        //    //{

                        //    //}

                        //    activeFunction = functionIndex;
                        //    word1 = functionIndex;

                        //    var function = functions.GetOrNew(functionIndex);
                        //    function.Address = currentAddress + 6;
                        //    List<Variable> variables = new List<Variable>();

                        //    function.SetVariableType(dataType);
                        //    function.Name = functionName;
                        //    function.IsLabel = isLabel;

                        //    var openParen = tr.ReadToken();
                        //    if (openParen != '(')
                        //    {
                        //        ThrowFormatException("expected ( after function name");
                        //    }

                        //    while (true)
                        //    {
                        //        Token token = tr.PeekToken();
                        //        if (token == ')')
                        //        {
                        //            tr.ReadToken();
                        //            break;
                        //        }
                        //        dataType = ReadDataType();

                        //        if (dataType.DataType == DataType.Void)
                        //        {
                        //            //for some reason
                        //            dataType.StructType = 0;
                        //        }

                        //        var newVariable = new Variable();
                        //        newVariable.SetVariableType(dataType);

                        //        token = tr.ReadToken();
                        //        newVariable.Name = token.Value;

                        //        token = tr.PeekToken();
                        //        if (token == ',')
                        //        {
                        //            tr.ReadToken();
                        //        }
                        //        variables.Add(newVariable);
                        //    }

                        //    function.ArgumentCount = variables.Count;

                        //    Token localVars = tr.PeekToken();
                        //    if (localVars.AsLowercase == "localvars")
                        //    {
                        //        tr.ReadToken();
                        //        openParen = tr.ReadToken();
                        //        if (openParen != '(')
                        //        {
                        //            ThrowFormatException("expected ( after localvars");
                        //        }

                        //        while (true)
                        //        {
                        //            Token token = tr.PeekToken();
                        //            if (token == ')')
                        //            {
                        //                tr.ReadToken();
                        //                break;
                        //            }
                        //            dataType = ReadDataType();

                        //            if (dataType.DataType == DataType.Void)
                        //            {
                        //                //for some reason
                        //                dataType.StructType = 0;
                        //            }

                        //            var newVariable = new Variable();
                        //            newVariable.SetVariableType(dataType);
                        //            token = tr.ReadToken();
                        //            newVariable.Name = token.Value;

                        //            variables.Add(newVariable);

                        //            token = tr.PeekToken();
                        //            if (token == ',')
                        //            {
                        //                tr.ReadToken();
                        //            }
                        //        }
                        //    }
                        //    else
                        //    {

                        //    }

                        //    function.Arguments = variables.ToArray();
                        //    //function.totalVariables = variables.Count;
                        //    functions.SetOrAdd(functionIndex, function);
                        //    if (firstPass)
                        //    {
                        //        FunctionNameToIndex.Set(function.Name, functionIndex);
                        //    }

                        //    var dic = FunctionVariablesInverse.GetOrNew(functionIndex);
                        //    for (int i = 0; i < variables.Count; i++)
                        //    {
                        //        dic.Set(variables[i].Name, i);
                        //    }
                        //    FunctionVariablesInverse.SetOrAdd(functionIndex, dic);
                        //}
                    }
                    break;
                //case Instruction.CALLFUNC:
                //case Instruction.CALLMETHOD:
                //    {
                //        var functionName = tr.ReadToken();
                //        if (functionName.IsNumber() && functionName.QuoteCharacter == 0)
                //        {
                //            word1 = functionName.ToInt();
                //        }
                //        else
                //        {
                //            if (FunctionNameToIndex.ContainsKey(functionName.Value))
                //            {
                //                word1 = FunctionNameToIndex[functionName.Value];
                //            }
                //            else
                //            {
                //                if (!firstPass)
                //                {
                //                    ThrowFormatException("Function name not found: " + functionName.Value);
                //                }
                //            }
                //        }
                //    }
                //    break;
                //case Instruction.CALLSYS:
                //    {
                //        var token = tr.ReadToken();
                //        if (token.IsNumber() && token.QuoteCharacter == 0)
                //        {
                //            word1 = token.ToInt();
                //        }
                //        else
                //        {
                //            var dot = tr.ReadToken();
                //            if (dot != '.')
                //            {
                //                ThrowFormatException("no dot in system function call");
                //            }

                //            var token2 = tr.ReadToken();

                //            string functionName = token.Value + dot.Value + token2.Value;
                //            if (AinFile.SystemCallNameToIndex.ContainsKey(functionName))
                //            {
                //                word1 = AinFile.SystemCallNameToIndex[functionName];
                //            }
                //            else
                //            {
                //                ThrowFormatException("Unknown system call: " + functionName);
                //            }
                //        }
                //    }
                //    break;
                //case Instruction.CALLHLL:
                //    {
                //        var token = tr.ReadToken();
                //        if (token.IsNumber() && token.QuoteCharacter == 0)
                //        {
                //            word1 = token.ToInt();
                //            //token = tr.PeekToken();
                //            //if (token == ',')
                //            //{
                //            //    tr.ReadToken();
                //            //}
                //            token = tr.ReadToken();
                //            word2 = token.ToInt();
                //        }
                //        else
                //        {
                //            var dot = tr.ReadToken();
                //            if (dot != '.')
                //            {
                //                ThrowFormatException("no dot after library name");
                //            }
                //            var token2 = tr.ReadToken();
                //            string libraryName = token.Value;
                //            string functionName = token2.Value;
                //            string fullName = libraryName + "." + functionName;
                //            if (LibraryFunctionNameToIndex.ContainsKey(fullName))
                //            {
                //                var pair = LibraryFunctionNameToIndex[fullName];
                //                word1 = pair.Key;
                //                word2 = pair.Value;
                //            }
                //            else
                //            {
                //                if (!firstPass)
                //                {
                //                    ThrowFormatException("Library call not found: " + fullName);
                //                }
                //            }
                //        }
                //    }
                //    break;
                //case Instruction.F_PUSH:
                //    {
                //        var floatNumber = tr.ReadToken();
                //        word1 = BitConverter.ToInt32(BitConverter.GetBytes(floatNumber.ToFloat()), 0);
                //    }
                //    break;
                case Instruction.PUSH:
                    {
                        switch (lastInstructionInfo.instruction)
                        {
                            case Instruction.PUSHLOCALPAGE:
                                ProcessGenericInstructionOverride(instructionInfo, ArgumentKind.Local);
                                break;
                            case Instruction.PUSHGLOBALPAGE:
                                ProcessGenericInstructionOverride(instructionInfo, ArgumentKind.Global);
                                break;
                            case Instruction.PUSHSTRUCTPAGE:
                                ProcessGenericInstructionOverride(instructionInfo, ArgumentKind.LocalMember);
                                break;
                            case Instruction.SR_REF:
                            case Instruction.SR_REF2:
                            case Instruction.SH_STRUCT_SR_REF:
                            case Instruction.SR_REFREF:
                                {
                                    //add any other instructions that include a struct number here
                                    int currentClass = this.currentClassNumber;
                                    this.currentClassNumber = this.lastSeenStructType;
                                    ProcessGenericInstructionOverride(instructionInfo, ArgumentKind.Member);
                                    this.currentClassNumber = currentClass;
                                }
                                break;
                            default:
                                ProcessGenericInstruction(instructionInfo);
                                break;
                        }
                    }
                    break;
                //case Instruction.ENDFUNC:
                //    {
                //        var number = tr.PeekToken();
                //        if (number.IsNumber() && number.QuoteCharacter == 0)
                //        {
                //            tr.ReadToken();
                //            word1 = number.ToInt();
                //        }
                //        else
                //        {
                //            word1 = activeFunction;
                //        }
                //    }
                //    break;
                //case Instruction.SH_GLOBALREF:
                //    {
                //        word1 = ReadGlobalVariable();
                //    }
                //    break;
                //case Instruction.SH_LOCALCREATE:
                //case Instruction.SH_LOCALASSIGN:
                //    {
                //        word1 = ReadLocalVariable();
                //        var equals = tr.ReadToken();
                //        if (equals != '=')
                //        {
                //            ThrowFormatException("expected = after localassign variable name");
                //        }
                //        word2 = tr.ReadToken().ToInt();
                //    }
                //    break;
                //case Instruction.SH_LOCALDEC:
                //case Instruction.SH_LOCALDELETE:
                //case Instruction.SH_LOCALINC:
                //case Instruction.SH_LOCALREF:
                //    {
                //        word1 = ReadLocalVariable();
                //    }
                //    break;
                //case Instruction.SH_STRUCTREF:
                //    {
                //        word1 = ReadMember();
                //    }
                //    break;
                //case Instruction.MSG:
                //    {
                //        var token = tr.ReadToken();
                //        int dupeNumber = -1;
                //        string str = "";
                //        bool useIdNumber = false;
                //        if (token.QuoteCharacter == 0 && token.IsNumber())
                //        {
                //            var token2 = tr.PeekToken();
                //            if (token2.QuoteCharacter == '\'')
                //            {
                //                tr.ReadToken();
                //                dupeNumber = token.ToInt();
                //                str = token2.Value;
                //            }
                //            else
                //            {
                //                //id number
                //                word1 = token.ToInt();
                //                useIdNumber = true;
                //            }
                //        }
                //        else
                //        {
                //            str = token.Value;
                //        }

                //        if (!useIdNumber)
                //        {
                //            word1 = AddMessage(str, dupeNumber);
                //        }

                //        //word1 = currentMessage;
                //        //var message = tr.ReadToken();
                //        //if (message.QuoteCharacter != '\'')
                //        //{
                //        //    ThrowFormatException("message should be in single quotes");
                //        //}
                //        //messages.SetOrAdd(currentMessage, message.Value);
                //        //currentMessage++;
                //    }
                //    break;
            }




            if (firstPass == false)
            {
                bw.Write((Int16)instruction);
                for (int i = 0; i < instructionInfo.totalArguments; i++)
                {
                    bw.Write(instructionInfo.words[i]);
                }

                //if (instructionArguments >= 1)
                //{
                //    //unchecked
                //    //{
                //    //    if (word1 == (int)0xDEADBEEF)
                //    //    {

                //    //    }
                //    //}
                //    bw.Write(word1);
                //}
                //if (instructionArguments >= 2)
                //{
                //    bw.Write(word2);
                //}
            }
            currentAddress = currentAddress + 2 + 4 * instructionInfo.totalArguments;
            //lastInstruction = instruction;
            lastInstructionInfo = instructionInfo;
        }

        private int ReadMember()
        {
            int structIndex = this.classNumber;
            int word;

            Token token = tr.ReadToken();
            string memberName = token.Value;

            if (structIndex >= 0 && structIndex < structs.Count)
            {
                var dic = StructVariablesInverse[structIndex];

                if (dic.ContainsKey(memberName))
                {
                    word = dic[memberName];
                    var member = structs[structIndex].Members[word];
                    this.classNumber = member.StructType;
                    goto done;
                }
            }

            if (token.IsNumber() && token.QuoteCharacter == 0)
            {
                word = token.ToInt();
            }
            else
            {
                word = -1;
                if (!firstPass)
                {
                    ThrowFormatException("Struct member not found: " + memberName);
                }
            }
        done:
            return word;
        }

        private int ReadLocalVariable()
        {
            int word1 = 0;
            Token token = tr.ReadToken();
            string localName = token.Value;
            var dic = FunctionVariablesInverse[activeFunction];
            if (dic.ContainsKey(localName))
            {
                word1 = dic[localName];
                var variable = this.functions[this.activeFunction].Parameters[word1];
                this.classNumber = variable.StructType;
            }
            else
            {
                if (token.IsNumber() && token.QuoteCharacter == 0)
                {
                    word1 = token.ToInt();
                }
                else
                {
                    if (!firstPass)
                    {
                        ThrowFormatException("Local not found: " + localName);
                    }
                }
            }
            return word1;
        }

        private int ReadGlobalVariable()
        {
            int word1 = 0;
            Token token = tr.ReadToken();
            string globalName;
            globalName = token.Value;
            var dot = tr.PeekToken();
            if (dot == '.')
            {
                tr.ReadToken();
                globalName = globalName + "." + tr.ReadToken().Value;
            }
            if (GlobalNameToIndex.ContainsKey(globalName))
            {
                word1 = GlobalNameToIndex[globalName];
                var global = globals[word1];
                this.classNumber = global.StructType;
            }
            else
            {
                if (token.IsNumber() && token.QuoteCharacter == 0)
                {
                    word1 = token.ToInt();
                }
                else
                {
                    if (!firstPass)
                    {
                        ThrowFormatException("Global not found: " + globalName);
                    }
                }
            }
            return word1;
        }

        private void ValidateLabel(string labelName, int labelAddress)
        {
            if (!firstPass && labelAddress == 0)
            {
                ThrowFormatException("Label not found: " + labelName);
            }
        }

        private void ReadFunctionType()
        {
            //functype datatype name([datatype1][, datatype2...]);

            var functionType = functionTypes.GetOrNew(currentFunctionType);

            var dataType = ReadDataType();
            string name = tr.ReadToken().Value;
            functionType.SetVariableType(dataType);
            functionType.Name = name;
            int variableNumber = 0;
            List<Variable> variables = new List<Variable>();

            Token separator = tr.PeekToken();
            if (separator == '(')
            {
                tr.ReadToken();
                while (true)
                {
                    separator = tr.PeekToken();
                    if (separator == ')')
                    {
                        tr.ReadToken();
                        break;
                    }
                    dataType = ReadDataType();
                    var newVariable = new Variable();
                    newVariable.SetVariableType(dataType);
                    if (dataType.DataType == DataType.Void)
                    {
                        newVariable.Name = "<void>";
                        newVariable.StructType = 0;
                    }
                    else
                    {
                        newVariable.Name = variableNumber.ToString(CultureInfo.InvariantCulture);
                        variableNumber++;
                    }


                    variables.Add(newVariable);

                    separator = tr.PeekToken();
                    if (separator == ',')
                    {
                        tr.ReadToken();
                    }
                }
            }

            functionType.ParameterCount = variables.Count;

            separator = tr.PeekToken();
            if (separator == ';')
            {
                tr.ReadToken();
            }
            else
            {
                //more data types and commas
                while (true)
                {
                    dataType = ReadDataType();
                    var newVariable = new Variable();
                    newVariable.SetVariableType(dataType);
                    newVariable.Name = variableNumber.ToString();
                    variableNumber++;
                    variables.Add(newVariable);

                    separator = tr.PeekToken();
                    if (separator == ',')
                    {
                        tr.ReadToken();
                    }

                    if (separator == ';')
                    {
                        tr.ReadToken();
                        break;
                    }
                }
            }
            functionType.Parameters.Clear();
            functionType.Parameters.AddRange(variables);
            functionTypes.SetOrAdd(currentFunctionType, functionType);
            FunctionTypeNameToIndex.Set(name, currentFunctionType);
            currentFunctionType++;
        }

        private void ReadDelegate()
        {
            //delegate datatype name([datatype1][, datatype2...]);

            var delg = delegates.GetOrNew(currentDelegate);

            var dataType = ReadDataType();
            string name = tr.ReadToken().Value;
            delg.SetVariableType(dataType);
            delg.Name = name;
            int variableNumber = 0;
            List<Variable> variables = new List<Variable>();

            Token separator = tr.PeekToken();
            if (separator == '(')
            {
                tr.ReadToken();
                while (true)
                {
                    separator = tr.PeekToken();
                    if (separator == ')')
                    {
                        tr.ReadToken();
                        break;
                    }
                    dataType = ReadDataType();
                    var newVariable = new Variable();
                    newVariable.SetVariableType(dataType);
                    if (dataType.DataType == DataType.Void)
                    {
                        newVariable.Name = "<void>";
                        newVariable.StructType = 0;
                    }
                    else
                    {
                        newVariable.Name = variableNumber.ToString(CultureInfo.InvariantCulture);
                        variableNumber++;
                    }

                    variables.Add(newVariable);

                    separator = tr.PeekToken();
                    if (separator == ',')
                    {
                        tr.ReadToken();
                    }
                }
            }

            delg.ParameterCount = variables.Count;

            separator = tr.PeekToken();
            if (separator == ';')
            {
                tr.ReadToken();
            }
            else
            {
                //more data types and commas
                while (true)
                {
                    dataType = ReadDataType();
                    var newVariable = new Variable();
                    newVariable.SetVariableType(dataType);
                    newVariable.Name = variableNumber.ToString();
                    variableNumber++;
                    variables.Add(newVariable);

                    separator = tr.PeekToken();
                    if (separator == ',')
                    {
                        tr.ReadToken();
                    }

                    if (separator == ';')
                    {
                        tr.ReadToken();
                        break;
                    }
                }
            }
            delg.Parameters.Clear();
            delg.Parameters.AddRange(variables);
            delegates.SetOrAdd(currentDelegate, delg);
            DelegateNameToIndex.Set(name, currentDelegate);
            currentDelegate++;
        }

        private int GetNumberValue(Token variableNameToken)
        {
            StartLogging(variableNameToken);

            int number;
            Token token = tr.ReadToken();
            if (token == '=')
            {
                token = tr.ReadToken();
            }
            if (token.IsNumber() && token.QuoteCharacter == 0)
            {
                number = token.ToInt();
            }
            else
            {
                ThrowFormatException("Expected a number, instead got " + token.Value);
                number = -1;
            }
            StopLogging();

            return number;
        }

        private bool GetBoolValue(Token variableNameToken)
        {
            StartLogging(variableNameToken);

            bool boolValue;
            Token token = tr.ReadToken();
            if (token == '=')
            {
                token = tr.ReadToken();
            }
            if (token.IsNumber() && token.QuoteCharacter == 0)
            {
                boolValue = token.ToInt() != 0;
            }
            else if ((token.Value.ToLowerInvariant() == "true" || token.Value.ToLowerInvariant() == "false") && token.QuoteCharacter == 0)
            {
                boolValue = bool.Parse(token.Value.ToLowerInvariant());
            }
            else
            {
                ThrowFormatException("Expected a boolean, instead got " + token.Value);
                boolValue = false;
            }
            StopLogging();

            return boolValue;
        }

        private int GetFunctionNameValue(Token variableNameToken)
        {
            StartLogging(variableNameToken);

            int number = 0;
            Token token = tr.ReadToken();
            if (token == '=')
            {
                token = tr.ReadToken();
            }
            if (this.FunctionNameToIndex.ContainsKey(token.Value))
            {
                number = this.FunctionNameToIndex[token.Value];
            }
            else
            {
                if (token.IsInt())
                {
                    number = token.ToInt();
                }
                else
                {
                    if (!firstPass)
                    {
                        ThrowFormatException("Cannot find function " + token.Value);
                    }
                }
            }
            StopLogging();

            return number;
        }

        private void IncludeFile(string fileName)
        {
            if (Path.IsPathRooted(fileName))
            {
                var encoding = EncodingDetector.DetectEncoding(fileName);
                var textReader = new StreamReader(fileName, encoding);
                tr.IncludeTextReader(textReader, fileName);
                return;
            }

            string currentPath = Directory.GetCurrentDirectory();
            string currentFile = tr.FileName;
            if (currentFile != null)
            {
                currentPath = Path.GetDirectoryName(currentFile);
            }

            fileName = Path.Combine(currentPath, fileName);
            {
                var encoding = EncodingDetector.DetectEncoding(fileName);
                var textReader = new StreamReader(fileName, encoding);
                tr.IncludeTextReader(textReader, fileName);
            }
        }

        private void ReadGlobalGroupNames()
        {
            Token openBrace = tr.ReadToken();
            if (openBrace != '{')
            {
                ThrowFormatException("expected open brace after 'globalgroupnames'");
            }

            while (true)
            {
                Token token = tr.ReadToken();
                if (token == '}')
                {
                    break;
                }
                if (token == ',' || token == ';')
                {

                }
                else
                {
                    this.globalGroupNames.SetOrAdd(currentGroupIndex, token.Value);
                    this.GlobalGroupNameToIndex.Set(token.Value, currentGroupIndex);
                    currentGroupIndex++;
                }
            }
        }

        private void ReadGlobals()
        {
            Token openBrace = tr.ReadToken();
            if (openBrace != '{')
            {
                ThrowFormatException("Expected { after 'globals'");
            }

            //globals
            //{
            //repeat:
            //  type [namespace.]name [= initialvalue];
            //}

            while (true)
            {
                Token separator = tr.PeekToken();
                if (separator == '}')
                {
                    break;
                }

                var global = globals.GetOrNew(currentGlobal);
                string globalName;
                var dataType = ReadDataType();
                global.SetVariableType(dataType);
                global.GroupIndex = -1;

                string name = tr.ReadToken().Value;
                globalName = name;
                global.Name = name;

                separator = tr.PeekToken();
                if (separator == ';')
                {
                    tr.ReadToken();
                }
                else
                {
                    if (separator == '.')
                    {
                        tr.ReadToken();
                        string name2 = tr.ReadToken().Value;
                        globalName = name + "." + name2;
                        if (GlobalGroupNameToIndex.ContainsKey(name))
                        {
                            global.Name = name2;
                            global.GroupIndex = GlobalGroupNameToIndex[name];
                        }
                        else
                        {
                            global.Name = globalName;
                            global.GroupIndex = -1;
                        }
                        separator = tr.PeekToken();
                    }
                    if (separator == '=')
                    {
                        tr.ReadToken();
                        var globalInitialValue = globalInitialValues.GetOrNew(currentGlobalInitialValue);
                        globalInitialValue.DataType = dataType.DataType;
                        globalInitialValue.GlobalIndex = currentGlobal;

                        var initialValue = tr.ReadToken();
                        if (dataType.DataType == DataType.String)
                        {
                            globalInitialValue.StringValue = initialValue.Value;
                        }
                        else if (dataType.DataType == DataType.Float)
                        {
                            globalInitialValue.IntValue = BitConverter.ToInt32(BitConverter.GetBytes(initialValue.ToFloat()), 0);
                        }
                        else if (dataType.DataType == DataType.Bool)
                        {
                            if (initialValue.IsNumber())
                            {
                                globalInitialValue.IntValue = initialValue.ToInt();
                            }
                            else
                            {
                                globalInitialValue.IntValue = (initialValue.AsLowercase == "true") ? 1 : 0;
                            }
                        }
                        else
                        {
                            globalInitialValue.IntValue = initialValue.ToInt();
                        }

                        globalInitialValues.SetOrAdd(currentGlobalInitialValue, globalInitialValue);
                        currentGlobalInitialValue++;
                        separator = tr.PeekToken();
                    }
                    if (separator == ';')
                    {
                        tr.ReadToken();
                    }
                }

                globals.SetOrAdd(currentGlobal, global);
                GlobalNameToIndex.Set(globalName, currentGlobal);
                //allow globals to be recognized without the namespace name if one without does not override it
                if (global.GroupIndex != -1)
                {
                    if (!GlobalNameToIndex.ContainsKey(global.Name))
                    {
                        GlobalNameToIndex.Add(global.Name, currentGlobal);
                    }
                    else
                    {
                        if (firstPass)
                        {

                        }
                    }
                }
                currentGlobal++;
            }

            Token closeBrace = tr.ReadToken();
            if (closeBrace != '}')
            {
                ThrowFormatException("Expected } after globals");
            }
        }

        private void ReadStruct()
        {
            //struct name
            //{
            //repeat:
            //  type name
            //  members separated by commas
            //  semicolon
            //}

            var structInfo = structs.GetOrNew(currentStruct);
            List<Variable> members = new List<Variable>();

            string structName = tr.ReadToken().Value;
            structInfo.Name = structName;

            Token openBrace = tr.ReadToken();
            if (openBrace != '{')
            {
                ThrowFormatException("Expected { after struct name");
            }

            while (true)
            {
                Token token = tr.PeekToken();
                if (token == '}')
                {
                    break;
                }
                if (token == ',')
                {
                    tr.ReadToken();
                    continue;
                }
                if (token == ';')
                {
                    tr.ReadToken();
                    continue;
                }
                var dataType = ReadDataType();
                if (dataType.DataType == DataType.Void)
                {
                    dataType.StructType = 0;
                }

                while (true)
                {
                    Token variableName = tr.ReadToken();
                    var newMember = dataType.Clone();
                    newMember.Name = variableName.Value;
                    members.Add(newMember);

                    Token separator = tr.PeekToken();
                    if (separator == ';')
                    {
                        tr.ReadToken();
                        break;
                    }
                    else if (separator == ',')
                    {
                        tr.ReadToken();
                    }
                    else
                    {
                        ThrowFormatException("Expected comma or semicolon to separate struct fields");
                    }
                }
            }

            Token endBrace = tr.ReadToken();
            if (endBrace != '}')
            {
                ThrowFormatException("Expected end brace after struct declaration");
            }

            Token semicolon = tr.PeekToken();
            if (semicolon == ';')
            {
                tr.ReadToken();
            }

            structInfo.Members.Clear();
            structInfo.Members.AddRange(members);
            structInfo.NumberOfMembers = members.Count;
            structs.SetOrAdd(currentStruct, structInfo);

            var dic = StructVariablesInverse.GetOrNew(currentStruct);
            for (int i = 0; i < members.Count; i++)
            {
                dic.Set(members[i].Name, i);
            }
            StructVariablesInverse.SetOrAdd(currentStruct, dic);

            StructNameToIndex.Set(structName, currentStruct);
            currentStruct++;
        }

        public static string ToInitialUppercase(string str)
        {
            if (str.Length >= 1)
            {
                return str.Substring(0, 1).ToUpperInvariant() + str.Substring(1).ToLowerInvariant();
            }
            else
            {
                return "";
            }
        }

        private Variable ReadDataType()
        {
            var token = tr.ReadToken().Value;
            bool isRef = false;
            bool isArray = false;
            int arrayDimensions = 0;
            int structType = -1;

            if (token == "ref")
            {
                isRef = true;
                token = tr.ReadToken().Value;
            }

            if (token.StartsWith("array@"))
            {
                isArray = true;
                token = token.Substring(6);

                if (token.Contains("@"))
                {
                    int atPos = token.IndexOf('@');
                    string arrayDimensionsString = token.Substring(atPos + 1);
                    arrayDimensions = int.Parse(arrayDimensionsString, CultureInfo.InvariantCulture);
                    token = token.Substring(0, atPos);
                }
                else
                {
                    arrayDimensions = 1;
                }
            }

            if (StructNameToIndex.ContainsKey(token))
            {
                int structIndex = StructNameToIndex[token];
                structType = structIndex;
                token = "struct";
            }
            else if (FunctionTypeNameToIndex.ContainsKey(token))
            {
                int functionIndex = FunctionTypeNameToIndex[token];
                structType = functionIndex;
                token = "functype";
            }
            else if (DelegateNameToIndex.ContainsKey(token))
            {
                int functionIndex = DelegateNameToIndex[token];
                structType = functionIndex;
                token = "delegate";
            }

        Back:
            StringBuilder sb = new StringBuilder();
            if (isRef)
            {
                sb.Append("Ref");
            }
            if (isArray)
            {
                sb.Append("Array");
            }
            sb.Append(ToInitialUppercase(token));

            string dataTypeName = sb.ToString();

            DataType dataType = (DataType)0;
            if (DataTypeNames.ContainsKey(dataTypeName))
            {
                dataType = DataTypeNames[dataTypeName];
            }
            else
            {
                if (token != "struct")
                {
                    token = "struct";
                    goto Back;
                }
                else
                {
                    dataType = DataType.Struct;
                }
            }
            return new Variable() { Name = "", DataType = dataType, StructType = structType, ArrayDimensions = arrayDimensions };
        }

        void ThrowFormatException(string message)
        {
            if (0.GetHashCode() == 0)
            {
                throw new FormatException(message + ("\r\nline: " + tr.Row + " column: " + tr.Column + " file:" + tr.FileName));
            }
            else
            {
                return;
            }
        }
    }

    public static partial class Extensions
    {
        public static void SetVariableType(this IVariable variable, IVariable other)
        {
            if (variable == null || other == null)
            {
                return;
            }
            variable.ArrayDimensions = other.ArrayDimensions;
            variable.DataType = other.DataType;
            variable.StructType = other.StructType;
        }

        public static Struct GetStructType(this IVariable variable)
        {
            var ainFile = variable.Root;
            int structType = variable.StructType;

            if (ainFile != null && structType >= 0 && structType < ainFile.Structs.Count)
            {
                return ainFile.Structs[structType];
            }
            else
            {
                return null;
            }
        }

        public static string GetDataTypeName(this IVariable variable)
        {
            return ExpressionDisplayer.GetDataTypeNameReal(variable);
        }
    }
}
