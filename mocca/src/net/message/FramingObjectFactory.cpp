#include "mocca/net/message/FramingObjectFactory.h"

using namespace mocca::net;

template <typename ProtocolType, typename StreamObjectFactoryType>
inline std::unique_ptr<IMessageConnection> FramingObjectFactory<ProtocolType, StreamObjectFactoryType>::connect(const std::string& args) {
    return std::unique_ptr<IMessageConnection>(
        new FramingConnection<ProtocolType, typename StreamObjectFactoryType::Stream_Type>(streamObjectFactory_.createStream(args)));
}

template <typename ProtocolType, typename StreamObjectFactoryType>
std::unique_ptr<IMessageConnectionAcceptor> FramingObjectFactory<ProtocolType, StreamObjectFactoryType>::bind(const std::string& args) {
    return std::unique_ptr<IMessageConnectionAcceptor>(
        new FramingConnectionAcceptor<ProtocolType, typename StreamObjectFactoryType::Acceptor_Type>(
            streamObjectFactory_.createAcceptor(args)));
}

template Prefixed_TCP_Factory;
template Prefixed_MessageQueue_Factory;