# Rail Climber #

The game at https://play.google.com/store/apps/details?id=net.holak.railclimber

See also https://www.holak.net/how_to_make_an_area-based_2d_smoke_effect_that_doesn_t_suck

**Rewrite in progress, not fully functional at the moment!**

## Bit of a back story

I created the original version of this game in 2012 as an entry into a competition at my university.
 
Since then, I made some occasional improvements and added support for more platforms. I wanted to
release the source code, but it was pretty ugly, written in a hurry, etc.

So I started rewriting certain parts little by little, but just didn't care enough to finish it.

**The rest of the docs is slightly outdated. It will be updated when I have some time.**

## Environment setup ##

As a cross-platform game, the development environment differs between platforms. The currently supported ones are:

- Android, with the Eclipse ADT plugin for now (will move to Android Studio later once the NDK support is finished)
- Tizen, using the official Tizen SDK
- Desktop (Windows, Linux, Mac OS X), using the cmake build tool. You can have cmake generate project files for pretty much any C++ IDE or just use it directly with CLion.
- iOS (WIP)
- emscripten (WIP)
- Raspberry Pi (WIP)

### Desktop ###

The following instructions are for the MinGW toolkit in the msys2 environment. They assume you don't have anything installed yet.
If you already have MinGW installed, skip/adjust the steps that don't apply to you. The required setup for Visual Studio should be fairly similar.

If you use CLion, just open the directory with `CMakeLists.txt` (`build\sdl`), then press `Ctrl+Shift+A` and look for `Change Project Root`.
Then change the project root to the top directory of RailClimber. You might also need to install some of the libraries that RailClimber uses
into your toolchain.

On Linux, read the Linux section below. Most or all of the required packages should available in your distribution's repository,
so the setup ends up being the simplest there.

On Mac OS X, you can install cmake and most of the dependencies from Homebrew and just use the same steps as on Linux.

#### Setting up MinGW ####
- Download and install [msys2](https://msys2.github.io/).
- Open the msys2 shell (should be in your start menu)
- Install mingw with `pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-make`
- Install the following packages
    - `mingw-w64-x86_64-SDL2`
    - `mingw-w64-x86_64-SDL2_image`
    - `mingw-w64-x86_64-SDL2_mixer`
    - `mingw-w64-x86_64-SDL2_ttf`
    - `mingw-w64-x86_64-glew`
    - `mingw-w64-x86_64-glm`
    - `mingw-w64-x86_64-box2d`
    - `mingw-w64-x86_64-expat`

#### Installing cmake ####
Not much to it. Just download and install [cmake](http://www.cmake.org/cmake/resources/software.html).
If you already have cmake installed, be careful about the setup in the following section.

#### Put mingw into your PATH ####
Setting environment variables in Windows kinda sucks unless you have [this handy tool](http://www.rapidee.com/en/about).
We'll be setting the `User Variables` (right half of the application).

For the compilation to work correctly, the mingw part of msys2 needs to be in PATH
Right-click `PATH`, select `Add value` and locate the directory where you installed msys2 followed by `\mingw64\bin`.

Save all the changes. If you had the cmake GUI running, restart it.

#### Using cmake and compiling ####
In addition to the option of calling `cmake` from the command line, it also comes with a convenient GUI tool.

In this tool:

* click `Browse Source...`, locate the `rail-climber/build/sdl` directory
* in the `Where to build the binaries`, put in the same path as in source, followed by `/build` (e.g. `C:\ ... \rail-climber\build\sdl\build`)

Click `Configure`.

The configuration should succeed. This will be indicated by a `Configuring done` line at the bottom of the output (in the bottom part of the window). Press `Generate`. Now you can exit the cmake GUI tool. 

#### Building from the command line ####
You can always build the project by simply running `make` in the `build/sdl/build` directory (after generating the Makefile with cmake). The resulting RailClimber.exe will be put into the `bin` directory.

#### Linux ####
Install packages for all the libraries mentioned above from your distribution's repositories. If you use a distribution that puts the development files into separate packages, you will need the `-dev` package for each library.

For example in Debian, the packages you need to install would be `libsdl2-dev`, `libsdl2-image-dev`, `libsdl2-mixer-dev`, `libsdl2-ttf-dev`, `libglew-dev`, `libbox2d-dev`, `libexpat1-dev`, `libglm-dev`.

Then of course, you need cmake, gcc, and g++.

Once all that's done, just create the `rail-climber/build/sdl/build` directory and enter it. There, run

    cmake ..

Now you can build the binary with `make`.

#### Mac OS X ###
Same as Linux, except the packages are installed form homebrew or whatever other package manager you use.

### Android ###

Coming soon, but basically you just import the project into Eclipse and set the environment variable for where the Android NDK is installed.

Build files for required external libraries `Box2D` and `expat` are included, you just add the source files (actually later those will probably be included with the project as well).

### Tizen ###

Just import the project into Eclipse...

Actually you also need to compile `Box2D` and `expat` for Tizen, but generally you just download them, create a new static library project in Eclipse, import all the source files and press build project.

### iOS ###

TODO, including dependency setup

### emscripten ###

TODO, especially dependency setup

### Raspberry Pi ###

TODO, this will most like use cmake as well
