import os
import sys
import argparse
import argparse
import requests 
import zipfile

print("""
    [+] -----------------------
    [+] Paxo Builder 0.1
    [+] 2023 (c) EletrixTime
    [+] MIT license
    [+] -----------------------
    """)

argParser = argparse.ArgumentParser()
argParser.add_argument("-n", "--build", help="Build (win/linux)")
args = argParser.parse_args()
if args.build and args.build == "win":
    print("[+] Downloading the source code via github...")
    os.system("git clone https://github.com/paxo-phone/PaxOS-8.git paxos")
    print("[+] Downloading CMake")
    r = requests.get("https://github.com/EletrixtimeYT/cmake-portable/releases/download/0.1/cmakeportable.zip", stream=True) 
    with open("cmake.zip", "wb") as zip: 
        for chunk in r.iter_content(chunk_size=5000): 
            if chunk: 
                zip.write(chunk) 
    print("[+] Extracting Cmake")
    with zipfile.ZipFile('cmake.zip', 'r') as zip_ref:
        zip_ref.extractall("cmake")
    os.remove("cmake.zip")
    os.environ['PATH'] += os.pathsep + os.path.abspath("cmake/bin")
    os.system("cmake.exe paxos")
    print("[+] Done :D")
    
elif args.build and args.build == "linux":
    print("[+] Cooming Soon !")
else:
    print("[+] Unknow arguments for --build (type --build win for building on windows and --build linux for building on linux)")
