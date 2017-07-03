#ifndef UTILITY_LINK_CPP
#define UTILITY_LINK_CPP

#include "utility_link.hpp"

void linker::ParseFileR(std::string filename){
    try{
        currFile.open(filename, std::ios::in);
    } catch(...){
        std::cout << "Could not open file " << filename << std::endl;
    }
} /*Parse File to Read*/
void linker::ParseFileW(std::string filename){
    try {
        outputFile.open(filename + ".mif", std::ios::out);
    } catch(...){
        std::cout << "Could not open file" << filename << std::endl;
    }
}
//linker Table: guarda todas as instruções e seus endereços relativos do .o (??)
void linker::BuildLinkerTable() {
    int mem;
    std::string output;
    std::regex externFormat("!(_[^ ]+) ([0-9]+)");
    std::smatch external;
    std::regex internalMemFormat("!([0-9]+)");
    std::smatch internalMem;

    currFile.clear();
    currFile.seekg(0, std::ios::beg);

    while(getline(currFile, output)) {
        //acha o tamanho do arquivo
        if(std::regex_search(output, internalMem, internalMemFormat)){
            mem = stoi(internalMem[1]);
        }
        //acha as funções, guarda o endereço com o ilc atual como base
        if(std::regex_search(output, external, externFormat)){
            linkerTable[external[1]] = ilc + std::stoi(external[2]);
        }
    }
    //termina de percorrer o arquivo, atualiza o ilc
    ilc += mem;
    currFile.close();
} /*Build linker Table*/

void linker::ParseLines(){
    std::string output;
    currFile.clear();
    currFile.seekg(0, std::ios::beg);
    while(getline(currFile, output)) {
        //se achar uma external call, substitui
        if(output[0] == '_'){
            output = Replace(output);
        }
        //printa o endereço e a linha no .mif (se não for dados extras do .o)
        if(output[0] != '!'){
            UpdateAddress(output);
        }
    }
    currFile.close();
}

std::string linker::Replace(std::string line){
    //encontra a label na tabela, retorna o endereço em 8 bits
    std::map<std::string, int>::iterator it;
    if(linkerTable.find(line) == linkerTable.end())
        std::cout << "error: label not found. missing external module." << std::endl;
    else it = linkerTable.find(line);
    int address = it->second;
    return std::bitset<8>(address).to_string();
}

//escreve no .mif o endereço e o binário da instrução
void linker::UpdateAddress(std::string output){
    char prev;
    prev = outputFile.fill('0');
    outputFile.width(2);
    outputFile << std::uppercase << std::hex << memCount << " : ";
    outputFile.fill(prev);
    outputFile << output << ";" << std::endl;
    memCount++;
}

//cabeçalho
void linker::Begin(){
    outputFile << "DEPTH = 256;" << std::endl;
    outputFile << "WIDTH = 8;" << std::endl;
    outputFile << "ADDRESS_RADIX = HEX;" << std::endl;
    outputFile << "DATA_RADIX = BIN;" << std::endl;
    outputFile << "CONTENT" << std::endl;
    outputFile << "BEGIN\n" << std::endl;
}

//final
void linker::End(){
    outputFile << "[";
    char prev;
    prev = outputFile.fill('0');
    outputFile.width(2);
    outputFile << std::uppercase << std::hex << memCount << ".." << std::uppercase << std::hex << ram-1 << "]: 00000000;" << std::endl;
    outputFile.fill(prev);
    outputFile << "END;";
}

#endif  //UTILITY_LINK_CPP
