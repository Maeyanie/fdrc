# fdrc

`fdrc` a fork of `fstl` modified to read Draco files.

`fstl` was originally written by [Matt Keeter](https://mattkeeter.com),
and is now primarily maintained by [@DeveloperPaul123](https://github.com/DeveloperPaul123).  
The modifications in this fork are by Maeyanie.

fdrc is designed to quickly load and render very high-polygon models;
showing 2 million triangles at 60+ FPS on a mid-range laptop.

## Setting `fdrc` as the Default Draco Viewer

### Windows

1. Right-click a Draco file
2. Select `Open With` >>> `Choose another app`
3. Select `More Apps` and `Look for another app on this PC`
4. Enter the path to the `fdrc` EXE file

### Linux

If `mimeopen` is available on your system, it can be used to set `fdrc` as the default viewer for Draco files.
Run the following in your terminal:

```bash
# replace example.drc with an actual file
mimeopen -d example.drc
```

The following output (or something vaguely similar) will result:

```
Please choose a default application for files of type model/drc

	1) Other...

use application #
```

Select the `Other` option and type `fdrc` as the desired command to open Draco files.
This will now become the system default, even when opening files from the file manager.

## Building

`fdrc` depends on [Qt 6](https://www.qt.io)
and [`Draco`](https://github.com/google/draco), 
plus [`cmake`](https://cmake.org/) for building.

Note: The below instructions are similar to `fstl` but not identical.
`fdrc` migrated from Qt 5 to Qt 6, and also requires `draco` installed in the `fdrc` source directory.
(I'm aware that's bad practice, I'll fix it and make it a proper package search eventually.)

### Linux

Install Qt with your distro's package manager (required libraries are Core, Gui,
Widgets, OpenGL, and OpenGLWidgets).

You can build fdrc with CMake:
```
git clone https://github.com/Maeyanie/fdrc
git clone https://github.com/google/draco
cd draco
mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=../../fdrc/draco
make -j8
make install
cd ../../fdrc
mkdir build
cd build
cmake ..
make -j8
./fdrc
```

--------------------------------------------------------------------------------

# License

Copyright (c) 2025 Maeyanie  
Copyright (c) 2014-2017 Matthew Keeter 

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
