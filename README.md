## Dependencies
* Git
* Unity (Recommended version is 6000.0.41f1)
* Visual Studio 2022 (for debugging Unity's C# scripts) 
* Visual Studio Code (for debugging a C++ dynamic link library)
* PkgConfig (for building with GStreamer)

## Preparation

### Unity
Add the module of Android Build Support.

### Install Visual Studio 2022
Addt the work load of development C++ for desktop application

### Visual Studio Code
Add the following extensions.

### PkgConfig
(I haven't tried other schemes.)

## How to debug
### Unity's C# scriptts

### C++ dynamic link library
Go to the project root directory and follow the instruction.
``` bash
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.bat
vcpkg install pkgconf
```
Add the path to the environment variable