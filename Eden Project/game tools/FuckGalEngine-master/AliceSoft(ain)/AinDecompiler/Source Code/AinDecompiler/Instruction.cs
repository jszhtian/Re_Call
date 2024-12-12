using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Text;
using System.Diagnostics.CodeAnalysis;

namespace AinDecompiler
{
    [SuppressMessage("Microsoft.Naming", "CA1705:LongAcronymsShouldBePascalCased")]
    [SuppressMessage("Microsoft.Naming", "CA1707:IdentifiersShouldNotContainUnderscores")]
    public enum Instruction
    {
        ///<summary>'Push a 32bit integer onto the stack'</summary>
        PUSH = 0x00,
        ///<summary>'Remove the topmost entry from the stack'</summary>
        POP = 0x01,
        REF = 0x02,
        REFREF = 0x03,
        ///<summary>'Push the global page index onto the stack'</summary>
        PUSHGLOBALPAGE = 0x04,
        ///<summary>'Push the local page index onto the stack'</summary>
        PUSHLOCALPAGE = 0x05,
        ///<summary>'pop A; A=-A; push A'</summary>
        INV = 0x06,
        ///<summary>'pop A; push !A'</summary>
        NOT = 0x07,
        ///<summary>'pop A; push ~A'</summary>
        COMPL = 0x08,
        ///<summary>'pop B; pop A; push A+B'</summary>
        ADD = 0x09,
        ///<summary>'pop B; pop A; push A-B'</summary>
        SUB = 0x0a,
        ///<summary>'pop B; pop A; push A*B'</summary>
        MUL = 0x0b,
        ///<summary>'pop B; pop A; push A/B'</summary>
        DIV = 0x0c,
        ///<summary>'pop B; pop A; push A%B'</summary>
        MOD = 0x0d,
        ///<summary>'pop B; pop A; push A&amp;B'</summary>
        AND = 0x0e,
        ///<summary>'pop B; pop A; push A|B'</summary>
        OR = 0x0f,
        ///<summary>'pop B; pop A; push A^B'</summary>
        XOR = 0x10,
        ///<summary>'pop B; pop A; push A&lt;&lt;B'</summary>
        LSHIFT = 0x11,
        ///<summary>'pop B; pop A; push A&gt;&gt;B (arithmetic)'</summary>
        RSHIFT = 0x12,
        ///<summary>'pop B; pop A; push A&lt;B'</summary>
        LT = 0x13,
        ///<summary>'pop B; pop A; push A&gt;B'</summary>
        GT = 0x14,
        ///<summary>'pop B; pop A; push A&lt;=B'</summary>
        LTE = 0x15,
        ///<summary>'pop B; pop A; push A&gt;=B'</summary>
        GTE = 0x16,
        ///<summary>'pop B; pop A; push A!=B'</summary>
        NOTE = 0x17,
        ///<summary>'pop B; pop A; push A==B'</summary>
        EQUALE = 0x18,
        ///<summary>'pop VALUE; pop VAR_NO; pop PAGE_NO; PAGE_NO[VAR_NO]=VALUE'</summary>
        ASSIGN = 0x19,
        ///<summary>'pop VALUE; pop VAR_NO; pop PAGE_NO; RESULT = PAGE_NO[VAR_NO]+=VALUE; push RESULT'</summary>
        PLUSA = 0x1a,
        ///<summary>'pop VALUE; pop VAR_NO; pop PAGE_NO; RESULT = PAGE_NO[VAR_NO]-=VALUE; push RESULT'</summary>
        MINUSA = 0x1b,
        ///<summary>'pop VALUE; pop VAR_NO; pop PAGE_NO; RESULT = PAGE_NO[VAR_NO]*=VALUE; push RESULT'</summary>
        MULA = 0x1c,
        ///<summary>'pop VALUE; pop VAR_NO; pop PAGE_NO; RESULT = PAGE_NO[VAR_NO]/=VALUE; push RESULT'</summary>
        DIVA = 0x1d,
        ///<summary>'pop VALUE; pop VAR_NO; pop PAGE_NO; RESULT = PAGE_NO[VAR_NO]%=VALUE; push RESULT'</summary>
        MODA = 0x1e,
        ///<summary>'pop VALUE; pop VAR_NO; pop PAGE_NO; RESULT = PAGE_NO[VAR_NO]&amp;=VALUE; push RESULT'</summary>
        ANDA = 0x1f,
        ///<summary>'pop VALUE; pop VAR_NO; pop PAGE_NO; RESULT = PAGE_NO[VAR_NO]|=VALUE; push RESULT'</summary>
        ORA = 0x20,
        ///<summary>'pop VALUE; pop VAR_NO; pop PAGE_NO; RESULT = PAGE_NO[VAR_NO]^=VALUE; push RESULT'</summary>
        XORA = 0x21,

