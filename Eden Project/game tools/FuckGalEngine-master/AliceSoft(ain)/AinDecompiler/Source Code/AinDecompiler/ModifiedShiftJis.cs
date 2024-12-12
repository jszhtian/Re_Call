using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Text;

namespace AinDecompiler
{
    class ModifiedShiftJis : Encoding
    {
        Encoding shiftJis = Encoding.GetEncoding("shift-jis");
        public override int GetByteCount(char[] chars, int index, int count)
        {
            int bI = 0;
            for (int cI = 0; cI < count; cI++)
            {
                char c = chars[cI + index];
                if (c >= 0xC0 && c <= 0xFF)
                {
                    bI++;
                }
                else
                {
                    bI += shiftJis.GetByteCount(chars, cI + index, 1);
                }
            }
            return bI;
        }

        public override int GetBytes(char[] chars, int charIndex, int charCount, byte[] bytes, int byteIndex)
        {
            int bI = 0;
            for (int cI = 0; cI < charCount; cI++)
            {
                char c = chars[cI + charIndex];
                if (c >= 0xC0 && c <= 0xFF)
                {
                    bytes[bI + byteIndex] = (byte)(c - 0x20);
                    bI++;
                }
                else
                {
                    bI += shiftJis.GetBytes(chars, cI + charIndex, 1, bytes, bI + byteIndex);
                }
            }
            return bI;
        }

        public override int GetCharCount(byte[] bytes, int index, int count)
        {
            return shiftJis.GetCharCount(bytes, index, count);
        }

        public override int GetChars(byte[] bytes, int byteIndex, int byteCount, char[] chars, int charIndex)
        {
            int charCount = shiftJis.GetChars(bytes, byteIndex, byteCount, chars, charIndex);
            int cI = 0;
            for (int bI = 0; bI < byteCount; bI++)
            {
                byte b = bytes[byteIndex + bI];
                if ((b >= 0x80 && b < 0xA0) || (b >= 0xE0 && b <= 0xFF))
                {
                    bI++;
                }
                else if (b >= 0xA0 && b < 0xE0)
                {
                    chars[cI] = (char)(b + 0x20);
                }
                else
                {

                }
                cI++;
            }
            return charCount;
        }

        public override int GetMaxByteCount(int charCount)
        {
            return shiftJis.GetMaxByteCount(charCount);
        }

        public override int GetMaxCharCount(int byteCount)
        {
            return shiftJis.GetMaxCharCount(byteCount);
        }
    }
}
