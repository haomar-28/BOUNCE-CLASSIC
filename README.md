# BOUNCE-CLASSIC

A bouncing-ball arcade game written in C with [raylib](https://www.raylib.com/) 5.5.
Everything the game needs (source, images, sounds, and raylib itself) is in this folder. Nothing is downloaded while building.

## Play it (no coding needed)

### Step 1 - Install three free tools (one time only)

| | macOS | Windows | Linux (Ubuntu/Debian) |
|---|---|---|---|
| VS Code | https://code.visualstudio.com | same | same |
| C compiler | Open Terminal, run `xcode-select --install` | Install **Visual Studio Build Tools**, tick **Desktop development with C++** | `sudo apt install build-essential` |
| CMake | `brew install cmake` (or download from https://cmake.org/download) | Download from https://cmake.org/download, tick **Add CMake to PATH** | `sudo apt install cmake` |

Linux also needs: `sudo apt install libasound2-dev libx11-dev libxrandr-dev libxi-dev libgl1-mesa-dev libglu1-mesa-dev libxcursor-dev libxinerama-dev`

### Step 2 - Get the game
On GitHub click **Code -> Download ZIP**, then unzip it. (Or `git clone` it.)

### Step 3 - Open and run
1. In VS Code: **File -> Open Folder...** and choose the unzipped `BOUNCE-CLASSIC` folder.
2. Click **Install** if VS Code offers to install the recommended C/C++ and CMake extensions.
3. Press **Terminal -> Run Task... -> Run Bounce Classic**.
   The first run takes a minute or two because it compiles raylib. After that it is fast.

The game window opens. Close it to quit.

### If something goes wrong
- `cmake: command not found` - CMake is not installed or VS Code needs a restart.
- Windows: no compiler found - install the Build Tools from step 1, then restart VS Code.
- Images/sound missing - run the game through the VS Code task (it starts from the project folder, where `assets/` lives).

## Project layout
```
finalGameShowdown.c   game source
assets/               images and sounds
dependencies/raylib/  raylib 5.5 source + license (zlib)
CMakeLists.txt        build recipe
.vscode/              VS Code build/run/debug tasks
```
Generated files go to `build/` and `bin/` and are ignored by git.

## Credits
Raylib is (c) Ramon Santamaria and contributors, licensed under zlib; see `dependencies/raylib/LICENSE`.
