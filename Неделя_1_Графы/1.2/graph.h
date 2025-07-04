#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <queue>
#include <iostream>
#include <limits>

#include "edge.h"

class Graph {
private:
    std::unordered_map<std::string, Node*> node_map;

    bool DfsCycleDetect(Node* node, std::unordered_set<Node*>& visited,
                        std::unordered_set<Node*>& rec_stack,
                        std::string& loop_from, std::string& loop_to) {
        visited.insert(node);
        rec_stack.insert(node);

        for (Edge* edge : node->out_edges) {
            Node* neighbor = edge->to;
            if (rec_stack.count(neighbor)) {
                loop_from = node->id;
                loop_to = neighbor->id;
                return true;
            } else if (!visited.count(neighbor)) {
                if (DfsCycleDetect(neighbor, visited, rec_stack, loop_from, loop_to))
                    return true;
            }
        }

        rec_stack.erase(node);
        return false;
    }

    void DfsPostOrder(Node* node, std::unordered_set<Node*>& visited,
                      std::vector<std::string>& result) {
        visited.insert(node);
        for (Edge* edge : node->out_edges) {
            if (!visited.count(edge->to)) {
                DfsPostOrder(edge->to, visited, result);
            }
        }
        result.push_back(node->id);
    }

public:
    void AddNode(const std::string& id) {
        if (!node_map.count(id)) {
            node_map[id] = new Node(id);
        }
    }

    void AddEdge(const std::string& from, const std::string& to, int weight) {
        if (!node_map.count(from) && !node_map.count(to)) {
            std::cout << "Unknown nodes " << from << " " << to << std::endl;
            return;
        } else if (!node_map.count(from)) {
            std::cout << "Unknown node " << from << std::endl;
            return;
        } else if (!node_map.count(to)) {
            std::cout << "Unknown node " << to << std::endl;
            return;
        }

        Node* from_node = node_map[from];
        Node* to_node = node_map[to];
        Edge* edge = new Edge(from_node, to_node, weight);
        from_node->out_edges.push_back(edge);
        to_node->in_edges.push_back(edge);
    }

    void RemoveNode(const std::string& id) {
        if (!node_map.count(id)) {
            std::cout << "Unknown node " << id << std::endl;
            return;
        }

        Node* target = node_map[id];

        // Удалить все входящие рёбра
        for (Edge* edge : target->in_edges) {
            Node* from = edge->from;
            auto& out_vec = from->out_edges;
            for (auto it = out_vec.begin(); it != out_vec.end(); ++it) {
                if (*it == edge) {
                    out_vec.erase(it);
                    break;
                }
            }
            delete edge;
        }

        // Удалить все исходящие рёбра
        for (Edge* edge : target->out_edges) {
            Node* to = edge->to;
            auto& in_vec = to->in_edges;
            for (auto it = in_vec.begin(); it != in_vec.end(); ++it) {
                if (*it == edge) {
                    in_vec.erase(it);
                    break;
                }
            }
            delete edge;
        }

        delete target;
        node_map.erase(id);
    }

    void RemoveEdge(const std::string& from, const std::string& to) {
        if (!node_map.count(from) && !node_map.count(to)) {
            std::cout << "Unknown nodes " << from << " " << to << std::endl;
            return;
        } else if (!node_map.count(from)) {
            std::cout << "Unknown node " << from << std::endl;
            return;
        } else if (!node_map.count(to)) {
            std::cout << "Unknown node " << to << std::endl;
            return;
        }

        Node* from_node = node_map[from];
        Node* to_node = node_map[to];

        for (auto it = from_node->out_edges.begin(); it != from_node->out_edges.end(); ++it) {
            if ((*it)->to == to_node) {
                Edge* edge = *it;
                from_node->out_edges.erase(it);

                auto& in_vec = to_node->in_edges;
                for (auto in_it = in_vec.begin(); in_it != in_vec.end(); ++in_it) {
                    if (*in_it == edge) {
                        in_vec.erase(in_it);
                        break;
                    }
                }

                delete edge;
                break;
            }
        }
    }

    void ProNumbering(const std::string& start_id) {
        if (!node_map.count(start_id)) {
            std::cout << "Unknown node " << start_id << std::endl;
            return;
        }

        std::unordered_set<Node*> visited, rec_stack;
        std::string loop_from, loop_to;

        if (DfsCycleDetect(node_map[start_id], visited, rec_stack, loop_from, loop_to)) {
            std::cout << "Found loop " << loop_to << "->" << loop_from << std::endl;
        }

        visited.clear();
        std::vector<std::string> post_order;
        DfsPostOrder(node_map[start_id], visited, post_order);

        for (size_t i = 0; i < post_order.size(); ++i) {
            std::cout << post_order[post_order.size() - i - 1];
            if (i + 1 < post_order.size())
                std::cout << " ";
        }
        std::cout << std::endl;
    }

    void Dijkstra (const std::string& start_id);
};
