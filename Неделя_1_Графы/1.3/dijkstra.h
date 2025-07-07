#include "graph.h"
#pragma once

void Graph::Dijkstra(const std::string& start_id) {
        if (node_map.find(start_id) == node_map.end()) {
            std::cout << "Unknown node " << start_id << std::endl;
            return;
        }


        Node* start_node = node_map[start_id];

        std::unordered_map<Node*, int> D;
        for (const auto& pair : node_map) {
            D[pair.second] = std::numeric_limits<int>::max();  // Все вершины изначально на "бесконечности"
        }
        D[start_node] = 0;

        // Массив для проверки посещённых вершин
        std::unordered_map<Node*, bool> A;
        for (const auto& pair : node_map) {
            A[pair.second] = false;  // Все вершины ещё не обработаны
        }

        // Основной цикл алгоритма Дейкстры
        size_t len = node_map.size();
        for (size_t i = 0; i < len; ++i) {
            Node* v = nullptr;

            // Находим вершину с минимальным расстоянием
            for (const auto& pair : node_map) {
                Node* u = pair.second;
                if (!A[u] && (v == nullptr || D[u] < D[v])) {
                    v = u;
                }
            }

            if (v == nullptr) break;  // Все вершины обработаны

            A[v] = true;  // Помечаем вершину как обработанную

            // Обрабатываем все рёбра, исходящие из текущей вершины
            for (Edge* edge : v->out_edges) {
                Node* u = edge->to;
                int W = edge->weight;

                // Обновляем расстояние до соседей
                D[u] = std::min(D[u], D[v] + W);
            }
        }

        // Выводим результат
        for (const auto& pair : node_map) {
            Node* node = pair.second;
            int dist = D[node];
            if (node->id == start_id){
                continue;
            } else if (dist == std::numeric_limits<int>::max()) {
                std::cout << node->id << " INF" << std::endl;
            } 
            else {
                std::cout << node->id << " " << dist << std::endl;
            }
        }
    }