using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Text;

namespace AinDecompiler.Compiler
{
    public partial class SecondPass
    {
        Dictionary<IVariable, TokenExpression> initialValueExpressions = new Dictionary<IVariable, TokenExpression>();
        Dictionary<IVariable, List<IVariable>> initialDependencies = new Dictionary<IVariable, List<IVariable>>();

        Dictionary<IVariable, InitialValue> InitialValues
        {
            get
            {
                return Parent.InitialValues;
            }
        }

        AinFile ainFile
        {
            get
            {
                return Parent.ainFile;
            }
        }

        public void GetInitialValues(List<TokenExpression> result)
        {
            //first do the constatns
            var constDeclarations = result.TokensWhere(TokenType.ConstDeclaration);
            var variableDeclarations = result.TokensWhere(TokenType.VariableDeclaration);
            foreach (var constDeclaration in constDeclarations)
            {
                var variableDeclaration = constDeclaration.GetFirstSubexpression(TokenType.VariableDeclaration);
                GetInitialValue(variableDeclaration);
            }

            //then do the globals
            foreach (var globalDeclaration in variableDeclarations)
            {
                GetInitialValue(globalDeclaration);
            }

            //then do the function parameters
            foreach (var pair in FunctionDeclarationTokens)
            {
                var function = pair.Key;
                var declarationToken = pair.Value;
                var parameterList = declarationToken.Subexpressions.TokensWhere(TokenType.FunctionParameterList).FirstOrDefault();
                if (parameterList != null)
                {
                    int i = 0;
                    foreach (var parameter in parameterList.Subexpressions.TokensWhere(TokenType.FunctionParameter))
                    {
                        var initialValueToken = parameter.Subexpressions.TokensWhere(TokenType.VariableInitialValue).FirstOrDefault();
                        if (initialValueToken != null)
                        {
                            var parameterVariable = function.GetNonVoidFunctionParameter(i);
                            GetVariableInitialValue(parameterVariable, initialValueToken);
                        }
                        i++;
                    }
                }
            }

            //get dependencies
            GetInitialValueDependencies();

            //resolve dependencies - O(n^2) method
            while (true)
            {
                var pairs = initialDependencies.ToArray();
                int count = pairs.Length;
                foreach (var pair in pairs)
                {
                    var variable = pair.Key;
                    var dependencyList = pair.Value;
                    bool isSatisfied = true;
                    foreach (var dep in dependencyList)
                    {
                        if (initialDependencies.ContainsKey(dep))
                        {
                            isSatisfied = false;
                        }
                    }
                    if (isSatisfied)
                    {
                        initialDependencies.Remove(variable);
                        var initialValueExpression = initialValueExpressions[variable];
                        var initialValue = EvaluateConstExpression(initialValueExpression, variable.DataType);
                        InitialValues.Set(variable, initialValue);
                    }
                }
                if (initialDependencies.Count == 0)
                {
                    break;
                }
                if (count == initialDependencies.Count)
                {
                    //error - circular dependencies
                    break;
                }
            }

            //Set global initial values
            foreach (var pair in InitialValues)
            {
                var variable = pair.Key;
                var global = variable as Global;
                if (global != null)
                {
                    var initialValue = pair.Value;
                    var globalInitialValue = new GlobalInitialValue(initialValue);
                    globalInitialValue.Root = ainFile;
                    globalInitialValue.GlobalIndex = global.Index;
                    ainFile.GlobalInitialValues.Add(globalInitialValue);
                }
            }
        }

