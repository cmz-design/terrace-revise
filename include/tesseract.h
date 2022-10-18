/*************************************************************************
	> File Name: tesseract.h
	> Author: theqihao 
	> Mail: 
	> Created Time: Wed 29 Jun 2022 11:44:33 AM CST
 ************************************************************************/

#ifndef _TESSERACT_H
#define _TESSERACT_H

#include <vector>
#include <atomic>
#include "adj.h"
#include "util.h"

// algorithm
struct Alg {
    uint32_t depth;
    uint32_t found;
    std::atomic<uint32_t> found2;
    
    Alg() {
        depth = 3;
        found = 0;
    }

    bool filter(const vector<uint32_t> & vertices, const AdjGraph & G);
    void process(const vector<uint32_t> & vertices, const AdjGraph & G);
};

struct Clique : Alg {
};


// mining
void mining_explore(const AdjGraph & G, vector<uint32_t> & vertices, Alg & alg, vector<uint32_t> & ignore);


#endif
