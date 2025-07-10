#include <vector>
#include <string>
#include <iostream>
#pragma once

struct Edge; //костыль: т.к. вершины должны иметь список входящий и исходящих рёбер, а рёбра - список вершин на их конца, то я объявляю Edge, затем определяю Node и уже затем переопределяю Edge

struct Node {
    std::vector<Edge*> out_edges; //список исходящих дуг
    std::vector<Edge*> in_edges; //список входящих дуг
    const std::string id; //идентификатор (сделал константным, потому что бессмысленно менять название вершин в процессе работы)

    Node(const std::string& identifier) : id(identifier) {} //конструктор от индентификатора (нужно для реализации метода NODE)
};
