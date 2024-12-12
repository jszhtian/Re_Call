using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.Runtime.InteropServices;
using System.Text.RegularExpressions;

namespace PackTool
{
    public class Pack
    {
        public Pack()
        {
        }

        public void Extract(string filePath, string outputPath, uint archiveKey, bool decryptScript, uint scriptKey)
        {
            using (Stream stream = File.OpenRead(filePath))
            using (BinaryReader reader = new BinaryReader(stream))
            {
                List<PackEntry> entries = new List<PackEntry>();

                uint count = reader.ReadUInt32().Reverse() ^ archiveKey;

                for (uint i = 0; i < count; i++)
                {
                    uint hash = reader.ReadUInt32().Reverse();
                    byte flags = (byte)(reader.ReadByte() ^ hash);
                    uint offset = reader.ReadUInt32().Reverse() ^ hash;
                    uint packedSize = reader.ReadUInt32().Reverse() ^ hash;
                    uint unpackedSize = reader.ReadUInt32().Reverse() ^ hash;

                    entries.Add(new PackEntry
                    {
                        Hash = hash,
                        Flags = flags,
                        Offset = offset,
                        PackedSize = packedSize,
                        UnpackedSize = unpackedSize
                    });
                }

                Directory.CreateDirectory(outputPath);

                for (int i = 0; i < entries.Count; i++)
                {
                    var item = entries[i];

                    stream.Position = item.Offset;
                    byte[] data = reader.ReadBytes(Convert.ToInt32(item.PackedSize));

                    if (decryptScript)
                    {
                        uint xor_key = item.Hash ^ scriptKey;
                        uint xor_len = item.PackedSize >> 2;

                        var data2 = MemoryMarshal.Cast<byte, uint>(data);
                        for (int j = 0; j < xor_len; j++)
                        {
                            data2[j] ^= xor_key;
                        }
                    }

                    if ((item.Flags & 0x02) != 0)
                    {
                        data = Util.Inflate(data);
                    }

                    string path = Path.Combine(outputPath, $"{i:D4}#{item.Hash:X8}");
                    File.WriteAllBytes(path, data);
                }
            }
        }

        public void Create(string inputPath, string filePath, uint archiveKey, bool encryptScript, uint scriptKey)
        {
            List<PackEntry> entries = new List<PackEntry>();

            foreach (var item in Directory.EnumerateFiles(inputPath))
            {
                string name = Path.GetFileName(item);

                Match m = Regex.Match(name, @"(\d+?)#(\w+)");
                if (!m.Success || m.Groups.Count != 3)
                {
                    throw new Exception("文件名格式必须是“xxxx#yyyyyyyy”。");
                }

                uint index = uint.Parse(m.Groups[1].Value);
                uint hash = uint.Parse(m.Groups[2].Value, NumberStyles.HexNumber);

                entries.Add(new PackEntry
                {
                    Hash = hash,
                    LocalPath = item
                });
            }

            using (Stream stream = File.Create(filePath))
            using (BinaryWriter writer = new BinaryWriter(stream))
            {
                writer.Write(((uint)(entries.Count ^ archiveKey)).Reverse());

                foreach (var item in entries)
                {
                    writer.Write(0);
                    writer.Write((byte)0);
                    writer.Write(0);
                    writer.Write(0);
                    writer.Write(0);
                }

                writer.Flush();

                foreach (var item in entries)
                {
                    byte[] data = File.ReadAllBytes(item.LocalPath);
                    byte[] packed_data = Util.Deflate(data);

                    if (encryptScript)
                    {
                        uint xor_key = item.Hash ^ scriptKey;
                        uint xor_len = (uint)packed_data.Length >> 2;
                        var pd = MemoryMarshal.Cast<byte, uint>(packed_data);
                        for (int i = 0; i < xor_len; i++)
                        {
                            pd[i] ^= xor_key;
                        }
                    }

                    item.Flags = 0x02;
                    item.Offset = Convert.ToUInt32(stream.Position);
                    item.PackedSize = Convert.ToUInt32(packed_data.Length);
                    item.UnpackedSize = Convert.ToUInt32(data.Length);

                    writer.Write(packed_data);
                }

                writer.Flush();
                stream.Position = 4;

                foreach (var item in entries)
                {
                    writer.Write(item.Hash.Reverse());
                    writer.Write((byte)(item.Flags ^ item.Hash));
                    writer.Write((item.Offset ^ item.Hash).Reverse());
                    writer.Write((item.PackedSize ^ item.Hash).Reverse());
                    writer.Write((item.UnpackedSize ^ item.Hash).Reverse());
                }

                writer.Flush();
            }
        }
    }

    class PackEntry
    {
        public uint Hash { get; set; }
        public byte Flags { get; set; }
        public uint Offset { get; set; }
        public uint PackedSize { get; set; }
        public uint UnpackedSize { get; set; }
        public string LocalPath { get; set; }
    }
}
