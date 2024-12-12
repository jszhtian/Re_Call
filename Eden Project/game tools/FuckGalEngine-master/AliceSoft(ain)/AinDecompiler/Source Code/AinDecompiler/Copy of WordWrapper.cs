using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Text;

namespace AinDecompiler
{
    class WordWrapper
    {
        AinFile ainFile;

        //WordWrapOptions _wordWrapOptions;
        public WordWrapOptions wordWrapOptions;
        //{
        //    get
        //    {
        //        return _wordWrapOptions;
        //    }
        //    set
        //    {
        //        _wordWrapOptions = value;
        //        if (value != null && value.ReduceMarginFunctionName != null)
        //        {
        //            this._reduceMarginFunctionNames = new HashSet<string>(value.ReduceMarginFunctionName.Split(new string []{"\r\n"}, StringSplitOptions.RemoveEmptyEntries));
        //        }
        //        else
        //        {
        //            this._reduceMarginFunctionNames = new HashSet<string>();
        //        }
        //    }
        //}

        string _reduceMarginFunctionName = null;

        public string NextLineFunctionName { get { return wordWrapOptions.NextLineFunctionName; } set { wordWrapOptions.NextLineFunctionName = value; } }
        public string NextMessageFunctionName { get { return wordWrapOptions.NextMessageFunctionName; } set { wordWrapOptions.NextMessageFunctionName = value; } }
        public string NextLineFunctionCode { get { return wordWrapOptions.NextLineFunctionCode; } set { wordWrapOptions.NextLineFunctionCode = value; } }
        public string NextMessageFunctionCode { get { return wordWrapOptions.NextMessageFunctionCode; } set { wordWrapOptions.NextMessageFunctionCode = value; } }
        public int MaxCharactersPerLineNormal { get { return wordWrapOptions.MaxCharactersPerLineNormal; } set { wordWrapOptions.MaxCharactersPerLineNormal = value; } }
        public int MaxCharactersPerLineReduced { get { return wordWrapOptions.MaxCharactersPerLineReduced; } set { wordWrapOptions.MaxCharactersPerLineReduced = value; } }
        public int MaxLinesPerMessage { get { return wordWrapOptions.MaxLinesPerMessage; } set { wordWrapOptions.MaxLinesPerMessage = value; } }
        public bool UseVariableWidthFont { get { return wordWrapOptions.UseVariableWidthFont; } set { wordWrapOptions.UseVariableWidthFont = value; } }
        public string ReduceMarginFunctionName { get { return wordWrapOptions.ReduceMarginFunctionName; } set { wordWrapOptions.ReduceMarginFunctionName = value; } }
        HashSet<string> _reduceMarginFunctionNames = new HashSet<string>();

        public HashSet<string> ReduceMarginFunctionNames
        {
            get
            {
                if (_reduceMarginFunctionName != this.ReduceMarginFunctionName && this.ReduceMarginFunctionName != null)
                {
                    _reduceMarginFunctionNames = new HashSet<string>(this.ReduceMarginFunctionName.Split(new string[] { "\r\n" }, StringSplitOptions.RemoveEmptyEntries));
                }
                return _reduceMarginFunctionNames;
            }
            set
            {
                this.ReduceMarginFunctionName = value.ToArray().Join("\r\n");
            }
        }

        //public string nextLineFunctionName, nextMessageFunctionName, reduceMarginFunctionName,
        //    nextLineFunctionCode, nextMessageFunctionCode;
        //public int maxCharactersPerLineNormal, maxCharactersPerLineReduced, maxLinesPerMessage;

        public WordWrapper(AinFile ainFile)
            : this(ainFile, new WordWrapOptions())
        {

        }

        public WordWrapper(AinFile ainFile, WordWrapOptions wordWrapOptions)
        {
            this.wordWrapOptions = wordWrapOptions;
            this.ainFile = ainFile;

            //this.NextLineFunctionName = "R";
            //this.NextLineFunctionCode = " CALLFUNC R ";
            //this.ReduceMarginFunctionName = "顔";
            //this.NextMessageFunctionName = "A";
            //this.NextMessageFunctionCode = " S_PUSH \"\"" + "\r\n" +
            //    "\t" + " CALLFUNC A ";
            //this.MaxCharactersPerLineNormal = 56 + 1; //one more
            //this.MaxCharactersPerLineReduced = 41 + 1; //one more
            //this.MaxLinesPerMessage = 3;

            this.maxCharactersPerLine = this.MaxCharactersPerLineNormal;
        }

        public void WordWrap(string outputAinFilename)
        {
            var options = new CodeDisplayOptions();
            var projectWriter = new AssemblerProjectWriter(ainFile, options);
            projectWriter.BeforeWritingInstruction += new EventHandler<InstructionInfoEventArgs>(projectWriter_BeforeWritingInstruction);


            string tempFile = Path.GetTempFileName();
            try
            {
                var saveProjectInBackground = new ExplorerForm.SaveProjectInBackground();
                saveProjectInBackground.SaveAsProject(projectWriter, tempFile, true);

                var buildProjectInBackground = new ExplorerForm.BuildProjectInBackground();
                buildProjectInBackground.Build(tempFile, outputAinFilename, true);

                //projectWriter.SaveAsProject(tempFile, true);
                //var projectReader = new AssemblerProjectReader();
                //projectReader.LoadProject(tempFile);
                //var outputAinFile = projectReader.MakeAinFile();
                //outputAinFile.WriteAndEncryptAinFile(outputAinFilename);
            }
            finally
            {
                File.Delete(tempFile);
            }
        }

