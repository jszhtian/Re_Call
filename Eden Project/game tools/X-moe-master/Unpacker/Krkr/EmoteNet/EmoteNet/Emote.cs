﻿using System;
using System.IO;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Collections;
using System.Threading.Tasks;
using System.Windows.Media;
using System.Windows.Media.Imaging;

namespace EmoteNet
{
    public class TexEntry : Entry
    {
        public string   TexType;
        public int      Width;
        public int      Height;
        public int      TruncatedWidth;
        public int      TruncatedHeight;
    }

    public class PsbScheme
    {
        public uint[] KnownKeys;
    }

    public class PsbOpener
    {
        public string Tag { get { return "PSB/EMOTE"; } }
        public string Description { get { return "E-mote engine texture container"; } }
        public uint Signature { get { return 0x425350; } } // 'PSB'
        public bool IsHierarchic { get { return false; } }
        public bool CanCreate { get { return false; } }

        public static uint[] KnownKeys = new uint[] { 192918854u };

    }

    /// <summary>
    /// PSB container deserialization.
    /// </summary>
    public class PsbReader : IDisposable
    {
        BinaryReader        m_input;

        public PsbReader (Stream input)
        {
            m_input = new ResReader (input);
        }

        public int      Version { get { return m_version; } }
        public bool IsEncrypted { get { return 0 != (m_flags & 3); } }
        public int   DataOffset { get { return m_chunk_data; } }

        public T GetRootKey<T> (string key)
        {
            int obj_offset;
            if (!GetKey (key, m_root, out obj_offset))
                return default(T);
            return (T)GetObject (obj_offset);
        }

        int m_version;
        int m_flags;

        uint[] m_key = new uint[6];
        Dictionary<int, string> m_name_map;

        public bool Parse (uint key)
        {
            m_key[0] = 0x075BCD15;
            m_key[1] = 0x159A55E5;
            m_key[2] = 0x1F123BB5;
            m_key[3] = key;
            m_key[4] = 0;
            m_key[5] = 0;

            if (!ReadHeader())
                return false;
            if (Version < 2)
                throw new NotSupportedException ("Not supported PSB version");
            m_name_map = ReadNames();

            return true;
        }

        public List<Entry> GetTextures ()
        {
            var source = GetRootKey<IDictionary> ("source");
            if (null == source || 0 == source.Count)
                return null;
            var dir = new List<Entry> (source.Count);
            foreach (DictionaryEntry item in source)
            {
                var item_value = item.Value as IDictionary;
                if (null == item_value)
                    continue;
                var texture = item_value["texture"] as IDictionary;
                if (null == texture)
                    continue;
                var pixel = texture["pixel"] as EmChunk;
                if (null == pixel)
                    continue;
                var entry = new TexEntry {
                    Name            = item.Key.ToString(),
                    Type            = "image",
                    Offset          = DataOffset + pixel.Offset,
                    Size            = (uint)pixel.Length,
                    TexType         = texture["type"].ToString(),
                    Width           = Convert.ToInt32 (texture["width"]),
                    Height          = Convert.ToInt32 (texture["height"]),
                    TruncatedWidth  = Convert.ToInt32 (texture["truncated_width"]),
                    TruncatedHeight = Convert.ToInt32 (texture["truncated_height"]),
                };
                dir.Add (entry);
            }
            return dir;
        }

        int m_names;
        int m_strings;
        int m_strings_data;
        int m_chunk_offsets;
        int m_chunk_lengths;
        int m_chunk_data;
        int m_root;
        byte[] m_data;

        bool ReadHeader ()
        {
            m_input.BaseStream.Position = 4;
            m_version = m_input.ReadUInt16();
            m_flags = m_input.ReadUInt16();
            if (m_version < 3)
                m_flags = 2;

            var header = new byte[0x20];
            m_input.Read (header, 0, header.Length);
            if (0 != (m_flags & 1))
                Decrypt (header, 0, 0x20);

            m_names         = LittleEndian.ToInt32 (header, 0x04);
            m_strings       = LittleEndian.ToInt32 (header, 0x08);
            m_strings_data  = LittleEndian.ToInt32 (header, 0x0C);
            m_chunk_offsets = LittleEndian.ToInt32 (header, 0x10);
            m_chunk_lengths = LittleEndian.ToInt32 (header, 0x14);
            m_chunk_data    = LittleEndian.ToInt32 (header, 0x18);
            m_root          = LittleEndian.ToInt32 (header, 0x1C);

            int buffer_length = (int)m_input.BaseStream.Length;
            if (!(m_names           >= 0x28 && m_names < m_chunk_data
                  && m_strings      >= 0x28 && m_strings < m_chunk_data
                  && m_strings_data >= 0x28 && m_strings_data < m_chunk_data
                  && m_chunk_offsets >= 0x28 && m_chunk_offsets < m_chunk_data
                  && m_chunk_lengths >= 0x28 && m_chunk_lengths < m_chunk_data
                  && m_chunk_data   >= 0x28 && m_chunk_data < buffer_length
                  && m_root         >= 0x28 && m_root < m_chunk_data))
                return false;

            if (null == m_data || m_data.Length < m_chunk_data)
                m_data = new byte[m_chunk_data];
            int data_pos = (int)m_input.BaseStream.Position;
            m_input.Read (m_data, data_pos, m_chunk_data-data_pos);
            if (0 != (m_flags & 2))
                Decrypt (m_data, m_names, m_chunk_offsets-m_names);
            // root object is a dictionary
            return 0x21 == m_data[m_root];
        }

