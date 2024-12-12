using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Text;

namespace AinDecompiler.Compiler
{
    public class CodeGenerator
    {
        AinFile ainFile;
        public CodeGenerator(AinFile ainFile)
        {
            this.ainFile = ainFile;
            this.Code = new MemoryStream();
            this.br = new BinaryReader(Code);
            this.bw = new BinaryWriter(Code);
        }

        int statementsToDiscard = 0;

        int isTopExpression = -1;

        int breakLabel = -1;
        int continueLabel = -1;

        int currentSwitchBlockIndex = -1;
        SwitchBlock currentSwitchBlock = null;

        /// <summary>
        /// Relates label number (key) to address (value)
        /// </summary>
        List<int> labels = new List<int>();
        /// <summary>
        /// Contains code index (key) and label number (value)
        /// </summary>
        List<KeyValuePair<int, int>> fixups = new List<KeyValuePair<int, int>>();

        Dictionary<int, int> CodelabelToLabel = new Dictionary<int, int>();

        int AddLabel(int address)
        {
            int labelNumber = labels.Count;
            labels.Add(labelNumber);
            return labelNumber;
        }

        int AddLabel()
        {
            return AddLabel(-1);
        }

        void SetLabel(int labelNumber)
        {
            this.labels[labelNumber] = this.Address;
        }

        void AddFixup(int codeAddress, int labelNumber)
        {
            fixups.Add(new KeyValuePair<int, int>(codeAddress, labelNumber));
        }

        void AddFixup(int labelNumber)
        {
            int codeAddress = (int)Code.Position;
            AddFixup(codeAddress, labelNumber);
        }


        public MemoryStream Code;
        private BinaryReader br;
        private BinaryWriter bw;

        public int Address;

        public void WriteInstruction(Instruction instruction)
        {
            bw.Write((short)instruction);
            Address += 2;
        }

        public void WriteInstruction(Instruction instruction, int value)
        {
            WriteInstruction(instruction);
            WriteIntValue(value);
        }

        public void WriteBranchInstruction(Instruction instruction, int labelNumber)
        {
            WriteInstruction(instruction);
            WriteDummyAddress(labelNumber);
        }


        public void WriteIntValue(int value)
        {
            bw.Write((int)value);
            Address += 4;
        }

        public void WriteDummyAddress(int labelNumber)
        {
            AddFixup(labelNumber);
            WriteIntValue(0);
        }

        public void GenerateCodeForFunction(Expression functionExpression)
        {
            GenerateCode(functionExpression);

            //Fix up labels
            foreach (var pair in fixups)
            {
                int codeAddress = pair.Key;
                int labelNumber = pair.Value;
                int labelAddress = labels[labelNumber];
                bw.Write(labelAddress, codeAddress);
            }
            this.fixups.Clear();
            this.labels.Clear();
        }

        private void GenerateCodeForSubexpressions(Expression expression)
        {
            //output each argument
            foreach (var subExpression in expression.Args)
            {
                GenerateCode(subExpression);
            }
        }

        private void GenerateCodeForThisExpression(Expression expression)
        {
            //output this instruction
            var instruction = expression.ExpressionType;
            WriteInstruction(instruction);

            int instructionArguments = instruction.GetNumberOfWords();
            if (instructionArguments >= 1)
            {
                WriteIntValue(expression.Value);
            }
            if (instructionArguments >= 2)
            {
                WriteIntValue(expression.Value2);
            }
            if (instructionArguments >= 3)
            {
                WriteIntValue(expression.Value3);
            }
        }

        void GenerateCodeDefault(Expression expression)
        {
            GenerateCodeForSubexpressions(expression);
            GenerateCodeForThisExpression(expression);
        }

        /// <summary>
        /// Outputs code, and outputs a POP instruction
        /// </summary>
        /// <param name="expression"></param>
        void GenerateCodeForStatement(Expression expression)
        {
            this.isTopExpression = 2;
            GenerateCode(expression);
            GeneratePop(expression);
        }

