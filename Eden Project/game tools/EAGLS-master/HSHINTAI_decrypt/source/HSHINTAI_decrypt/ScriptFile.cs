using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Text;

namespace scriptthingy
{
    public static class ScriptFile
    {
        const int maxLabelCount = 100;
        const int textOffset = maxLabelCount * 36;  //0xE10
        public static void ReplaceAllFiles(byte[] inputDecryptedIndexFile, byte[] inputDecryptedDataFile, string path, out byte[] outputDecryptedIndexFile, out byte[] outputDecryptedDataFile, bool useOriginalFile)
        {
            bool baseAddressKnown = false;
            int baseAddress = 0;

            var dataBytes = inputDecryptedDataFile;
            var indexFile = inputDecryptedIndexFile;

            var ms = new MemoryStream(indexFile);
            var br = new BinaryReader(ms);
            var shiftJis = Encoding.GetEncoding("shift-jis");

            var msIndex = new MemoryStream();
            var bwIndex = new BinaryWriter(msIndex);
            var msData = new MemoryStream();
            var bwData = new BinaryWriter(msData);

            while (true)
            {
                var fileNameBytes = br.ReadBytes(0x18);
                int zeroIndex = Array.IndexOf(fileNameBytes, (byte)0);
                if (zeroIndex == -1) zeroIndex = fileNameBytes.Length;
                if (zeroIndex == 0)
                {
                    break;
                }
                string fileName = shiftJis.GetString(fileNameBytes, 0, zeroIndex);
                int address = (int)br.ReadInt64();
                if (!baseAddressKnown)
                {
                    baseAddressKnown = true;
                    baseAddress = address;
                }
                address -= baseAddress;
                int length = (int)br.ReadInt64();

                if (useOriginalFile)
                {
                    string datFileName = Path.Combine(path, fileName);
                    var newData = File.ReadAllBytes(datFileName);

                    bwIndex.Write(fileNameBytes);
                    bwIndex.Write((Int64)(msData.Position + baseAddress));
                    bwIndex.Write((Int64)newData.Length);
                    bwData.Write(newData);
                }
                else
                {


                    int headerEndPosition = address + textOffset;
                    //headerEndPosition = SkipFirstLineBreak(headerEndPosition, dataBytes);

                    var originalHeader = dataBytes.SubArray(address, headerEndPosition - address);
                    var footer = dataBytes.SubArray(address + length - 2, 2);
                    string textFileName = Path.ChangeExtension(Path.Combine(path, fileName), ".txt");
                    var newTextData = File.ReadAllBytes(textFileName);
                    newTextData = RemoveCRAfterDollarSign(newTextData);
                    var header = BuildHeader(newTextData);

                    int newLength = header.Length + newTextData.Length + footer.Length;

                    bwIndex.Write(fileNameBytes);
                    bwIndex.Write((Int64)(msData.Position + baseAddress));
                    bwIndex.Write((Int64)newLength);
                    bwData.Write(header);
                    bwData.Write(newTextData);
                    bwData.Write(footer);
                }
            }

            msIndex.Write(inputDecryptedIndexFile, (int)msIndex.Position, (int)(inputDecryptedIndexFile.Length - msIndex.Position));
            outputDecryptedIndexFile = msIndex.ToArray();
            outputDecryptedDataFile = msData.ToArray();
        }

        private static byte[] BuildHeader(byte[] newTextData)
        {
            //create header by looking for $...0A
            //32 bytes for label name
            //4 bytes for offset within file
            //space for 100 labels
            var ms = new MemoryStream();
            var bw = new BinaryWriter(ms);


            Func<byte[], byte, int, int> IndexOf = (byteArr, lookFor, startPosition) =>
                {
                    int result = Array.IndexOf<byte>(byteArr, lookFor, startPosition);
                    if (result == -1)
                    {
                        result = byteArr.Length;
                    }
                    return result;
                };

            int position = -1;
            while (position < newTextData.Length)
            {
                position = IndexOf(newTextData, (byte)'$', position + 1);
                if (position < newTextData.Length)
                {
                    int position2 = IndexOf(newTextData, (byte)0x0A, position);
                    var labelName = newTextData.SubArray(position + 1, position2 - position - 1);
                    long streamPos = ms.Position;
                    bw.Write(labelName);
                    ms.SetLength(streamPos + 32);
                    ms.Position = streamPos + 32;
                    bw.Write(position);
                    if (ms.Position >= textOffset)
                    {
                        break;
                    }
                }
            }
            ms.SetLength(textOffset);
            return ms.ToArray();
        }

