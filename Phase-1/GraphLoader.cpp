#include "GraphLoader.h"
#include <fstream>
#include <sstream>
#include <unordered_set>
#include <algorithm>
#include <iostream>

CSRGraph load_graph(const std::string& filename) {
    std::unordered_set<int> vertex_set;
    std::vector<std::pair<int, int>> edge_list;

    std::ifstream infile(filename);
    if (!infile.is_open()) {
        std::cerr << "Error: cannot open " << filename << std::endl;
        exit(1);
    }

    std::string line;
    while (std::getline(infile, line)) {
        if (line.empty() || line[0] == '#') continue;
        std::istringstream iss(line);
        int src, dst;
        if (!(iss >> src >> dst)) continue;
        vertex_set.insert(src);
        vertex_set.insert(dst);
        edge_list.emplace_back(src, dst);
    }
    infile.close();

    // Assign contiguous indices
    std::vector<int> orig_vertices(vertex_set.begin(), vertex_set.end());
    std::sort(orig_vertices.begin(), orig_vertices.end());
    std::unordered_map<int, int> id_to_idx;
    for (size_t i = 0; i < orig_vertices.size(); ++i) {
        id_to_idx[orig_vertices[i]] = i;
    }

    int n = orig_vertices.size();
    std::vector<std::vector<int>> adj(n);
    for (auto& [src, dst] : edge_list) {
        int u = id_to_idx[src];
        int v = id_to_idx[dst];
        adj[u].push_back(v);
    }

    // Build CSR
    std::vector<int> offsets(n + 1, 0);
    std::vector<int> edges;
    for (int i = 0; i < n; ++i) {
        offsets[i] = edges.size();
        std::sort(adj[i].begin(), adj[i].end());
        for (int v_idx : adj[i]) {
            edges.push_back(orig_vertices[v_idx]); // store original ID
        }
    }
    offsets[n] = edges.size();

    return CSRGraph{
        .num_vertices = n,
        .offsets = std::move(offsets),
        .edges = std::move(edges),
        .orig_ids = std::move(orig_vertices),
        .id_to_idx = std::move(id_to_idx)
    };
}