#include "mocca/net/MoccaConnection.h"
#include "mocca/net/MoccaConnectionAcceptor.h"
#include "mocca/net/MoccaNetworkService.h"

using namespace mocca::net;

MoccaNetworkService::MoccaNetworkService(std::unique_ptr<IPhysicalNetworkService> physicalNetworkService)
    : physicalNetworkService_(std::move(physicalNetworkService)) {}

std::string MoccaNetworkService::transport() const {
    return physicalNetworkService_->transport();
}

std::string mocca::net::MoccaNetworkService::protocolStatic() {
    return "mocca";
}

std::string mocca::net::MoccaNetworkService::protocol() const {
    return protocolStatic();
}

std::unique_ptr<IProtocolConnection> MoccaNetworkService::connect(const std::string& connectionString) {
    auto physicalConnection = physicalNetworkService_->connect(connectionString);
    if (physicalConnection != nullptr) {
        return std::unique_ptr<IProtocolConnection>(new MoccaConnection(std::move(physicalConnection)));
    }
    return nullptr;
}

std::unique_ptr<IProtocolConnectionAcceptor> MoccaNetworkService::bind(const std::string& bindingString) {
    auto physicalConnectionAcceptor = physicalNetworkService_->bind(bindingString);
    if (physicalConnectionAcceptor != nullptr) {
        return std::unique_ptr<IProtocolConnectionAcceptor>(new MoccaConnectionAcceptor(std::move(physicalConnectionAcceptor)));
    }
    return nullptr;
}
