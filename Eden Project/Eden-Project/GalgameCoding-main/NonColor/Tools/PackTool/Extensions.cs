using System;
using System.Collections.Generic;
using System.Text;

namespace PackTool
{
    static class Extensions
    {
        public static uint Reverse(this uint value)
        {
            return value << 24 | (value & 0xff00) << 8 | (value & 0xff0000) >> 8 | value >> 24;
        }
    }
}
