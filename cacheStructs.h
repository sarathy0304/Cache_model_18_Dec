//
// Created by srini on 10/24/2021.
//

#ifndef CACHE_MODEL_CACHESTRUCT_H
#define CACHE_MODEL_CACHESTRUCT_H
#include<stdio.h>
#include<vector>
#include<map>
#include <cmath>
#include<iostream>

class cache
{
    int cacheSize;
    int noOfWays;
    int blockSize;
    int addrSize;

public:
    int block_offset = 0;
    int no_of_sets = 0;
    int tag;
    int index;
    int no_of_blocks;
    int getCacheSize() const {
        return cacheSize;
    }
    int getNoOfWays() const {
        return noOfWays;
    }

    void setNoOfWays(int noOfWays) {
        cache::noOfWays = noOfWays;
    }

    void setCacheSize(int cacheSize) {
        cache::cacheSize = cacheSize;
    }



    int getBlockSize() const {
        return blockSize;
    }

    void setBlockSize(int blockSize) {
        cache::blockSize = blockSize;
    }

    int getAddrSize() const {
        return addrSize;
    }

    void setAddrSize(int addrSize) {
        cache::addrSize = addrSize;
    }



    /*
     * Default 512KB Cache with 2 way set associative with block size = 2 word. 1 word = 64 bits. 32 bit physical address.
     */
    cache(){
        cacheSize = 512*1024*8;
        noOfWays = 2;
        blockSize = 128;
        addrSize = 32;
        calculateTagSize();
    }
    cache( int cacheSize, int noOfWays, int blockSize, int addrSize){
        this->cacheSize = cacheSize;
        this->noOfWays = noOfWays;
        this->blockSize = blockSize;
        this->addrSize = addrSize;
        calculateTagSize();
    }
    int calculateTagSize() {
         block_offset = log2(blockSize);
         std::cout<<"Block Offset is "<< block_offset <<std::endl;
         no_of_blocks = pow(2,(log2(cacheSize) - block_offset));
         std::cout <<"No of Blocks is "<< no_of_blocks << std::endl;
         no_of_sets = no_of_blocks/noOfWays;
         std::cout<<"Since it is "<< noOfWays <<" Way Set associative  No of Blocks is divided by "<<noOfWays <<" no_of_sets "<< no_of_sets<< std::endl;
         index = log2(no_of_sets);
         std::cout<<" Index = "<< index << std::endl;
         tag = addrSize - log2(blockSize) - index;
         std::cout<<" Tag = "<< tag << std::endl;
    }
};
#endif //CACHE_MODEL_CACHESTRUCT_H
