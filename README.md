![Captura de pantalla (8)](https://github.com/user-attachments/assets/62e06601-0c50-4411-bf35-9e4e3abfb94f)
You configure a struct in the C++ source and run build.bat.  This batch file needs both cl and lilypond in the path:
It compiles the source, runs the resulting executable to output a .ly file, and processes that .ly file with lilypond.
You'll get a sheet music pdf with random notes, in the build folder.  I use this to practice sight reading.

There's an example output of this process in the build directory, 
including the executable, which could trigger a false positive from your antivirus.
You could of course download just the code folder.
