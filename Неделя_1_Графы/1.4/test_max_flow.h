#include "fordfulkerson.h"
#pragma once

void TestFordFulkerson(){
    Graph test_graph;
    int result;

    //простой слсучай
    test_graph.AddNode("S");
    test_graph.AddNode("T");
    test_graph.AddEdge("S", "T", 10);
    result = test_graph.FordFulkerson("S", "T");
    assert(result == 10);
    test_graph.RemoveNode("S");
    test_graph.RemoveNode("T");

    //несколько путей с одинаковой пропускной способностью
    test_graph.AddNode("S");
    test_graph.AddNode("A");
    test_graph.AddNode("B");
    test_graph.AddNode("T");
    test_graph.AddEdge("S", "A", 10);
    test_graph.AddEdge("A", "T", 10);
    test_graph.AddEdge("S", "B", 10);
    test_graph.AddEdge("B", "T", 10);
    result = test_graph.FordFulkerson("S", "T");
    assert(result == 20);
    test_graph.RemoveNode("S");
    test_graph.RemoveNode("A");
    test_graph.RemoveNode("B");
    test_graph.RemoveNode("T");

    //узкое место, ограничивающее поток
    test_graph.AddNode("S");
    test_graph.AddNode("A");
    test_graph.AddNode("B");
    test_graph.AddNode("T");
    test_graph.AddEdge("S", "A", 100);
    test_graph.AddEdge("A", "B", 1);
    test_graph.AddEdge("B", "T", 100);
    result = test_graph.FordFulkerson("S", "T");
    assert(result == 1);
    test_graph.RemoveNode("S");
    test_graph.RemoveNode("A");
    test_graph.RemoveNode("B");
    test_graph.RemoveNode("T");

    //цикл с обратным ребром
    test_graph.AddNode("S");
    test_graph.AddNode("A");
    test_graph.AddNode("B");
    test_graph.AddNode("T");
    test_graph.AddEdge("S", "A", 10);
    test_graph.AddEdge("A", "B", 4);
    test_graph.AddEdge("B", "A", 6); //обратное ребро, создаёт цикл
    test_graph.AddEdge("B", "T", 10);
    result = test_graph.FordFulkerson("S", "T");
    assert(result == 4);
    test_graph.RemoveNode("S");
    test_graph.RemoveNode("A");
    test_graph.RemoveNode("B");
    test_graph.RemoveNode("T");

    //тест на 6 вершин
    test_graph.AddNode("S");
    test_graph.AddNode("A");
    test_graph.AddNode("B");
    test_graph.AddNode("C");
    test_graph.AddNode("D");
    test_graph.AddNode("T");
    test_graph.AddEdge("S", "A", 16);
    test_graph.AddEdge("S", "C", 13);
    test_graph.AddEdge("A", "B", 12);
    test_graph.AddEdge("B", "C", 9);
    test_graph.AddEdge("C", "A", 4);
    test_graph.AddEdge("B", "T", 20);
    test_graph.AddEdge("C", "D", 14);
    test_graph.AddEdge("D", "B", 7);
    test_graph.AddEdge("D", "T", 4);
    result = test_graph.FordFulkerson("S", "T");
    assert(result == 23);
    test_graph.RemoveNode("S");
    test_graph.RemoveNode("A");
    test_graph.RemoveNode("B");
    test_graph.RemoveNode("C");
    test_graph.RemoveNode("D");
    test_graph.RemoveNode("T");

    //тест на 19 вершин, искренне надеюсь, что считая потом руками, не обсчитался
    test_graph.AddNode("S");
    test_graph.AddNode("A");
    test_graph.AddNode("B");
    test_graph.AddNode("C");
    test_graph.AddNode("D");
    test_graph.AddNode("E");
    test_graph.AddNode("F");
    test_graph.AddNode("G");
    test_graph.AddNode("H");
    test_graph.AddNode("T");

    test_graph.AddEdge("S", "A", 10);
    test_graph.AddEdge("S", "C", 10);
    test_graph.AddEdge("A", "B", 4);
    test_graph.AddEdge("A", "D", 8);
    test_graph.AddEdge("B", "T", 10);
    test_graph.AddEdge("C", "D", 9);
    test_graph.AddEdge("C", "E", 4);
    test_graph.AddEdge("D", "B", 6);
    test_graph.AddEdge("D", "F", 10);
    test_graph.AddEdge("E", "F", 15);
    test_graph.AddEdge("F", "T", 10);
    test_graph.AddEdge("E", "G", 4);
    test_graph.AddEdge("G", "T", 10);
    test_graph.AddEdge("D", "G", 2);
    test_graph.AddEdge("B", "H", 3);
    test_graph.AddEdge("H", "T", 6);
    test_graph.AddEdge("C", "H", 3);
    test_graph.AddEdge("G", "H", 1);
    test_graph.AddEdge("A", "E", 2);
    test_graph.AddEdge("F", "H", 1);
    test_graph.AddEdge("E", "D", 3);
    test_graph.AddEdge("B", "G", 2);
    test_graph.AddEdge("G", "F", 1);

    result = test_graph.FordFulkerson("S", "T");
    assert(result == 20);
    std::cout << "Тесты на поиск макисмального потока пройдены" << std::endl;
}
