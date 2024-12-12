using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ScriptTool
{
    static class Util
    {
        public static string GetNullTerminatedStringFromBytes(byte[] bytes, Encoding encoding)
        {
            for (int i = 0; i < bytes.Length; i++)
            {
                if (bytes[i] == 0)
                {
                    if (i > 0)
                    {
                        return encoding.GetString(bytes, 0, i);
                    }
                    else
                    {
                        return string.Empty;
                    }
                }
            }

            return encoding.GetString(bytes);
        }

        public static bool PathIsFolder(string path)
        {
            return new FileInfo(path).Attributes.HasFlag(FileAttributes.Directory);
        }
    }
}
