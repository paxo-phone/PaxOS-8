#include "filetree.hpp"

void storage::init()
{
    #ifdef BUILD_PAXO
        pinMode(15, OUTPUT);
        digitalWrite(15, 1);
        SD.begin(SD_CS);
    #endif
}

#ifdef BUILD_EMU
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
#endif /* BUILD_EMU */

vector<string> storage::listdir(string path, bool onlyDirs)
{
    vector<string> list;

    #ifdef BUILD_EMU
        path="storage/"+path;
        #ifdef __APPLE__
            path = getMacOSPath(path);
        #endif
            std::vector<std::string> r;
            if (onlyDirs)
            {
                for(auto& p : std::filesystem::directory_iterator(path))
                    if (p.is_directory())
                        r.push_back(p.path().filename().string());
            } else {
                for(auto& p : std::filesystem::directory_iterator(path))
                    r.push_back(p.path().filename().string());
            }
            return r;
    #endif

    #ifdef BUILD_PAXO
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
    #endif

    return list;
}

bool storage::exists(const string& path)
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
        return SD.exists(("/storage/"+path).c_str());
    #endif
}

bool storage::isfile(const string& filepath)
{
    #ifdef BUILD_EMU
        #ifdef __APPLE__
    fstream file(storage::getMacOSPath(filepath), ios::in);
        #else
            fstream file(filepath, ios::in);
        #endif
        return file.good();
    #endif
    #ifdef BUILD_PAXO
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
        return SD.mkdir(("/storage/"+dirpath).c_str());
    #endif
}

bool storage::newfile(const string& filepath)
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
    #ifdef BUILD_EMU
        #ifdef __APPLE__
            return ::remove(getMacOSPath(path).c_str()); // from cstdio
        #else
            return ::remove(path.c_str()); // from cstdio
        #endif
    #endif
    #ifdef BUILD_PAXO
        return SD.remove(("/storage/"+path).c_str());
    #endif
}

bool storage::rename(const string& from, const string& to)
{
    #ifdef BUILD_EMU
        #ifdef __APPLE__
            return ::rename(getMacOSPath(from).c_str(), getMacOSPath(to).c_str()); // from cstdio
        #else
            return ::rename(from.c_str(), to.c_str()); // from cstdio
        #endif
    #endif
    #ifdef BUILD_PAXO
        return SD.rename(("/storage/"+from).c_str(), ("/storage/"+to).c_str());
    #endif
}