        private void GetInitialValueDependencies()
        {
            foreach (var pair in initialValueExpressions)
            {
                var variable = pair.Key;
                var expression = pair.Value;

                var symbols = expression.GetSubexpressionsRecursive(TokenType.Identifier);
                var variables = symbols.Select(token => Symbols.GetOrNull(token.Token.Value)).ToArray();
                if (variables.Contains(null))
                {
                    //error
                }
                variables = variables.Distinct().ToArray();

                if (variables.Length == 0)
                {
                    var initialValue = EvaluateConstExpression(expression, variable.DataType);
                    if (initialValue != null)
                    {
                        InitialValues.Set(variable, initialValue);
                    }
                    else
                    {

                    }
                }
                else
                {
                    var depsList = initialDependencies.GetOrAddNew(variable);
                    depsList.AddRange(variables);
                }
            }
        }

        public void GetInitialValue(TokenExpression variableDeclaration)
        {
            var variableDeclarationList = variableDeclaration.Subexpressions[1];
            var variableDeclarationEntries = variableDeclarationList.Subexpressions.TokensWhere(TokenType.VariableDeclarationEntry);
            foreach (var entry in variableDeclarationEntries)
            {
                //entry: identifier [arraybounds] [initialvalue]
                string name = null;
                var nameToken = entry.GetFirstSubexpression(TokenType.Identifier);
                IVariable variable = null;
                if (nameToken != null)
                {
                    name = nameToken.Token.Value;
                    variable = Symbols.GetOrNull(name);
                }
                var initialValue = entry.Subexpressions.TokensWhere(TokenType.VariableInitialValue).FirstOrDefault();
                if (initialValue != null && variable != null)
                {
                    GetVariableInitialValue(variable, initialValue);
                }
            }

        }

        private void GetVariableInitialValue(IVariable variable, TokenExpression variableInitialValueToken)
        {
            //initialvalue: [=], expression
            var reader = new TokenReader(variableInitialValueToken.Subexpressions);

            var lastReader = this.reader;
            this.reader = reader;
            var assign = ReadToken(TokenType.Assign);
            var expression = ReadToken(TokenType.Expression);
            if (expression != null)
            {
                if (reader.Index < variableInitialValueToken.Subexpressions.Count)
                {
                    //parse error
                }

                initialValueExpressions.Set(variable, expression);
            }
            this.reader = lastReader;
        }

        InitialValue EvaluateConstExpression(TokenExpression expression, DataType desiredDataType)
        {
            var result = EvaluateConstExpression(expression);
            if (result != null)
            {
                if (result.DataType != desiredDataType)
                {
                    if (desiredDataType == DataType.Bool || desiredDataType == DataType.RefBool)
                    {
                        if (result.DataType.IsInteger())
                        {
                            return new InitialValue(result.IntValue != 0);
                        }
                        if (result.DataType.IsFloat())
                        {
                            return new InitialValue(result.FloatValue != 0);
                        }
                        return null;
                        //if (result.DataType.IsString())
                        //{
                        //    return new InitialValue(result.StringValue != "");
                        //}
                    }
                    else if (desiredDataType.IsInteger())
                    {
                        if (result.DataType == DataType.Bool || result.DataType == DataType.RefBool)
                        {
                            return new InitialValue(result.IntValue);
                        }
                        if (result.DataType.IsFloat())
                        {
                            return new InitialValue((int)result.FloatValue);
                        }
                        return null;
                    }
                    else if (desiredDataType.IsFloat())
                    {
                        if (result.DataType.IsInteger() || result.DataType == DataType.Bool || result.DataType == DataType.RefBool)
                        {
                            return new InitialValue((float)result.IntValue);
                        }
                        return null;
                    }
                    else if (desiredDataType.IsString())
                    {
                        return null;
                    }
                    else if (desiredDataType.IsFunction())
                    {
                        return result;
                    }
                }
                else
                {
                    return result;
                }
            }
            return null;
        }

