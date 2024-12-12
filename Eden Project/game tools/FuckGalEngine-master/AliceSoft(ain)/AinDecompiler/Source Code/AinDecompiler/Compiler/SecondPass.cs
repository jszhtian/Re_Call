using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Text;

namespace AinDecompiler.Compiler
{
    public partial class SecondPass
    {
        public SecondPass(Compiler parent)
        {
            this.Parent = parent;
        }

        Compiler Parent;


        class TokenReader : MyEnumerator2<TokenExpression>
        {
            public TokenReader(IList<TokenExpression> array)
                : base(array)
            {

            }
        }

        TokenReader reader;

        TokenExpression ReadToken(TokenType tokenType)
        {
            var token = reader.Peek();
            if (token == null) return null;
            int startPosition = reader.Index;

            switch (tokenType)
            {
                default:
                    {
                        if (token.TokenType != tokenType) goto leave;
                        reader.Read();
                        return token;
                    }
                    break;
                case TokenType.Statement:
                    {
                        switch (token.TokenType)
                        {
                            //check for keyword
                            case TokenType.Assert:
                            case TokenType.Message:
                            case TokenType.If:
                            case TokenType.Break:
                            case TokenType.Continue:
                            case TokenType.Jumps:
                            case TokenType.Return:
                            case TokenType.Goto:
                            case TokenType.Jump:
                            case TokenType.While:
                            case TokenType.For:
                            case TokenType.Switch:
                            case TokenType.Case:
                            case TokenType.Default:
                            case TokenType.Block:
                            case TokenType.Const:
                                {
                                    var result = ReadToken(token.TokenType);
                                    return result;
                                }
                                break;
                            //a data type token indicates a variable declaration
                            case TokenType.Ref:
                            case TokenType.Array:
                            case TokenType.Int:
                            case TokenType.Float:
                            case TokenType.String:
                            case TokenType.Void:
                            case TokenType.Lint:
                            case TokenType.Bool:
                            case TokenType.IMainSystem:
                                {
                                    var variableDeclaration = ReadToken(TokenType.VariableDeclaration);
                                    if (variableDeclaration == null) goto leave;
                                    return variableDeclaration;
                                }

                            //case TokenType.Ifdef:
                            //case TokenType.Endif:
                            //case TokenType.Assert:
                            case TokenType.Identifier:
                                //check for label or variable declaration
                                {
                                    var label = ReadToken(TokenType.Label);
                                    if (label != null) return label;

                                    string name = token.Token.Value;

                                    if (Symbols.ContainsKey(name))
                                    {
                                        var type = Symbols[name];
                                        if (type as Struct != null || type as FunctionType != null)
                                        {
                                            var variableDeclaration = ReadToken(TokenType.VariableDeclaration);
                                            if (variableDeclaration != null) return variableDeclaration;
                                        }
                                    }
                                }
                                break;
                            case TokenType.OpenBrace:
                                //a block in the middle of code
                                {
                                    var block = ReadToken(TokenType.Block);
                                    if (block == null) goto leave;
                                    return block;
                                }
                        }

                        //check for expression and semicolon
                        var expression = ReadToken(TokenType.Expression);
                        if (expression == null) goto leave;
                        var semicolon = ReadToken(TokenType.Semicolon);
                        if (semicolon == null) goto leave;
                        return expression;
                    }
                    break;
                case TokenType.Expression:
                    {
                        var expression = ReadExpression();
                        if (expression == null) goto leave;
                        return expression;
                    }
                    break;
                case TokenType.Const:
                    {
                        var constToken = reader.Read();
                        var variableDeclaration = ReadToken(TokenType.VariableDeclaration);
                        if (variableDeclaration == null) goto leave;
                        constToken.Subexpressions.Add(variableDeclaration);
                        return constToken;
                    }
                    break;
                case TokenType.VariableDeclaration:
                    {
                        var variableDeclarationToken = new TokenExpression("VariableDeclaration");
                        variableDeclarationToken.TokenType = TokenType.VariableDeclaration;

                        //TODO
                        var dataType = ReadToken(TokenType.DataType);
                        if (dataType == null) goto leave;

                        variableDeclarationToken.Subexpressions.Add(dataType);

                        while (true)
                        {
                            var identifier = ReadToken(TokenType.Identifier);
                            if (identifier == null) goto leave;

                            var separator = reader.Read();
                            if (separator == null) goto leave;
                            if (separator.TokenType == TokenType.Assign)
                            {
                                var nextToken = reader.Peek();  //deleteme
                                var assignToken = separator;
                                bool oldCommaOkay = _commaOkay;
                                _commaOkay = false;
                                var expression = ReadToken(TokenType.Expression);
                                _commaOkay = oldCommaOkay;

                                assignToken.Subexpressions.Add(identifier);
                                assignToken.Subexpressions.Add(expression);
                                variableDeclarationToken.Subexpressions.Add(assignToken);

                                separator = reader.Read();
                                if (separator == null) goto leave;
                            }
                            else
                            {
                                variableDeclarationToken.Subexpressions.Add(identifier);
                            }

                            if (separator.TokenType == TokenType.Comma)
                            {
                                //var comma = separator;
                            }
                            else if (separator.TokenType == TokenType.Semicolon)
                            {
                                //var semicolon = separator;
                                break;
                            }
                        }

                        return variableDeclarationToken;
                    }
                    break;
                case TokenType.DataType:
                    {
                        var dataType = new TokenExpression(TokenType.DataType);
                        if (token.TokenType == TokenType.Ref)
                        {
                            dataType.Subexpressions.Add(reader.Read());
                            token = reader.Peek();
                            if (token == null) goto leave;
                        }
                        if (token.TokenType == TokenType.Array)
                        {
                            dataType.Subexpressions.Add(reader.Read());
                            var at = ReadToken(TokenType.At);
                            if (at == null) goto leave;
                            var arrayType = reader.Read();
                            if (arrayType == null) goto leave;
                            if (!(arrayType.IsDataType() || arrayType.TokenType == TokenType.Identifier)) goto leave;
                            dataType.Subexpressions.Add(arrayType);
                            at = reader.Peek();
                            if (at != null && at.TokenType == TokenType.At)
                            {
                                reader.Read();
                                var dimensions = ReadToken(TokenType.Number);
                                if (dimensions == null) goto leave;
                                dataType.Subexpressions.Add(dimensions);
                            }
                        }
                        else
                        {
                            token = reader.Read();
                            if (!(token.IsDataType() || token.TokenType == TokenType.Identifier)) goto leave;
                            dataType.Subexpressions.Add(token);
                        }
                        return dataType;
                    }
                    break;
                case TokenType.Label:
                    {
                        var identifier = ReadToken(TokenType.Identifier);
                        if (identifier == null) goto leave;
                        var colon = ReadToken(TokenType.Colon);
                        if (colon == null) goto leave;
                        return colon;
                    }
                    break;
                //case TokenType.ExpressionAndSemicolon:
                //    {
                //        //expression may be null - used by for loop and return statement
                //        var expression = ReadToken(TokenType.Expression);
                //        var semicolon = ReadToken(TokenType.Semicolon);
                //        if (semicolon == null) goto leave;
                //        if (expression == null) return semicolon;
                //        return expression;
                //    }
                //    break;
                case TokenType.BlockOrSingleStatement:
                    {
                        if (token.TokenType == TokenType.OpenBrace)
                        {
                            var block = ReadToken(TokenType.Block);
                            return block;
                        }
                        var expression = ReadToken(TokenType.Statement);
                        return expression;
                    }
                    break;
                case TokenType.Block:
                    {
                        var blockToken = new TokenExpression("...");
                        blockToken.TokenType = TokenType.Block;
                        // { statements }
                        var openBrace = ReadToken(TokenType.OpenBrace);
                        if (openBrace == null) return null;
                        while (true)
                        {
                            token = reader.Peek();
                            if (token == null) return null;
                            if (token.TokenType == TokenType.CloseBrace)
                            {
                                reader.Read();
                                break;
                            }
                            var statement = ReadToken(TokenType.Statement);
                            if (statement == null) goto leave;
                            blockToken.Subexpressions.Add(statement);
                        }
                        return blockToken;
                    }
                    break;


                case TokenType.Message:
                    {
                        var message = reader.Read();
                        return message;
                    }
                    break;
                case TokenType.If:
                    {
                        var newToken = reader.Read().Clone();
                        var openParenthesis = ReadToken(TokenType.OpenParenthesis);
                        if (openParenthesis == null) goto leave;
                        var expression = ReadToken(TokenType.Expression);
                        if (expression == null) goto leave;
                        var closedParenthesis = ReadToken(TokenType.ClosedParenthesis);
                        if (closedParenthesis == null) goto leave;
                        var ifBody = ReadToken(TokenType.BlockOrSingleStatement);
                        if (ifBody == null) goto leave;
                        var elseToken = ReadToken(TokenType.Else);
                        TokenExpression elseBody = null;
                        if (elseToken != null)
                        {
                            elseBody = ReadToken(TokenType.BlockOrSingleStatement);
                            if (elseBody == null) goto leave;
                        }
                        newToken.Subexpressions.Add(expression);
                        newToken.Subexpressions.Add(ifBody);
                        if (elseBody != null) newToken.Subexpressions.Add(elseBody);
                        return newToken;
                    }
                    break;
                case TokenType.Break:
                case TokenType.Continue:
                    {
                        var statementToken = reader.Read();
                        var semicolon = ReadToken(TokenType.Semicolon);
                        if (semicolon == null) goto leave;
                        return statementToken;
                    }
                    break;
                case TokenType.Jumps:
                case TokenType.Return:
                    {
                        var returnToken = reader.Read().Clone();
                        var nextToken = reader.Peek();
                        if (nextToken != null && nextToken.TokenType == TokenType.Semicolon)
                        {
                            nextToken = reader.Read();
                            return returnToken;
                        }
                        var expression = ReadToken(TokenType.Expression);
                        if (expression == null) goto leave;
                        var semicolon = ReadToken(TokenType.Semicolon);
                        if (semicolon == null) goto leave;
                        returnToken.Subexpressions.Add(expression);
                        return returnToken;
                    }
                    break;
                case TokenType.Goto:
                case TokenType.Jump:
                    {
                        var gotoToken = reader.Read().Clone();
                        var labelName = ReadToken(TokenType.Identifier);
                        if (labelName == null) goto leave;
                        var semicolon = ReadToken(TokenType.Semicolon);
                        if (semicolon == null) goto leave;
                        gotoToken.Subexpressions.Add(labelName);
                        return gotoToken;
                    }
                    break;
                case TokenType.While:
                    {
                        var whileToken = reader.Read().Clone();
                        var openParenthesisToken = ReadToken(TokenType.OpenParenthesis);
                        if (openParenthesisToken == null) goto leave;
                        var expression = ReadToken(TokenType.Expression);
                        if (expression == null) goto leave;
                        var closedParenthesisToken = ReadToken(TokenType.ClosedParenthesis);
                        if (closedParenthesisToken == null) goto leave;
                        var loopBody = ReadToken(TokenType.BlockOrSingleStatement);
                        if (loopBody == null) goto leave;
                        whileToken.Subexpressions.Add(expression);
                        whileToken.Subexpressions.Add(loopBody);
                        return whileToken;
                    }
                    break;
                case TokenType.For:
                    {
                        var forToken = reader.Read().Clone();
                        var openParenthesisToken = ReadToken(TokenType.OpenParenthesis);
                        if (openParenthesisToken == null) goto leave;
                        TokenExpression initExpression = null;
                        TokenExpression condExpression = null;
                        TokenExpression incExpression = null;
                        TokenExpression separator;

                        separator = ReadToken(TokenType.Semicolon);
                        if (separator == null)
                        {
                            initExpression = ReadToken(TokenType.Expression);
                            if (initExpression == null) goto leave;
                            separator = ReadToken(TokenType.Semicolon);
                            if (separator == null) goto leave;
                        }
                        separator = ReadToken(TokenType.Semicolon);
                        if (separator == null)
                        {
                            condExpression = ReadToken(TokenType.Expression);
                            if (condExpression == null) goto leave;
                            separator = ReadToken(TokenType.Semicolon);
                            if (separator == null) goto leave;
                        }
                        separator = ReadToken(TokenType.ClosedParenthesis);
                        if (separator == null)
                        {
                            incExpression = ReadToken(TokenType.Expression);
                            if (incExpression == null) goto leave;
                            separator = ReadToken(TokenType.ClosedParenthesis);
                            if (separator == null) goto leave;
                        }

                        var loopBody = ReadToken(TokenType.BlockOrSingleStatement);
                        if (loopBody == null) goto leave;

                        forToken.Subexpressions.Add(initExpression);
                        forToken.Subexpressions.Add(condExpression);
                        forToken.Subexpressions.Add(incExpression);
                        forToken.Subexpressions.Add(loopBody);
                        return forToken;
                    }
                    break;
                case TokenType.Switch:
                    {
                        var switchToken = reader.Read().Clone();
                        var openParenthesisToken = ReadToken(TokenType.OpenParenthesis);
                        if (openParenthesisToken == null) goto leave;
                        var expression = ReadToken(TokenType.Expression);
                        if (expression == null) goto leave;
                        var closedParenthesisToken = ReadToken(TokenType.ClosedParenthesis);
                        if (closedParenthesisToken == null) goto leave;
                        var block = ReadToken(TokenType.Block);
                        if (block == null) goto leave;
                        switchToken.Subexpressions.Add(expression);
                        switchToken.Subexpressions.Add(block);
                        return switchToken;
                    }
                    break;
                case TokenType.Case:
                    {
                        var caseToken = reader.Read().Clone();
                        var expression = ReadToken(TokenType.Expression);
                        if (expression == null) goto leave;
                        var colon = ReadToken(TokenType.Colon);
                        if (colon == null) goto leave;
                        caseToken.Subexpressions.Add(expression);
                        return caseToken;
                    }
                    break;
                case TokenType.Assert:
                    {
                        var assertToken = reader.Read();
                        if (assertToken == null) goto leave;
                        var openParenthesis = ReadToken(TokenType.OpenParenthesis);
                        if (openParenthesis == null) goto leave;
                        var expression = ReadToken(TokenType.Expression);
                        if (expression == null) goto leave;
                        var closedParenthesis = ReadToken(TokenType.ClosedParenthesis);
                        if (closedParenthesis == null) goto leave;
                        var semicolon = ReadToken(TokenType.Semicolon);
                        if (semicolon == null) goto leave;

                        assertToken = assertToken.Clone();
                        assertToken.Subexpressions.Add(expression);
                        return assertToken;
                    }
                    break;
                case TokenType.Default:
                    {
                        var defaultToken = reader.Read();
                        var colon = ReadToken(TokenType.Default);
                        if (colon == null) goto leave;
                        return defaultToken;
                    }
                    break;
            }




            reader.Index = startPosition;
            return null;
        leave:
            reader.Index = startPosition;
            return null;
        }

