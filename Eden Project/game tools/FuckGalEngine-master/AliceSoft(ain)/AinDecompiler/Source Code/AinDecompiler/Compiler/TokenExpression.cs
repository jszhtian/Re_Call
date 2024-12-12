using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Text;
using System.CodeDom.Compiler;

namespace AinDecompiler.Compiler
{
    public class TokenExpression
    {
        static KeywordTable keywordTable = new KeywordTable();

        public Token Token;
        public int Row;
        public int Column;

        public TokenType TokenType;
        public TokenExpression(IEnumerable<TokenExpression> expressions, TokenType tokenType)
            : this(new Token("..."))
        {
            this.TokenType = tokenType;
            if (expressions != null)
            {
                Subexpressions.AddRange(expressions);
            }

            if (tokenType == TokenType.First)
            {
                var first = Subexpressions.FirstOrDefault();
                if (first != null)
                {
                    this.TokenType = first.TokenType;
                }
            }
        }

        public TokenExpression(IEnumerable<TokenExpression> expressions)
            : this(expressions, TokenType.Block)
        {

        }

        public TokenExpression(TokenType tokenType)
            : this(tokenType.ToString())
        {
            this.TokenType = tokenType;
        }

        public TokenExpression(string text)
            : this(new Token(text))
        {

        }

        public TokenExpression()
            : this("...")
        {
            this.TokenType = TokenType.Block;
        }

        public TokenExpression(Token token)
        {
            this.Token = token;
            if (token.QuoteCharacter != 0)
            {
                if (token.QuoteCharacter == '"')
                {
                    this.TokenType = TokenType.StringLiteral;
                    return;
                }
                else if (token.QuoteCharacter == '\'')
                {
                    this.TokenType = TokenType.Message;
                    return;
                }
            }
            else if (token.IsNumber())
            {
                this.TokenType = TokenType.Number;
                return;
            }
            this.TokenType = keywordTable.GetOrDefault(token.Value, TokenType.Identifier);
            if (this.TokenType != TokenType.Identifier)
            {
                this.Token.Value = String.Intern(this.Token.Value);
            }
        }
        public List<TokenExpression> Subexpressions = new List<TokenExpression>();

        public bool IsMacro()
        {
            if (TokenType >= TokenType.FileMacro && TokenType <= TokenType.TimeMacro)
            {
                return true;
            }
            return false;
        }

        public bool IsDataType()
        {
            switch (TokenType)
            {
                case TokenType.Void:
                case TokenType.Int:
                case TokenType.Bool:
                case TokenType.String:
                case TokenType.Float:
                case TokenType.Lint:
                case TokenType.IMainSystem:
                    return true;
            }
            return false;
        }

        public bool IsBuiltInMethod()
        {
            return false;
            //return TokenType >= TokenType.Numof && TokenType <= TokenType.ConvertToInt;
        }

        private string ToStringLeaf()
        {
            if (this.TokenType == TokenType.Identifier ||
                this.TokenType == TokenType.Number ||
                this.TokenType == TokenType.StringLiteral ||
                this.TokenType == TokenType.Message)
            {
                if (this.Token.QuoteCharacter != 0)
                {
                    return this.Token.QuoteCharacter + this.Token.Value + this.Token.QuoteCharacter;
                }
                return this.Token.Value;
            }
            else
            {
                return this.TokenType.ToString();
            }
        }

        public override string ToString()
        {
            StringBuilder sb = new StringBuilder();
            sb.Append(this.ToStringLeaf());

            if (this.Subexpressions.Count > 0)
            {
                int limit = Math.Min(this.Subexpressions.Count, 2);
                for (int i = 0; i < limit; i++)
                {
                    sb.Append(" ");
                    var subexpression = this.Subexpressions[i];
                    if (subexpression == null)
                    {
                        sb.Append("null");
                    }
                    else
                    {
                        sb.Append(subexpression.ToStringLeaf());
                        if (subexpression.Subexpressions.Count > 0)
                        {
                            sb.Append("...");
                        }
                    }
                }
            }
            return sb.ToString();
        }

        public string ToIndentedString()
        {
            var sb = new StringBuilder();
            var sw = new StringWriter(sb);
            IndentedTextWriter tw = new IndentedTextWriter(sw, " ");
            this.WriteIndentedString(tw);

            tw.Flush();
            return sb.ToString();
        }

        private void WriteIndentedString(IndentedTextWriter tw)
        {
            int depth = tw.Indent;
            if (this.TokenType == TokenType.Identifier)
            {
                tw.WriteLine(this.Token.Value);
            }
            else
            {
                tw.WriteLine(this.TokenType.ToString());
            }

            if (this.Subexpressions.Count > 0)
            {
                tw.Indent++;
                if (depth >= 10)
                {
                    tw.WriteLine("...");
                }
                else
                {
                    for (int i = 0; i < this.Subexpressions.Count; i++)
                    {
                        var subexpression = this.Subexpressions[i];
                        if (subexpression == null)
                        {
                            tw.WriteLine("null");
                        }
                        else
                        {
                            subexpression.WriteIndentedString(tw);
                        }
                    }
                }
                tw.Indent--;
            }
        }

        public TokenExpression Clone()
        {
            var clone = new TokenExpression(Token.Clone());
            clone.Subexpressions = this.Subexpressions.ToList();
            clone.TokenType = this.TokenType;
            return clone;
        }

        public TokenExpression GetFirstSubexpression(TokenType tokenType)
        {
            return TokensWhere(this.Subexpressions, tokenType).FirstOrDefault();
        }

        public IEnumerable<TokenExpression> GetSubexpressionsRecursive(TokenType tokenType)
        {
            return TokensWhereRecursive(this, tokenType);
        }

        static IEnumerable<TokenExpression> TokensWhere(IEnumerable<TokenExpression> tokens, TokenType tokenType)
        {
            return tokens.Where(t => t.TokenType == tokenType);
        }

        static IEnumerable<TokenExpression> TokensWhereRecursive(TokenExpression token, TokenType tokenType)
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
}
