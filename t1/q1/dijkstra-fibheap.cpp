#include "loader/loader.h"
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
    Graph g{loader::loadGraphFile(argv[1])};

    auto [d, pred] = dijkstra(g, std::stoi(argv[2]));

    auto print_vector = [](const auto& v)
    {
        std::cout << "[";
        if(!v.empty())
        {
            if(v.front() == std::numeric_limits<int>::max())
                std::cout << "\u221E";
            else
                std::cout << v.front();
            for(auto it = std::next(v.cbegin()); it != v.cend(); ++it)
            {
                std::cout << ", ";
                if(*it == std::numeric_limits<int>::max())
                    std::cout << "\u221E";
                else
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
