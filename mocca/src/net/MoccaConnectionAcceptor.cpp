#include "mocca/net/MoccaConnection.h"
#include "mocca/net/MoccaConnectionAcceptor.h"
#include "mocca/net/TCPConnectionAcceptor.h"
#include "mocca/testing/LoopbackPhysicalConnectionAcceptor.h"

using namespace mocca::net;

template <typename PhysicalConnectionAcceptorType>
MoccaConnectionAcceptor<PhysicalConnectionAcceptorType>::MoccaConnectionAcceptor(
    std::unique_ptr<PhysicalConnectionAcceptorType> physicalConnectionAcceptor)
    : physicalConnectionAcceptor_(std::move(physicalConnectionAcceptor)) {}

template <typename PhysicalConnectionAcceptorType>
std::unique_ptr<IProtocolConnection>
MoccaConnectionAcceptor<PhysicalConnectionAcceptorType>::getConnection(std::chrono::milliseconds timeout) {
    auto physicalConnection = physicalConnectionAcceptor_->getConnection(timeout);
    if (physicalConnection != nullptr) {
        using ConnectionType = typename PhysicalConnectionAcceptorType::ConnectionType;
        return std::unique_ptr<IProtocolConnection>(new MoccaConnection<ConnectionType>(std::move(physicalConnection)));
    }
    return nullptr;
}


template class MoccaConnectionAcceptor<TCPConnectionAcceptor>;
template class MoccaConnectionAcceptor<LoopbackPhysicalConnectionAcceptor>;