using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Text;

namespace AinDecompiler
{
    public static class InstructionExtensions
    {
        public static int GetNumberOfWords(this Instruction instruction)
        {
            return Decompiler.GetArgumentCount(instruction);
        }

        public static Instruction Negate(this Instruction instruction)
        {
            if (InstructionInformation.NegatedInstructions.ContainsKey(instruction))
            {
                return InstructionInformation.NegatedInstructions[instruction];
            }
            else
            {
                return instruction;
            }
        }

        public static bool IsDirectAssignment(this Instruction instruction)
        {
            return (instruction == Instruction.ASSIGN ||
                instruction == Instruction.F_ASSIGN ||
                instruction == Instruction.S_ASSIGN ||
                instruction == Instruction.SR_ASSIGN ||
                instruction == Instruction.C_ASSIGN ||
                instruction == Instruction.R_ASSIGN ||
                instruction == Instruction.FT_ASSIGNS ||
                instruction == Instruction.SH_LOCALASSIGN ||
                instruction == Instruction.LI_ASSIGN ||
                instruction == Instruction.DG_ASSIGN);
        }

        public static bool IsAssignment(this Instruction instruction)
        {
            return Expression.IsAssignment(instruction);
        }

        public static bool IsBinary(this Instruction instruction)
        {
            return Expression.IsBinary(instruction);
        }

        public static bool IsBlock(this Instruction instruction)
        {
            return Expression.IsBlock(instruction);
        }

        public static bool IsOperator(this Instruction instruction)
        {
            return ExpressionDisplayer.OperatorTable.ContainsKey(instruction);
        }

        public static bool IsCast(this Instruction instruction)
        {
            return ExpressionDisplayer.castOperations.Contains(instruction);
        }

        public static bool IsFunctionCall(this Instruction instruction)
        {
            return (instruction == Instruction.CALLFUNC ||
                instruction == Instruction.CALLFUNC2 ||
                instruction == Instruction.CALLMETHOD ||
                instruction == Instruction.CALLHLL ||
                instruction == Instruction.CALLSYS ||
                instruction == Instruction.SH_S_ASSIGN_CALLSYS19);
        }

        static ArgumentKind[] blankArray = new ArgumentKind[0];

        public static ArgumentKind[] GetArgumentKinds(this Instruction instruction)
        {
            if (ArgumentKinds.InstructionArgumentKinds.ContainsKey((int)instruction))
            {
                return ArgumentKinds.InstructionArgumentKinds[(int)instruction];
            }
            int numberOfWords = instruction.GetNumberOfWords();
            if (numberOfWords > 0)
            {
                return Enumerable.Repeat(ArgumentKind.Int, numberOfWords).ToArray();
            }
            return blankArray;
        }

        public static int IndexOfStringArgument(this Instruction instruction)
        {
            if (ArgumentKinds.InstructionsThatUseStrings.ContainsKey((int)instruction))
            {
                return ArgumentKinds.InstructionsThatUseStrings[(int)instruction];
            }
            else
            {
                return -1;
            }
        }

        public static bool IsLabel(this Instruction instruction)
        {
            if (instruction == Instruction.Label ||
                instruction == Instruction.SwitchCase ||
                instruction == Instruction.DefaultSwitchCase)
            {
                return true;
            }
            return false;
        }

    }
}
