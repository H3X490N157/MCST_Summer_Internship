#include "dijkstra.h"
#include <sstream>

//предварительные тесты
int main() {
    Graph graph;
    std::string line;
    while (std::getline(std::cin, line)) {
        std::stringstream ss(line);
        std::string cmd;
        ss >> cmd;
        if (cmd == "NODE") {
            std::string id;
            ss >> id;
            graph.AddNode(id);
        } else if (cmd == "EDGE") {
            std::string from, to;
            int weight;
            ss >> from >> to >> weight;
            graph.AddEdge(from, to, weight);
        } else if (cmd == "REMOVE") {
            std::string type, a, b;
            ss >> type >> a;
            if (type == "NODE") {
                graph.RemoveNode(a);
            } else if (type == "EDGE") {
                ss >> b;
                graph.RemoveEdge(a, b);
            }
        } else if (cmd == "PRO_NUMBERING") {
            std::string id;
            ss >> id;
            graph.ProNumbering(id);
        } else if (cmd == "DIJKSTRA"){
            std::string id;
            ss >> id;
            graph.Dijkstra(id);
        }
    }
    return 0;
}

