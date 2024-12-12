using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Text;

namespace AinDecompiler.Compiler
{
    public static class CompilerExtensions
    {
        public static IEnumerable<TokenExpression> TokensWhere(this IEnumerable<TokenExpression> tokens, TokenType tokenType)
        {
            return tokens.Where(t => t.TokenType == tokenType);
        }

        public static IEnumerable<TokenExpression> TokensWhereRecursiveFirst(this IEnumerable<TokenExpression> tokens, TokenType tokenType)
        {
            return tokens.SelectMany(t => TokensWhereRecursiveFirst(t, tokenType));
        }

        public static IEnumerable<TokenExpression> TokensWhereRecursiveFirst(this TokenExpression token, TokenType tokenType)
        {
            if (token.TokenType == tokenType)
            {
                yield return token;
                yield break;
            }
            foreach (var subexpression in token.Subexpressions)
            {
                var whereRecursive = TokensWhereRecursiveFirst(subexpression, tokenType);
                foreach (var w in whereRecursive)
                {
                    yield return w;
                }
            }
        }

        public static IEnumerable<TokenExpression> TokensWhereRecursive(this TokenExpression token, TokenType tokenType)
        {
            if (token.TokenType == tokenType)
            {
                yield return token;
            }
            foreach (var subexpression in token.Subexpressions)
            {
                var whereRecursive = TokensWhereRecursive(subexpression, tokenType);
                foreach (var w in whereRecursive)
                {
                    yield return w;
                }
            }
        }

    }

    public partial class Compiler
    {
        string GetFunctionName(TokenExpression functionNameToken)
        {
            return GetFunctionName(functionNameToken, null);
        }

        string GetFunctionName(TokenExpression functionNameToken, string className)
        {
            StringBuilder sb = new StringBuilder();
            foreach (var token in functionNameToken.Subexpressions)
            {
                if (token.TokenType != TokenType.OpenParenthesis)
                {
                    if (token.TokenType == TokenType.ScopeResolution)
                    {
                        if (!string.IsNullOrEmpty(className))
                        {
                            sb.Length = 0;
                        }
                        else
                        {
                            sb.Append("@");
                        }
                    }
                    else
                    {
                        sb.Append(token.Token.Value);
                    }
                }
            }
            string functionName = sb.ToString();
            int indexOfAt = functionName.IndexOf('@');
            if (indexOfAt >= 0)
            {
                className = functionName.Substring(0, indexOfAt);
                functionName = functionName.Substring(indexOfAt + 1);
            }

            if (string.IsNullOrEmpty(className))
            {
                return functionName;
            }
            else
            {
                if (functionName == className)
                {
                    return className + "@0";
                }
                if (functionName == "~" + className)
                {
                    return className + "@1";
                }
                return className + "@" + functionName;
            }
        }

        private Variable GetDataType(TokenExpression dataTypeToken)
        {
            bool isRef = false;
            bool isArray = false;
            int structType = -1;
            int arrayDimensions = 0;
            DataType dataType = DataType.Void;

            for (int i = 0; i < dataTypeToken.Subexpressions.Count; i++)
            {
                var token = dataTypeToken.Subexpressions[i];

                switch (token.TokenType)
                {
                    case TokenType.ArrayType:
                        dataTypeToken = token;
                        i = -1;
                        continue;
                    case TokenType.Ref:
                        isRef = true;
                        break;
                    case TokenType.Array:
                        isArray = true;
                        i++;
                        arrayDimensions = 1;
                        break;
                    case TokenType.Number:
                        arrayDimensions = token.Token.ToInt();
                        break;
                    case TokenType.Void:
                        dataType = DataType.Void;
                        break;
                    case TokenType.Int:
                        dataType = DataType.Int;
                        if (isArray) dataType += 4;
                        if (isRef) dataType += 8;
                        break;
                    case TokenType.String:
                        dataType = DataType.String;
                        if (isArray) dataType += 4;
                        if (isRef) dataType += 8;
                        break;
                    case TokenType.Float:
                        dataType = DataType.Float;
                        if (isArray) dataType += 4;
                        if (isRef) dataType += 8;
                        break;
                    case TokenType.Bool:
                        dataType = DataType.Bool;
                        if (isArray && !isRef) dataType += 3;
                        else if (!isArray && isRef) dataType += 4;
                        else if (isArray && isRef) dataType += 5;
                        break;
                    case TokenType.Lint:
                        dataType = DataType.Lint;
                        if (isArray && !isRef) dataType += 3;
                        else if (!isArray && isRef) dataType += 4;
                        else if (isArray && isRef) dataType += 5;
                        break;
                    case TokenType.IMainSystem:
                        dataType = DataType.IMainSystem;
                        break;
                    case TokenType.Identifier:
                        string name = token.Token.Value;
                        if (Structs.Contains(name))
                        {
                            structType = Structs.NameToIndex[name];
                            dataType = DataType.Struct;
                            if (isArray) dataType += 4;
                            if (isRef) dataType += 8;
                        }
                        else if (Functypes.Contains(name))
                        {
                            structType = Functypes.NameToIndex[name];
                            dataType = DataType.Functype;
                            if (isArray && !isRef) dataType += 3;
                            else if (!isArray && isRef) dataType += 4;
                            else if (isArray && isRef) dataType += 5;
                        }
                        else if (Delegates.Contains(name))
                        {
                            structType = Functypes.NameToIndex[name];
                            dataType = DataType.Functype;
                            if (isArray && !isRef) dataType += 3;
                            else if (!isArray && isRef) dataType += 4;
                            else if (isArray && isRef) dataType += 5;
                        }
                        else
                        {
                            dataType = DataType.Struct;
                            structType = -1;
                        }
                        break;
                }
            }
            return new Variable("", dataType, structType, arrayDimensions);
        }

