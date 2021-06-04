#include "loader/loader.h"

#include <algorithm>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <limits>
#include <list>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

using Graph = std::unordered_map<int, std::unordered_map<int, int>>;
using Buckets = std::vector<std::list<int>>;

int extract_min(Buckets::iterator& it)
{
    while(it->empty()) ++it;

    auto ans = it->front();
    it->pop_front();
    return ans;
}

void decrease_key(Buckets& buckets, const std::vector<int>& d, int v, int k)
{
    std::cout << "decreasing from (" << d[v] << ", " << v
              << ") to (" << k << ", " << v << ")\n";
    auto it = std::find(buckets[d[v]].begin(), buckets[d[v]].end(), v);
    buckets[k].splice(buckets[k].end(), buckets[d[v]], it);
}

int highest_cost(const Graph& g)
{
    int hc = std::numeric_limits<int>::min();
    for(const auto& [_, edges] : g)
    {
        auto maxIt = std::max_element(edges.begin(), edges.end(),
                                      [](const std::pair<int, int>& a, const std::pair<int, int>& b)
                                      { return a.second < b.second; });

        if(maxIt != edges.end())
            hc = std::max(hc, maxIt->second);
    }
    return hc;
}

std::pair<std::vector<int>, std::vector<int>> dijkstra(const Graph& g, int s)
{
    auto nBuckets = (g.size() - 1) * highest_cost(g) + 2;

    // Passo 0: Inicialização
    std::unordered_set<int> S, S_;
    for(auto [v, _] : g)
        S_.insert(v);
    std::vector<int> d(g.size(), nBuckets - 1);
    d[s] = 0;
    std::vector<int> pred(g.size(), -1);

    // Estruturas específicas da solução
    Buckets buckets(nBuckets);
    for(std::size_t i = 0; i < d.size(); ++i)
    {
        auto& bucket = i == s
                       ? buckets.front()
                       : buckets.back();
        bucket.emplace_back(i);
    }
    auto bucketIt = buckets.begin();

    // Passo 1: Iteração
    while(!S_.empty())
    {
        // 1.1
        auto v = extract_min(bucketIt);
        std::cout << "selected min: " << d[v] << " " << v << "\n";

        // 1.2
        S.insert(v);
        S_.erase(v);

        // 1.3
        for(auto [w, lvw] : g.at(v))
        {
            if(d[v] < d[w] - lvw)
            {
                decrease_key(buckets, d, w, d[v] + lvw);
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
