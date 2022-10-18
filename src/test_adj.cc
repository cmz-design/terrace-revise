/*
 * ============================================================================
 *
 *       Filename:  test_pma.cc
 *
 *         Author:  Prashant Pandey (), ppandey2@cs.cmu.edu
 *   Organization:  Carnegie Mellon University
 *
 * ============================================================================
 */

#include <stdlib.h>
#include <assert.h>

#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <array>
#include <set>
#include <random>
#include <algorithm>
#include <queue>
#include "sys/time.h"
#include <openssl/rand.h>

#include "util.h"


#include "util.h"
#include "io_util.h"
#include "adj.h"

#include	<stdlib.h>

#define VERIFY 0

using namespace graphstore;
using namespace std;

bool cmp(const trip_uint &a, const trip_uint &b)            //const必须加，不然会错，目前不懂为啥。当return的是ture时，a先输出，所以示例中是升序
{
    if (a.x == b.x) {
        return a.y < b.y;
    } else {
        return a.x < b.x;
    }
}

/* 
 * ===  FUNCTION  =============================================================
 *         Name:  main
 *  Description:  
 * ============================================================================
 */
	int
main ( int argc, char *argv[] )
{
    // trip_uint data[6] = {{1,2,3}, {1,4,5}, {1,1,7}, {1,9,2}, {1,6,1}, {2,2,1}};
    // AdjGraph graph;
    // graph.init(data, 2, 6);

    uint32_t num_nodes{0};
    uint64_t num_edges{0};
    cout << argv[1] << endl;
    trip_uint *edges = get_wgh_edges_from_file(argv[1], 0, true, &num_edges, &num_nodes);
    
    cout << num_nodes << " " << num_edges << endl;

    for (int i = 0; i < 10; ++i) {
        cout << edges[i].x << " " << edges[i].y << " " << edges[i].z << endl;
    }

    // select updates
    // uint64_t updates = 2000;
    uint64_t updates = atoi(argv[2]);
    uint64_t num_inits = max(num_edges - updates, (uint64_t)0);

    // init graph
    AdjGraph adj_graph;
    // adj_graph.init(edges, num_nodes, num_inits);
    adj_graph.init(edges, num_nodes, num_edges);

    // sort edges, for two condition: src and dest
    sort(edges+num_inits, edges+num_inits+updates, cmp);
    // unordered map store <key, val> = <vertex id, marked>
    unordered_map<int, int> labels;
    unordered_map<int, bool> visited_vertices;
    for (uint64_t i = num_inits; i < num_edges; ++i) {
        labels[edges[i].x] = min(edges[i].x, edges[i].y);
        labels[edges[i].y] = min(edges[i].x, edges[i].y);
        visited_vertices[edges[i].x] = false;
        visited_vertices[edges[i].y] = false;
    }
    // bitvector for visited
    vector<bool> visited_edges(updates+1, false);
    vector<bool> visited_all(num_nodes+1, false);   
    // queue for a cluster to expand
    queue<int> Q;
    // BFS vertex by vertex using the sorted order. if find the update in two distance, mark the update with mini id.
    auto insertQ = [&](int v) {
        if (visited_vertices[v] == false) {
            visited_vertices[v] = true;
            Q.push(v);
        }
    };
    int cluster_num = 0;
    while (true) {
        // find an edge of unvisited
        for (int i = 0; i < updates; ++i) {
            if (visited_edges[i]) continue;
            else {
                visited_edges[i] = true;
                int x = edges[num_inits+i].x;
                int y = edges[num_inits+i].y;
                insertQ(x);
                insertQ(y);
                cout << x << " : " << y << endl;
                break;
            }
        }

        // if not find one edge, travesel all updated edges
        if (Q.empty()) break;
        cluster_num++;

        auto find_add_edges = [&](int vertex, int label) {
            for (int i = 0; i < updates; ++i) {
                if (edges[num_inits+i].x == vertex || edges[num_inits+i].y == vertex) {
                    if (visited_edges[i] == false) {
                        visited_edges[i] = true;
                        int x = edges[num_inits+i].x;
                        int y = edges[num_inits+i].y;
                        insertQ(x);
                        insertQ(y);
                        labels[x] = label;
                        labels[y] = label;
                    }
                }
            }
        };

        // find one edge
        int label = labels[Q.front()];
        while (!Q.empty()) {
            int cur_vertex = Q.front();
            Q.pop();

            for (auto n1 : adj_graph.graph[cur_vertex].dests) {
                find_add_edges(n1.dest, label);
                // for (auto n2 : adj_graph.graph[n1.dest].dests) {
                //     // if (labels.find(n2.dest) != labels.end()) {
                //     //     labels[n2.dest] = label; 
                //     // }
                //     find_add_edges(n2.dest, label);
                // }
            }
        }
    }

    unordered_map<int, int> clusters;

    for (int i = 0; i < updates; ++i) {
        int cid = labels[edges[num_inits+i].x];
        if (clusters.find(cid) == clusters.end()) {
            clusters[cid] = 1;
        } else {
            clusters[cid]++;
        }
    }

    cout << cluster_num << " vs " << clusters.size() << endl;
    for (auto cluster : clusters) {
        cout << cluster.first << " " << cluster.second << endl;
    }

	return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */
