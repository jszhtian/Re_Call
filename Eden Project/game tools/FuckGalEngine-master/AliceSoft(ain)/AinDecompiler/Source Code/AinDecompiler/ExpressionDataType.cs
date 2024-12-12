using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Text;

namespace AinDecompiler
{
    public partial class Expression
    {
        public DataType GetDataType()
        {
            switch (ExpressionType)
            {
                //void:
                case Instruction.Statement:
                case Instruction.SWITCH:
                case Instruction.SwitchCase:
                case Instruction.A_REALLOC:
                case Instruction.A_PUSHBACK:
                case Instruction.A_POPBACK:
                case Instruction.A_ALLOC:
                case Instruction.A_FREE:
                case Instruction.A_INSERT:
                case Instruction.A_REVERSE:
                case Instruction.A_SORT:
                case Instruction.ASSERT:
                case Instruction.Break:
                case Instruction.CALLONJUMP:
                case Instruction.Continue:
                case Instruction.DELETE:
                case Instruction.ENDFUNC:
                case Instruction.EOF:
                case Instruction.ForLoop:
                case Instruction.FUNC:
                case Instruction.IfElse:
                case Instruction.IFNZ:
                case Instruction.IFZ:
                case Instruction.JUMP:
                case Instruction.MSG:
                case Instruction.Nop:
                case Instruction.Null:
                case Instruction.OBJSWAP:
                case Instruction.POP:
                case Instruction.S_ERASE:
                case Instruction.S_ERASE2:
                case Instruction.S_POP:
                case Instruction.S_POPBACK:
                case Instruction.S_POPBACK2:
                case Instruction.S_PUSHBACK:
                case Instruction.S_PUSHBACK2:
                case Instruction.SJUMP:
                case Instruction.SP_DEC:
                case Instruction.SP_INC:
                case Instruction.SR_POP:
                case Instruction.SH_LOCALASSIGN:
                case Instruction.SH_LOCALINC:
                case Instruction.SH_LOCALDEC:
                case Instruction.SWAP:
                case Instruction.WhileLoop:
                    return DataType.Void;
                //int
                case Instruction.A_FIND:
                case Instruction.A_NUMOF:
                case Instruction.A_NUMOF_GLOB_1:
                case Instruction.A_NUMOF_STRUCT_1:
                case Instruction.ADD:
                case Instruction.AND:
                case Instruction.ANDA:
                case Instruction.COMPL:
                case Instruction.DEC:
                case Instruction.DIV:
                case Instruction.DIVA:
                case Instruction.FTOI:
                case Instruction.INC:
                case Instruction.INV:
                case Instruction.LSHIFT:
                case Instruction.LSHIFTA:
                case Instruction.MINUSA:
                case Instruction.MOD:
                case Instruction.MODA:
                case Instruction.MUL:
                case Instruction.MULA:
                case Instruction.OR:
                case Instruction.ORA:
                case Instruction.PLUSA:
                case Instruction.PUSH:
                case Instruction.RSHIFT:
                case Instruction.RSHIFTA:
                case Instruction.S_FIND:
                case Instruction.S_LENGTH:
                case Instruction.S_LENGTH2:
                case Instruction.S_LENGTHBYTE:
                case Instruction.S_LENGTHBYTE2:
                case Instruction.STOI:
                case Instruction.SUB:
                case Instruction.XOR:
                case Instruction.XORA:
                case Instruction.PreDecrement:
                case Instruction.PreIncrement:
                    return DataType.Int;
                //string
                case Instruction.FTOS:
                case Instruction.I_STRING:
                case Instruction.S_ADD:
                case Instruction.S_ASSIGN:
                case Instruction.S_GETPART:
                case Instruction.S_MOD:
                case Instruction.S_PLUSA:
                case Instruction.S_PLUSA2:
                case Instruction.S_PUSH:
                    return DataType.String;
                //bool
                case Instruction.A_EMPTY:
                case Instruction.EQUALE:
                case Instruction.F_EQUALE:
                case Instruction.F_GT:
                case Instruction.F_GTE:
                case Instruction.F_LTE:
                case Instruction.F_NOTE:
                case Instruction.GT:
                case Instruction.GTE:
                case Instruction.ITOB:
                case Instruction.LT:
                case Instruction.LTE:
                case Instruction.NOT:
                case Instruction.NOTE:
                case Instruction.R_EQUALE:
                case Instruction.R_NOTE:
                case Instruction.S_EMPTY:
                case Instruction.S_EQUALE:
                case Instruction.S_GT:
                case Instruction.S_GTE:
                case Instruction.S_LT:
                case Instruction.S_LTE:
                case Instruction.S_NOTE:
                case Instruction.LogicalAnd:
                case Instruction.LogicalOr:
                    return DataType.Bool;
                case Instruction.DG_CALLBEGIN:
                    {
                        int delegateNumber = this.Value;
                        if (delegateNumber >= 0 && delegateNumber < ainFile.Delegates.Count)
                        {
                            var dele = ainFile.Delegates[delegateNumber];
                            return dele.DataType;
                        }
                        else
                        {
                            return DataType.Void;
                        }
                    }
                    break;
                case Instruction.CALLFUNC:
                case Instruction.CALLMETHOD:
                    {
                        int functionNumber = this.Value;
                        var function = ainFile.Functions[functionNumber];
                        return function.DataType;
                    }
                    break;
                case Instruction.CALLHLL:
                    {
                        int libraryNumber = this.Value;
                        int functionNumber = this.Value2;
                        var library = ainFile.Libraries[libraryNumber];
                        var function = library.Functions[functionNumber];
                        return function.DataType;
                    }
                    break;
                case Instruction.CALLSYS:
                    {
                        int systemCallNumber = this.Value;
                        if (systemCallNumber >= 0 && systemCallNumber < AinFile.SystemCalls.Count)
                        {
                            var function = AinFile.SystemCalls[systemCallNumber];
                            return function.DataType;
                        }
                        return DataType.Void;
                    }
                    break;
                case Instruction.PUSHLOCALPAGE:
                    return DataType.LocalPage;
                case Instruction.PUSHGLOBALPAGE:
                    return DataType.GlobalPage;
                case Instruction.PUSHSTRUCTPAGE:
                    return DataType.Struct;
                case Instruction.C_ASSIGN:
                case Instruction.C_REF:
                    return DataType.Int;
                case Instruction.ArrayIndex:
                case Instruction.ArrayIndexReference:
                    {
                        var dataType = Arg1.GetDataType();
                        return dataType.GetTypeOfArrayElement();
                    }
                    break;
                case Instruction.LI_ADD:
                case Instruction.LI_ANDA:
                case Instruction.LI_ASSIGN:
                case Instruction.LI_DEC:
                case Instruction.LI_DIV:
                case Instruction.LI_DIVA:
                case Instruction.LI_INC:
                case Instruction.LI_LSHIFTA:
                case Instruction.LI_MINUSA:
                case Instruction.LI_MOD:
                case Instruction.LI_MODA:
                case Instruction.LI_MUL:
                case Instruction.LI_MULA:
                case Instruction.LI_ORA:
                case Instruction.LI_PLUSA:
                case Instruction.LI_RSHIFTA:
                case Instruction.LI_SUB:
                case Instruction.LI_XORA:
                case Instruction.ITOLI:
                    {
                        return DataType.Lint;
                    }
                    break;
                case Instruction.ITOF:
                case Instruction.F_PUSH:
                case Instruction.F_ADD:
                case Instruction.F_ASSIGN:
                case Instruction.F_DIV:
                case Instruction.F_DIVA:
                case Instruction.F_INV:
                case Instruction.F_MINUSA:
                case Instruction.F_MUL:
                case Instruction.F_MULA:
                case Instruction.F_PLUSA:
                case Instruction.F_SUB:
                    {
                        return DataType.Float;
                    }
                    break;
                case Instruction.DG_COPY:
                case Instruction.DG_NEW_FROM_METHOD:
                    {
                        return DataType.Delegate;
                    }
                    break;
                case Instruction.DG_NUMOF:
                    {
                        return DataType.Int;
                    }
                    break;



                case Instruction.A_COPY:
                case Instruction.A_FILL:
                case Instruction.A_ERASE:
                    {
                        return DataType.Int;
                    }
                case Instruction.SR_ASSIGN:
                    {
                        return DataType.Void;
                    }
                //case Instruction.A_REALLOC:
                case Instruction.FT_ASSIGNS:
                    {
                        return DataType.String;
                    }

            }
            return (DataType)0;
        }
    }
}
