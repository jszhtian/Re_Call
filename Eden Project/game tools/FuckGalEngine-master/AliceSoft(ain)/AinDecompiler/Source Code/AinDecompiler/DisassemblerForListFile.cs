using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Text;
using System.Globalization;

namespace AinDecompiler
{
    public class DisassemblerForListFile
    {
        private MyIndentedTextWriter tw;
        public AinFile ainFile;
        private ExpressionDisplayer displayer = new ExpressionDisplayer();

        static void PadToColumn(StringBuilder sb, int column)
        {
            if (sb.Length < column)
            {
                sb.Append("".PadLeft(column - sb.Length));
            }
        }

        public DisassemblerForListFile(AinFile ainFile)
        {
            this.ainFile = ainFile;
            displayer.ainFile = ainFile;
        }

        public string DisassembleListFile()
        {
            return DisassembleListFileRange(0, ainFile.Code.Length);
        }

        /// <summary>
        /// Returns the disassembly of code starting at startAddress, until and excluding endAddress.
        /// </summary>
        /// <param name="startAddress">The address to start at</param>
        /// <param name="endAddress">The address to end at (output will exclude code at this address)</param>
        /// <returns>The disassembly of code between startAddress and endAddress (excluding endAddress)</returns>
        public string DisassembleListFileRange(int startAddress, int endAddress)
        {
            var sw = new StringWriter(CultureInfo.InvariantCulture);
            tw = new MyIndentedTextWriter(sw);
            int address = startAddress;
            while (address < endAddress)
            {
                address = DisassembleOneInstructionForListFile(address);
            }
            return sw.GetStringBuilder().ToString();
        }

        public int DisassembleOneInstructionForListFile(int address)
        {
            int instructionAddress = address;

            var instructionInfo = Decompiler.Peek(ainFile.Code, address);
            Instruction instruction = instructionInfo.instruction;
            int word1 = instructionInfo.word1;
            int word2 = instructionInfo.word2;
            int word3 = instructionInfo.word3;
            int totalArguments = instructionInfo.totalArguments;
            address = instructionInfo.nextAddress;

            StringBuilder sb = new StringBuilder();
            sb.Append(instructionAddress.ToString("X8") + ":");
            if (instruction > Instruction.A_REVERSE && instruction.GetNumberOfWords() == 0 && !InstructionInformation.instructionsWithZeroArguments.Contains(instruction))
            {
                sb.Append("*");
            }

            PadToColumn(sb, 11);
            sb.Append(((int)instruction).ToString("X4"));
            if (totalArguments >= 1)
            {
                sb.Append(word1.ToString("X8"));
            }
            if (totalArguments >= 2)
            {
                sb.Append(word2.ToString("X8"));
            }
            if (totalArguments >= 3)
            {
                sb.Append(word3.ToString("X8"));
            }
            sb.Append(" ");
            PadToColumn(sb, 33);
            sb.Append(instruction.ToString());
            if (totalArguments >= 1)
            {
                sb.Append(" 0x");
                sb.Append(word1.ToString("X"));
            }
            if (totalArguments >= 2)
            {
                sb.Append(" 0x");
                sb.Append(word2.ToString("X"));
            }
            if (totalArguments >= 3)
            {
                sb.Append(" 0x");
                sb.Append(word3.ToString("X"));
            }

            InstructionInfo nextIntruction = new InstructionInfo();
            if (address < ainFile.Code.Length)
            {
                nextIntruction = Decompiler.Peek(ainFile.Code, address);
            }
            int nextValue = -1;
            if (nextIntruction.instruction == Instruction.PUSH)
            {
                nextValue = nextIntruction.word1;
            }

            switch (instruction)
            {
                case Instruction.S_PUSH:
                    {
                        PadToColumn(sb, 60);
                        sb.Append("; \"" + displayer.GetString(word1) + "\"");
                    }
                    break;

                case Instruction.PUSHGLOBALPAGE:
                    if (nextValue != -1)
                    {
                        PadToColumn(sb, 60);
                        sb.Append("; " + displayer.GetGlobalVariableDeclaration(nextValue));
                    }
                    break;
                case Instruction.PUSHLOCALPAGE:
                    if (nextValue != -1)
                    {
                        PadToColumn(sb, 60);
                        sb.Append("; " + displayer.GetLocalVariableDeclaration(nextValue));
                    }
                    break;
                case Instruction.PUSHSTRUCTPAGE:
                    if (nextValue != -1)
                    {
                        PadToColumn(sb, 60);
                        sb.Append("; " + displayer.GetLocalStructMemberDeclaration(nextValue));
                    }
                    break;
                case Instruction.SH_GLOBALREF:
                    PadToColumn(sb, 60);
                    sb.Append("; " + displayer.GetGlobalVariableDeclaration(word1));
                    break;
                case Instruction.SH_LOCALASSIGN:
                case Instruction.SH_LOCALDEC:
                case Instruction.SH_LOCALREF:
                    PadToColumn(sb, 60);
                    sb.Append("; " + displayer.GetLocalVariableDeclaration(word1));
                    break;
                case Instruction.SH_STRUCTREF:
                    PadToColumn(sb, 60);
                    sb.Append("; " + displayer.GetLocalStructMemberDeclaration(word1));
                    break;
                case Instruction.EOF:
                    PadToColumn(sb, 60);
                    sb.Append("; End of File \"" + displayer.GetFileName(word1) + "\"");
                    break;
                case Instruction.FUNC:
                    PadToColumn(sb, 60);
                    sb.Append("; " + displayer.GetFunctionDeclaration(word1));
                    displayer.currentFuncNumber = word1;
                    break;
                case Instruction.CALLFUNC:
                    PadToColumn(sb, 60);
                    sb.Append("; " + displayer.GetFunctionDeclaration(word1));
                    break;
                case Instruction.CALLSYS:
                    PadToColumn(sb, 60);
                    sb.Append("; " + displayer.GetSystemCallDeclaration(word1));
                    break;
                case Instruction.CALLHLL:
                    PadToColumn(sb, 60);
                    sb.Append("; " + displayer.GetHllDeclaration(word1, word2));
                    break;
                case Instruction.CALLMETHOD:
                    PadToColumn(sb, 60);
                    sb.Append("; " + displayer.GetFunctionDeclaration(word1));
                    break;
                case Instruction.ENDFUNC:
                    PadToColumn(sb, 60);
                    sb.Append("; End of Function " + displayer.GetFunctionDeclaration(word1));
                    break;
                case Instruction.MSG:
                    PadToColumn(sb, 60);
                    sb.Append("; '" + ainFile.Messages[word1] + "'");
                    break;
            }

            tw.WriteLine(sb.ToString());
            return address;
        }

    }
}
