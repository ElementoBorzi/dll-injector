# dll-injector
Allows you to inject one or more dlls into an executable exe file

## How to?
The purpose of program - reduce the number of additional clicks for injection dll in the program to 0

Advantages:

Inject multiple files.

Inject libraries in the folder by folder name.

Run the program with passing cmdline to it.


Usage (with cmdline):

Game.exe appname MyLib1 MyLib2 MyLib3 cmdline cmdline

Game.exe argname foldername appname MyLib

Game.exe MyLib appname foldername

Usage (with renaming injector): (cmdline fully pass to app)

appname_MyLib1_MyLib2_MyLib3_cmdline_cmdline.exe

argname_foldername_appname_MyLibs.exe

MyLib_appname_foldername.exe