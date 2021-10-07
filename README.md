# CF_Tool

##Development Instructions
- Install MinGW 64 bit compiler using msys64.
- Install Qt static library using msys64
- Install Qt Creator and load the .pro file located in this repository.
- Inside Qt Creator, go to Tools > Options.
- Point Qt Creator's compiler to the MinGW64 cmake.exe (It is located at c:\msys64\mingw64\bin\cmake.exe on my machine).
- Point Qt Creator's Qt Version to the Qt static library qmake.exe (It is located at c:\msys64\mingw64\qt5-static\bin\qmake.exe on my machine).
- Set up the Qt Creator kit to point at the new cmake and qmake installations.
- Compile the project.
- Download the latest release from this repo and copy your compiled executable over the downloaded exe.
- The executable should now run on any 64-bit machine.
