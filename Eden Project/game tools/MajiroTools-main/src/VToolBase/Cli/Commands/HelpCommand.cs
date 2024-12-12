using System;
using System.IO;
using System.Reflection;
using VToolBase.Core;

namespace VToolBase.Cli.Commands {
	public class HelpCommand : Command {
		public static string ExeName = Path.GetFileNameWithoutExtension(Assembly.GetEntryAssembly()!.Location).ToLower();
		public static string AppName = ExeName.UcFirst() + "Tool Cli";
		public static string LibName = ExeName.UcFirst() + "Tool Core";

		public HelpCommand(CommandParameters parameters) : base(parameters) { }

		public override string Name => "help";

		public override string[] Description => new[] {
			"Displays general information about " + AppName
		};

		public override (string syntax, string description)[] Usage => new[] {
			("", "Display general information about " + AppName),
			("\aacommand", "Display the help page of a specific \aacommand")
		};

		public override (char shorthand, string name, string fallback, string description)[] Flags => new[] {
			('q', "quiet", "false", "Disable user-friendly output"),
			('w', "wait", "false", "Whether to wait after displaying the help page")
		};

		public override bool Execute() {
			if(Parameters.GetBool("quiet", 'q', false)) return true;

			DisplayVersionInformation();
			Output.WriteLine();

			if(Arguments.Length > 0) {
				var command = CommandManager.CreateFromName(Arguments[0], CommandParameters.Empty);
				if(command != null) {
					DisplayCommandHelp(command);
					Wait();
					return true;
				}
			}

			DisplayBasicSyntax();
			DisplayAvailableCommands();

			Output.WriteLineColored($"For more information on a specific command type \"{ExeName} help \aacommand\a-\"");

			Wait();
			return true;
		}

		public static void DisplayVersionInformation() {
			var cliVersion = Assembly.GetEntryAssembly()?.GetName().Version;
			var coreVersion = Assembly.GetAssembly(typeof(LinqHelpers))?.GetName().Version;
			Output.WriteCaption($"{AppName} v{cliVersion?.ToString(4)} running {LibName} v{coreVersion?.ToString(4)}");
		}

		public static void DisplayBasicSyntax() {
			Output.WriteCaption("Basic syntax");
			Output.WriteLineColored($"  {ExeName} \ac[flags] \aacommand \abarg1 arg2 ...");
			Output.WriteLine();
		}

		public static void DisplayAvailableCommands() {
			Output.WriteCaption("Available commands");

			foreach(var command in CommandManager.AvailableCommands) {
				Output.WriteLine("  " + command.Name, ConsoleColor.Green);

				foreach(string desc in command.Description) {
					Output.WriteLineColored("   " + desc);
				}
			}

			Output.WriteLine();
		}

		public static void DisplayCommandHelp(Command command) {
			Output.WriteLineColored($"\aeCommand \aa{command.Name}");
			foreach(string desc in command.Description) {
				Output.WriteLineColored("  " + desc);
			}
			Output.WriteLine();

			DisplayCommandUsage(command);
			DisplayCommandFlags(command);
		}

		public static void DisplayCommandUsage(Command command) {
			Output.WriteCaption("Usage");

			foreach(var usage in command.Usage) {
				Output.WriteLineColored($"  {ExeName} \aa{command.Name} \ab{usage.syntax}");
				if(usage.description != null) {
					Output.WriteLineColored("   " + usage.description);
				}
			}

			Output.WriteLine();
		}

		private static void DisplayCommandFlags(Command command) {
			var flags = command.Flags;
			if(flags.Length == 0) return;

			Output.WriteCaption("Flags");

			foreach(var (shorthand, name, fallback, description) in flags) {

				// write flag
				string info = "  \ac";
				if(shorthand != ' ') {
					info += "-" + shorthand;
				}
				if(name != null) {
					if(shorthand != ' ') info += "\a-, \ac";
					info += "--" + name;
				}
				if(fallback != null) {
					info += $" \a-(defaults to \ab{fallback}\a-)";
				}
				Output.WriteLineColored(info);

				// write description
				if(description != null) {
					Output.WriteLineColored("   " + description);
				}
			}

			Output.WriteLine();
		}
	}
}