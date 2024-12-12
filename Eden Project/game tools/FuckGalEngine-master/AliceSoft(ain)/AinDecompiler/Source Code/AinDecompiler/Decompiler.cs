using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Text;
using System.Globalization;

namespace AinDecompiler
{
    public class Decompiler
    {
        //private MyIndentedTextWriter tw;
        public AinFile ainFile;
        private ExpressionDisplayer displayer = new ExpressionDisplayer();

        public static readonly Dictionary<int, string> InstructionToString = MakeInstructionToString();
        public static readonly Dictionary<int, int> NumberOfWordsPerInstruction = MakeNumberOfWordsPerInstruction();

        private static Dictionary<int, int> MakeNumberOfWordsPerInstruction()
        {
            var values = Enum.GetValues(typeof(Instruction)).Cast<Instruction>().ToArray();
            var dic = new Dictionary<int, int>(values.Length);

            foreach (var value in values)
            {
                int totalArguments = 0;
                if (InstructionInformation.instructionsWithOneArgument.Contains(value))
                {
                    totalArguments = 1;
                }
                if (InstructionInformation.instructionsWithTwoArguments.Contains(value))
                {
                    totalArguments = 2;
                }
                if (InstructionInformation.instructionsWithThreeArguments.Contains(value))
                {
                    totalArguments = 3;
                }
                dic.Add((int)value, totalArguments);
            }
            return dic;
        }

        private static Dictionary<int, string> MakeInstructionToString()
        {
            var names = Enum.GetNames(typeof(Instruction));
            var values = Enum.GetValues(typeof(Instruction)).Cast<int>().ToArray();
            var dic = new Dictionary<int, string>(names.Length);

            for (int i = 0; i < names.Length; i++)
            {
                dic.Add(values[i], names[i]);
            }
            return dic;
        }

        public Decompiler(AinFile ainFile)
        {
            this.ainFile = ainFile;
            displayer.ainFile = ainFile;
        }

        public static int GetArgumentCount(Instruction instruction)
        {
            if (Decompiler.NumberOfWordsPerInstruction.ContainsKey((int)instruction))
            {
                return Decompiler.NumberOfWordsPerInstruction[(int)instruction];
            }
            else
            {
                return 0;
            }

            //if (instructionsWithOneArgument.Contains(instruction))
            //{
            //    return 1;
            //}
            //if (instructionsWithTwoArguments.Contains(instruction))
            //{
            //    return 2;
            //}
            //return 0;
        }

        public InstructionInfo PeekPiece(int address, ref IEnumerator<InstructionInfo> enumerator)
        {
            if (enumerator != null && enumerator.MoveNext())
            {
                return enumerator.Current;
            }
            var instructionInfo = Peek(address);
            if (instructionInfo.instruction >= Instruction.SH_SR_ASSIGN)
            {
                enumerator = CompositeInstructions.EnumerateInstructions(instructionInfo).GetEnumerator();
                if (enumerator.MoveNext())
                {
                    return enumerator.Current;
                }
            }
            return instructionInfo;
        }

        private InstructionInfo Peek(int address)
        {
            return Peek(ainFile.Code, address);
        }

        public static InstructionInfo Peek(byte[] code, int address)
        {
            if (address < 0 || address >= code.Length)
            {
                return new InstructionInfo().Clone(Instruction.Nop);
            }

            Instruction instruction = (Instruction)BitConverter.ToInt16(code, address);
            address += 2;

            int word1 = 0, word2 = 0, word3 = 0;
            int totalArguments = instruction.GetNumberOfWords();
            //if (NumberOfWordsPerInstruction.ContainsKey((int)instruction))
            //{
            //    totalArguments = NumberOfWordsPerInstruction[(int)instruction];
            //}
            ////int totalArguments = GetArgumentCount(instruction);
            if (totalArguments >= 1)
            {
                word1 = BitConverter.ToInt32(code, address);
                address += 4;
            }
            if (totalArguments >= 2)
            {
                word2 = BitConverter.ToInt32(code, address);
                address += 4;
            }
            if (totalArguments >= 3)
            {
                word3 = BitConverter.ToInt32(code, address);
                address += 4;
            }
            return new InstructionInfo()
            {
                instruction = instruction,
                nextAddress = address,
                word1 = word1,
                word2 = word2,
                word3 = word3,
                totalArguments = totalArguments
            };
        }

        public Expression DecompileRange(int address, int limit)
        {
            this.labelNumber = 0;
            this.currentLabels.Clear();
            //this.labelsList.Clear();
            this.jumpList.Clear();
            Expression.defaultAinFile = this.ainFile;
            DecompileState state = new DecompileState();
            var returnExpression = DecompileRange(address, limit, state);
            CleanUpJumps(returnExpression);
            RemoveNops(returnExpression);
            return returnExpression;
        }

        private void RemoveNops(Expression returnExpression)
        {
            var e = returnExpression;
            while (e != null)
            {
                var next = e.GetNextExpression();
                while (next != null && next.ExpressionType == Instruction.Nop)
                {
                    var statementToRemove = next;
                    next = e.Parent;
                    Expression.RemoveStatement(statementToRemove);
                    //???
                    if (next.Parent == null && next.ExpressionType != Instruction.FUNC)
                    {
                        next = e.Parent;
                    }
                }
                e = next;
            }
        }

        public Expression DecompileFunction(Function function)
        {
            int functionAddress = function.Address;
            return DecompileFunction(functionAddress);
        }

        public Expression DecompileFunction(int functionAddress)
        {
            int startAddress = functionAddress - 6;
            int endAddress = FindEndAddress(functionAddress);
            var expression = DecompileRange(startAddress, endAddress);
            var lastInstruction = Peek(endAddress - 6);
            if (lastInstruction.instruction == Instruction.ENDFUNC)
            {
                expression.Arg2 = new Expression(Instruction.ENDFUNC, lastInstruction.CurrentAddress, lastInstruction.word1);
            }
            return expression;
        }

        public Expression DecompileFunction(string functionName)
        {
            if (ainFile.FunctionNameToIndex.ContainsKey(functionName))
            {
                var function = ainFile.Functions[ainFile.FunctionNameToIndex[functionName]];
                return DecompileFunction(function);
            }
            else
            {
                return null;
            }
        }

        /// <summary>
        /// Returns the address of the ENDFUNC instruction, or an address before the next FUNC or EOF instruction.
        /// </summary>
        /// <param name="functionAddress"></param>
        /// <returns></returns>
        public int FindEndAddress(int functionAddress)
        {
            //look ahead for FUNC, ENDFUNC, or EOF
            int address = functionAddress;
            int endAddress = -1;
            while (address < ainFile.Code.Length)
            {
                var instructionInfo = Peek(address);
                address = instructionInfo.nextAddress;
                if (instructionInfo.instruction == Instruction.FUNC ||
                    instructionInfo.instruction == Instruction.ENDFUNC ||
                    instructionInfo.instruction == Instruction.EOF)
                {
                    endAddress = address;
                    if (instructionInfo.instruction != Instruction.ENDFUNC)
                    {
                        endAddress -= 6;
                    }

                    break;
                }
            }
            return endAddress;
        }

        public class DecompileState
        {
            public int startAddress = -1, finalAddress = -1, exitAddress = -1, functionId = -1, breakAddress = -1, continueAddress = -1;
            public int parentLimitAddress = -1;
            public bool isStackExpression = false;
            public bool breakOnReturn = false;
            /// <summary>
            /// Indicates that we are inside an IF expression and checks for the end of an IF expression overrule labels
            /// </summary>
            public bool insideIfExpression = false;
            public bool isElse = false;
            public bool insideLogicalBoolean = false;
            public DecompileState()
            {

            }
            public DecompileState Clone()
            {
                var clone = (DecompileState)this.MemberwiseClone();
                clone.breakOnReturn = false;
                clone.insideIfExpression = false;
                clone.parentState = this;
                clone.statements = null;
                clone.isElse = false;
                return clone;
            }
            public DecompileState Clone(int parentLimitAddress)
            {
                var clone = Clone();
                clone.parentLimitAddress = parentLimitAddress;
                return clone;
            }

            public DecompileState parentState;
            public IList<Expression> statements;
        }

