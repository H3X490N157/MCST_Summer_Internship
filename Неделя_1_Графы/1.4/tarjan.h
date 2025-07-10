#include <stack>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <functional>
#pragma once

std::vector<std::vector<Node*>> Graph::Tarjan(const std::string& start_id) {//не совсем понятно, зачем нам выбор вершины (если по определению, вершина может принадлежать лишь одной КСС); в данной функции буду выводить компоненты сильной связности, достижимые из поданной вершины
    std::vector<std::vector<Node*>> components;
    if (node_map.find(start_id) == node_map.end()) {
        std::cout << "Unknown node " << start_id << std::endl;
        return components;
    }

    std::unordered_map<Node*, int> index;
    std::unordered_map<Node*, int> lowlink; //индекс наименьшей вершины, достижимой при текущем индексе
    std::unordered_map<Node*, bool> on_stack; //есть ли вершина в стеке
    std::stack<Node*> node_stack; //сам стек

    int current_index = 0;

    std::function<void(Node*)> strong_connection = [&](Node* v) {
        index[v] = current_index;
        lowlink[v] = current_index;
        current_index++;
        node_stack.push(v);
        on_stack[v] = true;

        std::vector<Edge*>::iterator it = v->out_edges.begin();
        while (it != v->out_edges.end()) {
            Edge* e = *it;
            Node* w = e->to;
            if (index.find(w) == index.end()) {
                strong_connection(w);
                lowlink[v] = std::min(lowlink[v], lowlink[w]);
            } else if (on_stack[w]) {
                lowlink[v] = std::min(lowlink[v], index[w]);
            }
            ++it;
        }

        if (lowlink[v] == index[v]) {
            std::vector<Node*> component;
            Node* w;
            do {
                w = node_stack.top();
                node_stack.pop();
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
            strong_connection(node);
            std::vector<Edge*>::iterator it = node->out_edges.begin();
            while (it != node->out_edges.end()) {
                Edge* edge = *it;
                dfs_all(edge->to);
                ++it;
            }
        }
    };

    dfs_all(node_map[start_id]);

    return components;
}


