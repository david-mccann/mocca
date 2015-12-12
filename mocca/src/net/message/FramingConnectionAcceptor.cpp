#include "mocca/net/message/FramingConnectionAcceptor.h"

#include "mocca/net/message/FramingConnection.h"

using namespace mocca::net;

FramingConnectionAcceptor::FramingConnectionAcceptor(std::unique_ptr<IStreamConnectionAcceptor> streamAcceptor,
                                                     std::unique_ptr<FramingStrategy> framingStrategy)
    : streamAcceptor_(std::move(streamAcceptor))
    , framingStrategy_(std::move(framingStrategy)) {}

std::unique_ptr<IMessageConnection> FramingConnectionAcceptor::accept(std::chrono::milliseconds timeout) {
    auto stream = streamAcceptor_->accept(timeout);
    if (stream != nullptr) {
        framingStrategy_->performHandshake(*stream, timeout);
        return std::unique_ptr<IMessageConnection>(new FramingConnection(std::move(stream), framingStrategy_->clone()));
    }
    return nullptr;
}