using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Text;

namespace AinDecompiler
{
    static class CompositeInstructions
    {
        public static Dictionary<int, Instruction[]> ExpandedInstructions = null;
        static CompositeInstructions()
        {
            ExpandedInstructions = new Dictionary<int, Instruction[]>();
            CreateExpandedInstructions();
        }

        public static void CreateExpandedInstructions()
        {
            ExpandedInstructions.Set((int)Instruction.SH_MEM_ASSIGN_IMM, new[]
            {
                Instruction.PUSHSTRUCTPAGE,
                Instruction.PUSH,
                Instruction.Word2, Instruction.PUSH,
                Instruction.ASSIGN,
                Instruction.POP,
            });

            ExpandedInstructions.Set((int)Instruction.SH_MEM_ASSIGN_LOCAL, new[]
            {
                Instruction.PUSHSTRUCTPAGE,
                Instruction.PUSH,
                Instruction.Word2, Instruction.SH_LOCALREF,
                Instruction.ASSIGN,
                Instruction.POP,
            });

            ExpandedInstructions.Set((int)Instruction.SH_LOCREF_ASSIGN_MEM, new[]
            {
                Instruction.PUSHLOCALPAGE,
                Instruction.PUSH,
                Instruction.Word2, Instruction.PUSH,
                Instruction.ASSIGN,
                Instruction.POP,
            });


            ExpandedInstructions.Set((int)Instruction.SH_LOCALSTRUCT_ASSIGN_IMM, new[]
            {
                Instruction.SH_LOCALREF,
                Instruction.Word2, Instruction.S_REF2,
                Instruction.Word3, Instruction.PUSH,
                Instruction.S_ASSIGN,
                Instruction.POP,
            });

            ExpandedInstructions.Set((int)Instruction.SH_STRUCTREF3, new[]
            {
                Instruction.SH_STRUCTREF,
                Instruction.Word2, Instruction.S_REF2,
                Instruction.Word3, Instruction.S_REF2,
            });
            
            ExpandedInstructions.Set((int)Instruction.SH_STRUCTREF2, new[]
            {
                //local struct ref, struct member number
                Instruction.SH_STRUCTREF,
                Instruction.Word2, Instruction.S_REF2,
            });

            ExpandedInstructions.Set((int)Instruction.SH_STRUCTREF_CALLMETHOD_NO_PARAM, new[]
            {
                //local struct ref, function number
                Instruction.SH_STRUCTREF,
                Instruction.Word2, Instruction.CALLMETHOD,
            });

            //
            ExpandedInstructions.Set((int)Instruction.SH_STRUCTREF2_CALLMETHOD_NO_PARAM, new[]
            {
                //local struct ref, function number
                Instruction.SH_STRUCTREF,
                Instruction.Word2, Instruction.S_REF2,
                Instruction.Word3, Instruction.CALLMETHOD,
            });

            ExpandedInstructions.Set((int)Instruction.SH_REF_STRUCTREF2, new[]
            {
                Instruction.S_REF2,
                Instruction.Word2, Instruction.S_REF2,
            });


            ExpandedInstructions.Set((int)Instruction.A_NUMOF_STRUCT_1, new[]
            {
                Instruction.PUSHSTRUCTPAGE,
                Instruction.PUSH,
                Instruction.Literal1, Instruction.PUSH,
                Instruction.A_NUMOF,
            });
            ExpandedInstructions.Set((int)Instruction.SH_LOCALREFREF, new[] 
            {
                Instruction.PUSHLOCALPAGE,
                Instruction.PUSH,
                Instruction.REFREF,
            });
            ExpandedInstructions.Set((int)Instruction.SH_GLOBAL_ASSIGN_LOCAL, new[] 
            {
                Instruction.PUSHGLOBALPAGE,
                Instruction.PUSH,
                Instruction.Word2, Instruction.SH_LOCALREF,
                Instruction.ASSIGN,
                Instruction.POP,
            });
            ExpandedInstructions.Set((int)Instruction.SH_IF_STRUCTREF_NE_LOCALREF, new[]
            {
                Instruction.SH_STRUCTREF,
                Instruction.Word2, Instruction.SH_LOCALREF,
                Instruction.EQUALE,
                Instruction.Word3, Instruction.IFZ,
            });
            ExpandedInstructions.Set((int)Instruction.SH_GLOBAL_ASSIGN_IMM, new[]
            {
                Instruction.PUSHGLOBALPAGE,
                Instruction.PUSH,
                Instruction.Word2,Instruction.PUSH,
                Instruction.ASSIGN,
                Instruction.POP,
            });
            ExpandedInstructions.Set((int)Instruction.SH_STRUCTSREF_EQ_LOCALSREF, new[]
            {
                Instruction.PUSHSTRUCTPAGE,
                Instruction.PUSH,
                Instruction.S_REF,
                Instruction.PUSHLOCALPAGE,
                Instruction.Word2, Instruction.PUSH,
                Instruction.S_REF,
                Instruction.S_NOTE,
            });
            ExpandedInstructions.Set((int)Instruction.SH_S_ASSIGN_STR0, new[]
            {
                Instruction.S_PUSH,
                Instruction.S_ASSIGN,
                Instruction.POP,
            });
            ExpandedInstructions.Set((int)Instruction.SH_LOCALSREF_EQ_STR0, new[]
            {
                Instruction.PUSHLOCALPAGE,
                Instruction.PUSH,
                Instruction.S_REF,
                Instruction.Word2, Instruction.S_PUSH,
                Instruction.EQUALE,
                //Instruction.Word3, Instruction.IFZ
            });
            ExpandedInstructions.Set((int)Instruction.SH_LOCALSREF_NE_STR0, new[]
            {
                Instruction.PUSHLOCALPAGE,
                Instruction.PUSH,
                Instruction.S_REF,
                Instruction.Word2, Instruction.S_PUSH,
                Instruction.NOTE,
                //Instruction.Word3, Instruction.IFZ
            });
            ExpandedInstructions.Set((int)Instruction.SH_LOCAL_S_REF, new[]
            {
                Instruction.PUSHLOCALPAGE,
                Instruction.PUSH,
                Instruction.S_REF,
            });
            ExpandedInstructions.Set((int)Instruction.SH_IF_LOC_NE_IMM, new[]
            {
                Instruction.SH_LOCALREF,
                Instruction.Word2, Instruction.PUSH,
                Instruction.EQUALE,
                Instruction.Word3, Instruction.IFZ,
            });
            ExpandedInstructions.Set((int)Instruction.SH_IF_LOC_GE_IMM, new[]
            {
                Instruction.SH_LOCALREF,
                Instruction.Word2, Instruction.PUSH,
                Instruction.LT,
                Instruction.Word3, Instruction.IFZ,
            });
            ExpandedInstructions.Set((int)Instruction.SH_IF_LOC_GT_IMM, new[]
            {
                Instruction.SH_LOCALREF,
                Instruction.Word2, Instruction.PUSH,
                Instruction.LTE,
                Instruction.Word3, Instruction.IFZ,
            });
            ExpandedInstructions.Set((int)Instruction.SH_IF_LOC_LT_IMM, new[]
            {
                Instruction.SH_LOCALREF,
                Instruction.Word2, Instruction.PUSH,
                Instruction.GTE,
                Instruction.Word3, Instruction.IFZ,
            });
            ExpandedInstructions.Set((int)Instruction.SH_SASSIGN_LOCALSREF, new[]
            {
                Instruction.PUSHLOCALPAGE,
                Instruction.PUSH,
                Instruction.S_REF,
                Instruction.S_ASSIGN,
                Instruction.POP,
            });
            ExpandedInstructions.Set((int)Instruction.SH_GLOBAL_S_REF, new[]
            {
                Instruction.PUSHGLOBALPAGE,
                Instruction.PUSH,
                Instruction.S_REF,
            });
            //SH_LOCALASSIGN_SUB_IMM
            ExpandedInstructions.Set((int)Instruction.SH_LOCALASSIGN_SUB_IMM, new[]
            {
                Instruction.SH_LOCALREF,
                Instruction.Word2, Instruction.PUSH,
                Instruction.MINUSA,
                Instruction.POP,
            });
            //SH_STRUCTREF_SASSIGN_LOCALSREF
            ExpandedInstructions.Set((int)Instruction.SH_STRUCTREF_SASSIGN_LOCALSREF, new[]
            {
                Instruction.PUSHSTRUCTPAGE,
                Instruction.PUSH,
                Instruction.S_REF,
                Instruction.Word2, Instruction.SH_LOCALREF,
                Instruction.S_ASSIGN,
                Instruction.POP,
            });
            //SH_LOCALREF_SASSIGN_LOCALSREF
            ExpandedInstructions.Set((int)Instruction.SH_LOCALREF_SASSIGN_LOCALSREF, new[]
            {
                Instruction.PUSHLOCALPAGE,
                Instruction.PUSH,
                Instruction.S_REF,
                Instruction.Word2, Instruction.SH_LOCALREF,
                Instruction.S_ASSIGN,
                Instruction.POP,
            });
            //SH_STRUCT_S_REF
            ExpandedInstructions.Set((int)Instruction.SH_STRUCT_S_REF, new[]
            {
                Instruction.PUSHSTRUCTPAGE,
                Instruction.PUSH,
                Instruction.S_REF,
            });
            //SH_LOCALSREF_EMPTY
            ExpandedInstructions.Set((int)Instruction.SH_LOCALSREF_EMPTY, new[]
            {
                Instruction.PUSHLOCALPAGE,
                Instruction.PUSH,
                Instruction.S_REF,
                Instruction.S_EMPTY,
            });
            //SH_STRUCTSREF_EMPTY
            ExpandedInstructions.Set((int)Instruction.SH_STRUCTSREF_EMPTY, new[]
            {
                Instruction.PUSHSTRUCTPAGE,
                Instruction.PUSH,
                Instruction.S_REF,
                Instruction.S_EMPTY,
            });
            //SH_SASSIGN_STRUCTSREF
            ExpandedInstructions.Set((int)Instruction.SH_SASSIGN_STRUCTSREF, new[]
            {
                Instruction.PUSHSTRUCTPAGE,
                Instruction.PUSH,
                Instruction.S_REF,
                Instruction.S_ASSIGN,
                Instruction.POP,
            });

            //SH_GLOBALSREF_NE_STR0
            ExpandedInstructions.Set((int)Instruction.SH_GLOBALSREF_NE_STR0, new[]
            {
                Instruction.PUSHGLOBALPAGE,
                Instruction.PUSH,
                Instruction.S_REF,
                Instruction.Word2, Instruction.S_PUSH,
                Instruction.S_NOTE,
            });

            //SH_S_ASSIGN_REF
            ExpandedInstructions.Set((int)Instruction.SH_S_ASSIGN_REF, new[]
            {
                Instruction.S_REF,
                Instruction.S_ASSIGN,
                Instruction.POP,
            });

            ExpandedInstructions.Set((int)Instruction.SH_LOCAL_ASSIGN_STRUCTREF, new[]
            {
                Instruction.PUSHLOCALPAGE,
                Instruction.PUSH,
                Instruction.REF,
                Instruction.PUSHSTRUCTPAGE,
                Instruction.Word2, Instruction.PUSH,
                Instruction.REF,
                Instruction.ASSIGN,
                Instruction.POP,
            });

            ExpandedInstructions.Set((int)Instruction.SH_STRUCT_SR_REF, new[]
            {
                Instruction.PUSHSTRUCTPAGE,
                Instruction.PUSH,
                Instruction.Word2, Instruction.SR_REF,
            });

            //SH_IF_STRUCT_A_NOT_EMPTY member, address
            ExpandedInstructions.Set((int)Instruction.SH_IF_STRUCT_A_NOT_EMPTY, new[]
            {
                Instruction.PUSHSTRUCTPAGE,
                Instruction.PUSH,
                Instruction.REF,
                Instruction.A_EMPTY,
                Instruction.Word2, Instruction.IFZ,
            });

            //SH_LOCAL_APUSHBACK_LOCALSREF 0,1
            ExpandedInstructions.Set((int)Instruction.SH_LOCAL_APUSHBACK_LOCALSREF, new[]
            {
                Instruction.PUSHLOCALPAGE,
                Instruction.PUSH,
                Instruction.REF,
                Instruction.PUSHLOCALPAGE,
                Instruction.Word2,Instruction.PUSH,
                Instruction.REF,
                Instruction.A_PUSHBACK,
            });

            //SH_IF_STRUCTREF_Z 2,jumpaddress  (ref is bool)
            ExpandedInstructions.Set((int)Instruction.SH_IF_STRUCTREF_Z, new[]
            {
                Instruction.PUSHSTRUCTPAGE,
                Instruction.PUSH,
                Instruction.REF,
                Instruction.Word2, Instruction.IFZ,
            });

            //SH_SREF_EMPTY
            ExpandedInstructions.Set((int)Instruction.SH_SREF_EMPTY, new[]
            {
                Instruction.S_REF,
                Instruction.S_EMPTY,
            });

            //SH_STRUCT_A_PUSHBACK_LOCAL_STRUCT 
            ExpandedInstructions.Set((int)Instruction.SH_STRUCT_A_PUSHBACK_LOCAL_STRUCT, new[]
            {
                Instruction.PUSHSTRUCTPAGE,
                Instruction.PUSH,
                Instruction.REF,
                Instruction.PUSHLOCALPAGE,
                Instruction.Word2, Instruction.PUSH,
                Instruction.REF,
                Instruction.A_PUSHBACK,
            });

            //SH_IF_SREF_NE_STR0 1A, jumpAddress
            ExpandedInstructions.Set((int)Instruction.SH_IF_SREF_NE_STR0, new[]
            {
                Instruction.S_REF,
                Instruction.S_PUSH,
                Instruction.S_EQUALE,
                Instruction.Word2, Instruction.IFZ,
            });

            //SH_STRUCTSREF_NE_STR0 1,0
            ExpandedInstructions.Set((int)Instruction.SH_STRUCTSREF_NE_STR0, new[]
            {
                Instruction.PUSHSTRUCTPAGE,
                Instruction.PUSH,
                Instruction.S_REF,
                Instruction.Word2, Instruction.S_PUSH,
                Instruction.S_NOTE,
            });

            //A_NUMOF_GLOB_1
            ExpandedInstructions.Set((int)Instruction.A_NUMOF_GLOB_1, new[]
            {
                Instruction.PUSHGLOBALPAGE,
                Instruction.PUSH,
                Instruction.Literal1, Instruction.PUSH,
                Instruction.A_NUMOF,
            });

            //SH_GLOBAL_APUSHBACK_LOCALSREF
            ExpandedInstructions.Set((int)Instruction.SH_GLOBAL_APUSHBACK_LOCALSREF, new[]
            {
                Instruction.PUSHGLOBALPAGE,
                Instruction.PUSH,
                Instruction.REF,
                Instruction.PUSHLOCALPAGE,
                Instruction.Word2, Instruction.PUSH,
                Instruction.S_REF,
                Instruction.A_PUSHBACK,
            });

            //SH_LOCAL_A_PUSHBACK_LOCAL_STRUCT 1,3
            ExpandedInstructions.Set((int)Instruction.SH_LOCAL_A_PUSHBACK_LOCAL_STRUCT, new[]
            {
                Instruction.PUSHLOCALPAGE,
                Instruction.PUSH,
                Instruction.REF,
                Instruction.PUSHLOCALPAGE,
                Instruction.Word2, Instruction.PUSH,
                Instruction.REF,
                Instruction.A_PUSHBACK,
            });

            //SH_S_ASSIGN_CALLSYS19  calls 'string System.GetFuncStackName(int)'
            ExpandedInstructions.Set((int)Instruction.SH_S_ASSIGN_CALLSYS19, new[]
            {
                Instruction.Literal19,
                Instruction.CALLSYS,
                Instruction.S_ASSIGN,
                Instruction.S_POP,
            });

            //SH_GLOBAL_A_PUSHBACK_LOCAL_STRUCT 0x32, 0x3
            ExpandedInstructions.Set((int)Instruction.SH_GLOBAL_A_PUSHBACK_LOCAL_STRUCT, new[]
            {
                Instruction.SH_GLOBALREF,
                Instruction.Word2, Instruction.SH_LOCALREF,
                Instruction.A_PUSHBACK,
            });

            //SH_LOC_LT_IMM_OR_LOC_GE_IMM
            ExpandedInstructions.Set((int)Instruction.SH_LOC_LT_IMM_OR_LOC_GE_IMM, new[]
            {
                Instruction.SH_LOCALREF,
                Instruction.Word2, Instruction.PUSH,
                Instruction.LT,
                Instruction.Word1,
                Instruction.SH_LOCALREF,
                Instruction.Word3, Instruction.PUSH,
                Instruction.GTE,
                Instruction.LogicalOr,
            });

            //SH_STRUCTREF_GT_IMM
            ExpandedInstructions.Set((int)Instruction.SH_STRUCTREF_GT_IMM, new[]
            {
                Instruction.SH_STRUCTREF,
                Instruction.Word2, Instruction.PUSH,
                Instruction.GT,
            });

            //SH_IF_STRUCTREF_GT_IMM
            ExpandedInstructions.Set((int)Instruction.SH_IF_STRUCTREF_GT_IMM, new[]
            {
                Instruction.SH_STRUCTREF,
                Instruction.Word2, Instruction.PUSH,
                Instruction.LTE,
                Instruction.Word3, Instruction.IFZ,
            });

            //SH_IF_STRUCTREF_EQ_IMM
            ExpandedInstructions.Set((int)Instruction.SH_IF_STRUCTREF_EQ_IMM, new[]
            {
                Instruction.SH_STRUCTREF,
                Instruction.Word2, Instruction.PUSH,
                Instruction.NOTE,
                Instruction.Word3, Instruction.IFZ,
            });

            //SH_IF_STRUCTREF_NE_IMM
            ExpandedInstructions.Set((int)Instruction.SH_IF_STRUCTREF_NE_IMM, new[]
            {
                Instruction.SH_STRUCTREF,
                Instruction.Word2, Instruction.PUSH,
                Instruction.EQUALE,
                Instruction.Word3, Instruction.IFZ,
            });

            //SH_STRUCT_ASSIGN_LOCALREF_ITOB
            ExpandedInstructions.Set((int)Instruction.SH_STRUCT_ASSIGN_LOCALREF_ITOB, new[]
            {
                Instruction.SH_STRUCTREF,
                Instruction.Word2, Instruction.SH_LOCALREF,
                Instruction.ITOB,
                Instruction.ASSIGN,
                Instruction.POP,
            });

            //SH_GLOBALSREF_EMPTY
            ExpandedInstructions.Set((int)Instruction.SH_GLOBALSREF_EMPTY, new[]
            {
                Instruction.SH_GLOBAL_S_REF,
                Instruction.S_EMPTY,
            });

            ExpandedInstructions.Set((int)Instruction.SH_STRUCTSREF_NE_LOCALSREF, new[]
            {
                Instruction.PUSHSTRUCTPAGE,
                Instruction.PUSH,
                Instruction.S_REF,
                Instruction.PUSHLOCALPAGE,
                Instruction.Word2, Instruction.PUSH,
                Instruction.S_REF,
                Instruction.S_NOTE,
            });

            //SH_STRUCT_APUSHBACK_LOCALSREF
            ExpandedInstructions.Set((int)Instruction.SH_STRUCT_APUSHBACK_LOCALSREF, new[]
            {
                Instruction.SH_STRUCTREF,
                Instruction.Word2, Instruction.PUSHLOCALPAGE,
                Instruction.PUSH,
                Instruction.S_REF,
                Instruction.A_PUSHBACK,
            });

            //SH_SASSIGN_GLOBALSREF
            ExpandedInstructions.Set((int)Instruction.SH_SASSIGN_GLOBALSREF, new[]
            {
                Instruction.SH_GLOBAL_S_REF,
                Instruction.ASSIGN,
                Instruction.POP,
            });

            //THISCALLMETHOD_NOPARAM
            ExpandedInstructions.Set((int)Instruction.THISCALLMETHOD_NOPARAM, new[]
            {
                Instruction.PUSHSTRUCTPAGE,
                Instruction.CALLMETHOD,
            });

            ////SH_STRUCTREF_CALLMETHOD_NO_PARAM
            //ExpandedInstructions.Set((int)Instruction.SH_STRUCTREF_CALLMETHOD_NO_PARAM, new[]
            //{
            //    Instruction.SH_STRUCTREF,
            //    Instruction.CALLMETHOD,
            //});




        }

