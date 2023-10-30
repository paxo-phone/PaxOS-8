#ifndef __FILE_STREAM__
#define __FILE_STREAM__

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

#include "filetree.hpp"

namespace storage
{
    /** 
     * Enumeration for different file open modes.
     */
    enum OPEN_MODE
    {
        READ,   /**< Read mode */
        WRITE   /**< Write mode */
    };

    /** 
     * A class to handle file stream operations
     */
    class FileStream
    {
        public:

            /** 
              * Default constructor.
              */
            FileStream()
            {
                #ifdef BUILD_EMU
                    this->stream = new fstream();
                #endif
            }

            /** 
              * Constructor to open a file stream with specified parameters.
              * @param path Path to the file
              * @param mode File open mode (READ/WRITE)
              * @param erase Flag to truncate/erase existing file content
              */
            FileStream(const string& path, OPEN_MODE mode, bool erase = false)
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

            /** 
              * Destructor to clean up resources.
              */
            ~FileStream()
            {
                #ifdef BUILD_EMU
                    delete stream;
                #endif
                #ifdef BUILD_PAXO
                    file.close();
                #endif
            }
            
            /** 
             * Opens a file with specified parameters.
             * @param path Path to the file
             * @param mode File open mode (READ/WRITE)
             * @param erase Flag to truncate/erase existing file content
             */
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
                        file = SD.open(const_cast<char*>(("/storage/"+path).c_str()), FILE_READ);

                    if(mode == WRITE && erase == true)
                        file = SD.open(const_cast<char*>(("/storage/"+path).c_str()), FILE_APPEND);

                    if(mode == WRITE && erase == false)
                        file = SD.open(const_cast<char*>(("/storage/"+path).c_str()), FILE_WRITE);
                #endif
            }

            /** 
              * Closes the opened file.
              */
            void close(void)
            {
                #ifdef BUILD_EMU
                    this->stream->close();
                #endif
                #ifdef BUILD_PAXO
                    file.close();
                #endif
            }

            /** 
             * Reads the entire content of the file and returns as a string.
             * @return Content of the file as a string
             */
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

            /** 
             * Reads a line from the file and returns as a string.
             * @return A line from the file as a string
             */
            string readline(void)
            {
                #ifdef BUILD_EMU
                    string line;
                    getline(*(this->stream), line);
                    return line;
                #endif /* BUILD_EMU */

                #ifdef BUILD_PAXO
                    string line = "";
 
                    char c = file.read();
                    while(c != -1 && c != '\n')
                    {
                        line += c;
                        c = file.read();
                    }

                    return line;

                #endif /* BUILD_PAXO */
            }

            /** 
             * Reads a word from the file and returns as a string.
             * @return A word from the file as a string
             */
            string readword(void)
            {
                #ifdef BUILD_EMU
                    string word;
                    *(this->stream) >> word;
                    return word;
                #endif /* BUILD_EMU */

                #ifdef BUILD_PAXO
                    string line = "";
 
                    char c = file.read();
                    while(c != -1 && c != '\n'
                                  && c != '\t'
                                  && c != ' ')
                    {
                        line += c;
                        c = file.read();
                    }

                    return line;

                #endif /* BUILD_PAXO */
            }

            
            /** 
             * Reads a single character from the file.
             * @return A character read from the file
             */
            char readchar(void)
            {
                #ifdef BUILD_EMU
                    return (this->stream)->get();
                #endif
                #ifdef BUILD_PAXO
                    return file.read();
                #endif
            }

            /** 
             * Writes a string to the file.
             * @param str String to be written to the file
             */
            void write(const string& str)
            {
                #ifdef BUILD_EMU
                    *(this->stream) << str;
                #endif
                #ifdef BUILD_PAXO
                    file.print(const_cast<char*>(str.c_str()));
                #endif
            }


            /** 
             * Writes a character to the file.
             * @param c Character to be written to the file
             */
            void write(const char& c)
            {
                #ifdef BUILD_EMU
                    *(this->stream) << c;
                #endif
                #ifdef BUILD_PAXO
                    file.write(c);
                #endif
            }

            /** 
             * Checks if the file is open.
             * @return True if the file is open, false otherwise
             */
            bool is_open(void)
            {
                #ifdef BUILD_EMU
                    return this->stream->is_open();
                #endif
                #ifdef BUILD_PAXO
                    return file;
                #endif
            }

            friend FileStream& operator<<(FileStream& stream, const std::string& text);
            friend FileStream& operator>>(FileStream& stream, std::string& buff);

        private:

            #ifdef BUILD_EMU
                fstream* stream;
            #endif

            #ifdef BUILD_PAXO
                File file;
            #endif
    };

    FileStream& operator<<(FileStream& stream, const string& text)
    {
        stream.write(text);
        return stream; // fun to allow this kind of syntax file << str1 << str2 << "\n"
    }

    FileStream& operator>>(FileStream& stream, string& buff)
    {
        buff = stream.readword();
        return stream; // fun to allow this kind of syntax file >> word1 >> word2 << "\n"
    }
}

#endif /* __FILE_STREAM__*/