        public Expression DecompileRange(int address, int limit, DecompileState state)
        {
            bool processingWholeFunction = false;
            int startAddress = address;
            Stack<Expression> stack = new Stack<Expression>();
            List<Expression> statements = new List<Expression>();
            state.finalAddress = address;
            state.startAddress = address;
            state.statements = statements;
            int functionId = state.functionId;

            displayer.currentFuncNumber = functionId;
            Expression.defaultFunctionNumber = functionId;

            HashSet<int> infiniteLoopTest = new HashSet<int>();

            IEnumerator<InstructionInfo> pieces = null;

            while (address < limit && address >= startAddress)
            {
                state.finalAddress = address;
                int currentAddress = address;
                var instructionInfo = PeekPiece(address, ref pieces);
                Instruction instruction = instructionInfo.instruction;
                address = instructionInfo.nextAddress;
                int word1 = instructionInfo.word1;
                int word2 = instructionInfo.word2;
                int totalArguments = instructionInfo.totalArguments;

                bool instructionHandledBySwitchBlock = true;

                switch (instruction)
                {
                    case Instruction.DG_SET:
                        {
                            //thing to assign to
                            //reference to a struct
                            //int (actually a function)
                            var C = stack.Pop();
                            var B = stack.Pop();
                            var A = stack.Pop();
                            stack.Push(new Expression(instruction, currentAddress, A, B, C));
                        }
                        break;
                    case Instruction.NEW:
                        {
                            var A = stack.Pop();
                            stack.Push(new Expression(instruction, currentAddress, A));
                        }
                        break;
                    case Instruction.FTOS:
                        {
                            var dummy = stack.Pop();
                            var A = stack.Pop();
                            var newExpression = new Expression(instruction, currentAddress, A, dummy);
                            stack.Push(newExpression);
                        }
                        break;
                    case Instruction.PUSHSTRUCTPAGE:
                        {
                            var newExpression = new Expression(instruction, currentAddress);
                            //weird code in "CMessageConfigView::CheckInput" puts a POP after this
                            newExpression.removeIfVoid = true;
                            stack.Push(newExpression);
                        }
                        break;
                    case Instruction.PAGE_REF:
                    case Instruction.S_REF2:
                        {
                            //var A = stack.Pop();
                            //stack.Push(new Expression(instruction, currentAddress, A, word1));

                            var B = new Expression(Instruction.PUSH, currentAddress, word1);
                            var A = stack.Pop();
                            stack.Push(new Expression(instruction, currentAddress, A, B));
                        }
                        break;
                    case Instruction.R_EQUALE:
                        {
                            var B = stack.PopReference(currentAddress);
                            var A = stack.PopReference(currentAddress);
                            stack.Push(new Expression(instruction, currentAddress, B, A));
                        }
                        break;
                    case Instruction.S_POPBACK:
                    case Instruction.S_POPBACK2:
                        {
                            var stringToPopFrom = stack.Pop();
                            stack.Push(new Expression(instruction, currentAddress, stringToPopFrom));
                            statements.Add(stack.Pop());
                        }
                        break;

                    case Instruction.S_PUSHBACK:
                    case Instruction.S_PUSHBACK2:
                        {
                            var pushCharacter = stack.Pop();
                            var stringToPushTo = stack.Pop();
                            stack.Push(new Expression(instruction, currentAddress, stringToPushTo, pushCharacter));
                            statements.Add(stack.Pop());
                        }
                        break;

                    case Instruction.SH_SR_ASSIGN:
                        {
                            var B = stack.Pop();
                            var A = stack.Pop();
                            var assignThis = new Expression(Instruction.AssignmentRef, currentAddress, A, B);

                            //var assignThis = stack.PopReference(currentAddress);
                            var assignToThis = stack.PopReference(currentAddress);
                            stack.Push(new Expression(instruction, currentAddress, assignToThis, assignThis));
                            statements.Add(stack.Pop());
                        }
                        break;
                    case Instruction.A_FIND:
                        {
                            var comparisonFunction = stack.Pop();
                            var lookFor = stack.Pop();
                            var endIndex = stack.Pop();
                            var startIndex = stack.Pop();
                            var array = stack.PopReference(currentAddress);
                            stack.Push(new Expression(instruction, currentAddress, array, startIndex, endIndex, lookFor, comparisonFunction));
                        }
                        break;
                    case Instruction.S_ERASE:
                    case Instruction.S_ERASE2:
                        {
                            var dummy = stack.Pop();
                            var index = stack.Pop();
                            var str = stack.PopReference(currentAddress);
                            stack.Push(new Expression(instruction, currentAddress, str, index, dummy));
                            statements.Add(stack.Pop());
                        }
                        break;
                    case Instruction.A_SORT:
                        {
                            var functionNumber = stack.Pop();
                            var array = stack.PopReference(currentAddress);
                            stack.Push(new Expression(instruction, currentAddress, array, functionNumber));
                            statements.Add(stack.Pop());
                        }
                        break;
                    case Instruction.A_REVERSE:
                        {
                            var array = stack.PopReference(currentAddress);
                            stack.Push(new Expression(instruction, currentAddress, array));
                            statements.Add(stack.Pop());
                        }
                        break;
                    case Instruction.A_FILL:
                        {
                            var fillValue = stack.Pop();
                            var length = stack.Pop();
                            var start = stack.Pop();
                            var b = stack.Pop();
                            var array = new Expression(Instruction.AssignmentRef, currentAddress, stack.Pop(), b);
                            stack.Push(new Expression(instruction, currentAddress, array, start, length, fillValue));
                        }
                        break;
                    case Instruction.OBJSWAP:
                        {
                            var dataType = stack.Pop();
                            Expression B = null;
                            Expression A = null;
                            var d = stack.Pop();
                            var c = stack.Pop();
                            var b = stack.Pop();
                            var a = stack.Pop();

                            var obj2 = new Expression(Instruction.AssignmentRef, currentAddress, c, d);
                            var obj1 = new Expression(Instruction.AssignmentRef, currentAddress, a, b);

                            stack.Push(new Expression(instruction, currentAddress, obj1, obj2, dataType));
                            statements.Add(stack.Pop());
                        }
                        break;
                    case Instruction.FT_ASSIGNS:
                        {
                            var functionType = stack.Pop();  //push
                            var functionName = stack.Pop();  //string reference
                            var assignTo = stack.PopReference(currentAddress);
                            stack.Push(new Expression(instruction, currentAddress, assignTo, functionName, functionType));
                        }
                        break;
                    case Instruction.R_ASSIGN:
                        {
                            //???
                            var B = stack.Pop();
                            var Btype = displayer.GetReferencedVariableType(B);
                            var A = stack.Pop();
                            var Atype = displayer.GetReferencedVariableType(A);
                            var reference = A;
                            if (Atype.ArrayDimensions > 0)
                            {
                                if (Atype.ArrayDimensions > 1)
                                {

                                }

                                B = new Expression(Instruction.ArrayIndex, currentAddress, A, B);
                                reference = stack.PopReference(currentAddress);
                            }
                            else if (!A.IsReference() && !B.IsReference())
                            {
                                if (A.ExpressionType == Instruction.PUSH && B.ExpressionType == Instruction.PUSH)
                                {
                                    stack.Push(new Expression(Instruction.Nop, currentAddress));
                                }
                                else
                                {
                                    B = new Expression(Instruction.AssignmentRef, currentAddress, A, B);
                                    reference = stack.PopReference(currentAddress);
                                }
                            }
                            else
                            {

                            }


                            stack.Push(new Expression(instruction, currentAddress, reference, B));
                        }
                        break;
                    case Instruction.A_ERASE:
                        {
                            var index = stack.Pop();
                            var arr = stack.PopReference(currentAddress);
                            stack.Push(new Expression(instruction, currentAddress, arr, index));
                            //statements.Add(stack.Pop());
                        }
                        break;
                    case Instruction.S_EMPTY:
                        {
                            var str = stack.Pop();  //or popreference?
                            stack.Push(new Expression(instruction, currentAddress, str));
                        }
                        break;
                    case Instruction.DEC:
                    case Instruction.LI_DEC:
                    case Instruction.INC:
                    case Instruction.LI_INC:
                        {
                            Expression B = stack.PopReference(currentAddress);
                            if (!B.removeIfVoid)
                            {

                            }
                            Expression A = stack.PopReference(currentAddress);
                            Expression newExpression = new Expression(instruction, currentAddress, A, B);
                            stack.Push(newExpression);

                            //peek ahead - are there two pops immediately after this?
                            var i1 = Peek(address);
                            var i2 = Peek(address + 2);
                            bool hasOnePop = i1.instruction == Instruction.POP;
                            bool hasTwoPops = (i1.instruction == Instruction.POP && i2.instruction == Instruction.POP);
                            if (hasTwoPops)
                            {
                                newExpression.ExpressionType = Instruction.PreIncrement;
                                if (instruction == Instruction.DEC || instruction == Instruction.LI_DEC) newExpression.ExpressionType = Instruction.PreDecrement;
                                var copy = newExpression.Clone();
                                copy.removeIfVoid = true;
                                stack.Push(copy);
                            }
                        }
                        break;
                    case Instruction.A_INSERT:
                        {
                            var value = stack.Pop();
                            var index = stack.Pop();
                            var array = stack.PopReference(currentAddress);
                            stack.Push(new Expression(instruction, currentAddress, array, index, value));
                            statements.Add(stack.Pop());
                        }
                        break;
                    case Instruction.CALLFUNC2:
                        {
                            var funcTypeStackValue = stack.Pop();
                            int funcTypeIndex = funcTypeStackValue.Value;
                            FunctionType funcType = null;
                            if (funcTypeIndex >= 0 && funcTypeIndex < ainFile.FunctionTypes.Count)
                            {
                                funcType = ainFile.FunctionTypes[funcTypeIndex];
                            }
                            else if (funcTypeIndex == -1 && ainFile.Version >= 6)
                            {
                                //void function that eats the whole stack
                                int newArgumentCount = stack.Count - 1;
                                funcType = new FunctionType();
                                funcType.ParameterCount = newArgumentCount;
                                funcType.DataType = DataType.Void;
                                funcType.Parameters.Clear();
                                for (int i = 0; i < newArgumentCount; i++)
                                {
                                    var newVariable = new Variable();
                                    newVariable.Name = i.ToString(CultureInfo.InvariantCulture);
                                    newVariable.DataType = DataType.Int;
                                    newVariable.Root = this.ainFile;
                                    newVariable.Parent = funcType;
                                    funcType.Parameters.Add(newVariable);
                                }
                                funcType.Root = this.ainFile;
                            }
                            else
                            {

                            }

                            var functionPointer = stack.Pop();

                            int functionArguments = 0;
                            if (funcType != null)
                            {
                                functionArguments = funcType.ParameterCount;
                            }
                            int currentArgument = functionArguments;
                            Expression argument = null;
                            while (functionArguments > 0)
                            {
                                functionArguments--;
                                currentArgument--;
                                Expression nextArgument = null;
                                if (currentArgument >= 0)
                                {
                                    var parameter = funcType.Parameters[currentArgument];
                                    if (parameter.DataType == DataType.Void)
                                    {
                                        nextArgument = stack.Pop();
                                        nextArgument = new Expression(Instruction.AssignmentRef, currentAddress, stack.Pop(), nextArgument);
                                        functionArguments--;
                                        currentArgument--;
                                    }
                                    else
                                    {
                                        nextArgument = stack.Pop();
                                    }
                                }
                                else
                                {
                                    nextArgument = stack.Pop();
                                }
                                if (argument == null)
                                {
                                    argument = nextArgument;
                                }
                                else
                                {
                                    argument = new Expression(Instruction.Comma, currentAddress, nextArgument, argument);
                                }
                            }
                            stack.Push(new Expression(instruction, currentAddress, functionPointer, funcTypeStackValue, argument));
                            if (funcType == null || funcType.DataType == DataType.Void)
                            {
                                statements.Add(stack.Pop());
                            }
                        }
                        break;
                    case Instruction.DG_CALLBEGIN:
                        {
                            int delegateTypeIndex = word1;
                            FunctionType delg = null;
                            if (delegateTypeIndex >= 0 && delegateTypeIndex < ainFile.Delegates.Count)
                            {
                                delg = ainFile.Delegates[delegateTypeIndex];
                            }
                            else
                            {

                            }
                            int functionArguments = 0;
                            if (delg != null)
                            {
                                functionArguments = delg.ParameterCount;
                            }
                            int currentArgument = functionArguments;
                            Expression argument = null;
                            while (functionArguments > 0)
                            {
                                functionArguments--;
                                currentArgument--;
                                Expression nextArgument = null;
                                if (currentArgument >= 0)
                                {
                                    var parameter = delg.Parameters[currentArgument];
                                    if (parameter.DataType == DataType.Void)
                                    {
                                        nextArgument = stack.Pop();
                                        nextArgument = new Expression(Instruction.AssignmentRef, currentAddress, stack.Pop(), nextArgument);
                                        functionArguments--;
                                        currentArgument--;
                                    }
                                    else
                                    {
                                        nextArgument = stack.Pop();
                                    }
                                }
                                else
                                {
                                    nextArgument = stack.Pop();
                                }
                                if (argument == null)
                                {
                                    argument = nextArgument;
                                }
                                else
                                {
                                    argument = new Expression(Instruction.Comma, currentAddress, nextArgument, argument);
                                }
                            }
                            var delegateVariable = stack.Pop();
                            stack.Push(new Expression(instruction, currentAddress, argument, delegateVariable, word1));

                            //is this followed by this pattern:
                            //label: UNKNOWN_F4 word1, label2
                            // JUMP label
                            //label2:
                            //Then skip until Label2.

                            var instructionInfo2 = Peek(address);
                            var instructionInfo3 = Peek(instructionInfo2.nextAddress);
                            if (instructionInfo2.instruction == Instruction.DG_CALL && instructionInfo3.instruction == Instruction.JUMP &&
                                instructionInfo3.word1 == address && instructionInfo2.word2 == instructionInfo3.nextAddress)
                            {
                                address = instructionInfo3.nextAddress;
                            }
                        }
                        break;
                    case Instruction.S_GETPART:
                        {
                            var B = stack.Pop();
                            var A = stack.Pop();
                            var str = stack.Pop();

                            stack.Push(new Expression(instruction, currentAddress, str, A, B));
                        }
                        break;
                    case Instruction.S_FIND:
                        {
                            var lookFor = stack.Pop();
                            var str = stack.PopReference(currentAddress);
                            stack.Push(new Expression(instruction, currentAddress, str, lookFor));
                        }
                        break;
                    case Instruction.A_REALLOC:
                    case Instruction.A_ALLOC:
                        {
                            var dimensionCountStackValue = stack.Pop();
                            int dimensionCount = 1;
                            if (dimensionCountStackValue.ExpressionType == Instruction.PUSH)
                            {
                                dimensionCount = dimensionCountStackValue.Value;
                            }
                            List<Expression> dimensionList = new List<Expression>();
                            int dimensionsRemaining = dimensionCount;
                            while (dimensionsRemaining > 0)
                            {
                                dimensionList.Add(stack.Pop());
                                dimensionsRemaining--;
                            }
                            var array = stack.PopReference(currentAddress);
                            dimensionList.Add(array);
                            dimensionList.Reverse();
                            var newExpression = new Expression(instruction, currentAddress, dimensionList.ToArray());
                            newExpression.Value = dimensionCount;
                            stack.Push(newExpression);
                            statements.Add(stack.Pop());
                        }
                        break;
                    case Instruction.ASSERT:
                        {
                            var D = stack.Pop();
                            var C = stack.Pop();
                            var B = stack.Pop();
                            var A = stack.Pop();
                            stack.Push(new Expression(instruction, currentAddress, A, B, C, D));
                            statements.Add(stack.Pop());
                        }
                        break;
                    case Instruction.SWAP:
                        {
                            var B = stack.Pop();
                            var A = stack.Pop();
                            stack.Push(B);
                            stack.Push(A);
                        }
                        break;
                    case Instruction.S_LENGTH:
                    case Instruction.S_LENGTHBYTE:
                        {
                            var str = stack.PopReference(currentAddress);
                            var variable = str.Variable;
                            if (variable != null && !((variable.DataType == DataType.String || variable.DataType == DataType.RefString) && variable.ArrayDimensions == 0))
                            {
                                str = new Expression(Instruction.AssignmentRef, currentAddress, stack.Pop(), str);
                            }
                            //var str = stack.PopReference(currentAddress);


                            stack.Push(new Expression(instruction, currentAddress, str));
                        }
                        break;
                    case Instruction.S_LENGTH2:
                    case Instruction.S_LENGTHBYTE2:
                        {
                            var str = stack.Pop();
                            stack.Push(new Expression(instruction, currentAddress, str));
                        }
                        break;
                    case Instruction.A_PUSHBACK:
                        {
                            var value = stack.Pop();
                            var array = stack.PopReference(currentAddress);
                            stack.Push(new Expression(instruction, currentAddress, array, value));
                            statements.Add(stack.Pop());
                        }
                        break;
                    case Instruction.A_POPBACK:
                        {
                            var array = stack.PopReference(currentAddress);
                            stack.Push(new Expression(instruction, currentAddress, array));
                            statements.Add(stack.Pop());
                        }
                        break;
                    case Instruction.I_STRING:
                        {
                            var number = stack.Pop();
                            stack.Push(new Expression(instruction, currentAddress, number));
                        }
                        break;
                    case Instruction.A_EMPTY:
                        {
                            var array = stack.PopReference(currentAddress);
                            stack.Push(new Expression(instruction, currentAddress, array));
                        }
                        break;
                    case Instruction.SP_INC:
                        {
                            stack.Pop();
                            Expression expression = null;
                            if (stack.Count > 0)
                            {
                                expression = stack.Peek();
                                var nextInstruction = Peek(address);
                                if (nextInstruction.instruction == Instruction.POP || (nextInstruction.instruction == Instruction.CALLSYS && nextInstruction.word1 == 4))
                                {
                                    var A = stack.Pop();
                                    A.removeIfVoid = true;
                                    stack.Push(A);
                                }
                                else
                                {

                                }
                            }
                        }
                        break;
                    case Instruction.SR_ASSIGN:
                        {
                            var intValue = stack.Pop();
                            var valueToAssign = stack.Pop();
                            var assignToThis = stack.Pop();

                            //var assignmentRef = new Expression(Instruction.StructAssignmentRef, currentAddress, assignToThis, intValue);

                            stack.Push(new Expression(instruction, currentAddress, assignToThis, valueToAssign, intValue));
                        }
                        break;
                    case Instruction.C_ASSIGN:
                        {
                            var characterValue = stack.Pop();
                            var characterIndex = stack.Pop();
                            var stringReference = stack.PopReference(currentAddress);
                            stack.Push(new Expression(instruction, currentAddress, stringReference, characterIndex, characterValue));
                        }
                        break;
                    case Instruction.C_REF:
                        {
                            var index = stack.Pop();
                            var str = stack.Pop();
                            stack.Push(new Expression(instruction, currentAddress, str, index));
                        }
                        break;
                    case Instruction.DG_POP:
                    case Instruction.S_POP:
                    case Instruction.SR_POP:
                    case Instruction.POP:
                        {
                            var A = stack.Pop();
                            //later remove from statements if 'removeifvoid' is set
                            statements.Add(A);
                        }
                        break;
                    case Instruction.A_NUMOF:
                        {
                            var dummy = stack.Pop();
                            var B = stack.Pop();
                            var A = stack.Pop();
                            var reference = new Expression(Instruction.AssignmentRef, currentAddress, A, B);
                            stack.Push(new Expression(instruction, currentAddress, reference, dummy));
                        }
                        break;
                    case Instruction.S_MOD:
                        {
                            var dummy = stack.Pop();
                            var A = stack.Pop();
                            var B = stack.Pop();
                            stack.Push(new Expression(instruction, currentAddress, B, A, dummy));
                            //S_mod takes in an extra parameter for some reason, value of 4
                        }
                        break;
                    case Instruction.DUP2:
                        {
                            Expression B, A = null;
                            Expression aClone = null, bClone;
                            if (stack.Count < 2)
                            {
                                if (stack.Count > 0)
                                {
                                    B = stack.Pop();
                                    bClone = B.Clone();
                                    B.removeIfVoid = true;
                                    bClone.removeIfVoid = true;
                                }
                                else
                                {
                                    B = statements.Pop() ?? Expression.Empty.Clone();
                                    bClone = B.Clone();
                                }
                                if (B.ExpressionType != Instruction.REFREF)
                                {
                                    if (stack.Count > 0)
                                    {
                                        A = stack.Pop();
                                        aClone = A.Clone();
                                        A.removeIfVoid = true;
                                        aClone.removeIfVoid = true;
                                    }
                                }
                                else
                                {
                                    A = B.Clone();
                                    aClone = A.Clone();
                                    A.removeIfVoid = true;
                                    aClone.removeIfVoid = true;
                                }
                            }
                            else
                            {
                                B = stack.Pop();
                                bClone = B.Clone();
                                if (B.ExpressionType != Instruction.REFREF)
                                {
                                    A = stack.Pop();
                                    aClone = A.Clone();
                                    A.removeIfVoid = true;
                                    aClone.removeIfVoid = true;
                                }
                                else
                                {
                                    A = B.Clone();
                                    aClone = A.Clone();
                                    A.removeIfVoid = true;
                                    aClone.removeIfVoid = true;
                                }

                                B.removeIfVoid = true;
                                bClone.removeIfVoid = true;
                            }
                            stack.Push(aClone);
                            stack.Push(bClone);
                            stack.Push(A);
                            stack.Push(B);
                        }
                        break;
                    case Instruction.DUP2_X1:
                        {
                            var C = stack.Pop();
                            var B = stack.Pop();
                            var A = stack.Pop();

                            //A.removeIfVoid = true;
                            //B.removeIfVoid = true;
                            //C.removeIfVoid = true;

                            var bClone = B.Clone();
                            var cClone = C.Clone();
                            stack.Push(B.Clone());
                            stack.Push(C.Clone());
                            stack.Push(A);
                            B.removeIfVoid = true;
                            C.removeIfVoid = true;
                            stack.Push(B);
                            stack.Push(C);
                        }
                        break;
                    case Instruction.DUP_X2:
                        {
                            var C = stack.Pop();
                            var B = stack.Pop();
                            var A = stack.Pop();

                            //A.removeIfVoid = true;
                            //B.removeIfVoid = true;

                            stack.Push(C.Clone());
                            if (Peek(instructionInfo.nextAddress).instruction != Instruction.POP)
                            {
                                C.removeIfVoid = true;
                            }
                            stack.Push(A);
                            stack.Push(B);
                            stack.Push(C);
                        }
                        break;
                    case Instruction.DUP:
                        {
                            var A = stack.Pop();
                            //A.removeIfVoid = true;
                            stack.Push(A.Clone());
                            stack.Push(A);
                        }
                        break;
                    case Instruction.CALLFUNC:
                    case Instruction.CALLMETHOD:
                        {
                            var info1 = Peek(currentAddress);

                            var function = this.ainFile.Functions[word1];
                            int functionArguments = function.ParameterCount;
                            //???
                            //if (info1.instruction == Instruction.SH_STRUCTREF_CALLMETHOD_NO_PARAM)
                            //{
                            //    functionArguments = 0;
                            //}

                            int currentArgument = functionArguments;

                            if (instruction == Instruction.CALLMETHOD)
                            {
                                functionArguments++;
                            }

                            Expression argument = null;
                            while (functionArguments > 0)
                            {
                                functionArguments--;
                                currentArgument--;
                                Expression nextArgument = null;
                                if (currentArgument >= 0)
                                {
                                    var parameter = function.Parameters[currentArgument];
                                    if (parameter.DataType == DataType.Void)
                                    {
                                        nextArgument = stack.Pop();
                                        if (nextArgument.ExpressionType != Instruction.REFREF)
                                        {
                                            nextArgument = new Expression(Instruction.AssignmentRef, currentAddress, stack.Pop(), nextArgument);
                                        }
                                        functionArguments--;
                                        currentArgument--;
                                    }
                                    else
                                    {
                                        nextArgument = stack.Pop();
                                    }
                                }
                                else
                                {
                                    nextArgument = stack.Pop();
                                }
                                if (argument == null)
                                {
                                    argument = nextArgument;
                                }
                                else
                                {
                                    argument = new Expression(Instruction.Comma, currentAddress, nextArgument, argument);
                                }
                            }
                            stack.Push(new Expression(instruction, currentAddress, argument, word1));
                            if (function.DataType == DataType.Void)
                            {
                                statements.Add(stack.Pop());
                            }
                        }
                        break;
                    case Instruction.CALLSYS:
                        {
                            int functionArguments = 0;
                            if (word1 >= 0 && word1 < AinFile.SystemCalls.Count)
                            {
                                var function = AinFile.SystemCalls[word1];
                                functionArguments = function.ParameterCount;
                                Expression argument = null;
                                while (functionArguments > 0)
                                {
                                    var nextArg = stack.Pop();
                                    if (stack.Count > 0)
                                    {
                                        var stackPeek = stack.Peek();
                                        if (stackPeek.ExpressionType == Instruction.PUSHGLOBALPAGE ||
                                            stackPeek.ExpressionType == Instruction.PUSHLOCALPAGE ||
                                            stackPeek.ExpressionType == Instruction.PUSHSTRUCTPAGE)
                                        {
                                            nextArg = new Expression(Instruction.AssignmentRef, currentAddress, stack.Pop(), nextArg);
                                        }
                                    }
                                    if (argument != null)
                                    {
                                        argument = new Expression(Instruction.Comma, currentAddress, nextArg, argument);
                                    }
                                    else
                                    {
                                        argument = nextArg;
                                    }
                                    functionArguments--;

                                }
                                stack.Push(new Expression(instruction, currentAddress, argument, word1));
                                if (function.DataType == DataType.Void)
                                {
                                    statements.Add(stack.Pop());
                                }
                            }
                            else
                            {
                                stack.Push(new Expression(instruction, currentAddress, word1));
                            }
                        }
                        break;
                    case Instruction.CALLHLL:
                        {
                            var functionInfo = this.ainFile.Libraries[word1].Functions[word2];
                            int functionArguments = functionInfo.ParameterCount;
                            Expression argument = null;
                            while (functionArguments > 0)
                            {
                                var hllArgument = functionInfo.Parameters[functionArguments - 1];
                                var nextArgument = stack.Pop();

                                if (stack.Count > 0)
                                {
                                    var stackPeek = stack.Peek();
                                    var variableType = stackPeek.Variable;
                                    if (hllArgument.DataType.IsRef())
                                    {
                                        var argumentVariable = nextArgument.Variable;
                                        bool argumentValueIsInt = argumentVariable != null && argumentVariable.DataType == DataType.Int;
                                        bool stackVariaibleIsArray = variableType != null && variableType.ArrayDimensions > 0;

                                        if (stackPeek.ExpressionType == Instruction.PUSHGLOBALPAGE ||
                                            stackPeek.ExpressionType == Instruction.PUSHLOCALPAGE ||
                                            stackPeek.ExpressionType == Instruction.PUSHSTRUCTPAGE ||
                                            (variableType != null && (variableType.DataType == DataType.Struct || variableType.DataType == DataType.RefStruct)) ||
                                            (argumentValueIsInt && stackVariaibleIsArray))
                                        {
                                            nextArgument = new Expression(Instruction.AssignmentRef, currentAddress, stack.Pop(), nextArgument);
                                        }
                                    }
                                }


                                if (argument != null)
                                {
                                    argument = new Expression(Instruction.Comma, currentAddress, nextArgument, argument);
                                }
                                else
                                {
                                    argument = nextArgument;
                                }
                                functionArguments--;
                            }
                            stack.Push(new Expression(instruction, currentAddress, argument, word1, word2));
                            if (functionInfo.DataType == DataType.Void)
                            {
                                statements.Add(stack.Pop());
                            }
                        }
                        break;
                    case Instruction.ENDFUNC:
                        {
                            goto Exit;
                        }
                        break;
                    case Instruction.IFNZ:
                        {
                            if (address >= limit && state.insideLogicalBoolean && Peek(currentAddress).instruction != Instruction.IFNZ)
                            {
                                break;
                            }

                            bool handled = false;
                            bool isStatement = false;
                            //check for logical OR
                            int target = word1;
                            if (target >= address + 22)
                            {
                                if (!handled)
                                {
                                    //do instructions match the pattern:
                                    //ifnz <target>
                                    //push 0
                                    //jmp <target+6>
                                    //target:
                                    //push 1
                                    var info1 = Peek(target - 18);
                                    var info2 = Peek(target - 12);
                                    var info3 = Peek(target - 6);
                                    var info4 = Peek(target);

                                    if (info2.instruction == Instruction.PUSH && info2.word1 == 0 &&
                                        info3.instruction == Instruction.JUMP && info3.word1 == target + 6 &&
                                        info4.instruction == Instruction.PUSH && info4.word1 == 1)
                                    {
                                        bool matched = IsMinus18BranchToTarget(target, ref info1);

                                        if (matched)
                                        {
                                            var rightTreeState = state.Clone(limit);
                                            rightTreeState.insideLogicalBoolean = true;
                                            var rightTree = DecompileRange(address, target - 18, rightTreeState);
                                            var rightExpression = rightTree;
                                            if (rightExpression.ExpressionType == Instruction.IfElse)
                                            {
                                                rightExpression = rightTree.Arg1;
                                                rightTree.Arg1 = null;
                                            }
                                            if (info1.instruction != Instruction.IFNZ)
                                            {
                                                //negate rightExpression
                                                Expression.Negate(ref rightExpression);
                                            }

                                            stack.Push(new Expression(Instruction.LogicalOr, currentAddress, stack.Pop(), rightExpression));
                                            address = target + 6;
                                            handled = true;
                                        }
                                    }
                                    //Check for negated logical OR
                                    //do instructions match the pattern:
                                    //ifnz <target>
                                    //push 1
                                    //jmp <target+6>
                                    //target:
                                    //push 0
                                    else if (info2.instruction == Instruction.PUSH && info2.word1 == 1 &&
                                        info3.instruction == Instruction.JUMP && info3.word1 == target + 6 &&
                                        info4.instruction == Instruction.PUSH && info4.word1 == 0)
                                    {
                                        bool matched = IsMinus18BranchToTarget(target, ref info1);
                                        if (matched)
                                        {
                                            var rightTreeState = state.Clone(limit);
                                            rightTreeState.insideLogicalBoolean = true;
                                            var rightTree = DecompileRange(address, target - 18, rightTreeState);
                                            var rightExpression = rightTree;
                                            if (rightExpression.ExpressionType == Instruction.IfElse)
                                            {
                                                rightExpression = rightTree.Arg1;
                                                rightTree.Arg1 = null;
                                            }
                                            if (info1.instruction != Instruction.IFNZ)
                                            {
                                                //negate rightExpression
                                                Expression.Negate(ref rightExpression);
                                            }

                                            var logicalOrOperation = new Expression(Instruction.LogicalOr, currentAddress, stack.Pop(), rightExpression);
                                            Expression.Negate(ref logicalOrOperation);
                                            stack.Push(logicalOrOperation);
                                            address = target + 6;
                                            handled = true;
                                        }
                                    }
                                }
                            }
                            if (!handled && target >= address + 6 && stack.Count > 0)
                            {
                                //check for negated while loop  (alice2010)
                                //back:
                                //(condition)
                                //IFNZ target
                                //...
                                //JUMP back
                                //target:
                                var stackPeek = stack.Peek();
                                var info1 = Peek(target - 6);
                                if (info1.instruction == Instruction.JUMP && info1.word1 == stackPeek.LowestAddress)
                                {
                                    //It's a negated while loop
                                    var condition = stack.Pop();
                                    Expression.Negate(ref condition);

                                    var loopBodyState = state.Clone(target);
                                    loopBodyState.breakAddress = target;
                                    loopBodyState.continueAddress = info1.word1;

                                    var loopBody = DecompileRange(address, target - 6, loopBodyState);
                                    stack.Push(new Expression(Instruction.WhileLoop, currentAddress, condition, loopBody));
                                    address = target;
                                    handled = true;
                                    isStatement = true;
                                }

                            }

                            if (!handled)
                            {
                                //check for Negated IF  (alice2010)
                                var ifTreeState = state.Clone(limit);
                                ifTreeState.insideIfExpression = true;
                                var ifTree = DecompileRange(address, target, ifTreeState);
                                int leftFinalAddress = ifTreeState.finalAddress;
                                var info1 = Peek(leftFinalAddress);
                                if (leftFinalAddress <= target - 6 && leftFinalAddress >= address &&
                                    info1.instruction == Instruction.JUMP && info1.word1 >= target && info1.word1 <= state.parentLimitAddress &&
                                    info1.word1 != state.breakAddress && info1.word1 != state.continueAddress)
                                {
                                    var info2 = Peek(info1.word1 - 6);
                                    if (!(info2.instruction == Instruction.JUMP && info2.word1 <= info2.CurrentAddress && info2.word1 != state.continueAddress))
                                    {
                                        var elseTreeState = state.Clone(limit);
                                        elseTreeState.insideIfExpression = true;
                                        elseTreeState.isElse = true;
                                        var elseTree = DecompileRange(target, info1.word1, elseTreeState);
                                        var condition = stack.Pop();
                                        Expression.Negate(ref condition);
                                        stack.Push(new Expression(Instruction.IfElse, currentAddress, condition, ifTree));
                                        address = info1.word1;
                                        handled = true;
                                        isStatement = true;
                                        if (ifTreeState.isStackExpression || elseTreeState.isStackExpression)
                                        {
                                            isStatement = false;
                                        }
                                    }
                                }
                                if (!handled && leftFinalAddress <= target - 2 && leftFinalAddress >= address &&
                                    target == ifTreeState.exitAddress)
                                {
                                    ifTreeState = state.Clone(target);
                                    ifTreeState.insideIfExpression = true;
                                    ifTree = DecompileRange(address, target, ifTreeState);
                                    var condition = stack.Pop();
                                    Expression.Negate(ref condition);
                                    stack.Push(new Expression(Instruction.IfElse, currentAddress, condition, ifTree, null));
                                    address = ifTreeState.exitAddress;
                                    handled = true;
                                    isStatement = true;
                                    if (ifTreeState.isStackExpression)
                                    {
                                        isStatement = false;
                                    }
                                }
                                else if (!handled)
                                {

                                }
                            }
                            if (!handled)
                            {
                                stack.Push(new Expression(instruction, currentAddress, stack.Pop(), word1));
                            }
                            else
                            {
                                if (isStatement)
                                {
                                    statements.Add(stack.Pop());
                                }
                            }
                        }
                        break;
                    case Instruction.IFZ:
                        {
                            if (address >= limit && state.insideLogicalBoolean && Peek(currentAddress).instruction != Instruction.IFZ)
                            {
                                break;
                            }

                            bool handled = false;
                            bool isStatement = false;
                            int target = word1;
                            //check for logical AND
                            if (!handled && target >= address + 22)
                            {
                                //do instructions match the pattern:
                                //ifz <target>
                                //push 1
                                //jmp <target+6>
                                //target:
                                //push 0
                                var info1 = Peek(target - 18);
                                var info2 = Peek(target - 12);
                                var info3 = Peek(target - 6);
                                var info4 = Peek(target);
                                if (info2.instruction == Instruction.PUSH && info2.word1 == 1 &&
                                    info3.instruction == Instruction.JUMP && info3.word1 == target + 6 &&
                                    info4.instruction == Instruction.PUSH && info4.word1 == 0)
                                {
                                    bool matched = IsMinus18BranchToTarget(target, ref info1);

                                    if (matched)
                                    {
                                        var rightTreeState = state.Clone(limit);
                                        rightTreeState.insideLogicalBoolean = true;
                                        var rightTree = DecompileRange(address, target - 18, rightTreeState);
                                        var rightExpression = rightTree;
                                        if (rightExpression.ExpressionType == Instruction.IfElse)
                                        {
                                            rightExpression = rightTree.Arg1;
                                            rightTree.Arg1 = null;
                                        }

                                        stack.Push(new Expression(Instruction.LogicalAnd, currentAddress, stack.Pop(), rightExpression));
                                        address = target + 6;
                                        handled = true;
                                    }
                                }
                            }
                            //check for logical OR (shorthand instructions cause this)
                            if (!handled && target >= address + 22)
                            {
                                //do instructions match the pattern:
                                //ifnz <target>
                                //push 0
                                //jmp <target+6>
                                //target:
                                //push 1
                                var info1 = Peek(target - 18);
                                var info2 = Peek(target - 12);
                                var info3 = Peek(target - 6);
                                var info4 = Peek(target);
                                if (info2.instruction == Instruction.PUSH && info2.word1 == 0 &&
                                    info3.instruction == Instruction.JUMP && info3.word1 == target + 6 &&
                                    info4.instruction == Instruction.PUSH && info4.word1 == 1)
                                {
                                    bool matched = IsMinus18BranchToTarget(target, ref info1);
                                    if (matched)
                                    {
                                        var rightTreeState = state.Clone(limit);
                                        rightTreeState.insideLogicalBoolean = true;
                                        var rightTree = DecompileRange(address, target - 18, rightTreeState);
                                        var leftTree = stack.Pop();
                                        Expression.Negate(ref leftTree);
                                        var rightExpression = rightTree;
                                        if (rightExpression.ExpressionType == Instruction.IfElse)
                                        {
                                            rightExpression = rightTree.Arg1;
                                            rightTree.Arg1 = null;
                                        }
                                        if (info1.instruction != Instruction.IFNZ)
                                        {
                                            Expression.Negate(ref rightExpression);
                                        }


                                        stack.Push(new Expression(Instruction.LogicalOr, currentAddress, leftTree, rightTree));
                                        address = target + 6;
                                        handled = true;
                                    }
                                }
                            }
                            //check for FOR
                            if (!handled && target >= address + 18)
                            {
                                //Init: ...
                                //Cond: ...
                                //IFZ After  <- we are here
                                //JUMP Body
                                //Inc: ...
                                // JUMP Cond
                                //Body: ... 
                                // JUMP Inc
                                //After:
                                int addressOfJumpToIncrement = target - 6;
                                int addressOfJumpToBody = address;
                                int addressOfJumpToCondition;

                                int conditionAddress;
                                int incrementAddress = addressOfJumpToBody + 6;
                                int bodyAddress;

                                var jumpToIncrement = Peek(addressOfJumpToIncrement);
                                var jumpToBody = Peek(addressOfJumpToBody);

                                if (jumpToIncrement.instruction == Instruction.JUMP && jumpToIncrement.word1 == incrementAddress && jumpToBody.instruction == Instruction.JUMP)
                                {
                                    bodyAddress = jumpToBody.word1;
                                    if (bodyAddress >= addressOfJumpToBody + 12 && bodyAddress < addressOfJumpToIncrement)
                                    {
                                        addressOfJumpToCondition = bodyAddress - 6;

                                        var jumpToCondition = Peek(addressOfJumpToCondition);
                                        if (jumpToCondition.instruction == Instruction.JUMP)
                                        {
                                            conditionAddress = jumpToCondition.word1;

                                            //check addresses
                                            if (conditionAddress <= addressOfJumpToBody - 6)
                                            {
                                                var condition = stack.Pop();
                                                Expression initial = null;
                                                if (statements.Count > 0)
                                                {
                                                    initial = statements[statements.Count - 1];
                                                    if (Expression.IsAssignment(initial.ExpressionType) || initial.ExpressionType == Instruction.SH_LOCALASSIGN)
                                                    {
                                                        statements.RemoveAt(statements.Count - 1);
                                                    }
                                                    else
                                                    {
                                                        initial = null;
                                                    }
                                                }

                                                //if (initial.ExpressionType == Instruction.ASSIGN || initial.ExpressionType == Instruction.SH_LOCALASSIGN)
                                                //{
                                                //    initial = stack.Pop();
                                                //}
                                                //else
                                                //{
                                                //    initial = null;
                                                //}
                                                var incrementState = state.Clone(limit);
                                                var increment = DecompileRange(incrementAddress, addressOfJumpToCondition, incrementState);
                                                var bodyState = state.Clone(limit);
                                                bodyState.breakAddress = target;
                                                bodyState.continueAddress = incrementAddress;
                                                var body = DecompileRange(bodyAddress, addressOfJumpToIncrement, bodyState);
                                                stack.Push(new Expression(Instruction.ForLoop, currentAddress, initial, condition, increment, body));

                                                address = target;
                                                handled = true;
                                                isStatement = true;
                                            }
                                        }
                                    }
                                }
                            }
                            //check for a WHILE loop
                            if (!handled && target >= address + 6 && stack.Count > 0)
                            {
                                //do instructions match the pattern:

                                //<first>
                                //ifz <target>
                                //...
                                //jump <before first>
                                //<target>:
                                var stackPeek = stack.Peek();
                                var info1 = Peek(target - 6);
                                if (info1.instruction == Instruction.JUMP && info1.word1 < currentAddress && info1.word1 == stackPeek.LowestAddress)
                                {
                                    var condition = stack.Pop();

                                    var loopBodyState = state.Clone(target);
                                    loopBodyState.breakAddress = target;
                                    loopBodyState.continueAddress = info1.word1;

                                    var loopBody = DecompileRange(address, target - 6, loopBodyState);
                                    stack.Push(new Expression(Instruction.WhileLoop, currentAddress, condition, loopBody));
                                    address = target;
                                    handled = true;
                                    isStatement = true;
                                }
                            }
                            if (!handled && target >= address)
                            {
                                var ifTreeState = state.Clone(limit);
                                ifTreeState.insideIfExpression = true;
                                var ifTree = DecompileRange(address, target, ifTreeState);
                                int leftFinalAddress = ifTreeState.finalAddress;
                                var info1 = Peek(leftFinalAddress);
                                //check for formal IF else block that uses jumps
                                //(conditon)
                                //IFZ elseBlock
                                //...
                                //JUMP endIf
                                //elseBlock:
                                //...
                                //endIf:
                                if (!handled && target >= address + 6)
                                {
                                    if (leftFinalAddress <= target - 6 && leftFinalAddress >= address && /* ifTreeState.exitAddress == target && */
                                        info1.instruction == Instruction.JUMP && info1.word1 >= target /*&&*/
                                        /*info1.word1 != state.breakAddress && info1.word1 != state.continueAddress*/)
                                    {
                                        var info2 = Peek(info1.word1 - 6);
                                        if (!(info2.instruction == Instruction.JUMP &&
                                            info2.word1 <= info2.CurrentAddress &&
                                            info2.word2 >= leftFinalAddress &&   //???  is this the right condition?
                                            info2.word1 != state.continueAddress))
                                        {
                                            var elseTreeState = state.Clone(limit);
                                            elseTreeState.insideIfExpression = true;
                                            elseTreeState.isElse = true;
                                            var elseTree = DecompileRange(target, info1.word1, elseTreeState);
                                            var condition = stack.Pop();
                                            stack.Push(new Expression(Instruction.IfElse, currentAddress, condition, ifTree, elseTree));
                                            address = info1.word1;
                                            handled = true;
                                            isStatement = true;
                                            if (ifTreeState.isStackExpression || elseTreeState.isStackExpression)
                                            {
                                                isStatement = false;
                                            }
                                        }
                                    }
                                }
                                //check for other kind of IF block that uses a single branch (for alice2010)
                                if (!handled && target >= address)
                                {
                                    ifTreeState = state.Clone(target);
                                    ifTreeState.insideIfExpression = true;
                                    ifTree = DecompileRange(address, target, ifTreeState);
                                    leftFinalAddress = ifTreeState.finalAddress;
                                    info1 = Peek(leftFinalAddress);

                                    if (leftFinalAddress >= address && ifTreeState.exitAddress == target)
                                    {
                                        var condition = stack.Pop();
                                        stack.Push(new Expression(Instruction.IfElse, currentAddress, condition, ifTree, null));
                                        address = ifTreeState.exitAddress;
                                        handled = true;
                                        isStatement = true;
                                        if (ifTreeState.isStackExpression)
                                        {
                                            isStatement = false;
                                        }
                                    }
                                    else
                                    {
                                        //check for 
                                    }
                                }
                            }
                            if (!handled)
                            {
                                var branch = new Expression(instruction, currentAddress, stack.Pop(), word1);
                                statements.Add(branch);
                                jumpList.Add(branch);
                            }
                            else
                            {
                                if (isStatement)
                                {
                                    statements.Add(stack.Pop());
                                    statements.Add(new Expression(Instruction.Nop, address));
                                }
                            }
                        }
                        break;
                    case Instruction.JUMP:
                        {
                            int target = word1;
                            bool handled = false;
                            bool statement = false;

                            //check for Break or Continue
                            if (!handled && !processingWholeFunction)
                            {
                                if (state.insideIfExpression && currentAddress == state.finalAddress && target == state.finalAddress + 6)
                                {

                                }
                                else
                                {
                                    if (target == state.breakAddress)
                                    {
                                        handled = true;
                                        statements.Add(new Expression(Instruction.Break, currentAddress));
                                        //if (!state.dontFollowJumps)
                                        //{
                                        //    goto Exit;
                                        //}
                                    }
                                    else if (target == state.continueAddress)
                                    {
                                        handled = true;
                                        statements.Add(new Expression(Instruction.Continue, currentAddress));
                                        //if (!state.dontFollowJumps)
                                        //{
                                        //    goto Exit;
                                        //}
                                    }
                                }
                            }

                            //check for jump back to a known statement in the same scope (empty FOR loop)
                            if (!handled && target < currentAddress)
                            {
                                int indexOfPreviousStatement = -1;
                                for (int i = 0; i < statements.Count; i++)
                                {
                                    if (statements[i].LowestAddress == target)
                                    {
                                        indexOfPreviousStatement = i;
                                        break;
                                    }
                                }

                                //check for weird FOR loop from Daibanchou
                                if (target == instructionInfo.CurrentAddress - 6)
                                {
                                    var previousInstruction = Peek(instructionInfo.CurrentAddress - 6);
                                    if (previousInstruction.instruction == Instruction.JUMP && previousInstruction.word1 == instructionInfo.nextAddress)
                                    {
                                        indexOfPreviousStatement = -1;
                                    }
                                }

                                //if (handled)
                                //{
                                //    statements.Add(new Expression(Instruction.Nop, instructionInfo.CurrentAddress));
                                //    break;
                                //}

                                //proceed
                                if (indexOfPreviousStatement != -1)
                                {
                                    DrainStack(stack, statements);
                                    statements.Add(new Expression(Instruction.Nop, currentAddress));
                                    var body = ConvertToStatements(statements, indexOfPreviousStatement, statements.Count);
                                    statements.RemoveRange(indexOfPreviousStatement, statements.Count - indexOfPreviousStatement);

                                    int newAddress = currentAddress;
                                    if (body != null)
                                    {
                                        newAddress = body.address;
                                    }
                                    statements.Add(new Expression(Instruction.ForLoop, body.address, null, null, null, body));
                                    statements.Add(new Expression(Instruction.Nop, address));
                                    handled = true;
                                }
                                else
                                {
                                    var newJump = new Expression(instruction, currentAddress, target);
                                    jumpList.Add(newJump);
                                    statements.Add(newJump);
                                    handled = true;

                                    ////Make a GOTO to another scope  TODO
                                    //int labelNumber = AddLabelAtAddress(word1, state);
                                    //if (labelNumber != -1)
                                    //{
                                    //    var newGoto = new Expression(Instruction.Goto, currentAddress, AddLabelAtAddress(word1, state));
                                    //    gotoList.Add(newGoto);
                                    //    statements.Add(newGoto);
                                    //}
                                    //else
                                    //{

                                    //}
                                }
                            }
                            if (!handled)
                            {
                                if ((state.insideIfExpression && !state.isElse) && address <= limit - 6)
                                {
                                    var newJump = new Expression(instruction, currentAddress, target);
                                    jumpList.Add(newJump);
                                    statements.Add(newJump);
                                    handled = true;
                                }
                                else if (!(state.insideIfExpression && !state.isElse))
                                {
                                    var newJump = new Expression(instruction, currentAddress, target);
                                    jumpList.Add(newJump);
                                    statements.Add(newJump);
                                    handled = true;
                                }
                                else if (state.insideIfExpression && !state.isElse && target > state.parentLimitAddress)
                                {
                                    var newJump = new Expression(instruction, currentAddress, target);
                                    jumpList.Add(newJump);
                                    statements.Add(newJump);
                                    handled = true;
                                }
                                else
                                {

                                }
                            }
                            ////follow the jump
                            //if (!handled && !processingWholeFunction)
                            //{
                            //    if (infiniteLoopTest.Contains(address))
                            //    {
                            //        handled = true;
                            //        goto Exit;
                            //    }
                            //    if (!state.dontFollowJumps)
                            //    {
                            //        infiniteLoopTest.Add(address);
                            //        handled = true;
                            //        push = true;
                            //        address = word1;
                            //    }
                            //}

                            if (statement)
                            {
                                stack.Push(new Expression(instruction, currentAddress, word1));
                            }
                        }
                        break;
                    case Instruction.FUNC:
                        {
                            processingWholeFunction = true;
                            functionId = word1;
                            state.functionId = functionId;
                            displayer.currentFuncNumber = functionId;
                            Expression.defaultFunctionNumber = functionId;
                        }
                        break;
                    case Instruction.SWITCH:
                    case Instruction.STRSWITCH:
                        {
                            var condition = stack.Pop();
                            var switchInfo = ainFile.Switches[word1];
                            var nextJump = Peek(address);
                            int nextJumpAddress = nextJump.word1;

                            //new code: decompile the entire block in one pass then add case labels.
                            var switchBlockState = state.Clone(limit);
                            switchBlockState.breakAddress = nextJumpAddress;
                            switchBlockState.insideIfExpression = false;
                            switchBlockState.breakOnReturn = false;
                            int switchBlockAddress = nextJump.nextAddress;
                            var switchBlockCode = DecompileRange(switchBlockAddress, nextJumpAddress, switchBlockState);

                            AddCaseLabels(switchBlockCode, switchInfo);
                            switchBlockCode = switchBlockCode.Root;
                            stack.Push(new Expression(instruction, currentAddress, condition, switchBlockCode));



                            ////build list of switch cases
                            //List<Expression> cases = new List<Expression>();
                            //Expression defaultSwitchCase = null;
                            //if (switchInfo.DefaultCaseAddress != -1)
                            //{
                            //    defaultSwitchCase = new Expression(Instruction.DefaultSwitchCase, switchInfo.DefaultCaseAddress, int.MaxValue);
                            //    cases.Add(defaultSwitchCase);
                            //}
                            //foreach (var switchCase in switchInfo.SwitchCases)
                            //{
                            //    var caseExpression = new Expression(Instruction.SwitchCase, switchCase.TargetAddress, switchCase.Value);
                            //    cases.Add(caseExpression);
                            //}

                            ////sort list by address
                            //cases = SortCasesByAddress(cases);

                            ////get bodies of cases
                            //for (int i = 0; i < cases.Count; i++)
                            //{
                            //    var switchCase = cases[i];
                            //    int caseStartAddress = switchCase.address;
                            //    int caseEndAddress;
                            //    if (i < cases.Count - 1)
                            //    {
                            //        //set link to next case
                            //        var nextCase = cases[i + 1];
                            //        switchCase.Arg2 = nextCase;
                            //        //get end address
                            //        caseEndAddress = nextCase.address;
                            //    }
                            //    else
                            //    {
                            //        caseEndAddress = nextJumpAddress;
                            //    }
                            //    var caseBlockState = state.Clone(limit);
                            //    caseBlockState.breakAddress = nextJumpAddress;
                            //    caseBlockState.breakOnReturn = false;  //?? do we need this anymore?
                            //    caseBlockState.dontFollowJumps = true;
                            //    var caseBlock = DecompileRange(caseStartAddress, caseEndAddress, caseBlockState);
                            //    switchCase.Arg1 = caseBlock;
                            //}

                            //Expression firstCase = cases.FirstOrDefault();

                            //stack.Push(new Expression(instruction, currentAddress, condition, firstCase));
                            address = nextJumpAddress;
                            statements.Add(stack.Pop());
                        }
                        break;
                    case Instruction.RETURN:
                        {
                            //what's my function return type
                            if (ainFile.Functions[functionId].DataType == DataType.Void)
                            {
                                stack.Push(new Expression(instruction, currentAddress));
                            }
                            else
                            {
                                var returnThis = stack.Pop();
                                stack.Push(new Expression(instruction, currentAddress, returnThis));
                            }
                            statements.Add(stack.Pop());

                            if (state.breakOnReturn)
                            {
                                goto Exit;
                            }
                        }
                        break;
                    case Instruction.SR_REF2:
                        {
                            var stackPeek = stack.Peek();
                            Expression reference = null;
                            if (stackPeek.ExpressionType == Instruction.REF || stackPeek.ExpressionType == Instruction.REFREF ||
                                Expression.IsAssignment(stackPeek.ExpressionType) || stackPeek.ExpressionType == Instruction.PUSHSTRUCTPAGE)
                            {
                                reference = stack.Pop();
                                //is this an array index?
                                if (reference.ExpressionType == Instruction.REF && stack.Count > 0)
                                {
                                    var stackPeek2 = stack.Peek();
                                    var variable = stackPeek2.Variable;
                                    if (variable != null && variable.ArrayDimensions > 0)
                                    {
                                        reference = new Expression(Instruction.AssignmentRef, currentAddress, stack.Pop(), reference);
                                    }
                                }
                            }
                            else
                            {
                                var B = stack.Pop();
                                var A = stack.Pop();
                                reference = new Expression(Instruction.AssignmentRef, currentAddress, A, B);
                            }

                            //var reference = stack.PopReference(currentAddress);
                            stack.Push(new Expression(instruction, currentAddress, reference, word1));

                        }
                        break;
                    case Instruction.SR_REF:
                        {
                            // pop variable, pop struct member index, word1 = struct type
                            var stackPeek = stack.Peek();
                            Expression reference = null;
                            if (stackPeek.ExpressionType == Instruction.REF || stackPeek.ExpressionType == Instruction.REFREF || Expression.IsAssignment(stackPeek.ExpressionType))
                            {
                                reference = stack.Pop();
                                //is this an array index?
                                if (reference.ExpressionType == Instruction.REF && stack.Count > 0)
                                {
                                    var stackPeek2 = stack.Peek();
                                    var variable = stackPeek2.Variable;
                                    if (variable != null && variable.ArrayDimensions > 0)
                                    {
                                        reference = new Expression(Instruction.AssignmentRef, currentAddress, stack.Pop(), reference);
                                    }
                                }
                            }
                            else
                            {
                                var B = stack.Pop();
                                var A = stack.Pop();
                                reference = new Expression(Instruction.AssignmentRef, currentAddress, A, B);
                            }

                            //var reference = stack.PopReference(currentAddress);
                            stack.Push(new Expression(instruction, currentAddress, reference, word1));

                        }
                        break;
                    case Instruction.A_FREE:
                        {
                            var array = stack.PopReference(currentAddress);
                            stack.Push(new Expression(instruction, currentAddress, array));
                            statements.Add(stack.Pop());
                        }
                        break;
                    case Instruction.A_COPY:
                        {
                            var arg4 = stack.Pop();
                            var arg3 = stack.Pop();
                            var arg2 = stack.Pop();
                            var arg1 = stack.Pop();
                            var array = stack.PopReference(currentAddress);

                            stack.Push(new Expression(instruction, currentAddress, array,
                                new Expression(Instruction.Comma, currentAddress, arg1,
                                    new Expression(Instruction.Comma, currentAddress, arg2,
                                        new Expression(Instruction.Comma, currentAddress, arg3, arg4)))));
                            //statements.Add(stack.Pop());
                        }
                        break;
                    case Instruction.DELETE:
                        {
                            var a = stack.PopReference(currentAddress); //???  or just Pop?
                            stack.Push(new Expression(instruction, currentAddress, a));
                            statements.Add(stack.Pop());
                        }
                        break;
                    case Instruction.SJUMP:
                        {
                            var a = stack.Pop();
                            statements.Add(new Expression(instruction, currentAddress, a));
                        }
                        break;
                    case Instruction.A_SORT_MEM:
                        {
                            //int, reference
                            var C = stack.Pop();
                            var B = stack.Pop();
                            var A = stack.Pop();
                            var reference = new Expression(Instruction.AssignmentRef, currentAddress, A, B);
                            stack.Push(new Expression(instruction, currentAddress, reference, C));
                            statements.Add(stack.Pop());
                        }
                        break;
                    default:
                        {
                            instructionHandledBySwitchBlock = false;
                        }
                        break;
                }

                if (!instructionHandledBySwitchBlock)
                {
                    if (Expression.IsAssignment(instruction))
                    {
                        if (Peek(currentAddress).instruction == Instruction.S_ASSIGN)
                        {

                        }
                        var value = stack.Pop();
                        Expression assignmentRef = stack.PopReference(currentAddress);
                        if (stack.Count > 0)
                        {
                            var indexVariable = assignmentRef.Variable;
                            var stackPeek = stack.Peek();
                            var variable = displayer.GetReferencedVariableType(stackPeek);
                            //fixme
                            if (variable != null && variable.ArrayDimensions != 0)
                            {
                                if (indexVariable != null &&
                                (indexVariable.DataType == DataType.Int || indexVariable.DataType == DataType.RefInt ||
                                indexVariable.DataType == DataType.Lint || indexVariable.DataType == DataType.RefLint))
                                {
                                    if (variable.ArrayDimensions > 1)
                                    {

                                    }

                                    assignmentRef = new Expression(Instruction.ArrayIndex, currentAddress, stack.Pop(), assignmentRef);
                                }
                                else
                                {

                                }
                            }
                        }

                        stack.Push(new Expression(instruction, currentAddress, assignmentRef, value));
                    }
                    else if (Expression.IsBinary(instruction))
                    {
                        var A = stack.Pop();
                        var B = stack.Pop();
                        stack.Push(new Expression(instruction, currentAddress, B, A));
                    }
                    else if (Expression.IsUnary(instruction))
                    {
                        stack.Push(new Expression(instruction, currentAddress, stack.Pop()));
                    }
                    else if (Expression.IsLeaf(instruction))
                    {
                        var newExpression = new Expression(instruction, currentAddress, word1);
                        var nextInstruction = Peek(address);
                        if (nextInstruction.instruction == Instruction.POP)
                        {
                            newExpression.removeIfVoid = true;
                        }
                        stack.Push(newExpression);
                    }
                    else if (Expression.IsRef(instruction))
                    {
                        var A = stack.Pop();
                        Expression B = null;
                        if (instruction == Instruction.REF &&
                            (A.ExpressionType == Instruction.INC ||
                            A.ExpressionType == Instruction.DEC ||
                            A.ExpressionType == Instruction.LI_INC ||
                            A.ExpressionType == Instruction.LI_DEC))
                        {
                            bool stackContainsArray = false;
                            if (stack.Count > 0)
                            {
                                var stackPeek = stack.Peek();
                                var variable = stackPeek.Variable;
                                if (variable != null && variable.DataType.IsArray() && variable.ArrayDimensions > 0)
                                {
                                    var nextInstruction = Peek(instructionInfo.nextAddress);
                                    if (nextInstruction.instruction != Instruction.REF)
                                    {
                                        stackContainsArray = true;
                                    }
                                }
                            }



                            if (!stackContainsArray)
                            {
                                Instruction newInstruction;
                                if (A.ExpressionType == Instruction.INC || A.ExpressionType == Instruction.LI_INC)
                                {
                                    newInstruction = Instruction.PreIncrement;
                                }
                                else
                                {
                                    newInstruction = Instruction.PreDecrement;
                                }
                                stack.Push(new Expression(newInstruction, currentAddress, A.Arg1));
                            }
                            else
                            {
                                stack.Push(new Expression(instruction, currentAddress, stack.Pop(), A));
                            }
                        }
                        else if (instruction == Instruction.REF && A.ExpressionType == Instruction.REFREF)
                        {
                            var newExpression = new Expression(instruction, currentAddress, A);
                            if (newExpression.Arg1.removeIfVoid == true)
                            {
                                newExpression.removeIfVoid = true;
                            }
                            stack.Push(newExpression);
                        }
                        else
                        {
                            bool found = false;
                            if (instruction == Instruction.A_REF)
                            {
                                var variable = A.Variable;
                                if (variable != null && variable.DataType.IsRefArray())
                                {
                                    stack.Push(new Expression(instruction, currentAddress, A));
                                    found = true;
                                }
                                else if (variable != null && variable.DataType.IsArray())
                                {
                                    //???
                                    stack.Push(new Expression(instruction, currentAddress, A));
                                    found = true;
                                }
                            }
                            if (!found)
                            {
                                B = stack.Pop();
                                var newExpression = new Expression(instruction, currentAddress, B, A);
                                if (A.removeIfVoid && B.removeIfVoid)
                                {
                                    newExpression.removeIfVoid = true;
                                }
                                stack.Push(newExpression);
                            }
                        }
                        //var A = stack.PopReference(currentAddress);

                        //if (A.IsReference()) //??? on second part
                        //{
                        //    stack.Push(new Expression(instruction, currentAddress, A));
                        //}
                        //else
                        //{
                        //    var B = stack.Pop();
                        //    stack.Push(new Expression(instruction, currentAddress, B, A));
                        //}
                    }
                    else if (Expression.IsLeafRef(instruction))
                    {
                        if (totalArguments == 2)
                        {
                            stack.Push(new Expression(instruction, currentAddress, word1, word2));
                        }
                        else
                        {
                            stack.Push(new Expression(instruction, currentAddress, word1));
                        }
                        if (instruction == Instruction.SH_LOCALASSIGN || instruction == Instruction.SH_LOCALDEC || instruction == Instruction.SH_LOCALINC ||
                            instruction == Instruction.SH_LOCALCREATE || instruction == Instruction.SH_LOCALDELETE ||
                            instruction == Instruction.SH_MEM_ASSIGN_IMM || instruction == Instruction.SH_MEM_ASSIGN_LOCAL)
                        {
                            statements.Add(stack.Pop());
                        }
                    }
                    else if (Expression.IsNoStack(instruction))
                    {
                        if (totalArguments == 0)
                        {
                            stack.Push(new Expression(instruction, currentAddress));
                        }
                        else if (totalArguments == 1)
                        {
                            stack.Push(new Expression(instruction, currentAddress, word1));
                        }
                        else if (totalArguments == 2)
                        {
                            stack.Push(new Expression(instruction, currentAddress, word1, word2));
                        }
                        statements.Add(stack.Pop());
                    }
                    else
                    {
                        //this is an unhandled instruction
                        0.GetHashCode();
                        if (totalArguments == 0)
                        {
                            stack.Push(new Expression(instruction, currentAddress));
                        }
                        else if (totalArguments == 1)
                        {
                            stack.Push(new Expression(instruction, currentAddress, word1));
                        }
                        else if (totalArguments == 2)
                        {
                            stack.Push(new Expression(instruction, currentAddress, word1, word2));
                        }
                    }
                }
            }
        Exit:
            state.exitAddress = address;
            while (stack.Count > 0 && stack.Peek().ExpressionType == Instruction.Nop)
            {
                stack.Pop();
            }

            if (stack.Count > 0 && statements.Count == 0)
            {
                statements.Add(stack.Pop());
                state.isStackExpression = true;
            }
            else
            {
                if (statements.Count > 0)
                {
                    statements.Add(new Expression(Instruction.Nop, state.finalAddress));
                }
            }

            DrainStack(stack, statements);


            if (statements.Count == 0)
            {
                return new Expression(Instruction.Nop, state.startAddress);
            }
            else
            {
                int last = statements.Count;
                int first = 0;
                var returnExpression = ConvertToStatements(statements, first, last);

                var cycleChecker = new CycleChecker();
                bool hadCycles = cycleChecker.CheckForCycles(returnExpression);

                if (functionId != -1 && processingWholeFunction)
                {
                    returnExpression = new Expression(Instruction.FUNC, returnExpression.address, returnExpression, functionId);
                }
                return returnExpression;
            }
        }

