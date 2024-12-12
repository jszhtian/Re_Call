using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Text;
using System.Diagnostics;

namespace AinDecompiler.Compiler
{
    public partial class Compiler
    {
        public Compiler()
        {
            ainFile = new AinFile();
            this.Functions = new ListAndDictionary<Function>(Symbols, ainFile, ainFile.Functions, ainFile.FunctionNameToIndex);
            this.Globals = new ListAndDictionary<Global>(Symbols, ainFile, ainFile.Globals, ainFile.GlobalNameToIndex);
            this.Constants = new ListAndDictionary<Variable>(Symbols, ainFile);
            this.Structs = new ListAndDictionary<Struct>(Symbols, ainFile, ainFile.Structs, ainFile.StructNameToIndex);
            this.Functypes = new ListAndDictionary<FunctionType>(Symbols, ainFile, ainFile.FunctionTypes, ainFile.FunctionNameToIndex);
            this.Delegates = new ListAndDictionary<FunctionType>(Symbols, ainFile, ainFile.Delegates);
            this.Libraries = new ListAndDictionary<HllLibrary>(Symbols, ainFile, ainFile.Libraries);
            this.Strings = new StringListAndDictionary(ainFile.Strings);
            this.Messages = new StringListAndDictionary(ainFile.Messages);
        }

        public void Test()
        {
            this.Strings.Add("");
            this.Functions.Get("NULL");
            this.Messages.Add("");


            var lastDefaultAinFile = Expression.defaultAinFile;
            Expression.defaultAinFile = this.ainFile;

            //List<TokenExpression> tokens = new List<TokenExpression>();
            ////string expression = "a - -obj.method()()[4] - -3;";
            ////string expression = "- -a - -obj.method()()[4] - -3;";
            ////string expression = "- -a - -obj.method()(-9*-8 + 5)[4 == 4 + 5 == 4] - -3;";
            ////string expression = "- -a - -obj.method()(-9*-8 + 5)[4 == 4 + 5 == 4] - -3;";
            //IncludeString(expression, tokens);
            //this.reader = new TokenReader(tokens);
            //var exp = ReadExpression();

            string filename = @"C:\temp\Sys42SDK_20041224\入門者向けセット\ポポリタン\開発環境\COPY\out.txt";
            //string filename = @"C:\temp\temp2\games\AliceSoft\Sengoku Rance\DECOMPILED\project.jaf";
            List<TokenExpression> tokens = new List<TokenExpression>();
            IncludeFile(filename, tokens);

            var firstPass = new FirstPass();

            var result = firstPass.CompileFirstPass(tokens);
            CreateSymbols(result);
            GetInitialValues(result);
            CompileSecondPass();
            GenerateExpressions();

            Expression.defaultAinFile = lastDefaultAinFile;

            //todo: fix _date_ macro, needs spaces instead of leading zeroes
        }

        private void GetInitialValues(List<TokenExpression> result)
        {
            var secondPass = new SecondPass(this);
            secondPass.GetInitialValues(result);
        }

        VariableMultiDictionary scope = new VariableMultiDictionary();

        Dictionary<string, IVariable> localScope;
        Dictionary<string, Dictionary<string, IVariable>> ClassNameToMembers = new Dictionary<string, Dictionary<string, IVariable>>();

        Dictionary<string, IVariable> GetClassMembers(string className)
        {
            if (String.IsNullOrEmpty(className))
            {
                return null;
            }
            if (ClassNameToMembers.ContainsKey(className))
            {
                return ClassNameToMembers[className];
            }
            if (this.Structs.Contains(className))
            {
                Dictionary<string, IVariable> results = new Dictionary<string, IVariable>();
                var structInfo = this.Structs.Get(className);
                string classNameAt = className + "@";
                var methods = this.Functions.List.Where(f => f.Name.StartsWith(classNameAt, StringComparison.Ordinal)).ToArray();
                foreach (var method in methods)
                {
                    results.Add(method.GetFunctionName(), method);
                }
                foreach (var member in structInfo.Members)
                {
                    results.Add(member.Name, member);
                }
                ClassNameToMembers[className] = results;
                return results;
            }
            else
            {
                ClassNameToMembers[className] = null;
                return null;
            }
        }

        private void GenerateExpressions()
        {
            scope.PushDictionary(Symbols);
            foreach (var pair in this.FunctionDefinitionTokens)
            {
                var function = pair.Key;
                var token = pair.Value;

                var classScope = GetClassMembers(function.GetClassName());
                var localScope = new Dictionary<string, IVariable>();

                var localVars = function.Parameters;
                foreach (var localvar in localVars)
                {
                    localScope[localvar.Name] = localvar;
                }

                if (classScope != null)
                {
                    scope.PushDictionary(classScope);
                }
                scope.PushDictionary(localScope);
                Expression.defaultAinFile = this.ainFile;
                Expression.defaultFunctionNumber = function.Index;
                Expression expr = GenerateExpression(token);
                scope.PopDictionary();
                if (classScope != null)
                {
                    scope.PopDictionary();
                }
            }
            scope.PopDictionary();
        }

        #region Operator Tables
        static Dictionary<TokenType, Instruction> IntegerAssignmentOperators = GetIntegerAssignmentOperators();
        static Dictionary<TokenType, Instruction> IntegerBinaryOperators = GetIntegerBinaryOperators();
        static Dictionary<TokenType, Instruction> IntegerUnaryOperators = GetIntegerUnaryOperators();
        static Dictionary<TokenType, Instruction> IntegerUnaryAssignmentOperators = GetIntegerUnaryAssignmentOperators();
        static Dictionary<Instruction, Instruction> FloatInstructions = GetFloatInstructions();
        static Dictionary<Instruction, Instruction> LintInstructions = GetLintInstructions();
        static Dictionary<Instruction, Instruction> StringInstructions = GetStringInstructions();

        private static Dictionary<T1, T2> BuildDictionary<T1, T2>(IList<T1> keys, IList<T2> values)
        {
            if (keys.Count != values.Count)
            {
                throw new ArgumentException("Sizes of arrays don't match!");
            }
            Dictionary<T1, T2> dic = new Dictionary<T1, T2>(keys.Count);
            for (int i = 0; i < keys.Count; i++)
            {
                var key = keys[i];
                var value = values[i];
                dic.Add(key, value);
            }
            return dic;
        }

