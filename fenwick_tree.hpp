#ifndef FENWICK_TREE_INCLUDED
#define FENWICK_TREE_INCLUDED

// MIT License
//
// Copyright (c) 2021 Daniel Feist
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <vector>
#include <numeric>
#include <iostream>

template <class T, class Allocator = std::allocator<T>, class = std::enable_if_t<std::is_arithmetic_v<T>>>
class fenwick_tree
{
public:
    typedef T value_type;
    typedef typename std::vector<value_type>::size_type size_type;
    typedef typename std::vector<value_type>::difference_type signed_type;
    typedef typename std::allocator_traits<Allocator>::allocator_type allocator_type;

    fenwick_tree() = default;

    template <class InputIterator>
    explicit fenwick_tree(InputIterator begin, InputIterator end)
    {
        construct(begin, end);
    }

    template <class InputIterator>
    void construct(InputIterator begin, InputIterator end)
    {
        static_assert(std::is_convertible_v<typename std::iterator_traits<InputIterator>::value_type, value_type>, "value type of InputIterator is not implicitly convertible to T.");

        tree.assign(std::distance(begin, end), value_type{0});
        for (auto it{begin}; it != end; ++it)
        {
            for (auto index{std::distance(begin, it)}; index < size(); index = index | (index + 1))
            {
                tree.at(index) += *it;
            }
        }
    }

    value_type sum()
    {
        if (size() == 0)
        {
            return 0;
        }
        return sum(size() - 1);
    }

    value_type sum(signed_type i)
    {
        if (i < 0 || i >= size())
        {
            throw std::out_of_range("sum(0, " + std::to_string(i) + ") exceeds range.");
        }
        value_type result{0};
        if (size() == 0)
        {
            return result;
        }
        for (; i >= 0; i = (i & (i + 1)) - 1)
        {
            result += tree.at(i);
        }
        return result;
    }

    value_type sum(signed_type i, signed_type j)
    {
        if (i > j)
        {
            throw std::out_of_range("sum(" + std::to_string(i) + ", " + std::to_string(j) + ") has a negative range.");
        }
        const auto upper = sum(j);
        --i;
        if (i < 0)
        {
            return upper;
        }
        const auto lower = sum(i);
        return upper - lower;
    }

    bool update(signed_type i, signed_type j, const value_type& value)
    {
        if (i > j)
        {
            throw std::out_of_range("update(" + std::to_string(i) + ", " + std::to_string(j) + ") has a negative range.");
        }
        ++j;
        if (i < 0 || j < 0 || i >= size() || j >= size())
        {
            return false;
        }
        for (; j <= size(); j += j & (-j))
        {
            tree[j] += -value;
        }
        for (; i <= size(); i += i & (-i))
        {
            tree[i] += value;
        }
        return true;
    }

    signed_type size() const noexcept
    {
        return std::distance(tree.begin(), tree.end());
    }

    void clear() noexcept
    {
        return tree.clear();
    }

    value_type operator[](signed_type i) const
    {
        return tree.at(i);
    }

    inline bool operator==(const fenwick_tree& other) const
    {
        return tree == other.tree;
    }

    inline bool operator!=(const fenwick_tree& other) const
    {
        return tree != other.tree;
    }

    friend std::ostream& operator<<(std::ostream& os, const fenwick_tree& fenwick_tree)
    {
        for (const auto& node : fenwick_tree.tree)
        {
            os << node << " ";
        }
        return os;
    }

private:

    std::vector<value_type, allocator_type> tree;
};

#endif // FENWICK_TREE_INCLUDED
