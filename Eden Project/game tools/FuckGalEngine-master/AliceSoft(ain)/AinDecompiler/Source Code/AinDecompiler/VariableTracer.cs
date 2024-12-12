using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Text;

namespace AinDecompiler
{
    [Flags]
    public enum VariableTraceMode
    {
        Invalid = -1,
        None = 0,
        DirectCopies = 1,
        Writes = 2,
        Reads = 4,
        Any = DirectCopies | Writes | Reads,
        LookInsideReferences = 8,
        LookInsideEverything = 16,
        Recursive = 32,
        DirectCopiesRecursive = DirectCopies | Recursive,
    }

    class VariableTracer
    {
        AinFile ainFile;
        bool recursive = false;
        VariableTraceMode mode = VariableTraceMode.None;
        IVariable lookFor;

        public VariableTracer(AinFile ainFile)
        {
            this.ainFile = ainFile;
        }

        HashSet<IVariable> matches = new HashSet<IVariable>();
        HashSet<IVariable> finished = new HashSet<IVariable>();

        public void EnqueueVariable(IVariable lookFor)
        {
            if (lookFor != null)
            {
                matches.Set(lookFor);
            }
        }

        public HashSet<IVariable> TraceVariable(IVariable lookFor, VariableTraceMode mode)
        {
            this.mode = mode;
            this.lookFor = lookFor;
            if (0 != (this.mode & VariableTraceMode.Recursive))
            {
                this.recursive = true;
            }

            EnqueueVariable(lookFor);
            RunQueue();
            matches.Remove(lookFor);
            return matches;
        }

        public void RunQueue()
        {
            do
            {
                var currentMatches = matches.ToArray();
                foreach (var variable in currentMatches)
                {
                    if (!finished.Contains(variable))
                    {
                        TraceVariableInternal(variable);
                    }
                }
                if (!recursive)
                {
                    break;
                }
            }
            while (finished.Count != matches.Count);
        }

        private void TraceVariableInternal(IVariable lookFor)
        {
            this.lookFor = lookFor;
            if (lookFor == null)
            {
                return;
            }

            var parentFunction = lookFor.Parent as Function;
            if (parentFunction != null)
            {
                var variable = lookFor as Variable;
                if (lookFor.Index >= parentFunction.ParameterCount)
                {
                    TraceLocalVariable(variable);
                }
                else
                {
                    TraceFunctionParameter(variable);
                }
            }
            var parentStruct = lookFor.Parent as Struct;
            if (parentStruct != null)
            {
                var variable = lookFor as Variable;
                TraceStructMember(variable);
            }
            var global = lookFor as Global;
            if (global != null)
            {
                TraceGlobal(global);
            }
            var function = lookFor as Function;
            if (function != null)
            {
                //this means we're looking for the return value of the function
                TraceFunction(function);
            }
            finished.Set(lookFor);
        }

        private void TraceFunction(Function function)
        {
            var usedBy = ainFile.CodeSearchCache.FunctionsCache.GetUsedBy(function);
            TraceInFunctions(usedBy, function);
        }

        private void TraceGlobal(Global global)
        {
            var usedBy = ainFile.CodeSearchCache.GlobalsCache.GetUsedBy(global);
            TraceInFunctions(usedBy, global);
        }

        private void TraceStructMember(Variable structMember)
        {
            var usedBy = ainFile.CodeSearchCache.GetFunctionsUsingStructMember(structMember);
            TraceInFunctions(usedBy, structMember);
        }

        private void TraceFunctionParameter(Variable parameter)
        {
            var parentFunction = (Function)parameter.Parent;
            //first trace it like a local variable
            TraceInFunction(parentFunction, parameter);

            //then trace calls to the function
            var usedBy = ainFile.CodeSearchCache.FunctionsCache.GetUsedBy(parentFunction);
            foreach (var function in usedBy)
            {
                TraceFunctionParameter(function, parentFunction, parameter);
            }
        }

        private void TraceLocalVariable(Variable localVariable)
        {
            var function = (Function)localVariable.Parent;
            TraceInFunction(function, localVariable);
        }