        private static Dictionary<TokenType, Instruction> GetIntegerAssignmentOperators()
        {
            TokenType[] tokens = new TokenType[]
            {
                TokenType.AndAssign, TokenType.Assign, TokenType.DivideAssign, TokenType.LeftShiftAssign, 
                TokenType.MinusAssign, TokenType.ModuloAssign, TokenType.MultiplyAssign, TokenType.PlusAssign, 
                TokenType.RightShiftAssign, TokenType.XorAssign
            };
            Instruction[] instructions = new Instruction[]
            {
                Instruction.ANDA, Instruction.ASSIGN, Instruction.DIVA, Instruction.LSHIFTA,
                Instruction.MINUSA, Instruction.MODA, Instruction.MULA, Instruction.PLUSA,
                Instruction.RSHIFTA, Instruction.XORA
            };
            return BuildDictionary(tokens, instructions);
        }

        private static Dictionary<TokenType, Instruction> GetIntegerBinaryOperators()
        {
            TokenType[] tokens = new TokenType[]
            {
                TokenType.And, TokenType.Divide, TokenType.EqualTo, TokenType.GreaterThan,
                TokenType.GreaterThanOrEqualTo, TokenType.LeftShift, TokenType.LessThan, TokenType.LessThanOrEqualTo,
                TokenType.LogicalAnd, TokenType.LogicalOr, TokenType.Minus, TokenType.Modulo,
                TokenType.Multiply, TokenType.NotEqualTo, TokenType.Or, TokenType.Plus,
                TokenType.RightShift, TokenType.Xor
            };
            Instruction[] instructions = new Instruction[]
            {
                Instruction.AND, Instruction.DIV, Instruction.EQUALE, Instruction.GT,
                Instruction.GTE, Instruction.LSHIFT, Instruction.LT, Instruction.LTE,
                Instruction.LogicalAnd, Instruction.LogicalOr, Instruction.SUB, Instruction.MOD,
                Instruction.MUL, Instruction.NOTE, Instruction.OR, Instruction.ADD,
                Instruction.RSHIFT, Instruction.XOR
            };
            return BuildDictionary(tokens, instructions);
        }

        private static Dictionary<TokenType, Instruction> GetIntegerUnaryOperators()
        {
            TokenType[] tokens = new TokenType[]
            {
                TokenType.Complement, TokenType.Negative, TokenType.Not, TokenType.Positive
            };
            Instruction[] instructions = new Instruction[]
            {
                Instruction.COMPL, Instruction.INV, Instruction.NOT, Instruction.Nop
            };
            return BuildDictionary(tokens, instructions);
        }

        private static Dictionary<TokenType, Instruction> GetIntegerUnaryAssignmentOperators()
        {
            TokenType[] tokens = new TokenType[]
            {
                TokenType.PostDecrement, TokenType.PostIncrement, TokenType.PreDecrement, TokenType.PreIncrement
            };
            Instruction[] instructions = new Instruction[]
            {
                Instruction.DEC, Instruction.INC, Instruction.PreDecrement, Instruction.PreIncrement
            };
            return BuildDictionary(tokens, instructions);
        }

        private static Dictionary<Instruction, Instruction> GetFloatInstructions()
        {
            Instruction[] floatInstructions = new Instruction[]
            {
                Instruction.F_ADD, Instruction.F_ASSIGN, Instruction.F_DIV, Instruction.F_DIVA,
                Instruction.F_EQUALE, Instruction.F_GT, Instruction.F_GTE, Instruction.F_INV,
                Instruction.F_LT, Instruction.F_LTE, Instruction.F_MINUSA, Instruction.F_MUL,
                Instruction.F_MULA, Instruction.F_NOTE, Instruction.F_PLUSA, Instruction.F_PUSH,
                Instruction.F_SUB,
            };
            Instruction[] intInstructions = new Instruction[]
            {
                Instruction.ADD, Instruction.ASSIGN, Instruction.DIV, Instruction.DIVA,
                Instruction.EQUALE, Instruction.GT, Instruction.GTE, Instruction.INV,
                Instruction.LT, Instruction.LTE, Instruction.MINUSA, Instruction.MUL,
                Instruction.MULA, Instruction.NOTE, Instruction.PLUSA, Instruction.PUSH,
                Instruction.SUB,
            };
            return BuildDictionary(intInstructions, floatInstructions);
        }

        private static Dictionary<Instruction, Instruction> GetLintInstructions()
        {
            Instruction[] lintInstructions = new Instruction[]
            {
                Instruction.LI_ADD, Instruction.LI_ANDA, Instruction.LI_ASSIGN, Instruction.LI_DEC,
                Instruction.LI_DIV, Instruction.LI_DIVA, Instruction.LI_INC, Instruction.LI_LSHIFTA,
                Instruction.LI_MINUSA, Instruction.LI_MOD, Instruction.LI_MODA, Instruction.LI_MUL,
                Instruction.LI_MULA, Instruction.LI_ORA, Instruction.LI_PLUSA, Instruction.LI_RSHIFTA,
                Instruction.LI_SUB, Instruction.LI_XORA
            };
            Instruction[] intInstructions = new Instruction[]
            {
                Instruction.ADD, Instruction.ANDA, Instruction.ASSIGN, Instruction.DEC,
                Instruction.DIV, Instruction.DIVA, Instruction.INC, Instruction.LSHIFTA,
                Instruction.MINUSA, Instruction.MOD, Instruction.MODA, Instruction.MUL,
                Instruction.MULA, Instruction.ORA, Instruction.PLUSA, Instruction.RSHIFTA,
                Instruction.SUB, Instruction.XORA
            };
            return BuildDictionary(intInstructions, lintInstructions);
        }

        private static Dictionary<Instruction, Instruction> GetStringInstructions()
        {
            Instruction[] lintInstructions = new Instruction[]
            {
                Instruction.S_ADD, Instruction.S_ASSIGN, Instruction.S_EQUALE, Instruction.S_GT,
                Instruction.S_GTE, Instruction.S_LT, Instruction.S_LTE, Instruction.S_MOD,
                Instruction.S_NOTE, Instruction.S_PLUSA, Instruction.S_PUSH
            };
            Instruction[] intInstructions = new Instruction[]
            {
                Instruction.ADD, Instruction.ASSIGN, Instruction.EQUALE, Instruction.GT,
                Instruction.GTE, Instruction.LT, Instruction.LTE, Instruction.MOD,
                Instruction.NOTE, Instruction.PLUSA, Instruction.PUSH
            };
            return BuildDictionary(intInstructions, lintInstructions);
        }
        #endregion