        LSHIFTA = 0x22,
        RSHIFTA = 0x23,

        F_ASSIGN = 0x24,
        F_PLUSA = 0x25,
        F_MINUSA = 0x26,
        F_MULA = 0x27,
        F_DIVA = 0x28,
        ///<summary>'pop B; pop A; push A; push B; push A; push B'</summary>
        DUP2 = 0x29,
        ///<summary>'pop C; pop B; pop A; push C; push A; push B; push C'</summary>
        DUP_X2 = 0x2a,

        CMP = 0x2b,  //???

        ///<summary>'Jump to the absolute address in the argument'</summary>
        JUMP = 0x2c,
        ///<summary>'Jump to the absolute address in the argument if top of stack is false'</summary>
        IFZ = 0x2d,
        ///<summary>'Jump to the absolute address in the argument if top of stack is true'</summary>
        IFNZ = 0x2e,
        ///<summary>'Return to the calling function'</summary>
        RETURN = 0x2f,
        ///<summary>'Call the function specified'</summary>
        CALLFUNC = 0x30,
        ///<summary>'pop VAR_NO; pop PAGE_NO; PAGE_NO[VAR_NO]++'</summary>
        INC = 0x31,
        ///<summary>'pop VAR_NO; pop PAGE_NO; PAGE_NO[VAR_NO]--'</summary>
        DEC = 0x32,
        ///<summary>'pop FLOAT; INT = ftoi(FLOAT); push INT'</summary>
        FTOI = 0x33,
        ///<summary>'pop INT; FLOAT = itof(INT); push FLOAT'</summary>
        ITOF = 0x34,
        F_INV = 0x35,
        F_ADD = 0x36,
        F_SUB = 0x37,
        F_MUL = 0x38,
        F_DIV = 0x39,
        F_LT = 0x3a,
        ///<summary>'pop B; pop A; push A>B'</summary>
        F_GT = 0x3b,
        ///<summary>'pop B; pop A; push A<=B'</summary>
        F_LTE = 0x3c,
        ///<summary>'pop B; pop A; push A>=B'</summary>
        F_GTE = 0x3D,
        ///<summary>'pop B; pop A; push A!=B'</summary>
        F_NOTE = 0x3E,
        ///<summary>'pop B; pop A; push A==B'</summary>
        F_EQUALE = 0x3f,
        ///<summary>'Push a 32bit (float?) value onto the stack'</summary>
        F_PUSH = 0x40,
        ///<summary>'Push the specified string onto the stack'</summary>
        S_PUSH = 0x41,
        S_POP = 0x42,
        S_ADD = 0x43,
        S_ASSIGN = 0x44,
        S_PLUSA = 0x45,
        S_REF = 0x46,
        S_REFREF = 0x47,
        S_NOTE = 0x48,
        S_EQUALE = 0x49,
        SF_CREATE = 0x4A,
        SF_CREATEPIXEL = 0x4B,
        SF_CREATEALPHA = 0x4C,

