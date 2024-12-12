using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Text;
using System.Collections.ObjectModel;

namespace AinDecompiler
{
    /// <summary>
    /// The basic unit of a program, expressions contain other expressions (subexpressions) in a tree-structure.  An expression could be a variable, operation, function call, if-else block, while loop, etc...
    /// </summary>
    public partial class Expression
    {
        /// <summary>
        /// A dummy expression used to avoid use of nulls when printing operators
        /// </summary>
        public static Expression Empty = new Expression(Instruction.Nop, -1);

        /// <summary>
        /// Backing field for the instruction that created this expression (uppercase letters), or a programming construct which is not an actual instruction (proper-case letters).
        /// </summary>
        private Instruction _expressionType;
        /// <summary>
        /// The first int value of the expression.  Could be a number, a string index, function index, or something else.
        /// </summary>
        public int Value;
        /// <summary>
        /// The second int value of the expression.  The local variable for a SH_LOCALASSIGN instruction, or the type of a switch case.
        /// </summary>
        public int Value2;
        /// <summary>
        /// The third int value of the expression.  Not used because the instructions with 3 arguments are converted to a sequence of instructions.
        /// </summary>
        public int Value3;
        /// <summary>
        /// The subexpressions of the expression.  Expressions added here are considered to be owned by this expression, and will have the parent set to this expression.
        /// </summary>
        public ExpressionCollection Args;
        /// <summary>
        /// A flag to indicate that this expression was created as part of stack item duplication, and is not real executable code if a POP instruction is encountered.
        /// </summary>
        public bool removeIfVoid = false;
        /// <summary>
        /// The AIN file associated with this expression.
        /// </summary>
        public AinFile ainFile;
        /// <summary>
        /// The function number associated with this expression, so it can get local variables.
        /// </summary>
        int functionNumber;
        /// <summary>
        /// The address of the code that created this expression.
        /// </summary>
        public int address;

        /// <summary>
        /// Backing field for the parent expression, or null if this expression was created for display only.
        /// </summary>
        protected Expression parent = null;

        /// <summary>
        /// The parent expression, or null if this expression was created for display only.
        /// </summary>
        public Expression Parent
        {
            get
            {
                return parent;
            }
        }

        /// <summary>
        /// Gets the root expression by iterating over the parent expressions.  Returns the FUNC instruction for the function that contains this expression, or returns the first expression if this expression has not yet been added to another expression.  Will not be null.
        /// </summary>
        public Expression Root
        {
            get
            {
                var e = this;
                while (e.Parent != null)
                {
                    e = e.Parent;
                }
                return e;
            }
        }

        /// <summary>
        /// A collection of expressions.  Items added to this collection are considered to be owned by the parent.
        /// </summary>
        public class ExpressionCollection : Collection<Expression>
        {
            protected Expression parent;
            /// <summary>
            /// Creates a new ExpressionCollection
            /// </summary>
            /// <param name="parent">The parent which will own items added to this collection</param>
            public ExpressionCollection(Expression parent)
            {
                this.parent = parent;
            }
            /// <summary>
            /// Adds multiple items to this collection, items added will be owned by this collection
            /// </summary>
            /// <param name="sequence">The sequence of items to be added to the collection</param>
            public void AddRange(IEnumerable<Expression> sequence)
            {
                foreach (var item in sequence)
                {
                    Add(item);
                }
            }
            /// <summary>
            /// Inserts an item at a particular index.  Overridden to indicate the new parent, and check if the item already had a parent item.
            /// </summary>
            /// <param name="index">Index to insert at</param>
            /// <param name="item">The expression to add</param>
            protected override void InsertItem(int index, Expression item)
            {
                if (item != null)
                {
                    if (item.parent != null)
                    {

                    }
                    item.parent = this.parent;
                }
                base.InsertItem(index, item);
            }
            /// <summary>
            /// Removes an item from a particular index.  Overriden to check if the parent wasn't set properly.
            /// </summary>
            /// <param name="index">Index to remove at</param>
            protected override void RemoveItem(int index)
            {
                var oldItem = this.Items[index];
                if (oldItem != null)
                {
                    if (oldItem.parent == null)
                    {

                    }
                }
                oldItem.parent = null;
                base.RemoveItem(index);
            }
            /// <summary>
            /// Replaces an item at a particular index.  Overriden to set the new parent, and check if the old item had a parent.
            /// </summary>
            /// <param name="index">Index to replace at</param>
            /// <param name="item">The replacement expression</param>
            protected override void SetItem(int index, Expression item)
            {
                var oldItem = this.Items[index];
                if (oldItem != null)
                {
                    if (oldItem.parent == null)
                    {

                    }
                    if (oldItem.parent != this.parent)
                    {

                    }
                }
                if (item != null)
                {
                    if (item.parent != null)
                    {

                    }
                    item.parent = this.parent;
                }
                else
                {
                    if (oldItem != null)
                    {
                        if (oldItem.parent == null)
                        {

                        }
                        else
                        {
                            oldItem.parent = null;
                        }
                    }
                }
                base.SetItem(index, item);
            }
            /// <summary>
            /// Recursively copies this collection to a new collection, with a new parent.
            /// </summary>
            /// <param name="newParent">The new parent to assign items to</param>
            /// <returns>A copy of the collection with a new parent</returns>
            public ExpressionCollection Clone(Expression newParent)
            {
                var clone = new ExpressionCollection(newParent);
                foreach (var item in this)
                {
                    clone.Add(item != null ? item.Clone() : null);
                }
                return clone;
            }
        }