        private void GeneratePop(Expression expression)
        {
            if (expression == null) return;
            var variable = expression.Variable;
            var instruction = expression.ExpressionType;
            DataType expressionDataType = DataType.Void;
            if (variable != null)
            {
                expressionDataType = variable.DataType;
                var dataType2 = expression.GetDataType();
                if (expressionDataType.GetTypeOfRef() != dataType2.GetTypeOfRef())
                {
                    if (instruction == Instruction.SH_LOCALASSIGN ||
                        instruction == Instruction.SH_LOCALDEC ||
                        instruction == Instruction.SH_LOCALINC ||
                        instruction == Instruction.SH_LOCALCREATE ||
                        instruction == Instruction.SH_LOCALDELETE)
                    {
                        expressionDataType = DataType.Void;
                    }
                    else if (instruction == Instruction.FT_ASSIGNS)
                    {
                        expressionDataType = DataType.String;
                    }
                    else if (instruction == Instruction.SR_ASSIGN)
                    {
                        if (!ainFile.IsOldVersion)
                        {
                            expressionDataType = DataType.Struct;
                        }
                    }
                    else if (instruction == Instruction.ASSIGN ||
                        instruction == Instruction.R_ASSIGN ||
                        instruction == Instruction.CALLFUNC2)
                    {

                    }
                    else
                    {

                    }
                }
            }
            else
            {
                expressionDataType = expression.GetDataType();
            }

            switch (expressionDataType)
            {
                case DataType.Void:
                    break;
                default:
                case DataType.RefStruct:
                    WriteInstruction(Instruction.POP);
                    break;
                case DataType.String:
                case DataType.RefString:
                    WriteInstruction(Instruction.S_POP);
                    break;
                case DataType.Struct:
                    WriteInstruction(Instruction.SR_POP);
                    break;
                case DataType.Delegate:
                case DataType.RefDelegate:
                    WriteInstruction(Instruction.DG_POP);
                    break;
            }
        }

