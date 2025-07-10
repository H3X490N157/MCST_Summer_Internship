#include <cassert>
#include <iostream>
#include <vector>
#include <string>

#include "fordfulkerson.h"
#include "tarjan.h"
#include "test_pro_numbering.h"
#include "test_dijkstra.h"
#include "test_max_flow.h"
#include "test_tarjan.h"

int main(){
    test_pro_numbering();
    test_dijkstra();
    test_ford_fulkerson();
    test_tarjan();
    return 0;
}