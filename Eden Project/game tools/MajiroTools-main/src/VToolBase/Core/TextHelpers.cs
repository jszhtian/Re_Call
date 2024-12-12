using System.Linq;
using System.Text.RegularExpressions;

namespace VToolBase.Core {
	public static class TextHelpers {

		public static string UcFirst(this string str) =>
			string.IsNullOrEmpty(str) ? str : char.ToUpper(str[0]) + str[1..].ToLower();

		public static string UcWords(this string str) =>
			str.Split(' ').Select(UcFirst).JoinString(", ");

		public static string Escape(this string value) {
			return value.Replace(@"\", @"\\").Replace(@"""", @"\""").Replace("\n", @"\n").Replace("\r", @"\r").Replace("\t", @"\t");
		}

		private static readonly Regex UnescapeRegex = new Regex(@"\\(.)");

		public static string Unescape(this string value) {
			return UnescapeRegex.Replace(value, match => {
				switch(match.Groups[1].Value[0]) {
					case '\\': return "\\";
					case 'r': return "\r";
					case 'n': return "\n";
					case 't': return "\t";
					case '"': return "\"";
					default: return match.Groups[1].Value;
				}
			});
		}

		public static string Humanize(ulong value) {
			if(value > 10_000_000_000_000)
				return $"{(double)value / 1_000_000_000_000:F2}t";
			if(value > 10_000_000_000)
				return $"{(double)value / 1_000_000_000:F2}b";
			if(value > 10_000_000)
				return $"{(double)value / 1_000_000:F2}m";
			if(value > 10_000)
				return $"{(double)value / 1_000:F2}k";
			return value.ToString();
		}
	}
}
