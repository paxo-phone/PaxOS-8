#ifndef __UTILS_STRING__
#define __UTILS_STRING__

#include <string>
#include <cctype>
#include <vector>

/*
    return true if c is a whitespace
*/

bool iswhitespace(char c);

/*
    increment i while the corresponding char of str is a whitespace
*/

void ignorewhitespaces(size_t& i, const std::string& str);

/*
    return true if str starts with target (from 0 by default)
*/

bool startwith(const std::string& target, 
               const std::string& str, 
               size_t from = 0);

// trim

/*
    removes all whitespaces at the start of the string
*/
std::string ltrim(const std::string& str);

/*
    removes all whitespaces at the end of the string
*/
std::string rtrim(const std::string& str);

/*
    remove all whitespaces arround the string
*/
std::string trim(const std::string &s);

// split

/*
    cuts the string into tokens separated by the delim character (' ' by default)
*/

std::vector<std::string> split(const std::string& str, char delim = ' ');

// uppercase and lowercase

/*
    Set all the letters of the string in lower case
*/
std::string tolower(const std::string& str);

/*
    Set all the letters of the string in upper case
*/
std::string toupper(const std::string& str);

#endif /* __UTILS_STRING__ */