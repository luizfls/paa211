#include "loader/loader.h"

#include <algorithm>
#include <cstddef>
#include <functional>
#include <iostream>
#include <iterator>
#include <limits>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

using Graph = std::unordered_map<int, std::unordered_map<int, int>>;

void sift_down(std::vector<std::pair<int, int>>& h,
               std::unordered_map<int, std::size_t>& m, std::size_t i)
{
    auto lc = 2 * i + 1; // lc: left child
    auto rc = 2 * i + 2; // rc: right child
    auto smallest = i;

    while(lc < h.size())
    {
        if(h[lc].first < h[smallest].first)
            smallest = lc;
        if(rc < h.size() && h[rc].first < h[smallest].first)
            smallest = rc;

        if(smallest != i)
        {
            std::swap(h[i], h[smallest]);
            m[h[i].second] = i;
            m[h[smallest].second] = smallest;
        }
        else
            break;

        i = smallest;
        lc = 2 * i + 1;
        rc = 2 * i + 2;
    }
}

std::pair<int, int> extract_min(std::vector<std::pair<int, int>>& h,
                                std::unordered_map<int, std::size_t>& m)
{
    auto [dmin, vmin] = h[0];
    m.erase(vmin);
    h.front() = h.back();
    h.pop_back();
    if(!h.empty())
        m[h[0].second] = 0;
    sift_down(h, m, 0);
    return {dmin, vmin};
}

void decrease_key(std::vector<std::pair<int, int>>& h,
                  std::unordered_map<int, std::size_t>& m, std::size_t i, int k)
{
    h[i].first = k;
    while(i > 0)
    {
        std::size_t p = (i + 1) / 2 - 1;
        if(h[p].first > h[i].first)
        {
            std::swap(h[i], h[p]);
            m[h[i].second] = i;
            m[h[p].second] = p;
        }
        else
            break;

        i = p;
    }
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
    std::vector<std::pair<int, int>> h(d.size());
    for(std::size_t i = 0; i < d.size(); ++i)
        h[i] = {d[i], i};
    std::make_heap(h.begin(), h.end(), std::greater<>{});
    std::unordered_map<int, std::size_t> m;
    for(std::size_t i = 0; i < h.size(); ++i)
        m.emplace(h[i].second, i);

    // Passo 1: Iteração
    while(!S_.empty())
    {
        // 1.1
        auto [_, v] = extract_min(h, m);

        // 1.2
        S.insert(v);
        S_.erase(v);

        // 1.3
        for(auto [w, lvw] : g.at(v))
        {
            if(d[v] < d[w] - lvw)
            {
                decrease_key(h, m, m[w], d[v] + lvw);
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
