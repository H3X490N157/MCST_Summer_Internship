#include "dijkstra.h"
#include "fordfulkerson.h"
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
        }else if (cmd == "PRO_NUMBERING") {
            std::string id;
            input >> id;
            std::vector<std::string> post_order = graph.ProNumbering(id);
            for (size_t i = 0; i < post_order.size(); ++i) {
                std::cout << post_order[post_order.size() - i - 1];
                if (i + 1 < post_order.size()) {
                    std::cout << " ";
                }
            }
            std::cout << std::endl;
        } else if (cmd == "DIJKSTRA"){
            std::string id;
            ss >> id;
            graph.Dijkstra(id);
        } else if (cmd == "MAX"){
            std::string skip, a, b;
            ss >> skip >> a >> b;
            std::cout << graph.FordFulkerson(a, b);
        }

    }
    return 0;
}

