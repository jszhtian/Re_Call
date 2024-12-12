using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;

namespace GSIWin_Script_Tool
{
    using TString = Tuple<int, string>;

    class Script
    {
        public Script()
        {
        }

        public void Load(string filePath)
        {
            using (var stream = File.OpenRead(filePath))
            using (var reader = new BinaryReader(stream))
            {
                Read(reader);
            }
        }

        static readonly Encoding _encoding = Encoding.GetEncoding("shift_jis");

        readonly List<Opcode> Opcodes = new List<Opcode>();

        void Read(BinaryReader reader)
        {
            var v1 = reader.ReadInt32();
            var v2 = reader.ReadInt32();

            var offset_table_0 = new HashSet<uint>();
            var offset_table_1 = new HashSet<uint>();

            for (int i = 0; i < v1; i++)
            {
                offset_table_0.Add(reader.ReadUInt32());
            }

            for (int i = 0; i < v2; i++)
            {
                offset_table_1.Add(reader.ReadUInt32());
            }

            Opcodes.Clear();

            long codebase = reader.BaseStream.Position;

            while (reader.BaseStream.Position < reader.BaseStream.Length)
            {
                uint addr = Convert.ToUInt32(reader.BaseStream.Position - codebase);
                byte code = reader.ReadByte();

                switch (code)
                {
                    case 0x00: // THROW
                    case 0x01: // THROW
                    case 0x02: // LOAD
                    case 0x03: // LOAD
                    case 0x04: // LOAD
                    case 0x05: // LOAD
                    case 0x06: // LOAD
                    case 0x07: // LOAD
                    case 0x08: // LOAD
                    case 0x09: // LOAD
                    case 0x0C: // STORE
                    case 0x0D: // STORE
                    case 0x0E: // STORE
                    case 0x0F: // STORE
                    case 0x10: // STORE
                    case 0x11: // STORE
                    case 0x12: // STORE
                    case 0x13: // STORE
                    case 0x17:
                    case 0x18:
                    case 0x34: // ADD
                    case 0x35: // SUB
                    case 0x36: // MUL
                    case 0x37: // DIV
                    case 0x38: // MOD
                    case 0x39: // RANDOM
                    case 0x3A: // LAND
                    case 0x3B: // LOR
                    case 0x3C: // AND
                    case 0x3D: // OR
                    case 0x3E: // LT
                    case 0x3F: // GT
                    case 0x40: // LE
                    case 0x41: // GE
                    case 0x42: // EQ
                    case 0x43: // NE
                    {
                        Opcodes.Add(new Opcode(addr, code));
                        break;
                    }
                    case 0x0A: // STR
                    case 0x0B: // STR
                    case 0x33: // PUSH STR
                    {
                        var args = reader.ReadCString(true);
                        Opcodes.Add(new Opcode(addr, code, args));
                        break;
                    }
                    case 0x14: // JZ DWORD [FIX]
                    case 0x15: // JMP DWORD [FIX]
                    case 0x19:
                    case 0x1A:
                    case 0x1B: // JMP DWORD [FIX]
                    case 0x32: // PUSH DWORD
                    {
                        if (code == 0x19)
                        {
                            if (!offset_table_0.Contains(addr))
                            {
                                throw new Exception("Address not found in table.");
                            }
                        }

                        if (code == 0x1A)
                        {
                            if (!offset_table_1.Contains(addr + 1 + 4))
                            {
                                throw new Exception("Address not found in table.");
                            }
                        }

                        var args = reader.ReadBytes(4);
                        Opcodes.Add(new Opcode(addr, code, args));
                        break;
                    }
                    case 0x1C:
                    {
                        var args = reader.ReadBytes(1);
                        Opcodes.Add(new Opcode(addr, code, args));
                        break;
                    }
                    default:
                    {
                        throw new Exception($"Unknown Opcode {code:X2}");
                    }
                }
            }
        }

        public void Save(string filePath)
        {
            // Build code section

            var offset_table_0 = new List<uint>();
            var offset_table_1 = new List<uint>();

            byte[] section_code;

            using (var stream = new MemoryStream(4096))
            using (var writer = new BinaryWriter(stream))
            {
                // Build code

                foreach (var item in Opcodes)
                {
                    uint addr = Convert.ToUInt32(stream.Position);
                    
                    // Update opcode address
                    item.NewAddr = addr;

                    // Write code
                    writer.Write(item.Code);

                    // Write argument
                    if (item.Args != null)
                    {
                        writer.Write(item.Args);
                    }
                }

                // Update address

                var opcodeMap = Opcodes.ToDictionary(a => a.Addr, b => b.NewAddr);

                foreach (var item in Opcodes)
                {
                    switch (item.Code)
                    {
                        case 0x14:
                        case 0x15:
                        case 0x1B:
                        {
                            // Get jump address
                            var addr = BitConverter.ToUInt32(item.Args, 0);
                            addr = Endian.Reverse(addr);

                            // Find jump target
                            if (!opcodeMap.TryGetValue(addr, out addr))
                                throw new Exception("Jump target not found.");

                            // Update address
                            addr = Endian.Reverse(addr);

                            // Write new target address
                            stream.Position = item.NewAddr + 1;
                            writer.Write(addr);

                            break;
                        }
                        case 0x19:
                        {
                            offset_table_0.Add(item.NewAddr);
                            break;
                        }
                        case 0x1A:
                        {
                            offset_table_1.Add(item.NewAddr + 1 + 4);
                            break;
                        }
                    }
                }

                writer.Flush();

                section_code = stream.ToArray();
            }

            // Write script file

            using (var stream = File.Create(filePath))
            using (var writer = new BinaryWriter(stream))
            {
                // Write table size
                writer.Write(offset_table_0.Count);
                writer.Write(offset_table_1.Count);

                // Write table 0
                foreach (var item in offset_table_0)
                {
                    writer.Write(item);
                }

                // Write table 1
                foreach (var item in offset_table_1)
                {
                    writer.Write(item);
                }

                // Write code
                writer.Write(section_code);

                // Finished
                writer.Flush();
            }
        }