        SR_POP = 0x4d,
        SR_ASSIGN = 0x4e,
        SR_REF = 0x4f,
        SR_REFREF = 0x50,
        A_ALLOC = 0x51,
        A_REALLOC = 0x52,
        A_FREE = 0x53,
        A_NUMOF = 0x54,
        A_COPY = 0x55,
        A_FILL = 0x56,
        /// <summary>
        /// Character index for string, pop index, pop string
        /// </summary>
        C_REF = 0x57,
        /// <summary>
        /// Character assign for string, pop characterValue, pop characterIndex, pop stringReference
        /// </summary>
        C_ASSIGN = 0x58,
        MSG = 0x59,
        CALLHLL = 0x5a,
        ///<summary>'Push the structure (this) page index onto the stack'</summary>
        PUSHSTRUCTPAGE = 0x5b,
        ///<summary>'Call the method specified'</summary>
        CALLMETHOD = 0x5c,
        SH_GLOBALREF = 0x5d,
        SH_LOCALREF = 0x5e,
        ///<summary>'Invoke a switch statement on the top of the stack, with the argument as the jump table number'</summary>
        SWITCH = 0x5f,
        STRSWITCH = 0x60,
        ///<summary>'Indicate the start of a function'</summary>
        FUNC = 0x61,
        ///<summary>'Indicate the end of a source file'</summary>
        EOF = 0x62,
        ///<summary>'Initiate system call'</summary>
        CALLSYS = 0x63,
        SJUMP = 0x64,
        CALLONJUMP = 0x65,
        ///<summary>'pop B; pop A; push B; push A'</summary>
        SWAP = 0x66,
        SH_STRUCTREF = 0x67,
        S_LENGTH = 0x68,
        S_LENGTHBYTE = 0x69,
        I_STRING = 0x6a,
        ///<summary>'Call the function indicated by the top of the stack'</summary>
        CALLFUNC2 = 0x6b,
        ///<summary>'pop C; pop B; pop A; push B; push C; push A; push B; push C'</summary>
        DUP2_X1 = 0x6c,
        R_ASSIGN = 0x6d,
        FT_ASSIGNS = 0x6e,
        ASSERT = 0x6f,
        S_LT = 0x70,
        S_GT = 0x71,
        S_LTE = 0x72,
        S_GTE = 0x73,
        S_LENGTH2 = 0x74,
        S_LENGTHBYTE2 = 0x75,
        NEW = 0x76,
        ///<summary>'Delete a page?'</summary>
        DELETE = 0x77,
        CHECKUDO = 0x78,
        A_REF = 0x79,
        ///<summary>'pop A; push A; push A'</summary>
        DUP = 0x7a,
        DUP_U2 = 0x7b,
        SP_INC = 0x7c,
        SP_DEC = 0x7d,
        ///<summary>'Indicate end of a function'</summary>
        ENDFUNC = 0x7e,
        R_EQUALE = 0x7f,
        R_NOTE = 0x80,
        SH_LOCALCREATE = 0x81,
        SH_LOCALDELETE = 0x82,
        STOI = 0x83,
        A_PUSHBACK = 0x84,
        A_POPBACK = 0x85,
        S_EMPTY = 0x86,
        A_EMPTY = 0x87,
        A_ERASE = 0x88,
        A_INSERT = 0x89,
        SH_LOCALINC = 0x8a,
        SH_LOCALDEC = 0x8b,
        SH_LOCALASSIGN = 0x8c,
        ITOB = 0x8d,
        S_FIND = 0x8e,
        S_GETPART = 0x8f,
        A_SORT = 0x90,
        S_PUSHBACK = 0x91,
        S_POPBACK = 0x92,
        FTOS = 0x93,
        S_MOD = 0x94,
        S_PLUSA2 = 0x95,
        OBJSWAP = 0x96,
        S_ERASE = 0x97,
        SR_REF2 = 0x98,
        S_ERASE2 = 0x99,
        S_PUSHBACK2 = 0x9A,
        S_POPBACK2 = 0x9B,
        ITOLI = 0x9c,
        LI_ADD = 0x9d,
        LI_SUB = 0x9e,
        LI_MUL = 0x9f,
        LI_DIV = 0xa0,
        LI_MOD = 0xa1,
        LI_ASSIGN = 0xa2,
        LI_PLUSA = 0xa3,
        LI_MINUSA = 0xa4,
        LI_MULA = 0xa5,
        LI_DIVA = 0xa6,
        LI_MODA = 0xa7,
        LI_ANDA = 0xa8,
        LI_ORA = 0xa9,
        LI_XORA = 0xaa,
        LI_LSHIFTA = 0xab,
        LI_RSHIFTA = 0xac,
        LI_INC = 0xad,
        LI_DEC = 0xae,
        A_FIND = 0xaf,
        A_REVERSE = 0xb0,
        SH_SR_ASSIGN,
        SH_MEM_ASSIGN_LOCAL,
        A_NUMOF_GLOB_1,
        A_NUMOF_STRUCT_1,
        SH_MEM_ASSIGN_IMM,
        SH_LOCALREFREF,
        SH_LOCALASSIGN_SUB_IMM,
        SH_IF_LOC_LT_IMM,
        SH_IF_LOC_GE_IMM,
        SH_LOCREF_ASSIGN_MEM,
        PAGE_REF,
        SH_GLOBAL_ASSIGN_LOCAL,
        SH_STRUCTREF_GT_IMM,
        SH_STRUCT_ASSIGN_LOCALREF_ITOB,
        SH_LOCAL_ASSIGN_STRUCTREF,
        SH_IF_STRUCTREF_NE_LOCALREF,
        SH_IF_STRUCTREF_GT_IMM,
        SH_STRUCTREF_CALLMETHOD_NO_PARAM,
        SH_STRUCTREF2,
        SH_REF_STRUCTREF2,
        SH_STRUCTREF3,
        SH_STRUCTREF2_CALLMETHOD_NO_PARAM,
        SH_IF_STRUCTREF_Z,
        SH_IF_STRUCT_A_NOT_EMPTY,
        SH_IF_LOC_GT_IMM,
        SH_IF_STRUCTREF_NE_IMM,
        THISCALLMETHOD_NOPARAM,
        SH_IF_LOC_NE_IMM,
        SH_IF_STRUCTREF_EQ_IMM,
        SH_GLOBAL_ASSIGN_IMM,
        SH_LOCALSTRUCT_ASSIGN_IMM,
        SH_STRUCT_A_PUSHBACK_LOCAL_STRUCT,
        SH_GLOBAL_A_PUSHBACK_LOCAL_STRUCT,
        SH_LOCAL_A_PUSHBACK_LOCAL_STRUCT,
        SH_IF_SREF_NE_STR0,
        SH_S_ASSIGN_REF,
        SH_A_FIND_SREF,
        SH_SREF_EMPTY,
        SH_STRUCTSREF_EQ_LOCALSREF,
        SH_LOCALSREF_EQ_STR0,
        SH_STRUCTSREF_NE_LOCALSREF,
        SH_LOCALSREF_NE_STR0,
        SH_STRUCT_SR_REF,
        SH_STRUCT_S_REF,
        S_REF2,
        SH_REF_LOCAL_ASSIGN_STRUCTREF2,
        SH_GLOBAL_S_REF,
        SH_LOCAL_S_REF,
        SH_LOCALREF_SASSIGN_LOCALSREF,
        SH_LOCAL_APUSHBACK_LOCALSREF,
        SH_S_ASSIGN_CALLSYS19,
        SH_S_ASSIGN_STR0,
        SH_SASSIGN_LOCALSREF,
        SH_STRUCTREF_SASSIGN_LOCALSREF,
        SH_LOCALSREF_EMPTY,
        SH_GLOBAL_APUSHBACK_LOCALSREF,
        SH_STRUCT_APUSHBACK_LOCALSREF,
        SH_STRUCTSREF_EMPTY,
        SH_GLOBALSREF_EMPTY,
        SH_SASSIGN_STRUCTSREF,
        SH_SASSIGN_GLOBALSREF,
        SH_STRUCTSREF_NE_STR0,
        SH_GLOBALSREF_NE_STR0,
        SH_LOC_LT_IMM_OR_LOC_GE_IMM,
        //instruction names are known, exact order of them are guesses after this point
        A_SORT_MEM,
        DG_ADD,
        DG_SET,
        DG_CALL,
        DG_NUMOF,
        DG_EXIST,
        DG_ERASE,
        DG_CLEAR,
        DG_COPY,
        DG_ASSIGN,
        DG_PLUSA,
        DG_POP,
        DG_NEW_FROM_METHOD,
        DG_MINUSA,
        DG_CALLBEGIN,

