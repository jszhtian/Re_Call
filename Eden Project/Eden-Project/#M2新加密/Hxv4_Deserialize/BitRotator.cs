using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Hxv4_Deserialize
{
    // Microsoft.VisualStudio.Utilities.BitRotator
    public static class BitRotator
    {
        /// <summary>
        /// Rotates the bits of a signed byte value to the left
        /// </summary>
        /// <param name="value">The value to rotate</param>
        /// <param name="count">The number of positions to rotate</param>
        /// <returns>The rotated value</returns>
        public static sbyte RotateLeft(sbyte value, int count)
        {
            return (sbyte)RotateLeft((byte)value, count);
        }

        /// <summary>
        /// Rotates the bits of an unsigned byte value to the left
        /// </summary>
        /// <param name="value">The value to rotate</param>
        /// <param name="count">The number of positions to rotate</param>
        /// <returns>The rotated value</returns>
        public static byte RotateLeft(byte value, int count)
        {
            if (count < 0)
            {
                return RotateRight(value, -count);
            }
            count %= 8;
            return (byte)((value << count) | (value >> 8 - count));
        }

        /// <summary>
        /// Rotates the bits of a signed byte value to the right
        /// </summary>
        /// <param name="value">The value to rotate</param>
        /// <param name="count">The number of positions to rotate</param>
        /// <returns>The rotated value</returns>
        public static sbyte RotateRight(sbyte value, int count)
        {
            return (sbyte)RotateRight((byte)value, count);
        }

        /// <summary>
        /// Rotates the bits of an unsigned byte value to the right
        /// </summary>
        /// <param name="value">The value to rotate</param>
        /// <param name="count">The number of positions to rotate</param>
        /// <returns>The rotated value</returns>
        public static byte RotateRight(byte value, int count)
        {
            if (count < 0)
            {
                return RotateLeft(value, -count);
            }
            count %= 8;
            return (byte)((value >> count) | (value << 8 - count));
        }

        /// <summary>
        /// Rotates the bits of a signed short value to the left
        /// </summary>
        /// <param name="value">The value to rotate</param>
        /// <param name="count">The number of positions to rotate</param>
        /// <returns>The rotated value</returns>
        public static short RotateLeft(short value, int count)
        {
            return (short)RotateLeft((ushort)value, count);
        }

        /// <summary>
        /// Rotates the bits of an unsigned short value to the left
        /// </summary>
        /// <param name="value">The value to rotate</param>
        /// <param name="count">The number of positions to rotate</param>
        /// <returns>The rotated value</returns>
        public static ushort RotateLeft(ushort value, int count)
        {
            if (count < 0)
            {
                return RotateRight(value, -count);
            }
            count %= 16;
            return (ushort)((value << count) | (value >> 16 - count));
        }

        /// <summary>
        /// Rotates the bits of a signed short value to the right
        /// </summary>
        /// <param name="value">The value to rotate</param>
        /// <param name="count">The number of positions to rotate</param>
        /// <returns>The rotated value</returns>
        public static short RotateRight(short value, int count)
        {
            return (short)RotateRight((ushort)value, count);
        }

        /// <summary>
        /// Rotates the bits of an unsigned short value to the right
        /// </summary>
        /// <param name="value">The value to rotate</param>
        /// <param name="count">The number of positions to rotate</param>
        /// <returns>The rotated value</returns>
        public static ushort RotateRight(ushort value, int count)
        {
            if (count < 0)
            {
                return RotateLeft(value, -count);
            }
            count %= 16;
            return (ushort)((value >> count) | (value << 16 - count));
        }

        /// <summary>
        /// Rotates the bits of a signed int value to the left
        /// </summary>
        /// <param name="value">The value to rotate</param>
        /// <param name="count">The number of positions to rotate</param>
        /// <returns>The rotated value</returns>
        public static int RotateLeft(int value, int count)
        {
            return (int)RotateLeft((uint)value, count);
        }

        /// <summary>
        /// Rotates the bits of an unsigned int value to the left
        /// </summary>
        /// <param name="value">The value to rotate</param>
        /// <param name="count">The number of positions to rotate</param>
        /// <returns>The rotated value</returns>
        public static uint RotateLeft(uint value, int count)
        {
            return (value << count) | (value >> 32 - count);
        }

        /// <summary>
        /// Rotates the bits of a signed int value to the right
        /// </summary>
        /// <param name="value">The value to rotate</param>
        /// <param name="count">The number of positions to rotate</param>
        /// <returns>The rotated value</returns>
        public static int RotateRight(int value, int count)
        {
            return (int)RotateRight((uint)value, count);
        }

        /// <summary>
        /// Rotates the bits of an unsigned int value to the right
        /// </summary>
        /// <param name="value">The value to rotate</param>
        /// <param name="count">The number of positions to rotate</param>
        /// <returns>The rotated value</returns>
        public static uint RotateRight(uint value, int count)
        {
            return (value >> count) | (value << 32 - count);
        }

        /// <summary>
        /// Rotates the bits of a signed long value to the left
        /// </summary>
        /// <param name="value">The value to rotate</param>
        /// <param name="count">The number of positions to rotate</param>
        /// <returns>The rotated value</returns>
        public static long RotateLeft(long value, int count)
        {
            return (long)RotateLeft((ulong)value, count);
        }

        /// <summary>
        /// Rotates the bits of an unsigned long value to the left
        /// </summary>
        /// <param name="value">The value to rotate</param>
        /// <param name="count">The number of positions to rotate</param>
        /// <returns>The rotated value</returns>
        public static ulong RotateLeft(ulong value, int count)
        {
            return (value << count) | (value >> 64 - count);
        }

        /// <summary>
        /// Rotates the bits of a signed long value to the right
        /// </summary>
        /// <param name="value">The value to rotate</param>
        /// <param name="count">The number of positions to rotate</param>
        /// <returns>The rotated value</returns>
        public static long RotateRight(long value, int count)
        {
            return (long)RotateRight((ulong)value, count);
        }

        /// <summary>
        /// Rotates the bits of an unsigned long value to the right
        /// </summary>
        /// <param name="value">The value to rotate</param>
        /// <param name="count">The number of positions to rotate</param>
        /// <returns>The rotated value</returns>
        public static ulong RotateRight(ulong value, int count)
        {
            return (value >> count) | (value << 64 - count);
        }
    }
}
