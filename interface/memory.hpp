#ifndef __STORAGE_HPP__
#define __STORAGE_HPP__

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
    #ifdef BUILD_EMU
        #ifdef __APPLE__
        /// Get the absolute path of a relative path that is inside the paxos folder.
        ///
        /// Needed only on macOS because if PaxOS's emulator is compiled and ran in Xcode, the binary that would be running is in a `Debug/` or `Release/` directory instead of directly in the project's directory.
        string getMacOSPath(const string& basePath)
        {
            if (filesystem::path(basePath).is_relative()) // If the path is a relative one then the check should be done
            {
                try
                {
                    string filePath = string(std::filesystem::current_path()); // Get current directory and convert it to a string
                    if (filePath.substr(filePath.find_last_of("\\/"), filePath.size() - 1) == "/Debug" || filePath.substr(filePath.find_last_of("\\/"), filePath.size() - 1) == "/Release") // Check if the binary is executed from Xcode
                    {
                        filePath = filePath.substr(0, filePath.find_last_of("\\/")); // Remove the last component of the path
                    }
                    return filePath+"/"+basePath;
                } catch (std::filesystem::filesystem_error exception)
                {
                    return basePath;
                }
            }
            return basePath;
        }
        #endif
    #endif

    enum OPEN_MODE
    {
        READ,   // 0
        WRITE   // 1
    };

    class LFile
    {
        public:

            LFile()
            {
                #ifdef BUILD_EMU
                    this->stream = new fstream();
                #endif
            }

            LFile(const string& path, OPEN_MODE mode, bool erase = false)
            {
                #ifdef BUILD_EMU

                #ifdef WIN32
                    if(mode == READ)
                        stream = new fstream(("storage/"+path), ios::in | ios::binary);

                    if(mode == WRITE && erase == true)
                        stream = new fstream(("storage/"+path), ios::out | ios::trunc | ios::binary);

                    if(mode == WRITE && erase == false)
                        stream = new fstream(("storage/"+path), ios::out | ios::binary);
                #else
                    #ifdef __APPLE__
                        if(mode == READ)
                            stream = new fstream(getMacOSPath("storage/"+path), ios::in);

                        if(mode == WRITE && erase == true)
                            stream = new fstream(getMacOSPath("storage/"+path), ios::out | ios::trunc);

                        if(mode == WRITE && erase == false)
                            stream = new fstream(getMacOSPath("storage/"+path), ios::out);
                    #else
                        if(mode == READ)
                            stream = new fstream(("storage/"+path), ios::in);

                        if(mode == WRITE && erase == true)
                            stream = new fstream(("storage/"+path), ios::out | ios::trunc);

                        if(mode == WRITE && erase == false)
                            stream = new fstream(("storage/"+path), ios::out);
                    #endif
                #endif

                #endif

                #ifdef BUILD_PAXO
                    if(mode == READ)
                        file = SD.open(const_cast<char*>(("/storage/"+path).c_str()), FILE_READ);

                    if(mode == WRITE && erase == true)
                        file = SD.open(const_cast<char*>(("/storage/"+path).c_str()), FILE_APPEND);

                    if(mode == WRITE && erase == false)
                        file = SD.open(const_cast<char*>(("/storage/"+path).c_str()), FILE_WRITE);
                #endif
            }

            ~LFile()
            {
                #ifdef BUILD_EMU
                    delete stream;
                #endif
                #ifdef BUILD_PAXO
                    file.close();
                #endif
            }
            
            void open(const string& path, OPEN_MODE mode, bool erase = false)
            {
                #ifdef BUILD_EMU

                    #ifdef __APPLE__
                        if(mode == READ)
                            stream = new fstream(getMacOSPath("storage/"+path), ios::in);

                        if(mode == WRITE && erase == true)
                            stream = new fstream(getMacOSPath("storage/"+path), ios::out | ios::trunc);

                        if(mode == WRITE && erase == false)
                            stream = new fstream(getMacOSPath("storage/"+path), ios::out);
                    #else
                        if(mode == READ)
                            stream = new fstream(("storage/"+path), ios::in);

                        if(mode == WRITE && erase == true)
                            stream = new fstream(("storage/"+path), ios::out | ios::trunc);

                        if(mode == WRITE && erase == false)
                            stream = new fstream(("storage/"+path), ios::out);
                    #endif
                #endif

                #ifdef BUILD_PAXO
                    if(mode == READ)
                        file = SD.open(const_cast<char*>(("/"+path).c_str()), FILE_READ);

                    if(mode == WRITE && erase == true)
                        file = SD.open(const_cast<char*>(("/"+path).c_str()), FILE_APPEND);

                    if(mode == WRITE && erase == false)
                        file = SD.open(const_cast<char*>(("/"+path).c_str()), FILE_WRITE);
                #endif
            }

            void close(void)
            {
                #ifdef BUILD_EMU
                    this->stream->close();
                #endif
                #ifdef BUILD_PAXO
                    file.close();
                #endif
            }

            string read(void)
            {
                #ifdef BUILD_EMU
                    string o;
                    
                    string line;
                    while( getline(*(this->stream), line) )
                        o += line + '\n';

                    return o;
                #endif
                #ifdef BUILD_PAXO
                    string o = "";
                    while (file.available())
                        o+=file.read();
                    return o;
                #endif
            }

            char readChr()
            {
                #ifdef BUILD_EMU
                    return (this->stream)->get();
                #endif
                #ifdef BUILD_PAXO
                    return file.read();
                #endif
            }

            void write(const string& str)
            {
                #ifdef BUILD_EMU
                    *(this->stream) << str;
                #endif
                #ifdef BUILD_PAXO
                    file.print(const_cast<char*>(str.c_str()));
                #endif
            }

            void write(const char& c)
            {
                #ifdef BUILD_EMU
                    *(this->stream) << c;
                #endif
                #ifdef BUILD_PAXO
                    file.write(c);
                #endif
            }

            bool is_open(void)
            {
                #ifdef BUILD_EMU
                    return this->stream->is_open();
                #endif
                #ifdef BUILD_PAXO
                    return file;
                #endif
            }

        private:

            #ifdef BUILD_EMU
                fstream* stream;
            #endif

            #ifdef BUILD_PAXO
                File file;
            #endif
    };

    void init()
    {
        #ifdef BUILD_PAXO
            pinMode(15, OUTPUT);
            digitalWrite(15, 1);
            SD.begin(SD_CS);
        #endif
    }

    vector<string> listdir(string path)
    {
        vector<string> list;

        #ifdef BUILD_EMU
            path="storage/"+path;
            #ifdef __APPLE__
                DIR* dir = opendir(getMacOSPath(path).c_str());
                if (dir)
                {
                    struct dirent* entry;
                    while ((entry = readdir(dir)) != nullptr)
                        list.push_back(entry->d_name);

                    closedir(dir);
                }
            #else
                DIR* dir = opendir(path.c_str());
                if (dir)
                {
                    struct dirent* entry;
                    while ((entry = readdir(dir)) != nullptr)
                        list.push_back(entry->d_name);

                    closedir(dir);
                }
            #endif
        #endif

        #ifdef BUILD_PAXO
            File dir = SD.open(path.c_str());
            if(dir)
            {
                while (true)
                {
                    File entry = dir.openNextFile();
                    if (!entry)
                        break;
                    
                    if (entry.isDirectory())
                        list.push_back(entry.name());
                    
                    entry.close();
                }
            }
        #endif

        return list;
    }

    bool exists(const string& path)
    {
        #ifdef BUILD_EMU
        struct stat s;
            #ifdef __APPLE__
                return stat(getMacOSPath(path).c_str(), &s) == 0;
            #else
                return stat(path.c_str(), &s) == 0;
            #endif
        #endif
        #ifdef BUILD_PAXO
            return SD.exists(path.c_str());
        #endif
    }
    
    bool isfile(const string& filepath)
    {
        #ifdef BUILD_EMU
            #ifdef __APPLE__
                fstream file(getMacOSPath(filepath), ios::in);
            #else
                fstream file(filepath, ios::in);
            #endif
            return file.good();
        #endif
        #ifdef BUILD_PAXO
            File file = SD.open(filepath.c_str());
            if(file)
            {
                if(file.isDirectory())
                {
                    file.close();
                    return false;
                }else
                {
                    file.close();
                    return true;
                }
                
            }
        #endif
        
    }

    bool isdir(const string& dirpath)
    {
        #ifdef BUILD_EMU
            if( isfile(dirpath) )
                return false;

            struct stat s;
            #ifdef __APPLE__
                if (stat(getMacOSPath(dirpath).c_str(), &s) == 0)
                    return (s.st_mode & S_IFDIR) != 0;
                else
                    return false;
            #else
                if (stat(dirpath.c_str(), &s) == 0)
                    return (s.st_mode & S_IFDIR) != 0;
                else
                    return false;
            #endif
        #endif
        #ifdef BUILD_PAXO
            File file = SD.open(dirpath.c_str());
            if(file)
            {
                if(file.isDirectory())
                {
                    file.close();
                    return true;
                }else
                {
                    file.close();
                    return false;
                }
                
            }
        #endif
    }

    // work for files AND directory

    bool newdir(const string& dirpath)
    {
        #ifdef BUILD_EMU
        #ifdef WIN32
            return _mkdir(dirpath.c_str()) == 0;
        #else
            #ifdef __APPLE__
                return mkdir(getMacOSPath(dirpath).c_str(), S_IRWXU | S_IRWXG | S_IRWXO) == 0;
            #else
                return mkdir(dirpath.c_str(), S_IRWXU | S_IRWXG | S_IRWXO) == 0;
            #endif
        #endif
        #endif
        #ifdef BUILD_PAXO
            return SD.mkdir(dirpath.c_str());
        #endif
    }

    bool newfile(const string& filepath)
    {
        #ifdef BUILD_EMU
            #ifdef __APPLE__
                std::ofstream file(getMacOSPath(filepath), std::ios::out);
            #else
                std::ofstream file(filepath, std::ios::out);
            #endif
            return file.is_open();
        #endif
        #ifdef BUILD_PAXO
            File file = SD.open(filepath.c_str(), FILE_WRITE);
            if(file)
            {
                file.close();
                return false;
            }else
            {
                return true;
            }
        #endif
    }
    
    bool remove(const string& path)
    {
        #ifdef BUILD_EMU
            #ifdef __APPLE__
                return ::remove(getMacOSPath(path).c_str()); // from cstdio
            #else
                return ::remove(path.c_str()); // from cstdio
            #endif
        #endif
        #ifdef BUILD_PAXO
            return SD.remove(path.c_str());
        #endif
    }
    
    bool rename(const string& from, const string& to)
    {
        #ifdef BUILD_EMU
            #ifdef __APPLE__
                return ::rename(getMacOSPath(from).c_str(), getMacOSPath(to).c_str()); // from cstdio
            #else
                return ::rename(from.c_str(), to.c_str()); // from cstdio
            #endif
        #endif
        #ifdef BUILD_PAXO
            return SD.rename(from.c_str(), to.c_str());
        #endif
    }
}

#endif /* __STORAGE_HPP__*/
