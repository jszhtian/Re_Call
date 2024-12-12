using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Text;
using System.Globalization;

namespace AinDecompiler
{
    public class Token
    {
        public char QuoteCharacter;

        public bool IsQuoted
        {
            get
            {
                return QuoteCharacter != 0;
            }
        }


        public Token(string value)
            : this(value, (char)0)
        {


        }

        public Token(char character)  : this(character.ToString())
        {

        }

        public Token(string value, char quoteCharacter)
        {
            this.Value = value;
            this.QuoteCharacter = quoteCharacter;
        }

        public override string ToString()
        {
            if (QuoteCharacter != 0)
            {
                return QuoteCharacter.ToString() + Value + QuoteCharacter.ToString();
            }
            return Value;
        }

        public string Value;

        string asLowercase = null;
        public string AsLowercase
        {
            get
            {
                if (Object.ReferenceEquals(asLowercase, null))
                {
                    asLowercase = Value.ToLowerInvariant();// ToLower(Value);
                }
                return asLowercase;
            }
        }

        public static unsafe string ToLower(string value)
        {
            if (String.IsNullOrEmpty(value))
            {
                return value;
            }
            char[] chars = value.ToCharArray();
            int length = value.Length;
            fixed (char* p1 = chars)
            {
                char* p = (char*)p1;
                do
                {
                    if (*p >= 'A' && *p <= 'Z')
                    {
                        *p += (char)32;
                    }
                    p++;
                    length--;
                } while (length > 0);
            }
            //for (int i = 0; i < length; i++)
            //{
            //    if (chars[i] >= 'A' && chars[i] <= 'Z')
            //    {
            //        chars[i] = (char)(chars[i] + 32);
            //    }
            //}
            return new string(chars);
        }

        //public static bool operator ==(Token token, Token other)
        //{
        //    return Object.ReferenceEquals(token, other);
        //}

        //public static bool operator !=(Token token, Token other)
        //{
        //    return !(token == other);
        //}

        public static implicit operator string(Token token)
        {
            if (Object.ReferenceEquals(token, null))
            {
                return null;
            }
            return token.Value;
        }

        //public static bool operator ==(Token token, string stringValue)
        //{
        //    if (Object.ReferenceEquals(token, null))
        //    {
        //        if (stringValue == null)
        //        {
        //            return true;
        //        }
        //        else
        //        {
        //            return false;
        //        }
        //    }
        //    if (stringValue == null)
        //    {
        //        return false;
        //    }
        //    return token.Value.Equals(stringValue, StringComparison.InvariantCultureIgnoreCase);
        //}

        //public static bool operator !=(Token token, string stringValue)
        //{
        //    return !(token == stringValue);
        //}

        public static bool operator ==(Token token, char charValue)
        {
            return (token.QuoteCharacter == 0) && (token.Value.Length == 1) && (token.Value[0] == charValue);
        }

        public static bool operator !=(Token token, char charValue)
        {
            return !(token == charValue);
        }

        int result;
        int intValue;
        float floatValue;

        private unsafe void ParseNumber()
        {
            unchecked
            {
                fixed (char* ptr = this.Value)
                {
                    char* value = ptr;
                    bool isHex = false;
                    bool isNegative = false;

                    int length = this.Value.Length;

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

                        if (!(*value >= '0' && *value <= '9'))
                        {
                            return;
                        }
                    }

                    if (length <= 0)
                    {
                        return;
                    }

                    int intValue = 0;
                    if (isHex)
                    {
                        if (length > 8)
                        {
                            return;
                        }
                        do
                        {
                            int digit = *value - '0'; //subtract 0x30
                            //digit should be 0-9 (0x00 to 0x09), A-F (0x11 to 0x16), a-f (0x31 to 0x36)
                            if (!((digit >= 0 && digit <= 9) ||
                                (digit >= 0x11 && digit <= 0x16) ||
                                (digit >= 0x31 && digit <= 0x66)))
                            {
                                return;
                            }
                            if (digit > 9)
                            {
                                //'A' = 0x11, want to get 0x0A
                                digit -= 7;
                            }
                            if (digit > 16)
                            {
                                //'a' = 0x31, want to get 0x0A
                                //already subtracted 7, subtract 0x20 more
                                digit -= 0x20;
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
                            if (digit == -2)
                            {
                                goto TryFloat;
                            }
                            if (digit < 0 || digit > 9)
                            {
                                return;
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
                    this.intValue = intValue;
                    result = 1;
                    return;
                }
            }
        TryFloat:
            {
                double doubleValue;
                var numberStyle = NumberStyles.AllowLeadingWhite | NumberStyles.AllowTrailingWhite | NumberStyles.AllowLeadingSign | NumberStyles.AllowDecimalPoint | NumberStyles.AllowThousands | NumberStyles.AllowExponent;
                if (double.TryParse(this.Value, numberStyle, CultureInfo.InvariantCulture, out doubleValue))
                {
                    floatValue = (float)doubleValue;
                    result = 2;
                    return;
                }
                return;
            }
        }

        public unsafe static bool IsNumber(string thisValue)
        {
            unchecked
            {
                fixed (char* ptr = thisValue)
                {
                    char* value = ptr;
                    bool isHex = false;
                    bool isNegative = false;

                    int length = thisValue.Length;

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
                            if (!((digit >= 0 && digit <= 9) ||
                                (digit >= 0x11 && digit <= 0x16) ||
                                (digit >= 0x31 && digit <= 0x66)))
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
                            if (digit == -2)
                            {
                                goto TryFloat;
                            }
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
                    return true;
                }
            }
        TryFloat:
            double doubleValue;
            var numberStyle = NumberStyles.AllowLeadingWhite | NumberStyles.AllowTrailingWhite | NumberStyles.AllowLeadingSign | NumberStyles.AllowDecimalPoint | NumberStyles.AllowThousands | NumberStyles.AllowExponent;
            if (double.TryParse(thisValue, numberStyle, CultureInfo.InvariantCulture, out doubleValue))
            {
                return true;
            }
            return false;
        }

        public bool IsNumber()
        {
            if (result == 0)
            {
                ParseNumber();
                result++;
            }
            return (result > 1);
        }

        public bool IsInt()
        {
            if (result == 0)
            {
                ParseNumber();
                result++;
            }
            return (result == 2);
        }

        public bool IsFloat()
        {
            if (result == 0)
            {
                ParseNumber();
                result++;
            }
            return (result == 3);
        }

        public int ToInt()
        {
            if (result == 0)
            {
                ParseNumber();
                result++;
            }
            if (result == 2)
            {
                return intValue;
            }
            else
            {
                throw new FormatException("Cannot convert " + Value + " to an int.");
            }
        }

        public float ToFloat()
        {
            if (result == 0)
            {
                ParseNumber();
                result++;
            }
            if (result == 3)
            {
                return floatValue;
            }
            if (result == 2)
            {
                result = 3;
                floatValue = (float)(double.Parse(this.Value, CultureInfo.InvariantCulture));
                return floatValue;
            }
            else
            {
                throw new FormatException("Cannot convert " + Value + " to a float.");
            }
        }

        public Token Clone()
        {
            return (Token)MemberwiseClone();
        }
    }
}
