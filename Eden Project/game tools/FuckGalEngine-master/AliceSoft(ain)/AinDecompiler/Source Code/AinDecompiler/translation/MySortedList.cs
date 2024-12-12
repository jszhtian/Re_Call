using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Text;
using System.Collections.ObjectModel;

namespace TranslateParserThingy
{
    public class MySortedList<TKey, TValue> : IDictionary<TKey, TValue> where TKey : IComparable<TKey>, IEquatable<TKey>
    {
        public class KeyCollection : ReadOnlyCollection<TKey>
        {
            internal KeyCollection(IList<TKey> list) : base(list) { }
        }
        public class ValueCollection : ReadOnlyCollection<TValue>
        {
            internal ValueCollection(IList<TValue> list) : base(list) { }
        }

        protected List<TKey> keys;
        protected List<TValue> values;

        protected KeyCollection keyCollection;
        protected ValueCollection valueCollection;

        public MySortedList()
            : this(0)
        {

        }

        public MySortedList(int capacity)
        {
            keys = new List<TKey>(capacity);
            values = new List<TValue>(capacity);

            this.keyCollection = new KeyCollection(keys);
            this.valueCollection = new ValueCollection(values);
        }

        public MySortedList(IDictionary<TKey, TValue> dictionary)
        {
            foreach (var pair in dictionary)
            {
                ((ICollection<KeyValuePair<TKey, TValue>>)this).Add(pair);
            }
        }

        public TKey KeyAt(int index)
        {
            return keys[index];
        }

        public TValue ValueAt(int index)
        {
            return values[index];
        }

        public void RemoveAt(int index)
        {
            keys.RemoveAt(index);
            values.RemoveAt(index);
        }

        public void RemoveRangeAt(int index, int count)
        {
            keys.RemoveRange(index, count);
            values.RemoveRange(index, count);
        }

        private void InsertAt(int index, TKey key, TValue value)
        {
            keys.Insert(index, key);
            values.Insert(index, value);
        }

        /// <summary>
        /// Searches for a value in a sorted array using Binary Search.  
        /// Returns an index such that:
        /// <para> * If there is a match, its value is the first match that appears in the array </para>
        /// <para> * If there is no match, the value must be the lowest value which is greater than searchKey (Index for inserting)</para>
        /// <para> * If searchKey is greater than entire array, return arrayLength (an out of bounds index) </para>
        /// </summary>
        /// <param name="list">The list to search</param>
        /// <param name="lookFor">The value in the list to look for</param>
        /// <returns>An index</returns>
        public int GetInsertIndex(TKey key)
        {
            int index = Util.BinarySearchFirstGreaterOrEqual<TKey>(this.keys, key);
            return index;
        }

        /// <summary>
        /// Searches for a value in a sorted array using Binary Search.  
        /// Returns an index such that: <br/>
        /// <para> * If there is a match, its value is the first match that appears in the array </para>
        /// <para> * If there is no match, the value must be the highest value which is less than searchKey (Index for searching)</para>
        /// <para> * If searchKey is greater than entire array, return last index in array </para>
        /// </summary>
        /// <param name="list">The list to search</param>
        /// <param name="lookFor">The value in the list to look for</param>
        /// <returns>An index</returns>
        public int GetFetchIndex(TKey key)
        {
            int index = Util.BinarySearchFirstLessOrEqual(this.keys, key);
            return index;
        }

        /// <summary>
        /// Returns the range of values within a particular key range.  Also includes the first value smaller than minimumKey if there's no match for that.
        /// </summary>
        /// <param name="minimumValue">The minimum value to look for (inclusive)</param>
        /// <param name="maxmimumValue">The maximum value to look for (exclusive)</param>
        /// <returns>The range of values within a particular key range</returns>
        public IEnumerable<TValue> ValuesInRange(TKey minimumKey, TKey maxmimumKey)
        {
            var keys = this.Keys;
            int indexOfMinimumKey = this.GetFetchIndex(minimumKey);
            int indexOfMaximumKey = this.GetInsertIndex(maxmimumKey);
            if (indexOfMinimumKey < 0)
            {
                indexOfMinimumKey = 0;
            }
            if (indexOfMaximumKey < 0)
            {
                indexOfMaximumKey = 0;
            }
            if (indexOfMinimumKey >= keys.Count)
            {
                yield break;
            }
            if (indexOfMaximumKey > keys.Count)
            {
                indexOfMaximumKey = keys.Count;
            }
            for (int i = indexOfMinimumKey; i < indexOfMaximumKey; i++)
            {
                yield return this.values[i];
            }
        }

        #region IDictionary<TKey,TValue> Members

        public void Add(TKey key, TValue value)
        {
            int index = GetInsertIndex(key);
            InsertAt(index, key, value);
        }

        public bool ContainsKey(TKey key)
        {
            int index = GetInsertIndex(key);
            if (index >= keys.Count || !key.Equals(keys[index]))
            {
                return false;
            }
            else
            {
                return true;
            }
        }

        public KeyCollection Keys
        {
            get { return keyCollection; }
        }

