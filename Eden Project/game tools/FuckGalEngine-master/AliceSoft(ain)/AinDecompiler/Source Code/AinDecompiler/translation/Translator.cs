using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Text;
using System.Runtime.InteropServices;
using System.Text.RegularExpressions;

namespace TranslateParserThingy
{
    public class TranslatorDll : IDisposable
    {
        [DllImport("test5.dll", CharSet = CharSet.Unicode)]
        extern private static void init();
        [DllImport("test5.dll", CharSet = CharSet.Unicode)]
        extern private static string translate(string text);
        [DllImport("test5.dll", CharSet = CharSet.Unicode)]
        extern private static void destroy();

        public TranslatorDll()
        {
            init();
        }

        public string Translate(string translateThis)
        {
            return translate(translateThis);
        }

        public void Dispose()
        {
            destroy();
        }
    }

    public static class Translator
    {
        public static TranslationOptions TranslationOptions = new TranslationOptions();

        //[DllImport("test5.dll", CharSet = CharSet.Unicode)]
        //extern private static string translate(string text);

        //public static string TranslateTogether(string text)
        //{
        //    return translate(text);
        //}

        public static string TranslateText(string text)
        {
            var translator = new BackgroundTranslation();
            return translator.TranslateText(text);

            //var lines = text.Split(new[] { "\r\n" }, StringSplitOptions.None);
            //var translatedLines = TranslateLines(lines, null);
            //return translatedLines.Join("\r\n");
        }

        public static string[] TranslateLines(IEnumerable<string> lines, Action<int, int> reportProgress)
        {
            using (var translator = new TranslatorDll())
            {
                int max = 0;
                Parse(lines, (s, i) =>
                {
                    i++;
                    if (max < i) max = i;
                }, null);

                var returnLines = Parse(lines, null, (s, i) =>
                {
                    if (reportProgress != null)
                    {
                        reportProgress(i, max);
                    }
                    //statusLabel.Refresh();
                    string returnValue = translator.Translate(s);
                    if (returnValue != null)
                    {
                        RegexOptions options = RegexOptions.Compiled | RegexOptions.Singleline | RegexOptions.CultureInvariant;
                        foreach (var expression in TranslationOptions.RegularExpressionsToRemove)
                        {
                            returnValue = Regex.Replace(returnValue, expression, "", options);
                        }

                        int maxIndex = Math.Max(TranslationOptions.RegularExpressionsToReplace.Length, TranslationOptions.RegularExpressionReplacements.Length);

                        for (int expressionIndex = 0; expressionIndex < maxIndex; expressionIndex++)
                        {
                            var expression = TranslationOptions.RegularExpressionsToReplace[expressionIndex];
                            var replacement = TranslationOptions.RegularExpressionReplacements[expressionIndex];

                            returnValue = Regex.Replace(returnValue, expression, replacement, options);
                        }


                        //"？！。";
                        if (TranslationOptions.RemoveBracketsAndSemicolons)
                        {
                            returnValue = returnValue.Replace("[", "");
                            returnValue = returnValue.Replace("]", "");
                            returnValue = returnValue.Replace(";", "");
                            returnValue = returnValue.Replace(" :", " ");
                            //returnValue = returnValue.Replace("?.", "?");
                            //returnValue = returnValue.Trim();
                        }
                        if (s.EndsWith("？") && returnValue.EndsWith(". "))
                        {
                            returnValue = returnValue.Substring(0, returnValue.Length - 2) + "? ";
                        }
                        if (s.EndsWith("！") && returnValue.EndsWith(". "))
                        {
                            returnValue = returnValue.Substring(0, returnValue.Length - 2) + "! ";
                        }

                        return returnValue;
                        //return AsciiToJascii(returnValue);
                    }
                    return null;
                });
                return returnLines;
            }
        }

