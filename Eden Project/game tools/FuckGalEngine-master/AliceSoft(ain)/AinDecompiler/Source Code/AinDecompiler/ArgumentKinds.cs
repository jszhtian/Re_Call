using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Text;

namespace AinDecompiler
{
    public enum ArgumentKind
    {
        Int,
        CodeAddress,
        Function,
        Float,
        String,
        StructType,
        Message,
        Library,
        LibraryFunction,
        Global,
        Local,
        Switch,
        Filename,
        SystemCall,
        Member,
        LocalMember,
        AssignInt,
        DelegateType,
    }

    public static class ArgumentKinds
    {
        public static Dictionary<int, ArgumentKind[]> InstructionArgumentKinds = null;
        public static Dictionary<int, int> BranchInstructions = null;
        public static Dictionary<int, int> InstructionsThatUseStrings = null;

        internal static void InitArgumentKinds()
        {
            var dic1 = new Dictionary<Instruction, ArgumentKind[]>();

            dic1.Set(Instruction.PUSH, new ArgumentKind[] { ArgumentKind.Int });
            dic1.Set(Instruction.JUMP, new ArgumentKind[] { ArgumentKind.CodeAddress });
            dic1.Set(Instruction.IFZ, new ArgumentKind[] { ArgumentKind.CodeAddress });
            dic1.Set(Instruction.IFNZ, new ArgumentKind[] { ArgumentKind.CodeAddress });
            dic1.Set(Instruction.CALLFUNC, new ArgumentKind[] { ArgumentKind.Function });
            dic1.Set(Instruction.F_PUSH, new ArgumentKind[] { ArgumentKind.Float });
            dic1.Set(Instruction.S_PUSH, new ArgumentKind[] { ArgumentKind.String });
            dic1.Set(Instruction.SR_REF, new ArgumentKind[] { ArgumentKind.StructType });
            dic1.Set(Instruction.MSG, new ArgumentKind[] { ArgumentKind.Message });
            dic1.Set(Instruction.CALLHLL, new ArgumentKind[] { ArgumentKind.Library, ArgumentKind.LibraryFunction });
            dic1.Set(Instruction.CALLMETHOD, new ArgumentKind[] { ArgumentKind.Function });
            dic1.Set(Instruction.SH_GLOBALREF, new ArgumentKind[] { ArgumentKind.Global });
            dic1.Set(Instruction.SH_LOCALREF, new ArgumentKind[] { ArgumentKind.Local });
            dic1.Set(Instruction.SWITCH, new ArgumentKind[] { ArgumentKind.Switch });
            dic1.Set(Instruction.STRSWITCH, new ArgumentKind[] { ArgumentKind.Switch });
            dic1.Set(Instruction.FUNC, new ArgumentKind[] { ArgumentKind.Function });
            dic1.Set(Instruction.EOF, new ArgumentKind[] { ArgumentKind.Filename });
            dic1.Set(Instruction.CALLSYS, new ArgumentKind[] { ArgumentKind.SystemCall });
            dic1.Set(Instruction.SH_STRUCTREF, new ArgumentKind[] { ArgumentKind.LocalMember });
            dic1.Set(Instruction.ENDFUNC, new ArgumentKind[] { ArgumentKind.Function });
            dic1.Set(Instruction.SH_LOCALCREATE, new ArgumentKind[] { ArgumentKind.Local, ArgumentKind.AssignInt });
            dic1.Set(Instruction.SH_LOCALDELETE, new ArgumentKind[] { ArgumentKind.Local });
            dic1.Set(Instruction.SH_LOCALINC, new ArgumentKind[] { ArgumentKind.Local });
            dic1.Set(Instruction.SH_LOCALDEC, new ArgumentKind[] { ArgumentKind.Local });
            dic1.Set(Instruction.SH_LOCALASSIGN, new ArgumentKind[] { ArgumentKind.Local, ArgumentKind.AssignInt });
            dic1.Set(Instruction.SR_REF2, new ArgumentKind[] { ArgumentKind.StructType });
            dic1.Set(Instruction.S_REF2, new ArgumentKind[] { ArgumentKind.Member });
            dic1.Set(Instruction.PAGE_REF, new ArgumentKind[] { ArgumentKind.Member });

            dic1.Set(Instruction.DG_CALLBEGIN, new ArgumentKind[] { ArgumentKind.DelegateType });
            //unknown instructions:
            dic1.Set(Instruction.DG_CALL, new ArgumentKind[] { ArgumentKind.DelegateType, ArgumentKind.CodeAddress });

            //dic1.Set(Instruction.SH_MEM_ASSIGN_LOCAL, new ArgumentKind[] { ArgumentKind.Local, ArgumentKind.Int });

            var dic2 = new Dictionary<int, ArgumentKind[]>();
            foreach (var pair in dic1)
            {
                dic2.Set((int)pair.Key, pair.Value);
            }

            //run it 3 times for some reason?
            AddCompositeInstructions(dic2);
            AddCompositeInstructions(dic2);
            AddCompositeInstructions(dic2);

            InstructionArgumentKinds = dic2;

            BranchInstructions = new Dictionary<int, int>();
            //find branch instructions
            foreach (var pair in dic2)
            {
                int i = Array.IndexOf(pair.Value, ArgumentKind.CodeAddress);
                if (i >= 0)
                {
                    BranchInstructions.Add(pair.Key, i);
                }
            }

            InstructionsThatUseStrings = new Dictionary<int, int>();
            foreach (var pair in dic2)
            {
                int i = Array.IndexOf(pair.Value, ArgumentKind.String);
                if (i >= 0)
                {
                    InstructionsThatUseStrings.Add(pair.Key, i);
                }
            }
        }

