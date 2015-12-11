#pragma once

#include "mocca/net/message/IMessageConnectionAcceptor.h"

namespace mocca {
namespace net {

template <typename ProtocolType, typename StreamAcceptorType> class FramingConnectionAcceptor : public IMessageConnectionAcceptor {
public:
    FramingConnectionAcceptor(std::unique_ptr<StreamAcceptorType> streamAcceptor)
        : streamAcceptor_(std::move(streamAcceptor)) {}

    std::unique_ptr<IMessageConnection> accept(std::chrono::milliseconds timeout);

private:
    std::unique_ptr<StreamAcceptorType> streamAcceptor_;
};
}
}
