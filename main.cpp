#include <iostream>
#include<fstream>
#include<cstring>
#include "cacheStructs.h"

int main() {
    cache c;
    bool autogen;
    char temp;
    int valid_array[c.tag -1][c.no_of_sets];
    std::vector<unsigned int> address;
    std::ifstream f;
    std::string line;
    std::cout<<"Auto Generate Address? Y/N"<<std::endl;
    std::cin>>temp;
    if(std::tolower(temp) == 'y'){
        autogen = true;
    }
    else {
        autogen = false;
    }
    if(!autogen) {
        f.open("Address.txt");

        if (!f.is_open())
            std::cout << "ERROR File not opened" << std::endl;
        int itr = 0;
        while (!f.eof() && f.is_open()) {
            std::getline(f, line);

            line.erase(4,1);
            char* linecpy = new char[line.length()+1];
            std::cout << line << std::endl;
            std::strcpy(linecpy,line.c_str());
            unsigned int temp_address = std::strtoul(linecpy, nullptr,16);
            itr++;
            address.push_back(temp_address);
            std::cout<<"Unsigned Address is "<< temp_address << std::endl;
        }
        f.close();
    }



}
