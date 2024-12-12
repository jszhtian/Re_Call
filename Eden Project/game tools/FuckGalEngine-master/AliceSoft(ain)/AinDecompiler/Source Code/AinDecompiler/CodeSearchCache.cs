using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Text;

namespace AinDecompiler
{
    public class CodeSearchCache
    {
        AinFile ainFile;
        CodeSearcher codeSearcher;
        public CodeSearchCache(AinFile ainFile)
        {
            this.ainFile = ainFile;
            this.codeSearcher = new CodeSearcher(ainFile);
            this.FunctionsCache = new Cache<Function>(ainFile, this.GetCalls2);
            this.GlobalsCache = new AllMatchesCache<Global>(ainFile, this.AddGlobals);
            this.IntsCache = new AllMatchesCache<int>(ainFile, this.AddInts);
            this.StringsCache = new AllMatchesCache<string>(ainFile, this.AddStrings);
            this.MessagesCache = new AllMatchesCache<string>(ainFile, this.AddMessages);
            this.LibraryCallsCache = new AllMatchesCache<HllFunction>(ainFile, this.AddLibraryCalls);
            this.SystemCallsCache = new AllMatchesCache<Function>(ainFile, this.AddSystemCalls);
            this.StructCache = new Cache2(ainFile);
        }

        public class Cache<TThing>
        {
            protected AinFile ainFile;
            public Cache(AinFile ainFile, Func<Function, HashSet<TThing>> getUsesFunction)
            {
                this.ainFile = ainFile;
                this.GetUsesFunction = getUsesFunction;
            }

            protected Dictionary<Function, HashSet<TThing>> Uses = new Dictionary<Function, HashSet<TThing>>();
            protected Dictionary<TThing, HashSet<Function>> UsedBy = new Dictionary<TThing, HashSet<Function>>();
            protected HashSet<Function> _functionsThatUseSomething = null;
            public HashSet<Function> FunctionsThatUseSomething
            {
                get
                {
                    if (_functionsThatUseSomething == null)
                    {
                        _functionsThatUseSomething = new HashSet<Function>();

                        foreach (var function in ainFile.Functions)
                        {
                            var uses = GetUses(function);
                            if (uses.Count > 0)
                            {
                                _functionsThatUseSomething.Add(function);
                            }
                        }
                    }
                    return _functionsThatUseSomething;
                }
            }

            public HashSet<TThing> GetUses(Function startFunction)
            {
                if (Uses.ContainsKey(startFunction))
                {
                    return Uses[startFunction];
                }
                else
                {
                    var set = GetUsesFunction(startFunction);
                    Uses.Add(startFunction, set);
                    return set;
                }
            }

            public HashSet<Function> GetUsedBy(TThing thing)
            {
                if (UsedBy.ContainsKey(thing))
                {
                    return UsedBy[thing];
                }
                else
                {
                    var set = GetUsedBy2(thing);
                    UsedBy.Add(thing, set);
                    return set;
                }
            }

            protected Func<Function, HashSet<TThing>> GetUsesFunction;

            protected HashSet<Function> GetUsedBy2(TThing thing)
            {
                HashSet<Function> result = new HashSet<Function>();
                foreach (var function in FunctionsThatUseSomething)
                {
                    var set = GetUses(function);
                    if (set.Contains(thing))
                    {
                        result.Add(function);
                    }
                }
                return result;
            }
        }

        public class AllMatchesCache<TThing>
        {
            protected AinFile ainFile;
            public AllMatchesCache(AinFile ainFile, Action<Function, Action<TThing>> intToObjectFunction)
            {
                this.ainFile = ainFile;
                this.IntToObjectFunction = intToObjectFunction;
            }

            bool created = false;

            private Dictionary<Function, HashSet<TThing>> Uses = new Dictionary<Function, HashSet<TThing>>();
            private Dictionary<TThing, HashSet<Function>> UsedBy = new Dictionary<TThing, HashSet<Function>>();

            private void GetAllUses()
            {
                foreach (var function in ainFile.Functions)
                {
                    GetUsesInternal(function);
                }
                created = true;
            }

            private void GetUsesInternal(Function startFunction)
            {
                var set = new HashSet<TThing>();
                IntToObjectFunction(startFunction,
                    (thing) =>
                    {
                        set.Set(thing);
                        UsedBy.Set(thing, startFunction);
                    });
                Uses.Add(startFunction, set);
            }

