using ICSharpCode.SharpZipLib.Zip.Compression;
using ICSharpCode.SharpZipLib.Zip.Compression.Streams;
using System;
using System.Collections.Generic;
using System.IO;
using System.Text;

namespace PackTool
{
    static class Util
    {
        public static byte[] Deflate(byte[] buffer)
        {
            Deflater deflater = new Deflater(Deflater.BEST_COMPRESSION);
            using (MemoryStream memoryStream = new MemoryStream())
            using (DeflaterOutputStream deflaterOutputStream = new DeflaterOutputStream(memoryStream, deflater))
            {
                deflaterOutputStream.Write(buffer, 0, buffer.Length);
                deflaterOutputStream.Flush();
                deflaterOutputStream.Finish();

                return memoryStream.ToArray();
            }
        }

        public static byte[] Inflate(byte[] buffer)
        {
            byte[] block = new byte[256];
            MemoryStream outputStream = new MemoryStream();

            Inflater inflater = new Inflater();
            using (MemoryStream memoryStream = new MemoryStream(buffer))
            using (InflaterInputStream inflaterInputStream = new InflaterInputStream(memoryStream, inflater))
            {
                while (true)
                {
                    int numBytes = inflaterInputStream.Read(block, 0, block.Length);
                    if (numBytes < 1)
                        break;
                    outputStream.Write(block, 0, numBytes);
                }
            }

            return outputStream.ToArray();
        }
    }
}
