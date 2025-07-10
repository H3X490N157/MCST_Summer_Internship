#include <tuple>
#include <algorithm>
#include <set>

#include "tarjan.h"

#pragma once

std::vector<std::vector<std::string>> ConvertToStringComponents(const std::vector<std::vector<Node*>>& node_components) {//костыльная вспомогательная функция для смены типа 
    std::vector<std::vector<std::string>> strong_components;
    for (const auto& comp : node_components) {//так как здесь тесты, то затраты по ресурсам не так страшны, поэтому использую auto
        std::vector<std::string> comp_names;
        for (const Node* node : comp) {
            comp_names.push_back(node->id);
        }
        strong_components.push_back(comp_names);
    }
    return strong_components;
}

void TestTarjan() {
    Graph test_graph;
    std::vector<std::vector<std::string>> strong_components;

    //одна вершина
    test_graph.AddNode("A");
    std::vector<std::vector<Node*>> raw_components = test_graph.Tarjan("A");
    strong_components = ConvertToStringComponents(raw_components);
    assert(strong_components.empty());
    test_graph.RemoveNode("A");

    //два несвязанных между собой узла
    test_graph.AddNode("A");
    test_graph.AddNode("B");
    raw_components = test_graph.Tarjan("A");
    strong_components = ConvertToStringComponents(raw_components);
    
    assert(strong_components.empty());
    test_graph.RemoveNode("A");
    test_graph.RemoveNode("B");

    //циклическая КСС
    test_graph.AddNode("A");
    test_graph.AddNode("B");
    test_graph.AddNode("C");
    test_graph.AddEdge("A", "B", 1);
    test_graph.AddEdge("B", "C", 1);
    test_graph.AddEdge("C", "A", 1);
    raw_components = test_graph.Tarjan("A");
    strong_components = ConvertToStringComponents(raw_components);

    assert(strong_components.size() == 1);
    std::vector<std::string> cycle = strong_components[0];
    std::sort(cycle.begin(), cycle.end());
    assert(cycle == std::vector<std::string>({"A", "B", "C"}));
    test_graph.RemoveNode("A");
    test_graph.RemoveNode("B");
    test_graph.RemoveNode("C");

    //две КСС, но достижима лишь одна
    test_graph.AddNode("A");
    test_graph.AddNode("B");
    test_graph.AddNode("C");
    test_graph.AddNode("D");
    test_graph.AddNode("E");
    test_graph.AddEdge("A", "B", 1);
    test_graph.AddEdge("B", "C", 1);
    test_graph.AddEdge("C", "A", 1);
    test_graph.AddEdge("D", "E", 1);
    test_graph.AddEdge("E", "D", 1);

    raw_components = test_graph.Tarjan("A");
    strong_components = ConvertToStringComponents(raw_components);
    int count = 0;
    for (const auto& comp : strong_components) {
        if (comp.size() > 1) {
            count++;
        }
    }
    assert(count == 1);
    
    test_graph.RemoveNode("A");
    test_graph.RemoveNode("B");
    test_graph.RemoveNode("C");
    test_graph.RemoveNode("D");
    test_graph.RemoveNode("E");

    //3 КСС + рёбра и вершины, которые в КСС не входят, при этом все 3 достижимы из A
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
    test_graph.AddNode("K");
    
    test_graph.AddEdge("A", "B", 1);
    test_graph.AddEdge("B", "C", 1);
    test_graph.AddEdge("C", "A", 1); //первая КСС
    
    test_graph.AddEdge("B", "D", 1);

    test_graph.AddEdge("D", "E", 1);
    test_graph.AddEdge("E", "F", 1);
    test_graph.AddEdge("F", "D", 1); //вторая КСС
    
    test_graph.AddEdge("F", "G", 1); 
    test_graph.AddEdge("G", "H", 1);
    test_graph.AddEdge("H", "I", 1);
    test_graph.AddEdge("I", "J", 1);
    test_graph.AddEdge("J", "G", 1); //третья КСС
    
    test_graph.AddEdge("J", "K", 1); 
    raw_components = test_graph.Tarjan("A");
    strong_components = ConvertToStringComponents(raw_components);
    count = 0;
    for (const auto& comp : strong_components) {
        if (comp.size() > 1) {
            count++;
        }
    }
    assert(count == 3);

    std::cout << "Тесты на поиск компонент сильной связности пройдены " << std::endl;
}
