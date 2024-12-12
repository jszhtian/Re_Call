using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Text;
using System.ComponentModel;
using System.Windows.Forms;

namespace AinDecompiler
{
    [Serializable]
    public class WordWrapOptions
    {
        [Bindable(true), Browsable(true), Description("Name of the New Line function, so it can detect new lines.  Usually R.")]
        public string NextLineFunctionName { get; set; }
        [Bindable(true), Browsable(true), Description("Name of the Next Message function, so it knows when the message ends.  Usually A.")]
        public string NextMessageFunctionName { get; set; }
        [Bindable(true), Browsable(true), Description("The set of 'Face Display' functions that makes the margins narrower." + "\r\n" +
            "If the game doesn't make text narrower when a face is displayed, use the same margin width for both cases.")]
        public string ReduceMarginFunctionName { get; set; }
        [Bindable(true), Browsable(true), Description("The ASM code that will be output when the word wrapper needs to split a line of text." + "\r\n" +
            "Usually CALLFUNC R, but sometimes it might need more arguments.")]
        public string NextLineFunctionCode { get; set; }
        [Bindable(true), Browsable(true), Description("The ASM code that will be output when the word wrapper needs to split a series of lines across multiple messages." + "\r\n" +
            "Usually CALLFUNC A, but sometimes it might need more arguments.")]
        public string NextMessageFunctionCode { get; set; }
        [Bindable(true), Browsable(true), Description("The width of a line of text in halfwidth characters.  Lines longer than this are split." + "\r\n" +
            "Fullwidth characters count as two halfwidth characters.")]
        public int MaxCharactersPerLineNormal { get; set; }
        [Bindable(true), Browsable(true), Description("The narrower width used when the game displays a face in the left side of the text box." + "\r\n" +
            "If the game doesn't do this, use the same value as the regular margin.")]
        public int MaxCharactersPerLineReduced { get; set; }
        [Bindable(true), Browsable(true), Description("The height of the text box in lines.  If more rows are displayed, it splits it into multiple messages.")]
        public int MaxLinesPerMessage { get; set; }
        [Bindable(true), Browsable(true), Description("Whether or not we are using a variable-width font (not a fixed width font)." + "\r\n" +
            "Number of characters per line will be measured in units of half a kanji character.")]
        public bool UseVariableWidthFont { get; set; }
        [Bindable(true), Browsable(true), Description("If the line wraps to a new line, remove the line break for that line." + "\r\n" +
            "This prevents short fragments of messages from taking up their own lines.")]
        public bool RemoveLineBreaksIfWordWrapping { get; set; }
        [Bindable(true), Browsable(true), Description("If the line begins with a '「' or fullwidth space, add fullwidth spaces to the next line if the line wraps.")]
        public bool MaintainIndentation { get; set; }
        [Bindable(true), Browsable(true), Description("Name of the font to use as an example of variable-width text")]
        public string TemplateFontName { get; set; }
        [Bindable(true), Browsable(true), Description("Size of the font to use as an example of variable-width text")]
        public float TemplateFontSize { get; set; }
        [Bindable(true), Browsable(true), Description("Weight of the font to use as an example of variable-width text")]
        public bool TemplateFontBold { get; set; }
        [Bindable(true), Browsable(true), Description("Width of a Kanji character for determining margin size in pixels")]
        public float TemplateKanjiWidth { get; set; }

        public WordWrapOptions()
        {
            this.NextLineFunctionName = "R";
            this.NextLineFunctionCode = " CALLFUNC R ";
            this.ReduceMarginFunctionName = "顔";
            this.NextMessageFunctionName = "A";
            this.NextMessageFunctionCode = " CALLFUNC A ";
            this.MaxCharactersPerLineNormal = 57;
            this.MaxCharactersPerLineReduced = 42;
            this.MaxLinesPerMessage = 3;
            this.UseVariableWidthFont = false;
            this.RemoveLineBreaksIfWordWrapping = true;
            this.MaintainIndentation = true;
            this.TemplateFontName = "Arial Unicode MS";
            this.TemplateFontSize = 22.0f;
            this.TemplateFontBold = false;
            this.TemplateKanjiWidth = 28;
        }

        public void AssignFrom(WordWrapOptions assignFromThis)
        {
            ReflectionUtil.MemberwiseAssign(this, assignFromThis);
        }

