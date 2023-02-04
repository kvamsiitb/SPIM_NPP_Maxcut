## Introduction
This project uses all components created by running "HOLOEYE SLM Display SDK (C and C++) v2.0.1 r3973 Installer.exe". TO run this project you require cmake

- cmake(preferrable 3.10 and above version).

## Running the Camera
To run this Camera
- Open Developer Command Prompt for VS2015
- cmake -G"Visual Studio 14 2015" -Ax64 -DCMAKE_BUILD_TYPE=Release ..
- cmake --build . --clean-first --config RELEASE --target INSTALL

## Running the SLM
To run this SLM
- run "HOLOEYE SLM Display SDK (C and C++) v2.0.1 r3973 Installer.exe"
- Go to "C:\Program Files\HOLOEYE Photonics\SLM Display SDK (ANSI C) v2.0.1"
- copy win32 and win64 folder 
- Go to Project\slm_display\lib and copy both the folders
- Go to the project folder and create a build folder using "mkdir build"
- cd build
#- cmake -Ax64 ..
- "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat"
- D:\Intel\SPIM\SPIM\ising_machine\build> cmake -G"Visual Studio 16 2019" -Ax64 -DCMAKE_PREFIX_PATH=G:\opencv\install\opencv\build -DCMAKE_BUILD_TYPE=Release ..
- cmake --build . --clean-first --target INSTALL --config Release
- Go back to the project folder and open bin folder and copy both dlls present in the 
Project\slm_display\lib\winXX\bin
- click on SLMTARGET.exe

## Issue
Issue pending
- copying manual libs and dll need to be done
