![Logo of the paxo project](logo.jpeg)

**PaxOS 8** is the latest version of **PaxOS**, a lightweight operating system for PaxoPhones. 

- [Build Instructions](#build-instructions)
  - [Linux](#linux)
  - [Windows](#windows)
  - [MacOS](#macos)
- [License](#license)
- [Contact](#contact)
- [See more](#see-more)
- [Contributors](#contributors)


# Build Instructions

The paxos offers a multi-platform emulator so that you can run it directly on your computer. In this section you will find instructions on how to build it for your operating system.

## Linux

Before you start, make sure that `git`, `cmake` and `make` are correctly installed on your computer. If not, install them using the command `sudo apt-get install build-essentials` (if you use `apt` as your package manager).

You can then start the build instructions : 
1. Clone the directory using `git clone https://github.com/paxo-phone/paxos_8.git`
2. Move to the newly created folder using `cd paxos_8/`
3. Load the sub-modules using `git submodule init && git submodule update`
4. Compile the project using `cmake . && make`
5. Run the executable using `./PaxOS`

The emulator should then open in a window.

## Windows

Before getting started, you need an IDE and know how to setup a CMake project with custom toolchain.

1. Install MinGW or CLang
2. Setup your favorite IDE (eg. CLion)
3. Setup CMake using Ninja (recommended)
4. You shouldn't have to install any library (everything is included)

## MacOS

The instructions for building under macos are fairly similar to those under Linux.

Before you start, make sure that `git`, `cmake` and `make` are correctly installed on your computer. If not, you can use the `brew` package manager to install them using the following command `brew install cmake make git`.

You can then start the build instructions : 
1. Clone the directory using `git clone https://github.com/paxo-phone/paxos_8.git`
2. Move to the newly created folder using `cd paxos_8/`
3. Load the sub-modules using `git submodule init && git submodule update`
4. Compile the project using `cmake . && make`
5. Run the executable using `./PaxOS`

The emulator should then open in a window.
Press the 'Escape' key to emulate the 'Home' button.

Troubleshooting :
- If you get the message `ld: warning: ignoring file /opt/homebrew/Cellar/sdl2/2.28.2/lib/libSDL2-2.0.0.dylib': found architecture 'arm64', required architecture 'x86_64'` followed by an error you can do:
  1. `cp src/lib/SDL2-2.28.2/libSDL2-2.0.0-2.dylib /opt/homebrew/Cellar/sdl2/2.28.2/lib/libSDL2-2.0.0.dylib`
  2. Disable the quarantine for the file: `xattr -dr com.apple.quarantine /opt/homebrew/Cellar/sdl2/2.28.2/lib/libSDL2-2.0.0.dylib`
- If you get the message ` Could not find a package configuration file provided by "SDL2" with any of the following names:`:
  1. Add `-DCMAKE_PREFIX_PATH=src/lib/SDL2-2.28.2/` to your cmake command.

# License
This project is distributed under the CC0 1.0 Universal License. See [LICENSE](/LICENSE) for more information.

# Contact

You can contact us via our [Website](https://www.paxo.fr) or our [Discord](https://discord.com/invite/MpqbWr3pUG) server

# See more

See more on [paxo.fr](https://www.paxo.fr)

# Contributors 

<a href="https://github.com/paxo-phone/PaxOS-8/graphs/contributors">
  <img src="https://contrib.rocks/image?repo=paxo-phone/PaxOS-8" />
</a>
