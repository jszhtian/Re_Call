using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Text;
using System.Windows.Forms;
using System.Drawing;
using System.Runtime.InteropServices;

namespace AinDecompiler
{
    public class CharacterWidthCache
    {
        float[] relativeCharacterWidths = new float[128];
        //float[] relativeCharacterWidths2 = new float[160];

        float templateFontSize;
        string templateFontName;
        bool templateFontBold;

        public CharacterWidthCache(WordWrapOptions options)
        {
            templateFontName = options.TemplateFontName;
            templateFontSize = options.TemplateFontSize;
            templateFontBold = options.TemplateFontBold;
            kanjiWidthRaw = options.TemplateKanjiWidth;
            Init();
        }

        Graphics graphics = null;
        Font font = null;
        Font kanjiFont = null;

        float emptyWidth;
        float kanjiWidthRaw;

        public float GetRelativeWidth(char c)
        {
            if (c >= ' ' && c <= '~')
            {
                return relativeCharacterWidths[c];
            }
            if (c >= 0x80)
            {
                if (c >= '｡' && c <= 'ﾟ')
                {
                    return 1;
                }
                if (c == '」') return relativeCharacterWidths[']'];
                return 2;
            }
            return 1;
        }

        private void Init()
        {
            var firstForm = Application.OpenForms.OfType<Form>().FirstOrDefault();
            try
            {
                this.graphics = firstForm.CreateGraphics();
                //this.font = new Font("MS UI Gothic", 28.0f, FontStyle.Bold);
                //this.font = new Font("Arial Unicode MS", 22.0f, FontStyle.Bold);
                this.font = new Font(templateFontName, templateFontSize, templateFontBold ? FontStyle.Bold : FontStyle.Regular);

                //this.kanjiFont = new Font("MS Gothic", 21.0f, FontStyle.Regular);
                InitMeasureString();
                //kanjiWidthRaw = MeasureString("本", kanjiFont);

                for (char c = ' '; c <= '~'; c++)
                {
                    relativeCharacterWidths[c] = GetRelativeCharacterWidth(c);
                }
                //for (char c = '！'; c <= 'ﾟ'; c++)
                //{
                //    int i = c - 0xFF00;
                //    relativeCharacterWidths2[i] = GetRelativeCharacterWidth(c);
                //}
            }
            finally
            {
                if (font != null)
                {
                    font.Dispose();
                    font = null;
                }
                if (graphics != null)
                {
                    graphics.Dispose();
                    graphics = null;
                }
            }
        }


        [DllImport("gdi32.dll", CharSet = CharSet.Unicode)]
        static extern bool GetTextExtentPoint32(IntPtr hdc, string lpString, int cbString, out Size lpSize);

        [DllImport("gdi32.dll", ExactSpelling = true, PreserveSig = true, SetLastError = true, CharSet = CharSet.Unicode)]
        static extern IntPtr SelectObject(IntPtr hdc, IntPtr hgdiobj);

        [DllImport("gdi32.dll")]
        static extern bool DeleteObject(IntPtr hObject);

        public static Size GetTextExtent(string text, Graphics graphics, Font font)
        {
            Size sz = Size.Empty;
            IntPtr hdc = graphics.GetHdc();
            IntPtr oldfnt = SelectObject(hdc, font.ToHfont());

            GetTextExtentPoint32(hdc, text, text.Length, out sz);

            // reset old font
            DeleteObject(SelectObject(hdc, oldfnt));
            graphics.ReleaseHdc(hdc);
            return sz;
        }

        private void InitMeasureString()
        {
            //var emptySize = graphics.MeasureString("■■", font);
            var emptySize = GetTextExtent("■■", graphics, font);
            emptyWidth = emptySize.Width;
        }

        public static float MeasureString(string text, Font font)
        {
            var firstForm = Application.OpenForms.OfType<Form>().FirstOrDefault();
            using (Graphics graphics = firstForm.CreateGraphics())
            {
                var size2 = GetTextExtent("■■", graphics, font);
                var size = GetTextExtent("■" + text + "■", graphics, font);
                float emptyWidth = size2.Width;
                float rawWidth = size.Width - emptyWidth;
                float width = (int)(rawWidth + 0.5);
                return width;
            }
        }

        //private float MeasureString(string text, Font font)
        //{
        //    //var size = graphics.MeasureString("■" + text + "■", font);
        //    var size2 = GetTextExtent("■■", graphics, font);
        //    var size = GetTextExtent("■" + text + "■", graphics, font);
        //    float emptyWidth = size2.Width;
        //    float rawWidth = size.Width - emptyWidth;
        //    float width = (int)(rawWidth + 0.5);
        //    return width;
        //}

        private float MeasureString(string text)
        {
            var size = GetTextExtent("■" + text + "■", graphics, font);
            float rawWidth = size.Width - emptyWidth;
            float width = (int)(rawWidth + 0.5);
            return width;
        }

        private float GetRelativeCharacterWidth(string c)
        {
            float characterWidth = MeasureString(c);
            return characterWidth / kanjiWidthRaw * 2.0f;
        }

        private float GetRelativeCharacterWidth(char c)
        {
            return GetRelativeCharacterWidth(c.ToString());
        }




    }
}
