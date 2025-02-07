﻿using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;

namespace Majiro.Script {
	public sealed class Opcode {
		public Opcode(ushort value, string mnemonic, string @operator, string encoding, string transition, string[] aliases) {
			Value = value;
			Mnemonic = mnemonic;
			Operator = @operator;
			Encoding = encoding;
			Transition = transition;
			Aliases = aliases;
		}

		// general information
		public readonly ushort Value;
		public readonly string Mnemonic;
		public readonly string Operator;
		public readonly string[] Aliases;

		// instruction encoding
		public readonly string Encoding;

		// stack transition
		public readonly string Transition;

		public bool IsJump => Encoding == "j";

		public static readonly IReadOnlyCollection<Opcode> List = Init();
		public static readonly IReadOnlyDictionary<ushort, Opcode> ByValue = new ReadOnlyDictionary<ushort, Opcode>(List.ToDictionary(op => op.Value));
		public static readonly IReadOnlyDictionary<string, Opcode> ByMnemonic = new ReadOnlyDictionary<string, Opcode>(List
			.SelectMany(op => op.Aliases
				.Select(alias => (mnemonic: alias, opcode: op))
				.Prepend((mnemonic: op.Mnemonic, opcode: op)))
			.ToDictionary(pair => pair.mnemonic, pair => pair.opcode));

		public override string ToString() => Mnemonic;

