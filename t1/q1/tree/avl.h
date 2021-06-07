#pragma once

#include "node.h"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <iterator>
#include <queue>
#include <stdexcept>
#include <tuple>
#include <vector>

namespace tree
{

template <typename Key>
class avl
{
    using Node_ = Node<Key>;

public:
    ~avl()
    {
        clear(root);
    }

    const Key* insert(Key k)
    {
        root = insert(k, root);
        ++_size;
        return find(k);
    }

    bool remove(Key k)
    {
        if(contains(k))
        {
            root = remove(k, root);
            --_size;
            return true;
        }
        return false;
    }

    Key extract_min()
    {
        if(empty())
            throw std::logic_error{"extract_min called on empty tree"};

        auto p = root;
        while(p->left != nullptr)
            p = p->left;
        auto ans = p->data.key;

        remove(ans);

        return ans;
    }

    bool contains(Key k) const
    { return find(k) != nullptr; }

    const Key* find(Key k) const
    { return find(k, root); }

    std::size_t size() const
    { return _size; }

    bool empty() const
    { return _size == 0; }

    friend std::ostream& operator<<(std::ostream& os, const avl& tree)
    {
        std::vector<std::vector<Node_*>> levels;
        std::queue<std::tuple<Node_*, std::size_t, std::size_t>> q({{tree.root, 0, 0}});

        while(!q.empty())
        {
            auto [node, level, i] = q.front();
            q.pop();
            if(node != nullptr)
            {
                if(level == levels.size())
                    levels.push_back(std::vector<Node_*>(std::pow(2, level), nullptr));
                levels[level][i] = node;
                q.push({node->left, level + 1, 2 * i});
                q.push({node->right, level + 1, 2 * i + 1});
            }
        }

        std::vector<Node_*> nodes;
        for(const auto& level : levels)
            for(const auto& node : level)
                nodes.push_back(node);

        auto it = std::find_if(nodes.crbegin(), nodes.crend(),
                               [](const auto& node){ return node != nullptr; });
        if(it != nodes.crend())
            nodes.erase(std::prev(std::next(it)).base(), nodes.end());

        os << "[";
        if(!nodes.empty())
        {
            os << *nodes.front();
            for(auto it = std::next(nodes.cbegin()); it != nodes.cend(); ++it)
            {
                os << ", ";
                if(*it != nullptr)
                    os << **it;
                else
                    os << "None";
            }
        }
        os << "]";

        return os;
    }

private:
    void clear(Node_* node)
    {
        if(node != nullptr)
        {
            clear(node->left);
            clear(node->right);
            delete node;
        }
    }

    Node_* insert(Key k, Node_* node)
    {
        if(node == nullptr)
            node = new Node_(k);
        else if(k < node->data.key)
            node->left = insert(k, node->left);
        else if(k > node->data.key)
            node->right = insert(k, node->right);
        else
            node->data.count++;

        update(node);

        node = rebalance(node);

        return node;
    }

    Node_* remove(Key k, Node_* node)
    {
        if(k < node->data.key)
            node->left = remove(k, node->left);
        else if(k > node->data.key)
            node->right = remove(k, node->right);
        else
        {
            --(node->data.count);
            if(node->data.count == 0)
            {
                if(node->left == nullptr && node->right == nullptr)
                {
                    delete node;
                    return nullptr;
                }
                else if(node->left == nullptr)
                {
                    delete node;
                    return node->right;
                }
                else if(node->right == nullptr)
                {
                    delete node;
                    return node->left;
                }
                else
                {
                    ++(node->data.count);
                    Node_* replacement = node->right;
                    if(replacement->left != nullptr)
                    {
                        auto p = &replacement->left;
                        while((*p)->left != nullptr)
                            p = &(*p)->left;
                        replacement = *p;
                        *p = node;
                    }
                    std::swap(node->left, replacement->left);
                    std::swap(node->right, replacement->right);
                    if(replacement->right == replacement)
                        replacement->right = node;
                    std::swap(node->height, replacement->height);
                    std::swap(node->bf, replacement->bf);
                    node = replacement;
                    node->right = remove(k, node->right);
                }
            }
        }

        update(node);

        node = rebalance(node);

        return node;
    }

    const Key* find(Key k, Node_* node) const
    {
        if(node == nullptr)
            return nullptr;
        else if(node->data.key == k)
            return &node->data.key;
        else if(k < node->data.key)
            return find(k, node->left);
        else /* k > node->data.key) */
            return find(k, node->right);
    }

    void update(Node_* node)
    {
        node->height = 1 + std::max(height(node->left), height(node->right));
        node->bf = height(node->right) - height(node->left);
    }

    Node_* rebalance(Node_* node)
    {
        if(node->bf < -1)
        {
            if(bf(node->left) <= 0)
                return rightRotate(node);
            else
            {
                node->left = leftRotate(node->left);
                return rightRotate(node);
            }
        }
        else if(node->bf > 1)
        {
            if(bf(node->right) >= 0)
                return leftRotate(node);
            else
            {
                node->right = rightRotate(node->right);
                return leftRotate(node);
            }
        }

        return node;
    }

    Node_* rightRotate(Node_* node)
    {
        auto newParent = node->left;
        node->left = newParent->right;
        newParent->right = node;

        update(node);
        update(newParent);

        return newParent;
    }

    Node_* leftRotate(Node_* node)
    {
        auto newParent = node->right;
        node->right = newParent->left;
        newParent->left = node;

        update(node);
        update(newParent);

        return newParent;
    }

    int height(Node_* node)
    { return node == nullptr ? -1 : node->height; }

    int bf(Node_* node)
    { return node == nullptr ? 0 : node->bf; }

    Node_* root = nullptr;

    std::size_t _size = 0;
};

}
