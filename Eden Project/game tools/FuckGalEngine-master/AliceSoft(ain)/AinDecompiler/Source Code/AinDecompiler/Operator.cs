using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Text;

namespace AinDecompiler
{
    public class OperatorTable : Dictionary<Instruction, Operator>
    {
        public OperatorTable()
        {
            Add(Instruction.ADD, "+", OperatorPosition.Infix, 12);
            Add(Instruction.SUB, "-", OperatorPosition.Infix, 12);
            Add(Instruction.MUL, "*", OperatorPosition.Infix, 13);
            Add(Instruction.DIV, "/", OperatorPosition.Infix, 13);
            Add(Instruction.MOD, "%", OperatorPosition.Infix, 13);
            Add(Instruction.AND, "&", OperatorPosition.Infix, 8);
            Add(Instruction.XOR, "^", OperatorPosition.Infix, 7);
            Add(Instruction.OR, "|", OperatorPosition.Infix, 6);
            Add(Instruction.LSHIFT, "<<", OperatorPosition.Infix, 11);
            Add(Instruction.RSHIFT, ">>", OperatorPosition.Infix, 11);
            Add(Instruction.LT, "<", OperatorPosition.Infix, 10);
            Add(Instruction.LTE, "<=", OperatorPosition.Infix, 10);
            Add(Instruction.GT, ">", OperatorPosition.Infix, 10);
            Add(Instruction.GTE, ">=", OperatorPosition.Infix, 10);
            Add(Instruction.NOTE, "!=", OperatorPosition.Infix, 9);
            Add(Instruction.EQUALE, "==", OperatorPosition.Infix, 9);

            Add(Instruction.LI_ADD, "+", OperatorPosition.Infix, 12);
            Add(Instruction.LI_SUB, "-", OperatorPosition.Infix, 12);
            Add(Instruction.LI_MUL, "*", OperatorPosition.Infix, 13);
            Add(Instruction.LI_DIV, "/", OperatorPosition.Infix, 13);
            Add(Instruction.LI_MOD, "%", OperatorPosition.Infix, 13);

            Add(Instruction.S_ADD, "+", OperatorPosition.Infix, 12);
            Add(Instruction.S_MOD, "%", OperatorPosition.Infix, 13);
            Add(Instruction.S_LT, "<", OperatorPosition.Infix, 10);
            Add(Instruction.S_LTE, "<=", OperatorPosition.Infix, 10);
            Add(Instruction.S_GT, ">", OperatorPosition.Infix, 10);
            Add(Instruction.S_GTE, ">=", OperatorPosition.Infix, 10);
            Add(Instruction.S_NOTE, "!=", OperatorPosition.Infix, 9);
            Add(Instruction.S_EQUALE, "==", OperatorPosition.Infix, 9);

            Add(Instruction.F_ADD, "+", OperatorPosition.Infix, 12);
            Add(Instruction.F_SUB, "-", OperatorPosition.Infix, 12);
            Add(Instruction.F_MUL, "*", OperatorPosition.Infix, 13);
            Add(Instruction.F_DIV, "/", OperatorPosition.Infix, 13);
            Add(Instruction.F_LT, "<", OperatorPosition.Infix, 10);
            Add(Instruction.F_LTE, "<=", OperatorPosition.Infix, 10);
            Add(Instruction.F_GT, ">", OperatorPosition.Infix, 10);
            Add(Instruction.F_GTE, ">=", OperatorPosition.Infix, 10);
            Add(Instruction.F_NOTE, "!=", OperatorPosition.Infix, 9);
            Add(Instruction.F_EQUALE, "==", OperatorPosition.Infix, 9);

            Add(Instruction.R_EQUALE, "==", OperatorPosition.Infix, 9);

            Add(Instruction.LogicalAnd, "&&", OperatorPosition.Infix, 5);
            Add(Instruction.LogicalOr, "||", OperatorPosition.Infix, 4);
            Add(Instruction.Comma, ",", OperatorPosition.Infix, 0).spaceBefore = false;

            Add(Instruction.IfElse, "?", OperatorPosition.Infix, 3); //just here for precedence

            Add(Instruction.ASSIGN, "=", OperatorPosition.Infix, 2);
            Add(Instruction.PLUSA, "+=", OperatorPosition.Infix, 2);
            Add(Instruction.MINUSA, "-=", OperatorPosition.Infix, 2);
            Add(Instruction.MULA, "*=", OperatorPosition.Infix, 2);
            Add(Instruction.DIVA, "/=", OperatorPosition.Infix, 2);
            Add(Instruction.MODA, "%=", OperatorPosition.Infix, 2);
            Add(Instruction.ANDA, "&=", OperatorPosition.Infix, 2);
            Add(Instruction.ORA, "|=", OperatorPosition.Infix, 2);
            Add(Instruction.XORA, "^=", OperatorPosition.Infix, 2);
            Add(Instruction.LSHIFTA, "<<=", OperatorPosition.Infix, 2);
            Add(Instruction.RSHIFTA, ">>=", OperatorPosition.Infix, 2);

            Add(Instruction.LI_ASSIGN, "=", OperatorPosition.Infix, 2);
            Add(Instruction.LI_PLUSA, "+=", OperatorPosition.Infix, 2);
            Add(Instruction.LI_MINUSA, "-=", OperatorPosition.Infix, 2);
            Add(Instruction.LI_MULA, "*=", OperatorPosition.Infix, 2);
            Add(Instruction.LI_DIVA, "/=", OperatorPosition.Infix, 2);
            Add(Instruction.LI_MODA, "%=", OperatorPosition.Infix, 2);
            Add(Instruction.LI_ANDA, "&=", OperatorPosition.Infix, 2);
            Add(Instruction.LI_ORA, "|=", OperatorPosition.Infix, 2);
            Add(Instruction.LI_XORA, "^=", OperatorPosition.Infix, 2);
            Add(Instruction.LI_LSHIFTA, "<<=", OperatorPosition.Infix, 2);
            Add(Instruction.LI_RSHIFTA, ">>=", OperatorPosition.Infix, 2);

            Add(Instruction.S_ASSIGN, "=", OperatorPosition.Infix, 2);
            Add(Instruction.S_PLUSA, "+=", OperatorPosition.Infix, 2);
            Add(Instruction.S_PLUSA2, "+=", OperatorPosition.Infix, 2);

            Add(Instruction.SR_ASSIGN, "=", OperatorPosition.Infix, 2);

            Add(Instruction.F_ASSIGN, "=", OperatorPosition.Infix, 2);
            Add(Instruction.F_PLUSA, "+=", OperatorPosition.Infix, 2);
            Add(Instruction.F_MINUSA, "-=", OperatorPosition.Infix, 2);
            Add(Instruction.F_MULA, "*=", OperatorPosition.Infix, 2);
            Add(Instruction.F_DIVA, "/=", OperatorPosition.Infix, 2);

            Add(Instruction.INV, "-", OperatorPosition.UnaryPrefix, 15);
            Add(Instruction.F_INV, "-", OperatorPosition.UnaryPrefix, 15);
            Add(Instruction.NOT, "!", OperatorPosition.UnaryPrefix, 15);
            Add(Instruction.COMPL, "~", OperatorPosition.UnaryPrefix, 15);
            Add(Instruction.INC, "++", OperatorPosition.UnaryPostfix, 16);
            Add(Instruction.DEC, "--", OperatorPosition.UnaryPostfix, 16);
            Add(Instruction.PreIncrement, "++", OperatorPosition.UnaryPrefix, 16);
            Add(Instruction.PreDecrement, "--", OperatorPosition.UnaryPrefix, 16);
            Add(Instruction.LI_INC, "++", OperatorPosition.UnaryPostfix, 16);
            Add(Instruction.LI_DEC, "--", OperatorPosition.UnaryPostfix, 16);




            //                 Instruction.ADD, Instruction.SUB, Instruction.MUL, Instruction.DIV,
            //                 Instruction.MOD, Instruction.AND, Instruction.OR, Instruction.XOR,
            //                 Instruction.LSHIFT, Instruction.RSHIFT, Instruction.LT, Instruction.GT,
            //                 Instruction.LTE, Instruction.GTE, Instruction.NOTE, Instruction.EQUALE,

            //                 Instruction.F_ADD,Instruction.F_SUB,Instruction.F_MUL,
            //                 Instruction.F_DIV,Instruction.F_LT,Instruction.F_GT,Instruction.F_GTE,
            //                 Instruction.F_EQUALE,


        }
        Operator Add(Instruction instruction, string symbol, OperatorPosition position, int precidence)
        {
            var newOperator = new Operator(symbol, position, precidence, instruction);
            if (position != OperatorPosition.Infix)
            {
                newOperator.spaceAfter = false;
                newOperator.spaceBefore = false;
            }
            this.Add(instruction, newOperator);
            return newOperator;
        }
    }
    public enum OperatorPosition
    {
        UnaryPrefix,
        UnaryPostfix,
        Infix,
    }

    public class Operator
    {
        public string symbol;
        public OperatorPosition position;
        public int precedence;
        public Instruction instruction;
        public bool spaceBefore = true;
        public bool spaceAfter = true;

        public Operator(string symbol, OperatorPosition position, int precedence, Instruction instruction)
        {
            this.symbol = symbol;
            this.position = position;
            this.precedence = precedence;
            this.instruction = instruction;
        }

    }
}
