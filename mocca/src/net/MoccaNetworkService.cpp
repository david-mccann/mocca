//#include "mocca/net/MoccaNetworkService.h"
//
//#include "mocca/net/stream/TCPNetworkService.h"
//#include "mocca/testing/LoopbackPhysicalNetworkService.h"
//
//using namespace mocca::net;
//
//template <typename PhysicalServiceType> std::string MoccaNetworkService<PhysicalServiceType>::protocol() const {
//    return "mocca." + PhysicalServiceType::protocol();
//}
//
//template <typename PhysicalServiceType>
//std::unique_ptr<IProtocolConnection> MoccaNetworkService<PhysicalServiceType>::connect(const std::string& address) {
//    auto physicalConnection = physicalNetworkService_.connect(address);
//    if (physicalConnection != nullptr) {
//        return std::unique_ptr<IProtocolConnection>(
//            new MoccaConnection<typename PhysicalServiceType::ConnectionType>(std::move(physicalConnection)));
//    }
//    return nullptr;
//}
//
//template <typename PhysicalServiceType>
//std::unique_ptr<IProtocolConnectionAcceptor> MoccaNetworkService<PhysicalServiceType>::bind(const std::string& bindingString) {
//    auto physicalConnectionAcceptor = physicalNetworkService_.bind(bindingString);
//    if (physicalConnectionAcceptor != nullptr) {
//        return std::unique_ptr<IProtocolConnectionAcceptor>(
//            new MoccaConnectionAcceptor<typename PhysicalServiceType::AcceptorType>(std::move(physicalConnectionAcceptor)));
//    }
//    return nullptr;
//}
//
//template class MoccaNetworkService<TCPNetworkService>;
//template class MoccaNetworkService<LoopbackPhysicalNetworkService>;