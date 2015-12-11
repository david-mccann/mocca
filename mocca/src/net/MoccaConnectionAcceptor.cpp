//#include "mocca/net/MoccaConnection.h"
//#include "mocca/net/MoccaConnectionAcceptor.h"
//#include "mocca/net/stream/TCPStreamAcceptor.h"
//#include "mocca/testing/LoopbackPhysicalConnectionAcceptor.h"
//
//using namespace mocca::net;
//
//template <typename PhysicalConnectionAcceptorType>
//MoccaConnectionAcceptor<PhysicalConnectionAcceptorType>::MoccaConnectionAcceptor(
//    std::unique_ptr<PhysicalConnectionAcceptorType> physicalConnectionAcceptor)
//    : physicalConnectionAcceptor_(std::move(physicalConnectionAcceptor)) {}
//
//template <typename PhysicalConnectionAcceptorType>
//std::unique_ptr<IProtocolConnection>
//MoccaConnectionAcceptor<PhysicalConnectionAcceptorType>::accept(std::chrono::milliseconds timeout) {
//    auto physicalConnection = physicalConnectionAcceptor_->accept(timeout);
//    if (physicalConnection != nullptr) {
//        using ConnectionType = typename PhysicalConnectionAcceptorType::ConnectionType;
//        return std::unique_ptr<IProtocolConnection>(new MoccaConnection<ConnectionType>(std::move(physicalConnection)));
//    }
//    return nullptr;
//}
//
//
//template class MoccaConnectionAcceptor<TCPConnectionAcceptor>;
//template class MoccaConnectionAcceptor<LoopbackPhysicalConnectionAcceptor>;