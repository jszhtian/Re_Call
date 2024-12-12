using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Text;

namespace AinDecompiler
{
    public class ExplorerHistoryItem
    {
        public string FunctionName;
        public string FileName;
        public string ClassName;
        public int StartAddress = -1;
        public int EndAddress = -1;
        public ScintillaState ScintillaState;
        public ScintillaState ScintillaState2;
        public ScintillaState ScintillaState3;

        public override string ToString()
        {
            return FunctionName ?? FileName ?? ClassName ?? "";
        }
    }

    public class CodeDisplayState
    {
        public AinFile ainFile;
        public Function Function = null;
        public string FunctionName = null;
        public int FunctionAddress = -1;
        public int EndAddress = -1;
        public string DecompiledCodeText;
        public string DisassembledCodeText;
        public string CombinedViewCodeText;
        public Expression FunctionExpression;
        public ExpressionMap ExpressionMap;
        public CodeDisplayOptions CodeDisplayOptions;
        public bool UseTry;
        public bool Revisiting;
        public string FileName;
        public string ClassName;
    }

    public class CodeDisplayOptions
    {
        //options for display
        public bool DeclareVariablesAtBeginning = false;
        public bool RemoveReturns = true;
        public bool ShowCasting = false;
        public bool ShowGlobalPrefix = true;
        public bool ShowThis = true;
        public bool Verbose = false;

        //options for exporting
        public bool ForceOriginalStringOrder = false;
        public bool MergeDuplicateStrings = false;
        public bool RemoveOrphanFunctions = false;
        public bool AnnotateWithDecompiledCode = false;
    }
}
