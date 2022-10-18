/*************************************************************************
	> File Name: adj.h
	> Author: 
	> Mail: 
	> Created Time: Tue 07 Jun 2022 04:48:51 PM CST
 ************************************************************************/

#ifndef _ADJ_H
#define _ADJ_H

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <vector>
#include <tuple>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <immintrin.h>
#include <atomic>


#include <stdint.h>
#include <queue>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "parallel.h"
#include "util.h"

using namespace std;

typedef uint32_t uint_t;
typedef int32_t int_t;

struct EdgeList {
    struct Dest {
        uint_t dest;
        uint_t time;

        Dest() {
            dest = 0;
            time = 0;
        }

        Dest(uint_t _dest, uint_t _time) {
            dest = _dest;
            time = _time;
        }

        bool operator<(const Dest & o) {
            return dest < o.dest;
        }
        bool operator==(const Dest & o) {
            return dest == o.dest && time == o.time;
        }
    };

public:
    uint_t src;
    vector<Dest> dests;

    void insert(uint_t dest, uint_t time);
    void del(uint_t dest);
    bool find2(uint_t dest) const;
    void print();
};

struct AdjGraph {
    uint_t max_vertex;
    uint_t max_edge;
    vector<EdgeList> graph;

    void init(trip_uint* edges, uint_t vertices_num, uint64_t edges_num);
    void print();
};

#endif
