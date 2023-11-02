# Super Mario 64 Port

- This repo contains a full decompilation of Super Mario 64 (J), (U), (E), and (SH).
- Naming and documentation of the source code and data structures are in progress.
- Beyond Nintendo 64, it can also target Linux and Windows natively.

This repo does not include all assets necessary for compiling the game.
A prior copy of the game is required to extract the assets.

## Quick Start (for Ubuntu)

1. Install prerequisites: `sudo apt install -y build-essential git binutils-mips-linux-gnu python3`
2. Clone the repo from within Linux: `git clone https://github.com/n64decomp/sm64.git`
3. Place a Super Mario 64 ROM called `baserom.<VERSION>.z64` into the project folder for asset extraction, where `VERSION` can be `us`, `jp`, `eu`, or `sh`.
4. Run `make` to build. Qualify the version through `make VERSION=<VERSION>`. Add `-j4` to improve build speed (hardware dependent).

Ensure the repo path length does not exceed 255 characters. Long path names result in build errors.

## Installation

### Windows

Install WSL and a distro of your choice following
[Windows Subsystem for Linux Installation Guide for Windows 10.](https://docs.microsoft.com/en-us/windows/wsl/install-win10)
We recommend either Debian or Ubuntu 18.04 Linux distributions under WSL.
Note: WSL1 does not currently support Ubuntu 20.04.

Next, clone the SM64 repo from within the Linux shell:
`git clone https://github.com/n64decomp/sm64.git`

Then continue following the directions in the [Linux](#linux) installation section below.

### Linux

1. Install prerequisites (Ubuntu): `sudo apt install -y git build-essential pkg-config libusb-1.0-0-dev libsdl2-dev`.
2. Clone the repo: `git clone https://github.com/sm64-port/sm64-port.git`, which will create a directory `sm64-port` and then **enter** it `cd sm64-port`.
3. Place a Super Mario 64 ROM called `baserom.<VERSION>.z64` into the repository's root directory for asset extraction, where `VERSION` can be `us`, `jp`, or `eu`.
4. Run `make` to build. Qualify the version through `make VERSION=<VERSION>`. Add `-j4` to improve build speed (hardware dependent based on the amount of CPU cores available).
5. The executable binary will be located at `build/<VERSION>_pc/sm64.<VERSION>.f3dex2e`.

### Windows

1. Install and update MSYS2, following all the directions listed on https://www.msys2.org/.
2. From the start menu, launch MSYS2 MinGW and install required packages depending on your machine (do **NOT** launch "MSYS2 MSYS"):
  * 64-bit: Launch "MSYS2 MinGW 64-bit" and install: `pacman -S git make python3 mingw-w64-x86_64-gcc`
  * 32-bit (will also work on 64-bit machines): Launch "MSYS2 MinGW 32-bit" and install: `pacman -S git make python3 mingw-w64-i686-gcc`
  * Do **NOT** by mistake install the package called simply `gcc`.
3. The MSYS2 terminal has a _current working directory_ that initially is `C:\msys64\home\<username>` (home directory). At the prompt, you will see the current working directory in yellow. `~` is an alias for the home directory. You can change the current working directory to `My Documents` by entering `cd /c/Users/<username>/Documents`.
4. Clone the repo: `git clone https://github.com/sm64-port/sm64-port.git`, which will create a directory `sm64-port` and then **enter** it `cd sm64-port`.
5. Place a *Super Mario 64* ROM called `baserom.<VERSION>.z64` into the repository's root directory for asset extraction, where `VERSION` can be `us`, `jp`, or `eu`.
6. Run `make` to build. Qualify the version through `make VERSION=<VERSION>`. Add `-j4` to improve build speed (hardware dependent based on the amount of CPU cores available).
7. The executable binary will be located at `build/<VERSION>_pc/sm64.<VERSION>.f3dex2e.exe` inside the repository.

#### Troubleshooting

1. If you get `make: gcc: command not found` or `make: gcc: No such file or directory` although the packages did successfully install, you probably launched the wrong MSYS2. Read the instructions again. The terminal prompt should contain "MINGW32" or "MINGW64" in purple text, and **NOT** "MSYS".
2. If you get `Failed to open baserom.us.z64!` you failed to place the baserom in the repository. You can write `ls` to list the files in the current working directory. If you are in the `sm64-port` directory, make sure you see it here.
3. If you get `make: *** No targets specified and no makefile found. Stop.`, you are not in the correct directory. Make sure the yellow text in the terminal ends with `sm64-port`. Use `cd <dir>` to enter the correct directory. If you write `ls` you should see all the project files, including `Makefile` if everything is correct.
4. If you get any error, be sure MSYS2 packages are up to date by executing `pacman -Syu` and `pacman -Su`. If the MSYS2 window closes immediately after opening it, restart your computer.
5. When you execute `gcc -v`, be sure you see `Target: i686-w64-mingw32` or `Target: x86_64-w64-mingw32`. If you see `Target: x86_64-pc-msys`, you either opened the wrong MSYS start menu entry or installed the incorrect gcc package.
6. When switching between building for other platforms, run `make -C tools clean` first to allow for the tools to recompile on the new platform. This also helps when switching between shells like WSL and MSYS2.

### Debugging

The code can be debugged using `gdb`. On Linux install the `gdb` package and execute `gdb <executable>`. On MSYS2 install by executing `pacman -S winpty gdb` and execute `winpty gdb <executable>`. The `winpty` program makes sure the keyboard works correctly in the terminal. Also consider changing the `-mwindows` compile flag to `-mconsole` to be able to see stdout/stderr as well as be able to press Ctrl+C to interrupt the program. In the Makefile, make sure you compile the sources using `-g` rather than `-O2` to include debugging symbols. See any online tutorial for how to use gdb.

## ROM building

Most modern Linux distributions should have equivalent packages to the other two listed above.
You may have to use a different version of GNU binutils. Listed below are fully compatible binutils
distributions with support in the makefile, and examples of distros that offer them:

* `mips64-elf-` (Arch AUR)
* `mips-linux-gnu-` (Ubuntu and other Debian-based distros)
* `mips64-linux-gnu-` (RHEL/CentOS/Fedora)

You may also use [Docker](#docker-installation) to handle installing an image with minimal dependencies.

#### Step 2: Copy baserom(s) for asset extraction

For each version (jp/us/eu/sh) for which you want to build a ROM, put an existing ROM at
`./baserom.<VERSION>.z64` for asset extraction.

##### Step 3: Build the ROM

Run `make` to build the ROM (defaults to `VERSION=us`).
Other examples:
```
make VERSION=jp -j4       # build (J) version instead with 4 jobs
make VERSION=eu COMPARE=0 # build (EU) version but do not compare ROM hashes
```

Resulting artifacts can be found in the `build` directory.

The full list of configurable variables are listed below, with the default being the first listed:

* ``VERSION``: ``us``, ``jp``, ``eu``, ``sh``
* ``GRUCODE``: ``f3d_old``, ``f3d_new``, ``f3dex``, ``f3dex2``, ``f3dzex``
* ``COMPARE``: ``1`` (compare ROM hash), ``0`` (do not compare ROM hash)
* ``NON_MATCHING``: Use functionally equivalent C implementations for non-matchings (Currently there aren't any non-matchings, but this will apply to iQue). Also will avoid instances of undefined behavior.
* ``CROSS``: Cross-compiler tool prefix (Example: ``mips64-elf-``).

### macOS

With macOS, you may either use Homebrew or [Docker](#docker-installation).

#### Homebrew

#### Step 1: Install dependencies
Install [Homebrew](https://brew.sh) and the following dependencies:
```
brew update
brew install capstone coreutils make pkg-config tehzz/n64-dev/mips64-elf-binutils
```

#### Step 2: Copy baserom(s) for asset extraction

For each version (jp/us/eu/sh) for which you want to build a ROM, put an existing ROM at
`./baserom.<VERSION>.z64` for asset extraction.

##### Step 3: Build the ROM

Use Homebrew's GNU make because the version included with macOS is too old.

```
gmake VERSION=jp -j4       # build (J) version instead with 4 jobs
```

### Docker Installation

#### Create Docker image

After installing and starting Docker, create the docker image. This only needs to be done once.
```
docker build -t sm64 .
```

#### Build

To build, mount the local filesystem into the Docker container and build the ROM with `docker run sm64 make`.

##### macOS example for (U):
```
docker run --rm --mount type=bind,source="$(pwd)",destination=/sm64 sm64 make VERSION=us -j4
```

##### Linux example for (U):
For a Linux host, Docker needs to be instructed which user should own the output files:
```
docker run --rm --mount type=bind,source="$(pwd)",destination=/sm64 --user $UID:$GID sm64 make VERSION=us -j4
```

Resulting artifacts can be found in the `build` directory.

## Project Structure
	
	sm64
	├── actors: object behaviors, geo layout, and display lists
	├── asm: handwritten assembly code, rom header
	│   └── non_matchings: asm for non-matching sections
	├── assets: animation and demo data
	│   ├── anims: animation data
	│   └── demos: demo data
	├── bin: C files for ordering display lists and textures
	├── build: output directory
	├── data: behavior scripts, misc. data
	├── doxygen: documentation infrastructure
	├── enhancements: example source modifications
	├── include: header files
	├── levels: level scripts, geo layout, and display lists
	├── lib: SDK library code
	├── rsp: audio and Fast3D RSP assembly code
	├── sound: sequences, sound samples, and sound banks
	├── src: C source code for game
	│   ├── audio: audio code
	│   ├── buffers: stacks, heaps, and task buffers
	│   ├── engine: script processing engines and utils
	│   ├── game: behaviors and rest of game source
	│   ├── goddard: Mario intro screen
	│   ├── menu: title screen and file, act, and debug level selection menus
	│   └── pc: port code, audio and video renderer
	├── text: dialog, level names, act names
	├── textures: skybox and generic texture data
	└── tools: build tools

## Contributing

Pull requests are welcome. For major changes, please open an issue first to
discuss what you would like to change.

Run `clang-format` on your code to ensure it meets the project's coding standards.

Official Discord: https://discord.gg/7bcNTPK