        private Expression GenerateExpression(TokenExpression token)
        {
            if (token == null) return null;
            var tokenType = token.TokenType;
            switch (tokenType)
            {
                case TokenType.Block:
                    {
                        var oldScope = localScope;
                        localScope = new Dictionary<string, IVariable>();
                        scope.PushDictionary(localScope);

                        List<Expression> statementList = new List<Expression>();


                        //Expression lastStatement = null;
                        //Expression firstStatement = null;
                        foreach (var statementToken in token.Subexpressions)
                        {
                            Expression expr = GenerateExpression(statementToken);
                            if (expr != null)
                            {
                                statementList.Add(expr);
                            }
                        }
                        scope.PopDictionary();
                        localScope = oldScope;
                        return ConvertToStatements(statementList);
                    }
                    break;
                case TokenType.Identifier:
                    {
                        string symbolName = token.Token.Value;
                        var symbol = scope.GetOrNull(symbolName);
                        return SymbolToExpression(symbol, symbolName);
                    }
                case TokenType.Dot:
                    {
                        return GenerateDot(token);
                    }
                    break;

                case TokenType.FunctionCall:
                    {
                        var functionArgumentsToken = token.Subexpressions.GetOrNull(1);
                        Expression functionCallExpression = null;
                        IFunction functionToCall = null;

                        var functionToken = token.Subexpressions.GetOrNull(0);
                        if (functionToken == null)
                        {
                            return null;
                        }
                        if (functionToken.TokenType == TokenType.Identifier)
                        {
                            functionCallExpression = GenerateExpression(functionToken);
                            if (functionCallExpression != null)
                            {
                                functionToCall = functionCallExpression.Variable as IFunction;
                            }
                        }
                        else if (functionToken.TokenType == TokenType.Dot)
                        {
                            functionCallExpression = GenerateExpression(functionToken);
                            if (functionCallExpression != null)
                            {
                                functionToCall = functionCallExpression.Variable as IFunction;
                            }
                        }
                        else
                        {

                        }

                        if (functionToCall != null)
                        {
                            int parameterCount = functionToCall.ParameterCount;
                            if (parameterCount > 0)
                            {
                                int argsIndex = 0;
                                if (functionCallExpression.ExpressionType == Instruction.CALLFUNC2)
                                {
                                    argsIndex = 2;
                                }


                                var objectParameter = functionCallExpression.Args.GetOrNull(argsIndex);

                                var parametersExpression = GenerateFunctionParameters(functionToCall, functionArgumentsToken);
                                if (objectParameter != null)
                                {
                                    if (parametersExpression == null)
                                    {
                                        parametersExpression = objectParameter;
                                    }
                                    else
                                    {
                                        parametersExpression = new Expression(Instruction.Comma, -1, objectParameter, parametersExpression);
                                    }
                                }
                                functionCallExpression.Args.SetOrAdd(argsIndex, parametersExpression);
                            }
                        }
                        else
                        {
                            //TODO
                        }
                        return functionCallExpression;
                    }
                    break;
                case TokenType.VariableDeclaration:
                    {
                        var function = ainFile.GetFunction(Expression.defaultFunctionNumber);
                        var dataTypeToken = token.Subexpressions.GetOrNull(0);
                        var dataType = GetDataType(dataTypeToken);
                        List<Expression> assignStatements = new List<Expression>();

                        for (int i = 1; i < token.Subexpressions.Count; i++)
                        {
                            var variableToken = token.Subexpressions[i];
                            TokenExpression assignToken = null;
                            if (variableToken.TokenType == TokenType.Assign)
                            {
                                assignToken = variableToken;
                                variableToken = variableToken.Subexpressions[0];
                            }

                            var variable = dataType.Clone();
                            variable.Name = variableToken.Token.Value;
                            if (localScope.ContainsKey(variable.Name))
                            {
                                //error: duplicate variable name
                            }
                            scope.Add(variable.Name, variable);
                            int newIndex = function.Parameters.Count;
                            function.Parameters.Add(variable);
                            variable.Index = newIndex;
                            variable.Parent = function;
                            variable.Root = ainFile;

                            //TODO: generate code for creating structs or arrays
                            if (variable.DataType == DataType.Struct)
                            {
                                assignStatements.Add(new Expression(Instruction.SH_LOCALDELETE, -1, variable.Index));
                                assignStatements.Add(new Expression(Instruction.SH_LOCALCREATE, -1, variable.Index, variable.StructType));

                            }

                            if (assignToken != null)
                            {
                                var assignmentExpression = GenerateExpression(assignToken);
                                assignStatements.Add(assignmentExpression);
                            }

                        }
                        return ConvertToStatements(assignStatements);
                    }
                    break;
                case TokenType.For:
                    {
                        var t1 = token.Subexpressions.GetOrNull(0);
                        var t2 = token.Subexpressions.GetOrNull(1);
                        var t3 = token.Subexpressions.GetOrNull(2);

                        Expression e1 = null, e2 = null, e3 = null;

                        if (t1 != null)
                        {
                            e1 = GenerateExpression(t1);
                        }
                        if (t2 != null)
                        {
                            e2 = GenerateExpression(t2);
                        }
                        if (t3 != null)
                        {
                            e3 = GenerateExpression(t3);
                        }
                        var blockToken = token.Subexpressions.GetOrNull(3);
                        var blockExpression = GenerateExpression(blockToken);

                        Expression forLoop = new Expression(Instruction.ForLoop, -1, e1, e2, e3, blockExpression);
                        return forLoop;
                    }
                    break;
                case TokenType.Assign:
                    {
                        var leftSide = token.Subexpressions.GetOrNull(0);
                        var rightSide = token.Subexpressions.GetOrNull(1);

                        var leftExpression = GenerateExpression(leftSide);
                        var rightExpression = GenerateExpression(rightSide);
                        //TODO, type checking and conversion
                        if (leftExpression.ExpressionType == Instruction.REF)
                        {
                            leftExpression.ExpressionType = Instruction.AssignmentRef;
                            var assignmentExpression = new Expression(Instruction.ASSIGN, -1, leftExpression, rightExpression);
                            return assignmentExpression;
                        }
                        else if (leftExpression.ExpressionType == Instruction.S_REF)
                        {
                            leftExpression.ExpressionType = Instruction.AssignmentRef;
                            var assignmentExpression = new Expression(Instruction.S_ASSIGN, -1, leftExpression, rightExpression);
                            return assignmentExpression;
                        }
                    }
                    break;
                case TokenType.Number:
                    {
                        if (token.Token.IsFloat())
                        {
                            return new Expression(Instruction.F_PUSH, -1, token.Token.ToFloat().FloatToInt());
                        }
                        else
                        {
                            return new Expression(Instruction.PUSH, -1, token.Token.ToInt());
                        }
                    }
                    break;
                case TokenType.If:
                    {
                        var t1 = token.Subexpressions.GetOrNull(0); //expression
                        var t2 = token.Subexpressions.GetOrNull(1); //ifbody
                        var t3 = token.Subexpressions.GetOrNull(2); //else

                        var e1 = GenerateExpression(t1);
                        var e2 = GenerateExpression(t2);
                        var e3 = GenerateExpression(t3);

                        return new Expression(Instruction.IfElse, -1, e1, e2, e3);
                    }
                    break;
                /*
            //left-associative binary operators
            case TokenType.And:
                {
                    var t1 = token.Subexpressions.GetOrNull(0);
                    var t2 = token.Subexpressions.GetOrNull(1);

                    var e1 = GenerateExpression(t1);
                    var e2 = GenerateExpression(t2);

                    //TODO: type checking
                    return new Expression(Instruction.AND, -1, e1, e2);
                }
            case TokenType.Divide:
                {
                    var t1 = token.Subexpressions.GetOrNull(0);
                    var t2 = token.Subexpressions.GetOrNull(1);

                    var e1 = GenerateExpression(t1);
                    var e2 = GenerateExpression(t2);

                    //TODO: type checking
                    return new Expression(Instruction.DIV, -1, e1, e2);
                }
            case TokenType.EqualTo:
                {
                    var t1 = token.Subexpressions.GetOrNull(0);
                    var t2 = token.Subexpressions.GetOrNull(1);

                    var e1 = GenerateExpression(t1);
                    var e2 = GenerateExpression(t2);

                    //TODO: type checking
                    return new Expression(Instruction.EQUALE, -1, e1, e2);
                }
            case TokenType.LeftShift:
                {
                    var t1 = token.Subexpressions.GetOrNull(0);
                    var t2 = token.Subexpressions.GetOrNull(1);

                    var e1 = GenerateExpression(t1);
                    var e2 = GenerateExpression(t2);

                    //TODO: type checking
                    return new Expression(Instruction.LSHIFT, -1, e1, e2);
                }
            case TokenType.LessThan:
                {
                    var t1 = token.Subexpressions.GetOrNull(0);
                    var t2 = token.Subexpressions.GetOrNull(1);

                    var e1 = GenerateExpression(t1);
                    var e2 = GenerateExpression(t2);

                    //TODO: type checking
                    return new Expression(Instruction.LT, -1, e1, e2);
                }
            case TokenType.LessThanOrEqualTo:
                {
                    var t1 = token.Subexpressions.GetOrNull(0);
                    var t2 = token.Subexpressions.GetOrNull(1);

                    var e1 = GenerateExpression(t1);
                    var e2 = GenerateExpression(t2);

                    //TODO: type checking
                    return new Expression(Instruction.LTE, -1, e1, e2);
                }
            case TokenType.LogicalAnd:
                {
                    var t1 = token.Subexpressions.GetOrNull(0);
                    var t2 = token.Subexpressions.GetOrNull(1);

                    var e1 = GenerateExpression(t1);
                    var e2 = GenerateExpression(t2);

                    //TODO: type checking
                    return new Expression(Instruction.LogicalAnd, -1, e1, e2);
                }
            case TokenType.LogicalOr:
                {
                    var t1 = token.Subexpressions.GetOrNull(0);
                    var t2 = token.Subexpressions.GetOrNull(1);

                    var e1 = GenerateExpression(t1);
                    var e2 = GenerateExpression(t2);

                    //TODO: type checking
                    return new Expression(Instruction.LogicalOr, -1, e1, e2);
                }
            case TokenType.GreaterThan:
                {
                    var t1 = token.Subexpressions.GetOrNull(0);
                    var t2 = token.Subexpressions.GetOrNull(1);

                    var e1 = GenerateExpression(t1);
                    var e2 = GenerateExpression(t2);

                    //TODO: type checking
                    return new Expression(Instruction.GT, -1, e1, e2);
                }
            case TokenType.GreaterThanOrEqualTo:
                {
                    var t1 = token.Subexpressions.GetOrNull(0);
                    var t2 = token.Subexpressions.GetOrNull(1);

                    var e1 = GenerateExpression(t1);
                    var e2 = GenerateExpression(t2);

                    //TODO: type checking
                    return new Expression(Instruction.GTE, -1, e1, e2);
                }
            case TokenType.Minus:
                {
                    var t1 = token.Subexpressions.GetOrNull(0);
                    var t2 = token.Subexpressions.GetOrNull(1);

                    var e1 = GenerateExpression(t1);
                    var e2 = GenerateExpression(t2);

                    //TODO: type checking
                    return new Expression(Instruction.SUB, -1, e1, e2);
                }
            case TokenType.Modulo:
                {
                    var t1 = token.Subexpressions.GetOrNull(0);
                    var t2 = token.Subexpressions.GetOrNull(1);

                    var e1 = GenerateExpression(t1);
                    var e2 = GenerateExpression(t2);

                    //TODO: type checking
                    return new Expression(Instruction.MOD, -1, e1, e2);
                }
            case TokenType.Multiply:
                {
                    var t1 = token.Subexpressions.GetOrNull(0);
                    var t2 = token.Subexpressions.GetOrNull(1);

                    var e1 = GenerateExpression(t1);
                    var e2 = GenerateExpression(t2);

                    //TODO: type checking
                    return new Expression(Instruction.MUL, -1, e1, e2);
                }
            case TokenType.NotEqualTo:
                {
                    var t1 = token.Subexpressions.GetOrNull(0);
                    var t2 = token.Subexpressions.GetOrNull(1);

                    var e1 = GenerateExpression(t1);
                    var e2 = GenerateExpression(t2);

                    //TODO: type checking
                    return new Expression(Instruction.NOTE, -1, e1, e2);
                }
            case TokenType.Or:
                {
                    var t1 = token.Subexpressions.GetOrNull(0);
                    var t2 = token.Subexpressions.GetOrNull(1);

                    var e1 = GenerateExpression(t1);
                    var e2 = GenerateExpression(t2);

                    //TODO: type checking
                    return new Expression(Instruction.OR, -1, e1, e2);
                }
            case TokenType.Plus:
                {
                    var t1 = token.Subexpressions.GetOrNull(0);
                    var t2 = token.Subexpressions.GetOrNull(1);

                    var e1 = GenerateExpression(t1);
                    var e2 = GenerateExpression(t2);

                    //TODO: type checking
                    return new Expression(Instruction.ADD, -1, e1, e2);
                }
            case TokenType.RightShift:
                {
                    var t1 = token.Subexpressions.GetOrNull(0);
                    var t2 = token.Subexpressions.GetOrNull(1);

                    var e1 = GenerateExpression(t1);
                    var e2 = GenerateExpression(t2);

                    //TODO: type checking
                    return new Expression(Instruction.RSHIFT, -1, e1, e2);
                }
            case TokenType.Xor:
                {
                    var t1 = token.Subexpressions.GetOrNull(0);
                    var t2 = token.Subexpressions.GetOrNull(1);

                    var e1 = GenerateExpression(t1);
                    var e2 = GenerateExpression(t2);

                    //TODO: type checking
                    return new Expression(Instruction.XOR, -1, e1, e2);
                }
            case TokenType.Complement:
                {
                    var t1 = token.Subexpressions.GetOrNull(0);
                    var e1 = GenerateExpression(t1);
                    //TODO: type checking
                    return new Expression(Instruction.COMPL, -1, e1);
                }
            case TokenType.Negative:
                {
                    var t1 = token.Subexpressions.GetOrNull(0);
                    var e1 = GenerateExpression(t1);
                    //TODO: type checking
                    return new Expression(Instruction.INV, -1, e1);
                }
            case TokenType.Not:
                {
                    var t1 = token.Subexpressions.GetOrNull(0);
                    var e1 = GenerateExpression(t1);
                    //TODO: type checking
                    return new Expression(Instruction.NOT, -1, e1);
                }
            case TokenType.Positive:
                {
                    var t1 = token.Subexpressions.GetOrNull(0);
                    var e1 = GenerateExpression(t1);
                    //TODO: type checking
                    return e1;
                }
                */

                case TokenType.Break:
                    return new Expression(Instruction.Break, -1);
                case TokenType.Continue:
                    return new Expression(Instruction.Continue, -1);

                case TokenType.Return:
                    {
                        var t1 = token.Subexpressions.GetOrNull(0);
                        var e1 = GenerateExpression(t1);
                        //TODO: type checking
                        return new Expression(Instruction.RETURN, -1, e1);
                    }
                    break;

                case TokenType.StringLiteral:
                    {
                        return new Expression(Instruction.S_PUSH, -1, Strings.Add(token.Token.Value));
                    }
                case TokenType.CharLiteral:
                    {
                        int charValue = token.Token.Value[0];
                        if (charValue < 128)
                        {
                            return new Expression(Instruction.PUSH, -1, charValue);
                        }
                        else
                        {
                            byte[] bytes = Extensions.BinaryEncoding.GetBytes(token.Token.Value);
                            if (bytes.Length >= 2)
                            {
                                charValue = bytes[0] + (bytes[1] << 8);
                            }
                            else
                            {
                                charValue = bytes[0];
                            }
                            return new Expression(Instruction.PUSH, -1, charValue);
                        }
                    }
                    break;
                case TokenType.Switch:
                    {
                        var t1 = token.Subexpressions.GetOrNull(0);
                        var t2 = token.Subexpressions.GetOrNull(1);

                        var e1 = GenerateExpression(t1);
                        var e2 = GenerateExpression(t2);

                        return new Expression(Instruction.SWITCH, -1, e1, e2);
                    }
                    break;
                case TokenType.Case:
                    {
                        var t1 = token.Subexpressions.GetOrNull(0);
                        if (t1.TokenType == TokenType.StringLiteral)
                        {
                            return new Expression(Instruction.SwitchCase, -1, Strings.Add(t1.Token.Value));
                        }
                        else
                        {
                            var e1 = GenerateExpression(t1);
                            if (e1.ExpressionType != Instruction.PUSH)
                            {

                            }
                            return new Expression(Instruction.SwitchCase, -1, e1.Value);
                        }
                    }
                    break;
                case TokenType.Jump:
                    {
                        var t1 = token.Subexpressions.GetOrNull(0);
                        var e1 = GenerateExpression(t1);
                        if (e1 != null)
                        {
                            var f1 = e1.Variable as Function;
                            if (f1 != null && f1.IsLabel != 0)
                            {
                                var e = new Expression(Instruction.S_PUSH, -1, Strings.Add(f1.Name));
                                var e2 = new Expression(Instruction.CALLONJUMP, -1, e);
                                var e3 = new Expression(Instruction.SJUMP, -1, e2);
                                return e3;
                            }
                            else
                            {

                            }
                        }
                        else
                        {

                        }
                        return null;
                    }
                    break;
                case TokenType.Default:
                    {
                        return new Expression(Instruction.DefaultSwitchCase, -1);
                    }
                    break;
                /*
            case TokenType.PreIncrement:
            case TokenType.PreDecrement:
            case TokenType.PostIncrement:
            case TokenType.PostDecrement:
                {
                    throw new NotImplementedException();
                }
                */
                case TokenType.While:
                    {
                        var tExpression = token.Subexpressions.GetOrNull(0);
                        var tBody = token.Subexpressions.GetOrNull(1);

                        var eExpression = GenerateExpression(tExpression);
                        var eBody = GenerateExpression(tBody);

                        var whileExpression = new Expression(Instruction.WhileLoop, -1, eExpression, eBody);
                    }
                    break;
                case TokenType.AddressOf:
                    {
                        var t1 = token.Subexpressions.GetOrNull(0);
                        var e1 = GenerateExpression(t1);
                        if (e1.ExpressionType == Instruction.CALLFUNC)
                        {
                            e1.ExpressionType = Instruction.PUSH;
                            return e1;
                        }

                    }
                    break;
                case TokenType.Message:
                    {
                        return new Expression(Instruction.MSG, -1, Messages.Add(token.Token.Value));
                    }
                    break;
                case TokenType.ArrayIndex:
                    {
                        var t1 = token.Subexpressions.GetOrNull(0);
                        var t2 = token.Subexpressions.GetOrNull(1);
                        var e1 = GenerateExpression(t1);
                        var e2 = GenerateExpression(t2);

                        var v1 = e1.Variable;
                        var v2 = e2.Variable;

                        if (v2.DataType.IsInteger())
                        {
                            if (v1.DataType.IsArray())
                            {
                                var e = new Expression(Instruction.REF, -1, e1, e2);
                                //GetRefType(e.Variable, ref e);
                                return e;
                            }
                            else if (v1.DataType.IsString())
                            {

                            }
                            else
                            {
                                //error?
                            }
                        }
                        else
                        {
                            //error?
                        }
                        return null;

                    }
                    break;
                default:
                    {
                        if (IntegerAssignmentOperators.ContainsKey(tokenType))
                        {
                            var t1 = token.Subexpressions.GetOrNull(0);
                            var t2 = token.Subexpressions.GetOrNull(1);

                            var e1 = GenerateExpression(t1);
                            var e2 = GenerateExpression(t2);

                            var v1 = e1.Variable;
                            var v2 = e2.Variable;

                            if (v1.DataType != DataType.Int || v2.DataType != DataType.Int)
                            {

                            }

                            var instruction = IntegerAssignmentOperators[tokenType];

                            if (e1.ExpressionType == Instruction.REF || e1.ExpressionType == Instruction.S_REF)
                            {
                                e1.ExpressionType = Instruction.AssignmentRef;
                            }
                            else
                            {

                            }

                            var e = new Expression(instruction, -1, e1, e2);
                            return e;
                        }
                        else if (IntegerBinaryOperators.ContainsKey(tokenType))
                        {
                            var t1 = token.Subexpressions.GetOrNull(0);
                            var t2 = token.Subexpressions.GetOrNull(1);

                            var e1 = GenerateExpression(t1);
                            var e2 = GenerateExpression(t2);

                            var v1 = e1.Variable;
                            var v2 = e2.Variable;

                            var instruction = IntegerBinaryOperators[tokenType];
                            if (v1.DataType != DataType.Int || v2.DataType != DataType.Int)
                            {
                                if (v1.DataType != DataType.Bool || v2.DataType != DataType.Bool)
                                {
                                    if (v1.DataType == DataType.String || v2.DataType == DataType.String)
                                    {
                                        if (StringInstructions.ContainsKey(instruction))
                                        {
                                            instruction = StringInstructions[instruction];
                                        }
                                    }
                                }
                            }

                            var e = new Expression(instruction, -1, e1, e2);
                            if (instruction == Instruction.S_MOD)
                            {
                                if (v1.DataType.IsString())
                                {
                                    int typeNumber = 2;
                                    switch (v2.DataType)
                                    {
                                        case DataType.Int:
                                        case DataType.RefInt:
                                            typeNumber = 2;
                                            break;
                                        case DataType.String:
                                        case DataType.RefString:
                                            typeNumber = 4;
                                            break;
                                        case DataType.Bool:
                                        case DataType.RefBool:
                                            typeNumber = 48;
                                            break;
                                        default:
                                            //FIXME: find out all these numbers
                                            break;
                                    }
                                    e.Arg3 = new Expression(Instruction.PUSH, -1, typeNumber);
                                }
                                else
                                {
                                    //error, first argument of S_MOD must be a string
                                }
                            }

                            return e;
                        }
                        else if (IntegerUnaryOperators.ContainsKey(tokenType))
                        {
                            var t1 = token.Subexpressions.GetOrNull(0);
                            var e1 = GenerateExpression(t1);

                            var v1 = e1.Variable;
                            if (v1.DataType != DataType.Int && v1.DataType != DataType.Bool)
                            {

                            }

                            var instruction = IntegerUnaryOperators[tokenType];
                            var e = new Expression(instruction, -1, e1);
                            return e;
                        }
                        else if (IntegerUnaryAssignmentOperators.ContainsKey(tokenType))
                        {
                            var t1 = token.Subexpressions.GetOrNull(0);
                            var e1 = GenerateExpression(t1);

                            var v1 = e1.Variable;
                            if (v1.DataType != DataType.Int)
                            {

                            }

                            
                            if (e1.ExpressionType == Instruction.REF || e1.ExpressionType == Instruction.S_REF)
                            {
                                e1.ExpressionType = Instruction.AssignmentRef;
                            }
                            else
                            {

                            }
                            var instruction = IntegerUnaryAssignmentOperators[tokenType];
                            var e = new Expression(instruction, -1, e1);
                            return e;
                        }
                        else
                        {
                            throw new NotImplementedException();
                        }
                    }
                    break;
            }
            return null;
        }