        int maxCharactersPerLine;
        int currentTextLine;

        /// <summary>
        /// When it sees one of these characters, it indents one more fullwidth space.
        /// </summary>
        static HashSet<char> indentCharacters = new HashSet<char>("　‘“（〔［｛〈《「『【".ToCharArray());

        /// <summary>
        /// Count the number of fullwidth spaces that must be added to maintain indentation.
        /// </summary>
        /// <param name="text"></param>
        /// <returns></returns>
        static int GetIndentSize(string text)
        {
            int indentSize = 0;
            int i = 0;
            int textLength = text.Length;
            while (i < textLength && indentCharacters.Contains(text[i]))
            {
                i++;
            }
            indentSize = i;
            return indentSize;
        }


        string remainingText = "";

        void projectWriter_BeforeWritingInstruction(object sender, InstructionInfoEventArgs e)
        {
            var instructionInfo = e.InstructionInfo;
            //bool haveRemainingText = false;

            if (instructionInfo.instruction == Instruction.MSG)
            {
                string messageText = ainFile.Messages[instructionInfo.word1];
                if (messageText.Length > maxCharactersPerLine)
                {
                    e.Handled = true;
                    int realSplitPosition = FindRealSplitPosition(messageText, maxCharactersPerLine);

                    while (realSplitPosition < messageText.Length)
                    {
                        int indentSize = GetIndentSize(messageText);
                        int position = FindSplitPoint(messageText, realSplitPosition);
                        if (position <= indentSize)
                        {
                            //for really long lines - split them at the wrap position instead of an infinite loop
                            position = realSplitPosition;
                        }

                        string line = messageText.Substring(0, position);

                        e.WriteLine(" '" + AssemblerProjectWriter.EscapeMessage(line) + "' ");
                        GenerateNextLine(e);
                        e.WriteLine("");

                        messageText = messageText.Substring(position);

                        //eat spaces
                        position = 0;
                        while (position < messageText.Length && messageText[position] == ' ')
                        {
                            position++;
                        }
                        if (position > 0)
                        {
                            messageText = messageText.Substring(position);
                        }

                        //add indent
                        if (indentSize > 0)
                        {
                            messageText = "".PadLeft(indentSize, '　') + messageText;
                        }

                        //find next split
                        realSplitPosition = FindRealSplitPosition(messageText, maxCharactersPerLine);
                    }

                    e.WriteText(" '" + AssemblerProjectWriter.EscapeMessage(messageText) + "' ");
                }

            }
            else if (instructionInfo.instruction == Instruction.CALLFUNC)
            {
                string functionName = ainFile.Functions[instructionInfo.word1].Name;
                if (functionName == NextLineFunctionName)
                {
                    NextLine(e);
                }
                else if (functionName == NextMessageFunctionName)
                {
                    NextMessage();
                }
                else if (ReduceMarginFunctionNames.Contains(functionName))
                {
                    this.maxCharactersPerLine = this.MaxCharactersPerLineReduced;
                }
            }
        }

        void GenerateNextLine(InstructionInfoEventArgs e)
        {
            if (this.currentTextLine < MaxLinesPerMessage)
            {
                e.WriteText(NextLineFunctionCode);
                e.Handled = true;
            }
            NextLine(e);
        }

        private void NextLine(InstructionInfoEventArgs e)
        {
            this.currentTextLine++;
            if (this.currentTextLine >= MaxLinesPerMessage)
            {
                GenerateNextMessage(e);
            }
        }

        private void GenerateNextMessage(InstructionInfoEventArgs e)
        {
            e.WriteText(NextMessageFunctionCode);
            e.Handled = true;
            NextMessage();
        }

        private void NextMessage()
        {
            this.maxCharactersPerLine = this.MaxCharactersPerLineNormal;
            this.currentTextLine = 0;
        }

        private static int FindSplitPoint(string line, int position)
        {
            int initialPosition = position;
            bool foundSpace = false;
            while (position >= 0)
            {
                //find a space, then find a non-space
                char c = line[position];

                if (c >= 0x80)
                {
                    break;
                }
                if (c == ' ')
                {
                    if (!foundSpace)
                    {
                        foundSpace = true;
                    }
                }
                else
                {
                    if (foundSpace)
                    {
                        break;
                    }
                }
                position--;
            }
            position++;

            if (position <= 0 || position > initialPosition)
            {
                position = initialPosition;
            }
            return position;
        }

        static CharacterWidthCache characterWidthCache = new CharacterWidthCache();

        private int FindRealSplitPosition(string messageText, int position)
        {
            if (!this.UseVariableWidthFont)
            {
                int stringLength = messageText.Length;
                int x = 0;
                int targetX = position;
                int i = 0;
                while (x < targetX && i < stringLength)
                {
                    char c = messageText[i];
                    if (c >= 0x80 && !(c >= 0xFF61 && c <= 0xFF9F))
                    {
                        x += 2;
                    }
                    else
                    {
                        x++;
                    }
                    i++;
                }
                return i;
            }
            else
            {
                int stringLength = messageText.Length;
                float targetX = position + 0.25f;
                float x = 0;
                int i = 0;
                while (x < targetX && i < stringLength)
                {
                    x += characterWidthCache.GetRelativeWidth(messageText[i]);
                    i++;
                }
                return i;
            }
        }

    }
}
