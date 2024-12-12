using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Text;

namespace AinDecompiler
{
    class LabelMover
    {
        AinFile ainFile;
        //Decompiler decompiler;
        TranslateParserThingy.MySortedList<int, Entry> list = new TranslateParserThingy.MySortedList<int, Entry>();
        public LabelMover(AinFile ainFile)
        {
            this.ainFile = ainFile;
            //this.decompiler = new Decompiler(ainFile);
            ScanCode();
        }

        struct SwitchEntry
        {
            public int SwitchBlockNumber;
            public int SwitchCaseNumber;
            public SwitchEntry(int switchBlockNumber, int switchCaseNumber)
            {
                this.SwitchBlockNumber = switchBlockNumber;
                this.SwitchCaseNumber = switchCaseNumber;
            }
        }

        void AddToAddresses(int startAddress, int displacement)
        {


        }

        void ScanCode()
        {
            int lastAddress = ainFile.Code.Length;
            int address = 0;

            while (address < lastAddress)
            {
                var instructionInfo = Decompiler.Peek(ainFile.Code, address);
                if (ArgumentKinds.BranchInstructions.ContainsKey((int)instructionInfo.instruction))
                {
                    int wordNumber = ArgumentKinds.BranchInstructions[(int)instructionInfo.instruction];
                    int targetAddress = instructionInfo.words[wordNumber];
                    int codeAddress = address + 2 + wordNumber * 4;
                    AddInstruction(targetAddress, codeAddress);
                }
                address = instructionInfo.nextAddress;
            }

            foreach (var switchBlock in ainFile.Switches)
            {
                int i = switchBlock.Index;
                int defaultAddress = switchBlock.DefaultCaseAddress;
                if (defaultAddress != -1)
                {
                    AddSwitchAddress(defaultAddress, i, -1);
                }
                foreach (var switchCase in switchBlock.SwitchCases)
                {
                    int i2 = switchCase.Index;
                    int caseAddress = switchCase.TargetAddress;
                    AddSwitchAddress(caseAddress, i, i2);
                }
            }

            foreach (var function in ainFile.Functions)
            {
                int functionAddress = function.Address;
                int functionIndex = function.Index;
                if (functionAddress >= 0 && functionAddress < ainFile.Code.Length)
                {
                    AddFunctionNumber(functionAddress, functionIndex);
                }
            }
        }

        void AddInstruction(int labelAddress, int codeAddress)
        {
            NewEntry(labelAddress).InstructionAddresses.Add(codeAddress);
        }

        void AddSwitchAddress(int labelAddress, int switchBlockNumber, int switchCaseNumber)
        {
            NewEntry(labelAddress).SwitchAddresses.Add(new SwitchEntry(switchBlockNumber, switchCaseNumber));
        }

        void AddFunctionNumber(int labelAddress, int functionNumber)
        {
            NewEntry(labelAddress).FunctionNumbers.Add(functionNumber);
        }

        Entry NewEntry(int labelAddress)
        {
            if (list.ContainsKey(labelAddress))
            {
                return list[labelAddress];
            }
            else
            {
                var entry = new Entry();
                list.Add(labelAddress, entry);
                return entry;
            }
        }




        class Entry
        {
            internal List<int> InstructionAddresses = new List<int>();
            internal List<SwitchEntry> SwitchAddresses = new List<SwitchEntry>();
            internal List<int> FunctionNumbers = new List<int>();

            public Entry()
            {

            }
        }



        //list that relates address to instructions that use it
        //list that relates address to switch cases that use it
        //list that relates address to switch blocks (default case) that use it
        //list that relates address to functions that use it


    }


}
