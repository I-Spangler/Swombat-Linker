#ifndef LINK_CPP
#define LINK_CPP
#include "utility_link.hpp"


int main(int argc, char *argv[]) {
	int file = 2;
	std::string filename_input, filename_output = argv[1];
	linker l;

    l.ParseFileW(filename_output);
    l.Begin();
    while(file < argc){
	    filename_input = argv[file];
        l.ParseFileR(filename_input);
        l.BuildLinkerTable();
        file++;
    }
    file = 2;
    while (file < argc){
	    filename_input = argv[file];
        l.ParseFileR(filename_input);
        l.ParseLines();
        file++;
    }
    l.End();
    return 0;
}
#endif  //LINK_CPP
