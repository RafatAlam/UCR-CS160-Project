#ifndef CSRGRAPH_H
#define CSRGRAPH_H

#include <vector>
#include <unordered_map>

struct CSRGraph {
    int num_vertices;
    std::vector<int> offsets;          // size = num_vertices + 1
    std::vector<int> edges;            // concatenated adjacency lists (original IDs)
    std::vector<int> orig_ids;         // original ID for each mapped index
    std::unordered_map<int, int> id_to_idx; // original ID -> mapped index

    bool has_vertex(int orig_id) const;
    int get_index(int orig_id) const;
};

#endif