        /// <summary>
        /// The instruction that created this expression (uppercase letters), or a programming construct which is not an actual instruction (proper-case letters)
        /// </summary>
        public Instruction ExpressionType
        {
            get
            {
                if (this != null)
                {
                    return _expressionType;
                }
                else
                {
                    return Instruction.Null;
                }
            }
            set
            {
                if (this != null)
                {
                    _expressionType = value;
                }
            }
        }

        /// <summary>
        /// The first subexpression of this expression.  Usually the child-expression of a unary operator, the first expression of a binary operator, a comma expression for a function call, the condition of an if-block or while loop, the initializer of a for loop, the object for a built-in method, or something else.
        /// </summary>
        public Expression Arg1
        {
            get
            {
                return Args.GetOrNull(0);
            }
            set
            {
                Args.SetOrAdd(0, value);
            }
        }
        /// <summary>
        /// The second subexpression of this expression.  Usually the second expression of a binary operator, the if part of an if block, the body of a while loop, the condition of a for loop, the first argument of a built-in method, or something else.
        /// </summary>
        public Expression Arg2
        {
            get
            {
                return Args.GetOrNull(1);
            }
            set
            {
                Args.SetOrAdd(1, value);
            }
        }

        /// <summary>
        /// The third subexpression of this expression.  Usually the else part of an if block, the increment of a for loop, the second argument of a built-in method, or something else.
        /// </summary>
        public Expression Arg3
        {
            get
            {
                return Args.GetOrNull(2);
            }
            set
            {
                Args.SetOrAdd(2, value);
            }
        }

        /// <summary>
        /// The fourth subexpression of this expression.  Usually the body of a for loop, the third argument of a built-in method, or something else.
        /// </summary>
        public Expression Arg4
        {
            get
            {
                return Args.GetOrNull(3);
            }
            set
            {
                Args.SetOrAdd(3, value);
            }
        }

        /// <summary>
        /// Returns true if this expression is displayed as a block. (If-else block, for loop, while loop, switch, or function body)
        /// </summary>
        /// <returns></returns>
        public bool IsBlock()
        {
            return (this != null) && Expression.IsBlock(ExpressionType);
        }

        /// <summary>
        /// Returns true if this expression is a reference to a variable, a field of a struct, or an element of an array.
        /// </summary>
        /// <returns></returns>
        public bool IsReference()
        {
            return (this != null) && (Expression.IsRef(ExpressionType) || Expression.IsLeafRef(ExpressionType));
        }

