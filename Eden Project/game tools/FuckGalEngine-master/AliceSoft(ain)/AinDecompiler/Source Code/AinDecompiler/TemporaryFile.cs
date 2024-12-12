using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Text;

namespace AinDecompiler
{
    public class TemporaryFile : IDisposable
    {
        public string FileName;
        public TemporaryFile(string extension, bool createIt)
        {
        tryAgain:
            string randomFileName = Path.Combine(Path.GetTempPath(), Path.GetRandomFileName().Replace(".", "") + "." + extension);
            if (File.Exists(randomFileName))
            {
                goto tryAgain;
            }
            this.FileName = randomFileName;
            if (createIt)
            {
                File.WriteAllBytes(this.FileName, new byte[0]);
            }
        }

        #region IDisposable Members

        public void Dispose()
        {
            if (File.Exists(this.FileName))
            {
                File.Delete(this.FileName);
            }
        }

        #endregion
    }
}
