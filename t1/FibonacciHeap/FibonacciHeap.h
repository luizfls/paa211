#pragma once

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <iterator>
#include <list>
#include <optional>
#include <stdexcept>
#include <utility>
#include <vector>

template <typename T>
struct FibonacciHeap
{
    using value_type = T;

private:
    struct Node;
public:
    using NodePtr = typename std::list<Node>::iterator;

    NodePtr insert(T data)
    {
        roots.emplace_back(data);
        if(min == roots.end())
            min = roots.begin();
        else if(data < min->data)
            min = std::prev(roots.end());
        ++size;
        return std::prev(roots.end());
    }

    T extract_min()
    {
        if(min == roots.end())
            throw std::logic_error{"extract_min called on empty heap"};

        for(auto& child : min->children)
        {
            child.parent = {};
            child.marked = false;
        }
        roots.splice(roots.end(), min->children);
        auto ans = min->data;
        roots.erase(min);
        --size;

        if(!roots.empty())
            cleanup();

        return ans;
    }

    void decrease_key(NodePtr node, int k)
    {
        node->data.first = k;
        if(node->parent && node->data < (*node->parent)->data)
        {
            cut(node);

            auto p = node->parent;
            while(p && (*p)->marked)
            {
                auto tmp = (*p)->parent;
                cut(*p);
                p = tmp;
            }
        }

        if(min == roots.end())
            throw std::logic_error{"bad min value"};
        else if(node->data < min->data)
            min = node;
    }

private:
    struct Node
    {
        Node(T pData)
            : data(pData)
            , marked(false)
        {}

        T data;
        std::optional<NodePtr> parent;
        std::list<Node> children;
        bool marked;
    };

    void cleanup()
    {
        std::vector<NodePtr> v(std::log2(size) + 2, roots.end());
        auto it = roots.begin();
        while(it != roots.end())
        {
            auto next = std::next(it);
            auto degree = it->children.size();
            while(v[degree] != roots.end())
            {
                auto min = v[degree];
                auto max = it;
                if(min->data > max->data)
                    std::swap(max, min);

                min->children.splice(min->children.end(), roots, max);
                max->parent = min;
                v[degree] = roots.end();
                it = min;
                degree = it->children.size();
            }
            v[degree] = it;
            it = next;
        }

        min = std::min_element(roots.begin(), roots.end(),
                               [](const auto& a, const auto& b)
                               { return a.data < b.data; });
    }

    void cut(NodePtr node)
    {
        if(!node->parent)
            throw std::logic_error{"cut called on root node"};

        roots.splice(roots.end(), (*node->parent)->children, node);
        node->parent = {};
        node->marked = false;
    }

    std::list<Node> roots;
    NodePtr min = roots.end();
    std::size_t size = 0;
};
