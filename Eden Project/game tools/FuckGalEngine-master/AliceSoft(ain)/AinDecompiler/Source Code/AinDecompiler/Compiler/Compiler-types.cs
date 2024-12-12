using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Text;

namespace AinDecompiler.Compiler
{
    public partial class Compiler
    {
        class StringListAndDictionary
        {
            public StringListAndDictionary()
            {
                this.List = new List<string>();
            }
            public StringListAndDictionary(IList<string> list)
            {
                this.List = list;
            }

            public IList<string> List;
            public Dictionary<string, int> NameToIndex = new Dictionary<string, int>();
            public int Add(string name)
            {
                if (name == null) name = "";
                if (NameToIndex.ContainsKey(name))
                {
                    return NameToIndex[name];
                }
                else
                {
                    int index = List.Count;
                    NameToIndex.Add(name, index);
                    List.Add(name);
                    return index;
                }
            }
        }

        class ListAndDictionary<T> where T : IVariable, new()
        {
            AinFile root;
            public ListAndDictionary(IDictionary<string, IVariable> parent, AinFile root)
            {
                this.root = root;
                this.parent = parent;
                this.List = new List<T>();
                this.NameToIndex = new Dictionary<string, int>();
            }
            public ListAndDictionary(IDictionary<string, IVariable> parent, AinFile root, IList<T> list)
            {
                this.root = root;
                this.parent = parent;
                this.List = list;
                this.NameToIndex = new Dictionary<string, int>();
            }
            public ListAndDictionary(IDictionary<string, IVariable> parent, AinFile root, IList<T> list, IDictionary<string, int> nameToIndex)
            {
                this.root = root;
                this.parent = parent;
                this.List = list;
                this.NameToIndex = nameToIndex;
            }
            IDictionary<string, IVariable> parent;
            public IList<T> List;
            public IDictionary<string, int> NameToIndex;
            public T Get(string name)
            {
                if (NameToIndex.ContainsKey(name))
                {
                    return List[NameToIndex[name]];
                }
                else
                {
                    var newItem = new T();
                    newItem.Name = name;
                    int index = List.Count;

                    newItem.Index = index;
                    newItem.Root = root;
                    List.Add(newItem);
                    this.NameToIndex.Add(name, index);

                    if (!parent.ContainsKey(name))
                    {
                        parent.Add(name, newItem);
                    }
                    return newItem;
                }
            }
            public bool Contains(string name)
            {
                return NameToIndex.ContainsKey(name);
            }
        }

    }
}