        static Dictionary<TokenType, int> InfixOperatorPrecedence = GetInfixOperatorPrecdence();
        static Dictionary<TokenType, int> PrefixOperatorPrecedence = GetPrefixOperatorPrecedence();
        static Dictionary<TokenType, int> PostfixOperatorPrecedence = GetPostfixOperatorPrecedence();
        static HashSet<TokenType> RightAssociativeOperators = GetRightAssociativeOperators();

        private static HashSet<TokenType> GetRightAssociativeOperators()
        {
            var set = new HashSet<TokenType>();

            set.Add(TokenType.Assign);
            set.Add(TokenType.PlusAssign);
            set.Add(TokenType.MinusAssign);
            set.Add(TokenType.MultiplyAssign);
            set.Add(TokenType.DivideAssign);
            set.Add(TokenType.ModuloAssign);
            set.Add(TokenType.LeftShiftAssign);
            set.Add(TokenType.RightShiftAssign);
            set.Add(TokenType.AndAssign);
            set.Add(TokenType.XorAssign);
            set.Add(TokenType.OrAssign);
            set.Add(TokenType.QuestionMark);
            set.Add(TokenType.Colon);
            set.Add(TokenType.ReferenceAssign);
            set.Add(TokenType.ReferenceSwap);
            set.Add(TokenType.Comma);
            return set;
        }

