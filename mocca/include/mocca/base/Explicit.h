#pragma once

namespace mocca {

template <typename T, typename Phantom> struct Explicit {
    Explicit() {}
    explicit Explicit(const T t)
        : value(t) {}
    Explicit(const Explicit& other)
        : value(other.value) {}

    Explicit& operator=(const Explicit& other) {
        value = other.value;
        return *this;
    }
    Explicit& operator=(const T& t) {
        value = t;
        return *this;
    }

    operator const T&() const { return value; }
    operator T&() { return value; }

    bool operator==(const Explicit& other) const { return value == other.value; }
    bool operator!=(const Explicit& other) const { return value != other.value; }
    bool operator<(const Explicit& other) const { return value < other.value; }

    T value;
};

template <typename Phantom> bool operator==(const std::string& lhs, const Explicit<std::string, Phantom>& rhs) {
    return lhs == rhs.value;
}

template <typename Phantom> bool operator==(const Explicit<std::string, Phantom>& lhs, const std::string& rhs) {
    return lhs.value == rhs;
}

template <typename Phantom> std::ostream& operator<<(std::ostream& os, const Explicit<std::string, Phantom>& obj) {
    return os << obj.value;
}
}

namespace std {
template <typename T, typename U> struct hash<typename mocca::Explicit<T, U>> {
    size_t operator()(const mocca::Explicit<T, U>& x) const { return hash<std::string>()(x.value); }
};
}