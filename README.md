# Get start
```bash
git clone git@github.com:zzLinus/easyX-unixlike-mingw-starter-template.git
git submodule init
git submodule update
make
wine main.exe
```
# Dependency
- you need to have mingw compiler to compile easyX because they use many windows specitfic header files like `windows.h` blah blah

- For me i'm on archlinux so i just need to insall all those pacages start with mingw-w64-..... if you are on other distro, good luck then.

- Set these cmake compiler config option to suit your env
```cmake
set(CMAKE_C_COMPILER "/usr/bin/i686-w64-mingw32-gcc")
set(CMAKE_CXX_COMPILER "/usr/bin/i686-w64-mingw32-g++")
```
