using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace TranslateParserThingy
{
    public static partial class Extensions
    {
        public static string Merge(this IEnumerable<string> strings)
        {
            int totalCapacity = strings.Sum(s => s.Length);

            StringBuilder sb = new StringBuilder(totalCapacity);
            foreach (var str in strings)
            {
                sb.Append(str);
            }
            return sb.ToString();
        }

        //public static int IndexOfNonAscii(this string str)
        //{
        //    for (int i = 0; i < str.Length; i++)
        //    {
        //        char c = str[i];
        //        if (c >= 128)
        //        {
        //            //if (c < 0xFF00 && c != '　')
        //            //{
        //            return i;
        //            //}
        //        }
        //    }
        //    return -1;
        //}

        public static string Join(this IList<string> strArr, char joinCharacter)
        {
            StringBuilder sb = new StringBuilder();
            for (int i = 0; i < strArr.Count; i++)
            {
                sb.Append(strArr[i]);
                if (i < strArr.Count - 1)
                {
                    sb.Append(joinCharacter);
                }
            }
            return sb.ToString();
        }

        //public static void ReplaceEach<T>(this IList<T> sequence, Func<T, T> action)
        //{
        //    for (int i = 0; i < sequence.Count; i++)
        //    {
        //        sequence[i] = action(sequence[i]);
        //    }
        //}

        //public static void ReplaceEach<T>(this T[] sequence, Func<T, T> action)
        //{
        //    for (int i = 0; i < sequence.Length; i++)
        //    {
        //        sequence[i] = action(sequence[i]);
        //    }
        //}

        ///// <summary>
        ///// Joins substrings together with no separator characters.
        ///// </summary>
        ///// <param name="strArr">The sequence of strings to join together.</param>
        ///// <returns>A string which is the result of joining the strings together.</returns>
        //public static string Join(this IEnumerable<string> strArr)
        //{
        //    StringBuilder sb = new StringBuilder();
        //    strArr.ForEach(s => sb.Append(s));
        //    return sb.ToString();
        //}

        //public static bool Matches<T>(this IEnumerable<T> first, IEnumerable<T> second)
        //{
        //    return first.SequenceEqual(second);
        //    //var e1 = first.GetEnumerator();
        //    //var e2 = second.GetEnumerator();
        //    //while (e1.MoveNext() && e2.MoveNext())
        //    //{
        //    //    if (e1.Current.Equals(e2.Current)) return false;
        //    //}
        //    //return true;
        //}

        public static int BinarySearch(this IList<int> list, int min, int max, int lookFor)
        {
            int index = (min + max) / 2;

            while (min < max)
            {
                int value = list[index];
                if (value < lookFor)
                {
                    min = index + 1;
                }
                else
                {
                    max = index;
                }
                index = (min + max) / 2;
            }
            return index;
        }

        /// <summary>
        /// Returns a sequence starting from the integer, until a specified limit.  Does not include the limit value.
        /// </summary>
        /// <param name="startValue">The value to start from.</param>
        /// <param name="limit">The value to continue until reaching.</param>
        /// <returns>A sequence starting from the start value, until it reaches the limit.  Excludes the limit value.</returns>
        public static IEnumerable<int> Until(this int startValue, int limit)
        {
            if (startValue > limit)
            {
                for (int i = startValue; i > limit; i--)
                {
                    yield return i;
                }
            }
            else
            {
                for (int i = startValue; i < limit; i++)
                {
                    yield return i;
                }
            }
        }

        /// <summary>
        /// Performs the specified action on each element of the sequence.
        /// </summary>
        /// <typeparam name="T">The type of the elements.</typeparam>
        /// <param name="sequence">The sequence to perform the actions on.</param>
        /// <param name="action">The delegate to perform on each element of the sequence.</param>
        public static void ForEach<T>(this IEnumerable<T> sequence, Action<T> action)
        {
            foreach (var element in sequence)
            {
                action(element);
            }
        }

        /// <summary>
        /// Performs the specified action on each element of the sequence.
        /// </summary>
        /// <typeparam name="T">The type of the elements.</typeparam>
        /// <param name="sequence">The sequence to perform the actions on.</param>
        /// <param name="action">The delegate to perform on each element of the sequence.
        /// The second int parameter is the position of the element.</param>
        public static void ForEach<T>(this IEnumerable<T> sequence, Action<T, int> action)
        {
            int i = 0;
            foreach (var element in sequence)
            {
                action(element, i);
                i++;
            }
        }


        /// <summary>
        /// Adds the elements of the specified sequence to the collection.
        /// </summary>
        /// <typeparam name="T">The type of the elements.</typeparam>
        /// <param name="collection">The collection to add items to.</param>
        /// <param name="sequence">
        /// The sequence whose elements should be added to the end of the collection.
        /// </param>
        public static void AddRange<T>(this ICollection<T> collection, IEnumerable<T> sequence)
        {
            foreach (var item in sequence)
            {
                collection.Add(item);
            }
        }

        /// <summary>
        /// Returns a string array that contains the substrings in this string that are
        /// delimited by a specified string.
        /// </summary>
        /// <param name="str">The string to split.</param>
        /// <param name="separator">A string that delimits the substrings in this string.</param>
        /// <returns>
        /// An array whose elements contain the substrings in this string that are delimited
        /// by the separator string.
        /// </returns>
        public static string[] Split(this string str, string separator)
        {
            return str.Split(new string[] { separator }, StringSplitOptions.None);
        }

        public static void RunWhenIdle(this Form form, Action actionToRun)
        {
            Action<object, EventArgs> handler = null;
            handler = (sender, e) =>
                {
                    Application.Idle -= new EventHandler(handler);
                    actionToRun();
                };

            Application.Idle += new EventHandler(handler);
        }
    }
}
