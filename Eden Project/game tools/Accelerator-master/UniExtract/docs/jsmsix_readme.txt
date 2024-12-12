MSI Unpacker from JSWare

_________________________________________________________
Introduction:
_________________________________________________________

  jsMSIx.exe is a compiled executable version of the JSWare MSI
Unpacker tools. This version is designed for use by people who
want it simple... who may be intimidated by script and HTAs, or
who just don't want to deal with that complexity. 

  Also, jsMSIx.exe is designed to work on all Windows versions.
The scripted unpackers require some editing of the code to
work on Windows Vista/7.

  This version may also be the easiest for people using Linux. 
It has minimal dependencies and is easy to use.

_________________________________________________________
Requirements: 
_________________________________________________________

 System files MSVBVM60.DLL, CABINET.DLL and MSI.DLL

  Those 3 files are pre-installed on nearly all Windows PCs. jsMSIx.exe
should work as a standalone program on all Windows versions from
Win98 to Win7. But on Linux those 3 files might need to be installed
through WINE.

_________________________________________________________
How to use jsMSIx.exe:
_________________________________________________________


• _________ Command line _________________
 
jsMSIx.exe MSIPath
jsMSIx.exe MSIPath|FolderPath

The first option will unpack the MSI file to its own parent folder.
The second option, using a "pipe" character, will unpack the MSI
to [FolderPath].

Examples: 

C:\jsMSIx.exe C:\fol1\some.msi
C:\jsMSIx.exe C:\fol1\some.msi|C:\fol2


• __________ Normal operation ______________

  jsMSIx.exe can be used as a normal program by simply running
it. In the Unpacker window there is an option to select an MSI
or MSM file, as well as a folder path for unpacking.

Click "GetMSI Info" to see a list of files and Registry settings in
the MSI or MSM.

Click "Unpack MSI file" to fully unpack the MSI or MSM and write
a log file documenting the files and Registry settings.

** NOTE: With very large files an unpacking or info. retrieval 
operation can take several minutes. For example, platform SDK 
MSIs typically record information for dozens of CAB files, each
containing dozens of files. The program needs to be allowed to
finish without interruption, and without excessive CPU demand
from other software.

_________________________________________________________
Other MSI Unpacking options:
_________________________________________________________

  In addition to jsMSIx there are three script options, which are less
polished but more flexible -- for people who might want to customize
the tools.

1) MSI Unpacker - An HTA (scripted webpage) tool to unpack MSIs
and MSMs. The MSI Unpacker also provides the ability to inspect
the components and features of an installer file.

2) One-click unpacker script - Like the MSI Unpacker, but with no
interface. It's just a VBScript that unpacks MSI and MSM files.

3) jsMSI.dll - A COM DLL that can be used by script or any COM-
compatible programming language. jsMSI.dll provides a full set of
easy functions to manage MSI/MSM files. Among the functions is
a function to unpack MSIs/MSMs. There are also dozens of functions
for editing the MSI database: Create tables and records. Change
values. etc. jsMSI.dll is a "wrapper". I wraps the clunky and confusing
MSI API, with its pseudo-SQL syntax, and provides simple functions
in their place.

  jsMSIx.exe and the three options listed above all unpack an MSI in 
basically the same way. They create folders to mirror the folder 
structure used when the software is actually installed. They then 
unpack the MSI files into their respective folders. They also write 
an extensive log file that details all files in the package and lists Registry 
settings that will be made when the MSI installation is run. 

_________________________________________________________
Explanation of Registry settings:
_________________________________________________________

The Registry settings are documented in the following manner, with
the intention that anyone who wants to design their own custom
installer from an MSI can easily "auto-parse" the unpacker's log file 
-- using script or some other tool -- and write the Registry settings
themselves with minimal effort.

Each setting is on 1 line and goes like this:

Path•Value•Type

Path is the path in the Registry.
Value is the value to set.
Type is the type of value. (String, Binary, DWord, etc.)
Each part is separated by a bullet character. (ASCII Chr 149)

Example: HKCU\Software\AceAndAcme\Settings\Theme•Traditional•SZ

_________________________________________________________
Troubleshooting:
_________________________________________________________

  The command line operation cannot provide error information,
but in general any errors will be documented in the unpacking
log file.
 
   Note that some MSI files are put inside EXE files. jsMSIx.exe can
