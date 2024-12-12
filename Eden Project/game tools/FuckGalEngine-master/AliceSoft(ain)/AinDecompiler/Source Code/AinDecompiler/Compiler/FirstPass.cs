using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Text;

namespace AinDecompiler.Compiler
{
    class FirstPass
    {
        class TokenReader : MyEnumerator2<TokenExpression>
        {
            public TokenReader(IList<TokenExpression> array)
                : base(array)
            {

            }
        }

        internal List<TokenExpression> CompileFirstPass(List<TokenExpression> tokens)
        {
            //At the global scope.
            //we expect to see:
            //  class definitions
            //  function declarations or definitions
            //    constructor, destructor, method, normal function
            //  global variables
            //  functypes, delegates
            //  defines, constants

            //first pass - external structure and type declarations

            List<TokenExpression> result = new List<TokenExpression>();

            var reader = new TokenReader(tokens);
            while (true)
            {
                //class definition: "struct" or "class"
                var token = reader.Peek();
                if (token == null)
                {
                    break;
                }

                bool failed = false;

                if (reader.Index == 3986)
                {

                }

                switch (token.TokenType)
                {
                    case TokenType.Struct:
                    case TokenType.Class:
                        {
                            var classDefinition = ReadToken(reader, TokenType.ClassDefinition);
                            if (classDefinition != null)
                            {
                                result.Add(classDefinition);
                            }
                            else
                            {
                                failed = true;
                            }

                            //reader.Read();

                            //var newToken = new TokenExpression(token.Token);
                            //newToken.TokenType = TokenType.ClassDefinition;

                            //var nameToken = reader.Peek();
                            //if (nameToken == null)
                            //{
                            //    //fail
                            //}

                            ////name token should be a type name
                            //if (nameToken.TokenType != TokenType.Identifier)
                            //{
                            //    //fail
                            //}

                            //newToken.Subexpressions.Add(nameToken);

                            //string className = nameToken.Token.Value;
                            //var structInfo = Structs.Get(className);

                            //reader.Read();
                            //var braceToken = reader.Peek();

                            //if (braceToken == null)
                            //{
                            //    //fail
                            //}

                            //if (braceToken.TokenType == TokenType.OpenBrace)
                            //{
                            //    reader.Read();
                            //    var betweenBraces = ReadToClosingBrace(reader);
                            //    var blockExpression = new TokenExpression(betweenBraces);
                            //    newToken.Subexpressions.Add(blockExpression);

                            //    var semicolonToken = reader.Peek();
                            //    if (semicolonToken.TokenType == TokenType.Semicolon)
                            //    {
                            //        reader.Read();
                            //    }
                            //    else
                            //    {
                            //        //fail
                            //    }
                            //}
                            //else if (braceToken.TokenType == TokenType.Semicolon)
                            //{
                            //    reader.Read();
                            //}
                            //else
                            //{
                            //    //fail
                            //}
                            //output.Add(newToken);
                        }
                        break;
                    case TokenType.Const:
                        {
                            var constToken = ReadToken(reader, TokenType.ConstDeclaration);
                            if (constToken != null)
                            {
                                result.Add(constToken);
                            }
                            else
                            {
                                failed = true;
                            }

                            //var newToken = new TokenExpression(token.Token);
                            //reader.Read();
                            ////get an int, float, string, or bool
                            //var typeToken = reader.Peek();
                            //if (typeToken.TokenType == TokenType.Int ||
                            //    typeToken.TokenType == TokenType.Float ||
                            //    typeToken.TokenType == TokenType.String ||
                            //    typeToken.TokenType == TokenType.Bool)
                            //{

                            //}
                            //else
                            //{
                            //    failed = true;
                            //    break;
                            //    //fail
                            //}
                            //reader.Read();

                            ////get variable name
                            //var variableNameToken = reader.Peek();
                            //if (variableNameToken.TokenType != TokenType.Identifier)
                            //{
                            //    failed = true;
                            //    break;
                            //    //fail
                            //}
                            //reader.Read();

                            //string variableName = variableNameToken.Token.Value;
                            //var constant = Constants.Get(variableName);

                            ////get equals sign
                            //var assignToken = reader.Peek();
                            //if (assignToken.TokenType != TokenType.Assign)
                            //{
                            //    failed = true;
                            //    break;
                            //    //fail
                            //}
                            //reader.Read();

                            ////read to semicolon
                            //var expression = ReadToSemicolon(reader);
                            //var block = new TokenExpression(expression, TokenType.Expression);
                            //newToken.Subexpressions.Add(variableNameToken);
                            //newToken.Subexpressions.Add(block);
                        }
                        break;
                    case TokenType.Functype:
                    case TokenType.Delegate:
                        {
                            var functypeToken = ReadToken(reader, TokenType.FunctypeDeclaration);
                            if (functypeToken != null)
                            {
                                result.Add(functypeToken);
                            }
                            else
                            {
                                failed = true;
                            }

                            //var newToken = new TokenExpression(token.Token);
                            //var returnTypeTokens = ReadDataType(reader);
                            //var dataTypeNameToken = reader.Peek();
                            //if (dataTypeNameToken.TokenType != TokenType.Identifier)
                            //{
                            //    failed = true;
                            //    break;
                            //    //fail
                            //}
                            //reader.Read();
                            //string functypeName = dataTypeNameToken.Token.Value;

                            //if (token.TokenType == TokenType.Functype)
                            //{
                            //    var funcType = this.Functypes.Get(functypeName);
                            //}
                            //else if (token.TokenType == TokenType.Delegate)
                            //{
                            //    var delegateInfo = this.Delegates.Get(functypeName);
                            //}

                            ////open parenthesis
                            //var openParen = reader.Peek();
                            //if (openParen.TokenType != TokenType.OpenParenthesis)
                            //{
                            //    failed = true;
                            //    break;
                            //    //fail
                            //}
                            //reader.Read();

                            //newToken.Subexpressions.Add(dataTypeNameToken);
                            //newToken.Subexpressions.Add(new TokenExpression(returnTypeTokens, TokenType.DataType));

                            //List<TokenExpression> parameters = new List<TokenExpression>();
                            //while (true)
                            //{
                            //    //check for closed parenthesis
                            //    var nextToken = reader.Peek();
                            //    if (nextToken.TokenType == TokenType.ClosedParenthesis)
                            //    {
                            //        if (parameters.Count > 0)
                            //        {
                            //            failed = true;
                            //            break;
                            //            //fail
                            //        }
                            //        reader.Read();
                            //        break;
                            //    }

                            //    var parameterDataType = ReadDataType(reader);
                            //    parameters.Add(new TokenExpression(parameterDataType, TokenType.DataType));
                            //    nextToken = reader.Peek();
                            //    if (nextToken.TokenType == TokenType.Comma)
                            //    {
                            //        reader.Read();
                            //    }
                            //    else if (nextToken.TokenType == TokenType.ClosedParenthesis)
                            //    {
                            //        reader.Read();
                            //        break;
                            //    }
                            //    else
                            //    {
                            //        failed = true;
                            //        break;
                            //        //fail
                            //    }
                            //}
                            //newToken.Subexpressions.Add(new TokenExpression(parameters));
                        }
                        break;
                    default:
                        {
                            var function = ReadToken(reader, TokenType.FunctionDeclaration);
                            if (function != null)
                            {
                                var functionDefinition = ReadToken(reader, TokenType.Block);
                                function.Subexpressions.Add(functionDefinition);
                                result.Add(function);
                            }
                            else
                            {
                                var globalDeclaration = ReadToken(reader, TokenType.VariableDeclaration);
                                if (globalDeclaration != null)
                                {
                                    result.Add(globalDeclaration);
                                }
                                else
                                {
                                    failed = true;
                                }
                            }
                        }

                        //if (token.TokenType == TokenType.Pound)
                        //{
                        //    //#functionName(arguments)
                        //    reader.Read();
                        //    var poundToken = token;
                        //    var functionNameToken = reader.Peek();
                        //    if (functionNameToken.TokenType != TokenType.Identifier)
                        //    {

                        //    }




                        //}
                        //else
                        //{
                        //    //function or global

                        //}
                        break;
                }
                if (failed)
                {
                    reader.Index++;
                }
            }
            return result;
        }

