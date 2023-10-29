#include "storage.hpp"

using namespace Storage;

FileStream::FileStream(std::string filename, Mode mode) : mode(mode)
{
    #ifdef BUILD_PAXO
        this->filename = "/storage/"+filename;
    #else
        this->filename = "storage/"+filename;
    #endif
}

bool FileStream::open()
{
    #ifdef ESP32
        if (mode == R)
        {
            file = SD.open(filename.c_str(), FILE_READ);
        }
        else if (mode == W)
        {
            file = SD.open(filename.c_str(), FILE_WRITE);
        }

        return file;
    #else
        if (mode == R)
        {
            file.open(filename, std::ios::in);
        }
        else if (mode == W)
        {
            file.open(filename, std::ios::out | std::ios::in | std::ios::trunc);
        }

        return file.is_open();
    #endif
}

void FileStream::close()
{
    #ifdef ESP32
        if (file)
        {
            file.close();
        }
    #else
        if (file.is_open())
        {
            file.close();
        }
    #endif
}

std::string FileStream::read(int from, int to)
{
    #ifdef ESP32
        if (!file)
            return "";

        if (from >= 0)
            file.seek(from);

        if (to >= 0)
        {
            char buffer[to - from + 1];
            memset(buffer, 0, sizeof(buffer));
            file.readBytes(buffer, to - from + 1);
            return std::string(buffer);
        }
        else
        {
            String data = file.readString();
            return std::string(data.c_str());
        }
    #else
        if (!file.is_open())
        {
            return ""; // File not open
        }

        if (from >= 0)
        {
            file.seekg(from);
        }

        std::stringstream buffer;
        if (to >= 0)
        {
            char c;
            int count = 0;
            while (file.get(c) && count <= to)
            {
                buffer.put(c);
                count++;
            }
        }
        else
        {
            buffer << file.rdbuf();
        }

        return buffer.str();
    #endif
}

void FileStream::setCursor(int pos)
{
    #ifdef ESP32
        if (file && pos >= 0)
        {
            file.seek(pos);
        }
    #else
        if (file.is_open() && pos >= 0)
        {
            file.seekp(pos);
        }
    #endif
    
}

void FileStream::write(std::string data)
{
    #ifdef ESP32
        if (file)
        {
            file.print(data.c_str());
        }
    #else
        if (file.is_open())
        {
            file << data;
        }
    #endif
}

std::vector<std::string> listFiles(std::string dir)
{
    #ifdef BUILD_PAXO
        dir = "/storage/"+dir;
    #else
        dir = "storage/"+dir;
    #endif

    #ifdef ESP32
        std::vector<std::string> files;

        File root = SD.open(dir.c_str());
        if (!root)
        {
            return files;
        }

        while (File file = root.openNextFile())
        {
            files.push_back(file.name());
            file.close();
        }

        return files;
    #else
        std::vector<std::string> files;
        DIR *dp;
        struct dirent *dirp;

        if ((dp = opendir(dir.c_str())) == NULL)
        {
            std::cerr << "Error opening directory: " << dir << std::endl;
            return files;
        }

        while ((dirp = readdir(dp)) != NULL)
        {
            std::string filename = dirp->d_name;
            if (filename != "." && filename != "..")
            {
                files.push_back(filename);
            }
        }

        closedir(dp);
        return files;
    #endif

}

bool Storage::mkdir(std::string dir)
{
    #ifdef BUILD_PAXO
        dir = "/storage/"+dir;
    #else
        dir = "storage/"+dir;
    #endif

    #ifdef ESP32
        return SD.mkdir(dir.c_str());
    #else
        int status = mkdir(dir.c_str());
        return status == 0;
    #endif
}

bool Storage::isDir(std::string dir)
{
    #ifdef BUILD_PAXO
        dir = "/storage/"+dir;
    #else
        dir = "storage/"+dir;
    #endif

    #ifdef ESP32
        File file = SD.open(dir.c_str());
        if (!file)
        {
            return false;
        }
        bool isDirectory = file.isDirectory();
        file.close();
        return isDirectory;
    #else
        struct stat st;
        if (stat(dir.c_str(), &st) != 0)
        {
            return false; // Path does not exist
        }

        return S_ISDIR(st.st_mode);
    #endif
}

bool Storage::isFile(std::string dir)
{
    #ifdef BUILD_PAXO
        dir = "/storage/"+dir;
    #else
        dir = "storage/"+dir;
    #endif

    #ifdef ESP32
        return SD.exists(dir.c_str());
    #else
        struct stat st;
        if (stat(dir.c_str(), &st) != 0)
        {
            return false; // Path does not exist
        }

        return S_ISREG(st.st_mode);
    #endif
}

bool Storage::mv(std::string from, std::string to)
{
    #ifdef BUILD_PAXO
        from = "/storage/"+from;
        to = "/storage/"+to;
    #else
        from = "storage/"+from;
        to = "storage/"+to;
    #endif

    #ifdef ESP32
        return SD.rename(from.c_str(), to.c_str());
    #else
        int status = rename(from.c_str(), to.c_str());
        return status == 0;
    #endif
}

bool Storage::rm(std::string path)
{
    #ifdef BUILD_PAXO
        path = "/storage/"+path;
    #else
        path = "storage/"+path;
    #endif

    #ifdef ESP32
        return SD.remove(path.c_str());
    #else
        int status = remove(path.c_str());
        return status == 0;
    #endif
}