        private static Dictionary<TokenType, int> GetPostfixOperatorPrecedence()
        {
            var dic = new Dictionary<TokenType, int>();
            dic.Add(TokenType.OpenParenthesis, 32);
            dic.Add(TokenType.OpenBracket, 32);
            dic.Add(TokenType.Increment, 32);
            dic.Add(TokenType.Decrement, 32);
            return dic;
        }

        private static Dictionary<TokenType, int> GetPrefixOperatorPrecedence()
        {
            var dic = new Dictionary<TokenType, int>();
            dic.Add(TokenType.Increment, 32);
            dic.Add(TokenType.Decrement, 32);
            dic.Add(TokenType.Not, 30);
            dic.Add(TokenType.Complement, 30);
            dic.Add(TokenType.Minus, 28); //negative
            dic.Add(TokenType.Plus, 28); //positive
            dic.Add(TokenType.And, 28); //'address of' a function
            dic.Add(TokenType.New, 28);

            dic.Add(TokenType.Comma, 2);  //empty comma expression (for function argument lists)
            return dic;
        }

        private static Dictionary<TokenType, int> GetInfixOperatorPrecdence()
        {
            var dic = new Dictionary<TokenType, int>();
            dic.Add(TokenType.Dot, 32);
            dic.Add(TokenType.Multiply, 24);
            dic.Add(TokenType.Divide, 24);
            dic.Add(TokenType.Modulo, 24);
            dic.Add(TokenType.Plus, 22);
            dic.Add(TokenType.Minus, 22);
            dic.Add(TokenType.LeftShift, 20);
            dic.Add(TokenType.RightShift, 20);
            dic.Add(TokenType.LessThan, 18);
            dic.Add(TokenType.LessThanOrEqualTo, 18);
            dic.Add(TokenType.GreaterThan, 18);
            dic.Add(TokenType.GreaterThanOrEqualTo, 18);
            dic.Add(TokenType.EqualTo, 16);
            dic.Add(TokenType.NotEqualTo, 16);
            dic.Add(TokenType.ReferenceEqualTo, 16);
            dic.Add(TokenType.ReferenceNotEqualTo, 16);
            dic.Add(TokenType.And, 14);
            dic.Add(TokenType.Xor, 12);
            dic.Add(TokenType.Or, 10);
            dic.Add(TokenType.LogicalAnd, 8);
            dic.Add(TokenType.LogicalOr, 6);
            dic.Add(TokenType.Assign, 4);
            dic.Add(TokenType.PlusAssign, 4);
            dic.Add(TokenType.MinusAssign, 4);
            dic.Add(TokenType.MultiplyAssign, 4);
            dic.Add(TokenType.DivideAssign, 4);
            dic.Add(TokenType.ModuloAssign, 4);
            dic.Add(TokenType.LeftShiftAssign, 4);
            dic.Add(TokenType.RightShiftAssign, 4);
            dic.Add(TokenType.AndAssign, 4);
            dic.Add(TokenType.XorAssign, 4);
            dic.Add(TokenType.OrAssign, 4);
            dic.Add(TokenType.QuestionMark, 4);
            dic.Add(TokenType.Colon, 4);
            dic.Add(TokenType.ReferenceAssign, 4);
            dic.Add(TokenType.ReferenceSwap, 4);
            dic.Add(TokenType.Comma, 2);
            return dic;
        }

