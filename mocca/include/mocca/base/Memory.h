#pragma once

#include <memory>
#include <type_traits>
#include <utility>


namespace mocca {
namespace impl {
template <typename T, typename... Args> std::unique_ptr<T> makeUniqueHelper(std::false_type, Args&&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

template <typename T, typename... Args> std::unique_ptr<T> makeUniqueHelper(std::true_type, Args&&... args) {
    static_assert(std::extent<T>::value == 0, "make_unique<T[N]>() is forbidden, please use make_unique<T[]>().");

    typedef typename std::remove_extent<T>::type U;
    return std::unique_ptr<T>(new U[sizeof...(Args)]{std::forward<Args>(args)...});
}
}

// remove and replace by std::make_unique when all supported compiler versions provide it
template <typename T, typename... Args> std::unique_ptr<T> makeUnique(Args&&... args) {
    return impl::makeUniqueHelper<T>(std::is_array<T>(), std::forward<Args>(args)...);
}
}