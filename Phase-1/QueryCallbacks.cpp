#include "QueryCallbacks.h"
#include <queue>
#include <vector>

std::string count_reachable(const CSRGraph& g, int src_orig, int K) {
    if (!g.has_vertex(src_orig) || K <= 0) return "0";
    int src_idx = g.get_index(src_orig);
    std::vector<bool> visited(g.num_vertices, false);
    std::queue<std::pair<int, int>> q; // (node_index, depth)
    q.emplace(src_idx, 0);
    visited[src_idx] = true;
    int count = 0;

    while (!q.empty()) {
        auto [u, depth] = q.front(); q.pop();
        if (depth >= K) continue;
        for (int e = g.offsets[u]; e < g.offsets[u+1]; ++e) {
            int v_orig = g.edges[e];
            int v_idx = g.get_index(v_orig);
            if (!visited[v_idx]) {
                visited[v_idx] = true;
                ++count;
                q.emplace(v_idx, depth + 1);
            }
        }
    }
    return std::to_string(count);
}

std::string max_reachable(const CSRGraph& g, int src_orig, int K) {
    if (!g.has_vertex(src_orig) || K <= 0) return "-1";
    int src_idx = g.get_index(src_orig);
    std::vector<bool> visited(g.num_vertices, false);
    std::queue<std::pair<int, int>> q;
    q.emplace(src_idx, 0);
    visited[src_idx] = true;
    int max_id = -1;

    while (!q.empty()) {
        auto [u, depth] = q.front(); q.pop();
        if (depth >= K) continue;
        for (int e = g.offsets[u]; e < g.offsets[u+1]; ++e) {
            int v_orig = g.edges[e];
            int v_idx = g.get_index(v_orig);
            if (!visited[v_idx]) {
                visited[v_idx] = true;
                if (v_orig > max_id) max_id = v_orig;
                q.emplace(v_idx, depth + 1);
            }
        }
    }
    return std::to_string(max_id);
}