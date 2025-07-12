#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <queue>
#include <iostream>
#include <limits>
#include <climits>

#include "edge.h"

#pragma once

class Graph {
private:
    std::unordered_map<std::string, Node*> node_map;

    void DfsCycleDetect(Node* node,
                   std::unordered_set<Node*>& visited,
                   std::unordered_set<Node*>& rec_stack,
                   std::vector<std::pair<std::string, std::string>>& cycles) {
    visited.insert(node);//помечаем текущую вершину как посещённую
    rec_stack.insert(node);//добавляем её в стек текущего пути (для отслеживания циклов)

    for (Edge* edge : node->out_edges) {//обходим всех соседей, в которых можно попасть по рёбрам
        Node* neighbour = edge->to;
        if (rec_stack.count(neighbour)) { //если сосед уже в стеке, значит -  цикл
            cycles.emplace_back(neighbour->id, node->id);//добавляем информацию о цикле
        } 
        else if (!visited.count(neighbour)) { //DFS от соседа
            DfsCycleDetect(neighbour, visited, rec_stack, cycles);
        }
    } 
    rec_stack.erase(node);//чистим стек
}


    void DfsPostOrder(Node* node, std::unordered_set<Node*>& visited,
                      std::vector<std::string>& result) { //метод для пронумерования с вершин с помощью пост-порядка (пост-ордера)
        visited.insert(node);
        for (Edge* edge : node->out_edges) {
            if (!visited.count(edge->to)) {
                DfsPostOrder(edge->to, visited, result); //буквально рекурсивный вызов DFS для пронумерования
            }
        }
        result.push_back(node->id); //добавление в вектор с результатом полученного порядка
    }

public:
    ~Graph() { // санитайзер ругался из-за отсутствия деструктора
        for (std::unordered_map<std::string, Node*>::iterator it = node_map.begin(); it != node_map.end(); ++it) {
            Node* node = it->second;
            for (std::vector<Edge*>::iterator edge_it = node->out_edges.begin(); edge_it != node->out_edges.end(); ++edge_it) {
                delete *edge_it;
            }
            
            node->out_edges.clear();
            node->in_edges.clear(); // чистим входящие и исходящие рёбры
        }
        
        for (std::unordered_map<std::string, Node*>::iterator it = node_map.begin(); it != node_map.end(); ++it) {
            delete it->second; // вычищаем вершины
        }
        node_map.clear(); // на всякий случай дочищаем node_map
    }
    
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
        for (std::vector<Edge*>::iterator it = target->in_edges.begin(); it != target->in_edges.end(); ++it) { // удаление всех входящих рёбер перед удалением самой вершины 
            Node* from = (*it)->from;
            std::vector<Edge*>& out_vec = from->out_edges;
            for (std::vector<Edge*>::iterator out_it = out_vec.begin(); out_it != out_vec.end(); ++out_it) {
                if (*out_it == *it) {
                    out_vec.erase(out_it);
                    break;
                }
            }
            delete *it;
        }

        for (std::vector<Edge*>::iterator it = target->out_edges.begin(); it != target->out_edges.end(); ++it) { // теперь удаление всех исходящих рёбер
            Node* to = (*it)->to;
            std::vector<Edge*>& in_vec = to->in_edges;
            for (std::vector<Edge*>::iterator in_it = in_vec.begin(); in_it != in_vec.end(); ++in_it) {
                if (*in_it == *it) {
                    in_vec.erase(in_it);
                    break;
                }
            }
            delete *it;
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

        for (std::vector<Edge*>::iterator it = from_node->out_edges.begin(); it != from_node->out_edges.end(); ++it) {
            if ((*it)->to == to_node) {
                Edge* edge = *it;
                from_node->out_edges.erase(it);

                std::vector<Edge*>& in_vec = to_node->in_edges;
                for (std::vector<Edge*>::iterator in_it = in_vec.begin(); in_it != in_vec.end(); ++in_it) {
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

    std::vector<std::string> RpoNumbering(const std::string& start_id) {
        std::vector<std::string> post_order;
        if (!node_map.count(start_id)) {
            std::cout << "Unknown node " << start_id << std::endl;
            return post_order;
        }

        std::unordered_set<Node*> visited, rec_stack;
        std::vector<std::pair<std::string, std::string>> cycles;

        DfsCycleDetect(node_map[start_id], visited, rec_stack, cycles);
        for (std::pair<std::string, std::string> p : cycles) {
            std::cout << "Found loop " << p.first << "->" << p.second << std::endl;
        }

        visited.clear();
        DfsPostOrder(node_map[start_id], visited, post_order);

        std::vector<std::string> order;
        for (size_t i = 0; i < post_order.size(); ++i) {
            order.push_back(post_order[post_order.size() - i - 1]);
        }
        return order;
    }


    void Dijkstra (const std::string& start_id);
};
