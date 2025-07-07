#include <stack>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <functional>
#pragma once

void Graph::Tarjan(const std::string& start_id) {
    if (node_map.find(start_id) == node_map.end()) {
        std::cout << "Unknown node " << start_id << std::endl;
        return;
    }

    std::unordered_map<Node*, int> index;
    std::unordered_map<Node*, int> lowlink;
    std::unordered_map<Node*, bool> on_stack;
    std::stack<Node*> S;
    std::vector<std::vector<Node*>> components;

    int current_index = 0;

    std::function<void(Node*)> strongconnect = [&](Node* v) {
        index[v] = current_index;
        lowlink[v] = current_index;
        current_index++;
        S.push(v);
        on_stack[v] = true;

        for (Edge* e : v->out_edges) {
            Node* w = e->to;
            if (index.find(w) == index.end()) {
                strongconnect(w);
                lowlink[v] = std::min(lowlink[v], lowlink[w]);
            } else if (on_stack[w]) {
                lowlink[v] = std::min(lowlink[v], index[w]);
            }
        }

        if (lowlink[v] == index[v]) {
            std::vector<Node*> component;
            Node* w;
            do {
                w = S.top();
                S.pop();
                on_stack[w] = false;
                component.push_back(w);
            } while (w != v);
            if (component.size() > 1) {
                components.push_back(component);
            }
        }
    };

    std::unordered_set<Node*> visited;
    std::function<void(Node*)> dfs_all = [&](Node* node) {
        if (!visited.count(node)) {
            visited.insert(node);
            strongconnect(node);
            for (Edge* edge : node->out_edges) {
                dfs_all(edge->to);
            }
        }
    };

    dfs_all(node_map[start_id]);

    for (const auto& comp : components) {
        for (size_t i = 0; i < comp.size(); ++i) {
            std::cout << comp[i]->id;
            if (i + 1 < comp.size()) std::cout << " ";
        }
        std::cout << std::endl;
    }
}