        private void GenerateCode(Expression expression)
        {
            if (expression == null)
            {
                return;
            }
            if (expression.address == 0x0001255e)
            {

            }

            this.isTopExpression--;
        nextStatement:
            var instruction = expression.ExpressionType;
            switch (instruction)
            {
                case Instruction.Statement:
                    {
                        var statementExpression = expression.Arg1;
                        var nextStatement = expression.Arg2;
                        if (statementsToDiscard > 0)
                        {
                            statementsToDiscard--;
                        }
                        else
                        {
                            GenerateCodeForStatement(statementExpression);
                        }
                        //prevent stack overflow
                        expression = nextStatement;
                        if (expression != null && expression.ExpressionType != Instruction.Statement)
                        {
                            GenerateCodeForStatement(expression);
                            return;
                        }
                        else
                        {
                            goto nextStatement;
                        }
                    }
                    break;
                case Instruction.FUNC:
                    {
                        GenerateCodeForThisExpression(expression);
                        GenerateCodeForSubexpressions(expression);

                        //if (expression.Root != null)
                        //{
                        //    var function = expression.Root.Variable;
                        //    if (function.Name.Contains('@') && !function.Name.EndsWith("@2"))   //fixme, constructors somtimes also get endfuncs?
                        //    {
                        //        //generate no endfunc
                        //    }
                        //    else
                        //    {
                        //        //generate ENDFUNC
                        //        WriteInstruction(Instruction.ENDFUNC, expression.Value);
                        //    }
                        //}
                    }
                    break;
                case Instruction.A_ALLOC:
                case Instruction.A_REALLOC:
                    {
                        GenerateCodeForSubexpressions(expression);
                        //restore value that was pushed onto the stack
                        WriteInstruction(Instruction.PUSH, expression.Value);
                        GenerateCodeForThisExpression(expression);
                    }
                    break;
                case Instruction.ITOF:
                    {
                        GenerateCodeDefault(expression);
                    }
                    break;
                case Instruction.F_ADD:
                case Instruction.F_ASSIGN:
                case Instruction.F_DIV:
                case Instruction.F_DIVA:
                case Instruction.F_EQUALE:
                case Instruction.F_GT:
                case Instruction.F_GTE:
                //case Instruction.F_INV:
                case Instruction.F_LT:
                case Instruction.F_LTE:
                case Instruction.F_MINUSA:
                case Instruction.F_MUL:
                case Instruction.F_MULA:
                case Instruction.F_NOTE:
                case Instruction.F_PLUSA:
                //case Instruction.F_PUSH:
                case Instruction.F_SUB:
                    {
                        bool generateSwaps = false;
                        if (expression.Arg1 != null && expression.Arg1.ExpressionType == Instruction.ITOF)
                        {
                            //??? still not quite there...
                            if (expression.Arg2 != null && expression.Arg2.ExpressionType != Instruction.ITOF)
                            {
                                generateSwaps = true;
                            }
                            //if (expression.Arg1.Arg1.ExpressionType == Instruction.SH_LOCALREF) //???
                            //{
                            //    generateSwaps = true;
                            //}
                        }

                        if (generateSwaps)
                        {
                            GenerateCodeForSubexpressions(expression.Arg1);
                            GenerateCode(expression.Arg2);
                            WriteInstruction(Instruction.SWAP);
                            WriteInstruction(Instruction.ITOF);
                            WriteInstruction(Instruction.SWAP);
                            WriteInstruction(expression.ExpressionType);

                        }
                        else
                        {
                            GenerateCodeDefault(expression);
                        }
                    }
                    break;

                case Instruction.RETURN:
                    {
                        var function = expression.Root.Variable;
                        if (function.DataType == DataType.RefStruct)
                        {
                            //if this is the last return, don't do anything special
                            var nextExpression = expression.GetParentNextExpression();
                            if (nextExpression == null || nextExpression.ExpressionType == Instruction.ENDFUNC)
                            {
                                GenerateCodeDefault(expression);
                            }
                            else
                            {
                                //This code IS necessary to make the games work.
                                GenerateCodeForSubexpressions(expression);
                                WriteInstruction(Instruction.DUP);
                                WriteInstruction(Instruction.SP_INC);
                                GenerateCodeForThisExpression(expression);
                            }
                        }
                        else
                        {
                            GenerateCodeDefault(expression);
                        }
                    }
                    break;
                case Instruction.ASSIGN:
                    {
                        //The game works fine without doing this, so this is just here to make it match the original file.

                        bool handled = false;
                        {
                            //are we assigning to a ref struct named "<dummy"?
                            var variable1 = expression.Arg1.Variable;
                            if (variable1 != null &&
                                variable1.DataType == DataType.RefStruct &&
                                variable1.Name.StartsWith("<dummy") &&
                                expression.Arg1.ExpressionType == Instruction.AssignmentRef &&
                                expression.Arg1.Arg1.ExpressionType == Instruction.PUSHLOCALPAGE &&
                                expression.Arg1.Arg2.ExpressionType == Instruction.PUSH)
                            {
                                //is previous line of code a DELETE?

                                //are we calling a method?
                                if (expression.Arg2.ExpressionType == Instruction.CALLMETHOD)
                                {
                                    var callMethodOnExpression = expression.Arg2.Arg1;
                                    Expression callMethodNextExpression = null;
                                    if (callMethodOnExpression.ExpressionType == Instruction.Comma)
                                    {
                                        callMethodNextExpression = callMethodOnExpression.Arg2;
                                        callMethodOnExpression = callMethodOnExpression.Arg1;

                                    }
                                    //output the struct we're calling the method on
                                    GenerateCode(callMethodOnExpression);
                                    //bubble it through the two instructions of the dummy reference
                                    GenerateCode(expression.Arg1.Arg1); //pushlocalpage
                                    WriteInstruction(Instruction.SWAP);
                                    GenerateCode(expression.Arg1.Arg2); //push
                                    WriteInstruction(Instruction.SWAP);
                                    //output the rest of the method arguments
                                    GenerateCode(callMethodNextExpression);
                                    //output the CALLMETHOD
                                    GenerateCodeForThisExpression(expression.Arg2);
                                    //output the ASSIGN
                                    GenerateCodeForThisExpression(expression);
                                    handled = true;
                                }


                            }
                        }


                        if (!handled)
                        {
                            GenerateCodeDefault(expression);
                        }
                    }
                    break;
                case Instruction.CALLFUNC2:
                    {
                        //This version also works, but does not match original files.
                        ////output arg3, arg1, arg2, this
                        //GenerateCode(expression.Arg3);
                        //GenerateCode(expression.Arg1);
                        //GenerateCode(expression.Arg2);
                        //GenerateCodeForThisExpression(expression);

                        //arg1 = function object
                        //arg2 = functype number (PUSH xx instruction)
                        //arg3 = functype parameters (Comma expressions)
                        //output arg1 (function object) and bubble it to the stack top
                        GenerateCode(expression.Arg1);

                        var funcType = ainFile.GetFuncType(expression.Arg2.Value);
                        if (funcType == null)
                        {
                            //TODO: handle daiteikoku later
                        }

                        //bubble the functype past each parameter up to the top of the stack
                        BubbleToTop(funcType, 0, expression.Arg3);

                        //output arg3, arg1, arg2, this
                        //GenerateCode(expression.Arg3);
                        //GenerateCode(expression.Arg1);
                        GenerateCode(expression.Arg2);
                        GenerateCodeForThisExpression(expression);
                    }
                    break;
                case Instruction.DELETE:
                    {
                        GenerateCodeForDelete(expression);
                    }
                    break;
                //case Instruction.SR_ASSIGN:
                //{
                //    var structExp = expression.Arg1;
                //    var assignmentRef = expression.Arg2;

                //    GenerateCode(structExp);
                //    GenerateCode(assignmentRef);
                //    GenerateCodeForThisExpression(expression);
                //}
                //break;
                case Instruction.StructAssignmentRef:
                case Instruction.ArrayIndex:
                case Instruction.AssignmentRef:
                case Instruction.Comma:
                    {
                        GenerateCodeForSubexpressions(expression);
                    }
                    break;
                case Instruction.Break:
                    {
                        if (breakLabel != -1)
                        {
                            WriteBranchInstruction(Instruction.JUMP, breakLabel);
                        }
                        else
                        {
                            //error
                        }
                    }
                    break;
                case Instruction.Continue:
                    {
                        if (continueLabel != -1)
                        {
                            WriteBranchInstruction(Instruction.JUMP, continueLabel);
                        }
                        else
                        {
                            //error
                        }
                    }
                    break;
                case Instruction.IfElse:
                    {
                        bool isQuestionColon = this.isTopExpression <= 0;

                        //TODO: make it distingush between value returning form "? :" and void form "if else".
                        int elseLabel = AddLabel();
                        int endifLabel = AddLabel();

                        var condition = expression.Arg1;
                        var ifBody = expression.Arg2;
                        var elseBlock = expression.Arg3;

                        GenerateCode(condition);

                        WriteBranchInstruction(Instruction.IFZ, elseLabel);

                        if (isQuestionColon)
                        {
                            GenerateCode(ifBody);
                        }
                        else
                        {
                            GenerateCodeForStatement(ifBody);
                        }

                        WriteBranchInstruction(Instruction.JUMP, endifLabel);

                        SetLabel(elseLabel);

                        if (isQuestionColon)
                        {
                            GenerateCode(elseBlock);
                        }
                        else
                        {
                            GenerateCodeForStatement(elseBlock);
                        }

                        SetLabel(endifLabel);
                    }
                    break;
                case Instruction.LogicalAnd:
                    {
                        var firstCondition = expression.Arg1;
                        var secondCondition = expression.Arg2;

                        int eitherFalseLabel = AddLabel();
                        int afterBothTrueLabel = AddLabel();

                        GenerateCode(firstCondition);

                        WriteBranchInstruction(Instruction.IFZ, eitherFalseLabel);

                        GenerateCode(secondCondition);

                        WriteBranchInstruction(Instruction.IFZ, eitherFalseLabel);

                        WriteInstruction(Instruction.PUSH, 1);
                        WriteBranchInstruction(Instruction.JUMP, afterBothTrueLabel);

                        SetLabel(eitherFalseLabel);

                        WriteInstruction(Instruction.PUSH, 0);

                        SetLabel(afterBothTrueLabel);
                    }
                    break;
                case Instruction.LogicalOr:
                    {
                        //fixme
                        var firstCondition = expression.Arg1;
                        var secondCondition = expression.Arg2;

                        int eitherTrueLabel = AddLabel();
                        int afterBothFalseLabel = AddLabel();

                        GenerateCode(firstCondition);

                        WriteBranchInstruction(Instruction.IFNZ, eitherTrueLabel);

                        GenerateCode(secondCondition);

                        WriteBranchInstruction(Instruction.IFNZ, eitherTrueLabel);

                        WriteInstruction(Instruction.PUSH, 0);
                        WriteBranchInstruction(Instruction.JUMP, afterBothFalseLabel);

                        SetLabel(eitherTrueLabel);

                        WriteInstruction(Instruction.PUSH, 1);

                        SetLabel(afterBothFalseLabel);
                    }
                    break;
                case Instruction.Label:
                    {
                        int labelNumber;
                        if (CodelabelToLabel.ContainsKey(expression.Value))
                        {
                            labelNumber = CodelabelToLabel[expression.Value];
                        }
                        else
                        {
                            labelNumber = AddLabel();
                            CodelabelToLabel.Add(expression.Value, labelNumber);
                        }
                        labels[labelNumber] = Address;
                    }
                    break;
                case Instruction.Goto:
                    {
                        int labelNumber;
                        if (CodelabelToLabel.ContainsKey(expression.Value))
                        {
                            labelNumber = CodelabelToLabel[expression.Value];
                        }
                        else
                        {
                            labelNumber = AddLabel();
                            CodelabelToLabel.Add(expression.Value, labelNumber);
                        }
                        WriteInstruction(Instruction.JUMP);
                        WriteDummyAddress(labelNumber);
                    }
                    break;
                case Instruction.ForLoop:
                    {
                        int oldBreakLabel = breakLabel;
                        int oldContinueLabel = continueLabel;

                        //Init: ...
                        //Cond: ...
                        //IFZ After
                        //JUMP Body
                        //Inc: ...
                        // JUMP Cond
                        //Body: ... 
                        // JUMP Inc
                        //After:

                        var initializer = expression.Arg1;
                        var condition = expression.Arg2;
                        var increment = expression.Arg3;
                        var loopBody = expression.Arg4;

                        int conditionLabel = AddLabel();
                        int incrementLabel = AddLabel();
                        int loopBodyLabel = AddLabel();
                        int afterLabel = AddLabel();

                        breakLabel = afterLabel;
                        continueLabel = incrementLabel;

                        GenerateCodeForStatement(initializer);

                        SetLabel(conditionLabel);

                        if (condition != null || increment != null)
                        {
                            GenerateCode(condition);
                            WriteBranchInstruction(Instruction.IFZ, afterLabel);
                            WriteBranchInstruction(Instruction.JUMP, loopBodyLabel);

                            SetLabel(incrementLabel);

                            GenerateCodeForStatement(increment);
                            WriteBranchInstruction(Instruction.JUMP, conditionLabel);
                        }
                        else
                        {
                            SetLabel(incrementLabel);
                        }

                        SetLabel(loopBodyLabel);

                        GenerateCodeForStatement(loopBody);
                        WriteBranchInstruction(Instruction.JUMP, incrementLabel);

                        SetLabel(afterLabel);

                        breakLabel = oldBreakLabel;
                        continueLabel = oldContinueLabel;
                    }
                    break;
                case Instruction.WhileLoop:
                    {
                        int oldBreakLabel = breakLabel;
                        int oldContinueLabel = continueLabel;

                        //<first>
                        //ifz <target>
                        //...
                        //jump <before first>
                        //<target>:

                        var condition = expression.Arg1;
                        var loopBody = expression.Arg2;

                        int conditionLabel = AddLabel();
                        //int loopBodyLabel = AddLabel();
                        int afterLabel = AddLabel();

                        continueLabel = conditionLabel;
                        breakLabel = afterLabel;

                        SetLabel(conditionLabel);
                        GenerateCode(condition);
                        WriteBranchInstruction(Instruction.IFZ, afterLabel);

                        GenerateCodeForStatement(loopBody);

                        WriteBranchInstruction(Instruction.JUMP, conditionLabel);

                        SetLabel(afterLabel);

                        breakLabel = oldBreakLabel;
                        continueLabel = oldContinueLabel;
                    }
                    break;
                case Instruction.SWITCH:
                case Instruction.STRSWITCH:
                    {
                        int oldBreakLabel = breakLabel;
                        int oldSwitchBlockIndex = currentSwitchBlockIndex;
                        SwitchBlock oldSwitchBlock = currentSwitchBlock;

                        currentSwitchBlockIndex = ainFile.Switches.Count;
                        currentSwitchBlock = new SwitchBlock();
                        currentSwitchBlock.CaseType2or4 = 2;
                        currentSwitchBlock.Index = currentSwitchBlockIndex;
                        currentSwitchBlock.Root = ainFile;
                        currentSwitchBlock.DefaultCaseAddress = -1;
                        ainFile.Switches.Add(currentSwitchBlock);

                        var switchValue = expression.Arg1;
                        var switchCase = expression.Arg2;
                        var switchVariaible = switchValue.Variable;
                        if (switchVariaible != null && switchVariaible.DataType.IsString())
                        {
                            currentSwitchBlock.CaseType2or4 = 4;
                        }

                        int afterLabel = AddLabel();
                        breakLabel = afterLabel;

                        GenerateCode(switchValue);
                        if (currentSwitchBlock.CaseType2or4 != 4)
                        {
                            WriteInstruction(Instruction.SWITCH);
                        }
                        else
                        {
                            WriteInstruction(Instruction.STRSWITCH);
                        }
                        WriteIntValue(currentSwitchBlockIndex);
                        WriteBranchInstruction(Instruction.JUMP, afterLabel);

                        GenerateCode(switchCase);

                        SetLabel(afterLabel);

                        currentSwitchBlock = oldSwitchBlock;
                        currentSwitchBlockIndex = oldSwitchBlockIndex;
                        breakLabel = oldBreakLabel;
                    }
                    break;
                case Instruction.DefaultSwitchCase:
                case Instruction.SwitchCase:
                    {
                        if (currentSwitchBlock == null)
                        {
                            //error
                        }

                        if (instruction == Instruction.DefaultSwitchCase)
                        {
                            currentSwitchBlock.DefaultCaseAddress = Address;
                        }
                        else
                        {
                            var newSwitchCase = new SwitchCase();
                            newSwitchCase.Parent = currentSwitchBlock;
                            newSwitchCase.Index = currentSwitchBlock.SwitchCases.Count;
                            newSwitchCase.Root = ainFile;
                            newSwitchCase.TargetAddress = Address;
                            newSwitchCase.Value = expression.Value;
                            currentSwitchBlock.SwitchCases.Add(newSwitchCase);
                        }

                        var caseBody = expression.Arg1;
                        var nextCase = expression.Arg2;

                        GenerateCodeForStatement(caseBody);
                        GenerateCode(nextCase);
                    }
                    break;
                case Instruction.Nop:
                    break;
                case Instruction.INC:
                case Instruction.LI_INC:
                case Instruction.DEC:
                case Instruction.LI_DEC:
                    {
                        if (expression.Arg1.ExpressionType == Instruction.REF)
                        {
                            GenerateCodeForSubexpressions(expression.Arg1);
                        }
                        else
                        {
                            GenerateCode(expression.Arg1);
                        }
                        WriteInstruction(Instruction.DUP2);  //duplicate page/index pair
                        WriteInstruction(Instruction.REF);   //read variable
                        WriteInstruction(Instruction.DUP_X2); //copy page/index pair and variaible contents
                        WriteInstruction(Instruction.POP);   //remove variaible that was read
                        WriteInstruction(instruction);       //do the operation
                    }
                    break;
                case Instruction.PreDecrement:
                case Instruction.PreIncrement:
                    {
                        if (expression.Arg1.ExpressionType == Instruction.REF)
                        {
                            GenerateCodeForSubexpressions(expression.Arg1);
                        }
                        else
                        {
                            GenerateCode(expression.Arg1);
                        }
                        var variable = expression.Arg1.Variable;
                        if (instruction == Instruction.PreIncrement)
                        {
                            if (variable.DataType == DataType.Lint || variable.DataType == DataType.RefLint)
                            {
                                instruction = Instruction.LI_INC;
                            }
                            else
                            {
                                instruction = Instruction.INC;
                            }
                        }
                        else
                        {
                            if (variable.DataType == DataType.Lint || variable.DataType == DataType.RefLint)
                            {
                                instruction = Instruction.LI_DEC;
                            }
                            else
                            {
                                instruction = Instruction.DEC;
                            }
                        }
                        WriteInstruction(Instruction.DUP2);
                        WriteInstruction(instruction);
                        if (variable.DataType == DataType.Int && variable.Parent is Function)
                        {
                            WriteInstruction(Instruction.REF);
                        }
                        else
                        {
                            WriteInstruction(Instruction.POP);
                        }
                    }
                    break;
                default:
                    if (instruction < 0)
                    {
                        //error
                    }
                    else
                    {
                        GenerateCodeDefault(expression);
                    }
                    break;
            }



        }