        class Opcode
        {
            public uint Addr;
            public uint NewAddr;
            public byte Code;
            public byte[] Args;

            public Opcode(uint addr, byte code)
            {
                Addr = addr;
                Code = code;
                Args = null;
            }

            public Opcode(uint addr, byte code, byte[] args)
            {
                Addr = addr;
                Code = code;
                Args = args;
            }
        }

        IList<TString> GetStrings()
        {
            var list = new List<TString>();

            for (int i = 0; i < Opcodes.Count; i++)
            {
                var opcode = Opcodes[i];

                switch (opcode.Code)
                {
                    case 0x0A:
                    {
                        var str = ReadCompressedString(opcode.Args);
                        list.Add(new TString(i, str));
                        break;
                    }
                    case 0x0B:
                    case 0x33:
                    {
                        var str = ReadCString(opcode.Args);
                        list.Add(new TString(i, str));
                        break;
                    }
                }
            }

            return list;
        }

        public void ExportStrings(string filePath, bool exportAll)
        {
            var strings = GetStrings();

            using (var writer = File.CreateText(filePath))
            {
                foreach (var item in strings)
                {
                    if (!exportAll)
                    {
                        if (item.Item2.Length > 0 && item.Item2[0] < 0x81 && item.Item2[0] != '\n' && item.Item2[0] != '\r' && item.Item2[0] != '\t')
                        {
                            // Ignore file name, etc.
                            continue;
                        }
                    }

                    var str = EscapeString(item.Item2);

                    writer.WriteLine($"◇{item.Item1:X8}◇{str}");
                    writer.WriteLine($"◆{item.Item1:X8}◆{str}");
                    writer.WriteLine();
                }
            }
        }

        public void ImportStrings(string filePath)
        {
            var translated = new List<TString>();

            // Read translation file
            using (StreamReader reader = File.OpenText(filePath))
            {
                int lineNo = 0;

                while (!reader.EndOfStream)
                {
                    int ln = lineNo;
                    var line = reader.ReadLine();
                    lineNo++;

                    if (line.Length == 0 || line[0] != '◆')
                    {
                        continue;
                    }

                    translated.Add(new TString(ln, line));
                }
            }

            // Convert to dictionary for fast match
            var strings = GetStrings().ToDictionary(a => a.Item1, b => b.Item2);

            // Import translated string
            for (int i = 0; i < translated.Count; i++)
            {
                // Parse line
                var line = translated[i].Item2;
                var m = Regex.Match(line, @"◆(\w+)◆(.+$)");

                // Check match result
                if (!m.Success || m.Groups.Count != 3)
                {
                    throw new Exception($"Bad format at line: {translated[i].Item1}");
                }

                // index of opcode
                int index = int.Parse(m.Groups[1].Value, NumberStyles.HexNumber);
                // translated string
                var str = UnespaceString(m.Groups[2].Value);

                // Check index valid
                if (!strings.ContainsKey(index))
                {
                    throw new Exception($"The index {index:X8} is not contained in the script.");
                }

                // Update string
                strings[index] = str;
            }

            // Update opcodes

            var translated_encoding = Encoding.GetEncoding("gbk");

            foreach (var item in strings)
            {
                var opcode = Opcodes[item.Key];

                Debug.Assert(opcode.Code == 0x0A || opcode.Code == 0x0B || opcode.Code == 0x33);

                var bytes = translated_encoding.GetBytes(item.Value);
                // String null terminated
                var args = new byte[bytes.Length + 1];
                Buffer.BlockCopy(bytes, 0, args, 0, bytes.Length);

                opcode.Args = args;
            }
        }

        static string ReadCompressedString(byte[] buffer)
        {
            var bytes = new List<byte>(256);

            int i = 0;

            while (i < buffer.Length)
            {
                byte c = buffer[i++];

                if (c == 0)
                {
                    break;
                }
                if (c >= 0x81 && c <= 0x9F || (c + 0x20) <= 0xF)
                {
                    bytes.Add(c);
                    c = buffer[i++];
                    bytes.Add(c);
                }
                else
                {
                    // Unpack
                    var v0 = (ushort)(c - 0x7D62);
                    var v1 = (byte)(v0 >> 8);
                    var v2 = (byte)(v0 & 0xFF);
                    bytes.Add(v1);
                    bytes.Add(v2);
                }
            }

            if (bytes.Count == 0)
            {
                return string.Empty;
            }

            return _encoding.GetString(bytes.ToArray());
        }

        static string ReadCString(byte[] buffer)
        {
            var bytes = buffer.TakeWhile(c => c != 0).ToArray();
            return _encoding.GetString(bytes);
        }

        static string EscapeString(string input)
        {
            input = input.Replace("\r", "\\r");
            input = input.Replace("\n", "\\n");
            input = input.Replace("\t", "\\t");

            return input;
        }

        static string UnespaceString(string input)
        {
            input = input.Replace("\\r", "\r");
            input = input.Replace("\\n", "\n");
            input = input.Replace("\\t", "\t");
            input = input.Replace("\\0", "\0");
            input = input.Replace("\\x1C", "\x1C");

            return input;
        }
    }
}