            public HashSet<TThing> GetUses(Function startFunction)
            {
                if (!created)
                {
                    GetAllUses();
                }
                return Uses.GetOrNew(startFunction);
            }

            public HashSet<Function> GetUsedBy(TThing thing)
            {
                if (!created)
                {
                    GetAllUses();
                }
                return UsedBy.GetOrNew(thing);
            }

            private Action<Function, Action<TThing>> IntToObjectFunction;
        }

        public Cache<Function> FunctionsCache;
        public AllMatchesCache<Global> GlobalsCache;
        public AllMatchesCache<int> IntsCache;
        public AllMatchesCache<string> StringsCache;
        public AllMatchesCache<string> MessagesCache;
        public AllMatchesCache<HllFunction> LibraryCallsCache;
        public AllMatchesCache<Function> SystemCallsCache;

        void AddInts(Function startFunction, Action<int> addSomething)
        {
            codeSearcher.GetAllSomething(startFunction, ArgumentKind.Int, addSomething);
        }

        void AddStrings(Function startFunction, Action<string> addSomething)
        {
            codeSearcher.GetAllSomething(startFunction, ArgumentKind.String, (i) => addSomething(ainFile.Strings[i]));
        }

        void AddMessages(Function startFunction, Action<string> addSomething)
        {
            codeSearcher.GetAllSomething(startFunction, ArgumentKind.Message, (i) => addSomething(ainFile.Messages[i]));
        }

        void AddGlobals(Function startFunction, Action<Global> addSomething)
        {
            codeSearcher.GetAllSomething(startFunction, ArgumentKind.Global, (i) => addSomething(ainFile.Globals[i]));
        }

        void AddLibraryCalls(Function startFunction, Action<HllFunction> addSomething)
        {
            codeSearcher.GetAllSomething(startFunction, ArgumentKind.LibraryFunction, (i) => addSomething(ainFile.Libraries[codeSearcher.libraryNumber].Functions[i]));
        }

        void AddSystemCalls(Function startFunction, Action<Function> addSomething)
        {
            codeSearcher.GetAllSomething(startFunction, ArgumentKind.SystemCall, (i) => addSomething(AinFile.SystemCalls[i]));
        }

        HashSet<int> UsesInts(Function startFunction)
        {
            return codeSearcher.GetAllSomething(startFunction, ArgumentKind.Int, (i) => i);
        }

        HashSet<string> UsesStrings(Function startFunction)
        {
            return codeSearcher.GetAllSomething(startFunction, ArgumentKind.String, (i) => ainFile.Strings[i]);
        }

        HashSet<string> UsesMessages(Function startFunction)
        {
            return codeSearcher.GetAllSomething(startFunction, ArgumentKind.Message, (i) => ainFile.Messages[i]);
        }

        HashSet<Global> GetUsesGlobals2(Function startFunction)
        {
            return codeSearcher.GetAllSomething(startFunction, ArgumentKind.Global, (i) => ainFile.Globals[i]);
            //HashSet<Global> globals = new HashSet<Global>();
            //codeSearcher.EnumerateCode(startFunction, ArgumentKind.Global, (i) =>
            //{
            //    globals.Set(ainFile.Globals[i]);
            //    return false;
            //});
            //return globals;
        }

