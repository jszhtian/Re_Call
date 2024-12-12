using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.IO;
using System.Text;
using System.Windows.Forms;
using System.Globalization;

namespace AinDecompiler
{
    public partial class SelectFontForm : Form
    {
        [Bindable(true), Browsable(true), Description("Name of the font to use as an example of variable-width text")]
        public string TemplateFontName
        {
            get
            {
                using (var font = new Font(this.FontNameComboBox1.Text, this.TemplateFontSize, TemplateFontBold ? FontStyle.Bold : FontStyle.Regular))
                {
                    return font.Name;
                }
            }
            set
            {
                this.FontNameComboBox1.Text = value;
            }
        }
        [Bindable(true), Browsable(true), Description("Size of the font to use as an example of variable-width text")]
        public float TemplateFontSize
        {
            get
            {
                float fontSize = 0;
                if (float.TryParse(FontSizeTextBox1.Text, NumberStyles.Number, CultureInfo.InvariantCulture, out fontSize))
                {
                    return fontSize;
                }
                else
                {
                    return 22.0f;
                }
            }
            set
            {
                FontSizeTextBox1.Text = value.FloatToString();
            }
        }
        [Bindable(true), Browsable(true), Description("Weight of the font to use as an example of variable-width text")]
        public bool TemplateFontBold
        {
            get
            {
                return FontBoldCheckBox1.Checked;
            }
            set
            {
                FontBoldCheckBox1.Checked = value;
            }
        }
        [Bindable(true), Browsable(true), Description("Width of a Kanji character for determining margin size in pixels")]
        public float TemplateKanjiWidth
        {
            get
            {
                float fontSize = 0;
                if (float.TryParse(KanjiWidthTextBox.Text, NumberStyles.Number, CultureInfo.InvariantCulture, out fontSize))
                {
                    return fontSize;
                }
                else
                {
                    return 28.0f;
                }
            }
            set
            {
                KanjiWidthTextBox.Text = value.FloatToString();
            }
        }

        public SelectFontForm()
        {
            InitializeComponent();
        }

        private void SelectFontForm_Load(object sender, EventArgs e)
        {
            LoadFonts();
        }

        private void LoadFonts()
        {
            using (var graphics = CreateGraphics())
            {
                const int englishLangauge = 0x09; //from winnt.h
                var fontFamilies = FontFamily.GetFamilies(graphics);
                var fontNames = fontFamilies.Select(fam => fam.GetName(englishLangauge)).OrderBy(n => n).ToArray();

                FontNameComboBox1.BeginUpdate();
                FontNameComboBox1.Items.Clear();
                FontNameComboBox1.Items.AddRange(fontNames);
                FontNameComboBox1.EndUpdate();

                FontNameComboBox2.BeginUpdate();
                FontNameComboBox2.Items.Clear();
                FontNameComboBox2.Items.AddRange(fontNames);
                FontNameComboBox2.EndUpdate();
            }
        }

        private void GetFromFontButton_Click(object sender, EventArgs e)
        {
            string fontName = FontNameComboBox2.Text;
            float fontSize = 22.0f;
            if (float.TryParse(FontSizeTextBox2.Text, NumberStyles.Number, CultureInfo.InvariantCulture, out fontSize))
            {

            }
            else
            {
                return;
            }
            bool fontBold = FontBoldCheckbox2.Checked;

            using (var font = new Font(fontName, fontSize, fontBold ? FontStyle.Bold : FontStyle.Regular))
            {
                float kanjiWidth = CharacterWidthCache.MeasureString("漢", font);
                KanjiWidthTextBox.Text = kanjiWidth.FloatToString();
            }
        }
    }
}
