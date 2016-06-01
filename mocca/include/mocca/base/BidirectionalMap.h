/*The MIT License(MIT)

Copyright(c) 2016 David McCann

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files(the
"Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and / or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to
the following conditions :

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.*/

#pragma once

#include "Error.h"

#include <algorithm>
#include <vector>

namespace mocca {

template <typename U, typename V> class BidirectionalMap {
public:
    void insert(const U& first, const V& second) {
        auto it = std::find(begin(first_), end(first_), first);
        if (it == end(first_)) {
            first_.push_back(first);
            second_.push_back(second);
        } else {
            auto index = std::distance(begin(first_), it);
            second_[index] = second;
        }
    }

    V& operator[](const U& first) {
        auto it = std::find(begin(first_), end(first_), first);
        if (it == end(first_)) {
            first_.push_back(first);
            second_.push_back(V());
            return second_.back();
        }
        auto index = std::distance(begin(first_), it);
        return second_[index];
    }

    V getByFirst(const U& first) const {
        auto it = std::find(begin(first_), end(first_), first);
        if (it == end(first_)) {
            throw Error("Map does not contain the specified entry in the first column", __FILE__, __LINE__);
        }
        auto index = std::distance(begin(first_), it);
        return second_[index];
    }

    U getBySecond(const V& second) const {
        auto it = std::find(begin(second_), end(second_), second);
        if (it == end(second_)) {
            throw Error("Map does not contain the specified entry in the second column", __FILE__, __LINE__);
        }
        auto index = std::distance(begin(second_), it);
        return first_[index];
    }

    size_t size() const {
        assert(first_.size() == first_.size());
        return first_.size();
    }

    bool empty() const { return first_.empty(); }

private:
    std::vector<U> first_;
    std::vector<V> second_;
};
}