        HashSet<Function> GetCalls2(Function startFunction)
        {
            HashSet<Function> calls = new HashSet<Function>();
            int address = startFunction.Address;
            int lastAddress = ainFile.Code.Length;
            InstructionInfo previousInstructionInfo = default(InstructionInfo);
            if (address < lastAddress)
            {
                previousInstructionInfo = Decompiler.Peek(ainFile.Code, address);
            }
            while (address < lastAddress)
            {
                int functionIndex = -1;
                var instructionInfo = Decompiler.Peek(ainFile.Code, address);

                if (instructionInfo.instruction == Instruction.FUNC ||
                    instructionInfo.instruction == Instruction.ENDFUNC)
                {
                    break;
                }

                if (instructionInfo.instruction == Instruction.CALLFUNC ||
                    instructionInfo.instruction == Instruction.CALLMETHOD)
                {
                    functionIndex = instructionInfo.word1;
                }

                if (instructionInfo.instruction == Instruction.CALLONJUMP)
                {
                    if (previousInstructionInfo.instruction == Instruction.S_PUSH)
                    {
                        int stringIndex = previousInstructionInfo.word1;
                        if (stringIndex >= 0 && stringIndex < ainFile.Strings.Count)
                        {
                            string str = ainFile.Strings[stringIndex];
                            if (ainFile.FunctionNameToIndex.ContainsKey(str))
                            {
                                functionIndex = ainFile.FunctionNameToIndex[str];
                            }
                        }
                    }
                }

                if (instructionInfo.instruction == Instruction.FT_ASSIGNS)
                {
                    if (previousInstructionInfo.instruction == Instruction.PUSH)
                    {
                        int functionTypeNumber = previousInstructionInfo.word1;
                        if (functionTypeNumber >= 0 && functionTypeNumber < ainFile.FunctionTypes.Count)
                        {
                            var stringInstructionInfo = Decompiler.Peek(ainFile.Code, previousInstructionInfo.CurrentAddress - 6);
                            if (stringInstructionInfo.instruction == Instruction.S_PUSH)
                            {
                                int stringNumber = stringInstructionInfo.word1;
                                if (stringNumber >= 0 && stringNumber < ainFile.Strings.Count)
                                {
                                    string functionName = ainFile.Strings[stringNumber];
                                    if (ainFile.FunctionNameToIndex.ContainsKey(functionName))
                                    {
                                        var function = ainFile.Functions[ainFile.FunctionNameToIndex[functionName]];
                                        functionIndex = function.Index;
                                    }
                                }
                            }
                        }
                    }
                }

                if (functionIndex >= 0 && functionIndex < ainFile.Functions.Count)
                {
                    var function = ainFile.Functions[functionIndex];
                    if (!calls.Contains(function))
                    {
                        calls.Add(function);
                    }
                    functionIndex = -1;
                }
                address = instructionInfo.nextAddress;
                previousInstructionInfo = instructionInfo;
            }
            return calls;
        }

        //HashSet<Function> _functionsThatCallSomething = null;
        //HashSet<Function> _functionsThatUseAGlobal = null;

        //Dictionary<Function, HashSet<Function>> Calls = new Dictionary<Function, HashSet<Function>>();
        //Dictionary<Function, HashSet<Function>> CalledBy = new Dictionary<Function, HashSet<Function>>();
        //Dictionary<Function, HashSet<Global>> UsesGlobals = new Dictionary<Function, HashSet<Global>>();
        //Dictionary<Global, HashSet<Function>> GlobalUsedBy = new Dictionary<Global, HashSet<Function>>();
        //Dictionary<Function, HashSet<int>> UsesInts = new Dictionary<Function, HashSet<int>>();
        //Dictionary<int, HashSet<Function>> IntUsedBy = new Dictionary<int, HashSet<Function>>();
        //Dictionary<Function, HashSet<string>> UsesStrings = new Dictionary<Function, HashSet<string>>();
        //Dictionary<string, HashSet<Function>> StringUsedBy = new Dictionary<string, HashSet<Function>>();
        //Dictionary<Function, HashSet<string>> UsesMessages = new Dictionary<Function, HashSet<string>>();
        //Dictionary<string, HashSet<Function>> MessageUsedBy = new Dictionary<string, HashSet<Function>>();

        //public HashSet<Function> GetCalls(Function startFunction)
        //{
        //    if (Calls.ContainsKey(startFunction))
        //    {
        //        return Calls[startFunction];
        //    }
        //    else
        //    {
        //        var set = GetCalls2(startFunction);
        //        Calls.Add(startFunction, set);
        //        return set;
        //    }
        //}

        //public HashSet<Function> GetCalledBy(Function startFunction)
        //{
        //    if (CalledBy.ContainsKey(startFunction))
        //    {
        //        return CalledBy[startFunction];
        //    }
        //    else
        //    {
        //        var set = GetCalledBy2(startFunction);
        //        CalledBy.Add(startFunction, set);
        //        return set;
        //    }
        //}

