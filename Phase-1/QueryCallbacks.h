#ifndef QUERYCALLBACKS_H
#define QUERYCALLBACKS_H

#include <functional>
#include <string>
#include "CSRGraph.h"

using QueryCallback = std::function<std::string(const CSRGraph&, int, int)>;

std::string count_reachable(const CSRGraph& g, int src_orig, int K);
std::string max_reachable(const CSRGraph& g, int src_orig, int K);

#endif