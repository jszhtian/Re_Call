using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Text;

namespace AinDecompiler
{
    public partial class Expression
    {
        public static AinFile defaultAinFile;
        public static int defaultFunctionNumber;

        static HashSet<Instruction> AssignmentOperations = new HashSet<Instruction>(
            new Instruction[]
            {
                Instruction.ASSIGN, Instruction.PLUSA, Instruction.MINUSA, Instruction.MULA,
                Instruction.DIVA, Instruction.MODA, Instruction.ANDA, Instruction.ORA, 
                Instruction.XORA, Instruction.LSHIFTA, Instruction.RSHIFTA,
                Instruction.F_ASSIGN, Instruction.F_PLUSA, Instruction.F_MINUSA,
                Instruction.F_MULA, Instruction.F_DIVA,
                Instruction.S_ASSIGN, Instruction.S_PLUSA, Instruction.S_PLUSA2,
                Instruction.LI_ASSIGN, Instruction.LI_PLUSA, Instruction.LI_MINUSA, Instruction.LI_MULA,
                Instruction.LI_DIVA, Instruction.LI_MODA, Instruction.LI_ANDA, Instruction.LI_ORA, 
                Instruction.LI_XORA, Instruction.LI_LSHIFTA, Instruction.LI_RSHIFTA,
                Instruction.FT_ASSIGNS,

                Instruction.SR_ASSIGN, Instruction.C_ASSIGN, Instruction.R_ASSIGN,
                Instruction.DG_ASSIGN, Instruction.DG_SET,
                Instruction.DG_PLUSA, Instruction.DG_MINUSA,
            });
        static HashSet<Instruction> BinaryOperations = new HashSet<Instruction>(
            new Instruction[]
            {
                Instruction.ADD, Instruction.SUB, Instruction.MUL, Instruction.DIV,
                Instruction.MOD, Instruction.AND, Instruction.OR, Instruction.XOR,
                Instruction.LSHIFT, Instruction.RSHIFT, Instruction.LT, Instruction.GT,
                Instruction.LTE, Instruction.GTE, Instruction.NOTE, Instruction.EQUALE,

                Instruction.LI_ADD, Instruction.LI_SUB, Instruction.LI_MUL, Instruction.LI_DIV,
                Instruction.LI_MOD, 

                Instruction.F_ADD, Instruction.F_SUB, Instruction.F_MUL, Instruction.F_DIV,
                Instruction.F_LT, Instruction.F_GT,
                Instruction.F_LTE, Instruction.F_GTE, Instruction.F_NOTE, Instruction.F_EQUALE,

                Instruction.S_ADD, //not Instruction.S_MOD, 
                Instruction.S_LT, Instruction.S_GT,
                Instruction.S_LTE, Instruction.S_GTE, Instruction.S_NOTE, Instruction.S_EQUALE,

                //fake instructions added so shorthand commands can use them
                Instruction.LogicalAnd, Instruction.LogicalOr,

                Instruction.DG_NEW_FROM_METHOD,
            });
        static HashSet<Instruction> LeafRefOperations = new HashSet<Instruction>(
            new Instruction[]
            {
                Instruction.SH_LOCALREF, Instruction.SH_GLOBALREF, Instruction.SH_STRUCTREF,
                Instruction.SH_LOCALCREATE, Instruction.SH_LOCALDELETE, Instruction.SH_LOCALINC, Instruction.SH_LOCALDEC, Instruction.SH_LOCALASSIGN,
                Instruction.SH_MEM_ASSIGN_IMM, Instruction.SH_MEM_ASSIGN_LOCAL, Instruction.SH_LOCREF_ASSIGN_MEM,

            });

        static HashSet<Instruction> RefOperations = new HashSet<Instruction>(
            new Instruction[]
            {
                Instruction.REF, Instruction.REFREF,
                Instruction.S_REF, Instruction.S_REFREF, Instruction.S_REF2,
                Instruction.SR_REF, Instruction.SR_REF2,
                Instruction.C_REF, Instruction.A_REF,
                //synthetic instructions
                Instruction.ArrayIndexReference, Instruction.ArrayIndex, Instruction.AssignmentRef, Instruction.GlobalReference, 
                Instruction.LocalReference, Instruction.StructAssignmentRef, Instruction.StructMemberReference, Instruction.ThisReference,
                Instruction.PAGE_REF
            });
        static HashSet<Instruction> UnaryOperations = new HashSet<Instruction>(
            new Instruction[]
            {
                Instruction.INV, Instruction.NOT, Instruction.COMPL, Instruction.INC,
                Instruction.DEC, Instruction.FTOI, Instruction.FTOS, Instruction.ITOF, 
                Instruction.F_INV,Instruction.STOI,Instruction.ITOB,

                Instruction.CALLONJUMP,
                Instruction.LI_INC, Instruction.LI_DEC, Instruction.ITOLI,
                //Instruction.A_FREE,
                //Instruction.DELETE,
                Instruction.DG_COPY,
                Instruction.DG_CLEAR,
                Instruction.DG_NUMOF,
            });

        static HashSet<Instruction> LeafOperations = new HashSet<Instruction>(
            new Instruction[]
            {
                Instruction.PUSH, Instruction.PUSHGLOBALPAGE, Instruction.PUSHLOCALPAGE,
                Instruction.PUSHSTRUCTPAGE, Instruction.F_PUSH, Instruction.S_PUSH,
                Instruction.PUSHSTRUCTPAGE,
            });

        static HashSet<Instruction> SkipThroughInDupOperations = new HashSet<Instruction>(
            new Instruction[]
            {
                Instruction.DELETE,
            });

        static HashSet<Instruction> NoStackLeafOperations = new HashSet<Instruction>(
            new Instruction[]
        {
            Instruction.MSG,
        });

        static HashSet<DataType> referenceDataTypes = new HashSet<DataType>(
            new DataType[]
        {
            DataType.RefArrayFloat, DataType.RefArrayInt, DataType.RefArrayLint, DataType.RefArrayString, DataType.RefArrayStruct,
            DataType.RefBool, DataType.RefFloat, DataType.RefInt, DataType.RefString, DataType.RefStruct,
        });


        public static bool IsAssignment(Instruction instruction)
        {
            return AssignmentOperations.Contains(instruction);
        }
        public static bool IsRef(Instruction instruction)
        {
            return RefOperations.Contains(instruction);
        }
        public static bool IsLeafRef(Instruction instruction)
        {
            return LeafRefOperations.Contains(instruction);
        }

        public static bool IsBinary(Instruction instruction)
        {
            return BinaryOperations.Contains(instruction);
        }
        public static bool IsUnary(Instruction instruction)
        {
            return UnaryOperations.Contains(instruction);
        }
        public static bool IsLeaf(Instruction instruction)
        {
            return LeafOperations.Contains(instruction);
        }
        public static bool IsSkipThrough(Instruction instruction)
        {
            return SkipThroughInDupOperations.Contains(instruction);
        }
        public static bool IsNoStack(Instruction instruction)
        {
            return NoStackLeafOperations.Contains(instruction);
        }

        public static bool IsBlock(Instruction expressionType)
        {
            return (expressionType == Instruction.IfElse ||
                expressionType == Instruction.FUNC ||
                expressionType == Instruction.SWITCH ||
                expressionType == Instruction.STRSWITCH ||
                expressionType == Instruction.WhileLoop ||
                expressionType == Instruction.ForLoop);

        }

        public static bool DataTypeIsReferenceType(DataType dataType)
        {
            return referenceDataTypes.Contains(dataType);
        }

    }

}
