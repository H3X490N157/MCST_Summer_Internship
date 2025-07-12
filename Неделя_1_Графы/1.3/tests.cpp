#include <cassert>
#include <iostream>
#include <vector>
#include <string>

#include "fordfulkerson.h"  
#include "test_rpo_numbering.h"
#include "test_dijkstra.h"
#include "test_max_flow.h"

int main(){
    TestRpoNumbering();
    TestDijkstra();
    TestFordFulkerson();
    return 0;
}