        static string[] Parse(IEnumerable<string> lines, Action<string, int> dumper, Func<string, int, string> replacer)
        {
            List<string> output = new List<string>();
            int sequence = 0;

            Func<string, string> handle = str =>
            {
                string replacement = str;
                if (dumper != null)
                {
                    dumper(str, sequence);
                }
                if (replacer != null)
                {
                    replacement = replacer(str, sequence);
                }
                sequence++;
                return replacement;
            };

            Func<string, string> mainParse = line =>
            {
                //seek to first japanese character
                int nonAsciiIndex = line.IndexOfNonAscii();
                if (nonAsciiIndex >= 0)
                {
                    string first = line.Substring(0, nonAsciiIndex);
                    string remaining = line.Substring(nonAsciiIndex);
                    remaining = handle(remaining);
                    line = first + remaining;
                }
                return line;
            };

            foreach (var rawLine in lines)
            {
                string line = rawLine;

                var partEntries = SplitJapanese(line);
                var parts = partEntries.Select(p => p.Text).ToArray();
                for (int i = 0; i < partEntries.Length; i++)
                {
                    //bool skip = false;
                    var pair = partEntries[i];
                    string part = pair.Text;
                    string newPart = part;
                    if (pair.TranslateThis && part.IndexOfNonAscii() >= 0)
                    {
                        newPart = mainParse(part);
                        if (newPart != part)
                        {
                            parts[i] = newPart;
                        }
                    }
                    //if (regexToIgnore != "")
                    //{
                    //    var match = Regex.Match(part, regexToIgnore, RegexOptions.Compiled | RegexOptions.Singleline | RegexOptions.CultureInvariant);
                    //    if (match.Success == true)
                    //    {
                    //        if (match.Index == 0 && match.Length == part.Length)
                    //        {
                    //            skip = true;
                    //        }
                    //    }
                    //}
                    //string newPart = part;
                    //if (part.IndexOfNonAscii() >= 0 && skip == false)
                    //{
                    //    newPart = mainParse(part);
                    //    if (newPart != part)
                    //    {
                    //        parts[i] = newPart;
                    //    }
                    //}
                }

                string newLine = parts.Join();

                if (newLine != line && TranslationOptions.UseCommentCharacter)
                {
                    output.Add(TranslationOptions.CommentCharacter + line);
                }
                output.Add(newLine);

            }
            return output.ToArray();
        }

        static string ContinuationSetCharacters = "&'()+-0123456789rs";
        static string BreakAfterCharacters = "？！。";

        static HashSet<char> ContinuationSet;
        static HashSet<char> BreakAfterSet;

        struct SplitResult
        {
            public string Text;
            public bool TranslateThis;
            public SplitResult(string text, bool translateThis)
            {
                this.Text = text;
                this.TranslateThis = translateThis;
            }
        }


        static SplitResult[] SplitJapanese(string input)
        {
            //stuff to make the continuation characters changable
            if (TranslationOptions.ContinuationCharacters == ContinuationSetCharacters)
            {
                ContinuationSet = null;
                ContinuationSetCharacters = TranslationOptions.ContinuationCharacters;
            }
            if (ContinuationSet == null)
            {
                ContinuationSet = new HashSet<char>(ContinuationSetCharacters.ToCharArray());
            }

            //stuff to make the break after characters changable
            if (TranslationOptions.BreakAfterCharacters == BreakAfterCharacters)
            {
                BreakAfterSet = null;
                BreakAfterCharacters = TranslationOptions.BreakAfterCharacters;
            }
            if (BreakAfterSet == null)
            {
                BreakAfterSet = new HashSet<char>(BreakAfterCharacters.ToCharArray());
            }

            return SplitJapanese_Whitelist(input);
        }

        static SplitResult[] SplitJapanese_Blacklist(string input)
        {
            var ignoreRegEx = TranslationOptions.RegularExpressionsToIgnore;

            if (ignoreRegEx.Length > 0 && !(ignoreRegEx.Length == 1 && ignoreRegEx[0] == ""))
            {
                var matches = new RegularExpressionMatchList(input, ignoreRegEx);
                List<SplitResult> result = new List<SplitResult>();

                foreach (var pair in matches)
                {
                    if (pair.IsMatch)
                    {
                        result.Add(new SplitResult(pair.StringValue, false));
                    }
                    else
                    {
                        result.AddRange(SplitJapanese_Main(pair.StringValue));
                    }
                }
                return result.ToArray();
            }
            else
            {
                return SplitJapanese_Main(input);
            }
        }

        static SplitResult[] SplitJapanese_Whitelist(string input)
        {
            var translateOnlyRegEx = TranslationOptions.RegularExpressionWhitelist;

            if (translateOnlyRegEx.Length > 0 && !(translateOnlyRegEx.Length == 1 && translateOnlyRegEx[0] == ""))
            {
                var matches = new RegularExpressionMatchList(input, translateOnlyRegEx);
                List<SplitResult> result = new List<SplitResult>();
                foreach (var pair in matches)
                {
                    if (pair.IsMatch)
                    {
                        result.AddRange(SplitJapanese_Blacklist(pair.StringValue));
                    }
                    else
                    {
                        result.Add(new SplitResult(pair.StringValue, false));
                    }
                }
                return result.ToArray();
            }
            else
            {
                return SplitJapanese_Blacklist(input);
            }
        }