        //fake instructions after this point
        Comma = -1000,
        LogicalAnd,
        LogicalOr,
        IfElse,
        /// <summary>
        /// A statement, subexpression 1 is the code, subexpression 2 is the next statement.
        /// </summary>
        Statement,
        AssignmentRef,
        SwitchCase,
        DefaultSwitchCase,
        Null,
        Break,

        StructAssignmentRef,
        ArrayIndex,

        WhileLoop,
        Continue,
        ForLoop,

        LocalReference,
        ThisReference,
        GlobalReference,
        StructMemberReference,
        ArrayIndexReference,
        Nop,

        PreIncrement,
        PreDecrement,

        Goto,
        Label,

        Word1, Word2, Word3, Literal0, Literal1, Literal19,

        PrintStructType,
    }

    public enum OldInstructions
    {
        UNKNOWN_F1 = 0xF1,
        UNKNOWN_F2,
        DELEGATE_ASSIGN_INT,
        AFTER_CALL_DELEGATE,
        DELEGATE_TO_INT,
        UNKNOWN_F6,
        UNKNOWN_F7,
        ERASE_DELEGATE,
        TO_DELEGATE,
        DELEGATE_ASSIGN,
        DELEGATE_ADD,
        DELEGATE_POP,
        BUILD_DELEGATE,
        DELEGATE_REMOVE,
        CALL_DELEGATE,
    }

