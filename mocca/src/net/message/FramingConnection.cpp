#include "mocca/net/message/FramingConnection.h"

using namespace mocca;
using namespace mocca::net;

FramingConnection::FramingConnection(std::unique_ptr<IStreamConnection> stream, std::unique_ptr<FramingStrategy> framingStrategy)
    : stream_(std::move(stream))
    , framingStrategy_(std::move(framingStrategy)) {}

std::shared_ptr<const ConnectionID> FramingConnection::connectionID() const {
    return stream_->connectionID();
}

void FramingConnection::send(ByteArray message, std::chrono::milliseconds timeout) const {
    framingStrategy_->writeFrameToStream(*stream_, std::move(message), timeout);
}

ByteArray FramingConnection::receive(std::chrono::milliseconds timeout) const {
    return framingStrategy_->readFrameFromStream(*stream_, timeout);
}