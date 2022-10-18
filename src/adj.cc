#include "adj.h"

using namespace std;

void EdgeList::insert(uint_t dest, uint_t time) {
    Dest temp = Dest(dest, time);
    // dests.push_back(Dest());
    if (dests.size() == 0) {
        dests.push_back(temp);
        return;
    }
    const auto it = lower_bound(dests.begin(), dests.end(), temp);
    if (dests.size() == 0 || it == dests.end()) {
        dests.push_back(temp);
    //} else if (it->dest == temp.dest) {
    //    it->time = time;
    } else {
        dests.insert(it, temp);
        /*
        dests.push_back(Dest());
        for (auto i = dests.rbegin(), auto j = dests.rbegin()+1; i != it; ++i, ++j) {
            (*i) = (*j); 
        }
        (*it) = temp;
        */
    }
}

bool EdgeList::find2(uint_t dest) const {
    for (auto x : dests) {
        if (dest == x.dest) {
            return true;
        }
    }
    return false;
}

void EdgeList::print() {
    if (dests.size() == 0) return;
    cout << "src: " << src << " : " << dests.size() << endl;
    for (auto it = dests.begin(); it != dests.end(); ++it) {
        cout << it->dest << " " << it->time << endl;
    }
}


void AdjGraph::init(trip_uint* edges, uint_t vertices_num, uint64_t edges_num) {
    max_vertex = vertices_num;
    max_edge = edges_num;

    graph.resize(vertices_num+5);

    // sizeof need align
    // cout << sizeof(edges) << endl;

    for (int i = 0; i < edges_num; ++i) {
        graph[edges[i].x].insert(edges[i].y, edges[i].z);
        // undircted
        graph[edges[i].y].insert(edges[i].x, edges[i].z);
    }

    // for (int i = 0; i < graph.size(); ++i) graph[i].print();
    // for (int i = 0; i < 20; ++i) graph[i].print();
}

void AdjGraph::print() {
    for (int i = 0; i < graph.size() ;++i) {
        graph[i].print();
    }
}
