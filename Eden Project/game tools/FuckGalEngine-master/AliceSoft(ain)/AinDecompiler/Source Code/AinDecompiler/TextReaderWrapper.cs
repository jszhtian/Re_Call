using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Text;

namespace AinDecompiler
{
    class TextReaderWrapper : TextReader
    {
        Stack<TextReader> otherTextReaders = new Stack<TextReader>();
        TextReader currentTextReader = null;
        public string FileName
        {
            get
            {
                var sr = currentTextReader as StreamReader;
                var trw = currentTextReader as TextReaderWrapper;
                if (trw != null)
                {
                    sr = trw.currentTextReader as StreamReader;
                }
                if (sr != null)
                {
                    var fs = sr.BaseStream as FileStream;
                    if (fs != null)
                    {
                        return fs.Name;
                    }
                }
                return "";
            }
        }
        
        public string DirectoryName
        {
            get
            {
                return Path.GetDirectoryName(this.FileName);
            }
        }

        public void IncludeTextReader(TextReader textReaderToInclude)
        {
            if (currentTextReader == null)
            {
                currentTextReader = textReaderToInclude;
            }
            else
            {
                otherTextReaders.Push(currentTextReader);
                currentTextReader = textReaderToInclude;
            }
        }

        public TextReaderWrapper(TextReader textReaderToInclude)
        {
            IncludeTextReader(textReaderToInclude);
        }

        public override void Close()
        {
            currentTextReader.Close();
            while (otherTextReaders.Count > 0)
            {
                NextTextReader();
                currentTextReader.Close();
            }
        }

        public override int Peek()
        {
            int result = currentTextReader.Peek();
            if (result == -1)
            {
                if (otherTextReaders.Count > 0)
                {
                    NextTextReader();
                    return Peek();
                }
            }
            return result;
        }

        public override int Read()
        {
            int result = currentTextReader.Read();
            if (result == -1)
            {
                if (otherTextReaders.Count > 0)
                {
                    NextTextReader();
                    return Read();
                }
            }
            return result;
        }

        protected override void Dispose(bool disposing)
        {
            currentTextReader.Dispose();
            otherTextReaders.ForEach(r => r.Dispose());
        }

        public override int Read(char[] buffer, int index, int count)
        {
            int result = currentTextReader.Read(buffer, index, count);
            if (result < count)
            {
                if (otherTextReaders.Count > 0)
                {
                    NextTextReader();
                    if (result == 0)
                    {
                        return Read(buffer, index, count);
                    }
                }
                return result;
            }
            return result;
        }

        private void NextTextReader()
        {
            if (otherTextReaders.Count > 0)
            {
                currentTextReader.Dispose();
                currentTextReader = otherTextReaders.Pop();
            }
        }

        public override int ReadBlock(char[] buffer, int index, int count)
        {
            int result = currentTextReader.ReadBlock(buffer, index, count);
            if (result < count)
            {
                if (otherTextReaders.Count > 0)
                {
                    NextTextReader();
                    if (result == 0)
                    {
                        return Read(buffer, index, count);
                    }
                }
                return result;
            }
            return result;
        }

        public override string ReadLine()
        {
            string result = currentTextReader.ReadLine();
            if (result == null)
            {
                if (otherTextReaders.Count > 0)
                {
                    NextTextReader();
                    return ReadLine();
                }
            }
            return result;
        }

        public override string ReadToEnd()
        {
            string result = currentTextReader.ReadToEnd();
            if (result == null)
            {
                if (otherTextReaders.Count > 0)
                {
                    NextTextReader();
                    return ReadToEnd();
                }
            }
            return result;
        }
    }
}