        private List<TokenExpression> ReadTokens(TokenReader reader, bool allowEmpty, RepeatStyle repeatStyle, TokenType tokenType)
        {
            return ReadTokens(reader, tokenType, allowEmpty, repeatStyle, tokenType);
        }

        private List<TokenExpression> ReadTokens(TokenReader reader, TokenType returnType, bool allowEmpty, RepeatStyle repeatStyle, params TokenType[] tokenTypes)
        {
            var token = reader.Peek();
            if (token == null) return null;
            var list = new List<TokenExpression>();

            TokenType continuationToken;
            TokenType exitToken;

            switch (repeatStyle)
            {
                case RepeatStyle.RepeatUntilEndOfBlock:
                    continuationToken = TokenType.Optional;
                    exitToken = TokenType.CloseBrace;
                    break;
                case RepeatStyle.RepeatWithCommaUntilCloseParenthesis:
                    continuationToken = TokenType.Comma;
                    exitToken = TokenType.ClosedParenthesis;
                    break;
                case RepeatStyle.RepeatWithCommaUntilSemicolon:
                    continuationToken = TokenType.Comma;
                    exitToken = TokenType.Semicolon;
                    break;
                default:
                    throw new ArgumentOutOfRangeException("returnType");
            }

            if (allowEmpty)
            {
                if (token.TokenType == exitToken)
                {
                    reader.Read();
                    goto leave;
                }
            }

            while (true)
            {
                if (tokenTypes.Length > 1)
                {
                    token = ReadToken(reader, returnType, tokenTypes);
                }
                else if (tokenTypes.Length == 1)
                {
                    token = ReadToken(reader, tokenTypes[0]);
                }
                if (token != null)
                {
                    list.Add(token);
                }
                token = reader.Peek();
                if (token == null) return null;
                if (token.TokenType == exitToken)
                {
                    reader.Read();
                    break;
                }
                if (continuationToken != TokenType.Optional)
                {
                    if (token.TokenType != continuationToken)
                    {
                        return null;
                    }
                    reader.Read();
                }
            }
        leave:
            return list;
        }


