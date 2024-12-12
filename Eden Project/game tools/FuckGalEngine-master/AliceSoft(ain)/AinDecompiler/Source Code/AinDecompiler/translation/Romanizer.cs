using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Text;

namespace TranslateParserThingy
{
    static class Romanizer
    {
        public static string Romanize(string hiragana)
        {
            if (!ready)
            {
                BuildDictionary();
            }
            StringBuilder sb = new StringBuilder();
            bool doubleConsonant = false;
            bool longVowel = false;
            string lastMatch = null;
            for (int i = 0; i < hiragana.Length; i++)
            {
                bool matched = false;
                string match = null;
                char c = hiragana[i];
                if (c == 'っ')
                {
                    doubleConsonant = true;
                    matched = true;
                }
                if (c == 'ー')
                {
                    longVowel = true;
                    matched = true;
                }
                else
                {
                    if (i < hiragana.Length - 1)
                    {
                        string twoChars = hiragana.Substring(i, 2);
                        if (dic.ContainsKey(twoChars))
                        {
                            matched = true;
                            match = dic[twoChars];
                            i++;
                        }
                    }
                    if (!matched)
                    {
                        if (dic.ContainsKey(c.ToString()))
                        {
                            matched = true;
                            match = dic[c.ToString()];
                        }
                    }
                }
                if (matched)
                {
                    if (longVowel)
                    {
                        if (lastMatch != null && lastMatch.Length >= 1)
                        {
                            char matchingVowel = lastMatch[lastMatch.Length - 1];
                            if (matchingVowel == 'a')
                            {
                                matchingVowel = 'r';
                            }
                            sb.Append(matchingVowel);
                        }
                        longVowel = false;
                    }
                    if (match != null)
                    {

                        if (doubleConsonant)
                        {
                            if (match.Length >= 2 && match.Substring(2) == "ch")
                            {
                                sb.Append("t" + match);
                            }
                            else
                            {
                                sb.Append(match[0] + match);
                            }
                            doubleConsonant = false;
                        }
                        else
                        {
                            sb.Append(match);
                        }
                        lastMatch = match;
                    }
                }
                else
                {
                    sb.Append(c);
                }
            }
            return sb.ToString();
        }

        private static void BuildDictionary()
        {
            dic = new Dictionary<string, string>();
            for (int i = 0; i < table.Length; i += 2)
            {
                string key, value;
                key = table[i];
                value = table[i + 1];
                dic.Add(key, value);
            }
            ready = true;
        }

        static bool ready = false;
        static Dictionary<string, string> dic;

        static string[] table = new string[]{
            "あ","a",
            "い","i",
            "う","u",
            "え","e",
            "お","o",
            "か","ka",
            "き","ki",
            "く","ku",
            "け","ke",
            "こ","ko",
            "きゃ","kya",
            "きゅ","kyu",
            "きょ","kyo",
            "さ","sa",
            "し","shi",
            "す","su",
            "せ","se",
            "そ","so",
            "しゃ","sha",
            "しゅ","shu",
            "しょ","sho",
            "た","ta",
            "ち","chi",
            "つ","tsu",
            "て","te",
            "と","to",
            "ちゃ","cha",
            "ちゅ","chu",
            "ちょ","cho",
            "な","na",
            "に","ni",
            "ぬ","nu",
            "ね","ne",
            "の","no",
            "にゃ","nya",
            "にゅ","nyu",
            "にょ","nyo",
            "は","ha",
            "ひ","hi",
            "ふ","fu",
            "へ","he",
            "ほ","ho",
            "ひゃ","hya",
            "ひゅ","hyu",
            "ひょ","hyo",
            "ま","ma",
            "み","mi",
            "む","mu",
            "め","me",
            "も","mo",
            "みゃ","mya",
            "みゅ","myu",
            "みょ","myo",
            "や","ya",
            "ゆ","yu",
            "よ","yo",
            "ら","ra",
            "り","ri",
            "る","ru",
            "れ","re",
            "ろ","ro",
            "りゃ","rya",
            "りゅ","ryu",
            "りょ","ryo",
            "わ","wa",
            "ゐ","wi",
            "ゑ","we",
            "を","wo",
            "ん","n",
            "が","ga",
            "ぎ","gi",
            "ぐ","gu",
            "げ","ge",
            "ご","go",
            "ぎゃ","gya",
            "ぎゅ","gyu",
            "ぎょ","gyo",
            "ざ","za",
            "じ","ji",
            "ず","zu",
            "ぜ","ze",
            "ぞ","zo",
            "じゃ","ja",
            "じゅ","ju",
            "じょ","jo",
            "だ","da",
            "ぢ","zi",
            "づ","du",
            "で","de",
            "ど","do",
            "ぢゃ","dya",
            "ぢゅ","dyu",
            "ぢょ","dyo",
            "ば","ba",
            "び","bi",
            "ぶ","bu",
            "べ","be",
            "ぼ","bo",
            "びゃ","bya",
            "びゅ","byu",
            "びょ","byo",
            "ぱ","pa",
            "ぴ","pi",
            "ぷ","pu",
            "ぺ","pe",
            "ぽ","po",
            "ぴゃ","pya",
            "ぴゅ","pyu",
            "ぴょ","pyo",
            "ゃ","+ya",
            "ゅ","+yu",
            "ょ","+yo",
            "いぇ","ye",
            "ふぁ","fa",
            "ふぃ","fi",
            "ふぇ","fe",
            "ふぉ","fo",
            "ヴァ","va",
            "ヴィ","vi",
            "ヴ","vu",
            "ヴェ","ve",
            "ヴォ","vo",
            "ぁ","+a",
            "ぃ","+i",
            "ぅ","+u",
            "ぇ","+e",
            "ぉ","+o"
        };
    }
}