        private Expression ConvertToStatements(List<Expression> statementList)
        {
            return Decompiler.ConvertToStatements(statementList);
        }

        private Expression GenerateFunctionParameters(IFunction functionToCall, TokenExpression functionArgumentsToken)
        {
            //TODO: type checking
            int argIndex = 0;
            int parameterCount = functionToCall.ParameterCount;
            var t = functionArgumentsToken;
            Expression eResult = null;
            Expression currentComma = null;

            while (true)
            {
                var parameter = functionToCall.Parameters.GetOrNull(argIndex);
                if (parameter == null)
                {
                    break;
                }

                bool isRef = false;
                if (parameter.DataType == DataType.Void)
                {
                    argIndex++;
                    isRef = true;
                    parameter = functionToCall.Parameters.GetOrNull(argIndex);
                    if (parameter == null)
                    {
                        break;
                    }
                }
                argIndex++;

                TokenExpression tArg;
                if (t != null && t.TokenType == TokenType.Comma)
                {
                    tArg = t.Subexpressions.GetOrNull(0);
                    var t2 = t.Subexpressions.GetOrNull(1);
                    t = t2;
                }
                else
                {
                    tArg = t;
                    t = null;
                }

                Expression eArg = null;
                if (tArg == null)
                {
                    var iv = InitialValues.GetOrNull(parameter);
                    //check for a default value?
                    if (iv != null)
                    {
                        eArg = ConstantToExpression(iv);
                    }
                    else if (parameter.HasDefaultValue())
                    {
                        var initialValue = parameter.GetDefaultValue();
                        eArg = ConstantToExpression(initialValue);
                    }
                    else
                    {
                        //error
                    }
                }
                else
                {
                    eArg = GenerateExpression(tArg);
                    //TODO: type checking/conversion
                }
                if (eArg != null)
                {
                    if (isRef)
                    {
                        if (eArg.ExpressionType == Instruction.REF)
                        {
                            eArg.ExpressionType = Instruction.AssignmentRef;
                        }
                        else
                        {
                            //error?
                        }
                    }


                    if (eResult == null)
                    {
                        eResult = eArg;
                    }
                    else
                    {
                        if (currentComma == null)
                        {
                            currentComma = new Expression(Instruction.Comma, -1, eResult, eArg);
                            eResult = currentComma;
                        }
                        else
                        {
                            var oldArg2 = currentComma.Arg2;
                            var newComma = new Expression(Instruction.Comma, -1, oldArg2, eArg);
                            currentComma.Arg2 = newComma;
                            currentComma = newComma;
                        }
                    }
                }
                else
                {
                    //error?
                }
            }
            return eResult;
        }

