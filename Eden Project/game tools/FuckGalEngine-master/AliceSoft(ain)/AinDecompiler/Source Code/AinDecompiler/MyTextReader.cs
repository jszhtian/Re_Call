using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Text;
using System.Globalization;

namespace AinDecompiler
{
    public class MyTextReader
    {
        TextReader tr;
        string fileName;

        public bool Logging
        {
            get;
            set;
        }

        List<Token> log = new List<Token>();
        public void ReplayLog()
        {
            for (int i = log.Count - 1; i >= 0; i--)
            {
                PutbackToken(log[i]);
            }
            log.Clear();
        }

        public string FileName
        {
            get
            {
                return fileName;
            }
        }

        public int Row
        {
            get
            {
                return row;
            }
        }

        public int Column
        {
            get
            {
                return column;
            }
        }

        class TextReaderState
        {
            public int column, row;
            public TextReader textReader;
            public string fileName;

            public TextReaderState(TextReader reader, string fileName, int column, int row)
            {
                this.column = column;
                this.row = row;
                this.textReader = reader;
                this.fileName = fileName;
            }
        }
        int column, row;

        Stack<TextReaderState> otherTextReaders = new Stack<TextReaderState>();
        //Stack<string> otherFilenames = new Stack<string>();
        Stack<int> putbackCharacters = new Stack<int>();
        Stack<Token> putbackTokens = new Stack<Token>();

        public MyTextReader()
        {

        }

        public MyTextReader(TextReader tr, string fileName)
        {
            IncludeTextReader(tr, fileName);
        }

        public void IncludeTextReader(TextReader tr, string fileName)
        {
            if (this.tr == null)
            {
                this.tr = tr;
                this.fileName = fileName;
                this.column = 0;
                this.row = 0;
            }
            else
            {
                otherTextReaders.Push(new TextReaderState(this.tr, this.fileName, this.column, this.row));
                //otherFilenames.Push(this.fileName);
                this.tr = tr;
                this.fileName = fileName;
                this.column = 0;
                this.row = 0;
            }
        }


        Token nextToken = null;

        int myCharacter = -1;

        protected void Putback(int character)
        {
            putbackCharacters.Push(character);
        }

        protected int Peek()
        {
            if (myCharacter != -1)
            {
                return myCharacter;
            }

            if (putbackCharacters.Count > 0)
            {
                myCharacter = putbackCharacters.Pop();
                return myCharacter;
            }

            while (this.tr == null && otherTextReaders.Count > 0)
            {
                var state = otherTextReaders.Pop();
                this.tr = state.textReader;
                this.column = state.column;
                this.row = state.row;
                this.fileName = state.fileName;
            }

            if (this.tr == null)
            {
                myCharacter = -1;
                return myCharacter;
            }

            int result = this.tr.Read();
            if (result == -1)
            {
                this.tr.Close();
                this.tr = null;
                this.fileName = null;
                return Peek();
            }
            else if (result == '\n')
            {
                row++;
                column = 0;
            }
            else
            {
                column++;
            }

            myCharacter = result;
            return result;
        }

        protected int Read()
        {
            int result = Peek();
            myCharacter = -1;
            return result;
        }

        protected void EatWhiteSpace()
        {
            while (true)
            {
                int c = this.Peek();
                if (c == -1)
                {
                    break;
                }
                else if (char.IsWhiteSpace((char)c))
                {
                    this.Read();
                }
                else
                {
                    break;
                }
            }
        }

        public static bool IsTokenCharacter(int character)
        {
            return tokenCharacters.Contains(character);
        }

        protected static HashSet<int> tokenCharacters = new HashSet<int>(new int[]
            {
                ',', '.', ',', ';', '{', '}', '(', ')', '\'', '"', '=', '/', ':',
            });