        InitialValue EvaluateConstExpression(TokenExpression expression)
        {
            if (expression == null)
            {
                return null;
            }
            //leaf? )
            if (expression.Subexpressions.Count == 0)
            {
                if (expression.TokenType == TokenType.Identifier)
                {
                    var variable = Symbols.GetOrNull(expression.Token.Value);
                    if (variable == null) return null;
                    if (InitialValues.ContainsKey(variable))
                    {
                        return InitialValues[variable];
                    }
                    else
                    {
                        if (variable.DataType.IsInteger())
                        {
                            return new InitialValue(0);
                        }
                        else if (variable.DataType.IsFloat())
                        {
                            return new InitialValue(0.0f);
                        }
                        else if (variable.DataType.IsString())
                        {
                            return new InitialValue("");
                        }
                        else if (variable.DataType == DataType.Bool || variable.DataType == DataType.RefBool)
                        {
                            return new InitialValue(false);
                        }
                        else if (variable is Function)
                        {
                            return new InitialValue(variable.Index);
                        }
                    }
                    return null;
                }
                else if (expression.TokenType == TokenType.Number)
                {
                    if (expression.Token.IsInt())
                    {
                        return new InitialValue(expression.Token.ToInt());
                    }
                    else if (expression.Token.IsFloat())
                    {
                        return new InitialValue(expression.Token.ToFloat());
                    }
                }
                else if (expression.TokenType == TokenType.StringLiteral)
                {
                    return new InitialValue(expression.Token.Value);
                }
                else if (expression.TokenType == TokenType.CharLiteral)
                {
                    var bytes = Extensions.BinaryEncoding.GetBytes(expression.Token.Value);
                    if (bytes.Length > 1)
                    {
                        return new InitialValue(bytes[1] * 256 + bytes[0]);
                    }
                    else
                    {
                        return new InitialValue(bytes[0]);
                    }
                }
                return null;
            }
            //unary operator?
            if (expression.Subexpressions.Count == 1)
            {
                var a = EvaluateConstExpression(expression.Subexpressions[0]);
                if (a == null) return null;

                switch (expression.TokenType)
                {
                    case TokenType.Complement:
                        {
                            if (a.DataType == DataType.Int || a.DataType == DataType.Bool)
                            {
                                return new InitialValue(~a.IntValue);
                            }
                            return null;
                        }
                    case TokenType.Not:
                        {
                            if (a.DataType == DataType.Int || a.DataType == DataType.Bool)
                            {
                                return new InitialValue(a.IntValue != 0);
                            }
                            if (a.DataType == DataType.Float)
                            {
                                return new InitialValue(a.FloatValue != 0);
                            }
                            return null;
                        }
                    case TokenType.Negative:
                        {
                            if (a.DataType == DataType.Int || a.DataType == DataType.Bool)
                            {
                                return new InitialValue(-a.IntValue);
                            }
                            else if (a.DataType == DataType.Float)
                            {
                                return new InitialValue(-a.FloatValue);
                            }
                            return null;
                        }
                    case TokenType.Positive:
                    case TokenType.PostDecrement:
                    case TokenType.PostIncrement:
                        {
                            if (a.DataType == DataType.Int || a.DataType == DataType.Bool || a.DataType == DataType.Float)
                            {
                                return a;
                            }
                            return null;
                        }
                    case TokenType.PreIncrement:
                        {
                            if (a.DataType == DataType.Int || a.DataType == DataType.Bool)
                            {
                                return new InitialValue(a.IntValue + 1);
                            }
                            else if (a.DataType == DataType.Float)
                            {
                                return new InitialValue(a.FloatValue + 1.0f);
                            }
                            return null;
                        }
                    case TokenType.PreDecrement:
                        {
                            if (a.DataType == DataType.Int || a.DataType == DataType.Bool)
                            {
                                return new InitialValue(a.IntValue - 1);
                            }
                            else if (a.DataType == DataType.Float)
                            {
                                return new InitialValue(a.FloatValue - 1.0f);
                            }
                            return null;
                        }
                }
                return null;
            }
            //binary expression?
            if (expression.Subexpressions.Count == 2)
            {
                var a = EvaluateConstExpression(expression.Subexpressions[0]);
                var b = EvaluateConstExpression(expression.Subexpressions[1]);

                //cast expressions (expressed as a functioncall)
                if (expression.TokenType == TokenType.FunctionCall && b != null)
                {
                    var exprA = expression.Subexpressions[0];
                    if (exprA.TokenType == TokenType.String)
                    {
                        if (b.DataType == DataType.String)
                        {
                            return b;
                        }
                        else
                        {
                            return new InitialValue(b.GetValue());
                        }
                    }
                    else if (exprA.TokenType == TokenType.Float)
                    {
                        InitialValue v = new InitialValue();
                        if (v.SetValue(DataType.Float, b.GetValue()))
                        {
                            return v;
                        }
                        else
                        {
                            return null;
                        }
                    }
                    else if (exprA.TokenType == TokenType.Int)
                    {
                        InitialValue v = new InitialValue();
                        if (v.SetValue(DataType.Int, b.GetValue()))
                        {
                            return v;
                        }
                        else
                        {
                            return null;
                        }
                    }
                }

                if (a == null || b == null)
                {
                    return null;
                }

                if (a.DataType == DataType.Float || b.DataType == DataType.Float)
                {
                    float floatA;
                    float floatB;

                    if (a.DataType == DataType.Int || a.DataType == DataType.Bool)
                    {
                        floatA = a.IntValue;
                    }
                    else if (a.DataType == DataType.Float)
                    {
                        floatA = a.FloatValue;
                    }
                    else
                    {
                        return null;
                    }

                    if (b.DataType == DataType.Int || b.DataType == DataType.Bool)
                    {
                        floatB = b.IntValue;
                    }
                    else if (b.DataType == DataType.Float)
                    {
                        floatB = b.FloatValue;
                    }
                    else
                    {
                        return null;
                    }

                    switch (expression.TokenType)
                    {
                        case TokenType.Multiply:
                        case TokenType.MultiplyAssign:
                            return new InitialValue(floatA * floatB);
                        case TokenType.Divide:
                        case TokenType.DivideAssign:
                            return new InitialValue(floatA / floatB);
                        case TokenType.Modulo:
                        case TokenType.ModuloAssign:
                            return new InitialValue(floatA % floatB);
                        case TokenType.Plus:
                        case TokenType.PlusAssign:
                            return new InitialValue(floatA + floatB);
                        case TokenType.Minus:
                        case TokenType.MinusAssign:
                            return new InitialValue(floatA - floatB);
                        case TokenType.LeftShift:
                        case TokenType.LeftShiftAssign:
                            return new InitialValue((int)floatA << (int)floatB);
                        case TokenType.RightShift:
                        case TokenType.RightShiftAssign:
                            return new InitialValue((int)floatA << (int)floatB);
                        case TokenType.LessThan:
                            return new InitialValue(floatA < floatB);
                        case TokenType.LessThanOrEqualTo:
                            return new InitialValue(floatA <= floatB);
                        case TokenType.GreaterThan:
                            return new InitialValue(floatA > floatB);
                        case TokenType.GreaterThanOrEqualTo:
                            return new InitialValue(floatA >= floatB);
                        case TokenType.EqualTo:
                            return new InitialValue(floatA == floatB);
                        case TokenType.NotEqualTo:
                            return new InitialValue(floatA != floatB);
                        case TokenType.And:
                        case TokenType.AndAssign:
                            return new InitialValue((int)floatA & (int)floatB);
                        case TokenType.Xor:
                        case TokenType.XorAssign:
                            return new InitialValue((int)floatA ^ (int)floatB);
                        case TokenType.Or:
                        case TokenType.OrAssign:
                            return new InitialValue((int)floatA | (int)floatB);
                        case TokenType.LogicalAnd:
                            return new InitialValue((floatA != 0) && (floatB != 0));
                        case TokenType.LogicalOr:
                            return new InitialValue((floatA != 0) || (floatB != 0));
                        case TokenType.Assign:
                            return new InitialValue(floatB);
                    }
                    return null;
                }

                if (a.DataType == DataType.String && b.DataType == DataType.String)
                {
                    string stringA = a.StringValue ?? "";
                    string stringB = b.StringValue ?? "";

                    switch (expression.TokenType)
                    {
                        case TokenType.Plus:
                        case TokenType.PlusAssign:
                            return new InitialValue(stringA + stringB);
                        case TokenType.LessThan:
                            return new InitialValue(String.Compare(stringA, stringB) < 0);
                        case TokenType.LessThanOrEqualTo:
                            return new InitialValue(String.Compare(stringA, stringB) <= 0);
                        case TokenType.GreaterThan:
                            return new InitialValue(String.Compare(stringA, stringB) > 0);
                        case TokenType.GreaterThanOrEqualTo:
                            return new InitialValue(String.Compare(stringA, stringB) >= 0);
                        case TokenType.EqualTo:
                            return new InitialValue(stringA == stringB);
                        case TokenType.NotEqualTo:
                            return new InitialValue(stringA != stringB);
                        case TokenType.LogicalAnd:
                            return new InitialValue((stringA != "") && (stringB != ""));
                        case TokenType.LogicalOr:
                            return new InitialValue((stringA != "") || (stringB != ""));
                        case TokenType.Assign:
                            return new InitialValue(stringB);
                    }
                    return null;
                }

                {
                    int intA;
                    int intB;

                    if (a.DataType == DataType.Int || a.DataType == DataType.Bool)
                    {
                        intA = a.IntValue;
                    }
                    else
                    {
                        return null;
                    }

                    if (b.DataType == DataType.Int || b.DataType == DataType.Bool)
                    {
                        intB = b.IntValue;
                    }
                    else
                    {
                        return null;
                    }

                    switch (expression.TokenType)
                    {
                        case TokenType.Multiply:
                        case TokenType.MultiplyAssign:
                            return new InitialValue(intA * intB);
                        case TokenType.Divide:
                        case TokenType.DivideAssign:
                            return new InitialValue(intA / intB);
                        case TokenType.Modulo:
                        case TokenType.ModuloAssign:
                            return new InitialValue(intA % intB);
                        case TokenType.Plus:
                        case TokenType.PlusAssign:
                            return new InitialValue(intA + intB);
                        case TokenType.Minus:
                        case TokenType.MinusAssign:
                            return new InitialValue(intA - intB);
                        case TokenType.LeftShift:
                        case TokenType.LeftShiftAssign:
                            return new InitialValue(intA << intB);
                        case TokenType.RightShift:
                        case TokenType.RightShiftAssign:
                            return new InitialValue(intA << intB);
                        case TokenType.LessThan:
                            return new InitialValue(intA < intB);
                        case TokenType.LessThanOrEqualTo:
                            return new InitialValue(intA <= intB);
                        case TokenType.GreaterThan:
                            return new InitialValue(intA > intB);
                        case TokenType.GreaterThanOrEqualTo:
                            return new InitialValue(intA >= intB);
                        case TokenType.EqualTo:
                            return new InitialValue(intA == intB);
                        case TokenType.NotEqualTo:
                            return new InitialValue(intA != intB);
                        case TokenType.And:
                        case TokenType.AndAssign:
                            return new InitialValue(intA & intB);
                        case TokenType.Xor:
                        case TokenType.XorAssign:
                            return new InitialValue(intA ^ intB);
                        case TokenType.Or:
                        case TokenType.OrAssign:
                            return new InitialValue(intA | intB);
                        case TokenType.LogicalAnd:
                            return new InitialValue((intA != 0) && (intB != 0));
                        case TokenType.LogicalOr:
                            return new InitialValue((intA != 0) || (intB != 0));
                        case TokenType.Assign:
                            return new InitialValue(intB);
                    }
                    return null;
                }
            }
            return null;
        }
    }
}
