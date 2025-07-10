#include "dijkstra.h"
#include <sstream>
#pragma once

void TestDijkstra() {//т.к. Дейкстра у меня void-функция, которая ничего не возвращает, но печает в консоль, то через sstream я буду считывать выведенные результаты из консоли и уже затем проверять 
    Graph test_graph;
    std::ostringstream out;
    std::streambuf* old_buf = std::cout.rdbuf(out.rdbuf());
    std::string result;

    //тест на 1 вершину
    test_graph.AddNode("A");
    out.str(""); out.clear();
    test_graph.Dijkstra("A");
    assert(out.str() == "");

    //тест на 2 несвязные вершины
    test_graph.AddNode("B");
    out.str(""); out.clear();
    test_graph.Dijkstra("A");
    assert(out.str() == "B INF\n");

    //связные вершины с одним путём
    test_graph.AddEdge("A", "B", 10);
    out.str(""); out.clear();
    test_graph.Dijkstra("A");
    assert(out.str() == "B 10\n");

    //три вершины
    test_graph.AddNode("C");
    test_graph.AddEdge("B", "C", 5);
    out.str(""); out.clear();
    test_graph.Dijkstra("A");
    result = out.str();
    assert(result.find("B 10\n") != std::string::npos);
    assert(result.find("C 15\n") != std::string::npos);

    //три вершины с циклом
    test_graph.AddEdge("C", "A", 2);
    out.str(""); out.clear();
    test_graph.Dijkstra("A");
    result = out.str();
    assert(result.find("B 10\n") != std::string::npos);
    assert(result.find("C 15\n") != std::string::npos);

    test_graph.RemoveNode("A");
    test_graph.RemoveNode("B");
    test_graph.RemoveNode("C");

    //много путей, выбор кратчайшего
    test_graph.AddNode("A");
    test_graph.AddNode("B");
    test_graph.AddNode("C");
    test_graph.AddNode("D");
    test_graph.AddEdge("A", "B", 2);
    test_graph.AddEdge("A", "C", 5);
    test_graph.AddEdge("B", "C", 1);
    test_graph.AddEdge("C", "D", 1);
    out.str(""); out.clear();
    test_graph.Dijkstra("A");
    result = out.str();
    assert(result.find("B 2\n") != std::string::npos);
    assert(result.find("C 3\n") != std::string::npos);
    assert(result.find("D 4\n") != std::string::npos); 

    test_graph.RemoveNode("A");
    test_graph.RemoveNode("B");
    test_graph.RemoveNode("C");
    test_graph.RemoveNode("D");

    //тест на 6 вершин
    test_graph.AddNode("A");
    test_graph.AddNode("B");
    test_graph.AddNode("C");
    test_graph.AddNode("D");
    test_graph.AddNode("E");
    test_graph.AddNode("F");
    test_graph.AddEdge("A", "B", 1);
    test_graph.AddEdge("B", "C", 2);
    test_graph.AddEdge("A", "D", 4);
    test_graph.AddEdge("D", "E", 1);
    test_graph.AddEdge("E", "C", 1);
    test_graph.AddEdge("C", "F", 3);
    out.str(""); out.clear();
    test_graph.Dijkstra("A");
    result = out.str();
    assert(result.find("B 1\n") != std::string::npos);
    assert(result.find("C 3\n") != std::string::npos);
    assert(result.find("D 4\n") != std::string::npos);
    assert(result.find("E 5\n") != std::string::npos);
    assert(result.find("F 6\n") != std::string::npos); 

    test_graph.RemoveNode("A");
    test_graph.RemoveNode("B");
    test_graph.RemoveNode("C");
    test_graph.RemoveNode("D");
    test_graph.RemoveNode("E");
    test_graph.RemoveNode("F");

    //тест на 10 вершин
    test_graph.AddNode("A");
    test_graph.AddNode("B");
    test_graph.AddNode("C");
    test_graph.AddNode("D");
    test_graph.AddNode("E");
    test_graph.AddNode("F");
    test_graph.AddNode("G");
    test_graph.AddNode("H");
    test_graph.AddNode("I");
    test_graph.AddNode("J");

    test_graph.AddEdge("A", "B", 3);
    test_graph.AddEdge("A", "C", 2);
    test_graph.AddEdge("B", "D", 4);
    test_graph.AddEdge("C", "D", 1);
    test_graph.AddEdge("D", "E", 6);
    test_graph.AddEdge("C", "F", 8);
    test_graph.AddEdge("F", "G", 2);
    test_graph.AddEdge("E", "H", 1);
    test_graph.AddEdge("G", "I", 1);
    test_graph.AddEdge("H", "J", 3);

    out.str(""); out.clear();
    test_graph.Dijkstra("A");
    result = out.str();
    assert(result.find("B 3\n") != std::string::npos);
    assert(result.find("C 2\n") != std::string::npos);
    assert(result.find("D 3\n") != std::string::npos);
    assert(result.find("E 9\n") != std::string::npos);
    assert(result.find("F 10\n") != std::string::npos);
    assert(result.find("G 12\n") != std::string::npos);
    assert(result.find("H 10\n") != std::string::npos);
    assert(result.find("I 13\n") != std::string::npos);
    assert(result.find("J 13\n") != std::string::npos);

    std::cout.rdbuf(old_buf);
    std::cout << "Тесты на алгоритм Дейкстры пройдены" << std::endl;
}