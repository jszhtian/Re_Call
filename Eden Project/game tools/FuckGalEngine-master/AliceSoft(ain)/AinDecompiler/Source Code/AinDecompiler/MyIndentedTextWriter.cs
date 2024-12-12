using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Text;
using System.Globalization;

namespace AinDecompiler
{
    public class MyIndentedTextWriter : TextWriter
    {
        class TextWriterWrapper : TextWriter
        {
            public TextWriterWrapper(TextWriter innerWriter)
            {
                this.innerWriter = innerWriter;
            }

            TextWriter innerWriter;

            public TextWriter InnerWriter
            {
                get
                {
                    return innerWriter;
                }
            }

            public int Column
            {
                get
                {
                    return column;
                }
            }

            public int LineNumber
            {
                get
                {
                    return line;
                }
            }
            int column, line;

            public override void Write(char value)
            {
                if (value == '\n')
                {
                    line++;
                }
                else if (value == '\t')
                {
                    column += 4;
                }
                else
                {
                    column++;
                }
                innerWriter.Write(value);
            }

            public override Encoding Encoding
            {
                get
                {
                    return innerWriter.Encoding;
                }
            }

            public override void Flush()
            {
                innerWriter.Flush();
            }

            public override void Close()
            {
                innerWriter.Close();
            }

            protected override void Dispose(bool disposing)
            {
                innerWriter.Dispose();
            }

            public override IFormatProvider FormatProvider
            {
                get
                {
                    return innerWriter.FormatProvider;
                }
            }

            public override string NewLine
            {
                get
                {
                    return innerWriter.NewLine;
                }
                set
                {
                    innerWriter.NewLine = value;
                }
            }
        }

        public const string DefaultTabString = "\t";
        int spacesPerTab = 4;
        private TextWriterWrapper writer;
        private int indentLevel;
        private bool tabsPending;
        public bool TabsPending
        {
            get
            {
                return tabsPending;
            }
        }

        public int LineNumber
        {
            get
            {
                return writer.LineNumber;
            }
        }

        public int Column
        {
            get
            {
                return writer.Column;
            }
        }

        private string tabString;
        public override Encoding Encoding
        {
            get
            {
                return this.writer.Encoding;
            }
        }
        public override string NewLine
        {
            get
            {
                return this.writer.NewLine;
            }
            set
            {
                this.writer.NewLine = value;
            }
        }
        public int Indent
        {
            get
            {
                return this.indentLevel;
            }
            set
            {
                if (value < 0)
                {
                    value = 0;
                }
                this.indentLevel = value;
            }
        }
        public TextWriter InnerWriter
        {
            get
            {
                return this.writer;
            }
        }
        internal string TabString
        {
            get
            {
                return this.tabString;
            }
        }
        public MyIndentedTextWriter(TextWriter writer)
            : this(writer, DefaultTabString)
        {
        }
        public MyIndentedTextWriter(TextWriter writer, string tabString)
            : base(CultureInfo.InvariantCulture)
        {
            this.writer = new TextWriterWrapper(writer);
            this.tabString = tabString;
            this.indentLevel = 0;
            this.tabsPending = false;
        }
        public override void Close()
        {
            this.writer.Close();
        }
        public override void Flush()
        {
            this.writer.Flush();
        }
        protected virtual void OutputTabs()
        {
            if (this.tabsPending)
            {
                for (int i = 0; i < this.indentLevel; i++)
                {
                    this.writer.Write(this.tabString);
                }
                this.tabsPending = false;
            }
        }
        public override void Write(string s)
        {
            this.OutputTabs();
            this.writer.Write(s);
        }
        public override void Write(bool value)
        {
            this.OutputTabs();
            this.writer.Write(value);
        }
        public override void Write(char value)
        {
            this.OutputTabs();
            this.writer.Write(value);
        }
        public override void Write(char[] buffer)
        {
            this.OutputTabs();
            this.writer.Write(buffer);
        }
        public override void Write(char[] buffer, int index, int count)
        {
            this.OutputTabs();
            this.writer.Write(buffer, index, count);
        }
        public override void Write(double value)
        {
            this.OutputTabs();
            this.writer.Write(value);
        }
        public override void Write(float value)
        {
            this.OutputTabs();
            this.writer.Write(value);
        }
        public override void Write(int value)
        {
            this.OutputTabs();
            this.writer.Write(value);
        }
        public override void Write(long value)
        {
            this.OutputTabs();
            this.writer.Write(value);
        }
        public override void Write(object value)
        {
            this.OutputTabs();
            this.writer.Write(value);
        }
        public override void Write(string format, object arg0)
        {
            this.OutputTabs();
            this.writer.Write(format, arg0);
        }
        public override void Write(string format, object arg0, object arg1)
        {
            this.OutputTabs();
            this.writer.Write(format, arg0, arg1);
        }
        public override void Write(string format, params object[] arg)
        {
            this.OutputTabs();
            this.writer.Write(format, arg);
        }
        public void WriteLineNoTabs(string s)
        {
            this.writer.WriteLine(s);
        }
        public override void WriteLine(string s)
        {
            this.OutputTabs();
            this.writer.WriteLine(s);
            this.tabsPending = true;
        }
        public override void WriteLine()
        {
            this.OutputTabs();
            this.writer.WriteLine();
            this.tabsPending = true;
        }
        public override void WriteLine(bool value)
        {
            this.OutputTabs();
            this.writer.WriteLine(value);
            this.tabsPending = true;
        }
        public override void WriteLine(char value)
        {
            this.OutputTabs();
            this.writer.WriteLine(value);
            this.tabsPending = true;
        }
        public override void WriteLine(char[] buffer)
        {
            this.OutputTabs();
            this.writer.WriteLine(buffer);
            this.tabsPending = true;
        }
        public override void WriteLine(char[] buffer, int index, int count)
        {
            this.OutputTabs();
            this.writer.WriteLine(buffer, index, count);
            this.tabsPending = true;
        }
        public override void WriteLine(double value)
        {
            this.OutputTabs();
            this.writer.WriteLine(value);
            this.tabsPending = true;
        }
        public override void WriteLine(float value)
        {
            this.OutputTabs();
            this.writer.WriteLine(value);
            this.tabsPending = true;
        }
        public override void WriteLine(int value)
        {
            this.OutputTabs();
            this.writer.WriteLine(value);
            this.tabsPending = true;
        }
        public override void WriteLine(long value)
        {
            this.OutputTabs();
            this.writer.WriteLine(value);
            this.tabsPending = true;
        }
        public override void WriteLine(object value)
        {
            this.OutputTabs();
            this.writer.WriteLine(value);
            this.tabsPending = true;
        }
        public override void WriteLine(string format, object arg0)
        {
            this.OutputTabs();
            this.writer.WriteLine(format, arg0);
            this.tabsPending = true;
        }
        public override void WriteLine(string format, object arg0, object arg1)
        {
            this.OutputTabs();
            this.writer.WriteLine(format, arg0, arg1);
            this.tabsPending = true;
        }
        public override void WriteLine(string format, params object[] arg)
        {
            this.OutputTabs();
            this.writer.WriteLine(format, arg);
            this.tabsPending = true;
        }
        [CLSCompliant(false)]
        public override void WriteLine(uint value)
        {
            this.OutputTabs();
            this.writer.WriteLine(value);
            this.tabsPending = true;
        }
        internal void InternalOutputTabs()
        {
            for (int i = 0; i < this.indentLevel; i++)
            {
                this.writer.Write(this.tabString);
            }
        }
    }
}
