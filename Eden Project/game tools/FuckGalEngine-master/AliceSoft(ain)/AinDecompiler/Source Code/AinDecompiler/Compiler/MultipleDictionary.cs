using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Text;

namespace AinDecompiler.Compiler
{
    public class MultipleDictionary<TKey, TValue> : IDictionary<TKey, TValue>
    {
        Stack<IDictionary<TKey, TValue>> dictionaries = new Stack<IDictionary<TKey, TValue>>();
        IDictionary<TKey, TValue> _dictionaryForNewItems;
        IDictionary<TKey, TValue> DictionaryForNewItems
        {
            get
            {
                if (_dictionaryForNewItems != null)
                {
                    return _dictionaryForNewItems;
                }
                else
                {
                    if (dictionaries.Count > 0)
                    {
                        return dictionaries.Peek();
                    }
                    else
                    {
                        return null;
                    }
                }
            }
        }

        public IDictionary<TKey, TValue> GetDictionaryContainingKey(TKey key)
        {
            foreach (var dic in dictionaries)
            {
                if (dic.ContainsKey(key))
                {
                    return dic;
                }
            }
            return null;
        }

        #region IDictionary<TKey,TValue> Members

        public void Add(TKey key, TValue value)
        {
            DictionaryForNewItems.Add(key, value);
        }

        public bool ContainsKey(TKey key)
        {
            var dictionaryWithKey = GetDictionaryContainingKey(key);
            if (dictionaryWithKey != null)
            {
                return true;
            }
            return false;
        }

        public ICollection<TKey> Keys
        {
            get
            {
                return dictionaries.SelectMany(d => d.Keys).ToArray();
            }
        }

        public bool Remove(TKey key)
        {
            bool removedAnything = false;
            while (true)
            {
                var dictionaryWithKey = GetDictionaryContainingKey(key);
                if (dictionaryWithKey == null)
                {
                    return removedAnything;
                }
                else
                {
                    dictionaryWithKey.Remove(key);
                    removedAnything = true;
                }
            }
        }

        public bool TryGetValue(TKey key, out TValue value)
        {
            if (ContainsKey(key))
            {
                value = this[key];
                return true;
            }
            else
            {
                value = default(TValue);
                return false;
            }
        }

        public ICollection<TValue> Values
        {
            get
            {
                return dictionaries.SelectMany(d => d.Values).ToArray();
            }
        }

        public TValue this[TKey key]
        {
            get
            {
                var dictionaryContainingKey = GetDictionaryContainingKey(key);
                if (dictionaryContainingKey == null)
                {
                    return default(TValue);
                }
                else
                {
                    return dictionaryContainingKey[key];
                }
            }
            set
            {
                DictionaryForNewItems[key] = value;
            }
        }

        #endregion

        #region ICollection<KeyValuePair<TKey,TValue>> Members

        void ICollection<KeyValuePair<TKey, TValue>>.Add(KeyValuePair<TKey, TValue> item)
        {
            this.Add(item.Key, item.Value);
        }

        public void Clear()
        {
            foreach (var dic in dictionaries)
            {
                dic.Clear();
            }
        }

        bool ICollection<KeyValuePair<TKey, TValue>>.Contains(KeyValuePair<TKey, TValue> item)
        {
            if (this.ContainsKey(item.Key))
            {
                var value = this[item.Key];
                if (object.Equals(value, item.Value))
                {
                    return true;
                }
            }
            return false;
        }

        void ICollection<KeyValuePair<TKey, TValue>>.CopyTo(KeyValuePair<TKey, TValue>[] array, int arrayIndex)
        {
            int i = 0;
            foreach (var pair in this)
            {
                array[i + arrayIndex] = pair;
                i++;
            }
        }

        public int Count
        {
            get
            {
                return this.dictionaries.Sum(d => d.Count);
            }
        }

        public bool IsReadOnly
        {
            get
            {
                return false;
            }
        }

        bool ICollection<KeyValuePair<TKey, TValue>>.Remove(KeyValuePair<TKey, TValue> item)
        {
            var key = item.Key;
            var dictionaryContainingKey = GetDictionaryContainingKey(key);
            if (dictionaryContainingKey != null)
            {
                var value = dictionaryContainingKey[key];
                if (Object.Equals(value, item.Value))
                {
                    dictionaryContainingKey.Remove(key);
                    return true;
                }
            }
            return false;
        }

        #endregion

        #region IEnumerable<KeyValuePair<TKey,TValue>> Members

        public IEnumerator<KeyValuePair<TKey, TValue>> GetEnumerator()
        {
            return enumerate().GetEnumerator();
        }

        #endregion

        IEnumerable<KeyValuePair<TKey, TValue>> enumerate()
        {
            foreach (var dic in this.dictionaries)
            {
                foreach (var pair in dic)
                {
                    yield return pair;
                }
            }
        }


        #region IEnumerable Members

        System.Collections.IEnumerator System.Collections.IEnumerable.GetEnumerator()
        {
            return this.GetEnumerator();
        }

        #endregion

        public void PushDictionary(IDictionary<TKey, TValue> item)
        {
            if (!this.dictionaries.Contains(item))
            {
                this.dictionaries.Push(item);
            }
        }

        public bool ContainsDictionary(IDictionary<TKey, TValue> item)
        {
            return this.dictionaries.Contains(item);
        }

        public IDictionary<TKey, TValue> PopDictionary()
        {
            var item = this.dictionaries.Peek();
            if (this._dictionaryForNewItems == item)
            {
                this._dictionaryForNewItems = null;
            }
            return this.dictionaries.Pop();
        }

        public void SetDictionaryForNewItems(IDictionary<TKey, TValue> dic)
        {
            if (this.ContainsDictionary(dic))
            {
                this._dictionaryForNewItems = dic;
            }
            else
            {
                this.PushDictionary(dic);
                this._dictionaryForNewItems = dic;
            }
        }
    }
}
