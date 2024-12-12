using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Text;

namespace AinDecompiler.Compiler
{
    class KeywordTable : Dictionary<string, TokenType>
    {
        public KeywordTable()
            : base()
        {
            this.Add("if", TokenType.If);
            this.Add("else", TokenType.Else);
            this.Add("while", TokenType.While);
            this.Add("for", TokenType.For);
            this.Add("goto", TokenType.Goto);
            this.Add("break", TokenType.Break);
            this.Add("continue", TokenType.Continue);
            this.Add("struct", TokenType.Struct);
            this.Add("class", TokenType.Class);
            this.Add("public", TokenType.Public);
            this.Add("private", TokenType.Private);
            this.Add("functype", TokenType.Functype);
            this.Add("delegate", TokenType.Delegate);
            this.Add("return", TokenType.Return);
            this.Add("switch", TokenType.Switch);
            this.Add("case", TokenType.Case);
            this.Add("new", TokenType.New);
            this.Add("jump", TokenType.Jump);
            this.Add("jumps", TokenType.Jumps);
            this.Add("const", TokenType.Const);
            this.Add("assert", TokenType.Assert);

            this.Add("void", TokenType.Void);
            this.Add("int", TokenType.Int);
            this.Add("bool", TokenType.Bool);
            this.Add("string", TokenType.String);
            this.Add("float", TokenType.Float);
            this.Add("lint", TokenType.Lint);
            this.Add("IMainSystem", TokenType.IMainSystem);

            this.Add("array", TokenType.Array);
            this.Add("ref", TokenType.Ref);

            this.Add("this", TokenType.This);

            this.Add("__FILE__", TokenType.FileMacro);
            this.Add("__LINE__", TokenType.LineMacro);
            this.Add("__FUNC__", TokenType.FuncMacro);
            this.Add("__DATE__", TokenType.DateMacro);
            this.Add("__TIME__", TokenType.TimeMacro);

            //this.Add("Numof", TokenType.Numof);
            //this.Add("Alloc", TokenType.Alloc);
            //this.Add("Realloc", TokenType.Realloc);
            //this.Add("Free", TokenType.Free);
            //this.Add("Copy", TokenType.Copy);
            //this.Add("PushBack", TokenType.PushBack);
            //this.Add("PopBack", TokenType.PopBack);
            //this.Add("LengthByte", TokenType.LengthByte);
            //this.Add("Empty", TokenType.Empty);
            //this.Add("Length", TokenType.Length);
            //this.Add("Find", TokenType.Find);
            //this.Add("GetPart", TokenType.GetPart);
            //this.Add("Insert", TokenType.Insert);
            //this.Add("Fill", TokenType.Fill);
            //this.Add("Sort", TokenType.Sort);
            //this.Add("Reverse", TokenType.Reverse);
            //this.Add("Erase", TokenType.Erase);
            //this.Add("String", TokenType.ConvertToString);
            //this.Add("Int", TokenType.ConvertToInt);

            this.Add("~", TokenType.Complement);
            this.Add("!", TokenType.Not);
            this.Add("@", TokenType.At);
            this.Add("#", TokenType.Pound);
            this.Add("$", TokenType.DollarSign);
            this.Add("$$", TokenType.DoubleDollarSign);
            this.Add("%", TokenType.Modulo);
            this.Add("^", TokenType.Xor);
            this.Add("&", TokenType.And);
            this.Add("*", TokenType.Multiply);
            this.Add("(", TokenType.OpenParenthesis);
            this.Add(")", TokenType.ClosedParenthesis);
            this.Add("-", TokenType.Minus);
            this.Add("+", TokenType.Plus);
            this.Add("=", TokenType.Assign);
            this.Add("[", TokenType.OpenBracket);
            this.Add("]", TokenType.CloseBracket);
            this.Add("|", TokenType.Or);
            this.Add("{", TokenType.OpenBrace);
            this.Add("}", TokenType.CloseBrace);
            this.Add(":", TokenType.Colon);
            this.Add(";", TokenType.Semicolon);
            this.Add("<", TokenType.LessThan);
            this.Add(">", TokenType.GreaterThan);
            this.Add("?", TokenType.QuestionMark);
            this.Add(",", TokenType.Comma);
            this.Add(".", TokenType.Dot);
            this.Add("/", TokenType.Divide);
            this.Add("<<", TokenType.LeftShift);
            this.Add(">>", TokenType.RightShift);
            this.Add("::", TokenType.ScopeResolution);
            this.Add("==", TokenType.EqualTo);
            this.Add("!=", TokenType.NotEqualTo);
            this.Add(">=", TokenType.GreaterThanOrEqualTo);
            this.Add("<=", TokenType.LessThanOrEqualTo);
            this.Add("%=", TokenType.ModuloAssign);
            this.Add("&=", TokenType.AndAssign);
            this.Add("*=", TokenType.MultiplyAssign);
            this.Add("+=", TokenType.PlusAssign);
            this.Add("-=", TokenType.MinusAssign);
            this.Add("/=", TokenType.DivideAssign);
            this.Add("^=", TokenType.XorAssign);
            this.Add("|=", TokenType.OrAssign);
            this.Add("<<=", TokenType.LeftShiftAssign);
            this.Add(">>=", TokenType.RightShiftAssign);
            this.Add("<-", TokenType.ReferenceAssign);
            this.Add("===", TokenType.ReferenceEqualTo);
            this.Add("!==", TokenType.ReferenceNotEqualTo);
            this.Add("<=>", TokenType.ReferenceSwap);
            this.Add("++", TokenType.Increment);
            this.Add("--", TokenType.Decrement);
            this.Add("&&", TokenType.LogicalAnd);
            this.Add("||", TokenType.LogicalOr);

        }
    }
}
