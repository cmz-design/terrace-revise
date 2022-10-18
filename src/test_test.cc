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
#include "tesseract.h"

#define VERIFY 0

using namespace graphstore;
using namespace std;

/*
 * argv[0]: graph file path
 * argv[1]: batch size
 *
 *
 */

int main(int argc, char * argv[]) {
    uint32_t num_nodes{0};
    uint64_t num_edges{0};
    cout << argv[1] << endl;
    trip_uint *edges = get_wgh_edges_from_file(argv[1], 1, false, &num_edges, &num_nodes);
    
    cout << num_nodes << " " << num_edges << endl;

    // for (int i = 0; i < max(10, num_edges); ++i) {
    //     cout << edges[i].x << " " << edges[i].y << " " << edges[i].z << endl;
    // }

    // select updates
    // uint64_t updates = 2000;
    uint64_t updates = atoi(argv[2]);
    uint64_t num_inits = max(num_edges - updates, (uint64_t)0);

    // init graph
    AdjGraph adj_graph;
    // adj_graph.init(edges, num_nodes, num_inits);
    adj_graph.init(edges, num_nodes, num_edges);
    // adj_graph.print();


    // shuffle the updates
    std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(edges+num_inits, edges+num_edges, g);

    // choose the alg
    // auto alg = new Clique();
    Alg alg;
    
    // find subgraph
	struct timeval start, end;
	struct timezone tzp;

	gettimeofday(&start, &tzp);
    
    parallel_for (int i = num_inits; i < num_edges; ++i) {
        // cout << "processing edge: " << edges[i].x << " " << edges[i].y << endl;
        vector<uint_t> v {edges[i].x, edges[i].y};
        vector<uint_t> ignore {edges[i].y};
        mining_explore(adj_graph, v , alg, ignore);
    }

	gettimeofday(&end, &tzp);

	print_time_elapsed("mining time: ", &start, &end);
    std::cout << alg.found2 << std::endl;

    return EXIT_SUCCESS;
}