    public static class InstructionInformation
    {
        //missing information for:
        //SH_A_FIND_SREF
        //SH_REF_LOCAL_ASSIGN_STRUCTREF2

        public static readonly HashSet<Instruction> instructionsWithZeroArguments = new HashSet<Instruction>(
            new Instruction[]
            {
                Instruction.SH_SR_ASSIGN, Instruction.SH_SREF_EMPTY, Instruction.SH_S_ASSIGN_REF,
                Instruction.SH_S_ASSIGN_CALLSYS19,
                Instruction.DG_SET, Instruction.DG_NUMOF, Instruction.DG_CLEAR, Instruction.DG_COPY,
                Instruction.DG_ASSIGN, Instruction.DG_PLUSA, Instruction.DG_POP, Instruction.DG_NEW_FROM_METHOD, Instruction.DG_MINUSA,
            });

        public static readonly HashSet<Instruction> instructionsWithOneArgument = new HashSet<Instruction>(
            new Instruction[]
            {
                Instruction.PUSH, Instruction.JUMP, Instruction.IFZ, Instruction.IFNZ,
                Instruction.CALLFUNC, Instruction.F_PUSH, Instruction.S_PUSH, Instruction.SR_REF,
                Instruction.MSG, Instruction.CALLMETHOD, Instruction.SH_GLOBALREF, Instruction.SH_LOCALREF,
                Instruction.SWITCH, Instruction.FUNC, Instruction.EOF, Instruction.CALLSYS,
                Instruction.SH_STRUCTREF,  Instruction.ENDFUNC, Instruction.SH_LOCALDELETE, Instruction.SH_LOCALINC, Instruction.SH_LOCALDEC, 
                Instruction.SH_LOCALREFREF, Instruction.SH_LOCAL_S_REF, Instruction.SH_S_ASSIGN_STR0, Instruction.SH_LOCALSREF_EMPTY,
                Instruction.PAGE_REF,Instruction.A_NUMOF_STRUCT_1,Instruction.SH_IF_STRUCTREF_NE_LOCALREF,
                Instruction.THISCALLMETHOD_NOPARAM,
                Instruction.SH_SASSIGN_STRUCTSREF,
                Instruction.SH_STRUCT_S_REF,
                Instruction.SH_STRUCTSREF_EMPTY,
                Instruction.SH_SASSIGN_LOCALSREF, Instruction.SH_GLOBAL_S_REF, Instruction.A_NUMOF_GLOB_1,
                Instruction.SH_GLOBALSREF_EMPTY, Instruction.SH_SASSIGN_GLOBALSREF, Instruction.STRSWITCH,
                Instruction.S_REF2,
                Instruction.SR_REF2,
                Instruction.DG_CALLBEGIN,
            });
        public static readonly HashSet<Instruction> instructionsWithTwoArguments = new HashSet<Instruction>(
            new Instruction[]
            {
                Instruction.CALLHLL, Instruction.SH_LOCALCREATE, Instruction.SH_LOCALASSIGN,
                Instruction.SH_MEM_ASSIGN_IMM,Instruction.SH_GLOBAL_ASSIGN_IMM, 
                //Instruction.SH_S_ASSIGN_CALLSYS19,
                Instruction.SH_MEM_ASSIGN_LOCAL,
                Instruction.SH_GLOBAL_ASSIGN_LOCAL, Instruction.SH_STRUCTSREF_EQ_LOCALSREF,
                Instruction.SH_STRUCTREF_SASSIGN_LOCALSREF, Instruction.SH_LOCAL_ASSIGN_STRUCTREF,
                Instruction.SH_STRUCT_SR_REF, Instruction.SH_IF_STRUCT_A_NOT_EMPTY,
                Instruction.SH_LOCALSREF_NE_STR0,
                Instruction.SH_LOCALSREF_EQ_STR0, Instruction.SH_LOCAL_APUSHBACK_LOCALSREF, Instruction.SH_IF_STRUCTREF_Z,
                Instruction.SH_STRUCT_A_PUSHBACK_LOCAL_STRUCT, Instruction.SH_IF_SREF_NE_STR0, Instruction.SH_STRUCTSREF_NE_STR0, Instruction.SH_LOCALASSIGN_SUB_IMM,
                Instruction.SH_LOCREF_ASSIGN_MEM, Instruction.SH_LOCAL_A_PUSHBACK_LOCAL_STRUCT, 
                Instruction.SH_LOCALREF_SASSIGN_LOCALSREF,
                Instruction.SH_GLOBAL_A_PUSHBACK_LOCAL_STRUCT, Instruction.SH_GLOBAL_APUSHBACK_LOCALSREF, Instruction.SH_GLOBALSREF_NE_STR0, Instruction.SH_STRUCTREF_GT_IMM,
                Instruction.SH_STRUCT_ASSIGN_LOCALREF_ITOB, Instruction.SH_STRUCTSREF_NE_LOCALSREF, Instruction.SH_STRUCT_APUSHBACK_LOCALSREF,
                Instruction.SH_STRUCTREF_CALLMETHOD_NO_PARAM, Instruction.SH_STRUCTREF2, Instruction.SH_REF_STRUCTREF2, 
                Instruction.DG_CALL,
            });
        public static readonly HashSet<Instruction> instructionsWithThreeArguments = new HashSet<Instruction>(
            new Instruction[]
            {
                Instruction.SH_IF_STRUCTREF_NE_LOCALREF, Instruction.SH_IF_STRUCTREF_NE_IMM,
                Instruction.SH_IF_LOC_LT_IMM, Instruction.SH_IF_LOC_GE_IMM, Instruction.SH_IF_LOC_NE_IMM, Instruction.SH_IF_LOC_GT_IMM,
                Instruction.SH_IF_STRUCTREF_EQ_IMM, Instruction.SH_IF_STRUCTREF_GT_IMM, Instruction.SH_IF_STRUCTREF_NE_IMM,
                Instruction.SH_LOC_LT_IMM_OR_LOC_GE_IMM,
                Instruction.SH_LOCALSTRUCT_ASSIGN_IMM, Instruction.SH_STRUCTREF2_CALLMETHOD_NO_PARAM, Instruction.SH_STRUCTREF3,
                //
            });

