#pragma once

#include "mocca/net/IMessageConnectionAcceptor.h"
#include "mocca/net/IStreamConnectionAcceptor.h"
#include "mocca/net/framing/FramingStrategy.h"

namespace mocca {
namespace net {

class FramingConnectionAcceptor : public IMessageConnectionAcceptor {
public:
    FramingConnectionAcceptor(std::unique_ptr<IStreamConnectionAcceptor> streamAcceptor, std::unique_ptr<FramingStrategy> framingStrategy);

    std::unique_ptr<IMessageConnection> accept(std::chrono::milliseconds timeout);

private:
    std::unique_ptr<IStreamConnectionAcceptor> streamAcceptor_;
    std::unique_ptr<FramingStrategy> framingStrategy_;
};
}
}
