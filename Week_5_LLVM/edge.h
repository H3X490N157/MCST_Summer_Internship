#include "node.h"
#pragma once

struct Edge {
    Node* from; 
    Node* to; 
    int weight; 
    
    Edge(Node* f, Node* t): from(f), to(t), weight(0) {}
};