unpack those MSIs, but they must first be extracted from the
EXE. Usually a good way is to open the TEMP folder, run the installer
EXE, fish the MSI out of TEMP, then cancel the install.

  Another installer variation that is more recent is an MSI file that
unpacks its own files before running. The MSI is inside a self-
executing CAB file, which unpacks the files on its own, as part of
the CAB SFX extraction process, into a folder tree in the TEMP folder! 
In those cases, jsMSIx.exe is still useful to document the files and 
Registry settings, but it needs access to the MSI file inside the SFX 
CAB.

_________________________________________________________

*************  UPDATE HISTORY: **********************
_________________________________________________________

____________________ Update July, 2011 ___________

  A very minor update to fix a small bug: The program would
sometimes crash if an MSI had a Registry table, but that table
had no items.


 ___________________ Update January, 2011 _______

   This is a minor update to deal with faulty MSIs. In rare cases the MSI 
unpackers here may have trouble with a particular MSI due to its being 
faulty. This update follows the discovery of such an MSI. 
The technical explanation:

The MSI documentation states that the MSI Directory table, listing folders 
in the install, has 3 columns:

Directory - unique ID of folder
Directory_Parent - unique ID of parent folder.
DefaultDir - name of folder when installed.

  The docs further state that there can be only 1 root folder. That folder has 
Directory property set to TARGETDIR and DefaultDir set to SourceDir. If 
there is no TARGETDIR then the ROOTDRIVE property is used. The docs also 
state that if the Directory_Parent property is the same as Directory, or if it 
is blank, then that entry represents the root folder. Up until now we were 
only aware of 1 case of a faulty MSI Directory table. It had two TARGETDIR 
entries and, oddly enough, was authored by a very well-known Microsoft MVP. 
More recently a second case has turned up, this time from Microsoft themselves. 
The file name is NetworkMonitor_Parsers.msi. It comes packed inside the Network 
MS Monitor software download. In NetworkMonitor_Parsers.msi Directory table 
there is the following:


TARGETDIR	NPLROOT		PackDir|NetworkMonitor Parsers
Base				SourceDir

  TARGETDIR is a keyword that should denote the root folder but it is not the root 
folder. "TARGETDIR" was just used as the alphanumeric ID of one of the folders! The 
root folder was named Base. Without figuring out the root folder name, unpacking 
ends up being partial at best, so this problem needed to be dealt with.

   The update involves a rewrite of the function -- in all of the unpackers -- that 
performs the job of sorting out installation folder paths. The new versions will 
look for a valid entry that has "SourceDir" in the 3rd column, with a 2nd column 
that is either blank or matches the first column. Only if no such match is found 
will it then check for entries with TARGETDIR or ROOTDRIVE in the first column.

____________________________________________

__________________ Update 11-29-2010 _________

 A minor update that allows for unpacking MSIs where there are
multiple CAB files with spanning. (Where 1 stored file is stored across
2 CAB files.)

____________________________________________

___________________ Update 11-21-2010 ________

  This is a minor, single bug fix. There was a typo in the code
that could cause a crash in some cases when clicking "Get MSI Info."
That was fixed.

_________________________________________________________

License:

You use all script code and components from JSWare at your own risk.

  The components (compiled DLL and EXE files) may be used for personal or
commercial purposes. No payment or attribution is required for either use.
The components may be redistributed if they are required as support files 
for scripts or software that you have written.
   Also, the script code may be used freely, in part or as whole scripts,
for any purpose, personal or commercial, without payment or attribution.

  I ask only that you not redistribute these scripts and components, except
as required for your direct use. Instead, please direct others to obtain copies
of JSWare scripts and components directly from www.jsware.net.

  Also, none of the code here may be redistributed under another license. If a 
work using code from JSWare is distributed with restrictions of any kind 
the code from JSWare must be kept exempt from those restrictions. 
This includes, but is not limited to, code sold for profit, code with usage restrictions
and code distributed as so-called "Open Source" with redistribution restrictions. 

                                         Joe Priestley


JSWare
www.jsware.net
jsware@jsware.net  

Please note: JSWare does not accept "webmail" from hotmail, 
yahoo, facebook, or gmail. For further explanation see:
www.jsware.net/jsware/contact.php5