        public bool Remove(TKey key)
        {
            int index = GetInsertIndex(key);
            if (index >= keys.Count || !key.Equals(keys[index]))
            {
                return false;
            }
            RemoveAt(index);
            return true;
        }

        public bool TryGetValue(TKey key, out TValue value)
        {
            int index = GetInsertIndex(key);
            if (index >= keys.Count || !key.Equals(keys[index]))
            {
                value = default(TValue);
                return false;
            }
            value = values[index];
            return true;
        }

        public ValueCollection Values
        {
            get { return valueCollection; }
        }

        public TValue this[TKey key]
        {
            get
            {
                int index = GetInsertIndex(key);
                if (index >= keys.Count || !key.Equals(keys[index]))
                {
                    return default(TValue);
                }
                return values[index];
            }
            set
            {
                int index = GetInsertIndex(key);
                if (index >= keys.Count || !key.Equals(keys[index]))
                {
                    InsertAt(index, key, value);
                }
                else
                {
                    values[index] = value;
                }
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
            this.keys.Clear();
            this.values.Clear();
        }

        public bool Contains(KeyValuePair<TKey, TValue> item)
        {
            TValue value;
            if (this.TryGetValue(item.Key, out value))
            {
                if (Comparer<TValue>.Default.Compare(value, item.Value) == 0)
                {
                    return true;
                }
            }
            return false;
        }

        public void CopyTo(KeyValuePair<TKey, TValue>[] array, int arrayIndex)
        {
            foreach (var pair in this)
            {
                array[arrayIndex] = pair;
                arrayIndex++;
            }
        }

        public int Count
        {
            get { return this.keys.Count; }
        }

        public bool IsReadOnly
        {
            get { return false; }
        }

        bool ICollection<KeyValuePair<TKey, TValue>>.Remove(KeyValuePair<TKey, TValue> item)
        {
            if (this.Contains(item))
            {
                return this.Remove(item.Key);
            }
            else
            {
                return false;
            }
        }

        #endregion

        private IEnumerable<KeyValuePair<TKey, TValue>> enumerate()
        {
            for (int i = 0; i < keys.Count; i++)
            {
                yield return new KeyValuePair<TKey, TValue>(keys[i], values[i]);
            }
        }

        #region IEnumerable<KeyValuePair<TKey,TValue>> Members

        public IEnumerator<KeyValuePair<TKey, TValue>> GetEnumerator()
        {
            return this.enumerate().GetEnumerator();
        }

        #endregion

        #region IEnumerable Members

        System.Collections.IEnumerator System.Collections.IEnumerable.GetEnumerator()
        {
            return this.GetEnumerator();
        }

        #endregion

        #region IDictionary<TKey,TValue> Members

        ICollection<TKey> IDictionary<TKey, TValue>.Keys
        {
            get { return this.Keys; }
        }

        ICollection<TValue> IDictionary<TKey, TValue>.Values
        {
            get { return this.Values; }
        }

        #endregion

    }

    public static class Util
    {
        /// <summary>
        /// Searches for a value in a sorted array using Binary Search.  
        /// Returns an index such that:
        /// <para> * If there is a match, its value is the first match that appears in the array </para>
        /// <para> * If there is no match, the value must be the lowest value which is greater than searchKey (Index for Inserting)</para>
        /// <para> * If searchKey is greater than entire array, return arrayLength (an out of bounds index) </para>
        /// </summary>
        /// <param name="list">The list to search</param>
        /// <param name="lookFor">The value in the list to look for</param>
        /// <returns>An index</returns>
        public static int BinarySearchFirstGreaterOrEqual<TKey>(IList<TKey> list, TKey lookFor) where TKey : IComparable<TKey>
        {
            int firstIndex = 0;
            int lastIndex = list.Count;
            int currentIndex;
            while (lastIndex > firstIndex)
            {
                currentIndex = (firstIndex + lastIndex) / 2;
                TKey currentValue = list[currentIndex];
                if (currentValue.CompareTo(lookFor) < 0)
                {
                    firstIndex = currentIndex + 1;
                }
                else
                {
                    lastIndex = currentIndex;
                }
            }
            return lastIndex;
        }

        /// <summary>
        /// Searches for a value in a sorted array using Binary Search.  
        /// Returns an index such that: <br/>
        /// <para> * If there is a match, its value is the first match that appears in the array </para>
        /// <para> * If there is no match, the value must be the highest value which is less than searchKey (Index for searching)</para>
        /// <para> * If searchKey is greater than entire array, return last index in array </para>
        /// </summary>
        /// <param name="list">The list to search</param>
        /// <param name="lookFor">The value in the list to look for</param>
        /// <returns>An index</returns>
        public static int BinarySearchFirstLessOrEqual<TKey>(IList<TKey> list, TKey lookFor) where TKey : IComparable<TKey>
        {
            int index = BinarySearchFirstGreaterOrEqual(list, lookFor);
            if (index >= 0 && index < list.Count)
            {
                TKey value = list[index];
                if (value.CompareTo(lookFor) > 0) return index - 1;
                return index;
            }
            if (index >= list.Count)
            {
                return index - 1;
            }
            return index;
        }
    }
}
