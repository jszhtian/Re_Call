using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Text;

namespace TranslateParserThingy
{
    public static class JapaneseTextUtil
    {
        // katakana -> hiragana conversion tables
        static int[] ch ={0x3092,0x3041,0x3043,0x3045,0x3047,0x3049,0x3083,0x3085,0x3087,0x3063,0x30FC,0x3042,0x3044,0x3046,
		    0x3048,0x304A,0x304B,0x304D,0x304F,0x3051,0x3053,0x3055,0x3057,0x3059,0x305B,0x305D,0x305F,0x3061,
		    0x3064,0x3066,0x3068,0x306A,0x306B,0x306C,0x306D,0x306E,0x306F,0x3072,0x3075,0x3078,0x307B,0x307E,
		    0x307F,0x3080,0x3081,0x3082,0x3084,0x3086,0x3088,0x3089,0x308A,0x308B,0x308C,0x308D,0x308F,0x3093};
        static int[] cv ={0x30F4,0xFF74,0xFF75,0x304C,0x304E,0x3050,0x3052,0x3054,0x3056,0x3058,0x305A,0x305C,0x305E,0x3060,
		    0x3062,0x3065,0x3067,0x3069,0xFF85,0xFF86,0xFF87,0xFF88,0xFF89,0x3070,0x3073,0x3076,0x3079,0x307C};
        static int[] cs = { 0x3071, 0x3074, 0x3077, 0x307A, 0x307D };
        /// <summary>
        /// Converts Katakana and Halfwidth Katakana to Hiragana
        /// note: katakana vu is never converted to hiragana
        /// </summary>
        /// <param name="word"></param>
        /// <returns></returns>
        public static string ToHiragana(this string word)
        {
            StringBuilder r = new StringBuilder();
            int p = 0;

            for (var i = 0; i < word.Length; ++i)
            {
                var u = (int)word[i];
                var v = u;

                if (u <= 0x3000) break;

                // full-width katakana to hiragana
                if ((u >= 0x30A1) && (u <= 0x30F3))
                {
                    u -= 0x60;
                }
                // half-width katakana to hiragana
                else if ((u >= 0xFF66) && (u <= 0xFF9D))
                {
                    u = ch[u - 0xFF66];
                }
                // voiced (used in half-width katakana) to hiragana
                else if (u == 0xFF9E)
                {
                    if ((p >= 0xFF73) && (p <= 0xFF8E))
                    {
                        r.Length--;
                        u = cv[p - 0xFF73];
                    }
                }
                // semi-voiced (used in half-width katakana) to hiragana
                else if (u == 0xFF9F)
                {
                    if ((p >= 0xFF8A) && (p <= 0xFF8E))
                    {
                        r.Length--;
                        u = cs[p - 0xFF8A];
                    }
                }
                // ignore J~
                else if (u == 0xFF5E)
                {
                    p = 0;
                    continue;
                }

                r.Append((char)u);
            }
            return r.ToString();
        }

        /// <summary>
        /// Converts Katakana and Halfwidth Katakana to Hiragana
        /// note: katakana vu is never converted to hiragana
        /// </summary>
        /// <param name="word"></param>
        /// <returns></returns>
        public static string ToHiragana2(this string text)
        {
            StringBuilder r = new StringBuilder(text.Length);
            int p = 0;

            for (var i = 0; i < text.Length; ++i)
            {
                var u = (int)text[i];

                // full-width katakana to hiragana
                if ((u >= 0x30A1) && (u <= 0x30F3))
                {
                    u -= 0x60;
                }
                // half-width katakana to hiragana
                else if ((u >= 0xFF66) && (u <= 0xFF9D))
                {
                    u = ch[u - 0xFF66];
                }
                // voiced (used in half-width katakana) to hiragana
                else if (u == 0xFF9E)
                {
                    if ((p >= 0xFF73) && (p <= 0xFF8E))
                    {
                        r.Length--;
                        u = cv[p - 0xFF73];
                    }
                }
                // semi-voiced (used in half-width katakana) to hiragana
                else if (u == 0xFF9F)
                {
                    if ((p >= 0xFF8A) && (p <= 0xFF8E))
                    {
                        r.Length--;
                        u = cs[p - 0xFF8A];
                    }
                }
                r.Append((char)u);
            }
            return r.ToString();
        }
    }
}