		private static IReadOnlyCollection<Opcode> Init() {
			var list = new List<Opcode>();

			void DefineOpcode(int value, string mnemonic, string op, string encoding, string transition, params string[] aliases) {
				list.Add(new Opcode(checked((ushort)value), mnemonic, op, encoding, transition, aliases));
			}

			void DefineBinaryOperator(ushort baseValue, string mnemonic, string op, MjoTypeMask allowedTypes, bool isComparison, params string[] aliases) {

				if(allowedTypes.HasFlag(MjoTypeMask.Int))
					if(allowedTypes == MjoTypeMask.Int)
						DefineOpcode(baseValue, mnemonic, op, "", isComparison ? "ii.b" : "ii.i", aliases.SelectMany(a => new[] { a, a + ".i" }).Prepend(mnemonic + ".i").ToArray());
					else
						DefineOpcode(baseValue, mnemonic + ".i", op, "", isComparison ? "ii.b" : "ii.i", aliases.SelectMany(a => new[] { a + ".i", a }).Prepend(mnemonic).ToArray());

				if(allowedTypes.HasFlag(MjoTypeMask.Float))
					DefineOpcode(baseValue + 1, mnemonic + ".r", op, "", isComparison ? "nn.b" : "nn.f", aliases.Select(a => a + ".r").ToArray());

				if(allowedTypes.HasFlag(MjoTypeMask.String))
					DefineOpcode(baseValue + 2, mnemonic + ".s", op, "", isComparison ? "ss.b" : "ss.s", aliases.Select(a => a + ".s").ToArray());

				if(allowedTypes.HasFlag(MjoTypeMask.IntArray))
					DefineOpcode(baseValue + 3, mnemonic + ".iarr", op, "", isComparison ? "II.b" : "-", aliases.Select(a => a + ".iarr").ToArray());

				if(allowedTypes.HasFlag(MjoTypeMask.FloatArray))
					DefineOpcode(baseValue + 4, mnemonic + ".rarr", op, "", isComparison ? "FF.b" : "-", aliases.Select(a => a + ".rarr").ToArray());

				if(allowedTypes.HasFlag(MjoTypeMask.StringArray))
					DefineOpcode(baseValue + 5, mnemonic + ".sarr", op, "", isComparison ? "SS.b" : "-", aliases.Select(a => a + ".sarr").ToArray());
			}

			void DefineAssignmentOperator(ushort baseValue, string mnemonic, string op, MjoTypeMask allowedTypes, bool pop, params string[] aliases) {

				if(allowedTypes.HasFlag(MjoTypeMask.Int))
					if(allowedTypes == MjoTypeMask.Int)
						DefineOpcode(baseValue, mnemonic, op, "fho", pop ? "i." : "i.i", aliases.SelectMany(a => new[] { a, a + ".i" }).Prepend(mnemonic + ".i").ToArray());
					else
						DefineOpcode(baseValue, mnemonic + ".i", op, "fho", pop ? "i." : "i.i", aliases.SelectMany(a => new[] { a + ".i", a }).Prepend(mnemonic).ToArray());

				if(allowedTypes.HasFlag(MjoTypeMask.Float))
					DefineOpcode(baseValue + 1, mnemonic + ".r", op, "fho", pop ? "n." : "n.f", aliases.Select(a => a + ".r").ToArray());

				if(allowedTypes.HasFlag(MjoTypeMask.String))
					DefineOpcode(baseValue + 2, mnemonic + ".s", op, "fho", pop ? "s." : "s.s", aliases.Select(a => a + ".s").ToArray());

				if(allowedTypes.HasFlag(MjoTypeMask.IntArray))
					DefineOpcode(baseValue + 3, mnemonic + ".iarr", op, "fho", pop ? "I." : "I.I", aliases.Select(a => a + ".iarr").ToArray());

				if(allowedTypes.HasFlag(MjoTypeMask.FloatArray))
					DefineOpcode(baseValue + 4, mnemonic + ".rarr", op, "fho", pop ? "F." : "F.F", aliases.Select(a => a + ".rarr").ToArray());

				if(allowedTypes.HasFlag(MjoTypeMask.StringArray))
					DefineOpcode(baseValue + 5, mnemonic + ".sarr", op, "fho", pop ? "S." : "S.S", aliases.Select(a => a + ".sarr").ToArray());
			}

			void DefineArrayAssignmentOperator(ushort baseValue, string mnemonic, string op, MjoTypeMask allowedTypes, bool pop, params string[] aliases) {

				if(allowedTypes.HasFlag(MjoTypeMask.Int))
					if(allowedTypes == MjoTypeMask.Int)
						DefineOpcode(baseValue, mnemonic, op, "fho", pop ? "i[i#d]." : "i[i#d].i", aliases.SelectMany(a => new[] { a, a + ".i" }).Prepend(mnemonic + ".i").ToArray());
					else
						DefineOpcode(baseValue, mnemonic + ".i", op, "fho", pop ? "i[i#d]." : "i[i#d].i", aliases.SelectMany(a => new[] { a + ".i", a }).Prepend(mnemonic).ToArray());

				if(allowedTypes.HasFlag(MjoTypeMask.Float))
					DefineOpcode(baseValue + 1, mnemonic + ".r", op, "fho", pop ? "n[i#d]." : "n[i#d].f", aliases.Select(a => a + ".r").ToArray());

				if(allowedTypes.HasFlag(MjoTypeMask.String))
					DefineOpcode(baseValue + 2, mnemonic + ".s", op, "fho", pop ? "s[i#d]." : "s[i#d].s", aliases.Select(a => a + ".s").ToArray());
			}
			
#pragma warning disable format

			{	// binary operators
				DefineBinaryOperator(0x100, "mul",  "*",  MjoTypeMask.Numeric, false);
				DefineBinaryOperator(0x108, "div",  "/",  MjoTypeMask.Numeric, false);
				DefineBinaryOperator(0x110, "rem",  "%",  MjoTypeMask.Int, false, "mod");
				DefineBinaryOperator(0x118, "add",  "+",  MjoTypeMask.Primitive, false);
				DefineBinaryOperator(0x120, "sub",  "-",  MjoTypeMask.Numeric, false);
				DefineBinaryOperator(0x128, "shr",  ">>", MjoTypeMask.Int, false);
				DefineBinaryOperator(0x130, "shl",  "<<", MjoTypeMask.Int, false);
				DefineBinaryOperator(0x138, "cle",  "<=", MjoTypeMask.Primitive, true);
				DefineBinaryOperator(0x140, "clt",  "<",  MjoTypeMask.Primitive, true);
				DefineBinaryOperator(0x148, "cge",  ">=", MjoTypeMask.Primitive, true);
				DefineBinaryOperator(0x150, "cgt",  ">",  MjoTypeMask.Primitive, true);
				DefineBinaryOperator(0x158, "ceq",  "==", MjoTypeMask.All, true);
				DefineBinaryOperator(0x160, "cne",  "!=", MjoTypeMask.All, true);
				DefineBinaryOperator(0x168, "xor",  "^",  MjoTypeMask.Int, false);
				DefineBinaryOperator(0x170, "andl", "&&", MjoTypeMask.Int, false);
				DefineBinaryOperator(0x178, "orl",  "||", MjoTypeMask.Int, false);
				DefineBinaryOperator(0x180, "and",  "&",  MjoTypeMask.Int, false);
				DefineBinaryOperator(0x188, "or",   "|",  MjoTypeMask.Int, false);
			}

			{	// unary operators / nops
				DefineOpcode(0x190, "notl",  "!", "", "i.i", "notl.i");
				DefineOpcode(0x198, "not",   "~", "", "i.i", "not.i");
				DefineOpcode(0x1a0, "neg.i", "-", "", "i.i");
				DefineOpcode(0x1a1, "neg.r", "-", "", "f.f");
				
				DefineOpcode(0x191, "nop.191", null, "", "");
				DefineOpcode(0x1a8, "nop.1a8", null, "", "");
				DefineOpcode(0x1a9, "nop.1a9", null, "", "");
			}

			{	// assignment operators
				DefineAssignmentOperator(0x1b0, "st",     "=",   MjoTypeMask.All, false);
				DefineAssignmentOperator(0x1b8, "st.mul", "*=",  MjoTypeMask.Numeric, false);
				DefineAssignmentOperator(0x1c0, "st.div", "/=",  MjoTypeMask.Numeric, false);
				DefineAssignmentOperator(0x1c8, "st.rem", "%=",  MjoTypeMask.Int, false, "st.mod");
				DefineAssignmentOperator(0x1d0, "st.add", "+=",  MjoTypeMask.Primitive, false);
				DefineAssignmentOperator(0x1d8, "st.sub", "-=",  MjoTypeMask.Numeric, false);
				DefineAssignmentOperator(0x1e0, "st.shl", "<<=", MjoTypeMask.Int, false);
				DefineAssignmentOperator(0x1e8, "st.shr", ">>=", MjoTypeMask.Int, false);
				DefineAssignmentOperator(0x1f0, "st.and", "&=",  MjoTypeMask.Int, false);
				DefineAssignmentOperator(0x1f8, "st.xor", "^=",  MjoTypeMask.Int, false);
				DefineAssignmentOperator(0x200, "st.or",  "|=",  MjoTypeMask.Int, false);

				DefineAssignmentOperator(0x210, "stp",     "=",   MjoTypeMask.All, true);
				DefineAssignmentOperator(0x218, "stp.mul", "*=",  MjoTypeMask.Numeric, true);
				DefineAssignmentOperator(0x220, "stp.div", "/=",  MjoTypeMask.Numeric, true);
				DefineAssignmentOperator(0x228, "stp.rem", "%=",  MjoTypeMask.Int, true, "stp.mod");
				DefineAssignmentOperator(0x230, "stp.add", "+=",  MjoTypeMask.Primitive, true);
				DefineAssignmentOperator(0x238, "stp.sub", "-=",  MjoTypeMask.Numeric, true);
				DefineAssignmentOperator(0x240, "stp.shl", "<<=", MjoTypeMask.Int, true);
				DefineAssignmentOperator(0x248, "stp.shr", ">>=", MjoTypeMask.Int, true);
				DefineAssignmentOperator(0x250, "stp.and", "&=",  MjoTypeMask.Int, true);
				DefineAssignmentOperator(0x258, "stp.xor", "^=",  MjoTypeMask.Int, true);
				DefineAssignmentOperator(0x260, "stp.or",  "|=",  MjoTypeMask.Int, true);
			}

			{	// array assignment operators
				DefineArrayAssignmentOperator(0x270, "stelem",     "=",   MjoTypeMask.Primitive, false);
				DefineArrayAssignmentOperator(0x278, "stelem.mul", "*=",  MjoTypeMask.Numeric, false);
				DefineArrayAssignmentOperator(0x280, "stelem.div", "/=",  MjoTypeMask.Numeric, false);
				DefineArrayAssignmentOperator(0x288, "stelem.rem", "%=", MjoTypeMask.Int, false, "stelem.mod");
				DefineArrayAssignmentOperator(0x290, "stelem.add", "+=",  MjoTypeMask.Primitive, false);
				DefineArrayAssignmentOperator(0x298, "stelem.sub", "-=",  MjoTypeMask.Numeric, false);
				DefineArrayAssignmentOperator(0x2a0, "stelem.shl", "<<=", MjoTypeMask.Int, false);
				DefineArrayAssignmentOperator(0x2a8, "stelem.shr", ">>=", MjoTypeMask.Int, false);
				DefineArrayAssignmentOperator(0x2b0, "stelem.and", "&=",  MjoTypeMask.Int, false);
				DefineArrayAssignmentOperator(0x2b8, "stelem.xor", "^=",  MjoTypeMask.Int, false);
				DefineArrayAssignmentOperator(0x2c0, "stelem.or",  "|=",  MjoTypeMask.Int, false);
				
				DefineArrayAssignmentOperator(0x2d0, "stelemp",     "=",   MjoTypeMask.Primitive, true);
				DefineArrayAssignmentOperator(0x2d8, "stelemp.mul", "*=",  MjoTypeMask.Numeric, true);
				DefineArrayAssignmentOperator(0x2e0, "stelemp.div", "/=",  MjoTypeMask.Numeric, true);
				DefineArrayAssignmentOperator(0x2e8, "stelemp.rem", "%=",  MjoTypeMask.Int, true, "stelemp.mod");
				DefineArrayAssignmentOperator(0x2f0, "stelemp.add", "+=",  MjoTypeMask.Primitive, true);
				DefineArrayAssignmentOperator(0x2f8, "stelemp.sub", "-=",  MjoTypeMask.Numeric, true);
				DefineArrayAssignmentOperator(0x300, "stelemp.shl", "<<=", MjoTypeMask.Int, true);
				DefineArrayAssignmentOperator(0x308, "stelemp.shr", ">>=", MjoTypeMask.Int, true);
				DefineArrayAssignmentOperator(0x310, "stelemp.and", "&=",  MjoTypeMask.Int, true);
				DefineArrayAssignmentOperator(0x318, "stelemp.xor", "^=",  MjoTypeMask.Int, true);
				DefineArrayAssignmentOperator(0x320, "stelemp.or",  "|=",  MjoTypeMask.Int, true);
			}

			{	// 0800 range opcodes
				DefineOpcode(0x800, "ldc.i", null, "i", ".i");
				DefineOpcode(0x801, "ldstr", null, "s", ".s", "ldc.s");
				DefineOpcode(0x802, "ld", null, "fho", ".#t", "ldvar");
				DefineOpcode(0x803, "ldc.r", null, "r", ".f");

				DefineOpcode(0x80f, "call",  null, "h0a", "[*#a].*");
				DefineOpcode(0x810, "callp", null, "h0a", "[*#a].");

				DefineOpcode(0x829, "alloca", null, "t", ".[#t]");
				DefineOpcode(0x82b, "ret", null, "", "[*].", "return");

				DefineOpcode(0x82c, "br", null, "j", ".", "jmp");
				DefineOpcode(0x82d, "brtrue", null, "j", "p.", "brinst", "jnz", "jne");
				DefineOpcode(0x82e, "brfalse", null, "j", "p.", "brnull", "brzero", "jz", "je");

				DefineOpcode(0x82f, "pop", null, "", "*.");
				
				DefineOpcode(0x830, "br.case", null, "j", "p.", "br.v", "jmp.v");
				DefineOpcode(0x831, "bne.case", null, "j", "p.", "bne.v", "jne.v");
				DefineOpcode(0x832, "bge.case", null, "j", "p.", "bge.v", "jge.v");
				DefineOpcode(0x833, "ble.case", null, "j", "p.", "ble.v", "jle.v");
				DefineOpcode(0x838, "blt.case", null, "j", "p.", "blt.v", "jlt.v");
				DefineOpcode(0x839, "bgt.case", null, "j", "p.", "bgt.v", "jgt.v");
				
				DefineOpcode(0x834, "syscall",  null, "ha", "[*#a].*");
				DefineOpcode(0x835, "syscallp", null, "ha", "[*#a].");
				
				DefineOpcode(0x836, "argcheck", null, "t", ".[#t]", "sigchk");
				
				DefineOpcode(0x837, "ldelem", null, "fho", "[i#d].#t");

				DefineOpcode(0x83a, "line", null, "l", ".");
				
				DefineOpcode(0x83b, "bsel.1", null, "j", ".");
				DefineOpcode(0x83c, "bsel.3", null, "j", ".");
				DefineOpcode(0x83d, "bsel.2", null, "j", ".");
				
				DefineOpcode(0x83e, "conv.i", null, "", "f.i");
				DefineOpcode(0x83f, "conv.r", null, "", "i.f");
				
				DefineOpcode(0x840, "text", null, "s", ".");
				DefineOpcode(0x841, "proc", null, "", ".");
				DefineOpcode(0x842, "ctrl", null, "s", "[#s].");
				DefineOpcode(0x843, "bsel.x", null, "j", ".");
				DefineOpcode(0x844, "bsel.clr", null, "", ".");
				DefineOpcode(0x845, "bsel.4", null, "j", ".");
				DefineOpcode(0x846, "bsel.jmp.4", null, "", ".");
				DefineOpcode(0x847, "bsel.5", null, "j", ".");

				DefineOpcode(0x850, "switch", null, "c", "i.");
			}

#pragma warning restore format

			return new ReadOnlyCollection<Opcode>(list);
		}
	}
}
