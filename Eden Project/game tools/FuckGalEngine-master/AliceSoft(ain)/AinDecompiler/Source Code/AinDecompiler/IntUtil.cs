using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Text;

namespace AinDecompiler
{
    public static class IntUtil
    {
        public static unsafe bool TryParse(string stringValue, out int outputIntValue)
        {
            outputIntValue = 0;
            unchecked
            {
                fixed (char* ptr = stringValue)
                {
                    char* value = ptr;
                    bool isHex = false;
                    bool isNegative = false;

                    int length = stringValue.Length;

                    if (*value == '-')
                    {
                        isNegative = true;
                        value++;
                        length--;
                    }

                    if (length >= 2 && *value == '0' && value[1] == 'x')
                    {
                        value += 2;
                        length -= 2;
                        isHex = true;
                    }

                    if (value[length - 1] == 'h')
                    {
                        length--;
                        isHex = true;

                        if (*value >= '0' && *value <= '9')
                        {
                            return false;
                        }
                    }

                    if (length <= 0)
                    {
                        return false;
                    }

                    int intValue = 0;
                    if (isHex)
                    {
                        if (length > 8)
                        {
                            return false;
                        }
                        do
                        {
                            int digit = *value - '0';
                            if (digit < 0 || (digit > 9 && digit < 17) || digit > 22)
                            {
                                return false;
                            }
                            if (digit > 9)
                            {
                                digit -= 7;
                            }
                            intValue <<= 4;
                            intValue += digit;
                            value++;
                            length--;
                        }
                        while (length > 0);
                    }
                    else
                    {
                        //if (length > 10)
                        //{
                        //    return;
                        //}
                        do
                        {
                            int digit = *value - '0';
                            //if (digit == -2)
                            //{
                            //    goto TryFloat;
                            //}
                            if (digit < 0 || digit > 9)
                            {
                                return false;
                            }
                            intValue *= 10;
                            intValue += digit;
                            value++;
                            length--;
                        }
                        while (length > 0);
                    }
                    if (isNegative)
                    {
                        intValue = -intValue;
                    }
                    outputIntValue = intValue;
                    return true;
                }
            }
        //TryFloat:
        //    {
        //        double doubleValue;
        //        var numberStyle = NumberStyles.AllowLeadingWhite | NumberStyles.AllowTrailingWhite | NumberStyles.AllowLeadingSign | NumberStyles.AllowDecimalPoint | NumberStyles.AllowThousands | NumberStyles.AllowExponent;
        //        if (double.TryParse(this.Value, numberStyle, CultureInfo.InvariantCulture, out doubleValue))
        //        {
        //            floatValue = (float)doubleValue;
        //            result = 2;
        //            return;
        //        }
        //        return;
        //    }
        }
    }
}
