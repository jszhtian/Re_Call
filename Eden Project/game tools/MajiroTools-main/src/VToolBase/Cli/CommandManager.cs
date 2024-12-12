using System;
using System.Collections.Generic;
using System.Linq;
using VToolBase.Cli.Commands;
using VToolBase.Core;

namespace VToolBase.Cli {
	public static class CommandManager {

		private static readonly Dictionary<string, Func<CommandParameters, Command>> CommandFactories = new Dictionary<string, Func<CommandParameters, Command>> {
			{"help", parameters => new HelpCommand(parameters) },
		};


		public static void Register(string name, Func<CommandParameters, Command> factory) {
			CommandFactories[name] = factory;
		}

		public static IEnumerable<Command> AvailableCommands => CommandFactories.Values.Select(factory => factory(CommandParameters.Empty));

		public static Command CreateFromName(string name, CommandParameters parameters) {
			return CommandFactories.TryGet(name, _ => null)(parameters);
		}

		public static bool TryRun(params string[] cliArgs) {
			var commandLine = CommandParameters.ParseArguments(cliArgs);
			if(commandLine.Arguments.Count == 0) return false;

			var command = CreateFromName(commandLine.Arguments[0], commandLine);
			if(command == null) return false;

			try { 
				command.Execute();
			}
			catch(Exception e) {
				Output.WriteLineColored($"Command \aa{command.Name}\a- failed with an error:");
				Output.WriteException(e);
			}
			return true;
		}
	}
}