        private void AddCaseLabels(Expression switchBlockCode, SwitchBlock switchInfo)
        {
            Dictionary<int, List<SwitchCase>> casesDictionary = new Dictionary<int, List<SwitchCase>>();
            if (switchInfo.DefaultCaseAddress != -1)
            {
                var list = casesDictionary.GetOrAddNew(switchInfo.DefaultCaseAddress);
                list.Add(null);
            }
            foreach (var switchCase in switchInfo.SwitchCases)
            {
                var list = casesDictionary.GetOrAddNew(switchCase.TargetAddress);
                list.Add(switchCase);
            }
            Expression expression = switchBlockCode;
            while (expression != null)
            {
                int address = expression.LowestAddress;
                if (casesDictionary.ContainsKey(address))
                {
                    var switchCaseList = casesDictionary[address];
                    casesDictionary.Remove(address);
                    foreach (var switchCase in switchCaseList)
                    {
                        if (switchCase != null)
                        {
                            InsertStatementBefore(expression, new Expression(Instruction.SwitchCase, address, switchCase.Value, switchInfo.CaseType2or4));
                        }
                        else
                        {
                            InsertStatementBefore(expression, new Expression(Instruction.DefaultSwitchCase, address));
                        }
                    }
                }
                expression = expression.GetNextExpression();
            }
            if (casesDictionary.Count > 0)
            {

            }
        }