        private TokenExpression ReadToken(TokenReader reader, TokenType returnType, params TokenType[] tokenTypes)
        {
            int startingIndex = reader.Index;
            List<TokenExpression> list = new List<TokenExpression>();

            bool isOptional = false;
            bool isChoice = false;
            bool ignoreNextToken = false;

            for (int i = 0; i < tokenTypes.Length; i++)
            {
                var tokenType = tokenTypes[i];
                if (tokenType == TokenType.Optional)
                {
                    isOptional = true;
                    continue;
                }
                if (tokenType == TokenType.Choice)
                {
                    isChoice = true;
                    continue;
                }
                if (ignoreNextToken)
                {
                    ignoreNextToken = false;
                    continue;
                }

                int currentTokenIndex = reader.Index;
                var token = ReadToken(reader, tokenType);
                if (token != null)
                {
                    list.Add(token);
                    isOptional = false;
                    if (isChoice)
                    {
                        ignoreNextToken = true;
                        isChoice = false;
                    }
                }
                else
                {
                    if (isChoice)
                    {
                        isOptional = true;
                        isChoice = false;
                    }
                    if (isOptional)
                    {
                        reader.Index = currentTokenIndex;
                        isOptional = false;
                    }
                    else
                    {
                        reader.Index = startingIndex;
                        return null;
                    }
                }

            }
            return new TokenExpression(list, returnType);
        }

