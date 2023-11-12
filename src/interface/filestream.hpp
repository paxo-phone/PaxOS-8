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
            FileStream();

            /** 
              * Constructor to open a file stream with specified parameters.
              * @param path Path to the file
              * @param mode File open mode (READ/WRITE)
              * @param erase Flag to truncate/erase existing file content
              */
            FileStream(const string& path, OPEN_MODE mode, bool erase = false);

            /** 
              * Destructor to clean up resources.
              */
            ~FileStream();
            
            /** 
             * Opens a file with specified parameters.
             * @param path Path to the file
             * @param mode File open mode (READ/WRITE)
             * @param erase Flag to truncate/erase existing file content
             */
            void open(const string& path, OPEN_MODE mode, bool erase = false);

            /** 
              * Closes the opened file.
              */
            void close(void);

            /** 
             * Reads the entire content of the file and returns as a string.
             * @return Content of the file as a string
             */
            string read(void);

            /** 
             * Reads a line from the file and returns as a string.
             * @return A line from the file as a string
             */
            string readline(void);

            /** 
             * Reads a word from the file and returns as a string.
             * @return A word from the file as a string
             */
            string readword(void);

            
            /** 
             * Reads a single character from the file.
             * @return A character read from the file
             */
            char readchar(void);

            /** 
             * Writes a string to the file.
             * @param str String to be written to the file
             */
            void write(const string& str);


            /** 
             * Writes a character to the file.
             * @param c Character to be written to the file
             */
            void write(const char& c);

            /** 
             * Checks if the file is open.
             * @return True if the file is open, false otherwise
             */
            bool is_open(void);

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

    FileStream& operator<<(FileStream& stream, const string& text);

    FileStream& operator>>(FileStream& stream, string& buff);
}

#endif /* __FILE_STREAM__*/
