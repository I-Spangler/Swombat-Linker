#ifndef UTILITY_LINK_HPP
#define UTILITY_LINK_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
#include <map>

class linker{
    private:
        int ilc = 0, memCount = 0, ram = 256;
        std::map<std::string, int> linkerTable;
        std::ifstream currFile;
        std::ofstream outputFile;
        std::string Replace(std::string line);
        void UpdateAddress(std::string output);
    public:
        void Begin();
        void End();
        void BuildLinkerTable();
        void ParseFileR(std::string filename);
        void ParseFileW(std::string filename);
        void ParseLines();
};

#endif  //UTILITY_LINK_HPP
