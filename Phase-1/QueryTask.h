#ifndef QUERYTASK_H
#define QUERYTASK_H

#include <string>
#include "QueryCallbacks.h"

struct QueryTask {
    int src;
    int K;
    QueryCallback cb;
    std::string expected;   // expected result
    std::string result;     // computed result
};

#endif