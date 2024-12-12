using System;
using System.Collections.Concurrent;
using System.Diagnostics;
using System.Globalization;
using System.IO;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace VToolBase.Cli {
	public static class Output {

		private static readonly BlockingCollection<(string text, ConsoleColor foregroundColor, ConsoleColor backgroundColor)> WriteQueue = new BlockingCollection<(string, ConsoleColor, ConsoleColor)>();
		private static readonly ConsoleColor DefaultForegroundColor = Console.ForegroundColor;
		private static readonly ConsoleColor DefaultBackgroundColor = Console.BackgroundColor;

		private static readonly AutoResetEvent QueueEmpty = new AutoResetEvent(false);
		private static readonly SemaphoreSlim FlushEvent = new SemaphoreSlim(0);

		static Output() {
			Task.Run(() => {
				foreach(var (text, foregroundColor, backgroundColor) in WriteQueue.GetConsumingEnumerable()) {
					if(text.StartsWith('\0')) {
						switch(text) {
							case "\0ClearLine":
								Console.CursorLeft = 0;
								Console.Write(new string(' ', Console.WindowWidth));
								Console.CursorLeft = 0;
								break;
							case "\0Flush":
								FlushEvent.Release();
								break;
						}
						continue;
					}
					Console.ForegroundColor = foregroundColor;
					Console.BackgroundColor = backgroundColor;
					Console.Write(text);
					Console.ResetColor();
				}
				QueueEmpty.Set();
			});
		}

		public static void Terminate() {
			WriteQueue.CompleteAdding();
			QueueEmpty.WaitOne();
		}

		public static void Flush() {
			Write("\0Flush");
			FlushEvent.Wait();
		}

		public static void ClearLine() => Write("\0ClearLine");

		#region Text output methods

		#region Write

		public static void Write(string text, ConsoleColor foregroundColor, ConsoleColor backgroundColor)
			=> WriteQueue.Add((text, foregroundColor, backgroundColor));

		public static void Write(string text, ConsoleColor foregroundColor)
			=> Write(text, foregroundColor, DefaultBackgroundColor);

		public static void Write(string text)
			=> Write(text, DefaultForegroundColor);

		#endregion

		#region WriteLine

		public static void WriteLine(string text, ConsoleColor foregroundColor, ConsoleColor backgroundColor)
			=> Write(text + Environment.NewLine, foregroundColor, backgroundColor);

		public static void WriteLine(string text, ConsoleColor foregroundColor)
			=> Write(text + Environment.NewLine, foregroundColor, DefaultBackgroundColor);

		public static void WriteLine(string text)
			=> Write(text + Environment.NewLine, DefaultForegroundColor);

		public static void WriteLine()
			=> Write(Environment.NewLine);

		#endregion

		public static void WriteCaption(string text)
			=> WriteLine(text, ConsoleColor.Yellow);

		public static void WriteColored(string text) {
			var foregroundColor = DefaultForegroundColor;
			var backgroundColor = DefaultBackgroundColor;
			var sb = new StringBuilder(text.Length);

			for(int i = 0; i < text.Length; i++) {
				char ch = text[i];
				switch(ch) {

					case '\a' when i < text.Length - 2:
						if(sb.Length > 0) {
							Write(sb.ToString(), foregroundColor, backgroundColor);
							sb.Clear();
						}
						foregroundColor = text[++i] == '-' ? DefaultForegroundColor : (ConsoleColor)int.Parse(text[i].ToString(), NumberStyles.HexNumber);
						break;

					case '\b' when i < text.Length - 2:
						if(sb.Length > 0) {
							Write(sb.ToString(), foregroundColor, backgroundColor);
							sb.Clear();
						}
						backgroundColor = text[++i] == '-' ? DefaultBackgroundColor : (ConsoleColor)int.Parse(text[i].ToString(), NumberStyles.HexNumber);
						break;

					default:
						sb.Append(ch);
						break;
				}
			}
			if(sb.Length > 0) Write(sb.ToString(), foregroundColor, backgroundColor);
		}

		public static void WriteLineColored(string text)
			=> WriteColored(text + Environment.NewLine);

		#endregion

		public static void WriteStackTrace(Exception exception) {
			var stackTrace = new StackTrace(exception, true);
			foreach(var frame in stackTrace.GetFrames()) {
				if(frame == null) continue;
				var method = frame.GetMethod();
				if(method == null) continue;
				var type = method.DeclaringType;
				if(type == null) continue;

				WriteColored($"   at \ab{type.Name}\a-.\aa{method.Name}");

				string file = frame.GetFileName();
				if(file != null) {
					WriteColored($" in \ab{Path.GetFileName(file)}\a-:\aa{frame.GetFileLineNumber()}");
				}

				WriteLine();
			}
		}

		public static void WriteException(Exception exception) {
			WriteLineColored($"\ac{exception.Message.Replace("\a-", "\ac")}");
			WriteStackTrace(exception);
			if(exception.InnerException != null) {
				Write("Caused by: ", ConsoleColor.Red);
				WriteException(exception.InnerException);
			}
		}
	}
}