        bool GetKey (string name, int dict_offset, out int value_offset)
        {
            value_offset = 0;
            int offset;
            if (!GetOffset (name, out offset))
                return false;
            var keys = GetArray (++dict_offset);
            if (0 == keys.Count)
                return false;

            int upper_bound = keys.Count;
            int lower_bound = 0;
            int key_index = 0;
            while (lower_bound < upper_bound)
            {
                key_index = (upper_bound + lower_bound) >> 1;
                int key = GetArrayElem (keys, key_index);
                if (key == offset)
                    break;
                if (key >= offset)
                    upper_bound = (upper_bound + lower_bound) >> 1;
                else
                    lower_bound = key_index + 1;
            }
            if (lower_bound >= upper_bound)
                return false;

            var values = GetArray (dict_offset + keys.ArraySize);
            int data_offset = GetArrayElem (values, key_index);
            value_offset = dict_offset + keys.ArraySize + values.ArraySize + data_offset;
            return true;
        }

        bool GetOffset (string name, out int offset)
        {
            // FIXME works for ASCII names only.
            var nm1 = GetArray (m_names);
            var nm2 = GetArray (m_names + nm1.ArraySize);
            int i = 0;
            for (int name_idx = 0; ; ++name_idx)
            {
                char symbol = name_idx < name.Length ? name[name_idx] : '\0';
                int prev_i = i;
                i = symbol + GetArrayElem (nm1, i);
                if (i >= nm1.Count || GetArrayElem (nm2, i) != prev_i)
                    break;

                if (name_idx >= name.Length)
                {
                    offset = GetArrayElem (nm1, i);
                    return true;
                }
            }
            offset = 0;
            return false;
        }

        Dictionary<int, string> ReadNames ()
        {
            // this implementation is utterly inefficient. FIXME
            var lookup = new Dictionary<int, byte[]>();
            var next_lookup = new Dictionary<int, byte[]>();
            var dict = new Dictionary<int, string>();
            var nm1 = GetArray (m_names);
            var nm2 = GetArray (m_names + nm1.ArraySize);
            lookup[0] = new byte[0];
            while (lookup.Count > 0)
            {
                foreach (var item in lookup)
                {
                    int first = GetArrayElem (nm1, item.Key);
                    for (int i = 0; i < 256; ++i)
                    {
                        if (GetArrayElem (nm2, i + first) == item.Key)
                        {
                            if (0 == i)
                                dict[GetArrayElem (nm1, i + first)] = Encoding.UTF8.GetString (item.Value);
                            else
                                next_lookup[i+first] = ArrayAppend (item.Value, (byte)i);
                        }
                    }
                }
                var tmp = lookup;
                lookup = next_lookup;
                next_lookup = tmp;
                next_lookup.Clear();
            }
            return dict;
        }

        static byte[] ArrayAppend (byte[] array, byte n)
        {
            var new_array = new byte[array.Length+1];
            Buffer.BlockCopy (array, 0, new_array, 0, array.Length);
            new_array[array.Length] = n;
            return new_array;
        }

        EmArray GetArray (int offset)
        {
            int data_offset = m_data[offset] - 10;
            var array = new EmArray {
                Count = GetInteger (offset, 0xC),
                ElemSize = m_data[offset + data_offset - 1] - 12,
                DataOffset = offset + data_offset,
            };
            array.ArraySize = array.Count * array.ElemSize + data_offset;
            return array;
        }

        int GetArrayElem (EmArray a1, int index)
        {
            int offset = index * a1.ElemSize;
            switch (a1.ElemSize)
            {
            case 1:
                return m_data[a1.DataOffset + offset];
            case 2:
                return LittleEndian.ToUInt16 (m_data, a1.DataOffset + offset);
            case 3:
                return LittleEndian.ToUInt16 (m_data, a1.DataOffset + offset) | m_data[a1.DataOffset + offset + 2] << 16;
            case 4:
                return LittleEndian.ToInt32 (m_data, a1.DataOffset + offset);
            default:
                throw new Exception ("Invalid PSB array structure");
            }
        }