        bool _colonOkay = false;
        bool _commaOkay = true;

        private TokenExpression ReadExpression()
        {
            bool oldColonOkey = _colonOkay;
            var expr = ReadExpression(0);
            _colonOkay = oldColonOkey;
            return expr;
        }

        bool IsPrefixOperator(TokenType tokenType)
        {
            return PrefixOperatorPrecedence.ContainsKey(tokenType) && !(!_commaOkay && tokenType == TokenType.Comma);
        }
        private bool IsInfixOperator(TokenType tokenType)
        {
            return InfixOperatorPrecedence.ContainsKey(tokenType) && !(!_commaOkay && tokenType == TokenType.Comma) && !(!_colonOkay && tokenType == TokenType.Colon);
        }

        private TokenExpression ReadExpression(int minimumPrecedence)
        {
            //TODO: Fix ? : operator (make : not work by itself)

            var token = reader.Peek();
            if (token == null) return null;
            var tokenType = token.TokenType;

            int currentOperatorPrecedence = -1;

            TokenExpression expression = null;

            if (tokenType == TokenType.OpenParenthesis)
            {
                var parenthesisToken = reader.Read();
                expression = ReadExpression();
                if (expression == null) return null;
                var closedParenthesisToken = ReadToken(TokenType.ClosedParenthesis);
                if (closedParenthesisToken == null) return null;

                token = reader.Peek();
            }
            else if (IsPrefixOperator(tokenType))
            {
                var prefixOperator = reader.Read().Clone();
                currentOperatorPrecedence = PrefixOperatorPrecedence[tokenType];

                if (tokenType == TokenType.Increment) prefixOperator.TokenType = TokenType.PreIncrement;
                if (tokenType == TokenType.Decrement) prefixOperator.TokenType = TokenType.PreDecrement;
                if (tokenType == TokenType.Minus) prefixOperator.TokenType = TokenType.Negative;
                if (tokenType == TokenType.Plus) prefixOperator.TokenType = TokenType.Positive;
                if (tokenType == TokenType.Comma) prefixOperator.Subexpressions.Add(null);
                if (tokenType == TokenType.And) prefixOperator.TokenType = TokenType.AddressOf;

                expression = prefixOperator;
                var nextExpression = ReadExpression(currentOperatorPrecedence);
                if (nextExpression == null) return null;
                expression.Subexpressions.Add(nextExpression);

                token = reader.Peek();
            }
            else
            {
                //check for a symbol or literal
                if (tokenType == TokenType.Identifier ||
                    tokenType == TokenType.Number ||
                    tokenType == TokenType.StringLiteral ||
                    tokenType == TokenType.Message ||
                    tokenType == TokenType.Int ||
                    tokenType == TokenType.String ||
                    tokenType == TokenType.Float)
                {
                    token = reader.Read();

                    if (tokenType == TokenType.Message)
                    {
                        token = token.Clone();
                        token.TokenType = TokenType.CharLiteral;
                        tokenType = TokenType.CharLiteral;
                    }
                    expression = token;
                }
                else
                {
                    return null;
                }

                token = reader.Peek();
            }

            //allow end of input
            if (token == null)
            {
                return expression;
            }
            tokenType = token.TokenType;

        continueParsing:

            currentOperatorPrecedence = -1;

            if (PostfixOperatorPrecedence.ContainsKey(tokenType))
            {
                currentOperatorPrecedence = PostfixOperatorPrecedence[tokenType];
                if (currentOperatorPrecedence >= minimumPrecedence)
                {
                    //check for parenthesis or braces
                    if (tokenType == TokenType.OpenParenthesis)
                    {
                        bool oldCommaOkay = _commaOkay;
                        _commaOkay = true;

                        var functionCall = reader.Read().Clone();
                        functionCall.TokenType = TokenType.FunctionCall;

                        var innerExpression = ReadExpression();
                        _commaOkay = oldCommaOkay;

                        //innerExpression may be null
                        var closedParenthesis = ReadToken(TokenType.ClosedParenthesis);
                        if (closedParenthesis == null) return null;

                        functionCall.Subexpressions.Add(expression);
                        functionCall.Subexpressions.Add(innerExpression);
                        expression = functionCall;
                    }
                    else if (tokenType == TokenType.OpenBracket)
                    {
                        var arrayIndex = reader.Read().Clone();
                        arrayIndex.TokenType = TokenType.ArrayIndex;
                        var innerExpression = ReadExpression();
                        if (innerExpression == null) return null;
                        var closedBracket = ReadToken(TokenType.CloseBracket);
                        if (closedBracket == null) return null;

                        arrayIndex.Subexpressions.Add(expression);
                        arrayIndex.Subexpressions.Add(innerExpression);
                        expression = arrayIndex;
                    }
                    else
                    {
                        var postfixOperator = reader.Read().Clone();
                        if (postfixOperator.TokenType == TokenType.Increment) postfixOperator.TokenType = TokenType.PostIncrement;
                        if (postfixOperator.TokenType == TokenType.Decrement) postfixOperator.TokenType = TokenType.PostDecrement;

                        postfixOperator.Subexpressions.Add(expression);
                        expression = postfixOperator;
                    }
                    token = reader.Peek();
                    if (token == null) return expression;
                    tokenType = token.TokenType;
                }
            }

            if (IsInfixOperator(tokenType))
            {
                currentOperatorPrecedence = InfixOperatorPrecedence[tokenType];
                if (currentOperatorPrecedence >= minimumPrecedence)
                {
                    if (tokenType == TokenType.QuestionMark)
                    {
                        _colonOkay = true;
                    }
                    //else if (tokenType == TokenType.Colon)
                    //{
                    //    if (_colonOkay)
                    //    {

                    //    }
                    //    else
                    //    {
                    //        return expression;
                    //    }
                    //}

                    var infixOperator = reader.Read().Clone();
                    bool isRightAssociative = RightAssociativeOperators.Contains(tokenType);
                    var nextExpression = ReadExpression(currentOperatorPrecedence + (isRightAssociative ? 0 : 1));

                    if (tokenType == TokenType.QuestionMark)
                    {
                        _colonOkay = false;
                    }

                    if (nextExpression == null) return null;

                    infixOperator.Subexpressions.Add(expression);
                    infixOperator.Subexpressions.Add(nextExpression);
                    expression = infixOperator;

                    token = reader.Peek();
                    if (token == null) return expression;
                    tokenType = token.TokenType;
                }
            }

            if (currentOperatorPrecedence == -1)
            {
                return expression;
            }

            if (currentOperatorPrecedence >= minimumPrecedence && (IsInfixOperator(tokenType) || PostfixOperatorPrecedence.ContainsKey(tokenType)))
            {
                goto continueParsing;
            }

            return expression;
        }

        Dictionary<Function, TokenExpression> FunctionDefinitionTokens
        {
            get
            {
                return this.Parent.FunctionDefinitionTokens;
            }
        }

        Dictionary<Function, TokenExpression> FunctionDeclarationTokens
        {
            get
            {
                return this.Parent.FunctionDeclarationTokens;
            }
        }

        Dictionary<string, IVariable> Symbols
        {
            get
            {
                return this.Parent.Symbols;
            }
        }

        public void CompileSecondPass()
        {
            var oldReader = this.reader;
            foreach (var pair in this.FunctionDefinitionTokens)
            {
                var function = pair.Key;
                var token = pair.Value;
                this.reader = new TokenReader(token.Subexpressions);

                List<TokenExpression> definition = new List<TokenExpression>();
                while (true)
                {
                    var nextToken = reader.Peek();
                    if (nextToken == null)
                    {
                        break;
                    }
                    var statement = ReadToken(TokenType.Statement);
                    if (statement != null)
                    {
                        definition.Add(statement);
                    }
                    else
                    {

                    }
                }
                token.Subexpressions.Clear();
                token.Subexpressions.AddRange(definition);
            }

            this.reader = oldReader;
        }
    }
}
