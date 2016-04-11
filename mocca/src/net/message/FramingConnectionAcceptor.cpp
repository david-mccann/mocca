/****************************************************************
* Copyright (C) 2016 Andrey Krekhov, David McCann
*
* The content of this file may not be copied and/or distributed
* without the expressed permission of the copyright owner.
*
****************************************************************/

#include "mocca/net/message/FramingConnectionAcceptor.h"

#include "mocca/net/message/FramingConnection.h"

using namespace mocca::net;

FramingConnectionAcceptor::FramingConnectionAcceptor(std::unique_ptr<IStreamConnectionAcceptor> streamAcceptor,
                                                     std::unique_ptr<FramingStrategy> framingStrategy)
    : streamAcceptor_(std::move(streamAcceptor))
    , framingStrategy_(std::move(framingStrategy)) {
    auto endpoint = *streamAcceptor_->localEndpoint();
    endpoint.protocol = endpoint.protocol + "." + framingStrategy_->name();
    endpoint_ = std::make_shared<Endpoint>(endpoint);
}

std::unique_ptr<IMessageConnection> FramingConnectionAcceptor::accept(std::chrono::milliseconds timeout) {
    auto stream = streamAcceptor_->accept(timeout);
    if (stream != nullptr) {
        auto strategyCopy = framingStrategy_->clone();
        strategyCopy->performHandshake(*stream);
        return std::unique_ptr<IMessageConnection>(new FramingConnection(std::move(stream), std::move(strategyCopy)));
    }
    return nullptr;
}

std::shared_ptr<const Endpoint> FramingConnectionAcceptor::localEndpoint() const {
    return endpoint_;
}