        /// <summary>
        /// Displays the entire expression as code using an ExpresisonDisplayer object, then displays the expression types of the subexpressions.
        /// </summary>
        /// <returns>The expression displayed as code</returns>
        public override string ToString()
        {
            StringBuilder sb = new StringBuilder();
            var displayer = new ExpressionDisplayer();
            displayer.ainFile = ainFile;
            displayer.currentFuncNumber = this.functionNumber;
            try
            {
                sb.Append(displayer.PrintExpression2(this, false));
            }
            catch (Exception ex)
            {

            }
            sb.Append("        ");


            sb.Append(ExpressionType.ToString());

            int words = ExpressionType.GetNumberOfWords();
            if (words >= 1)
            {
                sb.Append(" " + Value);
            }
            if (words >= 2)
            {
                sb.Append(" " + Value2);
            }
            if (words >= 3)
            {
                sb.Append(" " + Value3);
            }
            sb.Append(" (");
            foreach (var arg in Args)
            {
                if (arg != null)
                {
                    sb.Append(" " + arg.ExpressionType.ToString());
                }
                else
                {
                    sb.Append(" null");
                }

            }
            sb.Append(")");

            return sb.ToString();
        }

        /// <summary>
        /// Returns the variable corresponding to this expression.  May be a real variable, or a newly-created variable object (for the result of an operation, or an array element), or may be null.
        /// </summary>
        public IVariable Variable
        {
            get
            {
                var function = ainFile.GetFunction(this.functionNumber);
                var localStructInfo = function.GetClass();
                return ExpressionDisplayer.GetReferencedVariableType2(this, function, localStructInfo);
                //var displayer = new ExpressionDisplayer(ainFile, functionNumber);
                //return displayer.GetReferencedVariableType(this);
            }
        }

        /// <summary>
        /// Returns the lowest address contained in this expression tree.  Used to help identify loops, and sort code.
        /// </summary>
        public int LowestAddress
        {
            get
            {
                int lowAddress = this.address;
                if (lowAddress < 0)
                {
                    lowAddress = int.MaxValue;
                }
                for (int i = 0; i < Args.Count; i++)
                {
                    var arg = Args[i];
                    if (arg != null)
                    {
                        int argLowestAddress = arg.LowestAddress;
                        if (argLowestAddress < 0)
                        {
                            argLowestAddress = int.MaxValue;
                        }
                        if (argLowestAddress < lowAddress)
                        {
                            lowAddress = argLowestAddress;
                        }
                    }
                }
                return lowAddress;
            }
        }

        /// <summary>
        /// Creates a new blank expression.
        /// </summary>
        public Expression()
        {
            this.ainFile = defaultAinFile;
            this.functionNumber = defaultFunctionNumber;
            this.Args = new ExpressionCollection(this);
        }

        /// <summary>
        /// Creates an expression given the instruction, address, and three values.
        /// </summary>
        /// <param name="expressionType">The instruction</param>
        /// <param name="address">The address</param>
        /// <param name="value1">First int value</param>
        /// <param name="value2">Second int value</param>
        /// <param name="value3">Third int value</param>
        public Expression(Instruction expressionType, int address, int value1, int value2, int value3)
            : this()
        {
            this.ExpressionType = expressionType;
            this.address = address;
            this.Value = value1;
            this.Value2 = value2;
            this.Value3 = value3;
        }

        /// <summary>
        /// Creates an expression given the instruction, address, three values, and a number of subexpressions.
        /// </summary>
        /// <param name="expressionType">The instruction</param>
        /// <param name="address">The address</param>
        /// <param name="value1">First int value</param>
        /// <param name="value2">Second int value</param>
        /// <param name="value3">Third int value</param>
        /// <param name="args">One or more expressions to add as subexpressions</param>
        public Expression(Instruction expressionType, int address, int value1, int value2, int value3, params Expression[] args)
            : this(expressionType, address, value1, value2, value3)
        {
            this.Args.AddRange(args);
        }

        /// <summary>
        /// Returns the first ancestor of the desired instruction, excluding Statements.
        /// </summary>
        /// <param name="expressionType">The instruction to look for</param>
        /// <returns>The first ancestor of the desired instruction</returns>
        public Expression GetAncestorOfType(Instruction expressionType)
        {
            var e = this.parent;
            while (e != null && e.ExpressionType != expressionType && e.ExpressionType != Instruction.Statement)
            {
                e = e.parent;
            }
            return e;
        }