        private static void DrainStack(Stack<Expression> stack, List<Expression> statements)
        {
            if (stack.Count > 0)
            {
                bool seenFirstItem = false;
                //this is an error condition, code wasn't popped off the stack properly
                //insert remaining stack elements into statements list to minimize impact
                while (stack.Count > 0)
                {
                    var element = stack.Pop();
                    if (!element.removeIfVoid && element.ExpressionType != Instruction.Nop && !seenFirstItem && element.ExpressionType != Instruction.SH_LOCALASSIGN)
                    {
                        0.GetHashCode();
                        seenFirstItem = true;
                    }


                    int elementAddress = element.LowestAddress;
                    bool found = false;
                    for (int i = 0; i < statements.Count; i++)
                    {
                        var statement = statements[i];
                        int statementAddress = statement.LowestAddress;
                        if (statementAddress >= elementAddress)
                        {
                            statements.Insert(i, element);
                            found = true;
                            break;
                        }
                    }

                    if (!found)
                    {
                        statements.Add(element);
                    }
                }
            }
        }

        private bool IsMinus18BranchToTarget(int target, ref InstructionInfo info1)
        {
            bool matched = false;

            if (!matched)
            {
                info1 = Peek(target - 18 - 8);
                if (info1.totalArguments == 3 && ArgumentKinds.BranchInstructions.ContainsKey((int)info1.instruction))
                {
                    int wordNumber = ArgumentKinds.BranchInstructions[(int)info1.instruction];
                    int target2 = info1.words[wordNumber];
                    if (target == target2) matched = true;
                }
            }
            if (!matched)
            {
                info1 = Peek(target - 18 - 4);
                if (info1.totalArguments == 2 && ArgumentKinds.BranchInstructions.ContainsKey((int)info1.instruction))
                {
                    int wordNumber = ArgumentKinds.BranchInstructions[(int)info1.instruction];
                    int target2 = info1.words[wordNumber];
                    if (target == target2) matched = true;
                }
            }
            if (!matched)
            {
                info1 = Peek(target - 18 - 0);
                if (info1.totalArguments == 1 && ArgumentKinds.BranchInstructions.ContainsKey((int)info1.instruction))
                {
                    int wordNumber = ArgumentKinds.BranchInstructions[(int)info1.instruction];
                    int target2 = info1.words[wordNumber];
                    if (target == target2) matched = true;
                }
            }
            if (info1.instruction == Instruction.JUMP)
            {
                matched = false;
            }
            return matched;
        }

