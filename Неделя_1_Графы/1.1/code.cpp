#include "graph.h"
#include <sstream>

//примитивная консоль без UI
int main() {
    Graph graph;
    std::string line;
    while (std::getline(std::cin, line)) {
        std::stringstream input(line);
        std::string cmd;
        input >> cmd;
        if (cmd == "NODE") {
            std::string id;
            input >> id;
            graph.AddNode(id);
        } else if (cmd == "EDGE") {
            std::string from, to;
            int weight;
            input >> from >> to >> weight;
            graph.AddEdge(from, to, weight);
        } else if (cmd == "REMOVE") {
            std::string type, a, b;
            input >> type >> a;
            if (type == "NODE") {
                graph.RemoveNode(a);
            } else if (type == "EDGE") {
                input >> b;
                graph.RemoveEdge(a, b);
            }
        } else if (cmd == "PRO_NUMBERING") {
            std::string id;
            input >> id;
            std::vector<std::string> post_order = graph.ProNumbering(id);
            for (size_t i = 0; i < post_order.size(); ++i) {
                std::cout << post_order[i];
                if (i < post_order.size() - 1) {
                    std::cout << " ";
                }
            }
            std::cout << std::endl;
        }
    }
    return 0;
}

