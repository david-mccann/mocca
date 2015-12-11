#pragma once

#include "mocca/net/message/IMessageConnectionAcceptor.h"

namespace mocca {
namespace net {

template <typename FramingProtocolType, typename StreamAcceptorType> class StreamFramingConnectionAcceptor : public IMessageConnectionAcceptor {
public:
    StreamFramingConnectionAcceptor(const std::string& streamAddress)
        : streamAcceptor_(std::unique_ptr<StreamAcceptorType>(new StreamAcceptorType(streamAddress))) {}

    std::unique_ptr<IMessageConnection> accept(std::chrono::milliseconds timeout);

private:
    std::unique_ptr<StreamAcceptorType> streamAcceptor_;
};
}
}