        private bool IsBranchToTarget(int address, int target, ref InstructionInfo info1)
        {
            info1 = Peek(address);
            if (ArgumentKinds.BranchInstructions.ContainsKey((int)info1.instruction))
            {
                int wordNumber = ArgumentKinds.BranchInstructions[(int)info1.instruction];
                int target2 = info1.words[wordNumber];
                if (target == target2) return true;
            }
            return false;
        }

        private static List<Expression> SortCasesByAddress(List<Expression> cases)
        {
            cases = cases.OrderBy(c => c.Value).OrderBy(c => c.address).ToList();
            return cases;
        }

        //int labelNumber = 0;
        //List<Expression> labelsList = new List<Expression>();
        List<Expression> jumpList = new List<Expression>();
        //int AddLabelAtAddress(int address, DecompileState state)
        //{
        //    if (state == null)
        //    {
        //        return -1;
        //    }
        //    var list = state.statements;
        //    if (list == null)
        //    {
        //        return -1;
        //    }
        //    for (int i = 0; i < list.Count; i++)
        //    {
        //        var expression = list[i];
        //        if (expression.LowestAddress == address)
        //        {
        //            if (expression.ExpressionType == Instruction.Label)
        //            {
        //                return expression.Value;
        //            }
        //            else
        //            {
        //                int myLabelNumber = labelNumber;
        //                var newLabel = new Expression(Instruction.Label, address, myLabelNumber);
        //                list.Insert(i, newLabel);
        //                labelNumber++;
        //                labelsList.Add(newLabel);
        //                return myLabelNumber;
        //            }
        //        }
        //    }
        //    return AddLabelAtAddress(address, state.parentState);
        //}

