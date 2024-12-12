using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CriwareXtxDecoder
{
    public static class Binary
    {
        public static uint BigEndian(uint u)
        {
            return (u & 0xff) << 24 | (u & 0xff00) << 8 | (u & 0xff0000) >> 8 | (u & 0xff000000) >> 24;
        }
        public static int BigEndian(int i)
        {
            return (int)BigEndian((uint)i);
        }
        public static ushort BigEndian(ushort u)
        {
            return (ushort)((u & 0xff) << 8 | (u & 0xff00) >> 8);
        }
        public static short BigEndian(short i)
        {
            return (short)BigEndian((ushort)i);
        }
        public static ulong BigEndian(ulong u)
        {
            return (ulong)BigEndian((uint)(u & 0xffffffff)) << 32
                 | (ulong)BigEndian((uint)(u >> 32));
        }
        public static long BigEndian(long i)
        {
            return (long)BigEndian((ulong)i);
        }

        public static bool AsciiEqual(byte[] name1, string name2)
        {
            return AsciiEqual(name1, 0, name2);
        }

        public static bool AsciiEqual(byte[] name1, int offset, string name2)
        {
            if (name1.Length - offset < name2.Length)
                return false;
            for (int i = 0; i < name2.Length; ++i)
                if ((char)name1[offset + i] != name2[i])
                    return false;
            return true;
        }

        public static void CopyOverlapped(byte[] data, int src, int dst, int count)
        {
            if (dst > src)
            {
                while (count > 0)
                {
                    int preceding = System.Math.Min(dst - src, count);
                    System.Buffer.BlockCopy(data, src, data, dst, preceding);
                    dst += preceding;
                    count -= preceding;
                }
            }
            else
            {
                System.Buffer.BlockCopy(data, src, data, dst, count);
            }
        }

        public static string GetCString(byte[] data, int index, int length_limit, Encoding enc)
        {
            int name_length = 0;
            while (name_length < length_limit && 0 != data[index + name_length])
                name_length++;
            return enc.GetString(data, index, name_length);
        }

        public static string GetCString(byte[] data, int index, int length_limit)
        {
            return GetCString(data, index, length_limit, Encodings.cp932);
        }

        public static uint RotR(uint v, int count)
        {
            count &= 0x1F;
            return v >> count | v << (32 - count);
        }

        public static uint RotL(uint v, int count)
        {
            count &= 0x1F;
            return v << count | v >> (32 - count);
        }

        public static ulong RotR(ulong v, int count)
        {
            count &= 0x3F;
            return v >> count | v << (64 - count);
        }

        public static ulong RotL(ulong v, int count)
        {
            count &= 0x3F;
            return v << count | v >> (64 - count);
        }

        public static byte RotByteR(byte v, int count)
        {
            count &= 7;
            return (byte)(v >> count | v << (8 - count));
        }

        public static byte RotByteL(byte v, int count)
        {
            count &= 7;
            return (byte)(v << count | v >> (8 - count));
        }
    }

    public static class BigEndian
    {
        public static ushort ToUInt16(byte[] value, int index)
        {
            return (ushort)(value[index] << 8 | value[index + 1]);
        }

        public static short ToInt16(byte[] value, int index)
        {
            return (short)(value[index] << 8 | value[index + 1]);
        }

        public static uint ToUInt32(byte[] value, int index)
        {
            return (uint)(value[index] << 24 | value[index + 1] << 16 | value[index + 2] << 8 | value[index + 3]);
        }

        public static int ToInt32(byte[] value, int index)
        {
            return (int)ToUInt32(value, index);
        }
    }

    public static class LittleEndian
    {
        public static ushort ToUInt16(byte[] value, int index)
        {
            return (ushort)(value[index] | value[index + 1] << 8);
        }

        public static short ToInt16(byte[] value, int index)
        {
            return (short)(value[index] | value[index + 1] << 8);
        }

        public static uint ToUInt32(byte[] value, int index)
        {
            return (uint)(value[index] | value[index + 1] << 8 | value[index + 2] << 16 | value[index + 3] << 24);
        }

        public static int ToInt32(byte[] value, int index)
        {
            return (int)ToUInt32(value, index);
        }

        public static ulong ToUInt64(byte[] value, int index)
        {
            return (ulong)ToUInt32(value, index) | ((ulong)ToUInt32(value, index + 4) << 32);
        }

        public static long ToInt64(byte[] value, int index)
        {
            return (long)ToUInt64(value, index);
        }

        public static void Pack(ushort value, byte[] buf, int index)
        {
            buf[index] = (byte)(value);
            buf[index + 1] = (byte)(value >> 8);
        }

        public static void Pack(uint value, byte[] buf, int index)
        {
            buf[index] = (byte)(value);
            buf[index + 1] = (byte)(value >> 8);
            buf[index + 2] = (byte)(value >> 16);
            buf[index + 3] = (byte)(value >> 24);
        }

        public static void Pack(ulong value, byte[] buf, int index)
        {
            Pack((uint)value, buf, index);
            Pack((uint)(value >> 32), buf, index + 4);
        }

        public static void Pack(short value, byte[] buf, int index)
        {
            Pack((ushort)value, buf, index);
        }

        public static void Pack(int value, byte[] buf, int index)
        {
            Pack((uint)value, buf, index);
        }

        public static void Pack(long value, byte[] buf, int index)
        {
            Pack((ulong)value, buf, index);
        }
    }
}
