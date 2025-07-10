#include <cassert>
#include <iostream>
#include <vector>
#include <string>

#include "fordfulkerson.h"  
#include "test_pro_numbering.h"
#include "test_dijkstra.h"
#include "test_max_flow.h"

int main(){
    TestProNumbering();
    TestDijkstra();
    TestFordFulkerson();
    return 0;
}