        private TokenExpression ReadToken(TokenReader reader, TokenType tokenType)
        {
            int startIndex = reader.Index;
            var token = reader.Peek();
            if (token == null)
            {
                goto leave;
            }
            switch (tokenType)
            {
                default:
                    {
                        if (token.TokenType == tokenType)
                        {
                            return reader.Read();
                        }
                    }
                    break;
                case TokenType.Identifier:
                    {
                        if (token.TokenType == TokenType.Identifier)
                        {
                            reader.Read();
                            return token;
                        }
                        else if (token.IsBuiltInMethod())
                        {
                            var clone = token.Clone();
                            clone.TokenType = TokenType.Identifier;
                            reader.Read();
                            return clone;
                        }
                    }
                    break;
                case TokenType.SimpleDataType:
                    {
                        if (token.TokenType == TokenType.Identifier || token.IsDataType())
                        {
                            return reader.Read();
                        }
                    }
                    break;
                case TokenType.FunctionName:
                    {
                        var match = ReadToken(reader, tokenType, TokenType.Complement, TokenType.Identifier, TokenType.OpenParenthesis);
                        if (match != null) return match;
                        match = ReadToken(reader, tokenType, TokenType.Identifier, TokenType.ScopeResolution, TokenType.Complement, TokenType.Identifier, TokenType.OpenParenthesis);
                        if (match != null) return match;
                        match = ReadToken(reader, tokenType, TokenType.Identifier, TokenType.ScopeResolution, TokenType.Identifier, TokenType.OpenParenthesis);
                        if (match != null) return match;
                        match = ReadToken(reader, tokenType, TokenType.Identifier, TokenType.OpenParenthesis);
                        if (match != null) return match;
                    }
                    break;
                case TokenType.ArrayType:
                    {
                        var match = ReadToken(reader, tokenType, TokenType.Array, TokenType.At, TokenType.SimpleDataType, TokenType.At, TokenType.Number);
                        if (match != null) return match;
                        match = ReadToken(reader, tokenType, TokenType.Array, TokenType.At, TokenType.SimpleDataType);
                        if (match != null) return match;
                    }
                    break;
                case TokenType.DataType:
                    {
                        var match = ReadToken(reader, tokenType, TokenType.Optional, TokenType.Ref, TokenType.Choice, TokenType.ArrayType, TokenType.SimpleDataType);
                        if (match != null) return match;
                    }
                    break;
                case TokenType.FunctionParameterList:
                    {
                        //we have already consumed the open parenthesis
                        //check for immediately closing the argument list
                        var voidToken = ReadToken(reader, TokenType.FunctionParameter, TokenType.Void, TokenType.ClosedParenthesis);

                        //TODO fix void SetValue(int nValue, int bUpdate=false);

                        if (voidToken != null)
                        {
                            return new TokenExpression(null, tokenType);
                        }

                        var entryTokens = ReadTokens(reader, true, RepeatStyle.RepeatWithCommaUntilCloseParenthesis, TokenType.FunctionParameter);
                        if (entryTokens == null) goto leave;
                        return new TokenExpression(entryTokens, tokenType);

                        //List<TokenExpression> parametersList = new List<TokenExpression>();

                        //while (true)
                        //{
                        //    //look for (DataType variableName [ = expression], DataType variableName [ = expression]...)
                        //    //or "(void)"
                        //    var entryToken = ReadToken(reader, TokenType.FunctionParameter, TokenType.DataType, TokenType.Identifier, TokenType.Optional, TokenType.FunctionParameterInitialValue);
                        //    if (entryToken == null) goto leave;
                        //    parametersList.Add(entryToken);

                        //    var separator = ReadToken(reader, TokenType.First, TokenType.Choice, TokenType.Comma, TokenType.ClosedParenthesis);
                        //    if (separator == null) goto leave;

                        //    if (separator.TokenType == TokenType.ClosedParenthesis)
                        //    {
                        //        break;
                        //    }
                        //}
                        //return new TokenExpression(parametersList, tokenType);
                    }
                    break;
                case TokenType.FunctionParameter:
                    {
                        var match = ReadToken(reader, tokenType, TokenType.DataType, TokenType.Identifier, TokenType.Optional, TokenType.VariableInitialValue);
                        if (match != null) return match;
                    }
                    break;
                case TokenType.VariableInitialValue:
                    {
                        if (token.TokenType != TokenType.Assign) goto leave;
                        var tokens = SeekToSeparator(reader);
                        return new TokenExpression(tokens, tokenType);
                    }
                    break;
                case TokenType.FunctionDeclaration:
                    {
                        var match = ReadToken(reader, tokenType, TokenType.Pound, TokenType.FunctionName, TokenType.FunctionParameterList);
                        if (match == null) match = ReadToken(reader, tokenType, TokenType.DataType, TokenType.FunctionName, TokenType.FunctionParameterList);
                        if (match == null) match = ReadToken(reader, tokenType, TokenType.FunctionName, TokenType.FunctionParameterList);
                        return match;
                    }
                    break;
                case TokenType.VariableDeclaration:
                    {
                        var match = ReadToken(reader, tokenType, TokenType.DataType, TokenType.VariableDeclarationList);
                        if (match != null) return match;
                    }
                    break;
                case TokenType.VariableDeclarationList:
                    {
                        var tokens = ReadTokens(reader, false, RepeatStyle.RepeatWithCommaUntilSemicolon, TokenType.VariableDeclarationEntry);
                        if (tokens != null) return new TokenExpression(tokens, tokenType);
                    }
                    break;
                case TokenType.VariableDeclarationEntry:
                    {
                        var match = ReadToken(reader, tokenType, TokenType.Identifier, TokenType.Optional, TokenType.ArrayBoundsDeclaration, TokenType.Optional, TokenType.VariableInitialValue);
                        if (match != null) return match;
                    }
                    break;
                case TokenType.ConstDeclaration:
                    {
                        var match = ReadToken(reader, tokenType, TokenType.Const, TokenType.VariableDeclaration);
                        if (match != null) return match;
                    }
                    break;
                case TokenType.FunctypeDeclaration:
                    {
                        var match = ReadToken(reader, tokenType, TokenType.Choice, TokenType.Functype, TokenType.Delegate, TokenType.DataType, TokenType.FunctionName, TokenType.FunctypeParameterList, TokenType.Semicolon);
                        if (match != null) return match;
                    }
                    break;
                case TokenType.FunctypeParameterList:
                    {
                        var match = ReadTokens(reader, true, RepeatStyle.RepeatWithCommaUntilCloseParenthesis, TokenType.FunctypeParameterEntry);
                        if (match != null) return new TokenExpression(match, tokenType);
                        var match2 = ReadToken(reader, tokenType, TokenType.Void, TokenType.ClosedParenthesis);
                        if (match2 != null) return match2;
                    }
                    break;
                case TokenType.FunctypeParameterEntry:
                    {
                        //may contain both data type and name of a parameter, discard the name.
                        var match = ReadToken(reader, TokenType.FunctypeParameterEntry, TokenType.DataType, TokenType.Optional, TokenType.Identifier);
                        if (match == null) goto leave;
                        //return just the data type
                        return match.Subexpressions[0];
                    }
                    break;
                case TokenType.ArrayBoundsDeclaration:
                    {
                        if (token.TokenType != TokenType.OpenBracket) goto leave;
                        var tokens = SeekToBraceEnd(reader);
                        if (tokens == null) goto leave;
                        var endToken = reader.Read();
                        if (endToken == null || endToken.TokenType != TokenType.CloseBracket) goto leave;
                        return new TokenExpression(tokens, tokenType);
                    }
                    break;
                case TokenType.Block:
                    {
                        if (token.TokenType == TokenType.OpenBrace)
                        {
                            var tokens = SeekToBraceEnd(reader);
                            if (tokens == null) goto leave;
                            var closeBraceToken = reader.Read();
                            if (closeBraceToken == null || closeBraceToken.TokenType != TokenType.CloseBrace) goto leave;
                            return new TokenExpression(tokens, TokenType.Block);
                        }
                    }
                    break;
                case TokenType.ClassDefinition:
                    {
                        var match = ReadToken(reader, tokenType, TokenType.Choice, TokenType.Struct, TokenType.Class, TokenType.Identifier, TokenType.ClassDefinitionEntries, TokenType.Semicolon);
                        if (match != null) return match;
                        //if (token.TokenType == TokenType.Class || token.TokenType == TokenType.Struct)
                        //{
                        //    var classToken = reader.Read();
                        //    var identifier = ReadToken(reader, TokenType.Identifier);
                        //    if (identifier == null) goto leave;

                        //    var openBrace = ReadToken(reader, TokenType.OpenBrace);
                        //    if (openBrace == null) goto leave;

                        //    var list = ReadTokens(reader, true, RepeatStyle.RepeatUntilEndOfBlock, TokenType.ClassDefinitionEntry);

                        //    var semicolon = ReadToken(reader, TokenType.Semicolon);

                        //    //var list = new List<TokenExpression>();
                        //    //list.Add(classToken);
                        //    //list.Add(identifier);
                        //    //while (true)
                        //    //{
                        //    //    var closeBrace = ReadToken(reader, TokenType.CloseBrace);
                        //    //    if (closeBrace != null)
                        //    //    {
                        //    //        break;
                        //    //    }
                        //    //    var entry = ReadToken(reader, TokenType.ClassDefinitionEntry);
                        //    //    if (entry == null) goto leave;
                        //    //    list.Add(entry);
                        //    //}
                        //    return new TokenExpression(list, tokenType);
                        //}
                    }
                    break;
                case TokenType.ClassDefinitionEntries:
                    {
                        //open brace, list of class definition entries, close brace
                        var openBraceToken = ReadToken(reader, TokenType.OpenBrace);
                        if (openBraceToken == null) goto leave;
                        var entries = ReadTokens(reader, true, RepeatStyle.RepeatUntilEndOfBlock, TokenType.ClassDefinitionEntry);
                        if (entries == null) goto leave;
                        return new TokenExpression(entries, tokenType);
                    }
                    break;
                case TokenType.ClassDefinitionEntry:
                    {
                        //todo: constant value

                        var match = ReadToken(reader, tokenType, TokenType.FunctionDeclaration, TokenType.Choice, TokenType.Semicolon, TokenType.Block);
                        if (match != null) return match;
                        match = ReadToken(reader, tokenType, TokenType.Optional, TokenType.Const, TokenType.VariableDeclaration);
                        if (match != null) return match;
                        match = ReadToken(reader, tokenType, TokenType.Choice, TokenType.Public, TokenType.Private, TokenType.Colon);
                        if (match != null) return match;
                    }
                    break;
            }
            return null;
        leave:
            reader.Index = startIndex;
            return null;
        }

