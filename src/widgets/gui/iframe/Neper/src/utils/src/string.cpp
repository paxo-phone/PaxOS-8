#include "../include/string.hpp"

bool iswhitespace(char c)
{
    return 
    (
        c == ' '  || 
        c == '\n' || 
        c == '\r' || 
        c == '\t' || 
        c == '\f' ||
        c == '\v'
    );
}

void ignorewhitespaces(size_t& i, const std::string& str)
{
    while( iswhitespace(str[i]) )
        i++;
}

bool startwith(const std::string& target, 
               const std::string& str, 
               size_t from)
{

    if( str.size() < target.size() )
        return false;

    for(size_t i = 0; i < target.size(); i++)
    {
        if( str[i + from] != target[i] ) return false;
    }

    return true;
}

std::string ltrim(const std::string& str)
{
    size_t start = 0;
    while( iswhitespace(str[start]) )
        start++;

    return str.substr(start);
}

std::string rtrim(const std::string& str)
{
    size_t end = str.size() - 1;
    while( iswhitespace(str[ end ]) )
        end--;

    return str.substr(0, end + 1);
}

std::string trim(const std::string &s) 
{
    return rtrim(ltrim(s));
}

std::vector<std::string> split(const std::string& str, char delim)
{
    std::vector<std::string> o;
    
    std::string temp = "";
    for(size_t i = 0; i < str.size(); i++)
    {
        if( str[i] == delim )
        {
            o.push_back(temp);
            temp.clear();
        }
        else
        {
            temp += str[i];
        }
    }

    o.push_back(temp);

    return o;
}

std::string tolower(const std::string& str)
{
    std::string o = "";

    for(size_t i = 0; i < str.size(); i++)
        o += tolower( str[i] );

    return o;
}

std::string toupper(const std::string& str)
{
    std::string o;

    for(size_t i = 0; i < str.size(); i++)
        o += toupper( str[i] );

    return o;
}