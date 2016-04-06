/****************************************************************
* Copyright (C) 2016 Andrey Krekhov, David McCann
*
* The content of this file may not be copied and/or distributed
* without the expressed permission of the copyright owner.
*
****************************************************************/

#include "mocca/net/message/FramingConnection.h"

using namespace mocca;
using namespace mocca::net;

FramingConnection::FramingConnection(std::unique_ptr<IStreamConnection> stream, std::unique_ptr<FramingStrategy> framingStrategy)
    : stream_(std::move(stream))
    , framingStrategy_(std::move(framingStrategy)) {
    auto localEndpont = stream_->connectionID()->localEndpoint;
    auto peerEndpoint = stream_->connectionID()->peerEndpoint;

    localEndpont.protocol = localEndpont.protocol + "." + framingStrategy_->name();
    peerEndpoint.protocol = peerEndpoint.protocol + "." + framingStrategy_->name();

    connectionID_ = std::make_shared<ConnectionID>(localEndpont, peerEndpoint);
}

std::shared_ptr<const ConnectionID> FramingConnection::connectionID() const {
    return stream_->connectionID();
}

bool FramingConnection::isConnected() const {
    return stream_->isConnected();
}

void FramingConnection::send(Message message) const {
    framingStrategy_->writeMessageToStream(*stream_, std::move(message));
}

Message FramingConnection::receive(std::chrono::milliseconds timeout) const {
    return framingStrategy_->readMessageFromStream(*stream_, timeout);
}