        private Expression ConstantToExpression(InitialValue initialValue)
        {
            if (initialValue.DataType == DataType.Float)
            {
                return new Expression(Instruction.F_PUSH, -1, initialValue.IntValue);
            }
            else if (initialValue.DataType == DataType.String)
            {
                return new Expression(Instruction.S_PUSH, -1, Strings.Add(initialValue.StringValue ?? ""));
            }
            else //if (initialValue.DataType == DataType.Int || initialValue.DataType == DataType.Lint || initialValue.DataType == DataType.Bool)
            {
                return new Expression(Instruction.PUSH, -1, initialValue.IntValue);
            }
        }

        private Expression GenerateDot(TokenExpression token)
        {
            var leftToken = token.Subexpressions.GetOrNull(0);
            var rightToken = token.Subexpressions.GetOrNull(1);

            var leftExpression = GenerateExpression(leftToken);
            if (leftExpression != null)
            {
                var leftVariable = leftExpression.Variable;
                if (leftVariable != null && !leftVariable.DataType.IsArray())
                {
                    var structInfo = leftVariable.GetStructType();
                    if (structInfo != null)
                    {
                        if (rightToken.TokenType == TokenType.Identifier)
                        {
                            string memberName = rightToken.Token.Value;
                            var classMembers = GetClassMembers(structInfo.Name);
                            if (classMembers.ContainsKey(memberName))
                            {
                                var symbol = classMembers[memberName];
                                var expression = SymbolToExpression(symbol, memberName);

                                expression.Arg1 = leftExpression;
                                return expression;
                            }
                            //structInfo.Members
                        }
                        else
                        {

                        }

                    }
                    else
                    {
                        //error
                    }
                }
                else
                {
                    if (rightToken.TokenType == TokenType.Identifier)
                    {
                        if (leftVariable.DataType.IsArray())
                        {
                            Instruction ins = Instruction.Nop;
                            switch (rightToken.Token.Value)
                            {
                                case "Alloc":
                                    ins = Instruction.A_ALLOC;
                                    break;
                                case "Realloc":
                                    ins = Instruction.A_REALLOC;
                                    break;
                                case "Free":
                                    ins = Instruction.A_FREE;
                                    break;
                                case "Numof":
                                    ins = Instruction.A_NUMOF;
                                    break;
                                case "Copy":
                                    ins = Instruction.A_COPY;
                                    break;
                                case "Fill":
                                    ins = Instruction.A_FILL;
                                    break;
                                case "PushBack":
                                    ins = Instruction.A_PUSHBACK;
                                    break;
                                case "PopBack":
                                    ins = Instruction.A_POPBACK;
                                    break;
                                case "Empty":
                                    ins = Instruction.A_EMPTY;
                                    break;
                                case "Erase":
                                    ins = Instruction.A_ERASE;
                                    break;
                                case "Insert":
                                    ins = Instruction.A_INSERT;
                                    break;
                                case "Sort":
                                    ins = Instruction.A_SORT;
                                    break;

                            }
                            if (ins != Instruction.Nop)
                            {
                                return new Expression(ins, -1, leftExpression);
                            }
                            else
                            {
                                //error
                            }
                        }
                        else if (leftVariable.DataType.IsString())
                        {
                            Instruction ins = Instruction.Nop;
                            switch (rightToken.Token.Value)
                            {
                                case "Int":
                                    ins = Instruction.STOI;
                                    break;
                                case "Length":
                                    ins = Instruction.S_LENGTH;
                                    break;
                                case "LengthByte":
                                    ins = Instruction.S_LENGTHBYTE;
                                    break;
                                case "Empty":
                                    ins = Instruction.S_EMPTY;
                                    break;
                                case "Find":
                                    ins = Instruction.S_FIND;
                                    break;
                                case "GetPart":
                                    ins = Instruction.S_GETPART;
                                    break;
                                case "PushBack":
                                    ins = Instruction.S_PUSHBACK;
                                    break;
                                case "PopBack":
                                    ins = Instruction.S_POPBACK;
                                    break;
                                case "Erase":
                                    ins = Instruction.S_ERASE;
                                    break;

                            }
                            if (ins != Instruction.Nop)
                            {
                                return new Expression(ins, -1, leftExpression);
                            }
                            else
                            {
                                //error
                            }
                        }
                        else
                        {
                            //error
                        }
                    }
                    else
                    {
                        //error
                    }

                }
            }
            else
            {
                if (leftToken.TokenType == TokenType.Identifier && rightToken.TokenType == TokenType.Identifier)
                {
                    string left = leftToken.Token.Value;
                    string right = rightToken.Token.Value;
                    if (left == "system")
                    {
                        int callNumber = AinFile.SystemCallNameToIndex.GetOrDefault(left + "." + right, -1);
                        if (callNumber != -1)
                        {
                            return new Expression(Instruction.CALLSYS, -1, callNumber);
                        }
                        else
                        {

                        }
                    }
                    else
                    {
                        //TODO: library calls
                    }

                }
                else
                {

                }
            }
            return null;
        }