        public static Dictionary<Instruction, Instruction> NegatedInstructions = CreateNegatedInstructions();

        private static Dictionary<Instruction, Instruction> CreateNegatedInstructions()
        {
            Dictionary<Instruction, Instruction> dic = new Dictionary<Instruction, Instruction>();

            dic.Add(Instruction.F_GT, Instruction.F_LTE);
            dic.Add(Instruction.F_GTE, Instruction.F_LT);
            dic.Add(Instruction.F_LT, Instruction.F_GTE);
            dic.Add(Instruction.F_LTE, Instruction.F_GT);
            dic.Add(Instruction.F_EQUALE, Instruction.F_NOTE);
            dic.Add(Instruction.F_NOTE, Instruction.F_EQUALE);

            dic.Add(Instruction.GT, Instruction.LTE);
            dic.Add(Instruction.GTE, Instruction.LT);
            dic.Add(Instruction.LT, Instruction.GTE);
            dic.Add(Instruction.LTE, Instruction.GT);
            dic.Add(Instruction.EQUALE, Instruction.NOTE);
            dic.Add(Instruction.NOTE, Instruction.EQUALE);

            dic.Add(Instruction.S_GT, Instruction.S_LTE);
            dic.Add(Instruction.S_GTE, Instruction.S_LT);
            dic.Add(Instruction.S_LT, Instruction.S_GTE);
            dic.Add(Instruction.S_LTE, Instruction.S_GT);
            dic.Add(Instruction.S_EQUALE, Instruction.S_NOTE);
            dic.Add(Instruction.S_NOTE, Instruction.S_EQUALE);

            dic.Add(Instruction.R_EQUALE, Instruction.R_NOTE);
            dic.Add(Instruction.R_NOTE, Instruction.R_EQUALE);
            return dic;
        }

    }

}