        object GetObject (int offset)
        {
            switch (m_data[offset])
            {
            case 1: return null;
            case 2: return true;
            case 3: return false;

            case 4:
            case 5:
            case 6:
            case 7:
            case 8: return GetInteger (offset, 4);

            case 9:
            case 0x0A:
            case 0x0B:
            case 0x0C: return GetLong (offset);

            case 0x15:
            case 0x16:
            case 0x17:
            case 0x18: return GetString (offset);

            case 0x19:
            case 0x1A:
            case 0x1B:
            case 0x1C: return GetChunk (offset);

            case 0x1D:
            case 0x1E: return GetFloat (offset);
            case 0x1F: return GetDouble (offset);
            case 0x20: return GetList (offset);
            case 0x21: return GetDict (offset);
            default:
                throw new Exception (string.Format ("Unknown serialized object type 0x{0:X2}", m_data[offset]));
            }
        }

        int GetInteger (int offset, int base_type)
        {
            switch (m_data[offset] - base_type)
            {
            case 1: return m_data[offset+1];
            case 2: return LittleEndian.ToUInt16 (m_data, offset+1);
            case 3: return LittleEndian.ToUInt16 (m_data, offset+1) | m_data[offset+3] << 16;
            case 4: return LittleEndian.ToInt32 (m_data, offset+1);
            default: return 0;
            }
        }

        float GetFloat (int offset)
        {
            if (0x1E == m_data[offset])
                return BitConverter.ToSingle (m_data, offset+1); // FIXME endianness
            else
                return 0.0f;
        }

        double GetDouble (int offset)
        {
            if (0x1F == m_data[offset])
                return BitConverter.ToDouble (m_data, offset+1); // FIXME endianness
            else
                return 0.0;
        }

        long GetLong (int offset)
        {
            switch (m_data[offset])
            {
            case 0x09:  return LittleEndian.ToUInt32 (m_data, offset+1) | (long)(sbyte)m_data[offset+5] << 32;
            case 0x0A:  return LittleEndian.ToUInt32 (m_data, offset+1)
                               | (long)LittleEndian.ToInt16 (m_data, offset+5) << 32;
            case 0x0B:  return LittleEndian.ToUInt32 (m_data, offset+1)
                               | (long)LittleEndian.ToUInt16 (m_data, offset+5) << 32
                               | (long)(sbyte)m_data[offset+6] << 48;
            case 0x0C:  return LittleEndian.ToInt64 (m_data, offset+1);
            default:    return 0L;
            }
        }

        string GetString (int obj_offset)
        {
            int index = GetInteger (obj_offset, 0x14);
            var array = GetArray (m_strings);
            int data_offset = m_strings_data + GetArrayElem (array, index);
            return Binary.GetCString (m_data, data_offset, m_data.Length-data_offset, Encoding.UTF8);
        }

        IList GetList (int offset)
        {
            var array = GetArray (++offset);
            var list = new ArrayList (array.Count);
            for (int i = 0; i < array.Count; ++i)
            {
                int item_offset = offset + array.ArraySize + GetArrayElem (array, i);
                var item = GetObject (item_offset);
                list.Add (item);
            }
            return list;
        }

        IDictionary GetDict (int offset)
        {
            var keys = GetArray (++offset);
            if (0 == keys.Count)
                return new Dictionary<string, object>();
            var values = GetArray (offset + keys.ArraySize);
            var dict = new Dictionary<string, object> (keys.Count);
            for (int i = 0; i < keys.Count; ++i)
            {
                int key = GetArrayElem (keys, i);
                var value_offset = GetArrayElem (values, i);
                string key_name = m_name_map[key];
                dict[key_name] = GetObject (offset + value_offset + keys.ArraySize + values.ArraySize);
            }
            return dict;
        }

        EmChunk GetChunk (int offset)
        {
            var chunk_index = GetInteger (offset, 0x18);
            var chunks = GetArray (m_chunk_offsets);
            if (chunk_index >= chunks.Count)
                throw new Exception ("Invalid chunk index");
            var lengths = GetArray (m_chunk_lengths);
            return new EmChunk {
                Offset = GetArrayElem (chunks, chunk_index),
                Length = GetArrayElem (lengths, chunk_index),
            };
        }