        void CreateSymbols(List<TokenExpression> tokens)
        {
            var classDefinitions = tokens.TokensWhere(TokenType.ClassDefinition).ToArray();
            var functypeDeclarations = tokens.TokensWhere(TokenType.FunctypeDeclaration).ToArray();
            //var globalDefinitions = tokens.TokensWhere(TokenType.VariableDeclaration);
            //var constDefinitions = tokens.TokensWhere(TokenType.ConstDeclaration);
            //var functionDeclarations = tokens.TokensWhere(TokenType.FunctionDeclaration);

            //create structs
            foreach (var token in classDefinitions)
            {
                string className = token.Subexpressions[1].Token.Value;
                var structInfo = Structs.Get(className);
            }

            //create function/delegate
            foreach (var token in functypeDeclarations)
            {
                var functionNameToken = token.Subexpressions[2];
                string funcTypeName = GetFunctionName(functionNameToken, null);
                if (token.Subexpressions[0].TokenType == TokenType.Delegate)
                {
                    var delegateInfo = Delegates.Get(funcTypeName);
                }
                else
                {
                    var functype = Functypes.Get(funcTypeName);
                }
            }

            //data types exist now - now create structs, functions, functypes, globals, constants

            foreach (var token in tokens)
            {
                switch (token.TokenType)
                {
                    case TokenType.ClassDefinition:
                        {
                            string className = token.Subexpressions[1].Token.Value;
                            var structInfo = this.Structs.Get(className);

                            var entries = token.Subexpressions[2].Subexpressions;
                            foreach (var entry0 in entries)
                            {
                                var entry = entry0.Subexpressions[0];
                                switch (entry.TokenType)
                                {
                                    case TokenType.FunctionDeclaration:
                                        {
                                            var function = CreateSymbolForFunction(entry, className);
                                            var blockToken = entry0.GetFirstSubexpression(TokenType.Block);
                                            if (blockToken != null)
                                            {
                                                this.FunctionDefinitionTokens[function] = blockToken;
                                            }
                                            this.FunctionDeclarationTokens[function] = entry;
                                        }
                                        break;
                                    case TokenType.ConstDeclaration:
                                        {
                                            CreateSymbolForConst(entry, className);
                                        }
                                        break;
                                    case TokenType.VariableDeclaration:
                                        {
                                            var variables = GetVariablesInVariableDeclaration(entry);
                                            foreach (var variable in variables)
                                            {
                                                int index = structInfo.Members.Count;
                                                structInfo.Members.Add(variable);
                                                variable.Index = index;
                                                variable.Parent = structInfo;
                                                variable.Root = ainFile;
                                            }
                                        }
                                        break;
                                }
                            }
                        }
                        break;
                    case TokenType.FunctypeDeclaration:
                        {
                            var dataTypeToken = token.Subexpressions[1];
                            var functionNameToken = token.Subexpressions[2];
                            var parameterListToken = token.Subexpressions[3];

                            var dataType = GetDataType(dataTypeToken);
                            string functypeName = GetFunctionName(functionNameToken);

                            var funcType = Functypes.Get(functypeName);
                            funcType.SetVariableType(dataType);

                            foreach (var parameterDataTypeToken in parameterListToken.Subexpressions)
                            {
                                int index = funcType.Parameters.Count;
                                var parameter = GetDataType(parameterDataTypeToken);
                                parameter.Name = index.ToString();
                                parameter.Index = index;
                                parameter.Root = ainFile;
                                funcType.Parameters.Add(parameter);
                                if (parameter.DataType.IsPrimitiveRefType())
                                {
                                    var voidParameter = new Variable("<void>", DataType.Void, 0, 0);
                                    voidParameter.Index = index + 1;
                                    voidParameter.Root = ainFile;
                                    funcType.Parameters.Add(voidParameter);
                                }
                            }
                            funcType.ParameterCount = funcType.Parameters.Count;
                        }
                        break;
                    case TokenType.FunctionDeclaration:
                        {
                            var function = CreateSymbolForFunction(token);
                            var blockToken = token.GetFirstSubexpression(TokenType.Block);
                            if (blockToken != null)
                            {
                                this.FunctionDefinitionTokens[function] = blockToken;
                            }
                            this.FunctionDeclarationTokens[function] = token;
                        }
                        break;
                    case TokenType.VariableDeclaration:
                        {
                            var variables = GetVariablesInVariableDeclaration(token);
                            foreach (var variable in variables)
                            {
                                var global = Globals.Get(variable.Name);
                                global.SetVariableType(variable);
                            }
                        }
                        break;
                    case TokenType.ConstDeclaration:
                        {
                            CreateSymbolForConst(token);
                        }
                        break;
                }
            }

            //create 0 function
            Functions.Get("0");

            //create @2 functions for each constructor
            foreach (var structInfo in Structs.List)
            {
                string constructorName = structInfo.Name + "@0";
                if (Functions.Contains(constructorName))
                {
                    var function = Functions.Get(constructorName);
                    string arrayInitializerName = structInfo.Name + "@2";
                    var arrayInitializer = Functions.Get(arrayInitializerName);
                }
            }
        }

