using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace TranslateParserThingy
{
    public class TextUtility
    {
        /// <summary>
        /// Code page 932 (Japanese, Shift-Jis)
        /// </summary>
        const int SHIFTJIS = 932;

        /// <summary>
        /// Code Page 1252 (Western European)
        /// </summary>
        const int WESTERN = 1252;

        public static string AsciiToJascii(string str)
        {
            StringBuilder sb = new StringBuilder(str);
            for (int i = 0; i < str.Length; i++)
            {
                if (sb[i] == ' ')
                {
                    sb[i] = '　';
                }
                else if (sb[i] == '"')
                {
                    sb[i] = '゛';
                }
                else if (sb[i] == '\'')
                {
                    sb[i] = '’';
                }
                else if (sb[i] == '@')
                {
                    sb[i] = '@';
                }
                else if (sb[i] == '-')
                {
                    sb[i] = 'ー';
                }
                else if (sb[i] == '<')
                {
                    sb[i] = '〈';
                }
                else if (sb[i] == '>')
                {
                    sb[i] = '〉';
                }
                else if (sb[i] > 32 && sb[i] < 128)
                {
                    sb[i] = (char)(0xFF00 - 32 + sb[i]);
                }
            }
            return sb.ToString();
        }

        public static string JasciiToAscii(string str)
        {
            StringBuilder sb = new StringBuilder(str);
            for (int i = 0; i < str.Length; i++)
            {
                if (sb[i] == '　')
                {
                    sb[i] = ' ';
                }
                else if (sb[i] == '゛')
                {
                    sb[i] = '"';
                }
                else if (sb[i] == '’')
                {
                    sb[i] = '\'';
                }
                else if (sb[i] == 'ー')
                {
                    sb[i] = '-';
                }
                else if (sb[i] == '〈')
                {
                    sb[i] = '<';
                }
                else if (sb[i] == '〉')
                {
                    sb[i] = '>';
                }
                else if (sb[i] > 0xFF00 && sb[i] <= 0xFF5E)
                {
                    sb[i] = (char)(sb[i] - 0xFF00 + 32);
                }
            }
            return sb.ToString();
        }

        private static Dictionary<char, byte> _inverseWestern = null;
        private static Dictionary<char, byte> InverseWestern
        {
            get
            {
                if (_inverseWestern != null)
                {
                    return _inverseWestern;
                }
                _inverseWestern = new Dictionary<char, byte>();
                var Western = Encoding.GetEncoding(WESTERN);
                var text = Western.GetString(0.Until(256).Select(i => (byte)i).ToArray());
                text.ForEach((c, i) => _inverseWestern.Add(c, (byte)i));
                return _inverseWestern;
            }
        }

        private static int IndexOfNonWesternCharacter(string str, int start)
        {
            var inverseWestern = InverseWestern;
            int i;
            for (i = start; i < str.Length; i++)
            {
                if (!inverseWestern.ContainsKey(str[i]))
                {
                    return i;
                }
            }
            return i;
        }

        private static int IndexOfWesternCharacter(string str, int start)
        {
            var inverseWestern = InverseWestern;
            int i;
            for (i = start; i < str.Length; i++)
            {
                if (inverseWestern.ContainsKey(str[i]))
                {
                    return i;
                }
            }
            return i;
        }


        public static string FixBrokenText(string text)
        {
            var Western = Encoding.GetEncoding(WESTERN);
            var ShiftJis = Encoding.GetEncoding(SHIFTJIS);
            var chars = text.ToCharArray();
            StringBuilder sb = new StringBuilder();
            int i = 0;
            while (i < text.Length)
            {
                {
                    int next = IndexOfNonWesternCharacter(text, i);
                    int length = next - i;
                    var bytes = Western.GetBytes(chars, i, length);
                    var newText = ShiftJis.GetString(bytes);
                    sb.Append(newText);
                    i = next;
                }
                {
                    int next = IndexOfWesternCharacter(text, i);
                    int length = next - i;
                    var newText = text.Substring(i, length);
                    sb.Append(newText);
                    i = next;
                }
            }
            return sb.ToString();
        }

        public static string Romanize(string text)
        {
            string hiragana = JapaneseTextUtil.ToHiragana2(text);
            return Romanizer.Romanize(hiragana);
        }
    }
}
