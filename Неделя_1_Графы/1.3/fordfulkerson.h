#include "graph.h"
#pragma once

int Graph::FordFulkerson(const std::string& source_id, const std::string& sink_id) {
    if (!node_map.count(source_id) || !node_map.count(sink_id)) {
        std::cout << "Unknown node(s)" << std::endl;//в ТЗ не было сказано, как действовать в случае неправильного ввода в MAX FLOW
        return 0;
    }

    Node* source = node_map[source_id];
    Node* sink = node_map[sink_id];
    std::unordered_map<Edge*, int> flow;//поток на каждом ребре, изначально минимален, т.е. 0

    while (true) {
        std::unordered_map<Node*, Edge*> parent;
        std::queue<Node*> q;
        q.push(source);

        while (!q.empty()) {//BFS для нахождения увеличивающего пути
            Node* curr = q.front(); q.pop();
            std::vector<Edge*>::iterator it = curr->out_edges.begin();
            while (it != curr->out_edges.end()) {
                Edge* edge = *it;
                Node* to = edge->to;
                int capacity = edge->weight;
                int used_flow = flow[edge];
                int residual = capacity - used_flow;

                if (residual > 0 && !parent.count(to)) {
                    parent[to] = edge;
                    q.push(to);
                }
                ++it;
            }

            it = curr->in_edges.begin();
            while (it != curr->in_edges.end()) {//обратный поток
                Edge* edge = *it;
                Node* from = edge->from;
                if (flow[edge] > 0 && !parent.count(from)) {
                    parent[from] = edge;  //используем обратное направление потока
                    q.push(from);
                }
                ++it;
            }
        }

        if (!parent.count(sink)) {//прерывание в случае отсутствия пути в sink
            break;
        }

        int path_flow = INT_MAX;
        for (Node* v = sink; v != source;) {//обновим пропускную способность
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

        for (Node* v = sink; v != source;) {//поиск потоков
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

    int max_flow = 0;//подсчёт общего потока
    std::vector<Edge*>::iterator it = source->out_edges.begin();
    while (it != source->out_edges.end()) {
        Edge* edge = *it;
        max_flow += flow[edge];
        ++it;
    }

    return max_flow;
}
