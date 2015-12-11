#include "mocca/net/WSNetworkService.h"

#include "mocca/net/WSConnectionAcceptor.h"
#include "mocca/net/stream/TCPNetworkService.h"
#include "mocca/testing/LoopbackPhysicalNetworkService.h"

using namespace mocca::net;

template <typename PhysicalServiceType>
std::string WSNetworkService<PhysicalServiceType>::protocolStatic() {
    return "ws." + PhysicalServiceType::protocol();
}

template <typename PhysicalServiceType>
std::string WSNetworkService<PhysicalServiceType>::protocol() const { return protocolStatic(); }

template <typename PhysicalServiceType>
std::unique_ptr<IProtocolConnection> WSNetworkService<PhysicalServiceType>::connect(const std::string& address) {
    throw Error("WebSocket client connections not supported", __FILE__, __LINE__);
}

template <typename PhysicalServiceType>
std::unique_ptr<IProtocolConnectionAcceptor> WSNetworkService<PhysicalServiceType>::bind(const std::string& bindingString) {
    auto physicalConnectionAcceptor = physicalNetworkService_.bind(bindingString);
    if (physicalConnectionAcceptor != nullptr) {
        return std::unique_ptr<IProtocolConnectionAcceptor>(
            new WSConnectionAcceptor<typename PhysicalServiceType::AcceptorType>(std::move(physicalConnectionAcceptor)));
    }
    return nullptr;
}

template <typename PhysicalServiceType>
PhysicalServiceType& WSNetworkService<PhysicalServiceType>::physicalService() { return physicalNetworkService_; }

template class WSNetworkService<TCPNetworkService>;
template class WSNetworkService<LoopbackPhysicalNetworkService>;
