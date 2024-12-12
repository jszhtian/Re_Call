using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using System.Drawing;
using System.Windows.Media.Imaging;
using System.Windows.Markup;
using System.Drawing.Imaging;

namespace CriwareXtxDecoder
{
    class Program
    {
        static System.Drawing.Bitmap BitmapFromSource(BitmapSource bitmapsource)
        {
            System.Drawing.Bitmap bitmap;
            using (MemoryStream outStream = new MemoryStream())
            {
                BitmapEncoder enc = new BmpBitmapEncoder();

                enc.Frames.Add(BitmapFrame.Create(bitmapsource));
                enc.Save(outStream);
                bitmap = new System.Drawing.Bitmap(outStream);
            }
            return bitmap;
        }

        static void Main(string[] args)
        {
            if (args.Length != 1)
                return;

            FileStream fs = null;
            try
            {
                fs = new FileStream(args[0], FileMode.Open, FileAccess.Read);
            }
            catch(Exception e)
            {
                System.Console.WriteLine(e.ToString());
                return;
            }
            var TxtObject = new Cri.XtxFormat();
            var metaData = TxtObject.ReadMetaData(fs);
            if(metaData != null)
            {
                ImageData BitmapObject = null;

                try
                {
                    BitmapObject = TxtObject.Read(fs, metaData);
                }
                catch (Exception e)
                {
                    System.Console.WriteLine(e.ToString());
                    return;
                }

                string outFileName = args[0] + ".bmp";
                
                var bitmap = BitmapFromSource(BitmapObject.Bitmap);
                bitmap.LockBits(new Rectangle(0, 0, bitmap.Width, bitmap.Height),
                    ImageLockMode.ReadWrite, bitmap.PixelFormat);
                bitmap.Save(outFileName);
            }
        }
    }
}
