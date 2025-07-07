#include "graph.h"
#pragma once

int Graph::FordFulkerson(const std::string& source_id, const std::string& sink_id) {
    if (!node_map.count(source_id) || !node_map.count(sink_id)) {
        std::cout << "Unknown node(s)" << std::endl;
        return 0;
    }

    Node* source = node_map[source_id];
    Node* sink = node_map[sink_id];

    // Поток на каждом ребре (изначально ноль)
    std::unordered_map<Edge*, int> flow;

    // Добавим обратные рёбра (виртуально, в residual graph, но не изменяем граф)
    while (true) {
        std::unordered_map<Node*, Edge*> parent;
        std::queue<Node*> q;
        q.push(source);

        // BFS для нахождения увеличивающего пути
        while (!q.empty()) {
            Node* curr = q.front(); q.pop();
            for (Edge* edge : curr->out_edges) {
                Node* to = edge->to;
                int capacity = edge->weight;
                int used_flow = flow[edge];
                int residual = capacity - used_flow;

                if (residual > 0 && !parent.count(to)) {
                    parent[to] = edge;
                    q.push(to);
                }
            }

            // Добавляем возможность обратного потока
            for (Edge* edge : curr->in_edges) {
                Node* from = edge->from;
                if (flow[edge] > 0 && !parent.count(from)) {
                    parent[from] = edge;  // используем обратное направление
                    q.push(from);
                }
            }
        }

        // Нет пути до sink — всё
        if (!parent.count(sink)) {
            break;
        }

        // Найдём минимальную пропускную способность на пути
        int path_flow = INT_MAX;
        for (Node* v = sink; v != source;) {
            Edge* e = parent[v];
            if (e->to == v) {
                path_flow = std::min(path_flow, e->weight - flow[e]);
                v = e->from;
            } else {
                // обратный путь
                path_flow = std::min(path_flow, flow[e]);
                v = e->to;
            }
        }

        // Обновим потоки
        for (Node* v = sink; v != source;) {
            Edge* e = parent[v];
            if (e->to == v) {
                flow[e] += path_flow;
                v = e->from;
            } else {
                flow[e] -= path_flow;
                v = e->to;
            }
        }
    }

    // Подсчёт общего потока из source
    int max_flow = 0;
    for (Edge* edge : source->out_edges) {
        max_flow += flow[edge];
    }

    return max_flow;
}
