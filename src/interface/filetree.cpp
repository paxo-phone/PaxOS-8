#include "filetree.hpp"

void storage::init()
{
    #ifdef ESP32
        pinMode(15, OUTPUT);
        digitalWrite(15, 1);
        SD.begin(SD_CS);
    #endif
}

#if defined(__linux__) || defined(_WIN32) || defined(_WIN64) || defined(__APPLE__)
    #ifdef __APPLE__
    /// Get the absolute path of a relative path that is inside the paxos folder.
    ///
    /// Needed only on macOS because if PaxOS's emulator is compiled and ran in Xcode, the binary that would be running is in a `Debug/` or `Release/` directory instead of directly in the project's directory.
    string storage::getMacOSPath(const string& basePath)
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
#endif /* #if defined(__linux__) || defined(_WIN32) || defined(_WIN64) || defined(__APPLE__) */

vector<string> storage::listdir(string path, bool onlyDirs)
{
    #if defined(__linux__) || defined(_WIN32) || defined(_WIN64) || defined(__APPLE__)

        path = "storage/" + path;

        #ifdef __APPLE__
            path = getMacOSPath(path);
        #endif

        std::vector<std::string> foundDirs;

        for(const auto& foundDirectory : std::filesystem::directory_iterator(path))
        {
            if (onlyDirs && foundDirectory.is_directory())
                foundDirs.push_back(foundDirectory.path().filename().string());
        }

        return foundDirs;
    #endif

    #ifdef ESP32
        vector<string> list;

        File dir = SD.open(("/storage/"+path).c_str());
        if(dir)
        {
            while (true)
            {
                File entry = dir.openNextFile();
                if (!entry)
                    break;
                
                if (!onlyDirs || entry.isDirectory())
                    list.push_back(entry.name());

                Serial.println((path+"/"+entry.name()).c_str());
                
                entry.close();
            }
        }

        return list;
    #endif
}

bool storage::exists(const string& path)
{
    #if defined(__linux__) || defined(_WIN32) || defined(_WIN64) || defined(__APPLE__)
    struct stat s;
        #ifdef __APPLE__
            return stat(getMacOSPath("storage/"+path).c_str(), &s) == 0;
        #else
            return stat(("storage/"+path).c_str(), &s) == 0;
        #endif
    #endif
    #ifdef ESP32
        return SD.exists(("/storage/"+path).c_str());
    #endif
}

bool storage::isfile(const string& filepath)
{
    #if defined(__linux__) || defined(_WIN32) || defined(_WIN64) || defined(__APPLE__)
        #ifdef __APPLE__
            std::filesystem::path filePath("storage/" + filepath);
            return std::filesystem::is_regular_file(filePath);
        #else
            std::filesystem::path filePath("storage/" + filepath);
            return std::filesystem::is_regular_file(filePath);
        #endif
    #endif
    #ifdef ESP32
        File file = SD.open(("/storage/"+filepath).c_str());
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

bool storage::isdir(const string& dirpath)
{
    #if defined(__linux__) || defined(_WIN32) || defined(_WIN64) || defined(__APPLE__)
        if( isfile(dirpath) )
            return false;

        struct stat s;
        #ifdef __APPLE__
            if (stat(getMacOSPath("storage/"+dirpath).c_str(), &s) == 0)
                return (s.st_mode & S_IFDIR) != 0;
            else
                return false;
        #else
            if (stat(("storage/"+dirpath).c_str(), &s) == 0)
                return (s.st_mode & S_IFDIR) != 0;
            else
                return false;
        #endif
    #endif
    #ifdef ESP32
        File file = SD.open(("/storage/"+dirpath).c_str());
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

bool storage::newdir(const string& dirpath)
{
    #if defined(__linux__) || defined(_WIN32) || defined(_WIN64) || defined(__APPLE__)
    #ifdef WIN32
        return _mkdir(("storage/"+dirpath).c_str()) == 0;
    #else
        #ifdef __APPLE__
            return mkdir(getMacOSPath(("storage/"+dirpath)).c_str(), S_IRWXU | S_IRWXG | S_IRWXO) == 0;
        #else
            return mkdir(("storage/"+dirpath).c_str(), S_IRWXU | S_IRWXG | S_IRWXO) == 0;
        #endif
    #endif
    #endif
    #ifdef ESP32
        return SD.mkdir(("/storage/"+dirpath).c_str());
    #endif
}

bool storage::newfile(const string& filepath)
{
    #if defined(__linux__) || defined(_WIN32) || defined(_WIN64) || defined(__APPLE__)
        #ifdef __APPLE__
            std::ofstream file(getMacOSPath(("storage/"+filepath)), std::ios::out);
        #else
            std::ofstream file(("storage/"+filepath), std::ios::out);
        #endif
        return file.is_open();
    #endif
    #ifdef ESP32
        File file = SD.open(("/storage/"+filepath).c_str(), FILE_WRITE);
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

bool storage::remove(const string& path)
{
    #if defined(__linux__) || defined(_WIN32) || defined(_WIN64) || defined(__APPLE__)
        #ifdef __APPLE__
            return ::remove(getMacOSPath(("storage/"+path)).c_str()); // from cstdio
        #else
            return std::remove(("storage/"+path).c_str()) == 0; // from cstdio
        #endif
    #endif
    #ifdef ESP32
        return SD.remove(("/storage/"+path).c_str());
    #endif
}

bool storage::rename(const string& from, const string& to)
{
    #if defined(__linux__) || defined(_WIN32) || defined(_WIN64) || defined(__APPLE__)
        #ifdef __APPLE__
            return ::rename(getMacOSPath(("storage/"+from)).c_str(), getMacOSPath(("storage/"+to)).c_str()) == 0; // from cstdio
        #else
            return ::rename(("storage/"+from).c_str(), ("storage/"+to).c_str()) == 0; // from cstdio
        #endif
    #endif
    #ifdef ESP32
        return SD.rename(("/storage/"+from).c_str(), ("/storage/"+to).c_str());
    #endif
}
