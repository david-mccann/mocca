#pragma once

#include <memory>

template <typename Derived>
class CloneableMixin {
public:
    std::unique_ptr<Derived> clone() const { return std::unique_ptr<Derived>(new Derived(*this)); }
};