        //public HashSet<Global> GetUsesGlobals(Function startFunction)
        //{
        //    if (UsesGlobals.ContainsKey(startFunction))
        //    {
        //        return UsesGlobals[startFunction];
        //    }
        //    else
        //    {
        //        var set = GetUsesGlobals2(startFunction);
        //        UsesGlobals.Add(startFunction, set);
        //        return set;
        //    }
        //}

        //public HashSet<Function> GetGlobalUsedBy(Global global)
        //{
        //    if (GlobalUsedBy.ContainsKey(global))
        //    {
        //        return GlobalUsedBy[global];
        //    }
        //    else
        //    {
        //        var set = GetGlobalUsedBy2(global);
        //        GlobalUsedBy.Add(global, set);
        //        return set;
        //    }
        //}

        //HashSet<Function> FunctionsThatUseAGlobal
        //{
        //    get
        //    {
        //        if (_functionsThatUseAGlobal == null)
        //        {
        //            _functionsThatUseAGlobal = new HashSet<Function>();
        //            foreach (var function in ainFile.Functions)
        //            {
        //                var globals = GetUsesGlobals(function);
        //                if (globals.Count > 0)
        //                {
        //                    _functionsThatUseAGlobal.Add(function);
        //                }
        //            }
        //        }
        //        return _functionsThatUseAGlobal;
        //    }
        //}

        //HashSet<Function> GetGlobalUsedBy2(Global global)
        //{
        //    HashSet<Function> usedBy = new HashSet<Function>();
        //    foreach (var function in FunctionsThatUseAGlobal)
        //    {
        //        var globals = GetUsesGlobals(function);
        //        if (globals.Contains(global))
        //        {
        //            usedBy.Add(function);
        //        }
        //    }
        //    return usedBy;
        //}

        //HashSet<Function> FunctionsThatCallSomething
        //{
        //    get
        //    {
        //        if (_functionsThatCallSomething == null)
        //        {
        //            _functionsThatCallSomething = new HashSet<Function>();
        //            foreach (var function in ainFile.Functions)
        //            {
        //                var calls = GetCalls(function);
        //                if (calls.Count > 0)
        //                {
        //                    _functionsThatCallSomething.Add(function);
        //                }
        //            }
        //        }
        //        return _functionsThatCallSomething;
        //    }
        //}

        //HashSet<Function> GetCalledBy2(Function startFunction)
        //{
        //    HashSet<Function> calledBy = new HashSet<Function>();
        //    foreach (var function in FunctionsThatCallSomething)
        //    {
        //        var calls = GetCalls(function);
        //        if (calls.Contains(startFunction))
        //        {
        //            calledBy.Add(function);
        //        }
        //    }
        //    return calledBy;
        //}

        public class CodeSearcher
        {
            public CodeSearcher(AinFile ainFile)
            {
                this.ainFile = ainFile;
            }

            AinFile ainFile;
            int currentStructType = -1;
            int previousStructType = -1;
            int currentFunction = -1;
            int currentClass = -1;

            IList<IVariable> locals;
            //IList<IVariable> members;

            //public List<Function> FindInt(int intValue)
            //{
            //    return FindSomething(ArgumentKind.Int, i =>
            //    {
            //        return i == intValue;
            //    });
            //}

            //public List<Function> FindFunction(int functionIndex)
            //{
            //    List<Function> result = new List<Function>();
            //    foreach (var function in ainFile.Functions)
            //    {
            //        if (FindFunction(function, functionIndex))
            //        {
            //            result.Add(function);
            //        }
            //    }
            //    return result;
            //    //return FindSomething(ArgumentKind.Function, i =>
            //    //{
            //    //    return i == functionIndex;
            //    //});
            //}

            //public List<Function> FindGlobal(int globalIndex)
            //{
            //    return FindSomething(ArgumentKind.Global, i =>
            //    {
            //        return i == globalIndex;
            //    });
            //}

            //public List<Function> FindString(string stringValue)
            //{
            //    return FindSomething(ArgumentKind.String, i =>
            //    {
            //        return i >= 0 && i < ainFile.Strings.Count && ainFile.Strings[i] == stringValue;
            //    });
            //}

            //public List<Function> FindMessage(string messageValue)
            //{
            //    return FindSomething(ArgumentKind.Message, i =>
            //    {
            //        return i >= 0 && i < ainFile.Messages.Length && ainFile.Messages[i] == messageValue;
            //    });
            //}

