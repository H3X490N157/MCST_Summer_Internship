#include "graph.h"
#include <climits>

void Graph::Dijkstra(const std::string& start_id) { 
        if (node_map.find(start_id) == node_map.end()) {
            std::cout << "Unknown node " << start_id << std::endl;
            return;
        }


        Node* start_node = node_map[start_id];

        std::unordered_map<Node*, int> D;
        
        for (const auto& pair : node_map) {
            D[pair.second] = INT_MAX;  //все вершины изначально на "бесконечности"
        }
        D[start_node] = 0;

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

            for (Edge* edge : v->out_edges) {
                Node* u = edge->to;
                int W = edge->weight;
                
                D[u] = std::min(D[u], D[v] + W); 
                if (D[u] < 0){
                    D[u] = INT_MAX; //слабость в алгоритме. Если вершина B недостижима из A, а C достижима из B, то дистанция до C будет переопределена, как INT_MAX + что -то = чуть больше, чем INT_MIN. Для борьбы с этим добавлена доп.проверка
                }
            }
        }

        // Выводим результат
        for (const auto& id : node_order) {
    if (id == start_id) continue;

    Node* node = node_map[id];
    int dist = D[node];
    if (dist == INT_MAX) {
        std::cout << id << " INF" << std::endl; //для недостижимых вершин
    } else {
        std::cout << id << " " << dist << std::endl;
    }
}
    }