        internal static Expression ConvertToStatements(IList<Expression> statements)
        {
            return ConvertToStatements(statements, 0, statements.Count);
        }


        private static Expression ConvertToStatements(IList<Expression> statements, int first, int last)
        {
            if (last - first == 0)
            {
                return null;
            }
            int i = last - 1;
            var returnExpression = statements[i]; i--;
            while (i >= first)
            {
                var nextStatement = statements[i];
                if (nextStatement == null || nextStatement.removeIfVoid)
                {
                    var nopStatement = new Expression(Instruction.Nop, nextStatement.address);
                    returnExpression = new Expression(Instruction.Statement, nextStatement.address, nopStatement, returnExpression);
                }
                else
                {
                    returnExpression = new Expression(Instruction.Statement, nextStatement.address, nextStatement, returnExpression);
                }
                i--;
            }
            return returnExpression;
        }

        private static Expression GetParentLoop(Expression expression)
        {
            while (true)
            {
                if (expression == null)
                {
                    return expression;
                }
                if (expression.ExpressionType == Instruction.ForLoop || expression.ExpressionType == Instruction.WhileLoop)
                {
                    return expression;
                }
                expression = expression.Parent;
            }
        }

        Dictionary<int, int> currentLabels = new Dictionary<int, int>();
        int labelNumber = 0;