        public static void DumpAllFiles(byte[] decryptedIndexFile, byte[] decryptedDataFile, string path, bool extractOriginal)
        {
            bool baseAddressKnown = false;
            int baseAddress = 0;

            var ms = new MemoryStream(decryptedIndexFile);
            var br = new BinaryReader(ms);
            var shiftJis = Encoding.GetEncoding("shift-jis");

            while (true)
            {
                var fileNameBytes = br.ReadBytes(0x18);
                int zeroIndex = Array.IndexOf(fileNameBytes, (byte)0);
                if (zeroIndex == -1) zeroIndex = fileNameBytes.Length;
                if (zeroIndex == 0)
                {
                    break;
                }
                string fileName = shiftJis.GetString(fileNameBytes, 0, zeroIndex);
                long address = br.ReadInt64();
                if (!baseAddressKnown)
                {
                    baseAddressKnown = true;
                    baseAddress = (int)address;
                }
                address -= baseAddress;
                long length = br.ReadInt64();

                string textFileName = Path.ChangeExtension(Path.Combine(path, fileName), ".txt");

                if (extractOriginal)
                {
                    var rawbytes = decryptedDataFile.SubArray((int)address, (int)length);
                    string datFileName = Path.Combine(path, fileName);
                    File.WriteAllBytes(datFileName, rawbytes);
                }
                else
                {
                    int position = (int)address + textOffset;
                    //position = SkipFirstLineBreak(position, decryptedDataFile);

                    var bytes = decryptedDataFile.SubArray(position, (int)length - (position - (int)address) - 2);
                    var bytes2 = AddCRAfterDollarSign(bytes);
                    File.WriteAllBytes(textFileName, bytes2);

                    //using (var fs = new FileStream(textFileName, FileMode.Create, FileAccess.Write, FileShare.ReadWrite))
                    //{
                    //    int position = (int)address + textOffset;
                    //    //position = SkipFirstLineBreak(position, decryptedDataFile);
                    //    var bytes = decryptedDataFile.SubArray(position, (int)length - (position - (int)address) - 2);
                    //    var bytes2 = RemoveLF(bytes);
                    //    fs.Write(bytes2);
                    //    //fs.Write(decryptedDataFile, position, (int)length - (position - (int)address) - 2);
                    //}
                }
            }
        }

        public static byte[] AddCRAfterDollarSign(byte[] bytes)
        {
            MemoryStream output = new MemoryStream();
            //MemoryStream input = new MemoryStream(bytes);

            int start = 0;
            int pos = 0;

            Func<byte[], byte, int, int> IndexOf = (byteArr, lookFor, startAt) =>
            {
                int i = startAt;
                while (i < byteArr.Length)
                {
                    if (byteArr[i] == lookFor)
                    {
                        return i;
                    }
                    i++;
                }
                return byteArr.Length;
            };

            Action<int> moveTo = newPos =>
            {
                output.Write(bytes, start, newPos - start);
                start = newPos;
                pos = newPos;
            };

            while (pos < bytes.Length)
            {
                int indexOfDollarSign = IndexOf(bytes, (byte)'$', pos);
                if (indexOfDollarSign >= bytes.Length)
                {
                    moveTo(bytes.Length);
                    break;
                }
                int indexOfLf = IndexOf(bytes, (byte)'\n', indexOfDollarSign + 1);
                if (indexOfLf >= bytes.Length)
                {
                    moveTo(bytes.Length);
                    break;
                }

                int indexOfCr = IndexOf(bytes, (byte)'\r', indexOfDollarSign + 1);
                if (indexOfLf < indexOfCr)
                {
                    moveTo(indexOfLf);
                    output.WriteByte((byte)'\r');
                    moveTo(indexOfLf + 1);
                }
                else
                {
                    moveTo(indexOfCr);
                }
            }

            moveTo(bytes.Length);
            return output.ToArray();
        }

