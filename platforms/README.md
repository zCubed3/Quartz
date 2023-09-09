# ZQ2 Platforms

---

## SDL - WIP

Reimplements most of Quake 2's original `win32` platform but instead by using SDL2

---

## Win32 - Obsolete

The original Win32 implementation of Quake 2, it's buggy and broken

---

## How do I implement my own platform?

### Required file reimplementations
| Purpose | Recommended Name |
|---------|------------------|
| Video   | `vid_[plat]`     |
| Audio   | `snd_[plat]`     |
| Input   | `in_[plat]`      |
| Shared  | `qsh_[plat]`     |
| System  | `sys_[plat]`     |
| Network | `net_[plat]`     |
| OpenGL  | `qgl_[plat]`     |

### TODO