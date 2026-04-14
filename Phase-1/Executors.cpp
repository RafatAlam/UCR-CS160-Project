#include "Executors.h"
#include <thread>

void run_tasks_sequential(const CSRGraph& g, std::vector<QueryTask>& tasks) {
    for (auto& task : tasks) {
        task.result = task.cb(g, task.src, task.K);
    }
}

void run_tasks_parallel(const CSRGraph& g, std::vector<QueryTask>& tasks, int num_threads) {
    int n = tasks.size();
    std::vector<std::thread> threads;
    threads.reserve(num_threads);

    auto worker = [&](int start, int end) {
        for (int i = start; i < end; ++i) {
            tasks[i].result = tasks[i].cb(g, tasks[i].src, tasks[i].K);
        }
    };

    int chunk_size = (n + num_threads - 1) / num_threads;
    for (int t = 0; t < num_threads; ++t) {
        int start = t * chunk_size;
        int end = std::min(start + chunk_size, n);
        if (start < end) {
            threads.emplace_back(worker, start, end);
        }
    }
    for (auto& th : threads) th.join();
}