        private List<TokenExpression> SeekToBraceEnd(TokenReader reader)
        {
            int parenthesisLevel = 0;
            int bracketLevel = 0;
            int braceLevel = 0;

            var list = new List<TokenExpression>();

            while (true)
            {
                var token = reader.Peek();
                if (token == null)
                {
                    break;
                }
                switch (token.TokenType)
                {
                    case TokenType.OpenParenthesis:
                        if (parenthesisLevel == 0 && bracketLevel == 0 && braceLevel == 0)
                        {
                            parenthesisLevel++;
                            reader.Index++;
                            continue;
                        }
                        parenthesisLevel++;
                        break;
                    case TokenType.OpenBracket:
                        if (parenthesisLevel == 0 && bracketLevel == 0 && braceLevel == 0)
                        {
                            bracketLevel++;
                            reader.Index++;
                            continue;
                        }
                        bracketLevel++;
                        break;
                    case TokenType.OpenBrace:
                        if (parenthesisLevel == 0 && bracketLevel == 0 && braceLevel == 0)
                        {
                            braceLevel++;
                            reader.Index++;
                            continue;
                        }
                        braceLevel++;
                        break;
                    case TokenType.ClosedParenthesis:
                        parenthesisLevel--;
                        if (parenthesisLevel < 0) return null;

                        if (parenthesisLevel == 0 && bracketLevel == 0 && braceLevel == 0)
                        {
                            goto leave;
                        }
                        break;
                    case TokenType.CloseBracket:
                        bracketLevel--;
                        if (bracketLevel < 0) return null;
                        if (parenthesisLevel == 0 && bracketLevel == 0 && braceLevel == 0)
                        {
                            goto leave;
                        }
                        break;
                    case TokenType.CloseBrace:
                        braceLevel--;
                        if (braceLevel < 0) return null;
                        if (parenthesisLevel == 0 && bracketLevel == 0 && braceLevel == 0)
                        {
                            goto leave;
                        }
                        break;
                }
                reader.Index++;
                list.Add(token);
            }
        leave:
            ;
            return list;
        }

