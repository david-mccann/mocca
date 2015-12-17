#pragma once

#include "Error.h"

#include <algorithm>
#include <cassert>
#include <iterator>
#include <vector>
#include <memory>

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

template <typename Iter> using IterValueType = typename std::iterator_traits<Iter>::value_type;
template <typename Iter, typename U> std::vector<U> collectMembers(Iter it, Iter itEnd, const U IterValueType<Iter>::*Mem) {
    std::vector<U> result;
    for (; it != itEnd; ++it) {
        result.push_back(*it.*Mem);
    }
    return result;
}

template <typename T, typename U>
typename std::vector<T>::const_iterator findMemberEqual(const std::vector<T>& vec, const U T::*Mem, const U& val) {
    for (typename std::vector<T>::const_iterator it = begin(vec); it != end(vec); ++it) {
        if (*it.*Mem == val) {
            return it;
        }
    }
    return end(vec);
}

template <typename T, typename U> typename std::vector<T>::iterator findMemberEqual(std::vector<T>& vec, const U T::*Mem, const U& val) {
    for (typename std::vector<T>::iterator it = begin(vec); it != end(vec); ++it) {
        if (*it.*Mem == val) {
            return it;
        }
    }
    return end(vec);
}

template <typename T, typename U>
typename std::vector<T>::const_iterator findMemberEqual(const std::vector<T>& vec, U (T::*MemFn)() const, const U& val) {
    for (typename std::vector<T>::const_iterator it = begin(vec); it != end(vec); ++it) {
        if ((*it.*MemFn)() == val) {
            return it;
        }
    }
    return end(vec);
}

template <typename T, typename U>
typename std::vector<T>::iterator findMemberEqual(std::vector<T>& vec, U (T::*MemFn)() const, const U& val) {
    for (typename std::vector<T>::iterator it = begin(vec); it != end(vec); ++it) {
        if ((*it.*MemFn)() == val) {
            return it;
        }
    }
    return end(vec);
}

template <typename T, typename... Args>
std::unique_ptr<T> makeUnique(Args&&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

template <typename T, typename... Args>
std::vector<std::unique_ptr<T>> makeUniquePtrVec(Args&&... args) {
    std::unique_ptr<T> tmp[] = { std::move(args)... };
    return std::vector<std::unique_ptr<T>>{ std::make_move_iterator(std::begin(tmp)), std::make_move_iterator(std::end(tmp)) };

}
}