        private static void AddCompositeInstructions(Dictionary<int, ArgumentKind[]> dic2)
        {
            foreach (var pair in CompositeInstructions.ExpandedInstructions)
            {
                Instruction instruction = (Instruction)pair.Key;
                var instructions = pair.Value;
                int numberOfArguments = ((Instruction)instruction).GetNumberOfWords();
                if (numberOfArguments > 0)
                {
                    var argumentKinds = new ArgumentKind[numberOfArguments];
                    GetArgumentKinds(instructions, argumentKinds, dic2);
                    dic2.Set((int)instruction, argumentKinds);
                }
            }
        }

        private static void GetArgumentKinds(IList<Instruction> instructions, ArgumentKind[] argumentKinds, Dictionary<int, ArgumentKind[]> dic)
        {
            int wordNumber = 0;
            Instruction page = 0;
            for (int i = 0; i < instructions.Count; i++)
            {
                var instruction = instructions[i];
                switch (instruction)
                {
                    case Instruction.Literal0:
                    case Instruction.Literal1:
                    case Instruction.Literal19:
                        wordNumber = -1;
                        continue;
                    case Instruction.Word1:
                        wordNumber = 0;
                        continue;
                    case Instruction.Word2:
                        wordNumber = 1;
                        continue;
                    case Instruction.Word3:
                        wordNumber = 2;
                        continue;
                    case Instruction.PUSHGLOBALPAGE:
                    case Instruction.PUSHLOCALPAGE:
                    case Instruction.PUSHSTRUCTPAGE:
                        page = instruction;
                        continue;
                    case Instruction.PUSH:
                        break;
                    case Instruction.SR_REF:
                    case Instruction.SR_REF2:
                    case Instruction.SR_REFREF:
                        page = Instruction.SR_REF;
                        break;
                    default:
                        page = 0;
                        break;

                }
                if (dic.ContainsKey((int)instruction))
                {
                    if (wordNumber >= 0 && wordNumber < argumentKinds.Length)
                    {
                        var kinds = dic[(int)instruction];
                        var kind = kinds[0];
                        if (instruction == Instruction.PUSH && page != 0)
                        {
                            if (page == Instruction.PUSHLOCALPAGE)
                            {
                                kind = ArgumentKind.Local;
                            }
                            if (page == Instruction.PUSHSTRUCTPAGE)
                            {
                                kind = ArgumentKind.LocalMember;
                            }
                            if (page == Instruction.PUSHGLOBALPAGE)
                            {
                                kind = ArgumentKind.Global;
                            }
                            if (page == Instruction.SR_REF)
                            {
                                kind = ArgumentKind.Member;
                            }
                            page = 0;
                        }
                        else
                        {
                            //if (kind == ArgumentKind.Local || kind == ArgumentKind.Member || kind == ArgumentKind.Global)
                            //{
                            //    page = Instruction.PUSHSTRUCTPAGE;
                            //}
                        }
                        argumentKinds[wordNumber] = kind;
                    }
                }
            }
        }
    }
}
