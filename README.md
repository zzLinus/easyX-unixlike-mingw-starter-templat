# Get start
```bash
git clone git@github.com:zzLinus/easyX-unixlike-mingw-starter-template.git
git submodule init
git submodule update
mkdir build && cd build
cmake .. && make
```
# Dependency
- you need to have mingw compiler to compile easyX because they use many windows specitfic header files like `windows.h` blah blah

- For me i'm on archlinux so i just need to insall all those pacages start with mingw-w64-..... if you are on other distro, good luck then.

- Set these cmake compiler config option to suit your env
```cmake
set(CMAKE_C_COMPILER "/usr/bin/i686-w64-mingw32-gcc")
set(CMAKE_CXX_COMPILER "/usr/bin/i686-w64-mingw32-g++")
```


# btw
You need to manually copy the .a file to the defaul mingw lib path.I tried to use 'target_link_libraries(main -L/path/to/lib)' but the mingw linker can't find the library and keep give me undefine reference error, and for me i just `cd /usr/i686-w64-mingw32/lib ` and `sudo cp /path/to/3rdparty/easyx/lib-for-devcpp_5.4.0/libeasyx.a .` and it works...

If you can make the -L option works, good for you.
