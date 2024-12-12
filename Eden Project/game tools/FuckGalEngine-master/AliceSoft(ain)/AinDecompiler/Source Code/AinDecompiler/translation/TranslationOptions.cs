using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ComponentModel;
using System.Text.RegularExpressions;

namespace TranslateParserThingy
{
    [Serializable]
    public class TranslationOptions : ICloneable
    {
        public TranslationOptions()
        {
            this.UseCommentCharacter = true;
            this.CommentCharacter = ";";
            this.RemoveBracketsAndSemicolons = true;
            this.ConfigurationName = "Default Configuration";
            this.ContinuationCharacters = ""; // "&'()+-0123456789rs";
            this.BreakAfterCharacters = "？！。";
            this.RegularExpressionsToIgnore = new string[] { "／", "％", "＋", "－", "[Ａ-Ｚ０-９ａ-ｚ]+" };
            this.RegularExpressionWhitelist = new string[0];
            this.RegularExpressionsToRemove = new string[] { @"\[", @"\]", ";", " :" };
            this.RegularExpressionsToReplace = new string[0];
            this.RegularExpressionReplacements = new string[0];
        }

        public TranslationOptions Clone()
        {
            var options = (TranslationOptions)this.MemberwiseClone();
            options.RegularExpressionsToIgnore = (string[])(this.RegularExpressionsToIgnore.Clone());
            options.RegularExpressionWhitelist = (string[])(this.RegularExpressionWhitelist.Clone());
            options.RegularExpressionsToRemove = (string[])(this.RegularExpressionsToRemove.Clone());
            options.RegularExpressionsToReplace = (string[])(this.RegularExpressionsToReplace.Clone());
            options.RegularExpressionReplacements = (string[])(this.RegularExpressionReplacements.Clone());
            return options;
        }

        //bool ValidateRegularExpression(string expression)
        //{
        //    try
        //    {
        //        Regex.Matches("", expression);
        //        return true;
        //    }
        //    catch (Exception ex)
        //    {
        //        return false;
        //    }
        //}

        //bool ValidateRegularExpression(IEnumerable<string> expressions)
        //{
        //    foreach (var expression in expressions)
        //    {
        //        if (!ValidateRegularExpression(expression))
        //        {
        //            return false;
        //        }
        //    }
        //    return true;
        //}

        /// <summary>
        /// Name of the profile to save these settings to.
        /// </summary>
        [Description("Name of the profile to save these settings to.")]
        [DisplayName("Configuration Name")]
        public string ConfigurationName { get; set; }

        /// <summary>
        /// If enabled, includes the original Japanese text above the new text, commented out using the comment character.
        /// </summary>
        [Description("If enabled, includes the original Japanese text above the new text, commented out using the comment character.")]
        [DisplayName("Use Comment Character")]
        public bool UseCommentCharacter { get; set; }

        /// <summary>
        /// The character to use for commenting out original Japanese text (such as ; // or #)
        /// </summary>
        [Description("The character to use for commenting out original Japanese text (such as ; // or #)")]
        [DisplayName("Comment Character")]
        public string CommentCharacter { get; set; }

        /// <summary>
        /// The set of ascii characters which will NOT break a Japanese string apart.
        /// </summary>
        [Description("The set of ascii characters which will NOT break a Japanese string apart.")]
        [DisplayName("Continuation Characters")]
        public string ContinuationCharacters { get; set; }

        /// <summary>
        /// The set of characters which will break a Japanese string apart after reaching this.  The character is also included in the string to translate.
        /// </summary>
        [Description("The set of characters which will break a Japanese string apart after reaching this.  The character is also included in the string to translate.")]
        [DisplayName("Break After Characters")]
        public string BreakAfterCharacters { get; set; }

        /// <summary>
        /// Determines whether or not we want to post-process the text, and remove some junk.
        /// </summary>
        [Description("Determines whether or not we want to post-process the text, and remove some junk.")]
        [DisplayName("Remove Brackets and Semicolons")]
        public bool RemoveBracketsAndSemicolons { get; set; }

        /// <summary>
        /// The set of regular expressions for text to ignore.  If parts of text match any of these, it is not translated.  If you use a tagged expression, only matches inside tags will be considered.
        /// </summary>
        //[TypeConverter(typeof(ExpandableObjectConverter))]
        [Description("The set of regular expressions for text to ignore.  If parts of text match any of these, it is not translated.  If you use a tagged expression, only matches inside tags will be considered.")]
        [DisplayName("Regular Expressions to Ignore")]
        public string[] RegularExpressionsToIgnore { get; set; }

        /// <summary>
        /// The set of regular expressions which must be matched before anything is translated.  If parts of text match any of these, it is not translated.  If you use a tagged expression, only matches inside tags will be considered.
        /// </summary>
        //[TypeConverter(typeof(ExpandableObjectConverter))]
        [Description("The set of regular expressions which must be matched before anything is translated.  If parts of text match any of these, it is not translated.  If you use a tagged expression, only matches inside tags will be considered.")]
        [DisplayName("Only Regular Expressions to Translate")]
        public string[] RegularExpressionWhitelist { get; set; }

        /// <summary>
        /// The set of regular expressions to remove after text has been translated.  If parts of text match any of these, it is removed.  Does not affect text which has not been translated.
        /// </summary>
        //[TypeConverter(typeof(ExpandableObjectConverter))]
        [Description("The set of regular expressions to remove after text has been translated.  If parts of text match any of these, it is removed.  Does not affect text which has not been translated.")]
        [DisplayName("Regular Expressions to Remove*")]
        public string[] RegularExpressionsToRemove { get; set; }

        /// <summary>
        /// The set of regular expressions to replace after text has been translated.  If parts of text match any of these, it is replaced.  Does not affect text which has not been translated.
        /// </summary>
        //[TypeConverter(typeof(ExpandableObjectConverter))]
        [Description("The set of regular expressions to replace after text has been translated.  If parts of text match any of these, it is replaced.  Does not affect text which has not been translated.")]
        [DisplayName("Regular Expressions to Replace*")]
        public string[] RegularExpressionsToReplace { get; set; }

        /// <summary>
        /// The corresponding set of regular expressions to replace text with after text has been translated.
        /// </summary>
        //[TypeConverter(typeof(ExpandableObjectConverter))]
        [Description("The corresponding set of regular expressions to replace text with after text has been translated.")]
        [DisplayName("Regular Expression Replacements*")]
        public string[] RegularExpressionReplacements { get; set; }

        //private string[] _regularExpressionToIgnore;
        //private string[] _regularExpressionWhitelist;
        //private string[] _regularExpressionsToRemove;
        //private string[] _regularExpressionsToReplace;
        //private string[] _regularExpressionReplacements;

        #region ICloneable Members

        object ICloneable.Clone()
        {
            return this.Clone();
        }

        #endregion
    }
}
