#include "CSRGraph.h"

bool CSRGraph::has_vertex(int orig_id) const {
    return id_to_idx.find(orig_id) != id_to_idx.end();
}

int CSRGraph::get_index(int orig_id) const {
    auto it = id_to_idx.find(orig_id);
    return (it != id_to_idx.end()) ? it->second : -1;
}