        private Expression SymbolToExpression(IVariable symbol)
        {
            return SymbolToExpression(symbol, symbol.Name);
        }

        private Expression SymbolToExpression(IVariable symbol, string symbolName)
        {
            if (symbol != null)
            {
                {
                    var variable = symbol as Variable;
                    if (variable != null)
                    {
                        if (variable.Parent is Struct)
                        {
                            var eStruct = new Expression(Instruction.PUSHSTRUCTPAGE, -1);
                            var eIndex = new Expression(Instruction.PUSH, -1);
                            eIndex.Value = variable.Index;
                            var eRef = new Expression(Instruction.REF, -1);
                            eRef.Arg1 = eStruct;
                            eRef.Arg2 = eIndex;
                            GetRefType(symbol, ref eRef);
                            return eRef;
                        }
                        else if (variable.Parent is Function)
                        {
                            var eLocal = new Expression(Instruction.PUSHLOCALPAGE, -1);
                            var eIndex = new Expression(Instruction.PUSH, -1);
                            eIndex.Value = variable.Index;
                            var eRef = new Expression(Instruction.REF, -1);
                            eRef.Arg1 = eLocal;
                            eRef.Arg2 = eIndex;
                            GetRefType(symbol, ref eRef);
                            return eRef;
                        }
                        else if (this.InitialValues.ContainsKey(symbol) && this.Constants.Contains(symbolName))
                        {
                            var initialValue = this.InitialValues[symbol];
                            return ConstantToExpression(initialValue);
                        }
                        else
                        {

                        }
                    }
                }
                {
                    var global = symbol as Global;
                    if (global != null)
                    {
                        var eGlobal = new Expression(Instruction.PUSHGLOBALPAGE, -1);
                        var eIndex = new Expression(Instruction.PUSH, -1);
                        eIndex.Value = global.Index;
                        var eRef = new Expression(Instruction.REF, -1);
                        eRef.Arg1 = eGlobal;
                        eRef.Arg2 = eIndex;
                        GetRefType(symbol, ref eRef);
                        return eRef;
                    }
                }
                {
                    var ifunction = symbol as IFunction;
                    if (ifunction != null)
                    {
                        var function = ifunction as Function;
                        if (function != null)
                        {
                            if (function.Name != symbolName)
                            {
                                var eStruct = new Expression(Instruction.PUSHSTRUCTPAGE, -1);
                                var eCallMethod = new Expression(Instruction.CALLMETHOD, -1, function.Index);
                                eCallMethod.Arg1 = eStruct;
                                return eCallMethod;
                            }
                            else
                            {
                                var eCallFunc = new Expression(Instruction.CALLFUNC, -1, function.Index);
                                eCallFunc.Value2 = -1;
                                return eCallFunc;
                            }
                        }
                        var funcType = ifunction as FunctionType;
                        if (funcType != null)
                        {
                            var eCallFunc = new Expression(Instruction.CALLFUNC2, -1);
                            var ePush = new Expression(Instruction.PUSH, -1, funcType.Index);
                            eCallFunc.Arg2 = ePush;
                            eCallFunc.Value2 = -1;
                            return eCallFunc;
                        }
                        var structInfo = ifunction as Struct;
                        if (structInfo != null)
                        {
                            var exp = new Expression(Instruction.PrintStructType, -1, structInfo.Index);
                            return exp;
                        }
                    }
                }
            }
            return null;
        }

