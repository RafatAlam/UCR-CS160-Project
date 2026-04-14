#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <algorithm>

struct CSRGraph {
    int num_vertices;
    std::vector<int> offsets;
    std::vector<int> edges;
};

CSRGraph LoadGraph(const char *filename) {
    std::ifstream fin(filename);
    if (!fin) {
        throw std::runtime_error(std::string("Failed to open file: ") + filename);
    }

    std::string line;
    std::vector<std::pair<int, int>> edge_list;
    int max_vertex = -1;

    // Read edge list
    while (std::getline(fin, line)) {
        if (line.empty() || line[0] == '#') continue;

        std::istringstream iss(line);
        int src, dst;
        if (!(iss >> src >> dst)) continue;

        edge_list.emplace_back(src, dst);
        max_vertex = std::max(max_vertex, std::max(src, dst));
    }

    CSRGraph g;
    g.num_vertices = max_vertex + 1;
    g.offsets.assign(g.num_vertices + 1, 0);

    // Count degrees
    for (const auto &e : edge_list) {
        g.offsets[e.first + 1]++;
    }

    // Prefix sum
    for (int i = 1; i <= g.num_vertices; ++i) {
        g.offsets[i] += g.offsets[i - 1];
    }

    // Fill edges
    g.edges.assign(edge_list.size(), 0);
    std::vector<int> next_pos = g.offsets;

    for (const auto &e : edge_list) {
        g.edges[next_pos[e.first]++] = e.second;
    }

    return g;
}

// Helper: print neighbors
void PrintNeighbors(const CSRGraph &g, int v) {
    if (v < 0 || v >= g.num_vertices) {
        std::cout << "Invalid vertex\n";
        return;
    }

    std::cout << "Neighbors of vertex " << v << ": ";
    for (int i = g.offsets[v]; i < g.offsets[v + 1]; ++i) {
        std::cout << g.edges[i] << " ";
    }
    std::cout << "\nOut-degree: " << (g.offsets[v + 1] - g.offsets[v]) << "\n";
}

int main() {
    const char *filename = "soc-Slashdot0902.txt";

    try {
        CSRGraph g = LoadGraph(filename);

        std::cout << "Loaded graph from: " << filename << "\n";
        std::cout << "Number of vertices: " << g.num_vertices << "\n";
        std::cout << "Number of edges: " << g.edges.size() << "\n\n";

        // Example: print neighbors of vertex 6
        PrintNeighbors(g, 6);

    } catch (const std::exception &e) {
        std::cerr << e.what() << "\n";
        return 1;
    }

    return 0;
}