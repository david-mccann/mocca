/****************************************************************
* Copyright (C) 2016 Andrey Krekhov, David McCann
*
* The content of this file may not be copied and/or distributed
* without the expressed permission of the copyright owner.
*
****************************************************************/

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