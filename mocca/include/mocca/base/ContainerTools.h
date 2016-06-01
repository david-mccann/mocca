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

#include <algorithm>
#include <iterator>
#include <memory>
#include <vector>

namespace mocca {

template <typename Iter> using IterValueType = typename std::iterator_traits<Iter>::value_type;

template <typename Iter, typename U> std::vector<U> collectMembers(Iter it, Iter itEnd, const U IterValueType<Iter>::*Mem) {
    std::vector<U> result;
    for (; it != itEnd; ++it) {
        result.push_back(*it.*Mem);
    }
    return result;
}

template <typename Iter, typename U> Iter findMemberEqual(Iter it, Iter itEnd, const U IterValueType<Iter>::*Mem, const U& val) {
    for (; it != itEnd; ++it) {
        if (*it.*Mem == val) {
            return it;
        }
    }
    return itEnd;
}

template <typename Iter, typename U> Iter findMemberEqual(Iter it, Iter itEnd, U (IterValueType<Iter>::*MemFn)() const, const U& val) {
    for (; it != itEnd; ++it) {
        if ((*it.*MemFn)() == val) {
            return it;
        }
    }
    return itEnd;
}

template <typename T> std::vector<std::unique_ptr<T>> makeUniquePtrVec(std::unique_ptr<T> arg) {
    std::vector<std::unique_ptr<T>> result;
    result.push_back(std::move(arg));
    return result;
}

template <typename T, typename... Args> std::vector<std::unique_ptr<T>> makeUniquePtrVec(std::unique_ptr<T> arg, Args&&... args) {
    auto result = makeUniquePtrVec(std::move(arg));
    auto tail = makeUniquePtrVec(std::move(args)...);
    result.insert(result.end(), std::make_move_iterator(tail.begin()), std::make_move_iterator(tail.end()));
    return result;
}

template <typename Base, typename Iter>
std::vector<Base*> transformToBasePtrVec(Iter it, Iter itEnd) {
    std::vector<Base*> result;
    using ValueType = typename std::iterator_traits<Iter>::value_type;
    for (; it != itEnd; ++it) {
        result.push_back(static_cast<Base*>(const_cast<ValueType*>(&*it)));
    }
    return result;
}

}