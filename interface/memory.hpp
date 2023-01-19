#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <stdlib.h>
#include <iostream>

#ifdef BUILD_EMU
#include <fstream>
#endif

using namespace std;

enum MemoryOpenMode
{
    READ,
    WRITE,
    READ_WRITE,
};

class LFile
{
    public:
    LFile(std::string filename);
    bool open(MemoryOpenMode mode, bool erase = false); // ouvrir le fichier avec un mode d'ouverture, et la possibilité d'écraser son contenu

    char read();        // lire un carractère
    std::string readAll();   // lire tout les carractères du fichier

    void write(char chr);       // écrire un carractère
    void print(std::string data);    // écrire plusieurs carractères

    private:
    std::string filename = "";  // adresse du fichier

    #ifdef BUILD_EMU
    ofstream* file = nullptr;
    #endif

    #ifdef BUILD_PAXO
    File* file = nullptr;
    #endif
};

class Memory
{
    public:
    Memory();
};

#endif