using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Text;
using System.Text.RegularExpressions;

namespace TranslateParserThingy
{
    public struct RegularExpressionMatchValue
    {
        public string StringValue;
        public bool IsMatch;

        public override string ToString()
        {
            return StringValue + " (" + IsMatch.ToString() + ")";
        }
    }

    public class RegularExpressionMatchList : IEnumerable<RegularExpressionMatchValue>
    {
        string inputString;
        ExtentList extentList;
        public RegularExpressionMatchList(string inputString, IEnumerable<string> regularExpressions)
        {
            this.inputString = inputString;
            this.extentList = new ExtentList(inputString.Length);
            this.ProcessRegularExpressions(regularExpressions);
        }

        private void ProcessRegularExpressions(IEnumerable<string> regularExpressions)
        {
            RegexOptions options = RegexOptions.Singleline | RegexOptions.Compiled | RegexOptions.CultureInvariant;
            var matches = regularExpressions.SelectMany(expression => Regex.Matches(inputString, expression, options).OfType<Match>());

            foreach (var match in matches)
            {
                if (match.Groups.Count > 1)
                {
                    for (int i = 1; i < match.Groups.Count; i++)
                    {
                        var group = match.Groups[i];
                        extentList.SetRange(group.Index, group.Length);
                    }
                }
                else
                {
                    extentList.SetRange(match.Index, match.Length);
                }
            }
        }

        private IEnumerable<RegularExpressionMatchValue> Enumerate()
        {
            foreach (var extent in extentList.List)
            {
                yield return new RegularExpressionMatchValue { IsMatch = extent.Member, StringValue = inputString.Substring(extent.Start, extent.Length) };
            }
        }

        #region IEnumerable<ExtentListForRegularExpressionsResult> Members

        public IEnumerator<RegularExpressionMatchValue> GetEnumerator()
        {
            return this.Enumerate().GetEnumerator();
        }

        #endregion

        #region IEnumerable Members

        System.Collections.IEnumerator System.Collections.IEnumerable.GetEnumerator()
        {
            return this.GetEnumerator();
        }

        #endregion
    }

}
