#ifndef CSV_HPP
#define CSV_HPP

#define PARAMETER_PATH "settings.set"

struct Parameter
{
    std::string name = "";
    int value = 0;
};

class CSV
{
    public:
    CSV(std::string filename)
    {
        this->filename = filename;
        
        storage::LFile file(filename, storage::OPEN_MODE::READ);
        fileContent = file.read();
        file.close();
    }

    void save()
    {
        storage::LFile file(filename, storage::OPEN_MODE::WRITE, true);
        file.write(fileContent);
        file.close();
    }

    void append(Parameter par)
    {
        fileContent += "\n" + par.name + ":" + to_string(par.value);
    }

    std::string filename = "";
    std::string fileContent = "";
};

#endif