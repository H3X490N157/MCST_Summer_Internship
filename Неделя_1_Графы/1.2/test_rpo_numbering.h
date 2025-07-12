#include "graph.h"
#pragma once

void TestRpoNumbering() {//так как здесь особо нечего тестировать, то покрытие тестами будет меньше, чем для дальнейних подзаданий
    Graph g;
    //дефолтный тест на пронумерование
    g.AddNode("A");
    g.AddNode("B");
    g.AddEdge("A", "B", 0);
    std::vector<std::string> result = g.RpoNumbering("A");
    std::vector<std::string> expectation = {"A", "B"};
    assert(result == expectation);
    //тест на корректность работы удаления узла и пронумерования
    g.RemoveNode("A");
    result = g.RpoNumbering("B");
    expectation = {"B"};
    assert(result == expectation);
    //тест на корректность работы пронумеровки с 3 узлами
    g.AddNode("A");
    g.AddNode("C");
    g.AddEdge("A", "B", 1);
    g.AddEdge("B", "C", 1);
    result = g.RpoNumbering("A");
    expectation = {"A", "B", "C"};
    assert(result == expectation);
    //тест на корректность работы с циклом
    g.AddEdge("C", "A", 0);
    result = g.RpoNumbering("C");
    expectation = {"C", "A", "B"};
    assert(result == expectation);
    ///добавил тестов на несколько циклов
    g.RemoveNode("A");
    g.RemoveNode("B");
    g.RemoveNode("C");

    g.AddNode("A");
    g.AddNode("B");
    g.AddNode("C");
    g.AddNode("D");

    g.AddEdge("A", "B", 1);
    g.AddEdge("B", "D", 1);
    g.AddEdge("D", "A", 1);
    g.AddEdge("D", "C", 1);
    g.AddEdge("C", "B", 1);
    result = g.RpoNumbering("A");
    std::vector<std::string> nodes = {"A", "B", "D", "C"};
    assert(result.size() == 4);
    assert (result == nodes);
    //ещё один тест с тремя циклами
    g.RemoveNode("A");
    g.RemoveNode("B");
    g.RemoveNode("C");
    g.RemoveNode("D");
    nodes = {"A", "B", "C", "D", "E", "F", "G", "H"};
    for (std::string noda : nodes) {
        g.AddNode(noda);
    }
    
    g.AddEdge("A", "B", 1);
    g.AddEdge("B", "C", 1);
    g.AddEdge("C", "E", 2);
    g.AddEdge("E", "F", 2);
    g.AddEdge("F", "H", 3);
    g.AddEdge("C", "D", 55);
    g.AddEdge("D", "B", 42); 
    g.AddEdge("F", "G", 7);
    g.AddEdge("G", "E", 55); 
    g.AddEdge("F", "B", 5); 
    result = g.RpoNumbering("A");
    assert(result.size() == 8);
    assert(result == nodes);
    std::cout << "Тесты на базовые операции и PRO_NUMBERING пройдены" << std::endl;
}