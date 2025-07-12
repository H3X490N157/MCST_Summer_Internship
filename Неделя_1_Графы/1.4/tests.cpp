#include <cassert>
#include <iostream>
#include <vector>
#include <string>

#include "fordfulkerson.h"
#include "tarjan.h"
#include "test_rpo_numbering.h"
#include "test_dijkstra.h"
#include "test_max_flow.h"
#include "test_tarjan.h"

int main(){
    TestRpoNumbering();
    TestDijkstra();
    TestFordFulkerson();
    TestTarjan();
    return 0;
}