            //public List<Function> FindStructMember(IVariable structMember)
            //{
            //    var structInfo = structMember.Parent as Struct;
            //    if (structInfo == null)
            //    {
            //        throw new ArgumentException("argument is not a struct member");
            //    }
            //    int structTypeIndex = structInfo.Index;
            //    int memberIndex = structMember.Index;

            //    return FindSomething2(structMember);
            //}

            //public List<Function> FindStructMemberOld(IVariable structMember)
            //{
            //    var structInfo = structMember.Parent as Struct;
            //    if (structInfo == null)
            //    {
            //        throw new ArgumentException("argument is not a struct member");
            //    }
            //    int structTypeIndex = structInfo.Index;
            //    int memberIndex = structMember.Index;

            //    return FindSomething(ArgumentKind.Member, i =>
            //    {
            //        return i == memberIndex && this.previousStructType == structTypeIndex;
            //    });
            //}

            //private List<Function> FindSomething2(IVariable structMember)
            //{
            //    List<Function> result = new List<Function>();
            //    foreach (var function in ainFile.Functions)
            //    {
            //        var expressionMap = ainFile.DecompiledCodeCache.GetExpressionMap(function);
            //        var findResult = expressionMap.FindVariable(structMember);
            //        if (findResult.FirstOrDefault() != null)
            //        {
            //            result.Add(function);
            //        }
            //    }
            //    return result;
            //}

            //List<Function> FindSomething(ArgumentKind kindToFind, Func<int, bool> condition)
            //{
            //    List<Function> result = new List<Function>();
            //    foreach (var function in ainFile.Functions)
            //    {
            //        if (EnumerateCode(function, kindToFind, condition))
            //        {
            //            result.Add(function);
            //        }
            //    }
            //    return result;
            //}

            public HashSet<T> GetAllSomething<T>(Function function, ArgumentKind kindToFind, Func<int, T> objectToReturn)
            {
                HashSet<T> result = new HashSet<T>();
                EnumerateCode(function, kindToFind, (i) => { result.Set(objectToReturn(i)); return false; });
                return result;
            }

            public void GetAllSomething(Function function, ArgumentKind kindToFind, Action<int> action)
            {
                EnumerateCode(function, kindToFind, (i) => { action(i); return false; });
            }

            public bool EnumerateCode(Function function, ArgumentKind kindToFind, Func<int, bool> condition)
            {
                int startAddress = function.Address; //skipping the FUNC instruction
                var decompiler = new Decompiler(ainFile);
                int endAddress = decompiler.FindEndAddress(function.Address);
                currentFunction = function.Index;
                var currentClassObject = function.GetClass();
                currentClass = currentClassObject != null ? currentClassObject.Index : -1;
                //members = function.GetClass().Members;
                locals = ((IFunction)function).Parameters;

                int address = startAddress;

                while (address < endAddress)
                {
                    var instructionInfo = Decompiler.Peek(ainFile.Code, address);
                    int currentAddress = instructionInfo.CurrentAddress;
                    address = instructionInfo.nextAddress;
                    switch (instructionInfo.instruction)
                    {
                        case Instruction.PUSHGLOBALPAGE:
                        case Instruction.PUSHLOCALPAGE:
                        case Instruction.PUSHSTRUCTPAGE:
                        case Instruction.SR_REF:
                        case Instruction.SR_REF2:
                        case Instruction.SR_REFREF:
                        case Instruction.SH_STRUCT_SR_REF:
                            {
                                if (EnumerateGenericInstruction(instructionInfo, 0, kindToFind, condition)) return true;
                                var nextInstruction = Decompiler.Peek(ainFile.Code, address);
                                if (nextInstruction.instruction == Instruction.PUSH)
                                {
                                    var overrideKind = ArgumentKind.Member;
                                    if (instructionInfo.instruction == Instruction.PUSHGLOBALPAGE) overrideKind = ArgumentKind.Global;
                                    if (instructionInfo.instruction == Instruction.PUSHLOCALPAGE) overrideKind = ArgumentKind.Local;
                                    if (instructionInfo.instruction == Instruction.PUSHSTRUCTPAGE) overrideKind = ArgumentKind.LocalMember;
                                    if (EnumerateGenericInstruction(nextInstruction, overrideKind, kindToFind, condition)) return true;
                                    address = nextInstruction.nextAddress;
                                }
                            }
                            break;
                        case Instruction.STRSWITCH:
                            if (kindToFind == ArgumentKind.String)
                            {
                                var switchBlock = ainFile.Switches.GetOrNull(instructionInfo.word1);
                                foreach (var switchCase in switchBlock.SwitchCases)
                                {
                                    int value = switchCase.Value;
                                    if (condition(value)) return true;
                                }
                            }
                            break;
                        case Instruction.SWITCH:
                            if (kindToFind == ArgumentKind.Int)
                            {
                                var switchBlock = ainFile.Switches.GetOrNull(instructionInfo.word1);
                                foreach (var switchCase in switchBlock.SwitchCases)
                                {
                                    int value = switchCase.Value;
                                    if (condition(value)) return true;
                                }
                            }
                            break;
                        //case Instruction.CALLONJUMP:
                        //    {
                        //        if (HandleCallOnJump(instructionInfo, kindToFind, condition)) return true;
                        //    }
                        //    break;
                        default:
                            {
                                if (instructionInfo.totalArguments > 0)
                                {
                                    if (EnumerateGenericInstruction(instructionInfo, 0, kindToFind, condition)) return true;
                                }
                            }
                            break;
                    }
                }
                return false;
            }

