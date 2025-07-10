#include "dijkstra.h"

//реализация примитивной консоли
int main() {
    Graph graph;
    std::string cmd;

    while (std::cin >> cmd) {
        if (cmd == "NODE") {
            std::string id;
            std::cin >> id;
            graph.AddNode(id);
        } 
        else if (cmd == "EDGE") {
            std::string from, to;
            int weight;
            std::cin >> from >> to >> weight;
            graph.AddEdge(from, to, weight);
        } 
        else if (cmd == "REMOVE") {
            std::string type, a, b;
            std::cin >> type >> a;
            if (type == "NODE") {
                graph.RemoveNode(a);
            } else if (type == "EDGE") {
                std::cin >> b;
                graph.RemoveEdge(a, b);
            }
        } 
        else if (cmd == "PRO_NUMBERING") {
            std::string id;
            std::cin >> id;
            std::vector<std::string> post_order = graph.ProNumbering(id);
            for (size_t i = 0; i < post_order.size(); ++i) {
                std::cout << post_order[i];
                if (i < post_order.size() - 1) {
                    std::cout << " ";
                }
            }
            std::cout << std::endl;
        } 
        else if (cmd == "DIJKSTRA") {
            std::string id;
            std::cin >> id;
            graph.Dijkstra(id);
        } 
    }
    return 0;
}

