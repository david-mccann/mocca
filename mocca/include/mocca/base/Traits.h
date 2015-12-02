#pragma once

#include <string>
#include <type_traits>

namespace mocca {
template <typename T> struct isString : std::false_type {};
template <> struct isString<std::string> : std::true_type {};
template <> struct isString<const char*> : std::true_type {};
template <std::size_t N> struct isString<char[N]> : std::true_type {};
}