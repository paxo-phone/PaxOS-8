#include "filestream.hpp"

storage::FileStream::FileStream()
{
    #if defined(__linux__) || defined(_WIN32) || defined(_WIN64) || defined(__APPLE__)
        this->stream = new fstream();
    #endif
}


storage::FileStream::FileStream(const string& path, OPEN_MODE mode, bool erase)
{
    #if defined(__linux__) || defined(_WIN32) || defined(_WIN64) || defined(__APPLE__)

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

    #ifdef ESP32
        if(mode == READ)
            file = SD.open(const_cast<char*>(("/storage/"+path).c_str()), FILE_READ);

        if(mode == WRITE && erase == true)
            file = SD.open(const_cast<char*>(("/storage/"+path).c_str()), FILE_APPEND);

        if(mode == WRITE && erase == false)
            file = SD.open(const_cast<char*>(("/storage/"+path).c_str()), FILE_WRITE);
    #endif
}

storage::FileStream::~FileStream()
{
    #if defined(__linux__) || defined(_WIN32) || defined(_WIN64) || defined(__APPLE__)
        delete stream;
    #endif
    #ifdef ESP32
        file.close();
    #endif
}

void storage::FileStream::open(const string& path, OPEN_MODE mode, bool erase)
{
    #if defined(__linux__) || defined(_WIN32) || defined(_WIN64) || defined(__APPLE__)

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

    #ifdef ESP32
        if(mode == READ)
            file = SD.open(const_cast<char*>(("/storage/"+path).c_str()), FILE_READ);

        if(mode == WRITE && erase == true)
            file = SD.open(const_cast<char*>(("/storage/"+path).c_str()), FILE_APPEND);

        if(mode == WRITE && erase == false)
            file = SD.open(const_cast<char*>(("/storage/"+path).c_str()), FILE_WRITE);
    #endif
}

void storage::FileStream::close(void)
{
    #if defined(__linux__) || defined(_WIN32) || defined(_WIN64) || defined(__APPLE__)
        this->stream->close();
    #endif
    #ifdef ESP32
        file.close();
    #endif
}


string storage::FileStream::read(void)
{
    #if defined(__linux__) || defined(_WIN32) || defined(_WIN64) || defined(__APPLE__)
        string o;
        
        string line;
        while( getline(*(this->stream), line) )
            o += line + '\n';

        return o;
    #endif
    #ifdef ESP32
        string o = "";
        while (file.available())
            o+=file.read();
        return o;
    #endif
}

string storage::FileStream::readline(void)
{
    #if defined(__linux__) || defined(_WIN32) || defined(_WIN64) || defined(__APPLE__)
        string line;
        getline(*(this->stream), line);
        return line;
    #endif /* #if defined(__linux__) || defined(_WIN32) || defined(_WIN64) || defined(__APPLE__) */

    #ifdef ESP32
        string line = "";

        char c = file.read();
        while(c != -1 && c != '\n')
        {
            line += c;
            c = file.read();
        }

        return line;

    #endif /* ESP32 */
}

/**
 * Reads a word from the file and returns as a string.
 * @return A word from the file as a string
 */
string storage::FileStream::readword(void)
{
    #if defined(__linux__) || defined(_WIN32) || defined(_WIN64) || defined(__APPLE__)
        string word;
        *(this->stream) >> word;
        return word;
    #endif /* #if defined(__linux__) || defined(_WIN32) || defined(_WIN64) || defined(__APPLE__) */

    #ifdef ESP32
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

    #endif /* ESP32 */
}


/**
 * Reads a single character from the file.
 * @return A character read from the file
 */
char storage::FileStream::readchar(void)
{
    #if defined(__linux__) || defined(_WIN32) || defined(_WIN64) || defined(__APPLE__)
        return (this->stream)->get();
    #endif
    #ifdef ESP32
        return file.read();
    #endif
}

/**
 * Writes a string to the file.
 * @param str String to be written to the file
 */
void storage::FileStream::write(const string& str)
{
    #if defined(__linux__) || defined(_WIN32) || defined(_WIN64) || defined(__APPLE__)
        *(this->stream) << str;
    #endif
    #ifdef ESP32
        file.print(const_cast<char*>(str.c_str()));
    #endif
}


/**
 * Writes a character to the file.
 * @param c Character to be written to the file
 */
void storage::FileStream::write(const char& c)
{
    #if defined(__linux__) || defined(_WIN32) || defined(_WIN64) || defined(__APPLE__)
        *(this->stream) << c;
    #endif
    #ifdef ESP32
        file.write(c);
    #endif
}

/**
 * Checks if the file is open.
 * @return True if the file is open, false otherwise
 */
bool storage::FileStream::is_open(void)
{
    #if defined(__linux__) || defined(_WIN32) || defined(_WIN64) || defined(__APPLE__)
        return this->stream->is_open();
    #endif
    #ifdef ESP32
        return file;
    #endif
}

storage::FileStream& storage::operator<<(storage::FileStream& stream, const string& text)
{
    stream.write(text);
    return stream; // fun to allow this kind of syntax file << str1 << str2 << "\n"
}

storage::FileStream& storage::operator>>(storage::FileStream& stream, string& buff)
{
    buff = stream.readword();
    return stream; // fun to allow this kind of syntax file >> word1 >> word2 << "\n"
}