        private void GenerateCodeForDelete(Expression expression)
        {
            //this code IS necessary.

            bool handled = false;
            Expression nextStatement = expression;
            if (expression.Parent.ExpressionType == Instruction.Statement)
            {
                //skipping statements (needed for daibanchou)
                int statementsSkipped = 0;
                while (true)
                {
                    nextStatement = nextStatement.GetParentNextExpression();
                    if (nextStatement.ExpressionType == Instruction.Statement)
                    {
                        nextStatement = nextStatement.Arg1;
                    }
                    if (nextStatement.ExpressionType == Instruction.CALLSYS && (nextStatement.Value == 3 || nextStatement.Value == 4))
                    {
                        statementsSkipped++;
                    }
                    else
                    {
                        break;
                    }
                }

                var arg1 = expression.Arg1;
                IVariable arg1Variable = null;
                if (expression.Arg1 != null) arg1Variable = expression.Arg1.Variable;
                IVariable nextVariable = null;
                if (nextStatement.Arg1 != null) nextVariable = nextStatement.Arg1.Variable;

                //is it REF of a ref type, and next statement is an assignment?
                if (nextStatement.ExpressionType == Instruction.ASSIGN)
                {
                    if (expression.Arg1.ExpressionType == Instruction.REF &&
                        arg1Variable == nextVariable &&
                        arg1Variable.DataType == DataType.RefStruct)
                    {
                        //is previous statement a CALLSYS 03 (system.lockpeek)
                        //and next statement callsys 04?
                        var previousStatement = expression.Parent.Parent.Arg1;
                        var nextStatement1 = expression.GetParentNextExpression().Arg1;
                        var nextStatement2 = nextStatement1.GetParentNextExpression().Arg1;
                        var var2 = nextStatement.Arg2.Variable;

                        if (previousStatement.ExpressionType == Instruction.CALLSYS && previousStatement.Value == 3 && 
                            (ainFile.IsOldVersion || (ainFile.Version >= 5 && var2.DataType == DataType.Struct)))
                        {
                            bw.BaseStream.Position -= 8;
                            Address -= 8;
                            GenerateCodeForSubexpressions(arg1);
                            GenerateCodeForStatement(previousStatement); //callsys 03, pop
                        }
                        else
                        {
                            GenerateCodeForSubexpressions(arg1);
                        }


                        //output the arguments of the REF
                        //GenerateCodeForSubexpressions(arg1);
                        //output DUP2, REF, DELETE
                        WriteInstruction(Instruction.DUP2);
                        WriteInstruction(Instruction.REF);
                        WriteInstruction(Instruction.DELETE);
                        if (nextStatement1.ExpressionType == Instruction.CALLSYS && nextStatement1.Value == 4)
                        {
                            GenerateCodeForStatement(nextStatement1); //callsys 04, pop
                            GenerateCode(nextStatement.Arg2);
                            if (nextStatement2.ExpressionType == Instruction.CALLSYS && nextStatement2.Value == 3)
                            {
                                GenerateCodeForStatement(nextStatement2);
                                WriteInstruction(Instruction.DUP);
                                WriteInstruction(Instruction.SP_INC);
                                WriteInstruction(Instruction.ASSIGN);
                                WriteInstruction(Instruction.POP);
                            }
                            else
                            {
                                WriteInstruction(Instruction.ASSIGN);
                                WriteInstruction(Instruction.POP);
                            }
                        }
                        else
                        {
                            //var var2 = nextStatement.Arg2.Variable;
                            if (var2 != null && var2.Name.StartsWith("<dummy") && ainFile.IsOldVersion)
                            {
                                GenerateCode(nextStatement.Arg2);
                                WriteInstruction(Instruction.DUP);
                                WriteInstruction(Instruction.SP_INC);
                                WriteInstruction(Instruction.ASSIGN);
                                WriteInstruction(Instruction.POP);
                            }
                            else
                            {
                                if (var2.DataType == DataType.Int && ainFile.Version >= 5)
                                {
                                    GenerateCode(nextStatement.Arg2);
                                    WriteInstruction(Instruction.ASSIGN);
                                    WriteInstruction(Instruction.POP);
                                }
                                else if (var2.DataType == DataType.Struct && ainFile.Version >= 5)
                                {
                                    GenerateCode(nextStatement.Arg2);
                                    WriteInstruction(Instruction.DUP);
                                    WriteInstruction(Instruction.SP_INC);
                                    WriteInstruction(Instruction.ASSIGN);
                                    WriteInstruction(Instruction.POP);
                                }
                                else
                                {
                                    //output DUP2, (right side of assign), ASSIGN
                                    WriteInstruction(Instruction.DUP2);
                                    GenerateCode(nextStatement.Arg2);
                                    WriteInstruction(Instruction.ASSIGN);
                                    //output DUP_X2, POP, REF, SP_INC, POP
                                    WriteInstruction(Instruction.DUP_X2);
                                    WriteInstruction(Instruction.POP);
                                    WriteInstruction(Instruction.REF);
                                    WriteInstruction(Instruction.SP_INC);
                                    WriteInstruction(Instruction.POP);
                                }
                            }
                        }

                        statementsToDiscard = 1 + statementsSkipped;
                        handled = true;



                    }
                    else if (expression.Arg1.ExpressionType == Instruction.REF &&
                        arg1Variable == nextVariable &&
                        arg1Variable.DataType == DataType.RefArrayStruct)
                    {
                        //is previous statement a CALLSYS 03 (system.lockpeek)
                        //and next statements callsys 04 and callsys 03?
                        var previousStatement = expression.Parent.Parent.Arg1;
                        var nextStatement1 = expression.GetParentNextExpression().Arg1;
                        var nextStatement2 = nextStatement1.GetParentNextExpression().Arg1;
                        if (previousStatement.ExpressionType == Instruction.CALLSYS && previousStatement.Value == 3 &&
                            nextStatement1.ExpressionType == Instruction.CALLSYS && nextStatement1.Value == 4 &&
                            nextStatement2.ExpressionType == Instruction.CALLSYS && nextStatement2.Value == 3)
                        {
                            //go back 8 bytes to erase CALLSYS 0x03 / POP
                            bw.BaseStream.Position -= 8;
                            Address -= 8;
                            GenerateCodeForSubexpressions(arg1);
                            GenerateCodeForStatement(previousStatement); //callsys 03, pop
                            WriteInstruction(Instruction.DUP2);
                            WriteInstruction(Instruction.REF);
                            WriteInstruction(Instruction.DELETE);
                            GenerateCodeForStatement(nextStatement1); //callsys 04, pop
                            GenerateCode(nextStatement.Arg2);
                            GenerateCodeForStatement(nextStatement2); //callsys 03, pop
                            WriteInstruction(Instruction.DUP);
                            WriteInstruction(Instruction.SP_INC);
                            WriteInstruction(Instruction.ASSIGN);
                            WriteInstruction(Instruction.POP);

                            statementsToDiscard = 1 + statementsSkipped;
                            handled = true;
                        }
                        else
                        {
                            GenerateCodeForSubexpressions(arg1);
                            WriteInstruction(Instruction.DUP2);
                            WriteInstruction(Instruction.REF);
                            WriteInstruction(Instruction.DELETE);
                            GenerateCode(nextStatement.Arg2);
                            WriteInstruction(Instruction.DUP);
                            WriteInstruction(Instruction.SP_INC);
                            WriteInstruction(Instruction.ASSIGN);
                            WriteInstruction(Instruction.POP);

                            statementsToDiscard = 1 + statementsSkipped;
                            handled = true;
                        }

                    }
                }
                else if (nextStatement.ExpressionType == Instruction.R_ASSIGN)
                {
                    if (expression.Arg1.ExpressionType == Instruction.AssignmentRef &&
                        arg1Variable == nextVariable &&
                        arg1Variable.DataType.IsPrimitiveRefType())
                    {

                        GenerateCodeForSubexpressions(arg1);
                        WriteInstruction(Instruction.DUP2);
                        WriteInstruction(nextStatement.Arg1.ExpressionType);
                        WriteInstruction(Instruction.POP);
                        WriteInstruction(Instruction.DELETE);
                        WriteInstruction(Instruction.DUP2);
                        GenerateCode(nextStatement.Arg2);
                        WriteInstruction(nextStatement.ExpressionType);
                        WriteInstruction(Instruction.POP);
                        WriteInstruction(Instruction.POP);
                        WriteInstruction(Instruction.REF);
                        WriteInstruction(Instruction.SP_INC);

                        statementsToDiscard = 1 + statementsSkipped;
                        handled = true;
                    }
                }
                if (!handled)
                {
                    GenerateCodeDefault(expression);
                }
            }
        }

