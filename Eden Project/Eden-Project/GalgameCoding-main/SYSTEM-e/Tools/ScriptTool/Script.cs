using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;

namespace ScriptTool
{
    class Script
    {
        private List<byte[]> Commands;

        public Script()
        {
        }

        public void Load(string filePath)
        {
            Commands = new List<byte[]>();

            Encoding encoding = Encoding.GetEncoding("shift-jis");

            using (Stream stream = File.OpenRead(filePath))
            using (BinaryReader reader = new BinaryReader(stream))
            {
                while (stream.Position < stream.Length)
                {
                    long head = stream.Position;

                    byte code = reader.ReadByte();
                    byte par1 = reader.ReadByte();
                    byte par2 = reader.ReadByte();
                    byte par3 = reader.ReadByte();

                    switch (code)
                    {
                        default:
                        {
                            throw new Exception($"UNKNOWN OPCODE {code:X2}");
                        }

                        case 0x00:
                        {
                            //string text = reader.ReadNullTerminatedString(encoding);
                            //Debug.WriteLine(text);

                            stream.Position = head + par1 + par3;
                            break;
                        }
                        case 0x01:
                        {
                            stream.Position = head + par1;
                            break;
                        }
                        case 0x02:
                        {
                            //string str = encoding.GetString(reader.ReadBytes(par3));
                            //Debug.WriteLine($"0x02 {str}");

                            stream.Position = head + par1 + par3;
                            break;
                        }
                        case 0x04:
                        {
                            stream.Position = head + par1;
                            break;
                        }
                        case 0x0C:
                        {
                            stream.Position = head + par1;
                            break;
                        }
                        case 0x0D:
                        {
                            //string str = encoding.GetString(reader.ReadBytes(par3));
                            //Debug.WriteLine($"0x0D {str}");

                            stream.Position = head + par1 + par3;
                            break;
                        }
                        case 0x0F:
                        {
                            stream.Position = head + par1;
                            break;
                        }
                        case 0x11:
                        {
                            stream.Position = head + par1;
                            break;
                        }
                        case 0x12:
                        {
                            stream.Position = head + par1;
                            break;
                        }
                        case 0x18:
                        {
                            //string str = encoding.GetString(reader.ReadBytes(par3));
                            //Debug.WriteLine($"0x18 {str}");

                            stream.Position = head + par1 + par3;
                            break;
                        }
                        case 0x1C:
                        {
                            stream.Position = head + par1;
                            break;
                        }
                        case 0x1E:
                        {
                            reader.ReadByte();
                            reader.ReadByte();
                            reader.ReadByte();
                            byte len = reader.ReadByte();

                            //string str = encoding.GetString(reader.ReadBytes(len));
                            //Debug.WriteLine($"0x1E {str}");

                            stream.Position = head + par1 + len;
                            break;
                        }
                        case 0x1F:
                        {
                            reader.ReadByte();
                            reader.ReadByte();
                            reader.ReadByte();
                            byte len = reader.ReadByte();

                            //string str = encoding.GetString(reader.ReadBytes(len));
                            //Debug.WriteLine($"0x1F {str}");

                            stream.Position = head + par1 + len;
                            break;
                        }
                        case 0x20:
                        {
                            reader.ReadByte();
                            reader.ReadByte();
                            reader.ReadByte();
                            byte len = reader.ReadByte();

                            //string str = encoding.GetString(reader.ReadBytes(len));
                            //Debug.WriteLine($"0x20 {str}");

                            stream.Position = head + par1 + len;
                            break;
                        }
                        case 0x21:
                        {
                            stream.Position = head + par1;
                            break;
                        }
                        case 0x22:
                        {
                            stream.Position = head + par1;
                            break;
                        }
                        case 0x23:
                        {
                            stream.Position = head + par1;
                            break;
                        }
                        case 0x25:
                        {
                            stream.Position = head + par1;
                            break;
                        }
                        case 0x26:
                        {
                            //string str = encoding.GetString(reader.ReadBytes(par3));
                            //Debug.WriteLine($"0x26 {str}");

                            stream.Position = head + par1 + par3;
                            break;
                        }
                        case 0x2A:
                        {
                            stream.Position = head + par1;
                            break;
                        }
                        case 0x36:
                        {
                            reader.ReadByte();
                            reader.ReadByte();
                            reader.ReadByte();
                            reader.ReadByte();

                            //string str = encoding.GetString(reader.ReadBytes(par3));
                            //Debug.WriteLine($"0x36 {str}");

                            stream.Position = head + par1 + par3;
                            break;
                        }
                        case 0x37:
                        {
                            stream.Position = head + par1;
                            break;
                        }
                        case 0x3A:
                        {
                            reader.ReadByte();
                            reader.ReadByte();
                            reader.ReadByte();
                            reader.ReadByte();

                            reader.ReadByte();
                            reader.ReadByte();
                            reader.ReadByte();
                            reader.ReadByte();

                            //string str = encoding.GetString(reader.ReadBytes(par2));
                            //Debug.WriteLine($"0x3A {str}");

                            stream.Position = head + par1 + par2;
                            break;
                        }
                        case 0x3B:
                        {
                            stream.Position = head + par1;
                            break;
                        }
                        case 0x3C:
                        {
                            stream.Position = head + par1;
                            break;
                        }
                        case 0x3D:
                        {
                            stream.Position = head + par1;
                            break;
                        }
                        case 0x3E:
                        {
                            stream.Position = head + par1;
                            break;
                        }
                        case 0x3F:
                        {
                            stream.Position = head + par1; // VM_JMP
                            break;
                        }
                        case 0x38:
                        {
                            stream.Position = head + par1;
                            break;
                        }
                        case 0x40:
                        {
                            stream.Position = head + par1;
                            break;
                        }
                        case 0x41:
                        {
                            stream.Position = head + par1;
                            break;
                        }
                        case 0x42:
                        {
                            stream.Position = head + par1;
                            break;
                        }
                        case 0x43:
                        {
                            stream.Position = head + par1;
                            break;
                        }
                        case 0x44:
                        {
                            stream.Position = head + par1;
                            break;
                        }
                        case 0x45:
                        {
                            stream.Position = head + par1;
                            break;
                        }
                        case 0x46:
                        {
                            stream.Position = head + par1;
                            break;
                        }
                        case 0x47:
                        {
                            stream.Position = head + par1;
                            break;
                        }
                        case 0x49:
                        {
                            stream.Position = head + par1;
                            break;
                        }
                        case 0x4A:
                        {
                            stream.Position = head + par1;
                            break;
                        }
                    }

                    long tail = stream.Position;
                    long size = stream.Position - head;

                    Debug.Assert(size > 0);

                    byte[] cmd = new byte[size];
                    stream.Position = head;
                    stream.Read(cmd, 0, Convert.ToInt32(size));
                    stream.Position = tail;

                    Commands.Add(cmd);
                }
            }
        }

