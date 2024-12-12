using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Text;
using System.Globalization;

namespace AinDecompiler
{
    public class MyTextReader2
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

        public MyTextReader2()
        {

        }

        public MyTextReader2(TextReader tr, string fileName)
        {
            IncludeTextReader(tr, fileName);
        }

        bool _allowMultipleIncludes = false;
        public bool AllowMultipleIncludes
        {
            get
            {
                return _allowMultipleIncludes;
            }
            set
            {
                _allowMultipleIncludes = value;
            }
        }

        HashSet<string> IncludedPaths = new HashSet<string>();

        public void IncludeTextReader(string fileName)
        {
            fileName = CanonicalizeFileName(fileName);
            if (!CheckFileName(fileName)) return;
            Encoding encoding = EncodingDetector.DetectEncoding(fileName);
            StreamReader sr = new StreamReader(fileName, encoding);
            IncludeTextReader(sr, fileName);
        }

        private bool CheckFileName(string fileName)
        {
            bool proceed = true;
            if (!File.Exists(fileName))
            {
                throw new FileNotFoundException("The file to #include, \"" + fileName + "\" was not found.", fileName);
            }
            if (!AllowMultipleIncludes && IncludedPaths.Contains(fileName.ToUpperInvariant()))
            {
                proceed = false;
            }
            return proceed;
        }

        private string CanonicalizeFileName(string fileName)
        {
            if (Path.IsPathRooted(fileName))
            {

            }
            else
            {
                if (!String.IsNullOrEmpty(this.FileName))
                {
                    fileName = Path.GetFullPath(Path.Combine(Path.GetDirectoryName(this.FileName), fileName));
                }
                else
                {
                    fileName = Path.GetFullPath(fileName);
                }
            }
            return fileName;
        }

        public void IncludeTextReader(TextReader tr, string fileName)
        {
            fileName = CanonicalizeFileName(fileName);
            if (!CheckFileName(fileName)) return;
            IncludedPaths.Add(fileName.ToUpperInvariant());

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
            if (myCharacter != -1)
            {
                putbackCharacters.Push(myCharacter);
                myCharacter = -1;
            }
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

        public void EatWhiteSpace()
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
                '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '-', '+', '=',
                '[', ']', '|', '{', '}', ':', ';', '<', '>', '?', ',', '.', '/', '\'', '"',
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
                this.Read();
                int charInt2 = this.Peek();
                this.Putback(charInt);
                if (charInt2 == '/')
                {
                    this.Read();
                    this.Read();
                    SkipToLineFeed();
                    goto restart;
                }
                else if (charInt2 == '*')
                {
                    this.Read();
                    this.Read();
                    SkipToEndComment();
                    goto restart;
                }
                //else
                //{
                //    this.Putback(charInt);
                //}
            }
            else if (c == '#')
            {
                this.Read();
                int charInt2 = this.Peek();
                if (charInt2 == '#')
                {
                    SkipToLineFeed();
                    goto restart;
                }
                else
                {
                    this.Putback(charInt);
                }
            }

            if (c == '\'' || c == '"')
            {
                return GetEscapedString();
            }
            else if (IsTokenCharacter(charInt))
            {
                return GetOperator();
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

        private Token GetOperator()
        {
            //check for multi-character tokens:
            //!= >= <= ==
            //&& ||
            //++ --
            //<< >>
            //::
            //+= -= /= *= %= &= |= ^= <<= >>=

            int firstChar = this.Read();
            int secondChar = this.Peek();
            string twoCharacters = ((char)firstChar).ToString() + ((char)secondChar).ToString();
            switch (twoCharacters)
            {
                case "%=":
                case "&=":
                case "*=":
                case "+=":
                case "-=":
                case "/=":
                case ">=":
                case "^=":
                case "|=":
                case "<-":
                case "::":
                case "++":
                case "--":
                case "$$":
                case "&&":
                case "||":
                    this.Read();
                    return new Token(twoCharacters);
                case "!=":
                case "==":
                case "<<":
                case ">>":
                    {
                        this.Read();
                        int thirdChar = this.Peek();
                        if (thirdChar == '=')
                        {
                            this.Read();
                            return new Token(twoCharacters + (char)thirdChar);
                        }
                        else
                        {
                            return new Token(twoCharacters);
                        }
                    }
                case "<=":
                    {
                        this.Read();
                        int thirdChar = this.Peek();
                        if (thirdChar == '>')
                        {
                            this.Read();
                            return new Token(twoCharacters + (char)thirdChar);
                        }
                        else
                        {
                            return new Token(twoCharacters);
                        }
                    }
            }
            return new Token((char)firstChar);
        }

        private Token GetNumber()
        {
            StringBuilder sb = new StringBuilder(16);
            int seenE = 0;
            while (true)
            {
                int charInt = this.Peek();
                if (charInt == -1)
                {
                    break;
                }
                if (seenE == 0)
                {
                    if (charInt == 'E')
                    {
                        seenE = 1;
                    }
                    else if (charInt != '.' && (IsTokenCharacter(charInt) || char.IsWhiteSpace((char)charInt)))
                    {
                        break;
                    }
                }
                else if (seenE == 1)
                {
                    if (charInt != '+' && charInt != '-' && (IsTokenCharacter(charInt) || char.IsWhiteSpace((char)charInt)))
                    {
                        break;
                    }
                    seenE = 2;
                }
                else
                {
                    if (IsTokenCharacter(charInt) || char.IsWhiteSpace((char)charInt))
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

                    switch (charInt)
                    {
                        case 'r':
                            c = '\r';
                            break;
                        case 'n':
                            c = '\n';
                            break;
                        case 't':
                            c = '\t';
                            break;
                        case 'a':
                            c = '\a';
                            break;
                        case 'b':
                            c = '\b';
                            break;
                        case 'f':
                            c = '\f';
                            break;
                        case 'x':
                            {
                                int c1 = this.Peek();
                                if (IsHexChar(c1))
                                {
                                    this.Read();
                                }
                                else
                                {
                                    break;
                                }
                                int c2 = this.Peek();
                                if (IsHexChar(c2))
                                {
                                    this.Read();
                                }
                                else
                                {
                                    this.Putback(c1);
                                    break;
                                }
                                c = (char)int.Parse(((char)c1).ToString() + (char)c2, NumberStyles.AllowHexSpecifier);
                            }
                            break;
                        case '\r':
                            charInt = this.Peek();
                            if (charInt == '\n')
                            {
                                this.Read();
                                continue;
                            }
                            continue;
                        case '\n':
                            continue;
                    }
                }
                else
                {
                    if (c == firstChar || c == '\r' || c == '\n')
                    {
                        break;
                    }
                }
                sb.Append(c);
            }
            return new Token(sb.ToString(), firstChar);
        }

        private static bool IsHexChar(int nextChar)
        {
            return ((nextChar >= '0' && nextChar <= '9') ||
                (nextChar >= 'A' && nextChar <= 'F') ||
                (nextChar >= 'a' && nextChar <= 'f'));
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
