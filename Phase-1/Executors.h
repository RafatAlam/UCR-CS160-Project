#ifndef EXECUTORS_H
#define EXECUTORS_H

#include <vector>
#include "CSRGraph.h"
#include "QueryTask.h"

void run_tasks_sequential(const CSRGraph& g, std::vector<QueryTask>& tasks);
void run_tasks_parallel(const CSRGraph& g, std::vector<QueryTask>& tasks, int num_threads);

#endif