#include "FibonacciHeap/FibonacciHeap.h"

#include <algorithm>
#include <cstddef>
#include <iostream>
#include <limits>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

using Graph = std::unordered_map<int, std::unordered_map<int, int>>;
using FibHeap = FibonacciHeap<std::pair<int, int>>;

void decrease_key(FibHeap& fh, FibHeap::NodePtr node, int k)
{
    std::cout << "decreasing from (" << node->data.first << ", " << node->data.second
              << ") to (" << k << ", " << node->data.second << ")\n";
    fh.decrease_key(node, k);
}

std::pair<std::vector<int>, std::vector<int>> dijkstra(const Graph& g, int s)
{
    // Passo 0: Inicialização
    std::unordered_set<int> S, S_;
    for(auto [v, _] : g)
        S_.insert(v);
    std::vector<int> d(g.size(), std::numeric_limits<int>::max());
    d[s] = 0;
    std::vector<int> pred(g.size(), -1);

    // Estruturas específicas da solução
    FibHeap fh;
    std::unordered_map<int, FibHeap::NodePtr> m;
    for(std::size_t i = 0; i < d.size(); ++i)
    {
        auto it = fh.insert({d[i], i});
        m.emplace(i, it);
    }

    // Passo 1: Iteração
    while(!S_.empty())
    {
        // 1.1
        auto [_, v] = fh.extract_min();
        std::cout << "selected min: " << d[v] << " " << v << "\n";

        // 1.2
        S.insert(v);
        S_.erase(v);

        // 1.3
        for(auto [w, lvw] : g.at(v))
        {
            if(d[v] < d[w] - lvw)
            {
                decrease_key(fh, m[w], d[v] + lvw);
                d[w] = d[v] + lvw;
                pred[w] = v;
            }
        }
    }

    return {d, pred};
}

int main(int argc, char* argv[])
{
    Graph g({
        {0, {{1, 1}, {2, 3}, {5, 3}}},
        {1, {{0, 1}, {2, 5}, {3, 1}}},
        {2, {{0, 3}, {1, 5}, {3, 2}, {4, 1}}},
        {3, {{1, 1}, {2, 2}, {4, 4}}},
        {4, {{2, 1}, {3, 4}, {5, 5}}},
        {5, {{0, 3}, {4, 5}}}
    });

    auto [d, pred] = dijkstra(g, 3);

    auto print_vector = [](const auto& v)
    {
        std::cout << "[";
        if(!v.empty())
        {
            std::cout << v.front();
            for(auto it = std::next(v.cbegin()); it != v.cend(); ++it)
            {
                std::cout << ", ";
                std::cout << *it;
            }
        }
        std::cout << "]";
    };

    std::cout << "(";
    print_vector(d);
    std::cout << ", ";
    print_vector(pred);
    std::cout << ")\n";

    return 0;
}
