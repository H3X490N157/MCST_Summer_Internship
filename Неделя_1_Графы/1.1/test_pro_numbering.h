#include "graph.h"
#pragma once

void TestProNumbering() {//так как здесь особо нечего тестировать, то покрытие тестами будет меньше, чем для дальнейних подзаданий
    Graph g;
    //дефолтный тест на пронумерование
    g.AddNode("A");
    g.AddNode("B");
    g.AddEdge("A", "B");
    std::vector<std::string> result = g.ProNumbering("A");
    std::vector<std::string> expectation = {"A", "B"};
    assert(result == expectation);
    //тест на корректность работы удаления узла и пронумерования
    g.RemoveNode("A");
    result = g.ProNumbering("B");
    expectation = {"B"};
    assert(result == expectation);
    //тест на корректность работы пронумеровки с 3 узлами
    g.AddNode("A");
    g.AddNode("C");
    g.AddEdge("A", "B");
    g.AddEdge("B", "C");
    result = g.ProNumbering("A");
    expectation = {"A", "B", "C"};
    assert(result == expectation);
    //тест на корректность работы с циклом
    g.AddEdge("C", "A");
    result = g.ProNumbering("C");
    expectation = {"C", "A", "B"};
    assert(result == expectation);
    std::cout << "Тесты на базовые операции и PRO_NUMBERING пройдены" << std::endl;
}