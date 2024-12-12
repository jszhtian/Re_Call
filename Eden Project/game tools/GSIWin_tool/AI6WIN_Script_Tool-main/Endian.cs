using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GSIWin_Script_Tool
{
    static class Endian
    {
        public static ushort Reverse(ushort value)
        {
            return (ushort)((((int)value & 0xFF) << 8) | (int)((value >> 8) & 0xFF));
        }

        public static uint Reverse(uint value)
        {
            return (((uint)Reverse((ushort)value) & 0xFFFF) << 16)
                    | ((uint)Reverse((ushort)(value >> 16)) & 0xFFFF);
        }
    }
}