        public static IEnumerable<InstructionInfo> EnumerateInstructions(InstructionInfo baseInstruction)
        {
            var instructionInfo = baseInstruction.Clone();
            var ins = instructionInfo.instruction;

            int nextAddress = instructionInfo.nextAddress;
            int sameAddress = instructionInfo.CurrentAddress;

            if (ExpandedInstructions.ContainsKey((int)ins))
            {
                var arr = ExpandedInstructions[(int)ins];
                for (int i = 0; i < arr.Length; i++)
                {
                    ins = arr[i];
                    instructionInfo.instruction = ins;
                    switch (ins)
                    {
                        case Instruction.Literal0:
                            instructionInfo.word1 = 0;
                            continue;
                        case Instruction.Literal1:
                            instructionInfo.word1 = 1;
                            continue;
                        case Instruction.Literal19:
                            instructionInfo.word1 = 19;
                            continue;
                        case Instruction.Word1:
                            instructionInfo.word1 = baseInstruction.word1;
                            continue;
                        case Instruction.Word2:
                            instructionInfo.word1 = instructionInfo.word2;
                            continue;
                        case Instruction.Word3:
                            instructionInfo.word1 = instructionInfo.word3;
                            continue;
                        default:
                            if (ExpandedInstructions.ContainsKey((int)ins))
                            {
                                foreach (var t in EnumerateInstructions(instructionInfo))
                                {
                                    yield return t;
                                }
                            }
                            else
                            {
                                instructionInfo.totalArguments = ins.GetNumberOfWords();
                                if (i == arr.Length - 1)
                                {
                                    instructionInfo.nextAddress = nextAddress;
                                }
                                else
                                {
                                    instructionInfo.nextAddress = sameAddress;
                                }
                                yield return instructionInfo;
                            }
                            break;
                    }
                }
            }
            else
            {
                yield return instructionInfo;
            }
        }