            public int libraryNumber;

            bool EnumerateGenericInstruction(InstructionInfo instructionInfo, ArgumentKind overrideKind, ArgumentKind kindToFind, Func<int, bool> condition)
            {
                if (instructionInfo.totalArguments > 0)
                {
                    if (ArgumentKinds.InstructionArgumentKinds.ContainsKey((int)instructionInfo.instruction))
                    {
                        var argumentKinds = ArgumentKinds.InstructionArgumentKinds[(int)instructionInfo.instruction];
                        for (int i = 0; i < instructionInfo.totalArguments; i++)
                        {
                            var argumentKind = argumentKinds[i];
                            if (overrideKind != 0)
                            {
                                argumentKind = overrideKind;
                            }
                            int word = instructionInfo.words[i];
                            switch (argumentKind)
                            {
                                case ArgumentKind.AssignInt:
                                    argumentKind = ArgumentKind.Int;
                                    break;
                                case ArgumentKind.Function:
                                    if (word >= 0 && word < ainFile.Functions.Count)
                                    {
                                        var func = ainFile.Functions[word];
                                        previousStructType = currentStructType;
                                        currentStructType = func.StructType;
                                    }
                                    break;
                                case ArgumentKind.Global:
                                    if (word >= 0 && word < ainFile.Globals.Count)
                                    {
                                        var global = ainFile.Globals[word];
                                        previousStructType = currentStructType;
                                        currentStructType = global.StructType;
                                    }
                                    break;
                                case ArgumentKind.Library:
                                    libraryNumber = word;
                                    break;
                                case ArgumentKind.LibraryFunction:
                                    currentStructType = -1;
                                    break;
                                case ArgumentKind.Local:
                                    if (word >= 0 && word < locals.Count)
                                    {
                                        var local = locals[word];
                                        previousStructType = currentStructType;
                                        currentStructType = local.StructType;
                                    }
                                    break;
                                case ArgumentKind.LocalMember:
                                    argumentKind = ArgumentKind.Member;
                                    currentStructType = currentClass;
                                    if (currentStructType >= 0 && currentStructType < ainFile.Structs.Count)
                                    {
                                        var structInfo = ainFile.Structs[currentStructType];
                                        if (word >= 0 && word < structInfo.Members.Count)
                                        {
                                            var member = structInfo.Members[word];
                                            previousStructType = currentStructType;
                                            currentStructType = member.StructType;
                                        }
                                    }
                                    break;
                                case ArgumentKind.Member:
                                    if (currentStructType >= 0 && currentStructType < ainFile.Structs.Count)
                                    {
                                        var structInfo = ainFile.Structs[currentStructType];
                                        if (word >= 0 && word < structInfo.Members.Count)
                                        {
                                            var member = structInfo.Members[word];
                                            previousStructType = currentStructType;
                                            currentStructType = member.StructType;
                                        }
                                    }
                                    break;
                                case ArgumentKind.StructType:
                                    currentStructType = word;
                                    break;
                                case ArgumentKind.SystemCall:
                                    currentStructType = -1;
                                    break;
                            }
                            if (argumentKind == kindToFind)
                            {
                                if (condition(word))
                                {
                                    return true;
                                }
                            }
                        }
                    }
                }
                return false;
            }