        protected Token GetNextToken()
        {
        restart:
            this.EatWhiteSpace();
            int charInt = this.Peek();
            if (charInt == -1)
            {
                return null;
            }
            char c = (char)charInt;

            if (c == '/')
            {
                charInt = this.Read();
                int charInt2 = this.Peek();
                if (charInt2 == '/')
                {
                    SkipToLineFeed();
                    goto restart;
                }
                else if (charInt2 == '*')
                {
                    SkipToEndComment();
                    goto restart;
                }
                else
                {
                    //this.Putback(charInt2);
                    this.Putback(charInt);
                }
            }

            if (c == '\'' || c == '"')
            {
                return GetEscapedString();
            }
            else if (IsTokenCharacter(charInt))
            {
                return new Token(((char)this.Read()).ToString());
            }
            else if ((c >= '0' && c <= '9') || (c == '-'))
            {
                return GetNumber();
            }
            else
            {
                return GetWord();
            }
        }

        private Token GetNumber()
        {
            StringBuilder sb = new StringBuilder(16);
            bool seenE = false;
            while (true)
            {
                int charInt = this.Peek();
                if (charInt == -1)
                {
                    break;
                }
                if (!seenE)
                {
                    if (charInt == 'E')
                    {
                        seenE = true;
                    }
                    else if (charInt != '.' && (IsTokenCharacter(charInt) || char.IsWhiteSpace((char)charInt)))
                    {
                        break;
                    }
                }
                else
                {
                    if (charInt != '+' && charInt != '-' && (IsTokenCharacter(charInt) || char.IsWhiteSpace((char)charInt)))
                    {
                        break;
                    }
                }
                sb.Append((char)charInt);
                this.Read();
            }
            return new Token(sb.ToString());
        }

        private void SkipToEndComment()
        {
            while (true)
            {
                int charInt = this.Read();
                if (charInt == -1)
                {
                    break;
                }
                if (charInt == '*')
                {
                    charInt = this.Read();
                    if (charInt == -1)
                    {
                        break;
                    }
                    if (charInt == '/')
                    {
                        break;
                    }
                }
            }
        }

        private void SkipToLineFeed()
        {
            while (true)
            {
                int charInt = this.Read();
                if (charInt == -1)
                {
                    break;
                }
                if (charInt == '\r' || charInt == '\n')
                {
                    break;
                }
            }
        }

        private Token GetWord()
        {
            StringBuilder sb = new StringBuilder(16);
            while (true)
            {
                int charInt = this.Peek();
                if (charInt == -1)
                {
                    break;
                }
                if (IsTokenCharacter(charInt) || char.IsWhiteSpace((char)charInt))
                {
                    break;
                }
                sb.Append((char)charInt);
                this.Read();
            }
            return new Token(sb.ToString());
        }

        private Token GetEscapedString()
        {
            StringBuilder sb = new StringBuilder(16);
            int charInt = this.Read();
            if (charInt == -1)
            {
                return null;
            }
            char firstChar = (char)charInt;

            while (true)
            {
                charInt = this.Read();
                if (charInt == -1)
                {
                    break;
                }
                char c = (char)charInt;

                if (c == '\\')
                {
                    charInt = this.Read();
                    if (charInt == -1)
                    {
                        break;
                    }
                    c = (char)charInt;
                    if (c == 'r')
                    {
                        c = '\r';
                    }
                    else if (c == 'n')
                    {
                        c = '\n';
                    }
                    else if (c == 't')
                    {
                        c = '\t';
                    }
                    else if (c == '\r')
                    {
                        charInt = this.Peek();
                        if (charInt == '\n')
                        {
                            this.Read();
                            continue;
                        }
                        continue;
                    }
                    else if (c == '\n')
                    {
                        continue;
                    }
                }
                else
                {
                    if (c == firstChar)
                    {
                        break;
                    }
                }
                sb.Append(c);
            }
            return new Token(sb.ToString(), firstChar);
        }

        public Token PeekToken()
        {
            if (nextToken == null)
            {
                if (putbackTokens.Count > 0)
                {
                    nextToken = putbackTokens.Pop();
                    return nextToken;
                }
                nextToken = GetNextToken();
            }
            return nextToken;
        }

        public Token ReadToken()
        {
            Token tokenToReturn = PeekToken();
            nextToken = null;

            if (Logging)
            {
                log.Add(tokenToReturn);
            }

            return tokenToReturn;
        }

        public void PutbackToken(Token token)
        {
            if (nextToken != null)
            {
                putbackTokens.Push(nextToken);
                nextToken = null;
            }

            putbackTokens.Push(token);
        }
    }
}