        /// <summary>
        /// Returns the first ancestor of one of the desired instructions, excluding Statements.
        /// </summary>
        /// <param name="expressionTypes"></param>
        /// <returns>The first ancestor of a desired instruction</returns>
        public Expression GetAncestorOfType(HashSet<Instruction> expressionTypes)
        {
            var e = this.parent;
            while (e != null && !expressionTypes.Contains(e.ExpressionType) && e.ExpressionType != Instruction.Statement)
            {
                e = e.parent;
            }
            return e;
        }

        /// <summary>
        /// Set of function instructions used for GetFunctionCallParameter
        /// </summary>
        static HashSet<Instruction> _functionInstructions = new HashSet<Instruction>(new Instruction[]
        {
            Instruction.CALLFUNC, Instruction.CALLMETHOD, Instruction.CALLHLL, Instruction.CALLSYS,
        });

        /// <summary>
        /// Returns the function parameter that corresponds to this function argument expression by looking down the parent expressions for commas until it reaches a function call instruction.  Returns null if the expression is not a function argument, or the function parameter is out of bounds.
        /// </summary>
        /// <returns>The function parameter that corresponds to this function argument expression, or null</returns>
        public IVariable GetFunctionCallParameter()
        {
            int depth;
            return GetFunctionCallParameter(out depth);
        }

        /// <summary>
        /// Returns the function parameter that corresponds to this function argument expression by looking down the parent expressions for commas until it reaches a function call instruction.  Returns null if the expression is not a function argument, or the function parameter is out of bounds.
        /// </summary>
        /// <param name="depth">The parameter number of the function (not necessarily the index)</param>
        /// <returns>The function parameter that corresponds to this function argument expression, or null</returns>
        public IVariable GetFunctionCallParameter(out int depth)
        {
            //todo: fix for "<void>" variables - done
            var expression = this;
            var funcExpression = expression.GetAncestorOfType(_functionInstructions);
            depth = -1;
            if (funcExpression != null)
            {
                var comma = expression.GetAncestorOfType(Instruction.Comma);
                if (comma != null)
                {
                    depth = comma.GetDepthFromType(_functionInstructions);
                    if (comma.Arg2 != null && comma.Arg2 == expression)
                    {
                        depth++;
                    }
                }
                else
                {
                    depth = 0;
                }
                var funcType = funcExpression.Variable;
                var func2 = funcType as Function;
                if (func2 != null && func2.GetClass() != null)
                {
                    //fix method calls using the wrong argument number
                    depth--;
                }
                var func3 = funcType as IFunction;
                if (func3 != null)
                {
                    if (depth >= 0 && depth < func3.ParameterCount)
                    {
                        return func3.GetNonVoidFunctionParameter(depth);
                        //return func3.Parameters[depth];
                    }
                }
            }
            return null;
        }

        /// <summary>
        /// Returns the expression that is passed to the function at a particular parameter number.  Returns null if an argument is not found, or the expression is not a function call.
        /// </summary>
        /// <param name="argumentNumber">Function argument index</param>
        /// <returns>The expression passed to the </returns>
        public Expression GetFunctionCallArgument(int argumentNumber)
        {
            var functionCallExpression = this;
            if (!_functionInstructions.Contains(functionCallExpression.ExpressionType))
            {
                return null;
            }

            if (functionCallExpression.Parent == null) return null;

            var expression = functionCallExpression.Arg1;
            if (expression == null) return null;

            int currentArgumnetNumber = argumentNumber;

            if (functionCallExpression.ExpressionType == Instruction.CALLMETHOD)
            {
                currentArgumnetNumber++;
            }

            while (currentArgumnetNumber > 0)
            {
                if (expression.ExpressionType == Instruction.Comma)
                {
                    expression = expression.Arg2;
                    if (expression == null) return null;
                }
                else
                {
                    break;
                }
                currentArgumnetNumber--;
            }
            if (expression.ExpressionType == Instruction.Comma)
            {
                expression = expression.Arg1;
                if (expression == null) return null;
            }
            while (ExpressionDisplayer.castOperations.Contains(expression.ExpressionType))
            {
                expression = expression.Arg1;
                if (expression == null) return null;
            }
            return expression;
        }

