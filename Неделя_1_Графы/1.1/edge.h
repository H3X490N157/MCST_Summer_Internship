#include "node.h"
#pragma once

struct Edge {
    Node* from; 
    Node* to; 
    int weight; 
    
    Edge(Node* f, Node* t, int w) : from(f), to(t), weight(w) {}
};
