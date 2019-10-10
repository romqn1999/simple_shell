# simple_shell
simple shell run on unix, write in C

### Prerequisites
  - Complier for `C`, I use `gcc`
    ```console
    $ gcc --version
    gcc (Ubuntu 7.4.0-1ubuntu1~18.04.1) 7.4.0
    Copyright (C) 2017 Free Software Foundation, Inc.
    This is free software; see the source for copying conditions.  There is NO
    warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
    ```
  - CMake, minimum required version is 3.9
    ```console
    $ cmake --version
    cmake version 3.10.2
    ```
  
### Setup
  - Open `./simple_shell` directory in `terminal`.
  - Create `./build` directory.
    ```console
    $ mkdir build
    ```
  - Use CMake to generate your `makefiles`. We put these temporaries under a `./build` directory.
    ```console
    $ cd build
    $ cmake ..
    ```
  - Return to `./simple_shell` directory and run `make` in the `./build` directory to build project.
    ```console
    $ cd ..
    $ make -C build
    ```
  - Run `simple_shell` in the `./build` directory to run project.
    ```console
    $ ./build/simple_shell
    ```
### Note
  On Window, install Cygwin to build. Cygwin is:
  - a large collection of GNU and Open Source tools which provide functionality similar to a Linux distribution on Windows.
  - a DLL (cygwin1.dll) which provides substantial POSIX API functionality.
