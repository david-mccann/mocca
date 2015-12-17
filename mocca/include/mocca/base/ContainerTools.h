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

template <typename Iter, typename U>
Iter findMemberEqual(Iter it, Iter itEnd, const U IterValueType<Iter>::*Mem, const U& val) {
    for (; it != itEnd; ++it) {
        if (*it.*Mem == val) {
            return it;
        }
    }
    return itEnd;
}

template <typename Iter, typename U>
Iter findMemberEqual(Iter it, Iter itEnd, U (IterValueType<Iter>::*MemFn)() const, const U& val) {
    for (; it != itEnd; ++it) {
        if ((*it.*MemFn)() == val) {
            return it;
        }
    }
    return itEnd;
}

template <typename T, typename... Args> std::vector<std::unique_ptr<T>> makeUniquePtrVec(Args&&... args) {
    std::unique_ptr<T> tmp[] = {std::move(args)...};
    return std::vector<std::unique_ptr<T>>{std::make_move_iterator(std::begin(tmp)), std::make_move_iterator(std::end(tmp))};
}
}