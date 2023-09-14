# Zealot ("ZQ2") - A Quake 2 Fork



## Changes
* Removed `rhapsody`, `irix`, `solaris` and `linux` platforms
* Replaced `win32` platform with `sdl` (WIP)
  * Linux support is brought back by this change :) 
* Organized project structure
  * `ref_gl` is now `ref_gl2`
  * `ref_gl2`, `ref_soft` are now in the `renderers` folder
  * `null`, `sdl` are now in the `platforms` folder
  * `game` / `ctf` are now in the `games` folder
* Implemented `qcore`
  * `qcore` is `qcommon` and `game` unified into one library
  * `qcore` defines all shared headers / impls between the `client` and `game` 
* Rewrote C into C++ (for modernization)
  * `Sys_*` functions are now contained in the `idSystem` class
  * **This is an ongoing effort! More will be added with time!**

## Software Credits
* [SDL](https://github.com/libsdl-org/SDL) - Used in `plat_sdl` and `ref_gl2`
* [GLAD](https://github.com/Dav1dde/glad) - Used in `ref_gl2`

## Thanks to Id Software for open sourcing their old games!
A large majority of the source code is copyrighted by Id Software Inc., 1997-2001

[LICENSE Here](LICENSE)