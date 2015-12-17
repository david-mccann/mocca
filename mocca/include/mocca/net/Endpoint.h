#pragma once

#include <string>

namespace mocca {
namespace net {

class Endpoint {
public:
    Endpoint(const std::string& protocol, const std::string& address);
    Endpoint(const std::string& str);

    bool equals(const Endpoint& other) const;
    friend bool operator==(const Endpoint& lhs, const Endpoint& rhs);

    std::string toString() const;
    friend std::ostream& operator<<(std::ostream& os, const Endpoint& obj);

    std::string protocol() const;
    std::string address() const;

private:
    std::string protocol_;
    std::string address_;
};
}
}
