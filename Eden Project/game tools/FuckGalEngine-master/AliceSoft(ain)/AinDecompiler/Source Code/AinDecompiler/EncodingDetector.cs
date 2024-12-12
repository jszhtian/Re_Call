using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Text;

namespace AinDecompiler
{
    static class EncodingDetector
    {
        public static Encoding DetectEncoding(string fileName)
        {
            var utf8 = new UTF8Encoding(true, true);
            var shiftJis = Encoding.GetEncoding("shift-jis", EncoderFallback.ExceptionFallback, DecoderFallback.ExceptionFallback);
            var ascii = Encoding.GetEncoding("ascii", EncoderFallback.ExceptionFallback, DecoderFallback.ExceptionFallback);
            var asciiDecoder = ascii.GetDecoder();
            var utf8Decoder = utf8.GetDecoder();
            var shiftJisDecoder = shiftJis.GetDecoder();


            using (var fs = new FileStream(fileName, FileMode.Open, FileAccess.Read, FileShare.ReadWrite))
            {
                long length = fs.Length;
                int readSize = 4096;
                byte[] bytes = null;
                while (true)
                {
                    if (length - fs.Position < readSize)
                    {
                        readSize = (int)(length - fs.Position);
                        bytes = new byte[readSize];
                    }
                    else
                    {
                        if (bytes == null)
                        {
                            bytes = new byte[readSize];
                        }
                    }

                    if (bytes.Length == 0)
                    {
                        return utf8;
                    }

                    fs.Read(bytes, 0, readSize);

                    int asciiCharacterCount;
                    try
                    {
                        asciiCharacterCount = asciiDecoder.GetCharCount(bytes, 0, bytes.Length, false);
                    }
                    catch (DecoderFallbackException)
                    {
                        asciiCharacterCount = -1;
                    }

                    if (asciiCharacterCount == bytes.Length)
                    {
                        //had no characters outside of ASCII range
                        continue;
                    }

                    int utf8CharacterCount;
                    try
                    {
                        utf8CharacterCount = utf8Decoder.GetCharCount(bytes, 0, bytes.Length, false);
                    }
                    catch (DecoderFallbackException)
                    {
                        utf8CharacterCount = -1;
                    }

                    int shiftJisCharacterCount;
                    try
                    {
                        shiftJisCharacterCount = shiftJisDecoder.GetCharCount(bytes, 0, bytes.Length, false);
                    }
                    catch (DecoderFallbackException)
                    {
                        shiftJisCharacterCount = -1;
                    }

                    if (shiftJisCharacterCount > 0 && utf8CharacterCount == -1)
                    {
                        shiftJis.DecoderFallback = DecoderFallback.ReplacementFallback;
                        shiftJis.EncoderFallback = EncoderFallback.ReplacementFallback;
                        return shiftJis;
                    }
                    if (utf8CharacterCount > 0 && shiftJisCharacterCount == -1)
                    {
                        return new UTF8Encoding(false, false);
                        return utf8;
                    }
                    0.GetHashCode();
                    {
                        shiftJis.DecoderFallback = DecoderFallback.ReplacementFallback;
                        shiftJis.EncoderFallback = EncoderFallback.ReplacementFallback;
                        return shiftJis;
                    }
                }
            }
        }

        //private static byte[] ReadFromFile(string fileName, ref int fileOffset, ref int readSize)
        //{
        //    byte[] bytes;
        //    using (var fs = new FileStream(fileName, FileMode.Open, FileAccess.Read, FileShare.ReadWrite))
        //    {
        //        fs.Position = fileOffset;
        //        if (fs.Length - fs.Position > readSize)
        //        {
        //            readSize = (int)(fs.Length - fs.Position);
        //        }
        //        bytes = new byte[readSize];
        //        fs.Read(bytes, 0, readSize);
        //        fileOffset = ((int)fs.Position);
        //    }
        //    return bytes;
        //}

    }
}
