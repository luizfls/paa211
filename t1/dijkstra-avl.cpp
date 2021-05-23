#include "tree/avl.h"

#include <cstddef>
#include <iostream>
#include <iterator>
#include <limits>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

using Graph = std::unordered_map<int, std::unordered_map<int, int>>;

template <typename Tree>
void decrease_key(Tree& t, std::pair<int, int> old_key,
                           std::pair<int, int> new_key)
{
    std::cout << "decreasing from (" << old_key.first << ", "
              << old_key.second << ") to (" << new_key.first
              << ", " << new_key.second << ")\n";
    t.remove(old_key);
    t.insert(new_key);
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
    tree::avl<std::pair<int, int>> t;
    for(std::size_t i = 0; i < d.size(); ++i)
        t.insert({d[i], i});

    // Passo 1: Iteração
    while(!S_.empty())
    {
        // 1.1
        auto [_, v] = t.extract_min();
        std::cout << "selected min: " << _ << " " << v << "\n";

        // 1.2
        S.insert(v);
        S_.erase(v);

        // 1.3
        for(auto [w, lvw] : g.at(v))
        {
            if(d[v] < d[w] - lvw)
            {
                decrease_key(t, {d[w], w}, {d[v] + lvw, w});
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