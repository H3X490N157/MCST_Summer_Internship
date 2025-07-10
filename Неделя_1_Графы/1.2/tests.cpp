#include "graph.h"
#include <cassert>
#include <iostream>
#include <vector>
#include <string>
#include "graph.h"  

void test_pro_numbering() {
    Graph g;
    //дефолтный тест на пронумерование
    g.AddNode("A");
    g.AddNode("B");
    g.AddEdge("A", "B", 4);
    std::vector<std::string> result = g.ProNumbering("A");
    std::vector<std::string> expectation = {"A", "B"};
    assert(result == expectation);
    //тест на корректность работы удаления узла и пронумерования
    g.RemoveNode("A");
    result = g.ProNumbering("B");
    expectation = {"B"};
    assert(result == expectation);
    ///тест на корректность работы пронумеровки с 3 узлами
    g.AddNode("A");
    g.AddNode("C");
    g.AddEdge("A", "B", 12);
    g.AddEdge("B", "C", 7);
    result = g.ProNumbering("A");
    expectation = {"A", "B", "C"};
    assert(result == expectation);
    ///тест на корректность работы с циклом
    g.AddEdge("C", "A", 6);
    result = g.ProNumbering("C");
    expectation = {"C", "A", "B"};
    assert(result == expectation);
    std::cout << "Тесты на базовые операции и PRO_NUMBERING пройдены" << std::endl;
}

int main(){
    test_pro_numbering();
    return 0;
}