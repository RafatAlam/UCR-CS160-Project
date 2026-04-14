#ifndef GRAPHLOADER_H
#define GRAPHLOADER_H

#include <string>
#include "CSRGraph.h"

CSRGraph load_graph(const std::string& filename);

#endif