#include "graph.h"
#include <climits>
#pragma once

void Graph::Dijkstra(const std::string& start_id) {//так как у нас требует вывод INF, в случае, если вершина недостижимы, а я не придумал, как сделать вывод возврат INF, не превращая функцию в std::string distancesijkstra, то это будем void-функция с выводом текста в консоль изнутри (что может быть моветоном)
    if (node_map.find(start_id) == node_map.end()) {
        std::cout << "Unknown node " << start_id << std::endl;
        return;
    }

    Node* start_node = node_map[start_id];

    std::unordered_map<Node*, int> distances;
    std::unordered_map<Node*, bool> visited_nodes;//храню в словаре список посещённых вершин и дистанции до них
    
    std::unordered_map<std::string, Node*>::iterator it;//здесь и в дальнейшем не использую auto, а итераторы прописываю явно для оптимизации кода

    for (it = node_map.begin(); it != node_map.end(); ++it) {
        distances[it->second] = INT_MAX;//предварительно устанавливаем максимальное расстояние до вершины, как максимальное число
    }
    distances[start_node] = 0;
    for (it = node_map.begin(); it != node_map.end(); ++it) {
        visited_nodes[it->second] = false;
    }

    size_t len = node_map.size();
    for (size_t i = 0; i < len; ++i) {
        Node* v = nullptr;
        for (it = node_map.begin(); it != node_map.end(); ++it) {//поиск вершины с минимальным расстоянием
            Node* u = it->second;
            if (!visited_nodes[u] && (v == nullptr || distances[u] < distances[v])) {
                v = u;
            }
        }

        if (v == nullptr){
            break; //проверка на случай, если все вершины уже обработаны
        } 

        visited_nodes[v] = true;

        std::vector<Edge*>::iterator it_two;
        for (it_two = v->out_edges.begin(); it_two != v->out_edges.end(); ++it_two) {//обрабатываем все рёбра, исходящие из текущей вершины
            Edge* edge = *it_two;
            Node* u = edge->to;
            int W = edge->weight;

            distances[u] = std::min(distances[u], distances[v] + W);
            if (distances[u] < 0){
                distances[u] = INT_MAX; //если есть вершины visited_nodes, B, C, при этом есть только ребро B, C, то расстояние до C = INT_MAX + BC, что из-за семантики плюсов будет чуть больше INT_MIN и значительно меньше 0, поэтому нужно делать эту проверку
            }
        }
    }

    for (it = node_map.begin(); it != node_map.end(); ++it) {
        Node* node = it->second;
        int dist = distances[node];
        if (node->id == start_id){
            continue;
        } else if (dist == INT_MAX) {
            std::cout << node->id << " INF" << std::endl;
        } 
        else {
            std::cout << node->id << " " << dist << std::endl;
        }
    }
}
