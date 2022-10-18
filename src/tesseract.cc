#include <set>
#include "tesseract.h"

// algorithm 
bool Alg::filter(const vector<uint32_t> & vertices, const AdjGraph & G) {
    uint32_t vs = vertices.size();
    uint32_t edges = 0;
    
    for (uint32_t i = 0; i < vs; ++i) {
        for (uint32_t j = i+1; j < vs; ++j) {
            if (G.graph[vertices[i]].find2(vertices[j])) {
                edges++;
            }
        }
    }
    return (edges == (vs * (vs - 1) / 2));
}

void Alg::process(const vector<uint32_t> & vertices, const AdjGraph & G) {
    if (vertices.size() >= depth) {
        found2++;
    }
}


// mining
void get_neighbors(const AdjGraph & G, vector<uint32_t> & vertices, set<uint32_t> & ret) {
    for (auto v : vertices) {
        for (auto edge : G.graph[v].dests) {
            bool insert = true;
            for (auto v2 : vertices) {
                if (v2 == edge.dest) {
                    insert = false;
                    break;
                }
            }
            if (insert) {
                ret.insert(edge.dest);
            }   
        }
    }
}
/*
def canonical_r2(e, v, G, ignore=[]):
    found_neighbor = False
    for u in e:
        if not found_neighbor and G.has_edge(u, v):
            found_neighbor = True
        elif found_neighbor and u not in ignore and u > v:
            return False
    return True
*/

bool canonical(const AdjGraph & G, vector<uint32_t> & vertices, uint32_t v, vector<uint32_t> & ignore) {
    bool find_neighbor = false;
    for (auto u : vertices) {
        if (!find_neighbor && (G.graph)[u].find2(v)) {
            find_neighbor = true;
        } else if (find_neighbor && find(ignore.begin(), ignore.end(), u) == ignore.end() && u > v) {
            return false;
        }
    }
    return true;
}

void pv(const vector<uint32_t> & v) {
    for (auto x : v) {
        std::cout << x << " ";
    }
    std::cout << std::endl;
}
void ps(const set<uint32_t> & v) {
    for (auto x : v) {
        std::cout << x << " ";
    }
    std::cout << std::endl;
}

void mining_explore(const AdjGraph & G, vector<uint32_t> & vertices, Alg & alg, vector<uint32_t> & ignore) {
    // pv(vertices);
    uint32_t size = vertices.size();

    if (size == alg.depth) return ;

    set<uint32_t> neighbors;
    get_neighbors(G, vertices, neighbors);
    // ps(neighbors);
    for (auto v : neighbors) {
        if (canonical(G, vertices, v, ignore)) {
            vertices.push_back(v);
            // std::cout << v << " pass canonical" << std::endl; 
            if (alg.filter(vertices, G)) {
                // std::cout << v << " pass filter" << std::endl; 
                alg.process(vertices, G);
                // std::cout << v << " pass process" << std::endl; 
                mining_explore(G, vertices, alg, ignore);
            }

            vertices.pop_back();
        }
    }
}