        public static byte[] RemoveCRAfterDollarSign(byte[] bytes)
        {
            MemoryStream output = new MemoryStream();
            //MemoryStream input = new MemoryStream(bytes);

            int start = 0;
            int pos = 0;

            Func<byte[], byte, int, int> IndexOf = (byteArr, lookFor, startAt) =>
            {
                int i = startAt;
                while (i < byteArr.Length)
                {
                    if (byteArr[i] == lookFor)
                    {
                        return i;
                    }
                    i++;
                }
                return byteArr.Length;
            };

            Action<int> moveTo = newPos =>
            {
                output.Write(bytes, start, newPos - start);
                start = newPos;
                pos = newPos;
            };

            while (pos < bytes.Length)
            {
                int indexOfDollarSign = IndexOf(bytes, (byte)'$', pos);
                if (indexOfDollarSign >= bytes.Length)
                {
                    moveTo(bytes.Length);
                    break;
                }
                int indexOfCr = IndexOf(bytes, (byte)'\r', indexOfDollarSign + 1);
                if (indexOfCr >= bytes.Length)
                {
                    moveTo(bytes.Length);
                    break;
                }

                moveTo(indexOfCr);
                //eat carraige return
                pos++;
                start++;
            }

            moveTo(bytes.Length);
            return output.ToArray();
        }


        public static byte[] FindIndexFileKey(byte[] encryptedIndexFile)
        {
            //start at last 8KB (where it's all zeroes)
            uint seed = BitConverter.ToUInt32(encryptedIndexFile, encryptedIndexFile.Length - 4);
            var rnd = new Rand(seed);
            int startPosition = encryptedIndexFile.Length - 4 - 8192;
            for (int i = 0; i < startPosition; i++)
            {
                rnd.rand();
            }
            seed = rnd.Seed;

            byte[] outputKey = null;

            for (int l = 1; l < 1024; l++)
            {
                if (TryToFindIndexFileKey(encryptedIndexFile, startPosition, seed, l, ref outputKey))
                {
                    break;
                }
            }

            return outputKey;
        }

        static bool TryToFindIndexFileKey(byte[] encryptedIndexFile, int startPosition, uint seed, int keyLength, ref byte[] outputKey)
        {
            //Look at a bunch of zeroes in the file.
            var rnd = new Rand(seed);
            byte[] keyKnown = new byte[keyLength];
            byte[] key = new byte[keyLength];

            for (int i = startPosition; i < encryptedIndexFile.Length - 4; i++)
            {
                byte b = encryptedIndexFile[i];
                int a = rnd.rand();
                a = a % keyLength;
                if (keyKnown[a] == 0)
                {
                    keyKnown[a] = 1;
                    key[a] = b;

                    //if (keyKnown.All(x=>x == 1))
                    //{

                    //}
                }
                else
                {
                    if (key[a] != b)
                    {
                        return false;
                    }
                }
            }

            if (keyKnown.Any(b => b == 0))
            {
                return false;
            }

            outputKey = key;
            return true;
        }

        public static byte[] FindDataFileKey(byte[] encryptedDataFile, byte[] decryptedIndexFile)
        {
            bool fileStartOffsetKnown = false;
            int fileStartOffset = 0;

            //Look for 0D 0A pairs.
            List<KeyValuePair<int, int>> FileOffsets = new List<KeyValuePair<int, int>>();

            var ms = new MemoryStream(decryptedIndexFile);
            var br = new BinaryReader(ms);
            var shiftJis = Encoding.GetEncoding("shift-jis");

            while (true)
            {
                var fileNameBytes = br.ReadBytes(0x18);
                if (fileNameBytes[0] == 0)
                {
                    break;
                }
                long address = br.ReadInt64();
                if (fileStartOffsetKnown == false)
                {
                    fileStartOffsetKnown = true;
                    fileStartOffset = (int)address;
                }
                address -= fileStartOffset;
                long length = br.ReadInt64();
                FileOffsets.Add(new KeyValuePair<int, int>((int)address, (int)length));
            }

            byte[] outputKey = null;

            for (int l = 1; l < 1024; l++)
            {
                if (TryToFindDataFileKey(encryptedDataFile, FileOffsets, l, ref outputKey))
                {
                    break;
                }
            }

            return outputKey;

        }