        /// <summary>
        /// Returns the distance between this expression and an ancestor expression of a specific type.
        /// </summary>
        /// <param name="expressionTypes">The expression types to look for</param>
        /// <returns>The total number of parents encountered.</returns>
        public int GetDepthFromType(HashSet<Instruction> expressionTypes)
        {
            var e = this.parent;
            int depth = 0;
            while (e != null && !expressionTypes.Contains(e.ExpressionType) && e.ExpressionType != Instruction.Statement)
            {
                e = e.parent;
                depth++;
            }
            return depth;
        }

        /// <summary>
        /// Negates an expression.  May add or remove a NOT expression, flip a true/false value, or reverse an equality or inequality.
        /// </summary>
        /// <param name="ex">The expression to negate</param>
        public static void Negate(ref Expression ex)
        {
            if (InstructionInformation.NegatedInstructions.ContainsKey(ex.ExpressionType))
            {
                ex.ExpressionType = InstructionInformation.NegatedInstructions[ex.ExpressionType];
            }
            else if (ex.ExpressionType == Instruction.NOT)
            {
                var childExpression = ex.Arg1;
                RemoveUnaryParent(childExpression);
                ex = childExpression;
            }
            else if (ex.ExpressionType == Instruction.ITOB)
            {
                var arg1 = ex.Arg1;
                Negate(ref arg1);
                ex.Arg1 = arg1;
            }
            else if (ex.ExpressionType == Instruction.PUSH)
            {
                if (ex.Value == 0)
                {
                    ex.Value = 1;
                }
                else if (ex.Value == 1)
                {
                    ex.Value = 0;
                }
                else
                {
                    //add a not
                    var newExpression = new Expression(Instruction.NOT, ex.address, ex);
                    AddUnaryParent(newExpression, ex);
                    ex = newExpression;
                }
            }
            else
            {
                //add a not
                var newExpression = new Expression(Instruction.NOT, ex.address);
                AddUnaryParent(newExpression, ex);
                ex = newExpression;
            }
        }

        /// <summary>
        /// Moves an expression to a new parent unary expression, and removes the previous parent of the expression.
        /// </summary>
        /// <param name="newExpression">The expression to become the new parent</param>
        /// <param name="childExpression">The original expression to become the child of the new parent</param>
        public static void AddUnaryParent(Expression newParentExpression, Expression childExpression)
        {
            if (childExpression == null)
            {
                return;
            }
            var parent = childExpression.Parent;
            if (parent != null)
            {
                int indexInParent = parent.Args.IndexOf(childExpression);
                parent.Args[indexInParent] = null;
                parent.Args[indexInParent] = newParentExpression;
            }
            newParentExpression.Arg1 = childExpression;
        }

        /// <summary>
        /// Removes the parent expression of this expression if the parent is unary.  New parent expression becomes the parent's parent, and old parent becomes detached.
        /// </summary>
        /// <param name="expression">The expression to remove the parent from</param>
        public static void RemoveUnaryParent(Expression expression)
        {
            if (expression == null)
            {
                return;
            }
            var parent = expression.Parent;
            if (parent != null)
            {
                int indexInParent = parent.Args.IndexOf(expression);

                var parent2 = parent.Parent;
                if (parent2 != null)
                {
                    int indexInParent2 = parent2.Args.IndexOf(parent);
                    parent2.Args[indexInParent2] = null;
                    parent.Args[indexInParent] = null;
                    parent2.Args[indexInParent2] = expression;
                }
                else
                {
                    parent.Args[indexInParent] = null;
                }
            }
        }