        //public static InstructionInfo GetNextPiece(InstructionInfo instructionInfo, ref int inputPieceNumber)
        //{
        //    var miniStackIn = new MiniStack(inputPieceNumber);
        //    var miniStack = new MiniStack(0);
        //    int nextAddress = instructionInfo.nextAddress;
        //    int sameAddress = instructionInfo.CurrentAddress;

        //    var ins = instructionInfo.instruction;
        //    Instruction[] arr = null;
        //    int pieceNumber = 0;

        //    do
        //    {
        //        //we're inside recursion
        //        if (ExpandedInstructions.ContainsKey((int)ins))
        //        {
        //            arr = ExpandedInstructions[(int)ins];
        //            pieceNumber = miniStackIn.Pop();
        //            miniStack.Push(pieceNumber);
        //            if (pieceNumber >= 0 && pieceNumber < arr.Length)
        //            {
        //                ins = arr[pieceNumber];
        //            }
        //            else
        //            {
        //                goto NotFound;
        //            }
        //        }
        //        else
        //        {
        //            goto NotFound;
        //        }
        //    } while (miniStack.value > 0);

        //    pieceNumber = miniStack2.Pop();

        //    back:
        //        while (true)
        //        {
        //            if (pieceNumber >= 0 && pieceNumber < arr.Length)
        //            {
        //                break;
        //            }