        private void BubbleToTop(IFunction funcType, int parameterIndex, Expression expression)
        {
            if (expression == null) return;
            if (funcType == null) return;
            if (parameterIndex < 0 || parameterIndex >= funcType.ParameterCount) return;
            var parameter = funcType.Parameters[parameterIndex];
            Expression argumentExpression = expression;
            Expression nextExpression = null;
            if (argumentExpression.ExpressionType == Instruction.Comma)
            {
                argumentExpression = expression.Arg1;
                nextExpression = expression.Arg2;
            }

            if (parameter.DataType.IsPrimitiveRefType())
            {
                GenerateCode(argumentExpression);
                WriteInstruction(Instruction.DUP2_X1);
                WriteInstruction(Instruction.POP);
                WriteInstruction(Instruction.POP);
                parameterIndex += 2;
            }
            else
            {
                GenerateCode(argumentExpression);
                WriteInstruction(Instruction.SWAP);
                parameterIndex++;
            }
            BubbleToTop(funcType, parameterIndex, nextExpression);
        }
    }

    public static class MemoryStreamExtensions
    {
        public static void Write(this BinaryWriter bw, int value, int address)
        {
            long lastPosition = bw.BaseStream.Position;
            bw.BaseStream.Position = address;
            bw.Write(value);
            bw.BaseStream.Position = lastPosition;
        }

        public static int ReadInt32(this BinaryReader br, int address)
        {
            long lastPosition = br.BaseStream.Position;
            br.BaseStream.Position = address;
            int value = br.ReadInt32();
            br.BaseStream.Position = lastPosition;
            return value;
        }
    }
}
