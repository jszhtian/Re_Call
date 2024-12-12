using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CriwareXtxDecoder
{
    public static class Encodings
    {
        public static readonly Encoding cp932 = Encoding.GetEncoding(932);

        public static Encoding WithFatalFallback(this Encoding enc)
        {
            var encoding = enc.Clone() as Encoding;
            encoding.EncoderFallback = EncoderFallback.ExceptionFallback;
            return encoding;
        }
    }
}