            //bool FindFunction(Function function, int functionToFind)
            //{
            //    int startAddress = function.Address; //skipping the FUNC instruction
            //    var decompiler = new Decompiler(ainFile);
            //    int endAddress = decompiler.FindEndAddress(function.Address);

            //    int address = startAddress;

            //    while (address < endAddress)
            //    {
            //        var instructionInfo = Decompiler.Peek(ainFile.Code, address);
            //        int currentAddress = instructionInfo.CurrentAddress;
            //        address = instructionInfo.nextAddress;
            //        switch (instructionInfo.instruction)
            //        {
            //            case Instruction.CALLFUNC:
            //            case Instruction.CALLMETHOD:
            //                if (instructionInfo.word1 == functionToFind)
            //                {
            //                    return true;
            //                }
            //                break;
            //            case Instruction.THISCALLMETHOD_NOPARAM:
            //            case Instruction.SH_STRUCTREF_CALLMETHOD_NO_PARAM:
            //            case Instruction.SH_STRUCTREF2_CALLMETHOD_NO_PARAM:
            //                {
            //                    int word = instructionInfo.words[instructionInfo.totalArguments - 1];
            //                    if (word == functionToFind)
            //                    {
            //                        return true;
            //                    }
            //                }
            //                break;
            //            case Instruction.CALLONJUMP:
            //                {
            //                    var previousInstruction = Decompiler.Peek(ainFile.Code, instructionInfo.CurrentAddress - 6);
            //                    if (previousInstruction.instruction == Instruction.S_PUSH)
            //                    {
            //                        int stringIndex = previousInstruction.word1;
            //                        if (stringIndex >= 0 && stringIndex < ainFile.Strings.Count)
            //                        {
            //                            string stringValue = ainFile.Strings[stringIndex];
            //                            if (ainFile.FunctionNameToIndex.ContainsKey(stringValue))
            //                            {
            //                                int functionIndex = ainFile.FunctionNameToIndex[stringValue];
            //                                if (functionToFind == functionIndex)
            //                                {
            //                                    return true;
            //                                }
            //                            }
            //                        }
            //                    }

            //                }
            //                break;
            //        }
            //    }
            //    return false;
            //}
        }

        Cache2 StructCache;
        //StructSearcher structSearcher;

        public HashSet<Function> GetFunctionsUsingStructMember(IVariable structMember)
        {
            HashSet<Function> results = new HashSet<Function>();
            if (structMember != null)
            {
                var structInfo = structMember.Parent as Struct;
                if (structInfo != null)
                {
                    var possibleFunctions = GetPossibleFunctionsUsingStructType(structInfo);
                    foreach (var function in possibleFunctions)
                    {
                        var expressionMap = ainFile.DecompiledCodeCache.GetExpressionMap(function);
                        var nodes = expressionMap.FindVariable(structMember);
                        if (nodes.FirstOrDefault() != null)
                        {
                            results.Set(function);
                        }
                    }
                }
            }
            return results;
        }

        public HashSet<Function> GetFunctionsUsingStructType(Struct structInfo)
        {
            HashSet<Function> results = new HashSet<Function>();
            var possibleFunctions = GetPossibleFunctionsUsingStructType(structInfo);
            foreach (var function in possibleFunctions)
            {
                var expressionMap = ainFile.DecompiledCodeCache.GetExpressionMap(function);
                var nodes = expressionMap.FindStructType(structInfo.Index);
                if (nodes.FirstOrDefault() != null)
                {
                    results.Set(function);
                }
            }
            return results;
        }

        HashSet<Function> GetPossibleFunctionsUsingStructType(Struct structInfo)
        {
            var structTypes = GetStructTypesUsingStructType(structInfo);
            var globals = GetGlobalsUsingStructTypes(structTypes);
            var functions = GetFunctionsUsingStructTypes(structTypes);
            foreach (var global in globals)
            {
                var funcs2 = GlobalsCache.GetUsedBy(global);
                functions.UnionWith(funcs2);
            }
            return functions;
        }

