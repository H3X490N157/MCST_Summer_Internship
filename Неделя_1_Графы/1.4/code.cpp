#include "dijkstra.h"
#include "fordfulkerson.h"
#include "tarjan.h"

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
        else if (cmd == "RPO_NUMBERING") {
            std::string id;
            std::cin >> id;
            std::vector<std::string> post_order = graph.RpoNumbering(id);
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
        else if (cmd == "MAX") {
            std::string skip, a, b;
            std::cin >> skip >> a >> b;
            std::cout << graph.FordFulkerson(a, b) << std::endl;
        } 
        else if (cmd == "TARJAN") {
            std::string a;
            std::cin >> a;
            std::vector<std::vector<Node*>> result = graph.Tarjan(a);
            for (std::vector<Node*> comp: result) {
                for (size_t i = 0; i < comp.size(); ++i) {
                    std::cout << comp[i]->id;
                    if (i < comp.size() - 1) {
                        std::cout << " ";
                    }
                }
                std::cout << std::endl;
            }
        }
    }
    return 0;
}

