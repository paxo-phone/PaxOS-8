#!/usr/bin/python3
from argparse import ArgumentParser
import platform
from shutil import which
import subprocess
import os.path
import asyncio
import re

PKG_MANAGER_COMMAND = {
    "apt": ["install", "-y"],
    "dnf": ["install"],
    "yum": ["install"],
    "pacman": ["-S"],
    "zypper": ["in"]
}

LIBRARIES_LINUX = [
    {
        "name": "SDL2",
        "lib": "libSDL2.so",
        "apt": "libsdl2-dev",
        "yum": "SDL2-devel",
        "dnf": "SDL2-devel",
        "zypper": "SDL2-devel"
    },
    {
        "name": "curl",
        "lib": "libcurl.so",
        "apt": "libcurl-dev",
        "yum": "curl-devel",
        "dnf": "libcurl-devel",
        "zypper": "libcurl-devel",
        "pacman": "libcurl-devel"
    }
]

LIBRARIES_WINDOWS = [
    {
        "name": "SDL2",
        "lib": "SDL2.dll",
    }
]

def input_choices(prompt, choices):
    uinput = "~~~" # Placeholder
    while uinput not in choices:
        uinput = input(f"{prompt} [{'/'.join(choices)}] ").strip().upper()

    return uinput

def sub_download(system, pkg_manager):
    print("Checking libraries...")
    match system:
        case "Linux":
            missing = [lib for lib in LIBRARIES_LINUX 
                       if subprocess.check_output(["whereis", lib["lib"]]).find(b"/") == -1]
            if not missing: return False
        case "Windows":
            missing = [lib for lib in LIBRARIES_WINDOWS 
                       if not os.path.exists(".\\libs\\" + lib["lib"])]
            if not missing: return False

    print(f"Missing libraries: {', '.join([lib['name'] for lib in missing])}")
    if pkg_manager:
        res = input_choices(f"Do you want to install them with {pkg_manager}?", ['Y', 'N'])
        if res == 'N':
            exit()
        subprocess.run([pkg_manager] + PKG_MANAGER_COMMAND[pkg_manager] + [lib[pkg_manager] for lib in missing])
    else:
        exit(1)

    return True

async def sub_build():
    print("Generating build files...")
    proc = subprocess.run(['cmake', '.'], stdout=subprocess.PIPE)
    if proc.returncode != 0:
        print(proc.stdout.decode())
        print(proc.stderr.decode())
        exit(proc.returncode)
    
    print("Building program...")
    proc = await asyncio.create_subprocess_exec('make', stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    lastline=0
    while not proc.stdout.at_eof():
        line = (await proc.stdout.readline()).decode().strip()
        if not line:
            continue 
        
        print(end='\r')
        if lastline > len(line):
            print(line + (" " * (lastline-len(line))))
        else:
            print(line, end='\r')
        lastline=len(line)
    print()

    if proc.returncode != 0:
        exit(proc.returncode)
        
def sub_flash():
    print("Flash tool: Not implemented")
    exit(127)
    
async def main():
    parser = ArgumentParser(add_help=True, description='A tool for building and flashing PaxOS')
    parser.add_argument('-d', '--download-only',
                        dest='download_only',
                        action='store_true',
                        help='only download required libraries, and exit')
    parser.add_argument('--target-board',
                        dest='target_board',
                        action='store_true',
                        help='build for the PaxoPhone instead of an emulator (not implemented yet)')
    parser.add_argument('-f', '--flash',
                        dest='flash',
                        action='store_true',
                        help='flash the compiled program on the PaxoPhone (not implemented yet)')
    args = parser.parse_args()

    # Fetch environment
    system = platform.system()
    if not system in ['Linux', 'Darwin', 'Windows']:
        print(f"Unsupported system: {system}")
        exit(127)
    else:
        print(f"Detected operating system: {system}")

    pkg_manager = None
    if system == "Linux":
        if which("apt"):
            pkg_manager = "apt"
        elif which("dnf"):
            pkg_manager = "dnf"
        elif which("yum"):
            pkg_manager = "yum"
        elif which("pacman"):
            pkg_manager = "pacman"
        elif which("zypper"):
            pkg_manager = "zypper"

    download_done = sub_download(system, pkg_manager)
    if args.download_only:
        if not download_done:
            print("No libraries to install/update.")
    else:
        if args.flash:
            if args.build:
                await sub_build()
            sub_flash()
        else:
            await sub_build()

    print("Done")

if __name__ == "__main__":
    try:
        asyncio.run(main())
    except KeyboardInterrupt:
        print("Interrupted")