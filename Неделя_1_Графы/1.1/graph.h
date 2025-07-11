#include <unordered_map>
#include <unordered_set>
#include <stack>
#pragma once

#include "edge.h"

class Graph {
private:
    std::unordered_map<std::string, Node*> node_map;

    bool DfsCycleDetect(Node* node, std::unordered_set<Node*>& visited,
                        std::unordered_set<Node*>& rec_stack,
                        std::string& loop_from, std::string& loop_to) {
        visited.insert(node); //помечаем вершину, как посещённую
        rec_stack.insert(node); //добавляем в стек для 
        for (Edge* edge : node->out_edges) { //проверяем все вершины, в которые можем попасть из текущей
            Node* neighbor = edge->to;
            if (rec_stack.count(neighbor)) { //если её вершина-сосед уже в стеке, значит, есть цикл
                loop_from = node->id; //изменяем переменные начала конца и цикла
                loop_to = neighbor->id;
                return true;
            } else if (!visited.count(neighbor)) {
                if (DfsCycleDetect(neighbor, visited, rec_stack, loop_from, loop_to))//цикла нет - ищем дальше
                    return true;
            }
        }
        rec_stack.erase(node); //чистим стек от текущей вершины,
        return false;
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

    void AddEdge(const std::string& from, const std::string& to) {
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
        Edge* edge = new Edge(from_node, to_node);
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

    std::vector<std::string> ProNumbering(const std::string& start_id) {
        std::vector<std::string> post_order;

        if (!node_map.count(start_id)) {
            std::cout << "Unknown node " << start_id << std::endl;
            return post_order;
        }

        std::unordered_set<Node*> visited, rec_stack;
        std::string loop_from, loop_to;

        if (DfsCycleDetect(node_map[start_id], visited, rec_stack, loop_from, loop_to)) {
            std::cout << "Found loop " << loop_to << "->" << loop_from << std::endl;
        }

        visited.clear();

        DfsPostOrder(node_map[start_id], visited, post_order);

        std::vector<std::string> order;
        for (size_t i = 0; i < post_order.size(); ++i) {
            order.push_back(post_order[post_order.size() - i - 1]);
        }
        return order; 
    }
};