        private Variable[] GetVariablesInVariableDeclaration(TokenExpression variableDeclarationToken)
        {
            var dataTypeToken = variableDeclarationToken.Subexpressions[0];
            var variablesList = variableDeclarationToken.Subexpressions[1].Subexpressions;

            var dataType = GetDataType(dataTypeToken);

            List<Variable> variables = new List<Variable>();

            foreach (var entry in variablesList)
            {
                string variableName = entry.Subexpressions[0].Token.Value;
                var newVariable = new Variable(variableName, dataType.DataType, dataType.StructType, dataType.ArrayDimensions);
                newVariable.Root = ainFile;
                variables.Add(newVariable);
                if (newVariable.DataType.IsPrimitiveRefType())
                {
                    newVariable = new Variable("<void>", DataType.Void, 0, 0);
                    newVariable.Root = ainFile;
                    variables.Add(newVariable);
                }
            }

            return variables.ToArray();
        }

        private void CreateSymbolForConst(TokenExpression constDeclarationToken)
        {
            CreateSymbolForConst(constDeclarationToken, null);
        }

        private void CreateSymbolForConst(TokenExpression constDeclarationToken, string className)
        {
            var variables = GetVariablesInVariableDeclaration(constDeclarationToken.Subexpressions[1]);
            foreach (var variable in variables)
            {
                string constantName = variable.Name;
                if (!string.IsNullOrEmpty(className))
                {
                    constantName = className + "." + constantName;
                }

                var constant = Constants.Get(constantName);
                constant.SetVariableType(variable);
            }
        }

        private Function CreateSymbolForFunction(TokenExpression functionDeclarationToken, string className)
        {
            var dataTypeToken = functionDeclarationToken.GetFirstSubexpression(TokenType.DataType);
            Variable dataType;
            if (dataTypeToken != null)
            {
                dataType = GetDataType(dataTypeToken);
            }
            else
            {
                dataType = new Variable("", DataType.Void, -1, 0);
            }
            var functionNameToken = functionDeclarationToken.GetFirstSubexpression(TokenType.FunctionName);
            string functionName = GetFunctionName(functionNameToken, className);

            var function = Functions.Get(functionName);
            function.SetVariableType(dataType);

            var poundSignToken = functionDeclarationToken.GetFirstSubexpression(TokenType.Pound);
            if (poundSignToken != null)
            {
                function.IsLabel = 1;
            }


            //add parameters
            var functionParameterListToken = functionDeclarationToken.GetFirstSubexpression(TokenType.FunctionParameterList);
            if (functionParameterListToken != null)
            {
                var parameters = GetVariablesInFunctionParameters(functionParameterListToken);

                if (function.Parameters.Count < parameters.Length)
                {
                    function.Parameters.Clear();
                    foreach (var parameter in parameters)
                    {
                        int index = function.Parameters.Count;
                        parameter.Root = ainFile;
                        parameter.Parent = function;
                        parameter.Index = index;
                        function.Parameters.Add(parameter);
                    }
                    function.ParameterCount = function.Parameters.Count;
                }
            }

            return function;
        }

        private Variable[] GetVariablesInFunctionParameters(TokenExpression functionParameterListToken)
        {
            List<Variable> list = new List<Variable>();
            foreach (var entry in functionParameterListToken.Subexpressions)
            {
                var dataTypeToken = entry.Subexpressions[0];
                var parameterNameToken = entry.Subexpressions[1];

                var dataType = GetDataType(dataTypeToken);
                string parameterName = parameterNameToken.Token.Value;
                var newVariable = new Variable(parameterName, dataType.DataType, dataType.StructType, dataType.ArrayDimensions);
                list.Add(newVariable);

                if (dataType.DataType.IsPrimitiveRefType())
                {
                    var voidParameter = new Variable("<void>", DataType.Void, 0, 0);
                    list.Add(voidParameter);
                }
            }
            return list.ToArray();
        }

        private Function CreateSymbolForFunction(TokenExpression functionDeclarationToken)
        {
            return CreateSymbolForFunction(functionDeclarationToken, null);
        }
    }
}