        void TraceInFunctions(IEnumerable<Function> functions, IVariable variable)
        {
            foreach (var function in functions)
            {
                TraceInFunction(function, variable);
            }
        }

        void TraceInFunction(Function function, IVariable variable)
        {
            var expressionMap = ainFile.DecompiledCodeCache.GetExpressionMap(function);
            var variableUsage = expressionMap.FindVariable(variable);

            foreach (var node in variableUsage)
            {
                TraceExpressionUp2(node.item);
            }
        }

        private void TraceExpressionUp2(Expression firstExpression)
        {
            //Called on a instance of a variable expression in a function
            //Want to find:
            //  Look at operators and assignment, ? : operator, casting
            //  Copying to a function arguments
            //  Return values
            //Types of searches:
            //  Direct copies of variables, ignoring casting
            //  Other variables that appear in a variable read
            //  Other variables that appear in a variable write

            //IVariable functionParameter = null;
            IVariable lValue = null;
            IVariable rValue = null;

            var expression = firstExpression;
            if (expression == null)
            {
                return;
            }

            //look for a parent expression by seeking through operators
            while (true)
            {
                var parent = expression.Parent;
                if (parent == null || parent.ExpressionType == Instruction.Statement)
                {
                    break;
                }
                //check for Return
                if (parent.ExpressionType == Instruction.RETURN)
                {
                    expression = parent;
                    lValue = parent.Root.Variable;
                    break;
                }
                //check for function call or comma
                if (parent.ExpressionType.IsFunctionCall() || parent.ExpressionType == Instruction.Comma)
                {
                    //functionParameter = expression.GetFunctionCallParameter();
                    lValue = expression.GetFunctionCallParameter();
                    break;
                }
                //check for cast operator or expression
                if (!ExpressionIsOperatorOrCast(parent))
                {
                    break;
                }
                expression = parent;
            }

            //is this an assignment expression?
            if (lValue == null)
            {
                if (expression.ExpressionType.IsAssignment())
                {
                    lValue = expression.Variable;
                    var lValueCanon = lValue.Canonicalize();
                    var arg1ExpressionType = expression.Arg1.ExpressionType;
                    if (lValueCanon == null && (arg1ExpressionType == Instruction.REF || arg1ExpressionType == Instruction.ArrayIndex))
                    {
                        lValue = expression.Arg1.Arg1.Variable;
                    }
                    var arg2 = SkipCastExpressions(expression.Arg2);
                    if (arg2 != null)
                    {
                        rValue = arg2.Variable;
                    }
                    if (rValue != null)
                    {
                        rValue = rValue.Canonicalize();
                    }
                }
            }
            else
            {
                rValue = SkipCastExpressions(expression).Variable;
                if (rValue != null)
                {
                    rValue = rValue.Canonicalize();
                }
            }

            //fixme, lvalue and rvalue stuff got weird
            //Are we directly pointing to the variable? (excluding cast operations)
            Expression e2 = SkipCastExpressions(expression);
            bool isDirect = false;
            var v2 = e2.Variable;
            IVariable v2Canon = null;
            if (v2 != null)
            {
                v2Canon = v2.Canonicalize();
                if (v2Canon == v2 && !(expression.ExpressionType.IsAssignment() && !expression.ExpressionType.IsDirectAssignment()))
                {
                    isDirect = true;
                }
            }

            if (0 != (mode & VariableTraceMode.DirectCopies))
            {
                if (isDirect && (lValue == lookFor || v2 == lookFor || rValue == lookFor))
                {
                    EnqueueVariable(v2);
                    EnqueueVariable(lValue);
                    EnqueueVariable(rValue);
                }
            }
            if (0 != (mode & VariableTraceMode.Writes))
            {
                if (lValue == lookFor)
                {
                    TraceExpressionDown(expression);
                }
            }
            if (0 != (mode & VariableTraceMode.Reads))
            {
                var lValueCanon = lValue.Canonicalize();
                if (lValueCanon != null)
                {
                    EnqueueVariable(lValueCanon);
                }
                TraceExpressionDown(expression);
            }
        }

