#pragma once

#include "mocca/net/IMessageConnectionFactory.h"
#include "mocca/net/IStreamConnectionFactory.h"
#include "mocca/net/framing/FramingStrategy.h"

namespace mocca {
namespace net {

class FramingObjectFactory : public IMessageConnectionFactory {
public:
    FramingObjectFactory(std::unique_ptr<IStreamConnectionFactory> streamConnectionFactory,
                         std::unique_ptr<FramingStrategy> framingStrategy);

    std::unique_ptr<IMessageConnection> connect(const std::string& address);
    std::unique_ptr<IMessageConnectionAcceptor> bind(const std::string& address);

private:
    std::unique_ptr<IStreamConnectionFactory> streamConnectionFactory_;
    std::unique_ptr<FramingStrategy> framingStrategy_;
};
}
}
