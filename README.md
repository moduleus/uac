# UAC

## Description

The Ultrasound Acquisition Configuration file format is a structure of data to configure ultrasound acquisition.

You may use it with various languages (C++, Python and MATLAB).

[Get more informations in the Wiki](./wiki) about class diagram.

## Requirements

To use Uac library, you need to have Urx library installed.

## Table of Contents

  * Common installation and usage in [C++](#use-it-with-c), [MATLAB](#use-it-with-matlab), [Python](#use-it-with-python).
  * Build it yourself for C++, MATLAB, Python with [CMake](#build-it-with-cmake).

## Use it with C++

### C++/CMake

#### C++ installer

There is no installer for C++ user. You need to [build](#build-it-with-cmake) it yourself. If you don't need to read / write file, uac library is header-only.

#### Installed files

All CMake files will be installed in `INSTALL_PATH/lib/cmake/Uac-XXXYYY/` with:
  - XXX: the version of the Uac library (i.e. `1.1.0`),
  - YYY: empty if library has been built in shared mode, `_static` if library is static.

#### Configuration

First, you need to get Uac package with `find_package`. If package is not found, set `Uac_DIR=INSTALL_PATH/lib/cmake/Urx-XXXYYY`.

```cmake
find_package(Uac REQUIRED)
```

If package is not found, set `Uac_DIR=INSTALL_PATH/lib/cmake/Uac-XXXYYY`.

You don't need to `find_package(Urx)`. The Uac library will do it.

You will have access to targets:

  - `Uac::Uac`: header-only library of the uac struct.
  - `Uac::UacUtils`: library with helper to manipulate uac struct, including read / write in hdf5 file format.

#### Examples

```cmake
add_executable(UacTestIo ${SRCS_FILES})
target_link_libraries(
  UacTestIo PRIVATE Uac::Uac Uac::UacUtils ${HDF5_CXX_LIBRARIES})
```

`Uac::UacUtils` is only needed if you use helpers. If you only need the uac file format, only `Uac::Uac` is needed.

`HDF5_CXX_LIBRARIES` is only needed if the library has been built with HDF5 support.

You may found a complete example of `CMakeLists.txt` in `test/uac` or in `test/io` folders.

### C++ code

#### Installed files

All files will be installed in :

  - `INSTALL_PATH/include/Uac-XXXYYY/` (headers),
  - `INSTALL_PATH/lib` (dynamic libraries for Linux and static libraries for all operating system),
  - `INSTALL_PATH/bin` (dynamic libraries for Windows and executables for all operating system).

#### Examples

All dynamic memory allocation should be stored in a `shared_ptr`.

```cpp
#include <memory>

#include <uac/dataset.h>
#include <uac/probe.h>
#include <uac/utils/io/reader.h>
#include <uac/utils/io/writer.h>

auto dataset = std::make_shared<uac::Dataset>();

auto probe = std::make_shared<uac::Probe>();
probe->description = "Probe 1";
dataset->acquisition.probes.push_back(probe);

uac::utils::io::writer::saveToFile("file.uac", *dataset);

auto dataset_loaded = uac::utils::io::reader::loadFromFile("file.uac");
```

You may found an example for all C++ classes in file `uac/utils/io/test/io.h` in function `generateFakeDataset`.

## Use it with MATLAB

### MATLAB installer

Uac MATLAB homepage is hosted in [MATLAB File Exchange](https://fr.mathworks.com/matlabcentral/fileexchange/176403-ultrasound-acquisition-configuration) but binaries are hosted in [Github releases](https://github.com/moduleus/uac/releases). Be sure to click on `Show all 25 assets`, download your toolbox (`uac-X.Y.Z-linux-amd64.mltbx` for Linux or `uac-X.Y.Z-win-amd64.mltbx` for Windows) then open the toolbox with MATLAB.

You also need to install Urx toolbox from [Github releases](https://github.com/moduleus/urx/releases).

### Requirements

MATLAB needs a C compiler to load the C library in Uac toolbox.

If you have an error like `MATLAB:mex:NoCompilerFound_link_Win64` / `No supported compiler was found` / `Output from preprocessor is:'cl' is not recognized as an internal or external command, operable program or batch file.`, run `mex -setup c -v` and `mex -setup c++ -v`.

<details>
  <summary>Example of full error message</summary>

```
... Looking for compiler 'Intel Parallel Studio XE 2015 for C++ with Microsoft Visual Studio 2015' ...
... Looking for environment variable 'ICPP_COMPILER15' ...No.
Did not find installed compiler 'Intel Parallel Studio XE 2015 for C++ with Microsoft Visual Studio 2015'.
... Looking for compiler 'Intel Parallel Studio XE 2016 for C++ with Microsoft Visual Studio 2015' ...
... Looking for environment variable 'ICPP_COMPILER16' ...No.
Did not find installed compiler 'Intel Parallel Studio XE 2016 for C++ with Microsoft Visual Studio 2015'.
... Looking for compiler 'Intel Parallel Studio XE 2017 for C++ with Microsoft Visual Studio 2015' ...
... Looking for environment variable 'ICPP_COMPILER17' ...No.
Did not find installed compiler 'Intel Parallel Studio XE 2017 for C++ with Microsoft Visual Studio 2015'.
... Looking for compiler 'Intel Parallel Studio XE 2017 for C++ with Microsoft Visual Studio 2017' ...
... Looking for environment variable 'ICPP_COMPILER17' ...No.
Did not find installed compiler 'Intel Parallel Studio XE 2017 for C++ with Microsoft Visual Studio 2017'.
... Looking for compiler 'Intel Parallel Studio XE 2018 for C++ with Microsoft Visual Studio 2015' ...
... Looking for environment variable 'ICPP_COMPILER18' ...No.
Did not find installed compiler 'Intel Parallel Studio XE 2018 for C++ with Microsoft Visual Studio 2015'.
... Looking for compiler 'Intel Parallel Studio XE 2018 for C++ with Microsoft Visual Studio 2017' ...
... Looking for environment variable 'ICPP_COMPILER18' ...No.
Did not find installed compiler 'Intel Parallel Studio XE 2018 for C++ with Microsoft Visual Studio 2017'.
... Looking for compiler 'MinGW64 Compiler (C++)' ...
... Looking for environment variable 'MW_MINGW64_LOC' ...No.
Did not find installed compiler 'MinGW64 Compiler (C++)'.
... Looking for compiler 'MinGW64 Compiler with Windows 10 SDK or later (C++)' ...
... Looking for environment variable 'MW_MINGW64_LOC' ...No.
Did not find installed compiler 'MinGW64 Compiler with Windows 10 SDK or later (C++)'.
... Looking for compiler 'Microsoft Visual C++ 2013' ...
... Looking for registry setting 'HKLM\SOFTWARE\Microsoft\VisualStudio\SxS\VS7' 12.0 ...No.
... Looking for registry setting 'HKCU\SOFTWARE\Microsoft\VisualStudio\SxS\VS7' 12.0 ...No.
... Looking for registry setting 'HKLM\SOFTWARE\Wow6432Node\Microsoft\VisualStudio\SxS\VS7' 12.0 ...No.
... Looking for registry setting 'HKCU\SOFTWARE\Wow6432Node\Microsoft\VisualStudio\SxS\VS7' 12.0 ...No.
Did not find installed compiler 'Microsoft Visual C++ 2013'.
... Looking for compiler 'Microsoft Visual C++ 2015' ...
... Looking for registry setting 'HKLM\SOFTWARE\Microsoft\VisualStudio\SxS\VC7' 14.0 ...No.
... Looking for registry setting 'HKCU\SOFTWARE\Microsoft\VisualStudio\SxS\VC7' 14.0 ...No.
... Looking for registry setting 'HKLM\SOFTWARE\Wow6432Node\Microsoft\VisualStudio\SxS\VC7' 14.0 ...No.
... Looking for registry setting 'HKCU\SOFTWARE\Wow6432Node\Microsoft\VisualStudio\SxS\VC7' 14.0 ...No.
Did not find installed compiler 'Microsoft Visual C++ 2015'.
... Looking for compiler 'Microsoft Visual C++ 2017' ...
... Looking for registry setting 'HKLM\SOFTWARE\Microsoft\VisualStudio\SxS\VS7' 15.0 ...No.
... Looking for registry setting 'HKCU\SOFTWARE\Microsoft\VisualStudio\SxS\VS7' 15.0 ...No.
... Looking for registry setting 'HKLM\SOFTWARE\Wow6432Node\Microsoft\VisualStudio\SxS\VS7' 15.0 ...No.
... Looking for registry setting 'HKCU\SOFTWARE\Wow6432Node\Microsoft\VisualStudio\SxS\VS7' 15.0 ...No.
Did not find installed compiler 'Microsoft Visual C++ 2017'.
... Looking for compiler 'MinGW64 Compiler (C++)' ...
... Looking for environment variable 'MW_MINGW64_LOC' ...No.
Did not find installed compiler 'MinGW64 Compiler (C++)'.
Error using mex
No supported compiler was found. You can install the freely
available MinGW-w64 C/C++ compiler; see Install MinGW-w64
Compiler. For more options, visit
https://www.mathworks.com/support/compilers.
```

</details>

#### Use MinGW

If you want to use a free compiler, follow the next steps:

  - Download `MinGW-w64` from [https://www.mingw-w64.org/downloads](https://www.mingw-w64.org/downloads). You may use `LLVM-MinGW` at Github [https://github.com/mstorsjo/llvm-mingw/releases](https://github.com/mstorsjo/llvm-mingw/releases) and get the latest release from the file `llvm-mingw-YYYYMMDD-msvcrt-x86_64.zip`.
  - Extract it and install it on `c:\llvm-mingw-YYYYMMDD-msvcrt-x86_64` (path `c:\llvm-mingw-YYYYMMDD-msvcrt-x86_64\bin` must exist) or anywhere but don't use path with space inside.
  - In MATLAB, run:
```MATLAB
setenv('MW_MINGW64_LOC', 'C:/llvm-mingw-YYYYMMDD-msvcrt-x86_64')
mex -setup c -v
mex -setup c++ -v
```
Check you have a message `Found installed compiler 'MinGW64 Compiler (C++)'.`.

### Installed files

All MATLAB files will be installed in `C:\Users\CURRENT_USER\AppData\Roaming\MathWorks\MATLAB Add-Ons\Toolboxes\uac` :

  - `+uac` (all MATLAB script),
  - `include` (C++ header, needed to get symbols in C++ library),
  - `UacMatlabBinding.dll` (main C++ library).

### Configuration

If you are using Uac from MATLAB toolbox, you just have to enable Unicode if you need it (`feature('DefaultCharacterSet','UTF-8');`).

If you want to enable logging, set `UAC_DEBUG` environment variable to:

  - `COUT` to display message to the standard output or,
  - the filename where you want to store log messages.

If you built Uac from source, before using `Uac` in MATLAB, you first need to:

  - allow utf-8 caracters,
  - load the uac library.

```MATLAB
addpath([pwd 'INSTALL_PATH/share/Urx-XXXYYY/matlab']);
addpath([pwd 'INSTALL_PATH/share/Uac-XXXYYY/matlab']);
feature('DefaultCharacterSet','UTF-8');
urx.LibBinding.getInstance([path to libUrxMatlabBinding.so], [path to INSTALL_PATH/include/Urx-XXXYYY/urx/matlab/bindings.h], {[path to INSTALL_PATH/include/Urx-XXXYYY]}, {});
uac.LibBinding.getInstance([path to libUacMatlabBinding.so], [path to INSTALL_PATH/include/Uac-XXXYYY/uac/matlab/bindings.h], {[path to INSTALL_PATH/include/Urx-XXXYYY], [path to INSTALL_PATH/include/Uac-XXXYYY]}, {});
```

⚠ When invoking `uac.LibBinding.getInstance` the first time, environment variable `HDF5_DISABLE_VERSION_CHECK` is set. MATLAB have it's own version of HDF5 library. Under Linux it will conflict with the one from Uac.

If you try to use `uac.saveToFile(...)` or `uac.loadFromFile(...)` without setting `HDF5_DISABLE_VERSION_CHECK`, MATLAB will crash with error: `Headers are 1.8.23, library is 1.8.12`.

<details>
  <summary>Example of full error message</summary>
  
  ```
Warning! ***HDF5 library version mismatched error***
The HDF5 header files used to compile this application do not match
the version used by the HDF5 library to which this application is linked.
Data corruption or segmentation faults may occur if the application continues.
This can happen when an application was compiled by one version of HDF5 but
linked with a different version of static or shared HDF5 library.
You should recompile the application or check your shared library related
settings such as 'LD_LIBRARY_PATH'.
'HDF5_DISABLE_VERSION_CHECK' environment variable is set to 1, application will
continue at your own risk.
Headers are 1.8.23, library is 1.8.12
	    SUMMARY OF THE HDF5 CONFIGURATION
	    =================================
General Information:
-------------------
		   HDF5 Version: 1.8.12
		  Configured on: Mon May 10 15:29:10 EDT 2021
		  Configured by: batserve@bat1062601glnxa64 at The MathWorks, Inc., Natick, MA 01760
		 Configure mode: production
		    Host system: x86_64-pc-linux-gnu
	      Uname information: Linux bat1062601glnxa64 4.19.0-13-amd64 #1 SMP Debian 4.19.160-2 (2020-11-28) x86_64 GNU/Linux
		       Byte sex: little-endian
		      Libraries: shared
	     Installation point: /mathworks/devel/bat/B3p3/build/3p/install/7215999/glnxa64/hdf5-1.8
Compiling Options:
------------------
               Compilation Mode: production
                     C Compiler: /mathworks/hub/3rdparty/internal/3349624/glnxa64/gcc-6.3.0/bin/gcc ( gcc (MW GCC 6.3.0-GLIBC2.12-gold) 6.3.0)
                         CFLAGS: -O2 -pipe -pthread -fdebug-prefix-map=/mathworks/devel/bat/B3p3/build= -fPIC 
                      H5_CFLAGS:   -ansi -pedantic -Wall -W -Wundef -Wshadow -Wpointer-arith -Wbad-function-cast -Wcast-qual -Wcast-align -Wwrite-strings -Wconversion -Waggregate-return -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations -Wredundant-decls -Wnested-externs -Winline -O -fomit-frame-pointer -finline-functions
                      AM_CFLAGS: 
                       CPPFLAGS: 
                    H5_CPPFLAGS: -D_POSIX_C_SOURCE=199506L   -DNDEBUG -UH5_DEBUG_API
                    AM_CPPFLAGS: -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_BSD_SOURCE 
               Shared C Library: yes
               Static C Library: no
  Statically Linked Executables: no
                        LDFLAGS: -Wl,--as-needed -fuse-ld=gold -Wl,--disable-new-dtags -Wl,--hash-style=both -Wl,--version-script=/mathworks/devel/bat/B3p3/build/3p/sources/hdf5-1.8//hdf5_linux_exported_symbol.map
                     H5_LDFLAGS: 
                     AM_LDFLAGS: 
 	 	Extra libraries:  -lz -ldl -lm 
 		       Archiver: ar
 		 	 Ranlib: ranlib
 	      Debugged Packages: 
		    API Tracing: no
Languages:
----------
                        Fortran: no
                            C++: no
Features:
---------
                  Parallel HDF5: no
             High Level library: yes
                   Threadsafety: no
            Default API Mapping: v18
 With Deprecated Public Symbols: yes
         I/O filters (external): deflate(zlib)
         I/O filters (internal): shuffle,fletcher32,nbit,scaleoffset
                            MPE: no
                     Direct VFD: no
                        dmalloc: no
Clear file buffers before write: yes
           Using memory checker: no
         Function Stack Tracing: no
                           GPFS: no
      Strict File Format Checks: no
   Optimization Instrumentation: no
       Large File Support (LFS): yes
```
</details>

Uac must use the same version (at least major / minor number) than the one from MATLAB (here 1.8).

Why is there a problem? Under Linux, the same symbol from two differents libraries are merged. So you can't know which one will be used. By setting `HDF5_DISABLE_VERSION_CHECK` to 1, you are disabling version check. Under Windows, the operating system keeps symbols from different libraries separate.

### Examples

```MATLAB
dataset = uac.Dataset();

probe1 = uac.Probe();
probe1.description = 'Probe 1';
dataset.acquisition.excitations = [excitation1];

uac.saveToFile('test.uac', dataset);

dataset2 = uac.loadFromFile('test.uac');
```

You may found a complete example in file `test/matlab/+uac/+UnitTests/CreateUff.m`.

### Important note

Some field are optional or can be empty (see C++ UML diagram). If you want to unassign a value, don't use `[]` but use `empty` method of the class (i.e. `uac.ElementGeometry.empty`).

## Use it with Python

### Python wheel

Uac MATLAB homepage is hosted in [Pypi](https://pypi.org/project/ultrasound-acquisition-configuration/). Binaries are hosted in Pypi and [Github releases](https://github.com/moduleus/uac/releases). Be sure to click on `Show all 25 assets`, download the wheel of your python version (i.e. `ultrasound_acquisition_configuration-1.1.0-cp312-cp312-win32.whl`) then run `pip install ultrasound_acquisition_configuration-1.1.0-cp312-cp312-win32.whl`.

### Examples

```python
import ultrasound_acquisition_configuration as uac;

dataset = uac.Dataset()

probe1 = uac.Probe()
probe1.description = 'Probe 1'
dataset.acquisition.excitations = [excitation1]

uac.saveToFile('test.uac', dataset)

dataset2 = uac.loadFromFile('test.uac')
```

You may found a basic example in folder `test/python`.

## Build it with CMake

You can use CMake to build urx for C++ / MATLAB / Python user.

See `.gitlab-ci.yml` file for various examples.

### Build for Windows

Requirements:

  * [git](https://gitforwindows.org/),
  * [cmake v3.28 minimum](https://gitforwindows.org/),
  * [Python v3.8 minimum](https://www.python.org/downloads/),

Under Windows, you need to explicitly set Visual Studio generator.

`cmake.exe -S root_path -B CI -G "Visual Studio 17 2022" -A x64`

Full example with urx already built in git-bash console:

```sh
git clone https://github.com/moduleus/uac.git
cd uac
git clone https://github.com/microsoft/vcpkg.git
vcpkg/bootstrap-vcpkg.bat
git clone https://github.com/moduleus/vcpkg-registry.git
cmake -S . -B build -G "Visual Studio 17 2022" -A x64 -DCMAKE_TOOLCHAIN_FILE=vcpkg/scripts/buildsystems/vcpkg.cmake -DBUILD_SHARED_LIBS=OFF -DURX_SRC=../urx -DVCPKG_TARGET_TRIPLET=x64-wsmep -DVCPKG_HOST_TRIPLET=x64-wsmep -DVCPKG_OVERLAY_TRIPLETS=vcpkg-registry/triplets -DWITH_PYTHON:BOOL=ON -DWITH_PYTHON_WHL:BOOL=ON -DWITH_HDF5:BOOL=ON -DWITH_MATLAB:BOOL=ON -DBUILD_TESTING:BOOL=OFF -DENABLE_PCH:BOOL=OFF -DCMAKE_INSTALL_PREFIX=install
cmake --build build --config Release --parallel 4
```

### Build for Linux

`cmake -S . -B CI`

Full example with urx already built:

```sh
git clone https://github.com/moduleus/uac.git
cd uac
git clone https://github.com/microsoft/vcpkg.git
vcpkg/bootstrap-vcpkg.sh
git clone https://github.com/moduleus/vcpkg-registry.git
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=vcpkg/scripts/buildsystems/vcpkg.cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=OFF -DURX_SRC=../urx -DVCPKG_TARGET_TRIPLET=x64-lsrp -DVCPKG_HOST_TRIPLET=x64-lsrp -DVCPKG_OVERLAY_TRIPLETS=vcpkg-registry/triplets -DWITH_PYTHON:BOOL=ON -DWITH_PYTHON_WHL:BOOL=ON -DWITH_HDF5:BOOL=ON -DWITH_MATLAB:BOOL=ON -DBUILD_TESTING:BOOL=OFF -DENABLE_PCH:BOOL=OFF -DCMAKE_INSTALL_PREFIX=install
cmake --build build --parallel 4
```

### Common features

  * Install path

Set `-DCMAKE_INSTALL_PREFIX=` variable to define the installation path.

  * Debug or Release

If you use a single build type generator (like Makefile in Linux), you need to set `CMAKE_BUILD_TYPE` to `Debug` or `Release`.

  * Static or dynamic libraries

Set `BUILD_SHARED_LIBS` to `OFF` or `ON`.

Note that you should set `BUILD_SHARED_LIBS` to `OFF` if you want to build MATLAB or Python buiding.

Be sure to use a vcpkg triplet with the same shared / static link option. For example:
  - Windows and static: `x64-wsmep`
  - Windows and shared: `x64-wdrep`
  - Linux and static: `x64-lsrp`
  - Linux and shared: `x64-ldr`

  * Use external 3rd party with vcpkg

`vcpkg` is a package manager. You first need to clone it with `git clone https://github.com/microsoft/vcpkg.git`.

Also, clone `vcpkg-registry` with `git clone https://github.com/moduleus/vcpkg-registry.git`.

Then pass the arguments below. ⚠ If you use `cmake-gui`, be sure to set these variables BEFORE running configure once. Otherwise, you need to reset cache.

Windows:
```cmake
-DCMAKE_TOOLCHAIN_FILE="vcpkg_path/scripts/buildsystems/vcpkg.cmake"
-DVCPKG_TARGET_TRIPLET=x64-wsmep -DVCPKG_HOST_TRIPLET=x64-wsmep
-DVCPKG_OVERLAY_TRIPLETS=vcpkg-registry/triplets"
```

Linux with static libraries (make sure that BUILD_SHARED_LIBS is set to OFF).
```cmake
-DCMAKE_TOOLCHAIN_FILE="vcpkg_path/scripts/buildsystems/vcpkg.cmake"
-DVCPKG_TARGET_TRIPLET=x64-linux -DVCPKG_HOST_TRIPLET=x64-linux
-DVCPKG_OVERLAY_TRIPLETS=vcpkg-registry/triplets
```

We provide various triplet with following syntax:

  - `w` for Windows,
  - `l` for Linux,
  - `d` for Linux dynamic (will be static otherwise),
  - `sm` for Windows static with /MD option (will be dynamic otherwise). See [VCPKG_CRT_LINKAGE](https://learn.microsoft.com/en-us/vcpkg/users/triplets#vcpkg_crt_linkage),
  - `e` to pass environment variable for Windows. See [VCPKG_ENV_PASSTHROUGH](https://learn.microsoft.com/en-us/vcpkg/users/triplets#vcpkg_env_passthrough),
  - `p` to pass Python3_EXECUTABLE to vcpkg via `VCPKG_Python3_EXECUTABLE` environment variable.

  * Urx from vcpkg

If you want to have urx from vcpkg, you need to set `WITH_VCPKG_URX:BOOL=ON` CMake variable. See various examples in `.gitlab-ci.yml`.

If you want to have urx from submodule, you need to set `URX_SRC=urx` CMake variable. `URX_SRC` is the path of the urx folder. See `build:ubuntu-without-3rd-party` in `.gitlab-ci.yml`.

If you want to have urx from `find_package`, you need to set `Urx_DIR` CMake variable. `Urx_DIR` is the path of the CMake files in the Urx install path.

  * Enable HDF5

This feature is needed to read / write uac structure to file.

Set `WITH_HDF5` to `OFF` or `ON`.

  * Enable Python binding

Set `WITH_PYTHON` to `OFF` or `ON`.

You can also set `WITH_PYTHON_WHL` to `ON` if you want to generate wheel package without using `pip wheel`.

  * Enable MATLAB binding

Set `WITH_MATLAB` to `OFF` or `ON`.

You don't need to have MATLAB installed on your computer to build the MATLAB binding.

  * Build with Clang

You need to set lots of CMake variables and use CMake 3.29 if you want to use `CMAKE_LINKER_TYPE` variable.

```cmake
-DCMAKE_C_COMPILER=/usr/bin/clang-17
-DCMAKE_CXX_COMPILER=/usr/bin/clang++-17
-DCMAKE_AR=/usr/bin/llvm-ar-17
-DCMAKE_AS=/usr/bin/llvm-as-17
-DCMAKE_RANLIB=/usr/bin/llvm-ranlib-17
-DCMAKE_LINKER_TYPE=LLD
```

You may found various command to build Uac in file `.gitlab-ci.yml`.

## Build Python module with pip wheel

### Using vcpkg

You may generate wheel archive by launching `pip wheel .` from the root directory of the project.

The `setup.py` file support the following options:

  - `CMAKE_TOOLCHAIN_FILE` to set vcpkg path (repository cloned if option is not given),
  - `cmake_build_type` to set CMAKE_BUILD_TYPE in CMake (Release by default),
  - `vcpkg_triplet` to set vcpkg triplet (guessed if omited),

You can also set CMake variable with `-DVARIABLE=VALUE`.

Example:

```sh
python.exe -m pip install .
--config-settings="--global-option=CMAKE_TOOLCHAIN_FILE=vcpkg_path/scripts/buildsystems/vcpkg.cmake"
--config-settings="--global-option=cmake_build_type=RelWithDebInfo"
--config-settings="--global-option=vcpkg_triplet=x64-wsmrep"
--config-settings="--global-option=-DBUILD_SHARED_LIBS=OFF"
--config-settings="--global-option=-DWITH_HDF5=ON"
-v
```

See `build:windows-python` and `build:ubuntu-python` in `.gitlab-ci.yml` file for real examples.

### Without vcpkg

If you want to disable vcpkg and use you own 3rd party library (that you already built), you can set `DISABLE_VCPKG` to `ON` and set all `XXX_DIR` CMake variable (path of the CMake files of the 3rd party).

See CMake target `UacPythonWheel` in `python/CMakeLists.txt` file for real example.