        //private void TraceExpressionUp(Expression firstExpression)
        //{
        //    //Look For These operations:
        //    //operators and assignment
        //    // ? : operator (special kind of if-else operator)
        //    //casting

        //    //Not These:
        //    //array indexing
        //    //struct member operator
        //    //if-else, while, for

        //    var expression = firstExpression;
        //    if (expression == null)
        //    {
        //        return;
        //    }

        //    while (true)
        //    {
        //        var parent = expression.Parent;
        //        if (parent == null || parent.ExpressionType == Instruction.Statement)
        //        {
        //            break;
        //        }
        //        //check for a Return, this indicates that the variable influences the return value of the function.
        //        if (parent.ExpressionType == Instruction.RETURN)
        //        {
        //            var root = parent.Root;
        //            if (root != null)
        //            {
        //                var function = root.Variable as Function;
        //                if (function != null)
        //                {
        //                    EnqueueVariable(function);
        //                }
        //            }
        //        }
        //        if (!ExpressionIsOperatorOrCast(parent))
        //        {
        //            break;
        //        }
        //        expression = parent;
        //    }
        //    TraceExpressionDown(expression);
        //}

        private void TraceExpressionDown(Expression expression)
        {
            if (ExpressionIsOperatorOrCast(expression))
            {
                foreach (var child in expression.Args)
                {
                    if (child != null)
                    {
                        TraceExpressionDown(child);
                    }
                }
            }
            else
            {
                var variable = expression.Variable;
                if (variable != null)
                {
                    variable = variable.Canonicalize();
                }
                if (variable != null)
                {
                    if (variable.Index != -1)
                    {
                        EnqueueVariable(variable);
                    }
                }
            }
        }

        //private bool TraceExpressionDown2(Expression expression)
        //{
        //    if (ExpressionIsOperatorOrCast(expression))
        //    {
        //        foreach (var child in expression.Args)
        //        {
        //            if (child != null)
        //            {
        //                TraceExpressionDown(child);
        //            }
        //        }
        //    }
        //    else
        //    {
        //        return false;
        //    }
        //    return true;
        //}

        private static Expression SkipCastExpressions(Expression expression)
        {
            if (expression == null)
            {
                return null;
            }
            while (expression.ExpressionType.IsCast() && expression.Arg1 != null)
            {
                expression = expression.Arg1;
            }
            return expression;
        }

        private static bool ExpressionIsOperatorOrCast(Expression expression)
        {
            //accept cast operators and arithmetic operators, reject everything else
            if (ExpressionDisplayer.castOperations.Contains(expression.ExpressionType))
            {

            }
            else if (ExpressionDisplayer.OperatorTable.ContainsKey(expression.ExpressionType))
            {
                if (expression.ExpressionType == Instruction.Comma)
                {
                    return false;
                }
                if (expression.ExpressionType == Instruction.IfElse)
                {
                    var parent2 = expression.Parent;
                    if (parent2 == null || parent2.ExpressionType == Instruction.Statement)
                    {
                        return false;
                    }
                }
            }
            else if (expression.ExpressionType == Instruction.REF || expression.ExpressionType == Instruction.ArrayIndex)
            {
                //check for array index operator
                if (expression.Arg1 != null)
                {
                    var arg1Variable = expression.Arg1.Variable;
                    if (arg1Variable.DataType.IsArray())
                    {
                        return true;
                    }
                }
                return false;
            }
            else
            {
                return false;
            }
            return true;
        }

        private void TraceFunctionParameter(Function function, Function lookFor, Variable parameter)
        {
            int parameterIndex = parameter.Index;

            var expressionMap = ainFile.DecompiledCodeCache.GetExpressionMap(function);
            var variableUsage = expressionMap.FindVariable(lookFor);

            foreach (var node in variableUsage)
            {
                var expression = node.item;
                var argument = expression.GetFunctionCallArgument(parameterIndex);

                TraceExpressionUp2(argument);
            }
        }
    }
}
