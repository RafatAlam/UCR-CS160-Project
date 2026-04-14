#include <iostream>
#include <chrono>
#include <thread>
#include "GraphLoader.h"
#include "QueryCallbacks.h"
#include "QueryTask.h"
#include "Executors.h"
#include <fstream>

std::vector<QueryTask> load_queries(const std::string& filename) {
    std::vector<QueryTask> tasks;
    std::ifstream infile(filename);
    if (!infile.is_open()) {
        std::cerr << "Error: cannot open " << filename << std::endl;
        exit(1);
    }
    std::string line;
    while (std::getline(infile, line)) {
        if (line.empty() || line[0] == '#') continue;
        std::istringstream iss(line);
        int src, K, qtype;
        std::string expected;
        if (!(iss >> src >> K >> qtype >> expected)) continue;
        QueryCallback cb;
        if (qtype == 1) cb = count_reachable;
        else if (qtype == 2) cb = max_reachable;
        else continue;
        tasks.push_back({src, K, cb, expected, ""});
    }
    return tasks;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <graph_edges.txt> <queries.txt>" << std::endl;
        return 1;
    }

    std::string graph_file = argv[1];
    std::string query_file = argv[2];

    std::cout << "Loading graph from " << graph_file << " ..." << std::endl;
    CSRGraph graph = load_graph(graph_file);
    std::cout << "Graph loaded: " << graph.num_vertices << " vertices, "
              << graph.edges.size() << " edges." << std::endl;

    std::cout << "Loading queries from " << query_file << " ..." << std::endl;
    std::vector<QueryTask> tasks = load_queries(query_file);
    std::cout << "Loaded " << tasks.size() << " queries." << std::endl;

    // Sequential
    std::vector<QueryTask> seq_tasks = tasks;
    auto start_seq = std::chrono::high_resolution_clock::now();
    run_tasks_sequential(graph, seq_tasks);
    auto end_seq = std::chrono::high_resolution_clock::now();
    auto seq_duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_seq - start_seq).count();

    // Parallel
    int num_threads = std::thread::hardware_concurrency();
    if (num_threads == 0) num_threads = 4;
    std::cout << "Using " << num_threads << " threads for parallel execution." << std::endl;

    std::vector<QueryTask> par_tasks = tasks;
    auto start_par = std::chrono::high_resolution_clock::now();
    run_tasks_parallel(graph, par_tasks, num_threads);
    auto end_par = std::chrono::high_resolution_clock::now();
    auto par_duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_par - start_par).count();

    // Verification
    bool all_correct = true;
    for (size_t i = 0; i < tasks.size(); ++i) {
        if (seq_tasks[i].result != tasks[i].expected) {
            std::cout << "Mismatch (sequential) for query " << i
                      << ": got " << seq_tasks[i].result
                      << ", expected " << tasks[i].expected << std::endl;
            all_correct = false;
        }
        if (par_tasks[i].result != tasks[i].expected) {
            std::cout << "Mismatch (parallel) for query " << i
                      << ": got " << par_tasks[i].result
                      << ", expected " << tasks[i].expected << std::endl;
            all_correct = false;
        }
        if (seq_tasks[i].result != par_tasks[i].result) {
            std::cout << "Internal inconsistency for query " << i << std::endl;
            all_correct = false;
        }
    }

    if (all_correct)
        std::cout << "\nAll queries produced the expected results.\n" << std::endl;
    else
        std::cout << "\nSome queries failed verification.\n" << std::endl;

    std::cout << "=== Performance Report ===" << std::endl;
    std::cout << "Sequential time: " << seq_duration << " ms" << std::endl;
    std::cout << "Parallel time  : " << par_duration << " ms" << std::endl;
    if (par_duration > 0) {
        double speedup = static_cast<double>(seq_duration) / par_duration;
        std::cout << "Speedup        : " << speedup << "x" << std::endl;
    }
    std::cout << "==========================" << std::endl;

    return 0;
}