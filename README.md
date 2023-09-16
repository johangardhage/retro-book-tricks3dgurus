# retro-book-tricks3dgurus

Demos from Tricks of the 3d Game Programming Gurus, brought back from the 90's.

![Screenshot](/screenshots/demo10_2.png "demo10_2")

## Prerequisites

To build the demo programs, you must first install the following tools:

- [GCC](https://gcc.gnu.org/)
- [Ninja](https://ninja-build.org/)
- [SDL2](https://www.libsdl.org/)

### Install dependencies

#### openSUSE

`$ sudo zypper install ninja gcc-c++ libSDL2-devel`

#### Ubuntu

`$ sudo apt install ninja-build g++ libsdl2-dev`

#### Windows

* Manually download and install [MSYS2](https://www.msys2.org/).
* Run `View advanced system settings` from the Start menu and add a PATH to `C:\msys64\mingw64\bin`.
* Run `MSYS2 MSYS` from the Start menu. Update the package database and base packages with `pacman -Syu`.
* Run `MSYS2 MSYS` from the Start menu again. Update the rest of the base packages with `pacman -Syu`.
* Install the development tools with `pacman -S git mingw-w64-x86_64-gcc mingw-w64-x86_64-ninja mingw-w64-x86_64-SDL2`.
* Close the `MSYS2 MSYS` window and run `MSYS MinGW 64-bit` from the Start menu.
* Clone the git repository with `git clone https://github.com/johangardhage/retro-book-gardens.git`.
* Finally, to be able to build the demos on Windows, edit the file `build.ninja` and uncomment the line [#  command = $cc $in $windows -o $out.](build.ninja#L10).

## Build instructions

To build the demo programs, run:

`$ ninja`

A `build` directory will be created, containing the demo programs.

## Usage

```
Usage: demo [OPTION]...

Options:
 -h, --help           Display this text and exit
 -w, --window         Render in a window
     --fullwindow     Render in a fullscreen window
 -f, --fullscreen     Render in fullscreen
 -v, --vsync          Enable sync to vertical refresh
     --novsync        Disable sync to vertical refresh
 -l, --linear         Render using linear filtering
     --nolinear       Render using nearest pixel sampling
 -c, --showcursor     Show mouse cursor
     --nocursor       Hide mouse cursor
     --showfps        Show frame rate in window title
     --nofps          Hide frame rate
     --capfps=VALUE   Limit frame rate to the specified VALUE
```

## License

Licensed under MIT license. See [LICENSE](LICENSE) for more information.

## Authors

Original code written by Andre LaMothe for [Tricks of the 3d Game Programming Gurus](https://www.amazon.com/Tricks-Programming-Gurus-Advanced-Graphics-Rasterization/dp/0672318350/) (Sams)

## Screenshots

![Screenshot](/screenshots/demo8_6.png "demo8_6")
![Screenshot](/screenshots/demo9_2.png "demo9_2")
![Screenshot](/screenshots/demo9_3.png "demo9_3")
![Screenshot](/screenshots/demo9_4.png "demo9_4")
![Screenshot](/screenshots/demo10_2.png "demo10_2")
![Screenshot](/screenshots/demo11_2.png "demo11_2")
![Screenshot](/screenshots/demo12_4.png "demo12_4")
![Screenshot](/screenshots/demo12_2.png "demo12_2")
![Screenshot](/screenshots/demo12_3.png "demo12_3")
![Screenshot](/screenshots/demo12_5.png "demo12_5")
![Screenshot](/screenshots/demo12_6.png "demo12_6")
![Screenshot](/screenshots/demo12_7.png "demo12_7")
![Screenshot](/screenshots/demo12_12.png "demo12_12")
![Screenshot](/screenshots/demo14_1.png "demo14_1")
![Screenshot](/screenshots/demo14_2.png "demo14_2")
![Screenshot](/screenshots/demo14_4.png "demo14_4")
![Screenshot](/screenshots/demo14_5.png "demo14_5")
![Screenshot](/screenshots/demo14_6.png "demo14_6")
![Screenshot](/screenshots/demo15_1.png "demo15_1")
![Screenshot](/screenshots/demo13_1.png "demo13_1")