        HashSet<Struct> GetStructTypesUsingStructType(Struct structInfo)
        {
            var searcher = new StructSearcher(ainFile, StructCache);
            return searcher.GetStructTypesUsingStructType(structInfo);
        }

        HashSet<Global> GetGlobalsUsingStructTypes(HashSet<Struct> structs)
        {
            HashSet<Global> globalsUsingStructType = new HashSet<Global>();
            foreach (var global in ainFile.Globals)
            {
                if (global.StructType != -1 && structs.Contains(global.GetStructType()))
                {
                    globalsUsingStructType.Set(global);
                }
            }
            return globalsUsingStructType;
        }

        HashSet<Function> GetFunctionsUsingStructTypes(HashSet<Struct> structs)
        {
            HashSet<Function> functionsUsingStructType = new HashSet<Function>();
            //using the struct type as an argument, or local variable, or "this"
            foreach (var function in ainFile.Functions)
            {
                if (structs.Contains(function.GetClass()))
                {
                    functionsUsingStructType.Set(function);
                }
                else
                {
                    foreach (var variable in function.Parameters)
                    {
                        if (variable.StructType != -1 && structs.Contains(variable.GetStructType()))
                        {
                            functionsUsingStructType.Set(function);
                            break;
                        }
                    }
                }
            }
            return functionsUsingStructType;
        }

        class StructSearcher
        {
            Cache2 structCache;
            AinFile ainFile;

            HashSet<Struct> result = new HashSet<Struct>();

            public StructSearcher(AinFile ainFile, Cache2 structCache)
            {
                this.ainFile = ainFile;
                this.structCache = structCache;
            }

            public HashSet<Struct> GetStructTypesUsingStructType(Struct structInfo)
            {
                result.Set(structInfo);
                foreach (var otherStruct in structCache.GetUsedBy(structInfo))
                {
                    if (!result.Contains(otherStruct))
                    {
                        GetStructTypesUsingStructType(otherStruct);
                    }
                }
                return result;
            }
        }

        public class Cache2
        {
            protected AinFile ainFile;
            public Cache2(AinFile ainFile)
            {
                this.ainFile = ainFile;
            }

            public Dictionary<Struct, HashSet<Struct>> Uses = new Dictionary<Struct, HashSet<Struct>>();
            public Dictionary<Struct, HashSet<Struct>> UsedBy = new Dictionary<Struct, HashSet<Struct>>();
            protected HashSet<Struct> _structsThatUseSomething = null;
            public HashSet<Struct> StructsThatUseSomething
            {
                get
                {
                    if (_structsThatUseSomething == null)
                    {
                        _structsThatUseSomething = new HashSet<Struct>();

                        foreach (var structInfo in ainFile.Structs)
                        {
                            var uses = GetUses(structInfo);
                            if (uses.Count > 0)
                            {
                                _structsThatUseSomething.Add(structInfo);
                            }
                        }
                    }
                    return _structsThatUseSomething;
                }
            }

            public HashSet<Struct> GetUses(Struct startStruct)
            {
                if (Uses.ContainsKey(startStruct))
                {
                    return Uses[startStruct];
                }
                else
                {
                    var set = GetUses2(startStruct);
                    Uses.Add(startStruct, set);
                    return set;
                }
            }

            public HashSet<Struct> GetUsedBy(Struct thing)
            {
                if (UsedBy.ContainsKey(thing))
                {
                    return UsedBy[thing];
                }
                else
                {
                    var set = GetUsedBy2(thing);
                    UsedBy.Add(thing, set);
                    return set;
                }
            }

            protected HashSet<Struct> GetUses2(Struct startStruct)
            {
                HashSet<Struct> result = new HashSet<Struct>();
                foreach (var member in startStruct.Members)
                {
                    if (member.StructType != -1)
                    {
                        result.Add(ainFile.Structs[member.StructType]);
                    }
                }
                return result;
            }

            protected HashSet<Struct> GetUsedBy2(Struct thing)
            {
                HashSet<Struct> result = new HashSet<Struct>();
                foreach (var structInfo in StructsThatUseSomething)
                {
                    var set = GetUses(structInfo);
                    if (set.Contains(thing))
                    {
                        result.Add(structInfo);
                    }
                }
                return result;
            }
        }

    }
}
