#ifndef __STORAGE_HPP__
#define __STORAGE_HPP__

#ifdef BUILD_EMU
    #include <fstream>
    #include <cstdio>
    #include <iostream>
    #include <vector>
    #include <string>
    #include <sys/stat.h>
    #include <dirent.h>
    using namespace std;
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

                    if(mode == READ)
                        stream = new fstream(("storage/"+path), ios::in);

                    if(mode == WRITE && erase == true)
                        stream = new fstream(("storage/"+path), ios::out | ios::trunc);

                    if(mode == WRITE && erase == false)
                        stream = new fstream(("storage/"+path), ios::out);

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

                    if(mode == READ)
                        stream = new fstream(("storage/"+path), ios::in);

                    if(mode == WRITE && erase == true)
                        stream = new fstream(("storage/"+path), ios::out | ios::trunc);

                    if(mode == WRITE && erase == false)
                        stream = new fstream(("storage/"+path), ios::out);
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

    vector<string> listdir(const string& path) 
    {
        vector<string> list;

        #ifdef BUILD_EMU
            DIR* dir = opendir(path.c_str());
            if (dir)
            {
                struct dirent* entry;
                while ((entry = readdir(dir)) != nullptr) 
                    list.push_back(entry->d_name);

                closedir(dir);
            }
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
            return stat(path.c_str(), &s) == 0;
        #endif
        #ifdef BUILD_PAXO
            return SD.exists(path.c_str());
        #endif
    }
    
    bool isfile(const string& filepath)
    {
        #ifdef BUILD_EMU
            fstream file(filepath, ios::in);
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
            if (stat(dirpath.c_str(), &s) == 0) 
                return (s.st_mode & S_IFDIR) != 0;
            else 
                return false;
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
            return mkdir(dirpath.c_str(), S_IRWXU | S_IRWXG | S_IRWXO) == 0;
        #endif
        #ifdef BUILD_PAXO
            return SD.mkdir(dirpath.c_str());
        #endif
    }

    bool newfile(const string& filepath)
    {
        #ifdef BUILD_EMU
            std::ofstream file(filepath, std::ios::out);
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
            return ::remove(path.c_str()); // from cstdio
        #endif 
        #ifdef BUILD_PAXO
            return SD.remove("logData.txt");
        #endif
    }
    
    bool rename(const string& from, const string& to)
    {
        #ifdef BUILD_EMU
            return ::rename(from.c_str(), to.c_str()); // from cstdio
        #endif
        #ifdef BUILD_PAXO
            return SD.rename(from.c_str(), to.c_str());
        #endif
    }
}

#endif /* __STORAGE_HPP__*/
