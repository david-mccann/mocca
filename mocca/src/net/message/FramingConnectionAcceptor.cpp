#include "mocca/net/message/FramingConnectionAcceptor.h"

#include "mocca/net/message/FramingConnection.h"

using namespace mocca::net;

FramingConnectionAcceptor::FramingConnectionAcceptor(std::unique_ptr<IStreamConnectionAcceptor> streamAcceptor,
                                                     std::unique_ptr<FramingStrategy> framingStrategy)
    : streamAcceptor_(std::move(streamAcceptor))
    , framingStrategy_(std::move(framingStrategy)) {}

std::string mocca::net::FramingConnectionAcceptor::protocol() const {
    return streamAcceptor_->protocol() + "." + framingStrategy_->name();
}

Endpoint mocca::net::FramingConnectionAcceptor::connectionEndpoint() const {
    return Endpoint(protocol(), streamAcceptor_->connectionEndpoint().address());
}

std::unique_ptr<IMessageConnection> FramingConnectionAcceptor::accept(std::chrono::milliseconds timeout) {
    auto stream = streamAcceptor_->accept(timeout);
    if (stream != nullptr) {
        auto strategyCopy = framingStrategy_->clone();
        strategyCopy->performHandshake(*stream, timeout);
        return std::unique_ptr<IMessageConnection>(new FramingConnection(std::move(stream), std::move(strategyCopy)));
    }
    return nullptr;
}