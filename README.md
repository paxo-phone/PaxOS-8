# PaxOS 8

![Logo of the paxo project](logo.png)

**PaxOS 8** is the latest version of **PaxOS**, a lightweight operating system for PaxoPhones. 

## what's new ?

It brings a very reliable event system allowing a very fluid use of the device. Its new graphic module adds new, more aesthetic widgets in a precise theme designed on figma!

The homepage features 8 simplistic apps but none too many:

- The phone app for making calls
- The message app, to send bubble messages to contacts
- The contacts app, which allows you to add and delete contacts that can be used in other applications
- The Calculator
- Tte Clock, which will allows you to program alarms and launch stopwatches and timers
- The Map to calculate its GPS coordinates
- The Snake
- The game 2048

Many improvements are coming soon.

Feel free to suggest contributions.

See more on [paxo.fr](https://www.paxo.fr)

### Windows Build Instructions

Before getting started, you need an IDE, a Visual Studio License (Community Edition is enough) and know how to setup a CMake project with custom toolchain.

1. Install Visual Studio Community or Visual Studio Build Tools with CLang

![CLang selected on the right of Visual Studio Installer](docs/images/clang_visual_studio_install.png)

2. Setup your favorite IDE (eg. CLion) with CLang (very important ! not compatible with MSVC or MinGW)

3. Setup CMake using Ninja (recommended)

4. You shouldn't have to install any library (everything is included)