        /// <summary>
        /// Detaches a statement expression from the expression tree.
        /// </summary>
        /// <param name="expression"></param>
        public static void RemoveStatement(Expression expression)
        {
            if (expression == null)
            {
                return;
            }
            //is parent a statement?
            var parent = expression.Parent;
            if (parent != null)
            {
                int parentIndex = parent.Args.IndexOf(expression);

                if (parent.ExpressionType == Instruction.Statement)
                {
                    var statementToBypass = parent;
                    int exprToRemoveIndex = parentIndex;
                    int exprToUseIndex = exprToRemoveIndex ^ 1;

                    //bypass the statement
                    var parent2 = parent.Parent;
                    if (parent2 != null)
                    {
                        int index = parent2.Args.IndexOf(parent);
                        if (index != -1)
                        {
                            var exprToUse = statementToBypass.Args[exprToUseIndex];
                            statementToBypass.Args[exprToUseIndex] = null;
                            statementToBypass.Args[exprToRemoveIndex] = null;
                            parent2.Args[index] = null;
                            parent2.Args[index] = exprToUse;
                        }
                        else
                        {

                        }
                    }
                    else
                    {

                    }
                }
                else
                {
                    //remove from parent directly - this probably indicates a bug
                    if (parentIndex != -1)
                    {
                        parent.Args[parentIndex] = null;
                    }
                    else
                    {

                    }
                }
            }
            else
            {

            }
        }

        /// <summary>
        /// Returns the child expressions of this expression, excluding itself.
        /// </summary>
        /// <returns>The child expressions of this expression, excluding itself</returns>
        public IEnumerable<Expression> GetChildExpressions()
        {
            return GetChildExpressions(this);
        }

        /// <summary>
        /// Returns the child expressions of the expression, excluding itself.
        /// </summary>
        /// <param name="expression">The expression to get the child expressions from.</param>
        /// <returns>The child expressions of the expression, excluding itself</returns>
        public static IEnumerable<Expression> GetChildExpressions(Expression expression)
        {
            Expression endExpression = expression.GetParentNextExpression();
            while (expression != endExpression)
            {
                yield return expression;
                expression = expression.GetNextExpression();
            }
        }

        /// <summary>
        /// Returns the Next expression, which is the first child of this expression, or next child of a parent or ancestor expression.
        /// </summary>
        /// <returns>The next expression</returns>
        public Expression GetNextExpression()
        {
            return GetNextExpression(this);
        }

        /// <summary>
        /// Returns the next child of a parent's expression.
        /// </summary>
        /// <returns>The next child of a parent's expresion</returns>
        public Expression GetParentNextExpression()
        {
            return GetParentNextExpression(this);
        }

        /// <summary>
        /// Returns the Next expression, which is the first child of the expression, or next child of a parent or ancestor expression.
        /// </summary>
        /// <param name="expression">The expression to start at</param>
        /// <returns>The next expression</returns>
        private static Expression GetNextExpression(Expression expression)
        {
            if (expression == null)
            {
                return null;
            }
            for (int i = 0; i < expression.Args.Count; i++)
            {
                if (expression.Args[i] != null)
                {
                    return expression.Args[i];
                }
            }
            return GetParentNextExpression(expression);
        }

        /// <summary>
        /// Returns the next child of a parent's expression.
        /// </summary>
        /// <param name="expression">The expression to start at</param>
        /// <returns>The next child of a parent's expression</returns>
        private static Expression GetParentNextExpression(Expression expression)
        {
        Restart:
            if (expression == null)
            {
                return null;
            }
            var childExpression = expression;
            expression = expression.Parent;
            if (expression == null)
            {
                return expression;
            }
            int index = expression.Args.IndexOf(childExpression);

        Back1:
            if (index != -1)
            {
                index++;
                if (index < expression.Args.Count)
                {
                    if (expression.Args[index] == null)
                    {
                        goto Back1;
                    }
                    else
                    {
                        return expression.Args[index];
                    }
                }
            }
            goto Restart;
        }

        /// <summary>
        /// Returns the first expression after all cast expressions
        /// </summary>
        /// <returns>The first expression after all cast expressions</returns>
        public Expression SkipChildCastOperations()
        {
            return SkipChildCastOperations(this);
        }

        /// <summary>
        /// Returns the first expression after all cast expressions
        /// </summary>
        /// <param name="expression">The expression to skip the cast expressions of</param>
        /// <returns>Returns the first expression after all cast expressions</returns>
        private static Expression SkipChildCastOperations(Expression expression)
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

        /// <summary>
        /// Returns the first parent expression before all cast expressions 
        /// </summary>
        /// <returns>The first parent expression before all cast expressions</returns>
        public Expression SkipParentCastOperations()
        {
            return SkipParentCastOperations(this.parent);
        }

