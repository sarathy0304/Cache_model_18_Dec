#include <iostream>
#include<fstream>
#include<cstring>
#include<sstream>
#include<bitset>
#include "cacheStructs.h"

// Need to inject address repetition and more spatial locality
// Add ways to see compulsory misses
// Add Write/ Read address capability

int main() {
    cache c;
    bool autogen;
    std::bitset<32> bit_addr;
    char temp;
    int tot_capacity_miss = 0;
    int tot_read_miss = 0;
    int noOfAddress = 10;
    int pow2tag = pow(2,c.tag);
    std::cout << "Pow2Tag is " << pow2tag << std::endl;
    int** valid_array = new int*[(c.no_of_sets)];
    int** tag_array = new int*[(c.no_of_sets)];
    for(size_t i = 0 ; i<(c.no_of_sets); i++){
        valid_array[i] = new int[c.getNoOfWays()];
        tag_array[i] = new int[c.getNoOfWays()];
    }
    std::vector<std::string> address;
    std::ifstream f;
    std::string line;
    std::cout<<"Auto Generate Address? Y/N"<<std::endl;
    std::cin>>temp;
    srand(time(NULL));
    if(std::tolower(temp) == 'y'){
        std::cout<< " Please enter number of address to generate"<<std::endl;
        std::cin>>noOfAddress;
        autogen = true;
    }
    else
    {
        autogen = false;
    }
    if(!autogen) {
        f.open("Address_gen.txt");

        if (!f.is_open())
            std::cout << "ERROR File not opened" << std::endl;
        int itr = 0;
        while (!f.eof() && f.is_open()) {
            std::getline(f, line);
            char* linecpy = new char[line.length()+1];
            std::cout << line << std::endl;
            std::strcpy(linecpy,line.c_str());
            itr++;
            address.push_back(linecpy);
            std::cout<<"Unsigned Address is "<< linecpy << std::endl;
        }
        f.close();
    }
    else {
        std::ofstream f;
        f.open("Address_gen.txt");
        rand();
        std::stringstream ss;
        unsigned int temp_address;
        unsigned int prev_addr;
        for(int i = 0; i < noOfAddress ; i++) {
            bool reuse_addr = (float)rand()/RAND_MAX < 0.6 ? 1: 0 ;
            std::cout << " reuse_addr is " << reuse_addr << std::endl;
            if(reuse_addr) {

                temp_address = prev_addr + 64;
                prev_addr = temp_address;
                std::cout << " Address Generated is " << temp_address << std::endl;
            }
            else {
                temp_address = rand() % 4294967295; // Address that are 32 bit addressable
                prev_addr = temp_address;Y
                std::cout << " Address Generated is " << temp_address << std::endl;
            }
            /*
             * Idea 1: Divide the addr space into N sectors. Each addr rand will happen within that addr sector, forcing reuse
             * Idea 2: When an addr is picked, also pick close by addresses and push into queue. have a counter and repeat
             * +/- 10 address for 20 times before moving to a different random address
             * Idea 3: 
             */

            temp_address = temp_address/64; // Word addressable -- Address from Processor;
            temp_address = temp_address/(c.getBlockSize()/64); // Cache addressable
            std::cout << temp_address << std::endl;
            ss << std::hex << temp_address;
            std::string address_str (ss.str());
            //std::cout << " Address in String " << address_str << std::endl;
            if(f.is_open()) {
                f<<address_str<<std::endl;
            }
            address.push_back(address_str);
            ss.str("");

        }
        f.close();
    }
    for(auto itr = address.begin(); itr !=  address.end() ; itr++) {
        bit_addr = std::strtoul(itr->c_str(), nullptr,16);
        //std::cout << "ADDR in bitset is "  << bit_addr << std::endl;
        int block_addr;
        int tag;
        int index;
        bool hit_flag = false;
        bool available_way = false;
        int wayIndex = 0;
        // Calculate block address and remove the block address from Phy addr
        block_addr = std::strtoul(bit_addr.to_string().substr(32-c.block_offset,32).c_str(), nullptr,2);
        index = std::strtoul(bit_addr.to_string().substr(32-c.block_offset-c.index,c.index).c_str(), nullptr,2);
        tag = std::strtoul(bit_addr.to_string().substr(32-c.block_offset-c.index-c.tag,c.tag).c_str(), nullptr,2);
        std::cout << " Block Address is "<< block_addr << std::endl;
        std::cout << "Index is "<< index << std::endl;
        std::cout << "Tag is " << tag << std::endl;
        for(int jj =0 ; jj <c.getNoOfWays(); jj++) {
            if(valid_array[index][jj] == 1) {
                if(tag_array[index][jj] == tag) {
                    hit_flag = true;
                    //std::cout << "READ HIT for Address " << itr->c_str() << std::endl;
                    break;
                }
                //capacity_miss &= capacity_miss;
            }
            else {
                available_way = true;
                wayIndex = jj;
                //std::cout << "There is a available way to place this line way index " << jj << std::endl;

            }
        }

        if(!hit_flag) {
            if(available_way){
                //std::cout << "READ MISS for Address " << itr->c_str() << std::endl;
                tot_read_miss += 1;
                valid_array[index][wayIndex] = 1;
                tag_array[index][wayIndex] = tag;
            }
            else {
                int rand_way_index = rand() % c.getNoOfWays();
                //std::cout << "READ MISS for address " << itr->c_str() << " Randomly choosing wayIndex "<< rand_way_index << std::endl;
                tot_capacity_miss += 1;
                valid_array[index][rand_way_index] = 1;
                tag_array[index][rand_way_index] = tag;
            }


        }
    }
    std::cout << "Total Capacity Misses are " << tot_capacity_miss << std::endl;
    std::cout << "Total Read Misses are " << tot_read_miss << std::endl;

    for (size_t i = c.no_of_sets; i > 0; i--) {
        delete[] valid_array[i];
        delete[] tag_array[i];

    }
    delete[] valid_array;
    delete[] tag_array;
}
