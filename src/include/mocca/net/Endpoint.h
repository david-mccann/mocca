#pragma once

#include "mocca/base/Containers.h"

namespace mocca {
namespace net {

class Endpoint {
public:
    Endpoint(const std::string& transport, const std::string& connectionString);

    bool equals(const Endpoint& other) const;
    friend bool operator==(const Endpoint& lhs, const Endpoint& rhs) { return lhs.equals(rhs); }
    std::string toString() const;
    friend std::ostream& operator<<(std::ostream& os, const Endpoint& obj) { return os << obj.toString(); }

    std::string transport() const;
    std::string connectionString() const;

private:
    std::string transport_;
    std::string connectionString_;
};
}
}
