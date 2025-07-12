#include <cassert>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>

#include "dijkstra.h"
#include "test_rpo_numbering.h"
#include "test_dijkstra.h"


int main(){
    TestRpoNumbering();
    TestDijkstra();
    return 0;
}