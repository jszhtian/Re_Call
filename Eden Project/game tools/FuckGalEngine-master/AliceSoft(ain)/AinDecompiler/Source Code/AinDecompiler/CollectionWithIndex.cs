using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Text;
using System.Collections.ObjectModel;

namespace AinDecompiler
{
    public class CollectionWithIndex<T> : Collection<T> where T : IWithIndex
    {
        protected override void ClearItems()
        {
            foreach (var item in this)
            {
                if (item.Index == -1)
                {

                }
                item.Index = -1;
            }
            base.ClearItems();
        }

        protected override void InsertItem(int index, T item)
        {
            if (item.Index != -1)
            {

            }

            if (index < this.Count)
            {
                for (int i = index; i < this.Count; i++)
                {
                    this[i].Index++;
                }
            }
            item.Index = index;
            base.InsertItem(index, item);
        }

        protected override void RemoveItem(int index)
        {
            var oldItem = this[index];
            if (oldItem != null)
            {
                if (oldItem.Index == -1)
                {

                }
                oldItem.Index = -1;
            }
            base.RemoveItem(index);
        }

        protected override void SetItem(int index, T item)
        {
            if (item.Index != -1)
            {

            }
            var oldItem = this[index];
            if (oldItem != null)
            {
                if (oldItem.Index == -1)
                {

                }
                oldItem.Index = -1;
            }
            base.SetItem(index, item);
        }
    }
}
