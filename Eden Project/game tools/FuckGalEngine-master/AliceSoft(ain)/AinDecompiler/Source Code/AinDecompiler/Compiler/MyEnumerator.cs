using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Text;

namespace AinDecompiler.Compiler
{
    class MyEnumerator<T> where T : class
    {
        IEnumerator<T> enumerator;
        T current;
        public MyEnumerator(IEnumerator<T> enumerator)
        {
            this.enumerator = enumerator;
        }
        public T Peek()
        {
            if (this.current == null)
            {
                if (!enumerator.MoveNext())
                {
                    return null;
                }
                this.current = enumerator.Current;
            }
            return this.current;
        }
        public T Read()
        {
            Peek();
            var value = this.current;
            this.current = null;
            return value;
        }
    }
}