        public void SaveToRegistry(AinFile ainFile)
        {
            string baseName = Path.GetFileNameWithoutExtension(ainFile.OriginalFilename);
            string pathName = "WordWrapOptions\\" + baseName;
            SaveToRegistry(pathName);
        }

        public void SaveToRegistry(string registryPathName)
        {
            RegistryUtility.SaveObject(registryPathName, this);
        }

        public static WordWrapOptions GetWordWrapOptions(AinFile ainFile)
        {
            var wordWrapOptions = new WordWrapOptions();
            string baseName = Path.GetFileNameWithoutExtension(ainFile.OriginalFilename);
            string pathName = "WordWrapOptions\\" + baseName;
            if (RegistryUtility.PathExists(pathName))
            {
                RegistryUtility.GetObject(pathName, wordWrapOptions);
            }
            else
            {
                //generate default code for next line and next message commands
                wordWrapOptions.NextLineFunctionCode = GetGeneratedCode(wordWrapOptions.NextLineFunctionName, ainFile);
                wordWrapOptions.NextMessageFunctionCode = GetGeneratedCode(wordWrapOptions.NextMessageFunctionName, ainFile);

                //Face portrait detection for a few games

                //hoken
                MatchFunction(ainFile, wordWrapOptions, 57, 41, "顔表示", DataType.Int);

                //tsumashibori
                MatchFunction(ainFile, wordWrapOptions, 57, 42, "顔", DataType.Int, DataType.Int);

                //rancequest
                if (MatchFunction(ainFile, wordWrapOptions, 57, 42, "セリフ２", DataType.Int, DataType.String) &&
                    MatchFunction(ainFile, wordWrapOptions, 57, 42, "セリフ", DataType.Int, DataType.String) &&
                    MatchFunction(ainFile, wordWrapOptions, 57, 42, "ト書き") &&
                    MatchFunction(ainFile, wordWrapOptions, 57, 42, "思考", DataType.Int, DataType.String))
                {
                    wordWrapOptions.ReduceMarginFunctionName = new string[] { "セリフ", "セリフ２", "ト書き", "思考" }.Join("\r\n");
                    wordWrapOptions.UseVariableWidthFont = true;
                }

                //oyakoranken
                if (MatchFunction(ainFile, wordWrapOptions, -1, -1, "◎台詞", DataType.String, DataType.String))
                {
                    wordWrapOptions.UseVariableWidthFont = true;
                }
            }
            return wordWrapOptions;
        }

        private static bool MatchFunction(AinFile ainFile, WordWrapOptions wordWrapOptions, int maxCharactersPerLineNormal, int maxCharactersPerLineReduced, string functionName, params DataType[] dataTypes)
        {
            var function = ainFile.GetFunction(functionName);
            if (function != null && function.ParameterCount == dataTypes.Length && function.Parameters.Take(function.ParameterCount).Select(a => a.DataType).SequenceEqual(dataTypes))
            {
                //if we specified values for the max lengths, set them
                if (maxCharactersPerLineNormal > 0)
                {
                    wordWrapOptions.MaxCharactersPerLineNormal = maxCharactersPerLineNormal;
                }
                if (maxCharactersPerLineReduced > 0)
                {
                    wordWrapOptions.MaxCharactersPerLineReduced = maxCharactersPerLineReduced;
                }

                //set the function name
                wordWrapOptions.ReduceMarginFunctionName = functionName;

                //indicate success
                return true;
            }
            return false;
        }

        private static string GetGeneratedCode(string functionName, AinFile ainFile)
        {
            StringBuilder sb = new StringBuilder();
            if (ainFile.FunctionNameToIndex.ContainsKey(functionName))
            {
                Function function = ainFile.Functions[ainFile.FunctionNameToIndex[functionName]];
                if (function.ParameterCount > 0)
                {
                    for (int i = 0; i < function.ParameterCount; i++)
                    {
                        var arg = function.Parameters[i];
                        if (arg.DataType == DataType.Int)
                        {
                            sb.AppendLine("\t" + "PUSH 0");
                        }
                        else if (arg.DataType == DataType.String)
                        {
                            sb.AppendLine("\t" + "S_PUSH \"\"");
                        }
                        else
                        {
                            sb.AppendLine("\t" + "PUSH 0");
                        }
                    }
                }
            }
            sb.Append("\t" + "CALLFUNC " + functionName);
            return sb.ToString();
        }
    }
}
