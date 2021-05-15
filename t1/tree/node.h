#pragma once

#include <cstddef>
#include <ostream>

namespace tree
{

template <typename Key>
struct Node
{
private:
    struct Data
    {
        Key key;
        std::size_t count;
    };

public:
    Node(Key k)
        : data({k, 1})
        , left(nullptr)
        , right(nullptr)
        , height(0)
        , bf(0)
    {}

    Data data;
    Node* left;
    Node* right;
    int height, bf;

    friend std::ostream& operator<<(std::ostream& os, const Node& node)
    {
        os << "'{key=" << node.data.key << ", "
           << "count=" << node.data.count << ", "
           << "height=" << node.height << ", "
           << "bf=" << node.bf << "}'";
        return os;
    }
};

}