        public void Save(string filePath)
        {
            using (Stream stream = File.Create(filePath))
            using (BinaryWriter writer = new BinaryWriter(stream))
            {
                foreach (var item in Commands)
                {
                    writer.Write(item);
                }
            }
        }

        public void ExportString(string filePath)
        {
            Encoding encoding = Encoding.GetEncoding("shift-jis");

            using (StreamWriter writer = File.CreateText(filePath))
            {
                for (int i = 0; i < Commands.Count; i++)
                {
                    var item = Commands[i];

                    switch (item[0])
                    {
                        case 0x00:
                        {
                            string text = Util.GetNullTerminatedStringFromBytes(item.AsSpan().Slice(item[1]).ToArray(), encoding);
                            writer.WriteLine($"◇{i:D4}◇{text}");
                            writer.WriteLine($"◆{i:D4}◆{text}");
                            writer.WriteLine(string.Empty);
                            break;
                        }
                    }
                }
            }
        }

        public void ImportString(string filePath)
        {
            List<string> translated = new List<string>();

            using (StreamReader reader = File.OpenText(filePath))
            {
                while (!reader.EndOfStream)
                {
                    string text = reader.ReadLine();

                    if (text.Length == 0 || text[0] != '◆')
                    {
                        continue;
                    }

                    translated.Add(text);
                }
            }

            int count = Commands.Count(a => a[0] == 0x00);

            if (translated.Count != count)
            {
                throw new Exception("String count does not match");
            }

            Encoding encoding = Encoding.GetEncoding("gbk");

            for (int i = 0; i < translated.Count; i++)
            {
                string input = translated[i];
                Match m = Regex.Match(input, @"◆(\d+)◆(.+$)");

                if (!m.Success || m.Groups.Count != 3)
                {
                    throw new Exception("Bad text format");
                }

                int index = int.Parse(m.Groups[1].Value);
                string text = ReplaceCharacter(m.Groups[2].Value);

                byte[] bytes = encoding.GetBytes(text);
                int text_length = bytes.Length + 1;
                int aligned_length = text_length + 3 & ~3;

                if (aligned_length > 0xFF)
                {
                    throw new Exception("Text is too long");
                }

                byte[] cmd = new byte[4 + aligned_length];

                cmd[0] = 0x00; // opcode
                cmd[1] = 0x04; // offset to sting
                cmd[2] = 0x00; // unused
                cmd[3] = Convert.ToByte(aligned_length); // length of string

                Buffer.BlockCopy(bytes, 0, cmd, cmd[1], bytes.Length); // copy string
                cmd[cmd[1] + bytes.Length] = 0x00; // null terminated

                // fill unused bytes
                for (int j = cmd[1] + bytes.Length + 1; j < cmd.Length; j++)
                {
                    cmd[j] = 0xCD;
                }

                Commands[index] = cmd;
            }
        }

        private static string ReplaceCharacter(string source)
        {
            source = source.Replace('≫', '〕');
            source = source.Replace('≪', '〔');
            source = source.Replace('・', '·');
            return source;
        }
    }
}