        private static void GetRefType(IVariable symbol, ref Expression expression)
        {
            var dataType = symbol.DataType;
            switch (dataType)
            {
                //TODO: verify this
                case DataType.RefArrayBool:
                case DataType.RefArrayDelegate:
                case DataType.RefArrayFloat:
                case DataType.RefArrayFunctype:
                case DataType.RefArrayInt:
                case DataType.RefArrayLint:
                case DataType.RefArrayString:
                case DataType.RefArrayStruct:
                case DataType.RefBool:
                case DataType.RefDelegate:
                case DataType.RefFloat:
                case DataType.RefFunctype:
                case DataType.RefInt:
                case DataType.RefLint:
                    expression.ExpressionType = Instruction.REFREF;
                    expression = new Expression(Instruction.REF, -1, expression);
                    return;
                case DataType.RefStruct:
                    expression.ExpressionType = Instruction.SR_REFREF;
                    expression = new Expression(Instruction.REF, -1, expression);
                    return;
                case DataType.RefString:
                    expression.ExpressionType = Instruction.S_REFREF;
                    expression = new Expression(Instruction.REF, -1, expression);
                    return;
                case DataType.String:
                    expression.ExpressionType = Instruction.S_REF;
                    return;
                case DataType.Struct:
                    expression.ExpressionType = Instruction.SR_REF;
                    expression.Value = symbol.StructType;
                    return;
                case DataType.ArrayBool:
                case DataType.ArrayDelegate:
                case DataType.ArrayFloat:
                case DataType.ArrayFunctype:
                case DataType.ArrayInt:
                case DataType.ArrayLint:
                case DataType.ArrayString:
                case DataType.ArrayStruct:
                    //expression.ExpressionType = Instruction.A_REF;
                    expression.ExpressionType = Instruction.REF;
                    return;
                default:
                    expression.ExpressionType = Instruction.REF;
                    return;
            }
        }