        //            ins = arr[pieceNumber];
        //            pieceNumber++;
        //            switch (ins)
        //            {
        //                case Instruction.Literal0:
        //                    instructionInfo.word1 = 0;
        //                    continue;
        //                case Instruction.Literal1:
        //                    instructionInfo.word1 = 1;
        //                    continue;
        //                case Instruction.Word2:
        //                    instructionInfo.word1 = instructionInfo.word2;
        //                    continue;
        //                case Instruction.Word3:
        //                    instructionInfo.word1 = instructionInfo.word3;
        //                    continue;
        //            }
        //            instructionInfo.instruction = ins;
        //            if (ExpandedInstructions.ContainsKey((int)ins))
        //            {
        //                miniStack.Push(pieceNumber);
        //                pieceNumber = 0;
        //                goto back;
        //            }

        //            if (pieceNumber >= arr.Length - 1)
        //            {
        //                pieceNumber = 0;
        //                instructionInfo.nextAddress = nextAddress;
        //            }
        //            else
        //            {
        //                instructionInfo.nextAddress = sameAddress;
        //            }
        //            break;
        //        }


        //    }

        //    instructionInfo.totalArguments = instructionInfo.instruction.GetNumberOfWords();

        //    miniStack.Push(pieceNumber);
        //    inputPieceNumber = miniStack.value;
        //    return instructionInfo;
        //NotFound:
        //    inputPieceNumber = 0;
        //    return instructionInfo;
        //}



