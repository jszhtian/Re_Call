using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media.Imaging;
using System.Windows.Media;
using System.Windows;

namespace CriwareXtxDecoder
{
    public class ImageData
    {
        private BitmapSource m_bitmap;

        public BitmapSource Bitmap { get { return m_bitmap; } }
        public uint Width { get { return (uint)m_bitmap.PixelWidth; } }
        public uint Height { get { return (uint)m_bitmap.PixelHeight; } }
        public int OffsetX { get; set; }
        public int OffsetY { get; set; }
        public int BPP { get { return m_bitmap.Format.BitsPerPixel; } }

        public static double DefaultDpiX { get; set; }
        public static double DefaultDpiY { get; set; }

        static ImageData ()
        {
            SetDefaultDpi (96, 96);
        }

        public static void SetDefaultDpi (double x, double y)
        {
            DefaultDpiX = x;
            DefaultDpiY = y;
        }

        public ImageData (BitmapSource data, ImageMetaData meta)
        {
            m_bitmap = data;
            OffsetX = meta.OffsetX;
            OffsetY = meta.OffsetY;
        }

        public ImageData (BitmapSource data, int x = 0, int y = 0)
        {
            m_bitmap = data;
            OffsetX = x;
            OffsetY = y;
        }

        public static ImageData Create (ImageMetaData info, PixelFormat format, BitmapPalette palette,
                                        byte[] pixel_data, int stride)
        {
            var bitmap = BitmapSource.Create ((int)info.Width, (int)info.Height, DefaultDpiX, DefaultDpiY,
                                              format, palette, pixel_data, stride);
            bitmap.Freeze();
            return new ImageData (bitmap, info);
        }

        public static ImageData Create (ImageMetaData info, PixelFormat format, BitmapPalette palette,
                                        byte[] pixel_data)
        {
            return Create (info, format, palette, pixel_data, (int)info.Width*((format.BitsPerPixel+7)/8));
        }

        public static ImageData CreateFlipped (ImageMetaData info, PixelFormat format, BitmapPalette palette,
                                        byte[] pixel_data, int stride)
        {
            var bitmap = BitmapSource.Create ((int)info.Width, (int)info.Height, DefaultDpiX, DefaultDpiY,
                                              format, palette, pixel_data, stride);
            var flipped = new TransformedBitmap (bitmap, new ScaleTransform { ScaleY = -1 });
            flipped.Freeze();
            return new ImageData (flipped, info);
        }
    }
}
