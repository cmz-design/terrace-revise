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
#include <map>
#include <random>
#include <algorithm>
#include <queue>
#include "sys/time.h"
#include <openssl/rand.h>

#include "util.h"


#include "util.h"
#include "io_util.h"
#include "adj.h"

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
    trip_uint *edges = get_wgh_edges_from_file(argv[1], 1, false, &num_edges, &num_nodes);
    
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
    
    AdjGraph updates_graph;
    updates_graph.init(edges+num_inits, num_nodes, updates);

    // connected
    unordered_map<int, bool> visited_vertices;
    unordered_map<int, unordered_set<int> > graph_find;
    unordered_map<int, multiset<int> > graph_find_multi;

    for (int i = 0; i < updates; i++) {
        auto x = edges[i+num_inits].x;
        auto y = edges[i+num_inits].y;
        visited_vertices[x] = false;
        visited_vertices[y] = false;
        graph_find[x].insert(y);
        graph_find[y].insert(x);
        graph_find_multi[x].insert(y);
        graph_find_multi[y].insert(x);
    }

    int sum_edges = 0;
    int sum_edges_multi = 0;
    for (auto x : graph_find) {
        sum_edges += x.second.size();
    }
    for (auto x : graph_find_multi) {
        sum_edges_multi += x.second.size();
    }

    cout << "vertices num " << visited_vertices.size() << endl;
    cout << "edge num " << sum_edges << endl;
    cout << "edge num multi " << sum_edges_multi << endl;

    queue<int> q;
    int subgraph_num = 0;
    int max_subgraph = 0;
    int max_subgraph_multi = 0;
    for(auto it = visited_vertices.begin(); it != visited_vertices.end(); it++) {
        if (it->second == false) {
            it->second= true;
            
            q.push(it->first);
            subgraph_num++;
            int count = 1;
            queue<int> temp_q;
            temp_q.push(it->first);
            while (!q.empty()) {
                int cur = q.front();
                q.pop();

                for (auto next : graph_find[cur]) {
                    if (visited_vertices[next] == false) {
                        visited_vertices[next] = true;
                        q.push(next);
                        temp_q.push(next);
                        count++;
                    }
                }
            }
            // if (count == 1) continue;
            // cout << "cur " << it->first << " " << count << endl;
            // for (auto n : graph_find[it->first]) {
            //     cout << n << " ";
            // }
            // cout << endl;
            int subgraph_edge = 0;
            int subgraph_edge_multi = 0;
            // those vertices consist which edge?
            while (!temp_q.empty()) {
                int cur = temp_q.front(); temp_q.pop();
                subgraph_edge += graph_find[cur].size();
                subgraph_edge_multi += graph_find_multi[cur].size();
            }

            // subgraph_edge = (subgraph_edge + 1) /  2;


            max_subgraph = max(max_subgraph, subgraph_edge);
            max_subgraph_multi = max(max_subgraph_multi, subgraph_edge_multi);
        }
    }
    cout << subgraph_num << endl;
    cout << "rate : " << 1.0 * max_subgraph / sum_edges << endl;
    cout << "rate : " << 1.0 * max_subgraph_multi / sum_edges_multi << endl;


    /*  power law
    int sum = 0;
    vector<int> degrees;
    for (int i = 0; i < updates_graph.graph.size(); ++i) {
        int num = updates_graph.graph[i].dests.size();
        if (num) {
            degrees.push_back(num);
            sum += num;
        }
    }

    sort(degrees.begin(), degrees.end(), greater<int>());
    cout << "sum " << sum << endl;
    cout << "updates " << updates << endl;

    cout << degrees.size() << endl;
    int i = 0;
    sum = 0;
    for (auto d : degrees) {
        sum += d;
        i++;
        if (i > 4000) break;
        cout << i << " : " << d << " : " << 1.0 * sum / updates / 2 << endl;
    }
    */
    
    /*
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
    */
	return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */
