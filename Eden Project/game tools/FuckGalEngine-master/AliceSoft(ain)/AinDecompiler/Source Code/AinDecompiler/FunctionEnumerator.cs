using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Text;

namespace AinDecompiler
{
    public class FunctionEnumerationResult
    {
        public string name;
        public List<FunctionEnumerationResult> children;
        public int address;
        public int lastAddress;
        public int id;
        public override string ToString()
        {
            return name;
        }
    }

    class FunctionEnumerator
    {
        public FunctionEnumerator(AinFile ainFile)
        {
            this.ainFile = ainFile;
        }
        AinFile ainFile;

        public FunctionEnumerationResult[] GetFilesAndFunctions_old()
        {
            int address = 0;
            int codeLength = ainFile.Code.Length;

            List<FunctionEnumerationResult> files = new List<FunctionEnumerationResult>();
            FunctionEnumerationResult currentFunction = null;
            FunctionEnumerationResult currentFile = new FunctionEnumerationResult();
            currentFile.children = new List<FunctionEnumerationResult>();

            currentFile.address = 0;

            while (address < codeLength)
            {
                var info = Decompiler.Peek(ainFile.Code, address);
                if (info.instruction == Instruction.EOF)
                {
                    if (info.word1 >= 0 && info.word1 < ainFile.Filenames.Count)
                    {
                        currentFile.name = ainFile.Filenames[info.word1];
                        currentFile.id = info.word1;
                        if (currentFunction != null)
                        {
                            currentFunction.lastAddress = address;
                            currentFile.children.Add(currentFunction);
                            currentFunction = null;
                        }
                        currentFile.lastAddress = info.nextAddress;
                        files.Add(currentFile);
                        currentFile = new FunctionEnumerationResult();
                        currentFile.children = new List<FunctionEnumerationResult>();
                        currentFile.address = info.nextAddress;
                    }
                }
                if (info.instruction == Instruction.FUNC)
                {
                    if (currentFunction != null)
                    {
                        currentFunction.lastAddress = address;
                        currentFile.children.Add(currentFunction);
                    }
                    if (info.word1 >= 0 && info.word1 < ainFile.Functions.Count)
                    {
                        currentFunction = new FunctionEnumerationResult();
                        currentFunction.id = info.word1;
                        currentFunction.name = ainFile.Functions[info.word1].Name;
                        currentFunction.address = address;
                    }
                }
                if (info.instruction == Instruction.ENDFUNC)
                {
                    if (currentFunction != null)
                    {
                        currentFunction.lastAddress = info.nextAddress;
                        currentFile.children.Add(currentFunction);
                        currentFunction = null;
                    }
                }
                address = info.nextAddress;
            }
            if (currentFile.children.Count > 0)
            {
                if (String.IsNullOrEmpty(currentFile.name))
                {
                    currentFile.name = "remaining.jaf";
                }
                files.Add(currentFile);
            }
            return files.ToArray();
        }

        public FunctionEnumerationResult[] GetFilesAndFunctions()
        {
            int address = 0;
            int codeLength = ainFile.Code.Length;

            List<FunctionEnumerationResult> files = new List<FunctionEnumerationResult>();
            FunctionEnumerationResult currentFunction = null;
            FunctionEnumerationResult currentFile = new FunctionEnumerationResult();
            currentFile.children = new List<FunctionEnumerationResult>();

            currentFile.address = 0;

            bool wantNewFile = false;
            InstructionInfo newFileInstruction = null;

            bool haveFunction = false;

            while (address < codeLength)
            {
                var info = Decompiler.Peek(ainFile.Code, address);
                if (info.instruction == Instruction.EOF)
                {
                    if (wantNewFile)
                    {
                        FinishFile(address, files, ref currentFunction, ref currentFile, newFileInstruction);
                        wantNewFile = false;
                        newFileInstruction = null;
                    }
                    wantNewFile = true;
                    newFileInstruction = info;
                }
                if (info.instruction == Instruction.FUNC)
                {
                    if (wantNewFile)
                    {
                        FinishFile(address, files, ref currentFunction, ref currentFile, newFileInstruction);
                        wantNewFile = false;
                        newFileInstruction = null;
                    }
                    FinishFunction(address, ref currentFunction, currentFile);

                    if (info.word1 >= 0 && info.word1 < ainFile.Functions.Count)
                    {
                        currentFunction = new FunctionEnumerationResult();
                        currentFunction.id = info.word1;
                        currentFunction.name = ainFile.Functions[info.word1].Name;
                        currentFunction.address = address;
                    }
                }
                //if (info.instruction == Instruction.ENDFUNC)
                //{
                //    FinishFunction(address, ref currentFunction, currentFile);
                //}
                address = info.nextAddress;
            }
            if (currentFile.children.Count > 0)
            {
                if (String.IsNullOrEmpty(currentFile.name))
                {
                    currentFile.name = "remaining.jaf";
                }
                FinishFile(address, files, ref currentFunction, ref currentFile, newFileInstruction);
                //files.Add(currentFile);
            }

            return files.ToArray();
        }

        private void FinishFile(int address, List<FunctionEnumerationResult> files, ref FunctionEnumerationResult currentFunction, ref FunctionEnumerationResult currentFile, InstructionInfo newFileInstruction)
        {
            if (newFileInstruction == null || (newFileInstruction.word1 >= 0 && newFileInstruction.word1 < ainFile.Filenames.Count))
            {
                if (newFileInstruction != null)
                {
                    currentFile.name = ainFile.Filenames[newFileInstruction.word1];
                    currentFile.id = newFileInstruction.word1;
                }
                FinishFunction(address, ref currentFunction, currentFile);
                currentFile.lastAddress = address;
                files.Add(currentFile);
                currentFile = new FunctionEnumerationResult();
                currentFile.children = new List<FunctionEnumerationResult>();
                currentFile.address = address;
            }
        }

        private static void FinishFunction(int address, ref FunctionEnumerationResult currentFunction, FunctionEnumerationResult currentFile)
        {
            if (currentFunction != null)
            {
                currentFunction.lastAddress = address;
                currentFile.children.Add(currentFunction);
                currentFunction = null;
            }
        }

        public FunctionEnumerationResult[] GetClasses()
        {
            var classesObject = new FunctionEnumerationResult() { address = -1, name = "classes", id = -1, lastAddress = -1};
            classesObject.children = new List<FunctionEnumerationResult>(ainFile.Structs.Count);
            foreach (var structInfo in ainFile.Structs)
            {
                var newObject = new FunctionEnumerationResult() { address = -1, name = structInfo.Name, id = structInfo.Index, lastAddress = -1 };
                classesObject.children.Add(newObject);
            }
            return new[] { classesObject };
        }
    }
}