        private List<TokenExpression> SeekToSeparator(TokenReader reader)
        {
            int parenthesisLevel = 0;
            int bracketLevel = 0;
            int braceLevel = 0;

            var list = new List<TokenExpression>();

            while (true)
            {
                var token = reader.Peek();
                if (token == null)
                {
                    break;
                }
                switch (token.TokenType)
                {
                    case TokenType.OpenParenthesis:
                        parenthesisLevel++;
                        break;
                    case TokenType.OpenBracket:
                        bracketLevel++;
                        break;
                    case TokenType.OpenBrace:
                        braceLevel++;
                        break;
                    case TokenType.Comma:
                        if (parenthesisLevel == 0 && bracketLevel == 0 && braceLevel == 0)
                        {
                            goto leave;
                        }
                        break;
                    case TokenType.Semicolon:
                        if (parenthesisLevel == 0 && bracketLevel == 0 && braceLevel == 0)
                        {
                            goto leave;
                        }
                        break;
                    case TokenType.ClosedParenthesis:
                        parenthesisLevel--;
                        if (parenthesisLevel < 0) goto leave;
                        break;
                    case TokenType.CloseBracket:
                        bracketLevel--;
                        if (bracketLevel < 0) goto leave;
                        break;
                    case TokenType.CloseBrace:
                        braceLevel--;
                        if (braceLevel < 0) goto leave;
                        break;
                }
                reader.Index++;
                list.Add(token);
            }
        leave:
            ;
            return list;

        }

