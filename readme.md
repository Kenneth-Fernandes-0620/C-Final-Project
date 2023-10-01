## Install MSYS2  
Link for download: https://www.msys2.org/

## Setup Gtk4
open msys2 annd run the following command  
*pacman -S mingw-w64-x86_64-gtk4*

## Setup Environment Variables
Add the following to the environment path  
- _**'C:\msys64\mingw64\bin'**_  
- _**'C:\msys64\mingw64\include'**_  
- _**'C:\msys64\mingw64\lib'**_  

## Update VsCode's Includepath for Syntax Highlighting
c_cpp_properties.json
```
{
    "configurations": [
        {
            "name": "Win32",
            "includePath": [
                "${workspaceFolder}/**",
                "C:/msys64/mingw64/lib/**",
                "C:/msys64/mingw64/include/**",
                "C:/msys64/mingw64/bin/**"
            ],
            "defines": [
                "_DEBUG",
                "UNICODE",
                "_UNICODE"
            ],
            "compilerPath": "C:/msys64/mingw64/bin/gcc.exe",
            "cStandard": "c11",
            "intelliSenseMode": "windows-gcc-x64"
        }
    ],
    "version": 4
}
```

## Build and Run
Run the Below Command to build your exe file 
```
gcc -Wall `pkg-config --cflags gtk4` -o output_file.exe c_file.c `pkg-config --libs gtk4`
```
To run the file, use ./output_file 