        static SplitResult[] SplitJapanese_Main(string input)
        {
            var continuationSet = ContinuationSet;
            var breakAfterSet = BreakAfterSet;

            List<SplitResult> list = new List<SplitResult>();
            if (input.Length == 0) return list.ToArray();
            bool isJapanese = input[0] >= 128;

            bool sawOk = false;

            int lastOkayI = -1;
            int lastSplitPoint = 0;

            bool breakAfterThis = false;

            for (int i = 0; i < input.Length; i++)
            {
                //OKAY characters for either ASCII or Japanese: 
                //&'()+-0123456789rs
                char c = input[i];
                if (isJapanese)
                {
                    //if (breakAfterThis && breakAfterSet.Contains(c))
                    //{
                    //    breakAfterThis = false;
                    //}
                    if (c < 128 || breakAfterThis)
                    {
                        breakAfterThis = false;
                        if (continuationSet.Contains(c))
                        {
                            if (sawOk == false)
                            {
                                sawOk = true;
                                lastOkayI = i;
                            }
                        }
                        else
                        {
                            int splitPoint = i;
                            if (sawOk)
                            {
                                splitPoint = lastOkayI;
                            }
                            string piece = input.Substring(lastSplitPoint, splitPoint - lastSplitPoint);
                            list.Add(new SplitResult(piece, isJapanese));  //isJapanese == true
                            lastSplitPoint = splitPoint;

                            sawOk = false;
                            lastOkayI = -1;
                            isJapanese = false;
                        }
                    }
                    else
                    {
                        if (breakAfterSet.Contains(c))
                        {
                            breakAfterThis = true;
                        }
                        sawOk = false;
                        lastOkayI = -1;
                    }
                }
                if (!isJapanese && c > 128)
                {
                    int splitPoint = i;
                    string piece = input.Substring(lastSplitPoint, splitPoint - lastSplitPoint);
                    list.Add(new SplitResult(piece, isJapanese));  //isJapanese == false
                    lastSplitPoint = splitPoint;
                    isJapanese = true;
                }
            }
            {
                int splitPoint = input.Length;
                string piece = input.Substring(lastSplitPoint, splitPoint - lastSplitPoint);
                list.Add(new SplitResult(piece, isJapanese));
                lastSplitPoint = splitPoint;
            }
            return list.ToArray();
        }
    }

    public static class TranslatorExtensions
    {
        public static int IndexOfNonAscii(this string str)
        {
            for (int i = 0; i < str.Length; i++)
            {
                char c = str[i];
                if (c >= 128)
                {
                    //if (c < 0xFF00 && c != '　')
                    //{
                    return i;
                    //}
                }
            }
            return -1;
        }

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

        public static string Join(this IList<string> strArr, string joinString)
        {
            StringBuilder sb = new StringBuilder();
            for (int i = 0; i < strArr.Count; i++)
            {
                sb.Append(strArr[i]);
                if (i < strArr.Count - 1)
                {
                    sb.Append(joinString);
                }
            }
            return sb.ToString();
        }

        ///// <summary>
        ///// Joins substrings together with no separator characters.
        ///// </summary>
        ///// <param name="strings">The sequence of strings to join together.</param>
        ///// <returns>A string which is the result of joining the strings together.</returns>
        public static string Join(this IEnumerable<string> strings)
        {
            int totalCapacity = strings.Sum(s => s.Length);

            StringBuilder sb = new StringBuilder(totalCapacity);
            strings.ForEach(s => sb.Append(s));
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

        //public static bool Matches<T>(this IEnumerable<T> first, IEnumerable<T> second)
        //{
        //    var e1 = first.GetEnumerator();
        //    var e2 = second.GetEnumerator();
        //    while (e1.MoveNext() && e2.MoveNext())
        //    {
        //        if (e1.Current.Equals(e2.Current)) return false;
        //    }
        //    return true;
        //}

        //public static int BinarySearch(this IList<int> list, int min, int max, int lookFor)
        //{
        //    int index = (min + max) / 2;

        //    while (min < max)
        //    {
        //        int value = list[index];
        //        if (value < lookFor)
        //        {
        //            min = index + 1;
        //        }
        //        else
        //        {
        //            max = index;
        //        }
        //        index = (min + max) / 2;
        //    }
        //    return index;
        //}

        //public static IEnumerable<int> Until(this int startValue, int limit)
        //{
        //    if (startValue > limit)
        //    {
        //        for (int i = startValue; i > limit; i--)
        //        {
        //            yield return i;
        //        }
        //    }
        //    else
        //    {
        //        for (int i = startValue; i < limit; i++)
        //        {
        //            yield return i;
        //        }
        //    }
        //}

        //public static void ForEach<T>(this IEnumerable<T> sequence, Action<T> action)
        //{
        //    foreach (var element in sequence)
        //    {
        //        action(element);
        //    }
        //}

        //public static void ForEach<T>(this IEnumerable<T> sequence, Action<T, int> action)
        //{
        //    int i = 0;
        //    foreach (var element in sequence)
        //    {
        //        action(element, i);
        //        i++;
        //    }
        //}
    }
}
