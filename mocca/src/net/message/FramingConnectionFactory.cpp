#include "mocca/net/message/FramingConnectionFactory.h"

#include "mocca/net/message/FramingConnection.h"
#include "mocca/net/message/FramingConnectionAcceptor.h"

using namespace mocca::net;

FramingConnectionFactory::FramingConnectionFactory(std::unique_ptr<IStreamConnectionFactory> streamConnectionFactory,
                                                   std::unique_ptr<FramingStrategy> framingStrategy)
    : streamConnectionFactory_(std::move(streamConnectionFactory))
    , framingStrategy_(std::move(framingStrategy)) {}

std::unique_ptr<IMessageConnection> FramingConnectionFactory::connect(const std::string& address) {
    return std::unique_ptr<IMessageConnection>(
        new FramingConnection(streamConnectionFactory_->connect(address), framingStrategy_->clone()));
}

std::unique_ptr<IMessageConnectionAcceptor> FramingConnectionFactory::bind(const std::string& address) {
    return std::unique_ptr<IMessageConnectionAcceptor>(
        new FramingConnectionAcceptor(streamConnectionFactory_->bind(address), framingStrategy_->clone()));
}

IStreamConnectionFactory& mocca::net::FramingConnectionFactory::streamConnectionFactory() {
    return *streamConnectionFactory_;
}