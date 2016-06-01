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