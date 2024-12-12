using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Text;

namespace TranslateParserThingy
{
    public struct ExtentListNode : IComparable<ExtentListNode>, IEquatable<ExtentListNode>
    {
        public int Start, Length;
        public bool Member;
        public ExtentListNode(int start, int length, bool member)
        {
            this.Start = start;
            this.Length = length;
            this.Member = member;
        }
        public int CompareTo(ExtentListNode other)
        {
            int compare = this.Start.CompareTo(other.Start);
            if (compare != 0) return compare;
            compare = this.Length.CompareTo(other.Length);
            if (compare != 0) return compare;
            return this.Member.CompareTo(other.Member);
        }
        public bool Equals(ExtentListNode other)
        {
            return this.CompareTo(other) == 0;
        }
        public override string ToString()
        {
            return "Start = " + Start.ToString() + " Length = " + Length.ToString() + " Member = " + Member.ToString();
        }
    }

    public class ExtentList
    {
        public ExtentList(int length)
        {
            Add(0, length, false);
        }

        MySortedList<int, ExtentListNode> list = new MySortedList<int, ExtentListNode>();

        public MySortedList<int, ExtentListNode>.ValueCollection List
        {
            get
            {
                return list.Values;
            }
        }

        public void SetRange(int start, int length)
        {
            SetRange(start, length, true);
        }

        public void SetRange(int start, int length, bool member)
        {
            SplitNode(start);
            SplitNode(start + length);
            int leftIndex = list.GetFetchIndex(start);
            int rightIndex = list.GetInsertIndex(start + length);
            list.RemoveRangeAt(leftIndex, rightIndex - leftIndex);
            //list.RemoveRangeAt(leftIndex, rightIndex - leftIndex);
            Add(start, length, member);
            JoinNodes(start);
        }

        private void JoinNodes(int start)
        {
            int index = list.GetFetchIndex(start);
            if (index < 0 || index >= list.Count)
            {
                return;
            }
            JoinNodesAtIndex(index);
        }

        private void JoinNodesAtIndex(int index)
        {
            ExtentListNode node = list.ValueAt(index);
            ExtentListNode? prevNode = null;
            ExtentListNode? nextNode = null;
            if (index - 1 >= 0)
            {
                prevNode = list.ValueAt(index - 1);
            }
            if (index + 1 < list.Count)
            {
                nextNode = list.ValueAt(index + 1);
            }

            //if any node is length 0, remove it
            if (prevNode != null && prevNode.Value.Length == 0)
            {
                Remove(prevNode.Value);
                return;
            }
            if (nextNode != null && nextNode.Value.Length == 0)
            {
                Remove(nextNode.Value);
                return;
            }

            //see if we can join all three nodes together
            if (prevNode != null && nextNode != null &&
                node.Member == prevNode.Value.Member && node.Member == nextNode.Value.Member)
            {
                int totalSize = prevNode.Value.Length + node.Length + nextNode.Value.Length;
                //if any node is a file-backed node
                list.RemoveRangeAt(index - 1, 3);
                Add(prevNode.Value.Start, totalSize, node.Member);
                return;
            }

            //see if we can join previous node to middle node
            if (prevNode != null && node.Member == prevNode.Value.Member)
            {
                int totalSize = prevNode.Value.Length + node.Length;
                list.RemoveRangeAt(index - 1, 2);
                Add(prevNode.Value.Start, totalSize, prevNode.Value.Member);
                return;
            }

            //see if we can join next node to middle node
            if (nextNode != null && node.Member == nextNode.Value.Member)
            {
                int totalSize = node.Length + nextNode.Value.Length;
                list.RemoveRangeAt(index, 2);
                Add(node.Start, totalSize, node.Member);
                return;
            }
        }

        private void SplitNode(int start)
        {
            int index = list.GetFetchIndex(start);
            if (index < 0 || index >= list.Count)
            {
                return;
            }
            var node = list.ValueAt(index);
            if (node.Start == start)
            {
                return;
            }

            int offset = start - node.Start;
            int newLength = node.Length - offset;
            bool member = node.Member;

            Remove(node);
            Add(node.Start, offset, member);
            Add(start, newLength, member);
        }

        private void Add(int start, int length, bool member)
        {
            list.Add(start, new ExtentListNode(start, length, member));
        }

        private void Remove(ExtentListNode node)
        {
            list.Remove(node.Start);
        }
    }
}
