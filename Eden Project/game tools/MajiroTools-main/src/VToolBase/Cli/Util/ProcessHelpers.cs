using System;
using System.ComponentModel;
using System.Diagnostics;
using System.Runtime.InteropServices;

namespace VToolBase.Cli.Util {

	// Adapted from https://stackoverflow.com/a/3346055/7391324
	public static class ProcessHelpers {

		public static bool WasStartedFromExplorer() {
			return WasStartedFromExplorer(Process.GetCurrentProcess().Handle);
		}

		public static bool WasStartedFromExplorer(IntPtr handle) {
			return GetParentProcess().ProcessName == "explorer";
		}

		/// <summary>
		/// Gets the parent process of the current process.
		/// </summary>
		/// <returns>An instance of the Process class.</returns>
		public static Process GetParentProcess() {
			return GetParentProcess(Process.GetCurrentProcess().Handle);
		}

		/// <summary>
		/// Gets the parent process of a specified process.
		/// </summary>
		/// <param name="handle">The process handle.</param>
		/// <returns>An instance of the Process class.</returns>
		public static Process GetParentProcess(IntPtr handle) {
			var pbi = new ParentProcessUtilities();
			int status = NtQueryInformationProcess(handle, 0, ref pbi, Marshal.SizeOf(pbi), out _);
			if(status != 0)
				throw new Win32Exception(status);

			try {
				return Process.GetProcessById(pbi.InheritedFromUniqueProcessId.ToInt32());
			}
			catch(ArgumentException) {
				// not found
				return null;
			}
		}

		[DllImport("ntdll")]
		private static extern int NtQueryInformationProcess(IntPtr processHandle, int processInformationClass, ref ParentProcessUtilities processInformation, int processInformationLength, out int returnLength);

		[StructLayout(LayoutKind.Sequential)]
		private struct ParentProcessUtilities {
			// These members must match PROCESS_BASIC_INFORMATION
			private IntPtr Reserved1;
			private IntPtr PebBaseAddress;
			private IntPtr Reserved2_0;
			private IntPtr Reserved2_1;
			private IntPtr UniqueProcessId;
			internal IntPtr InheritedFromUniqueProcessId;
		}
	}
}
