using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Text;

namespace AinDecompiler
{
    public class ExpressionMap : StartEndList<Expression>
    {
        public ExpressionDisplayer displayer;

        public IEnumerable<ExpressionMap.ExpressionListNode> FindVariable(IVariable variable)
        {
            foreach (var node in nodes.OrderBy(n => n.start))
            {
                if (node.item.Variable == variable)
                {
                    yield return node;
                }
            }
        }

        public int[] GetAddressPerLineNumber()
        {
            List<List<ExpressionMap.ExpressionListNode>> nodesPerLine = new List<List<StartEndList<Expression>.ExpressionListNode>>();

            foreach (var node in this.nodes)
            {
                int lineNumber = node.lineNumber;
                var list = nodesPerLine.GetOrNew(lineNumber);
                list.Add(node);
                nodesPerLine.SetOrAdd(lineNumber, list);
            }

            List<int> lowestAddressPerLine = new List<int>();

            for (int l = 0; l < nodesPerLine.Count; l++)
            {
                var nodesOnLine = nodesPerLine[l];
                if (nodesOnLine != null)
                {
                    int lowestAddress = nodesOnLine.Min(n => n.item.LowestAddress);
                    if (lowestAddress == int.MaxValue || lowestAddress < 0)
                    {
                        lowestAddress = 0;
                    }
                    lowestAddressPerLine.SetOrAdd(l, lowestAddress);
                }
                else
                {
                    lowestAddressPerLine.SetOrAdd(l, 0);
                }

            }
            return lowestAddressPerLine.ToArray();
        }

        public IEnumerable<ExpressionMap.ExpressionListNode> FindStructType(int structType)
        {
            foreach (var node in nodes.OrderBy(n => n.start))
            {
                if (node.item.Variable.StructType == structType)
                {
                    yield return node;
                }
            }
        }
    }

    public class StartEndList<T>
    {
        protected List<ExpressionListNode> nodes = new List<ExpressionListNode>();

        public class ExpressionListNode : IComparable<ExpressionListNode>
        {
            public T item;
            public int start, end, index, lineNumber, column;
            //public string text;
            public ExpressionListNode(T item, int start, int end, int index, int lineNumber, int column)
            {
                if (start < 0) start = 0;
                if (end < 0) end = 0;
                if (end < start) end = start;

                this.item = item;
                this.start = start;
                this.end = end;
                this.index = index;
                //this.text = text;
                this.lineNumber = lineNumber;
                this.column = column;
            }

            public int Size
            {
                get
                {
                    return end - start;
                }
            }

            public int CompareTo(ExpressionListNode other)
            {
                int difference = this.Size - other.Size;
                if (difference == 0)
                {
                    difference = this.start - other.start;
                }
                if (difference == 0)
                {
                    difference = this.index - other.index;
                }
                return difference;
            }

            public override string ToString()
            {
                return "start=" + start + " end=" + end;
            }

            public string GetText(byte[] utf8Characters)
            {
                return UTF8Encoding.UTF8.GetString(utf8Characters, start, end - start);
            }

            public string GetText(string textBoxText)
            {
                return GetText(UTF8Encoding.UTF8.GetBytes(textBoxText));
            }
        }

        public void Add(T item, int start, int end, int lineNumber, int column)
        {
            int index = nodes.Count;
            nodes.Add(new ExpressionListNode(item, start, end, index, lineNumber, column));
        }

        private IEnumerable<T> GetSequenceAt(int location)
        {
            var seq1 = nodes.Where(n => location >= n.start);
            var seq2 = seq1.Where(n => location < n.end);
            var sorted = seq2.OrderBy(n => n);
            var matches = sorted.Select(n => n.item);
            return matches;
        }

        public T GetItemAt(int location)
        {
            var matches = GetSequenceAt(location);
            var item = matches.FirstOrDefault();
            return item;
        }

        public T[] GetItemsAt(int location)
        {
            var matches = GetSequenceAt(location);
            return matches.ToArray();
        }


    }
}