        void CleanUpJumps(Expression root)
        {
            foreach (var jump in jumpList)
            {
                if ((jump.ExpressionType == Instruction.JUMP || jump.ExpressionType == Instruction.IFZ) && (jump.Root == root))
                {
                    var parentLoop = GetParentLoop(jump);
                    Expression nextExpression = null;
                    if (parentLoop != null)
                    {
                        nextExpression = parentLoop.GetParentNextExpression();
                    }

                    int parentAddress = -1;
                    if (parentLoop != null) parentAddress = parentLoop.LowestAddress;
                    int nextAddress = -1;
                    int nextAddress2 = -1;
                    if (nextExpression != null) nextAddress = nextExpression.LowestAddress;
                    if (nextExpression != null && (
                        nextExpression.ExpressionType == Instruction.IfElse ||
                        (nextExpression.ExpressionType == Instruction.Statement && nextExpression.Arg1.ExpressionType == Instruction.IfElse)))
                    {
                        nextAddress2 = nextAddress - 6;
                    }

                    if (jump.Value == parentAddress)
                    {
                        jump.ExpressionType = Instruction.Continue;
                        jump.Value = 0;
                    }
                    else if (jump.Value == nextAddress || jump.Value == nextAddress2)
                    {
                        jump.ExpressionType = Instruction.Break;
                        jump.Value = 0;
                    }
                    else
                    {
                        int labelNumber = AddLabel(jump.Value, root);
                        if (labelNumber != -1)
                        {
                            if (jump.ExpressionType == Instruction.JUMP)
                            {
                                jump.ExpressionType = Instruction.Goto;
                            }
                            jump.Value = labelNumber;
                        }
                    }
                }
            }
        }