        private static bool TryToFindDataFileKey(byte[] dataFile, List<KeyValuePair<int, int>> FileOffsets, int keyLength, ref byte[] outputKey)
        {
            byte[] keyKnown = new byte[keyLength];
            byte[] key = new byte[keyLength];

            foreach (var pair in FileOffsets)
            {
                //OD bytes are preceeded by 0A bytes
                int fileOffset = pair.Key;
                int fileLength = pair.Value;

                int seed = (int)((sbyte)dataFile[fileOffset + fileLength - 1]);
                var rnd = new Rand(seed);

                byte b2 = 0;
                for (int i = textOffset + 1; i < fileLength - 1; i += 2)
                {
                    int a = rnd.rand();
                    a = a % keyLength;
                    byte b = dataFile[i + fileOffset];

                    if (b2 == 0x0D)
                    {
                        byte c;
                        //next byte of previous value should be 0A
                        c = dataFile[i + fileOffset - 1];
                        c ^= 0x0A;
                        if (keyKnown[a] == 0)
                        {
                            keyKnown[a] = 1;
                            key[a] = c;
                        }
                        else
                        {
                            if (key[a] != c)
                            {
                                return false;
                            }
                        }
                    }
                    b2 = b;
                }
            }

            if (keyKnown.Any(b => b == 0))
            {
                return false;
            }
            outputKey = key;
            return true;
        }

        public static void DecryptIndexFile(byte[] encryptedIndexFile, byte[] key)
        {
            //string keyString = "1qaz2wsx3edc4rfv5tgb6yhn7ujm8ik,9ol.0p;/-@:^[]";
            //byte[] key = UTF8Encoding.UTF8.GetBytes(keyString);
            int keyLength = key.Length;
            int seed = BitConverter.ToInt32(encryptedIndexFile, encryptedIndexFile.Length - 4);
            var rnd = new Rand(seed);
            for (int i = 0; i < encryptedIndexFile.Length - 4; i++)
            {
                int a = rnd.rand();
                a = a % keyLength;
                encryptedIndexFile[i] ^= key[a];
            }
        }

        public static void DecryptFiles(byte[] decryptedIndexFile, byte[] encryptedDataFile, byte[] key)
        {
            bool baseAddressKnown = false;
            int baseAddress = 0;

            var ms = new MemoryStream(decryptedIndexFile);
            var br = new BinaryReader(ms);
            var shiftJis = Encoding.GetEncoding("shift-jis");

            while (true)
            {
                var fileNameBytes = br.ReadBytes(0x18);
                int zeroIndex = Array.IndexOf(fileNameBytes, (byte)0);
                if (zeroIndex == -1) zeroIndex = fileNameBytes.Length;
                if (zeroIndex == 0)
                {
                    break;
                }
                string fileName = shiftJis.GetString(fileNameBytes, 0, zeroIndex);
                long address = br.ReadInt64();
                if (!baseAddressKnown)
                {
                    baseAddressKnown = true;
                    baseAddress = (int)address;
                }
                address -= baseAddress;
                long length = br.ReadInt64();

                DecryptFile(encryptedDataFile, key, (int)address, (int)length);
            }
        }

        private static void DecryptFile(byte[] encryptedDateFile, byte[] key, int start, int length)
        {
            //string keyString = "EAGLS_SYSTEM";
            //byte[] key = UTF8Encoding.UTF8.GetBytes(keyString);
            int keyLength = key.Length;
            int seed = (int)((sbyte)encryptedDateFile[start + length - 1]);
            var rnd = new Rand(seed);
            for (int i = start + textOffset + 0; i < start + length - 1; i += 2)
            {
                int a = rnd.rand();
                a = a % keyLength;
                byte keyByte = key[a];
                encryptedDateFile[i] ^= keyByte;
            }

        }
    }

    public class Rand
    {
        public Rand(uint seed)
        {
            srand(seed);
        }
        public Rand(int seed)
        {
            srand(seed);
        }
        uint _seed;
        public uint Seed
        {
            get
            {
                return _seed;
            }
            set
            {
                _seed = value;
            }
        }
        public void srand(uint seed)
        {
            this.Seed = seed;
        }
        public void srand(int seed)
        {
            this.Seed = (uint)seed;
        }
        public int rand()
        {
            unchecked
            {
                uint a = this._seed;
                a *= 0x343FD;
                a += 0x269EC3;
                this._seed = a;

                return (int)((a >> 16) & 0x7FFF);
            }
        }

    }

    public static class Extensions
    {
        public static byte[] SubArray(this byte[] array, int start, int length)
        {
            byte[] newArray = new byte[length];
            Array.Copy(array, start, newArray, 0, length);
            return newArray;
        }

        //public static byte[] PadArray(this byte[] array, int newLength, byte padValue)
        //{
        //    var newArray = new byte[newLength];
        //    int copySize = Math.Min(newLength, array.Length);
        //    Array.Copy(array, newArray, copySize);

        //}

    }
}
