# Compiling on Windows 
Describing the steps to compile the source code on Windows machines

## Requirements

Make sure you have installed the Microsoft Visual C++ Build Tools. It's installed with Visual Studio by default. 
Please see : https://docs.microsoft.com/en-us/cpp/build/walkthrough-compiling-a-native-cpp-program-on-the-command-line?view=vs-2019

Open the Developer command prompt and navigate to the root folder of the source code. You shoudl see the content below on the root folder: 

```

C:\europa>dir
 Volume in drive C is OSDisk
 Volume Serial Number is B0F3-D59D

 Directory of C:\europa

20/01/2020  22:40    <DIR>          .
20/01/2020  22:40    <DIR>          ..
19/01/2020  19:07               554 .gitignore
20/01/2020  22:40    <DIR>          bin
20/01/2020  22:41    <DIR>          doc
15/01/2020  19:02               237 Makefile
20/01/2020  22:40               211 Makefile_win
20/01/2020  22:14    <DIR>          src
15/01/2020  18:48    <DIR>          test
15/01/2020  18:48    <DIR>          _test
               5 File(s)          1,013 bytes
               8 Dir(s)  61,276,880,896 bytes free

```


## Compiling 

Run the command below to compile the source code. The parameter "/NOLOGO" it's optional.

```
nmake /NOLOGO /F Makefile_win
```

A file "europa.exe" will be generated under the "bin" folder.

## Cleanning 

To cleanup the files generate by compilation use: 

```
nmake /NOLOGO /F Makefile_win clean
```

Remove all *.obj and bin/europa.exe files