        private int AddLabel(int address, Expression root)
        {
            if (currentLabels.ContainsKey(address))
            {
                return currentLabels[address];
            }

            Expression expression = root;
            while (expression != null)
            {
                int lowestAddress = expression.LowestAddress;
                if (lowestAddress == address)
                {
                    //insert label here
                    int currentLabelNumber = labelNumber;
                    var label = new Expression(Instruction.Label, address, currentLabelNumber);
                    InsertStatementBefore(expression, label);
                    currentLabels.Add(address, currentLabelNumber);
                    labelNumber++;
                    return currentLabelNumber;
                }
                if (lowestAddress > address)
                {

                }

                expression = expression.GetNextExpression();
            }
            return -1;
        }

        private void InsertStatementBefore(Expression expression, Expression expressionToAdd)
        {
            var parent = expression.Parent;
            if (parent == null)
            {
                Expression newExpression;
                if (expressionToAdd.ExpressionType == Instruction.Statement)
                {
                    if (expressionToAdd.Arg2 == null)
                    {
                        newExpression = expressionToAdd;
                        expressionToAdd.Arg2 = expression;
                    }
                    else
                    {
                        //error
                    }
                }
                else
                {
                    newExpression = new Expression(Instruction.Statement, expressionToAdd.address, expressionToAdd, expression);
                }
            }
            else
            {
                int index = parent.Args.IndexOf(expression);
                if (index != -1)
                {
                    parent.Args[index] = null;
                    var newExpression = new Expression(Instruction.Statement, expression.address, expressionToAdd, expression);
                    parent.Args[index] = newExpression;
                }
                else
                {
                    //not found, error
                }
            }
        }


        //private static bool IsFirstExpressionInLoop(Expression expression)
        //{
        //    bool isStartOfLoop = false;
        //    var parentLoop = GetParentLoop(expression);
        //    if (parentLoop != null)
        //    {
        //        Expression loopBody = null;
        //        if (parentLoop.ExpressionType == Instruction.WhileLoop)
        //        {
        //            loopBody = parentLoop.Arg2;
        //        }
        //        if (parentLoop.ExpressionType == Instruction.ForLoop)
        //        {
        //            loopBody = parentLoop.Arg4;
        //        }
        //        if (loopBody != null && (loopBody == expression || loopBody.Arg1 == expression))
        //        {
        //            isStartOfLoop = true;
        //        }
        //    }
        //    return isStartOfLoop;
        //}

        //void CleanUpLabels()
        //{
        //    //Dictionary<int, Expression> labelNumberToExpression = new Dictionary<int, Expression>();
        //    //foreach (var label in labelsList)
        //    //{
        //    //    labelNumberToExpression.Add(label.Value, label);
        //    //}

        //    //for each label: is it at the beginning of a loop?
        //    bool[] IsAtStartOfLoop = new bool[labelsList.Count];
        //    for (int i = 0; i < labelsList.Count; i++)
        //    {
        //        IsAtStartOfLoop[i] = IsFirstExpressionInLoop(labelsList[i]);
        //    }
        //    int[] labelReferenceCount = new int[labelsList.Count];
        //    foreach (var gotoEntry in gotoList)
        //    {
        //        int labelNumber = gotoEntry.Value;
        //        labelReferenceCount[labelNumber]++;
        //    }

        //    foreach (var gotoEntry in gotoList)
        //    {
        //        int labelNumber = gotoEntry.Value;
        //        if (IsAtStartOfLoop[labelNumber])
        //        {
        //            var myParent = GetParentLoop(gotoEntry);
        //            var label = labelsList[gotoEntry.Value];
        //            var labelParent = GetParentLoop(label);
        //            if (myParent == labelParent)
        //            {
        //                if (labelParent.ExpressionType == Instruction.WhileLoop ||
        //                    (labelParent.ExpressionType == Instruction.ForLoop &&
        //                    (labelParent.Arg1 == null && label.Arg2 == null && label.Arg3 == null)))
        //                {
        //                    gotoEntry.ExpressionType = Instruction.Continue;
        //                    gotoEntry.Value = 0;
        //                    labelReferenceCount[labelNumber]--;
        //                }
        //            }
        //            else
        //            {

        //            }
        //        }
        //    }
        //    for (int i = 0; i < labelReferenceCount.Length; i++)
        //    {
        //        if (labelReferenceCount[i] == 0)
        //        {
        //            RemoveStatement(labelsList[i]);
        //        }
        //    }
        //}


    }
    public static partial class Extensions
    {
        public static Expression Pop(this List<Expression> statements)
        {
            int i = statements.Count - 1;
            while (i >= 0)
            {
                if (!Expression.IsSkipThrough(statements[i].ExpressionType))
                {
                    var result = statements[i];
                    bool isVoid = false;
                    if (result.ExpressionType.IsFunctionCall())
                    {
                        var variable = result.Variable;
                        if (variable != null && variable.DataType == DataType.Void)
                        {
                            isVoid = true;
                        }
                    }

                    if (!isVoid)
                    {
                        statements.RemoveAt(i);
                        return result;
                    }
                }
                i--;
            }
            return null;
        }

        public static Expression PopReference(this Stack<Expression> stack, int currentAddress)
        {
            Expression a = stack.Pop();
            DataType dataType = a.GetDataType();
            if (a.IsReference() || dataType == DataType.RefInt || dataType == DataType.Void)
            {

            }
            else
            {
                a = new Expression(Instruction.AssignmentRef, currentAddress, stack.Pop(), a);
                if (a.Arg1.removeIfVoid && a.Arg2.removeIfVoid)
                {
                    a.removeIfVoid = true;
                }
            }
            return a;
        }

        public static Expression PopAndSkip(this Stack<Expression> stack, Stack<Expression> skipped)
        {
            while (Expression.IsSkipThrough(stack.Peek().ExpressionType))
            {
                skipped.Push(stack.Pop());
            }
            return stack.Pop();
        }

        public static void PushFrom<T>(this Stack<T> stack, Stack<T> popFromThis)
        {
            while (popFromThis.Count > 0)
            {
                stack.Push(popFromThis.Pop());
            }
        }
    }

    public class InstructionInfo
    {
        public Instruction instruction;
        public int[] words = new int[3];
        public int word1
        {
            get
            {
                return words[0];
            }
            set
            {
                words[0] = value;
            }
        }
        public int word2
        {
            get
            {
                return words[1];
            }
            set
            {
                words[1] = value;
            }
        }
        public int word3
        {
            get
            {
                return words[2];
            }
            set
            {
                words[2] = value;
            }
        }

        public int nextAddress, totalArguments;
        public InstructionInfo Clone()
        {
            return (InstructionInfo)MemberwiseClone();
        }
        public InstructionInfo Clone(Instruction newInstruction)
        {
            var clone = (InstructionInfo)MemberwiseClone();
            clone.instruction = newInstruction;
            clone.totalArguments = newInstruction.GetNumberOfWords();
            return clone;
        }
        public int CurrentAddress
        {
            get
            {
                return nextAddress - totalArguments * 4 - 2;
            }
        }
        public override string ToString()
        {
            StringBuilder sb = new StringBuilder();
            bool needComma = false;
            sb.Append("0x" + CurrentAddress.ToString("X") + ":    ");
            sb.Append(instruction);
            if (totalArguments > 0)
            {
                sb.Append(" ");
                for (int wordNumber = 0; wordNumber < totalArguments; wordNumber++)
                {
                    int word = words[wordNumber];
                    Util.PrintComma(sb, ref needComma);
                    sb.Append("0x" + word.ToString("X"));
                }
            }
            return sb.ToString();
        }
    }
}

