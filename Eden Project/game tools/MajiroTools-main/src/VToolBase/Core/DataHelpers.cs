using System.IO;

namespace VToolBase.Core {
	public static class DataHelpers {

		public static BinaryReader NewReader(this Stream s) => new BinaryReader(s);
		public static BinaryWriter NewWriter(this Stream s) => new BinaryWriter(s);
		
		public static TextReader NewTextReader(this Stream s) => new StreamReader(s);
		public static TextWriter NewTextWriter(this Stream s) => new StreamWriter(s);
	}
}
