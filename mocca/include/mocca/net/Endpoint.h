#pragma once

#include <string>

namespace mocca {
namespace net {

class Endpoint {
public:
    Endpoint(const std::string& protocol, const std::string& address);

    bool equals(const Endpoint& other) const;
    friend bool operator==(const Endpoint& lhs, const Endpoint& rhs) { return lhs.equals(rhs); }

    std::string toString() const;
    friend std::ostream& operator<<(std::ostream& os, const Endpoint& obj) { return os << obj.toString(); }

    std::string protocol() const;
    std::string address() const;

private:
    std::string protocol_;
    std::string address_;
};
}
}