        private List<TokenExpression> ReadToSemicolon(TokenReader reader)
        {
            var list = new List<TokenExpression>();
            while (true)
            {
                var token = reader.Read();
                if (token == null)
                {
                    break;
                }
                if (token.TokenType == TokenType.Semicolon)
                {
                    break;
                }
                list.Add(token);
            }
            return list;
        }

        private List<TokenExpression> ReadDataType(TokenReader reader)
        {
            var list = new List<TokenExpression>();
            var token = reader.Peek();
            if (token == null)
            {
                //fail
            }
            if (token.TokenType == TokenType.Ref)
            {
                reader.Read();
                list.Add(token);
                token = reader.Peek();
            }
            if (token.TokenType == TokenType.Array)
            {
                reader.Read();
                list.Add(token);
                token = reader.Peek();

                //next token must be @
                if (token.TokenType != TokenType.At)
                {
                    //fail
                }

                reader.Read();
                list.Add(token);
                token = reader.Peek();

                //next token must be a data type
                if (token.IsDataType() || token.TokenType == TokenType.Identifier)
                {

                }
                else
                {
                    //fail
                }

                reader.Read();
                list.Add(token);
                token = reader.Peek();

                //next token might be another @
                if (token.TokenType == TokenType.At)
                {
                    reader.Read();
                    list.Add(token);
                    token = reader.Peek();

                    //next token must be a number
                    if (token.TokenType == TokenType.Number && token.Token.IsInt())
                    {

                    }
                    else
                    {
                        //fail
                    }
                    reader.Read();
                    list.Add(token);
                    token = reader.Peek();
                }
            }
            else
            {
                //next token must be a data type
                if (token.IsDataType() || token.TokenType == TokenType.Identifier)
                {

                }
                else
                {
                    //fail
                }
                reader.Read();
                list.Add(token);
            }
            return list;
        }

        private List<TokenExpression> ReadToClosingBrace(TokenReader reader)
        {
            var list = new List<TokenExpression>();
            int braceLevel = 1;
            while (true)
            {
                var token = reader.Read();

                if (token == null)
                {
                    break;
                }

                if (token.TokenType == TokenType.OpenBrace)
                {
                    braceLevel++;
                }
                else if (token.TokenType == TokenType.CloseBrace)
                {
                    braceLevel--;
                    if (braceLevel == 0)
                    {
                        break;
                    }
                }
                list.Add(token);
            }
            return list;
        }

    }
}