        //private static InstructionInfo GetNextPiece_old(InstructionInfo instructionInfo, ref int pieceNumber)
        //{
        //    int nextAddress = instructionInfo.nextAddress;
        //    int sameAddress = instructionInfo.CurrentAddress;
        //    switch (instructionInfo.instruction)
        //    {
        //        case Instruction.SH_SR_ASSIGN:
        //            break;
        //        case Instruction.SH_MEM_ASSIGN_LOCAL:
        //            break;
        //        case Instruction.A_NUMOF_GLOB_1:
        //            break;
        //        case Instruction.A_NUMOF_STRUCT_1:
        //            instructionInfo.nextAddress = sameAddress;
        //            if (pieceNumber == 0)
        //            {
        //                instructionInfo.instruction = Instruction.PUSHSTRUCTPAGE;
        //            }
        //            else if (pieceNumber == 1)
        //            {
        //                instructionInfo.instruction = Instruction.PUSH;
        //            }
        //            else if (pieceNumber == 2)
        //            {
        //                instructionInfo.instruction = Instruction.PUSH;
        //                instructionInfo.word1 = 1;
        //            }
        //            else
        //            {
        //                instructionInfo.instruction = Instruction.A_NUMOF;
        //                instructionInfo.nextAddress = nextAddress;
        //            }
        //            break;
        //        case Instruction.SH_MEM_ASSIGN_IMM:
        //            break;
        //        case Instruction.SH_LOCALREFREF:
        //            instructionInfo.nextAddress = sameAddress;
        //            if (pieceNumber == 0)
        //            {
        //                instructionInfo.instruction = Instruction.PUSHLOCALPAGE;
        //            }
        //            else if (pieceNumber == 1)
        //            {
        //                instructionInfo.instruction = Instruction.PUSH;
        //            }
        //            else
        //            {
        //                instructionInfo.instruction = Instruction.REFREF;
        //                instructionInfo.nextAddress = nextAddress;
        //            }
        //            break;
        //        case Instruction.SH_LOCALASSIGN_SUB_IMM:
        //            break;
        //        case Instruction.SH_IF_LOC_LT_IMM:
        //            break;
        //        case Instruction.SH_IF_LOC_GE_IMM:
        //            break;
        //        case Instruction.SH_LOCREF_ASSIGN_MEM:
        //            break;
        //        case Instruction.PAGE_REF:
        //            break;
        //        case Instruction.SH_GLOBAL_ASSIGN_LOCAL:
        //            instructionInfo.nextAddress = sameAddress;
        //            if (pieceNumber == 0)
        //            {
        //                instructionInfo.instruction = Instruction.PUSHGLOBALPAGE;
        //            }
        //            else if (pieceNumber == 1)
        //            {
        //                instructionInfo.instruction = Instruction.PUSH;
        //            }
        //            else if (pieceNumber == 2)
        //            {
        //                instructionInfo.instruction = Instruction.SH_LOCALREF;
        //                instructionInfo.word1 = instructionInfo.word2;
        //            }
        //            else if (pieceNumber == 3)
        //            {
        //                instructionInfo.instruction = Instruction.ASSIGN;
        //            }
        //            else
        //            {
        //                instructionInfo.instruction = Instruction.POP;
        //                instructionInfo.nextAddress = nextAddress;
        //            }
        //            break;
        //        case Instruction.SH_STRUCTREF_GT_IMM:
        //            break;
        //        case Instruction.SH_STRUCT_ASSIGN_LOCALREF_ITOB:
        //            break;
        //        case Instruction.SH_LOCAL_ASSIGN_STRUCTREF:
        //            break;
        //        case Instruction.SH_IF_STRUCTREF_NE_LOCALREF:
        //            instructionInfo.nextAddress = sameAddress;
        //            if (pieceNumber == 0)
        //            {
        //                instructionInfo.instruction = Instruction.SH_STRUCTREF;
        //            }
        //            else if (pieceNumber == 1)
        //            {
        //                instructionInfo.instruction = Instruction.SH_LOCALREF;
        //                instructionInfo.word1 = instructionInfo.word2;
        //            }
        //            else if (pieceNumber == 2)
        //            {
        //                instructionInfo.instruction = Instruction.EQUALE;
        //            }
        //            else if (pieceNumber == 3)
        //            {
        //                instructionInfo.instruction = Instruction.IFZ;
        //                instructionInfo.word1 = instructionInfo.word3;
        //                instructionInfo.nextAddress = nextAddress;
        //            }
        //            break;
        //        case Instruction.SH_IF_STRUCTREF_GT_IMM:
        //            break;
        //        case Instruction.SH_STRUCTREF_CALLMETHOD_NO_PARAM:
        //            break;
        //        case Instruction.SH_STRUCTREF2:
        //            break;
        //        case Instruction.SH_REF_STRUCTREF2:
        //            break;
        //        case Instruction.SH_STRUCTREF3:
        //            break;
        //        case Instruction.SH_STRUCTREF2_CALLMETHOD_NO_PARAM:
        //            break;
        //        case Instruction.SH_IF_STRUCTREF_Z:
        //            break;
        //        case Instruction.SH_IF_STRUCT_A_NOT_EMPTY:
        //            break;
        //        case Instruction.SH_IF_LOC_GT_IMM:
        //            break;
        //        case Instruction.SH_IF_STRUCTREF_NE_IMM:
        //            break;
        //        case Instruction.THISCALLMETHOD_NOPARAM:
        //            break;
        //        case Instruction.SH_IF_LOC_NE_IMM:
        //            break;
        //        case Instruction.SH_IF_STRUCTREF_EQ_IMM:
        //            break;
        //        case Instruction.SH_GLOBAL_ASSIGN_IMM:
        //            instructionInfo.nextAddress = sameAddress;
        //            if (pieceNumber == 0)
        //            {
        //                instructionInfo.instruction = Instruction.PUSHGLOBALPAGE;
        //            }
        //            else if (pieceNumber == 1)
        //            {
        //                instructionInfo.instruction = Instruction.PUSH;
        //            }
        //            else if (pieceNumber == 2)
        //            {
        //                instructionInfo.instruction = Instruction.PUSH;
        //                instructionInfo.word1 = instructionInfo.word2;
        //            }
        //            else if (pieceNumber == 3)
        //            {
        //                instructionInfo.instruction = Instruction.ASSIGN;
        //            }
        //            else
        //            {
        //                instructionInfo.instruction = Instruction.POP;
        //                instructionInfo.nextAddress = nextAddress;
        //            }
        //            break;
        //        case Instruction.SH_LOCALSTRUCT_ASSIGN_IMM:
        //            break;
        //        case Instruction.SH_STRUCT_A_PUSHBACK_LOCAL_STRUCT:
        //            break;
        //        case Instruction.SH_GLOBAL_A_PUSHBACK_LOCAL_STRUCT:
        //            break;
        //        case Instruction.SH_LOCAL_A_PUSHBACK_LOCAL_STRUCT:
        //            break;
        //        case Instruction.SH_IF_SREF_NE_STR0:
        //            break;
        //        case Instruction.SH_S_ASSIGN_REF:
        //            break;
        //        case Instruction.SH_A_FIND_SREF:
        //            break;
        //        case Instruction.SH_SREF_EMPTY:
        //            break;
        //        case Instruction.SH_STRUCTSREF_EQ_LOCALSREF:
        //            instructionInfo.nextAddress = sameAddress;
        //            if (pieceNumber == 0)
        //            {
        //                instructionInfo.instruction = Instruction.PUSHSTRUCTPAGE;
        //            }
        //            else if (pieceNumber == 1)
        //            {
        //                instructionInfo.instruction = Instruction.PUSH;
        //            }
        //            else if (pieceNumber == 2)
        //            {
        //                instructionInfo.instruction = Instruction.S_REF;
        //            }
        //            else if (pieceNumber == 3)
        //            {
        //                instructionInfo.instruction = Instruction.PUSHLOCALPAGE;
        //            }
        //            else if (pieceNumber == 4)
        //            {
        //                instructionInfo.instruction = Instruction.PUSH;
        //                instructionInfo.word1 = instructionInfo.word2;
        //            }
        //            else if (pieceNumber == 5)
        //            {
        //                instructionInfo.instruction = Instruction.S_REF;
        //            }
        //            else if (pieceNumber == 6)
        //            {
        //                instructionInfo.instruction = Instruction.S_NOTE;
        //                instructionInfo.nextAddress = nextAddress;
        //            }
        //            break;
        //        case Instruction.SH_LOCALSREF_EQ_STR0:
        //            break;
        //        case Instruction.SH_STRUCTSREF_NE_LOCALSREF:
        //            break;
        //        case Instruction.SH_LOCALSREF_NE_STR0:
        //            break;
        //        case Instruction.SH_STRUCT_SR_REF:
        //            break;
        //        case Instruction.SH_STRUCT_S_REF:
        //            break;
        //        case Instruction.S_REF2:
        //            break;
        //        case Instruction.SH_REF_LOCAL_ASSIGN_STRUCTREF2:
        //            break;
        //        case Instruction.SH_GLOBAL_S_REF:
        //            break;
        //        case Instruction.SH_LOCAL_S_REF:
        //            instructionInfo.instruction = Instruction.SH_LOCALREF;
        //            break;
        //        case Instruction.SH_LOCALREF_SASSIGN_LOCALSREF:
        //            break;
        //        case Instruction.SH_LOCAL_APUSHBACK_LOCALSREF:
        //            break;
        //        case Instruction.SH_S_ASSIGN_CALLSYS19:
        //            break;
        //            instructionInfo.nextAddress = sameAddress;
        //            if (pieceNumber == 0)
        //            {
        //                instructionInfo.instruction = Instruction.ASSIGN;
        //            }
        //            else if (pieceNumber == 1)
        //            {
        //                instructionInfo.instruction = Instruction.POP;
        //            }
        //            else
        //            {
        //                instructionInfo.instruction = Instruction.CALLHLL;
        //                instructionInfo.nextAddress = nextAddress;
        //            }
        //            break;
        //        case Instruction.SH_S_ASSIGN_STR0:
        //            instructionInfo.nextAddress = sameAddress;
        //            if (pieceNumber == 0)
        //            {
        //                instructionInfo.instruction = Instruction.S_PUSH;
        //            }
        //            else if (pieceNumber == 1)
        //            {
        //                instructionInfo.instruction = Instruction.S_ASSIGN;
        //            }
        //            else
        //            {
        //                instructionInfo.instruction = Instruction.POP;
        //                instructionInfo.nextAddress = nextAddress;
        //            }
        //            break;
        //        case Instruction.SH_SASSIGN_LOCALSREF:
        //            break;
        //        case Instruction.SH_STRUCTREF_SASSIGN_LOCALSREF:
        //            break;
        //        case Instruction.SH_LOCALSREF_EMPTY:
        //            break;
        //        case Instruction.SH_GLOBAL_APUSHBACK_LOCALSREF:
        //            break;
        //        case Instruction.SH_STRUCT_APUSHBACK_LOCALSREF:
        //            break;
        //        case Instruction.SH_STRUCTSREF_EMPTY:
        //            break;
        //        case Instruction.SH_GLOBALSREF_EMPTY:
        //            break;
        //        case Instruction.SH_SASSIGN_STRUCTSREF:
        //            break;
        //        case Instruction.SH_SASSIGN_GLOBALSREF:
        //            break;
        //        case Instruction.SH_STRUCTSREF_NE_STR0:
        //            break;
        //        case Instruction.SH_GLOBALSREF_NE_STR0:
        //            break;
        //        case Instruction.SH_LOC_LT_IMM_OR_LOC_GE_IMM:
        //            break;
        //        case Instruction.UNKNOWN_F1:
        //            break;
        //    }
        //    instructionInfo.totalArguments = instructionInfo.instruction.GetNumberOfWords();
        //    return instructionInfo;
        //}
    }
}
