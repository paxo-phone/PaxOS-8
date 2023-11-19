#ifndef __FILE_TREE__
#define __FILE_TREE__

#include "../includes.h"
#include <cstdio>
#include <vector>
#include <string>

using namespace std;

#ifdef BUILD_EMU
    #include <filesystem>
    #include <fstream>
    #include <cstdio>
    #include <iostream>
    #include <vector>
    #include <string>
    #include <sys/stat.h>
    #include <dirent.h>
    #ifdef WIN32
        #include <direct.h>
    #endif
#endif
#ifdef BUILD_PAXO
    #define SD_CS 4
    #include "FS.h"
    #include "SD.h"
    #include "SPI.h"
    #include <vector>
#endif


namespace storage
{
    /**
     * @brief Initialize the storage system (specific to BUILD_PAXO).
     */
    void init();

    #ifdef BUILD_EMU
        #ifdef __APPLE__
        /// Get the absolute path of a relative path that is inside the paxos folder.
        ///
        /// Needed only on macOS because if PaxOS's emulator is compiled and ran in Xcode, the binary that would be running is in a `Debug/` or `Release/` directory instead of directly in the project's directory.
        string getMacOSPath(const string& basePath);
        #endif
    #endif /* BUILD_EMU */


    /**
     * @brief List directories and files in the specified path.
     * @param path The path to the directory.
     * @param onlyDirs Flag to list only directories.
     * @return A vector of strings containing directory and file names.
     */
    vector<string> listdir(string path, bool onlyDirs = false);

    /**
     * @brief Check if a file or directory exists at the provided path.
     * @param path The path to the file or directory.
     * @return True if the file or directory exists, False otherwise.
     */
    bool exists(const string& path);
    
    /**
     * @brief Check if the provided path corresponds to a file.
     * @param filepath The path to the file.
     * @return True if the path is a file, False otherwise.
     */
    bool isfile(const string& filepath);

    /**
     * @brief Check if the provided path corresponds to a directory.
     * @param dirpath The path to the directory.
     * @return True if the path is a directory, False otherwise.
     */
    bool isdir(const string& dirpath);

    // work for files AND directory

    /**
     * @brief Create a new directory at the specified path.
     * @param dirpath The path for the new directory.
     * @return True if the directory creation was successful, False otherwise.
     */
    bool newdir(const string& dirpath);

    /**
     * @brief Create a new file at the specified path.
     * @param filepath The path for the new file.
     * @return True if the file creation was successful, False otherwise.
     */
    bool newfile(const string& filepath);
    
    /**
     * @brief Remove the file or directory at the specified path.
     * @param path The path to the file or directory for removal.
     * @return True if the removal was successful, False otherwise.
     */
    bool remove(const string& path);
    
    /**
     * @brief Rename a file or directory from 'from' path to 'to' path.
     * @param from The original path of the file or directory.
     * @param to The new path for the file or directory.
     * @return True if the renaming was successful, False otherwise.
     */
    bool rename(const string& from, const string& to);
}

#endif /* __FILE_TREE__ */
