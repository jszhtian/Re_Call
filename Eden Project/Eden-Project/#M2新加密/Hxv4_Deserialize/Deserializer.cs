using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Numerics;
using System.Text;
using System.Threading.Tasks;

namespace Hxv4_Deserialize
{
    class Deserializer
    {
        public void Deserialize(Stream stream)
        {
            using var reader = new BinaryReader(stream, Encoding.UTF8, true);
            var obj = DeserializeObject(reader);

            //using (var writer = File.CreateText(@"C:\Users\Shiroha\Desktop\花鐘カナデグラム\MEM_03052170_0002B3D3.json"))
            //{
            //    var serializer = Newtonsoft.Json.JsonSerializer.Create();
            //    serializer.Formatting = Newtonsoft.Json.Formatting.Indented;
            //    serializer.Serialize(writer, obj);
            //    writer.Flush();
            //}

            Debug.Assert(stream.Position == stream.Length);
        }

        object DeserializeObject(BinaryReader reader)
        {
            var type = reader.ReadByte();

            switch(type)
            {
                case 0x00:
                {
                    return null;
                }
                case 0x01:
                {
                    return null;
                }
                case 0x02:
                {
                    return ReadString(reader);
                }
                case 0x03:
                {
                    return DeserializeByteArray(reader);
                }
                case 0x04:
                {
                    return ReadInt64(reader);
                }
                case 0x05:
                {
                    return ReadInt64(reader);
                }
                case 0x81:
                {
                    return DeserializeArray(reader);
                }
                case 0xC1:
                {
                    return DeserializeDictionary(reader);
                }
                default:
                {
                    throw new Exception("unknown object type");
                }
            }
        }

        byte[] DeserializeByteArray(BinaryReader reader)
        {
            var count = ReadInt32(reader);
            var array = reader.ReadBytes(count);
            return array;
        }

        List<object> DeserializeArray(BinaryReader reader)
        {
            var count = ReadInt32(reader);

            var array = new List<object>(count);

            for (int i = 0; i < count; i++)
            {
                var obj = DeserializeObject(reader);
                array.Add(obj);
            }

            return array;
        }

        Dictionary<string, object> DeserializeDictionary(BinaryReader reader)
        {
            var count = ReadInt32(reader);

            var dictionary = new Dictionary<string, object>(count);

            for (int i = 0; i < count; i++)
            {
                var name = ReadString(reader);
                var obj = DeserializeObject(reader);
                dictionary.Add(name, obj);
            }

            return dictionary;
        }

        int ReadInt32(BinaryReader reader)
        {
            var lo = reader.ReadUInt16();
            var hi = reader.ReadUInt16();
            var dw = BitRotator.RotateLeft(hi, 8) | (BitRotator.RotateLeft(lo, 8) << 16);
            return dw;
        }

        long ReadInt64(BinaryReader reader)
        {
            var lo = ReadInt32(reader);
            var hi = ReadInt32(reader);
            var qw = (long)hi | ((long)lo << 32);
            return qw;
        }

        string ReadString(BinaryReader reader)
        {
            var length = ReadInt32(reader);
            var buffer = reader.ReadBytes(length * 2);
            return Encoding.Unicode.GetString(buffer);
        }
    }
}