        /// <summary>
        /// Returns the first parent expression before all cast expressions 
        /// </summary>
        /// <param name="expression">The expression to skip the cast expressions of</param>
        /// <returns>The first parent expression before all cast expressions</returns>
        private static Expression SkipParentCastOperations(Expression expression)
        {
            if (expression == null)
            {
                return null;
            }
            if (expression.parent != null && expression.ExpressionType.IsCast())
            {
                expression = expression.parent;
            }
            return expression;
        }

        /// <summary>
        /// For a binary operator, returns the other side of the expression, otherwise returns null.
        /// </summary>
        /// <returns>The other side of the binary expression, or null</returns>
        public Expression GetOtherSideOfBinaryExpression()
        {
            return GetOtherSideOfBinaryExpression(this);
        }

        /// <summary>
        /// For a binary operator, returns the other side of the expression, otherwise returns null.
        /// </summary>
        /// <param name="expression">The child expression of the parent binary expression</param>
        /// <returns>The other side of the binary expression, or null</returns>
        private static Expression GetOtherSideOfBinaryExpression(Expression expression)
        {
            if (expression == null)
            {
                return null;
            }
            var parent = expression.Parent;
            while (parent != null && parent.ExpressionType.IsCast())
            {
                expression = parent;
                parent = expression.parent;
            }

            if (parent == null)
            {
                return null;
            }
            if (parent.ExpressionType.IsBinary() || parent.ExpressionType.IsAssignment())
            {
                int indexOfMe = parent.Args.IndexOf(expression);
                int indexOfOther = indexOfMe ^ 1;
                return parent.Args[indexOfOther];
            }
            return null;
        }



        /// <summary>
        /// Creates a new expression given the instruction and address.
        /// </summary>
        /// <param name="expressionType">The instruction</param>
        /// <param name="address">The address</param>
        public Expression(Instruction expressionType, int address) : this(expressionType, address, 0, 0, 0) { }
        /// <summary>
        /// Creates a new expression given the instruction, address, and an int value.
        /// </summary>
        /// <param name="expressionType">The instruction</param>
        /// <param name="address">The address</param>
        /// <param name="value">The int value</param>
        public Expression(Instruction expressionType, int address, int value) : this(expressionType, address, value, 0, 0) { }
        /// <summary>
        /// Creates a new expression given the instruction, address, and two int values.
        /// </summary>
        /// <param name="expressionType">The instruction</param>
        /// <param name="address">The address</param>
        /// <param name="value">The first int value</param>
        /// <param name="value2">The second int value</param>
        public Expression(Instruction expressionType, int address, int value, int value2) : this(expressionType, address, value, value2, 0) { }
        /// <summary>
        /// Creates a new expression given the instruction, address, and a subexpression.
        /// </summary>
        /// <param name="expressionType">The instruction</param>
        /// <param name="address">The address</param>
        /// <param name="expression">The subexpression</param>
        public Expression(Instruction expressionType, int address, Expression expression) : this(expressionType, address, 0, 0, 0, expression) { }
        /// <summary>
        /// Creates a new expression given the instruction, address, a subexpression, and an int value.
        /// </summary>
        /// <param name="expressionType">The instruction</param>
        /// <param name="address">The address</param>
        /// <param name="expression">The subexpression</param>
        /// <param name="value">The int value</param>
        public Expression(Instruction expressionType, int address, Expression expression, int value) : this(expressionType, address, value, 0, 0, expression) { }
        /// <summary>
        /// Creates a new expression given the instruction, address, a subexpression, and two int values.
        /// </summary>
        /// <param name="expressionType">The instruction</param>
        /// <param name="address">The address</param>
        /// <param name="expression">The subexpression</param>
        /// <param name="value">The first int value</param>
        /// <param name="value">The second int value</param>
        public Expression(Instruction expressionType, int address, Expression expression, int value, int value2) : this(expressionType, address, value, value2, 0, expression) { }
        /// <summary>
        /// Creates a new expression given the instruction, address, and two subexpressions.
        /// </summary>
        /// <param name="expressionType">The instruction</param>
        /// <param name="address">The address</param>
        /// <param name="expression">The first subexpression</param>
        /// <param name="expression">The second subexpression</param>
        public Expression(Instruction expressionType, int address, Expression expression, Expression expression2) : this(expressionType, address, 0, 0, 0, expression, expression2) { }
        /// <summary>
        /// Creates a new expression given the instruction, address, and three subexpressions.
        /// </summary>
        /// <param name="expressionType">The instruction</param>
        /// <param name="address">The address</param>
        /// <param name="expression">The first subexpression</param>
        /// <param name="expression2">The second subexpression</param>
        /// <param name="expression3">The third subexpression</param>
        public Expression(Instruction expressionType, int address, Expression expression, Expression expression2, Expression expression3) : this(expressionType, address, 0, 0, 0, expression, expression2, expression3) { }
        /// <summary>
        /// Creates a new expression given the instruction, address, two subexpressions, and an int value.
        /// </summary>
        /// <param name="expressionType">The instruction</param>
        /// <param name="address">The address</param>
        /// <param name="expression">The first subexpression</param>
        /// <param name="expression2">The second subexpression</param>
        /// <param name="value">The int value</param>
        public Expression(Instruction expressionType, int address, Expression expression, Expression expression2, int value) : this(expressionType, address, value, 0, 0, expression, expression2) { }
        /// <summary>
        /// Creates a new expression given the instruction, address, and any number of subexpressions.
        /// </summary>
        /// <param name="expressionType">The instruction</param>
        /// <param name="address">The address</param>
        /// <param name="expressions"></param>
        public Expression(Instruction expressionType, int address, params Expression[] expressions) : this(expressionType, address, 0, 0, 0, expressions) { }
        /// <summary>
        /// Creates a new expression given the instruction, address, an int value, and any number of subexpressions.
        /// </summary>
        /// <param name="expressionType">The instruction</param>
        /// <param name="address">The address</param>
        /// <param name="value">The int value</param>
        /// <param name="expressions">Any number of subexpressions</param>
        public Expression(Instruction expressionType, int address, int value, params Expression[] expressions) : this(expressionType, address, value, 0, 0, expressions) { }