        void Decrypt (byte[] data, int offset, int length)
        {
            for (int i = 0; i < length; ++i)
            {
                if (0 == m_key[4])
                {
                    var v5 = m_key[3];
                    var v6 = m_key[0] ^ (m_key[0] << 11);
                    m_key[0] = m_key[1];
                    m_key[1] = m_key[2];
                    var eax = v6 ^ v5 ^ ((v6 ^ (v5 >> 11)) >> 8);
                    m_key[2] = v5;
                    m_key[3] = eax;
                    m_key[4] = eax;
                }
                data[offset+i] ^= (byte)m_key[4];
                m_key[4] >>= 8;
            }
        }

        internal class EmArray
        {
            public int  ArraySize;
            public int  Count;
            public int  ElemSize;
            public int  DataOffset;
        }

        internal class EmChunk
        {
            public int  Offset;
            public int  Length;
        }

        #region IDisposable Members
        bool _disposed = false;
        public void Dispose ()
        {
            if (!_disposed)
            {
                m_input.Dispose();
                _disposed = true;
            }
        }
        #endregion
    }

    internal class PsbTexMetaData : ImageMetaData
    {
        public string   TexType;
        public int      FullWidth;
        public int      FullHeight;
        public int      DataOffset;
    }

    public class PsbTextureFormat
    {
        public string         Tag { get { return "PSB/TEXTURE"; } }
        public string Description { get { return "PSB texture format"; } }
        public uint     Signature { get { return 0x81C3D2D1; } } // 'PSB' ^ 0x81818181

        public IEnumerable<string> Extensions { get; protected set; }
        public PsbTextureFormat ()
        {
            Extensions = new string[0];
        }

        public ImageMetaData ReadMetaData (Stream stream)
        {
            stream.Position = 4;
            using (var reader = new BinaryReader (stream, Encoding.UTF8, true))
            {
                var info = new PsbTexMetaData { BPP = 32 };
                info.DataOffset = reader.ReadInt32();
                info.FullWidth = reader.ReadInt32();
                info.FullHeight = reader.ReadInt32();
                info.Width = reader.ReadUInt32();
                info.Height = reader.ReadUInt32();
                info.TexType = reader.ReadString();
                return info;
            }
        }


        public ImageMetaData ReadMetaData(TexEntry entry)
        {
            var info = new PsbTexMetaData { BPP = 32 };
            info.DataOffset = (int)entry.Offset;
            info.FullWidth = entry.Width;
            info.FullHeight = entry.Height;
            info.Width = (uint)entry.TruncatedWidth;
            info.Height = (uint)entry.TruncatedHeight;
            info.TexType = entry.TexType;
            return info;
        }

        public ImageData Read (Stream stream, ImageMetaData info)
        {
            var meta = (PsbTexMetaData)info;
            var pixels = new byte[meta.Width * meta.Height * 4];
            if ("RGBA8" == meta.TexType)
                ReadRgba8 (stream, meta, pixels);
            else if ("RGBA4444" == meta.TexType)
                ReadRgba4444 (stream, meta, pixels);
            else
                throw new NotImplementedException (string.Format ("PSB texture format '{0}' not implemented", meta.TexType));
            return ImageData.Create (info, PixelFormats.Bgra32, null, pixels);
        }

        void ReadRgba8 (Stream input, PsbTexMetaData meta, byte[] output)
        {
            int dst_stride = (int)meta.Width * 4;
            long next_row = meta.DataOffset;
            int src_stride = meta.FullWidth * 4;
            int dst = 0;
            for (uint i = 0; i < meta.Height; ++i)
            {
                input.Position = next_row;
                input.Read (output, dst, dst_stride);
                dst += dst_stride;
                next_row += src_stride;
            }
        }

        void ReadRgba4444 (Stream input, PsbTexMetaData meta, byte[] output)
        {
            int dst_stride = (int)meta.Width * 4;
            int src_stride = meta.FullWidth * 2;
            int dst = 0;
            var row = new byte[src_stride];
            input.Position = meta.DataOffset;
            for (uint i = 0; i < meta.Height; ++i)
            {
                input.Read (row, 0, src_stride);
                int src = 0;
                for (int x = 0; x < dst_stride; x += 4)
                {
                    uint p = LittleEndian.ToUInt16 (row, src);
                    src += 2;
                    output[dst++] = (byte)((p & 0x000Fu) * 0xFFu / 0x000Fu);
                    output[dst++] = (byte)((p & 0x00F0u) * 0xFFu / 0x00F0u);
                    output[dst++] = (byte)((p & 0x0F00u) * 0xFFu / 0x0F00u);
                    output[dst++] = (byte)((p & 0xF000u) * 0xFFu / 0xF000u);
                }
            }
        }

        public void Write (Stream file, ImageData image)
        {
            throw new NotSupportedException ("PsbTextureFormat.Write not supported");
        }
    }
}

