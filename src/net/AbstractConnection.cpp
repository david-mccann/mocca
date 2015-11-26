#include "mocca/net/AbstractConnection.h"

namespace mocca {
namespace net {

AbstractConnection::AbstractConnection()
    : identifier_(createIdentifier())
{}

std::string AbstractConnection::identifier() const
{
    return identifier_;
}

void AbstractConnection::setIdentifier(const std::string& identifier)
{
    identifier_ = identifier;
}

std::string AbstractConnection::createIdentifier() const {
    static int count = 0;
    count++;
    return "connection " + std::to_string(count);
}
}
}