using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Text;
using ScintillaNET;

namespace AinDecompiler
{
    public class ScintillaState
    {
        public int SelectionStart;
        public int SelectionEnd;
        public int CursorPosition;
        public int ScrollXPosition;
        public int ScrollPosition;
        public string FunctionName;

        public void ReadFromScintilla(Scintilla textBox)
        {
            this.ScrollPosition = textBox.PositionFromPoint(0, 0);
            this.ScrollXPosition = textBox.Scrolling.XOffset;
            this.CursorPosition = textBox.CurrentPos;
            this.SelectionStart = textBox.Selection.Start;
            this.SelectionEnd = textBox.Selection.End;
        }

        public void WriteToScintilla(Scintilla textBox)
        {
            textBox.CurrentPos = textBox.GetRange().End;
            textBox.Scrolling.ScrollToCaret();
            textBox.CurrentPos = ScrollPosition;
            textBox.Scrolling.ScrollToCaret();
            textBox.Scrolling.XOffset = this.ScrollXPosition;
            textBox.CurrentPos = this.CursorPosition;
            textBox.Selection.Start = this.SelectionStart;
            textBox.Selection.End = this.SelectionEnd;
        }

    }

}