        /// <summary>
        /// Part of the clone operation, copies the fields of an expression so it can be cloned.  (Fields are instruction, address, original file, function number, and int values)  Does not copy subexpressions here.
        /// </summary>
        /// <param name="destination">The destination object have the fields be assigned to</param>
        /// <param name="source">The source object to have the fields read from</param>
        private static void CopyFields(Expression destination, Expression source)
        {
            destination._expressionType = source._expressionType;
            destination.address = source.address;
            destination.ainFile = source.ainFile;
            destination.functionNumber = source.functionNumber;
            destination.Value = source.Value;
            destination.Value2 = source.Value2;
            destination.Value3 = source.Value3;
            destination.removeIfVoid = source.removeIfVoid;
        }

        /// <summary>
        /// Part of the clone operation, copies the fields of an expression, then creates the same number of blank subexpressions, these get overwritten later.
        /// </summary>
        /// <param name="destination">The expression to copy the fields and create empty subexpressions inside</param>
        /// <param name="source">The source expression to get the fields and subexpression count from</param>
        private static void CloneExpressionStep(Expression destination, Expression source)
        {
            CopyFields(destination, source);
            for (int i = 0; i < source.Args.Count; i++)
            {
                if (source.Args[i] != null)
                {
                    destination.Args.Add(new Expression());
                }
                else
                {
                    destination.Args.Add(null);
                }
            }
        }

        /// <summary>
        /// Clones an expression tree, including the fields and subexpressions.
        /// </summary>
        /// <param name="source">The expression tree to clone.</param>
        /// <returns>A copy of the expression tree.</returns>
        public static Expression CloneExpression(Expression source)
        {
            var finalExpression = new Expression();
            var outputExpression = finalExpression;
            var inputExpression = source;
            while (inputExpression != null)
            {
                CloneExpressionStep(outputExpression, inputExpression);
                inputExpression = inputExpression.GetNextExpression();
                outputExpression = outputExpression.GetNextExpression();
            }
            return finalExpression;
        }

        /// <summary>
        /// Clones an expression tree, including the fields and subexpressions.
        /// </summary>
        /// <returns>A copy of the expression tree.</returns>
        public Expression Clone()
        {
            return CloneExpression(this);
        }
    }
}