        private Expression GenerateSymbolExpression(TokenExpression e1)
        {
            string symbolName = e1.Token.Value;
            var symbol = Symbols.GetOrNull(symbolName);



            return new Expression(Instruction.PUSHGLOBALPAGE, -1);
        }

        private void CompileSecondPass()
        {
            var secondPass = new SecondPass(this);
            secondPass.CompileSecondPass();
        }



        public AinFile ainFile;

        ListAndDictionary<Function> Functions;
        ListAndDictionary<Global> Globals;
        ListAndDictionary<Variable> Constants;
        ListAndDictionary<Struct> Structs;
        ListAndDictionary<FunctionType> Functypes;
        ListAndDictionary<FunctionType> Delegates;
        ListAndDictionary<HllLibrary> Libraries;
        StringListAndDictionary Strings;
        StringListAndDictionary Messages;

        internal Dictionary<string, IVariable> Symbols = new Dictionary<string, IVariable>();
        internal Dictionary<IVariable, InitialValue> InitialValues = new Dictionary<IVariable, InitialValue>();
        internal Dictionary<Function, TokenExpression> FunctionDefinitionTokens = new Dictionary<Function, TokenExpression>();
        internal Dictionary<Function, TokenExpression> FunctionDeclarationTokens = new Dictionary<Function, TokenExpression>();

        void IncludeFile(string fileName, List<TokenExpression> tokens)
        {
            var encoding = EncodingDetector.DetectEncoding(fileName);
            var trBase = new StreamReader(fileName, encoding);
            var tr = new MyTextReader2(trBase, fileName);
            IncludeFile(tr, tokens);
        }

        void IncludeString(string expression, List<TokenExpression> tokens)
        {
            var sr = new StringReader(expression);
            var tr = new MyTextReader2(sr, "");
            IncludeFile(tr, tokens);
        }

        HashSet<string> seenFileNames = new HashSet<string>();

        private void IncludeFile(MyTextReader2 tr, List<TokenExpression> tokens)
        {
            while (true)
            {
                tr.EatWhiteSpace();
                int row = tr.Row;
                int column = tr.Column;
                var token = tr.ReadToken();

                if (token == null)
                {
                    break;
                }
                var tokenExpression = new TokenExpression(token);
                tokenExpression.Row = row;
                tokenExpression.Column = column;
                if (tokenExpression.TokenType == TokenType.Pound)
                {
                    var includeToken = tr.PeekToken();
                    if (includeToken.Value.Equals("include", StringComparison.OrdinalIgnoreCase))
                    {
                        includeToken = tr.ReadToken();
                        var fileNameToken = tr.PeekToken();
                        if (fileNameToken.IsQuoted)
                        {
                            try
                            {
                                tr.IncludeTextReader(fileNameToken.Value);
                            }
                            catch (IOException)
                            {
                                throw;
                            }
                        }
                        else
                        {
                            tr.PutbackToken(includeToken);
                        }
                    }
                }

                if (tokenExpression.IsMacro())
                {
                    switch (tokenExpression.TokenType)
                    {
                        case TokenType.FileMacro:
                            tokenExpression.Token = new Token(tr.FileName);
                            tokenExpression.Token.QuoteCharacter = '"';
                            tokenExpression.TokenType = TokenType.StringLiteral;
                            break;
                        case TokenType.LineMacro:
                            tokenExpression.Token = new Token((row + 1).ToString());
                            tokenExpression.Token.QuoteCharacter = '"';
                            tokenExpression.TokenType = TokenType.StringLiteral;
                            break;
                        case TokenType.FuncMacro:
                            break;
                        case TokenType.DateMacro:
                            StringBuilder dateString = new StringBuilder();
                            dateString.Append(DateTime.Today.ToString("yyyy/MM/dd"));
                            if (dateString[5] == '0') dateString[5] = ' ';
                            if (dateString[8] == '0') dateString[8] = ' ';
                            tokenExpression.Token = new Token(dateString.ToString());
                            tokenExpression.Token.QuoteCharacter = '"';
                            tokenExpression.TokenType = TokenType.StringLiteral;
                            break;
                        case TokenType.TimeMacro:
                            tokenExpression.Token = new Token(DateTime.Now.ToString("HH:mm:ss"));
                            tokenExpression.Token.QuoteCharacter = '"';
                            tokenExpression.TokenType = TokenType.StringLiteral;
                            break;
                    }
                }
                tokens.Add(tokenExpression);
            }
        }


        class VariableMultiDictionary : MultipleDictionary<string, IVariable>
        {

        }
    }
}
