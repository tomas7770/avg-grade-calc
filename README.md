# Average Grade Calculator

A very simple desktop app for calculating the weighted average of grades in a scale of 0 to 20.

## Build instructions

### Windows

1. Install [MSYS2](https://www.msys2.org/) and start with the **UCRT64** environment.

2. Update the packages:
   `pacman -Syu`
   You may need to run this step **more than once** to make sure that all packages are up-to-date.

3. Install build tools and dependencies:
   `pacman -S mingw-w64-ucrt-x86_64-toolchain mingw-w64-ucrt-x86_64-wxwidgets3.2-msw`

4. Download/clone this repository and `cd` to its directory.

5. Build it (dynamically linked):
   `mingw32-make`
   Alternatively, build a statically linked executable:
   `mingw32-make STATIC=1`

6. Test it (optional):
   `bin/AvgGradeCalc.exe`

7. Strip debug symbols to reduce executable size:
   `strip --strip-all bin/AvgGradeCalc.exe`

8. If you built a dynamically linked executable, you'll need to copy DLLs from `/ucrt64/bin` for the app to start outside MSYS2.

### Linux

1. Install **g++, Make, and wxWidgets 3.2 (development libraries)**. The procedure for this depends on your distro.

2. Download/clone this repository and `cd` to its directory.

3. Build it (dynamically linked):
   `make`

4. Test it (optional):
   `bin/AvgGradeCalc`

5. Strip debug symbols to reduce executable size:
   `strip --strip-all bin/AvgGradeCalc`
   
   
   
