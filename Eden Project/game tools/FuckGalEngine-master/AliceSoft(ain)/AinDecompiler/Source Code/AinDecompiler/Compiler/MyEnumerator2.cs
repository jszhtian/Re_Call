using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Text;

namespace AinDecompiler.Compiler
{
    class MyEnumerator2<T> where T : class
    {
        IList<T> array;
        int index = 0;
        int lastCommittedIndex = 0;
        public MyEnumerator2(IList<T> array)
        {
            this.array = array;
        }
        public void Accept()
        {
            lastCommittedIndex = index;
        }
        public void Rollback()
        {
            index = lastCommittedIndex;
        }
        public int Index
        {
            get
            {
                return this.index;
            }
            set
            {
                this.index = value;
            }
        }
        public T Peek()
        {
            if (index >= 0 && index < array.Count)
            {
                return array[index];
            }
            else
            {
                return null;
            }
        }
        public T Read()
        {
            var returnValue = Peek();
            index++;
            return returnValue;
        }
    }
}
